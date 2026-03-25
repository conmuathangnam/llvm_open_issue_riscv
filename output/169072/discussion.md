# templated attribute((constructor)) function crashes clang

**Author:** daltenty
**URL:** https://github.com/llvm/llvm-project/issues/169072
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-11-28T07:26:44Z

## Body

Since a205695de4dce3c839dedbb78dd67e2a7758947a landed, the following templated `attribute((constructor))` hello world test case will crash clang:
```
extern "C" int printf(const char *, ...);

template <typename C, void (C::*F)(void) const>
__attribute__((constructor)) void baz() { (C().*F)(); }

struct B {
  B() {
   struct A {
      void foo() const { printf("hello world!\n"); }
      void bar() const {
         ::baz<A, &A::foo>();
      }
   };
  }
};

int main() {
  return 66;
}
```

Expected:
```
$ /home/daltenty/llvm/dev/build/bin/clang++ test.cc
$ ./a.out
hello world!
$ echo $?
66
```
Actual:
```
/home/daltenty/llvm/dev/build/bin/clang++ test.cc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/daltenty/llvm/dev/build/bin/clang-22 -cc1 -triple powerpc64le-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cc -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu ppc64le -mfloat-abi hard -target-abi elfv2 -debugger-tuning=gdb -fdebug-compilation-dir=/home/daltenty/llvm/dev/build -fcoverage-compilation-dir=/home/daltenty/llvm/dev/build -resource-dir /home/daltenty/llvm/dev/build/lib/clang/22 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/powerpc64le-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/daltenty/llvm/dev/build/lib/clang/22/include/ppc_wrappers -internal-isystem /home/daltenty/llvm/dev/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../powerpc64le-linux-gnu/include -internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=214 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-eaadd7.o -x c++ /home/daltenty/test/test.cc
1.      /home/daltenty/test/test.cc:11:28: current parser token ')'
2.      /home/daltenty/test/test.cc:6:1: parsing struct/union/class body 'B'
3.      /home/daltenty/test/test.cc:7:7: parsing function body 'B::B'
4.      /home/daltenty/test/test.cc:7:7: in compound statement ('{}')
5.      /home/daltenty/test/test.cc:8:4: parsing struct/union/class body 'A'
6.      /home/daltenty/test/test.cc:10:24: parsing function body 'B::B()::A::bar'
7.      /home/daltenty/test/test.cc:10:24: in compound statement ('{}')
 #0 0x0000108d487b2a6c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/daltenty/llvm/dev/build/bin/clang-22+0x4d32a6c)
 #1 0x0000108d487b3214 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x0000108d487af5ec llvm::sys::RunSignalHandlers() (/home/daltenty/llvm/dev/build/bin/clang-22+0x4d2f5ec)
 #3 0x0000108d487b3e7c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00006a21d9360444 (linux-vdso64.so.1+0x444)
 #5 0x0000108d4c39a954 clang::Expr::getIntegerConstantExpr(clang::ASTContext const&) const (/home/daltenty/llvm/dev/build/bin/clang-22+0x891a954)
 #6 0x0000108d4bc40294 clang::Sema::InstantiateAttrs(clang::MultiLevelTemplateArgumentList const&, clang::Decl const*, clang::Decl*, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::LocalInstantiationScope*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81c0294)
 #7 0x0000108d4bc517a0 clang::TemplateDeclInstantiator::InitFunctionInstantiation(clang::FunctionDecl*, clang::FunctionDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d17a0)
 #8 0x0000108d4bc4f0f8 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81cf0f8)
 #9 0x0000108d4bc57850 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d7850)
#10 0x0000108d4bbec06c clang::declvisitor::Base<std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000108d4bca521c void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(long) SemaTemplateInstantiateDecl.cpp:0:0
#12 0x0000108d49309c18 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5889c18)
#13 0x0000108d4b305350 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7885350)
#14 0x0000108d4bc58b68 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d8b68)
#15 0x0000108d4bb33ba4 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x80b3ba4)
#16 0x0000108d4bb94090 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2>(long) SemaTemplateDeduction.cpp:0:0
#17 0x0000108d49309c18 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5889c18)
#18 0x0000108d4b305350 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7885350)
#19 0x0000108d4bb374a8 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x80b74a8)
#20 0x0000108d4b9ec724 AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#21 0x0000108d4b9eb9ec clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f6b9ec)
#22 0x0000108d4ba03294 AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#23 0x0000108d4ba03064 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f83064)
#24 0x0000108d4ba036e4 clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f836e4)
#25 0x0000108d4ba03c10 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f83c10)
#26 0x0000108d4b642984 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7bc2984)
#27 0x0000108d4b65cb80 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7bdcb80)
#28 0x0000108d4b191af0 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7711af0)
#29 0x0000108d4b193764 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7713764)
#30 0x0000108d4b18ed24 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/home/daltenty/llvm/dev/build/bin/clang-22+0x770ed24)
#31 0x0000108d4b18ebc0 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/home/daltenty/llvm/dev/build/bin/clang-22+0x770ebc0)
#32 0x0000108d4b22f048 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77af048)
#33 0x0000108d4b22c9e8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77ac9e8)
#34 0x0000108d4b22bed0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77abed0)
#35 0x0000108d4b237238 clang::Parser::ParseCompoundStatementBody(bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b7238)
#36 0x0000108d4b2384c8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b84c8)
#37 0x0000108d4b267cb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7cb0)
#38 0x0000108d4b267970 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7970)
#39 0x0000108d4b265ee4 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e5ee4)
#40 0x0000108d4b1c3ffc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7743ffc)
#41 0x0000108d4b1c106c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x774106c)
#42 0x0000108d4b1ec5fc clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776c5fc)
#43 0x0000108d4b1e610c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776610c)
#44 0x0000108d4b1e5b90 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7765b90)
#45 0x0000108d4b22e48c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77ae48c)
#46 0x0000108d4b22bed0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77abed0)
#47 0x0000108d4b237238 clang::Parser::ParseCompoundStatementBody(bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b7238)
#48 0x0000108d4b2384c8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b84c8)
#49 0x0000108d4b267cb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7cb0)
#50 0x0000108d4b267970 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7970)
#51 0x0000108d4b265ee4 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e5ee4)
#52 0x0000108d4b1c3ffc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7743ffc)
#53 0x0000108d4b1c106c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x774106c)
#54 0x0000108d4b1ec5fc clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776c5fc)
#55 0x0000108d4b1699a4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e99a4)
#56 0x0000108d4b1694f4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e94f4)
#57 0x0000108d4b1684d4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e84d4)
#58 0x0000108d4b16604c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e604c)
#59 0x0000108d4b158b40 clang::ParseAST(clang::Sema&, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76d8b40)
#60 0x0000108d496ccc0c clang::ASTFrontendAction::ExecuteAction() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5c4cc0c)
#61 0x0000108d49095430 clang::CodeGenAction::ExecuteAction() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5615430)
#62 0x0000108d496cc0b0 clang::FrontendAction::Execute() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5c4c0b0)
#63 0x0000108d49629a40 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5ba9a40)
#64 0x0000108d497fee04 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5d7ee04)
#65 0x0000108d472b6218 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x3836218)
#66 0x0000108d472b1694 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#67 0x0000108d472b05e8 clang_main(int, char**, llvm::ToolContext const&) (/home/daltenty/llvm/dev/build/bin/clang-22+0x38305e8)
#68 0x0000108d472c4574 main (/home/daltenty/llvm/dev/build/bin/clang-22+0x3844574)
#69 0x00006a21d8336ca4 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#70 0x00006a21d8336eec __libc_start_main ./csu/../sysdeps/unix/sysv/linux/powerpc/libc-start.c:77:48
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/daltenty/llvm/dev/build/bin
Build config: +assertions
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-f56c58.cpp
clang++: note: diagnostic msg: /tmp/test-f56c58.sh
clang++: note: diagnostic msg:

********************
```

```
$ cat /tmp/test-f56c58.sh
# Crash reproducer for clang version 22.0.0git
# Driver args: "--driver-mode=g++" "/home/daltenty/test/test.cc"
# Original command:  "/home/daltenty/llvm/dev/build/bin/clang-22" "-cc1" "-triple" "powerpc64le-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "test.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "ppc64le" "-mfloat-abi" "hard" "-target-abi" "elfv2" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/daltenty/llvm/dev/build" "-fcoverage-compilation-dir=/home/daltenty/llvm/dev/build" "-resource-dir" "/home/daltenty/llvm/dev/build/lib/clang/22" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/powerpc64le-linux-gnu/c++/14" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14/backward" "-internal-isystem" "/home/daltenty/llvm/dev/build/lib/clang/22/include/ppc_wrappers" "-internal-isystem" "/home/daltenty/llvm/dev/build/lib/clang/22/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../powerpc64le-linux-gnu/include" "-internal-externc-isystem" "/usr/include/powerpc64le-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdeprecated-macro" "-ferror-limit" "19" "-fmessage-length=214" "-fno-signed-char" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/test-eaadd7.o" "-x" "c++" "/home/daltenty/test/test.cc"
 "/home/daltenty/llvm/dev/build/bin/clang-22" "-cc1" "-triple" "powerpc64le-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "test.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "ppc64le" "-mfloat-abi" "hard" "-target-abi" "elfv2" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/daltenty/llvm/dev/build" "-fcoverage-compilation-dir=/home/daltenty/llvm/dev/build" "-fdeprecated-macro" "-ferror-limit" "19" "-fmessage-length=214" "-fno-signed-char" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "test-f56c58.cpp"
```




## Comments

### Comment 1 - daltenty

FYI @tynasello 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Tenty (daltenty)

<details>
Since a205695de4dce3c839dedbb78dd67e2a7758947a landed, the following templated `attribute((constructor))` hello world test case will crash clang:
```
extern "C" int printf(const char *, ...);

template &lt;typename C, void (C::*F)(void) const&gt;
__attribute__((constructor)) void baz() { (C().*F)(); }

struct B {
  B() {
   struct A {
      void foo() const { printf("hello world!\n"); }
      void bar() const {
         ::baz&lt;A, &amp;A::foo&gt;();
      }
   };
  }
};

int main() {
  return 66;
}
```

Expected:
```
$ /home/daltenty/llvm/dev/build/bin/clang++ test.cc
$ ./a.out
hello world!
$ echo $?
66
```
Actual:
```
/home/daltenty/llvm/dev/build/bin/clang++ test.cc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/daltenty/llvm/dev/build/bin/clang-22 -cc1 -triple powerpc64le-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cc -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu ppc64le -mfloat-abi hard -target-abi elfv2 -debugger-tuning=gdb -fdebug-compilation-dir=/home/daltenty/llvm/dev/build -fcoverage-compilation-dir=/home/daltenty/llvm/dev/build -resource-dir /home/daltenty/llvm/dev/build/lib/clang/22 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/powerpc64le-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/daltenty/llvm/dev/build/lib/clang/22/include/ppc_wrappers -internal-isystem /home/daltenty/llvm/dev/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../powerpc64le-linux-gnu/include -internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=214 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-eaadd7.o -x c++ /home/daltenty/test/test.cc
1.      /home/daltenty/test/test.cc:11:28: current parser token ')'
2.      /home/daltenty/test/test.cc:6:1: parsing struct/union/class body 'B'
3.      /home/daltenty/test/test.cc:7:7: parsing function body 'B::B'
4.      /home/daltenty/test/test.cc:7:7: in compound statement ('{}')
5.      /home/daltenty/test/test.cc:8:4: parsing struct/union/class body 'A'
6.      /home/daltenty/test/test.cc:10:24: parsing function body 'B::B()::A::bar'
7.      /home/daltenty/test/test.cc:10:24: in compound statement ('{}')
 #<!-- -->0 0x0000108d487b2a6c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/daltenty/llvm/dev/build/bin/clang-22+0x4d32a6c)
 #<!-- -->1 0x0000108d487b3214 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x0000108d487af5ec llvm::sys::RunSignalHandlers() (/home/daltenty/llvm/dev/build/bin/clang-22+0x4d2f5ec)
 #<!-- -->3 0x0000108d487b3e7c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00006a21d9360444 (linux-vdso64.so.1+0x444)
 #<!-- -->5 0x0000108d4c39a954 clang::Expr::getIntegerConstantExpr(clang::ASTContext const&amp;) const (/home/daltenty/llvm/dev/build/bin/clang-22+0x891a954)
 #<!-- -->6 0x0000108d4bc40294 clang::Sema::InstantiateAttrs(clang::MultiLevelTemplateArgumentList const&amp;, clang::Decl const*, clang::Decl*, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::LocalInstantiationScope*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81c0294)
 #<!-- -->7 0x0000108d4bc517a0 clang::TemplateDeclInstantiator::InitFunctionInstantiation(clang::FunctionDecl*, clang::FunctionDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d17a0)
 #<!-- -->8 0x0000108d4bc4f0f8 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81cf0f8)
 #<!-- -->9 0x0000108d4bc57850 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d7850)
#<!-- -->10 0x0000108d4bbec06c clang::declvisitor::Base&lt;std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*&gt;::Visit(clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000108d4bca521c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->12 0x0000108d49309c18 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5889c18)
#<!-- -->13 0x0000108d4b305350 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7885350)
#<!-- -->14 0x0000108d4bc58b68 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x81d8b68)
#<!-- -->15 0x0000108d4bb33ba4 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x80b3ba4)
#<!-- -->16 0x0000108d4bb94090 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_2&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->17 0x0000108d49309c18 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5889c18)
#<!-- -->18 0x0000108d4b305350 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7885350)
#<!-- -->19 0x0000108d4bb374a8 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x80b74a8)
#<!-- -->20 0x0000108d4b9ec724 AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->21 0x0000108d4b9eb9ec clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f6b9ec)
#<!-- -->22 0x0000108d4ba03294 AddOverloadedCallCandidate(clang::Sema&amp;, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool) SemaOverload.cpp:0:0
#<!-- -->23 0x0000108d4ba03064 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f83064)
#<!-- -->24 0x0000108d4ba036e4 clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f836e4)
#<!-- -->25 0x0000108d4ba03c10 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7f83c10)
#<!-- -->26 0x0000108d4b642984 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7bc2984)
#<!-- -->27 0x0000108d4b65cb80 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7bdcb80)
#<!-- -->28 0x0000108d4b191af0 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7711af0)
#<!-- -->29 0x0000108d4b193764 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7713764)
#<!-- -->30 0x0000108d4b18ed24 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/home/daltenty/llvm/dev/build/bin/clang-22+0x770ed24)
#<!-- -->31 0x0000108d4b18ebc0 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/home/daltenty/llvm/dev/build/bin/clang-22+0x770ebc0)
#<!-- -->32 0x0000108d4b22f048 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77af048)
#<!-- -->33 0x0000108d4b22c9e8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77ac9e8)
#<!-- -->34 0x0000108d4b22bed0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77abed0)
#<!-- -->35 0x0000108d4b237238 clang::Parser::ParseCompoundStatementBody(bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b7238)
#<!-- -->36 0x0000108d4b2384c8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b84c8)
#<!-- -->37 0x0000108d4b267cb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7cb0)
#<!-- -->38 0x0000108d4b267970 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7970)
#<!-- -->39 0x0000108d4b265ee4 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e5ee4)
#<!-- -->40 0x0000108d4b1c3ffc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7743ffc)
#<!-- -->41 0x0000108d4b1c106c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x774106c)
#<!-- -->42 0x0000108d4b1ec5fc clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776c5fc)
#<!-- -->43 0x0000108d4b1e610c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776610c)
#<!-- -->44 0x0000108d4b1e5b90 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7765b90)
#<!-- -->45 0x0000108d4b22e48c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77ae48c)
#<!-- -->46 0x0000108d4b22bed0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77abed0)
#<!-- -->47 0x0000108d4b237238 clang::Parser::ParseCompoundStatementBody(bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b7238)
#<!-- -->48 0x0000108d4b2384c8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77b84c8)
#<!-- -->49 0x0000108d4b267cb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7cb0)
#<!-- -->50 0x0000108d4b267970 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e7970)
#<!-- -->51 0x0000108d4b265ee4 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x77e5ee4)
#<!-- -->52 0x0000108d4b1c3ffc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x7743ffc)
#<!-- -->53 0x0000108d4b1c106c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x774106c)
#<!-- -->54 0x0000108d4b1ec5fc clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/daltenty/llvm/dev/build/bin/clang-22+0x776c5fc)
#<!-- -->55 0x0000108d4b1699a4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e99a4)
#<!-- -->56 0x0000108d4b1694f4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e94f4)
#<!-- -->57 0x0000108d4b1684d4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e84d4)
#<!-- -->58 0x0000108d4b16604c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76e604c)
#<!-- -->59 0x0000108d4b158b40 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/daltenty/llvm/dev/build/bin/clang-22+0x76d8b40)
#<!-- -->60 0x0000108d496ccc0c clang::ASTFrontendAction::ExecuteAction() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5c4cc0c)
#<!-- -->61 0x0000108d49095430 clang::CodeGenAction::ExecuteAction() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5615430)
#<!-- -->62 0x0000108d496cc0b0 clang::FrontendAction::Execute() (/home/daltenty/llvm/dev/build/bin/clang-22+0x5c4c0b0)
#<!-- -->63 0x0000108d49629a40 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5ba9a40)
#<!-- -->64 0x0000108d497fee04 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x5d7ee04)
#<!-- -->65 0x0000108d472b6218 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/daltenty/llvm/dev/build/bin/clang-22+0x3836218)
#<!-- -->66 0x0000108d472b1694 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->67 0x0000108d472b05e8 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/daltenty/llvm/dev/build/bin/clang-22+0x38305e8)
#<!-- -->68 0x0000108d472c4574 main (/home/daltenty/llvm/dev/build/bin/clang-22+0x3844574)
#<!-- -->69 0x00006a21d8336ca4 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->70 0x00006a21d8336eec __libc_start_main ./csu/../sysdeps/unix/sysv/linux/powerpc/libc-start.c:77:48
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/daltenty/llvm/dev/build/bin
Build config: +assertions
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-f56c58.cpp
clang++: note: diagnostic msg: /tmp/test-f56c58.sh
clang++: note: diagnostic msg:

********************
```

```
$ cat /tmp/test-f56c58.sh
# Crash reproducer for clang version 22.0.0git
# Driver args: "--driver-mode=g++" "/home/daltenty/test/test.cc"
# Original command:  "/home/daltenty/llvm/dev/build/bin/clang-22" "-cc1" "-triple" "powerpc64le-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "test.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "ppc64le" "-mfloat-abi" "hard" "-target-abi" "elfv2" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/daltenty/llvm/dev/build" "-fcoverage-compilation-dir=/home/daltenty/llvm/dev/build" "-resource-dir" "/home/daltenty/llvm/dev/build/lib/clang/22" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/powerpc64le-linux-gnu/c++/14" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../include/c++/14/backward" "-internal-isystem" "/home/daltenty/llvm/dev/build/lib/clang/22/include/ppc_wrappers" "-internal-isystem" "/home/daltenty/llvm/dev/build/lib/clang/22/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/powerpc64le-linux-gnu/14/../../../../powerpc64le-linux-gnu/include" "-internal-externc-isystem" "/usr/include/powerpc64le-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdeprecated-macro" "-ferror-limit" "19" "-fmessage-length=214" "-fno-signed-char" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/test-eaadd7.o" "-x" "c++" "/home/daltenty/test/test.cc"
 "/home/daltenty/llvm/dev/build/bin/clang-22" "-cc1" "-triple" "powerpc64le-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "test.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "ppc64le" "-mfloat-abi" "hard" "-target-abi" "elfv2" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/daltenty/llvm/dev/build" "-fcoverage-compilation-dir=/home/daltenty/llvm/dev/build" "-fdeprecated-macro" "-ferror-limit" "19" "-fmessage-length=214" "-fno-signed-char" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "test-f56c58.cpp"
```



</details>


---

