# LLDB crash when debugging Tokenizer's Rust crate

**Author:** brosenfi
**URL:** https://github.com/llvm/llvm-project/issues/163048
**Status:** Closed
**Labels:** lldb, crash
**Closed Date:** 2025-11-17T10:39:12Z

## Body

LLDB is crashing when inspecting a frame with the Tokenizer's Rust crate included.  I created a [repo](https://github.com/brosenfi/test-tokenizers-codelldb-debugging-issue) to make it easy to reproduce the issue (using VSCode and dev-containers).

There was only an empty file in the LLDB diagnostics dir:
```
vscode ➜ /workspaces/test-tokenizers-codelldb-debugging-issue (main) $ ls -alt /tmp/diagnostics-dd7d25
total 8
drwxrwxrwt 1 root   root   4096 Oct 12 07:02 ..
drwxr-x--- 2 vscode vscode 4096 Oct 12 07:02 .
-rw-r--r-- 1 vscode vscode    0 Oct 12 07:02 diagnostics.log
```

The stack dump / overflow
```
0.      Program arguments: lldb ./target/debug/test-issue
  #0 0x000055758b93bf1e llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
  #1 0x000055758b93c3ea PrintStackTraceSignalHandler(void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
  #2 0x000055758b93989e llvm::sys::RunSignalHandlers() /opt/llvm-project/llvm/lib/Support/Signals.cpp:104:20
  #3 0x000055758b93b88a SignalHandler(int, siginfo_t*, void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
  #4 0x0000750e080d3050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
  #5 0x0000750e081987d9 syscall (/lib/x86_64-linux-gnu/libc.so.6+0x1017d9)
  #6 0x0000750e151cb0b5 SignalHandler(int, siginfo_t*, void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:78
  #7 0x0000750e080d3050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
  #8 0x0000750e14fa090a clang::Redeclarable<clang::TagDecl>::DeclLink::getPrevious(clang::TagDecl const*) const /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:120:16
  #9 0x0000750e14f9a0b3 clang::Redeclarable<clang::TagDecl>::getNextRedeclaration() const /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:187:3
 #10 0x0000750e14f9a0da clang::Redeclarable<clang::TagDecl>::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:225:3
 #11 0x0000750e14f9880c clang::RecordDecl::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/Decl.h:4286:28
 #12 0x0000750e14f98a68 clang::CXXRecordDecl::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:540:31
 #13 0x0000750e14f98a8a clang::CXXRecordDecl::getMostRecentDecl() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:546:3
 #14 0x0000750e14f9899e clang::CXXRecordDecl::dataPtr() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:458:12
 #15 0x0000750e14f989c4 clang::CXXRecordDecl::data() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:462:23
 #16 0x0000750e1504cc8c clang::CXXRecordDecl::bases_begin() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:625:73
 #17 0x0000750e1504ccaf clang::CXXRecordDecl::bases_end() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:628:23
 #18 0x0000750e185cdf3d clang::CXXRecordDecl::bases() const /opt/llvm-project/clang/include/clang/AST/DeclCXX.h:621:12
 #19 0x0000750e1cecc6aa (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:188:51
 #20 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #21 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #22 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #23 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #24 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #25 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #26 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #27 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #28 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #29 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #30 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #31 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #32 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #33 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #34 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #35 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #36 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #37 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #38 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #39 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #40 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #41 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #42 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #43 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #44 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #45 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #46 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #47 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #48 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #49 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #50 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #51 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #52 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #53 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #54 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #55 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #56 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #57 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #58 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #59 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #60 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #61 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #62 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #63 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #64 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #65 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #66 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #67 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #68 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #69 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #70 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #71 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #72 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #73 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #74 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #75 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #76 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #77 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #78 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #79 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #80 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #81 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #82 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #83 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #84 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #85 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #86 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #87 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #88 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #89 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #90 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #91 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #92 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #93 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #94 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #95 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #96 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #97 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #98 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #99 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#100 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#101 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#102 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#103 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#104 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#105 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#106 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#107 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#108 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#109 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#110 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#111 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#112 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#113 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#114 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#115 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#116 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#117 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#118 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#119 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#120 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#121 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#122 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#123 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#124 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#125 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#126 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#127 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#128 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#129 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#130 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#131 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#132 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#133 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#134 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#135 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#136 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#137 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#138 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#139 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#140 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#141 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#142 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#143 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#144 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#145 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#146 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#147 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#148 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#149 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#150 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#151 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#152 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#153 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#154 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#155 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#156 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#157 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#158 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#159 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#160 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#161 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#162 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#163 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#164 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#165 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#166 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#167 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#168 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#169 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#170 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#171 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#172 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#173 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#174 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#175 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#176 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#177 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#178 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#179 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#180 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#181 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#182 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#183 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#184 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#185 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#186 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#187 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#188 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#189 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#190 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#191 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#192 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#193 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#194 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#195 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#196 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#197 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#198 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#199 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#200 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#201 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#202 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#203 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#204 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#205 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#206 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#207 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#208 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#209 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#210 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#211 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#212 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#213 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#214 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#215 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#216 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#217 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#218 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#219 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#220 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#221 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#222 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#223 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#224 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#225 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#226 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#227 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#228 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#229 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#230 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#231 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#232 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#233 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#234 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#235 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#236 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#237 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#238 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#239 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#240 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#241 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#242 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#243 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#244 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#245 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#246 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#247 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#248 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#249 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#250 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#251 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#252 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#253 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#254 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#255 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Brian Rosenfield (brosenfi)

<details>
LLDB is crashing when inspecting a frame with the Tokenizer's Rust crate included.  I created a [repo](https://github.com/brosenfi/test-tokenizers-codelldb-debugging-issue) to make it easy to reproduce the issue (using VSCode and dev-containers).

There was only an empty file in the LLDB diagnostics dir:
```
vscode ➜ /workspaces/test-tokenizers-codelldb-debugging-issue (main) $ ls -alt /tmp/diagnostics-dd7d25
total 8
drwxrwxrwt 1 root   root   4096 Oct 12 07:02 ..
drwxr-x--- 2 vscode vscode 4096 Oct 12 07:02 .
-rw-r--r-- 1 vscode vscode    0 Oct 12 07:02 diagnostics.log
```

The stack dump / overflow
```
0.      Program arguments: lldb ./target/debug/test-issue
  #<!-- -->0 0x000055758b93bf1e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
  #<!-- -->1 0x000055758b93c3ea PrintStackTraceSignalHandler(void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
  #<!-- -->2 0x000055758b93989e llvm::sys::RunSignalHandlers() /opt/llvm-project/llvm/lib/Support/Signals.cpp:104:20
  #<!-- -->3 0x000055758b93b88a SignalHandler(int, siginfo_t*, void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
  #<!-- -->4 0x0000750e080d3050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
  #<!-- -->5 0x0000750e081987d9 syscall (/lib/x86_64-linux-gnu/libc.so.6+0x1017d9)
  #<!-- -->6 0x0000750e151cb0b5 SignalHandler(int, siginfo_t*, void*) /opt/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:78
  #<!-- -->7 0x0000750e080d3050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
  #<!-- -->8 0x0000750e14fa090a clang::Redeclarable&lt;clang::TagDecl&gt;::DeclLink::getPrevious(clang::TagDecl const*) const /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:120:16
  #<!-- -->9 0x0000750e14f9a0b3 clang::Redeclarable&lt;clang::TagDecl&gt;::getNextRedeclaration() const /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:187:3
 #<!-- -->10 0x0000750e14f9a0da clang::Redeclarable&lt;clang::TagDecl&gt;::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/Redeclarable.h:225:3
 #<!-- -->11 0x0000750e14f9880c clang::RecordDecl::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/Decl.h:4286:28
 #<!-- -->12 0x0000750e14f98a68 clang::CXXRecordDecl::getMostRecentDecl() /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:540:31
 #<!-- -->13 0x0000750e14f98a8a clang::CXXRecordDecl::getMostRecentDecl() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:546:3
 #<!-- -->14 0x0000750e14f9899e clang::CXXRecordDecl::dataPtr() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:458:12
 #<!-- -->15 0x0000750e14f989c4 clang::CXXRecordDecl::data() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:462:23
 #<!-- -->16 0x0000750e1504cc8c clang::CXXRecordDecl::bases_begin() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:625:73
 #<!-- -->17 0x0000750e1504ccaf clang::CXXRecordDecl::bases_end() const /opt/llvm-project/llvm/../clang/include/clang/AST/DeclCXX.h:628:23
 #<!-- -->18 0x0000750e185cdf3d clang::CXXRecordDecl::bases() const /opt/llvm-project/clang/include/clang/AST/DeclCXX.h:621:12
 #<!-- -->19 0x0000750e1cecc6aa (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:188:51
 #<!-- -->20 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->21 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->22 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->23 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->24 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->25 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->26 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->27 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->28 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->29 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->30 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->31 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->32 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->33 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->34 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->35 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->36 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->37 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->38 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->39 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->40 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->41 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->42 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->43 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->44 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->45 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->46 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->47 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->48 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->49 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->50 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->51 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->52 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->53 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->54 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->55 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->56 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->57 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->58 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->59 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->60 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->61 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->62 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->63 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->64 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->65 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->66 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->67 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->68 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->69 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->70 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->71 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->72 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->73 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->74 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->75 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->76 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->77 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->78 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->79 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->80 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->81 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->82 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->83 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->84 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->85 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->86 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->87 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->88 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->89 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->90 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->91 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->92 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->93 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->94 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->95 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->96 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
 #<!-- -->97 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
 #<!-- -->98 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
 #<!-- -->99 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->100 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->101 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->102 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->103 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->104 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->105 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->106 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->107 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->108 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->109 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->110 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->111 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->112 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->113 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->114 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->115 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->116 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->117 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->118 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->119 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->120 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->121 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->122 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->123 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->124 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->125 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->126 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->127 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->128 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->129 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->130 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->131 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->132 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->133 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->134 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->135 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->136 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->137 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->138 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->139 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->140 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->141 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->142 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->143 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->144 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->145 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->146 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->147 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->148 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->149 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->150 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->151 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->152 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->153 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->154 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->155 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->156 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->157 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->158 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->159 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->160 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->161 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->162 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->163 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->164 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->165 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->166 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->167 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->168 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->169 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->170 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->171 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->172 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->173 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->174 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->175 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->176 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->177 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->178 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->179 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->180 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->181 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->182 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->183 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->184 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->185 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->186 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->187 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->188 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->189 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->190 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->191 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->192 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->193 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->194 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->195 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->196 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->197 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->198 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->199 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->200 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->201 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->202 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->203 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->204 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->205 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->206 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->207 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->208 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->209 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->210 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->211 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->212 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->213 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->214 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->215 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->216 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->217 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->218 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->219 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->220 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->221 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->222 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->223 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->224 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->225 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->226 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->227 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->228 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->229 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->230 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->231 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->232 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->233 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->234 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->235 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->236 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->237 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->238 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->239 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->240 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->241 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->242 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->243 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->244 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->245 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->246 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->247 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->248 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->249 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->250 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->251 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->252 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
#<!-- -->253 0x0000750e1cecc8a4 (anonymous namespace)::EmptySubobjectMap::ComputeEmptySubobjectSizes() /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:216:63
#<!-- -->254 0x0000750e1cecc680 (anonymous namespace)::EmptySubobjectMap::EmptySubobjectMap(clang::ASTContext const&amp;, clang::CXXRecordDecl const*) /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:172:3
#<!-- -->255 0x0000750e1ced78b8 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const /opt/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3418:65
Segmentation fault (core dumped)
```
</details>


---

### Comment 2 - Walnut356

I think I'm getting this same crash, and it looks like it has something to do with nested enums. CodeLLDB is reporting a stackoverflow for me and my minimal reproducible example is this:

```rust
pub enum B {
    B(u8),
}

enum A {
    A(B),
}

fn main() {
    let a = A::A(B::B(100));

    println!("Hello world!");
}
```

CodeLLDB is currently built on LLDB 20.1.4, but I experience the same crash in standlone LLDB  21.1.3. 

I am currently on Windows 10, this crash occurs for `x86_64-windows-gnu` but not `x86_64-windows-msvc`

---

### Comment 3 - Michael137

> I am currently on Windows 10, this crash occurs for x86_64-windows-gnu but not x86_64-windows-msvc

Is this using DWARF?

---

### Comment 4 - Walnut356

`x86_64-windows-gnu` uses DWARF

---

### Comment 5 - Michael137

Yea that makes sense. The PDB plugin isn't aware of Rust at all (AFAIK), whereas the DWARF plugin tries to parse the DWARF tags for rust variants and make the AST nodes look like C++ structs. We probably create some invalid `CXXRecordDecl` here. Would be interesting to see if this reproduces with a debug-build that has this assert: https://github.com/llvm/llvm-project/pull/154134

This was motivated by the same kind of recursion (the root cause was malformed DWARF however)

---

### Comment 6 - brosenfi

Thanks much for the minimal reproduction @Walnut356 - I tried that locally and it definitely looks like the same issue.  It must be when the enum and members are the same name.  I slightly modified the code to have different enum vs. member names and no issue:
```rust
pub enum B1B1B1B1 {
    B(u8),
}

enum A1A1A1A1 {
    A(B1B1B1B1),
}

fn main() {
    let a = A1A1A1A1::A(B1B1B1B1::B(100));

    println!("Hello world!");
}
```
The llvm-dwarfdump output looks about like I would think it should look with my limited knowledge of dwarf (comparing the [working](https://github.com/brosenfi/test-tokenizers-codelldb-debugging-issue/blob/main/test_issue_llvm_dwarf_dump-no_issue.txt) and [problem](https://github.com/brosenfi/test-tokenizers-codelldb-debugging-issue/blob/main/test_issue_llvm_dwarf_dump-with_issue.txt) dump text).

---

### Comment 7 - imkiva

In `void DWARFASTParserClang::ParseRustVariantPart` I dumped the `decl_context` generated with probmantic code:

```rust
pub enum B {
    B(u8),
}

enum A {
    A(B),
}
```

```
Thread 1 "lldb" hit Breakpoint 4, DWARFASTParserClang::ParseRustVariantPart (this=0x55555672e650, die=..., parent_die=..., class_clang_type=..., default_accesibility=lldb::eAccessPublic, layout_info=...) at /home/kiva/upstream/llvm-upstream/lldb/source/Plugins/SymbolFile/DWARF/DWARFASTParserClang.cpp:3922
3922	  layout_info.field_offsets.insert({inner_field, 0});
The target architecture is set to "auto" (currently "i386:x86-64").
(gdb) p decl_context->dumpAsDecl()
$1 = void

CXXRecordDecl 0x5555568d5760 <<invalid sloc>> <invalid sloc> struct A
|-CXXRecordDecl 0x5555568d58a0 <<invalid sloc>> <invalid sloc> union test_issue::A$Inner definition
| |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable trivial has_variant_members
| | `-Destructor simple irrelevant trivial needs_implicit
| |-PackedAttr 0x5555568d59d8 <<invalid sloc>> Implicit
| |-CXXRecordDecl 0x5555568d5a38 <<invalid sloc>> <invalid sloc> struct A$Variant definition
| | |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable trivial has_constexpr_non_copy_move_ctor can_const_default_init
| | | `-Destructor simple irrelevant trivial needs_implicit
| | `-FieldDecl 0x5555568d5b70 <<invalid sloc>> <invalid sloc> value 'test_issue::A'
| `-FieldDecl 0x5555568d5be0 <<invalid sloc>> <invalid sloc> $variant$ 'test_issue::A::test_issue::A$Inner::A$Variant'
`-FieldDecl 0x5555568d5c50 <<invalid sloc>> <invalid sloc> $variants$ 'test_issue::A::test_issue::A$Inner'

```

I noticed in `A$Variant` the Field is referencing to `test_issue::A` which caused the cyclic reference, while it should be referring to `test_issue::A::A` according to dwarfdump.

As comparison, the CXXRecordDecl from the following code

```rust
pub enum BB {
    B(u8),
}

enum AA {
    A(BB),
}
```

```
CXXRecordDecl 0x5555568d5970 <<invalid sloc>> <invalid sloc> struct AA
|-CXXRecordDecl 0x5555568d5ab0 <<invalid sloc>> <invalid sloc> union test_issue::AA$Inner definition
| |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable trivial has_variant_members
| | `-Destructor simple irrelevant trivial needs_implicit
| |-PackedAttr 0x5555568d5be8 <<invalid sloc>> Implicit
| |-CXXRecordDecl 0x5555568d5d18 <<invalid sloc>> <invalid sloc> struct A$Variant definition
| | |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable trivial
| | | `-Destructor simple irrelevant trivial needs_implicit
| | `-FieldDecl 0x555555a77880 <<invalid sloc>> <invalid sloc> value 'test_issue::AA::A'
| `-FieldDecl 0x555555a778f0 <<invalid sloc>> <invalid sloc> $variant$ 'test_issue::AA::test_issue::AA$Inner::A$Variant'
|-CXXRecordDecl 0x5555568d5c48 <<invalid sloc>> <invalid sloc> struct A definition
| |-DefinitionData pass_in_registers standard_layout trivially_copyable trivial
| | `-Destructor simple irrelevant trivial needs_implicit
| |-AccessSpecDecl 0x555555a77838 <<invalid sloc>> <invalid sloc> private
| `-FieldDecl 0x555555a777e0 <<invalid sloc>> <invalid sloc> __0 'test_issue::BB'
`-FieldDecl 0x555555a77960 <<invalid sloc>> <invalid sloc> $variants$ 'test_issue::AA::test_issue::AA$Inner'
```


---

### Comment 8 - imkiva

I found the cause. In DWARFASTParserClang.cpp function `ParseStructureLikeDIE` was called when parsing the type `test_issue::A::A` from dwarf:

```c++
DWARFASTParserClang::ParseStructureLikeDIE(const SymbolContext &sc,
                                           const DWARFDIE &die,
                                           ParsedDWARFTypeAttributes &attrs) {
  // ...

  if (attrs.name) {
    GetUniqueTypeNameAndDeclaration(die, cu_language, unique_typename,
                                    unique_decl);
```

But `GetUniqueTypeNameAndDeclaration` didn't return the correct unique fully qualiied name for `test_issue::A::A`, instead it returned `A` which is ambiguous with other types like `test_issue::A`.

```c++
void DWARFASTParserClang::GetUniqueTypeNameAndDeclaration(
    const lldb_private::plugin::dwarf::DWARFDIE &die,
    lldb::LanguageType language, lldb_private::ConstString &unique_typename,
    lldb_private::Declaration &decl_declaration) {
  // For C++, we rely solely upon the one definition rule that says
  // only one thing can exist at a given decl context. We ignore the
  // file and line that things are declared on.
  if (!die.IsValid() || !Language::LanguageIsCPlusPlus(language) || unique_typename.IsEmpty())
    return;
```

And the result is amazing, `GetUniqueTypeNameAndDeclaration` simply failed to compute the qualified name as `!Language::LanguageIsCPlusPlus(language)` evaluated to `true`.

I composed a PR that adding a check for Rust, since Rust and C++ have similar rules about how they generate qualified names. But I am not sure if this is good enough or should be done in Rust way.
Please help me review

---

### Comment 9 - Michael137

Closed by https://github.com/llvm/llvm-project/pull/165840

---

### Comment 10 - Michael137

Please re-open if this still happens

---

### Comment 11 - brosenfi

Thanks very much for the fix @imkiva !

---

