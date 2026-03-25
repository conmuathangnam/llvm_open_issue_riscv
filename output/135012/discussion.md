# Emitting attribute diagnostics twice in some template instantiations

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/135012

## Body

Consider the following:
```
template <class T>
void f(T __attribute__((align_value(4))) x) {} // expected-warning 2 {{'align_value' attribute only applies to a pointer or reference ('int' is invalid)}}

void foo() {
  f<int>(0); // expected-note {{while substituting explicitly-specified template arguments into function template 'f'}} \
                expected-note {{while substituting deduced template arguments into function template 'f' [with T = int]}}
}
```
It would be ideal to only diagnose once rather than twice. We get one diagnostic from explicit template arguments and another one for deduced template arguments, which seems silly.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-1

Author: Aaron Ballman (AaronBallman)

<details>
Consider the following:
```
template &lt;class T&gt;
void f(T __attribute__((align_value(4))) x) {} // expected-warning 2 {{'align_value' attribute only applies to a pointer or reference ('int' is invalid)}}

void foo() {
  f&lt;int&gt;(0); // expected-note {{while substituting explicitly-specified template arguments into function template 'f'}} \
                expected-note {{while substituting deduced template arguments into function template 'f' [with T = int]}}
}
```
It would be ideal to only diagnose once rather than twice. We get one diagnostic from explicit template arguments and another one for deduced template arguments, which seems silly.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
Consider the following:
```
template &lt;class T&gt;
void f(T __attribute__((align_value(4))) x) {} // expected-warning 2 {{'align_value' attribute only applies to a pointer or reference ('int' is invalid)}}

void foo() {
  f&lt;int&gt;(0); // expected-note {{while substituting explicitly-specified template arguments into function template 'f'}} \
                expected-note {{while substituting deduced template arguments into function template 'f' [with T = int]}}
}
```
It would be ideal to only diagnose once rather than twice. We get one diagnostic from explicit template arguments and another one for deduced template arguments, which seems silly.
</details>


---

### Comment 3 - erichkeane

I looked into this a bit, and I think there is SOMETHING more insidious template-wise that requires someone to take a look into (that actually COULD be a decent perf improvement?).

Basically, in the example we instantiate the parameter 2x, both originating from `DeduceTemplateArguments`.

The first is the call to `SubstituteExplicitTemplateArguments`.  This results in the first version to `SubstParmVarDecl` that results in the attribute diagnostic.

The second call is from `FinishTemplateArgumentDeduction`, which eventually finds its way back to `SubstParmVarDecl`, but creates the SAME declaration. Unfortunately it seems that the information from substituting the explicit args isn't passed on in any way, so we end up RE-instantiating the same template arguments.

Optimially we should pass the 'already deduced' arguments onto the latter call to `TransformFunctionTypeParams` (and various `SubstFunctionTypeParams` calls).

I'm unfortunately without the time to look closer, but hope someone has time to look at this.  I would hope we could just pass the information down the call stack somehow?  @zyn0217 or @mizvekov : any thoughts/ideas?


---

### Comment 4 - zyn0217

> Optimially we should pass the 'already deduced' arguments onto the latter call to TransformFunctionTypeParams (and various SubstFunctionTypeParams calls).

There is a concern that these instantiated function parameters do have their own implicit instantiated function declaration as their DeclContext, and sharing them across different instantiations might cause us troubles.

I think a less-risky solution would be, we can probably avoid the substitution in `FinishTemplateArgumentDeduction` if all the dependent argumemts are already deduced after `SubstituteExplicitTemplateArguments`, from which we reuse the instantiation.

Edit: Even worse, `SubstituteExplicitTemplateArguments` doesn't seem to create a new function declaration, but repeats what `TransformFunctionProtoType` does. And it would bind transformed function parameters to the TemplateDecl, which is not great.

---

### Comment 5 - erichkeane

> > Optimially we should pass the 'already deduced' arguments onto the latter call to TransformFunctionTypeParams (and various SubstFunctionTypeParams calls).
> 
> There is a concern that these instantiated function parameters do have their own implicit instantiated function declaration as their DeclContext, and sharing them across different instantiations might cause us troubles.
> 
Sorry, I was unclear... We did the explicit instantiation of these for the current function, then they seem to be thrown away?

> I think a less-risky solution would be, we can probably avoid the substitution in `FinishTemplateArgumentDeduction` if all the dependent argumemts are already deduced after `SubstituteExplicitTemplateArguments`, from which we reuse the instantiation.
> 

> Edit: Even worse, `SubstituteExplicitTemplateArguments` doesn't seem to create a new function declaration, but repeats what `TransformFunctionProtoType` does. And it would bind transformed function parameters to the TemplateDecl, which is not great.

Right, i think this is what I spotted.

---

### Comment 6 - zyn0217

Here's an interesting case involving partially substituted packs:

https://compiler-explorer.com/z/9T1nKMneM
```cpp

template <class... T>
void f(__attribute__((align_value(4))) T... x) {

}

void foo() {
  f<float, int>(0., 1, 2, 3, 4, 5, 6);
}

```

We currently have a total of 2 + 7 invalid align_value warnings: the first 2 come from substituting explicit template arguments, and the remaining 7 arise during instantiation.

This suggests that pack expansion is also contributing to the diagnostic noise. In this case, even if instantiation could reuse parts of the explicit argument substitution, it wouldn't save us much. Reusing instantiations doesn't completely resolve the issue - though it is still an optimization.

---

