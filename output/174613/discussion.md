# [clang][RISCV] error for type with attribute `riscv_rvv_vector_bits`

**Author:** exur00
**URL:** https://github.com/llvm/llvm-project/issues/174613
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2026-01-08T06:34:08Z

## Body

Compiling the following code with args: `-march=rv32i_zve64d_zvl128b -mabi=ilp32d -mrvv-vector-bits=zvl`

```C++
#include <riscv_vector.h>

typedef vint64m2_t vint64m2_t_fixed __attribute__((riscv_rvv_vector_bits(__riscv_v_fixed_vlen * 2)));

vint32m1_t foo(vint64m2_t_fixed v)
{
    return __riscv_vncvt_x_x_w_i32m1(v, __riscv_v_fixed_vlen/32);
}
```
results in a clang error: `clang frontend command failed with exit code 139`
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv32-unknown-linux-gnu -march=rv32gc -mabi=ilp32d --gcc-toolchain=/opt/compiler-explorer/riscv32/gcc-14.2.0/riscv32-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv32/gcc-14.2.0/riscv32-unknown-linux-gnu/riscv32-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=rv32i_zve64d_zvl128b -mabi=ilp32d -mrvv-vector-bits=zvl <source>
1.	<eof> parser at end of file
2.	<source>:6:12: LLVM IR generation of declaration 'foo'
3.	<source>:6:12: Generating code for declaration 'foo'
4.	<source>:6:12: Mangling declaration 'foo'
#0 0x0000000003d50ee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d50ee8)
#1 0x0000000003d4e8bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d4e8bc)
#2 0x0000000003c94778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 0x000078de47442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x0000000000c8a56e (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) (.cold) ItaniumMangle.cpp:0:0
```
to reproduce:
- vector element type must be 64 bit
- architecture must be rv32
- Argument type of `foo` must be `vint64m2_t_fixed `
- typedef needs the attribute

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Loeka Rogge (exur00)

<details>
Compiling the following code with args: `-march=rv32i_zve64d_zvl128b -mabi=ilp32d -mrvv-vector-bits=zvl`

```C++
#include &lt;riscv_vector.h&gt;

typedef vint64m2_t vint64m2_t_fixed __attribute__((riscv_rvv_vector_bits(__riscv_v_fixed_vlen * 2)));

vint32m1_t foo(vint64m2_t_fixed v)
{
    return __riscv_vncvt_x_x_w_i32m1(v, __riscv_v_fixed_vlen/32);
}
```
results in a clang error: `clang frontend command failed with exit code 139`
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv32-unknown-linux-gnu -march=rv32gc -mabi=ilp32d --gcc-toolchain=/opt/compiler-explorer/riscv32/gcc-14.2.0/riscv32-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv32/gcc-14.2.0/riscv32-unknown-linux-gnu/riscv32-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=rv32i_zve64d_zvl128b -mabi=ilp32d -mrvv-vector-bits=zvl &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:12: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:6:12: Generating code for declaration 'foo'
4.	&lt;source&gt;:6:12: Mangling declaration 'foo'
#<!-- -->0 0x0000000003d50ee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d50ee8)
#<!-- -->1 0x0000000003d4e8bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d4e8bc)
#<!-- -->2 0x0000000003c94778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#<!-- -->3 0x000078de47442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#<!-- -->4 0x0000000000c8a56e (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) (.cold) ItaniumMangle.cpp:0:0
```
to reproduce:
- vector element type must be 64 bit
- architecture must be rv32
- Argument type of `foo` must be `vint64m2_t_fixed `
- typedef needs the attribute
</details>


---

