# [CIR] Assertion failure when compiling struct with default initializer

**Author:** HendrikHuebner
**URL:** https://github.com/llvm/llvm-project/issues/167923
**Status:** Closed
**Labels:** crash, ClangIR
**Closed Date:** 2025-11-13T21:39:39Z

## Body

I'm seeing an assertion failure when compiling the below example with the main branch checked out. I could not reproduce it with the incubator repo.

```
struct Foo {
  int i = 123;
};

void test() {
  Foo f{}; // Only happens with brace initialization !
}
```

Stacktrace:
```
Stack dump:
0.      Program arguments: build/release/bin/clang test.cc -fclangir -emit-cir
1.      <eof> parser at end of file
 #0 0x000000000493db38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/hhuebner/Documents/workspace/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #1 0x000000000493b6b3 llvm::sys::RunSignalHandlers() /home/hhuebner/Documents/workspace/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x00000000048b2836 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/hhuebner/Documents/workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00000000048b2836 CrashRecoverySignalHandler(int) /home/hhuebner/Documents/workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x00007fa8b6ff8450 __restore_rt (/lib64/libc.so.6+0x1a450)
 #5 0x00007fa8b70517d4 __pthread_kill_implementation (/lib64/libc.so.6+0x737d4)
 #6 0x00007fa8b6ff839e gsignal (/lib64/libc.so.6+0x1a39e)
 #7 0x00007fa8b6fdf902 abort (/lib64/libc.so.6+0x1902)
 #8 0x00007fa8b6fdf81e _nl_load_domain.cold (/lib64/libc.so.6+0x181e)
 #9 0x00007fa8b6ff04c7 (/lib64/libc.so.6+0x124c7)
#10 0x0000000005995414 (build/release/bin/clang+0x5995414)
#11 0x000000000598e4c5 clang::CIRGen::CIRGenFunction::emitScalarExpr(clang::Expr const*, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenExprScalar.cpp:1422:3
#12 0x00000000059b8ab2 clang::CIRGen::CIRGenFunction::emitScalarInit(clang::Expr const*, mlir::Location, clang::CIRGen::LValue, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:534:7
#13 0x00000000059b4d68 (anonymous namespace)::AggExprEmitter::visitCXXParenListOrInitListExpr(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::FieldDecl*, clang::Expr*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenExprAggregate.cpp:0:7
#14 0x00000000059b0451 clang::CIRGen::CIRGenFunction::emitAggExpr(clang::Expr const*, clang::CIRGen::AggValueSlot) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenExprAggregate.cpp:908:1
#15 0x00000000059b6e1b clang::CIRGen::CIRGenFunction::SourceLocRAIIObject::restore() /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenFunction.h:417:37
#16 0x00000000059b6e1b clang::CIRGen::CIRGenFunction::SourceLocRAIIObject::~SourceLocRAIIObject() /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenFunction.h:418:30
#17 0x00000000059b6e1b clang::CIRGen::CIRGenFunction::emitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CIRGen::LValue, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:583:1
#18 0x00000000059b6789 clang::CIRGen::CIRGenFunction::emitAutoVarInit(clang::CIRGen::CIRGenFunction::AutoVarEmission const&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:213:9
#19 0x00000000059b71a9 clang::CIRGen::CIRGenFunction::emitAutoVarDecl(clang::VarDecl const&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:263:3
#20 0x00000000059b71a9 clang::CIRGen::CIRGenFunction::emitVarDecl(clang::VarDecl const&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:297:10
#21 0x00000000059b8cec clang::CIRGen::CIRGenFunction::emitDecl(clang::Decl const&, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenDecl.cpp:680:9
#22 0x00000000059cefb0 clang::CIRGen::CIRGenFunction::emitDeclStmt(clang::DeclStmt const&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenStmt.cpp:440:22
#23 0x00000000059cefb0 clang::CIRGen::CIRGenFunction::emitSimpleStmt(clang::Stmt const*, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenStmt.cpp:305:12
#24 0x00000000059ce68c clang::CIRGen::CIRGenFunction::emitStmt(clang::Stmt const*, bool, llvm::ArrayRef<clang::Attr const*>) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenStmt.cpp:0:23
#25 0x00000000059ce4ef clang::CIRGen::CIRGenFunction::emitCompoundStmtWithoutScope(clang::CompoundStmt const&, clang::CIRGen::Address*, clang::CIRGen::AggValueSlot) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenStmt.cpp:80:11
#26 0x000000000597990e clang::CIRGen::CIRGenFunction::emitFunctionBody(clang::Stmt const*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenFunction.cpp:543:12
#27 0x000000000597990e clang::CIRGen::CIRGenFunction::generateCode(clang::GlobalDecl, cir::FuncOp, cir::FuncType) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenFunction.cpp:655:24
#28 0x00000000059656cf mlir::OpBuilder::InsertPoint::isSet() const /home/hhuebner/Documents/workspace/llvm-project/llvm/../mlir/include/mlir/IR/Builders.h:337:40
#29 0x00000000059656cf mlir::OpBuilder::restoreInsertionPoint(mlir::OpBuilder::InsertPoint) /home/hhuebner/Documents/workspace/llvm-project/llvm/../mlir/include/mlir/IR/Builders.h:391:12
#30 0x00000000059656cf mlir::OpBuilder::InsertionGuard::~InsertionGuard() /home/hhuebner/Documents/workspace/llvm-project/llvm/../mlir/include/mlir/IR/Builders.h:355:18
#31 0x00000000059656cf clang::CIRGen::CIRGenModule::emitGlobalFunctionDefinition(clang::GlobalDecl, mlir::Operation*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenModule.cpp:450:3
#32 0x0000000005964965 clang::CIRGen::CIRGenModule::emitGlobalDefinition(clang::GlobalDecl, mlir::Operation*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenModule.cpp:919:5
#33 0x0000000005964ead clang::CIRGen::CIRGenModule::emitGlobal(clang::GlobalDecl) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenModule.cpp:402:5
#34 0x00000000059661c5 clang::CIRGen::CIRGenModule::emitTopLevelDecl(clang::Decl*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenModule.cpp:1469:7
#35 0x000000000596222c cir::CIRGenerator::HandleTopLevelDecl(clang::DeclGroupRef) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/CodeGen/CIRGenerator.cpp:80:19
#36 0x00000000058757fb cir::CIRGenConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/CIR/FrontendAction/CIRGenAction.cpp:84:5
#37 0x00000000076e36fa clang::ParseAST(clang::Sema&, bool, bool) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Parse/ParseAST.cpp:174:11
#38 0x000000000552ebd6 clang::FrontendAction::Execute() /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:10
#39 0x000000000549e57d llvm::Error::getPtr() const /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#40 0x000000000549e57d llvm::Error::operator bool() /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#41 0x000000000549e57d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1003:23
#42 0x00000000056157e6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#43 0x000000000371e672 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/hhuebner/Documents/workspace/llvm-project/clang/tools/driver/cc1_main.cpp:300:15
#44 0x000000000371ab4b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/hhuebner/Documents/workspace/llvm-project/clang/tools/driver/driver.cpp:225:12
#45 0x000000000371cd2d clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /home/hhuebner/Documents/workspace/llvm-project/clang/tools/driver/driver.cpp:0:12
#46 0x000000000371cd2d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#47 0x00000000053145f9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Driver/Job.cpp:437:30
#48 0x00000000053145f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#49 0x00000000048b24de llvm::function_ref<void ()>::operator()() const /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#50 0x00000000048b24de llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/hhuebner/Documents/workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#51 0x0000000005313d33 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Driver/Job.cpp:437:7
#52 0x00000000052d844c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#53 0x00000000052d8667 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#54 0x00000000052f0a68 llvm::SmallVectorBase<unsigned int>::empty() const /home/hhuebner/Documents/workspace/llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#55 0x00000000052f0a68 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/hhuebner/Documents/workspace/llvm-project/clang/lib/Driver/Driver.cpp:2253:23
#56 0x000000000371a4ec clang_main(int, char**, llvm::ToolContext const&) /home/hhuebner/Documents/workspace/llvm-project/clang/tools/driver/driver.cpp:407:21
#57 0x000000000372a097 main /home/hhuebner/Documents/workspace/llvm-project/build/release/tools/clang/tools/driver/clang-driver.cpp:17:10
#58 0x00007fa8b6fe1448 __libc_start_call_main (/lib64/libc.so.6+0x3448)
#59 0x00007fa8b6fe150b __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x350b)
#60 0x0000000003718565 _start (build/release/bin/clang+0x3718565)
```

## Comments

### Comment 1 - HendrikHuebner

cc @andykaylor 

---

### Comment 2 - AmrDeveloper

Hello @HendrikHuebner,

Can you reproduce it with the current master? I can compile that code snippet now without any issue

```
cir.func dso_local @_Z4testv() inline(never) {
    %0 = cir.alloca !rec_Foo, !cir.ptr<!rec_Foo>, ["f", init]
    %1 = cir.get_member %0[0] {name = "i"} : !cir.ptr<!rec_Foo> -> !cir.ptr<!s32i> 
    %2 = cir.const #cir.int<123> : !s32i
    cir.store align(4) %2, %1 : !s32i, !cir.ptr<!s32i> 
    cir.return
}
```

And it produces valid code.

Thanks

---

### Comment 3 - andykaylor

@AmrDeveloper Did you try it with a release build with asserts disabled?

@HendrikHuebner We're missing a bit of code in the codegen handling. It's disappointing that we didn't report a diagnostic before hitting the assertion.
I am also hitting an assertion with your reproducer, but I get a different callstack (so a different assertion). This may just be because I'm at a different point in the revision history than you, or it may be because you are building differently (release with asserts?). In my sandbox, the assertion is happening inside `clang::CIRGen::Address::getPointer()` called from `CIRGenFunction::CXXDefaultInitExprScope::CXXDefaultInitExprScope()` and appears to be happening because we failed to initialize `cgf.cxxDefaultInitExprThis`.

I have a simple fix for the problem.

---

### Comment 4 - AmrDeveloper

> [@AmrDeveloper](https://github.com/AmrDeveloper) Did you try it with a release build with asserts disabled?
> 
> [@HendrikHuebner](https://github.com/HendrikHuebner) We're missing a bit of code in the codegen handling. It's disappointing that we didn't report a diagnostic before hitting the assertion. I am also hitting an assertion with your reproducer, but I get a different callstack (so a different assertion). This may just be because I'm at a different point in the revision history than you, or it may be because you are building differently (release with asserts?). In my sandbox, the assertion is happening inside `clang::CIRGen::Address::getPointer()` called from `CIRGenFunction::CXXDefaultInitExprScope ::CXXDefaultInitExprScope()` and appears to be happening because we failed to initialize `cgf.cxxDefaultInitExprThis`.
> 
> I have a simple fix for the problem.

Sorry, I was in a debug build with assert enabled

---

### Comment 5 - andykaylor

> Sorry, I was in a debug build with assert enabled

That's odd. I was also using a debug build with asserts enabled, and I hit an assertion. Did you compile it as C or C++?

---

### Comment 6 - AmrDeveloper

> > Sorry, I was in a debug build with assert enabled
> 
> That's odd. I was also using a debug build with asserts enabled, and I hit an assertion. Did you compile it as C or C++?

I am using C++ with or without triple, it works 🤔 

```
./bin/clang++ -fclangir -emit-cir main.cpp -o main.cir
./bin/clang -cc1 -std=c++20 -triple x86_64-unknown-linux-gnu -fclangir -emit-cir main.cpp -o main.cir
```

---

### Comment 7 - AmrDeveloper

I merged this PR today: https://github.com/llvm/llvm-project/pull/165994 :D

Can you please rebase with the current master, because `cxxDefaultInitExprThis` will be initialized

---

### Comment 8 - andykaylor

> I merged this PR today: [#165994](https://github.com/llvm/llvm-project/pull/165994) :D

Yes, that was the change that fixed it for me. I found that code was missing in my sandbox, but I failed to make the connection with your PR.

---

### Comment 9 - HendrikHuebner

This commit fixed it for me too. I'll close the issue. THanks!

---

