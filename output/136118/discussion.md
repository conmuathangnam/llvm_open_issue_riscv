# Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."` failed

**Author:** MacroModel
**URL:** https://github.com/llvm/llvm-project/issues/136118
**Status:** Closed
**Labels:** clang:frontend, concepts, confirmed, crash-on-valid, regression:16
**Closed Date:** 2025-10-10T12:40:08Z

## Body

```cpp
#include <cstddef>
#include <utility>
#include <type_traits>
template <typename... Fs>
inline consteval bool allow_multi_result_vector() noexcept
{
    return[]<::std::size_t... I>(::std::index_sequence<I...>) constexpr noexcept
    {
        return ((
            []<typename FsCurr>() constexpr noexcept
            -> bool
        {
            // check irreplaceable
            if constexpr (requires { requires ::std::same_as<::std::remove_cvref_t<decltype(FsCurr::allow_multi_result_vector)>, bool>; })
            {
                constexpr bool tallow{ FsCurr::allow_multi_result_vector };
                return tallow;
            }
            else { return false; }
        }.template operator() < Fs...[I] > ()) ||
            ...);
    }(::std::make_index_sequence<sizeof...(Fs)>{});
}

struct A
{
    inline static constexpr bool allow_multi_result_vector = true;
};

struct B
{
    inline static constexpr bool allow_multi_result_vector = false;
};

int main()
{
    constexpr auto i = allow_multi_result_vector<A, B>();
}

```

```bash
[MacroModel@MacroModel test]$ clang -v
clang version 21.0.0git (https://mirrors.tuna.tsinghua.edu.cn/git/llvm-project.git 95d526f7f587cc7a3db785169967f8dad0ba3978)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
Found candidate GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.0
Found candidate GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1
Selected GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1
Candidate multilib: .;@m64
Selected multilib: .;@m64
[MacroModel@MacroModel test]$ clang++ -o test test.cc -std=c++26
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir test- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cc -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/MacroModel/test -fcoverage-compilation-dir=/home/MacroModel/test -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-pc-linux-gnu -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-0b75f1.o -x c++ test.cc
1.      test.cc:36:56: current parser token ')'
2.      test.cc:35:1: parsing function body 'main'
3.      test.cc:35:1: in compound statement ('{}')
4.      test.cc:5:23: instantiating function definition 'allow_multi_result_vector<A, B>'
 #0 0x00007f79f8bc7518 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/lib/libLLVM.so.21.0git+0x3e81518)
 #1 0x00007f79f8bc519e llvm::sys::RunSignalHandlers() (/usr/local/lib/libLLVM.so.21.0git+0x3e7f19e)
 #2 0x00007f79f8bc7bc1 (/usr/local/lib/libLLVM.so.21.0git+0x3e81bc1)
 #3 0x00007f79f47eeae0 (/usr/lib/libc.so.6+0x3cae0)
 #4 0x00007f79fdfa3d05 (/usr/local/lib/libclang-cpp.so.21.0git+0x17b8d05)
 #5 0x00007f79fdf7e2ef (/usr/local/lib/libclang-cpp.so.21.0git+0x17932ef)
 #6 0x00007f79fdf7a1b2 (/usr/local/lib/libclang-cpp.so.21.0git+0x178f1b2)
 #7 0x00007f79fdf79b37 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/usr/local/lib/libclang-cpp.so.21.0git+0x178eb37)
 #8 0x00007f79fe67466d (/usr/local/lib/libclang-cpp.so.21.0git+0x1e8966d)
 #9 0x00007f79fe673bbb (/usr/local/lib/libclang-cpp.so.21.0git+0x1e88bbb)
#10 0x00007f79fec2639d (/usr/local/lib/libclang-cpp.so.21.0git+0x243b39d)
#11 0x00007f79fec1a0aa (/usr/local/lib/libclang-cpp.so.21.0git+0x242f0aa)
#12 0x00007f79fec3dec2 (/usr/local/lib/libclang-cpp.so.21.0git+0x2452ec2)
#13 0x00007f79fec3b873 (/usr/local/lib/libclang-cpp.so.21.0git+0x2450873)
#14 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#15 0x00007f79fec2ac53 (/usr/local/lib/libclang-cpp.so.21.0git+0x243fc53)
#16 0x00007f79fec1d4e2 (/usr/local/lib/libclang-cpp.so.21.0git+0x24324e2)
#17 0x00007f79fec21c6b (/usr/local/lib/libclang-cpp.so.21.0git+0x2436c6b)
#18 0x00007f79fec1f351 (/usr/local/lib/libclang-cpp.so.21.0git+0x2434351)
#19 0x00007f79fec1a513 (/usr/local/lib/libclang-cpp.so.21.0git+0x242f513)
#20 0x00007f79fec21544 (/usr/local/lib/libclang-cpp.so.21.0git+0x2436544)
#21 0x00007f79fec37145 (/usr/local/lib/libclang-cpp.so.21.0git+0x244c145)
#22 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#23 0x00007f79fec2ac53 (/usr/local/lib/libclang-cpp.so.21.0git+0x243fc53)
#24 0x00007f79fec1d4e2 (/usr/local/lib/libclang-cpp.so.21.0git+0x24324e2)
#25 0x00007f79fec1f351 (/usr/local/lib/libclang-cpp.so.21.0git+0x2434351)
#26 0x00007f79fec37145 (/usr/local/lib/libclang-cpp.so.21.0git+0x244c145)
#27 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#28 0x00007f79fec138ca clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/usr/local/lib/libclang-cpp.so.21.0git+0x24288ca)
#29 0x00007f79fec60179 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x2475179)
#30 0x00007f79fe8773ed (/usr/local/lib/libclang-cpp.so.21.0git+0x208c3ed)
#31 0x00007f79fda9140f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/local/lib/libclang-cpp.so.21.0git+0x12a640f)
#32 0x00007f79fe7e7d11 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x1ffcd11)
#33 0x00007f79fe7ec876 (/usr/local/lib/libclang-cpp.so.21.0git+0x2001876)
#34 0x00007f79fe7b6f2d clang::Sema::MarkDeclRefReferenced(clang::DeclRefExpr*, clang::Expr const*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fcbf2d)
#35 0x00007f79fe7b693f clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fcb93f)
#36 0x00007f79feaa00ee clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/usr/local/lib/libclang-cpp.so.21.0git+0x22b50ee)
#37 0x00007f79feaa197e (/usr/local/lib/libclang-cpp.so.21.0git+0x22b697e)
#38 0x00007f79feaa1893 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x22b6893)
#39 0x00007f79fe7b1593 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fc6593)
#40 0x00007f79fe7c493b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fd993b)
#41 0x00007f79fdc29f9e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/usr/local/lib/libclang-cpp.so.21.0git+0x143ef9e)
#42 0x00007f79fdc2cf54 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1441f54)
#43 0x00007f79fdc2d00f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/local/lib/libclang-cpp.so.21.0git+0x144200f)
#44 0x00007f79fdc27e48 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/local/lib/libclang-cpp.so.21.0git+0x143ce48)
#45 0x00007f79fdbfc027 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1411027)
#46 0x00007f79fdbf97ef clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140e7ef)
#47 0x00007f79fdbf8c03 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140dc03)
#48 0x00007f79fdbf8580 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140d580)
#49 0x00007f79fdc7ded6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/local/lib/libclang-cpp.so.21.0git+0x1492ed6)
#50 0x00007f79fdc7d726 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1492726)
#51 0x00007f79fdc8558e clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x149a58e)
#52 0x00007f79fdc8612a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/local/lib/libclang-cpp.so.21.0git+0x149b12a)
#53 0x00007f79fdc9d01a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b201a)
#54 0x00007f79fdbfa719 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140f719)
#55 0x00007f79fdc9c11d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b111d)
#56 0x00007f79fdc9bb09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b0b09)
#57 0x00007f79fdc9aef0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/lib/libclang-cpp.so.21.0git+0x14afef0)
#58 0x00007f79fdc99823 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/lib/libclang-cpp.so.21.0git+0x14ae823)
#59 0x00007f79fdbe59ae clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x13fa9ae)
#60 0x00007f79ff792a4d clang::FrontendAction::Execute() (/usr/local/lib/libclang-cpp.so.21.0git+0x2fa7a4d)
#61 0x00007f79ff706e84 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/lib/libclang-cpp.so.21.0git+0x2f1be84)
#62 0x00007f79ff8174aa clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/lib/libclang-cpp.so.21.0git+0x302c4aa)
#63 0x000055cb42c463db cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-21+0x123db)
#64 0x000055cb42c42f51 (/usr/local/bin/clang-21+0xef51)
#65 0x000055cb42c422bd clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-21+0xe2bd)
#66 0x000055cb42c50357 main (/usr/local/bin/clang-21+0x1c357)
#67 0x00007f79f47d7c88 (/usr/lib/libc.so.6+0x25c88)
#68 0x00007f79f47d7d4c __libc_start_main (/usr/lib/libc.so.6+0x25d4c)
#69 0x000055cb42c40d75 _start (/usr/local/bin/clang-21+0xcd75)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (https://mirrors.tuna.tsinghua.edu.cn/git/llvm-project.git 95d526f7f587cc7a3db785169967f8dad0ba3978)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-144021.cpp
clang++: note: diagnostic msg: /tmp/test-144021.sh
clang++: note: diagnostic msg:

********************
```

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MacroModel)

<details>
```cpp
#include &lt;cstddef&gt;
#include &lt;utility&gt;
#include &lt;type_traits&gt;
template &lt;typename... Fs&gt;
inline consteval bool allow_multi_result_vector() noexcept
{
    return[]&lt;::std::size_t... I&gt;(::std::index_sequence&lt;I...&gt;) constexpr noexcept
    {
        return ((
            []&lt;typename FsCurr&gt;() constexpr noexcept
            -&gt; bool
        {
            // check irreplaceable
            if constexpr (requires { requires ::std::same_as&lt;::std::remove_cvref_t&lt;decltype(FsCurr::allow_multi_result_vector)&gt;, bool&gt;; })
            {
                constexpr bool tallow{ FsCurr::allow_multi_result_vector };
                return tallow;
            }
            else { return false; }
        }.template operator() &lt; Fs...[I] &gt; ()) ||
            ...);
    }(::std::make_index_sequence&lt;sizeof...(Fs)&gt;{});
}

struct A
{
    inline static constexpr bool allow_multi_result_vector = true;
};

struct B
{
    inline static constexpr bool allow_multi_result_vector = false;
};

int main()
{
    constexpr auto i = allow_multi_result_vector&lt;A, B&gt;();
}

```

```bash
[MacroModel@<!-- -->MacroModel test]$ clang -v
clang version 21.0.0git (https://mirrors.tuna.tsinghua.edu.cn/git/llvm-project.git 95d526f7f587cc7a3db785169967f8dad0ba3978)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
Found candidate GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.0
Found candidate GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1
Selected GCC installation: /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
[MacroModel@<!-- -->MacroModel test]$ clang++ -o test test.cc -std=c++26
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir test- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cc -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/MacroModel/test -fcoverage-compilation-dir=/home/MacroModel/test -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-pc-linux-gnu -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/local/bin/../lib/gcc/x86_64-pc-linux-gnu/15.0.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-0b75f1.o -x c++ test.cc
1.      test.cc:36:56: current parser token ')'
2.      test.cc:35:1: parsing function body 'main'
3.      test.cc:35:1: in compound statement ('{}')
4.      test.cc:5:23: instantiating function definition 'allow_multi_result_vector&lt;A, B&gt;'
 #<!-- -->0 0x00007f79f8bc7518 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/lib/libLLVM.so.21.0git+0x3e81518)
 #<!-- -->1 0x00007f79f8bc519e llvm::sys::RunSignalHandlers() (/usr/local/lib/libLLVM.so.21.0git+0x3e7f19e)
 #<!-- -->2 0x00007f79f8bc7bc1 (/usr/local/lib/libLLVM.so.21.0git+0x3e81bc1)
 #<!-- -->3 0x00007f79f47eeae0 (/usr/lib/libc.so.6+0x3cae0)
 #<!-- -->4 0x00007f79fdfa3d05 (/usr/local/lib/libclang-cpp.so.21.0git+0x17b8d05)
 #<!-- -->5 0x00007f79fdf7e2ef (/usr/local/lib/libclang-cpp.so.21.0git+0x17932ef)
 #<!-- -->6 0x00007f79fdf7a1b2 (/usr/local/lib/libclang-cpp.so.21.0git+0x178f1b2)
 #<!-- -->7 0x00007f79fdf79b37 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/usr/local/lib/libclang-cpp.so.21.0git+0x178eb37)
 #<!-- -->8 0x00007f79fe67466d (/usr/local/lib/libclang-cpp.so.21.0git+0x1e8966d)
 #<!-- -->9 0x00007f79fe673bbb (/usr/local/lib/libclang-cpp.so.21.0git+0x1e88bbb)
#<!-- -->10 0x00007f79fec2639d (/usr/local/lib/libclang-cpp.so.21.0git+0x243b39d)
#<!-- -->11 0x00007f79fec1a0aa (/usr/local/lib/libclang-cpp.so.21.0git+0x242f0aa)
#<!-- -->12 0x00007f79fec3dec2 (/usr/local/lib/libclang-cpp.so.21.0git+0x2452ec2)
#<!-- -->13 0x00007f79fec3b873 (/usr/local/lib/libclang-cpp.so.21.0git+0x2450873)
#<!-- -->14 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#<!-- -->15 0x00007f79fec2ac53 (/usr/local/lib/libclang-cpp.so.21.0git+0x243fc53)
#<!-- -->16 0x00007f79fec1d4e2 (/usr/local/lib/libclang-cpp.so.21.0git+0x24324e2)
#<!-- -->17 0x00007f79fec21c6b (/usr/local/lib/libclang-cpp.so.21.0git+0x2436c6b)
#<!-- -->18 0x00007f79fec1f351 (/usr/local/lib/libclang-cpp.so.21.0git+0x2434351)
#<!-- -->19 0x00007f79fec1a513 (/usr/local/lib/libclang-cpp.so.21.0git+0x242f513)
#<!-- -->20 0x00007f79fec21544 (/usr/local/lib/libclang-cpp.so.21.0git+0x2436544)
#<!-- -->21 0x00007f79fec37145 (/usr/local/lib/libclang-cpp.so.21.0git+0x244c145)
#<!-- -->22 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#<!-- -->23 0x00007f79fec2ac53 (/usr/local/lib/libclang-cpp.so.21.0git+0x243fc53)
#<!-- -->24 0x00007f79fec1d4e2 (/usr/local/lib/libclang-cpp.so.21.0git+0x24324e2)
#<!-- -->25 0x00007f79fec1f351 (/usr/local/lib/libclang-cpp.so.21.0git+0x2434351)
#<!-- -->26 0x00007f79fec37145 (/usr/local/lib/libclang-cpp.so.21.0git+0x244c145)
#<!-- -->27 0x00007f79fec2411b (/usr/local/lib/libclang-cpp.so.21.0git+0x243911b)
#<!-- -->28 0x00007f79fec138ca clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/local/lib/libclang-cpp.so.21.0git+0x24288ca)
#<!-- -->29 0x00007f79fec60179 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x2475179)
#<!-- -->30 0x00007f79fe8773ed (/usr/local/lib/libclang-cpp.so.21.0git+0x208c3ed)
#<!-- -->31 0x00007f79fda9140f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/local/lib/libclang-cpp.so.21.0git+0x12a640f)
#<!-- -->32 0x00007f79fe7e7d11 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x1ffcd11)
#<!-- -->33 0x00007f79fe7ec876 (/usr/local/lib/libclang-cpp.so.21.0git+0x2001876)
#<!-- -->34 0x00007f79fe7b6f2d clang::Sema::MarkDeclRefReferenced(clang::DeclRefExpr*, clang::Expr const*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fcbf2d)
#<!-- -->35 0x00007f79fe7b693f clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fcb93f)
#<!-- -->36 0x00007f79feaa00ee clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/usr/local/lib/libclang-cpp.so.21.0git+0x22b50ee)
#<!-- -->37 0x00007f79feaa197e (/usr/local/lib/libclang-cpp.so.21.0git+0x22b697e)
#<!-- -->38 0x00007f79feaa1893 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x22b6893)
#<!-- -->39 0x00007f79fe7b1593 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fc6593)
#<!-- -->40 0x00007f79fe7c493b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1fd993b)
#<!-- -->41 0x00007f79fdc29f9e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/local/lib/libclang-cpp.so.21.0git+0x143ef9e)
#<!-- -->42 0x00007f79fdc2cf54 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1441f54)
#<!-- -->43 0x00007f79fdc2d00f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/local/lib/libclang-cpp.so.21.0git+0x144200f)
#<!-- -->44 0x00007f79fdc27e48 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/local/lib/libclang-cpp.so.21.0git+0x143ce48)
#<!-- -->45 0x00007f79fdbfc027 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1411027)
#<!-- -->46 0x00007f79fdbf97ef clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140e7ef)
#<!-- -->47 0x00007f79fdbf8c03 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140dc03)
#<!-- -->48 0x00007f79fdbf8580 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140d580)
#<!-- -->49 0x00007f79fdc7ded6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/local/lib/libclang-cpp.so.21.0git+0x1492ed6)
#<!-- -->50 0x00007f79fdc7d726 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/lib/libclang-cpp.so.21.0git+0x1492726)
#<!-- -->51 0x00007f79fdc8558e clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x149a58e)
#<!-- -->52 0x00007f79fdc8612a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/lib/libclang-cpp.so.21.0git+0x149b12a)
#<!-- -->53 0x00007f79fdc9d01a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b201a)
#<!-- -->54 0x00007f79fdbfa719 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/lib/libclang-cpp.so.21.0git+0x140f719)
#<!-- -->55 0x00007f79fdc9c11d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b111d)
#<!-- -->56 0x00007f79fdc9bb09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/lib/libclang-cpp.so.21.0git+0x14b0b09)
#<!-- -->57 0x00007f79fdc9aef0 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/lib/libclang-cpp.so.21.0git+0x14afef0)
#<!-- -->58 0x00007f79fdc99823 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/lib/libclang-cpp.so.21.0git+0x14ae823)
#<!-- -->59 0x00007f79fdbe59ae clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/lib/libclang-cpp.so.21.0git+0x13fa9ae)
#<!-- -->60 0x00007f79ff792a4d clang::FrontendAction::Execute() (/usr/local/lib/libclang-cpp.so.21.0git+0x2fa7a4d)
#<!-- -->61 0x00007f79ff706e84 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/lib/libclang-cpp.so.21.0git+0x2f1be84)
#<!-- -->62 0x00007f79ff8174aa clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/lib/libclang-cpp.so.21.0git+0x302c4aa)
#<!-- -->63 0x000055cb42c463db cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0x123db)
#<!-- -->64 0x000055cb42c42f51 (/usr/local/bin/clang-21+0xef51)
#<!-- -->65 0x000055cb42c422bd clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe2bd)
#<!-- -->66 0x000055cb42c50357 main (/usr/local/bin/clang-21+0x1c357)
#<!-- -->67 0x00007f79f47d7c88 (/usr/lib/libc.so.6+0x25c88)
#<!-- -->68 0x00007f79f47d7d4c __libc_start_main (/usr/lib/libc.so.6+0x25d4c)
#<!-- -->69 0x000055cb42c40d75 _start (/usr/local/bin/clang-21+0xcd75)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (https://mirrors.tuna.tsinghua.edu.cn/git/llvm-project.git 95d526f7f587cc7a3db785169967f8dad0ba3978)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-144021.cpp
clang++: note: diagnostic msg: /tmp/test-144021.sh
clang++: note: diagnostic msg:

********************
```

```
</details>


---

### Comment 2 - Endilll

Confirmed: https://godbolt.org/z/f4xYYobKs
Clang 21 crash:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:16973:
bool clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, const clang::ASTContext&, clang::Expr::ConstantExprKind) const: 
Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2c <source>
1.	<source>:37:56: current parser token ')'
2.	<source>:36:1: parsing function body 'main'
3.	<source>:36:1: in compound statement ('{}')
4.	<source>:5:23: instantiating function definition 'allow_multi_result_vector<A, B>'
 #0 0x0000000003ef55b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef55b8)
 #1 0x0000000003ef3244 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef3244)
 #2 0x0000000003e37d78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000711043042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007110430969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000711043042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007110430287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071104302871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000711043039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000783012a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x783012a)
#10 0x0000000006843df9 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#11 0x0000000006845518 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#12 0x00000000068459f2 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68459f2)
#13 0x000000000712d149 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000712e6e6 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000070f5939 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000070f5b00 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000713cdb6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007135c1e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000070f46cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000070f51f2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000070f577f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000712381c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
#23 0x00000000070f551f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000007102126 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000070f55f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x00000000070fec98 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#27 0x00000000070f58d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#28 0x0000000007102d39 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#29 0x00000000070f5537 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#30 0x00000000070f7785 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#31 0x00000000070f7b2c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000007135c1e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#33 0x00000000070f46cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#34 0x00000000070f51f2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#35 0x00000000070f577f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#36 0x0000000007102126 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#37 0x00000000070f55f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#38 0x00000000070f7785 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#39 0x00000000070f7b2c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#40 0x0000000007135c1e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#41 0x000000000713ef34 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713ef34)
#42 0x000000000719f457 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x719f457)
#43 0x0000000006a7e80e void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
#44 0x0000000007e2d651 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e2d651)
#45 0x0000000006a7c94f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7c94f)
#46 0x0000000006a7cfcd MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#47 0x0000000006a6d059 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6d059)
#48 0x0000000006e6b0fd clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6b0fd)
#49 0x0000000006ea1b7e FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#50 0x0000000006ea2c60 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea2c60)
#51 0x0000000006aa2266 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2266)
#52 0x0000000006aa30ee clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa30ee)
#53 0x00000000065d96ad clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d96ad)
#54 0x00000000065d20ba clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d20ba)
#55 0x00000000065d232e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d232e)
#56 0x00000000065d4257 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d4257)
#57 0x00000000065d42e9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d42e9)
#58 0x000000000658cd28 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658cd28)
#59 0x000000000659bd69 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659bd69)
#60 0x00000000065a4a27 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a4a27)
#61 0x00000000065a4ee9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a4ee9)
#62 0x0000000006654742 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6654742)
#63 0x000000000665544d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665544d)
#64 0x000000000665d0e3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665d0e3)
#65 0x000000000665d8aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665d8aa)
#66 0x00000000065666a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65666a3)
#67 0x000000000659c76d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659c76d)
#68 0x000000000655a1de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655a1de)
#69 0x000000000655a999 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655a999)
#70 0x00000000065622ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65622ba)
#71 0x000000000656325d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x656325d)
#72 0x000000000655563a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655563a)
#73 0x0000000004894938 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4894938)
#74 0x0000000004b87cd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87cd5)
#75 0x0000000004b06a9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b06a9e)
#76 0x0000000004c76bce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c76bce)
#77 0x0000000000d9ff3f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9ff3f)
#78 0x0000000000d96f4a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#79 0x00000000048fb1b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#80 0x0000000003e38214 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e38214)
#81 0x00000000048fb7cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#82 0x00000000048bdc2d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bdc2d)
#83 0x00000000048becbe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48becbe)
#84 0x00000000048c6b15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c6b15)
#85 0x0000000000d9cc88 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9cc88)
#86 0x0000000000c241d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc241d4)
#87 0x0000711043029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#88 0x0000711043029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#89 0x0000000000d969f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd969f5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - Endilll

Reduced by me and C-Reduce: https://godbolt.org/z/4esWerzK5
```cpp
template <typename... Fs> 
consteval int allow_multi_result_vector() {
  [] <int... I> {
    ([]<typename FsCurr> {
      requires {
        requires __is_same(decltype(FsCurr::allow_multi_result_vector), int);
      };
    }.template operator()<Fs...[I]>() ||
     ...);
  };
}

template int allow_multi_result_vector<>();
```

---

### Comment 4 - Endilll

CC @cor3ntin 

---

### Comment 5 - zyn0217

Reduced further:

```cpp
template <typename Fs>
int allow_multi_result_vector() {
  ([]<typename FsCurr> {
    requires {
      requires __is_same(decltype(FsCurr::allow_multi_result_vector), int);
    };
  }.template operator()<Fs>());
}

template int allow_multi_result_vector<int>();
```

https://godbolt.org/z/PrjfWY8x1

This is unrelated to pack indexing but rather deferred constraint evaluation: the requires expressions remain untransformed until the final instantiation, at which point we sadly don't have full-level template arguments to go with.

---

### Comment 6 - Endilll

Hmm, seems to be a Clang 16 regression: https://godbolt.org/z/MeYe1ox73

---

### Comment 7 - shafik

There are a lot w/ this same assertion but the backtraces are all a little different, this one though seems similar enough to maybe be related: https://github.com/llvm/llvm-project/issues/94811

---

### Comment 8 - shafik

git bisect points to 13d44a8f5647819efd3ef57af384a3666f99d066 which is a revert unfortunately

---

### Comment 9 - cor3ntin

Fixed in trunk, probably by #161671 

---

