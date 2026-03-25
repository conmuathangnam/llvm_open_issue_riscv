# [Crash][UNREACHABLE] Didn't find this decl on its identifier's chain!

**Author:** Sameeranjoshi
**URL:** https://github.com/llvm/llvm-project/issues/54279

## Body

While compiling the application dealii[1] with trunk, I am getting an error in the semantic phase of Clang.

TL;DR version
```
Didn't find this decl on its identifier's chain!
UNREACHABLE executed at ${HOME}/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:82!
```
Attaching the bug report, script, and preprocessed sources as 

- [x] [bug_report.txt](https://github.com/llvm/llvm-project/files/8207514/bug_report.txt)
- [ ] Script = Unable to attach script
- [ ] Preprocessed Sources = Unable to attach a `.cpp` file 

Not sure if GitHub is not allowing to upload the files, but if needed I can attach or paste text.
[1] https://github.com/dealii/dealii


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - Sameeranjoshi

Can there be some traction on solving this issue ?
Do I need to assign this to someone ? I am probably missing out some information after the migration from bugzilla.


---

### Comment 3 - steakhal

I'm also interested in a similar crash. For my case, here is a reproducer:
https://godbolt.org/z/x7b93YfG8
```C++
template <typename T> void f(const T &a, int c = 0);
template <> void f(const int &unused, int) {
  f(42);
}
```

My crash seems to be introduced by a5569f089844209dbea2e3241460173d7b6b1420 back in Jul. 9 2020 by Richard Smith (_Push parameters into the local instantiation scope before instantiating a default argument_).

@AaronBallman Do you know anyone who might be interested in this?

---

### Comment 4 - AaronBallman

The issue has to do with the default parameter (removing it or making it mandatory causes the issue to disappear). From my debugging of this, we do:

Relating to the primary template declaration of f:
ActOnTypeParameter calls IdResolver.AddDecl for "T"
ActOnParamDeclarator calls IdResolver.AddDecl for "a"
ActOnParamDeclarator calls IdResolver.AddDecl for "c"
ActOnPopScope (coming from ParseDirectDeclarator) calls IdResolver.RemoveDecl for "a"
ActOnPopScope (coming from ParseDirectDeclarator) calls IdResolver.RemoveDecl for "c"
PushOnScopeChains (coming from HandleDeclarator) calls IdResolver.AddDecl for "f"
ActOnPopScope (coming from MultiParseScope::Exit which itself comes from ParseTemplateDeclarationOrSpecialization) calls IdResolver.RemoveDecl for "T"

Relating to the template specialization of f:
ActOnParamDeclarator calls IdResolver.AddDecl for "unused"
ActOnPopScope (coming from ParseDirectDeclarator) calls IdResolver.RemoveDecl for "unused"
PushOnScopeChains (coming from ActOnStartOfFunctionDecl) calls IdResolver.AddDecl for "unused"
ActOnPopScope (coming from ParseFunctionStatementBody) calls IdResolver.RemoveDecl for "a" <--- the WTF is here

---

### Comment 5 - AaronBallman

FWIW, this looks suspicious to me: https://github.com/llvm/llvm-project/blob/main/clang/lib/Sema/SemaDecl.cpp#L14488

Every other call to IdResolver.AddDecl is paired with a call to `AddDecl` on the scope object as well, except for this case. However, when I pair them up together, the assertion moves around rather than goes away. It may be an orthogonal issue, or it may be related.

Either way, it seems like we're somehow getting the declaration for `a` in the wrong scope and then trying to remove it from the `IdResolver` explodes.

---

### Comment 6 - AaronBallman

> Either way, it seems like we're somehow getting the declaration for `a` in the wrong scope and then trying to remove it from the `IdResolver` explodes.

It may be more deeply strange than that. I put breakpoints in `Scope::AddDecl`, `IdentifierResolver::AddDecl`, and `IdentifierResolver::RemoveDecl` (`Scope::RemoveDecl` also had a breakpoint was never called):
```
Scope::AddDecl for unused 0x00000259e63be820
IdentifierResolver::AddDecl for unused 0x00000259e63be820

Scope::AddDecl for <Empty> 0x00000259e63be8a0
IdentifierResolver::AddDecl not called (might be questionable behavior)

IdentifierResolver::RemoveDecl for unused 0x00000259e63be820

Scope::AddDecl for unused 0x00000259e63be820
IdentifierResolver::AddDecl for unused 0x00000259e63be820

IdentifierResolver::RemoveDecl for a 0x00000259e63be820
```
Note how the name changed but the pointer value remained the same for the last three entries.

---

### Comment 7 - steakhal

Interesting. Thanks @AaronBallman for going the extra mile!
I'll see if I can make progress on this. No promises though.

---

### Comment 8 - steakhal

> > Either way, it seems like we're somehow getting the declaration for `a` in the wrong scope and then trying to remove it from the `IdResolver` explodes.
> 
> It may be more deeply strange than that. I put breakpoints in `Scope::AddDecl`, `IdentifierResolver::AddDecl`, and `IdentifierResolver::RemoveDecl` (`Scope::RemoveDecl` also had a breakpoint was never called):
> 
> ```
> Scope::AddDecl for unused 0x00000259e63be820
> IdentifierResolver::AddDecl for unused 0x00000259e63be820
> 
> Scope::AddDecl for <Empty> 0x00000259e63be8a0
> IdentifierResolver::AddDecl not called (might be questionable behavior)
> 
> IdentifierResolver::RemoveDecl for unused 0x00000259e63be820
> 
> Scope::AddDecl for unused 0x00000259e63be820
> IdentifierResolver::AddDecl for unused 0x00000259e63be820
> 
> IdentifierResolver::RemoveDecl for a 0x00000259e63be820
> ```
> 
> Note how the name changed but the pointer value remained the same for the last three entries.

Interesting. By debugging I can see that when the parser reaches the call to `f()` within the body of the function template specialization, it gathers the call arguments. As such, it finds that it needs to instantiate a default argument there.
The important call-stack is:
1) `BuildCXXDefaultArgExpr()`
2) `CheckCXXDefaultArgExpr()` /// Instantiate or parse a C++ default argument expression as necessary.
3) `InstantiateDefaultArgument()`
4) `SubstDefaultArgument()` /// Substitute the given template arguments into the default argument.
5) `addInstantiatedParametersToScope()`

This last function `(5)` will eventually call `FunctionParam->setDeclName(PatternParam->getDeclName());` which will overwrite the name of the parameter from `unused` to `a`. This will cause a discrepancy when we get to leave the function template specialization's body and look for a decl with the name `a` for removal.

BTW by debugging this, here is an even shorter reproducer :D
```c++
template <typename T> void f(const T &a = 0);
template <> void f(const int &b)
{ // AddDecl(ParmVarDecl 'b')
  f<int>(); // addInstantiatedParametersToScope() overwrites the name of the ParmVarDecl to 'a'.
} // RemoveDecl(ParamVarDecl 'a'), crash!
```

I struggle to see why `addInstantiatedParametersToScope()` modifies the `ParmVarDecl`. Maybe that shouldn't be called? IDK. Maybe @cor3ntin or someone else has some idea.

---

### Comment 9 - erichkeane

The idea of addInstantiatedParametersToScope is so that we can instantiate the body of the function, and refer to the parameters based on the 'old' var decls.  That is, if the body of 'f' contained a reference to the version in the primary template, it would get the correct parameter, post instantiation (there is a Decl* to Decl* association container).

The goofy part there is either the explicit instantiation (I didn't think we needed to do addInstantiatedParametersToScope when explicitly instantiating?!), or the recursive instantiation there.  Typically we'd notice that f<int> is already being instantiated and skip it I think?  

Its ALSO strange that we're renaming the variable, even in cases where it already has a name?  It might be interesting to see what happens if we only do that if it is unnamed?  ALSO-ALSO strange that our association is by name, which we are typically pretty smart about being mutable.


EDIT: AH, wait, the storing by name isn't weird, when we consider that is an Identifier Resolver, intending to get from the identifer to the variable.  So something else is amiss here.

---

### Comment 10 - shafik

CC @zygoloid your commit was mentioned above: https://github.com/llvm/llvm-project/commit/a5569f089844209dbea2e3241460173d7b6b1420

---

### Comment 11 - ChrisZRJ

> The idea of addInstantiatedParametersToScope is so that we can instantiate the body of the function, and refer to the parameters based on the 'old' var decls. That is, if the body of 'f' contained a reference to the version in the primary template, it would get the correct parameter, post instantiation (there is a Decl* to Decl* association container).
> 
> The goofy part there is either the explicit instantiation (I didn't think we needed to do addInstantiatedParametersToScope when explicitly instantiating?!), or the recursive instantiation there. Typically we'd notice that f is already being instantiated and skip it I think?
> 
> Its ALSO strange that we're renaming the variable, even in cases where it already has a name? It might be interesting to see what happens if we only do that if it is unnamed? ALSO-ALSO strange that our association is by name, which we are typically pretty smart about being mutable.
> 
> EDIT: AH, wait, the storing by name isn't weird, when we consider that is an Identifier Resolver, intending to get from the identifer to the variable. So something else is amiss here.

I had the same problem on dealii. The original commit https://github.com/llvm/llvm-project/commit/a5569f089844209dbea2e3241460173d7b6b1420 seems to fix the scenario of "Default arguments refer to
parameters of the same function". In this case, the result of  `Expr *PatternExpr = Param->getUninstantiatedDefaultArg(); ` is the kind of `CXXUnresolvedConstructExpr`, and the above case is not.

So i add a conditon `isa<CXXUnresolvedConstructExpr>(PatternExpr)` berfore calling the `addInstantiatedParametersToScope`, it seems to work. 

I am wondering is this enough to make `addInstantiatedParametersToScope` work only under the idea of https://github.com/llvm/llvm-project/commit/a5569f089844209dbea2e3241460173d7b6b1420.

---

### Comment 12 - chandraghale

@ChrisZRJ @AaronBallman  @erichkeane 
Can we get a little traction on this issue? I tried adding the condition as suggested before calling `addInstantiatedParametersToScope`, and it seems to be working fine. Can we push these changes to unblock the issue?
```

+++ b/clang/lib/Sema/SemaTemplateInstantiate.cpp
@@ -3548,7 +3548,7 @@ bool Sema::SubstDefaultArgument(
     ContextRAII SavedContext(*this, FD);
     std::unique_ptr<LocalInstantiationScope> LIS;
 
-    if (ForCallExpr) {
+    if (ForCallExpr && isa<CXXUnresolvedConstructExpr>(PatternExpr) ) {
```

---

### Comment 13 - erichkeane

> [@ChrisZRJ](https://github.com/ChrisZRJ) [@AaronBallman](https://github.com/AaronBallman) [@erichkeane](https://github.com/erichkeane) Can we get a little traction on this issue? I tried adding the condition as suggested before calling `addInstantiatedParametersToScope`, and it seems to be working fine. Can we push these changes to unblock the issue?
> 
> ```
> 
> +++ b/clang/lib/Sema/SemaTemplateInstantiate.cpp
> @@ -3548,7 +3548,7 @@ bool Sema::SubstDefaultArgument(
>      ContextRAII SavedContext(*this, FD);
>      std::unique_ptr<LocalInstantiationScope> LIS;
>  
> -    if (ForCallExpr) {
> +    if (ForCallExpr && isa<CXXUnresolvedConstructExpr>(PatternExpr) ) {
> ```

I don't believe that to be the solution here, as that will result in the parameter not being added correctly.  As far as 'traction', as most contributors are volunteers, it comes down to someone picking this up to work on.

---

### Comment 14 - zygoloid

OK, so for:
```c++
template <typename T> void f(const T &a, int c = 0);
template <> void f(const int &unused, int) {
  f(42);
}
```
we're going to form the `f<int>` specialization, with a first parameter named `a` and a second parameter named `c`. Then we find a need to instantiate the default argument from the primary template, while still building that specialization. So we need to switch back into the context of the primary template so we can instantiate the default argument. And doing that in the middle of building the specialization certainly seems bound to cause problems.

One of the problems is, when instantiating a default argument, we want to push the parameters back into scope so they can be referenced. Which, among other things, means they need to have the right names, so that if the default argument of `c` mentions `a`, we can find it, and it has the right type, etc. But if we're already in the middle of forming a specialization of `f` (or an instantiation from a different declaration), we'll need to do this a lot more carefully: for example, we need to set the name back when we're done with it.

Maybe we shouldn't be doing default argument instantiation this way at all? We could instead try to instantiate a completely separate `f<int>` from the primary template, distinct from the `f<int>` explicit specialization, with its own parameters. And then instantiating the default argument expression for that specialization should be unproblematic. But that'd potentially introduce some quite significant cost -- we'd be instantiating a declaration of `f<int>` just to throw it away again, and just in case the default argument references part of it.

Maybe another option would be to somehow lazily instantiate parameters of `f` from the primary template if/when they get used by the default argument. I think the CWG2369 resolution is also going to require something like lazy parameter instantiation in the case where the constraints on a function refer to a parameter.

---

