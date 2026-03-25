# [RISCV] Miscompile caused by #126108 (Decompose single source shuffles (without exact VLEN))

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/126943
**Status:** Closed
**Labels:** backend:RISC-V, miscompilation
**Closed Date:** 2025-02-12T20:18:35Z

## Body

There's active discussion on #126108, so this issue is written with some haste to provide a clear reproducer an doesn't have in-depth analysis and hasn't further reduced the input.

The issue was initially spotted for RVA23 with EVL tail folding, but can be reproduced just with v:

```
wget https://raw.githubusercontent.com/llvm/llvm-test-suite/refs/heads/main/SingleSource/Regression/C/gcc-c-torture/execute/20050826-2.c
$HOME/llvm-project/build/stage1/bin/clang  \
  --target=riscv64-linux-gnu \
  -fuse-ld=lld \
  --sysroot=$HOME/rvsysroot \
  -O3 \
  -march=rva20u64_v \
  20050826-2.c
export QEMU_LD_PREFIX="$HOME/rvsysroot"
./a.out # program aborts
```

(rvsysroot should work with any sysroot with the minimal files for linking and loading a working executable). The above assumes you have a working qemu-riscv64 that executes via binfmt_misc.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
There's active discussion on #<!-- -->126108, so this issue is written with some haste to provide a clear reproducer an doesn't have in-depth analysis and hasn't further reduced the input.

The simplest reproducer:
```
wget https://raw.githubusercontent.com/llvm/llvm-test-suite/refs/heads/main/SingleSource/Regression/C/gcc-c-torture/execute/20050826-2.c
$HOME/llvm-project/build/stage1/bin/clang  \
  --target=riscv64-linux-gnu \
  -fuse-ld=lld \
  --sysroot=$HOME/rvsysroot \
  -O3 \
  -march=rva23u64 -mllvm -force-tail-folding-style=data-with-evl -mllvm -prefer-predicate-over-epilogue=predicate-else-scalar-epilogue \
  20050826-2.c
export QEMU_LD_PREFIX="$HOME/rvsysroot"
export QEMU_CPU="rv64,zba=true,zbb=true,zbc=false,zbs=true,zfhmin=true,v=true,vext_spec=v1.0,zkt=true,zvfhmin=true,zvbb=true,zvkt=true,zihintntl=true,zicond=true,zimop=true,zcmop=true,zcb=true,zfa=true,zawrs=true,rvv_ta_all_1s=true,rvv_ma_all_1s=true,rvv_vl_half_avl=true"
```
</details>


---

### Comment 2 - preames

I have manually confirmed that the bug I noted after reverting https://github.com/llvm/llvm-project/pull/126108 does trigger on this example.  The fix for that bug is in https://github.com/llvm/llvm-project/pull/126951, and is the "fixed" version referred to below.  

I inspected the resulting assembly, and confirmed the presence of that miscompile.  I couldn't get the example to link with my (probably fairly broken) sysroot environment, so I haven't executed the fixed binary to confirm there's not another miscompile which I'm not noticing. 

The shuffle being lowering is: t12: v8i64 = vector_shuffle<0,0,0,0,0,0,0,1> t97, undef:v8i64
Indices are: t124: v8i16 = BUILD_VECTOR Constant:i64<0>, Constant:i64<0>, Constant:i64<0>, Constant:i64<0>, Constant:i64<0>, Constant:i64<0>, Constant:i64<0>, Constant:i64<1>

With zvl128b and a runtime VLENB, the miscompile is resulting in the last element being element 7, not element 1.  

Here are the assembly diffs:
```

$ diff 20050826.s-old 20050826.s-broken 
257a258
> 	csrr	a1, vlenb
262c263,264
< 	vsetvli	zero, zero, e64, m4, ta, ma
---
> 	srli	a1, a1, 3
> 	vsetvli	a0, zero, e64, m1, ta, ma
263a266,278
> 	vsetivli	zero, 8, e16, m1, ta, ma
> 	vslidedown.vx	v12, v16, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v21, v13, v12
> 	vsetivli	zero, 8, e16, m1, ta, ma
> 	vslidedown.vx	v12, v12, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v22, v14, v12
> 	vsetivli	zero, 8, e16, m1, ta, ma
> 	vslidedown.vx	v12, v12, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v23, v15, v12
> 	vsetivli	zero, 8, e64, m4, ta, ma
359c374
< 	.ident	"clang version 21.0.0git (https://github.com/llvm/llvm-project.git c77d2027592c93ca1a2009a90b64b5cbdf8cfba3)"
---
> 	.ident	"clang version 21.0.0git (https://github.com/llvm/llvm-project.git 7b942f96049474b64f2e8090cbbcf179bf0b2893)"

$ diff 20050826.s-old 20050826.s-fixed 
256c256
< 	vmv.v.i	v16, 0
---
> 	vmv.v.i	v13, 0
257a258
> 	csrr	a1, vlenb
261,264c262,277
< 	vslide1down.vx	v16, v16, a0
< 	vsetvli	zero, zero, e64, m4, ta, ma
< 	vrgatherei16.vv	v20, v12, v16
< 	vmseq.vv	v12, v8, v20
---
> 	vslide1down.vx	v13, v13, a0
> 	srli	a1, a1, 3
> 	vslidedown.vx	v14, v13, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v17, v12, v14
> 	vsetivli	zero, 8, e16, m1, ta, ma
> 	vslidedown.vx	v14, v14, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v16, v12, v13
> 	vsetivli	zero, 8, e16, m1, ta, ma
> 	vslidedown.vx	v13, v14, a1
> 	vsetvli	a0, zero, e64, m1, ta, ma
> 	vrgatherei16.vv	v18, v12, v14
> 	vrgatherei16.vv	v19, v12, v13
> 	vsetivli	zero, 8, e64, m4, ta, ma
> 	vmseq.vv	v12, v8, v16
359c372
< 	.ident	"clang version 21.0.0git (https://github.com/llvm/llvm-project.git c77d2027592c93ca1a2009a90b64b5cbdf8cfba3)"
---
> 	.ident	"clang version 21.0.0git (https://github.com/llvm/llvm-project.git 08a383d29a2b55bcd5d9057d4591003b1614a90d)"


```

---

### Comment 3 - asb

I can confirm the fix committed from #126951 as 9478822f4f63aa2e5f. Marking as closed!

---

