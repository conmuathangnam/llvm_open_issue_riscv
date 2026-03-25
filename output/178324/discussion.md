# A/F: `isDependentContext()' failed after commit 154507c

**Author:** dyung
**URL:** https://github.com/llvm/llvm-project/issues/178324

## Body

We recently hit an assertion in a test which I was able to reduce to the following code (with help from @gregbedwell):
```c++
struct a {
  using e = int;
};
void current(const char * = __builtin_FUNCSIG());
template <class> void c() { decltype(a(current()))::e; }
```

When compiled with `-fms-extensions` using a compiler that has assertions enabled, it hits the following assertion failure:
```
clang: /home/dyung/src/upstream/llvm-project-build/clang/lib/AST/CXXInheritance.cpp:117: bool clang::CXXRecordDecl::isCurrentInstantiation(const clang::DeclContext*) const: Assertion `isDependentContext()' failed.
```

I was able to bisect the failure back to commit 154507cf403e1859b9a81fa76af406e8489daa4b.

Here is the output of compiling with a compiler built from 154507cf403e1859b9a81fa76af406e8489daa4b:
```
$ ~/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang -c -fms-extensions repro4.cpp
clang: /home/dyung/src/upstream/llvm-project-build/clang/lib/AST/CXXInheritance.cpp:117: bool clang::CXXRecordDecl::isCurrentInstantiation(const clang::DeclContext*) const: Assertion `isDependentContext()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang -c -fms-extensions repro4.cpp
1.      repro4.cpp:5:53: current parser token 'e'
2.      repro4.cpp:5:27: parsing function body 'c'
3.      repro4.cpp:5:27: in compound statement ('{}')
 #0 0x000057205824a9a2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x22579a2)
 #1 0x0000572058247a0f llvm::sys::RunSignalHandlers() (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2254a0f)
 #2 0x0000572058185d38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073a47e245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073a47e29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073a47e29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073a47e29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073a47e24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073a47e2288ff abort ./stdlib/abort.c:81:7
 #9 0x000073a47e22881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073a47e23b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000057205b747585 clang::CXXRecordDecl::isCurrentInstantiation(clang::DeclContext const*) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x5754585)
#12 0x000057205a8d0639 getCurrentInstantiationOf(clang::QualType, clang::DeclContext*) SemaCXXScopeSpec.cpp:0:0
#13 0x000057205a8d272f clang::Sema::computeDeclContext(clang::CXXScopeSpec const&, bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x48df72f)
#14 0x000057205aa84e57 clang::Sema::getTypeName(clang::IdentifierInfo const&, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr<clang::QualType>, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x4a91e57)
#15 0x000057205a6df3b1 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46ec3b1)
#16 0x000057205a6e0197 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46ed197)
#17 0x000057205a803879 clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x4810879)
#18 0x000057205a8094b2 clang::Parser::isCXXDeclarationStatement(bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x48164b2)
#19 0x000057205a7dff2f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ecf2f)
#20 0x000057205a7e175c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ee75c)
#21 0x000057205a7e2941 clang::Parser::ParseCompoundStatementBody(bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ef941)
#22 0x000057205a7e30cf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47f00cf)
#23 0x000057205a6e6397 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f3397)
#24 0x000057205a72d124 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x473a124)
#25 0x000057205a7f2bdb clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ffbdb)
#26 0x000057205a8009f8 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#27 0x000057205a800e1a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x480de1a)
#28 0x000057205a72e5f0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x473b5f0)
#29 0x000057205a6e98f0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f68f0)
#30 0x000057205a6ea75d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f775d)
#31 0x000057205a6da4ca clang::ParseAST(clang::Sema&, bool, bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46e74ca)
#32 0x0000572058eece97 clang::FrontendAction::Execute() (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2ef9e97)
#33 0x0000572058e6ffcd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2e7cfcd)
#34 0x0000572058fd4f27 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2fe1f27)
#35 0x0000572056cc5a5a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcd2a5a)
#36 0x0000572056cbb9c7 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x0000572058c2c79d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x00005720581861fe llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x21931fe)
#39 0x0000572058c2d898 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x0000572058beaeea clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2bf7eea)
#41 0x0000572058bec052 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2bf9052)
#42 0x0000572058bf9454 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2c06454)
#43 0x0000572056cc10d8 clang_main(int, char**, llvm::ToolContext const&) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcce0d8)
#44 0x0000572056bc995a main (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xbd695a)
#45 0x000073a47e22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#46 0x000073a47e22a28b call_init ./csu/../csu/libc-start.c:128:20
#47 0x000073a47e22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#48 0x0000572056cbaf45 _start (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcc7f45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0 (https://github.com/llvm/llvm-project.git 154507cf403e1859b9a81fa76af406e8489daa4b)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin
Build config: +assertions
```

For comparison, here is the output when building from the immediately preceding commit 2f7e685e3d8813ca584cc2f278ac3cd57b43772d:
```
$ ~/src/upstream/2f7e685e3d8813ca584cc2f278ac3cd57b43772d-linux/bin/clang -c -fms-extensions repro4.cpp
repro4.cpp:5:29: warning: declaration does not declare anything [-Wmissing-declarations]
    5 | template <class> void c() { decltype(a(current()))::e; }
      |                             ^~~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
```

@mizvekov, this was your change, could you take a look?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (dyung)

<details>
We recently hit an assertion in a test which I was able to reduce to the following code (with help from @<!-- -->gregbedwell):
```c++
struct a {
  using e = int;
};
void current(const char * = __builtin_FUNCSIG());
template &lt;class&gt; void c() { decltype(a(current()))::e; }
```

When compiled with `-fms-extensions` using a compiler that has assertions enabled, it hits the following assertion failure:
```
clang: /home/dyung/src/upstream/llvm-project-build/clang/lib/AST/CXXInheritance.cpp:117: bool clang::CXXRecordDecl::isCurrentInstantiation(const clang::DeclContext*) const: Assertion `isDependentContext()' failed.
```

I was able to bisect the failure back to commit 154507cf403e1859b9a81fa76af406e8489daa4b.

Here is the output of compiling with a compiler built from 154507cf403e1859b9a81fa76af406e8489daa4b:
```
$ ~/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang -c -fms-extensions repro4.cpp
clang: /home/dyung/src/upstream/llvm-project-build/clang/lib/AST/CXXInheritance.cpp:117: bool clang::CXXRecordDecl::isCurrentInstantiation(const clang::DeclContext*) const: Assertion `isDependentContext()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang -c -fms-extensions repro4.cpp
1.      repro4.cpp:5:53: current parser token 'e'
2.      repro4.cpp:5:27: parsing function body 'c'
3.      repro4.cpp:5:27: in compound statement ('{}')
 #<!-- -->0 0x000057205824a9a2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x22579a2)
 #<!-- -->1 0x0000572058247a0f llvm::sys::RunSignalHandlers() (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2254a0f)
 #<!-- -->2 0x0000572058185d38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073a47e245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000073a47e29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000073a47e29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000073a47e29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000073a47e24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000073a47e2288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000073a47e22881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000073a47e23b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000057205b747585 clang::CXXRecordDecl::isCurrentInstantiation(clang::DeclContext const*) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x5754585)
#<!-- -->12 0x000057205a8d0639 getCurrentInstantiationOf(clang::QualType, clang::DeclContext*) SemaCXXScopeSpec.cpp:0:0
#<!-- -->13 0x000057205a8d272f clang::Sema::computeDeclContext(clang::CXXScopeSpec const&amp;, bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x48df72f)
#<!-- -->14 0x000057205aa84e57 clang::Sema::getTypeName(clang::IdentifierInfo const&amp;, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x4a91e57)
#<!-- -->15 0x000057205a6df3b1 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46ec3b1)
#<!-- -->16 0x000057205a6e0197 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46ed197)
#<!-- -->17 0x000057205a803879 clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x4810879)
#<!-- -->18 0x000057205a8094b2 clang::Parser::isCXXDeclarationStatement(bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x48164b2)
#<!-- -->19 0x000057205a7dff2f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ecf2f)
#<!-- -->20 0x000057205a7e175c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ee75c)
#<!-- -->21 0x000057205a7e2941 clang::Parser::ParseCompoundStatementBody(bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ef941)
#<!-- -->22 0x000057205a7e30cf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47f00cf)
#<!-- -->23 0x000057205a6e6397 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f3397)
#<!-- -->24 0x000057205a72d124 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x473a124)
#<!-- -->25 0x000057205a7f2bdb clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x47ffbdb)
#<!-- -->26 0x000057205a8009f8 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->27 0x000057205a800e1a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x480de1a)
#<!-- -->28 0x000057205a72e5f0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x473b5f0)
#<!-- -->29 0x000057205a6e98f0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f68f0)
#<!-- -->30 0x000057205a6ea75d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46f775d)
#<!-- -->31 0x000057205a6da4ca clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x46e74ca)
#<!-- -->32 0x0000572058eece97 clang::FrontendAction::Execute() (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2ef9e97)
#<!-- -->33 0x0000572058e6ffcd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2e7cfcd)
#<!-- -->34 0x0000572058fd4f27 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2fe1f27)
#<!-- -->35 0x0000572056cc5a5a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcd2a5a)
#<!-- -->36 0x0000572056cbb9c7 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->37 0x0000572058c2c79d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x00005720581861fe llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x21931fe)
#<!-- -->39 0x0000572058c2d898 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x0000572058beaeea clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2bf7eea)
#<!-- -->41 0x0000572058bec052 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2bf9052)
#<!-- -->42 0x0000572058bf9454 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0x2c06454)
#<!-- -->43 0x0000572056cc10d8 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcce0d8)
#<!-- -->44 0x0000572056bc995a main (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xbd695a)
#<!-- -->45 0x000073a47e22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->46 0x000073a47e22a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->47 0x000073a47e22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->48 0x0000572056cbaf45 _start (/home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin/clang+0xcc7f45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0 (https://github.com/llvm/llvm-project.git 154507cf403e1859b9a81fa76af406e8489daa4b)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/dyung/src/upstream/154507cf403e1859b9a81fa76af406e8489daa4b-linux/bin
Build config: +assertions
```

For comparison, here is the output when building from the immediately preceding commit 2f7e685e3d8813ca584cc2f278ac3cd57b43772d:
```
$ ~/src/upstream/2f7e685e3d8813ca584cc2f278ac3cd57b43772d-linux/bin/clang -c -fms-extensions repro4.cpp
repro4.cpp:5:29: warning: declaration does not declare anything [-Wmissing-declarations]
    5 | template &lt;class&gt; void c() { decltype(a(current()))::e; }
      |                             ^~~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
```

@<!-- -->mizvekov, this was your change, could you take a look?
</details>


---

### Comment 2 - shafik

Goes back to clang-21: https://godbolt.org/z/bKo1K459M

---

### Comment 3 - cor3ntin

@mizvekov 

---

