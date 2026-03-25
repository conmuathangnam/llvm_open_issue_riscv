# Clang crashed in ASTContext::getCommonSugaredType

**Author:** spavloff
**URL:** https://github.com/llvm/llvm-project/issues/97005
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-03-25T18:13:53Z

## Body

Clang ToT version is used:
```
$ clang --version
clang version 19.0.0git (https://github.com/llvm/llvm-project.git 5bbec8f4b6a26e8b4a4209d90a2ad0a0d2416a55)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /ext/sdc/arbeit/space1/build/bin
Build config: +assertions   
```

The source code is:
```
$ cat reduce.cpp
bool a;
constexpr const unsigned char c[] = { 5 };
constexpr const unsigned char d[1] = { 0 };
auto b = (a ? d : c);
```

Running the compiler results in a crash:
```
$ clang -fsyntax-only reduce.cpp 
clang: /export/serge/llvm/space1/llvm-project/clang/lib/AST/ASTContext.cpp:13501: clang::QualType clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool): Assertion `QX == QY' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -fsyntax-only reduce.cpp
1.	reduce.cpp:4:20: current parser token ')'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang     0x0000000002070b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 40
1  clang     0x000000000206e55e llvm::sys::RunSignalHandlers() + 238
2  clang     0x0000000001fe5727
3  libc.so.6 0x00007f9621462520
4  libc.so.6 0x00007f96214b69fc pthread_kill + 300
5  libc.so.6 0x00007f9621462476 raise + 22
6  libc.so.6 0x00007f96214487f3 abort + 211
7  libc.so.6 0x00007f962144871b
8  libc.so.6 0x00007f9621459e96
9  clang     0x000000000529813c clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool) + 8332
10 clang     0x00000000049ad0d4 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) + 1412
11 clang     0x000000000483ac6f clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) + 367
12 clang     0x00000000044111e6 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) + 4742
13 clang     0x0000000004419b5d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 3021
14 clang     0x0000000004416e76 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 11270
15 clang     0x0000000004411761 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) + 33
16 clang     0x000000000440ff04 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 196
17 clang     0x00000000043d2156 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) + 3670
18 clang     0x00000000043cf299 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 3433
19 clang     0x00000000043a6285 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1365
20 clang     0x00000000043a5ab4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 596
21 clang     0x00000000043a4abb clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 3659
22 clang     0x00000000043a2229 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 2217
23 clang     0x000000000439d4e5 clang::ParseAST(clang::Sema&, bool, bool) + 581
24 clang     0x0000000002b3be76 clang::FrontendAction::Execute() + 262
25 clang     0x0000000002aa0934 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 1092
26 clang     0x0000000002c0dd07 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 535
27 clang     0x0000000000b39f37 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 4967
28 clang     0x0000000000b36993
29 clang     0x00000000028cce69
30 clang     0x0000000001fe5469 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 137
31 clang     0x00000000028cc83a clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 266
32 clang     0x00000000028831df clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 463
33 clang     0x000000000288375a clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 154
34 clang     0x00000000028a7b17 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 743
35 clang     0x0000000000b35865 clang_main(int, char**, llvm::ToolContext const&) + 5461
36 clang     0x0000000000b46347 main + 87
37 libc.so.6 0x00007f9621449d90
38 libc.so.6 0x00007f9621449e40 __libc_start_main + 128
39 clang     0x0000000000b33f85 _start + 37
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 19.0.0git (https://github.com/llvm/llvm-project.git 5bbec8f4b6a26e8b4a4209d90a2ad0a0d2416a55)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /ext/sdc/arbeit/space1/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/reduce-159e8c.cpp
clang: note: diagnostic msg: /tmp/reduce-159e8c.sh
clang: note: diagnostic msg: 

********************
```

The crash is triggered here:
```
clang::QualType clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool): Assertion `QX == QY' failed.
```

It can also be obverved online: https://godbolt.org/z/GGnfzbPv7.


## Comments

### Comment 1 - AaronBallman

`QX` is `const` while `QY` is not, which is why the assertion triggers. Looking at a similar example (https://godbolt.org/z/hG59GYnec), it's interesting to note that we seem to get a funky type for `d`: `VarDecl <line:3:1, col:42> col:31 d 'const const unsigned char[1]' constexpr cinit`

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Serge Pavlov (spavloff)

<details>
Clang ToT version is used:
```
$ clang --version
clang version 19.0.0git (https://github.com/llvm/llvm-project.git 5bbec8f4b6a26e8b4a4209d90a2ad0a0d2416a55)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /ext/sdc/arbeit/space1/build/bin
Build config: +assertions   
```

The source code is:
```
$ cat reduce.cpp
bool a;
constexpr const unsigned char c[] = { 5 };
constexpr const unsigned char d[1] = { 0 };
auto b = (a ? d : c);
```

Running the compiler results in a crash:
```
$ clang -fsyntax-only reduce.cpp 
clang: /export/serge/llvm/space1/llvm-project/clang/lib/AST/ASTContext.cpp:13501: clang::QualType clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool): Assertion `QX == QY' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -fsyntax-only reduce.cpp
1.	reduce.cpp:4:20: current parser token ')'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang     0x0000000002070b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 40
1  clang     0x000000000206e55e llvm::sys::RunSignalHandlers() + 238
2  clang     0x0000000001fe5727
3  libc.so.6 0x00007f9621462520
4  libc.so.6 0x00007f96214b69fc pthread_kill + 300
5  libc.so.6 0x00007f9621462476 raise + 22
6  libc.so.6 0x00007f96214487f3 abort + 211
7  libc.so.6 0x00007f962144871b
8  libc.so.6 0x00007f9621459e96
9  clang     0x000000000529813c clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool) + 8332
10 clang     0x00000000049ad0d4 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) + 1412
11 clang     0x000000000483ac6f clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) + 367
12 clang     0x00000000044111e6 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) + 4742
13 clang     0x0000000004419b5d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 3021
14 clang     0x0000000004416e76 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 11270
15 clang     0x0000000004411761 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) + 33
16 clang     0x000000000440ff04 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 196
17 clang     0x00000000043d2156 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) + 3670
18 clang     0x00000000043cf299 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 3433
19 clang     0x00000000043a6285 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1365
20 clang     0x00000000043a5ab4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 596
21 clang     0x00000000043a4abb clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 3659
22 clang     0x00000000043a2229 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 2217
23 clang     0x000000000439d4e5 clang::ParseAST(clang::Sema&amp;, bool, bool) + 581
24 clang     0x0000000002b3be76 clang::FrontendAction::Execute() + 262
25 clang     0x0000000002aa0934 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 1092
26 clang     0x0000000002c0dd07 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 535
27 clang     0x0000000000b39f37 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 4967
28 clang     0x0000000000b36993
29 clang     0x00000000028cce69
30 clang     0x0000000001fe5469 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 137
31 clang     0x00000000028cc83a clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 266
32 clang     0x00000000028831df clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 463
33 clang     0x000000000288375a clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 154
34 clang     0x00000000028a7b17 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 743
35 clang     0x0000000000b35865 clang_main(int, char**, llvm::ToolContext const&amp;) + 5461
36 clang     0x0000000000b46347 main + 87
37 libc.so.6 0x00007f9621449d90
38 libc.so.6 0x00007f9621449e40 __libc_start_main + 128
39 clang     0x0000000000b33f85 _start + 37
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 19.0.0git (https://github.com/llvm/llvm-project.git 5bbec8f4b6a26e8b4a4209d90a2ad0a0d2416a55)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /ext/sdc/arbeit/space1/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/reduce-159e8c.cpp
clang: note: diagnostic msg: /tmp/reduce-159e8c.sh
clang: note: diagnostic msg: 

********************
```

The crash is triggered here:
```
clang::QualType clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool): Assertion `QX == QY' failed.
```

It can also be obverved online: https://godbolt.org/z/GGnfzbPv7.

</details>


---

### Comment 3 - AaronBallman

We drop the extra `const` qualifier for `c` when converting the incomplete array type for the variable declaration to being a complete array type, which is why `c` and `d` differ in types.

It's not clear to me that we should be adding `const` to the array type. An array's qualifications are determined by the qualification of the element type: http://eel.is/c++draft/basic.types#basic.type.qualifier-3 However, the standard is clear that `constexpr` adds `const` to the object being declared: https://eel.is/c++draft/dcl.constexpr#6 and arrays are objects. This makes for an interesting question about what should happen for `constexpr int foo[1] = { 1 };` -- should that make a `int[1] const` or should that make an `int[1]` or should that make a `const int[1] const`? CC @zygoloid because I'm probably missing something.

---

### Comment 4 - efriedma-quic

Applying a const-qualifier to an array is the same as applying the qualifier to the element type.  So, for example, `typedef const int t[1]; typedef const t t;` is valid.  Any code dealing with qualifiers on non-canonical types needs to special-case arrays.

---

### Comment 5 - berolinux

Probably the same bug:

```
static void a(int b) {
        static constexpr const char c[][2] = { "1" };
        const char *d = (b > 1 ? "2" : c[b]);
}
```

crashes with a very similar backtrace.

---

