# [HLSL][Matrix] Explicit cast of boolean matrix to boolean vector results in assertion

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/177712
**Status:** Closed
**Labels:** clang:codegen, crash, HLSL
**Closed Date:** 2026-02-23T23:45:36Z

## Body

After allowing matrix types to be flattened with PR #177708 I have found an issue where casting a boolean matrix to a boolean vector results in an assertion error.

Repro: `clang-dxc -T lib_6_8 test.hlsl`
```hlsl
export bool2 fn3(bool1x2 M) {
    bool2 V = (bool2)M;
    return V;
}
```

Log:
```
clang-dxc: /workspace/llvm-project/clang/include/clang/AST/TypeBase.h:9183: const T *clang::Type::castAs() const [T = clang::ConstantMatrixType]: Assertion `isa<T>(CanonicalType)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/llvm-project/build/bin/clang-dxc -T lib_6_8 ./test.hlsl
1.      <eof> parser at end of file
2.      ./test.hlsl:1:1: LLVM IR generation of declaration
3.      ./test.hlsl:1:14: Generating code for declaration 'fn3'
 #0 0x00005ef52dddad4b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:13
 #1 0x00005ef52ddd86f3 llvm::sys::RunSignalHandlers() /workspace/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x00005ef52dd3f610 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00005ef52dd3f610 CrashRecoverySignalHandler(int) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x000074e7692419c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #5 0x000074e76929caac __pthread_kill_implementation (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x9caac)
 #6 0x000074e76924190e gsignal (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x4190e)
 #7 0x000074e769228942 abort (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x28942)
 #8 0x000074e76922885e _nl_load_domain.cold (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2885e)
 #9 0x000074e7692396f7 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x396f7)
#10 0x00005ef52e1f9323 decltype(auto) llvm::cast<clang::ConstantMatrixType, clang::Type const>(clang::Type const*) /workspace/llvm-project/llvm/include/llvm/Support/Casting.h:572:3
#11 0x00005ef52e1f9323 clang::ConstantMatrixType const* clang::Type::castAs<clang::ConstantMatrixType>() const /workspace/llvm-project/clang/include/clang/AST/TypeBase.h:9184:10
#12 0x00005ef52e1f5d9f clang::CodeGen::CodeGenFunction::EmitLoadOfLValue(clang::CodeGen::LValue, clang::SourceLocation) /workspace/llvm-project/clang/lib/CodeGen/CGExpr.cpp:2466:30
#13 0x00005ef52e22fb7f clang::CodeGen::RValue::isScalar() const /workspace/llvm-project/clang/lib/CodeGen/CGValue.h:64:41
#14 0x00005ef52e22fb7f EmitHLSLElementwiseCast(clang::CodeGen::CodeGenFunction&, clang::CodeGen::LValue, clang::QualType, clang::SourceLocation) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:2467:7
#15 0x00005ef52e22e794 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:3068:12
#16 0x00005ef52e21178c (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#17 0x00005ef52e21178c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5975:8
#18 0x00005ef52e269161 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:796:15
#19 0x00005ef52e26dabd clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:0:12
#20 0x00005ef52e266dce clang::CodeGen::CodeGenFunction::EmitAutoVarDecl(clang::VarDecl const&) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:1351:3
#21 0x00005ef52e266dce clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:230:10
#22 0x00005ef52e266812 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:172:9
#23 0x00005ef52e180fe0 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1738:22
#24 0x00005ef52e17600f clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:0:5
#25 0x00005ef52e17550f clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:66:7
#26 0x00005ef52e1821c1 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:591:10
#27 0x00005ef52e15ecc4 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1379:1
#28 0x00005ef52e15fec4 clang::CodeGen::CodeGenFunction::getLangOpts() const /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:2181:51
#29 0x00005ef52e15fec4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1638:7
#30 0x00005ef52e01da07 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6599:3
#31 0x00005ef52e01553b llvm::TimeTraceScope::~TimeTraceScope() /workspace/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#32 0x00005ef52e01553b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4652:3
#33 0x00005ef52e01a3a1 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4363:5
#34 0x00005ef52e013e35 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7510:5
#35 0x00005ef52e02591b memcpy /nix/store/gi4cz4ir3zlwhf1azqfgxqdnczfrwsr7-glibc-2.40-66-dev/include/bits/string_fortified.h:29:10
#36 0x00005ef52e02591b llvm::detail::PunnedPointer<clang::Decl*>::asInt() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#37 0x00005ef52e02591b llvm::detail::PunnedPointer<clang::Decl*>::operator long() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#38 0x00005ef52e02591b llvm::PointerIntPair<clang::Decl*, 3u, clang::Decl::ModuleOwnershipKind, llvm::PointerLikeTypeTraits<clang::Decl*>, llvm::PointerIntPairInfo<clang::Decl*, 3u, llvm::PointerLikeTypeTraits<clang::Decl*>>>::getPointer() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#39 0x00005ef52e02591b clang::Decl::getNextDeclInContext() /workspace/llvm-project/clang/include/clang/AST/DeclBase.h:445:62
#40 0x00005ef52e02591b clang::DeclContext::decl_iterator::operator++() /workspace/llvm-project/clang/include/clang/AST/DeclBase.h:2350:26
#41 0x00005ef52e02591b clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7478:16
#42 0x00005ef52e57a85c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /workspace/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:194:20
#43 0x00005ef52e5712d5 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /workspace/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:171:22
#44 0x00005ef5300e7879 clang::ParseAST(clang::Sema&, bool, bool) /workspace/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#45 0x00005ef52eac6b8f clang::HLSLFrontendAction::ExecuteAction() /workspace/llvm-project/clang/lib/Frontend/FrontendActions.cpp:0:35
#46 0x00005ef52ea90087 clang::FrontendAction::Execute() /workspace/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:10
#47 0x00005ef52e9f9cad llvm::Error::getPtr() const /workspace/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#48 0x00005ef52e9f9cad llvm::Error::operator bool() /workspace/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#49 0x00005ef52e9f9cad clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /workspace/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#50 0x00005ef52eb863d2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /workspace/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#51 0x00005ef52c88ec1a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /workspace/llvm-project/clang/tools/driver/cc1_main.cpp:304:15
#52 0x00005ef52c88b022 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /workspace/llvm-project/clang/tools/driver/driver.cpp:226:12
#53 0x00005ef52c88d3ec clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /workspace/llvm-project/clang/tools/driver/driver.cpp:0:12
#54 0x00005ef52c88d3ec int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#55 0x00005ef52e8668de clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /workspace/llvm-project/clang/lib/Driver/Job.cpp:442:30
#56 0x00005ef52e8668de void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#57 0x00005ef52dd3f2a9 llvm::function_ref<void ()>::operator()() const /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#58 0x00005ef52dd3f2a9 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#59 0x00005ef52e865eb9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /workspace/llvm-project/clang/lib/Driver/Job.cpp:442:7
#60 0x00005ef52e826e86 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#61 0x00005ef52e8270f8 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#62 0x00005ef52e842e89 llvm::SmallVectorBase<unsigned int>::empty() const /workspace/llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#63 0x00005ef52e842e89 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /workspace/llvm-project/clang/lib/Driver/Driver.cpp:2265:23
#64 0x00005ef52c88a9a3 clang_main(int, char**, llvm::ToolContext const&) /workspace/llvm-project/clang/tools/driver/driver.cpp:414:21
#65 0x00005ef52c89ad8f main /workspace/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#66 0x000074e76922a4d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#67 0x000074e76922a59b __libc_start_main@GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#68 0x00005ef52c888925 _start (/workspace/llvm-project/build/bin/clang-dxc+0x2f35925)
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (git@github.com:Icohedron/llvm-project.git 21287b87994c3dce5e1615c9bace88d819526324)
Target: dxilv1.8-unknown-shadermodel6.8-library
Thread model: posix
InstalledDir: /workspace/llvm-project/build/bin
Build config: +assertions
clang-dxc: note: diagnostic msg:
********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Deric C. (Icohedron)

<details>
After allowing matrix types to be flattened with PR #<!-- -->177708 I have found an issue where casting a boolean matrix to a boolean vector results in an assertion error.

Repro: `clang-dxc -T lib_6_8 test.hlsl`
```hlsl
export bool2 fn3(bool1x2 M) {
    bool2 V = (bool2)M;
    return V;
}
```

Log:
```
clang-dxc: /workspace/llvm-project/clang/include/clang/AST/TypeBase.h:9183: const T *clang::Type::castAs() const [T = clang::ConstantMatrixType]: Assertion `isa&lt;T&gt;(CanonicalType)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/llvm-project/build/bin/clang-dxc -T lib_6_8 ./test.hlsl
1.      &lt;eof&gt; parser at end of file
2.      ./test.hlsl:1:1: LLVM IR generation of declaration
3.      ./test.hlsl:1:14: Generating code for declaration 'fn3'
 #<!-- -->0 0x00005ef52dddad4b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:13
 #<!-- -->1 0x00005ef52ddd86f3 llvm::sys::RunSignalHandlers() /workspace/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->2 0x00005ef52dd3f610 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00005ef52dd3f610 CrashRecoverySignalHandler(int) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #<!-- -->4 0x000074e7692419c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #<!-- -->5 0x000074e76929caac __pthread_kill_implementation (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x9caac)
 #<!-- -->6 0x000074e76924190e gsignal (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x4190e)
 #<!-- -->7 0x000074e769228942 abort (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x28942)
 #<!-- -->8 0x000074e76922885e _nl_load_domain.cold (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2885e)
 #<!-- -->9 0x000074e7692396f7 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x396f7)
#<!-- -->10 0x00005ef52e1f9323 decltype(auto) llvm::cast&lt;clang::ConstantMatrixType, clang::Type const&gt;(clang::Type const*) /workspace/llvm-project/llvm/include/llvm/Support/Casting.h:572:3
#<!-- -->11 0x00005ef52e1f9323 clang::ConstantMatrixType const* clang::Type::castAs&lt;clang::ConstantMatrixType&gt;() const /workspace/llvm-project/clang/include/clang/AST/TypeBase.h:9184:10
#<!-- -->12 0x00005ef52e1f5d9f clang::CodeGen::CodeGenFunction::EmitLoadOfLValue(clang::CodeGen::LValue, clang::SourceLocation) /workspace/llvm-project/clang/lib/CodeGen/CGExpr.cpp:2466:30
#<!-- -->13 0x00005ef52e22fb7f clang::CodeGen::RValue::isScalar() const /workspace/llvm-project/clang/lib/CodeGen/CGValue.h:64:41
#<!-- -->14 0x00005ef52e22fb7f EmitHLSLElementwiseCast(clang::CodeGen::CodeGenFunction&amp;, clang::CodeGen::LValue, clang::QualType, clang::SourceLocation) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:2467:7
#<!-- -->15 0x00005ef52e22e794 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:3068:12
#<!-- -->16 0x00005ef52e21178c (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#<!-- -->17 0x00005ef52e21178c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /workspace/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5975:8
#<!-- -->18 0x00005ef52e269161 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:796:15
#<!-- -->19 0x00005ef52e26dabd clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:0:12
#<!-- -->20 0x00005ef52e266dce clang::CodeGen::CodeGenFunction::EmitAutoVarDecl(clang::VarDecl const&amp;) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:1351:3
#<!-- -->21 0x00005ef52e266dce clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:230:10
#<!-- -->22 0x00005ef52e266812 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) /workspace/llvm-project/clang/lib/CodeGen/CGDecl.cpp:172:9
#<!-- -->23 0x00005ef52e180fe0 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1738:22
#<!-- -->24 0x00005ef52e17600f clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:0:5
#<!-- -->25 0x00005ef52e17550f clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:66:7
#<!-- -->26 0x00005ef52e1821c1 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) /workspace/llvm-project/clang/lib/CodeGen/CGStmt.cpp:591:10
#<!-- -->27 0x00005ef52e15ecc4 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1379:1
#<!-- -->28 0x00005ef52e15fec4 clang::CodeGen::CodeGenFunction::getLangOpts() const /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:2181:51
#<!-- -->29 0x00005ef52e15fec4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) /workspace/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1638:7
#<!-- -->30 0x00005ef52e01da07 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6599:3
#<!-- -->31 0x00005ef52e01553b llvm::TimeTraceScope::~TimeTraceScope() /workspace/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#<!-- -->32 0x00005ef52e01553b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4652:3
#<!-- -->33 0x00005ef52e01a3a1 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4363:5
#<!-- -->34 0x00005ef52e013e35 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7510:5
#<!-- -->35 0x00005ef52e02591b memcpy /nix/store/gi4cz4ir3zlwhf1azqfgxqdnczfrwsr7-glibc-2.40-66-dev/include/bits/string_fortified.h:29:10
#<!-- -->36 0x00005ef52e02591b llvm::detail::PunnedPointer&lt;clang::Decl*&gt;::asInt() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->37 0x00005ef52e02591b llvm::detail::PunnedPointer&lt;clang::Decl*&gt;::operator long() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->38 0x00005ef52e02591b llvm::PointerIntPair&lt;clang::Decl*, 3u, clang::Decl::ModuleOwnershipKind, llvm::PointerLikeTypeTraits&lt;clang::Decl*&gt;, llvm::PointerIntPairInfo&lt;clang::Decl*, 3u, llvm::PointerLikeTypeTraits&lt;clang::Decl*&gt;&gt;&gt;::getPointer() const /workspace/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->39 0x00005ef52e02591b clang::Decl::getNextDeclInContext() /workspace/llvm-project/clang/include/clang/AST/DeclBase.h:445:62
#<!-- -->40 0x00005ef52e02591b clang::DeclContext::decl_iterator::operator++() /workspace/llvm-project/clang/include/clang/AST/DeclBase.h:2350:26
#<!-- -->41 0x00005ef52e02591b clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) /workspace/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7478:16
#<!-- -->42 0x00005ef52e57a85c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /workspace/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:194:20
#<!-- -->43 0x00005ef52e5712d5 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /workspace/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:171:22
#<!-- -->44 0x00005ef5300e7879 clang::ParseAST(clang::Sema&amp;, bool, bool) /workspace/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#<!-- -->45 0x00005ef52eac6b8f clang::HLSLFrontendAction::ExecuteAction() /workspace/llvm-project/clang/lib/Frontend/FrontendActions.cpp:0:35
#<!-- -->46 0x00005ef52ea90087 clang::FrontendAction::Execute() /workspace/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:10
#<!-- -->47 0x00005ef52e9f9cad llvm::Error::getPtr() const /workspace/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#<!-- -->48 0x00005ef52e9f9cad llvm::Error::operator bool() /workspace/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#<!-- -->49 0x00005ef52e9f9cad clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /workspace/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#<!-- -->50 0x00005ef52eb863d2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /workspace/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#<!-- -->51 0x00005ef52c88ec1a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /workspace/llvm-project/clang/tools/driver/cc1_main.cpp:304:15
#<!-- -->52 0x00005ef52c88b022 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /workspace/llvm-project/clang/tools/driver/driver.cpp:226:12
#<!-- -->53 0x00005ef52c88d3ec clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /workspace/llvm-project/clang/tools/driver/driver.cpp:0:12
#<!-- -->54 0x00005ef52c88d3ec int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->55 0x00005ef52e8668de clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const /workspace/llvm-project/clang/lib/Driver/Job.cpp:442:30
#<!-- -->56 0x00005ef52e8668de void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->57 0x00005ef52dd3f2a9 llvm::function_ref&lt;void ()&gt;::operator()() const /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->58 0x00005ef52dd3f2a9 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#<!-- -->59 0x00005ef52e865eb9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /workspace/llvm-project/clang/lib/Driver/Job.cpp:442:7
#<!-- -->60 0x00005ef52e826e86 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#<!-- -->61 0x00005ef52e8270f8 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#<!-- -->62 0x00005ef52e842e89 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /workspace/llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#<!-- -->63 0x00005ef52e842e89 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /workspace/llvm-project/clang/lib/Driver/Driver.cpp:2265:23
#<!-- -->64 0x00005ef52c88a9a3 clang_main(int, char**, llvm::ToolContext const&amp;) /workspace/llvm-project/clang/tools/driver/driver.cpp:414:21
#<!-- -->65 0x00005ef52c89ad8f main /workspace/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#<!-- -->66 0x000074e76922a4d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#<!-- -->67 0x000074e76922a59b __libc_start_main@<!-- -->GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#<!-- -->68 0x00005ef52c888925 _start (/workspace/llvm-project/build/bin/clang-dxc+0x2f35925)
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (git@<!-- -->github.com:Icohedron/llvm-project.git 21287b87994c3dce5e1615c9bace88d819526324)
Target: dxilv1.8-unknown-shadermodel6.8-library
Thread model: posix
InstalledDir: /workspace/llvm-project/build/bin
Build config: +assertions
clang-dxc: note: diagnostic msg:
********************
```
</details>


---

