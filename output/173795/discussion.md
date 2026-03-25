# [clangsa] crash while analysing certain funcs from QEMU source code

**Author:** VisualEhrmanntraut
**URL:** https://github.com/llvm/llvm-project/issues/173795
**Status:** Closed
**Labels:** clang:static analyzer, crash
**Closed Date:** 2026-01-05T11:16:17Z

## Body

in particular: mulu128, udiv256_qrnnd, divu256, divs256
```
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21 -cc1 -triple arm64-apple-macosx26.0.0 -O2 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -analyze -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name host-utils.c -analyzer-checker=core -analyzer-checker=apiModeling -analyzer-checker=unix -analyzer-checker=osx -analyzer-checker=security.insecureAPI.decodeValueOfObjCType -analyzer-checker=deadcode -analyzer-checker=security.insecureAPI.UncheckedReturn -analyzer-checker=security.insecureAPI.getpw -analyzer-checker=security.insecureAPI.gets -analyzer-checker=security.insecureAPI.mktemp -analyzer-checker=security.insecureAPI.mkstemp -analyzer-checker=security.insecureAPI.vfork -analyzer-checker=nullability.NullPassedToNonnull -analyzer-checker=nullability.NullReturnedFromNonnull -analyzer-output plist -w -setup-static-analyzer -analyzer-config-compatibility-mode=true -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -relaxed-aliasing -ffp-contract=on -fno-rounding-math -funwind-tables=1 -target-sdk-version=26.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debugger-tuning=lldb -fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -target-linker-version 1230.1 -fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -D _GNU_SOURCE -D _FILE_OFFSET_BITS=64 -D _LARGEFILE_SOURCE -D NCURSES_WIDECHAR=1 -Wno-gnu-variable-sized-type-not-at-end -Wno-initializer-overrides -Wno-missing-include-dirs -Wno-psabi -Wno-shift-negative-value -Wno-string-plus-int -Wno-tautological-type-limit-compare -Wno-typedef-redefinition -std=gnu11 -ferror-limit 19 -fzero-call-used-regs=used-gpr -fwrapv -stack-protector 2 -ftrivial-auto-var-init=zero -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fmax-type-align=16 -vectorize-loops -vectorize-slp -analyzer-opt-analyze-headers -analyzer-output=plist-multi-file -analyzer-config expand-macros=true -analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated -analyzer-config aggressive-binary-operation-simplification=true -analyzer-config crosscheck-with-z3=true -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c host-utils-6257f4.c
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling mulu128 at line 121014
	#1 Calling udiv256_qrnnd at line 121092
	#2 Calling divu256 at line 121148
	#3 Calling divs256
3.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
4.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
 #0 0x0000000117aa9b64 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e49b64)
 #1 0x0000000117aaa394 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e4a394)
 #2 0x00000001933e0764 (/usr/lib/system/libsystem_platform.dylib+0x1804e4764)
 #3 0x0000000108765148 clang::ento::SymExprVisitor<(anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>, clang::ento::SVal)::Simplifier, clang::ento::SVal>::Visit(clang::ento::SymExpr const*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c01148)
 #4 0x0000000108764c30 (anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c00c30)
 #5 0x0000000108762ab8 (anonymous namespace)::SimpleSValBuilder::simplifySVal(llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfeab8)
 #6 0x000000010875eba0 (anonymous namespace)::SimpleSValBuilder::evalBinOpNN(llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>, clang::BinaryOperatorKind, clang::ento::NonLoc, clang::ento::NonLoc, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfaba0)
 #7 0x000000010876a908 clang::ento::SValBuilder::evalBinOp(llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>, clang::BinaryOperatorKind, clang::ento::SVal, clang::ento::SVal, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c06908)
 #8 0x000000010934f9b8 (anonymous namespace)::BuiltinFunctionChecker::handleOverflowBuiltin(clang::ento::CallEvent const&, clang::ento::CheckerContext&, clang::BinaryOperatorKind, clang::QualType) const (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x27eb9b8)
 #9 0x000000010878e5d0 bool clang::ento::eval::Call::_evalCall<(anonymous namespace)::BuiltinFunctionChecker>(void*, clang::ento::CallEvent const&, clang::ento::CheckerContext&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c2a5d0)
#10 0x000000010869cbd4 clang::ento::CheckerManager::runCheckersForEvalCall(clang::ento::ExplodedNodeSet&, clang::ento::ExplodedNodeSet const&, clang::ento::CallEvent const&, clang::ento::ExprEngine&, clang::ento::EvalCallOptions const&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b38bd4)
#11 0x00000001086f7b40 clang::ento::ExprEngine::evalCall(clang::ento::ExplodedNodeSet&, clang::ento::ExplodedNode*, clang::ento::CallEvent const&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b93b40)
#12 0x00000001086f785c clang::ento::ExprEngine::VisitCallExpr(clang::CallExpr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b9385c)
#13 0x00000001086c3590 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5f590)
#14 0x00000001086c0120 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5c120)
#15 0x00000001086a1f2c clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3df2c)
#16 0x00000001086a19bc clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3d9bc)
#17 0x0000000108976d14 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e12d14)
#18 0x0000000108975530 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e11530)
#19 0x0000000106d3bd88 clang::ParseAST(clang::Sema&, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1d7d88)
#20 0x0000000108581548 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a1d548)
#21 0x0000000108518ab4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x19b4ab4)
#22 0x00000001085b67fc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a527fc)
#23 0x0000000102a053a0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x1000053a0)
#24 0x0000000102a03550 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100003550)
#25 0x0000000102a01ad4 clang_main(int, char**, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100001ad4)
#26 0x0000000102a0d81c main (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x10000d81c)
#27 0x000000019300dd54
./host-utils-6257f4.sh: line 4: 10990 Segmentation fault: 11  "/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21" "-cc1" "-triple" "arm64-apple-macosx26.0.0" "-O2" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-analyze" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "host-utils.c" "-analyzer-checker=core" "-analyzer-checker=apiModeling" "-analyzer-checker=unix" "-analyzer-checker=osx" "-analyzer-checker=security.insecureAPI.decodeValueOfObjCType" "-analyzer-checker=deadcode" "-analyzer-checker=security.insecureAPI.UncheckedReturn" "-analyzer-checker=security.insecureAPI.getpw" "-analyzer-checker=security.insecureAPI.gets" "-analyzer-checker=security.insecureAPI.mktemp" "-analyzer-checker=security.insecureAPI.mkstemp" "-analyzer-checker=security.insecureAPI.vfork" "-analyzer-checker=nullability.NullPassedToNonnull" "-analyzer-checker=nullability.NullReturnedFromNonnull" "-analyzer-output" "plist" "-w" "-setup-static-analyzer" "-analyzer-config-compatibility-mode=true" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-relaxed-aliasing" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=26.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-enable-tlsdesc" "-target-cpu" "apple-m1" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-target-linker-version" "1230.1" "-fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-D" "_GNU_SOURCE" "-D" "_FILE_OFFSET_BITS=64" "-D" "_LARGEFILE_SOURCE" "-D" "NCURSES_WIDECHAR=1" "-Wno-gnu-variable-sized-type-not-at-end" "-Wno-initializer-overrides" "-Wno-missing-include-dirs" "-Wno-psabi" "-Wno-shift-negative-value" "-Wno-string-plus-int" "-Wno-tautological-type-limit-compare" "-Wno-typedef-redefinition" "-std=gnu11" "-ferror-limit" "19" "-fzero-call-used-regs=used-gpr" "-fwrapv" "-stack-protector" "2" "-ftrivial-auto-var-init=zero" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fmax-type-align=16" "-vectorize-loops" "-vectorize-slp" "-analyzer-opt-analyze-headers" "-analyzer-output=plist-multi-file" "-analyzer-config" "expand-macros=true" "-analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated" "-analyzer-config" "aggressive-binary-operation-simplification=true" "-analyzer-config" "crosscheck-with-z3=true" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "host-utils-6257f4.c"
```

clang tidy trace omitted as it's identical
initiated using CodeChecker as suggested by Clang docs, ~1k files failed analysis because of this issue

[host-utils-6257f4.c](https://github.com/user-attachments/files/24361565/host-utils-6257f4.c)
[host-utils-6257f4.sh](https://github.com/user-attachments/files/24361579/host-utils-6257f4.sh)
[clang-21-2025-12-28-211637.ips.txt](https://github.com/user-attachments/files/24361599/clang-21-2025-12-28-211637.ips.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Visual Ehrmanntraut (VisualEhrmanntraut)

<details>
in particular: mulu128, udiv256_qrnnd, divu256, divs256
```
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21 -cc1 -triple arm64-apple-macosx26.0.0 -O2 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -analyze -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name host-utils.c -analyzer-checker=core -analyzer-checker=apiModeling -analyzer-checker=unix -analyzer-checker=osx -analyzer-checker=security.insecureAPI.decodeValueOfObjCType -analyzer-checker=deadcode -analyzer-checker=security.insecureAPI.UncheckedReturn -analyzer-checker=security.insecureAPI.getpw -analyzer-checker=security.insecureAPI.gets -analyzer-checker=security.insecureAPI.mktemp -analyzer-checker=security.insecureAPI.mkstemp -analyzer-checker=security.insecureAPI.vfork -analyzer-checker=nullability.NullPassedToNonnull -analyzer-checker=nullability.NullReturnedFromNonnull -analyzer-output plist -w -setup-static-analyzer -analyzer-config-compatibility-mode=true -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -relaxed-aliasing -ffp-contract=on -fno-rounding-math -funwind-tables=1 -target-sdk-version=26.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debugger-tuning=lldb -fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -target-linker-version 1230.1 -fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -D _GNU_SOURCE -D _FILE_OFFSET_BITS=64 -D _LARGEFILE_SOURCE -D NCURSES_WIDECHAR=1 -Wno-gnu-variable-sized-type-not-at-end -Wno-initializer-overrides -Wno-missing-include-dirs -Wno-psabi -Wno-shift-negative-value -Wno-string-plus-int -Wno-tautological-type-limit-compare -Wno-typedef-redefinition -std=gnu11 -ferror-limit 19 -fzero-call-used-regs=used-gpr -fwrapv -stack-protector 2 -ftrivial-auto-var-init=zero -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fmax-type-align=16 -vectorize-loops -vectorize-slp -analyzer-opt-analyze-headers -analyzer-output=plist-multi-file -analyzer-config expand-macros=true -analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated -analyzer-config aggressive-binary-operation-simplification=true -analyzer-config crosscheck-with-z3=true -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c host-utils-6257f4.c
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling mulu128 at line 121014
	#<!-- -->1 Calling udiv256_qrnnd at line 121092
	#<!-- -->2 Calling divu256 at line 121148
	#<!-- -->3 Calling divs256
3.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
4.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
 #<!-- -->0 0x0000000117aa9b64 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e49b64)
 #<!-- -->1 0x0000000117aaa394 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e4a394)
 #<!-- -->2 0x00000001933e0764 (/usr/lib/system/libsystem_platform.dylib+0x1804e4764)
 #<!-- -->3 0x0000000108765148 clang::ento::SymExprVisitor&lt;(anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal)::Simplifier, clang::ento::SVal&gt;::Visit(clang::ento::SymExpr const*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c01148)
 #<!-- -->4 0x0000000108764c30 (anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c00c30)
 #<!-- -->5 0x0000000108762ab8 (anonymous namespace)::SimpleSValBuilder::simplifySVal(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfeab8)
 #<!-- -->6 0x000000010875eba0 (anonymous namespace)::SimpleSValBuilder::evalBinOpNN(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::BinaryOperatorKind, clang::ento::NonLoc, clang::ento::NonLoc, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfaba0)
 #<!-- -->7 0x000000010876a908 clang::ento::SValBuilder::evalBinOp(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::BinaryOperatorKind, clang::ento::SVal, clang::ento::SVal, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c06908)
 #<!-- -->8 0x000000010934f9b8 (anonymous namespace)::BuiltinFunctionChecker::handleOverflowBuiltin(clang::ento::CallEvent const&amp;, clang::ento::CheckerContext&amp;, clang::BinaryOperatorKind, clang::QualType) const (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x27eb9b8)
 #<!-- -->9 0x000000010878e5d0 bool clang::ento::eval::Call::_evalCall&lt;(anonymous namespace)::BuiltinFunctionChecker&gt;(void*, clang::ento::CallEvent const&amp;, clang::ento::CheckerContext&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c2a5d0)
#<!-- -->10 0x000000010869cbd4 clang::ento::CheckerManager::runCheckersForEvalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNodeSet const&amp;, clang::ento::CallEvent const&amp;, clang::ento::ExprEngine&amp;, clang::ento::EvalCallOptions const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b38bd4)
#<!-- -->11 0x00000001086f7b40 clang::ento::ExprEngine::evalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNode*, clang::ento::CallEvent const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b93b40)
#<!-- -->12 0x00000001086f785c clang::ento::ExprEngine::VisitCallExpr(clang::CallExpr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b9385c)
#<!-- -->13 0x00000001086c3590 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5f590)
#<!-- -->14 0x00000001086c0120 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5c120)
#<!-- -->15 0x00000001086a1f2c clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3df2c)
#<!-- -->16 0x00000001086a19bc clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3d9bc)
#<!-- -->17 0x0000000108976d14 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e12d14)
#<!-- -->18 0x0000000108975530 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e11530)
#<!-- -->19 0x0000000106d3bd88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1d7d88)
#<!-- -->20 0x0000000108581548 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a1d548)
#<!-- -->21 0x0000000108518ab4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x19b4ab4)
#<!-- -->22 0x00000001085b67fc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a527fc)
#<!-- -->23 0x0000000102a053a0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x1000053a0)
#<!-- -->24 0x0000000102a03550 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100003550)
#<!-- -->25 0x0000000102a01ad4 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100001ad4)
#<!-- -->26 0x0000000102a0d81c main (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x10000d81c)
#<!-- -->27 0x000000019300dd54
./host-utils-6257f4.sh: line 4: 10990 Segmentation fault: 11  "/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21" "-cc1" "-triple" "arm64-apple-macosx26.0.0" "-O2" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-analyze" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "host-utils.c" "-analyzer-checker=core" "-analyzer-checker=apiModeling" "-analyzer-checker=unix" "-analyzer-checker=osx" "-analyzer-checker=security.insecureAPI.decodeValueOfObjCType" "-analyzer-checker=deadcode" "-analyzer-checker=security.insecureAPI.UncheckedReturn" "-analyzer-checker=security.insecureAPI.getpw" "-analyzer-checker=security.insecureAPI.gets" "-analyzer-checker=security.insecureAPI.mktemp" "-analyzer-checker=security.insecureAPI.mkstemp" "-analyzer-checker=security.insecureAPI.vfork" "-analyzer-checker=nullability.NullPassedToNonnull" "-analyzer-checker=nullability.NullReturnedFromNonnull" "-analyzer-output" "plist" "-w" "-setup-static-analyzer" "-analyzer-config-compatibility-mode=true" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-relaxed-aliasing" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=26.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-enable-tlsdesc" "-target-cpu" "apple-m1" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-target-linker-version" "1230.1" "-fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-D" "_GNU_SOURCE" "-D" "_FILE_OFFSET_BITS=64" "-D" "_LARGEFILE_SOURCE" "-D" "NCURSES_WIDECHAR=1" "-Wno-gnu-variable-sized-type-not-at-end" "-Wno-initializer-overrides" "-Wno-missing-include-dirs" "-Wno-psabi" "-Wno-shift-negative-value" "-Wno-string-plus-int" "-Wno-tautological-type-limit-compare" "-Wno-typedef-redefinition" "-std=gnu11" "-ferror-limit" "19" "-fzero-call-used-regs=used-gpr" "-fwrapv" "-stack-protector" "2" "-ftrivial-auto-var-init=zero" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fmax-type-align=16" "-vectorize-loops" "-vectorize-slp" "-analyzer-opt-analyze-headers" "-analyzer-output=plist-multi-file" "-analyzer-config" "expand-macros=true" "-analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated" "-analyzer-config" "aggressive-binary-operation-simplification=true" "-analyzer-config" "crosscheck-with-z3=true" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "host-utils-6257f4.c"
```

clang tidy trace omitted as it's identical
initiated using CodeChecker as suggested by Clang docs, ~1k files failed analysis because of this issue

[host-utils-6257f4.c](https://github.com/user-attachments/files/24361565/host-utils-6257f4.c)
[host-utils-6257f4.sh](https://github.com/user-attachments/files/24361579/host-utils-6257f4.sh)
[clang-21-2025-12-28-211637.ips.txt](https://github.com/user-attachments/files/24361599/clang-21-2025-12-28-211637.ips.txt)
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: Visual Ehrmanntraut (VisualEhrmanntraut)

<details>
in particular: mulu128, udiv256_qrnnd, divu256, divs256
```
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21 -cc1 -triple arm64-apple-macosx26.0.0 -O2 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -analyze -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name host-utils.c -analyzer-checker=core -analyzer-checker=apiModeling -analyzer-checker=unix -analyzer-checker=osx -analyzer-checker=security.insecureAPI.decodeValueOfObjCType -analyzer-checker=deadcode -analyzer-checker=security.insecureAPI.UncheckedReturn -analyzer-checker=security.insecureAPI.getpw -analyzer-checker=security.insecureAPI.gets -analyzer-checker=security.insecureAPI.mktemp -analyzer-checker=security.insecureAPI.mkstemp -analyzer-checker=security.insecureAPI.vfork -analyzer-checker=nullability.NullPassedToNonnull -analyzer-checker=nullability.NullReturnedFromNonnull -analyzer-output plist -w -setup-static-analyzer -analyzer-config-compatibility-mode=true -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -relaxed-aliasing -ffp-contract=on -fno-rounding-math -funwind-tables=1 -target-sdk-version=26.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debugger-tuning=lldb -fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -target-linker-version 1230.1 -fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build -D _GNU_SOURCE -D _FILE_OFFSET_BITS=64 -D _LARGEFILE_SOURCE -D NCURSES_WIDECHAR=1 -Wno-gnu-variable-sized-type-not-at-end -Wno-initializer-overrides -Wno-missing-include-dirs -Wno-psabi -Wno-shift-negative-value -Wno-string-plus-int -Wno-tautological-type-limit-compare -Wno-typedef-redefinition -std=gnu11 -ferror-limit 19 -fzero-call-used-regs=used-gpr -fwrapv -stack-protector 2 -ftrivial-auto-var-init=zero -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fmax-type-align=16 -vectorize-loops -vectorize-slp -analyzer-opt-analyze-headers -analyzer-output=plist-multi-file -analyzer-config expand-macros=true -analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated -analyzer-config aggressive-binary-operation-simplification=true -analyzer-config crosscheck-with-z3=true -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c host-utils-6257f4.c
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling mulu128 at line 121014
	#<!-- -->1 Calling udiv256_qrnnd at line 121092
	#<!-- -->2 Calling divu256 at line 121148
	#<!-- -->3 Calling divs256
3.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
4.	/Users/visual/Developer/QEMUAppleSilicon/include/qemu/host-utils.h:621:11: Error evaluating statement
 #<!-- -->0 0x0000000117aa9b64 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e49b64)
 #<!-- -->1 0x0000000117aaa394 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3e4a394)
 #<!-- -->2 0x00000001933e0764 (/usr/lib/system/libsystem_platform.dylib+0x1804e4764)
 #<!-- -->3 0x0000000108765148 clang::ento::SymExprVisitor&lt;(anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal)::Simplifier, clang::ento::SVal&gt;::Visit(clang::ento::SymExpr const*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c01148)
 #<!-- -->4 0x0000000108764c30 (anonymous namespace)::SimpleSValBuilder::simplifySValOnce(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c00c30)
 #<!-- -->5 0x0000000108762ab8 (anonymous namespace)::SimpleSValBuilder::simplifySVal(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::ento::SVal) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfeab8)
 #<!-- -->6 0x000000010875eba0 (anonymous namespace)::SimpleSValBuilder::evalBinOpNN(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::BinaryOperatorKind, clang::ento::NonLoc, clang::ento::NonLoc, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1bfaba0)
 #<!-- -->7 0x000000010876a908 clang::ento::SValBuilder::evalBinOp(llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;, clang::BinaryOperatorKind, clang::ento::SVal, clang::ento::SVal, clang::QualType) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c06908)
 #<!-- -->8 0x000000010934f9b8 (anonymous namespace)::BuiltinFunctionChecker::handleOverflowBuiltin(clang::ento::CallEvent const&amp;, clang::ento::CheckerContext&amp;, clang::BinaryOperatorKind, clang::QualType) const (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x27eb9b8)
 #<!-- -->9 0x000000010878e5d0 bool clang::ento::eval::Call::_evalCall&lt;(anonymous namespace)::BuiltinFunctionChecker&gt;(void*, clang::ento::CallEvent const&amp;, clang::ento::CheckerContext&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1c2a5d0)
#<!-- -->10 0x000000010869cbd4 clang::ento::CheckerManager::runCheckersForEvalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNodeSet const&amp;, clang::ento::CallEvent const&amp;, clang::ento::ExprEngine&amp;, clang::ento::EvalCallOptions const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b38bd4)
#<!-- -->11 0x00000001086f7b40 clang::ento::ExprEngine::evalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNode*, clang::ento::CallEvent const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b93b40)
#<!-- -->12 0x00000001086f785c clang::ento::ExprEngine::VisitCallExpr(clang::CallExpr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b9385c)
#<!-- -->13 0x00000001086c3590 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5f590)
#<!-- -->14 0x00000001086c0120 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b5c120)
#<!-- -->15 0x00000001086a1f2c clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3df2c)
#<!-- -->16 0x00000001086a19bc clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1b3d9bc)
#<!-- -->17 0x0000000108976d14 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e12d14)
#<!-- -->18 0x0000000108975530 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1e11530)
#<!-- -->19 0x0000000106d3bd88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1d7d88)
#<!-- -->20 0x0000000108581548 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a1d548)
#<!-- -->21 0x0000000108518ab4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x19b4ab4)
#<!-- -->22 0x00000001085b67fc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libclang-cpp.dylib+0x1a527fc)
#<!-- -->23 0x0000000102a053a0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x1000053a0)
#<!-- -->24 0x0000000102a03550 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100003550)
#<!-- -->25 0x0000000102a01ad4 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x100001ad4)
#<!-- -->26 0x0000000102a0d81c main (/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21+0x10000d81c)
#<!-- -->27 0x000000019300dd54
./host-utils-6257f4.sh: line 4: 10990 Segmentation fault: 11  "/opt/homebrew/Cellar/llvm/21.1.8/bin/clang-21" "-cc1" "-triple" "arm64-apple-macosx26.0.0" "-O2" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-analyze" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "host-utils.c" "-analyzer-checker=core" "-analyzer-checker=apiModeling" "-analyzer-checker=unix" "-analyzer-checker=osx" "-analyzer-checker=security.insecureAPI.decodeValueOfObjCType" "-analyzer-checker=deadcode" "-analyzer-checker=security.insecureAPI.UncheckedReturn" "-analyzer-checker=security.insecureAPI.getpw" "-analyzer-checker=security.insecureAPI.gets" "-analyzer-checker=security.insecureAPI.mktemp" "-analyzer-checker=security.insecureAPI.mkstemp" "-analyzer-checker=security.insecureAPI.vfork" "-analyzer-checker=nullability.NullPassedToNonnull" "-analyzer-checker=nullability.NullReturnedFromNonnull" "-analyzer-output" "plist" "-w" "-setup-static-analyzer" "-analyzer-config-compatibility-mode=true" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-relaxed-aliasing" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=26.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-enable-tlsdesc" "-target-cpu" "apple-m1" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-target-linker-version" "1230.1" "-fcoverage-compilation-dir=/Users/visual/Developer/QEMUAppleSilicon/build" "-D" "_GNU_SOURCE" "-D" "_FILE_OFFSET_BITS=64" "-D" "_LARGEFILE_SOURCE" "-D" "NCURSES_WIDECHAR=1" "-Wno-gnu-variable-sized-type-not-at-end" "-Wno-initializer-overrides" "-Wno-missing-include-dirs" "-Wno-psabi" "-Wno-shift-negative-value" "-Wno-string-plus-int" "-Wno-tautological-type-limit-compare" "-Wno-typedef-redefinition" "-std=gnu11" "-ferror-limit" "19" "-fzero-call-used-regs=used-gpr" "-fwrapv" "-stack-protector" "2" "-ftrivial-auto-var-init=zero" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fmax-type-align=16" "-vectorize-loops" "-vectorize-slp" "-analyzer-opt-analyze-headers" "-analyzer-output=plist-multi-file" "-analyzer-config" "expand-macros=true" "-analyzer-checker=core.BitwiseShift,core.CallAndMessage,core.DivideZero,core.FixedAddressDereference,core.NonNullParamChecker,core.NullDereference,core.StackAddressEscape,core.UndefinedBinaryOperatorResult,core.VLASize,core.uninitialized.ArraySubscript,core.uninitialized.Assign,core.uninitialized.Branch,core.uninitialized.CapturedBlockVariable,core.uninitialized.NewArraySize,core.uninitialized.UndefReturn,cplusplus.ArrayDelete,cplusplus.InnerPointer,cplusplus.Move,cplusplus.NewDelete,cplusplus.NewDeleteLeaks,cplusplus.PlacementNew,cplusplus.PureVirtualCall,cplusplus.StringChecker,deadcode.DeadStores,nullability.NullPassedToNonnull,nullability.NullReturnedFromNonnull,optin.cplusplus.UninitializedObject,optin.cplusplus.VirtualCall,optin.portability.UnixAPI,security.ArrayBound,security.FloatLoopCounter,security.MmapWriteExec,security.PointerSub,security.PutenvStackArray,security.SetgidSetuidOrder,security.cert.env.InvalidPtr,security.insecureAPI.UncheckedReturn,security.insecureAPI.getpw,security.insecureAPI.gets,security.insecureAPI.mkstemp,security.insecureAPI.mktemp,security.insecureAPI.rand,security.insecureAPI.vfork,unix.API,unix.BlockInCriticalSection,unix.Chroot,unix.Malloc,unix.MallocSizeof,unix.MismatchedDeallocator,unix.StdCLibraryFunctions,unix.Stream,unix.Vfork,unix.cstring.BadSizeArg,unix.cstring.NotNullTerminated,unix.cstring.NullArg,valist.CopyToSelf,valist.Uninitialized,valist.Unterminated" "-analyzer-config" "aggressive-binary-operation-simplification=true" "-analyzer-config" "crosscheck-with-z3=true" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "host-utils-6257f4.c"
```

clang tidy trace omitted as it's identical
initiated using CodeChecker as suggested by Clang docs, ~1k files failed analysis because of this issue

[host-utils-6257f4.c](https://github.com/user-attachments/files/24361565/host-utils-6257f4.c)
[host-utils-6257f4.sh](https://github.com/user-attachments/files/24361579/host-utils-6257f4.sh)
[clang-21-2025-12-28-211637.ips.txt](https://github.com/user-attachments/files/24361599/clang-21-2025-12-28-211637.ips.txt)
</details>


---

### Comment 3 - steakhal

By judging the stack trace I think Ive reduced one like this last week.
Do you use 128bit wide variables?

---

### Comment 4 - VisualEhrmanntraut

well, it's not my code, it's from upstream QEMU. but it looks like the answer's yes, because according to that .c it told me to attach, CONFIG_INT128 is set

<img width="751" height="635" alt="Image" src="https://github.com/user-attachments/assets/14001755-de3c-48b0-b4ca-62209f961195" />

---

### Comment 5 - steakhal

Thats gotta be it. Highly likely I'll land a fix 1st week next year. Nevertheless thanks for the report btw.
It would be awesome if you could also try the fix once I have one.

---

### Comment 6 - VisualEhrmanntraut

No problem. So just to confirm, there's no fix on LLVM trunk yet right?

---

### Comment 7 - steakhal

I can confirm that the issue I found last week affects llvm main, even today.
The workaround is to disable the builtinfn checker to sidestep the crash inside it.

---

### Comment 8 - VisualEhrmanntraut

> The workaround is to disable the builtinfn checker to sidestep the crash inside it.

Sorry, not sure how to do that, been searching for the past 20 mins. I would appreciate if you'd elaborate further.

And I'll be on the lookout for testing the fix when it lands as well.

---

### Comment 9 - steakhal

You should disable the `core.builtin.BuiltinFunctions` checker. Usually achieved by `-analyzer-disable-checker=checkername`. However, I dont exactly know how to do this with clang tidy.

---

### Comment 10 - VisualEhrmanntraut

> You should disable the `core.builtin.BuiltinFunctions` checker. Usually achieved by `-analyzer-disable-checker=checkername`.

just found that a few moments ago, but still unsure how to pass this through CodeChecker (at least without overriding all args via `clangsa:cc-verbatim-args-file=...`) :-| 


---

### Comment 11 - VisualEhrmanntraut

> However, I dont exactly know how to do this with clang tidy.

it was because I had the clang analyser group enabled in tidy, so it's afaict purely a clangsa issue :-)

---

### Comment 12 - steakhal

Repro I reduced: https://godbolt.org/z/eEaEqKGhP

```c++
void add(__int128_t a, __int128_t b) {
  __int128_t result = 0;
  (void)__builtin_add_overflow(a, b, &result); // crash
}
```
rdar://166709144

---

### Comment 13 - steakhal

The problem is caused by this code in the `clang/lib/StaticAnalyzer/Checkers/BuiltinFunctionChecker.cpp`:
```c++
QualType getSufficientTypeForOverflowOp(CheckerContext &C, const QualType &T) {
  // Calling a builtin with a non-integer type result produces compiler error.
  assert(T->isIntegerType());

  ASTContext &ACtx = C.getASTContext();

  unsigned BitWidth = ACtx.getIntWidth(T);
  return ACtx.getIntTypeForBitwidth(BitWidth * 2, T->isSignedIntegerType());
}
```

`ACtx.getIntTypeForBitwidth()` returns Null. BitWidth: 128, T: __int128.

The fix would be to replace this function to just use the type from the CallExpr when we call the builtin instead of reinventing the wheel here.

---

### Comment 14 - zeyi2

Here is a small patch to fix the problem:

[173795.patch](https://github.com/user-attachments/files/24400803/173795.patch)

I'm not sure if there is anyone already working on this issue, so I decide to upload this file here.

---

### Comment 15 - steakhal

@VisualEhrmanntraut Could you please try again if this issue is fixed now?

---

### Comment 16 - VisualEhrmanntraut

I have yes, sorry, just forgot to mention. The crash is gone, thanks for the quick resolution.

---

