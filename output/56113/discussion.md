# clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)

**Author:** mango-wyc
**URL:** https://github.com/llvm/llvm-project/issues/56113

## Body

when i use astmatcher look for functionDecl , always show me this error , what happened , anyone has the same problem?

```console
1.	<eof> parser at end of file
2.	ASTMatcher: Processing '<unknown>' against:
	FunctionDecl isascii : </Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/_ctype.h:134:1 <Spelling=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/sys/cdefs.h:347:36>, /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/_ctype.h:138:1>
--- Bound Nodes Begin ---
    FunctiondFeclWithCall - { FunctionDecl isascii : </Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/_ctype.h:134:1 <Spelling=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/sys/cdefs.h:347:36>, /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk/usr/include/_ctype.h:138:1> }
--- Bound Nodes End ---
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang                    0x0000000112d73e4d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 61
1  clang                    0x0000000112d743cb PrintStackTraceSignalHandler(void*) + 27
2  clang                    0x0000000112d72176 llvm::sys::RunSignalHandlers() + 134
3  clang                    0x0000000112d736ae llvm::sys::CleanupOnSignal(unsigned long) + 110
4  clang                    0x0000000112c779d0 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) + 192
5  clang                    0x0000000112c77df3 CrashRecoverySignalHandler(int) + 195
6  libsystem_platform.dylib 0x00007ff80092adfd _sigtramp + 29
7  clang                    0x000000010e24eb2c llvm::SmallVectorTemplateCommon<unsigned int, void>::isSmall() const + 28
8  libsystem_c.dylib        0x00007ff800860d24 abort + 123
9  libsystem_c.dylib        0x00007ff8008600cb err + 0
10 clang                    0x0000000113495a3f clang::Diagnostic::getArgKind(unsigned int) const + 111
11 clang                    0x00000001134944ba clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const + 1498
12 clang                    0x0000000113493e97 clang::Diagnostic::FormatDiagnostic(llvm::SmallVectorImpl<char>&) const + 199
13 clang                    0x000000011468509d clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) + 109
14 clang                    0x000000011441dc1c clang::ChainedDiagnosticConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) + 60
15 clang                    0x00000001134a7f44 clang::DiagnosticIDs::EmitDiag(clang::DiagnosticsEngine&, clang::DiagnosticIDs::Level) const + 132
16 clang                    0x00000001134a7d17 clang::DiagnosticIDs::ProcessDiag(clang::DiagnosticsEngine&) const + 823
17 clang                    0x0000000113493cb9 clang::DiagnosticsEngine::ProcessDiag() + 41
18 clang                    0x0000000113493c4b clang::DiagnosticsEngine::EmitCurrentDiagnostic(bool) + 267
19 refMethod.dylib          0x000000013d70612f clang::DiagnosticBuilder::Emit() + 63
20 refMethod.dylib          0x000000013d7060d9 clang::DiagnosticBuilder::~DiagnosticBuilder() + 25
21 refMethod.dylib          0x000000013d705aa5 clang::DiagnosticBuilder::~DiagnosticBuilder() + 21
22 refMethod.dylib          0x000000013d705811 refMethod::CJMatchCallback::run(clang::ast_matchers::MatchFinder::MatchResult const&) + 177
23 clang                    0x0000000118c0cc2f clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&) + 127
24 clang                    0x0000000118ca4b5f clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) + 191
25 clang                    0x0000000118c09716 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) + 854
26 clang                    0x0000000118c09390 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchDispatch(clang::Decl const*) + 64
27 clang                    0x0000000118c0918d void clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::match<clang::Decl>(clang::Decl const&) + 29
28 clang                    0x0000000118c1d53d clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) + 333
29 clang                    0x0000000118c89088 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) + 200
30 clang                    0x0000000118c2aca1 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) + 513
31 clang                    0x0000000118c1e455 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) + 3781
32 clang                    0x0000000118c1d54a clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) + 346
33 clang                    0x0000000118b6bb02 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseAST(clang::ASTContext&) + 50
34 clang                    0x0000000118b6b96c clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) + 108
35 refMethod.dylib          0x000000013d705461 refMethod::CJASTConsumer::HandleTranslationUnit(clang::ASTContext&) + 33
36 clang                    0x00000001146344b3 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) + 115
37 clang                    0x00000001173ff1b1 clang::ParseAST(clang::Sema&, bool, bool) + 897
38 clang                    0x000000011454dd3c clang::ASTFrontendAction::ExecuteAction() + 300
39 clang                    0x0000000113cc86bc clang::CodeGenAction::ExecuteAction() + 92
40 clang                    0x000000011454d35c clang::FrontendAction::Execute() + 124
41 clang                    0x0000000114434d8d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 957
42 clang                    0x000000011469c737 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 935
43 clang                    0x000000010dd69a9a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 1530
44 clang                    0x000000010dd5a8ec ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) + 380
45 clang                    0x0000000114102415 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_1::operator()() const + 37
46 clang                    0x00000001141023e5 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_1>(long) + 21
47 clang                    0x0000000112c77869 llvm::function_ref<void ()>::operator()() const + 25
48 clang                    0x0000000112c7780c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 236
49 clang                    0x00000001140fc3e8 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const + 536
50 clang                    0x000000011408a092 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&) const + 770
51 clang                    0x000000011408a498 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) const + 152
52 clang                    0x00000001140a9995 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) + 325
53 clang                    0x000000010dd59f23 clang_main(int, char**) + 4595
54 clang                    0x000000010dd58522 main + 34
55 dyld                     0x00000001403eb51e start + 462
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 15.0.0
Target: x86_64-apple-ios15.2-simulator
Thread model: posix
InstalledDir: /Users/wangyuchu/Documents/llvm-project-main/llvm-build/Debug/bin
clang: error: unable to execute command: Abort trap: 6
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - AaronBallman

Does this issue still reproduce for you (with Clang 19, 20, or trunk)? If so, can you provide code for us to reproduce it?

---

