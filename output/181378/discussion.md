# [hexagon] UNREACHABLE in LegalizeTypes.cpp

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/181378
**Status:** Closed
**Labels:** backend:Hexagon, release:backport, crash
**Closed Date:** 2026-02-20T01:36:35Z

## Body

# Crash Report

**LLVM Ref:** `release/22.x` (c780d890daf3474c73c4ffa736e155c619c3d0be)

Same signature was observed from compiling the cases below from llvm-test-suite, all with the same flags (`-O2 -target-cpu hexagonv73 -target-feature +hvxv73 -target-feature +hvx-length128b`):

- `DeflateEncoder`
- `jquant2`
- `Ppmd8`
- `ssad`
- `usad`


## Crash Signature

```
Unprocessed value in a map! SplitVectors
UNREACHABLE executed at /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:183!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name DeflateEncoder.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -Wno-error=narrowing -w -fdeprecated-macro -ferror-limit 19 -pthread -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp -o /dev/null
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp'.
4.	Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@_ZNK9NCompress8NDeflate8NEncoder6CCoder15GetLzBlockPriceEv'
 #0 0x000000000204d877 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204d877)
 #1 0x000000000204b0b5 llvm::sys::RunSignalHandlers() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204b0b5)
 #2 0x000000000204e67e SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00002b14b61c5520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00002b14b62199fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00002b14b61c5476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00002b14b61ab7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000001fa0e51 (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x1fa0e51)
 #8 0x0000000003bcd4cd llvm::DAGTypeLegalizer::PerformExpensiveChecks() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bcd4cd)
 #9 0x0000000003bcd928 llvm::DAGTypeLegalizer::run() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bcd928)
#10 0x0000000003bd3055 llvm::SelectionDAG::LegalizeTypes() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bd3055)
#11 0x0000000003a3a0bf llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3a3a0bf)
```

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -fcxx-exceptions -vectorize-slp -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp -o /dev/null
```

## LLVM IR

IR file: `test.ll` (584086 bytes)

## Reduced Test Case

Reduced file: `reduced.ll` (2492 bytes)

```
./bin/llc -mtriple=hexagon -mcpu=hexagonv73 -mattr=+hvxv73,+hvx-length128b -enable-legalize-types-checking ./reduced.ll
```

```llvm
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define <64 x i32> @_ZNK9NCompress8NDeflate8NEncoder6CCoder15GetLzBlockPriceEv(<64 x i8> %0, <64 x i8> %1, <64 x i8> %2) {
entry:
  %3 = zext <64 x i8> %0 to <64 x i32>
  %4 = shufflevector <64 x i32> zeroinitializer, <64 x i32> %3, <64 x i32> <i32 0, i32 1, i32 2, i32 64, i32 65, i32 66, i32 67, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63>
  %5 = zext <64 x i8> %1 to <64 x i32>
  %6 = shufflevector <64 x i32> %4, <64 x i32> %5, <64 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 64, i32 65, i32 66, i32 67, i32 68, i32 69, i32 70, i32 71, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63>
  %7 = zext <64 x i8> %2 to <64 x i32>
  %8 = shufflevector <64 x i32> %6, <64 x i32> %7, <64 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 64, i32 65, i32 66, i32 67, i32 68, i32 69, i32 70, i32 71, i32 72, i32 73, i32 74, i32 75, i32 76, i32 77, i32 78, i32 79, i32 80, i32 81, i32 82, i32 83, i32 84, i32 85, i32 86, i32 87, i32 88, i32 89, i32 90, i32 91, i32 92, i32 93, i32 94, i32 95, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63>
  ret <64 x i32> %8
}
```


## Comments

### Comment 1 - androm3da

Fixed by #181377

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
# Crash Report

**LLVM Ref:** `release/22.x` (c780d890daf3474c73c4ffa736e155c619c3d0be)

Same signature was observed from compiling the cases below from llvm-test-suite, all with the same flags (`-O2 -target-cpu hexagonv73 -target-feature +hvxv73 -target-feature +hvx-length128b`):

- `DeflateEncoder`
- `jquant2`
- `Ppmd8`
- `ssad`
- `usad`


## Crash Signature

```
Unprocessed value in a map! SplitVectors
UNREACHABLE executed at /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:183!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name DeflateEncoder.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -Wno-error=narrowing -w -fdeprecated-macro -ferror-limit 19 -pthread -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp -o /dev/null
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp'.
4.	Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_ZNK9NCompress8NDeflate8NEncoder6CCoder15GetLzBlockPriceEv'
 #<!-- -->0 0x000000000204d877 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204d877)
 #<!-- -->1 0x000000000204b0b5 llvm::sys::RunSignalHandlers() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204b0b5)
 #<!-- -->2 0x000000000204e67e SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00002b14b61c5520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00002b14b62199fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00002b14b61c5476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00002b14b61ab7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000001fa0e51 (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x1fa0e51)
 #<!-- -->8 0x0000000003bcd4cd llvm::DAGTypeLegalizer::PerformExpensiveChecks() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bcd4cd)
 #<!-- -->9 0x0000000003bcd928 llvm::DAGTypeLegalizer::run() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bcd928)
#<!-- -->10 0x0000000003bd3055 llvm::SelectionDAG::LegalizeTypes() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3bd3055)
#<!-- -->11 0x0000000003a3a0bf llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x3a3a0bf)
```

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -fcxx-exceptions -vectorize-slp -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-DeflateEncoder-715f9d/DeflateEncoder-715f9d.cpp -o /dev/null
```

## LLVM IR

IR file: `test.ll` (584086 bytes)

## Reduced Test Case

Reduced file: `reduced.ll` (2492 bytes)

```
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define &lt;64 x i32&gt; @<!-- -->_ZNK9NCompress8NDeflate8NEncoder6CCoder15GetLzBlockPriceEv(&lt;64 x i8&gt; %0, &lt;64 x i8&gt; %1, &lt;64 x i8&gt; %2) {
entry:
  %3 = zext &lt;64 x i8&gt; %0 to &lt;64 x i32&gt;
  %4 = shufflevector &lt;64 x i32&gt; zeroinitializer, &lt;64 x i32&gt; %3, &lt;64 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 64, i32 65, i32 66, i32 67, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63&gt;
  %5 = zext &lt;64 x i8&gt; %1 to &lt;64 x i32&gt;
  %6 = shufflevector &lt;64 x i32&gt; %4, &lt;64 x i32&gt; %5, &lt;64 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 64, i32 65, i32 66, i32 67, i32 68, i32 69, i32 70, i32 71, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63&gt;
  %7 = zext &lt;64 x i8&gt; %2 to &lt;64 x i32&gt;
  %8 = shufflevector &lt;64 x i32&gt; %6, &lt;64 x i32&gt; %7, &lt;64 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 64, i32 65, i32 66, i32 67, i32 68, i32 69, i32 70, i32 71, i32 72, i32 73, i32 74, i32 75, i32 76, i32 77, i32 78, i32 79, i32 80, i32 81, i32 82, i32 83, i32 84, i32 85, i32 86, i32 87, i32 88, i32 89, i32 90, i32 91, i32 92, i32 93, i32 94, i32 95, i32 47, i32 48, i32 49, i32 50, i32 51, i32 52, i32 53, i32 54, i32 55, i32 56, i32 57, i32 58, i32 59, i32 60, i32 61, i32 62, i32 63&gt;
  ret &lt;64 x i32&gt; %8
}

```

</details>


---

### Comment 3 - topperc

What is the llc command for reduced.ll test? I can't reproduce the failure and the test is different that what's in #181377

---

### Comment 4 - androm3da

> What is the llc command for reduced.ll test? I can't reproduce the failure and the test is different that what's in [#181377](https://github.com/llvm/llvm-project/pull/181377)

Sorry for omitting that, @topperc :

```
./bin/llc -mtriple=hexagon -mcpu=hexagonv73 -mattr=+hvxv73,+hvx-length128b -enable-legalize-types-checking ./reduced.ll
```

I'll edit the description to close that gap.

---

### Comment 5 - androm3da

/cherry-pick 4d3217d68914ddac47d760b215d71441b820720e

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#182418

---

