# ICE if returned lambda unpacks captured parameter pack in requires clause

**Author:** FalcoGer
**URL:** https://github.com/llvm/llvm-project/issues/169968

## Body

This code causes an internal compiler error.

https://godbolt.org/z/5W9x1EvY7

```cpp
#include <utility>
#include <functional>
#include <string_view>
#include <print>

template <auto Func, auto ...constexprs, typename... Params>
[[nodiscard]] constexpr auto bind_front(Params&&... params) noexcept
{
    if constexpr (sizeof...(params) == 0)
    {
        return []<typename... Inner>(Inner&&... inner) static
          requires std::invocable<decltype(Func), decltype(constexprs)..., Inner...>
        {
            return std::invoke(Func, constexprs..., std::forward<Inner>(inner)...);
        };
    }
    else
    {
        return [...params = std::forward<Params>(params)]
           <typename... Inner>(Inner&&... inner)
             // compiles without this requires clause
             requires std::invocable<decltype(Func), decltype(constexprs)..., decltype(params)..., Inner...>
           {
               return std::invoke(Func, constexprs..., params..., std::forward<Inner>(inner)...);
           };
    }
}

void print(const std::string_view prefix, const std::string_view suffix)
{
    std::println("{} {}", prefix, suffix);
}

int main()
{
    const auto hi = bind_front<print>("Hello");
    hi("world!");

    return 0;
}
```

```text
clang++ -std=c++26 clang_ice.cpp -o clang_ice
clang_ice.cpp:20:20: warning: declaration shadows a local variable [-Wshadow-uncaptured-local]
   20 |         return [...params = std::forward<Params>(params)]
      |                    ^
clang_ice.cpp:7:53: note: previous declaration is here
    7 | [[nodiscard]] constexpr auto bind_front(Params&&... params) noexcept
      |                                                     ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-22/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-llvm-bc -flto=full -flto-unit -dumpdir clang_ice- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names-main-file-name clang_ice.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu bdver4 -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature +xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature -xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx10.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature -ptwrite -target-feature -widekl -target-feature -movrs -target-feature -invpcid -target-feature +64bit -target-feature -xsavec -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature -kl -target-feature -sha512 -target-feature -avxvnni -target-feature -rtm -target-feature -adx -target-feature +avx2 -target-feature -hreset -target-feature -movdiri -target-feature -serialize -target-feature -vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature -clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature -rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature -pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature +mwaitx -target-feature +lwp -target-feature +lzcnt -target-feature -sha -target-feature -movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -avxneconvert -target-feature +tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature -avx10.2 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature -clwb -target-feature +mmx -target-feature +sse2 -target-feature -rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature-amx-movrs -target-feature -rdpid -target-feature +fma4 -target-feature -avx512vbmi -target-feature -shstk -target-feature -vaes -target-feature -waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature +sse4a -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Desktop/test -fcoverage-compilation-dir=/home/user/Desktop/test -resource-dir /usr/lib/llvm-22/lib/clang/22 -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-pc-linux-gnu -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /usr/lib/llvm-22/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Wdouble-promotion -Wformat=2 -Wformat-nonliteral -Wformat-y2k -Wnull-dereference -Wimplicit-fallthrough -Wmissing-include-dirs -Wswitch -Wswitch-bool -Wcovered-switch-default -Wunused -Woverloaded-virtual -Wuninitialized -Walloca -Wconversion -Wfloat-conversion -Wsign-conversion -Wduplicate-decl-specifier -Wfloat-equal -Wshadow-all -Wundef -Wcast-qual -Wcast-align -Wredundant-decls -Wold-style-cast -Wstack-protector -Werror=pedantic -Werror=char-subscripts -Werror=null-dereference -Werror=dangling-gsl -Werror=init-self -Werror=switch -Werror=implicit-fallthrough -Werror=misleading-indentation -Werror=missing-braces -Werror=sequence-point -Werror=return-type -Werror=multichar -Werror=non-virtual-dtor -Werror=overloaded-virtual -Werror=reinterpret-base-class -pedantic-errors -std=c++26 -fdeprecated-macro -fexperimental-library -ferror-limit 19 -fmessage-length=227 -stack-protector 1 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fvirtual-function-elimination -fwhole-program-vtables -fsplit-lto-unit -faddrsig -fdwarf2-cfi-asm -o /tmp/clang_ice-ccd40b.o -x c++ clang_ice.cpp
1.	clang_ice.cpp:39:16: current parser token ')'
2.	clang_ice.cpp:37:1: parsing function body 'main'
3.	clang_ice.cpp:37:1: in compound statement ('{}')
4.	clang_ice.cpp:20:16: instantiating function definition 'bind_front(const char (&)[6])::(anonymous class)::operator()<const char (&)[7]>'
5.	clang_ice.cpp:20:16: LLVM IR generation of declaration 'bind_front(const char (&)[6])::(anonymous class)::operator()'
6.	clang_ice.cpp:20:16: Mangling declaration 'bind_front(const char (&)[6])::(anonymous class)::operator()'
  #0 0x0000755ed1ac58cf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc58cf)
  #1 0x0000755ed1ac3147 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc3147)
  #2 0x0000755ed1ac669a (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc669a)
  #3 0x0000755ecc645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #4 0x0000755ed77c6b49 clang::MangleContext::shouldMangleDeclName(clang::NamedDecl const*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bc6b49)
  #5 0x0000755ed779565c (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9565c)
  #6 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
  #7 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
  #8 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
  #9 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #10 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #11 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #12 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #13 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #14 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #15 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #16 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #17 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #18 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #19 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #20 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #21 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #22 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #23 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #24 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #25 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #26 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #27 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #28 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #29 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #30 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #31 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #32 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #33 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #34 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #35 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #36 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #37 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #38 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #39 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #40 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #41 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #42 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #43 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #44 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #45 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #46 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #47 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #48 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #49 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #50 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #51 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #52 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #53 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #54 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #55 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #56 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #57 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #58 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #59 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #60 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #61 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #62 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #63 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #64 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #65 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #66 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #67 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #68 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #69 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #70 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #71 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #72 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #73 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #74 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #75 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #76 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #77 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #78 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #79 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #80 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #81 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #82 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #83 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #84 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #85 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #86 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #87 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #88 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #89 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #90 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #91 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #92 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #93 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #94 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #95 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #96 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #97 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #98 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #99 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#100 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#101 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#102 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#103 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#104 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#105 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#106 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#107 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#108 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#109 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#110 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#111 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#112 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#113 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#114 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#115 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#116 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#117 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#118 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#119 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#120 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#121 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#122 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#123 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#124 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#125 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#126 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#127 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#128 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#129 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#130 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#131 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#132 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#133 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#134 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#135 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#136 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#137 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#138 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#139 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#140 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#141 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#142 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#143 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#144 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#145 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#146 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#147 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#148 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#149 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#150 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#151 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#152 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#153 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#154 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#155 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#156 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#157 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#158 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#159 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#160 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#161 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#162 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#163 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#164 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#165 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#166 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#167 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#168 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#169 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#170 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#171 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#172 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#173 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#174 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#175 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#176 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#177 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#178 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#179 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#180 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#181 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#182 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#183 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#184 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#185 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#186 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#187 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#188 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#189 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#190 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#191 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#192 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#193 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#194 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#195 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#196 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#197 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#198 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#199 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#200 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#201 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#202 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#203 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#204 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#205 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#206 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#207 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#208 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#209 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#210 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#211 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#212 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#213 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#214 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#215 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#216 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#217 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#218 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#219 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#220 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#221 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#222 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#223 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#224 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#225 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#226 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#227 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#228 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#229 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#230 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#231 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#232 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#233 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#234 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#235 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#236 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#237 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#238 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#239 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#240 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#241 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#242 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#243 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#244 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#245 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#246 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#247 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#248 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#249 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#250 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#251 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#252 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#253 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#254 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#255 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 22.0.0 (++20251128081833+1cea4a0841da-1~exp1~20251128081957.1317)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/clang_ice-ac7ea9.cpp
clang++: note: diagnostic msg: /tmp/clang_ice-ac7ea9.sh
clang++: note: diagnostic msg:

********************
```

[clang_ice-ac7ea9.cpp](https://github.com/user-attachments/files/23831037/clang_ice-ac7ea9.cpp)
[clang_ice-ac7ea9.sh](https://github.com/user-attachments/files/23831038/clang_ice-ac7ea9.sh)

## Comments

### Comment 1 - FalcoGer

clang++ --version
Ubuntu clang version 22.0.0 (++20251128081833+1cea4a0841da-1~exp1~20251128081957.1317)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (FalcoGer)

<details>
This code causes an internal compiler error.

https://godbolt.org/z/5W9x1EvY7

```cpp
#include &lt;utility&gt;
#include &lt;functional&gt;
#include &lt;string_view&gt;
#include &lt;print&gt;

template &lt;auto Func, auto ...constexprs, typename... Params&gt;
[[nodiscard]] constexpr auto bind_front(Params&amp;&amp;... params) noexcept
{
    if constexpr (sizeof...(params) == 0)
    {
        return []&lt;typename... Inner&gt;(Inner&amp;&amp;... inner) static
          requires std::invocable&lt;decltype(Func), decltype(constexprs)..., Inner...&gt;
        {
            return std::invoke(Func, constexprs..., std::forward&lt;Inner&gt;(inner)...);
        };
    }
    else
    {
        return [...params = std::forward&lt;Params&gt;(params)]
           &lt;typename... Inner&gt;(Inner&amp;&amp;... inner)
             // compiles without this requires clause
             requires std::invocable&lt;decltype(Func), decltype(constexprs)..., decltype(params)..., Inner...&gt;
           {
               return std::invoke(Func, constexprs..., params..., std::forward&lt;Inner&gt;(inner)...);
           };
    }
}

void print(const std::string_view prefix, const std::string_view suffix)
{
    std::println("{} {}", prefix, suffix);
}

int main()
{
    const auto hi = bind_front&lt;print&gt;("Hello");
    hi("world!");

    return 0;
}
```

```text
clang++ -std=c++26 clang_ice.cpp -o clang_ice
clang_ice.cpp:20:20: warning: declaration shadows a local variable [-Wshadow-uncaptured-local]
   20 |         return [...params = std::forward&lt;Params&gt;(params)]
      |                    ^
clang_ice.cpp:7:53: note: previous declaration is here
    7 | [[nodiscard]] constexpr auto bind_front(Params&amp;&amp;... params) noexcept
      |                                                     ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-22/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-llvm-bc -flto=full -flto-unit -dumpdir clang_ice- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names-main-file-name clang_ice.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu bdver4 -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature +xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature -xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx10.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature -ptwrite -target-feature -widekl -target-feature -movrs -target-feature -invpcid -target-feature +64bit -target-feature -xsavec -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature -kl -target-feature -sha512 -target-feature -avxvnni -target-feature -rtm -target-feature -adx -target-feature +avx2 -target-feature -hreset -target-feature -movdiri -target-feature -serialize -target-feature -vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature -clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature -rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature -pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature +mwaitx -target-feature +lwp -target-feature +lzcnt -target-feature -sha -target-feature -movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -avxneconvert -target-feature +tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature -avx10.2 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature -clwb -target-feature +mmx -target-feature +sse2 -target-feature -rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature-amx-movrs -target-feature -rdpid -target-feature +fma4 -target-feature -avx512vbmi -target-feature -shstk -target-feature -vaes -target-feature -waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature +sse4a -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Desktop/test -fcoverage-compilation-dir=/home/user/Desktop/test -resource-dir /usr/lib/llvm-22/lib/clang/22 -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-pc-linux-gnu -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /usr/lib/llvm-22/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/local/gcc/latest/lib/gcc/x86_64-pc-linux-gnu/16.0.0/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Wdouble-promotion -Wformat=2 -Wformat-nonliteral -Wformat-y2k -Wnull-dereference -Wimplicit-fallthrough -Wmissing-include-dirs -Wswitch -Wswitch-bool -Wcovered-switch-default -Wunused -Woverloaded-virtual -Wuninitialized -Walloca -Wconversion -Wfloat-conversion -Wsign-conversion -Wduplicate-decl-specifier -Wfloat-equal -Wshadow-all -Wundef -Wcast-qual -Wcast-align -Wredundant-decls -Wold-style-cast -Wstack-protector -Werror=pedantic -Werror=char-subscripts -Werror=null-dereference -Werror=dangling-gsl -Werror=init-self -Werror=switch -Werror=implicit-fallthrough -Werror=misleading-indentation -Werror=missing-braces -Werror=sequence-point -Werror=return-type -Werror=multichar -Werror=non-virtual-dtor -Werror=overloaded-virtual -Werror=reinterpret-base-class -pedantic-errors -std=c++26 -fdeprecated-macro -fexperimental-library -ferror-limit 19 -fmessage-length=227 -stack-protector 1 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fvirtual-function-elimination -fwhole-program-vtables -fsplit-lto-unit -faddrsig -fdwarf2-cfi-asm -o /tmp/clang_ice-ccd40b.o -x c++ clang_ice.cpp
1.	clang_ice.cpp:39:16: current parser token ')'
2.	clang_ice.cpp:37:1: parsing function body 'main'
3.	clang_ice.cpp:37:1: in compound statement ('{}')
4.	clang_ice.cpp:20:16: instantiating function definition 'bind_front(const char (&amp;)[6])::(anonymous class)::operator()&lt;const char (&amp;)[7]&gt;'
5.	clang_ice.cpp:20:16: LLVM IR generation of declaration 'bind_front(const char (&amp;)[6])::(anonymous class)::operator()'
6.	clang_ice.cpp:20:16: Mangling declaration 'bind_front(const char (&amp;)[6])::(anonymous class)::operator()'
  #<!-- -->0 0x0000755ed1ac58cf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc58cf)
  #<!-- -->1 0x0000755ed1ac3147 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc3147)
  #<!-- -->2 0x0000755ed1ac669a (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4cc669a)
  #<!-- -->3 0x0000755ecc645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #<!-- -->4 0x0000755ed77c6b49 clang::MangleContext::shouldMangleDeclName(clang::NamedDecl const*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bc6b49)
  #<!-- -->5 0x0000755ed779565c (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9565c)
  #<!-- -->6 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
  #<!-- -->7 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
  #<!-- -->8 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
  #<!-- -->9 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->10 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->11 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->12 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->13 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->14 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->15 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->16 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->17 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->18 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->19 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->20 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->21 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->22 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->23 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->24 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->25 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->26 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->27 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->28 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->29 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->30 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->31 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->32 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->33 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->34 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->35 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->36 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->37 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->38 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->39 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->40 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->41 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->42 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->43 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->44 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->45 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->46 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->47 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->48 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->49 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->50 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->51 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->52 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->53 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->54 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->55 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->56 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->57 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->58 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->59 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->60 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->61 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->62 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->63 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->64 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->65 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->66 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->67 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->68 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->69 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->70 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->71 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->72 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->73 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->74 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->75 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->76 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->77 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->78 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->79 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->80 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->81 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->82 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->83 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->84 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->85 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->86 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->87 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
 #<!-- -->88 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
 #<!-- -->89 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
 #<!-- -->90 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
 #<!-- -->91 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
 #<!-- -->92 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
 #<!-- -->93 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
 #<!-- -->94 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
 #<!-- -->95 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
 #<!-- -->96 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
 #<!-- -->97 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
 #<!-- -->98 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
 #<!-- -->99 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->100 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->101 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->102 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->103 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->104 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->105 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->106 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->107 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->108 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->109 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->110 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->111 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->112 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->113 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->114 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->115 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->116 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->117 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->118 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->119 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->120 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->121 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->122 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->123 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->124 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->125 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->126 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->127 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->128 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->129 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->130 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->131 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->132 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->133 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->134 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->135 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->136 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->137 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->138 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->139 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->140 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->141 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->142 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->143 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->144 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->145 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->146 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->147 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->148 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->149 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->150 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->151 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->152 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->153 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->154 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->155 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->156 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->157 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->158 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->159 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->160 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->161 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->162 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->163 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->164 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->165 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->166 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->167 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->168 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->169 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->170 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->171 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->172 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->173 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->174 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->175 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->176 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->177 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->178 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->179 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->180 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->181 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->182 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->183 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->184 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->185 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->186 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->187 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->188 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->189 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->190 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->191 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->192 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->193 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->194 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->195 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->196 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->197 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->198 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->199 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->200 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->201 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->202 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->203 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->204 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->205 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->206 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->207 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->208 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->209 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->210 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->211 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->212 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->213 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->214 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->215 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->216 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->217 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->218 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->219 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->220 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->221 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->222 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->223 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->224 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->225 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->226 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->227 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->228 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->229 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->230 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->231 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->232 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->233 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->234 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->235 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->236 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->237 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->238 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->239 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->240 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->241 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->242 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->243 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
#<!-- -->244 0x0000755ed77aaee5 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1baaee5)
#<!-- -->245 0x0000755ed7795b5f (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b95b5f)
#<!-- -->246 0x0000755ed77acae3 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1bacae3)
#<!-- -->247 0x0000755ed77963d2 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b963d2)
#<!-- -->248 0x0000755ed77a4d43 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba4d43)
#<!-- -->249 0x0000755ed779f5e9 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9f5e9)
#<!-- -->250 0x0000755ed779b18b (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9b18b)
#<!-- -->251 0x0000755ed779727d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9727d)
#<!-- -->252 0x0000755ed779729d (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1b9729d)
#<!-- -->253 0x0000755ed77a5269 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba5269)
#<!-- -->254 0x0000755ed77a3a29 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba3a29)
#<!-- -->255 0x0000755ed77a07b1 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ba07b1)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 22.0.0 (++20251128081833+1cea4a0841da-1~exp1~20251128081957.1317)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/clang_ice-ac7ea9.cpp
clang++: note: diagnostic msg: /tmp/clang_ice-ac7ea9.sh
clang++: note: diagnostic msg:

********************
```

[clang_ice-ac7ea9.cpp](https://github.com/user-attachments/files/23831037/clang_ice-ac7ea9.cpp)
[clang_ice-ac7ea9.sh](https://github.com/user-attachments/files/23831038/clang_ice-ac7ea9.sh)
</details>


---

### Comment 3 - frederick-vs-ja

Reduced to
```C++
template<class...>
concept truth = true;

constexpr auto capture_accessor(auto&&... args) {
  return [...capts = args]() requires truth<decltype(capts)...> { // referring to the capture pack in the constraint
    return true;
  };
}

int main() {
  capture_accessor(42)(); // not in constant evaluation
}
```

Started to crash since Clang 18, [Godbolt link](https://godbolt.org/z/41fbvn91r).

---

### Comment 4 - shafik

@zygoloid this bisects to 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 

---

