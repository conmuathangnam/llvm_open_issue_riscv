# Clang-19 crash: void* clang::DeclarationName::getFETokenInfo() const: Assertion `getPtr() && "getFETokenInfo on an empty DeclarationName!"' failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/95420

## Body

### Description: 
When compiling the following C++ code using Clang version 19.0.0 on Ubuntu, an internal compiler error occurs, leading to a stack dump and failure to compile.

Also ICE on trunk, compiler explorer:https:https://godbolt.org/z/158z8qv81

### OS and Platform:
```console
# uname -a
Linux ubuntu 4.15.0-213-generic #224-Ubuntu SMP Mon Jun 19 13:30:12 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux

# clang++ -v
Ubuntu clang version 19.0.0 (++20240301064251+dd426fa5f931-1~exp1~20240301184412.1845)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7.5.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

### Program:
```cpp
# cat code_0.cpp

#include <string>
#include <vector>
struct Foo {
  std::vector<Foo> data;
};
struct Bar {
  void f2(std::size_t size) const {}
};

template <typename T>
void f1(std::size_t, const Foo& foo, const Bar& bar = {});

template <>
void f1<Foo>(std::size_t size, const Foo& foo, const ::Bar& bar) {
  std::size_t pos = 0;
  for(const auto& e: foo.data) {
    f1<Foo>(pos, e);
  }
  bar.f2(size);
}

int main() {
  Foo data{};
  std::string buffer{};
  f1<Foo>(buffer.size(), data);
  return 0;
}

```

### Command Lines:
```
# clang++ code_0.cpp -Wall -Wextra -Werror -std=c++17 -O2 -fno-strict-aliasing -fstack-protector-strong -fno-rtti -fno-exceptions -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Werror=array-bounds -Werror=unused                         -Werror=volatile-register-var -Werror=enum-conversion -Werror=implicit-function-declaration -Werror=implicit-int  -Werror=pointer-arith -Werror=pointer-compare -Werror=null-pointer-subtraction -Werror=shift-count-negative  -c -o code_0.o
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ code_0.cpp -Wall -Wextra -Werror -std=c++17 -O2 -fno-strict-aliasing -fstack-protector-strong -fno-rtti -fno-exceptions -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Werror=array-bounds -Werror=unused -Werror=volatile-register-var -Werror=enum-conversion -Werror=implicit-function-declaration -Werror=implicit-int -Werror=pointer-arith -Werror=pointer-compare -Werror=null-pointer-subtraction -Werror=shift-count-negative -c -o code_0.o
1.      code_0.cpp:20:14: current parser token ')'
2.      code_0.cpp:15:66: parsing function body 'f1<Foo>'
3.      code_0.cpp:15:66: in compound statement ('{}')
 #0 0x00007fbfe6d6d216 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdc1216)
 #1 0x00007fbfe6d6aec0 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdbeec0)
 #2 0x00007fbfe6d6c5f4 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdc05f4)
 #3 0x00007fbfe6cba430 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xd0e430)
 #4 0x00007fbff175c980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #5 0x00007fbfeeaf8e41 clang::IdentifierResolver::iterator::incrementSlowCase() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x13f3e41)
 #6 0x00007fbfef023a9e clang::Sema::CppLookupName(clang::LookupResult&, clang::Scope*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x191ea9e)
 #7 0x00007fbfef027758 clang::Sema::LookupName(clang::LookupResult&, clang::Scope*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1922758)
 #8 0x00007fbfeee2dd5a clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1728d5a)
 #9 0x00007fbfee303bda clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbfebda)
#10 0x00007fbfee2fcc2e clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7c2e)
#11 0x00007fbfee308e5a clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc03e5a)
#12 0x00007fbfee2ff968 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbfa968)
#13 0x00007fbfee3040c8 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbff0c8)
#14 0x00007fbfee2fcc2e clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7c2e)
#15 0x00007fbfee2fcb09 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7b09)
#16 0x00007fbfee35f5d5 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc5a5d5)
#17 0x00007fbfee35d8f6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc588f6)
#18 0x00007fbfee35d2cb clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc582cb)
#19 0x00007fbfee365c80 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc60c80)
#20 0x00007fbfee366def clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc61def)
#21 0x00007fbfee380c36 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc7bc36)
#22 0x00007fbfee2cb3a7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbc63a7)
#23 0x00007fbfee36ed89 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc69d89)
#24 0x00007fbfee36df1c clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc68f1c)
#25 0x00007fbfee36d8e6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc688e6)
#26 0x00007fbfee2c8d7f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbc3d7f)
#27 0x00007fbfee37e1b0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc791b0)
#28 0x00007fbfee37cd3c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc77d3c)
#29 0x00007fbfee2b511e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbb011e)
#30 0x00007fbff01b2825 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2aad825)
#31 0x00007fbff012e0d4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2a290d4)
#32 0x00007fbff022bf7e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2b26f7e)
#33 0x0000563d68201fad cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x12fad)
#34 0x0000563d681ff075 (/usr/lib/llvm-19/bin/clang+0x10075)
#35 0x00007fbfefdbe439 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x26b9439)
#36 0x00007fbfe6cba1dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xd0e1dc)
#37 0x00007fbfefdbddfe clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x26b8dfe)
#38 0x00007fbfefd85901 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2680901)
#39 0x00007fbfefd85b4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2680b4e)
#40 0x00007fbfefda26cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x269d6cc)
#41 0x0000563d681fe9e5 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xf9e5)
#42 0x0000563d6820c556 main (/usr/lib/llvm-19/bin/clang+0x1d556)
#43 0x00007fbfe529dc87 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21c87)
#44 0x0000563d681fbbfa _start (/usr/lib/llvm-19/bin/clang+0xcbfa)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240301064251+dd426fa5f931-1~exp1~20240301184412.1845)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 

```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
### Description: 
When compiling the following C++ code using Clang version 19.0.0 on Ubuntu, an internal compiler error occurs, leading to a stack dump and failure to compile.

Also ICE on trunk, compiler explorer:https:https://godbolt.org/z/158z8qv81

### OS and Platform:
```
# uname -a
Linux ubuntu 4.15.0-213-generic #<!-- -->224-Ubuntu SMP Mon Jun 19 13:30:12 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux

# clang++ -v
Ubuntu clang version 19.0.0 (++20240301064251+dd426fa5f931-1~exp1~20240301184412.1845)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7.5.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

### Program:
```
# cat code_0.cpp

#include &lt;string&gt;
#include &lt;vector&gt;
struct Foo {
  std::vector&lt;Foo&gt; data;
};
struct Bar {
  void f2(std::size_t size) const {}
};

template &lt;typename T&gt;
void f1(std::size_t, const Foo&amp; foo, const Bar&amp; bar = {});

template &lt;&gt;
void f1&lt;Foo&gt;(std::size_t size, const Foo&amp; foo, const ::Bar&amp; bar) {
  std::size_t pos = 0;
  for(const auto&amp; e: foo.data) {
    f1&lt;Foo&gt;(pos, e);
  }
  bar.f2(size);
}

int main() {
  Foo data{};
  std::string buffer{};
  f1&lt;Foo&gt;(buffer.size(), data);
  return 0;
}

```

### Command Lines:
```
# clang++ code_0.cpp -Wall -Wextra -Werror -std=c++17 -O2 -fno-strict-aliasing -fstack-protector-strong -fno-rtti -fno-exceptions -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Werror=array-bounds -Werror=unused                         -Werror=volatile-register-var -Werror=enum-conversion -Werror=implicit-function-declaration -Werror=implicit-int  -Werror=pointer-arith -Werror=pointer-compare -Werror=null-pointer-subtraction -Werror=shift-count-negative  -c -o code_0.o
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ code_0.cpp -Wall -Wextra -Werror -std=c++17 -O2 -fno-strict-aliasing -fstack-protector-strong -fno-rtti -fno-exceptions -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Wno-vla -Wno-gnu-zero-variadic-macro-arguments -Werror=array-bounds -Werror=unused -Werror=volatile-register-var -Werror=enum-conversion -Werror=implicit-function-declaration -Werror=implicit-int -Werror=pointer-arith -Werror=pointer-compare -Werror=null-pointer-subtraction -Werror=shift-count-negative -c -o code_0.o
1.      code_0.cpp:20:14: current parser token ')'
2.      code_0.cpp:15:66: parsing function body 'f1&lt;Foo&gt;'
3.      code_0.cpp:15:66: in compound statement ('{}')
 #<!-- -->0 0x00007fbfe6d6d216 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdc1216)
 #<!-- -->1 0x00007fbfe6d6aec0 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdbeec0)
 #<!-- -->2 0x00007fbfe6d6c5f4 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xdc05f4)
 #<!-- -->3 0x00007fbfe6cba430 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xd0e430)
 #<!-- -->4 0x00007fbff175c980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #<!-- -->5 0x00007fbfeeaf8e41 clang::IdentifierResolver::iterator::incrementSlowCase() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x13f3e41)
 #<!-- -->6 0x00007fbfef023a9e clang::Sema::CppLookupName(clang::LookupResult&amp;, clang::Scope*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x191ea9e)
 #<!-- -->7 0x00007fbfef027758 clang::Sema::LookupName(clang::LookupResult&amp;, clang::Scope*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1922758)
 #<!-- -->8 0x00007fbfeee2dd5a clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1728d5a)
 #<!-- -->9 0x00007fbfee303bda clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbfebda)
#<!-- -->10 0x00007fbfee2fcc2e clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7c2e)
#<!-- -->11 0x00007fbfee308e5a clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc03e5a)
#<!-- -->12 0x00007fbfee2ff968 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbfa968)
#<!-- -->13 0x00007fbfee3040c8 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbff0c8)
#<!-- -->14 0x00007fbfee2fcc2e clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7c2e)
#<!-- -->15 0x00007fbfee2fcb09 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbf7b09)
#<!-- -->16 0x00007fbfee35f5d5 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc5a5d5)
#<!-- -->17 0x00007fbfee35d8f6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc588f6)
#<!-- -->18 0x00007fbfee35d2cb clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc582cb)
#<!-- -->19 0x00007fbfee365c80 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc60c80)
#<!-- -->20 0x00007fbfee366def clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc61def)
#<!-- -->21 0x00007fbfee380c36 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc7bc36)
#<!-- -->22 0x00007fbfee2cb3a7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbc63a7)
#<!-- -->23 0x00007fbfee36ed89 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc69d89)
#<!-- -->24 0x00007fbfee36df1c clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc68f1c)
#<!-- -->25 0x00007fbfee36d8e6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc688e6)
#<!-- -->26 0x00007fbfee2c8d7f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbc3d7f)
#<!-- -->27 0x00007fbfee37e1b0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc791b0)
#<!-- -->28 0x00007fbfee37cd3c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc77d3c)
#<!-- -->29 0x00007fbfee2b511e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xbb011e)
#<!-- -->30 0x00007fbff01b2825 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2aad825)
#<!-- -->31 0x00007fbff012e0d4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2a290d4)
#<!-- -->32 0x00007fbff022bf7e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2b26f7e)
#<!-- -->33 0x0000563d68201fad cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x12fad)
#<!-- -->34 0x0000563d681ff075 (/usr/lib/llvm-19/bin/clang+0x10075)
#<!-- -->35 0x00007fbfefdbe439 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x26b9439)
#<!-- -->36 0x00007fbfe6cba1dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xd0e1dc)
#<!-- -->37 0x00007fbfefdbddfe clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x26b8dfe)
#<!-- -->38 0x00007fbfefd85901 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2680901)
#<!-- -->39 0x00007fbfefd85b4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2680b4e)
#<!-- -->40 0x00007fbfefda26cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x269d6cc)
#<!-- -->41 0x0000563d681fe9e5 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xf9e5)
#<!-- -->42 0x0000563d6820c556 main (/usr/lib/llvm-19/bin/clang+0x1d556)
#<!-- -->43 0x00007fbfe529dc87 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21c87)
#<!-- -->44 0x0000563d681fbbfa _start (/usr/lib/llvm-19/bin/clang+0xcbfa)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240301064251+dd426fa5f931-1~exp1~20240301184412.1845)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 

```

</details>


---

### Comment 2 - shafik

Looks like this started after clang-10: https://godbolt.org/z/fhceaK86T

Current assertions trunk: https://godbolt.org/z/qT84fjbsd

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclarationName.h:503:
void* clang::DeclarationName::getFETokenInfo() const:
Assertion `getPtr() && "getFETokenInfo on an empty DeclarationName!"' failed.
```

---

### Comment 3 - shafik

Much reduced: https://godbolt.org/z/aMx8v94d5

```cpp
#include <string> // Need this include 

struct A {
  A* data[2];
};

template <typename T>
void f1(unsigned long, const A& f, const A& b = {});

template <>
void f1<A>(unsigned long size, const A& f, const A& b) { // We also need b 
  f1<A>(0, *f.data[0]); // We still need this line
  
  size; // Changing the name of size prevents crash
}
```

This is an odd one, we require the include and the name of `size` to be that name. There much be a `size` variable/type we are mixing up from `<string>`. 

CC @Endilll I am hoping a reduction will help clarify the issue.

---

### Comment 4 - Endilll

Reduced by C-Reduce (https://godbolt.org/z/3T7h8E43P):
```cpp
template <typename _Container>
constexpr auto
size(const _Container &__cont) noexcept -> decltype(__cont.size0);

struct A {
  A* data[2];
};

template <typename T>
void f1(unsigned long, const A& f, const A& b = {});

template <>
void f1<A>(unsigned long size, const A& f, const A& b) { // We also need b 
  f1<A>(0, *f.data[0]); // We still need this line
  
  size; // Changing the name of size prevents crash
}
```

---

### Comment 5 - shafik

Wait this looks like exactly the same issue as: https://github.com/llvm/llvm-project/issues/60066

which was closed as a duplicate of: https://github.com/llvm/llvm-project/issues/54279

although it is not clear to me they are totally related.

---

### Comment 6 - mark-de-wever-sonarsource

This is not an exact duplicate of #54279, it has a different stack trace. #54279 is caused by the function argument `b` having a different name. Here the usage of `size` in the body is an issue. While working on #142338 I wanted to test whether something like this could happen, but I couldn't find a good test case. #142338 fixes this issue too.

---

