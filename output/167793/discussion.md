# 21.1.x clang-cl crash [Exception Code: 0xC0000005]

**Author:** Rinn
**URL:** https://github.com/llvm/llvm-project/issues/167793
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2025-12-05T11:55:09Z

## Body

Making a new ticket for this crash as it is different than the previous one I reported.

> I also got another crash with the same exception code, unsure if it's related or not as the stack dump looks different. If not I can make a new issue, but same as before this crash does not occur with 20.1.8 but does with 21.1.0 and later.
> 
> [FieldOfViewNetObjectPrioritizer-8aa4e8.zip](https://github.com/user-attachments/files/23511878/FieldOfViewNetObjectPrioritizer-8aa4e8.zip)
> 
> ```
> PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
> Stack dump:
> 0. Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe @../Intermediate/Build/XB1/x64/UnrealGameNU/Development/IrisCore/FieldOfViewNetObjectPrioritizer.cpp.obj.rsp
> 1. <eof> parser at end of file
> 2. Code generation
> 3. Running pass 'Function Pass Manager' on module 'Runtime/Net/Iris/Private/Iris/ReplicationSystem/Prioritization/FieldOfViewNetObjectPrioritizer.cpp'.
> 4. Running pass 'X86 DAG->DAG Instruction Selection' on function '@"?PrioritizeBatch@UFieldOfViewNetObjectPrioritizer@@IEAAXAEAUFBatchParams@1@@Z"'
> Exception Code: 0xC0000005
>  #0 0x00007ff6f64211fe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x15311fe)
>  #1 0x00007ff6f6420db8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1530db8)
>  #2 0x00007ff6f689389c (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19a389c)
>  #3 0x00007ff6f688e332 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x199e332)
>  #4 0x00007ff6f69c67ef (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad67ef)
>  #5 0x00007ff6f69c2dcc (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad2dcc)
>  #6 0x00007ff6f693a8a9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a4a8a9)
>  #7 0x00007ff6f51c1de9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2d1de9)
>  #8 0x00007ff6f54288d4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x5388d4)
>  #9 0x00007ff6f68a536e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19b536e)
> #10 0x00007ff6f68c61d0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d61d0)
> #11 0x00007ff6f5186e0a (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x296e0a)
> #12 0x00007ff6f51868de (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2968de)
> #13 0x00007ff6f518041e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x29041e)
> #14 0x00007ff6f68c0dda (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d0dda)
> #15 0x00007ff6f6e29e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
> #16 0x00007ff6f551a493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
> #17 0x00007ff6f551a0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
> #18 0x00007ff6f5516d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
> #19 0x00007ff6f55138f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
> #20 0x00007ff6f551218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
> #21 0x00007ff6f5dfee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
> #22 0x00007ff6f56a2685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
> #23 0x00007ff6f56a2478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
> #24 0x00007ff6f53c7825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
> #25 0x00007ff6f53c716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
> #26 0x00007ff6f53c6d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
> #27 0x00007ff6f53bda86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
> #28 0x00007ff6f53bb023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
> #29 0x00007ff6f6edd0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
> #30 0x00007ffc7f4ee8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
> #31 0x00007ffc8000c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
> clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
> clang version 21.1.5
> Target: x86_64-pc-windows-msvc
> Thread model: posix
> InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
> clang-cl: note: diagnostic msg:
> ********************
> PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
> Preprocessed source(s) and associated run script(s) are located at:
> clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.cpp
> clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.sh
> clang-cl: note: diagnostic msg:
> ********************
> ``` 

 _Originally posted by @Rinn in [#167433](https://github.com/llvm/llvm-project/issues/167433#issuecomment-3524403361)_

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Joe Kirchoff (Rinn)

<details>
Making a new ticket for this crash as it is different than the previous one I reported.

&gt; I also got another crash with the same exception code, unsure if it's related or not as the stack dump looks different. If not I can make a new issue, but same as before this crash does not occur with 20.1.8 but does with 21.1.0 and later.
&gt; 
&gt; [FieldOfViewNetObjectPrioritizer-8aa4e8.zip](https://github.com/user-attachments/files/23511878/FieldOfViewNetObjectPrioritizer-8aa4e8.zip)
&gt; 
&gt; ```
&gt; PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
&gt; Stack dump:
&gt; 0. Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe @../Intermediate/Build/XB1/x64/UnrealGameNU/Development/IrisCore/FieldOfViewNetObjectPrioritizer.cpp.obj.rsp
&gt; 1. &lt;eof&gt; parser at end of file
&gt; 2. Code generation
&gt; 3. Running pass 'Function Pass Manager' on module 'Runtime/Net/Iris/Private/Iris/ReplicationSystem/Prioritization/FieldOfViewNetObjectPrioritizer.cpp'.
&gt; 4. Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@"?PrioritizeBatch@<!-- -->UFieldOfViewNetObjectPrioritizer@@<!-- -->IEAAXAEAUFBatchParams@<!-- -->1@@<!-- -->Z"'
&gt; Exception Code: 0xC0000005
&gt;  #<!-- -->0 0x00007ff6f64211fe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x15311fe)
&gt;  #<!-- -->1 0x00007ff6f6420db8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1530db8)
&gt;  #<!-- -->2 0x00007ff6f689389c (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19a389c)
&gt;  #<!-- -->3 0x00007ff6f688e332 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x199e332)
&gt;  #<!-- -->4 0x00007ff6f69c67ef (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad67ef)
&gt;  #<!-- -->5 0x00007ff6f69c2dcc (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad2dcc)
&gt;  #<!-- -->6 0x00007ff6f693a8a9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a4a8a9)
&gt;  #<!-- -->7 0x00007ff6f51c1de9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2d1de9)
&gt;  #<!-- -->8 0x00007ff6f54288d4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x5388d4)
&gt;  #<!-- -->9 0x00007ff6f68a536e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19b536e)
&gt; #<!-- -->10 0x00007ff6f68c61d0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d61d0)
&gt; #<!-- -->11 0x00007ff6f5186e0a (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x296e0a)
&gt; #<!-- -->12 0x00007ff6f51868de (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2968de)
&gt; #<!-- -->13 0x00007ff6f518041e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x29041e)
&gt; #<!-- -->14 0x00007ff6f68c0dda (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d0dda)
&gt; #<!-- -->15 0x00007ff6f6e29e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
&gt; #<!-- -->16 0x00007ff6f551a493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
&gt; #<!-- -->17 0x00007ff6f551a0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
&gt; #<!-- -->18 0x00007ff6f5516d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
&gt; #<!-- -->19 0x00007ff6f55138f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
&gt; #<!-- -->20 0x00007ff6f551218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
&gt; #<!-- -->21 0x00007ff6f5dfee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
&gt; #<!-- -->22 0x00007ff6f56a2685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
&gt; #<!-- -->23 0x00007ff6f56a2478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
&gt; #<!-- -->24 0x00007ff6f53c7825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
&gt; #<!-- -->25 0x00007ff6f53c716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
&gt; #<!-- -->26 0x00007ff6f53c6d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
&gt; #<!-- -->27 0x00007ff6f53bda86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
&gt; #<!-- -->28 0x00007ff6f53bb023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
&gt; #<!-- -->29 0x00007ff6f6edd0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
&gt; #<!-- -->30 0x00007ffc7f4ee8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
&gt; #<!-- -->31 0x00007ffc8000c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
&gt; clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
&gt; clang version 21.1.5
&gt; Target: x86_64-pc-windows-msvc
&gt; Thread model: posix
&gt; InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
&gt; clang-cl: note: diagnostic msg:
&gt; ********************
&gt; PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
&gt; Preprocessed source(s) and associated run script(s) are located at:
&gt; clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.cpp
&gt; clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.sh
&gt; clang-cl: note: diagnostic msg:
&gt; ********************
&gt; ``` 

 _Originally posted by @<!-- -->Rinn in [#<!-- -->167433](https://github.com/llvm/llvm-project/issues/167433#issuecomment-3524403361)_
</details>


---

### Comment 2 - RKSimon

Partially reduced:
```ll
define void @PR167793() #0 {
  br label %1

1:                                                ; preds = %40, %.critedge
  %2 = icmp ult i32 0, poison
  br i1 %2, label %3, label %.critedge2

.critedge2:                                       ; preds = %1
  ret void

3:                                                ; preds = %1
  br label %4

4:                                                ; preds = %.critedge3, %3
  %5 = phi <4 x double> [ poison, %3 ], [ %39, %.critedge3 ]
  %6 = icmp slt i32 poison, poison
  br i1 %6, label %7, label %40

7:                                                ; preds = %4
  br i1 poison, label %.critedge3, label %8

8:                                                ; preds = %7
  br label %.critedge3

.critedge3:                                       ; preds = %8, %7
  %9 = load <4 x double>, ptr poison, align 32
  %10 = fsub <4 x double> %9, zeroinitializer
  %11 = load <4 x double>, ptr null, align 32
  %12 = fsub <4 x double> %11, zeroinitializer
  %13 = load <4 x double>, ptr null, align 32
  %14 = fsub <4 x double> %13, zeroinitializer
  %15 = fmul <4 x double> %10, %10
  %16 = shufflevector <4 x double> %15, <4 x double> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  %17 = fadd <4 x double> %15, %16
  %18 = fmul <4 x double> %12, %12
  %19 = shufflevector <4 x double> %18, <4 x double> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  %20 = fadd <4 x double> %18, %19
  %21 = fmul <4 x double> %14, %14
  %22 = fadd <4 x double> %21, zeroinitializer
  %23 = shufflevector <4 x double> %17, <4 x double> poison, <2 x i32> <i32 poison, i32 3>
  %24 = shufflevector <4 x double> %17, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %25 = fadd <2 x double> %23, %24
  %26 = shufflevector <4 x double> %20, <4 x double> poison, <2 x i32> <i32 poison, i32 3>
  %27 = shufflevector <4 x double> %20, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %28 = fadd <2 x double> %26, %27
  %29 = shufflevector <4 x double> %22, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %30 = fadd <2 x double> zeroinitializer, %29
  %31 = shufflevector <2 x double> %28, <2 x double> %30, <4 x i32> <i32 poison, i32 poison, i32 1, i32 3>
  %32 = shufflevector <2 x double> %25, <2 x double> poison, <4 x i32> <i32 1, i32 3, i32 poison, i32 poison>
  %33 = shufflevector <4 x double> %32, <4 x double> %31, <4 x i32> <i32 0, i32 1, i32 6, i32 7>
  %34 = fcmp ole <4 x double> %33, zeroinitializer
  %35 = load <4 x double>, ptr poison, align 32
  %36 = select <4 x i1> %34, <4 x double> %35, <4 x double> zeroinitializer
  %37 = call noundef <4 x double> @llvm.x86.avx.max.pd.256(<4 x double> %5, <4 x double> poison)
  %38 = call noundef <4 x double> @llvm.x86.avx.max.pd.256(<4 x double> poison, <4 x double> %36)
  %39 = call noundef <4 x double> @llvm.x86.avx.max.pd.256(<4 x double> %37, <4 x double> %38)
  br label %4

40:                                               ; preds = %4
  %41 = call noundef <4 x float> @llvm.x86.avx.cvt.pd2.ps.256(<4 x double> %5)
  br label %1
}

attributes #0 = { "target-cpu"="btver2" }
```

Still occurs on trunk

---

### Comment 3 - RKSimon

```ll
define <4 x double> @PR167793(<4 x double> %a0, <4 x double> %a1) #0 {
entry:
  %i5 = shufflevector <4 x double> %a0, <4 x double> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  %i6 = fadd <4 x double> %a0, %i5
  %i8 = shufflevector <4 x double> %a1, <4 x double> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  %i9 = fadd <4 x double> %a1, %i8
  %i10 = shufflevector <4 x double> %i6, <4 x double> poison, <2 x i32> <i32 poison, i32 3>
  %i11 = shufflevector <4 x double> %i6, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %i12 = fadd <2 x double> %i10, %i11
  %i13 = shufflevector <4 x double> %i9, <4 x double> poison, <2 x i32> <i32 poison, i32 3>
  %i14 = shufflevector <4 x double> %i9, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %i15 = fadd <2 x double> %i13, %i14
  %i16 = shufflevector <4 x double> zeroinitializer, <4 x double> poison, <2 x i32> <i32 poison, i32 1>
  %i18 = shufflevector <2 x double> %i15, <2 x double> %i16, <4 x i32> <i32 poison, i32 poison, i32 1, i32 3>
  %i19 = shufflevector <2 x double> %i12, <2 x double> poison, <4 x i32> <i32 1, i32 3, i32 poison, i32 poison>
  %i20 = shufflevector <4 x double> %i19, <4 x double> %i18, <4 x i32> <i32 0, i32 1, i32 6, i32 7>
  ret <4 x double> %i20
}

attributes #0 = { "target-cpu"="btver2" }
```

---

### Comment 4 - RKSimon

I have a fix - canonicalizeShuffleMaskWithHorizOp canonicalization is affecting shuffle lowering - I'll get this sorted out tomorrow.

---

