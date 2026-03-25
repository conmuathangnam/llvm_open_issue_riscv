# [clang] Crash when defining a previously declared function template as a non-template friend with global scope qualifier

**Author:** dgodfrey206
**URL:** https://github.com/llvm/llvm-project/issues/185341

## Body



```cpp
template<class>
struct D;

template<class T>
void foo(D<T>);

template<class T>
struct D {
    friend void ::foo(D) {}
};

int main() {
    foo( D<int>{} );
}
```

```
Compiler stderr
<source>:9:17: error: friend function definition cannot be qualified with '::'
    9 |     friend void ::foo(D) {}
Stack dump:
0.	Program arguments: /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/lib/clang/23 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/backward -internal-isystem /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/lib/clang/23/include -internal-isystem /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/../include/x86_64-unknown-linux-gnu -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-d10c73.o -x c++ <source>
1.	<eof> parser at end of file
2.	<source>:9:19: instantiating function definition 'foo<int>'
 #0 0x0000000003e59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x3e59568)
 #1 0x0000000003e566a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000000003e56798 SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000077cda0242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000000c9cbca clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.cold) SemaTemplateInstantiateDecl.cpp:0:0
 #5 0x000000000702ada8 clang::Sema::PerformPendingInstantiations(bool, bool) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x702ada8)
 #6 0x00000000065be469 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #7 0x00000000065be744 clang::Sema::ActOnEndOfTranslationUnit() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x65be744)
 #8 0x000000000641d53f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x641d53f)
 #9 0x00000000063f623a clang::ParseAST(clang::Sema&, bool, bool) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x63f623a)
#10 0x00000000048394e5 clang::CodeGenAction::ExecuteAction() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x48394e5)
#11 0x0000000004b5b5ca clang::FrontendAction::Execute() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4b5b5ca)
#12 0x0000000004ad84bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4ad84bb)
#13 0x0000000004c53e7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4c53e7b)
#14 0x0000000000df8375 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdf8375)
#15 0x0000000000df003b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#16 0x0000000000df2fcf clang_main(int, char**, llvm::ToolContext const&) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdf2fcf)
#17 0x0000000000ca51fa main (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xca51fa)
#18 0x000077cda0229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#19 0x000077cda0229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#20 0x0000000000defad5 _start (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdefad5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed with exit code -2 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Godfrey (dgodfrey206)

<details>


```cpp
template&lt;class&gt;
struct D;

template&lt;class T&gt;
void foo(D&lt;T&gt;);

template&lt;class T&gt;
struct D {
    friend void ::foo(D) {}
};

int main() {
    foo( D&lt;int&gt;{} );
}
```

```
Compiler stderr
&lt;source&gt;:9:17: error: friend function definition cannot be qualified with '::'
    9 |     friend void ::foo(D) {}
Stack dump:
0.	Program arguments: /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/lib/clang/23 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/backward -internal-isystem /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/lib/clang/23/include -internal-isystem /cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/../include/x86_64-unknown-linux-gnu -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-d10c73.o -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:9:19: instantiating function definition 'foo&lt;int&gt;'
 #<!-- -->0 0x0000000003e59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x3e59568)
 #<!-- -->1 0x0000000003e566a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000000003e56798 SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000077cda0242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000000c9cbca clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.cold) SemaTemplateInstantiateDecl.cpp:0:0
 #<!-- -->5 0x000000000702ada8 clang::Sema::PerformPendingInstantiations(bool, bool) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x702ada8)
 #<!-- -->6 0x00000000065be469 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #<!-- -->7 0x00000000065be744 clang::Sema::ActOnEndOfTranslationUnit() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x65be744)
 #<!-- -->8 0x000000000641d53f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x641d53f)
 #<!-- -->9 0x00000000063f623a clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x63f623a)
#<!-- -->10 0x00000000048394e5 clang::CodeGenAction::ExecuteAction() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x48394e5)
#<!-- -->11 0x0000000004b5b5ca clang::FrontendAction::Execute() (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4b5b5ca)
#<!-- -->12 0x0000000004ad84bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4ad84bb)
#<!-- -->13 0x0000000004c53e7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0x4c53e7b)
#<!-- -->14 0x0000000000df8375 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdf8375)
#<!-- -->15 0x0000000000df003b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->16 0x0000000000df2fcf clang_main(int, char**, llvm::ToolContext const&amp;) (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdf2fcf)
#<!-- -->17 0x0000000000ca51fa main (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xca51fa)
#<!-- -->18 0x000077cda0229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->19 0x000077cda0229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->20 0x0000000000defad5 _start (/cefs/52/52629d580b115717d73c025b_clang-trunk-20260308/bin/clang-23+0xdefad5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed with exit code -2 (use -v to see invocation)
```
</details>


---

### Comment 2 - zyn0217

https://compiler-explorer.com/z/6bonsWqxo

```console
SemaTemplateInstantiateDecl.cpp:5262: 
void clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool): 
Assertion `It != Primary->redecls().end() && "Should't get here without a definition"' failed.
```


---

### Comment 3 - shafik

@zyn0217 splitting assertions into three lines makes them a lot more readable.

---

### Comment 4 - shafik

@mizvekov this bisects to 346077aaa6bef5652a72a2f3d9fc134ea8fc6a5b

---

