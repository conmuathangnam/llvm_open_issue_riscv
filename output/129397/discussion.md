# ICE with clang-21 HEAD

**Author:** jcelerier
**URL:** https://github.com/llvm/llvm-project/issues/129397
**Status:** Closed
**Labels:** clang:frontend, crash, c++23, regression:20
**Closed Date:** 2025-05-02T16:45:26Z

## Body

- commit: b65e0947cade9bd39036a7700b54c1df4ec00756
- os: arch linux (x86_64)

- llvm build options: I used https://github.com/mengkernel/clang-build like this: 
```
$ python build-llvm.py -t X86 -D LLVM_ENABLE_UNWIND_TABLES=ON LLVM_ENABLE_EH=ON LLVM_ENABLE_RTTI=ON LLVM_ENABLE_WARNINGS=OFF LLVM_INCLUDE_EXAMPLES=0 LLVM_INCLUDE_TESTS=0  LLVM_APPEND_VC_REV=OFF LLVM_TARGETS_TO_BUILD="X86" LLVM_ENABLE_BINDINGS=0 LLVM_INCLUDE_BENCHMARKS=0 LLVM_ENABLE_LIBEDIT=0  LLVM_ENABLE_LIBXML2=0 LLVM_ENABLE_LIBPFM=0 LLVM_ENABLE_HTTPLIB=0   LLVM_ENABLE_CURSES=0 LLVM_ENABLE_TERMINFO=0
```

repro source files: 

[repro.tar.gz](https://github.com/user-attachments/files/19040442/repro.tar.gz)

same code works fine with current clang stable release on every platform

A quick inspection hints to something in https://github.com/ned14/llfio causing the ICE

## Comments

### Comment 1 - Endilll

This seems to be a regression in C++23 mode introduced after Clang 19.1: https://godbolt.org/z/bbfn5o4bd
Reduced by C-Reduce:
```cpp
extern "C" void *memcpy(void *, const void *, unsigned long);

struct status_code;

void _do_erased_copy(status_code &dst) {
  memcpy(&dst, 0, 0);
}
```
Clang 21.0 (trunk) crash:
```
clang++: /root/llvm-project/clang/lib/AST/Type.cpp:2387:
bool clang::Type::isConstantSizeType() const:
Assertion `!isIncompleteType() && "This doesn't make sense for incomplete types"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:6:20: current parser token ')'
2.	<source>:5:40: parsing function body '_do_erased_copy'
3.	<source>:5:40: in compound statement ('{}')
 #0 0x0000000003e6b938 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6b938)
 #1 0x0000000003e695f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e695f4)
 #2 0x0000000003db5f08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076326f642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076326f6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076326f642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076326f6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076326f62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076326f639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007a8ae2f clang::Type::isConstantSizeType() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a8ae2f)
#10 0x000000000766f6ac HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#11 0x00000000076b474a tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long&) ExprConstant.cpp:0:0
#12 0x00000000076b5333 clang::Expr::tryEvaluateObjectSize(unsigned long&, clang::ASTContext&, unsigned int) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76b5333)
#13 0x000000000665c952 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda1'(unsigned int)::operator()(unsigned int) const SemaChecking.cpp:0:0
#14 0x0000000006667a6a clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6667a6a)
#15 0x00000000069656bb clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69656bb)
#16 0x0000000006d525ba FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#17 0x0000000006d534dc clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d534dc)
#18 0x0000000006967156 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6967156)
#19 0x0000000006967fbc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6967fbc)
#20 0x00000000064a63b2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a63b2)
#21 0x000000000649ecc1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x649ecc1)
#22 0x00000000064a0e27 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a0e27)
#23 0x00000000064a0eb9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a0eb9)
#24 0x00000000064a58f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a58f9)
#25 0x0000000006529629 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6529629)
#26 0x000000000651f98e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651f98e)
#27 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65208e0)
#28 0x00000000065217d1 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65217d1)
#29 0x0000000006522fba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6522fba)
#30 0x00000000064347e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64347e3)
#31 0x000000000646959d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x646959d)
#32 0x000000000642851e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642851e)
#33 0x0000000006428cd9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6428cd9)
#34 0x00000000064304b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64304b3)
#35 0x000000000643138d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643138d)
#36 0x000000000642381a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642381a)
#37 0x000000000482f028 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482f028)
#38 0x0000000004af7965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af7965)
#39 0x0000000004a7b13e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7b13e)
#40 0x0000000004be662e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be662e)
#41 0x0000000000d522e7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd522e7)
#42 0x0000000000d490aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#43 0x0000000004877429 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000003db63b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db63b4)
#45 0x0000000004877a1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x000000000483a1fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483a1fd)
#47 0x000000000483b27e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483b27e)
#48 0x0000000004843195 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4843195)
#49 0x0000000000d4e583 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4e583)
#50 0x0000000000c16164 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc16164)
#51 0x000076326f629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x000076326f629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000d48b55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd48b55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 2 - Endilll

Worth noting that this same reduction segfaults with a different stack trace on my local build:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/user/endill/ramdisk/llvm-build/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -std=c++23 -fcxx-exceptions -fexceptions -fcolor-diagnostics -x c++ pp.ii
1.      pp.ii:4:20: current parser token ')'
2.      pp.ii:3:46: parsing function body '_do_erased_copy'
3.      pp.ii:3:46: in compound statement ('{}')
 #0 0x00007f46a8fe4e68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/user/endill/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:13
 #1 0x00007f46a8fe2b92 llvm::sys::RunSignalHandlers() /home/user/endill/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #2 0x00007f46a8fe55b1 SignalHandler(int, siginfo_t*, void*) /home/user/endill/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #3 0x00007f46a8a49da0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fda0)
 #4 0x00007f46a7a98de4 clang::Decl::getKind() const /home/user/endill/llvm-project/clang/include/clang/AST/DeclBase.h:445:51
 #5 0x00007f46a7a98de4 clang::CXXRecordDecl::classof(clang::Decl const*) /home/user/endill/llvm-project/clang/include/clang/AST/DeclCXX.h:1895:62
 #6 0x00007f46a7a98de4 llvm::isa_impl<clang::CXXRecordDecl, clang::RecordDecl, void>::doit(clang::RecordDecl const&) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
 #7 0x00007f46a7a98de4 llvm::isa_impl_cl<clang::CXXRecordDecl, clang::RecordDecl const*>::doit(clang::RecordDecl const*) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
 #8 0x00007f46a7a98de4 llvm::isa_impl_wrap<clang::CXXRecordDecl, clang::RecordDecl const*, clang::RecordDecl const*>::doit(clang::RecordDecl const* const&) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
 #9 0x00007f46a7a98de4 llvm::isa_impl_wrap<clang::CXXRecordDecl, clang::RecordDecl const* const, clang::RecordDecl const*>::doit(clang::RecordDecl const* const&) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#10 0x00007f46a7a98de4 llvm::CastIsPossible<clang::CXXRecordDecl, clang::RecordDecl const*, void>::isPossible(clang::RecordDecl const* const&) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#11 0x00007f46a7a98de4 llvm::CastInfo<clang::CXXRecordDecl, clang::RecordDecl const*, void>::doCastIfPossible(clang::RecordDecl const* const&) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
#12 0x00007f46a7a98de4 decltype(auto) llvm::dyn_cast<clang::CXXRecordDecl, clang::RecordDecl const>(clang::RecordDecl const*) /home/user/endill/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
#13 0x00007f46a7a98de4 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /home/user/endill/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:0:0
#14 0x00007f46a7546496 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:2387:37
#15 0x00007f46a7546732 clang::ASTContext::getTypeInfo(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:1948:17
#16 0x00007f46a7546732 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:0:0
#17 0x00007f46a7547b0f clang::ASTContext::getTypeInfo(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:1948:17
#18 0x00007f46a7546403 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:0:0
#19 0x00007f46a7547881 clang::ASTContext::getTypeInfo(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:1948:17
#20 0x00007f46a7547881 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:1862:19
#21 0x00007f46a7548779 clang::ASTContext::getTypeSizeInChars(clang::QualType) const /home/user/endill/llvm-project/clang/lib/AST/ASTContext.cpp:2531:10
#22 0x00007f46a77f78c0 HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) /home/user/endill/llvm-project/clang/lib/AST/ExprConstant.cpp:0:0
#23 0x00007f46a77e891d determineEndOffset((anonymous namespace)::EvalInfo&, clang::SourceLocation, unsigned int, (anonymous namespace)::LValue const&, clang::CharUnits&)::$_0::operator()(clang::QualType, clang::CharUnits&) const /home/user/endill/llvm-project/clang/lib/AST/ExprConstant.cpp:12709:12
#24 0x00007f46a77e891d determineEndOffset((anonymous namespace)::EvalInfo&, clang::SourceLocation, unsigned int, (anonymous namespace)::LValue const&, clang::CharUnits&) /home/user/endill/llvm-project/clang/lib/AST/ExprConstant.cpp:12729:22
#25 0x00007f46a77e891d tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long&) /home/user/endill/llvm-project/clang/lib/AST/ExprConstant.cpp:12816:8
#26 0x00007f46a77e825b clang::Expr::tryEvaluateObjectSize(unsigned long&, clang::ASTContext&, unsigned int) const /home/user/endill/llvm-project/clang/lib/AST/ExprConstant.cpp:17843:10
#27 0x00007f46a6393243 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::$_2::operator()(unsigned int) const /home/user/endill/llvm-project/clang/lib/Sema/SemaChecking.cpp:1211:9
#28 0x00007f46a63925f0 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) /home/user/endill/llvm-project/clang/lib/Sema/SemaChecking.cpp:0:23
#29 0x00007f46a659c7cb clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) /home/user/endill/llvm-project/clang/lib/Sema/SemaExpr.cpp:6796:7
#30 0x00007f46a687c0b0 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) /home/user/endill/llvm-project/clang/lib/Sema/SemaOverload.cpp:14189:20
#31 0x00007f46a687be41 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/user/endill/llvm-project/clang/lib/Sema/SemaOverload.cpp:14332:10
#32 0x00007f46a65858f5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/user/endill/llvm-project/clang/lib/Sema/SemaExpr.cpp:6581:16
#33 0x00007f46a659a65b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /home/user/endill/llvm-project/clang/lib/Sema/SemaExpr.cpp:6461:7
#34 0x00007f46a6d3a947 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) /home/user/endill/llvm-project/clang/lib/Parse/ParseExpr.cpp:2269:23
#35 0x00007f46a6d3c444 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /home/user/endill/llvm-project/clang/lib/Parse/ParseExpr.cpp:1962:7
#36 0x00007f46a6d38a18 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /home/user/endill/llvm-project/clang/lib/Parse/ParseExpr.cpp:729:20
#37 0x00007f46a6d38a18 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /home/user/endill/llvm-project/clang/lib/Parse/ParseExpr.cpp:184:20
#38 0x00007f46a6d38949 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /home/user/endill/llvm-project/clang/lib/Parse/ParseExpr.cpp:136:10
#39 0x00007f46a6d8cfc6 clang::ActionResult<clang::Expr*, true>::isInvalid() const /home/user/endill/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#40 0x00007f46a6d8cfc6 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /home/user/endill/llvm-project/clang/lib/Parse/ParseStmt.cpp:565:12
#41 0x00007f46a6d8b6f3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /home/user/endill/llvm-project/clang/lib/Parse/ParseStmt.cpp:315:5
#42 0x00007f46a6d8b0ab clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /home/user/endill/llvm-project/clang/lib/Parse/ParseStmt.cpp:125:20
#43 0x00007f46a6d92756 clang::Parser::ParseCompoundStatementBody(bool) /home/user/endill/llvm-project/clang/lib/Parse/ParseStmt.cpp:1267:11
#44 0x00007f46a6d9336a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /home/user/endill/llvm-project/clang/lib/Parse/ParseStmt.cpp:2577:21
#45 0x00007f46a6da8eb8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#46 0x00007f46a6d0dea4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/user/endill/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:17
#47 0x00007f46a6da833e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:0:10
#48 0x00007f46a6da7d74 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:1266:12
#49 0x00007f46a6da71e6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:0:14
#50 0x00007f46a6da5d37 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:758:10
#51 0x00007f46a6cf941e clang::ParseAST(clang::Sema&, bool, bool) /home/user/endill/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#52 0x00007f46aaac311d clang::FrontendAction::Execute() /home/user/endill/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:10
#53 0x00007f46aaa41774 llvm::Error::getPtr() const /home/user/endill/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#54 0x00007f46aaa41774 llvm::Error::operator bool() /home/user/endill/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#55 0x00007f46aaa41774 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/user/endill/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:23
#56 0x00007f46ac032572 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/user/endill/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#57 0x000056179009a84a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/user/endill/llvm-project/clang/tools/driver/cc1_main.cpp:290:15
#58 0x0000561790097b42 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/user/endill/llvm-project/clang/tools/driver/driver.cpp:218:12
#59 0x0000561790097055 clang_main(int, char**, llvm::ToolContext const&) /home/user/endill/llvm-project/clang/tools/driver/driver.cpp:259:12
#60 0x00005617900a32f7 main /home/user/endill/ramdisk/llvm-build/tools/clang/tools/driver/clang-driver.cpp:17:10
#61 0x00007f46a8a33d68 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#62 0x00007f46a8a33e25 call_init ./csu/../csu/libc-start.c:128:20
#63 0x00007f46a8a33e25 __libc_start_main ./csu/../csu/libc-start.c:347:5
#64 0x0000561790095b61 _start (/home/user/endill/ramdisk/llvm-build/bin/clang+0xab61)
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jean-Michaël Celerier (jcelerier)

<details>
- commit: b65e0947cade9bd39036a7700b54c1df4ec00756
- os: arch linux (x86_64)

- llvm build options: I used https://github.com/mengkernel/clang-build like this: 
```
$ python build-llvm.py -t X86 -D LLVM_ENABLE_UNWIND_TABLES=ON LLVM_ENABLE_EH=ON LLVM_ENABLE_RTTI=ON LLVM_ENABLE_WARNINGS=OFF LLVM_INCLUDE_EXAMPLES=0 LLVM_INCLUDE_TESTS=0  LLVM_APPEND_VC_REV=OFF LLVM_TARGETS_TO_BUILD="X86" LLVM_ENABLE_BINDINGS=0 LLVM_INCLUDE_BENCHMARKS=0 LLVM_ENABLE_LIBEDIT=0  LLVM_ENABLE_LIBXML2=0 LLVM_ENABLE_LIBPFM=0 LLVM_ENABLE_HTTPLIB=0   LLVM_ENABLE_CURSES=0 LLVM_ENABLE_TERMINFO=0
```

repro source files: 

[repro.tar.gz](https://github.com/user-attachments/files/19040442/repro.tar.gz)

same code works fine with current clang stable release on every platform

A quick inspection hints to something in https://github.com/ned14/llfio causing the ICE
</details>


---

