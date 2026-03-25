# Segmentation fault when trying to disassemble VOP3 with literal constant on gfx900

**Author:** misos1
**URL:** https://github.com/llvm/llvm-project/issues/170318
**Status:** Open
**Labels:** llvm:mc, crash

## Body

`Ubuntu clang version 22.0.0 (++20251025081749+57828a6d5de0-1~exp1~20251025201905.1249)`

```
echo 0 0 3 209 0 254 1 0 | llvm-mc -disassemble -arch amdgcn -mcpu=gfx900
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llvm-mc -disassemble -arch amdgcn -mcpu=gfx900
 #0 0x00007714fed2da0f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2da0f)
 #1 0x00007714fed2b287 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2b287)
 #2 0x00007714fed2e7da (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2e7da)
 #3 0x00007714f9a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007714fecd87d1 llvm::Twine::printOneChild(llvm::raw_ostream&, llvm::Twine::Child, llvm::Twine::NodeKind) const (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4ad87d1)
 #5 0x00007714fecd8572 llvm::Twine::print(llvm::raw_ostream&) const (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4ad8572)
 #6 0x00007715015f35c6 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f35c6)
 #7 0x00007715015f3497 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f3497)
 #8 0x00007715015f3f33 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f3f33)
 #9 0x00006269426cf587 (/usr/lib/llvm-22/bin/llvm-mc+0x16587)
#10 0x00006269426c8910 (/usr/lib/llvm-22/bin/llvm-mc+0xf910)
#11 0x00007714f9a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#12 0x00007714f9a2a28b call_init ./csu/../csu/libc-start.c:128:20
#13 0x00007714f9a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#14 0x00006269426c41f5 (/usr/lib/llvm-22/bin/llvm-mc+0xb1f5)
Segmentation fault (core dumped)
```

There is another case this time for gfx1100 (VOP3 with literal constant and DPP):
```
echo 0 0 3 213 250 254 1 0 0 0 0 0 | llvm-mc -disassemble -arch amdgcn -mcpu=gfx1100
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: None (misos1)

<details>
`Ubuntu clang version 22.0.0 (++20251025081749+57828a6d5de0-1~exp1~20251025201905.1249)`

```
echo 0 0 3 209 0 254 1 0 | llvm-mc -disassemble -arch amdgcn -mcpu=gfx900
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llvm-mc -disassemble -arch amdgcn -mcpu=gfx900
 #<!-- -->0 0x00007714fed2da0f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2da0f)
 #<!-- -->1 0x00007714fed2b287 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2b287)
 #<!-- -->2 0x00007714fed2e7da (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4b2e7da)
 #<!-- -->3 0x00007714f9a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007714fecd87d1 llvm::Twine::printOneChild(llvm::raw_ostream&amp;, llvm::Twine::Child, llvm::Twine::NodeKind) const (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4ad87d1)
 #<!-- -->5 0x00007714fecd8572 llvm::Twine::print(llvm::raw_ostream&amp;) const (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4ad8572)
 #<!-- -->6 0x00007715015f35c6 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f35c6)
 #<!-- -->7 0x00007715015f3497 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f3497)
 #<!-- -->8 0x00007715015f3f33 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x73f3f33)
 #<!-- -->9 0x00006269426cf587 (/usr/lib/llvm-22/bin/llvm-mc+0x16587)
#<!-- -->10 0x00006269426c8910 (/usr/lib/llvm-22/bin/llvm-mc+0xf910)
#<!-- -->11 0x00007714f9a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->12 0x00007714f9a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->13 0x00007714f9a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->14 0x00006269426c41f5 (/usr/lib/llvm-22/bin/llvm-mc+0xb1f5)
Segmentation fault (core dumped)
```

There is another case this time for gfx1100 (VOP3 with literal constant and DPP):
```
echo 0 0 3 213 250 254 1 0 0 0 0 0 | llvm-mc -disassemble -arch amdgcn -mcpu=gfx1100
```

</details>


---

