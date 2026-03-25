# `llvm-clang-x86_64-expensive-checks-win` test fails in CI

**Author:** firewave
**URL:** https://github.com/llvm/llvm-project/issues/106352
**Status:** Closed
**Labels:** backend:X86, llvm:codegen, crash
**Closed Date:** 2025-08-11T15:53:46Z

## Body

https://lab.llvm.org/buildbot/#/builders/14/builds/1208

Now that #106290 has been resolved and the build is performed again it revealed a test failure:

```
******************** TEST 'LLVM :: CodeGen/X86/avx512f-large-stack.ll' FAILED ********************
Exit Code: 2
Command Output (stdout):
--
# RUN: at line 2
c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\llc.exe -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs < C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\test\CodeGen\X86\avx512f-large-stack.ll | c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\filecheck.exe C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\test\CodeGen\X86\avx512f-large-stack.ll --check-prefix=CHECK
# executed command: 'c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\llc.exe' -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs
# .---command stderr------------
# | Assertion failed: (!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!", file C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Target\X86\X86RegisterInfo.cpp, line 963
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
# | Stack dump:
# | 0.	Program arguments: c:\\a\\llvm-clang-x86_64-expensive-checks-win\\build\\bin\\llc.exe -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs
# | 1.	Running pass 'Function Pass Manager' on module '<stdin>'.
# | 2.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@f'
# | Exception Code: 0x80000003
# |  #0 0x00007ff7b9a39e2c HandleAbort C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Support\Windows\Signals.inc:429:0
# |  #1 0x00007ffdd9c490ed (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa90ed)
# |  #2 0x00007ffdd9c4ae49 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xaae49)
# |  #3 0x00007ffdd9c50c6f (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xb0c6f)
# |  #4 0x00007ffdd9c4eba1 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xaeba1)
# |  #5 0x00007ffdd9c518af (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xb18af)
# |  #6 0x00007ff7b5eb7fb8 llvm::X86RegisterInfo::eliminateFrameIndex(class llvm::MachineInstrBundleIterator<class llvm::MachineInstr, 0>, int, unsigned int, class llvm::RegScavenger *) const C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Target\X86\X86RegisterInfo.cpp:962:0
# |  #7 0x00007ff7b7d73bc0 `anonymous namespace'::PEI::replaceFrameIndices C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:1559:0
# |  #8 0x00007ff7b7d73734 `anonymous namespace'::PEI::replaceFrameIndices C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:1384:0
# |  #9 0x00007ff7b7d70d3e `anonymous namespace'::PEI::runOnMachineFunction C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:293:0
# | #10 0x00007ff7b78eb41d llvm::MachineFunctionPass::runOnFunction(class llvm::Function &) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\MachineFunctionPass.cpp:94:0
# | #11 0x00007ff7b81a3ab7 llvm::FPPassManager::runOnFunction(class llvm::Function &) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1408:0
# | #12 0x00007ff7b81a3fc0 llvm::FPPassManager::runOnModule(class llvm::Module &) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1454:0
# | #13 0x00007ff7b81a6e02 `anonymous namespace'::MPPassManager::runOnModule C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1523:0
# | #14 0x00007ff7b81a7d57 llvm::legacy::PassManagerImpl::run(class llvm::Module &) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:541:0
# | #15 0x00007ff7b819cf2c llvm::legacy::PassManager::run(class llvm::Module &) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1651:0
# | #16 0x00007ff7b3058ca5 compileModule C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\tools\llc\llc.cpp:742:0
# | #17 0x00007ff7b305a5e7 main C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\tools\llc\llc.cpp:409:0
# | #18 0x00007ff7bbaae5e9 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
# | #19 0x00007ff7bbaae4ce __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #20 0x00007ff7bbaae38e __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
# | #21 0x00007ff7bbaae67e mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
# | #22 0x00007ffdf2347374 (C:\WINDOWS\System32\KERNEL32.DLL+0x17374)
# | #23 0x00007ffdf275cc91 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x4cc91)
```

This appears to be related to the existing issues #69365, #31315 , #25051 and #10860.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Oliver Stöneberg (firewave)

<details>
https://lab.llvm.org/buildbot/#/builders/14/builds/1208

Now that #<!-- -->106290 has been resolved and the build is performed again it revealed a test failure:

```
******************** TEST 'LLVM :: CodeGen/X86/avx512f-large-stack.ll' FAILED ********************
Exit Code: 2
Command Output (stdout):
--
# RUN: at line 2
c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\llc.exe -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs &lt; C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\test\CodeGen\X86\avx512f-large-stack.ll | c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\filecheck.exe C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\test\CodeGen\X86\avx512f-large-stack.ll --check-prefix=CHECK
# executed command: 'c:\a\llvm-clang-x86_64-expensive-checks-win\build\bin\llc.exe' -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs
# .---command stderr------------
# | Assertion failed: (!Is64Bit || isInt&lt;32&gt;((long long)FIOffset + Imm)) &amp;&amp; "Requesting 64-bit offset in 32-bit immediate!", file C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Target\X86\X86RegisterInfo.cpp, line 963
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
# | Stack dump:
# | 0.	Program arguments: c:\\a\\llvm-clang-x86_64-expensive-checks-win\\build\\bin\\llc.exe -O0 -mtriple=x86_64 -mattr=+avx512f -verify-machineinstrs
# | 1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
# | 2.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->f'
# | Exception Code: 0x80000003
# |  #<!-- -->0 0x00007ff7b9a39e2c HandleAbort C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Support\Windows\Signals.inc:429:0
# |  #<!-- -->1 0x00007ffdd9c490ed (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa90ed)
# |  #<!-- -->2 0x00007ffdd9c4ae49 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xaae49)
# |  #<!-- -->3 0x00007ffdd9c50c6f (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xb0c6f)
# |  #<!-- -->4 0x00007ffdd9c4eba1 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xaeba1)
# |  #<!-- -->5 0x00007ffdd9c518af (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xb18af)
# |  #<!-- -->6 0x00007ff7b5eb7fb8 llvm::X86RegisterInfo::eliminateFrameIndex(class llvm::MachineInstrBundleIterator&lt;class llvm::MachineInstr, 0&gt;, int, unsigned int, class llvm::RegScavenger *) const C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\Target\X86\X86RegisterInfo.cpp:962:0
# |  #<!-- -->7 0x00007ff7b7d73bc0 `anonymous namespace'::PEI::replaceFrameIndices C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:1559:0
# |  #<!-- -->8 0x00007ff7b7d73734 `anonymous namespace'::PEI::replaceFrameIndices C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:1384:0
# |  #<!-- -->9 0x00007ff7b7d70d3e `anonymous namespace'::PEI::runOnMachineFunction C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\PrologEpilogInserter.cpp:293:0
# | #<!-- -->10 0x00007ff7b78eb41d llvm::MachineFunctionPass::runOnFunction(class llvm::Function &amp;) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\CodeGen\MachineFunctionPass.cpp:94:0
# | #<!-- -->11 0x00007ff7b81a3ab7 llvm::FPPassManager::runOnFunction(class llvm::Function &amp;) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1408:0
# | #<!-- -->12 0x00007ff7b81a3fc0 llvm::FPPassManager::runOnModule(class llvm::Module &amp;) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1454:0
# | #<!-- -->13 0x00007ff7b81a6e02 `anonymous namespace'::MPPassManager::runOnModule C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1523:0
# | #<!-- -->14 0x00007ff7b81a7d57 llvm::legacy::PassManagerImpl::run(class llvm::Module &amp;) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:541:0
# | #<!-- -->15 0x00007ff7b819cf2c llvm::legacy::PassManager::run(class llvm::Module &amp;) C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1651:0
# | #<!-- -->16 0x00007ff7b3058ca5 compileModule C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\tools\llc\llc.cpp:742:0
# | #<!-- -->17 0x00007ff7b305a5e7 main C:\a\llvm-clang-x86_64-expensive-checks-win\llvm-project\llvm\tools\llc\llc.cpp:409:0
# | #<!-- -->18 0x00007ff7bbaae5e9 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
# | #<!-- -->19 0x00007ff7bbaae4ce __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #<!-- -->20 0x00007ff7bbaae38e __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
# | #<!-- -->21 0x00007ff7bbaae67e mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
# | #<!-- -->22 0x00007ffdf2347374 (C:\WINDOWS\System32\KERNEL32.DLL+0x17374)
# | #<!-- -->23 0x00007ffdf275cc91 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x4cc91)
```

This appears to be related to the existing issues #<!-- -->69365, #<!-- -->31315 , #<!-- -->25051 and #<!-- -->10860.
</details>


---

### Comment 2 - firewave

This is working since https://lab.llvm.org/buildbot/#/builders/14/builds/1213. That build shows no changes though and the previous one was cancelled. So it was probably fixed with https://lab.llvm.org/buildbot/#/builders/14/builds/1212.

But the only change in that build does not look related though so I have no idea what fixed this.

---

