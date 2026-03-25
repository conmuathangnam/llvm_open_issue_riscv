# Assertion `VNI && "No live value at use"

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/128714
**Status:** Closed
**Labels:** llvm:codegen, release:backport, crash
**Closed Date:** 2025-04-08T03:37:18Z

## Body

This reduced test case from `llvm-test-suite/External/ffmpeg`'s `checkasm_check_v210enc()` asserts when compiling at O2:

```
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @checkasm_check_v210enc(i32 %.pre) {
entry:
  br label %for.body147

for.body147:                                      ; preds = %for.body170, %entry
  %add11.i526 = or i32 %.pre, 1
  br label %for.body158

for.body158:                                      ; preds = %for.body158, %for.body147
  %i154.0604 = phi i32 [ 0, %for.body147 ], [ %add166.3, %for.body158 ]
  %add11.i536602603 = phi i32 [ %add11.i526, %for.body147 ], [ 0, %for.body158 ]
  %and8.i534 = and i32 %add11.i536602603, 1
  %arrayidx9.i535 = getelementptr [64 x i32], ptr null, i32 0, i32 %and8.i534
  store i32 0, ptr %arrayidx9.i535, align 4
  %add166.3 = add i32 %i154.0604, 1
  %cmp157.3 = icmp ult i32 %i154.0604, 510
  br i1 %cmp157.3, label %for.body158, label %for.body170

for.body170:                                      ; preds = %for.body170, %for.body158
  %i154.1608 = phi i32 [ %add182.1, %for.body170 ], [ 0, %for.body158 ]
  %add11.i556606607 = phi i32 [ 0, %for.body170 ], [ 1, %for.body158 ]
  %add.ptr179 = getelementptr i16, ptr null, i32 %i154.1608
  store i32 0, ptr %add.ptr179, align 8
  %add182 = or disjoint i32 %i154.1608, 2
  %sub.i547.1 = add i32 %add11.i556606607, 1
  %and.i548.1 = and i32 %sub.i547.1, 1
  %arrayidx.i549.1 = getelementptr [64 x i32], ptr null, i32 0, i32 %and.i548.1
  %0 = load i32, ptr %arrayidx.i549.1, align 4
  %add.ptr178.1 = getelementptr i16, ptr null, i32 %add182
  store i32 %0, ptr %add.ptr178.1, align 4
  %add182.1 = add i32 %i154.1608, 1
  %cmp169.1 = icmp ult i32 %i154.1608, 254
  br i1 %cmp169.1, label %for.body170, label %for.body147
}

```

```
./bin/llc -O2 --mtriple=hexagon ../toolchain_for_hexagon/reduced_v210enc.ll 
ninja: no work to do.
llc: /local/mnt/workspace/upstream/llvm-project/llvm/lib/CodeGen/MachineScheduler.cpp:1406: void llvm::ScheduleDAGMILive::updatePressureDiffs(ArrayRef<RegisterMaskPair>): Assertion `VNI && "No live value at use."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./bin/llc -O2 --mtriple=hexagon ../toolchain_for_hexagon/reduced_v210enc.ll
1.	Running pass 'Function Pass Manager' on module '../toolchain_for_hexagon/reduced_v210enc.ll'.
2.	Running pass 'Modulo Software Pipelining' on function '@checkasm_check_v210enc'
 #0 0x000055e40be39ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/llc+0x3fbeff8)
 #1 0x000055e40be37a30 llvm::sys::RunSignalHandlers() (./bin/llc+0x3fbca30)
 #2 0x000055e40be3a71a SignalHandler(int) Signals.cpp:0:0
 #3 0x00007f14d8a66520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f14d8aba9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f14d8aba9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f14d8aba9fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f14d8a66476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f14d8a4c7f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f14d8a4c71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f14d8a5de96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055e40afc3333 llvm::ScheduleDAGMILive::updatePressureDiffs(llvm::ArrayRef<llvm::RegisterMaskPair>) (./bin/llc+0x3148333)
#12 0x000055e40afc4958 llvm::ScheduleDAGMILive::scheduleMI(llvm::SUnit*, bool) (./bin/llc+0x3149958)
#13 0x000055e40b255e97 llvm::VLIWMachineScheduler::schedule() (./bin/llc+0x33dae97)
#14 0x000055e40b261c64 llvm::WindowScheduler::run() (./bin/llc+0x33e6c64)
#15 0x000055e40af5d901 llvm::MachinePipeliner::runWindowScheduler(llvm::MachineLoop&) (./bin/llc+0x30e2901)
#16 0x000055e40af5b896 llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&) (./bin/llc+0x30e0896)
#17 0x000055e40af5b7eb llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&) (./bin/llc+0x30e07eb)
#18 0x000055e40af5b70b llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&) (./bin/llc+0x30e070b)
#19 0x000055e40aeeb999 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (./bin/llc+0x3070999)
#20 0x000055e40b44bfd9 llvm::FPPassManager::runOnFunction(llvm::Function&) (./bin/llc+0x35d0fd9)
#21 0x000055e40b454632 llvm::FPPassManager::runOnModule(llvm::Module&) (./bin/llc+0x35d9632)
#22 0x000055e40b44cc34 llvm::legacy::PassManagerImpl::run(llvm::Module&) (./bin/llc+0x35d1c34)
#23 0x000055e40a047d27 main (./bin/llc+0x21ccd27)
#24 0x00007f14d8a4dd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#25 0x00007f14d8a4de40 call_init ./csu/../csu/libc-start.c:128:20
#26 0x00007f14d8a4de40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#27 0x000055e40a041d25 _start (./bin/llc+0x21c6d25)
Aborted (core dumped)

```

I have bisected this failure to b6bf4024a031a5e7b58aff1425d94841a88002d6

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
This reduced test case from `llvm-test-suite/External/ffmpeg`'s `checkasm_check_v210enc()` asserts when compiling at O2:

```
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @<!-- -->checkasm_check_v210enc(i32 %.pre) {
entry:
  br label %for.body147

for.body147:                                      ; preds = %for.body170, %entry
  %add11.i526 = or i32 %.pre, 1
  br label %for.body158

for.body158:                                      ; preds = %for.body158, %for.body147
  %i154.0604 = phi i32 [ 0, %for.body147 ], [ %add166.3, %for.body158 ]
  %add11.i536602603 = phi i32 [ %add11.i526, %for.body147 ], [ 0, %for.body158 ]
  %and8.i534 = and i32 %add11.i536602603, 1
  %arrayidx9.i535 = getelementptr [64 x i32], ptr null, i32 0, i32 %and8.i534
  store i32 0, ptr %arrayidx9.i535, align 4
  %add166.3 = add i32 %i154.0604, 1
  %cmp157.3 = icmp ult i32 %i154.0604, 510
  br i1 %cmp157.3, label %for.body158, label %for.body170

for.body170:                                      ; preds = %for.body170, %for.body158
  %i154.1608 = phi i32 [ %add182.1, %for.body170 ], [ 0, %for.body158 ]
  %add11.i556606607 = phi i32 [ 0, %for.body170 ], [ 1, %for.body158 ]
  %add.ptr179 = getelementptr i16, ptr null, i32 %i154.1608
  store i32 0, ptr %add.ptr179, align 8
  %add182 = or disjoint i32 %i154.1608, 2
  %sub.i547.1 = add i32 %add11.i556606607, 1
  %and.i548.1 = and i32 %sub.i547.1, 1
  %arrayidx.i549.1 = getelementptr [64 x i32], ptr null, i32 0, i32 %and.i548.1
  %0 = load i32, ptr %arrayidx.i549.1, align 4
  %add.ptr178.1 = getelementptr i16, ptr null, i32 %add182
  store i32 %0, ptr %add.ptr178.1, align 4
  %add182.1 = add i32 %i154.1608, 1
  %cmp169.1 = icmp ult i32 %i154.1608, 254
  br i1 %cmp169.1, label %for.body170, label %for.body147
}

```

I have bisected this failure to b6bf4024a031a5e7b58aff1425d94841a88002d6
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Brian Cain (androm3da)

<details>
This reduced test case from `llvm-test-suite/External/ffmpeg`'s `checkasm_check_v210enc()` asserts when compiling at O2:

```
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @<!-- -->checkasm_check_v210enc(i32 %.pre) {
entry:
  br label %for.body147

for.body147:                                      ; preds = %for.body170, %entry
  %add11.i526 = or i32 %.pre, 1
  br label %for.body158

for.body158:                                      ; preds = %for.body158, %for.body147
  %i154.0604 = phi i32 [ 0, %for.body147 ], [ %add166.3, %for.body158 ]
  %add11.i536602603 = phi i32 [ %add11.i526, %for.body147 ], [ 0, %for.body158 ]
  %and8.i534 = and i32 %add11.i536602603, 1
  %arrayidx9.i535 = getelementptr [64 x i32], ptr null, i32 0, i32 %and8.i534
  store i32 0, ptr %arrayidx9.i535, align 4
  %add166.3 = add i32 %i154.0604, 1
  %cmp157.3 = icmp ult i32 %i154.0604, 510
  br i1 %cmp157.3, label %for.body158, label %for.body170

for.body170:                                      ; preds = %for.body170, %for.body158
  %i154.1608 = phi i32 [ %add182.1, %for.body170 ], [ 0, %for.body158 ]
  %add11.i556606607 = phi i32 [ 0, %for.body170 ], [ 1, %for.body158 ]
  %add.ptr179 = getelementptr i16, ptr null, i32 %i154.1608
  store i32 0, ptr %add.ptr179, align 8
  %add182 = or disjoint i32 %i154.1608, 2
  %sub.i547.1 = add i32 %add11.i556606607, 1
  %and.i548.1 = and i32 %sub.i547.1, 1
  %arrayidx.i549.1 = getelementptr [64 x i32], ptr null, i32 0, i32 %and.i548.1
  %0 = load i32, ptr %arrayidx.i549.1, align 4
  %add.ptr178.1 = getelementptr i16, ptr null, i32 %add182
  store i32 %0, ptr %add.ptr178.1, align 4
  %add182.1 = add i32 %i154.1608, 1
  %cmp169.1 = icmp ult i32 %i154.1608, 254
  br i1 %cmp169.1, label %for.body170, label %for.body147
}

```

I have bisected this failure to b6bf4024a031a5e7b58aff1425d94841a88002d6
</details>


---

### Comment 3 - androm3da

Note also -- this same assertion also occurs with other tests in the test suite at O2, full list:

```
llvm-test-suite/MultiSource/Benchmarks/Prolangs-C++/shapes/bjarne.cpp
llvm-test-suite/test-suite-externals/ffmpeg/libavcodec/mss4.c
llvm-test-suite/test-suite-externals/ffmpeg/libavcodec/vp9.c
llvm-test-suite/test-suite-externals/ffmpeg/tests/checkasm/v210enc.c
```

---

### Comment 4 - iajbar

@huaatian, since https://github.com/llvm/llvm-project/commit/b6bf4024a031a5e7b58aff1425d94841a88002d6 is the culprit, could you please take a look at this crash? Thank you.

---

### Comment 5 - huaatian

> [@huaatian](https://github.com/huaatian), since [b6bf402](https://github.com/llvm/llvm-project/commit/b6bf4024a031a5e7b58aff1425d94841a88002d6) is the culprit, could you please take a look at this crash? Thank you.

OK, I'll take a look at this issue.

---

### Comment 6 - androm3da

/cherry-pick b09b9ac1081d19c8021df8e55e96cd1325f0eed0

---

### Comment 7 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: b09b9ac1081d19c8021df8e55e96cd1325f0eed0

https://github.com/llvm/llvm-project/actions/runs/13902530883

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 8 - androm3da

> Failed to cherry-pick: [b09b9ac](https://github.com/llvm/llvm-project/commit/b09b9ac1081d19c8021df8e55e96cd1325f0eed0)
> 
> https://github.com/llvm/llvm-project/actions/runs/13902530883
> 
> Please manually backport the fix and push it to your github fork. Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

@huaatian do you think this change is appropriate to be cherry-picked to `release/20.x`?  If so, are there other commits on `main` that this might depend on?  If you give me this guidance/hints I can do the backport/cherry-pick.

---

### Comment 9 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: .

https://github.com/llvm/llvm-project/actions/runs/13903448191

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 10 - huaatian

> > Failed to cherry-pick: [b09b9ac](https://github.com/llvm/llvm-project/commit/b09b9ac1081d19c8021df8e55e96cd1325f0eed0)
> > https://github.com/llvm/llvm-project/actions/runs/13902530883
> > Please manually backport the fix and push it to your github fork. Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)
> 
> [@huaatian](https://github.com/huaatian) do you think this change is appropriate to be cherry-picked to `release/20.x`? If so, are there other commits on `main` that this might depend on? If you give me this guidance/hints I can do the backport/cherry-pick.

@androm3da
I think patch https://github.com/llvm/llvm-project/pull/129204 is safe to cherry-picked to `release/20.x`. 
I have resolved its conflict with patch https://github.com/llvm/llvm-project/pull/130237, which may be the reason for the cherrypick failure.

---

### Comment 11 - huaatian

I think this bugfix patch (https://github.com/llvm/llvm-project/pull/132352 ) is needed, too.

---

### Comment 12 - androm3da

/cherry-pick 616f27764012  b09b9ac1081d 7e6594429227

---

### Comment 13 - llvmbot

/pull-request llvm/llvm-project#134775

---

