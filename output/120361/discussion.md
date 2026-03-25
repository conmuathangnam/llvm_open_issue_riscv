# [InstCombine] `samesign` should be dropped when folding logical and of icmps

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/120361
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2024-12-18T12:40:35Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/YSW_NC
```
define i1 @src(i8 %0, i8 %1) {
start:
  %2 = icmp samesign eq i8 %0, -1
  %3 = icmp ne i8 %0, 0
  %4 = icmp ugt i8 %0, %0
  %result = select i1 %3, i1 %2, i1 %4
  ret i1 %result
}

define i1 @tgt(i8 %0, i8 %1) {
start:
  %2 = icmp samesign eq i8 %0, -1
  ret i1 %2
}
```
```

----------------------------------------
define i1 @src(i8 %#0, i8 %#1) {
start:
  %#2 = icmp samesign eq i8 %#0, 255
  %#3 = icmp ne i8 %#0, 0
  %#4 = icmp ugt i8 %#0, %#0
  %result = select i1 %#3, i1 %#2, i1 %#4
  ret i1 %result
}
=>
define i1 @tgt(i8 %#0, i8 %#1) {
start:
  %#2 = icmp samesign eq i8 %#0, 255
  ret i1 %#2
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
i8 %#0 = #x00 (0)
i8 %#1 = poison

Source:
i1 %#2 = poison
i1 %#3 = #x0 (0)
i1 %#4 = #x0 (0)
i1 %result = #x0 (0)

Target:
i1 %#2 = poison
Source value: #x0 (0)
Target value: poison
```
Reported by https://github.com/dtcxzyw/llvm-mutation-based-fuzz-service/issues/14 while reviewing https://github.com/llvm/llvm-project/pull/120177.




