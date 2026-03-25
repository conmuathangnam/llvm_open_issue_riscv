# [Clang17+] Regression when inheriting operators from recursive base classes

**Author:** eerkela
**URL:** https://github.com/llvm/llvm-project/issues/146614
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:17
**Closed Date:** 2025-07-18T07:23:33Z

## Body

Clang 17.0.1+ segfaults when compiling this simple definition of an overload set:

```
#include <iostream>
#include <utility>


template <typename F, typename... Fs>
struct overloads {
    std::decay_t<F> func;

    overloads(F&& f) : func(std::move(f)) {}

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (requires{{func(std::forward<A>(args)...)};})
    {
        return (func(std::forward<A>(args)...));
    }
};


template <typename F1, typename F2, typename... Fs>
struct overloads<F1, F2, Fs...> : overloads<F2, Fs...> {
    std::decay_t<F1> func;

    overloads(F1&& f1, F2&& f2, Fs&&... fs) :
        overloads<F2, Fs...>(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    using overloads<F2, Fs...>::operator();  // <- problem at this line

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (requires{{func(std::forward<A>(args)...)};})
    {
        return (func(std::forward<A>(args)...));
    }
};


template <typename... Fs>
overloads(Fs...) -> overloads<Fs...>;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout << func(1) << "\n";
    std::cout << func("abc") << "\n";
}
```

Previous versions of Clang (tested back to 10.0.0 via compiler explorer) and all versions of GCC 10.1+ compile and run just fine, but Clang 17.0.1 and beyond produces the following error:

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-17.0.1/bin/clang-17 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-17.0.1/lib/clang/17 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/backward -internal-isystem /opt/compiler-explorer/clang-17.0.1/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -fdebug-compilation-dir=/app -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-363665.o -x c++ <source>
1.	<source>:48:6: current parser token ';'
2.	<source>:44:12: parsing function body 'main'
3.	<source>:44:12: in compound statement ('{}')
4.	<source>:6:8: instantiating class definition 'overloads<(lambda at <source>:46:9), (lambda at <source>:47:9)>'
 #0 0x0000000003349f18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3349f18)
 #1 0x0000000003347adc SignalHandler(int) Signals.cpp:0:0
 #2 0x0000767963442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000000000643acd0 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, std::optional<unsigned int>&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643acd0)
 #4 0x00000000063aaf58 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
 #5 0x00000000063b2afc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #6 0x00000000063a9f86 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #7 0x00000000063ce908 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x00000000063a975e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x00000000063b4d65 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x00000000063a9842 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000063d886f clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63d886f)
#12 0x0000000005b67257 SubstituteConstraintExpression(clang::Sema&, clang::NamedDecl const*, clang::Expr const*) SemaConcept.cpp:0:0
#13 0x0000000005b69753 clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::NamedDecl const*, clang::Expr const*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5b69753)
#14 0x000000000617e373 clang::Sema::IsOverload(clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) (.part.0) SemaOverload.cpp:0:0
#15 0x000000000617ea82 clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&, clang::NamedDecl*&, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x617ea82)
#16 0x0000000005c34848 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5c34848)
#17 0x000000000643134d clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, std::optional<clang::ASTTemplateArgumentListInfo const*>, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643134d)
#18 0x000000000643660d clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643660d)
#19 0x00000000063c3405 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63c3405)
#20 0x00000000063df2e5 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63df2e5)
#21 0x000000000645ad1f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#22 0x0000000005a14cc1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5a14cc1)
#23 0x000000000645fa3d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fa3d)
#24 0x000000000645fac5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fac5)
#25 0x000000000645fb90 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fb90)
#26 0x0000000005c284ff clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5c284ff)
#27 0x00000000058e8ee1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58e8ee1)
#28 0x00000000058fca30 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fca30)
#29 0x00000000058fddfd clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fddfd)
#30 0x00000000058fe2df clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fe2df)
#31 0x00000000059a9a32 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59a9a32)
#32 0x00000000059aa342 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59aa342)
#33 0x00000000059ab149 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59ab149)
#34 0x00000000059ac40a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59ac40a)
#35 0x00000000058cabc8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cabc8)
#36 0x00000000058fcb79 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fcb79)
#37 0x00000000058c5212 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58c5212)
#38 0x00000000058c632f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#39 0x00000000058cda8f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cda8f)
#40 0x00000000058cef0a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cef0a)
#41 0x00000000058be95a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58be95a)
#42 0x00000000044f8b05 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x44f8b05)
#43 0x0000000003dd5f21 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3dd5f21)
#44 0x0000000003d5b9fb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3d5b9fb)
#45 0x0000000003eb5c23 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3eb5c23)
#46 0x0000000000bd50c5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbd50c5)
#47 0x0000000000bce42d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#48 0x0000000000bd0f24 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbd0f24)
#49 0x0000000000acf511 main (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xacf511)
#50 0x0000767963429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x0000767963429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000bcdc2e _start (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbcdc2e)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

The only compile option used was `-std=c++20` to access the `requires` clause for the call operators.  Interestingly, if the `requires` clause is removed from the base class, then the problem resolves itself for the problematic releases:

```
#include <iostream>
#include <utility>


template <typename F, typename... Fs>
struct overloads {
    std::decay_t<F> func;

    overloads(F&& f) : func(std::move(f)) {}

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        // requires (requires{{func(std::forward<A>(args)...)};})  // commenting this out fixes the problem
    {
        return (func(std::forward<A>(args)...));
    }
};


template <typename F1, typename F2, typename... Fs>
struct overloads<F1, F2, Fs...> : overloads<F2, Fs...> {
    std::decay_t<F1> func;

    overloads(F1&& f1, F2&& f2, Fs&&... fs) :
        overloads<F2, Fs...>(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    using overloads<F2, Fs...>::operator();  // <- problem at this line

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (requires{{func(std::forward<A>(args)...)};})
    {
        return (func(std::forward<A>(args)...));
    }
};


template <typename... Fs>
overloads(Fs...) -> overloads<Fs...>;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout << func(1) << "\n";
    std::cout << func("abc") << "\n";
}
```

Which suggests the regression has something to do with applying template constraints for recursively inherited methods with a `using` statement.

I encountered this problem while trying to build a simple overload set that retains lvalue references for the component functions, which currently seems to be impossible in clang due to this bug.  A simpler fold expression such as this:

```
template <typename... Fs>
struct overloads : public Fs... { using Fs::operator()...; };
```

... Would work, but the inheritance forces all `Fs...` to be non-reference types, therefore forcing an extra copy when the overload set is constructed, which I'd like to avoid as long as lifetimes would not be an issue.  Is there any workaround while this bug is being resolved, or hope of a backport to the problematic releases once it's fixed?

Godbolt:
https://godbolt.org/z/W5hrb4559

Thank you for all your work!

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Erkela (eerkela)

<details>
Clang 17.0.1+ segfaults when compiling this simple definition of an overload set:

```
#include &lt;iostream&gt;
#include &lt;utility&gt;


template &lt;typename F, typename... Fs&gt;
struct overloads {
    std::decay_t&lt;F&gt; func;

    overloads(F&amp;&amp; f) : func(std::move(f)) {}

    template &lt;typename... A&gt;
    decltype(auto) operator()(A&amp;&amp;... args)
        requires (requires{{func(std::forward&lt;A&gt;(args)...)};})
    {
        return (func(std::forward&lt;A&gt;(args)...));
    }
};


template &lt;typename F1, typename F2, typename... Fs&gt;
struct overloads&lt;F1, F2, Fs...&gt; : overloads&lt;F2, Fs...&gt; {
    std::decay_t&lt;F1&gt; func;

    overloads(F1&amp;&amp; f1, F2&amp;&amp; f2, Fs&amp;&amp;... fs) :
        overloads&lt;F2, Fs...&gt;(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    using overloads&lt;F2, Fs...&gt;::operator();  // &lt;- problem at this line

    template &lt;typename... A&gt;
    decltype(auto) operator()(A&amp;&amp;... args)
        requires (requires{{func(std::forward&lt;A&gt;(args)...)};})
    {
        return (func(std::forward&lt;A&gt;(args)...));
    }
};


template &lt;typename... Fs&gt;
overloads(Fs...) -&gt; overloads&lt;Fs...&gt;;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout &lt;&lt; func(1) &lt;&lt; "\n";
    std::cout &lt;&lt; func("abc") &lt;&lt; "\n";
}
```

Previous versions of Clang (tested back to 10.0.0 via compiler explorer) and all versions of GCC 10.1+ compile and run just fine, but Clang 17.0.1 and beyond produces the following error:

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-17.0.1/bin/clang-17 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-17.0.1/lib/clang/17 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/backward -internal-isystem /opt/compiler-explorer/clang-17.0.1/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -fdebug-compilation-dir=/app -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-363665.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:48:6: current parser token ';'
2.	&lt;source&gt;:44:12: parsing function body 'main'
3.	&lt;source&gt;:44:12: in compound statement ('{}')
4.	&lt;source&gt;:6:8: instantiating class definition 'overloads&lt;(lambda at &lt;source&gt;:46:9), (lambda at &lt;source&gt;:47:9)&gt;'
 #<!-- -->0 0x0000000003349f18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3349f18)
 #<!-- -->1 0x0000000003347adc SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x0000767963442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000000000643acd0 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef&lt;std::pair&lt;llvm::PointerUnion&lt;clang::TemplateTypeParmType const*, clang::NamedDecl*&gt;, clang::SourceLocation&gt;&gt;, clang::MultiLevelTemplateArgumentList const&amp;, bool&amp;, bool&amp;, std::optional&lt;unsigned int&gt;&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643acd0)
 #<!-- -->4 0x00000000063aaf58 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->5 0x00000000063b2afc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x00000000063a9f86 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x00000000063ce908 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x00000000063a975e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x00000000063b4d65 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x00000000063a9842 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x00000000063d886f clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63d886f)
#<!-- -->12 0x0000000005b67257 SubstituteConstraintExpression(clang::Sema&amp;, clang::NamedDecl const*, clang::Expr const*) SemaConcept.cpp:0:0
#<!-- -->13 0x0000000005b69753 clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::NamedDecl const*, clang::Expr const*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5b69753)
#<!-- -->14 0x000000000617e373 clang::Sema::IsOverload(clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) (.part.0) SemaOverload.cpp:0:0
#<!-- -->15 0x000000000617ea82 clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&amp;, clang::NamedDecl*&amp;, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x617ea82)
#<!-- -->16 0x0000000005c34848 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&amp;, bool, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5c34848)
#<!-- -->17 0x000000000643134d clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, std::optional&lt;clang::ASTTemplateArgumentListInfo const*&gt;, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643134d)
#<!-- -->18 0x000000000643660d clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x643660d)
#<!-- -->19 0x00000000063c3405 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63c3405)
#<!-- -->20 0x00000000063df2e5 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x63df2e5)
#<!-- -->21 0x000000000645ad1f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->22 0x0000000005a14cc1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5a14cc1)
#<!-- -->23 0x000000000645fa3d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fa3d)
#<!-- -->24 0x000000000645fac5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fac5)
#<!-- -->25 0x000000000645fb90 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x645fb90)
#<!-- -->26 0x0000000005c284ff clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x5c284ff)
#<!-- -->27 0x00000000058e8ee1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58e8ee1)
#<!-- -->28 0x00000000058fca30 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fca30)
#<!-- -->29 0x00000000058fddfd clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fddfd)
#<!-- -->30 0x00000000058fe2df clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fe2df)
#<!-- -->31 0x00000000059a9a32 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59a9a32)
#<!-- -->32 0x00000000059aa342 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59aa342)
#<!-- -->33 0x00000000059ab149 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59ab149)
#<!-- -->34 0x00000000059ac40a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x59ac40a)
#<!-- -->35 0x00000000058cabc8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cabc8)
#<!-- -->36 0x00000000058fcb79 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58fcb79)
#<!-- -->37 0x00000000058c5212 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58c5212)
#<!-- -->38 0x00000000058c632f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->39 0x00000000058cda8f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cda8f)
#<!-- -->40 0x00000000058cef0a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58cef0a)
#<!-- -->41 0x00000000058be95a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x58be95a)
#<!-- -->42 0x00000000044f8b05 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x44f8b05)
#<!-- -->43 0x0000000003dd5f21 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3dd5f21)
#<!-- -->44 0x0000000003d5b9fb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3d5b9fb)
#<!-- -->45 0x0000000003eb5c23 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0x3eb5c23)
#<!-- -->46 0x0000000000bd50c5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbd50c5)
#<!-- -->47 0x0000000000bce42d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->48 0x0000000000bd0f24 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbd0f24)
#<!-- -->49 0x0000000000acf511 main (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xacf511)
#<!-- -->50 0x0000767963429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x0000767963429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000bcdc2e _start (/opt/compiler-explorer/clang-17.0.1/bin/clang-17+0xbcdc2e)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

The only compile option used was `-std=c++20` to access the `requires` clause for the call operators.  Interestingly, if the `requires` clause is removed from the base class, then the problem resolves itself for the problematic releases:

```
#include &lt;iostream&gt;
#include &lt;utility&gt;


template &lt;typename F, typename... Fs&gt;
struct overloads {
    std::decay_t&lt;F&gt; func;

    overloads(F&amp;&amp; f) : func(std::move(f)) {}

    template &lt;typename... A&gt;
    decltype(auto) operator()(A&amp;&amp;... args)
        // requires (requires{{func(std::forward&lt;A&gt;(args)...)};})  // commenting this out fixes the problem
    {
        return (func(std::forward&lt;A&gt;(args)...));
    }
};


template &lt;typename F1, typename F2, typename... Fs&gt;
struct overloads&lt;F1, F2, Fs...&gt; : overloads&lt;F2, Fs...&gt; {
    std::decay_t&lt;F1&gt; func;

    overloads(F1&amp;&amp; f1, F2&amp;&amp; f2, Fs&amp;&amp;... fs) :
        overloads&lt;F2, Fs...&gt;(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    using overloads&lt;F2, Fs...&gt;::operator();  // &lt;- problem at this line

    template &lt;typename... A&gt;
    decltype(auto) operator()(A&amp;&amp;... args)
        requires (requires{{func(std::forward&lt;A&gt;(args)...)};})
    {
        return (func(std::forward&lt;A&gt;(args)...));
    }
};


template &lt;typename... Fs&gt;
overloads(Fs...) -&gt; overloads&lt;Fs...&gt;;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout &lt;&lt; func(1) &lt;&lt; "\n";
    std::cout &lt;&lt; func("abc") &lt;&lt; "\n";
}
```

Which suggests the regression has something to do with applying template constraints for recursively inherited methods with a `using` statement.

I encountered this problem while trying to build a simple overload set that retains lvalue references for the component functions, which currently seems to be impossible in clang due to this bug.  A simpler fold expression such as this:

```
template &lt;typename... Fs&gt;
struct overloads : public Fs... { using Fs::operator()...; };
```

... Would work, but the inheritance forces all `Fs...` to be non-reference types, therefore forcing an extra copy when the overload set is constructed, which I'd like to avoid as long as lifetimes would not be an issue.  Is there any workaround while this bug is being resolved, or hope of a backport to the problematic releases once it's fixed?

Godbolt:
https://godbolt.org/z/W5hrb4559

Thank you for all your work!
</details>


---

### Comment 2 - shafik

With assertions: https://godbolt.org/z/37fEdvWrh

Using assertions builds tends to be more helpful, they usually crash earlier and give us a better indication of the root issue.

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4644: 
llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::ValueDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	<source>:48:6: current parser token ';'
2.	<source>:44:12: parsing function body 'main'
3.	<source>:44:12: in compound statement ('{}')
4.	<source>:6:8: instantiating class definition 'overloads<(lambda at <source>:46:9), (lambda at <source>:47:9)>'
 #0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78848)
 #1 0x0000000003f764d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f764d4)
 #2 0x0000000003ebb048 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076c106042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076c1060969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076c106042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076c1060287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076c10602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076c106039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007149a5d clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7149a5d)
#10 0x0000000007217128 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, clang::UnsignedOrNone&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7217128)
#11 0x000000000717a414 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000717e5a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007151d02 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000071770ab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007179586 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000715206e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000071588d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007152020 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000715b5bf clang::Sema::SubstConstraintExprWithoutSatisfaction(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715b5bf)
#20 0x000000000690600b SubstituteConstraintExpressionWithoutSatisfaction(clang::Sema&, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) SemaConcept.cpp:0:0
#21 0x000000000691197a clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691197a)
#22 0x0000000006ed2eb4 IsOverloadOrOverrideImpl(clang::Sema&, clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) SemaOverload.cpp:0:0
#23 0x0000000006ed344d clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&, clang::NamedDecl*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ed344d)
#24 0x00000000069fbd1e clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69fbd1e)
#25 0x000000000720ac78 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720ac78)
#26 0x000000000720e5b5 clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720e5b5)
#27 0x0000000007168193 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7168193)
#28 0x000000000719f87b clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x719f87b)
#29 0x000000000721e61e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#30 0x0000000007eadef1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eadef1)
#31 0x00000000072273ca clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72273ca)
#32 0x00000000072279c5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72279c5)
#33 0x0000000007227b45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7227b45)
#34 0x00000000069f0c68 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f0c68)
#35 0x000000000665977d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665977d)
#36 0x0000000006668e79 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6668e79)
#37 0x0000000006671b47 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6671b47)
#38 0x0000000006672009 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6672009)
#39 0x0000000006731f72 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6731f72)
#40 0x0000000006732c4d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6732c4d)
#41 0x000000000673ab73 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ab73)
#42 0x000000000673b33a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673b33a)
#43 0x0000000006633bd3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633bd3)
#44 0x000000000666987d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666987d)
#45 0x00000000066276be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66276be)
#46 0x0000000006627e69 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6627e69)
#47 0x000000000662f7aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f7aa)
#48 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663077d)
#49 0x0000000006622aea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6622aea)
#50 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492ed48)
#51 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c28c65)
#52 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba5ffe)
#53 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1b441)
#54 0x0000000000d8e81f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e81f)
#55 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#56 0x0000000004995b49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#57 0x0000000003ebb4e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebb4e4)
#58 0x000000000499615f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#59 0x0000000004957f8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957f8d)
#60 0x000000000495901e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495901e)
#61 0x00000000049614d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49614d5)
#62 0x0000000000d8accf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8accf)
#63 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc41924)
#64 0x000076c106029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#65 0x000076c106029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#66 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84ea5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```



---

### Comment 3 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/139940

CC @mizvekov 

---

### Comment 4 - eerkela

Regarding a workaround for the affected versions, the following compiles and accomplishes the same goal, albeit with slightly worse diagnostics if something goes wrong:

```
#include <concepts>
#include <iostream>
#include <utility>


template <typename F, typename... Fs>
struct overloads {
    std::decay_t<F> func;

    overloads(F&& f) : func(std::move(f)) {}

    template <typename... A> requires (std::invocable<F&, A...>)
    decltype(auto) operator()(A&&... args) {
        return (func(std::forward<A>(args)...));
    }
};


template <typename F1, typename F2, typename... Fs>
struct overloads<F1, F2, Fs...> : overloads<F2, Fs...> {
    std::decay_t<F1> func;

    overloads(F1&& f1, F2&& f2, Fs&&... fs) :
        overloads<F2, Fs...>(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (
            std::invocable<F1&, A...> &&
            !std::invocable<overloads<F2, Fs...>&, A...>
        )
    {
        return (func(std::forward<A>(args)...));
    }

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (
            !std::invocable<F1&, A...> &&
            std::invocable<overloads<F2, Fs...>&, A...>
        )
    {
        return (overloads<F2, Fs...>::operator()(std::forward<A>(args)...));
    }
};


template <typename... Fs>
overloads(Fs...) -> overloads<Fs...>;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout << func(1) << "\n";
    std::cout << func("abc") << "\n";
}
```

I also wrote a more complicated implementation that uses an explicit index to select a matching overload, in case anyone is interested

```
#include <concepts>
#include <iostream>
#include <utility>


// recursive base classes are separated from final interface.  The final
// overload is chosen by explicit index
template <typename F, typename... Fs>
struct overloads_base {
    std::decay_t<F> func;

    overloads_base(F&& f) : func(std::move(f)) {}

    template <size_t I, typename... A> requires (std::invocable<F&, A...>)
    decltype(auto) operator()(A&&... args) {
        return (func(std::forward<A>(args)...));
    }
};


template <typename F1, typename F2, typename... Fs>
struct overloads_base<F1, F2, Fs...> : overloads_base<F2, Fs...> {
    std::decay_t<F1> func;

    overloads_base(F1&& f1, F2&& f2, Fs&&... fs) :
        overloads_base<F2, Fs...>(std::move(f2), std::move(fs)...),
        func(std::move(f1))
    {}

    template <size_t I, typename... A>
    decltype(auto) operator()(A&&... args) {
        if constexpr (I == 0) {
            return (func(std::forward<A>(args)...));
        } else {
            return (overloads_base<F2, Fs...>::template operator()<I - 1>(
                std::forward<A>(args)...
            ));
        }
    }
};


// a metaprogram over the arguments `A...` that identifies a matching overload
// in `Fs...` by index, returning `sizeof...(Fs)` in case of failure
template <typename... A>
struct find_overload {
    template <typename...>
    static constexpr bool ambiguous = false;
    template <std::invocable<A...> F, typename... Fs>
    static constexpr bool ambiguous<F, Fs...> = true;
    template <typename F, typename... Fs>
    static constexpr bool ambiguous<F, Fs...> = ambiguous<Fs...>;

    template <typename...>
    static constexpr size_t index = 0;
    template <std::invocable<A...> F, typename... Fs>
    static constexpr size_t index<F, Fs...> =
        !ambiguous<Fs...> ? 0 : sizeof...(Fs) + 1;  // reject ambiguous overloads
    template <typename F, typename... Fs>
    static constexpr size_t index<F, Fs...> = index<Fs...> + 1;
};


template <typename F, typename... Fs>
struct overloads : overloads_base<F, Fs...> {
    overloads(F&& f, Fs&&... fs) :
        overloads_base<F, Fs...>(std::move(f), std::move(fs)...)
    {}

    template <typename... A>
    decltype(auto) operator()(A&&... args)
        requires (find_overload<A...>::template index<F, Fs...> < (sizeof...(Fs) + 1))
    {
        constexpr size_t I = find_overload<A...>::template index<F, Fs...>;
        return overloads_base<F, Fs...>::template operator()<I>(std::forward<A>(args)...);
    }
};


template <typename... Fs>
overloads(Fs...) -> overloads<Fs...>;


int main() {
    overloads func {
        [](int x) { return 0; },
        [](const char* y) { return 1; }
    };
    std::cout << func(1) << "\n";
    std::cout << func("abc") << "\n";
}
```

https://godbolt.org/z/4jMbz8ddq

---

### Comment 5 - eerkela

I was able to reduce the bug down to just this:

```
template <typename T>
struct base {
    template <typename A>
    void foo(A x)
        requires (requires{x;})  // buggy version - consistently crashes on clang 17.0.1+
    {}
};


struct child : base<int> {
    using base<int>::foo;  // interaction with requires clause on base class triggers the bug

    template <typename A>
    void foo(A x)
        requires (false)  // this clause must be present, but the condition is irrelevant
    {}
};
```

While doing that, I also discovered that the problem goes away if you add a parameter list to the base class's `requires` clause:

```
template <typename T>
struct base {
    template <typename A>
    void foo(A x)
        requires (requires(A x) {x;})  // adding a parameter list resolves the bug
    {}
};


struct child : base<int> {
    using base<int>::foo;  // interaction with requires clause on base class triggers the bug

    template <typename A>
    void foo(A x)
        requires (false)  // this clause must be present, but the condition is irrelevant
    {}
};
```

So it seems like `Sema` is specifically failing to form an overload set between the child method and a base method that has been inherited via a `using` statement, but only when the base class and both methods are constrained templates, and the base method uses a trailing requires clause without any parameters.  It does not depend on the specific operator being inherited, the recursive nature of the inheritance, or the data members of either class - just the interaction between `using`, `requires`, and templated base/methods.  If either of the `using` statement, base class, or `foo` methods are omitted or reduced to non-templates, or the base requires clause is defined with an explicit parameter list, then the problem goes away (as far as I can tell).

https://godbolt.org/z/eTr9sTzTa

Here's the assertion output for the reduced demonstrator:

```
Compiler returned: 254
Compiler stderr
clang-17: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4181: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-17.0.1/lib/clang/17 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0 -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-17.0.1/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -fdebug-compilation-dir=/app -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-8e712c.o -x c++ <source>
1.	<source>:17:5: current parser token '{'
2.	<source>:11:1: parsing struct/union/class body 'child'
 #0 0x00000000036e7558 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x36e7558)
 #1 0x00000000036e4e4c SignalHandler(int) Signals.cpp:0:0
 #2 0x000070a14d642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000070a14d6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000070a14d642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000070a14d6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000070a14d62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000070a14d639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000068e3b85 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x68e3b85)
 #9 0x000000000696c80f clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x696c80f)
#10 0x00000000068d6711 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000690c570 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000690ca4b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000068eaef5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006913311 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006914d1e (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000068eab42 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000068f7bc8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000068eabdb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006925718 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x6925718)
#20 0x0000000006081b9f SubstituteConstraintExpression(clang::Sema&, clang::NamedDecl const*, clang::Expr const*) SemaConcept.cpp:0:0
#21 0x000000000608abab clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::NamedDecl const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x608abab)
#22 0x00000000066b3d3b clang::Sema::IsOverload(clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) (.part.0) SemaOverload.cpp:0:0
#23 0x00000000066b488f clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&, clang::NamedDecl*&, bool) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x66b488f)
#24 0x0000000006155dc5 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x6155dc5)
#25 0x000000000615ccb7 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x615ccb7)
#26 0x0000000006160df3 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x6160df3)
#27 0x000000000622a379 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x622a379)
#28 0x0000000005e03f0c clang::Parser::ParseCXXInlineMethodDef(clang::AccessSpecifier, clang::ParsedAttributesView const&, clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::VirtSpecifiers const&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e03f0c)
#29 0x0000000005e4478c clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e4478c)
#30 0x0000000005ee691d clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5ee691d)
#31 0x0000000005eec631 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#32 0x0000000005e44b3c clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e44b3c)
#33 0x0000000005e46c25 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e46c25)
#34 0x0000000005e4851a clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e4851a)
#35 0x0000000005e4a510 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e4a510)
#36 0x0000000005e23d78 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5e23d78)
#37 0x0000000005defbd4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5defbd4)
#38 0x0000000005df04df clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#39 0x0000000005df6b51 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5df6b51)
#40 0x0000000005df7452 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5df7452)
#41 0x0000000005deb76a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x5deb76a)
#42 0x0000000004939258 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x4939258)
#43 0x00000000041a1c09 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x41a1c09)
#44 0x00000000041278ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x41278ce)
#45 0x0000000004281de6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0x4281de6)
#46 0x0000000000bc67eb cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0xbc67eb)
#47 0x0000000000bbefaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#48 0x0000000000bc3aea clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0xbc3aea)
#49 0x0000000000abef61 main (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0xabef61)
#50 0x000070a14d629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x000070a14d629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000bbea8e _start (/opt/compiler-explorer/clang-assertions-17.0.1/bin/clang-17+0xbbea8e)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

Hope this helps!

---

### Comment 6 - mizvekov

Yeah, this does not look related to https://github.com/llvm/llvm-project/issues/139940.

It looks like we are missing a transform of the function parameters in the same local instantiation context as we transform the constraint expression when we are checking if constraint expressions are equivalent.

Otherwise, we won't find the transformed parameters when we transform any entities which reference them.

This is sort of similar to https://github.com/llvm/llvm-project/issues/133792, but this is happening on separate parts of the code base, so these are orthogonal to fix.

---

