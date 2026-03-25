# [clang] crash on CTAD of alias template

**Author:** jy-lefort
**URL:** https://github.com/llvm/llvm-project/issues/139442

## Body

Reproduction:

```
#include <unordered_set>
#include <vector>

template <class Key,
	  class Hash = std::hash<Key>,
	  class KeyEqual = std::equal_to<Key>,
	  class Allocator = std::allocator<Key>>
using my_unordered_set = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

int main (int, char **) {
    std::vector v{123};

    // this CTAD causes the crash
    auto s = my_unordered_set(v.begin(), v.end());
}
```

Crash:

```
0  libLLVM.so.20.1      0x00007f76a0c2914a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 58
1  libLLVM.so.20.1      0x00007f76a0c260d4 llvm::sys::RunSignalHandlers() + 84
2  libLLVM.so.20.1      0x00007f76a0b3f8e4
3  libc.so.6            0x00007f76a0427c30
4  libclang-cpp.so.20.1 0x00007f76a953556c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const + 1036
5  libclang-cpp.so.20.1 0x00007f76a95365c7 clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const + 183
6  libclang-cpp.so.20.1 0x00007f76a953496a clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const + 906
7  libclang-cpp.so.20.1 0x00007f76aabf5f3f clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) + 223
8  libclang-cpp.so.20.1 0x00007f76aabf1fa6 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) + 326
9  libclang-cpp.so.20.1 0x00007f76aabf197d clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&) + 61
10 libclang-cpp.so.20.1 0x00007f76aadc552b clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) + 1019
11 libclang-cpp.so.20.1 0x00007f76aadc48a1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) + 193
12 libclang-cpp.so.20.1 0x00007f76aadd35bc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) + 188
13 libclang-cpp.so.20.1 0x00007f76aae4554b clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) + 2011
14 libclang-cpp.so.20.1 0x00007f76aae6f0df clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 863
15 libclang-cpp.so.20.1 0x00007f76aae66ba9 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 553
16 libclang-cpp.so.20.1 0x00007f76aae6b954 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 1540
17 libclang-cpp.so.20.1 0x00007f76aae65765 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) + 213
18 libclang-cpp.so.20.1 0x00007f76aaf2802c
19 libclang-cpp.so.20.1 0x00007f76aae32632 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) + 210
20 libclang-cpp.so.20.1 0x00007f76a9412e06 clang::ParseAST(clang::Sema&, bool, bool) + 646
21 libclang-cpp.so.20.1 0x00007f76abb1642f clang::FrontendAction::Execute() + 47
22 libclang-cpp.so.20.1 0x00007f76aba78cc4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 1076
23 libclang-cpp.so.20.1 0x00007f76abba9f3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 654
24 clang++              0x000055e30c898695 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 8085
25 clang++              0x000055e30c894727
26 libclang-cpp.so.20.1 0x00007f76ab6193dd
27 libLLVM.so.20.1      0x00007f76a0b3f5f0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 144
28 libclang-cpp.so.20.1 0x00007f76ab618c95 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 373
29 libclang-cpp.so.20.1 0x00007f76ab5d4eb9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 553
30 libclang-cpp.so.20.1 0x00007f76ab5d5177 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 151
31 libclang-cpp.so.20.1 0x00007f76ab5f7d39 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 489
32 clang++              0x000055e30c893b54 clang_main(int, char**, llvm::ToolContext const&) + 6452
33 clang++              0x000055e30c8a39a8 main + 104
34 libc.so.6            0x00007f76a04115f5
35 libc.so.6            0x00007f76a04116a8 __libc_start_main + 136
36 clang++              0x000055e30c891e05 _start + 37
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
```

## Comments

### Comment 1 - frederick-vs-ja

This seems fixed on trunk. Only Clang 20 seems buggy.

Clang 19
- https://godbolt.org/z/371qxMKPb
- https://godbolt.org/z/3vMqPGKKr

Clang 20
- https://godbolt.org/z/z41Mo4Mcq
- https://godbolt.org/z/zron7vPEY

Clang 21 (trunk)
- https://godbolt.org/z/xez8bbjK5
- https://godbolt.org/z/8s9qxvbb1

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (jy-lefort)

<details>
Reproduction:

```
#include &lt;unordered_set&gt;
#include &lt;vector&gt;

template &lt;class Key,
	  class Hash = std::hash&lt;Key&gt;,
	  class KeyEqual = std::equal_to&lt;Key&gt;,
	  class Allocator = std::allocator&lt;Key&gt;&gt;
using my_unordered_set = std::unordered_set&lt;Key, Hash, KeyEqual, Allocator&gt;;

int main (int, char **) {
    std::vector v{123};

    // this CTAD causes the crash
    auto s = my_unordered_set(v.begin(), v.end());
}
```

Crash:

```
0  libLLVM.so.20.1      0x00007f76a0c2914a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 58
1  libLLVM.so.20.1      0x00007f76a0c260d4 llvm::sys::RunSignalHandlers() + 84
2  libLLVM.so.20.1      0x00007f76a0b3f8e4
3  libc.so.6            0x00007f76a0427c30
4  libclang-cpp.so.20.1 0x00007f76a953556c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const + 1036
5  libclang-cpp.so.20.1 0x00007f76a95365c7 clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const + 183
6  libclang-cpp.so.20.1 0x00007f76a953496a clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const + 906
7  libclang-cpp.so.20.1 0x00007f76aabf5f3f clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) + 223
8  libclang-cpp.so.20.1 0x00007f76aabf1fa6 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) + 326
9  libclang-cpp.so.20.1 0x00007f76aabf197d clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) + 61
10 libclang-cpp.so.20.1 0x00007f76aadc552b clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 1019
11 libclang-cpp.so.20.1 0x00007f76aadc48a1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 193
12 libclang-cpp.so.20.1 0x00007f76aadd35bc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) + 188
13 libclang-cpp.so.20.1 0x00007f76aae4554b clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) + 2011
14 libclang-cpp.so.20.1 0x00007f76aae6f0df clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 863
15 libclang-cpp.so.20.1 0x00007f76aae66ba9 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 553
16 libclang-cpp.so.20.1 0x00007f76aae6b954 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 1540
17 libclang-cpp.so.20.1 0x00007f76aae65765 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) + 213
18 libclang-cpp.so.20.1 0x00007f76aaf2802c
19 libclang-cpp.so.20.1 0x00007f76aae32632 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) + 210
20 libclang-cpp.so.20.1 0x00007f76a9412e06 clang::ParseAST(clang::Sema&amp;, bool, bool) + 646
21 libclang-cpp.so.20.1 0x00007f76abb1642f clang::FrontendAction::Execute() + 47
22 libclang-cpp.so.20.1 0x00007f76aba78cc4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 1076
23 libclang-cpp.so.20.1 0x00007f76abba9f3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 654
24 clang++              0x000055e30c898695 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 8085
25 clang++              0x000055e30c894727
26 libclang-cpp.so.20.1 0x00007f76ab6193dd
27 libLLVM.so.20.1      0x00007f76a0b3f5f0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 144
28 libclang-cpp.so.20.1 0x00007f76ab618c95 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 373
29 libclang-cpp.so.20.1 0x00007f76ab5d4eb9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 553
30 libclang-cpp.so.20.1 0x00007f76ab5d5177 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 151
31 libclang-cpp.so.20.1 0x00007f76ab5f7d39 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 489
32 clang++              0x000055e30c893b54 clang_main(int, char**, llvm::ToolContext const&amp;) + 6452
33 clang++              0x000055e30c8a39a8 main + 104
34 libc.so.6            0x00007f76a04115f5
35 libc.so.6            0x00007f76a04116a8 __libc_start_main + 136
36 clang++              0x000055e30c891e05 _start + 37
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
```
</details>


---

### Comment 3 - frederick-vs-ja

@zyn0217 Did #134807 also fix this?

---

### Comment 4 - frederick-vs-ja

BTW, the deduction should succeed, but Clang currently rejects it. This bug is probably tracked in #133132.

---

### Comment 5 - zyn0217

Reopen as it turned out #133132 is not strictly a duplicate of it.

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (jy-lefort)

<details>
Reproduction:

```
#include &lt;unordered_set&gt;
#include &lt;vector&gt;

template &lt;class Key,
	  class Hash = std::hash&lt;Key&gt;,
	  class KeyEqual = std::equal_to&lt;Key&gt;,
	  class Allocator = std::allocator&lt;Key&gt;&gt;
using my_unordered_set = std::unordered_set&lt;Key, Hash, KeyEqual, Allocator&gt;;

int main (int, char **) {
    std::vector v{123};

    // this CTAD causes the crash
    auto s = my_unordered_set(v.begin(), v.end());
}
```

Crash:

```
0  libLLVM.so.20.1      0x00007f76a0c2914a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 58
1  libLLVM.so.20.1      0x00007f76a0c260d4 llvm::sys::RunSignalHandlers() + 84
2  libLLVM.so.20.1      0x00007f76a0b3f8e4
3  libc.so.6            0x00007f76a0427c30
4  libclang-cpp.so.20.1 0x00007f76a953556c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const + 1036
5  libclang-cpp.so.20.1 0x00007f76a95365c7 clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const + 183
6  libclang-cpp.so.20.1 0x00007f76a953496a clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const + 906
7  libclang-cpp.so.20.1 0x00007f76aabf5f3f clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) + 223
8  libclang-cpp.so.20.1 0x00007f76aabf1fa6 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) + 326
9  libclang-cpp.so.20.1 0x00007f76aabf197d clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) + 61
10 libclang-cpp.so.20.1 0x00007f76aadc552b clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 1019
11 libclang-cpp.so.20.1 0x00007f76aadc48a1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 193
12 libclang-cpp.so.20.1 0x00007f76aadd35bc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) + 188
13 libclang-cpp.so.20.1 0x00007f76aae4554b clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) + 2011
14 libclang-cpp.so.20.1 0x00007f76aae6f0df clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 863
15 libclang-cpp.so.20.1 0x00007f76aae66ba9 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 553
16 libclang-cpp.so.20.1 0x00007f76aae6b954 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 1540
17 libclang-cpp.so.20.1 0x00007f76aae65765 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) + 213
18 libclang-cpp.so.20.1 0x00007f76aaf2802c
19 libclang-cpp.so.20.1 0x00007f76aae32632 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) + 210
20 libclang-cpp.so.20.1 0x00007f76a9412e06 clang::ParseAST(clang::Sema&amp;, bool, bool) + 646
21 libclang-cpp.so.20.1 0x00007f76abb1642f clang::FrontendAction::Execute() + 47
22 libclang-cpp.so.20.1 0x00007f76aba78cc4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 1076
23 libclang-cpp.so.20.1 0x00007f76abba9f3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 654
24 clang++              0x000055e30c898695 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 8085
25 clang++              0x000055e30c894727
26 libclang-cpp.so.20.1 0x00007f76ab6193dd
27 libLLVM.so.20.1      0x00007f76a0b3f5f0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 144
28 libclang-cpp.so.20.1 0x00007f76ab618c95 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 373
29 libclang-cpp.so.20.1 0x00007f76ab5d4eb9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 553
30 libclang-cpp.so.20.1 0x00007f76ab5d5177 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 151
31 libclang-cpp.so.20.1 0x00007f76ab5f7d39 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 489
32 clang++              0x000055e30c893b54 clang_main(int, char**, llvm::ToolContext const&amp;) + 6452
33 clang++              0x000055e30c8a39a8 main + 104
34 libc.so.6            0x00007f76a04115f5
35 libc.so.6            0x00007f76a04116a8 __libc_start_main + 136
36 clang++              0x000055e30c891e05 _start + 37
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
```
</details>


---

### Comment 7 - MagentaTreehouse

Reduced (https://godbolt.org/z/a84Tc973T):
```c++
template <class T>
struct traits { using type = T; };

template <class Key, class U = Key>
class unordered_set {
public:
    template <class T>
    unordered_set(T) {}
};

template <class T, class U = traits<T>::type>
unordered_set(T, U = U())
    -> unordered_set<typename traits<T>::type, U>;

template <class Key, class U = Key>
using my_unordered_set = unordered_set<Key, U>;

auto s = my_unordered_set(0);
```

---

### Comment 8 - zyn0217

It's really weird to see that GCC is the only implementation to accept the example, which shouldn't be the case if we really deduce `s` from the deduction guide `my_unordered_set` because `Key` cannot be deduced.

https://godbolt.org/z/64Me6T4dn

GCC seems to bypass the deduction guide if the template-id of the using alias declaration is "simple" enough, so if we make the first template argument non-deducible, GCC rejects it as well:

https://godbolt.org/z/8Tj5j1EY3

I don't know if this is allowed by the standard, though it seems intuitively reasonable.

---

### Comment 9 - zyn0217

https://gcc.gnu.org/bugzilla/show_bug.cgi?id=121080

---

