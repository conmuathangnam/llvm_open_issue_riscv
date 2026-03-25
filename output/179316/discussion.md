# [clang] coverage: malformed profile generated from macro expansion from system header

**Author:** mizvekov
**URL:** https://github.com/llvm/llvm-project/issues/179316
**Status:** Open
**Labels:** clang:codegen, coverage, crash

## Body

The implementation fails to deal with macro expansions where these macros come from system headers, where coverage is set by default to ignore coverage on those headers.

Reproducer:
```C++
/// test.cc
int main() {
#define X ;
#include <test.hh>
}

/// test.hh
#define Y X
Y
```

Compiled with:
```sh
clang++ -isystem . -fprofile-instr-generate -fcoverage-mapping test.cc
```

Will fail with the following assertion:
```
Assertion failed: (SystemHeadersCoverage || !SM.isInSystemHeader(SM.getSpellingLoc(Loc))), function gatherFileIDs, file CoverageMappingGen.cpp, line 405.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
1.	<eof> parser at end of file
2.	ASTVisitor.cpp:1:5: LLVM IR generation of declaration 'main'
3.	ASTVisitor.cpp:1:5: Generating code for declaration 'main'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-23                 0x0000000102f8dd30 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 56
1  clang-23                 0x0000000102f8bc94 llvm::sys::RunSignalHandlers() + 172
2  clang-23                 0x0000000102f8e7d8 SignalHandler(int, __siginfo*, void*) + 300
3  libsystem_platform.dylib 0x00000001983f3744 _sigtramp + 56
4  libsystem_pthread.dylib  0x00000001983e9888 pthread_kill + 296
5  libsystem_c.dylib        0x00000001982ee850 abort + 124
6  libsystem_c.dylib        0x00000001982eda84 err + 0
7  clang-23                 0x00000001059deb74 (anonymous namespace)::CoverageMappingBuilder::emitSourceRegions(llvm::SmallSet<std::__1::pair<clang::SourceLocation, clang::SourceLocation>, 8u, std::__1::less<std::__1::pair<clang::SourceLocation, clang::SourceLocation>>> const&) (.cold.1) + 0
8  clang-23                 0x00000001036b22f4 (anonymous namespace)::CoverageMappingBuilder::gatherFileIDs(llvm::SmallVectorImpl<unsigned int>&) + 1916
9  clang-23                 0x00000001036a9598 clang::CodeGen::CoverageMappingGen::emitCounterMapping(clang::Decl const*, llvm::raw_ostream&) + 732
10 clang-23                 0x0000000103687208 clang::CodeGen::CodeGenPGO::emitCounterRegionMapping(clang::Decl const*) + 364
11 clang-23                 0x0000000103686d44 clang::CodeGen::CodeGenPGO::assignRegionCounters(clang::GlobalDecl, llvm::Function*) + 404
12 clang-23                 0x0000000103644a68 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) + 868
13 clang-23                 0x0000000103665f04 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 460
14 clang-23                 0x000000010365fc94 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 276
15 clang-23                 0x00000001036639cc clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 1548
16 clang-23                 0x000000010365e8f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) + 268
17 clang-23                 0x00000001036d749c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) + 144
18 clang-23                 0x0000000103636738 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) + 192
19 clang-23                 0x000000010449e260 clang::ParseAST(clang::Sema&, bool, bool) + 640
20 clang-23                 0x00000001039aa1a0 clang::FrontendAction::Execute() + 48
21 clang-23                 0x0000000103949154 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 876
22 clang-23                 0x0000000103a2694c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 704
23 clang-23                 0x000000010085d798 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 1424
24 clang-23                 0x000000010085ba5c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) + 1000
25 clang-23                 0x000000010085b320 clang_main(int, char**, llvm::ToolContext const&) + 5628
26 clang-23                 0x0000000100866c30 main + 88
27 dyld                     0x0000000198021d54 start + 7184
```

Absent the assertions, the generated profile will not be valid, and llvm-cov will fail to process it, producing a "truncated coverage data" error.

A simpler way to reproduce the assertion, without a separate header file: https://godbolt.org/z/vW4bs5vcz
```C++
int main() {
#define X ;
# 1 "./DeclNodes.inc" 1 3
#define Y X
Y
}
```

Related to https://github.com/llvm/llvm-project/pull/91446 and https://github.com/llvm/llvm-project/issues/78920

FYI @chapuni 

## Comments

### Comment 1 - chapuni

Likely a bug. Let me investigate.

---

### Comment 2 - Nuullll

It looks like I'm 5 hours late to the party! I was just about to file this same issue after spending a whole day chasing down the "truncated coverage data" error when enabling coverage for llvm codebase. 

---

### Comment 3 - Nuullll

FWIW, the following patch worked for me:
```diff
diff --git a/clang/lib/CodeGen/CoverageMappingGen.cpp b/clang/lib/CodeGen/CoverageMappingGen.cpp
index 67a9caf8b4ec..a4b1461249fc 100644
--- a/clang/lib/CodeGen/CoverageMappingGen.cpp
+++ b/clang/lib/CodeGen/CoverageMappingGen.cpp
@@ -402,8 +402,14 @@ public:
     if (!Visited.insert(File).second)
       continue;

-    assert(SystemHeadersCoverage ||
-           !SM.isInSystemHeader(SM.getSpellingLoc(Loc)));
+    // Ignore regions from system headers unless collecting coverage from
+    // system headers is explicitly enabled.
+    if (!SystemHeadersCoverage &&
+        SM.isInSystemHeader(SM.getSpellingLoc(Loc))) {
+      assert(!Region.isMCDCBranch() && !Region.isMCDCDecision() &&
+             "Don't suppress the condition in system headers");
+      continue;
+    }

     unsigned Depth = 0;
     for (SourceLocation Parent = getIncludeOrExpansionLoc(Loc);
```

---

