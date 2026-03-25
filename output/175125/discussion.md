# [WebAssembly] Backend crash: Cannot select v2f64 insert_vector_elt with SIMD

**Author:** thedaneeffect
**URL:** https://github.com/llvm/llvm-project/issues/175125
**Status:** Closed
**Labels:** backend:WebAssembly, crash
**Closed Date:** 2026-01-09T18:13:50Z

## Body

## Summary
LLVM crashes during WebAssembly instruction selection when compiling code with `-msimd128` enabled.

## Environment
- **Clang**: 22.0.0git (bundled with Emscripten 4.0.22-git, installed via Homebrew)
- **Target**: wasm32-unknown-emscripten
- **OS**: macOS 15.6.1 (Apple Silicon / arm64)
- **Project**: Godot Engine ([commit 6f15a05b](https://github.com/godotengine/godot/commit/6f15a05b6cf5f87de80e09f960a0e7a7449db640))

## Reproduction
Building Godot Engine for web platform:
```
scons platform=web target=template_release
```

Key flags from the failing command:
- `-target wasm32-unknown-emscripten`
- `-msimd128`
- `-Os`
- `-std=gnu++17`

Source file: [thirdparty/vhacd/src/FloatMath.cpp](https://github.com/godotengine/godot/blob/6f15a05b6cf5f87de80e09f960a0e7a7449db640/thirdparty/vhacd/src/FloatMath.cpp)

## Error
```
fatal error: error in backend: Cannot select: 0x106b7da90: v2f64 = insert_vector_elt 0x10685f470, 0x106861b00,
      Constant:i32<1>
  0x10685f470: v2f64 = scalar_to_vector 0x10685d810
    0x10685d810: v2f64,ch = load<(load (s128) from %ir.35, align 8, !tbaa !126)> 0x10610fc18, 0x106862190, undef:i32
      0x106862190: i32 = add nuw 0x106b7e2e0, Constant:i32<8>
        0x106b7e2e0: i32,ch = CopyFromReg 0x10610fc18, Register:i32 %16
  0x106861b00: v2f64 = splat_vector 0x10685f860
    0x10685f860: f64,ch = load<(dereferenceable load (s64) from %ir.31, !tbaa !56)> 0x10610fc18, 0x106fd2470, undef:i32
      0x106fd2470: i32 = add nuw 0x106b7d470, Constant:i32<8>
        0x106b7d470: i32,ch = CopyFromReg 0x10610fc18, Register:i32 %15
In function: _ZN10FLOAT_MATH13MyVertexIndex8getIndexEPKdRb
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang++ -target wasm32-unknown-emscripten -fignore-exceptions -pthread -mllvm -combiner-global-alias-analysis=false -mllvm -wasm-enable-sjlj -mllvm -disable-lsr --sysroot=/opt/homebrew/Cellar/emscripten/4.0.22/libexec/cache/sysroot -D__EMSCRIPTEN_SHARED_MEMORY__=1 -DEMSCRIPTEN -Xclang -iwithsysroot/include/fakesdl -Xclang -iwithsysroot/include/compat -obin/obj/thirdparty/vhacd/src/FloatMath.web.template_release.wasm32.o -c -std=gnu++17 -fno-exceptions -msimd128 -fcolor-diagnostics -Os -w -Wshadow-field-in-constructor -Wshadow-uncaptured-local -Wno-ordered-compare-function-pointers -Wenum-conversion -DNDEBUG -DWEB_ENABLED -DUNIX_ENABLED -DUNIX_SOCKET_UNAVAILABLE -DGLES3_ENABLED -DJAVASCRIPT_EVAL_ENABLED -DPTHREAD_NO_RENAME -DGDSCRIPT_NO_LSP -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -DMINIZIP_ENABLED -DBROTLI_ENABLED -DOVERRIDE_ENABLED -DTHREADS_ENABLED -DCLIPPER2_ENABLED -DZSTD_STATIC_LINKING_ONLY -DGODOT_MODULE -Ithirdparty/vhacd/inc -Ithirdparty/libpng -Ithirdparty/zstd -Ithirdparty/zlib -Ithirdparty/clipper2/include -Ithirdparty/brotli/include -Iplatform/web -I. -Imodules/Cellar/emscripten/4.0.22/bin/cache/sysroot/include thirdparty/vhacd/src/FloatMath.cpp
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'thirdparty/vhacd/src/FloatMath.cpp'.
4.	Running pass 'WebAssembly Instruction Selection' on function '@_ZN10FLOAT_MATH13MyVertexIndex8getIndexEPKdRb'
 #0 0x0000000101b51a3c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d39a3c)
 #1 0x0000000101b51470 llvm::sys::CleanupOnSignal(unsigned long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d39470)
 #2 0x0000000101ad4548 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc548)
 #3 0x0000000101ad44f4 llvm::CrashRecoveryContext::HandleExit(int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc4f4)
 #4 0x0000000101b4e350 llvm::sys::Process::Exit(int, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d36350)
 #5 0x0000000100e1d1fc llvm::raw_ostream::SetUnbuffered() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1000051fc)
 #6 0x0000000101adb4d8 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cc34d8)
 #7 0x00000001026fceb0 llvm::SelectionDAGISel::isOrEquivalentToAdd(llvm::SDNode const*) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018e4eb0)
 #8 0x00000001026fc8d0 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018e48d0)
 #9 0x0000000100e49a0c (anonymous namespace)::WebAssemblyDAGToDAGISel::Select(llvm::SDNode*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100031a0c)
#10 0x00000001026f66ec llvm::SelectionDAGISel::DoInstructionSelection() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018de6ec)
#11 0x00000001026f60c0 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018de0c0)
#12 0x00000001026f5994 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018dd994)
#13 0x00000001026f44f4 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018dc4f4)
#14 0x00000001026f3628 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018db628)
#15 0x0000000101391e9c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100579e9c)
#16 0x0000000101603e68 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ebe68)
#17 0x0000000101607038 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ef038)
#18 0x000000010160422c llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ec22c)
#19 0x0000000101e32f84 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10101af84)
#20 0x0000000101ff59cc clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1011dd9cc)
#21 0x00000001027f3bb0 clang::ParseAST(clang::Sema&, bool, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1019dbbb0)
#22 0x00000001022ad1c0 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1014951c0)
#23 0x000000010225da00 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x101445a00)
#24 0x00000001022fd0a4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1014e50a4)
#25 0x0000000100e1cc88 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100004c88)
#26 0x0000000100e1ac00 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100002c00)
#27 0x0000000100e1c10c int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10000410c)
#28 0x000000010214668c void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10132e68c)
#29 0x0000000101ad44cc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc4cc)
#30 0x00000001021461f0 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10132e1f0)
#31 0x0000000102121fb0 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x101309fb0)
#32 0x00000001021220d4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10130a0d4)
#33 0x0000000102132934 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10131a934)
#34 0x0000000100e1a678 clang_main(int, char**, llvm::ToolContext const&) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100002678)
#35 0x0000000100e249c0 main (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10000c9c0)
#36 0x0000000185ba6b98
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: /opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/qw/10lgwvn54cz90m7z191fs21c0000gn/T/FloatMath-8c5533.cpp
clang++: note: diagnostic msg: /var/folders/qw/10lgwvn54cz90m7z191fs21c0000gn/T/FloatMath-8c5533.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/dane/Library/Logs/DiagnosticReports/clang-22_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg:

********************
scons: *** [bin/obj/thirdparty/vhacd/src/FloatMath.web.template_release.wasm32.o] Error 1
scons: building terminated because of errors.
INFO: Time elapsed: 00:01:14.64

```

## Attachments
Preprocessed source and run script available at:
https://gist.github.com/thedaneeffect/1ecaef7380046ef5de812fa355e6199f

- `FloatMath-8c5533.cpp` - Preprocessed source
- `FloatMath-8c5533.sh` - Run script
- `output.txt` - Full build output with complete stack trace

**Note:** The crash backtrace `.crash` file mentioned in the compiler output was not actually saved to disk by macOS.


## Comments

### Comment 1 - kleisauke

This issue does not appear to reproduce with tip-of-tree binaries.
```console
$ git clone https://gist.github.com/thedaneeffect/1ecaef7380046ef5de812fa355e6199f llvm-issue-175125
$ cd llvm-issue-175125
$ emcc -std=gnu++17 -Os -msimd128 FloatMath-8c5533.cpp
$ emcc -v
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 4.0.23-git (035c704d5cbeb85481eb1c3273617d21b881d674)
clang version 22.0.0git (https:/github.com/llvm/llvm-project 451ca458cf51d553f5c49e67d841280e8166f933)
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: /home/kleisauke/emsdk/upstream/bin
Build config: +assertions
```

So, it looks like a duplicate of https://github.com/emscripten-core/emscripten/issues/25991, which was fixed with commit 7a22bea5125a051012f1a44be460bd38d9e5d0d1. In the meantime, you could workaround it by downgrading Emscripten to 4.0.21.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Dane (thedaneeffect)

<details>
## Summary
LLVM crashes during WebAssembly instruction selection when compiling code with `-msimd128` enabled.

## Environment
- **Clang**: 22.0.0git (bundled with Emscripten 4.0.22-git, installed via Homebrew)
- **Target**: wasm32-unknown-emscripten
- **OS**: macOS 15.6.1 (Apple Silicon / arm64)
- **Project**: Godot Engine ([commit 6f15a05b](https://github.com/godotengine/godot/commit/6f15a05b6cf5f87de80e09f960a0e7a7449db640))

## Reproduction
Building Godot Engine for web platform:
```
scons platform=web target=template_release
```

Key flags from the failing command:
- `-target wasm32-unknown-emscripten`
- `-msimd128`
- `-Os`
- `-std=gnu++17`

Source file: [thirdparty/vhacd/src/FloatMath.cpp](https://github.com/godotengine/godot/blob/6f15a05b6cf5f87de80e09f960a0e7a7449db640/thirdparty/vhacd/src/FloatMath.cpp)

## Error
```
fatal error: error in backend: Cannot select: 0x106b7da90: v2f64 = insert_vector_elt 0x10685f470, 0x106861b00,
      Constant:i32&lt;1&gt;
  0x10685f470: v2f64 = scalar_to_vector 0x10685d810
    0x10685d810: v2f64,ch = load&lt;(load (s128) from %ir.35, align 8, !tbaa !126)&gt; 0x10610fc18, 0x106862190, undef:i32
      0x106862190: i32 = add nuw 0x106b7e2e0, Constant:i32&lt;8&gt;
        0x106b7e2e0: i32,ch = CopyFromReg 0x10610fc18, Register:i32 %16
  0x106861b00: v2f64 = splat_vector 0x10685f860
    0x10685f860: f64,ch = load&lt;(dereferenceable load (s64) from %ir.31, !tbaa !56)&gt; 0x10610fc18, 0x106fd2470, undef:i32
      0x106fd2470: i32 = add nuw 0x106b7d470, Constant:i32&lt;8&gt;
        0x106b7d470: i32,ch = CopyFromReg 0x10610fc18, Register:i32 %15
In function: _ZN10FLOAT_MATH13MyVertexIndex8getIndexEPKdRb
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang++ -target wasm32-unknown-emscripten -fignore-exceptions -pthread -mllvm -combiner-global-alias-analysis=false -mllvm -wasm-enable-sjlj -mllvm -disable-lsr --sysroot=/opt/homebrew/Cellar/emscripten/4.0.22/libexec/cache/sysroot -D__EMSCRIPTEN_SHARED_MEMORY__=1 -DEMSCRIPTEN -Xclang -iwithsysroot/include/fakesdl -Xclang -iwithsysroot/include/compat -obin/obj/thirdparty/vhacd/src/FloatMath.web.template_release.wasm32.o -c -std=gnu++17 -fno-exceptions -msimd128 -fcolor-diagnostics -Os -w -Wshadow-field-in-constructor -Wshadow-uncaptured-local -Wno-ordered-compare-function-pointers -Wenum-conversion -DNDEBUG -DWEB_ENABLED -DUNIX_ENABLED -DUNIX_SOCKET_UNAVAILABLE -DGLES3_ENABLED -DJAVASCRIPT_EVAL_ENABLED -DPTHREAD_NO_RENAME -DGDSCRIPT_NO_LSP -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -DMINIZIP_ENABLED -DBROTLI_ENABLED -DOVERRIDE_ENABLED -DTHREADS_ENABLED -DCLIPPER2_ENABLED -DZSTD_STATIC_LINKING_ONLY -DGODOT_MODULE -Ithirdparty/vhacd/inc -Ithirdparty/libpng -Ithirdparty/zstd -Ithirdparty/zlib -Ithirdparty/clipper2/include -Ithirdparty/brotli/include -Iplatform/web -I. -Imodules/Cellar/emscripten/4.0.22/bin/cache/sysroot/include thirdparty/vhacd/src/FloatMath.cpp
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'thirdparty/vhacd/src/FloatMath.cpp'.
4.	Running pass 'WebAssembly Instruction Selection' on function '@<!-- -->_ZN10FLOAT_MATH13MyVertexIndex8getIndexEPKdRb'
 #<!-- -->0 0x0000000101b51a3c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d39a3c)
 #<!-- -->1 0x0000000101b51470 llvm::sys::CleanupOnSignal(unsigned long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d39470)
 #<!-- -->2 0x0000000101ad4548 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc548)
 #<!-- -->3 0x0000000101ad44f4 llvm::CrashRecoveryContext::HandleExit(int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc4f4)
 #<!-- -->4 0x0000000101b4e350 llvm::sys::Process::Exit(int, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100d36350)
 #<!-- -->5 0x0000000100e1d1fc llvm::raw_ostream::SetUnbuffered() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1000051fc)
 #<!-- -->6 0x0000000101adb4d8 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cc34d8)
 #<!-- -->7 0x00000001026fceb0 llvm::SelectionDAGISel::isOrEquivalentToAdd(llvm::SDNode const*) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018e4eb0)
 #<!-- -->8 0x00000001026fc8d0 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018e48d0)
 #<!-- -->9 0x0000000100e49a0c (anonymous namespace)::WebAssemblyDAGToDAGISel::Select(llvm::SDNode*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100031a0c)
#<!-- -->10 0x00000001026f66ec llvm::SelectionDAGISel::DoInstructionSelection() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018de6ec)
#<!-- -->11 0x00000001026f60c0 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018de0c0)
#<!-- -->12 0x00000001026f5994 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018dd994)
#<!-- -->13 0x00000001026f44f4 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018dc4f4)
#<!-- -->14 0x00000001026f3628 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1018db628)
#<!-- -->15 0x0000000101391e9c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100579e9c)
#<!-- -->16 0x0000000101603e68 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ebe68)
#<!-- -->17 0x0000000101607038 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ef038)
#<!-- -->18 0x000000010160422c llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1007ec22c)
#<!-- -->19 0x0000000101e32f84 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::__1::unique_ptr&lt;llvm::raw_pwrite_stream, std::__1::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10101af84)
#<!-- -->20 0x0000000101ff59cc clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1011dd9cc)
#<!-- -->21 0x00000001027f3bb0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1019dbbb0)
#<!-- -->22 0x00000001022ad1c0 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1014951c0)
#<!-- -->23 0x000000010225da00 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x101445a00)
#<!-- -->24 0x00000001022fd0a4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x1014e50a4)
#<!-- -->25 0x0000000100e1cc88 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100004c88)
#<!-- -->26 0x0000000100e1ac00 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100002c00)
#<!-- -->27 0x0000000100e1c10c int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10000410c)
#<!-- -->28 0x000000010214668c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10132e68c)
#<!-- -->29 0x0000000101ad44cc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100cbc4cc)
#<!-- -->30 0x00000001021461f0 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10132e1f0)
#<!-- -->31 0x0000000102121fb0 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x101309fb0)
#<!-- -->32 0x00000001021220d4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10130a0d4)
#<!-- -->33 0x0000000102132934 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10131a934)
#<!-- -->34 0x0000000100e1a678 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x100002678)
#<!-- -->35 0x0000000100e249c0 main (/opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin/clang-22+0x10000c9c0)
#<!-- -->36 0x0000000185ba6b98
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: /opt/homebrew/Cellar/emscripten/4.0.22/libexec/llvm/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/qw/10lgwvn54cz90m7z191fs21c0000gn/T/FloatMath-8c5533.cpp
clang++: note: diagnostic msg: /var/folders/qw/10lgwvn54cz90m7z191fs21c0000gn/T/FloatMath-8c5533.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/dane/Library/Logs/DiagnosticReports/clang-22_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg:

********************
scons: *** [bin/obj/thirdparty/vhacd/src/FloatMath.web.template_release.wasm32.o] Error 1
scons: building terminated because of errors.
INFO: Time elapsed: 00:01:14.64

```

## Attachments
Preprocessed source and run script available at:
https://gist.github.com/thedaneeffect/1ecaef7380046ef5de812fa355e6199f

- `FloatMath-8c5533.cpp` - Preprocessed source
- `FloatMath-8c5533.sh` - Run script
- `output.txt` - Full build output with complete stack trace

**Note:** The crash backtrace `.crash` file mentioned in the compiler output was not actually saved to disk by macOS.

## Related
- Emscripten issue: https://github.com/emscripten-core/emscripten/issues/26060
</details>


---

