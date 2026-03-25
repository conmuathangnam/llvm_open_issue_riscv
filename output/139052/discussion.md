# Clang Crash When Defining operator<< as a Friend Template Function of a Class Template since clang 20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139052
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:20
**Closed Date:** 2025-05-14T03:45:34Z

## Body

Clang crashes during instantiation of a friend function template declared outside the class when used with BinaryTree<T>. The crash occurs in the function InstantiateFunctionDefinition in clang 20. **Haven´t been able to reduce code more**

## Assertion 

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5767: void clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool): Assertion `It != Primary->redecls().end() && "Should't get here without a definition"' failed.`
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<eof> parser at end of file
2.	<source>:19:26: instantiating function definition 'operator<<<int>'
 #0 0x0000000003f7c0a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7c0a8)
 #1 0x0000000003f79d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f79d34)
 #2 0x0000000003ebebf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000790a8e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000790a8e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000790a8e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000790a8e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000790a8e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000790a8e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007244186 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7244186)
#10 0x0000000007241b46 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7241b46)
#11 0x0000000006790242 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#12 0x0000000006790ae2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6790ae2)
#13 0x00000000065fe6eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fe6eb)
#14 0x00000000065f09ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f09ea)
#15 0x000000000491ebe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491ebe8)
#16 0x0000000004c11f15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c11f15)
#17 0x0000000004b9131e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9131e)
#18 0x0000000004d05d39 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d05d39)
#19 0x0000000000da788f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda788f)
#20 0x0000000000d9da5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#21 0x00000000049854a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x0000000003ebf094 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebf094)
#23 0x0000000004985abf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x0000000004947f0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4947f0d)
#25 0x0000000004948f9e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4948f9e)
#26 0x0000000004950df5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4950df5)
#27 0x0000000000da37f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda37f8)
#28 0x0000000000c28b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28b74)
#29 0x0000790a8e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x0000790a8e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000d9d505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d505)
```

## Program 

```
#include <iostream>
#include <memory>
template <typename T>
class BinaryTree;
template <typename T>
std::ostream& operator<<(std::ostream& os, BinaryTree<T>& b);
template <typename T>
struct Node {
    Node(T data) : data(data) {}
    T data;
};
template <typename T>
struct BinaryTree {
    std::unique_ptr<Node<T>> root;
    BinaryTree() = default;
    BinaryTree(T data) {
        root = std::make_unique<Node<T>>(data);
    }
    friend std::ostream& operator<< <T>(std::ostream& os, BinaryTree& b) {
        return (b.root)? os << b.root->data : os << "<empty tree>";
    }
};
int main() {
    BinaryTree<int> a(1);
    std::cout << a << std::endl;
}
```

## To quickly reproduce

it happens since clang 20

https://gcc.godbolt.org/z/WWWqdcbjj

## Comments

### Comment 1 - frederick-vs-ja

Reduced library-free example ([Godbolt link](https://gcc.godbolt.org/z/o5dchz6z4)):

```C++
struct FakeStream {};

template <typename T>
class BinaryTree;
template <typename T>
FakeStream& operator<<(FakeStream& os, BinaryTree<T>& b);

template <typename T>
struct BinaryTree {
  T* root{};
  friend FakeStream& operator<< <T>(FakeStream& os, BinaryTree&) {
    return os;
  }
};

int main() {
  FakeStream fakeout;
  BinaryTree<int> a{};
  fakeout << a;
}
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Clang crashes during instantiation of a friend function template declared outside the class when used with BinaryTree&lt;T&gt;. The crash occurs in the function InstantiateFunctionDefinition in clang 20. **Haven´t been able to reduce code more**

## Assertion 

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5767: void clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool): Assertion `It != Primary-&gt;redecls().end() &amp;&amp; "Should't get here without a definition"' failed.`
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:19:26: instantiating function definition 'operator&lt;&lt;&lt;int&gt;'
 #<!-- -->0 0x0000000003f7c0a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7c0a8)
 #<!-- -->1 0x0000000003f79d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f79d34)
 #<!-- -->2 0x0000000003ebebf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000790a8e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000790a8e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000790a8e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000790a8e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000790a8e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000790a8e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007244186 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7244186)
#<!-- -->10 0x0000000007241b46 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7241b46)
#<!-- -->11 0x0000000006790242 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->12 0x0000000006790ae2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6790ae2)
#<!-- -->13 0x00000000065fe6eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fe6eb)
#<!-- -->14 0x00000000065f09ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f09ea)
#<!-- -->15 0x000000000491ebe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491ebe8)
#<!-- -->16 0x0000000004c11f15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c11f15)
#<!-- -->17 0x0000000004b9131e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9131e)
#<!-- -->18 0x0000000004d05d39 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d05d39)
#<!-- -->19 0x0000000000da788f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda788f)
#<!-- -->20 0x0000000000d9da5a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->21 0x00000000049854a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->22 0x0000000003ebf094 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebf094)
#<!-- -->23 0x0000000004985abf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->24 0x0000000004947f0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4947f0d)
#<!-- -->25 0x0000000004948f9e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4948f9e)
#<!-- -->26 0x0000000004950df5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4950df5)
#<!-- -->27 0x0000000000da37f8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda37f8)
#<!-- -->28 0x0000000000c28b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28b74)
#<!-- -->29 0x0000790a8e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x0000790a8e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000d9d505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d505)
```

## Program 

```
#include &lt;iostream&gt;
#include &lt;memory&gt;
template &lt;typename T&gt;
class BinaryTree;
template &lt;typename T&gt;
std::ostream&amp; operator&lt;&lt;(std::ostream&amp; os, BinaryTree&lt;T&gt;&amp; b);
template &lt;typename T&gt;
struct Node {
    Node(T data) : data(data) {}
    T data;
};
template &lt;typename T&gt;
struct BinaryTree {
    std::unique_ptr&lt;Node&lt;T&gt;&gt; root;
    BinaryTree() = default;
    BinaryTree(T data) {
        root = std::make_unique&lt;Node&lt;T&gt;&gt;(data);
    }
    friend std::ostream&amp; operator&lt;&lt; &lt;T&gt;(std::ostream&amp; os, BinaryTree&amp; b) {
        return (b.root)? os &lt;&lt; b.root-&gt;data : os &lt;&lt; "&lt;empty tree&gt;";
    }
};
int main() {
    BinaryTree&lt;int&gt; a(1);
    std::cout &lt;&lt; a &lt;&lt; std::endl;
}
```

## To quickly reproduce

it happens since clang 20

https://gcc.godbolt.org/z/WWWqdcbjj
</details>


---

