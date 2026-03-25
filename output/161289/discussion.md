# AArch64 Scalable vector debug info crash

**Author:** aemerson
**URL:** https://github.com/llvm/llvm-project/issues/161289
**Status:** Open
**Labels:** backend:AArch64, debuginfo, SVE, crash

## Body

See https://godbolt.org/z/9dKMr7vT6

```
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

define <vscale x 16 x half> @scalable_vector_debug_info(<vscale x 16 x half> %arg) #0 !dbg !7 {
; CHECK-LABEL: scalable_vector_debug_info:
; CHECK:       .Lfunc_begin0:
; CHECK-NEXT:    .cfi_startproc
; CHECK-NEXT:  // %bb.0:
; CHECK-NEXT:    //DEBUG_VALUE: scalable_vector_debug_info:arg <- $z0
; CHECK-NEXT:    ret
  #dbg_value(<vscale x 16 x half> %arg, !13, !DIExpression(), !14)
  %extract = tail call <vscale x 8 x half> @llvm.vector.extract.nxv8f16.nxv16f16(<vscale x 16 x half> %arg, i64 0)
  ret <vscale x 16 x half> %arg
}

declare <vscale x 8 x half> @llvm.vector.extract.nxv8f16.nxv16f16(<vscale x 16 x half>, i64 immarg)

attributes #0 = { "aarch64_pstate_sm_enabled" "target-features"="+sme" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "scalable-vector-debug.c", directory: "/tmp")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"frame-pointer", i32 2}
!7 = distinct !DISubprogram(name: "scalable_vector_debug_info", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !12)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10}
!10 = !DIBasicType(name: "svfloat16_t", size: 16, encoding: DW_ATE_float)
!11 = !DIBasicType(name: "__fp16", size: 16, encoding: DW_ATE_float)
!12 = !{!13}
!13 = !DILocalVariable(name: "arg", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!14 = !DILocation(line: 0, scope: !7)
!15 = distinct !DISubprogram(name: "streaming_sve_debug_info", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !18)
!16 = !DILocalVariable(name: "arg", arg: 1, scope: !15, file: !1, line: 5, type: !10)
!17 = !DILocation(line: 0, scope: !15)
!18 = !{!16}
```

Compiling with `llc -mtriple aarch64 -mattr=+sme` triggers:
`LLVM ERROR: Cannot implicitly convert a scalable size to a fixed-width size in TypeSize::operator ScalarTy()`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Amara Emerson (aemerson)

<details>
See https://godbolt.org/z/9dKMr7vT6

```
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

define &lt;vscale x 16 x half&gt; @<!-- -->scalable_vector_debug_info(&lt;vscale x 16 x half&gt; %arg) #<!-- -->0 !dbg !7 {
; CHECK-LABEL: scalable_vector_debug_info:
; CHECK:       .Lfunc_begin0:
; CHECK-NEXT:    .cfi_startproc
; CHECK-NEXT:  // %bb.0:
; CHECK-NEXT:    //DEBUG_VALUE: scalable_vector_debug_info:arg &lt;- $z0
; CHECK-NEXT:    ret
  #dbg_value(&lt;vscale x 16 x half&gt; %arg, !13, !DIExpression(), !14)
  %extract = tail call &lt;vscale x 8 x half&gt; @<!-- -->llvm.vector.extract.nxv8f16.nxv16f16(&lt;vscale x 16 x half&gt; %arg, i64 0)
  ret &lt;vscale x 16 x half&gt; %arg
}

declare &lt;vscale x 8 x half&gt; @<!-- -->llvm.vector.extract.nxv8f16.nxv16f16(&lt;vscale x 16 x half&gt;, i64 immarg)

attributes #<!-- -->0 = { "aarch64_pstate_sm_enabled" "target-features"="+sme" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "scalable-vector-debug.c", directory: "/tmp")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"frame-pointer", i32 2}
!7 = distinct !DISubprogram(name: "scalable_vector_debug_info", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !12)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10}
!10 = !DIBasicType(name: "svfloat16_t", size: 16, encoding: DW_ATE_float)
!11 = !DIBasicType(name: "__fp16", size: 16, encoding: DW_ATE_float)
!12 = !{!13}
!13 = !DILocalVariable(name: "arg", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!14 = !DILocation(line: 0, scope: !7)
!15 = distinct !DISubprogram(name: "streaming_sve_debug_info", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !18)
!16 = !DILocalVariable(name: "arg", arg: 1, scope: !15, file: !1, line: 5, type: !10)
!17 = !DILocation(line: 0, scope: !15)
!18 = !{!16}
```

Compiling with `llc -mtriple aarch64 -mattr=+sme` triggers:
`LLVM ERROR: Cannot implicitly convert a scalable size to a fixed-width size in TypeSize::operator ScalarTy()`
</details>


---

### Comment 2 - aemerson

@sdesmalen-arm @davemgreen  could your teams take a look?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Amara Emerson (aemerson)

<details>
See https://godbolt.org/z/9dKMr7vT6

```
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

define &lt;vscale x 16 x half&gt; @<!-- -->scalable_vector_debug_info(&lt;vscale x 16 x half&gt; %arg) #<!-- -->0 !dbg !7 {
; CHECK-LABEL: scalable_vector_debug_info:
; CHECK:       .Lfunc_begin0:
; CHECK-NEXT:    .cfi_startproc
; CHECK-NEXT:  // %bb.0:
; CHECK-NEXT:    //DEBUG_VALUE: scalable_vector_debug_info:arg &lt;- $z0
; CHECK-NEXT:    ret
  #dbg_value(&lt;vscale x 16 x half&gt; %arg, !13, !DIExpression(), !14)
  %extract = tail call &lt;vscale x 8 x half&gt; @<!-- -->llvm.vector.extract.nxv8f16.nxv16f16(&lt;vscale x 16 x half&gt; %arg, i64 0)
  ret &lt;vscale x 16 x half&gt; %arg
}

declare &lt;vscale x 8 x half&gt; @<!-- -->llvm.vector.extract.nxv8f16.nxv16f16(&lt;vscale x 16 x half&gt;, i64 immarg)

attributes #<!-- -->0 = { "aarch64_pstate_sm_enabled" "target-features"="+sme" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "scalable-vector-debug.c", directory: "/tmp")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"frame-pointer", i32 2}
!7 = distinct !DISubprogram(name: "scalable_vector_debug_info", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !12)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10}
!10 = !DIBasicType(name: "svfloat16_t", size: 16, encoding: DW_ATE_float)
!11 = !DIBasicType(name: "__fp16", size: 16, encoding: DW_ATE_float)
!12 = !{!13}
!13 = !DILocalVariable(name: "arg", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!14 = !DILocation(line: 0, scope: !7)
!15 = distinct !DISubprogram(name: "streaming_sve_debug_info", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !18)
!16 = !DILocalVariable(name: "arg", arg: 1, scope: !15, file: !1, line: 5, type: !10)
!17 = !DILocation(line: 0, scope: !15)
!18 = !{!16}
```

Compiling with `llc -mtriple aarch64 -mattr=+sme` triggers:
`LLVM ERROR: Cannot implicitly convert a scalable size to a fixed-width size in TypeSize::operator ScalarTy()`
</details>


---

### Comment 4 - MacDue

This might need some rework of the `DW_OP_LLVM_fragment` opcode. It looks like this is attempting to make two `<vscale x 8 x half>` fragments for the `<vscale x 16 x half>` operand, and the issue is that `DW_OP_LLVM_fragment` takes `OffsetInBits`  and `SizeInBits` as literals, dropping the `vscale` multiple.  

---

### Comment 5 - MacDue

Would you be happy with simply dropping the debug info here (with a `FIXME` to support scalable vectors)? 

---

### Comment 6 - aemerson

> Would you be happy with simply dropping the debug info here (with a `FIXME` to support scalable vectors)? 

I wouldn't go as far as happy, but that's certainly preferable to crashing.

---

