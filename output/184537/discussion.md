# [hexagon] Cannot select: f64 = bitcast

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/184537
**Status:** Open
**Labels:** backend:Hexagon, crash

## Body

The Hexagon backend crashes with `LLVM ERROR: Cannot select: f64 = bitcast` when LLVM IR contains bitcasts between `<N x i32>` and `<N/2 x double>`, observed on 22.1.0.

This occurs in Eigen's **generic clang vector backend** (`EIGEN_VECTORIZE_GENERIC`) for `complex<float>` packet math. The generic backend uses `<2 x double>` as an intermediate type for 64-bit-wide bitmask operations on complex elements (each complex element is a real+imag float pair = 64 bits).

Example:

```bash
llc -march=hexagon -mcpu=hexagonv79 -mattr=+hvx-length128b,+hvx-ieee-fp \
    f64_bitcast_minimal.ll -o /dev/null
```

f64_bitcast_minimal.ll:

```llvm
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @f64_bitcast_crash(ptr %out, <4 x float> %input) {
entry:
  ; Step 1: compute a <4 x i32> bitmask from comparison results
  %cmp = fcmp olt <4 x float> %input, zeroinitializer
  %sext = sext <4 x i1> %cmp to <4 x i32>
  %shuffled = shufflevector <4 x i32> %sext, <4 x i32> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  %mask = or <4 x i32> %shuffled, %sext

  ; Step 2: bitcast <4 x i32> mask to <2 x double> for 64-bit-wide operations
  %mask_d = bitcast <4 x i32> %mask to <2 x double>

  ; Step 3: fcmp on the <2 x double> (really checking if 64-bit mask is nonzero)
  %nonzero = fcmp une <2 x double> %mask_d, zeroinitializer

  ; Step 4: use the <2 x double> result for selection
  %val_d = bitcast <4 x float> %input to <2 x double>
  %result_d = select <2 x i1> %nonzero, <2 x double> %mask_d, <2 x double> %val_d

  ; Step 5: convert back and store
  %result = bitcast <2 x double> %result_d to <4 x float>
  store <4 x float> %result, ptr %out, align 16
  ret void
}

```

Output:
```
	.attribute	4, 68	// Tag_arch
	.attribute	10, 1	// Tag_cabac
	.file	"<stdin>"
LLVM ERROR: Cannot select: t100: f64 = bitcast t94
  t94: v2i32 = or t173, t90
    t173: v2i32 = bitcast t186
      t186: i64 = HexagonISD::COMBINE t175, t176
        t175: i32 = EXTRACT_SUBREG t174, TargetConstant:i32<2>
          t174: i64 = bitcast t90
            t90: v2i32 = sign_extend t148
              t148: v2i1 = C2_tfrrp t147
                t147: i32 = or t145, t146
                  t145: i32 = or t190, t192


                  t146: i32 = or t194, t196


        t176: i32 = EXTRACT_SUBREG t174, TargetConstant:i32<1>
          t174: i64 = bitcast t90
            t90: v2i32 = sign_extend t148
              t148: v2i1 = C2_tfrrp t147
                t147: i32 = or t145, t146
                  t145: i32 = or t190, t192


                  t146: i32 = or t194, t196


    t90: v2i32 = sign_extend t148
      t148: v2i1 = C2_tfrrp t147
        t147: i32 = or t145, t146
          t145: i32 = or t190, t192
            t190: i32 = select t46, t189, t130
              t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch
                t8: f32,ch = CopyFromReg t0, Register:f32 %3
              t189: i32 = or t130, Constant:i32<1>
                t130: i32 = select t46, Constant:i32<2>, Constant:i32<0>
                  t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch

              t130: i32 = select t46, Constant:i32<2>, Constant:i32<0>
                t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch
                  t8: f32,ch = CopyFromReg t0, Register:f32 %3
            t192: i32 = select t46, t191, t133
              t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch
                t8: f32,ch = CopyFromReg t0, Register:f32 %3
              t191: i32 = or t133, Constant:i32<4>
                t133: i32 = select t46, Constant:i32<8>, Constant:i32<0>
                  t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch

              t133: i32 = select t46, Constant:i32<8>, Constant:i32<0>
                t46: i1 = setcc t8, ConstantFP:f32<0.000000e+00>, setolt:ch
                  t8: f32,ch = CopyFromReg t0, Register:f32 %3
          t146: i32 = or t194, t196
            t194: i32 = select t54, t193, t136
              t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch
                t10: f32,ch = CopyFromReg t0, Register:f32 %4
              t193: i32 = or t136, Constant:i32<16>
                t136: i32 = select t54, Constant:i32<32>, Constant:i32<0>
                  t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch

              t136: i32 = select t54, Constant:i32<32>, Constant:i32<0>
                t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch
                  t10: f32,ch = CopyFromReg t0, Register:f32 %4
            t196: i32 = select t54, t195, t140
              t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch
                t10: f32,ch = CopyFromReg t0, Register:f32 %4
              t195: i32 = or t140, Constant:i32<64>
                t140: i32 = select t54, Constant:i32<128>, Constant:i32<0>
                  t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch

              t140: i32 = select t54, Constant:i32<128>, Constant:i32<0>
                t54: i1 = setcc t10, ConstantFP:f32<0.000000e+00>, setolt:ch
                  t10: f32,ch = CopyFromReg t0, Register:f32 %4
In function: f64_bitcast_crash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./bin/llc -O2 -mtriple=hexagon-linux
1.	Running pass 'Function Pass Manager' on module '<stdin>'.
2.	Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@f64_bitcast_crash'
 #0 0x000055b5752953d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/llc+0x49d53d8)
 #1 0x000055b575292b49 llvm::sys::RunSignalHandlers() (./bin/llc+0x49d2b49)
 #2 0x000055b575295ff1 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0

```

## Comments

### Comment 1 - androm3da

cc @sgundapa 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
The Hexagon backend crashes with `LLVM ERROR: Cannot select: f64 = bitcast` when LLVM IR contains bitcasts between `&lt;N x i32&gt;` and `&lt;N/2 x double&gt;`, observed on 22.1.0.

This occurs in Eigen's **generic clang vector backend** (`EIGEN_VECTORIZE_GENERIC`) for `complex&lt;float&gt;` packet math. The generic backend uses `&lt;2 x double&gt;` as an intermediate type for 64-bit-wide bitmask operations on complex elements (each complex element is a real+imag float pair = 64 bits).

Example:

```bash
llc -march=hexagon -mcpu=hexagonv79 -mattr=+hvx-length128b,+hvx-ieee-fp \
    f64_bitcast_minimal.ll -o /dev/null
```

f64_bitcast_minimal.ll:

```llvm
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @<!-- -->f64_bitcast_crash(ptr %out, &lt;4 x float&gt; %input) {
entry:
  ; Step 1: compute a &lt;4 x i32&gt; bitmask from comparison results
  %cmp = fcmp olt &lt;4 x float&gt; %input, zeroinitializer
  %sext = sext &lt;4 x i1&gt; %cmp to &lt;4 x i32&gt;
  %shuffled = shufflevector &lt;4 x i32&gt; %sext, &lt;4 x i32&gt; poison, &lt;4 x i32&gt; &lt;i32 1, i32 0, i32 3, i32 2&gt;
  %mask = or &lt;4 x i32&gt; %shuffled, %sext

  ; Step 2: bitcast &lt;4 x i32&gt; mask to &lt;2 x double&gt; for 64-bit-wide operations
  %mask_d = bitcast &lt;4 x i32&gt; %mask to &lt;2 x double&gt;

  ; Step 3: fcmp on the &lt;2 x double&gt; (really checking if 64-bit mask is nonzero)
  %nonzero = fcmp une &lt;2 x double&gt; %mask_d, zeroinitializer

  ; Step 4: use the &lt;2 x double&gt; result for selection
  %val_d = bitcast &lt;4 x float&gt; %input to &lt;2 x double&gt;
  %result_d = select &lt;2 x i1&gt; %nonzero, &lt;2 x double&gt; %mask_d, &lt;2 x double&gt; %val_d

  ; Step 5: convert back and store
  %result = bitcast &lt;2 x double&gt; %result_d to &lt;4 x float&gt;
  store &lt;4 x float&gt; %result, ptr %out, align 16
  ret void
}

```

Output:
```
	.attribute	4, 68	// Tag_arch
	.attribute	10, 1	// Tag_cabac
	.file	"&lt;stdin&gt;"
LLVM ERROR: Cannot select: t100: f64 = bitcast t94
  t94: v2i32 = or t173, t90
    t173: v2i32 = bitcast t186
      t186: i64 = HexagonISD::COMBINE t175, t176
        t175: i32 = EXTRACT_SUBREG t174, TargetConstant:i32&lt;2&gt;
          t174: i64 = bitcast t90
            t90: v2i32 = sign_extend t148
              t148: v2i1 = C2_tfrrp t147
                t147: i32 = or t145, t146
                  t145: i32 = or t190, t192


                  t146: i32 = or t194, t196


        t176: i32 = EXTRACT_SUBREG t174, TargetConstant:i32&lt;1&gt;
          t174: i64 = bitcast t90
            t90: v2i32 = sign_extend t148
              t148: v2i1 = C2_tfrrp t147
                t147: i32 = or t145, t146
                  t145: i32 = or t190, t192


                  t146: i32 = or t194, t196


    t90: v2i32 = sign_extend t148
      t148: v2i1 = C2_tfrrp t147
        t147: i32 = or t145, t146
          t145: i32 = or t190, t192
            t190: i32 = select t46, t189, t130
              t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                t8: f32,ch = CopyFromReg t0, Register:f32 %3
              t189: i32 = or t130, Constant:i32&lt;1&gt;
                t130: i32 = select t46, Constant:i32&lt;2&gt;, Constant:i32&lt;0&gt;
                  t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch

              t130: i32 = select t46, Constant:i32&lt;2&gt;, Constant:i32&lt;0&gt;
                t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                  t8: f32,ch = CopyFromReg t0, Register:f32 %3
            t192: i32 = select t46, t191, t133
              t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                t8: f32,ch = CopyFromReg t0, Register:f32 %3
              t191: i32 = or t133, Constant:i32&lt;4&gt;
                t133: i32 = select t46, Constant:i32&lt;8&gt;, Constant:i32&lt;0&gt;
                  t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch

              t133: i32 = select t46, Constant:i32&lt;8&gt;, Constant:i32&lt;0&gt;
                t46: i1 = setcc t8, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                  t8: f32,ch = CopyFromReg t0, Register:f32 %3
          t146: i32 = or t194, t196
            t194: i32 = select t54, t193, t136
              t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                t10: f32,ch = CopyFromReg t0, Register:f32 %4
              t193: i32 = or t136, Constant:i32&lt;16&gt;
                t136: i32 = select t54, Constant:i32&lt;32&gt;, Constant:i32&lt;0&gt;
                  t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch

              t136: i32 = select t54, Constant:i32&lt;32&gt;, Constant:i32&lt;0&gt;
                t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                  t10: f32,ch = CopyFromReg t0, Register:f32 %4
            t196: i32 = select t54, t195, t140
              t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                t10: f32,ch = CopyFromReg t0, Register:f32 %4
              t195: i32 = or t140, Constant:i32&lt;64&gt;
                t140: i32 = select t54, Constant:i32&lt;128&gt;, Constant:i32&lt;0&gt;
                  t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch

              t140: i32 = select t54, Constant:i32&lt;128&gt;, Constant:i32&lt;0&gt;
                t54: i1 = setcc t10, ConstantFP:f32&lt;0.000000e+00&gt;, setolt:ch
                  t10: f32,ch = CopyFromReg t0, Register:f32 %4
In function: f64_bitcast_crash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./bin/llc -O2 -mtriple=hexagon-linux
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->f64_bitcast_crash'
 #<!-- -->0 0x000055b5752953d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./bin/llc+0x49d53d8)
 #<!-- -->1 0x000055b575292b49 llvm::sys::RunSignalHandlers() (./bin/llc+0x49d2b49)
 #<!-- -->2 0x000055b575295ff1 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0

```
</details>


---

