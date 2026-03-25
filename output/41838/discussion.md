# Passing long double args on 32-bit SPARC violates ABI

**Author:** rorth
**URL:** https://github.com/llvm/llvm-project/issues/41838
**Status:** Closed
**Labels:** backend:Sparc, bugzilla, miscompilation, ABI
**Closed Date:** 2025-12-27T10:11:13Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [42493](https://llvm.org/bz42493) |
| Version | trunk |
| OS | Solaris |
| CC | @efriedma-quic |

## Extended Description 
I'm current working to fix the remaining compiler-rt testsuite bugs on SPARC
(https://reviews.llvm.org/D40900).  One of the failures is

    Builtins-sparc-sunos :: divtc3_test.c
    Builtins-sparcv9-sunos :: divtc3_test.c

While the sparcv9 failure is different, the sparc one boils down to incorrectly
passing long double arguments.  Consider the following testcase:

$ cat caller.c
extern void callee (long double);

int
main (void)
{
  long double ld = 1.0;

  callee (ld);
  return 0;
}
$ cat callee.c
extern void abort (void);

void
callee (long double ld)
{
  if (ld != 1.0)
    abort ();
}
$ clang -m32 -c caller.c -o caller.clang.o
$ clang -m32 -c callee.c -o callee.clang.o
$ gcc -m32 -c caller.c -o caller.gcc.o
$ gcc -m32 -c callee.c -o callee.gcc.o
$ gcc -m32 -o clang-gcc caller.clang.o callee.gcc.o 
$ ./clang-gcc 
Segmentation Fault (core dumped)
$ gcc -m32 -o gcc-clang caller.gcc.o callee.clang.o 
$ ./gcc-clang 
Abort (core dumped)

The SPARC psABI, p.3-15 (Structure, Union, and Quad-Precision Arguments)
requires long double args to be passed by reference, while clang passes
them by value (as is correct for SPARCv9).

## Comments

### Comment 1 - rorth

Thanks to your hints I've now been able to get the 128-bit long double support
working.  I've posted the current state of affairs at https://reviews.llvm.org/D89130
where I believe it's easier to discuss than in this PR.

---

### Comment 2 - rorth


> Okay, maybe a little more complicated. :)
> 
> For compiler-rt, see the bit that enables -fforce-enable-int128 for riscv in
> compiler-rt/lib/builtins/CMakeLists.txt .

Nice.  Unfortunately, this is clang-only, so ultimately a different solution
will have to be chose to continue supporting builds with gcc.  For the moment,
it certainly helps me along.

> > fatal error: error in backend: SPARCv8 does not handle f128 in calls; pass indirectly
> 
> Ideally, the fatal error here should be replaced with code to implement
> this; it doesn't make sense to refuse to lower calls involving f128, and
> optimizations are likely to explode if the backend doesn't implement it. 
> Let me know if you want tips on implementing this.

That would certainly be helpful: I've looked at both SparcCallingConv.td and
SparcISelLowering.cpp a bit, but am somewhat lost there.

> On the clang side, if you want to mess with the way calls are lowered to
> LLVM IR, the relevant code is clang/lib/CodeGen/TargetInfo.cpp .  There's
> some SPARC-specific code, although it's pretty bare-bones at the moment.

From a quick look, maybe the FP128TyID needs to be carried over from SparcV9;
can't tell yet.

> > I must admit I'm pretty confused about the interaction of the LongDouble*
> > variables above, resetDataLayout calls and maybe more.  Besides, there's
> > SparcCallingConv.td and support routines in llvm, so this tends to get more
> > complex.
> 
> In general, clang has its own view of types and memory layout, independent
> of LLVM IR.  This is used, for example, to compute sizeof().  Theoretically,
> you could use the clang libraries to analyze code on a target that doesn't
> have an LLVM backend.  The datalayout is only used when it comes time to
> emit IR.

Ah, thanks.  Strangely SparcV8 already uses f128:64 in clang/lib/Basic/Targets/Sparc.h
although clang otherwise uses 64-bit long double on SparcV8.

I've meanwhile found that despite the SVR4 SPARC psABI requiring 128-bit long
double, several targets do their own thing here which complicates things
tremendously:

* Solaris/SPARC with -m32 always uses 128-bit long double, following the psABI

* so do FreeBSD (but supports -mlong-double-64, too), OpenBSD (always 128-bit),
  sparc64-*-elf* and sparc64-*-rtems*

* NetBSD/sparc (i.e. 32-bit kernel) uses 64-bit long double, NetBSD/sparc64
  has 128-bit instead, optionally with support for -mlong-double-64

* similarly, Linux/sparc goes for 64-bit, while Linux/sparc64 does 128-bit, 
  both with the -mlong-double-64 option

With the possible exception of Linux/sparc64 (in the GCC compile farm) I can
test none of those, so I'll leave them alone, allowing for the respective
maintainers to make adjustments.

Right now there are only two failing tests due to this issue in a 2-stage
build:

  UBSan-Standalone-sparc :: TestCases/Float/cast-overflow.cpp
  UBSan-Standalone-sparc :: TestCases/Misc/log-path_test.cpp

I wonder if it wouldn't be appropriate to XFAIL them to try and get the 
Solaris/sparcv9 buildbot green finally, and either work in parallel on this
issue or leave it to the Sparc maintainers?

---

### Comment 3 - rorth



> I ran into quite a number of issues, however:

> * To make things worse, even on sparcv9 (where long double was supposed to be
>   correct), I find quite a number of long double test failing in a 2-stage
>   build:
> 
>   Builtins-sparcv9-sunos :: addtf3_test.c
>   Builtins-sparcv9-sunos :: divtf3_test.c
>   Builtins-sparcv9-sunos :: extenddftf2_test.c
>   Builtins-sparcv9-sunos :: extendsftf2_test.c
>   Builtins-sparcv9-sunos :: floatditf_test.c
>   Builtins-sparcv9-sunos :: floatsitf_test.c
>   Builtins-sparcv9-sunos :: floattitf_test.c
>   Builtins-sparcv9-sunos :: floatunditf_test.c
>   Builtins-sparcv9-sunos :: floatunsitf_test.c
>   Builtins-sparcv9-sunos :: floatuntitf_test.c
>   Builtins-sparcv9-sunos :: multf3_test.c
>   Builtins-sparcv9-sunos :: subtf3_test.c
> 
>   while the tests PASS in a 1-stage build with gcc.
> 
>   I haven't yet investigated more closely, but this shows that long double on
>   sparc has a considerable number of issues, not just on 32-bit.

I've now analyzed this further and filed Bug 47729 for the (unrelated) issue.

---

### Comment 4 - efriedma-quic

Okay, maybe a little more complicated. :)

For compiler-rt, see the bit that enables -fforce-enable-int128 for riscv in compiler-rt/lib/builtins/CMakeLists.txt .

> fatal error: error in backend: SPARCv8 does not handle f128 in calls; pass indirectly

Ideally, the fatal error here should be replaced with code to implement this; it doesn't make sense to refuse to lower calls involving f128, and optimizations are likely to explode if the backend doesn't implement it.  Let me know if you want tips on implementing this.

On the clang side, if you want to mess with the way calls are lowered to LLVM IR, the relevant code is clang/lib/CodeGen/TargetInfo.cpp .  There's some SPARC-specific code, although it's pretty bare-bones at the moment.

> I must admit I'm pretty confused about the interaction of the LongDouble*
> variables above, resetDataLayout calls and maybe more.  Besides, there's
> SparcCallingConv.td and support routines in llvm, so this tends to get more
> complex.

In general, clang has its own view of types and memory layout, independent of LLVM IR.  This is used, for example, to compute sizeof().  Theoretically, you could use the clang libraries to analyze code on a target that doesn't have an LLVM backend.  The datalayout is only used when it comes time to emit IR.

---

### Comment 5 - rorth


> 
> > So clang even gets the size of long double wrong on 32-bit SPARC.
> 
> That's easy to fix; changing the underlying type for long double just
> requires adding a few lines in clang/lib/Basic/Targets/ .  (It's controlled
> by LongDoubleFormat/LongDoubleWidth/LongDoubleAlign.)

That's what I tried, matching the sparcv9 case except for the smaller alignment.

I ran into quite a number of issues, however:

* The long double tests in compiler-rt assume __int128_t for 128-bit long double.
  However, that doesn't exist on 32-bit (sparc or otherwise).

* I also ran into

fatal error: error in backend: SPARCv8 does not handle f128 in calls; pass indirectly

  I must admit I'm pretty confused about the interaction of the LongDouble*
  variables above, resetDataLayout calls and maybe more.  Besides, there's
  SparcCallingConv.td and support routines in llvm, so this tends to get more
  complex.

* To make things worse, even on sparcv9 (where long double was supposed to be
  correct), I find quite a number of long double test failing in a 2-stage
  build:

  Builtins-sparcv9-sunos :: addtf3_test.c
  Builtins-sparcv9-sunos :: divtf3_test.c
  Builtins-sparcv9-sunos :: extenddftf2_test.c
  Builtins-sparcv9-sunos :: extendsftf2_test.c
  Builtins-sparcv9-sunos :: floatditf_test.c
  Builtins-sparcv9-sunos :: floatsitf_test.c
  Builtins-sparcv9-sunos :: floattitf_test.c
  Builtins-sparcv9-sunos :: floatunditf_test.c
  Builtins-sparcv9-sunos :: floatunsitf_test.c
  Builtins-sparcv9-sunos :: floatuntitf_test.c
  Builtins-sparcv9-sunos :: multf3_test.c
  Builtins-sparcv9-sunos :: subtf3_test.c

  while the tests PASS in a 1-stage build with gcc.

  I haven't yet investigated more closely, but this shows that long double on
  sparc has a considerable number of issues, not just on 32-bit.

---

### Comment 6 - efriedma-quic


> So clang even gets the size of long double wrong on 32-bit SPARC.

That's easy to fix; changing the underlying type for long double just requires adding a few lines in clang/lib/Basic/Targets/ .  (It's controlled by LongDoubleFormat/LongDoubleWidth/LongDoubleAlign.)

---

### Comment 7 - rorth

I just discovered that things are even worse:

$ cat sald.c
#include <stdio.h>
#include <stdalign.h>

int
main (void)
{
  printf ("sizeof (long double) = %ld\n", (long) sizeof (long double));
  printf ("alignof(long double) = %ld\n", (long) alignof (long double));
  return 0;
}
$ cc -m32 -o sald-cc sald.c
$ ./sald-cc 
sizeof (long double) = 16
alignof(long double) = 8
$ gcc -m32 -o sald-gcc sald.c
$ ./sald-gcc
sizeof (long double) = 16
alignof(long double) = 8
$ clang -m32 -o sald-clang sald.c
./sald-clang
sizeof (long double) = 8
alignof(long double) = 8

So clang even gets the size of long double wrong on 32-bit SPARC.

---

### Comment 8 - beetrees

For reference, on current LLVM the following LLVM IR ([compiler explorer](https://godbolt.org/z/Mb5GhE87E)):
```llvm
target triple = "sparc-unknown-linux-gnu"

define void @f(fp128) {
    ret void
}
```
fails to compile with the following error message:
```
LLVM ERROR: SPARCv8 does not handle f128 in calls; pass indirectly
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'SPARC DAG->DAG Pattern Instruction Selection' on function '@f'
 #0 0x0000000003a97bd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3a97bd8)
 #1 0x0000000003a9534c SignalHandler(int) Signals.cpp:0:0
 #2 0x00007f2ce6842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f2ce68969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f2ce6842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f2ce68287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000072c1a1 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x72c1a1)
 #7 0x00000000039e6fc8 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x39e6fc8)
 #8 0x0000000001c053a8 llvm::SparcTargetLowering::LowerFormalArguments_32(llvm::SDValue, unsigned int, bool, llvm::SmallVectorImpl<llvm::ISD::InputArg> const&, llvm::SDLoc const&, llvm::SelectionDAG&, llvm::SmallVectorImpl<llvm::SDValue>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1c053a8)
 #9 0x00000000037882e4 llvm::SelectionDAGISel::LowerArguments(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37882e4)
#10 0x0000000003850dfd llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3850dfd)
#11 0x0000000003851a71 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3851a71)
#12 0x000000000384278f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x384278f)
#13 0x0000000002a466b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x000000000302e6c3 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x302e6c3)
#15 0x000000000302e901 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x302e901)
#16 0x000000000302f145 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x302f145)
#17 0x000000000084319c compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#18 0x0000000000733a8e main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x733a8e)
#19 0x00007f2ce6829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007f2ce6829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x0000000000839cde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x839cde)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

### Comment 9 - tgross35

@rorth any chance you would be able to reboot https://reviews.llvm.org/D89130? The LLVM portion at least seemed pretty complete.

---

### Comment 10 - rorth

> [@rorth](https://github.com/rorth) any chance you would be able to reboot https://reviews.llvm.org/D89130? The LLVM portion at least seemed pretty complete.

I doubt it: this was one of the few attempts at backend patches of mine that all ended in a disaster one way or another: I'm pretty much ignorant of the backend, and those patches took huge amounts of time, but amounted to nothing in the end.  I'd rather not attempt something like this again.

I guess @koachan is way more qualified to address this: he's done extensive work on the SPARC backend recently.

---

### Comment 11 - koachan

Sure~ I haven't actually worked on any of the long double handling parts of the backend, but lemme see what I can do for it.

---

### Comment 12 - koachan

A question about https://reviews.llvm.org/D89130, @efriedma-quic 
There you said in the CC_Sparc32 handling:
> Maybe worth adding code to use CCPassIndirect for f128? Might allow you to simplify the clang handling.

What do I pass as the `destTy` for it? Is it the pointer type or something else?
Would something like `CCIfType<[f128], CCPassIndirect<i32>>` be correct?

---

### Comment 13 - brad0

Fixed in b5b66d5a2d21abead43319b8c155f83d002646a3

---

