# [VPlan] Interleave group narrowing buggy for scalable vectors

**Author:** artagnon
**URL:** https://github.com/llvm/llvm-project/issues/185860
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-03-13T16:04:45Z

## Body

Building llvm-test-suite's `MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT` with flags `-mcpu=neoverse-512tvb -msve-vector-bits=256 -O3` results in a miscompiled PENNANT binary, which fails tests.

It looks like https://github.com/llvm/llvm-project/pull/183204 is the first failing commit from the buildbot (https://lab.llvm.org/buildbot/#/builders/143/builds/14601).

CC: @davemgreen, @david-arm, @sdesmalen-arm, @fhahn, @antmox

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Ramkumar Ramachandra (artagnon)

<details>
Building llvm-test-suite's `MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT` with flags `-mcpu=neoverse-512tvb -msve-vector-bits=256 -O3` results in a PENNANT binary with an illegal `ptrue` instruction:

```
-&gt;  0x100013848 &lt;+152&gt;: ptrue  p0.b
    0x10001384c &lt;+156&gt;: mov    w9, #<!-- -->0x50 ; =80
    0x100013850 &lt;+160&gt;: ldr    x8, [x8, #<!-- -->0x178]
    0x100013854 &lt;+164&gt;: st1b   { z0.b }, p0, [x20, x9]
```

It's not clear which commit introduced the issue, but this is a problem on tip-of-tree. CC: @<!-- -->davemgreen, @<!-- -->antmox.
</details>


---

### Comment 2 - fhahn

cc @david-arm @sdesmalen-arm 

---

### Comment 3 - davemgreen

Hi - What machine are you running on? The instruction is only valid on a machine with +sve, but I don't believe it should be invalid itself.

It sounds like the kind of error you get when you try to run a binary with SVE on a machine without it (i.e, it fails on the first SVE instruction it finds, usually a ptrue setting up a predicate).

---

### Comment 4 - artagnon

> Hi - What machine are you running on? The instruction is only valid on a machine with +sve, but I don't believe it should be invalid itself.

A Macbook M4: I thought it had SVE? I also tried on an Ampere AArch64 Linux box, with a SIGILL. See also failing buildbot: https://lab.llvm.org/buildbot/#/builders/143/builds/14601

---

### Comment 5 - MacDue

> A Macbook M4: I thought it had SVE?

I don't believe the M4 has SVE outside of "streaming mode", which  I don't think applies here. 

---

### Comment 6 - artagnon

> > A Macbook M4: I thought it had SVE?
> 
> I don't believe the M4 has SVE outside of "streaming mode", which I don't think applies here.

Thanks, updated summary: it crashes on `index` on the Ampere machine:

```
BogoMIPS        : 50.00
Features        : fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm lrcpc dcpop asimddp ssbs
CPU implementer : 0x41
CPU architecture: 8
CPU variant     : 0x3
CPU part        : 0xd0c
CPU revision    : 1
```

---

### Comment 7 - MacDue

Looks like that does not have SVE either (from the features list). 

---

### Comment 8 - artagnon

> Looks like that does not have SVE either (from the features list).

Hm, is the buildbot is misconfigured? It's hard to believe that none of the other binaries contained any SVE instructions. As I don't seem to have an appropriate machine to test on, I'm afraid I'm going to have to pass the investigation of the buildbot failure to someone else?

---

### Comment 9 - antmox

yes, hard to believe that the buildbot is misconfigured. I can run tests on the buildbot machine if that helps

---

### Comment 10 - artagnon

> yes, hard to believe that the buildbot is misconfigured. I can run tests on the buildbot machine if that helps

Can you run the PENNANT binary with the input by hand on an SVE-capable machine, and report the actual error?

---

### Comment 11 - antmox

it runs correctly. output is different from the reference output

```
+ /home/antoine/repro/test/sandbox/build/tools/timeit-target --timeout 7200 --limit-core 0 --limit-cpu 7200 --limit-file-size 209715200 --limit-rss-size 838860800 --append-exitstatus --redirect-output /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/Output/PENNANT.test.out --redirect-input /dev/null --chdir /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT --summary /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/Output/PENNANT.test.time /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/PENNANT intermediate_leblanc.pnt/home/antoine/repro/test/sandbox/build/tools/fpcmp-target -r 0.001 /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/Output/PENNANT.test.out /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/PENNANT.reference_output

+ /home/antoine/repro/test/sandbox/build/tools/fpcmp-target -r 0.001 /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/Output/PENNANT.test.out /home/antoine/repro/test/sandbox/build/MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT/PENNANT.reference_output/home/antoine/repro/test/sandbox/build/tools/fpcmp-target: Comparison failed, textual difference between 'C' and 'd'

Input 1:
********************
Running PENNANT v0.9
********************

--- Mesh Information ---
Points:  91001
Zones:  90000
Sides:  360000
Edges:  181000
Side chunks:  704
Point chunks:  178
Zone chunks:  176
Chunk size:  512
------------------------
Energy check:  total energy  =   9.424778e-01
(internal =   9.424778e-01, kinetic =   0.000000e+00)
dt limiter: Initial timestep
dt limiter: Hydro Courant limit for z = 30000

Run complete
cycle =     10,         cstop =     10
time  =   3.619254e-02, tstop =   6.000000e+00

Energy check:  total energy  =   9.424779e-01
(internal =   9.414265e-01, kinetic =   1.051465e-03)
exit 0

Input 2:
********************
Running PENNANT v0.9
********************

--- Mesh Information ---
Points:  91001
Zones:  90000
Sides:  360000
Edges:  181000
Side chunks:  704
Point chunks:  178
Zone chunks:  176
Chunk size:  512
------------------------
Energy check:  total energy  =   9.424778e-01
(internal =   9.424778e-01, kinetic =   0.000000e+00)
dt limiter: Initial timestep
dt limiter: Hydro dV/V limit for z = 30167

Run complete
cycle =     10,         cstop =     10
time  =   5.589080e-02, tstop =   6.000000e+00

Energy check:  total energy  =   9.424778e-01
(internal =   9.406983e-01, kinetic =   1.779474e-03)
exit 0

```

---

### Comment 12 - artagnon

> it runs correctly. output is different from the reference output

Thanks, will check the diff with the WidenCast commit reverted.

---

### Comment 13 - artagnon

> > it runs correctly. output is different from the reference output
> 
> Thanks, will check the diff with the WidenCast commit reverted.

There are large diffs in the Hydro, Mesh, and QCS source files, but further reduction with llvm-reduce would require some way of running the example resulting in different outputs? Can someone with access to an SVE-capable machine kindly step in to help me reduce this?

---

### Comment 14 - MacDue

I've had a little look into this. The issue seems to occur in the function `_ZN5Hydro7doCycleEd`. I've attached the IR, I've not reduced it further yet, but I expect it may be a fixed-length SVE ISEL issue. 

[extract.ll.txt](https://github.com/user-attachments/files/25915187/extract.ll.txt)

---

### Comment 15 - DavidSpickett

`clang-aarch64-sve-vls` is one of the fixed length bots, perhaps @antmox can check whether this showed up on any of the VLA (length agnostic) builders?

---

### Comment 16 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Ramkumar Ramachandra (artagnon)

<details>
Building llvm-test-suite's `MultiSource/Benchmarks/DOE-ProxyApps-C++/PENNANT` with flags `-mcpu=neoverse-512tvb -msve-vector-bits=256 -O3` results in a miscompiled PENNANT binary, which fails tests.

It looks like https://github.com/llvm/llvm-project/pull/183204 is the first failing commit from the buildbot (https://lab.llvm.org/buildbot/#/builders/143/builds/14601).

CC: @<!-- -->davemgreen, @<!-- -->david-arm, @<!-- -->sdesmalen-arm, @<!-- -->fhahn, @<!-- -->antmox
</details>


---

### Comment 17 - antmox

@DavidSpickett Issue showed only on this bot. @artagnon I have access to such a machine, I can help reducing this

---

### Comment 18 - artagnon

> I have access to such a machine, I can help reducing this

Could you run CReduce with `clang -mllvm -vectorize-loops=false` and `clang` using clang-tip with the additional CXXFLAGS, and reduce on the condition that outputs mismatch?

---

### Comment 19 - antmox

Yes I'll do that.

Could you revert this patch temporarily in order to make the bot green again until this is fixed ?

---

### Comment 20 - artagnon

See https://github.com/llvm/llvm-project/pull/186072 for the revert. I have no way to check if it will fix the issue.

---

### Comment 21 - david-arm

@artagnon Thanks for this, I'm currently looking into this as well to find out what's going. I've reproduced the issue locally.

---

### Comment 22 - david-arm

First investigation shows that disabling vectorisation in Hydro::calcAccel seems to fix the issue.

---

### Comment 23 - david-arm

I believe this is a vectorisation bug, perhaps exposed by https://github.com/llvm/llvm-project/pull/183204. More details to follow ...

---

### Comment 24 - david-arm

cc @fhahn Before vectorisation in the loop in calcAccel we have:
   
```
for.body:                                         ; preds = %for.body.preheader, %for.body
  %indvars.iv = phi i64 [ %0, %for.body.preheader ], [ %indvars.iv.next, %for.body ]
  %arrayidx = getelementptr inbounds [16 x i8], ptr %pf, i64 %indvars.iv
  %arrayidx3 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %indvars.iv
  %1 = load double, ptr %arrayidx3, align 8, !tbaa !51
  %cmp.i = fcmp olt double %1, 1.000000e-99
  %2 = select i1 %cmp.i, double 1.000000e-99, double %1
  %div.i = fdiv double 1.000000e+00, %2
  %3 = load double, ptr %arrayidx, align 8, !tbaa !117, !noalias !248
  %mul.i = fmul double %3, %div.i
  %y.i = getelementptr inbounds nuw i8, ptr %arrayidx, i64 8
  %4 = load double, ptr %y.i, align 8, !tbaa !119, !noalias !248  
  %mul2.i = fmul double %4, %div.i
  %arrayidx5 = getelementptr inbounds [16 x i8], ptr %pa, i64 %indvars.iv
  store double %mul.i, ptr %arrayidx5, align 8, !tbaa !117
  %y3.i = getelementptr inbounds nuw i8, ptr %arrayidx5, i64 8
  store double %mul2.i, ptr %y3.i, align 8, !tbaa !119
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond.not, label %for.cond.cleanup.loopexit, label %for.body, !llvm.loop !251
```
  
There is a single scalar load from array %pmass for every pair of doubles loaded from %pf and pair of doubles stored to %pa. When compiling with just `-mcpu=neoverse-512tvb -O3` after vectorisation we get something like this:

```
vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %offset.idx = add i64 %index, %0
  %7 = add i64 %offset.idx, 1
  %8 = add i64 %offset.idx, 2
  %9 = add i64 %offset.idx, 3
  %10 = getelementptr inbounds [16 x i8], ptr %pf, i64 %offset.idx
  %11 = getelementptr inbounds [16 x i8], ptr %pf, i64 %7
  %12 = getelementptr inbounds [16 x i8], ptr %pf, i64 %8
  %13 = getelementptr inbounds [16 x i8], ptr %pf, i64 %9
  %14 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %offset.idx
  %15 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %7
  %16 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %8
  %17 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %9
  %18 = load double, ptr %14, align 8, !tbaa !51, !alias.scope !234
  %19 = load double, ptr %15, align 8, !tbaa !51, !alias.scope !234
  %20 = load double, ptr %16, align 8, !tbaa !51, !alias.scope !234
  %21 = load double, ptr %17, align 8, !tbaa !51, !alias.scope !234
...
  %wide.load = load <2 x double>, ptr %10, align 8
  %wide.load38 = load <2 x double>, ptr %11, align 8
  %wide.load39 = load <2 x double>, ptr %12, align 8
  %wide.load40 = load <2 x double>, ptr %13, align 8
```

which looks correct, since it follows the same pattern as scalar. A pair of doubles loaded from %pf for every load from %pmass, with all loads being contiguous.
  
However, if I add the pragma `#pragma clang loop vectorize_width(2,scalable)` to loop in calcAccel I also observe the miscompile with just the flags `-O3 -mcpu=neoverse-512tvb` which has nothing to do with VLS SVE. The final IR looks like

```
vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %offset.idx = add i64 %index, %0
  %13 = add i64 %offset.idx, %8
  %14 = add i64 %offset.idx, %11
  %15 = add i64 %offset.idx, %12
  %16 = getelementptr inbounds [16 x i8], ptr %pf, i64 %offset.idx
  %17 = getelementptr inbounds [16 x i8], ptr %pf, i64 %13
  %18 = getelementptr inbounds [16 x i8], ptr %pf, i64 %14
  %19 = getelementptr inbounds [16 x i8], ptr %pf, i64 %15
  %20 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %offset.idx
  %21 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %13
  %22 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %14
  %23 = getelementptr inbounds [8 x i8], ptr %pmass, i64 %15
  %24 = load double, ptr %20, align 8, !tbaa !51, !alias.scope !234
  %25 = load double, ptr %21, align 8, !tbaa !51, !alias.scope !234
  %26 = load double, ptr %22, align 8, !tbaa !51, !alias.scope !234
  %27 = load double, ptr %23, align 8, !tbaa !51, !alias.scope !234
...
  %wide.load = load <vscale x 2 x double>, ptr %16, align 8
  %wide.load38 = load <vscale x 2 x double>, ptr %17, align 8
  %wide.load39 = load <vscale x 2 x double>, ptr %18, align 8
  %wide.load40 = load <vscale x 2 x double>, ptr %19, align 8
```
  
which looks wrong because we're loading a scalable vector's worth from %pf for every contiguous scalar load from %pmass. The IR is only correct when vscale=1 and I would expect to the same bug for RISCV targets. cc @lukel97 

---

### Comment 25 - fhahn

Hm interesting. I can have a look

---

### Comment 26 - artagnon

The bug is probably in this codepath?

```cpp
  if (VFToOptimize->isScalable()) {
    VPValue *VScale = PHBuilder.createElementCount(
        VectorLoop->getCanonicalIVType(), ElementCount::getScalable(1));
    VPValue *VScaleUF = PHBuilder.createOverflowingOp(
        Instruction::Mul, {VScale, UF}, {true, false});
    Inc->setOperand(1, VScaleUF);
    Plan.getVF().replaceAllUsesWith(VScale);
  } else {
```

---

### Comment 27 - artagnon

> The IR is only correct when vscale=1 and I would expect to the same bug for RISCV targets.

Hm, can @asb confirm that there was a failure on the RISC-V buildbots prior to the recent revert? I think the buildbots are configured to check the result via qemu emulation, and I believe there's also some real hardware in the RISC-V farm now?

---

### Comment 28 - artagnon

I tried a quick example, and there seems to be a larger problem with narrowing interleave groups for scalable VF: https://godbolt.org/z/dxKKrdbEd -- I will look into this, if Florian doesn't get to it first.

---

