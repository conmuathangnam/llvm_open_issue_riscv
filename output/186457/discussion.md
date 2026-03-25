# crash in clang++-21 compilation when using -fkeep-static-consts

**Author:** itajsh
**URL:** https://github.com/llvm/llvm-project/issues/186457
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction, regression:19

## Body

update: It seems that it wasn't adding these 2 options, but in fact adding -fkeep-static-consts that causes the crash.
update: more minimal reproduction code https://godbolt.org/z/K5Yb8xrxe


Adding either or both of these options to my compile command causes a crash: -fno-omit-frame-pointer -fno-optimize-sibling-calls.
In this case a trace when adding them both:

```
Stack dump:
0.	Program arguments: clang++-21 -Werror -Weverything -Wconversion -Wsign-conversion -Wbool-conversion -Wdouble-promotion -Wfloat-conversion -Wstring-conversion -fsafe-buffer-usage-suggestions -Wno-reserved-identifier -Wnullability-completeness -Wno-c++98-compat -Wno-c++98-compat-pedantic -gfull -O0 -fstandalone-debug -gcolumn-info -fno-limit-debug-info -fkeep-static-consts -fno-delete-null-pointer-checks -fno-strict-aliasing -fno-inline -fno-inline-functions -fno-omit-frame-pointer -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_ASSERTIONS -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=implicit-unsigned-integer-truncation -fsanitize=implicit-signed-integer-truncation -fsanitize=implicit-integer-sign-change -fsanitize=integer-divide-by-zero -fsanitize=implicit-bitfield-conversion -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=nullability-arg -fsanitize=nullability-assign -fsanitize=nullability-return -fsanitize=returns-nonnull-attribute -fsanitize=pointer-overflow -fsanitize=unreachable -fsanitize=return -fsanitize=shift -fsanitize=unsigned-shift-base -fsanitize=signed-integer-overflow -fsanitize=vla-bound -fsanitize=vptr -isystem /home/itaj/cncrdata/libr/boost_1_90_0 -std=c++23 -nostdinc++ -isystem /lib/llvm-21/include/c++/v1 -c result.bsheri_api/cpp_train/test_x1.exec.cld_user.cpp -o temp_generated_content.bsheri_gen/cpp_train/test_x1.exec.cld_user.o
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.1      0x000079b4448fc91f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.21.1      0x000079b4448fa1c7 llvm::sys::RunSignalHandlers() + 135
2  libLLVM.so.21.1      0x000079b444833e87
3  libc.so.6            0x000079b43fc45330
4  libLLVM.so.21.1      0x000079b44498b32e llvm::ConstantExpr::getPointerBitCastOrAddrSpaceCast(llvm::Constant*, llvm::Type*) + 14
5  libclang-cpp.so.21.1 0x000079b44b31f212
6  libclang-cpp.so.21.1 0x000079b44b312020 clang::CodeGen::CodeGenModule::Release() + 3424
7  libclang-cpp.so.21.1 0x000079b44b3c94a2
8  libclang-cpp.so.21.1 0x000079b44b2f4806 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) + 166
9  libclang-cpp.so.21.1 0x000079b449e882c9 clang::ParseAST(clang::Sema&, bool, bool) + 617
10 libclang-cpp.so.21.1 0x000079b44ba876d2 clang::FrontendAction::Execute() + 50
11 libclang-cpp.so.21.1 0x000079b44b9fe134 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 948
12 libclang-cpp.so.21.1 0x000079b44bb0f37c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 700
13 clang++-21           0x000056c52027e31b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 5995
14 clang++-21           0x000056c52027b024
15 libclang-cpp.so.21.1 0x000079b44b6c2172
16 libLLVM.so.21.1      0x000079b444833b07 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
17 libclang-cpp.so.21.1 0x000079b44b6c19c6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 406
18 libclang-cpp.so.21.1 0x000079b44b684f02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 546
19 libclang-cpp.so.21.1 0x000079b44b6850ef clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
20 libclang-cpp.so.21.1 0x000079b44b6a3866 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 406
21 clang++-21           0x000056c52027aaf9 clang_main(int, char**, llvm::ToolContext const&) + 6297
22 clang++-21           0x000056c520288ef3 main + 131
23 libc.so.6            0x000079b43fc2a1ca
24 libc.so.6            0x000079b43fc2a28b __libc_start_main + 139
25 clang++-21           0x000056c520278d25 _start + 37
clang++-21: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.cpp
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.sh
clang++-21: note: diagnostic msg:

********************
```
[test_x1-f1dae6.sh](https://github.com/user-attachments/files/25977323/test_x1-f1dae6.sh)
[test_x1-f1dae6.cpp](https://github.com/user-attachments/files/25977324/test_x1-f1dae6.cpp)


## Comments

### Comment 1 - itajsh

it happens now without these options as well, I'll check what exactly was the change that caused it and report.



---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Itaj Sherman (itajsh)

<details>
Adding either or both of these options to my compile command causes a crash: -fno-omit-frame-pointer -fno-optimize-sibling-calls.
In this case a trace when adding them both:

```
Stack dump:
0.	Program arguments: clang++-21 -Werror -Weverything -Wconversion -Wsign-conversion -Wbool-conversion -Wdouble-promotion -Wfloat-conversion -Wstring-conversion -fsafe-buffer-usage-suggestions -Wno-reserved-identifier -Wnullability-completeness -Wno-c++98-compat -Wno-c++98-compat-pedantic -gfull -O0 -fstandalone-debug -gcolumn-info -fno-limit-debug-info -fkeep-static-consts -fno-delete-null-pointer-checks -fno-strict-aliasing -fno-inline -fno-inline-functions -fno-omit-frame-pointer -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_ASSERTIONS -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=implicit-unsigned-integer-truncation -fsanitize=implicit-signed-integer-truncation -fsanitize=implicit-integer-sign-change -fsanitize=integer-divide-by-zero -fsanitize=implicit-bitfield-conversion -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=nullability-arg -fsanitize=nullability-assign -fsanitize=nullability-return -fsanitize=returns-nonnull-attribute -fsanitize=pointer-overflow -fsanitize=unreachable -fsanitize=return -fsanitize=shift -fsanitize=unsigned-shift-base -fsanitize=signed-integer-overflow -fsanitize=vla-bound -fsanitize=vptr -isystem /home/itaj/cncrdata/libr/boost_1_90_0 -std=c++23 -nostdinc++ -isystem /lib/llvm-21/include/c++/v1 -c result.bsheri_api/cpp_train/test_x1.exec.cld_user.cpp -o temp_generated_content.bsheri_gen/cpp_train/test_x1.exec.cld_user.o
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.1      0x000079b4448fc91f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM.so.21.1      0x000079b4448fa1c7 llvm::sys::RunSignalHandlers() + 135
2  libLLVM.so.21.1      0x000079b444833e87
3  libc.so.6            0x000079b43fc45330
4  libLLVM.so.21.1      0x000079b44498b32e llvm::ConstantExpr::getPointerBitCastOrAddrSpaceCast(llvm::Constant*, llvm::Type*) + 14
5  libclang-cpp.so.21.1 0x000079b44b31f212
6  libclang-cpp.so.21.1 0x000079b44b312020 clang::CodeGen::CodeGenModule::Release() + 3424
7  libclang-cpp.so.21.1 0x000079b44b3c94a2
8  libclang-cpp.so.21.1 0x000079b44b2f4806 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) + 166
9  libclang-cpp.so.21.1 0x000079b449e882c9 clang::ParseAST(clang::Sema&amp;, bool, bool) + 617
10 libclang-cpp.so.21.1 0x000079b44ba876d2 clang::FrontendAction::Execute() + 50
11 libclang-cpp.so.21.1 0x000079b44b9fe134 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 948
12 libclang-cpp.so.21.1 0x000079b44bb0f37c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 700
13 clang++-21           0x000056c52027e31b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 5995
14 clang++-21           0x000056c52027b024
15 libclang-cpp.so.21.1 0x000079b44b6c2172
16 libLLVM.so.21.1      0x000079b444833b07 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 151
17 libclang-cpp.so.21.1 0x000079b44b6c19c6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 406
18 libclang-cpp.so.21.1 0x000079b44b684f02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 546
19 libclang-cpp.so.21.1 0x000079b44b6850ef clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 159
20 libclang-cpp.so.21.1 0x000079b44b6a3866 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 406
21 clang++-21           0x000056c52027aaf9 clang_main(int, char**, llvm::ToolContext const&amp;) + 6297
22 clang++-21           0x000056c520288ef3 main + 131
23 libc.so.6            0x000079b43fc2a1ca
24 libc.so.6            0x000079b43fc2a28b __libc_start_main + 139
25 clang++-21           0x000056c520278d25 _start + 37
clang++-21: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.cpp
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.sh
clang++-21: note: diagnostic msg:

********************

[test_x1-f1dae6.sh](https://github.com/user-attachments/files/25977323/test_x1-f1dae6.sh)
[test_x1-f1dae6.cpp](https://github.com/user-attachments/files/25977324/test_x1-f1dae6.cpp)
```
</details>


---

### Comment 3 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.

Please provide reproducer, preferably on https://godbolt.org 


---

### Comment 4 - itajsh

The option that causes it is -fkeep-static-consts, meaning is the one you need to take out of the list for it not to reproduce.


---

### Comment 5 - itajsh

@EugeneZelenko I never tried either (installed an unpacked version, nor godbolt).
I'll try to see how to get the exact configuration to godbolt.
I don't immediately see where I could config all the command line settings that I use in order to reproduce.

---

### Comment 6 - shafik

> [@EugeneZelenko](https://github.com/EugeneZelenko) I never tried either (installed an unpacked version, nor godbolt). I'll try to see how to get the exact configuration to godbolt. I don't immediately see where I could config all the command line settings that I use in order to reproduce.

If you can't get it reproduce in godbolt then we need the preprocessed file from the crash or even better a reduction of the that file, see [How to submit an LLVM bug report](https://llvm.org/docs/HowToSubmitABug.html)

---

### Comment 7 - itajsh

The preprocessed file from the crash,
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.cpp
clang++-21: note: diagnostic msg: /tmp/test_x1-f1dae6.sh

I attached with my first message.
Are they not attached correctly?

It reproduces the crash on godbolt clang 22, but it’s complicated.
https://godbolt.org/z/7d3cx6f66

It has to do with a bad println format string.
It possibly depends on std::println from the libc++ of 21.

Because I have several versions of clang installed, the way I compile with clang++-21 is:
clang++-21 … -nostdinc++ -isystem /lib/llvm-21/include/c++/v1 …

and I minimized the code that reproduces it:
```c++
#include <print>
inline void bad_println()
{
	std::println("+partial_sums()");
}
```
while the good string works:
```c++
#include <print>
inline void good_println()
{
	std::println("{}", "+partial_sums()");
}
```
But it doesn’t reproduce on godbolt with just this code.
I have to paste the preprocessed file from my machine that already includes `<print>` from -isystem /lib/llvm-21/include/c++/v1.

The compiler settings I use, pasted in godbolt, the last option causes the bug to reproduce.
```
-nostdinc++ -std=c++23 -Werror -Weverything -Wconversion -Wsign-conversion -Wbool-conversion -Wdouble-promotion -Wfloat-conversion -Wstring-conversion -fsafe-buffer-usage-suggestions -Wno-reserved-identifier -Wnullability-completeness -Wno-unused-macros -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-gnu-line-marker -gfull -O0 -fstandalone-debug -gcolumn-info -fno-limit-debug-info -fno-delete-null-pointer-checks -fno-strict-aliasing -fno-inline -fno-inline-functions -fno-optimize-sibling-calls -fno-omit-frame-pointer -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_ASSERTIONS -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=implicit-unsigned-integer-truncation -fsanitize=implicit-signed-integer-truncation -fsanitize=implicit-integer-sign-change -fsanitize=integer-divide-by-zero -fsanitize=implicit-bitfield-conversion -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=nullability-arg -fsanitize=nullability-assign -fsanitize=nullability-return -fsanitize=returns-nonnull-attribute -fsanitize=pointer-overflow -fsanitize=unreachable -fsanitize=return -fsanitize=shift -fsanitize=unsigned-shift-base -fsanitize=signed-integer-overflow -fsanitize=vla-bound -fsanitize=vptr -fkeep-static-consts
```


---

### Comment 8 - shafik

Ideally a reproducer does not have headers b/c they can hide a lot.

---

### Comment 9 - itajsh

Ok, I’m sorry I didn’t figure it out earlier,
To run on godblot I had to remove my -isystem /lib/llvm-21/include/c++/v1,
but it made it use its default prolly libstdc++.
Once I added -stdlib=libc++ to the compiler options (to replace my local -isystem),
it *reproduce* on both clang 22 and clang 21.
(using the local #include <print> rather than my preprocessed file).
https://godbolt.org/z/K5Yb8xrxe


---

### Comment 10 - shafik

This goes back to clang-19 but it only works in clang-18 if we remove `-fsanitize=implicit-bitfield-conversion`

---

