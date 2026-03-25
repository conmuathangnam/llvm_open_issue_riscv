# Assertion failure in SemaTemplateInstantiate.cpp

**Author:** ccotter
**URL:** https://github.com/llvm/llvm-project/issues/177734

## Body

The following program hits https://github.com/llvm/llvm-project/blob/adc64c6e1745a14896efa48d23fc541e4efe5a53/clang/lib/Sema/SemaTemplateInstantiate.cpp#L4623 in a build with assertions enabled. The release build does not trigger the assert, and the program appears to compile fine otherwise. The assert happens on clang-19 and HEAD as of submitting this issue.

```c++
template<typename T>
struct result { using type = int; };

template<typename T>
using call_result_t = typename result<T>::type;

template<typename EnvProvider>
using env_of_t = call_result_t<EnvProvider>;

template<typename Env>
using env_t = call_result_t<Env>;

template<typename S, typename E>
concept my_concept = requires { typename env_t<E>; };

template<typename Receiver>
static auto mk_state_fn(const Receiver&) noexcept {
  using __env_of_t = env_of_t<Receiver>;
  return
    []<my_concept<env_t<__env_of_t>>... Child>() noexcept {
      return 0;
    };
}

void foo() {
  using F = decltype(mk_state_fn(0));
  auto lambda = mk_state_fn(0);
  auto result = lambda.template operator()<int>();
}
```

```
clang-21: /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4641: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::ValueDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /bb/data/ccotter/llvm-project/build/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name ice.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/bb/data/ccotter/llvm-project -fcoverage-compilation-dir=/bb/data/ccotter/llvm-project -resource-dir /bb/data/ccotter/llvm-project/build/lib/clang/23 -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13 -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/x86_64-redhat-linux -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/backward -internal-isystem /bb/data/ccotter/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -fdwarf2-cfi-asm -o /bb/data/tmp/ice-01cb72.o -x c++ ice.cpp
1.      ice.cpp:28:49: current parser token ')'
2.      ice.cpp:25:12: parsing function body 'foo'
3.      ice.cpp:25:12: in compound statement ('{}')

...

#8 0x00000000070528d3 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4641:3
 #9 0x00000000070528d3 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4612:1
#10 0x0000000007132e28 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6917:7
#11 0x00000000070c2f83 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypedefType(clang::TypeLocBuilder&, clang::TypedefTypeLoc) /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:6972:48
#12 0x00000000070a728c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /bb/data/ccotter/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:83:1
#13 0x00000000070aa746 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (.constprop.0) /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:5445:47
#14 0x00000000070c1596 TransformTemplateArgument /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:5060:37
...
```

Full output: https://gist.github.com/ccotter/38fbaa0168bd9c2d02d8331b63ebd1fa

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Chris Cotter (ccotter)

<details>
The following program hits https://github.com/llvm/llvm-project/blob/adc64c6e1745a14896efa48d23fc541e4efe5a53/clang/lib/Sema/SemaTemplateInstantiate.cpp#L4623 in a build with assertions enabled. The release build does not trigger the assert, and the program appears to compile fine otherwise. The assert happens on clang-19 and HEAD as of submitting this issue.

```
template&lt;typename T&gt;
struct result { using type = int; };

template&lt;typename T&gt;
using call_result_t = typename result&lt;T&gt;::type;

template&lt;typename EnvProvider&gt;
using env_of_t = call_result_t&lt;EnvProvider&gt;;

template&lt;typename Env&gt;
using env_t = call_result_t&lt;Env&gt;;

template&lt;typename S, typename E&gt;
concept my_concept = requires { typename env_t&lt;E&gt;; };

template&lt;typename Receiver&gt;
static auto mk_state_fn(const Receiver&amp;) noexcept {
  using __env_of_t = env_of_t&lt;Receiver&gt;;
  return
    []&lt;my_concept&lt;env_t&lt;__env_of_t&gt;&gt;... Child&gt;() noexcept {
      return 0;
    };
}

void foo() {
  using F = decltype(mk_state_fn(0));
  auto lambda = mk_state_fn(0);
  auto result = lambda.template operator()&lt;int&gt;();
}
```

```
clang-21: /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4641: llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::ValueDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /bb/data/ccotter/llvm-project/build/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name ice.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/bb/data/ccotter/llvm-project -fcoverage-compilation-dir=/bb/data/ccotter/llvm-project -resource-dir /bb/data/ccotter/llvm-project/build/lib/clang/23 -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13 -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/x86_64-redhat-linux -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/backward -internal-isystem /bb/data/ccotter/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -fdwarf2-cfi-asm -o /bb/data/tmp/ice-01cb72.o -x c++ ice.cpp
1.      ice.cpp:28:49: current parser token ')'
2.      ice.cpp:25:12: parsing function body 'foo'
3.      ice.cpp:25:12: in compound statement ('{}')

...

#<!-- -->8 0x00000000070528d3 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4641:3
 #<!-- -->9 0x00000000070528d3 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4612:1
#<!-- -->10 0x0000000007132e28 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) /bb/data/ccotter/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6917:7
#<!-- -->11 0x00000000070c2f83 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTypedefType(clang::TypeLocBuilder&amp;, clang::TypedefTypeLoc) /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:6972:48
#<!-- -->12 0x00000000070a728c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /bb/data/ccotter/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:83:1
#<!-- -->13 0x00000000070aa746 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (.constprop.0) /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:5445:47
#<!-- -->14 0x00000000070c1596 TransformTemplateArgument /bb/data/ccotter/llvm-project/clang/lib/Sema/TreeTransform.h:5060:37
...
```

Full output: https://gist.github.com/ccotter/38fbaa0168bd9c2d02d8331b63ebd1fa
</details>


---

