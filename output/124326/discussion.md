# Compiler crash while using variants.

**Author:** bogado
**URL:** https://github.com/llvm/llvm-project/issues/124326

## Body

The following program caused the crash:

```C++
#include <variant>
#include <utility>
#include <concepts>
#include <cstdint>
#include <array>

template<typename NUMERICAL>
concept is_numeric = requires(const NUMERICAL val) {
    { val + val } -> std::convertible_to<NUMERICAL>;
    { val *val } -> std::convertible_to<NUMERICAL>;
    { 1 * val } -> std::convertible_to<NUMERICAL>;
    { 1.0 * val } -> std::convertible_to<NUMERICAL>;
} && std::three_way_comparable_with<NUMERICAL, NUMERICAL>;

template<is_numeric... NUMERICs>
struct numeric_union
{
    using value_type = std::variant<NUMERICs...>;

    value_type value;

    static constexpr auto type_count = sizeof...(NUMERICs);

    template <std::integral auto I>
    requires(I >= 0 && I < type_count)
    using type = std::variant_alternative_t<I, value_type>;

    template <std::integral auto I>
    constexpr static auto prototype = type<I>{};

    constexpr static auto sizes = []<std::size_t... INDEX>(std::index_sequence<INDEX...>) {
        return std::array{ sizeof(prototype<INDEX>)... };
    }(std::make_index_sequence<type_count-1>{});
};
using int_value = numeric_union<std::int8_t,
                                    std::int16_t,
                                    std::int32_t,
                                    std::int64_t>;

using unsigned_value = numeric_union<std::uint8_t,
                                    std::uint16_t,
                                    std::uint32_t,
                                    std::uint64_t>;

//using integral_value = numeric_union<int_value, unsigned_value>;

using float_value = numeric_union<float, double>;

static_assert(sizeof(float_value)==sizeof(float_value::value_type));
```
tested on trunk from compiler-explorer and latest fedora.

https://compiler-explorer.com/z/96q5d9vj5

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:2415:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A->getDeducedType().isNull() && "cannot request the size of an undeduced or dependent auto type"' failed.
```

Stack dump:
```console
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 -O2 <source>
1.	<source>:49:34: current parser token '=='
2.	<source>:16:8: instantiating class definition 'numeric_union<float, double>'
 #0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077a56a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006ff2330 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff2330)
 #5 0x0000000006fdc9db clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6fdc9db)
 #6 0x0000000006ff405b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff405b)
 #7 0x0000000006ff48f9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff48f9)
 #8 0x000000000721b8a3 HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
 #9 0x0000000007282f6f clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#10 0x000000000724bbfe Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#11 0x0000000007262bee EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#12 0x0000000007264544 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#13 0x00000000072a0191 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x0000000007262bb5 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#15 0x00000000072652e0 (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>) ExprConstant.cpp:0:0
#16 0x000000000729e6a6 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#17 0x00000000072a2ad4 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::RecordExprEvaluator>::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#18 0x00000000072a2d37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#19 0x000000000729db52 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#20 0x0000000007262d31 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#21 0x000000000725c268 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#22 0x000000000725c438 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#23 0x0000000007260f36 HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#24 0x0000000007286030 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::RecordExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#25 0x000000000729dae3 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#26 0x00000000072a2ad4 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::RecordExprEvaluator>::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#27 0x00000000072a2d37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#28 0x000000000729d9e6 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#29 0x000000000729dd27 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#30 0x0000000007262d31 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#31 0x000000000726b72d clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x726b72d)
#32 0x0000000007184e66 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x7184e66)
#33 0x000000000718517e clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x718517e)
#34 0x00000000062d509b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d509b)
#35 0x00000000062d9f62 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d9f62)
#36 0x0000000006be08f8 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6be08f8)
#37 0x0000000006c23ae2 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c23ae2)
#38 0x0000000006c2f623 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c2f623)
#39 0x0000000006b9dcf5 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6b9dcf5)
#40 0x0000000006bbdc5a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6bbdc5a)
#41 0x0000000006c6043f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#42 0x0000000006097151 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6097151)
#43 0x0000000006c648f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c648f5)
#44 0x0000000006c64975 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c64975)
#45 0x00000000064968cc clang::Sema::CheckUnaryExprOrTypeTraitOperand(clang::QualType, clang::SourceLocation, clang::SourceRange, clang::UnaryExprOrTypeTrait, llvm::StringRef) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x64968cc)
#46 0x000000000652a3d8 clang::Sema::CreateUnaryExprOrTypeTraitExpr(clang::TypeSourceInfo*, clang::SourceLocation, clang::UnaryExprOrTypeTrait, clang::SourceRange) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x652a3d8)
#47 0x0000000006550aee clang::Sema::ActOnUnaryExprOrTypeTraitExpr(clang::SourceLocation, clang::UnaryExprOrTypeTrait, bool, void*, clang::SourceRange) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6550aee)
#48 0x0000000005fab121 clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fab121)
#49 0x0000000005f9d36a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9d36a)
#50 0x0000000005fabac9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fabac9)
#51 0x0000000005f81cbf clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f81cbf)
#52 0x0000000005f71028 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f71028)
#53 0x0000000005f27cfa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cfa)
#54 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#55 0x0000000005f1877a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#56 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#57 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#58 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#59 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#60 0x0000000000c874dc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#61 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#62 0x0000000004017309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#63 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#64 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#65 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#66 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#67 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#68 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#69 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#70 0x000077a56a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#71 0x000077a56a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#72 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Victor Bogado da Silva Lins (bogado)

<details>
The following program caused the crash:

```C++
#include &lt;variant&gt;
#include &lt;utility&gt;
#include &lt;concepts&gt;
#include &lt;cstdint&gt;
#include &lt;array&gt;

template&lt;typename NUMERICAL&gt;
concept is_numeric = requires(const NUMERICAL val) {
    { val + val } -&gt; std::convertible_to&lt;NUMERICAL&gt;;
    { val *val } -&gt; std::convertible_to&lt;NUMERICAL&gt;;
    { 1 * val } -&gt; std::convertible_to&lt;NUMERICAL&gt;;
    { 1.0 * val } -&gt; std::convertible_to&lt;NUMERICAL&gt;;
} &amp;&amp; std::three_way_comparable_with&lt;NUMERICAL, NUMERICAL&gt;;

template&lt;is_numeric... NUMERICs&gt;
struct numeric_union
{
    using value_type = std::variant&lt;NUMERICs...&gt;;

    value_type value;

    static constexpr auto type_count = sizeof...(NUMERICs);

    template &lt;std::integral auto I&gt;
    requires(I &gt;= 0 &amp;&amp; I &lt; type_count)
    using type = std::variant_alternative_t&lt;I, value_type&gt;;

    template &lt;std::integral auto I&gt;
    constexpr static auto prototype = type&lt;I&gt;{};

    constexpr static auto sizes = []&lt;std::size_t... INDEX&gt;(std::index_sequence&lt;INDEX...&gt;) {
        return std::array{ sizeof(prototype&lt;INDEX&gt;)... };
    }(std::make_index_sequence&lt;type_count-1&gt;{});
};
using int_value = numeric_union&lt;std::int8_t,
                                    std::int16_t,
                                    std::int32_t,
                                    std::int64_t&gt;;

using unsigned_value = numeric_union&lt;std::uint8_t,
                                    std::uint16_t,
                                    std::uint32_t,
                                    std::uint64_t&gt;;

//using integral_value = numeric_union&lt;int_value, unsigned_value&gt;;

using float_value = numeric_union&lt;float, double&gt;;

static_assert(sizeof(float_value)==sizeof(float_value::value_type));
```
tested on trunk from compiler-explorer and latest fedora.

https://compiler-explorer.com/z/96q5d9vj5

Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 -O2 &lt;source&gt;
1.	&lt;source&gt;:49:34: current parser token '=='
2.	&lt;source&gt;:16:8: instantiating class definition 'numeric_union&lt;float, double&gt;'
 #<!-- -->0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #<!-- -->1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #<!-- -->2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077a56a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006ff2330 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff2330)
 #<!-- -->5 0x0000000006fdc9db clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6fdc9db)
 #<!-- -->6 0x0000000006ff405b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff405b)
 #<!-- -->7 0x0000000006ff48f9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6ff48f9)
 #<!-- -->8 0x000000000721b8a3 HandleSizeof((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::CharUnits&amp;, SizeOfType) ExprConstant.cpp:0:0
 #<!-- -->9 0x0000000007282f6f clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->10 0x000000000724bbfe Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007262bee EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007264544 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000072a0191 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007262bb5 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000072652e0 (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;) ExprConstant.cpp:0:0
#<!-- -->16 0x000000000729e6a6 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00000000072a2ad4 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->18 0x00000000072a2d37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x000000000729db52 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007262d31 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->21 0x000000000725c268 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->22 0x000000000725c438 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->23 0x0000000007260f36 HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->24 0x0000000007286030 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->25 0x000000000729dae3 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->26 0x00000000072a2ad4 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->27 0x00000000072a2d37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->28 0x000000000729d9e6 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->29 0x000000000729dd27 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->30 0x0000000007262d31 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->31 0x000000000726b72d clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x726b72d)
#<!-- -->32 0x0000000007184e66 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x7184e66)
#<!-- -->33 0x000000000718517e clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x718517e)
#<!-- -->34 0x00000000062d509b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d509b)
#<!-- -->35 0x00000000062d9f62 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d9f62)
#<!-- -->36 0x0000000006be08f8 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6be08f8)
#<!-- -->37 0x0000000006c23ae2 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c23ae2)
#<!-- -->38 0x0000000006c2f623 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c2f623)
#<!-- -->39 0x0000000006b9dcf5 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6b9dcf5)
#<!-- -->40 0x0000000006bbdc5a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6bbdc5a)
#<!-- -->41 0x0000000006c6043f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->42 0x0000000006097151 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6097151)
#<!-- -->43 0x0000000006c648f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c648f5)
#<!-- -->44 0x0000000006c64975 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6c64975)
#<!-- -->45 0x00000000064968cc clang::Sema::CheckUnaryExprOrTypeTraitOperand(clang::QualType, clang::SourceLocation, clang::SourceRange, clang::UnaryExprOrTypeTrait, llvm::StringRef) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x64968cc)
#<!-- -->46 0x000000000652a3d8 clang::Sema::CreateUnaryExprOrTypeTraitExpr(clang::TypeSourceInfo*, clang::SourceLocation, clang::UnaryExprOrTypeTrait, clang::SourceRange) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x652a3d8)
#<!-- -->47 0x0000000006550aee clang::Sema::ActOnUnaryExprOrTypeTraitExpr(clang::SourceLocation, clang::UnaryExprOrTypeTrait, bool, void*, clang::SourceRange) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6550aee)
#<!-- -->48 0x0000000005fab121 clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fab121)
#<!-- -->49 0x0000000005f9d36a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9d36a)
#<!-- -->50 0x0000000005fabac9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fabac9)
#<!-- -->51 0x0000000005f81cbf clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f81cbf)
#<!-- -->52 0x0000000005f71028 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f71028)
#<!-- -->53 0x0000000005f27cfa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cfa)
#<!-- -->54 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#<!-- -->55 0x0000000005f1877a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#<!-- -->56 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#<!-- -->57 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#<!-- -->58 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#<!-- -->59 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#<!-- -->60 0x0000000000c874dc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#<!-- -->61 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->62 0x0000000004017309 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->63 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#<!-- -->64 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->65 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#<!-- -->66 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#<!-- -->67 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#<!-- -->68 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#<!-- -->69 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#<!-- -->70 0x000077a56a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->71 0x000077a56a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->72 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
</details>


---

### Comment 2 - shafik

You can use ` ```console ` to quote any sort of console output such as stack dumps.

---

### Comment 3 - shafik

The last reproducer here: https://github.com/llvm/llvm-project/issues/75847

has the same backtrace. This could be a duplicate but we need a reduction in order to know for sure.

---

### Comment 4 - shafik

CC @Endilll 

---

### Comment 5 - bogado

This version works: https://compiler-explorer.com/z/a4zz1PGr3


---

### Comment 6 - bogado

Smaller version showing the issue : 

```C++
#include <variant>
#include <concepts>
#include <array>

template<typename... NUMERICs>
struct numeric_union
{
    using value_type = std::variant<NUMERICs...>;

    template <std::integral auto I>
    static constexpr auto prototype = std::variant_alternative_t<I, value_type>{};

    constexpr static auto sizes = []<std::size_t... INDEX>(std::index_sequence<INDEX...>) {
        return std::array{ sizeof(prototype<INDEX>)... };
    }(std::make_index_sequence<std::variant_size_v<value_type>-1>{});
};

using float_value = numeric_union<float, double>;

static_assert(sizeof(float_value)==sizeof(float_value::value_type));
```

while this version seems to work fine: 

```C++
#include <variant>
#include <array>

template<typename... NUMERICs>
struct numeric_union
{
    using value_type = std::variant<NUMERICs...>;

    template <int I>
    using prototype = std::variant_alternative_t<I, value_type>;

    constexpr static auto sizes = []<std::size_t... INDEX>(std::index_sequence<INDEX...>) {
        return std::array{ sizeof(prototype<INDEX>)... };
    }(std::make_index_sequence<std::variant_size_v<value_type>-1>{});
};

static_assert(sizeof(numeric_union<float, double>)==1);
```

Using `sizeof(«value»)` triggers the issue, while `sizeof(«type»)` does not.

---

### Comment 7 - MagentaTreehouse

Reduced: https://compiler-explorer.com/z/eG7eGjMrb
```c++
template<typename...>
struct numeric_union
{
    template <int>
    static constexpr auto prototype = 0;

    constexpr static auto sizes = sizeof(prototype<0>);
};

numeric_union<int> a;
```

The difference from #75847 is that the problematic `sizeof` in this issue is inside the class template. They could have the same underlying cause.

---

### Comment 8 - shafik

> Reduced: https://compiler-explorer.com/z/eG7eGjMrb
> 
> template<typename...>
> struct numeric_union
> {
>     template <int>
>     static constexpr auto prototype = 0;
> 
>     constexpr static auto sizes = sizeof(prototype<0>);
> };
> 
> numeric_union<int> a;
> The difference from [#75847](https://github.com/llvm/llvm-project/issues/75847) is that the problematic `sizeof` in this issue is inside the class template. They could have the same underlying cause.

They could be the same but it is not conclusive at this point. Once we have a proposed fix it should become more obvious.

---

### Comment 9 - Endilll

@shafik  It seems that the reduction has been done. Let me know if that's not the case.

---

