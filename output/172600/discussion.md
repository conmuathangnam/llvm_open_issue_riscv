# [LoongArch] Crash when running the LoongArch Optimize W Instructions pass

**Author:** SixWeining
**URL:** https://github.com/llvm/llvm-project/issues/172600
**Status:** Closed
**Labels:** crash, backend:loongarch
**Closed Date:** 2025-12-18T01:40:40Z

## Body

llvm commit: f734cebc396bfb0a3523d205071764f689432ab4
testcase: [reduced.ll.txt](https://github.com/user-attachments/files/24205982/reduced.ll.txt) (rename it to `reduced.ll`) (This is a test case simplified from ffmpeg's source code h264dsp_lasx.c)

```
$ _build/bin/llc reduced.ll 
llc: /home/user/llvm-project/llvm/include/llvm/CodeGen/Register.h:88: unsigned int llvm::Register::virtRegIndex() const: Assertion `isVirtual() && "Not a virtual register"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/user/llvm-project/_build/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'LoongArch Optimize W Instructions' on function '@ff_h264_v_lpf_luma_8_lasx'
^C
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: Lu Weining (SixWeining)

<details>
llvm commit: f734cebc396bfb0a3523d205071764f689432ab4
testcase: [reduced.ll.txt](https://github.com/user-attachments/files/24205982/reduced.ll.txt) (rename it to `reduced.ll`) (This is a test case simplified from ffmpeg's source code h264dsp_lasx.c)

```
$ _build/bin/llc reduced.ll 
llc: /home/user/llvm-project/llvm/include/llvm/CodeGen/Register.h:88: unsigned int llvm::Register::virtRegIndex() const: Assertion `isVirtual() &amp;&amp; "Not a virtual register"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/user/llvm-project/_build/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'LoongArch Optimize W Instructions' on function '@<!-- -->ff_h264_v_lpf_luma_8_lasx'
^C
```
</details>


---

