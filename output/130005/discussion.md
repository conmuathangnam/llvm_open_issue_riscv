# Segfault on capturing lambda in fold expression in requires clause of lambda

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/130005
**Status:** Closed
**Labels:** duplicate, clang:frontend, concepts, crash-on-valid, lambda, regression:18
**Closed Date:** 2025-03-06T14:05:47Z

## Body

https://godbolt.org/z/Gxja75597
```cpp
int main() { [](auto... args) requires(([args] {}, ..., true)) {}(); }
```
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 5422e2c68107650c33f39e6224d1ff0064467550)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk-20250306/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
 (in-process)
 "/opt/compiler-explorer/clang-trunk-20250306/bin/clang-21" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -mllvm --x86-asm-syntax=intel -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /app/output.s -x c++ <source>
clang -cc1 version 21.0.0git based upon LLVM 21.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include <...> search starts here:
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward
 /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


## Comments

### Comment 1 - frederick-vs-ja

Looks like a duplicate of #70064.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/Gxja75597
```cpp
int main() { [](auto... args) requires(([args] {}, ..., true)) {}(); }
```
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 5422e2c68107650c33f39e6224d1ff0064467550)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk-20250306/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
 (in-process)
 "/opt/compiler-explorer/clang-trunk-20250306/bin/clang-21" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -mllvm --x86-asm-syntax=intel -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /app/output.s -x c++ &lt;source&gt;
clang -cc1 version 21.0.0git based upon LLVM 21.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu
 /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward
 /opt/compiler-explorer/clang-trunk-20250306/lib/clang/21/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

