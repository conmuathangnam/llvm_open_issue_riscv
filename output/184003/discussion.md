# [DebugInfo] Segfault in `DwarfCompileUnit` with empty struct types

**Author:** MacDue
**URL:** https://github.com/llvm/llvm-project/issues/184003
**Status:** Closed
**Labels:** llvm:codegen, debuginfo, crash-on-valid
**Closed Date:** 2026-03-19T09:57:06Z

## Body

```llvm
target triple = "x86_64-apple-darwin"

define void @test_empty_struct_debug() !dbg !4 {
entry:
  %tmp = alloca { { i1, {} }, ptr, { { {} }, { {} } }, i64 }, align 8
    #dbg_value({ { {} }, { {} } } zeroinitializer, !5, !DIExpression(), !6)
    #dbg_value(i64 2, !7, !DIExpression(), !6)
  %0 = insertvalue { { i1, {} }, ptr, { { {} }, { {} } }, i64 } { { i1, {} } zeroinitializer, ptr null, { { {} }, { {} } } zeroinitializer, i64 2 }, ptr null, 1, !dbg !6
  %1 = insertvalue { { i1, {} }, ptr, { { {} }, { {} } }, i64 } %0, { i1, {} } zeroinitializer, 0, !dbg !8
  store { { i1, {} }, ptr, { { {} }, { {} } }, i64 } %1, ptr %tmp, align 8
  ret void
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly)
!1 = !DIFile(filename: "test_selectiondag.cpp", directory: "/home/AnonTokyo/documents/llvm-project/temp")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_empty_struct_debug", scope: !1, file: !1, line: 1, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DILocalVariable(name: "v1", scope: !4, file: !1, line: 2)
!6 = !DILocation(line: 2, column: 1, scope: !4)
!7 = !DILocalVariable(name: "v2", scope: !4, file: !1, line: 3)
!8 = !DILocation(line: 3, column: 1, scope: !4)
```

<details>
<summary>Stack trace</summary>
<pre>
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=x86_64-apple-darwin <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'X86 Assembly Printer' on function '@test_empty_struct_debug'
 #0 0x0000000003e262e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3e262e8)
 #1 0x0000000003e23424 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000726367642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000002b03fb5 llvm::DwarfCompileUnit::constructSubprogramScopeDIE(llvm::DISubprogram const*, llvm::Function const&, llvm::LexicalScope*, llvm::MCSymbol*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2b03fb5)
 #4 0x0000000002b2ef4a llvm::DwarfDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2b2ef4a)
 #5 0x0000000002aebd6b llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2aebd6b)
 #6 0x0000000002ae49a2 llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-trunk/bin/llc+0x2ae49a2)
 #7 0x0000000001ff4263 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x1ff4263)
 #8 0x0000000002d883dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2d883dd)
 #9 0x00000000032e9672 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32e9672)
#10 0x00000000032e9901 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32e9901)
#11 0x00000000032eb177 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32eb177)
#12 0x00000000008fd75f compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#13 0x00000000007cc14c main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cc14c)
#14 0x0000726367629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#15 0x0000726367629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#16 0x00000000008f3cc5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f3cc5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
</pre>
</details>

Compiler Explorer: https://godbolt.org/z/PKr6o5ex9

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Benjamin Maxwell (MacDue)

<details>
```llvm
define void @<!-- -->test_empty_struct_debug() !dbg !4 {
entry:
  %tmp = alloca { { i1, {} }, ptr, { { {} }, { {} } }, i64 }, align 8
    #dbg_value({ { {} }, { {} } } zeroinitializer, !5, !DIExpression(), !6)
    #dbg_value(i64 2, !7, !DIExpression(), !6)
  %0 = insertvalue { { i1, {} }, ptr, { { {} }, { {} } }, i64 } { { i1, {} } zeroinitializer, ptr null, { { {} }, { {} } } zeroinitializer, i64 2 }, ptr null, 1, !dbg !6
  %1 = insertvalue { { i1, {} }, ptr, { { {} }, { {} } }, i64 } %0, { i1, {} } zeroinitializer, 0, !dbg !8
  store { { i1, {} }, ptr, { { {} }, { {} } }, i64 } %1, ptr %tmp, align 8
  ret void
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly)
!1 = !DIFile(filename: "test_selectiondag.cpp", directory: "/home/AnonTokyo/documents/llvm-project/temp")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_empty_struct_debug", scope: !1, file: !1, line: 1, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DILocalVariable(name: "v1", scope: !4, file: !1, line: 2)
!6 = !DILocation(line: 2, column: 1, scope: !4)
!7 = !DILocalVariable(name: "v2", scope: !4, file: !1, line: 3)
!8 = !DILocation(line: 3, column: 1, scope: !4)
```

&lt;details&gt;
&lt;summary&gt;Stack trace&lt;/summary&gt;
&lt;pre&gt;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=x86_64-apple-darwin &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'X86 Assembly Printer' on function '@<!-- -->test_empty_struct_debug'
 #<!-- -->0 0x0000000003e262e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3e262e8)
 #<!-- -->1 0x0000000003e23424 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000726367642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000002b03fb5 llvm::DwarfCompileUnit::constructSubprogramScopeDIE(llvm::DISubprogram const*, llvm::Function const&amp;, llvm::LexicalScope*, llvm::MCSymbol*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2b03fb5)
 #<!-- -->4 0x0000000002b2ef4a llvm::DwarfDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2b2ef4a)
 #<!-- -->5 0x0000000002aebd6b llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2aebd6b)
 #<!-- -->6 0x0000000002ae49a2 llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-trunk/bin/llc+0x2ae49a2)
 #<!-- -->7 0x0000000001ff4263 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x1ff4263)
 #<!-- -->8 0x0000000002d883dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2d883dd)
 #<!-- -->9 0x00000000032e9672 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32e9672)
#<!-- -->10 0x00000000032e9901 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32e9901)
#<!-- -->11 0x00000000032eb177 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32eb177)
#<!-- -->12 0x00000000008fd75f compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->13 0x00000000007cc14c main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cc14c)
#<!-- -->14 0x0000726367629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->15 0x0000726367629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->16 0x00000000008f3cc5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f3cc5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
&lt;/pre&gt;
&lt;/details&gt;

Compiler Explorer: https://godbolt.org/z/T1aqGhW44
</details>


---

### Comment 2 - Michael137

Do you have more context on how this IR came to be? E.g., a C++ reproducer that compiles down to this IR?

---

### Comment 3 - MacDue

This test case came from @MetalOxideSemi. They may have more context.  

---

### Comment 4 - MetalOxideSemi

> This test case came from [@MetalOxideSemi](https://github.com/MetalOxideSemi). They may have more context.

Hi, @MacDue it came from [Codon](https://github.com/exaloop/codon) compiled IR, not C++. When I tried to compile it in release mode while still keeping the debug info, the crash occurs. Unfortunately, it has been a while since I encountered this issue, and I no longer have the original Codon user script in my environment, so I'm unable to reproduce it at the moment.

---

