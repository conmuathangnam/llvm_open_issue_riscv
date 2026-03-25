# Clang frontend segfaults during CTAD with nested initializer_lists

**Author:** mrguenther
**URL:** https://github.com/llvm/llvm-project/issues/81436
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2024-12-10T10:37:31Z

## Body

# Clang frontend segfaults during CTAD with paramer packs
## Reproduction steps
Compile the attached code with `clang++` version `16.0.0` and the `--std=c++20` flag. The compiler will crash with a segmentation fault.

Note that the attached code snippets mention a simple change that will prevent the crash.

Also note that `g++` version `9.4.0` compiles the code successfully (though that compiler version needs `--std=c++2a` instead of `--std=c++20`).

The full version information for the compilers I used is below:
```
$ clang++ --version
clang version 16.0.0
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

I don't recall for sure where I got my installed copy of `clang-16`, but I *think* I got it from the official LLVM repo.

In case it's relevant, I was running Clang in a WSL2 environment.

### Failing example
```cpp
#include <cstddef>
#include <initializer_list>

template <typename T, typename... Ts>
struct InferRepeatedTypeHelper {
  // In the interest of keeping this example simple, just assume every typename
  // in the pack matches the first one.
  using Result = T;
};

template <typename... Ts>
using InferRepeatedType = typename InferRepeatedTypeHelper<Ts...>::Result;

template <typename T, std::size_t>
class Foo {
 public:
  explicit Foo(std::initializer_list<std::initializer_list<T>> list_of_lists) {}
};

// This deduction guide unpacks the top-level initializer_list to count its
// elements, uses the result for CTAD, and then lets the real constructor
// operate on the original, top-level initializer_list while knowing its size.
// Note: Checking sizeof...(Ts) instead of sizeof...(init_lists) in the
// deduction guide prevents the crash.
template <typename... Ts>
explicit Foo(std::initializer_list<Ts>... lists)
    -> Foo<InferRepeatedType<Ts...>, sizeof...(lists)>;

int main(int argc, char* argv[]) {
  Foo foo{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  return 0;
}
```

### Reduced failing example
```cpp
#include <cstddef>
#include <initializer_list>

template <typename T, std::size_t>
class Foo {
 public:
  Foo(std::initializer_list<std::initializer_list<T>>) {}
};

// Note: Checking sizeof...(Ts) instead of sizeof...(init_lists) in the
// deduction guide prevents the crash.
template <typename... Ts>
Foo(std::initializer_list<Ts>... init_lists) -> Foo<int, sizeof...(init_lists)>;

int main() {
  Foo{{2}};
  return 0;
}
```

## Expected behavior
I believe the expected behavior is for the attached code to compile. (Note that `g++` is happy with it.) It's possible that I'm wrong and it's not technically standard-compliant—it uses deduction guides in a somewhat odd manner that I'm not certain is allowed—but if that's the case, Clang should give a compilation error message instead of crashing with a segfault.

## Observed behavior
Clang crashes due to a segmentation fault.

If the explicit deduction guide is modified to check `sizeof...(Ts)` instead of `sizeof...(lists)` (where `lists` is a function parameter pack declared as `std::initializer_list<Ts>... lists`), the segmentation fault no longer occurs.

## Preprocessed sources and run scripts
Note that these files are associated with the reduced version of the failing example.

```cpp
# 1 "failing_example_reduced.cc"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 447 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "failing_example_reduced.cc" 2
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 1 3
# 43 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 3






# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 1 3
# 256 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
namespace std
{
  typedef long unsigned int size_t;
  typedef long int ptrdiff_t;


  typedef decltype(nullptr) nullptr_t;

}
# 278 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
# 528 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 1 3
# 39 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 3
# 1 "/usr/include/features.h" 1 3 4
# 439 "/usr/include/features.h" 3 4
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 440 "/usr/include/features.h" 2 3 4
# 461 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 452 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 453 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 454 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 462 "/usr/include/features.h" 2 3 4
# 485 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 486 "/usr/include/features.h" 2 3 4
# 40 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 2 3
# 529 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3


# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h" 1 3
# 532 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3
# 694 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/pstl/pstl_config.h" 1 3
# 695 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3
# 50 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 2 3
# 1 "/usr/lib/clang/16/include/stddef.h" 1 3
# 35 "/usr/lib/clang/16/include/stddef.h" 3
typedef long int ptrdiff_t;
# 46 "/usr/lib/clang/16/include/stddef.h" 3
typedef long unsigned int size_t;
# 60 "/usr/lib/clang/16/include/stddef.h" 3
typedef long unsigned int rsize_t;
# 109 "/usr/lib/clang/16/include/stddef.h" 3
# 1 "/usr/lib/clang/16/include/__stddef_max_align_t.h" 1 3
# 19 "/usr/lib/clang/16/include/__stddef_max_align_t.h" 3
typedef struct {
  long long __clang_max_align_nonce1
      __attribute__((__aligned__(__alignof__(long long))));
  long double __clang_max_align_nonce2
      __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
# 110 "/usr/lib/clang/16/include/stddef.h" 2 3
# 51 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 2 3

extern "C++"
{

namespace std
{

  using ::max_align_t;
}



namespace std
{




  enum class byte : unsigned char {};

  template<typename _IntegerType> struct __byte_operand { };
  template<> struct __byte_operand<bool> { using __type = byte; };
  template<> struct __byte_operand<char> { using __type = byte; };
  template<> struct __byte_operand<signed char> { using __type = byte; };
  template<> struct __byte_operand<unsigned char> { using __type = byte; };

  template<> struct __byte_operand<wchar_t> { using __type = byte; };


  template<> struct __byte_operand<char8_t> { using __type = byte; };

  template<> struct __byte_operand<char16_t> { using __type = byte; };
  template<> struct __byte_operand<char32_t> { using __type = byte; };
  template<> struct __byte_operand<short> { using __type = byte; };
  template<> struct __byte_operand<unsigned short> { using __type = byte; };
  template<> struct __byte_operand<int> { using __type = byte; };
  template<> struct __byte_operand<unsigned int> { using __type = byte; };
  template<> struct __byte_operand<long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long> { using __type = byte; };
  template<> struct __byte_operand<long long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long long> { using __type = byte; };
# 110 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 3
  template<typename _IntegerType>
    struct __byte_operand<const _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<volatile _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<const volatile _IntegerType>
    : __byte_operand<_IntegerType> { };

  template<typename _IntegerType>
    using __byte_op_t = typename __byte_operand<_IntegerType>::__type;

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>&
    operator<<=(byte& __b, _IntegerType __shift) noexcept
    { return __b = byte(static_cast<unsigned char>(__b) << __shift); }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>
    operator<<(byte __b, _IntegerType __shift) noexcept
    { return byte(static_cast<unsigned char>(__b) << __shift); }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>&
    operator>>=(byte& __b, _IntegerType __shift) noexcept
    { return __b = byte(static_cast<unsigned char>(__b) >> __shift); }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>
    operator>>(byte __b, _IntegerType __shift) noexcept
    { return byte(static_cast<unsigned char>(__b) >> __shift); }

  constexpr byte&
  operator|=(byte& __l, byte __r) noexcept
  {
    return __l =
      byte(static_cast<unsigned char>(__l) | static_cast<unsigned char>(__r));
  }

  constexpr byte
  operator|(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast<unsigned char>(__l) | static_cast<unsigned char>(__r));
  }

  constexpr byte&
  operator&=(byte& __l, byte __r) noexcept
  {
   return __l =
     byte(static_cast<unsigned char>(__l) & static_cast<unsigned char>(__r));
  }

  constexpr byte
  operator&(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast<unsigned char>(__l) & static_cast<unsigned char>(__r));
  }

  constexpr byte&
  operator^=(byte& __l, byte __r) noexcept
  {
    return __l =
      byte(static_cast<unsigned char>(__l) ^ static_cast<unsigned char>(__r));
  }

  constexpr byte
  operator^(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast<unsigned char>(__l) ^ static_cast<unsigned char>(__r));
  }

  constexpr byte
  operator~(byte __b) noexcept
  { return byte(~static_cast<unsigned char>(__b)); }

  template<typename _IntegerType>
    constexpr _IntegerType
    to_integer(__byte_op_t<_IntegerType> __b) noexcept
    { return _IntegerType(__b); }


}

}
# 2 "failing_example_reduced.cc" 2
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/initializer_list" 1 3
# 34 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/initializer_list" 3





#pragma GCC visibility push(default)



namespace std
{

  template<class _E>
    class initializer_list
    {
    public:
      typedef _E value_type;
      typedef const _E& reference;
      typedef const _E& const_reference;
      typedef size_t size_type;
      typedef const _E* iterator;
      typedef const _E* const_iterator;

    private:
      iterator _M_array;
      size_type _M_len;


      constexpr initializer_list(const_iterator __a, size_type __l)
      : _M_array(__a), _M_len(__l) { }

    public:
      constexpr initializer_list() noexcept
      : _M_array(0), _M_len(0) { }


      constexpr size_type
      size() const noexcept { return _M_len; }


      constexpr const_iterator
      begin() const noexcept { return _M_array; }


      constexpr const_iterator
      end() const noexcept { return begin() + size(); }
    };






  template<class _Tp>
    constexpr const _Tp*
    begin(initializer_list<_Tp> __ils) noexcept
    { return __ils.begin(); }






  template<class _Tp>
    constexpr const _Tp*
    end(initializer_list<_Tp> __ils) noexcept
    { return __ils.end(); }
}

#pragma GCC visibility pop
# 3 "failing_example_reduced.cc" 2

template <typename T, std::size_t>
class Foo {
 public:
  Foo(std::initializer_list<std::initializer_list<T>>) {}
};



template <typename... Ts>
Foo(std::initializer_list<Ts>... init_lists) -> Foo<int, sizeof...(init_lists)>;

int main() {
  Foo{{2}};
  return 0;
}
```

```sh
# Crash reproducer for clang version 16.0.0
# Driver args: "--driver-mode=g++" "--std=c++20" "failing_example_reduced.cc"
# Original command:  "/usr/bin/clang-16" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "failing_example_reduced.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-resource-dir" "/usr/lib/clang/16" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward" "-internal-isystem" "/usr/lib/clang/16/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/failing_example_reduced-7f404f.o" "-x" "c++" "failing_example_reduced.cc"
 "/usr/bin/clang-16" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "failing_example_reduced.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "failing_example_reduced-763f77.cpp"
```


## Comments

### Comment 1 - EugeneZelenko

Could you please try 18 or `main` branch? https://godbolt.org should be helpful.


---

### Comment 2 - mrguenther

Does Godbolt's `x86-64 clang (trunk)` track `main` or the latest release version? Both `x86-64 clang (trunk)` and `x86-64 clang 17.0.1` are still failing. Here's the output from the former:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:16:11: current parser token ';'
2.	<source>:15:12: parsing function body 'main'
3.	<source>:15:12: in compound statement ('{}')
 #0 0x00000000035f3908 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35f3908)
 #1 0x00000000035f1a54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35f1a54)
 #2 0x000000000353eec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f396f042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000626d667 rebuildPotentialResultsAsNonOdrUsed(clang::Sema&, clang::Expr*, clang::NonOdrUseReason) SemaExpr.cpp:0:0
 #5 0x000000000626e434 clang::Sema::CheckLValueToRValueConversionOperand(clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626e434)
 #6 0x000000000626e4db clang::Sema::ActOnConstantExpression(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626e4db)
 #7 0x00000000067f9536 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #8 0x00000000067facb9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSizeOfPackExpr(clang::SizeOfPackExpr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x00000000067e2d37 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#10 0x00000000067f952a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000067fca98 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000067f0533 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000068090b0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000067eefd1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006810a8e clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6810a8e)
#16 0x000000000681feb3 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x681feb3)
#17 0x000000000686bf1e clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x686bf1e)
#18 0x0000000006876b4f void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#19 0x0000000005e245c1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e245c1)
#20 0x000000000681fd3a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x681fd3a)
#21 0x00000000067a26f1 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#22 0x00000000067a356a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#23 0x0000000005e245c1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e245c1)
#24 0x00000000067b90bf clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67b90bf)
#25 0x00000000065c817d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65c817d)
#26 0x000000000643aba5 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#27 0x000000000643b4ea clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x643b4ea)
#28 0x0000000006343503 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6343503)
#29 0x00000000063ca77d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63ca77d)
#30 0x0000000005d481c5 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d481c5)
#31 0x0000000005d36824 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d36824)
#32 0x0000000005d384ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d384ca)
#33 0x0000000005d38669 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d38669)
#34 0x0000000005d3daf9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d3daf9)
#35 0x0000000005dc3ab7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dc3ab7)
#36 0x0000000005db9596 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5db9596)
#37 0x0000000005db9ee8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5db9ee8)
#38 0x0000000005dbacf9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dbacf9)
#39 0x0000000005dbcf5a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dbcf5a)
#40 0x0000000005cc655a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cc655a)
#41 0x0000000005d08c06 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d08c06)
#42 0x0000000005cbfb8d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cbfb8d)
#43 0x0000000005cc0731 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cc0731)
#44 0x0000000005cc8b32 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cc8b32)
#45 0x0000000005cca697 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cca697)
#46 0x0000000005cb9a8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cb9a8a)
#47 0x0000000003e3bfe5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e3bfe5)
#48 0x00000000040c6ce1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40c6ce1)
#49 0x00000000040430eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40430eb)
#50 0x00000000041a8983 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41a8983)
#51 0x0000000000c1fc46 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc1fc46)
#52 0x0000000000c1859d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#53 0x0000000003e835d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#54 0x000000000353f2f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x353f2f3)
#55 0x0000000003e837f9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#56 0x0000000003e4b5d7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e4b5d7)
#57 0x0000000003e4bf8d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e4bf8d)
#58 0x0000000003e55cec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e55cec)
#59 0x0000000000c1cec1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc1cec1)
#60 0x0000000000b14a64 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xb14a64)
#61 0x00007f396f029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#62 0x00007f396f029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#63 0x0000000000c17d9e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc17d9e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matthias Guenther (mrguenther)

<details>
# Clang frontend segfaults during CTAD with paramer packs
## Reproduction steps
Compile the attached code with `clang++` version `16.0.0` and the `--std=c++20` flag. The compiler will crash with a segmentation fault.

Note that the attached code snippets mention a simple change that will prevent the crash.

Also note that `g++` version `9.4.0` compiles the code successfully (though that compiler version needs `--std=c++2a` instead of `--std=c++20`).

The full version information for the compilers I used is below:
```
$ clang++ --version
clang version 16.0.0
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

I don't recall for sure where I got my installed copy of `clang-16`, but I *think* I got it from the official LLVM repo.

In case it's relevant, I was running Clang in a WSL2 environment.

### Failing example
```cpp
#include &lt;cstddef&gt;
#include &lt;initializer_list&gt;

template &lt;typename T, typename... Ts&gt;
struct InferRepeatedTypeHelper {
  // In the interest of keeping this example simple, just assume every typename
  // in the pack matches the first one.
  using Result = T;
};

template &lt;typename... Ts&gt;
using InferRepeatedType = typename InferRepeatedTypeHelper&lt;Ts...&gt;::Result;

template &lt;typename T, std::size_t&gt;
class Foo {
 public:
  explicit Foo(std::initializer_list&lt;std::initializer_list&lt;T&gt;&gt; list_of_lists) {}
};

// This deduction guide unpacks the top-level initializer_list to count its
// elements, uses the result for CTAD, and then lets the real constructor
// operate on the original, top-level initializer_list while knowing its size.
// Note: Checking sizeof...(Ts) instead of sizeof...(init_lists) in the
// deduction guide prevents the crash.
template &lt;typename... Ts&gt;
explicit Foo(std::initializer_list&lt;Ts&gt;... lists)
    -&gt; Foo&lt;InferRepeatedType&lt;Ts...&gt;, sizeof...(lists)&gt;;

int main(int argc, char* argv[]) {
  Foo foo{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  return 0;
}
```

### Reduced failing example
```cpp
#include &lt;cstddef&gt;
#include &lt;initializer_list&gt;

template &lt;typename T, std::size_t&gt;
class Foo {
 public:
  Foo(std::initializer_list&lt;std::initializer_list&lt;T&gt;&gt;) {}
};

// Note: Checking sizeof...(Ts) instead of sizeof...(init_lists) in the
// deduction guide prevents the crash.
template &lt;typename... Ts&gt;
Foo(std::initializer_list&lt;Ts&gt;... init_lists) -&gt; Foo&lt;int, sizeof...(init_lists)&gt;;

int main() {
  Foo{{2}};
  return 0;
}
```

## Expected behavior
I believe the expected behavior is for the attached code to compile. (Note that `g++` is happy with it.) It's possible that I'm wrong and it's not technically standard-compliant—it uses deduction guides in a somewhat odd manner that I'm not certain is allowed—but if that's the case, Clang should give a compilation error message instead of crashing with a segfault.

## Observed behavior
Clang crashes due to a segmentation fault.

If the explicit deduction guide is modified to check `sizeof...(Ts)` instead of `sizeof...(lists)` (where `lists` is a function parameter pack declared as `std::initializer_list&lt;Ts&gt;... lists`), the segmentation fault no longer occurs.

## Preprocessed sources and run scripts
Note that these files are associated with the reduced version of the failing example.

```cpp
# 1 "failing_example_reduced.cc"
# 1 "&lt;built-in&gt;" 1
# 1 "&lt;built-in&gt;" 3
# 447 "&lt;built-in&gt;" 3
# 1 "&lt;command line&gt;" 1
# 1 "&lt;built-in&gt;" 2
# 1 "failing_example_reduced.cc" 2
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 1 3
# 43 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 3






# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 1 3
# 256 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
namespace std
{
  typedef long unsigned int size_t;
  typedef long int ptrdiff_t;


  typedef decltype(nullptr) nullptr_t;

}
# 278 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
# 528 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 1 3
# 39 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 3
# 1 "/usr/include/features.h" 1 3 4
# 439 "/usr/include/features.h" 3 4
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 440 "/usr/include/features.h" 2 3 4
# 461 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 452 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 453 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 454 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 462 "/usr/include/features.h" 2 3 4
# 485 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 486 "/usr/include/features.h" 2 3 4
# 40 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/os_defines.h" 2 3
# 529 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3


# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h" 1 3
# 532 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3
# 694 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 3
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/pstl/pstl_config.h" 1 3
# 695 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9/bits/c++config.h" 2 3
# 50 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 2 3
# 1 "/usr/lib/clang/16/include/stddef.h" 1 3
# 35 "/usr/lib/clang/16/include/stddef.h" 3
typedef long int ptrdiff_t;
# 46 "/usr/lib/clang/16/include/stddef.h" 3
typedef long unsigned int size_t;
# 60 "/usr/lib/clang/16/include/stddef.h" 3
typedef long unsigned int rsize_t;
# 109 "/usr/lib/clang/16/include/stddef.h" 3
# 1 "/usr/lib/clang/16/include/__stddef_max_align_t.h" 1 3
# 19 "/usr/lib/clang/16/include/__stddef_max_align_t.h" 3
typedef struct {
  long long __clang_max_align_nonce1
      __attribute__((__aligned__(__alignof__(long long))));
  long double __clang_max_align_nonce2
      __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
# 110 "/usr/lib/clang/16/include/stddef.h" 2 3
# 51 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 2 3

extern "C++"
{

namespace std
{

  using ::max_align_t;
}



namespace std
{




  enum class byte : unsigned char {};

  template&lt;typename _IntegerType&gt; struct __byte_operand { };
  template&lt;&gt; struct __byte_operand&lt;bool&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;char&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;signed char&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;unsigned char&gt; { using __type = byte; };

  template&lt;&gt; struct __byte_operand&lt;wchar_t&gt; { using __type = byte; };


  template&lt;&gt; struct __byte_operand&lt;char8_t&gt; { using __type = byte; };

  template&lt;&gt; struct __byte_operand&lt;char16_t&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;char32_t&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;short&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;unsigned short&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;int&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;unsigned int&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;long&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;unsigned long&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;long long&gt; { using __type = byte; };
  template&lt;&gt; struct __byte_operand&lt;unsigned long long&gt; { using __type = byte; };
# 110 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/cstddef" 3
  template&lt;typename _IntegerType&gt;
    struct __byte_operand&lt;const _IntegerType&gt;
    : __byte_operand&lt;_IntegerType&gt; { };
  template&lt;typename _IntegerType&gt;
    struct __byte_operand&lt;volatile _IntegerType&gt;
    : __byte_operand&lt;_IntegerType&gt; { };
  template&lt;typename _IntegerType&gt;
    struct __byte_operand&lt;const volatile _IntegerType&gt;
    : __byte_operand&lt;_IntegerType&gt; { };

  template&lt;typename _IntegerType&gt;
    using __byte_op_t = typename __byte_operand&lt;_IntegerType&gt;::__type;

  template&lt;typename _IntegerType&gt;
    constexpr __byte_op_t&lt;_IntegerType&gt;&amp;
    operator&lt;&lt;=(byte&amp; __b, _IntegerType __shift) noexcept
    { return __b = byte(static_cast&lt;unsigned char&gt;(__b) &lt;&lt; __shift); }

  template&lt;typename _IntegerType&gt;
    constexpr __byte_op_t&lt;_IntegerType&gt;
    operator&lt;&lt;(byte __b, _IntegerType __shift) noexcept
    { return byte(static_cast&lt;unsigned char&gt;(__b) &lt;&lt; __shift); }

  template&lt;typename _IntegerType&gt;
    constexpr __byte_op_t&lt;_IntegerType&gt;&amp;
    operator&gt;&gt;=(byte&amp; __b, _IntegerType __shift) noexcept
    { return __b = byte(static_cast&lt;unsigned char&gt;(__b) &gt;&gt; __shift); }

  template&lt;typename _IntegerType&gt;
    constexpr __byte_op_t&lt;_IntegerType&gt;
    operator&gt;&gt;(byte __b, _IntegerType __shift) noexcept
    { return byte(static_cast&lt;unsigned char&gt;(__b) &gt;&gt; __shift); }

  constexpr byte&amp;
  operator|=(byte&amp; __l, byte __r) noexcept
  {
    return __l =
      byte(static_cast&lt;unsigned char&gt;(__l) | static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte
  operator|(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast&lt;unsigned char&gt;(__l) | static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte&amp;
  operator&amp;=(byte&amp; __l, byte __r) noexcept
  {
   return __l =
     byte(static_cast&lt;unsigned char&gt;(__l) &amp; static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte
  operator&amp;(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast&lt;unsigned char&gt;(__l) &amp; static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte&amp;
  operator^=(byte&amp; __l, byte __r) noexcept
  {
    return __l =
      byte(static_cast&lt;unsigned char&gt;(__l) ^ static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte
  operator^(byte __l, byte __r) noexcept
  {
    return
      byte(static_cast&lt;unsigned char&gt;(__l) ^ static_cast&lt;unsigned char&gt;(__r));
  }

  constexpr byte
  operator~(byte __b) noexcept
  { return byte(~static_cast&lt;unsigned char&gt;(__b)); }

  template&lt;typename _IntegerType&gt;
    constexpr _IntegerType
    to_integer(__byte_op_t&lt;_IntegerType&gt; __b) noexcept
    { return _IntegerType(__b); }


}

}
# 2 "failing_example_reduced.cc" 2
# 1 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/initializer_list" 1 3
# 34 "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/initializer_list" 3





#pragma GCC visibility push(default)



namespace std
{

  template&lt;class _E&gt;
    class initializer_list
    {
    public:
      typedef _E value_type;
      typedef const _E&amp; reference;
      typedef const _E&amp; const_reference;
      typedef size_t size_type;
      typedef const _E* iterator;
      typedef const _E* const_iterator;

    private:
      iterator _M_array;
      size_type _M_len;


      constexpr initializer_list(const_iterator __a, size_type __l)
      : _M_array(__a), _M_len(__l) { }

    public:
      constexpr initializer_list() noexcept
      : _M_array(0), _M_len(0) { }


      constexpr size_type
      size() const noexcept { return _M_len; }


      constexpr const_iterator
      begin() const noexcept { return _M_array; }


      constexpr const_iterator
      end() const noexcept { return begin() + size(); }
    };






  template&lt;class _Tp&gt;
    constexpr const _Tp*
    begin(initializer_list&lt;_Tp&gt; __ils) noexcept
    { return __ils.begin(); }






  template&lt;class _Tp&gt;
    constexpr const _Tp*
    end(initializer_list&lt;_Tp&gt; __ils) noexcept
    { return __ils.end(); }
}

#pragma GCC visibility pop
# 3 "failing_example_reduced.cc" 2

template &lt;typename T, std::size_t&gt;
class Foo {
 public:
  Foo(std::initializer_list&lt;std::initializer_list&lt;T&gt;&gt;) {}
};



template &lt;typename... Ts&gt;
Foo(std::initializer_list&lt;Ts&gt;... init_lists) -&gt; Foo&lt;int, sizeof...(init_lists)&gt;;

int main() {
  Foo{{2}};
  return 0;
}
```

```sh
# Crash reproducer for clang version 16.0.0
# Driver args: "--driver-mode=g++" "--std=c++20" "failing_example_reduced.cc"
# Original command:  "/usr/bin/clang-16" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "failing_example_reduced.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-resource-dir" "/usr/lib/clang/16" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward" "-internal-isystem" "/usr/lib/clang/16/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/failing_example_reduced-7f404f.o" "-x" "c++" "failing_example_reduced.cc"
 "/usr/bin/clang-16" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "failing_example_reduced.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/mrguenther/win_profile/src/mrg/clang_bug_report" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "failing_example_reduced-763f77.cpp"
```

</details>


---

### Comment 4 - shafik

Greatly reduced: https://godbolt.org/z/hTEn43oPx

```cpp
namespace std {
  template<class _E>
    class initializer_list{};

}

template <typename T, unsigned>
struct Foo {
  Foo(std::initializer_list<std::initializer_list<T>>) {}
};


template <typename... Ts>
Foo(std::initializer_list<Ts>... init_lists) -> Foo<int, sizeof...(init_lists)>;

void f() {
  Foo{{2}};
}
```

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclCXX.h:464: 
clang::CXXRecordDecl::DefinitionData& clang::CXXRecordDecl::data() const: 
Assertion `DD && "queried property of class with no definition"' failed.
```

---

### Comment 5 - hokein

Reduced a bit more:

```
namespace std {
template <class _E>
class initializer_list {};

}  // namespace std

template <int>
struct Foo {};

template <typename... Ts>
auto func(std::initializer_list<Ts>... init_lists)
    -> Foo<sizeof...(init_lists)> {}

void f() {
    func({2});
}
```

It is not CTAD-specific, the crash occurs within the processing of `TransformSizeOfPackExpr` for the trailing return type (`Foo<sizeof...(init_lists)>`), the `std::initializer_list<int>` decl is not instantiated at this point, but the code assumes it was.



---

### Comment 6 - jcsxky

Maybe we need to check if the `CXXRecordDecl` has a definition by `RD->hasDefinition()`

https://github.com/llvm/llvm-project/blob/2a67c28abe8cfde47c5058abbeb4b5ff9a393192/clang/lib/Sema/SemaExpr.cpp#L20129


---

### Comment 7 - chris0e3

Some further data points.  I tried this on Compiler Explorer with clang++ 11.0 through 18.1 & trunk (x86_64 & ARM-64).
Options were `-std=c++20 -O1 -Wall`  (but any -O# or c++2# behaves the same).  GCC had no problem.
When compiled with clang + assertion (trunk), this produces an assertion identical to the one shown above.

This crashes clang:
```C++
#include <initializer_list>

void func(auto ...vs) requires (sizeof...(vs) > 1)
{
    func(std::initializer_list<int>{vs...});
}
```

This compiles:
```C++
#include <initializer_list>

void func(auto ...vs) requires (sizeof...(vs) > 1)
{
    func(std::initializer_list{vs...});
}
```

This compiles (also with `#include <initializer_list>`):
```C++
void func(auto ...vs) requires (sizeof...(vs) > 1)
{
    func({vs...});
}
```

This compiles:
```C++
#include <initializer_list>

template<typename ...Ts> concept more_args = (sizeof...(Ts) > 1);

void func(auto) {}
void func(auto ...vs) requires more_args<decltype(vs)...>
{
    func(std::initializer_list<int>{vs...});
}
```


---

