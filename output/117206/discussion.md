# [X86AsmPrinter] Assertion `!NodePtr->isKnownSentinel()' failed

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/117206
**Status:** Closed
**Labels:** llvm:codegen, debuginfo, crash
**Closed Date:** 2025-02-10T11:22:35Z

## Body

```
$ cat /tmp/a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

; Function Attrs: noinline optnone
define void @f() #0 !dbg !3 {
entry:
  %0 = call ptr @llvm.returnaddress(i32 0)
  br label %do.body

do.body:                                          ; preds = %entry
  unreachable
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress(i32 immarg) #1

attributes #0 = { noinline optnone "frame-pointer"="all" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "a.c", directory: "/tmp", checksumkind: CSK_MD5, checksum: "e84fa2105300221d1aebb85a89a53960")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = distinct !DISubprogram(name: "f", scope: !1, file: !1, line: 37, type: !4, scopeLine: 37, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !0)
!4 = !DISubroutineType(types: !5)
!5 = !{}

$ llc -o /dev/null /tmp/a.ll
llc: ../../llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineBasicBlock, true, false, void, false, void>, false, true>::operator*() const [OptionsT = llvm::ilist_detail::node_options<llvm::MachineBasicBlock, true, false, void, false, void>, IsReverse = false, IsConst = true]: Assertion `!NodePtr->isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: build/rel/bin/llc -o /dev/null /tmp/b.ll
1.      Running pass 'Function Pass Manager' on module '/tmp/b.ll'.
2.      Running pass 'X86 Assembly Printer' on function '@"_ZZNK3cel11StructValue14GetRuntimeTypeEvENK3$_0clINSt6__tsan9monostateEEENS_10StructTypeERKT_"'
 #0 0x000056416e8a76a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x000056416e8a52ee llvm::sys::RunSignalHandlers() llvm-project/build/rel/../../llvm/lib/Support/Signals.cpp:106:18
 #2 0x000056416e8a7d38 SignalHandler(int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:413:1
 #3 0x00007fc90b82c1a0 (/lib/x86_64-linux-gnu/libc.so.6+0x3d1a0)
 #4 0x00007fc90b87a0ec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007fc90b82c102 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x00007fc90b8154f2 abort ./stdlib/abort.c:81:7
 #7 0x00007fc90b815415 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #8 0x00007fc90b824d32 (/lib/x86_64-linux-gnu/libc.so.6+0x35d32)
 #9 0x000056416df3bcd2 findPrologueEndLoc llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:0:0
#10 0x000056416df3bcd2 llvm::DwarfDebug::emitInitialLocDirective(llvm::MachineFunction const&, unsigned int) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2284:53
#11 0x000056416df3d70b llvm::DwarfDebug::beginFunctionImpl(llvm::MachineFunction const*) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2472:16
#12 0x000056416df1493e llvm::AsmPrinter::emitFunctionHeader() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1057:37
#13 0x000056416df1714d llvm::AsmPrinter::emitFunctionBody() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1768:3
#14 0x000056416f98424b llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&) llvm-project/build/rel/../../llvm/lib/Target/X86/X86AsmPrinter.cpp:91:3
#15 0x000056416dba4137 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) llvm-project/build/rel/../../llvm/lib/CodeGen/MachineFunctionPass.cpp:0:13
#16 0x000056416e497e4a llvm::FPPassManager::runOnFunction(llvm::Function&) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:0:27
#17 0x000056416e49f7e2 llvm::FPPassManager::runOnModule(llvm::Module&) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1452:13
#18 0x000056416e498946 runOnModule llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1521:27
#19 0x000056416e498946 llvm::legacy::PassManagerImpl::run(llvm::Module&) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:539:44
#20 0x000056416d6ddc7e compileModule llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:753:17
#21 0x000056416d6ddc7e main llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:411:22
#22 0x00007fc90b816b8a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007fc90b816c45 call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007fc90b816c45 __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x000056416d6d5da1 _start (build/rel/bin/llc+0x17cdda1)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Arthur Eubanks (aeubanks)

<details>
```
$ cat /tmp/a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

; Function Attrs: noinline optnone
define void @<!-- -->f() #<!-- -->0 !dbg !3 {
entry:
  %0 = call ptr @<!-- -->llvm.returnaddress(i32 0)
  br label %do.body

do.body:                                          ; preds = %entry
  unreachable
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @<!-- -->llvm.returnaddress(i32 immarg) #<!-- -->1

attributes #<!-- -->0 = { noinline optnone "frame-pointer"="all" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "a.c", directory: "/tmp", checksumkind: CSK_MD5, checksum: "e84fa2105300221d1aebb85a89a53960")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = distinct !DISubprogram(name: "f", scope: !1, file: !1, line: 37, type: !4, scopeLine: 37, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !0)
!4 = !DISubroutineType(types: !5)
!5 = !{}

$ llc -o /dev/null /tmp/a.ll
llc: ../../llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, true&gt;::operator*() const [OptionsT = llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, IsReverse = false, IsConst = true]: Assertion `!NodePtr-&gt;isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: build/rel/bin/llc -o /dev/null /tmp/b.ll
1.      Running pass 'Function Pass Manager' on module '/tmp/b.ll'.
2.      Running pass 'X86 Assembly Printer' on function '@"_ZZNK3cel11StructValue14GetRuntimeTypeEvENK3$_0clINSt6__tsan9monostateEEENS_10StructTypeERKT_"'
 #<!-- -->0 0x000056416e8a76a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x000056416e8a52ee llvm::sys::RunSignalHandlers() llvm-project/build/rel/../../llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x000056416e8a7d38 SignalHandler(int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x00007fc90b82c1a0 (/lib/x86_64-linux-gnu/libc.so.6+0x3d1a0)
 #<!-- -->4 0x00007fc90b87a0ec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007fc90b82c102 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x00007fc90b8154f2 abort ./stdlib/abort.c:81:7
 #<!-- -->7 0x00007fc90b815415 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #<!-- -->8 0x00007fc90b824d32 (/lib/x86_64-linux-gnu/libc.so.6+0x35d32)
 #<!-- -->9 0x000056416df3bcd2 findPrologueEndLoc llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:0:0
#<!-- -->10 0x000056416df3bcd2 llvm::DwarfDebug::emitInitialLocDirective(llvm::MachineFunction const&amp;, unsigned int) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2284:53
#<!-- -->11 0x000056416df3d70b llvm::DwarfDebug::beginFunctionImpl(llvm::MachineFunction const*) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2472:16
#<!-- -->12 0x000056416df1493e llvm::AsmPrinter::emitFunctionHeader() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1057:37
#<!-- -->13 0x000056416df1714d llvm::AsmPrinter::emitFunctionBody() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1768:3
#<!-- -->14 0x000056416f98424b llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) llvm-project/build/rel/../../llvm/lib/Target/X86/X86AsmPrinter.cpp:91:3
#<!-- -->15 0x000056416dba4137 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) llvm-project/build/rel/../../llvm/lib/CodeGen/MachineFunctionPass.cpp:0:13
#<!-- -->16 0x000056416e497e4a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:0:27
#<!-- -->17 0x000056416e49f7e2 llvm::FPPassManager::runOnModule(llvm::Module&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1452:13
#<!-- -->18 0x000056416e498946 runOnModule llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1521:27
#<!-- -->19 0x000056416e498946 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:539:44
#<!-- -->20 0x000056416d6ddc7e compileModule llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:753:17
#<!-- -->21 0x000056416d6ddc7e main llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:411:22
#<!-- -->22 0x00007fc90b816b8a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007fc90b816c45 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007fc90b816c45 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x000056416d6d5da1 _start (build/rel/bin/llc+0x17cdda1)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Arthur Eubanks (aeubanks)

<details>
```
$ cat /tmp/a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

; Function Attrs: noinline optnone
define void @<!-- -->f() #<!-- -->0 !dbg !3 {
entry:
  %0 = call ptr @<!-- -->llvm.returnaddress(i32 0)
  br label %do.body

do.body:                                          ; preds = %entry
  unreachable
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @<!-- -->llvm.returnaddress(i32 immarg) #<!-- -->1

attributes #<!-- -->0 = { noinline optnone "frame-pointer"="all" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "a.c", directory: "/tmp", checksumkind: CSK_MD5, checksum: "e84fa2105300221d1aebb85a89a53960")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = distinct !DISubprogram(name: "f", scope: !1, file: !1, line: 37, type: !4, scopeLine: 37, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !0)
!4 = !DISubroutineType(types: !5)
!5 = !{}

$ llc -o /dev/null /tmp/a.ll
llc: ../../llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, true&gt;::operator*() const [OptionsT = llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, IsReverse = false, IsConst = true]: Assertion `!NodePtr-&gt;isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: build/rel/bin/llc -o /dev/null /tmp/b.ll
1.      Running pass 'Function Pass Manager' on module '/tmp/b.ll'.
2.      Running pass 'X86 Assembly Printer' on function '@"_ZZNK3cel11StructValue14GetRuntimeTypeEvENK3$_0clINSt6__tsan9monostateEEENS_10StructTypeERKT_"'
 #<!-- -->0 0x000056416e8a76a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x000056416e8a52ee llvm::sys::RunSignalHandlers() llvm-project/build/rel/../../llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x000056416e8a7d38 SignalHandler(int) llvm-project/build/rel/../../llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x00007fc90b82c1a0 (/lib/x86_64-linux-gnu/libc.so.6+0x3d1a0)
 #<!-- -->4 0x00007fc90b87a0ec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007fc90b82c102 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x00007fc90b8154f2 abort ./stdlib/abort.c:81:7
 #<!-- -->7 0x00007fc90b815415 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #<!-- -->8 0x00007fc90b824d32 (/lib/x86_64-linux-gnu/libc.so.6+0x35d32)
 #<!-- -->9 0x000056416df3bcd2 findPrologueEndLoc llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:0:0
#<!-- -->10 0x000056416df3bcd2 llvm::DwarfDebug::emitInitialLocDirective(llvm::MachineFunction const&amp;, unsigned int) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2284:53
#<!-- -->11 0x000056416df3d70b llvm::DwarfDebug::beginFunctionImpl(llvm::MachineFunction const*) llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/DwarfDebug.cpp:2472:16
#<!-- -->12 0x000056416df1493e llvm::AsmPrinter::emitFunctionHeader() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1057:37
#<!-- -->13 0x000056416df1714d llvm::AsmPrinter::emitFunctionBody() llvm-project/build/rel/../../llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1768:3
#<!-- -->14 0x000056416f98424b llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) llvm-project/build/rel/../../llvm/lib/Target/X86/X86AsmPrinter.cpp:91:3
#<!-- -->15 0x000056416dba4137 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) llvm-project/build/rel/../../llvm/lib/CodeGen/MachineFunctionPass.cpp:0:13
#<!-- -->16 0x000056416e497e4a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:0:27
#<!-- -->17 0x000056416e49f7e2 llvm::FPPassManager::runOnModule(llvm::Module&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1452:13
#<!-- -->18 0x000056416e498946 runOnModule llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:1521:27
#<!-- -->19 0x000056416e498946 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) llvm-project/build/rel/../../llvm/lib/IR/LegacyPassManager.cpp:539:44
#<!-- -->20 0x000056416d6ddc7e compileModule llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:753:17
#<!-- -->21 0x000056416d6ddc7e main llvm-project/build/rel/../../llvm/tools/llc/llc.cpp:411:22
#<!-- -->22 0x00007fc90b816b8a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007fc90b816c45 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007fc90b816c45 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x000056416d6d5da1 _start (build/rel/bin/llc+0x17cdda1)
```
</details>


---

### Comment 3 - jmorse

Thanks for the report; patch in #117320 

---

### Comment 4 - jmorse

This was fixed by 624e52b1e310c349e21cc0b4f67452b0fa9d1f96

---

