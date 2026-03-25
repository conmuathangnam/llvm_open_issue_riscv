# [DirectX]  Add support for `llvm.assume` intrinsic to fix crash in DXIL Embedder

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/165051
**Status:** Closed
**Labels:** crash, backend:DirectX
**Closed Date:** 2025-11-06T19:05:32Z

## Body


The issue is ` tail call void @llvm.assume(i1 %15)`.  We likely need to just let this intrinsic pass on through because right now it is getting stuck here. And the assume intrinsic has existed since llvm 3.6 well before the DXIL fork https://releases.llvm.org/3.6.1/docs/LangRef.html#llvm-assume-intrinsic.
https://github.com/llvm/llvm-project/blob/7e76473d3fa90b954d8533f558274df1b95256fd/llvm/lib/Target/DirectX/DXILOpLowering.cpp#L914-L919

Adding support just means we add `llvm.assume` to the allow list of intrinsics we pass on to tthe DXIL bitcodewriter.

Without support for this intrinsic we cause the DXIL Bitcodewriter to crash. 
```gdb
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'DXIL Embedder' on module '/Users/farzonlotfi/Projects/llvm_debug_offload_build/tools/OffloadTest/test/clang-mtl/Basic/Output/matrix_scalar_constructor.test.tmp/source.hlsl'.
 #0 0x00000001060079f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034679f8)
 #1 0x0000000106008008 PrintStackTraceSignalHandler(void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103468008)
 #2 0x0000000106005884 llvm::sys::RunSignalHandlers() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103465884)
 #3 0x0000000106009a98 SignalHandler(int, __siginfo*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103469a98)
 #4 0x000000018437d6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x0000000184343848 (/usr/lib/system/libsystem_pthread.dylib+0x180473848)
 #6 0x000000018424c9e4 (/usr/lib/system/libsystem_c.dylib+0x18037c9e4)
 #7 0x0000000105e9ed48 llvm::install_out_of_memory_new_handler() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1032fed48)
 #8 0x000000010802ef10 llvm::dxil::DXILBitcodeWriter::writeTypeTable() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10548ef10)
 #9 0x000000010802c73c llvm::dxil::DXILBitcodeWriter::write() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10548c73c)
#10 0x000000010802c400 llvm::dxil::BitcodeWriter::writeModule(llvm::Module const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10548c400)
#11 0x000000010802c2a8 llvm::dxil::WriteDXILToFile(llvm::Module const&, llvm::raw_ostream&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10548c2a8)
#12 0x0000000108067330 (anonymous namespace)::EmbedDXILPass::runOnModule(llvm::Module&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1054c7330)
#13 0x0000000104c89d84 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e9d84)
#14 0x0000000104c89908 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e9908)
#15 0x0000000104c8fb80 llvm::legacy::PassManager::run(llvm::Module&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020efb80)
#16 0x00000001069ff9a0 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>&, std::__1::unique_ptr<llvm::ToolOutputFile, std::__1::default_delete<llvm::ToolOutputFile>>&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103e5f9a0)
#17 0x00000001069effa0 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103e4ffa0)
#18 0x00000001069ef448 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x103e4f448)
#19 0x00000001071a3b68 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104603b68)
#20 0x00000001098a28f8 clang::ParseAST(clang::Sema&, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x106d028f8)
#21 0x0000000107b7cbe8 clang::ASTFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104fdcbe8)
#22 0x00000001071a967c clang::CodeGenAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10460967c)
#23 0x0000000107b7cebc clang::WrapperFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104fdcebc)
#24 0x0000000107ba0884 clang::HLSLFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x105000884)
#25 0x0000000107b7c3f8 clang::FrontendAction::Execute() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104fdc3f8)
#26 0x0000000107a90684 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104ef0684)
#27 0x0000000107cc8ecc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x105128ecc)
#28 0x0000000102bb2e58 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100012e58)
#29 0x0000000102ba4fe8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100004fe8)
#30 0x0000000102ba3cd8 clang_main(int, char**, llvm::ToolContext const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100003cd8)
#31 0x0000000102bdf90c main (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10003f90c)
#32 0x0000000183fa2b98
clang-dxc: error: unable to execute command: Abort trap: 6
clang-dxc: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (git@github.com:llvm/llvm-project.git 0b01b96864983c4b150776b869a3d048b0d50e2c)
Target: dxilv1.0-unknown-shadermodel6.0-compute
Thread model: posix
InstalledDir: /Users/farzonlotfi/Projects/llvm_debug_offload_build/bin
Build config: +unoptimized, +assertions
clang-dxc: warning: dxv not found; resulting DXIL will not be validated or signed for use in release environment [-Wdxil-validation]
clang-dxc: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/source-9a84a0.hlsl
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/source-9a84a0.sh
clang-dxc: note: diagnostic msg: Crash backtrace is located in
clang-dxc: note: diagnostic msg: /Users/farzonlotfi/Library/Logs/DiagnosticReports/clang-21_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang-dxc: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang-dxc: note: diagnostic msg: 

********************
```

```llvm
target datalayout = "e-m:e-p:32:32-i1:32-i8:8-i16:16-i32:32-i64:64-f16:16-f32:32-f64:64-n8:16:32:64-v48:16:16-v96:32:32-v192:64:64"
target triple = "dxilv1.0-unknown-shadermodel6.0-compute"

%"RWBuffer<int32_t>" = type { i32 }

@.str = internal unnamed_addr constant [3 x i8] c"In\00", align 1
@.str.2 = internal unnamed_addr constant [4 x i8] c"Out\00", align 1
@In = external constant %"RWBuffer<int32_t>"
@Out = external constant %"RWBuffer<int32_t>"

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: write)
define void @main() local_unnamed_addr #0 {
entry:
  %matrixext.i.alloca = alloca [6 x i32], align 4
  %0 = tail call target("dx.TypedBuffer", i32, 1, 0, 1) @llvm.dx.resource.handlefrombinding.tdx.TypedBuffer_i32_1_0_1t(i32 0, i32 0, i32 1, i32 0, ptr nonnull @.str)
  %1 = tail call target("dx.TypedBuffer", i32, 1, 0, 1) @llvm.dx.resource.handlefrombinding.tdx.TypedBuffer_i32_1_0_1t(i32 0, i32 1, i32 1, i32 0, ptr nonnull @.str.2)
  %2 = call i32 @dx.op.flattenedThreadIdInGroup.i32(i32 96) #5
  %3 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 0)
  %4 = extractvalue { i32, i1 } %3, 0
  %5 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 1)
  %6 = extractvalue { i32, i1 } %5, 0
  %7 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 2)
  %8 = extractvalue { i32, i1 } %7, 0
  %9 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 3)
  %10 = extractvalue { i32, i1 } %9, 0
  %11 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 4)
  %12 = extractvalue { i32, i1 } %11, 0
  %13 = call { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 5)
  %14 = extractvalue { i32, i1 } %13, 0
  %matrixext.i.index12 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 0
  store i32 %4, ptr %matrixext.i.index12, align 4
  %matrixext.i.index213 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 1
  store i32 %6, ptr %matrixext.i.index213, align 4
  %matrixext.i.index414 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 2
  store i32 %8, ptr %matrixext.i.index414, align 4
  %matrixext.i.index615 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 3
  store i32 %10, ptr %matrixext.i.index615, align 4
  %matrixext.i.index816 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 4
  store i32 %12, ptr %matrixext.i.index816, align 4
  %matrixext.i.index1017 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 5
  store i32 %14, ptr %matrixext.i.index1017, align 4
  %15 = icmp ult i32 %2, 6
  tail call void @llvm.assume(i1 %15)
  %16 = mul i32 %2, 1
  %17 = add i32 0, %16
  %matrixext.i.index1118 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 %17
  %matrixext.i.load = load i32, ptr %matrixext.i.index1118, align 4
  call void @llvm.dx.resource.store.typedbuffer.tdx.TypedBuffer_i32_1_0_1t.i32(target("dx.TypedBuffer", i32, 1, 0, 1) %1, i32 %2, i32 %matrixext.i.load)
  ret void
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef %0) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(none)
declare target("dx.TypedBuffer", i32, 1, 0, 1) @llvm.dx.resource.handlefrombinding.tdx.TypedBuffer_i32_1_0_1t(i32 %0, i32 %1, i32 %2, i32 %3, ptr %4) #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.dx.resource.getpointer.p0.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 %1) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(read)
declare { i32, i1 } @llvm.dx.resource.load.typedbuffer.i32.tdx.TypedBuffer_i32_1_0_1t(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 %1) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(write)
declare void @llvm.dx.resource.store.typedbuffer.tdx.TypedBuffer_i32_1_0_1t.i32(target("dx.TypedBuffer", i32, 1, 0, 1) %0, i32 %1, i32 %2) #4

declare i32 @dx.op.flattenedThreadIdInGroup.i32(i32 %0)

attributes #0 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: write) "frame-pointer"="all" "hlsl.numthreads"="6,1,1" "hlsl.shader"="compute" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(none) }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(read) }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(write) }
attributes #5 = { memory(none) }

!llvm.module.flags = !{!0, !1}
!dx.valver = !{!2}
!llvm.ident = !{!3}
!llvm.errno.tbaa = !{!4}
!dx.shaderModel = !{!8}
!dx.version = !{!9}
!dx.resources = !{!10}
!dx.entryPoints = !{!15}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{i32 1, i32 8}
!3 = !{!"clang version 22.0.0git (git@github.com:llvm/llvm-project.git 0b01b96864983c4b150776b869a3d048b0d50e2c)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"int", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C++ TBAA"}
!8 = !{!"cs", i32 6, i32 0}
!9 = !{i32 1, i32 0}
!10 = !{null, !11, null, null}
!11 = !{!12, !14}
!12 = !{i32 0, ptr @In, !"In", i32 0, i32 0, i32 1, i32 10, i1 false, i1 false, i1 false, !13}
!13 = !{i32 0, i32 4}
!14 = !{i32 1, ptr @Out, !"Out", i32 0, i32 1, i32 1, i32 10, i1 false, i1 false, i1 false, !13}
!15 = !{ptr @main, !"main", null, !10, !16}
!16 = !{i32 4, !17}
!17 = !{i32 6, i32 1, i32 1}
```

The source code is pretty small
`clang-dxc -metal -T cs_6_0 -Fo matrix_scalar_constructor.test.tmp.o  source.hlsl`
```hlsl

RWBuffer<int> In : register(u0);
RWBuffer<int> Out : register(u1);

[numthreads(6,1,1)]
void main(uint GI : SV_GroupIndex) {
  int2x3 A = int2x3(In[0], In[1], In[2],
                    In[3], In[4], In[5]);

  int row = GI / 2;
  int col = GI % 2;
  Out[GI] = A[col][row];
}
```


