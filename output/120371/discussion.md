# Crash initializing a static constexpr member of incomplete type in a templated class

**Author:** ArcheCadia
**URL:** https://github.com/llvm/llvm-project/issues/120371

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -I/home/arche/ntfs/CS/chess/engine/src/engine -I/home/arche/ntfs/CS/chess/engine/external -I/home/arche/ntfs/CS/chess/engine/src/games/core -I/home/arche/ntfs/CS/chess/engine/src/games -I/home/arche/ntfs/CS/chess/engine/src/games/chess -I/home/arche/ntfs/CS/chess/engine/src -I/home/arche/ntfs/CS/chess/engine/src/common -g -Og -Wall -Wextra -Wconversion -Wpedantic -std=gnu++20 -MD -MT src/engine/CMakeFiles/engine_objects.dir/selfplay_data.cpp.o -MF CMakeFiles/engine_objects.dir/selfplay_data.cpp.o.d -o CMakeFiles/engine_objects.dir/selfplay_data.cpp.o -c /home/arche/ntfs/CS/chess/engine/src/engine/selfplay_data.cpp
1.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:75:47: current parser token ';'
2.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:10:1: parsing namespace 'my_engine'
3.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:13:1: parsing struct/union/class body 'my_engine::Graph'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007f13c27c4d90 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 64
1  libLLVM.so.18.1      0x00007f13c27c226d llvm::sys::CleanupOnSignal(unsigned long) + 141
2  libLLVM.so.18.1      0x00007f13c26a88e9
3  libc.so.6            0x00007f13c1a4c1d0
4  libclang-cpp.so.18.1 0x00007f13cadf482c clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) + 252
5  libclang-cpp.so.18.1 0x00007f13cb1a82a3 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) + 1091
6  libclang-cpp.so.18.1 0x00007f13cb1aa390 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) + 3088
7  libclang-cpp.so.18.1 0x00007f13ca7c7546 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) + 8182
8  libclang-cpp.so.18.1 0x00007f13ca7c8428 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) + 1528
9  libclang-cpp.so.18.1 0x00007f13ca7c8933 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) + 1043
10 libclang-cpp.so.18.1 0x00007f13ca7ca9bb clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) + 5371
11 libclang-cpp.so.18.1 0x00007f13ca7cc11a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) + 3226
12 libclang-cpp.so.18.1 0x00007f13ca867b01 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) + 945
13 libclang-cpp.so.18.1 0x00007f13ca868bea clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) + 1242
14 libclang-cpp.so.18.1 0x00007f13ca869156 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) + 182
15 libclang-cpp.so.18.1 0x00007f13ca7dcfdb clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) + 667
16 libclang-cpp.so.18.1 0x00007f13ca86954b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 763
17 libclang-cpp.so.18.1 0x00007f13ca7bb47f clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) + 527
18 libclang-cpp.so.18.1 0x00007f13ca7bc5f9 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) + 4185
19 libclang-cpp.so.18.1 0x00007f13ca7dd18c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) + 1100
20 libclang-cpp.so.18.1 0x00007f13ca86954b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 763
21 libclang-cpp.so.18.1 0x00007f13ca86a0eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 427
22 libclang-cpp.so.18.1 0x00007f13ca78d038 clang::ParseAST(clang::Sema&, bool, bool) + 632
23 libclang-cpp.so.18.1 0x00007f13cc258451 clang::FrontendAction::Execute() + 433
24 libclang-cpp.so.18.1 0x00007f13cc1f9097 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 439
25 libclang-cpp.so.18.1 0x00007f13cc2b07fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 445
26 clang++              0x00005a1be51883a4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 8900
27 clang++              0x00005a1be518c70a
28 libclang-cpp.so.18.1 0x00007f13cbf2e485
29 libLLVM.so.18.1      0x00007f13c26a8d3a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 42
30 libclang-cpp.so.18.1 0x00007f13cbf30e16
31 libclang-cpp.so.18.1 0x00007f13cbeec039 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 169
32 libclang-cpp.so.18.1 0x00007f13cbeec43c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 252
33 libclang-cpp.so.18.1 0x00007f13cbf0a864 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 388
34 clang++              0x00005a1be518f045 clang_main(int, char**, llvm::ToolContext const&) + 9669
35 clang++              0x00005a1be517ffd5 main + 133
36 libc.so.6            0x00007f13c1a34e08
37 libc.so.6            0x00007f13c1a34ecc __libc_start_main + 140
38 clang++              0x00005a1be5180035 _start + 37
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/selfplay_data-4ebb42.cpp
clang++: note: diagnostic msg: /tmp/selfplay_data-4ebb42.sh
clang++: note: diagnostic msg: 

********************
```
[selfplay_data-4ebb42_sh.txt](https://github.com/user-attachments/files/18176582/selfplay_data-4ebb42_sh.txt)
[selfplay_data-4ebb42_cpp.txt](https://github.com/user-attachments/files/18176584/selfplay_data-4ebb42_cpp.txt)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (JeremieHuteau)

<details>
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -I/home/arche/ntfs/CS/chess/engine/src/engine -I/home/arche/ntfs/CS/chess/engine/external -I/home/arche/ntfs/CS/chess/engine/src/games/core -I/home/arche/ntfs/CS/chess/engine/src/games -I/home/arche/ntfs/CS/chess/engine/src/games/chess -I/home/arche/ntfs/CS/chess/engine/src -I/home/arche/ntfs/CS/chess/engine/src/common -g -Og -Wall -Wextra -Wconversion -Wpedantic -std=gnu++20 -MD -MT src/engine/CMakeFiles/engine_objects.dir/selfplay_data.cpp.o -MF CMakeFiles/engine_objects.dir/selfplay_data.cpp.o.d -o CMakeFiles/engine_objects.dir/selfplay_data.cpp.o -c /home/arche/ntfs/CS/chess/engine/src/engine/selfplay_data.cpp
1.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:75:47: current parser token ';'
2.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:10:1: parsing namespace 'my_engine'
3.	/home/arche/ntfs/CS/chess/engine/src/engine/graph.hpp:13:1: parsing struct/union/class body 'my_engine::Graph'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007f13c27c4d90 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 64
1  libLLVM.so.18.1      0x00007f13c27c226d llvm::sys::CleanupOnSignal(unsigned long) + 141
2  libLLVM.so.18.1      0x00007f13c26a88e9
3  libc.so.6            0x00007f13c1a4c1d0
4  libclang-cpp.so.18.1 0x00007f13cadf482c clang::QualType::isNonConstantStorage(clang::ASTContext const&amp;, bool, bool) + 252
5  libclang-cpp.so.18.1 0x00007f13cb1a82a3 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) + 1091
6  libclang-cpp.so.18.1 0x00007f13cb1aa390 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) + 3088
7  libclang-cpp.so.18.1 0x00007f13ca7c7546 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject*) + 8182
8  libclang-cpp.so.18.1 0x00007f13ca7c8428 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) + 1528
9  libclang-cpp.so.18.1 0x00007f13ca7c8933 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) + 1043
10 libclang-cpp.so.18.1 0x00007f13ca7ca9bb clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) + 5371
11 libclang-cpp.so.18.1 0x00007f13ca7cc11a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) + 3226
12 libclang-cpp.so.18.1 0x00007f13ca867b01 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) + 945
13 libclang-cpp.so.18.1 0x00007f13ca868bea clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) + 1242
14 libclang-cpp.so.18.1 0x00007f13ca869156 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) + 182
15 libclang-cpp.so.18.1 0x00007f13ca7dcfdb clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) + 667
16 libclang-cpp.so.18.1 0x00007f13ca86954b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 763
17 libclang-cpp.so.18.1 0x00007f13ca7bb47f clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) + 527
18 libclang-cpp.so.18.1 0x00007f13ca7bc5f9 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) + 4185
19 libclang-cpp.so.18.1 0x00007f13ca7dd18c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) + 1100
20 libclang-cpp.so.18.1 0x00007f13ca86954b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 763
21 libclang-cpp.so.18.1 0x00007f13ca86a0eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 427
22 libclang-cpp.so.18.1 0x00007f13ca78d038 clang::ParseAST(clang::Sema&amp;, bool, bool) + 632
23 libclang-cpp.so.18.1 0x00007f13cc258451 clang::FrontendAction::Execute() + 433
24 libclang-cpp.so.18.1 0x00007f13cc1f9097 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 439
25 libclang-cpp.so.18.1 0x00007f13cc2b07fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 445
26 clang++              0x00005a1be51883a4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 8900
27 clang++              0x00005a1be518c70a
28 libclang-cpp.so.18.1 0x00007f13cbf2e485
29 libLLVM.so.18.1      0x00007f13c26a8d3a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 42
30 libclang-cpp.so.18.1 0x00007f13cbf30e16
31 libclang-cpp.so.18.1 0x00007f13cbeec039 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 169
32 libclang-cpp.so.18.1 0x00007f13cbeec43c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 252
33 libclang-cpp.so.18.1 0x00007f13cbf0a864 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 388
34 clang++              0x00005a1be518f045 clang_main(int, char**, llvm::ToolContext const&amp;) + 9669
35 clang++              0x00005a1be517ffd5 main + 133
36 libc.so.6            0x00007f13c1a34e08
37 libc.so.6            0x00007f13c1a34ecc __libc_start_main + 140
38 clang++              0x00005a1be5180035 _start + 37
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/selfplay_data-4ebb42.cpp
clang++: note: diagnostic msg: /tmp/selfplay_data-4ebb42.sh
clang++: note: diagnostic msg: 

********************
```
[selfplay_data-4ebb42_sh.txt](https://github.com/user-attachments/files/18176582/selfplay_data-4ebb42_sh.txt)
[selfplay_data-4ebb42_cpp.txt](https://github.com/user-attachments/files/18176584/selfplay_data-4ebb42_cpp.txt)

</details>


---

### Comment 2 - MagentaTreehouse

Reduced:
```c++
template <class>
struct Graph {
 class NodeId;
 static constexpr NodeId nil_node_id_{nullptr};
};
```

Assertion:
```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclCXX.h:465:
clang::CXXRecordDecl::DefinitionData& clang::CXXRecordDecl::data() const:
Assertion `DD && "queried property of class with no definition"' failed.
```

Stack dump:
```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:47: current parser token ';'
2.	<source>:2:1: parsing struct/union/class body 'Graph'
 #0 0x0000000003c5ca08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c5ca08)
 #1 0x0000000003c5a714 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c5a714)
 #2 0x0000000003ba7c78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bed19a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bed19a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bed19a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bed19a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007bed19a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007bed19a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000003f219d6 clang::CXXRecordDecl::data() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f219d6)
#10 0x0000000007b29f50 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b29f50)
#11 0x0000000006973268 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6973268)
#12 0x0000000006979a4e clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6979a4e)
#13 0x000000000664a4fa clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664a4fa)
#14 0x000000000664b621 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664b621)
#15 0x000000000664d147 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664d147)
#16 0x000000000664f8c3 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664f8c3)
#17 0x0000000006623e40 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6623e40)
#18 0x00000000066ecf1a clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ecf1a)
#19 0x00000000066fa86c clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#20 0x00000000066fab8a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fab8a)
#21 0x000000000662ab33 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662ab33)
#22 0x00000000065e9907 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9907)
#23 0x00000000065ea78d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ea78d)
#24 0x00000000065eac30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65eac30)
#25 0x00000000065dcc93 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65dcc93)
#26 0x00000000045e6858 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e6858)
#27 0x000000000489fe39 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489fe39)
#28 0x000000000482283e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482283e)
#29 0x000000000498c86e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498c86e)
#30 0x0000000000ce2f7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce2f7f)
#31 0x0000000000cdaa1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x000000000462acc9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003ba8124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ba8124)
#34 0x000000000462b2bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x00000000045f05cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f05cd)
#36 0x00000000045f16bd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f16bd)
#37 0x00000000045f8c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f8c35)
#38 0x0000000000cdfdc3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcdfdc3)
#39 0x0000000000bad264 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbad264)
#40 0x00007bed19a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x00007bed19a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000cda4c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcda4c5)
```

Compiler Explorer: https://compiler-explorer.com/z/dY3Mce3bs

---

### Comment 3 - shafik

Looks like a regression going back to clang-18: https://compiler-explorer.com/z/41fnbrodM

Feels pretty amazing we did not catch this earlier.

---

### Comment 4 - shafik

@dwblaikie git bisect points to 19f2b68095fe727e40079b7c6380b36b6462e691 as the cause of this regression and it does indeed look like `isNonConstantStorage` is the culprit here.

---

