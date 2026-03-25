# [Clang] Clang hangs when compiling valid code involving std::thread and lambda with `--std=c++23`

**Author:** Xniao
**URL:** https://github.com/llvm/llvm-project/issues/151451

## Body

Clang trunk hangs when compiling following program under C++23 mode. The same code compiles successfully and quickly with `--std=c++20`.
## Program
```cpp
#include <thread>

int main() {
    std::thread memory_thread([]() {
      const int alloc_size = 1024 * 1024;
      while (true) {
        char *buffer = new char[alloc_size];
        if (buffer == nullptr)
          break;
        delete[] buffer;
      }
    });
  return 0;
}
```
Here is the repro: https://godbolt.org/z/4v1c5Wreq

## Comments

### Comment 1 - tbaederr

Looks like we're constant-evaluating the lambda in codegen:
```
(lldb) bt
* thread #1, name = 'clang++', stop reason = signal SIGSTOP
  * frame #0: 0x00007fffdc702ebe libclang-cpp.so.22.0git`bool llvm::detail::isPresent<clang::QualType>(t=0x00007e0f831fe1e8) at Casting.h:630:3
    frame #1: 0x00007fffdd153da5 libclang-cpp.so.22.0git`decltype(auto) llvm::dyn_cast<clang::InjectedClassNameType, clang::QualType>(Val=0x00007e0f831fe1e8) at Casting.h:650:3
    frame #2: 0x00007fffdd0b8587 libclang-cpp.so.22.0git`clang::InjectedClassNameType const* clang::Type::getAs<clang::InjectedClassNameType>(this=0x00007e0f831fe1e0) const at TypeNodes.inc:96:1
    frame #3: 0x00007fffdf9a7764 libclang-cpp.so.22.0git`clang::Type::getAsTagDecl(this=0x00007e0f831fe1e0) const at Type.cpp:1940:30
    frame #4: 0x00007fffdf96cf05 libclang-cpp.so.22.0git`clang::Type::getAsCXXRecordDecl(this=0x00007e0f831fe1e0) const at Type.cpp:1930:42
    frame #5: 0x00007fffde49f27a libclang-cpp.so.22.0git`HandleDestructionImpl(Info=0x00007bff822a9860, CallRange=SourceRange @ 0x00007bff82333020, This=0x00007bff8237e160, Value=0x00007bff7b347f88, T=QualType @ 0x00007bff82333040) at ExprConstant.cpp:7009:32
    frame #6: 0x00007fffde49f0e4 libclang-cpp.so.22.0git`HandleDestructionImpl(Info=0x00007bff822a9860, CallRange=SourceRange @ 0x00007bff8237e020, This=0x00007bff82014620, Value=0x00007caf831eada8, T=QualType @ 0x00007bff8237e040) at ExprConstant.cpp:7000:12
    frame #7: 0x00007fffde49df10 libclang-cpp.so.22.0git`HandleDestruction(Info=0x00007bff822a9860, Loc=(ID = 1935), LVBase=LValueBase @ 0x00007ffffffed510, Value=0x00007caf831eada8, T=QualType @ 0x00007ffffffed508) at ExprConstant.cpp:7175:10
    frame #8: 0x00007fffde72d024 libclang-cpp.so.22.0git`(anonymous namespace)::VoidExprEvaluator::VisitCXXDeleteExpr(this=0x00007bff81e7a140, E=0x00007bff7e4ff538) at ExprConstant.cpp:16673:8
    frame #9: 0x00007fffde71c341 libclang-cpp.so.22.0git`clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(this=0x00007bff81e7a140, S=0x00007bff7e4ff538) at StmtNodes.inc:736:1
    frame #10: 0x00007fffde7161b0 libclang-cpp.so.22.0git`EvaluateVoid(E=0x00007bff7e4ff538, Info=0x00007bff822a9860) at ExprConstant.cpp:16692:34
    frame #11: 0x00007fffde4172dc libclang-cpp.so.22.0git`Evaluate(Result=0x00007bff81e7a0a0, Info=0x00007bff822a9860, E=0x00007bff7e4ff538) at ExprConstant.cpp:16756:10
    frame #12: 0x00007fffde4a966f libclang-cpp.so.22.0git`EvaluateIgnoredValue(Info=0x00007bff822a9860, E=0x00007bff7e4ff538) at ExprConstant.cpp:2077:8
    frame #13: 0x00007fffde49806b libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fbeb0, Info=0x00007bff822a9860, S=0x00007bff7e4ff538, Case=0x0000000000000000) at ExprConstant.cpp:5627:14
    frame #14: 0x00007fffde498d90 libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fbeb0, Info=0x00007bff822a9860, S=0x00007bff7e4ff560, Case=0x0000000000000000) at ExprConstant.cpp:5677:28
    frame #15: 0x00007fffde4a9184 libclang-cpp.so.22.0git`EvaluateLoopBody(Result=0x00007bff821fbeb0, Info=0x00007bff822a9860, Body=0x00007bff7e4ff560, Case=0x0000000000000000) at ExprConstant.cpp:5373:24
    frame #16: 0x00007fffde499a9d libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fbeb0, Info=0x00007bff822a9860, S=0x00007bff7e4ff590, Case=0x0000000000000000) at ExprConstant.cpp:5737:28
    frame #17: 0x00007fffde498d90 libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fbeb0, Info=0x00007bff822a9860, S=0x00007bff7e4ff5b8, Case=0x0000000000000000) at ExprConstant.cpp:5677:28
    frame #18: 0x00007fffde41a16d libclang-cpp.so.22.0git`HandleFunctionCall(CallLoc=(ID = 2132595), Callee=0x00007bff7e4fef50, ObjectArg=0x00007bff822ae060, E=0x00007bff7d75cbc8, Args=ArrayRef<const clang::Expr *> @ 0x00007bff821fbc40, Call=CallRef @ 0x00007bff821fbc20, Body=0x00007bff7e4ff5b8, Info=0x00007bff822a9860, Result=0x00007bff81ebdc20, ResultSlot=0x0000000000000000) at ExprConstant.cpp:6709:24
    frame #19: 0x00007fffde73aac4 libclang-cpp.so.22.0git`(anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::VoidExprEvaluator>::handleCallExpr(this=0x00007bff81ebdbc0, E=0x00007bff7d75cbc8, Result=0x00007bff81ebdc20, ResultSlot=0x0000000000000000) at ExprConstant.cpp:8524:10
    frame #20: 0x00007fffde73783a libclang-cpp.so.22.0git`(anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::VoidExprEvaluator>::VisitCallExpr(this=0x00007bff81ebdbc0, E=0x00007bff7d75cbc8) at ExprConstant.cpp:8318:10
    frame #21: 0x00007fffde72acaa libclang-cpp.so.22.0git`(anonymous namespace)::VoidExprEvaluator::VisitCallExpr(this=0x00007bff81ebdbc0, E=0x00007bff7d75cbc8) at ExprConstant.cpp:16594:35
    frame #22: 0x00007fffde72aee3 libclang-cpp.so.22.0git`clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::VisitCXXOperatorCallExpr(this=0x00007bff81ebdbc0, S=0x00007bff7d75cbc8) at StmtNodes.inc:618:1
    frame #23: 0x00007fffde71b804 libclang-cpp.so.22.0git`clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(this=0x00007bff81ebdbc0, S=0x00007bff7d75cbc8) at StmtNodes.inc:618:1
    frame #24: 0x00007fffde7161b0 libclang-cpp.so.22.0git`EvaluateVoid(E=0x00007bff7d75cbc8, Info=0x00007bff822a9860) at ExprConstant.cpp:16692:34
    frame #25: 0x00007fffde4172dc libclang-cpp.so.22.0git`Evaluate(Result=0x00007bff81ebb420, Info=0x00007bff822a9860, E=0x00007bff7d75cbc8) at ExprConstant.cpp:16756:10
    frame #26: 0x00007fffde498a9b libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fb2b0, Info=0x00007bff822a9860, S=0x00007bff7d75cc00, Case=0x0000000000000000) at ExprConstant.cpp:5667:17
    frame #27: 0x00007fffde498d90 libclang-cpp.so.22.0git`EvaluateStmt(Result=0x00007bff821fb2b0, Info=0x00007bff822a9860, S=0x00007bff7d75cc18, Case=0x0000000000000000) at ExprConstant.cpp:5677:28
    frame #28: 0x00007fffde41a16d libclang-cpp.so.22.0git`HandleFunctionCall(CallLoc=(ID = 2134009), Callee=0x00007bff7d75c078, ObjectArg=0x0000000000000000, E=0x00007bff7d75ccd8, Args=ArrayRef<const clang::Expr *> @ 0x00007bff821fb040, Call=CallRef @ 0x00007bff821fb020, Body=0x00007bff7d75cc18, Info=0x00007bff822a9860, Result=0x00007bff81ebb420, ResultSlot=0x0000000000000000) at ExprConstant.cpp:6709:24
    frame #29: 0x00007fffde73aac4 libclang-cpp.so.22.0git`(anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::VoidExprEvaluator>::handleCallExpr(this=0x00007bff81ebb3c0, E=0x00007bff7d75ccd8, Result=0x00007bff81ebb420, ResultSlot=0x0000000000000000) at ExprConstant.cpp:8524:10
    frame #30: 0x00007fffde73783a libclang-cpp.so.22.0git`(anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::VoidExprEvaluator>::VisitCallExpr(this=0x00007bff81ebb3c0, E=0x00007bff7d75ccd8) at ExprConstant.cpp:8318:10
    frame #31: 0x00007fffde72acaa libclang-cpp.so.22.0git`(anonymous namespace)::VoidExprEvaluator::VisitCallExpr(this=0x00007bff81ebb3c0, E=0x00007bff7d75ccd8) at ExprConstant.cpp:16594:35
    frame #32: 0x00007fffde71b6d2 libclang-cpp.so.22.0git`clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(this=0x00007bff81ebb3c0, S=0x00007bff7d75ccd8) at StmtNodes.inc:608:1
    frame #33: 0x00007fffde7161b0 libclang-cpp.so.22.0git`EvaluateVoid(E=0x00007bff7d75ccd8, Info=0x00007bff822a9860) at ExprConstant.cpp:16692:34
    frame #34: 0x00007fffde4172dc libclang-cpp.so.22.0git`Evaluate(Result=0x00007bff81f06a30, Info=0x00007bff822a9860, E=0x00007bff7d75ccd8) at ExprConstant.cpp:16756:10
    frame #35: 0x00007fffde40c217 libclang-cpp.so.22.0git`EvaluateAsRValue(Info=0x00007bff822a9860, E=0x00007bff7d75ccd8, Result=0x00007bff81f06a30) at ExprConstant.cpp:16835:8
    frame #36: 0x00007fffde3fd93e libclang-cpp.so.22.0git`EvaluateAsRValue(E=0x00007bff7d75ccd8, Result=0x00007bff81f06a20, Ctx=0x00007e9f831e0200, Info=0x00007bff822a9860) at ExprConstant.cpp:16918:10
    frame #37: 0x00007fffde3fc95c libclang-cpp.so.22.0git`clang::Expr::EvaluateAsRValue(this=0x00007bff7d75ccd8, Result=0x00007bff81f06a20, Ctx=0x00007e9f831e0200, InConstantContext=true) const at ExprConstant.cpp:16967:10
    frame #38: 0x00007fffde40adb8 libclang-cpp.so.22.0git`clang::Expr::isEvaluatable(this=0x00007bff7d75ccd8, Ctx=0x00007e9f831e0200, SEK=SE_NoSideEffects) const at ExprConstant.cpp:17237:10
    frame #39: 0x00007fffe55b375d libclang-cpp.so.22.0git`clang::CodeGen::CodeGenFunction::EmitReturnStmt(this=0x00007bff8245e040, S=0x00007bff7d75cd18) at CGStmt.cpp:1718:18
```

where the expr is:
```
CallExpr 0x7bff7d75ccd8 'void'
|-ImplicitCastExpr 0x7bff7d75ccb8 'void (*)(__invoke_other, class (lambda at array.cpp:90:31) &&)' <FunctionToPointerDecay>
| `-DeclRefExpr 0x7bff7d75c1f0 'void (__invoke_other, class (lambda at array.cpp:90:31) &&)' lvalue Function 0x7bff7d75c078 '__invoke_impl' 'void (__invoke_other, class (lambda at array.cpp:90:31) &&)' (FunctionTemplate 0x7ecf8344df40 '__invoke_impl')
|   `-NestedNameSpecifier Namespace 0x7ecf8344ca60 'std'
|-CXXFunctionalCastExpr 0x7bff7d75b1c0 '__tag':'struct std::__invoke_other' functional cast to __tag <NoOp>
| `-InitListExpr 0x7bff7d75b178 '__tag':'struct std::__invoke_other'
`-CallExpr 0x7bff7d75ba88 'class (lambda at array.cpp:90:31)' xvalue
  |-ImplicitCastExpr 0x7bff7d75ba68 'class (lambda at array.cpp:90:31) &&(*)(typename std::remove_reference<(lambda at array.cpp:90:31)>::type &) noexcept' <BuiltinFnToFnPtr>
  | `-DeclRefExpr 0x7bff7d75b9f8 '<builtin fn type>' Function 0x7bff7d875890 'forward' 'class (lambda at array.cpp:90:31) &&(typename std::remove_reference<(lambda at array.cpp:90:31)>::type &) noexcept' (FunctionTemplate 0x7e8f8328b0f8 'forward')
  |   `-NestedNameSpecifier Namespace 0x7ecf8344ca60 'std'
  `-DeclRefExpr 0x7bff7d75b2a8 'class (lambda at array.cpp:90:31)' lvalue ParmVar 0x7bff7d7568a8 '__fn' 'class (lambda at array.cpp:90:31) &&'
```

---

### Comment 2 - tbaederr

We have something like `1'048'563` steps we do and each of the loop iteration only accounts for 4 of those because we don't count the destruction and creation of the variables. So we do _a few_ iterations before realizing that this won't work.

---

### Comment 3 - shafik

Looks ok in clang-19: https://godbolt.org/z/856Pb485z

---

### Comment 4 - efriedma-quic

Reduced:

```c++
constexpr int x = []() {
      while (true) {
        char *buffer = new char[1024];
        if (buffer == nullptr)
          break;
        delete[] buffer;
      }
      return 1;
    }();
```

Goes back all the way to clang 10.

---

### Comment 5 - shafik

> Goes back all the way to clang 10.

The original code only trigger in C++23 mode, do you have a feeling why you think this is the same bug?


---

### Comment 6 - efriedma-quic

The contents of the lambda are the same comparing my reduction and the original.  The only difference is whether the compiler tries to perform constant evaluation.  Which isn't really important, unless you have some reason to believe constant evaluation is illegal here.

Probably the difference is due to constexpr-unknown.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jireh Guo (Xniao)

<details>
Clang trunk hangs when compiling following program under C++23 mode. The same code compiles successfully and quickly with `--std=c++20`.
## Program
```cpp
#include &lt;thread&gt;

int main() {
    std::thread memory_thread([]() {
      const int alloc_size = 1024 * 1024;
      while (true) {
        char *buffer = new char[alloc_size];
        if (buffer == nullptr)
          break;
        delete[] buffer;
      }
    });
  return 0;
}
```
Here is the repro: https://godbolt.org/z/4v1c5Wreq
</details>


---

