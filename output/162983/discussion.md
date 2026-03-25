# [AArch64] incorrect use of EVT::getVectorNumElements()

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/162983
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2025-10-12T07:53:28Z

## Body

This IR reproducer:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define i64 @_Z1ePA2_A2_x(<2 x i32> %0) #0 {
  %2 = sdiv <2 x i32> splat (i32 1), %0
  %3 = sext <2 x i32> %2 to <2 x i64>
  %4 = add <2 x i64> %3, %3
  %5 = tail call i64 @llvm.vector.reduce.add.v2i64(<2 x i64> %4)
  ret i64 %5
}

declare i64 @llvm.vector.reduce.add.v2i64(<2 x i64>) #1

attributes #0 = { "target-cpu"="grace" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

results in this crash:

```
LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector. Scalable flag may be dropped, use EVT::getVectorElementCount() instead
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@_Z1ePA2_A2_x'
 #0 0x0000000003be2f38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3be2f38)
 #1 0x0000000003be0104 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007c96dd242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007c96dd2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007c96dd242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007c96dd2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007ba680 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000003b2c6c9 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b2c6c9)
 #8 0x0000000000afa481 (/opt/compiler-explorer/clang-trunk/bin/llc+0xafa481)
 #9 0x0000000000c82ffb performUADDVAddCombine(llvm::SDValue, llvm::SelectionDAG&) (.isra.0) AArch64ISelLowering.cpp:0:0
#10 0x0000000000c8943f performUADDVCombine(llvm::SDNode*, llvm::SelectionDAG&) AArch64ISelLowering.cpp:0:0
#11 0x0000000000d0fd6d llvm::AArch64TargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const (/opt/compiler-explorer/clang-trunk/bin/llc+0xd0fd6d)
#12 0x000000000383911d (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#13 0x000000000383a53d (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
```

See also: https://godbolt.org/z/eWGocKGqb

The IR reproducer comes from this C reproducer:

```
long long a;
short b;
int c;
short *d;
void e(long long f[][2][2]) {
  for (short g;;)
    for (long h = 0; h < 22; h++) {
      a -= ~(c / d[g]);
      b = f[2][2][h] > 0;
    }
}
```

See also: https://godbolt.org/z/nqzKvcbj5

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Sjoerd Meijer (sjoerdmeijer)

<details>
This IR reproducer:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define i64 @<!-- -->_Z1ePA2_A2_x(&lt;2 x i32&gt; %0) #<!-- -->0 {
  %2 = sdiv &lt;2 x i32&gt; splat (i32 1), %0
  %3 = sext &lt;2 x i32&gt; %2 to &lt;2 x i64&gt;
  %4 = add &lt;2 x i64&gt; %3, %3
  %5 = tail call i64 @<!-- -->llvm.vector.reduce.add.v2i64(&lt;2 x i64&gt; %4)
  ret i64 %5
}

declare i64 @<!-- -->llvm.vector.reduce.add.v2i64(&lt;2 x i64&gt;) #<!-- -->1

attributes #<!-- -->0 = { "target-cpu"="grace" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

results in this crash:

```
LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector. Scalable flag may be dropped, use EVT::getVectorElementCount() instead
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z1ePA2_A2_x'
 #<!-- -->0 0x0000000003be2f38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3be2f38)
 #<!-- -->1 0x0000000003be0104 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007c96dd242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007c96dd2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007c96dd242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007c96dd2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007ba680 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000003b2c6c9 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b2c6c9)
 #<!-- -->8 0x0000000000afa481 (/opt/compiler-explorer/clang-trunk/bin/llc+0xafa481)
 #<!-- -->9 0x0000000000c82ffb performUADDVAddCombine(llvm::SDValue, llvm::SelectionDAG&amp;) (.isra.0) AArch64ISelLowering.cpp:0:0
#<!-- -->10 0x0000000000c8943f performUADDVCombine(llvm::SDNode*, llvm::SelectionDAG&amp;) AArch64ISelLowering.cpp:0:0
#<!-- -->11 0x0000000000d0fd6d llvm::AArch64TargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/opt/compiler-explorer/clang-trunk/bin/llc+0xd0fd6d)
#<!-- -->12 0x000000000383911d (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->13 0x000000000383a53d (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
```

See also: https://godbolt.org/z/eWGocKGqb

The IR reproducer comes from this C reproducer:

```
long long a;
short b;
int c;
short *d;
void e(long long f[][2][2]) {
  for (short g;;)
    for (long h = 0; h &lt; 22; h++) {
      a -= ~(c / d[g]);
      b = f[2][2][h] &gt; 0;
    }
}
```

See also: https://godbolt.org/z/nqzKvcbj5
</details>


---

### Comment 2 - sjoerdmeijer

CC: @paulwalker-arm , @david-arm, @davemgreen , @sdesmalen-arm 

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sjoerd Meijer (sjoerdmeijer)

<details>
This IR reproducer:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define i64 @<!-- -->_Z1ePA2_A2_x(&lt;2 x i32&gt; %0) #<!-- -->0 {
  %2 = sdiv &lt;2 x i32&gt; splat (i32 1), %0
  %3 = sext &lt;2 x i32&gt; %2 to &lt;2 x i64&gt;
  %4 = add &lt;2 x i64&gt; %3, %3
  %5 = tail call i64 @<!-- -->llvm.vector.reduce.add.v2i64(&lt;2 x i64&gt; %4)
  ret i64 %5
}

declare i64 @<!-- -->llvm.vector.reduce.add.v2i64(&lt;2 x i64&gt;) #<!-- -->1

attributes #<!-- -->0 = { "target-cpu"="grace" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

results in this crash:

```
LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector. Scalable flag may be dropped, use EVT::getVectorElementCount() instead
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z1ePA2_A2_x'
 #<!-- -->0 0x0000000003be2f38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3be2f38)
 #<!-- -->1 0x0000000003be0104 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007c96dd242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007c96dd2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007c96dd242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007c96dd2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007ba680 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000003b2c6c9 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b2c6c9)
 #<!-- -->8 0x0000000000afa481 (/opt/compiler-explorer/clang-trunk/bin/llc+0xafa481)
 #<!-- -->9 0x0000000000c82ffb performUADDVAddCombine(llvm::SDValue, llvm::SelectionDAG&amp;) (.isra.0) AArch64ISelLowering.cpp:0:0
#<!-- -->10 0x0000000000c8943f performUADDVCombine(llvm::SDNode*, llvm::SelectionDAG&amp;) AArch64ISelLowering.cpp:0:0
#<!-- -->11 0x0000000000d0fd6d llvm::AArch64TargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/opt/compiler-explorer/clang-trunk/bin/llc+0xd0fd6d)
#<!-- -->12 0x000000000383911d (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->13 0x000000000383a53d (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
```

See also: https://godbolt.org/z/eWGocKGqb

The IR reproducer comes from this C reproducer:

```
long long a;
short b;
int c;
short *d;
void e(long long f[][2][2]) {
  for (short g;;)
    for (long h = 0; h &lt; 22; h++) {
      a -= ~(c / d[g]);
      b = f[2][2][h] &gt; 0;
    }
}
```

See also: https://godbolt.org/z/nqzKvcbj5
</details>


---

