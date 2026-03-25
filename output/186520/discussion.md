# [ControlHeightReductionPass] Crash when compiling using profile data with PGO: Assertion `!NodePtr->isKnownSentinel()' failed.

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/186520
**Status:** Open
**Labels:** PGO, crash

## Body

Compiler Explorer: https://godbolt.org/z/1ej4o3YEn

small.ll
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define fastcc void @f1() !prof !29 {
entry:
  br i1 false, label %for.body147.lr.ph.i.us.us.us.us.us, label %f2.exit.us.us.us.us.us, !prof !30

for.body147.lr.ph.i.us.us.us.us.us:               ; preds = %entry
  %or.cond.peel.i.us.us.us.us.us = select i1 false, i1 false, i1 false, !prof !31
  br i1 false, label %cleanup.thread.peel.i.us.us.us.us.us, label %cond.false.i.peel.i.us.us.us.us.us, !prof !32

cond.false.i.peel.i.us.us.us.us.us:               ; preds = %for.body147.lr.ph.i.us.us.us.us.us
  br label %cleanup.thread.peel.i.us.us.us.us.us

cleanup.thread.peel.i.us.us.us.us.us:             ; preds = %cond.false.i.peel.i.us.us.us.us.us, %for.body147.lr.ph.i.us.us.us.us.us
  %cmp335.peel.i.us.us.us.us.us = select i1 %or.cond.peel.i.us.us.us.us.us, i1 false, i1 false, !prof !31
  br label %f2.exit.us.us.us.us.us

f2.exit.us.us.us.us.us:                           ; preds = %cleanup.thread.peel.i.us.us.us.us.us, %entry
  ret void
}

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"ProfileSummary", !1}
!1 = !{!2, !3, !4, !5, !6, !7, !8, !9, !10, !11}
!2 = !{!"ProfileFormat", !"InstrProf"}
!3 = !{!"TotalCount", i64 247478}
!4 = !{!"MaxCount", i64 56438}
!5 = !{!"MaxInternalCount", i64 56438}
!6 = !{!"MaxFunctionCount", i64 52772}
!7 = !{!"NumCounts", i64 431}
!8 = !{!"NumFunctions", i64 4}
!9 = !{!"IsPartialProfile", i64 0}
!10 = !{!"PartialProfileRatio", double 0.000000e+00}
!11 = !{!"DetailedSummary", !12}
!12 = !{!13, !14, !15, !16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28}
!13 = !{i32 10000, i64 56438, i32 1}
!14 = !{i32 100000, i64 56438, i32 1}
!15 = !{i32 200000, i64 56438, i32 1}
!16 = !{i32 300000, i64 52772, i32 2}
!17 = !{i32 400000, i64 52772, i32 2}
!18 = !{i32 500000, i64 51393, i32 3}
!19 = !{i32 600000, i64 51393, i32 3}
!20 = !{i32 700000, i64 41218, i32 4}
!21 = !{i32 800000, i64 41218, i32 4}
!22 = !{i32 900000, i64 11814, i32 6}
!23 = !{i32 950000, i64 8076, i32 7}
!24 = !{i32 990000, i64 996, i32 9}
!25 = !{i32 999000, i64 5, i32 128}
!26 = !{i32 999900, i64 2, i32 195}
!27 = !{i32 999990, i64 1, i32 211}
!28 = !{i32 999999, i64 1, i32 211}
!29 = !{!"function_entry_count", i64 5213}
!30 = !{!"branch_weights", i32 1, i32 -2}
!31 = !{!"branch_weights", i32 49297, i32 332925863}
!32 = !{!"branch_weights", i32 1, i32 997}
```

Reproduce:

```
$ opt -passes='require<profile-summary>,function(chr)' small.ll -o /dev/null
```

Crash:

```
opt: /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:322: llvm::ilist_iterator_w_bits::reference llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>::operator*() const [OptionsT = llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, IsReverse = false, IsConst = false]: Assertion `!NodePtr->isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: opt -passes=require<profile-summary>,function(chr) small.ll -o /dev/null
1.      Running pass "function(chr)" on module "small.ll"
2.      Running pass "chr" on function "f1"
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  opt       0x00005620fe3f5c87 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 39
1  opt       0x00005620fe3f392a llvm::sys::RunSignalHandlers() + 154
2  opt       0x00005620fe3f6b25
3  libc.so.6 0x00007faf77c8f520
4  libc.so.6 0x00007faf77ce39fc pthread_kill + 300
5  libc.so.6 0x00007faf77c8f476 raise + 22
6  libc.so.6 0x00007faf77c757f3 abort + 211
7  libc.so.6 0x00007faf77c7571b
8  libc.so.6 0x00007faf77c86e96
9  opt       0x00005620fe069d13 llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>::transferNodesFromList(llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>&, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) + 355
10 opt       0x00005620fe06c9d2 llvm::BasicBlock::splice(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::BasicBlock*, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) + 114
11 opt       0x00005620fe06c7a8 llvm::BasicBlock::splitBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::Twine const&) + 376
12 opt       0x00005620fd7b549d
13 opt       0x00005620fd7b5386 llvm::SplitBlock(llvm::BasicBlock*, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*, llvm::Twine const&) + 22
14 opt       0x00005620fc5f971b
15 opt       0x00005620fc5e899a llvm::ControlHeightReductionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) + 9114
16 opt       0x00005620fc2079ad
17 opt       0x00005620fe1db9d7 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) + 407
18 opt       0x00005620fa9fdbfd
19 opt       0x00005620fe1df49e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 478
20 opt       0x00005620fa53047d
21 opt       0x00005620fe1daab7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 407
22 opt       0x00005620fa52803f llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) + 11455
23 opt       0x00005620fa51df53 optMain + 7267
24 libc.so.6 0x00007faf77c76d90
25 libc.so.6 0x00007faf77c76e40 __libc_start_main + 128
26 opt       0x00005620fa51bc15 _start + 37
[1]    3840196 IOT instruction (core dumped)  opt -passes='require<profile-summary>,function(chr)' small.ll -o /dev/null
```

The crash was originally triggered by [small.c](https://github.com/user-attachments/files/25982745/small.c) compiling with [profile.txt](https://github.com/user-attachments/files/25982788/profile.txt) at -O3 with PGO:

```
$ llvm-profdata merge --text test.profdata -o profile.txt 
$ clang-trunk -O3 -fprofile-instr-use=test.profdata -I"$CSMITH_INCLUDE" small.c 
```




