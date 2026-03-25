# Clang Segmentation Fault When Instantiating a Template with Dynamic Array Initialization

**Author:** SOHAIBALOUSSI
**URL:** https://github.com/llvm/llvm-project/issues/123129
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash
**Closed Date:** 2025-01-15T23:21:06Z

## Body

this bug occurs when compiling C++ code with Clang 12.0.1 (Ubuntu 12.0.1-19ubuntu3):

A segfault is triggered during template instantiation when initializing a dynamic array in the constructor of a class template
```SQL
./Array.hpp:19:19: error: cannot determine allocated array size from initializer
        arr = new T[]();
                  ^
main.cpp:5:16: note: in instantiation of member function 'Array<int>::Array' requested here
    Array<int> arr(10);
               ^
```
the bug arises when attempting to allocate an array with the following constructor signature in Array.hpp:

```c++
template<typename T>
class Array {
public:
    Array(size_t size) {
        arr = new T[]();  // Error: Cannot determine array size
    }
};
```

the Clang compiler crashes with a segmentation fault, and the following stack dump is provided in the report

```
#0 0x00007fa27317e2a1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int)
...
#34 0x000000000040ea25 _start (/usr/lib/llvm-12/bin/clang+0x40ea25)
```

In file included from main.cpp:1:
./Array.hpp:19:19: error: cannot determine allocated array size from initializer
        arr = new T[]();
                  ^
main.cpp:5:16: note: in instantiation of member function 'Array<int>::Array' requested here
    Array<int> arr(10);
               ^
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-12/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -tune-cpu generic -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /usr/lib/llvm-12/lib/clang/12.0.1 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward -internal-isystem /usr/local/include -internal-isystem /usr/lib/llvm-12/lib/clang/12.0.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/sait-alo/Desktop/cpp/CPP07/ex02 -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/main-f44f29.o -x c++ main.cpp
1.      <eof> parser at end of file
2.      ./Array.hpp:18:5: instantiating function definition 'Array<int>::Array'
 #0 0x00007f094daa02a1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd732a1)
 #1 0x00007f094da9e3d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd713d0)
 #2 0x00007f094daa09ad (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd739ad)
 #3 0x00007f094c813520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f09534e4ff8 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc36ff8)
 #5 0x00007f09534d2f70 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc24f70)
 #6 0x00007f09534d0a4e (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc22a4e)
 #7 0x00007f09534c153b (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1353b)
 #8 0x00007f09534c406a (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1606a)
 #9 0x00007f09534c138b (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1338b)
#10 0x00007f09534bf517 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc11517)
#11 0x00007f09534bf655 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc11655)
#12 0x00007f09538f646d clang::Sema::CheckForIntOverflow(clang::Expr*) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x104846d)
#13 0x00007f09538f6b19 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1048b19)
#14 0x00007f0953b6e9a9 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x12c09a9)
#15 0x00007f0953cfd981 clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x144f981)
#16 0x00007f0953e1cbc0 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x156ebc0)
#17 0x00007f0953e0601c clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x155801c)
#18 0x00007f0953e3fc78 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1591c78)
#19 0x00007f0953e422cd clang::Sema::PerformPendingInstantiations(bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x15942cd)
#20 0x00007f0953864fd3 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xfb6fd3)
#21 0x00007f09538656a6 clang::Sema::ActOnEndOfTranslationUnit() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xfb76a6)
#22 0x00007f09532d52ac clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xa272ac)
#23 0x00007f095321c74d clang::ParseAST(clang::Sema&, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x96e74d)
#24 0x00007f095412b393 clang::CodeGenAction::ExecuteAction() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x187d393)
#25 0x00007f0954866006 clang::FrontendAction::Execute() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1fb8006)
#26 0x00007f09547ed701 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1f3f701)
#27 0x00007f09548cdcb4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x201fcb4)
#28 0x0000000000413799 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-12/bin/clang+0x413799)
#29 0x0000000000411a6f (/usr/lib/llvm-12/bin/clang+0x411a6f)
#30 0x0000000000411866 main (/usr/lib/llvm-12/bin/clang+0x411866)
#31 0x00007f094c7fad90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007f094c7fae40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f094c7fae40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000000000040ea25 _start (/usr/lib/llvm-12/bin/clang+0x40ea25)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 12.0.1-19ubuntu3
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

[main-7097c7.zip](https://github.com/user-attachments/files/18430962/main-7097c7.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sohaib. (SOHAIBALOUSSI)

<details>
this bug occurs when compiling C++ code with Clang 12.0.1 (Ubuntu 12.0.1-19ubuntu3):

A segfault is triggered during template instantiation when initializing a dynamic array in the constructor of a class template
```SQL
./Array.hpp:19:19: error: cannot determine allocated array size from initializer
        arr = new T[]();
                  ^
main.cpp:5:16: note: in instantiation of member function 'Array&lt;int&gt;::Array' requested here
    Array&lt;int&gt; arr(10);
               ^
```
the bug arises when attempting to allocate an array with the following constructor signature in Array.hpp:

```c++
template&lt;typename T&gt;
class Array {
public:
    Array(size_t size) {
        arr = new T[]();  // Error: Cannot determine array size
    }
};
```

the Clang compiler crashes with a segmentation fault, and the following stack dump is provided in the report

```
#<!-- -->0 0x00007fa27317e2a1 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int)
...
#<!-- -->34 0x000000000040ea25 _start (/usr/lib/llvm-12/bin/clang+0x40ea25)
```

In file included from main.cpp:1:
./Array.hpp:19:19: error: cannot determine allocated array size from initializer
        arr = new T[]();
                  ^
main.cpp:5:16: note: in instantiation of member function 'Array&lt;int&gt;::Array' requested here
    Array&lt;int&gt; arr(10);
               ^
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-12/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -tune-cpu generic -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /usr/lib/llvm-12/lib/clang/12.0.1 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward -internal-isystem /usr/local/include -internal-isystem /usr/lib/llvm-12/lib/clang/12.0.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/sait-alo/Desktop/cpp/CPP07/ex02 -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/main-f44f29.o -x c++ main.cpp
1.      &lt;eof&gt; parser at end of file
2.      ./Array.hpp:18:5: instantiating function definition 'Array&lt;int&gt;::Array'
 #<!-- -->0 0x00007f094daa02a1 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd732a1)
 #<!-- -->1 0x00007f094da9e3d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd713d0)
 #<!-- -->2 0x00007f094daa09ad (/lib/x86_64-linux-gnu/libLLVM-12.so.1+0xd739ad)
 #<!-- -->3 0x00007f094c813520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f09534e4ff8 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc36ff8)
 #<!-- -->5 0x00007f09534d2f70 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc24f70)
 #<!-- -->6 0x00007f09534d0a4e (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc22a4e)
 #<!-- -->7 0x00007f09534c153b (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1353b)
 #<!-- -->8 0x00007f09534c406a (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1606a)
 #<!-- -->9 0x00007f09534c138b (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc1338b)
#<!-- -->10 0x00007f09534bf517 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc11517)
#<!-- -->11 0x00007f09534bf655 clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xc11655)
#<!-- -->12 0x00007f09538f646d clang::Sema::CheckForIntOverflow(clang::Expr*) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x104846d)
#<!-- -->13 0x00007f09538f6b19 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1048b19)
#<!-- -->14 0x00007f0953b6e9a9 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x12c09a9)
#<!-- -->15 0x00007f0953cfd981 clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x144f981)
#<!-- -->16 0x00007f0953e1cbc0 (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x156ebc0)
#<!-- -->17 0x00007f0953e0601c clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x155801c)
#<!-- -->18 0x00007f0953e3fc78 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1591c78)
#<!-- -->19 0x00007f0953e422cd clang::Sema::PerformPendingInstantiations(bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x15942cd)
#<!-- -->20 0x00007f0953864fd3 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xfb6fd3)
#<!-- -->21 0x00007f09538656a6 clang::Sema::ActOnEndOfTranslationUnit() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xfb76a6)
#<!-- -->22 0x00007f09532d52ac clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0xa272ac)
#<!-- -->23 0x00007f095321c74d clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x96e74d)
#<!-- -->24 0x00007f095412b393 clang::CodeGenAction::ExecuteAction() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x187d393)
#<!-- -->25 0x00007f0954866006 clang::FrontendAction::Execute() (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1fb8006)
#<!-- -->26 0x00007f09547ed701 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x1f3f701)
#<!-- -->27 0x00007f09548cdcb4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib/x86_64-linux-gnu/libclang-cpp.so.12+0x201fcb4)
#<!-- -->28 0x0000000000413799 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-12/bin/clang+0x413799)
#<!-- -->29 0x0000000000411a6f (/usr/lib/llvm-12/bin/clang+0x411a6f)
#<!-- -->30 0x0000000000411866 main (/usr/lib/llvm-12/bin/clang+0x411866)
#<!-- -->31 0x00007f094c7fad90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->32 0x00007f094c7fae40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007f094c7fae40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->34 0x000000000040ea25 _start (/usr/lib/llvm-12/bin/clang+0x40ea25)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 12.0.1-19ubuntu3
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

[main-7097c7.zip](https://github.com/user-attachments/files/18430962/main-7097c7.zip)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 19 or `main` branch? Only most recent release is maintained. https://godbolt.org should be helpful.

---

### Comment 3 - SOHAIBALOUSSI

> Could you please try 19 or `main` branch? Only most recent release is maintained. https://godbolt.org should be helpful.

it seems that the bug does not occur in the 19.1 👍
```
<source>:15:19: error: cannot determine allocated array size from initializer
   15 |         arr = new T[]();
      |                   ^
<source>:33:16: note: in instantiation of member function 'Array<int>::Array' requested here
   33 |     Array<int> arr(10);
      |                ^
1 error generated.
Compiler returned: 1
```  

---

### Comment 4 - EugeneZelenko

You need to upgrade compiler and probably operation system.

---

