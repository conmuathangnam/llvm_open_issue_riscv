# `clang -lstdc++ -ccc-print-phases` crashed

**Author:** pzzp
**URL:** https://github.com/llvm/llvm-project/issues/160046
**Status:** Open
**Labels:** clang:driver, crash

## Body

reproduce:
```
$ clang -lstdc++ -ccc-print-phases
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -ccc-print-phases -lstdc++
1.      Compilation construction
 #0 0x0000757e27fc4d90 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.18.1+0x7c4d90)
 #1 0x0000757e27fc1d4d (/usr/lib/libLLVM.so.18.1+0x7c1d4d)
 #2 0x0000757e2724c1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #3 0x0000757e2738e61c (/usr/lib/libc.so.6+0x17f61c)
 #4 0x0000757e2fdf7fc5 (/usr/lib/libclang-cpp.so.18.1+0x3f7fc5)
 #5 0x0000757e316e87d9 (/usr/lib/libclang-cpp.so.18.1+0x1ce87d9)
 #6 0x0000757e316e8fb0 (/usr/lib/libclang-cpp.so.18.1+0x1ce8fb0)
 #7 0x0000757e316eae4f clang::driver::Driver::PrintActions(clang::driver::Compilation const&) const (/usr/lib/libclang-cpp.so.18.1+0x1ceae4f)
 #8 0x0000757e3172b9db clang::driver::Driver::BuildCompilation(llvm::ArrayRef<char const*>) (/usr/lib/libclang-cpp.so.18.1+0x1d2b9db)
 #9 0x000060512ebf5a8f clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-18+0x18a8f)
#10 0x000060512ebe7fd5 main (/usr/bin/clang-18+0xafd5)
#11 0x0000757e27234e08 (/usr/lib/libc.so.6+0x25e08)
#12 0x0000757e27234ecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#13 0x000060512ebe8035 _start (/usr/bin/clang-18+0xb035)
[1]    64 segmentation fault (core dumped)  clang -ccc-print-phases -lstdc++
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-driver

Author: None (pzzp)

<details>
reproduce:
```
$ clang -lstdc++ -ccc-print-phases
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -ccc-print-phases -lstdc++
1.      Compilation construction
 #<!-- -->0 0x0000757e27fc4d90 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.18.1+0x7c4d90)
 #<!-- -->1 0x0000757e27fc1d4d (/usr/lib/libLLVM.so.18.1+0x7c1d4d)
 #<!-- -->2 0x0000757e2724c1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #<!-- -->3 0x0000757e2738e61c (/usr/lib/libc.so.6+0x17f61c)
 #<!-- -->4 0x0000757e2fdf7fc5 (/usr/lib/libclang-cpp.so.18.1+0x3f7fc5)
 #<!-- -->5 0x0000757e316e87d9 (/usr/lib/libclang-cpp.so.18.1+0x1ce87d9)
 #<!-- -->6 0x0000757e316e8fb0 (/usr/lib/libclang-cpp.so.18.1+0x1ce8fb0)
 #<!-- -->7 0x0000757e316eae4f clang::driver::Driver::PrintActions(clang::driver::Compilation const&amp;) const (/usr/lib/libclang-cpp.so.18.1+0x1ceae4f)
 #<!-- -->8 0x0000757e3172b9db clang::driver::Driver::BuildCompilation(llvm::ArrayRef&lt;char const*&gt;) (/usr/lib/libclang-cpp.so.18.1+0x1d2b9db)
 #<!-- -->9 0x000060512ebf5a8f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang-18+0x18a8f)
#<!-- -->10 0x000060512ebe7fd5 main (/usr/bin/clang-18+0xafd5)
#<!-- -->11 0x0000757e27234e08 (/usr/lib/libc.so.6+0x25e08)
#<!-- -->12 0x0000757e27234ecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#<!-- -->13 0x000060512ebe8035 _start (/usr/bin/clang-18+0xb035)
[1]    64 segmentation fault (core dumped)  clang -ccc-print-phases -lstdc++
```
</details>


---

