# Miscompile (x86_64 VPTERNLOG) after shuffling register/memory operands (with repro)

**Author:** atomgalaxy
**URL:** https://github.com/llvm/llvm-project/issues/157929
**Status:** Open
**Labels:** backend:X86, miscompilation

## Body

The attached llvmir miscompiles on the x86_64 backend.

[pternlog-bug-repro.ll](https://github.com/user-attachments/files/22260709/pternlog-bug.ll.3.txt)

What happens:

```
$ for level in 0 1 2 3 s z; do clang++ -O$level pternlog-bug-repro.ll; ./a.out; echo $?; done
 0
1
1
1
1
1
```

What I expect to happen: optimization level should not affect program success.

Fix: replace around line 45 to reshuffle the registers:
```
-  %7 = tail call <8 x i32> @llvm.x86.avx512.pternlog.d.256(<8 x i32> <i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 0>, <8 x i32> %6, <8 x i32> splat (i32 2147483647), i32 228)
+  %7 = tail call <8 x i32> @llvm.x86.avx512.pternlog.d.256(<8 x i32> %6, <8 x i32> <i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 0>, <8 x i32> splat (i32 2147483647), i32 228)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Gašper Ažman (atomgalaxy)

<details>
The attached llvmir miscompiles on the x86_64 backend.

[pternlog-bug-repro.ll](https://github.com/user-attachments/files/22260709/pternlog-bug.ll.3.txt)

What happens:

```
$ for level in 0 1 2 3 s z; do clang++ -O$level pternlog-bug-repro.ll; ./a.out; echo $?; done
 0
1
1
1
1
1
```

What I expect to happen: optimization level should not affect program success.

Fix: replace around line 45 to reshuffle the registers:
```
-  %7 = tail call &lt;8 x i32&gt; @<!-- -->llvm.x86.avx512.pternlog.d.256(&lt;8 x i32&gt; &lt;i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 0&gt;, &lt;8 x i32&gt; %6, &lt;8 x i32&gt; splat (i32 2147483647), i32 228)
+  %7 = tail call &lt;8 x i32&gt; @<!-- -->llvm.x86.avx512.pternlog.d.256(&lt;8 x i32&gt; %6, &lt;8 x i32&gt; &lt;i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 1067282596, i32 0&gt;, &lt;8 x i32&gt; splat (i32 2147483647), i32 228)
```
</details>


---

