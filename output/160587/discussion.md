# SEGV on IR snippit reduced from `test_simple_atomic()` from pcsections.ll

**Author:** palmer-dabbelt
**URL:** https://github.com/llvm/llvm-project/issues/160587
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

While testing https://github.com/llvm/llvm-project/pull/160580 I noticed a test failure in `llvm/test/CodeGen/AArch64/pcsections.ll`'s `test_simple_atomic()`.  I tried to pull that function out to test it and I ended up with

````
@foo = dso_local global i64 0, align 8
@bar = dso_local global i64 0, align 8

define i64 @test_simple_atomic() {
; CHECK-LABEL: test_simple_atomic:
; CHECK:       .Lpcsection1:
; CHECK-NEXT:    ldr
; CHECK-NOT:   .Lpcsection2
; CHECK:         ldr
; CHECK:         add
; CHECK-NEXT:    ret
; CHECK:       .section section_no_aux,"awo",@progbits,.text
; CHECK-NEXT:  .Lpcsection_base3:
; DEFCM-NEXT:  .word    .Lpcsection1-.Lpcsection_base3
; LARGE-NEXT:  .xword   .Lpcsection1-.Lpcsection_base3
; CHECK-NEXT:  .text
entry:
  %0 = load atomic i64, ptr @foo monotonic, align 8, !pcsections !0
  %1 = load i64, ptr @bar, align 8
  %add = add nsw i64 %1, %0
  ret i64 %add
}

!0 = !{!"section_no_aux"}
````

which SEGVs while generating assembly.  Specifically:

````
$ ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_test_simple_atomic             ; -- Begin function test_simple_atomic
	.p2align	2
_test_simple_atomic:                    ; @test_simple_atomic
	.cfi_startproc
; %bb.0:                                ; %entry
Lloh0:
	adrp	x8, _foo@PAGE
Lloh1:
	adrp	x9, _bar@PAGE
Lpcsection0:
Lloh2:
	ldr	x8, [x8, _foo@PAGEOFF]
Lloh3:
	ldr	x9, [x9, _bar@PAGEOFF]
	add	x0, x9, x8
	ret
	.loh AdrpLdr	Lloh1, Lloh3
	.loh AdrpLdr	Lloh0, Lloh2
	.cfi_endproc
	.section	PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
1.	Running pass 'Function Pass Manager' on module 'test.ll'.
2.	Running pass 'AArch64 Assembly Printer' on function '@test_simple_atomic'
 #0 0x0000000103157408 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Volumes/life/llvm-project/build/bin/llc+0x1022cb408)
 #1 0x0000000103155344 llvm::sys::RunSignalHandlers() (/Volumes/life/llvm-project/build/bin/llc+0x1022c9344)
 #2 0x0000000103157eb8 SignalHandler(int, __siginfo*, void*) (/Volumes/life/llvm-project/build/bin/llc+0x1022cbeb8)
 #3 0x000000019c0296a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #4 0x0000000102b3af04 llvm::MCAsmInfoDarwin::printSwitchToSection(llvm::MCSection const&, unsigned int, llvm::Triple const&, llvm::raw_ostream&) const (/Volumes/life/llvm-project/build/bin/llc+0x101caef04)
 #5 0x0000000102afbef0 (anonymous namespace)::MCAsmStreamer::switchSection(llvm::MCSection*, unsigned int) (/Volumes/life/llvm-project/build/bin/llc+0x101c6fef0)
 #6 0x00000001024981e0 llvm::AsmPrinter::emitPCSections(llvm::MachineFunction const&)::$_1::operator()(llvm::MDNode const&, llvm::ArrayRef<llvm::MCSymbol const*>, bool) const (/Volumes/life/llvm-project/build/bin/llc+0x10160c1e0)
 #7 0x0000000102497ec8 llvm::AsmPrinter::emitPCSections(llvm::MachineFunction const&) (/Volumes/life/llvm-project/build/bin/llc+0x10160bec8)
 #8 0x000000010249c0bc llvm::AsmPrinter::emitFunctionBody() (/Volumes/life/llvm-project/build/bin/llc+0x1016100bc)
 #9 0x0000000101059e88 (anonymous namespace)::AArch64AsmPrinter::runOnMachineFunction(llvm::MachineFunction&) (/Volumes/life/llvm-project/build/bin/llc+0x1001cde88)
#10 0x000000010269bad8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/Volumes/life/llvm-project/build/bin/llc+0x10180fad8)
#11 0x0000000102a67a50 llvm::FPPassManager::runOnFunction(llvm::Function&) (/Volumes/life/llvm-project/build/bin/llc+0x101bdba50)
#12 0x0000000102a6db58 llvm::FPPassManager::runOnModule(llvm::Module&) (/Volumes/life/llvm-project/build/bin/llc+0x101be1b58)
#13 0x0000000102a683c0 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/Volumes/life/llvm-project/build/bin/llc+0x101bdc3c0)
#14 0x0000000100e8fbec main (/Volumes/life/llvm-project/build/bin/llc+0x100003bec)
#15 0x000000019bc4eb98
Segmentation fault: 11     ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
````

Note that I just started using both OSX and LLVM, so it's somewhat likely I've just done something stupid here.  This is also all a little close to what I was just touching, but as far as I can tell it's still manifesting on a clean recent main -- specifically eb8589987267 ("[InstCombine] Fold selects into masked loads (#160522)"), but I don't have any reason to believe that's an issue.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Palmer Dabbelt (palmer-dabbelt)

<details>
While testing https://github.com/llvm/llvm-project/pull/160580 I noticed a test failure in `llvm/test/CodeGen/AArch64/pcsections.ll`'s `test_simple_atomic()`.  I tried to pull that function out to test it and I ended up with

````
@<!-- -->foo = dso_local global i64 0, align 8
@<!-- -->bar = dso_local global i64 0, align 8

define i64 @<!-- -->test_simple_atomic() {
; CHECK-LABEL: test_simple_atomic:
; CHECK:       .Lpcsection1:
; CHECK-NEXT:    ldr
; CHECK-NOT:   .Lpcsection2
; CHECK:         ldr
; CHECK:         add
; CHECK-NEXT:    ret
; CHECK:       .section section_no_aux,"awo",@<!-- -->progbits,.text
; CHECK-NEXT:  .Lpcsection_base3:
; DEFCM-NEXT:  .word    .Lpcsection1-.Lpcsection_base3
; LARGE-NEXT:  .xword   .Lpcsection1-.Lpcsection_base3
; CHECK-NEXT:  .text
entry:
  %0 = load atomic i64, ptr @<!-- -->foo monotonic, align 8, !pcsections !0
  %1 = load i64, ptr @<!-- -->bar, align 8
  %add = add nsw i64 %1, %0
  ret i64 %add
}

!0 = !{!"section_no_aux"}
````

which SEGVs while generating assembly.  Specifically:

````
$ ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_test_simple_atomic             ; -- Begin function test_simple_atomic
	.p2align	2
_test_simple_atomic:                    ; @<!-- -->test_simple_atomic
	.cfi_startproc
; %bb.0:                                ; %entry
Lloh0:
	adrp	x8, _foo@<!-- -->PAGE
Lloh1:
	adrp	x9, _bar@<!-- -->PAGE
Lpcsection0:
Lloh2:
	ldr	x8, [x8, _foo@<!-- -->PAGEOFF]
Lloh3:
	ldr	x9, [x9, _bar@<!-- -->PAGEOFF]
	add	x0, x9, x8
	ret
	.loh AdrpLdr	Lloh1, Lloh3
	.loh AdrpLdr	Lloh0, Lloh2
	.cfi_endproc
	.section	PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
1.	Running pass 'Function Pass Manager' on module 'test.ll'.
2.	Running pass 'AArch64 Assembly Printer' on function '@<!-- -->test_simple_atomic'
 #<!-- -->0 0x0000000103157408 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Volumes/life/llvm-project/build/bin/llc+0x1022cb408)
 #<!-- -->1 0x0000000103155344 llvm::sys::RunSignalHandlers() (/Volumes/life/llvm-project/build/bin/llc+0x1022c9344)
 #<!-- -->2 0x0000000103157eb8 SignalHandler(int, __siginfo*, void*) (/Volumes/life/llvm-project/build/bin/llc+0x1022cbeb8)
 #<!-- -->3 0x000000019c0296a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->4 0x0000000102b3af04 llvm::MCAsmInfoDarwin::printSwitchToSection(llvm::MCSection const&amp;, unsigned int, llvm::Triple const&amp;, llvm::raw_ostream&amp;) const (/Volumes/life/llvm-project/build/bin/llc+0x101caef04)
 #<!-- -->5 0x0000000102afbef0 (anonymous namespace)::MCAsmStreamer::switchSection(llvm::MCSection*, unsigned int) (/Volumes/life/llvm-project/build/bin/llc+0x101c6fef0)
 #<!-- -->6 0x00000001024981e0 llvm::AsmPrinter::emitPCSections(llvm::MachineFunction const&amp;)::$_1::operator()(llvm::MDNode const&amp;, llvm::ArrayRef&lt;llvm::MCSymbol const*&gt;, bool) const (/Volumes/life/llvm-project/build/bin/llc+0x10160c1e0)
 #<!-- -->7 0x0000000102497ec8 llvm::AsmPrinter::emitPCSections(llvm::MachineFunction const&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x10160bec8)
 #<!-- -->8 0x000000010249c0bc llvm::AsmPrinter::emitFunctionBody() (/Volumes/life/llvm-project/build/bin/llc+0x1016100bc)
 #<!-- -->9 0x0000000101059e88 (anonymous namespace)::AArch64AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x1001cde88)
#<!-- -->10 0x000000010269bad8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x10180fad8)
#<!-- -->11 0x0000000102a67a50 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x101bdba50)
#<!-- -->12 0x0000000102a6db58 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x101be1b58)
#<!-- -->13 0x0000000102a683c0 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/Volumes/life/llvm-project/build/bin/llc+0x101bdc3c0)
#<!-- -->14 0x0000000100e8fbec main (/Volumes/life/llvm-project/build/bin/llc+0x100003bec)
#<!-- -->15 0x000000019bc4eb98
Segmentation fault: 11     ./build/bin/llc -march=aarch64 test.ll -o /dev/stdout
````

Note that I just started using both OSX and LLVM, so it's somewhat likely I've just done something stupid here.  This is also all a little close to what I was just touching, but as far as I can tell it's still manifesting on a clean recent main -- specifically eb8589987267 ("[InstCombine] Fold selects into masked loads (#<!-- -->160522)"), but I don't have any reason to believe that's an issue.
</details>


---

### Comment 2 - efriedma-quic

https://godbolt.org/z/TnjMxWoMM

---

