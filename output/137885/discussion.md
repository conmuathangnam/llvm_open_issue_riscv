# A parameter pack appears both empty and non-empty at the same time

**Author:** HolyBlackCat
**URL:** https://github.com/llvm/llvm-project/issues/137885
**Status:** Closed
**Labels:** clang:frontend, confirmed, consteval
**Closed Date:** 2025-05-04T11:27:10Z

## Body

Found a really concerning bug:
```cpp
template <typename ...P>
struct A {};

struct B
{
    consteval B() {} // Making this constexpr makes the assert below pass.

    template <typename ...P>
    consteval operator A<P...>() const
    {
        static_assert(sizeof...(P) > 0); // This fails on Clang.
        return {};
    }
};

template <typename T>
struct type_identity 
{
    using type = T;
};

template <typename ...P>
void foo(typename type_identity<A<P...>>::type a, P...) {}

int main()
{
    foo(B(), 1, 2);
}
```
The `static_assert` fires on Clang, meaning it calls `operator A<>` with no template arguments! But the corresponding function parameter is `A<int, int>`.

Apparently `A<>` gets reinterpreted (!!) into `A<int, int>` at compile-time. E.g. if you add a `int x = sizeof...(P)` data member to it, it'll be `0` despite the type actually being `A<int, int>`.

Here's an example of this being used to `reinterpret_cast` a `float` into an `int`: https://gcc.godbolt.org/z/Txbsj8ofM

---

Tested on Clang 20 and trunk. The only flag needed is `-std=c++20` for `consteval`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Egor (HolyBlackCat)

<details>
Found a really concerning bug:
```cpp
template &lt;typename ...P&gt;
struct A {};

struct B
{
    consteval B() {} // Making this constexpr makes the assert below pass.

    template &lt;typename ...P&gt;
    consteval operator A&lt;P...&gt;() const
    {
        static_assert(sizeof...(P) &gt; 0); // This fails on Clang.
        return {};
    }
};

template &lt;typename T&gt;
struct type_identity 
{
    using type = T;
};

template &lt;typename ...P&gt;
void foo(typename type_identity&lt;A&lt;P...&gt;&gt;::type a, P...) {}

int main()
{
    foo(B(), 1, 2);
}
```
The `static_assert` fires on Clang, meaning it calls `operator A&lt;&gt;` with no template arguments! But the corresponding function parameter is `A&lt;int, int&gt;`.

Apparently `A&lt;&gt;` gets reinterpreted (!!) into `A&lt;int, int&gt;` at compile-time. E.g. if you add a `int x = sizeof...(P)` data member to it, it'll be `0` despite the type actually being `A&lt;int, int&gt;`.

Here's an example of this being used to `reinterpret_cast` a `float` into an `int`: https://gcc.godbolt.org/z/Txbsj8ofM

---

Tested on Clang 20 and trunk. The only flag needed is `-std=c++20` for `consteval`.
</details>


---

### Comment 2 - shafik

It crashes on assertions build, so this is a straight up bug: https://godbolt.org/z/jdKKWfnGe

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Expr.cpp:1745:
clang::MemberExpr::MemberExpr(clang::Expr*, bool, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation, clang::ValueDecl*, clang::DeclAccessPair, const clang::DeclarationNameInfo&, const clang::TemplateArgumentListInfo*, clang::QualType, clang::ExprValueKind, clang::ExprObjectKind, clang::NonOdrUseReason):
Assertion `!NameInfo.getName() || MemberDecl->getDeclName() == NameInfo.getName()' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<eof> parser at end of file
2.	<source>:26:1: parsing function body 'main'
 #0 0x0000000003f1c8c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1c8c8)
 #1 0x0000000003f1a554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1a554)
 #2 0x0000000003e5f088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d7581842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d75818969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d7581842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d75818287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d758182871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d7581839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077a90b1 clang::MemberExpr::MemberExpr(clang::Expr*, bool, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation, clang::ValueDecl*, clang::DeclAccessPair, clang::DeclarationNameInfo const&, clang::TemplateArgumentListInfo const*, clang::QualType, clang::ExprValueKind, clang::ExprObjectKind, clang::NonOdrUseReason) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77a90b1)
#10 0x00000000077bb4d6 clang::MemberExpr::Create(clang::ASTContext const&, clang::Expr*, bool, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation, clang::ValueDecl*, clang::DeclAccessPair, clang::DeclarationNameInfo, clang::TemplateArgumentListInfo const*, clang::QualType, clang::ExprValueKind, clang::ExprObjectKind, clang::NonOdrUseReason) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77bb4d6)
#11 0x0000000006c143c5 clang::Sema::BuildMemberExpr(clang::Expr*, bool, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation, clang::ValueDecl*, clang::DeclAccessPair, bool, clang::DeclarationNameInfo const&, clang::QualType, clang::ExprValueKind, clang::ExprObjectKind, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c143c5)
#12 0x0000000006e96fcb clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e96fcb)
#13 0x0000000006ecb134 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ecb134)
#14 0x0000000006acd63d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acd63d)
#15 0x0000000006ace5fe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ace5fe)
#16 0x0000000006b3c91b clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformCallExpr(clang::CallExpr*) SemaExpr.cpp:0:0
#17 0x0000000006aa36cd clang::Sema::PopExpressionEvaluationContext() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa36cd)
#18 0x0000000006934db6 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6934db6)
#19 0x0000000006688a6f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6688a6f)
#20 0x0000000006591833 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6591833)
#21 0x00000000065c78fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c78fd)
#22 0x000000000658536e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658536e)
#23 0x0000000006585b29 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6585b29)
#24 0x000000000658d44a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658d44a)
#25 0x000000000658e3ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658e3ed)
#26 0x00000000065807ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65807ca)
#27 0x00000000048bc388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bc388)
#28 0x0000000004bb0125 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0125)
#29 0x0000000004b2e5ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e5ce)
#30 0x0000000004c9f999 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9f999)
#31 0x0000000000da28df cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda28df)
#32 0x0000000000d998ea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000004922c19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003e5f524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5f524)
#35 0x000000000492322f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x00000000048e568d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e568d)
#37 0x00000000048e671e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e671e)
#38 0x00000000048ee575 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ee575)
#39 0x0000000000d9f628 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f628)
#40 0x0000000000c25054 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc25054)
#41 0x00007d7581829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007d7581829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d99395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd99395)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

CC @cor3ntin 

---

### Comment 3 - shafik

Goes all the way back to clang-12: https://godbolt.org/z/z7McThjjq

---

### Comment 4 - cor3ntin

I struggle to find the exact root cause. In the presence of a consteval sub-expression, we rewrite the full expression (
ie `B()` causes us to rewrite `foo(B(), 1, 2);` which we do with a `TreeTransform`  - to remove `ConstantExpr` nodes -

And either because we do it in a slightly wrong context or at a slightly different time, we do not reconstitute the exact same expression. 

We should try to figure out why, but I also wonder if instead of a full rewrite, we should maybe just set a flag to "disable" the `ConstantExpr`, so that we can reuse the same sub-ast.

---

### Comment 5 - HolyBlackCat

After more experimentation, removing `consteval` from the constructor isn't a proper workaround. Things still break in really weird ways.

This errors as expected: https://gcc.godbolt.org/z/xoE3W7b3P
```cpp
template <typename ...P>
struct A {};

struct B
{
    int n;
    constexpr B(int n) : n(n) {}

    template <typename ...P>
    consteval operator A<P...>() const
    {
        if (n != sizeof...(P))
            throw "up";
        return {};
    }
};

template <typename T>
struct type_identity 
{
    using type = T;
};

template <typename ...P>
void foo(typename type_identity<A<P...>>::type a, P...) {}

int main()
{
    foo(B(0), 10);
}
```
But now duplicating `foo(B(0), 10);` in the `main` body removes those errors (!!).

Moreover, if I do `foo(B(0), 10); foo(B(1), 10);`, now the **second** call errors, not the first one. And if I remove the first one, the second compiles as expected.

This is completely busted. 🙂 


---

