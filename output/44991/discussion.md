# __builtin_bit_cast hits unreachable when casting a cast pointer

**Author:** john-brawn-arm
**URL:** https://github.com/llvm/llvm-project/issues/44991
**Status:** Closed
**Labels:** clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2026-03-06T18:06:05Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [45646](https://llvm.org/bz45646) |
| Version | trunk |
| OS | Linux |
| CC | @atnnn,@zygoloid |

## Extended Description 
If I compile

```c++
  long fn() {
    return __builtin_bit_cast(long, (long)&fn);
  }
```

I get

  LValue subobject in bit_cast?
  UNREACHABLE executed at /home/jb/work/llvm-project/clang/lib/AST/ExprConstant.cpp:6331!

With a backtrace that shows this happened when trying to evaluate the __builtin_bit_cast as a constant expression during CodeGen.

This rather odd use of __builtin_bit_cast came originally from cmse_nsfptr_create which is a macro defined in arm_cmse.h (in clang/lib/Headers) as

```c++
  #define cmse_nsfptr_create(p)                                                  \
    __builtin_bit_cast(__typeof__(p),                                            \
                       (__builtin_bit_cast(uintptr_t, p) & ~(uintptr_t)1))
```

so you get this failure if you do (compiling with clang --target=arm-none-eabi -mcpu=cortex-m33 -mcmse)

```c++
  #include <arm_cmse.h>
  unsigned fn() {
    cmse_nsfptr_create((unsigned)&fn);
  }
```

## Comments

### Comment 1 - tbaederr

This does not reproduce anymore: https://godbolt.org/z/M8cqEfWGa

Closing.

---

### Comment 2 - tbaederr

Ignore me, the godbolt link only works because it doesn't have assertions enabled. This still reproduces in current `main.

---

### Comment 3 - Endilll

Still crashing as of post-17 trunk: https://godbolt.org/z/MYKoja8vG
```
LValue subobject in bit_cast?
UNREACHABLE executed at /root/llvm-project/clang/lib/AST/ExprConstant.cpp:7002!

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -Wno-implicit-int -Wno-implicit-function-declaration <source>
1.	<eof> parser at end of file
2.	<source>:1:8: LLVM IR generation of declaration 'fn'
3.	<source>:1:8: Generating code for declaration 'fn'
 #0 0x0000000003673848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3673848)
 #1 0x00000000036716cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36716cc)
 #2 0x00000000035bef28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f3508d3c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f35087ff00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f35087de859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00000000035ca1da (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x35ca1da)
 #7 0x0000000006e83a7f (anonymous namespace)::APValueToBufferConverter::visit(clang::APValue const&, clang::QualType, clang::CharUnits) ExprConstant.cpp:0:0
 #8 0x0000000006ea1310 (anonymous namespace)::handleLValueToRValueBitCast((anonymous namespace)::EvalInfo&, clang::APValue&, clang::APValue&, clang::CastExpr const*) (.part.0) ExprConstant.cpp:0:0
 #9 0x0000000006ed7fa9 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#10 0x0000000006ea3b84 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x0000000006e9289e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#12 0x0000000006e9cb39 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#13 0x0000000006e9e210 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e9e210)
#14 0x0000000006e9e3dc clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e9e3dc)
#15 0x0000000003981ef2 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3981ef2)
#16 0x00000000039895bb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39895bb)
#17 0x000000000398f8ac clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x398f8ac)
#18 0x00000000039ed076 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39ed076)
#19 0x0000000003a0016a clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a0016a)
#20 0x0000000003a614cd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a614cd)
#21 0x0000000003a5c685 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a5c685)
#22 0x0000000003a5cc43 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a5cc43)
#23 0x0000000003a65e13 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#24 0x00000000048c8726 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#25 0x00000000048baec8 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48baec8)
#26 0x0000000005d80464 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5d80464)
#27 0x00000000048c5798 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c5798)
#28 0x000000000412d9d9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412d9d9)
#29 0x00000000040b1d6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40b1d6e)
#30 0x000000000420dc46 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x420dc46)
#31 0x0000000000bcb672 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbcb672)
#32 0x0000000000bc3e5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000003f11c19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000035bf3d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x35bf3d4)
#35 0x0000000003f1220f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000003eda175 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eda175)
#37 0x0000000003edabdd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edabdd)
#38 0x0000000003ee2705 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee2705)
#39 0x0000000000bc9927 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbc9927)
#40 0x0000000000ac39f1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xac39f1)
#41 0x00007f35087e0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#42 0x0000000000bc393e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbc393e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-codegen

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: John Brawn (john-brawn-arm)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [45646](https://llvm.org/bz45646) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->atnnn,@<!-- -->zygoloid |

## Extended Description 
If I compile

```c++
  long fn() {
    return __builtin_bit_cast(long, (long)&amp;fn);
  }
```

I get

  LValue subobject in bit_cast?
  UNREACHABLE executed at /home/jb/work/llvm-project/clang/lib/AST/ExprConstant.cpp:6331!

With a backtrace that shows this happened when trying to evaluate the __builtin_bit_cast as a constant expression during CodeGen.

This rather odd use of __builtin_bit_cast came originally from cmse_nsfptr_create which is a macro defined in arm_cmse.h (in clang/lib/Headers) as

```c++
  #define cmse_nsfptr_create(p)                                                  \
    __builtin_bit_cast(__typeof__(p),                                            \
                       (__builtin_bit_cast(uintptr_t, p) &amp; ~(uintptr_t)1))
```

so you get this failure if you do (compiling with clang --target=arm-none-eabi -mcpu=cortex-m33 -mcmse)

```c++
  #include &lt;arm_cmse.h&gt;
  unsigned fn() {
    cmse_nsfptr_create((unsigned)&amp;fn);
  }
```
</details>


---

