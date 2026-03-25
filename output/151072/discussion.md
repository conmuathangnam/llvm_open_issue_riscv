# [Clang] Segmentation fault in CXXNameMangler::mangleName due to infinite recursion

**Author:** Xniao
**URL:** https://github.com/llvm/llvm-project/issues/151072

## Body


## Description
When compiling the following test case, Clang 20.1.8 crashes with a segmentation fault.
Here is the godbolt link: https://godbolt.org/z/Ee4rcYExj

## Clang Version
```bash
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/build/bin/llvm/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Candidate multilib: .;@m64
Selected multilib: .;@m64
```


## Stack Dump
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/build/bin/llvm/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/cppfuzz -fcoverage-compilation-dir=/home/cppfuzz -resource-dir /home/build/bin/llvm/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /home/build/bin/llvm/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include --std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-4b1a75.o -x c++ /home/test.cpp
1.      <eof> parser at end of file
2.      /home/test.cpp:4:25: instantiating function definition 'tmain<int>'
  #0 0x000055e420763d40 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/build/bin/llvm/bin/clang-20+0x3e27d40)
  #1 0x000055e420761581 SignalHandler(int) Signals.cpp:0:0
  #2 0x00007f3ed70d4520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x000055e423ee4f9a (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #4 0x000055e423ef39bd (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #5 0x000055e423ee477a (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #6 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #7 0x000055e423ef39bd (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #8 0x000055e423ee477a (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #9 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #10 0x000055e423ee6776 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #11 0x000055e423efb200 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #12 0x000055e423ef8840 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) ItaniumMangle.cpp:0:0
 #13 0x000055e423ee4f11 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #14 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
...
#252 0x000055e423ef8840 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) ItaniumMangle.cpp:0:0
#253 0x000055e423ee4f11 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#254 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#255 0x000055e423ee6776 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/build/bin/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-a1a98e.cpp
clang++: note: diagnostic msg: /tmp/test-a1a98e.sh
clang++: note: diagnostic msg: 

********************
```
Here is the entire stack dump.
[dump.log](https://github.com/user-attachments/files/21479105/dump.log)

## Program
```cpp
#include <functional>
#include <memory>

template <typename T> T tmain() {
    while (true) {
      const int max_depth = 1000;
      int count = 0;
      std::function<void(int &, const std::shared_ptr<class B> &)>
          a_method =
              [&](int &count,
                  const std::shared_ptr<class B> &b_instance) {
                if (count >= max_depth)
                  return;
                ++count;
              };
    }
  return T();
}

int main() {
  return tmain<int>();
}
```

## Command 
```bash
clang++ test.cpp
```

## Comments

### Comment 1 - shafik

Likely: https://github.com/llvm/llvm-project/issues/63271

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jireh Guo (Xniao)

<details>

## Description
When compiling the following test case, Clang 20.1.8 crashes with a segmentation fault.
Here is the godbolt link: https://godbolt.org/z/Ee4rcYExj

## Clang Version
```bash
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/build/bin/llvm/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```


## Stack Dump
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/build/bin/llvm/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/cppfuzz -fcoverage-compilation-dir=/home/cppfuzz -resource-dir /home/build/bin/llvm/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /home/build/bin/llvm/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include --std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-4b1a75.o -x c++ /home/test.cpp
1.      &lt;eof&gt; parser at end of file
2.      /home/test.cpp:4:25: instantiating function definition 'tmain&lt;int&gt;'
  #<!-- -->0 0x000055e420763d40 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/build/bin/llvm/bin/clang-20+0x3e27d40)
  #<!-- -->1 0x000055e420761581 SignalHandler(int) Signals.cpp:0:0
  #<!-- -->2 0x00007f3ed70d4520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->3 0x000055e423ee4f9a (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #<!-- -->4 0x000055e423ef39bd (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #<!-- -->5 0x000055e423ee477a (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
  #<!-- -->6 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #<!-- -->7 0x000055e423ef39bd (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #<!-- -->8 0x000055e423ee477a (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
  #<!-- -->9 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->10 0x000055e423ee6776 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #<!-- -->11 0x000055e423efb200 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #<!-- -->12 0x000055e423ef8840 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) ItaniumMangle.cpp:0:0
 #<!-- -->13 0x000055e423ee4f11 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
 #<!-- -->14 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
...
#<!-- -->252 0x000055e423ef8840 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) ItaniumMangle.cpp:0:0
#<!-- -->253 0x000055e423ee4f11 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
#<!-- -->254 0x000055e423ee5687 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->255 0x000055e423ee6776 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/build/bin/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-a1a98e.cpp
clang++: note: diagnostic msg: /tmp/test-a1a98e.sh
clang++: note: diagnostic msg: 

********************
```
Here is the entire stack dump.
[dump.log](https://github.com/user-attachments/files/21479105/dump.log)

## Program
```cpp
#include &lt;functional&gt;
#include &lt;memory&gt;

template &lt;typename T&gt; T tmain() {
    while (true) {
      const int max_depth = 1000;
      int count = 0;
      std::function&lt;void(int &amp;, const std::shared_ptr&lt;class B&gt; &amp;)&gt;
          a_method =
              [&amp;](int &amp;count,
                  const std::shared_ptr&lt;class B&gt; &amp;b_instance) {
                if (count &gt;= max_depth)
                  return;
                ++count;
              };
    }
  return T();
}

int main() {
  return tmain&lt;int&gt;();
}
```

## Command 
```bash
clang++ test.cpp
```
</details>


---

### Comment 3 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/6s6c7eenq)):
```C++
template <class>
struct foo_templ {};

template <class>
void test() {
    // class B;
    [[maybe_unused]] auto x = [](foo_templ<class B>) {};
}

template void test<int>(); // explicit instantiation
```

The error have been present since Clang 17. Clang can correctly accept it with the forward declaration of `B` added.


---

