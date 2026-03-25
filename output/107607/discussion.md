# [RISCV] Miscompilation on rv64

**Author:** mikhailramalho
**URL:** https://github.com/llvm/llvm-project/issues/107607
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2024-12-11T06:21:50Z

## Body

The following program miscompiles when using clang++ for rv64:
```
$ cat foo.cpp 
#include <cmath>

int main() {
  long double y = NAN;
  _Float16 a = static_cast<_Float16>(y);
  return (a != a);
}
```
The cast of `NAN` from `long double` to `_Float16` generates `0`, instead of `NAN`.

It only happens with clang++ in rv64. It works just fine in intel 64 and rv32. Compiling with g++ for rv64 also works.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Mikhail R. Gadelha (mikhailramalho)

<details>
The following program miscompiles when using clang++ for rv64:
```
$ cat foo.cpp 
#include &lt;cmath&gt;

int main() {
  long double y = NAN;
  _Float16 a = static_cast&lt;_Float16&gt;(y);
  return (a != a);
}
```
The cast of `NAN` from `long double` to `_Float16` generates `0`, instead of `NAN`.

It only happens with clang++ in rv64. It works just fine in intel 64 and rv32. Compiling with g++ for rv64 also works.

</details>


---

### Comment 2 - topperc

What -march or -mcpu? With or without optimizations?

---

### Comment 3 - mikhailramalho

I'm calling `clang++ foo.cpp -static -g`, the same for g++.

The full cmdline:
```
clang version 18.1.8
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
 "/usr/bin/clang++" "-cc1" "-triple" "riscv64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "foo.cpp" "-static-define" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "generic-rv64" "-target-feature" "+m" "-target-feature" "+a" "-target-feature" "+f" "-target-feature" "+d" "-target-feature" "+c" "-target-feature" "+zicsr" "-target-feature" "-e" "-target-feature" "-h" "-target-feature" "-smaia" "-target-feature" "-smepmp" "-target-feature" "-ssaia" "-target-feature" "-svinval" "-target-feature" "-svnapot" "-target-feature" "-svpbmt" "-target-feature" "-v" "-target-feature" "-xcvalu" "-target-feature" "-xcvbi" "-target-feature" "-xcvbitmanip" "-target-feature" "-xcvelw" "-target-feature" "-xcvmac" "-target-feature" "-xcvmem" "-target-feature" "-xcvsimd" "-target-feature" "-xsfvcp" "-target-feature" "-xsfvfnrclipxfqf" "-target-feature" "-xsfvfwmaccqqq" "-target-feature" "-xsfvqmaccdod" "-target-feature" "-xsfvqmaccqoq" "-target-feature" "-xtheadba" "-target-feature" "-xtheadbb" "-target-feature" "-xtheadbs" "-target-feature" "-xtheadcmo" "-target-feature" "-xtheadcondmov" "-target-feature" "-xtheadfmemidx" "-target-feature" "-xtheadmac" "-target-feature" "-xtheadmemidx" "-target-feature" "-xtheadmempair" "-target-feature" "-xtheadsync" "-target-feature" "-xtheadvdot" "-target-feature" "-xventanacondops" "-target-feature" "-za128rs" "-target-feature" "-za64rs" "-target-feature" "-zawrs" "-target-feature" "-zba" "-target-feature" "-zbb" "-target-feature" "-zbc" "-target-feature" "-zbkb" "-target-feature" "-zbkc" "-target-feature" "-zbkx" "-target-feature" "-zbs" "-target-feature" "-zca" "-target-feature" "-zcb" "-target-feature" "-zcd" "-target-feature" "-zce" "-target-feature" "-zcf" "-target-feature" "-zcmp" "-target-feature" "-zcmt" "-target-feature" "-zdinx" "-target-feature" "-zfa" "-target-feature" "-zfh" "-target-feature" "-zfhmin" "-target-feature" "-zfinx" "-target-feature" "-zhinx" "-target-feature" "-zhinxmin" "-target-feature" "-zic64b" "-target-feature" "-zicbom" "-target-feature" "-zicbop" "-target-feature" "-zicboz" "-target-feature" "-ziccamoa" "-target-feature" "-ziccif" "-target-feature" "-zicclsm" "-target-feature" "-ziccrse" "-target-feature" "-zicntr" "-target-feature" "-zicond" "-target-feature" "-zifencei" "-target-feature" "-zihintntl" "-target-feature" "-zihintpause" "-target-feature" "-zihpm" "-target-feature" "-zk" "-target-feature" "-zkn" "-target-feature" "-zknd" "-target-feature" "-zkne" "-target-feature" "-zknh" "-target-feature" "-zkr" "-target-feature" "-zks" "-target-feature" "-zksed" "-target-feature" "-zksh" "-target-feature" "-zkt" "-target-feature" "-zmmul" "-target-feature" "-zvbb" "-target-feature" "-zvbc" "-target-feature" "-zve32f" "-target-feature" "-zve32x" "-target-feature" "-zve64d" "-target-feature" "-zve64f" "-target-feature" "-zve64x" "-target-feature" "-zvfh" "-target-feature" "-zvfhmin" "-target-feature" "-zvkb" "-target-feature" "-zvkg" "-target-feature" "-zvkn" "-target-feature" "-zvknc" "-target-feature" "-zvkned" "-target-feature" "-zvkng" "-target-feature" "-zvknha" "-target-feature" "-zvknhb" "-target-feature" "-zvks" "-target-feature" "-zvksc" "-target-feature" "-zvksed" "-target-feature" "-zvksg" "-target-feature" "-zvksh" "-target-feature" "-zvkt" "-target-feature" "-zvl1024b" "-target-feature" "-zvl128b" "-target-feature" "-zvl16384b" "-target-feature" "-zvl2048b" "-target-feature" "-zvl256b" "-target-feature" "-zvl32768b" "-target-feature" "-zvl32b" "-target-feature" "-zvl4096b" "-target-feature" "-zvl512b" "-target-feature" "-zvl64b" "-target-feature" "-zvl65536b" "-target-feature" "-zvl8192b" "-target-feature" "-experimental-zacas" "-target-feature" "-experimental-zcmop" "-target-feature" "-experimental-zfbfmin" "-target-feature" "-experimental-zicfilp" "-target-feature" "-experimental-zicfiss" "-target-feature" "-experimental-zimop" "-target-feature" "-experimental-ztso" "-target-feature" "-experimental-zvfbfmin" "-target-feature" "-experimental-zvfbfwma" "-target-feature" "+relax" "-target-abi" "lp64d" "-msmall-data-limit" "8" "-debug-info-kind=constructor" "-dwarf-version=4" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/user/llvm-project/build" "-fcoverage-compilation-dir=/home/user/llvm-project/build" "-resource-dir" "/usr/lib/clang/18" "-internal-isystem" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/../../../../include/c++/14.2.1" "-internal-isystem" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/../../../../include/c++/14.2.1/riscv64-unknown-linux-gnu" "-internal-isystem" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward" "-internal-isystem" "/usr/lib/clang/18/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/../../../../riscv64-unknown-linux-gnu/include" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "2" "-fno-signed-char" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/foo-06710d.o" "-x" "c++" "foo.cpp"
 "/usr/bin/ld" "--hash-style=gnu" "--build-id" "--eh-frame-hdr" "-m" "elf64lriscv" "-X" "-static" "-o" "a.out" "/lib/crt1.o" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/crti.o" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/crtbeginT.o" "-L/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1" "-L/lib" "-L/usr/lib" "/tmp/foo-06710d.o" "-lstdc++" "-lm" "--start-group" "-lgcc" "-lgcc_eh" "-lc" "--end-group" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/crtend.o" "/usr/bin/../lib/gcc/riscv64-unknown-linux-gnu/14.2.1/crtn.o"
``` 


---

### Comment 4 - mikhailramalho

https://godbolt.org/z/ch99WGfdh

I'm trying to debug this now... Up until after the call to `__trunctfhf2` the registers look pretty much the same. g++ generates some extra instructions and gets the result right.

---

### Comment 5 - topperc

@mikhailramalho I just watched your talk from the LLVM conference where you mentioned you have a fix for this. Can you share?

---

