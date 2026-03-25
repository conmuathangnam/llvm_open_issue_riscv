# [AArch64] SIGSEGV in Greedy/Basic Register Allocator on valid IR with phi nodes

**Author:** davidnich
**URL:** https://github.com/llvm/llvm-project/issues/181566
**Status:** Open
**Labels:** llvm:regalloc, crash

## Body

## Description

`llc` crashes with SIGSEGV during the Greedy Register Allocator pass when compiling valid LLVM IR on AArch64. The IR passes `opt -passes=verify` successfully. The crash also reproduces with `-regalloc=basic` but NOT with `-regalloc=fast` or `-O0`.

## Reproducer

Reduced with `llvm-reduce`:

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"

declare void @qore_rt_decref(i64)

declare i64 @qore_rt_has_exception()

declare i64 @qore_rt_load_local()

define i64 @"getDisplayName@187927647982096"(i1 %.not, i64 %0, i1 %.not190, i64 %1, i64 %2, i64 %3, i64 %4, i64 %5, i64 %6, i1 %.not185, i64 %7, i64 %reload_tracker8.2.sink) {
entry:
  %8 = call i64 @qore_rt_load_local()
  br i1 %.not, label %no_exception, label %common.ret

for.cond.0:                                       ; preds = %no_exception11
  %9 = load i64, ptr null, align 8
  br i1 %.not190, label %no_exception20, label %common.ret

or.rhs.4:                                         ; preds = %no_exception20
  %10 = load volatile i64, ptr null, align 8
  %11 = call i64 @qore_rt_has_exception()
  %.not211 = icmp eq i64 0, 0
  br label %common.ret

if.then.6:                                        ; preds = %no_exception20
  %12 = add i64 0, 0
  %13 = call i64 @qore_rt_has_exception()
  %.not213 = icmp eq i64 0, 0
  br label %common.ret

common.ret:                                       ; preds = %no_exception11, %no_exception9, %no_exception, %if.then.6, %or.rhs.4, %for.cond.0, %entry
  %reload_tracker.2.sink = phi i64 [ 0, %entry ], [ 0, %no_exception ], [ %7, %no_exception9 ], [ %7, %no_exception11 ], [ %9, %or.rhs.4 ], [ 0, %if.then.6 ], [ %9, %for.cond.0 ]
  %reload_tracker6.3.sink = phi i64 [ 0, %entry ], [ 0, %no_exception ], [ 1, %no_exception9 ], [ 0, %no_exception11 ], [ 0, %or.rhs.4 ], [ 0, %if.then.6 ], [ 0, %for.cond.0 ]
  %reload_tracker8.2.sink1 = phi i64 [ 1, %entry ], [ 0, %no_exception ], [ 0, %no_exception9 ], [ 0, %no_exception11 ], [ 0, %or.rhs.4 ], [ 0, %if.then.6 ], [ 0, %for.cond.0 ]
  %cleanup30.2.sink4 = phi i64 [ 0, %entry ], [ 0, %no_exception ], [ 0, %no_exception9 ], [ 0, %no_exception11 ], [ 0, %or.rhs.4 ], [ 0, %if.then.6 ], [ 0, %for.cond.0 ]
  call void @qore_rt_decref(i64 %3)
  store volatile i64 %1, ptr null, align 8
  store volatile i64 %5, ptr null, align 8
  store volatile i64 %6, ptr null, align 8
  store volatile i64 %reload_tracker.2.sink, ptr null, align 8
  store volatile i64 %reload_tracker6.3.sink, ptr null, align 8
  store volatile i64 %reload_tracker8.2.sink, ptr null, align 8
  store volatile i64 %2, ptr null, align 8
  store volatile i64 %4, ptr null, align 8
  store volatile i64 %0, ptr null, align 8
  ret i64 0

no_exception:                                     ; preds = %entry
  br i1 %.not190, label %no_exception9, label %common.ret

no_exception9:                                    ; preds = %no_exception
  br i1 %.not185, label %no_exception11, label %common.ret

no_exception11:                                   ; preds = %no_exception9
  br i1 %.not, label %for.cond.0, label %common.ret

no_exception20:                                   ; preds = %for.cond.0
  br i1 %.not, label %if.then.6, label %or.rhs.4
}
```

## Steps to Reproduce

Save the above as `reduced.ll`, then:

```
$ llc reduced.ll -o /dev/null
```

## Crash Output

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llc-20 /tmp/reduced.ll -o /dev/null
1.	Running pass 'Function Pass Manager' on module '/tmp/reduced.ll'.
2.	Running pass 'Greedy Register Allocator' on function '@"getDisplayName@187927647982096"'
 #0  llvm::sys::PrintStackTrace(llvm::raw_ostream&, int)
 ...
 #8  llvm::RegAllocBase::allocatePhysRegs()
 #9  <RAGreedy::runOnMachineFunction>
 #10 llvm::MachineFunctionPass::runOnFunction(llvm::Function&)
 #11 llvm::FPPassManager::runOnFunction(llvm::Function&)
 #12 llvm::FPPassManager::runOnModule(llvm::Module&)
 #13 llvm::legacy::PassManagerImpl::run(llvm::Module&)
Segmentation fault (core dumped)
```

## Observations

| Command | Result |
|---|---|
| `llc reduced.ll` | **SIGSEGV** in Greedy Register Allocator |
| `llc -regalloc=basic reduced.ll` | **SIGSEGV** in Basic Register Allocator |
| `llc -regalloc=fast reduced.ll` | **OK** |
| `llc -O0 reduced.ll` | **OK** |
| `opt -passes=verify reduced.ll` | **OK** (IR is valid) |

## Environment

- LLVM version: 20.1.8 (Ubuntu `1:20.1.8-0ubuntu4`)
- Target: `aarch64-unknown-linux-gnu`
- Host CPU: `cortex-a72`
- OS: Ubuntu 25.10, Linux 6.17.0-12-generic aarch64

