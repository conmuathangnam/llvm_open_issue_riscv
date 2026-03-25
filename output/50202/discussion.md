# Run dead loop after compiling github microsoft compiler-tests `sehframes.cpp` with clang-cl and lld

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/50202
**Status:** Open
**Labels:** c++, clang:codegen, bugzilla, miscompilation, extension:microsoft

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [50858](https://llvm.org/bz50858) |
| Version | trunk |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@zygoloid |

## Extended Description 
The test c file is in https://github.com/microsoft/compiler-tests/blob/master/seh/sehframes.cpp

No problem if use cl.exe
```
D:\llvm\compiler-tests\seh>cl.exe seh0020.c
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30037 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

seh0020.c
Microsoft (R) Incremental Linker Version 14.29.30037.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:seh0020.exe
seh0020.obj

D:\llvm\compiler-tests\seh>cl.exe sehframes.cpp
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30037 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

sehframes.cpp
Microsoft (R) Incremental Linker Version 14.29.30037.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:sehframes.exe
sehframes.obj

D:\llvm\compiler-tests\seh>sehframes.exe
Test pass 1 - no exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test pass 2 - exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test passed

Dead loop if use clang-cl.exe
$ /c/work/llvm/llvm-project-4/build/Release/bin/clang-cl.exe sehframes.cpp -fuse-ld=lld
$ ./sehframes.exe 
Test pass 1 - no exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test pass 2 - exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
Try
GSCookie
....
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [50858](https://llvm.org/bz50858) |
| Version | trunk |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
The test c file is in https://github.com/microsoft/compiler-tests/blob/master/seh/sehframes.cpp

No problem if use cl.exe
```
D:\llvm\compiler-tests\seh&gt;cl.exe seh0020.c
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30037 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

seh0020.c
Microsoft (R) Incremental Linker Version 14.29.30037.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:seh0020.exe
seh0020.obj

D:\llvm\compiler-tests\seh&gt;cl.exe sehframes.cpp
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30037 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

sehframes.cpp
Microsoft (R) Incremental Linker Version 14.29.30037.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:sehframes.exe
sehframes.obj

D:\llvm\compiler-tests\seh&gt;sehframes.exe
Test pass 1 - no exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test pass 2 - exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test passed

Dead loop if use clang-cl.exe
$ /c/work/llvm/llvm-project-4/build/Release/bin/clang-cl.exe sehframes.cpp -fuse-ld=lld
$ ./sehframes.exe 
Test pass 1 - no exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
GSCookieAndAlign
TryAndGSCookieAndAlign
Alloca
TryAndAlloca
GSCookieAndAlloca
TryAndGSCookieAndAlloca
AlignAndAlloca
TryAndAlignAndAlloca
GSCookieAndAlignAndAlloca
TryAndGSCookieAndAlignAndAlloca
BigLocals
TryAndBigLocals
GSCookieAndBigLocals
TryAndGSCookieAndBigLocals
AlignAndBigLocals
TryAndAlignAndBigLocals
GSCookieAndAlignAndBigLocals
TryAndGSCookieAndAlignAndBigLocals
AllocaAndBigLocals
TryAndAllocaAndBigLocals
GSCookieAndAllocaAndBigLocals
TryAndGSCookieAndAllocaAndBigLocals
AlignAndAllocaAndBigLocals
TryAndAlignAndAllocaAndBigLocals
GSCookieAndAlignAndAllocaAndBigLocals
TryAndGSCookieAndAlignAndAllocaAndBigLocals
EbpAdj
TryAndEbpAdj
GSCookieAndEbpAdj
TryAndGSCookieAndEbpAdj
AlignAndEbpAdj
TryAndAlignAndEbpAdj
GSCookieAndAlignAndEbpAdj
TryAndGSCookieAndAlignAndEbpAdj
AllocaAndEbpAdj
TryAndAllocaAndEbpAdj
GSCookieAndAllocaAndEbpAdj
TryAndGSCookieAndAllocaAndEbpAdj
AlignAndAllocaAndEbpAdj
TryAndAlignAndAllocaAndEbpAdj
GSCookieAndAlignAndAllocaAndEbpAdj
TryAndGSCookieAndAlignAndAllocaAndEbpAdj
Test pass 2 - exceptions
Simple
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
Try
GSCookie
TryAndGSCookie
Align
TryAndAlign
Try
GSCookie
....
```
</details>


---

### Comment 2 - frederick-vs-ja

@jaykrell Is any of your recently reported issues same as this?

---

