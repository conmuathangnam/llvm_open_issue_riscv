# When emitting `DW_AT_discr_value`, discriminants that don't fit in a 64-bit integer are truncated or cause LLVM assertions

**Author:** beetrees
**URL:** https://github.com/llvm/llvm-project/issues/119655
**Status:** Closed
**Labels:** llvm:codegen, debuginfo, crash
**Closed Date:** 2025-02-04T21:36:24Z

## Body

When compiling the following IR ([compiler explorer](https://godbolt.org/z/PxhfM1KP4); the IR is [compiled from this Rust code](https://godbolt.org/z/8doGjqq36)):
<details><summary>LLVM IR</summary>

```llvm
; ModuleID = 'example.bbc0a90001a6c25f-cgu.0'
source_filename = "example.bbc0a90001a6c25f-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nonlazybind uwtable
define void @f(ptr align 16 %arg) unnamed_addr #0 !dbg !7 {
start:
  %arg.dbg.spill = alloca [8 x i8], align 8
  store ptr %arg, ptr %arg.dbg.spill, align 8
    #dbg_declare(ptr %arg.dbg.spill, !29, !DIExpression(), !30)
  ret void, !dbg !31
}

attributes #0 = { nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}
!llvm.dbg.cu = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 2, !"RtLibUseGOT", i32 1}
!2 = !{i32 2, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{!"rustc version 1.85.0-nightly (33c245b9e 2024-12-10)"}
!5 = distinct !DICompileUnit(language: DW_LANG_Rust, file: !6, producer: "clang LLVM (rustc version 1.85.0-nightly (33c245b9e 2024-12-10))", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!6 = !DIFile(filename: "/app/example.rs/@/example.bbc0a90001a6c25f-cgu.0", directory: "/app")
!7 = distinct !DISubprogram(name: "f", scope: !9, file: !8, line: 10, type: !10, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !5, templateParams: !23, retainedNodes: !28)
!8 = !DIFile(filename: "example.rs", directory: "/app", checksumkind: CSK_MD5, checksum: "a5e69d61728bea50929f951cefeb214d")
!9 = !DINamespace(name: "example", scope: null)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12}
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, name: "&example::Enum", baseType: !13, size: 64, align: 64, dwarfAddressSpace: 0)
!13 = !DICompositeType(tag: DW_TAG_structure_type, name: "Enum", scope: !9, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !15, templateParams: !23, identifier: "469ee2dc29a0e6d2c8e5f6f5942b091")
!14 = !DIFile(filename: "<unknown>", directory: "")
!15 = !{!16}
!16 = !DICompositeType(tag: DW_TAG_variant_part, scope: !13, file: !14, size: 256, align: 128, elements: !17, templateParams: !23, identifier: "190ce2747378be38ac7362eb40a80906", discriminator: !26)
!17 = !{!18, !24}
!18 = !DIDerivedType(tag: DW_TAG_member, name: "VariantA", scope: !16, file: !14, baseType: !19, size: 256, align: 128, extraData: i128 0)
!19 = !DICompositeType(tag: DW_TAG_structure_type, name: "VariantA", scope: !13, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !20, templateParams: !23, identifier: "588fffa66c9ca296e16b6bf021973a50")
!20 = !{!21}
!21 = !DIDerivedType(tag: DW_TAG_member, name: "__0", scope: !19, file: !14, baseType: !22, size: 8, align: 8, offset: 128, flags: DIFlagPrivate)
!22 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned)
!23 = !{}
!24 = !DIDerivedType(tag: DW_TAG_member, name: "VariantB", scope: !16, file: !14, baseType: !25, size: 256, align: 128, extraData: i128 18446744073709551616)
!25 = !DICompositeType(tag: DW_TAG_structure_type, name: "VariantB", scope: !13, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !23, identifier: "ace4dd05425154b121f11815fa7d85bc")
!26 = !DIDerivedType(tag: DW_TAG_member, scope: !13, file: !14, baseType: !27, size: 128, align: 128, flags: DIFlagArtificial)
!27 = !DIBasicType(name: "u128", size: 128, encoding: DW_ATE_unsigned)
!28 = !{!29}
!29 = !DILocalVariable(name: "arg", arg: 1, scope: !7, file: !8, line: 10, type: !12)
!30 = !DILocation(line: 10, column: 6, scope: !7)
!31 = !DILocation(line: 12, column: 2, scope: !32)
!32 = !DILexicalBlockFile(scope: !7, file: !8, discriminator: 0)
```

</details>

An LLVM assertion is triggered by [the `getZExtValue`/`getSExtValue` call in `DwarfUnit::constructTypeDIE`](https://github.com/llvm/llvm-project/blob/da71203e6fc6b8e08c9979204506d385e9cb07b8/llvm/lib/CodeGen/AsmPrinter/DwarfUnit.cpp#L975-L980) (if assertions are disabled, the discriminant value is instead truncated to a 64-bit integer), as the discriminant value of `VariantB` does not fit in a 64-bit integer.

<details><summary>Assertion backtrace</summary>

```
llc: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:1523: uint64_t llvm::APInt::getZExtValue() const: Assertion `getActiveBits() <= 64 && "Too many bits for uint64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
 #0 0x0000000003c27c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3c27c38)
 #1 0x0000000003c2563c SignalHandler(int) Signals.cpp:0:0
 #2 0x0000742c74a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000742c74a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000742c74a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000742c74a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000742c74a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000742c74a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000291ad4d (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291ad4d)
 #9 0x000000000291aba8 llvm::DwarfUnit::constructTypeDIE(llvm::DIE&, llvm::DICompositeType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291aba8)
#10 0x000000000291ae27 llvm::DwarfUnit::createTypeDIE(llvm::DIScope const*, llvm::DIE&, llvm::DIType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291ae27)
#11 0x00000000029116eb llvm::DwarfUnit::addType(llvm::DIE&, llvm::DIType const*, llvm::dwarf::Attribute) (.constprop.0) DwarfUnit.cpp:0:0
#12 0x00000000029157b7 llvm::DwarfUnit::constructTypeDIE(llvm::DIE&, llvm::DIDerivedType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x29157b7)
#13 0x000000000291b00f llvm::DwarfUnit::createTypeDIE(llvm::DIScope const*, llvm::DIE&, llvm::DIType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291b00f)
#14 0x00000000029114ca llvm::DwarfUnit::addType(llvm::DIE&, llvm::DIType const*, llvm::dwarf::Attribute) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x29114ca)
#15 0x000000000296bde4 llvm::DwarfCompileUnit::applyCommonDbgVariableAttributes(llvm::DbgVariable const&, llvm::DIE&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x296bde4)
#16 0x00000000028dab10 llvm::DwarfDebug::finishEntityDefinitions() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28dab10)
#17 0x00000000028fb975 llvm::DwarfDebug::finalizeModuleInfo() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28fb975)
#18 0x00000000028ff922 llvm::DwarfDebug::endModule() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28ff922)
#19 0x00000000028b94fe llvm::AsmPrinter::doFinalization(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28b94fe)
#20 0x0000000003187ded llvm::FPPassManager::doFinalization(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3187ded)
#21 0x00000000031928e6 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x31928e6)
#22 0x0000000000876b98 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#23 0x00000000007625ae main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7625ae)
#24 0x0000742c74a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000742c74a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000086d4c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x86d4c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: None (beetrees)

<details>
When compiling the following IR ([compiler explorer](https://godbolt.org/z/PxhfM1KP4); the IR is [compiled from this Rust code](https://godbolt.org/z/8doGjqq36)):
&lt;details&gt;&lt;summary&gt;LLVM IR&lt;/summary&gt;

```llvm
; ModuleID = 'example.bbc0a90001a6c25f-cgu.0'
source_filename = "example.bbc0a90001a6c25f-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nonlazybind uwtable
define void @<!-- -->f(ptr align 16 %arg) unnamed_addr #<!-- -->0 !dbg !7 {
start:
  %arg.dbg.spill = alloca [8 x i8], align 8
  store ptr %arg, ptr %arg.dbg.spill, align 8
    #dbg_declare(ptr %arg.dbg.spill, !29, !DIExpression(), !30)
  ret void, !dbg !31
}

attributes #<!-- -->0 = { nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}
!llvm.dbg.cu = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 2, !"RtLibUseGOT", i32 1}
!2 = !{i32 2, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{!"rustc version 1.85.0-nightly (33c245b9e 2024-12-10)"}
!5 = distinct !DICompileUnit(language: DW_LANG_Rust, file: !6, producer: "clang LLVM (rustc version 1.85.0-nightly (33c245b9e 2024-12-10))", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!6 = !DIFile(filename: "/app/example.rs/@/example.bbc0a90001a6c25f-cgu.0", directory: "/app")
!7 = distinct !DISubprogram(name: "f", scope: !9, file: !8, line: 10, type: !10, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !5, templateParams: !23, retainedNodes: !28)
!8 = !DIFile(filename: "example.rs", directory: "/app", checksumkind: CSK_MD5, checksum: "a5e69d61728bea50929f951cefeb214d")
!9 = !DINamespace(name: "example", scope: null)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12}
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, name: "&amp;example::Enum", baseType: !13, size: 64, align: 64, dwarfAddressSpace: 0)
!13 = !DICompositeType(tag: DW_TAG_structure_type, name: "Enum", scope: !9, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !15, templateParams: !23, identifier: "469ee2dc29a0e6d2c8e5f6f5942b091")
!14 = !DIFile(filename: "&lt;unknown&gt;", directory: "")
!15 = !{!16}
!16 = !DICompositeType(tag: DW_TAG_variant_part, scope: !13, file: !14, size: 256, align: 128, elements: !17, templateParams: !23, identifier: "190ce2747378be38ac7362eb40a80906", discriminator: !26)
!17 = !{!18, !24}
!18 = !DIDerivedType(tag: DW_TAG_member, name: "VariantA", scope: !16, file: !14, baseType: !19, size: 256, align: 128, extraData: i128 0)
!19 = !DICompositeType(tag: DW_TAG_structure_type, name: "VariantA", scope: !13, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !20, templateParams: !23, identifier: "588fffa66c9ca296e16b6bf021973a50")
!20 = !{!21}
!21 = !DIDerivedType(tag: DW_TAG_member, name: "__0", scope: !19, file: !14, baseType: !22, size: 8, align: 8, offset: 128, flags: DIFlagPrivate)
!22 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned)
!23 = !{}
!24 = !DIDerivedType(tag: DW_TAG_member, name: "VariantB", scope: !16, file: !14, baseType: !25, size: 256, align: 128, extraData: i128 18446744073709551616)
!25 = !DICompositeType(tag: DW_TAG_structure_type, name: "VariantB", scope: !13, file: !14, size: 256, align: 128, flags: DIFlagPrivate, elements: !23, identifier: "ace4dd05425154b121f11815fa7d85bc")
!26 = !DIDerivedType(tag: DW_TAG_member, scope: !13, file: !14, baseType: !27, size: 128, align: 128, flags: DIFlagArtificial)
!27 = !DIBasicType(name: "u128", size: 128, encoding: DW_ATE_unsigned)
!28 = !{!29}
!29 = !DILocalVariable(name: "arg", arg: 1, scope: !7, file: !8, line: 10, type: !12)
!30 = !DILocation(line: 10, column: 6, scope: !7)
!31 = !DILocation(line: 12, column: 2, scope: !32)
!32 = !DILexicalBlockFile(scope: !7, file: !8, discriminator: 0)
```

&lt;/details&gt;

An LLVM assertion is triggered by [the `getZExtValue`/`getSExtValue` call in `DwarfUnit::constructTypeDIE`](https://github.com/llvm/llvm-project/blob/da71203e6fc6b8e08c9979204506d385e9cb07b8/llvm/lib/CodeGen/AsmPrinter/DwarfUnit.cpp#L975-L980) (if assertions are disabled, the discriminant value is instead truncated to a 64-bit integer), as the discriminant value of `VariantB` does not fit in a 64-bit integer.

&lt;details&gt;&lt;summary&gt;Assertion backtrace&lt;/summary&gt;

```
llc: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:1523: uint64_t llvm::APInt::getZExtValue() const: Assertion `getActiveBits() &lt;= 64 &amp;&amp; "Too many bits for uint64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
 #<!-- -->0 0x0000000003c27c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3c27c38)
 #<!-- -->1 0x0000000003c2563c SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x0000742c74a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000742c74a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000742c74a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000742c74a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000742c74a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000742c74a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000291ad4d (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291ad4d)
 #<!-- -->9 0x000000000291aba8 llvm::DwarfUnit::constructTypeDIE(llvm::DIE&amp;, llvm::DICompositeType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291aba8)
#<!-- -->10 0x000000000291ae27 llvm::DwarfUnit::createTypeDIE(llvm::DIScope const*, llvm::DIE&amp;, llvm::DIType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291ae27)
#<!-- -->11 0x00000000029116eb llvm::DwarfUnit::addType(llvm::DIE&amp;, llvm::DIType const*, llvm::dwarf::Attribute) (.constprop.0) DwarfUnit.cpp:0:0
#<!-- -->12 0x00000000029157b7 llvm::DwarfUnit::constructTypeDIE(llvm::DIE&amp;, llvm::DIDerivedType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x29157b7)
#<!-- -->13 0x000000000291b00f llvm::DwarfUnit::createTypeDIE(llvm::DIScope const*, llvm::DIE&amp;, llvm::DIType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x291b00f)
#<!-- -->14 0x00000000029114ca llvm::DwarfUnit::addType(llvm::DIE&amp;, llvm::DIType const*, llvm::dwarf::Attribute) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x29114ca)
#<!-- -->15 0x000000000296bde4 llvm::DwarfCompileUnit::applyCommonDbgVariableAttributes(llvm::DbgVariable const&amp;, llvm::DIE&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x296bde4)
#<!-- -->16 0x00000000028dab10 llvm::DwarfDebug::finishEntityDefinitions() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28dab10)
#<!-- -->17 0x00000000028fb975 llvm::DwarfDebug::finalizeModuleInfo() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28fb975)
#<!-- -->18 0x00000000028ff922 llvm::DwarfDebug::endModule() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28ff922)
#<!-- -->19 0x00000000028b94fe llvm::AsmPrinter::doFinalization(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x28b94fe)
#<!-- -->20 0x0000000003187ded llvm::FPPassManager::doFinalization(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3187ded)
#<!-- -->21 0x00000000031928e6 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x31928e6)
#<!-- -->22 0x0000000000876b98 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->23 0x00000000007625ae main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7625ae)
#<!-- -->24 0x0000742c74a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000742c74a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x000000000086d4c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x86d4c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

&lt;/details&gt;
</details>


---

