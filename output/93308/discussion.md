# [clang++] extremely slow when trying to use undeclared identifiers

**Author:** MonsterDruide1
**URL:** https://github.com/llvm/llvm-project/issues/93308
**Status:** Closed
**Labels:** clang:frontend, slow-compile
**Closed Date:** 2025-06-13T10:45:42Z

## Body

Given a program in the following format:
```cpp
void test() {
  int a11;
  [...]  // a bunch more  variable declarations not used afterwards
  
  if( a97 == a95 ) {}  // comparing non-existant variables
}
```

`clang++` can take up to 5 minutes compiling this or even crashes, while `clang` can compile it just fine.

<details>
<summary>Full code example</summary>

```cpp
void test() {
  int  a11;
  int  a12;
  int  a13;
  int  a14;
  int* a15;
  int  a16;
  int  a17;
  int  a18;
  int  a22;
  int  a24;
  int  a25;
  int  a35;
  int  a36;
  int  a37;
  int  a38;
  int  a39;
  int  a40;
  int  a42;
  int  a91;
  int  a92;
  int  a98;
  int  a99;

  if(
    a97 == a95,
    a96 == a96,
    a97 == a96,
    a95 == a93,
    a96 == a105
  ) {}
}
```
</details>

The example above compiles for more than 5 minutes for me. Variants can be created by adding/removing more variable declarations, but the code is somehow very sensitive to working correctly (compiling in less than 0.1 second) again.

I'm not able to pinpoint what actually causes this to create an even more minimized example, but as it seems to be influenced by the variable names, I believe that the generator for "did you mean variable x instead?" is somehow at fault here.

Here's some other things I tried/observed:
- Running `time` on it shows that `user/sys` is really low, only `real` increases over time - meaning `clang++` doesn't actually take so long for compiling, but waits for the most time.
- Saving the code above as `test.c` and running `clang++ test.c` takes very long, while using `clang test.c` takes less than a second.
- Changing that single pointer to a normal `int` "fixes" it.
- Using standard assignments instead of `if` checks fixes it, but assigning it within the `if` clause also causes the issue.
- Changing the variable names too much results in taking less time, down to 0.1 seconds.
- The time it takes to finish "compiling" (display errors) varies with the number of declarations and comparisons, but removing the "wrong" ones can fix it again.
- When encountering it in my normal project, I first got some actual crashes from Clang - I cannot re-create those though, and they only occured after what felt like a timeout, when touching the file it read from (saving again)... I can't reproduce it though, so feel free to ignore this one and only focus on the long compile time.

After trying around with this for more than 3 hours without being able to further "destill" down the issue, let's hope someone else around here has a theory on why this occurs.

For experiments, feel free to test with this Godbolt:
[Godbolt](https://godbolt.org/z/rEEY9Tfjx)
- gcc: ~140 ms
- clang (any version): at least 25000 ms

## Comments

### Comment 1 - DimitryAndric

> `clang++` can take up to 5 minutes compiling this or even crashes, while `clang` can compile it just fine.

I think you meant the second item to be "gcc" here? Because using `clang` or `clang++` should not matter if the extension is .cpp, it will always compile it as C++.

That said, you have apparently found an interesting corner case:

```text
  PID USERNAME    THR PRI NICE   SIZE    RES STATE    C   TIME    WCPU COMMAND
28476 dim           1 133    0  9212M  7801M CPU0     0   1:47 100.19% clang
```

For some reason it starts allocating memory like crazy for this,  even if optimization is off. This is definitely a bug, albeit a strange one.


---

### Comment 2 - MonsterDruide1

No, I did actually mean `clang`.
```
monsterdruide1@pc:~/clang-test$ time clang test.c
test.c:26:5: error: use of undeclared identifier 'a97'
[...]
10 errors generated.

real    0m0.178s
user    0m0.011s
sys     0m0.034s
monsterdruide1@pc:~/clang-test$ time clang++ test.c
clang: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Wdeprecated]
[aborting after a while]^C

real    0m36.801s
user    0m0.015s
sys     0m0.016s
```

Note that this is `clang-14` though, I didn't test with `clang-18` apart from Godbolt.

---

### Comment 3 - EugeneZelenko

Even 18 is not maintained anymore. Please try `main` on https://godbolt.org.

---

### Comment 4 - MonsterDruide1

Sorry, where can I select `main` as compiler version in Godbolt? I can only see some `trunk`s, but those seem completely broken (with CMake).

---

### Comment 5 - EugeneZelenko

> Sorry, where can I select `main` as compiler version in Godbolt? I can only see some `trunk`s, but those seem completely broken (with CMake).

`trunk` is name for build based on `main` branch.

---

### Comment 6 - MonsterDruide1

Ah, thanks. And yes, that still has the issue, being killed for `timeout` after 25000 ms.

---

### Comment 7 - DimitryAndric

Ah, I had saved the file as a .cpp file. If you use clang++ on a .c file, it warns `treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Wdeprecated]`, but it does treat it as C++. Indeed, that causes the slowdown.

In any case, it's also very slow with `llvmorg-19-init-11697-g`2c2e0507e92.


---

### Comment 8 - DimitryAndric

This is apparently a very old bug since it also reproduces with `llvmorg-10-init-00000-g`8f5b44aead8, and probably even older versions.

It looks like it is spending lots of time in `TransformTypos::RecursiveTransformLoop`, e.g.:

```text
clang::ASTContext::isPromotableBitField(clang::Expr*) const ()
clang::Sema::UsualUnaryConversions(clang::Expr*) ()
clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) ()
checkArithmeticOrEnumeralCompare(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) ()
clang::Sema::CheckCompareOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) ()
clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) ()
clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
(anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) ()
clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) ()
clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) ()
clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) ()
```



---

### Comment 9 - shafik

godbolt times out for clang: https://godbolt.org/z/6hvEx5a49

---

### Comment 10 - erichkeane

> This is apparently a very old bug since it also reproduces with `llvmorg-10-init-00000-g`[8f5b44a](https://github.com/llvm/llvm-project/commit/8f5b44aead89a56c6fbf85ccfda03ae1e82ac431), and probably even older versions.
> 
> It looks like it is spending lots of time in `TransformTypos::RecursiveTransformLoop`, e.g.:
> 
> ```
> clang::ASTContext::isPromotableBitField(clang::Expr*) const ()
> clang::Sema::UsualUnaryConversions(clang::Expr*) ()
> clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) ()
> checkArithmeticOrEnumeralCompare(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) ()
> clang::Sema::CheckCompareOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) ()
> clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) ()
> clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
> clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
> clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
> clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
> clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) ()
> (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) ()
> clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) ()
> clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) ()
> clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) ()
> ```

It appears we're hitting some really bad scaling issue when trying to do `TransformTypos` when there are (shockingly few) many alternatives here.  Someone probably has to dig in with a debugger and see if there is a good reason for this (the 1 not being an int, but a pointer is strange), and whether we can skip trying to suggest typo suggestions if there are 'too many' options (or something similar).

Not quite a beginner friendly bug, but at least mostly self-contained.

---

### Comment 11 - Backl1ght

After some debugging, I think I have found out why clang takes so many time compiling this code.

There are 10 typo exprs inside the if statement(i.e. `a97`, `a95`, ...), each of them has a few typo correction candidates. For example, `a97` has 6 candidates and `a105` has 3 candidates. clang will try correct these typos by searching all ways of replacement until one valid way found. Assume the $i$-th typo has $a_i$ candidates, and the number of possible ways will be $\prod a_i$, which might grow exponentially.

Assume we got 3 typos and each typo has 2 candidates, we can use a binary string $s$ to represent a way of replacement, $s_i$ means replace $i$-th typo with it's $s_i$-th candiate, clang will search in the order like 000, 100, 010, 110, 001 and so on.

Unfortunately, `a105` is the last typo and it's first candidate is `a15`, replace `a105` with `a15` will not make the expr valid. As a result, clang try tens of millions of ways to finally find one valid. If we replace `a105` with `a106`, clang finish the compilation quickly.

---

### Comment 12 - Backl1ght

I haven't figure out an elegant way to resolve this issue. Is simply limit the number of searches acceptable?

---

### Comment 13 - cor3ntin

@Backl1ght I don't understand how we get to a millions of elements in the search space, where are these elements created?
I think it's reasonable to limit the search space. In particular, limiting how "deep" typos corrections can participate to the search of other typos correct would make sense to me

---

### Comment 14 - Backl1ght

To explain the problem more clearly, here are some debug messages.

```plaintext
Loop count: 45177217
TYPO 0: name is a97, number of candidates is 7, candidats is ["","a98","a92","a99","a91","a17","a37",], candidate chosen is 1
TYPO 1: name is a95, number of candidates is 8, candidats is ["","a98","a92","a25","a99","a91","a15","a35",], candidate chosen is 1
TYPO 2: name is a96, number of candidates is 7, candidats is ["","a98","a92","a16","a99","a91","a36",], candidate chosen is 1
TYPO 3: name is a96, number of candidates is 7, candidats is ["","a98","a92","a16","a99","a91","a36",], candidate chosen is 1
TYPO 4: name is a97, number of candidates is 7, candidats is ["","a98","a92","a99","a91","a17","a37",], candidate chosen is 1
TYPO 5: name is a96, number of candidates is 7, candidats is ["","a98","a92","a16","a99","a91","a36",], candidate chosen is 1
TYPO 6: name is a95, number of candidates is 8, candidats is ["","a98","a92","a25","a99","a91","a15","a35",], candidate chosen is 1
TYPO 7: name is a93, number of candidates is 6, candidats is ["","a98","a92","a99","a91","a13",], candidate chosen is 1
TYPO 8: name is a96, number of candidates is 7, candidats is ["","a98","a92","a16","a99","a91","a36",], candidate chosen is 1
TYPO 9: name is a105, number of candidates is 4, candidats is ["","a15","a25","a12",], candidate chosen is 2
```

The while loop here executes 45177217 times, and the product of first 9 typos' candidates size is 45177216, which means the expr became valid right after candidate of typo 9 changed from `a15` to `a25`.

https://github.com/llvm/llvm-project/blob/369e24aa3852baccffe61c69ce891dc47fd5e176/clang/lib/Sema/SemaExprCXX.cpp#L8895-L8912

I have no idea about limiting how "deep" typos corrections can participate to the search of other typos correct, since all candidates of the same typo are in the same scope and have same edit distance. cc @cor3ntin 

---

### Comment 15 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MonsterDruide1)

<details>
Given a program in the following format:
```cpp
void test() {
  int a11;
  [...]  // a bunch more  variable declarations not used afterwards
  
  if( a97 == a95 ) {}  // comparing non-existant variables
}
```

`clang++` can take up to 5 minutes compiling this or even crashes, while `clang` can compile it just fine.

&lt;details&gt;
&lt;summary&gt;Full code example&lt;/summary&gt;

```cpp
void test() {
  int  a11;
  int  a12;
  int  a13;
  int  a14;
  int* a15;
  int  a16;
  int  a17;
  int  a18;
  int  a22;
  int  a24;
  int  a25;
  int  a35;
  int  a36;
  int  a37;
  int  a38;
  int  a39;
  int  a40;
  int  a42;
  int  a91;
  int  a92;
  int  a98;
  int  a99;

  if(
    a97 == a95,
    a96 == a96,
    a97 == a96,
    a95 == a93,
    a96 == a105
  ) {}
}
```
&lt;/details&gt;

The example above compiles for more than 5 minutes for me. Variants can be created by adding/removing more variable declarations, but the code is somehow very sensitive to working correctly (compiling in less than 0.1 second) again.

I'm not able to pinpoint what actually causes this to create an even more minimized example, but as it seems to be influenced by the variable names, I believe that the generator for "did you mean variable x instead?" is somehow at fault here.

Here's some other things I tried/observed:
- Running `time` on it shows that `user/sys` is really low, only `real` increases over time - meaning `clang++` doesn't actually take so long for compiling, but waits for the most time.
- Saving the code above as `test.c` and running `clang++ test.c` takes very long, while using `clang test.c` takes less than a second.
- Changing that single pointer to a normal `int` "fixes" it.
- Using standard assignments instead of `if` checks fixes it, but assigning it within the `if` clause also causes the issue.
- Changing the variable names too much results in taking less time, down to 0.1 seconds.
- The time it takes to finish "compiling" (display errors) varies with the number of declarations and comparisons, but removing the "wrong" ones can fix it again.
- When encountering it in my normal project, I first got some actual crashes from Clang - I cannot re-create those though, and they only occured after what felt like a timeout, when touching the file it read from (saving again)... I can't reproduce it though, so feel free to ignore this one and only focus on the long compile time.

After trying around with this for more than 3 hours without being able to further "destill" down the issue, let's hope someone else around here has a theory on why this occurs.

For experiments, feel free to test with this Godbolt:
[Godbolt](https://godbolt.org/z/rEEY9Tfjx)
- gcc: ~140 ms
- clang (any version): at least 25000 ms
</details>


---

### Comment 16 - zygoloid

Limiting the search space -- or just disabling typo correction if there are too many poosibilities -- makes sense for cases that are fundamentally exponential. Eg, `f(a12, a13, a14, ..., aN)` seems like we'd need to consider all possibilities to get the right answer).

I think we should also think about how to take advantage of common substructure between the options: if a subexpression is invalid with one typo correction applied, the choice of corrections elsewhere won't change that, so we can consider corrections independently in some cases. It's probably a bit tricky to change our current algorithm to handle this, but it would bring the runtime for examples like the original one here down to being linear.

---

