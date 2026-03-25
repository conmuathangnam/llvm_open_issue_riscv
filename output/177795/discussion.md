# Crash in PrecompiledPreamble::Build when BeginSourceFile fails after CreateASTConsumer succeeds

**Author:** ddkilzer
**URL:** https://github.com/llvm/llvm-project/issues/177795
**Status:** Open
**Labels:** crash, clang:PCH

## Body

### Description

When building a precompiled preamble with `StoreInMemory=false` and `ChainedIncludes` set to a non-existent file, clang crashes during cleanup. Building with AddressSanitizer is required to reliably reproduce the crashes.

#### Bug 1: Crash in `PrecompilePreambleAction` cleanup

When `FrontendAction::BeginSourceFile` fails after `PrecompilePreambleAction::CreateASTConsumer` has succeeded, the `FailureCleanup` lambda calls `CI.clearOutputFiles()`. This frees the `OutputFile` that was created via `GeneratePCHAction::CreateOutputFile`. However, `PrecompilePreambleAction` holds a `TrackedProxy` stream (`FileOS`) that references this `OutputFile`. When `PrecompilePreambleAction` is destroyed, the `TrackedProxy` destructor attempts to access the already-freed `OutputFile`, causing a crash in `llvm::vfs::OutputFile::TrackedProxy::~TrackedProxy()`.

#### Regression:
- PR #113364
- Commit 7dd63e18b604

#### Bug 2: Crash in `createChainedIncludesSource` cleanup

After fixing Bug 1, ASan reveals a second crash. In `createChainedIncludesSource`, temporary `CompilerInstance` objects are created by copying the main `CompilerInvocation`. This copy includes `PreprocessorOpts.RemappedFileBuffers`, which contains raw pointers to `MemoryBuffer` objects owned by the caller. When a temporary `CompilerInstance` is destroyed (e.g., due to failure to read a chained include file), its `SourceManager` destructor may access or affect these shared buffer pointers. Later, when the main `CompilerInstance` is destroyed, its `SourceManager` attempts to access the same memory, causing a crash in `clang::SourceManager::~SourceManager()`.

#### Regression:
- Latent bug from 2011 in `createChainedIncludesSource` that never cleared `RemappedFileBuffers` when copying `CompilerInvocation`
- Became reproducible in June 2017 when `PrecompiledPreamble::Build` was introduced, which adds a `RemappedFileBuffer` before calling `BeginSourceFile`
- Commit 200b3289726f "Moved code handling precompiled preamble out of the ASTUnit."
- Differential Revision: https://reviews.llvm.org/D34287

### Reproduction

A test case is attached as a patch. The test:
1. Creates a preamble build with `StoreInMemory=false` (triggers file-based PCH output)
2. Sets `ChainedIncludes` to a non-existent path to force `BeginSourceFile` failure
3. Expects the preamble build to fail gracefully without crashing

To reproduce, build with `-DLLVM_USE_SANITIZER=Address` and run:
```
./tools/clang/unittests/AllClangUnitTests --gtest_filter="*PreambleBuildFailureCleanup*"
```

### Notes

This description and the test case were written with the assistance of Claude Code.

rdar://165233002


## Comments

### Comment 1 - ddkilzer

Repro test:  [llvm-issue-177795-test.patch](https://github.com/user-attachments/files/24839875/llvm-issue-177795-test.patch)

---

### Comment 2 - ddkilzer

Cc @cachemeifyoucan for Bug 1

---

