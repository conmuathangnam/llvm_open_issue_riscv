# Cannot select: 0x2c4ff9c0: i32 = X86ISD::CMP undef:i1, Constant:i1<0>

**Author:** DaniPopes
**URL:** https://github.com/llvm/llvm-project/issues/161693
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2025-10-03T07:09:57Z

## Body

Originally reported here: https://github.com/DaniPopes/const-hex/issues/29

LLVM crashes when building the project for x86_64-unknown-none (Rust target).
You can reproduce with `cargo build --lib --target x86_64-unknown-none -Zbuild-std --no-default-features` at [6f58e10b6935110be20fa9c9f5db69f5eddcd835](https://github.com/DaniPopes/const-hex/commit/6f58e10b6935110be20fa9c9f5db69f5eddcd835).

Full error message ([compiling the IR in Godbolt](https://godbolt.org/z/oc618r1fo)):

```
LLVM ERROR: Cannot select: 0x2c4ff9c0: i32 = X86ISD::CMP undef:i1, Constant:i1<0>
In function: _ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE'
 #0 0x0000000003bb89d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bb89d8)
 #1 0x0000000003bb5ba4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f5ea8442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f5ea84969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f5ea8442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f5ea84287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007b7be8 llvm::UniqueStringSaver::save(llvm::Twine const&) (.cold) StringSaver.cpp:0:0
 #7 0x000000000395e5ca llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395e5ca)
 #8 0x00000000039627f9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39627f9)
 #9 0x0000000001fd232a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
#10 0x000000000395b734 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x395b734)
#11 0x0000000003967af2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3967af2)
#12 0x000000000396a47a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396a47a)
#13 0x000000000396c3ce llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396c3ce)
#14 0x000000000395b2c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395b2c1)
#15 0x0000000002c374ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c374ad)
#16 0x00000000031a4a82 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a4a82)
#17 0x00000000031a4d11 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a4d11)
#18 0x00000000031a65e7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a65e7)
#19 0x00000000008ffd69 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#20 0x00000000007cc78e main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cc78e)
#21 0x00007f5ea8429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x00007f5ea8429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00000000008f75b5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f75b5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Generated IR from rustc (with [`cargo-show-asm`](https://github.com/pacak/cargo-show-asm): `cargo asm --keep-mangled --llvm-input --lib --target x86_64-unknown-none -Zbuild-std --no-default-features --everything`):

```llvm
; ModuleID = 'const_hex.bde2b1e042712010-cgu.0'
source_filename = "const_hex.bde2b1e042712010-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-none-elf"

@alloc_13203aafdb6c352c42340fcc75ed8209 = private unnamed_addr constant [256 x i8] c"\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\00\01\02\03\04\05\06\07\08\09\FF\FF\FF\FF\FF\FF\FF\0A\0B\0C\0D\0E\0F\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\0A\0B\0C\0D\0E\0F\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF", align 1
@alloc_466e6e10d4d5a95679ab35d1156aa54e = private unnamed_addr constant [11 x i8] c"src/lib.rs\00", align 1
@alloc_1974143a4ae9ab2f28281db29a2fc68f = private unnamed_addr constant <{ ptr, [16 x i8] }> <{ ptr @alloc_466e6e10d4d5a95679ab35d1156aa54e, [16 x i8] c"\0A\00\00\00\00\00\00\00\A1\02\00\00\0C\00\00\00" }>, align 8

; const_hex::invalid_hex_error
; Function Attrs: cold noredzone nounwind nonlazybind
define void @_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #0 {
start:
  br label %bb1

bb1:                                              ; preds = %bb1, %start
  %iter.sroa.7.0 = phi i64 [ %input.1, %start ], [ %rest.1, %bb1 ]
  %iter.sroa.0.0 = phi ptr [ %input.0, %start ], [ %rest.0, %bb1 ]
  %_4.not = icmp ne i64 %iter.sroa.7.0, 0
  tail call void @llvm.assume(i1 %_4.not)
  %rest.0 = getelementptr inbounds nuw i8, ptr %iter.sroa.0.0, i64 1
  %rest.1 = add i64 %iter.sroa.7.0, -1
  %_11 = load i8, ptr %iter.sroa.0.0, align 1, !noundef !4
  %_10 = zext i8 %_11 to i64
  %0 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_10
  %_8 = load i8, ptr %0, align 1, !noundef !4
  %_7 = icmp eq i8 %_8, -1
  br i1 %_7, label %bb4, label %bb1

bb4:                                              ; preds = %bb1
  %_14 = sub i64 %input.1, %iter.sroa.7.0
  %_23.not = icmp ugt i64 %iter.sroa.7.0, %input.1
  br i1 %_23.not, label %panic2, label %bb9

bb9:                                              ; preds = %bb4
  %1 = getelementptr inbounds nuw i8, ptr %input.0, i64 %_14
  %_21 = load i8, ptr %1, align 1, !noundef !4
  %_20 = zext i8 %_21 to i32
  %2 = getelementptr inbounds nuw i8, ptr %_0, i64 4
  store i32 %_20, ptr %2, align 4
  %3 = getelementptr inbounds nuw i8, ptr %_0, i64 8
  store i64 %_14, ptr %3, align 8
  store i32 0, ptr %_0, align 8
  ret void

panic2:                                           ; preds = %bb4
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h5aac0c5333fc2ce7E(i64 noundef %_14, i64 noundef %input.1, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24) @alloc_1974143a4ae9ab2f28281db29a2fc68f) #7
  unreachable
}

; const_hex::decode_to_slice_inner
; Function Attrs: noredzone nounwind nonlazybind
define void @_ZN9const_hex21decode_to_slice_inner17h730469d314907b36E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1, ptr noalias noundef nonnull writeonly align 1 captures(none) %output.0, i64 noundef %output.1) unnamed_addr #1 {
start:
  %_3 = and i64 %input.1, 1
  %0 = icmp eq i64 %_3, 0
  br i1 %0, label %bb2, label %bb1

bb2:                                              ; preds = %start
  %_13 = icmp ugt i64 %input.1, 1
  br i1 %_13, label %bb9, label %bb7

bb1:                                              ; preds = %start
  store i32 1, ptr %_0, align 8
  br label %bb6

bb9:                                              ; preds = %bb2
  %1 = load i8, ptr %input.0, align 1, !noundef !4
  %2 = icmp eq i8 %1, 48
  br i1 %2, label %bb10, label %bb7

bb10:                                             ; preds = %bb9
  %3 = getelementptr inbounds nuw i8, ptr %input.0, i64 1
  %4 = load i8, ptr %3, align 1, !noundef !4
  %5 = icmp eq i8 %4, 120
  br i1 %5, label %bb11, label %bb7

bb11:                                             ; preds = %bb10
  %6 = getelementptr inbounds nuw i8, ptr %input.0, i64 2
  %7 = add i64 %input.1, -2
  br label %bb7

bb7:                                              ; preds = %bb2, %bb9, %bb10, %bb11
  %input.sroa.0.0 = phi ptr [ %6, %bb11 ], [ %input.0, %bb10 ], [ %input.0, %bb9 ], [ %input.0, %bb2 ]
  %input.sroa.4.0 = phi i64 [ %7, %bb11 ], [ %input.1, %bb10 ], [ %input.1, %bb9 ], [ %input.1, %bb2 ]
  %_92 = lshr i64 %input.sroa.4.0, 1
  %_7.not = icmp eq i64 %output.1, %_92
  br i1 %_7.not, label %bb4, label %bb3

bb4:                                              ; preds = %bb7
  tail call void @llvm.experimental.noalias.scope.decl(metadata !5)
  tail call void @llvm.experimental.noalias.scope.decl(metadata !8)
  tail call void @llvm.experimental.noalias.scope.decl(metadata !10)
  br label %bb3.i

bb3.i:                                            ; preds = %bb4.i, %bb4
  %_5.sroa.8.0.i = phi i64 [ %input.sroa.4.0, %bb4 ], [ %_10.1.i, %bb4.i ]
  %_5.sroa.0.0.i = phi ptr [ %input.sroa.0.0, %bb4 ], [ %_10.0.i, %bb4.i ]
  %_8.not.i = icmp eq i64 %_5.sroa.8.0.i, 0
  br i1 %_8.not.i, label %bb8.i, label %bb4.i

bb8.i:                                            ; preds = %bb3.i
  tail call void @llvm.experimental.noalias.scope.decl(metadata !12)
  %_10.i6.not.i = icmp eq i64 %output.1, 0
  br i1 %_10.i6.not.i, label %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, label %bb3.i.i

bb3.i.i:                                          ; preds = %bb8.i, %bb3.i.i
  %i.sroa.0.0.i8.i = phi i64 [ %10, %bb3.i.i ], [ 0, %bb8.i ]
  %output.i.sroa.0.07.i = phi ptr [ %_7.i.i.i, %bb3.i.i ], [ %output.0, %bb8.i ]
  %_14.i.i = shl nuw i64 %i.sroa.0.0.i8.i, 1
  %_35.i.i = getelementptr inbounds nuw i8, ptr %input.sroa.0.0, i64 %_14.i.i
  %hex.i.i = load i8, ptr %_35.i.i, align 1, !alias.scope !15, !noalias !16, !noundef !4
  %_18.i.i = zext i8 %hex.i.i to i64
  %8 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_18.i.i
  %high.i.i = load i8, ptr %8, align 1, !noalias !18, !noundef !4
  %_22.i.i = or disjoint i64 %_14.i.i, 1
  %_38.i.i = icmp ult i64 %_22.i.i, %input.sroa.4.0
  tail call void @llvm.assume(i1 %_38.i.i)
  %_37.i.i = getelementptr inbounds nuw i8, ptr %input.sroa.0.0, i64 %_22.i.i
  %hex1.i.i = load i8, ptr %_37.i.i, align 1, !alias.scope !15, !noalias !16, !noundef !4
  %_27.i.i = zext i8 %hex1.i.i to i64
  %9 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_27.i.i
  %low.i.i = load i8, ptr %9, align 1, !noalias !18, !noundef !4
  %_33.i.i = shl i8 %high.i.i, 4
  %_32.i.i = or i8 %low.i.i, %_33.i.i
  store i8 %_32.i.i, ptr %output.i.sroa.0.07.i, align 1, !alias.scope !10, !noalias !19
  %_7.i.i.i = getelementptr inbounds nuw i8, ptr %output.i.sroa.0.07.i, i64 1
  %10 = add nuw nsw i64 %i.sroa.0.0.i8.i, 1
  %exitcond.not.i = icmp eq i64 %10, %output.1
  br i1 %exitcond.not.i, label %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, label %bb3.i.i

_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i: ; preds = %bb3.i.i, %bb8.i
  store i32 3, ptr %_0, align 8, !alias.scope !5, !noalias !24
  br label %bb6

bb4.i:                                            ; preds = %bb3.i
  %_10.0.i = getelementptr inbounds nuw i8, ptr %_5.sroa.0.0.i, i64 1
  %_10.1.i = add i64 %_5.sroa.8.0.i, -1
  %_9.i = load i8, ptr %_5.sroa.0.0.i, align 1, !alias.scope !8, !noalias !25, !noundef !4
  %_14.i = zext i8 %_9.i to i64
  %11 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_14.i
  %_12.i = load i8, ptr %11, align 1, !noalias !26, !noundef !4
  %_11.i = icmp eq i8 %_12.i, -1
  br i1 %_11.i, label %bb6.i, label %bb3.i, !prof !27

bb6.i:                                            ; preds = %bb4.i
; call const_hex::invalid_hex_error
  tail call void @_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr noalias noundef nonnull sret([16 x i8]) align 8 captures(address) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) %input.sroa.0.0, i64 noundef %input.sroa.4.0) #8, !noalias !10
  br label %bb6

bb3:                                              ; preds = %bb7
  store i32 2, ptr %_0, align 8
  br label %bb6

bb6:                                              ; preds = %bb6.i, %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, %bb1, %bb3
  ret void
}

; const_hex::arch::x86::check_sse2
; Function Attrs: nofree norecurse noredzone nosync nounwind nonlazybind memory(argmem: read, inaccessiblemem: readwrite)
define noundef zeroext i1 @_ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE(ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #2 {
start:
  tail call void @llvm.experimental.noalias.scope.decl(metadata !28)
  %_8.i.i = and i64 %input.1, -16
  %0 = getelementptr inbounds nuw i8, ptr %input.0, i64 %_8.i.i
  %1 = and i64 %input.1, 15
  br label %bb1.i.i

bb1.i.i:                                          ; preds = %bb3.i.i, %start
  %2 = phi ptr [ %4, %bb3.i.i ], [ %input.0, %start ]
  %3 = phi i64 [ %5, %bb3.i.i ], [ %_8.i.i, %start ]
  %_2.i.not.not.not.i.not.i = icmp eq i64 %3, 0
  br i1 %_2.i.not.not.not.i.not.i, label %bb8.i, label %bb3.i.i

bb3.i.i:                                          ; preds = %bb1.i.i
  %4 = getelementptr inbounds nuw i8, ptr %2, i64 16
  %5 = add i64 %3, -16
  %.val.i.i89 = load <16 x i8>, ptr %2, align 1, !alias.scope !28, !noalias !31
  %6 = add <16 x i8> %.val.i.i89, splat (i8 -58)
  %.not49.i = icmp ult <16 x i8> %6, splat (i8 -10)
  %7 = and <16 x i8> %.val.i.i89, splat (i8 -33)
  %8 = add <16 x i8> %7, splat (i8 -71)
  %.not51.i = icmp ult <16 x i8> %8, splat (i8 -6)
  %.not46.i = and <16 x i1> %.not49.i, %.not51.i
  %9 = bitcast <16 x i1> %.not46.i to i16
  %_0.i = icmp eq i16 %9, 0
  br i1 %_0.i, label %bb1.i.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit

bb8.i:                                            ; preds = %bb1.i.i, %bb9.i
  %remainder.sroa.5.0.i = phi i64 [ %_18.1.i, %bb9.i ], [ %1, %bb1.i.i ]
  %remainder.sroa.0.0.i = phi ptr [ %_18.0.i, %bb9.i ], [ %0, %bb1.i.i ]
  %_16.not.i = icmp eq i64 %remainder.sroa.5.0.i, 0
  br i1 %_16.not.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit, label %bb9.i

bb9.i:                                            ; preds = %bb8.i
  %_18.0.i = getelementptr inbounds nuw i8, ptr %remainder.sroa.0.0.i, i64 1
  %_18.1.i = add nsw i64 %remainder.sroa.5.0.i, -1
  %_17.i = load i8, ptr %remainder.sroa.0.0.i, align 1, !alias.scope !28, !noalias !36, !noundef !4
  %_22.i = zext i8 %_17.i to i64
  %10 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_22.i
  %_20.i = load i8, ptr %10, align 1, !noalias !37, !noundef !4
  %_19.i = icmp eq i8 %_20.i, -1
  br i1 %_19.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit, label %bb8.i

_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit: ; preds = %bb3.i.i, %bb8.i, %bb9.i
  %_0.sroa.0.0.i = phi i1 [ true, %bb8.i ], [ false, %bb9.i ], [ false, %bb3.i.i ]
  ret i1 %_0.sroa.0.0.i
}

; const_hex::check::check_inner
; Function Attrs: noredzone nounwind nonlazybind
define void @_ZN9const_hex5check11check_inner17h71993f089409e449E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #1 {
start:
  %e = alloca [16 x i8], align 8
  %_2 = and i64 %input.1, 1
  %0 = icmp eq i64 %_2, 0
  br i1 %0, label %bb2, label %bb1

bb2:                                              ; preds = %start
  %_14 = icmp ugt i64 %input.1, 1
  br i1 %_14, label %bb10, label %bb8

bb1:                                              ; preds = %start
  store i32 1, ptr %_0, align 8
  br label %bb7

bb10:                                             ; preds = %bb2
  %1 = load i8, ptr %input.0, align 1, !noundef !4
  %2 = icmp eq i8 %1, 48
  br i1 %2, label %bb11, label %bb8

bb11:                                             ; preds = %bb10
  %3 = getelementptr inbounds nuw i8, ptr %input.0, i64 1
  %4 = load i8, ptr %3, align 1, !noundef !4
  %5 = icmp eq i8 %4, 120
  br i1 %5, label %bb12, label %bb8

bb12:                                             ; preds = %bb11
  %6 = getelementptr inbounds nuw i8, ptr %input.0, i64 2
  %7 = add i64 %input.1, -2
  br label %bb8

bb8:                                              ; preds = %bb2, %bb10, %bb11, %bb12
  %stripped.sroa.5.0 = phi i64 [ %7, %bb12 ], [ %input.1, %bb11 ], [ %input.1, %bb10 ], [ %input.1, %bb2 ]
  %stripped.sroa.0.0 = phi ptr [ %6, %bb12 ], [ %input.0, %bb11 ], [ %input.0, %bb10 ], [ %input.0, %bb2 ]
  br label %bb13

bb13:                                             ; preds = %bb14, %bb8
  %_6.sroa.0.0 = phi ptr [ %stripped.sroa.0.0, %bb8 ], [ %_18.0, %bb14 ]
  %_6.sroa.8.0 = phi i64 [ %stripped.sroa.5.0, %bb8 ], [ %_18.1, %bb14 ]
  %_16.not = icmp eq i64 %_6.sroa.8.0, 0
  br i1 %_16.not, label %bb18, label %bb14

bb18:                                             ; preds = %bb13
  store i32 3, ptr %_0, align 8
  br label %bb7

bb14:                                             ; preds = %bb13
  %_18.0 = getelementptr inbounds nuw i8, ptr %_6.sroa.0.0, i64 1
  %_18.1 = add i64 %_6.sroa.8.0, -1
  %_17 = load i8, ptr %_6.sroa.0.0, align 1, !noundef !4
  %_22 = zext i8 %_17 to i64
  %8 = getelementptr inbounds nuw i8, ptr @alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_22
  %_20 = load i8, ptr %8, align 1, !noundef !4
  %_19 = icmp eq i8 %_20, -1
  br i1 %_19, label %bb16, label %bb13, !prof !27

bb16:                                             ; preds = %bb14
; call const_hex::invalid_hex_error
  call void @_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr noalias noundef nonnull sret([16 x i8]) align 8 captures(address) dereferenceable(16) %e, ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) %stripped.sroa.0.0, i64 noundef %stripped.sroa.5.0) #8
  %9 = load i32, ptr %e, align 8, !range !38, !noundef !4
  %10 = icmp eq i32 %9, 0
  br i1 %10, label %bb4, label %bb5

bb4:                                              ; preds = %bb16
  %_11 = sub i64 %input.1, %stripped.sroa.5.0
  %11 = getelementptr inbounds nuw i8, ptr %e, i64 8
  %12 = load i64, ptr %11, align 8, !noundef !4
  %13 = add i64 %_11, %12
  store i64 %13, ptr %11, align 8
  br label %bb5

bb5:                                              ; preds = %bb4, %bb16
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %_0, ptr noundef nonnull align 8 dereferenceable(16) %e, i64 16, i1 false)
  br label %bb7

bb7:                                              ; preds = %bb18, %bb5, %bb1
  ret void
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #4

; core::panicking::panic_bounds_check
; Function Attrs: cold minsize noinline noredzone noreturn nounwind nonlazybind optsize
declare void @_ZN4core9panicking18panic_bounds_check17h5aac0c5333fc2ce7E(i64 noundef, i64 noundef, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24)) unnamed_addr #5

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite)
declare void @llvm.experimental.noalias.scope.decl(metadata) #6

attributes #0 = { cold noredzone nounwind nonlazybind "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #1 = { noredzone nounwind nonlazybind "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #2 = { nofree norecurse noredzone nosync nounwind nonlazybind memory(argmem: read, inaccessiblemem: readwrite) "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float,+sse,+sse2" }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #5 = { cold minsize noinline noredzone noreturn nounwind nonlazybind optsize "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #6 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite) }
attributes #7 = { noreturn nounwind }
attributes #8 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 1, !"Code Model", i32 2}
!2 = !{i32 2, !"RtLibUseGOT", i32 1}
!3 = !{!"rustc version 1.92.0-nightly (4082d6a3f 2025-09-27)"}
!4 = !{}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %_0"}
!7 = distinct !{!7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE"}
!8 = !{!9}
!9 = distinct !{!9, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %input.0"}
!10 = !{!11}
!11 = distinct !{!11, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %output.0"}
!12 = !{!13}
!13 = distinct !{!13, !14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE: %input.0"}
!14 = distinct !{!14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE"}
!15 = !{!13, !9}
!16 = !{!17, !6, !11}
!17 = distinct !{!17, !14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE: argument 1"}
!18 = !{!13, !17, !6, !9, !11}
!19 = !{!20, !22, !13, !6, !9}
!20 = distinct !{!20, !21, !"_ZN70_$LT$$RF$mut$u20$$u5b$u8$u5d$$u20$as$u20$const_hex..output..Output$GT$5write17h2dddefcfecac2e8fE: %self"}
!21 = distinct !{!21, !"_ZN70_$LT$$RF$mut$u20$$u5b$u8$u5d$$u20$as$u20$const_hex..output..Output$GT$5write17h2dddefcfecac2e8fE"}
!22 = distinct !{!22, !23, !"_ZN9const_hex6output6Output10write_byte17h04c3e62ee13ae08cE: %self"}
!23 = distinct !{!23, !"_ZN9const_hex6output6Output10write_byte17h04c3e62ee13ae08cE"}
!24 = !{!9, !11}
!25 = !{!6, !11}
!26 = !{!6, !9, !11}
!27 = !{!"branch_weights", !"expected", i32 1, i32 2000}
!28 = !{!29}
!29 = distinct !{!29, !30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E: %input.0"}
!30 = distinct !{!30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E"}
!31 = !{!32, !34, !35}
!32 = distinct !{!32, !33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE: %self"}
!33 = distinct !{!33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE"}
!34 = distinct !{!34, !33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE: %f"}
!35 = distinct !{!35, !30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E: %check_chunk"}
!36 = !{!35}
!37 = !{!29, !35}
!38 = !{i32 0, i32 3}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (DaniPopes)

<details>
Originally reported here: https://github.com/DaniPopes/const-hex/issues/29

LLVM crashes when building the project for x86_64-unknown-none (Rust target).
You can reproduce with `cargo build --lib --target x86_64-unknown-none -Zbuild-std --no-default-features` at [6f58e10b6935110be20fa9c9f5db69f5eddcd835](https://github.com/DaniPopes/const-hex/commit/6f58e10b6935110be20fa9c9f5db69f5eddcd835).

Full error message ([compiling the IR in Godbolt](https://godbolt.org/z/oc618r1fo)):

```
LLVM ERROR: Cannot select: 0x2c4ff9c0: i32 = X86ISD::CMP undef:i1, Constant:i1&lt;0&gt;
In function: _ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE'
 #<!-- -->0 0x0000000003bb89d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bb89d8)
 #<!-- -->1 0x0000000003bb5ba4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f5ea8442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007f5ea84969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007f5ea8442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007f5ea84287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007b7be8 llvm::UniqueStringSaver::save(llvm::Twine const&amp;) (.cold) StringSaver.cpp:0:0
 #<!-- -->7 0x000000000395e5ca llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395e5ca)
 #<!-- -->8 0x00000000039627f9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39627f9)
 #<!-- -->9 0x0000000001fd232a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
#<!-- -->10 0x000000000395b734 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x395b734)
#<!-- -->11 0x0000000003967af2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3967af2)
#<!-- -->12 0x000000000396a47a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396a47a)
#<!-- -->13 0x000000000396c3ce llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396c3ce)
#<!-- -->14 0x000000000395b2c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395b2c1)
#<!-- -->15 0x0000000002c374ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c374ad)
#<!-- -->16 0x00000000031a4a82 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a4a82)
#<!-- -->17 0x00000000031a4d11 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a4d11)
#<!-- -->18 0x00000000031a65e7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31a65e7)
#<!-- -->19 0x00000000008ffd69 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->20 0x00000000007cc78e main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cc78e)
#<!-- -->21 0x00007f5ea8429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->22 0x00007f5ea8429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->23 0x00000000008f75b5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f75b5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Generated IR from rustc (with [`cargo-show-asm`](https://github.com/pacak/cargo-show-asm): `cargo asm --keep-mangled --llvm-input --lib --target x86_64-unknown-none -Zbuild-std --no-default-features --everything`):

```llvm
; ModuleID = 'const_hex.bde2b1e042712010-cgu.0'
source_filename = "const_hex.bde2b1e042712010-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-none-elf"

@<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209 = private unnamed_addr constant [256 x i8] c"\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\00\01\02\03\04\05\06\07\08\09\FF\FF\FF\FF\FF\FF\FF\0A\0B\0C\0D\0E\0F\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\0A\0B\0C\0D\0E\0F\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF", align 1
@<!-- -->alloc_466e6e10d4d5a95679ab35d1156aa54e = private unnamed_addr constant [11 x i8] c"src/lib.rs\00", align 1
@<!-- -->alloc_1974143a4ae9ab2f28281db29a2fc68f = private unnamed_addr constant &lt;{ ptr, [16 x i8] }&gt; &lt;{ ptr @<!-- -->alloc_466e6e10d4d5a95679ab35d1156aa54e, [16 x i8] c"\0A\00\00\00\00\00\00\00\A1\02\00\00\0C\00\00\00" }&gt;, align 8

; const_hex::invalid_hex_error
; Function Attrs: cold noredzone nounwind nonlazybind
define void @<!-- -->_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #<!-- -->0 {
start:
  br label %bb1

bb1:                                              ; preds = %bb1, %start
  %iter.sroa.7.0 = phi i64 [ %input.1, %start ], [ %rest.1, %bb1 ]
  %iter.sroa.0.0 = phi ptr [ %input.0, %start ], [ %rest.0, %bb1 ]
  %_4.not = icmp ne i64 %iter.sroa.7.0, 0
  tail call void @<!-- -->llvm.assume(i1 %_4.not)
  %rest.0 = getelementptr inbounds nuw i8, ptr %iter.sroa.0.0, i64 1
  %rest.1 = add i64 %iter.sroa.7.0, -1
  %_11 = load i8, ptr %iter.sroa.0.0, align 1, !noundef !4
  %_10 = zext i8 %_11 to i64
  %0 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_10
  %_8 = load i8, ptr %0, align 1, !noundef !4
  %_7 = icmp eq i8 %_8, -1
  br i1 %_7, label %bb4, label %bb1

bb4:                                              ; preds = %bb1
  %_14 = sub i64 %input.1, %iter.sroa.7.0
  %_23.not = icmp ugt i64 %iter.sroa.7.0, %input.1
  br i1 %_23.not, label %panic2, label %bb9

bb9:                                              ; preds = %bb4
  %1 = getelementptr inbounds nuw i8, ptr %input.0, i64 %_14
  %_21 = load i8, ptr %1, align 1, !noundef !4
  %_20 = zext i8 %_21 to i32
  %2 = getelementptr inbounds nuw i8, ptr %_0, i64 4
  store i32 %_20, ptr %2, align 4
  %3 = getelementptr inbounds nuw i8, ptr %_0, i64 8
  store i64 %_14, ptr %3, align 8
  store i32 0, ptr %_0, align 8
  ret void

panic2:                                           ; preds = %bb4
; call core::panicking::panic_bounds_check
  tail call void @<!-- -->_ZN4core9panicking18panic_bounds_check17h5aac0c5333fc2ce7E(i64 noundef %_14, i64 noundef %input.1, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24) @<!-- -->alloc_1974143a4ae9ab2f28281db29a2fc68f) #<!-- -->7
  unreachable
}

; const_hex::decode_to_slice_inner
; Function Attrs: noredzone nounwind nonlazybind
define void @<!-- -->_ZN9const_hex21decode_to_slice_inner17h730469d314907b36E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1, ptr noalias noundef nonnull writeonly align 1 captures(none) %output.0, i64 noundef %output.1) unnamed_addr #<!-- -->1 {
start:
  %_3 = and i64 %input.1, 1
  %0 = icmp eq i64 %_3, 0
  br i1 %0, label %bb2, label %bb1

bb2:                                              ; preds = %start
  %_13 = icmp ugt i64 %input.1, 1
  br i1 %_13, label %bb9, label %bb7

bb1:                                              ; preds = %start
  store i32 1, ptr %_0, align 8
  br label %bb6

bb9:                                              ; preds = %bb2
  %1 = load i8, ptr %input.0, align 1, !noundef !4
  %2 = icmp eq i8 %1, 48
  br i1 %2, label %bb10, label %bb7

bb10:                                             ; preds = %bb9
  %3 = getelementptr inbounds nuw i8, ptr %input.0, i64 1
  %4 = load i8, ptr %3, align 1, !noundef !4
  %5 = icmp eq i8 %4, 120
  br i1 %5, label %bb11, label %bb7

bb11:                                             ; preds = %bb10
  %6 = getelementptr inbounds nuw i8, ptr %input.0, i64 2
  %7 = add i64 %input.1, -2
  br label %bb7

bb7:                                              ; preds = %bb2, %bb9, %bb10, %bb11
  %input.sroa.0.0 = phi ptr [ %6, %bb11 ], [ %input.0, %bb10 ], [ %input.0, %bb9 ], [ %input.0, %bb2 ]
  %input.sroa.4.0 = phi i64 [ %7, %bb11 ], [ %input.1, %bb10 ], [ %input.1, %bb9 ], [ %input.1, %bb2 ]
  %_92 = lshr i64 %input.sroa.4.0, 1
  %_7.not = icmp eq i64 %output.1, %_92
  br i1 %_7.not, label %bb4, label %bb3

bb4:                                              ; preds = %bb7
  tail call void @<!-- -->llvm.experimental.noalias.scope.decl(metadata !5)
  tail call void @<!-- -->llvm.experimental.noalias.scope.decl(metadata !8)
  tail call void @<!-- -->llvm.experimental.noalias.scope.decl(metadata !10)
  br label %bb3.i

bb3.i:                                            ; preds = %bb4.i, %bb4
  %_5.sroa.8.0.i = phi i64 [ %input.sroa.4.0, %bb4 ], [ %_10.1.i, %bb4.i ]
  %_5.sroa.0.0.i = phi ptr [ %input.sroa.0.0, %bb4 ], [ %_10.0.i, %bb4.i ]
  %_8.not.i = icmp eq i64 %_5.sroa.8.0.i, 0
  br i1 %_8.not.i, label %bb8.i, label %bb4.i

bb8.i:                                            ; preds = %bb3.i
  tail call void @<!-- -->llvm.experimental.noalias.scope.decl(metadata !12)
  %_10.i6.not.i = icmp eq i64 %output.1, 0
  br i1 %_10.i6.not.i, label %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, label %bb3.i.i

bb3.i.i:                                          ; preds = %bb8.i, %bb3.i.i
  %i.sroa.0.0.i8.i = phi i64 [ %10, %bb3.i.i ], [ 0, %bb8.i ]
  %output.i.sroa.0.07.i = phi ptr [ %_7.i.i.i, %bb3.i.i ], [ %output.0, %bb8.i ]
  %_14.i.i = shl nuw i64 %i.sroa.0.0.i8.i, 1
  %_35.i.i = getelementptr inbounds nuw i8, ptr %input.sroa.0.0, i64 %_14.i.i
  %hex.i.i = load i8, ptr %_35.i.i, align 1, !alias.scope !15, !noalias !16, !noundef !4
  %_18.i.i = zext i8 %hex.i.i to i64
  %8 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_18.i.i
  %high.i.i = load i8, ptr %8, align 1, !noalias !18, !noundef !4
  %_22.i.i = or disjoint i64 %_14.i.i, 1
  %_38.i.i = icmp ult i64 %_22.i.i, %input.sroa.4.0
  tail call void @<!-- -->llvm.assume(i1 %_38.i.i)
  %_37.i.i = getelementptr inbounds nuw i8, ptr %input.sroa.0.0, i64 %_22.i.i
  %hex1.i.i = load i8, ptr %_37.i.i, align 1, !alias.scope !15, !noalias !16, !noundef !4
  %_27.i.i = zext i8 %hex1.i.i to i64
  %9 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_27.i.i
  %low.i.i = load i8, ptr %9, align 1, !noalias !18, !noundef !4
  %_33.i.i = shl i8 %high.i.i, 4
  %_32.i.i = or i8 %low.i.i, %_33.i.i
  store i8 %_32.i.i, ptr %output.i.sroa.0.07.i, align 1, !alias.scope !10, !noalias !19
  %_7.i.i.i = getelementptr inbounds nuw i8, ptr %output.i.sroa.0.07.i, i64 1
  %10 = add nuw nsw i64 %i.sroa.0.0.i8.i, 1
  %exitcond.not.i = icmp eq i64 %10, %output.1
  br i1 %exitcond.not.i, label %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, label %bb3.i.i

_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i: ; preds = %bb3.i.i, %bb8.i
  store i32 3, ptr %_0, align 8, !alias.scope !5, !noalias !24
  br label %bb6

bb4.i:                                            ; preds = %bb3.i
  %_10.0.i = getelementptr inbounds nuw i8, ptr %_5.sroa.0.0.i, i64 1
  %_10.1.i = add i64 %_5.sroa.8.0.i, -1
  %_9.i = load i8, ptr %_5.sroa.0.0.i, align 1, !alias.scope !8, !noalias !25, !noundef !4
  %_14.i = zext i8 %_9.i to i64
  %11 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_14.i
  %_12.i = load i8, ptr %11, align 1, !noalias !26, !noundef !4
  %_11.i = icmp eq i8 %_12.i, -1
  br i1 %_11.i, label %bb6.i, label %bb3.i, !prof !27

bb6.i:                                            ; preds = %bb4.i
; call const_hex::invalid_hex_error
  tail call void @<!-- -->_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr noalias noundef nonnull sret([16 x i8]) align 8 captures(address) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) %input.sroa.0.0, i64 noundef %input.sroa.4.0) #<!-- -->8, !noalias !10
  br label %bb6

bb3:                                              ; preds = %bb7
  store i32 2, ptr %_0, align 8
  br label %bb6

bb6:                                              ; preds = %bb6.i, %_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE.exit.i, %bb1, %bb3
  ret void
}

; const_hex::arch::x86::check_sse2
; Function Attrs: nofree norecurse noredzone nosync nounwind nonlazybind memory(argmem: read, inaccessiblemem: readwrite)
define noundef zeroext i1 @<!-- -->_ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE(ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #<!-- -->2 {
start:
  tail call void @<!-- -->llvm.experimental.noalias.scope.decl(metadata !28)
  %_8.i.i = and i64 %input.1, -16
  %0 = getelementptr inbounds nuw i8, ptr %input.0, i64 %_8.i.i
  %1 = and i64 %input.1, 15
  br label %bb1.i.i

bb1.i.i:                                          ; preds = %bb3.i.i, %start
  %2 = phi ptr [ %4, %bb3.i.i ], [ %input.0, %start ]
  %3 = phi i64 [ %5, %bb3.i.i ], [ %_8.i.i, %start ]
  %_2.i.not.not.not.i.not.i = icmp eq i64 %3, 0
  br i1 %_2.i.not.not.not.i.not.i, label %bb8.i, label %bb3.i.i

bb3.i.i:                                          ; preds = %bb1.i.i
  %4 = getelementptr inbounds nuw i8, ptr %2, i64 16
  %5 = add i64 %3, -16
  %.val.i.i89 = load &lt;16 x i8&gt;, ptr %2, align 1, !alias.scope !28, !noalias !31
  %6 = add &lt;16 x i8&gt; %.val.i.i89, splat (i8 -58)
  %.not49.i = icmp ult &lt;16 x i8&gt; %6, splat (i8 -10)
  %7 = and &lt;16 x i8&gt; %.val.i.i89, splat (i8 -33)
  %8 = add &lt;16 x i8&gt; %7, splat (i8 -71)
  %.not51.i = icmp ult &lt;16 x i8&gt; %8, splat (i8 -6)
  %.not46.i = and &lt;16 x i1&gt; %.not49.i, %.not51.i
  %9 = bitcast &lt;16 x i1&gt; %.not46.i to i16
  %_0.i = icmp eq i16 %9, 0
  br i1 %_0.i, label %bb1.i.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit

bb8.i:                                            ; preds = %bb1.i.i, %bb9.i
  %remainder.sroa.5.0.i = phi i64 [ %_18.1.i, %bb9.i ], [ %1, %bb1.i.i ]
  %remainder.sroa.0.0.i = phi ptr [ %_18.0.i, %bb9.i ], [ %0, %bb1.i.i ]
  %_16.not.i = icmp eq i64 %remainder.sroa.5.0.i, 0
  br i1 %_16.not.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit, label %bb9.i

bb9.i:                                            ; preds = %bb8.i
  %_18.0.i = getelementptr inbounds nuw i8, ptr %remainder.sroa.0.0.i, i64 1
  %_18.1.i = add nsw i64 %remainder.sroa.5.0.i, -1
  %_17.i = load i8, ptr %remainder.sroa.0.0.i, align 1, !alias.scope !28, !noalias !36, !noundef !4
  %_22.i = zext i8 %_17.i to i64
  %10 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_22.i
  %_20.i = load i8, ptr %10, align 1, !noalias !37, !noundef !4
  %_19.i = icmp eq i8 %_20.i, -1
  br i1 %_19.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit, label %bb8.i

_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit: ; preds = %bb3.i.i, %bb8.i, %bb9.i
  %_0.sroa.0.0.i = phi i1 [ true, %bb8.i ], [ false, %bb9.i ], [ false, %bb3.i.i ]
  ret i1 %_0.sroa.0.0.i
}

; const_hex::check::check_inner
; Function Attrs: noredzone nounwind nonlazybind
define void @<!-- -->_ZN9const_hex5check11check_inner17h71993f089409e449E(ptr dead_on_unwind noalias noundef writable writeonly sret([16 x i8]) align 8 captures(none) dereferenceable(16) %_0, ptr noalias noundef nonnull readonly align 1 captures(none) %input.0, i64 noundef %input.1) unnamed_addr #<!-- -->1 {
start:
  %e = alloca [16 x i8], align 8
  %_2 = and i64 %input.1, 1
  %0 = icmp eq i64 %_2, 0
  br i1 %0, label %bb2, label %bb1

bb2:                                              ; preds = %start
  %_14 = icmp ugt i64 %input.1, 1
  br i1 %_14, label %bb10, label %bb8

bb1:                                              ; preds = %start
  store i32 1, ptr %_0, align 8
  br label %bb7

bb10:                                             ; preds = %bb2
  %1 = load i8, ptr %input.0, align 1, !noundef !4
  %2 = icmp eq i8 %1, 48
  br i1 %2, label %bb11, label %bb8

bb11:                                             ; preds = %bb10
  %3 = getelementptr inbounds nuw i8, ptr %input.0, i64 1
  %4 = load i8, ptr %3, align 1, !noundef !4
  %5 = icmp eq i8 %4, 120
  br i1 %5, label %bb12, label %bb8

bb12:                                             ; preds = %bb11
  %6 = getelementptr inbounds nuw i8, ptr %input.0, i64 2
  %7 = add i64 %input.1, -2
  br label %bb8

bb8:                                              ; preds = %bb2, %bb10, %bb11, %bb12
  %stripped.sroa.5.0 = phi i64 [ %7, %bb12 ], [ %input.1, %bb11 ], [ %input.1, %bb10 ], [ %input.1, %bb2 ]
  %stripped.sroa.0.0 = phi ptr [ %6, %bb12 ], [ %input.0, %bb11 ], [ %input.0, %bb10 ], [ %input.0, %bb2 ]
  br label %bb13

bb13:                                             ; preds = %bb14, %bb8
  %_6.sroa.0.0 = phi ptr [ %stripped.sroa.0.0, %bb8 ], [ %_18.0, %bb14 ]
  %_6.sroa.8.0 = phi i64 [ %stripped.sroa.5.0, %bb8 ], [ %_18.1, %bb14 ]
  %_16.not = icmp eq i64 %_6.sroa.8.0, 0
  br i1 %_16.not, label %bb18, label %bb14

bb18:                                             ; preds = %bb13
  store i32 3, ptr %_0, align 8
  br label %bb7

bb14:                                             ; preds = %bb13
  %_18.0 = getelementptr inbounds nuw i8, ptr %_6.sroa.0.0, i64 1
  %_18.1 = add i64 %_6.sroa.8.0, -1
  %_17 = load i8, ptr %_6.sroa.0.0, align 1, !noundef !4
  %_22 = zext i8 %_17 to i64
  %8 = getelementptr inbounds nuw i8, ptr @<!-- -->alloc_13203aafdb6c352c42340fcc75ed8209, i64 %_22
  %_20 = load i8, ptr %8, align 1, !noundef !4
  %_19 = icmp eq i8 %_20, -1
  br i1 %_19, label %bb16, label %bb13, !prof !27

bb16:                                             ; preds = %bb14
; call const_hex::invalid_hex_error
  call void @<!-- -->_ZN9const_hex17invalid_hex_error17hf9d3bca479f15984E(ptr noalias noundef nonnull sret([16 x i8]) align 8 captures(address) dereferenceable(16) %e, ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) %stripped.sroa.0.0, i64 noundef %stripped.sroa.5.0) #<!-- -->8
  %9 = load i32, ptr %e, align 8, !range !38, !noundef !4
  %10 = icmp eq i32 %9, 0
  br i1 %10, label %bb4, label %bb5

bb4:                                              ; preds = %bb16
  %_11 = sub i64 %input.1, %stripped.sroa.5.0
  %11 = getelementptr inbounds nuw i8, ptr %e, i64 8
  %12 = load i64, ptr %11, align 8, !noundef !4
  %13 = add i64 %_11, %12
  store i64 %13, ptr %11, align 8
  br label %bb5

bb5:                                              ; preds = %bb4, %bb16
  call void @<!-- -->llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %_0, ptr noundef nonnull align 8 dereferenceable(16) %e, i64 16, i1 false)
  br label %bb7

bb7:                                              ; preds = %bb18, %bb5, %bb1
  ret void
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #<!-- -->3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @<!-- -->llvm.assume(i1 noundef) #<!-- -->4

; core::panicking::panic_bounds_check
; Function Attrs: cold minsize noinline noredzone noreturn nounwind nonlazybind optsize
declare void @<!-- -->_ZN4core9panicking18panic_bounds_check17h5aac0c5333fc2ce7E(i64 noundef, i64 noundef, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24)) unnamed_addr #<!-- -->5

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite)
declare void @<!-- -->llvm.experimental.noalias.scope.decl(metadata) #<!-- -->6

attributes #<!-- -->0 = { cold noredzone nounwind nonlazybind "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #<!-- -->1 = { noredzone nounwind nonlazybind "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #<!-- -->2 = { nofree norecurse noredzone nosync nounwind nonlazybind memory(argmem: read, inaccessiblemem: readwrite) "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float,+sse,+sse2" }
attributes #<!-- -->3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #<!-- -->4 = { mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #<!-- -->5 = { cold minsize noinline noredzone noreturn nounwind nonlazybind optsize "probe-stack"="inline-asm" "target-cpu"="x86-64" "target-features"="-mmx,-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2,+soft-float" }
attributes #<!-- -->6 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite) }
attributes #<!-- -->7 = { noreturn nounwind }
attributes #<!-- -->8 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 1, !"Code Model", i32 2}
!2 = !{i32 2, !"RtLibUseGOT", i32 1}
!3 = !{!"rustc version 1.92.0-nightly (4082d6a3f 2025-09-27)"}
!4 = !{}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %_0"}
!7 = distinct !{!7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE"}
!8 = !{!9}
!9 = distinct !{!9, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %input.0"}
!10 = !{!11}
!11 = distinct !{!11, !7, !"_ZN9const_hex14decode_checked17hb1d7aca4f7177b9eE: %output.0"}
!12 = !{!13}
!13 = distinct !{!13, !14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE: %input.0"}
!14 = distinct !{!14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE"}
!15 = !{!13, !9}
!16 = !{!17, !6, !11}
!17 = distinct !{!17, !14, !"_ZN9const_hex4arch7generic18decode_maybe_check17h740888d3ce8521dfE: argument 1"}
!18 = !{!13, !17, !6, !9, !11}
!19 = !{!20, !22, !13, !6, !9}
!20 = distinct !{!20, !21, !"_ZN70_$LT$$RF$mut$u20$$u5b$u8$u5d$$u20$as$u20$const_hex..output..Output$GT$5write17h2dddefcfecac2e8fE: %self"}
!21 = distinct !{!21, !"_ZN70_$LT$$RF$mut$u20$$u5b$u8$u5d$$u20$as$u20$const_hex..output..Output$GT$5write17h2dddefcfecac2e8fE"}
!22 = distinct !{!22, !23, !"_ZN9const_hex6output6Output10write_byte17h04c3e62ee13ae08cE: %self"}
!23 = distinct !{!23, !"_ZN9const_hex6output6Output10write_byte17h04c3e62ee13ae08cE"}
!24 = !{!9, !11}
!25 = !{!6, !11}
!26 = !{!6, !9, !11}
!27 = !{!"branch_weights", !"expected", i32 1, i32 2000}
!28 = !{!29}
!29 = distinct !{!29, !30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E: %input.0"}
!30 = distinct !{!30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E"}
!31 = !{!32, !34, !35}
!32 = distinct !{!32, !33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE: %self"}
!33 = distinct !{!33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE"}
!34 = distinct !{!34, !33, !"_ZN4core4iter6traits8iterator8Iterator8try_fold17h9044e46baeab562bE: %f"}
!35 = distinct !{!35, !30, !"_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E: %check_chunk"}
!36 = !{!35}
!37 = !{!29, !35}
!38 = !{i32 0, i32 3}
```
</details>


---

### Comment 2 - RKSimon

```ll
; ModuleID = 'bugpoint-reduced-simplified.bc'
source_filename = "const_hex.bde2b1e042712010-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-none-elf"

define void @_ZN9const_hex4arch3x8610check_sse217h277ec5271ba6c83bE() unnamed_addr #0 {
start:
  br label %bb1.i.i

bb1.i.i:                                          ; preds = %bb1.i.i, %start
  %.val.i.i89 = load <16 x i8>, ptr poison, align 1
  %.not49.i = icmp ult <16 x i8> zeroinitializer, splat (i8 -10)
  %i = and <16 x i8> %.val.i.i89, splat (i8 -33)
  %i1 = add <16 x i8> %i, splat (i8 -71)
  %.not51.i = icmp ult <16 x i8> %i1, splat (i8 -6)
  %.not46.i = and <16 x i1> %.not49.i, %.not51.i
  %i2 = bitcast <16 x i1> %.not46.i to i16
  %_0.i = icmp eq i16 %i2, 0
  br i1 %_0.i, label %bb1.i.i, label %_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit

_ZN9const_hex4arch7generic22check_unaligned_chunks17hc9ab627f8c105c23E.exit: ; preds = %bb1.i.i
  ret void
}

attributes #0 = { "target-features"="+soft-float" }
```

---

### Comment 3 - RKSimon

@DaniPopes any reason why you've enabled soft-float?

---

### Comment 4 - DaniPopes

That's the default target features for x86_64-unknown-none in Rust: https://github.com/rust-lang/rust/blob/94ecb52bbeeccc990545d4ebc9e9c35dcaf285e3/compiler/rustc_target/src/spec/targets/x86_64_unknown_none.rs#L24

---

