# LLVM ERROR: out of memory when llvm-cov show with the exe file about 800Mb

**Author:** yingh-zhang
**URL:** https://github.com/llvm/llvm-project/issues/180127
**Status:** Open
**Labels:** platform:windows, crash, tools:llvm-cov

## Body

```
llvm-cov show --summary-only UnitTest_ACTE.exe --num-threads=1 -j 1 -instr-profile="$mergedData" -format=html -ignore-filename-regex="Gtest_Test_.*\.cpp" -output-dir=html_cov
LLVM ERROR: out of memory
Allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: C:\\D\\Programs\\X86\\LLVM\\bin\\llvm-cov.exe show --summary-only UnitTest_ACTE.exe --num-threads=1 -j 1 -instr-profile=D:\\test\\data\\core1_test\\merged.profdata -format=html -ignore-filename-regex=Gtest_Test_.*\\.cpp -output-dir=html_cov
Exception Code: 0xC000001D
 #0 0x001e1818 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0xb1818)
 #1 0x76e377a8 (C:\Windows\System32\ucrtbase.dll+0x277a8)
 #2 0x001e42bf (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0xb42bf)
 #3 0x004430e9 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x3130e9)
 #4 0x001c505a (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x9505a)
 #5 0x001b9cc7 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x89cc7)
 #6 0x001ba51f (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x8a51f)
 #7 0x001baad6 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x8aad6)
 #8 0x001bae52 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x8ae52)
 #9 0x0013f741 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0xf741)
#10 0x0013ce52 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0xce52)
#11 0x00138343 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x8343)
#12 0x001375f1 (C:\D\Programs\X86\LLVM\bin\llvm-cov.exe+0x75f1)
#13 0x6f72505c
#14 0x6d617267
#15 0x38585c73
#16 0x4c4c5c36
#17 0x625c4d56
#18 0x6c5c6e69
#19 0x2d6d766c
#20 0x2e766f63
#21 0x20657865
#22 0x776f6873 (C:\Windows\System32\combase.dll+0x86873)
PS D:\test\data\UT_test>
```

## Comments

### Comment 1 - frederick-vs-ja

Could you provide a simple way to generating such an exe file?

---

### Comment 2 - yingh-zhang

> Could you provide a simple way to generating such an exe file?

Sorry, this exe is for our project's UT testing, we can not provide it to you directly.

But you can generate any UT.exe freely, only required that the size should be about more than 787 Mb.

---

