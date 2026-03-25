# Clang crash in void clang::LocalInstantiationScope::InstantiatedLocal "Already instantiated this local"

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/140450

## Body

Clang crashes with an assertion failure on valid code:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4579: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl*, clang::Decl*): Assertion `cast<Decl *>(Stored) == Inst && "Already instantiated this local"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:55:6: instantiating function definition '(anonymous namespace)::Component<(lambda at <source>:59:13)>'
3.	<source>:36:12: instantiating function definition '(anonymous namespace)::S::apply<(lambda at <source>:59:13), int, int>::Op::operator()'
 #0 0x0000000003f9b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b2a8)
 #1 0x0000000003f98f34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98f34)
 #2 0x0000000003eddb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b3979442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b39794969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b3979442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b39794287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b397942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b3979439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000071e68d5 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e68d5)
#10 0x0000000007221aa4 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7221aa4)
#11 0x0000000007222996 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000071fd83c clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071feaf7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000720d7bb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000072109f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000071ff733 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000721ecc4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000071fe982 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#21 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000007212dc5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypeWithDeducedTST(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#23 0x00000000072132ff clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#24 0x00000000071ed69c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000071ef3b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#26 0x00000000071fa209 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#27 0x00000000071ed6d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#28 0x000000000722d777 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#29 0x000000000722e00e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x00000000071ec7c6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x00000000071ed2d2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x00000000071ed85f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000720b41e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCStyleCastExpr(clang::CStyleCastExpr*) SemaTemplateInstantiate.cpp:0:0
#34 0x00000000071ed6ec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#35 0x000000000722d777 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#36 0x000000000722e00e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#37 0x0000000007237334 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7237334)
#38 0x0000000007296ac7 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7296ac7)
#39 0x0000000007294916 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294916)
#40 0x0000000007296d7b clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7296d7b)
#41 0x0000000007294916 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294916)
#42 0x00000000067df402 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#43 0x00000000067dfca2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67dfca2)
#44 0x00000000066400cb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66400cb)
#45 0x00000000066323ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66323ba)
#46 0x0000000004946b78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946b78)
#47 0x0000000004c3eb45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3eb45)
#48 0x0000000004bbd78e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbd78e)
#49 0x0000000004d31629 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d31629)
#50 0x0000000000dace0f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdace0f)
#51 0x0000000000da2fda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#52 0x00000000049ade69 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#53 0x0000000003ede004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede004)
#54 0x00000000049ae47f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#55 0x000000000497024d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497024d)
#56 0x00000000049712de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49712de)
#57 0x00000000049795c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49795c5)
#58 0x0000000000da8d78 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8d78)
#59 0x0000000000c2e774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e774)
#60 0x00007b3979429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#61 0x00007b3979429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#62 0x0000000000da2a85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2a85)
```

Reduced test case: https://gcc.godbolt.org/z/sdxq1aP89
```
namespace {
struct Call {
  template <typename F>
  struct apply : F::template apply<void> {};
};
template <typename MetaExpr>
struct DoEval {
  using type = MetaExpr;
};
template <typename MetaFun, typename... MetaExprs>
struct DoEval<MetaFun(MetaExprs...)> {
  using type = typename DoEval<typename MetaFun::template apply<
      typename DoEval<MetaExprs>::type...>::type>::type;
};
template <typename MetaFun, typename... MetaExprs>
struct DoEval<MetaFun (*)(MetaExprs...)> {
  using type = typename DoEval<typename MetaFun::template apply<
      typename DoEval<MetaExprs>::type...>::type>::type;
};
template <typename MetaExpr>
using Eval = typename DoEval<MetaExpr>::type;
struct GetNthType {
  template <typename N>
  struct apply {
    using type = int;
  };
};
template <typename F, typename... Args>
static auto invoke(Args...) -> void;
template <typename>
struct Arg {};
struct S {
  template <typename Lambda, typename... Indexes>
  struct apply {
    struct Op {
      void operator()() {
        (void)[] { invoke<Lambda>(Arg<GetNthType(Indexes)>()...); };
      }
    };
    using type = Op;
  };
};
struct F {
  template <typename Lambda>
  struct apply {
    struct type {
      template<typename C>
      struct apply {
        using type = S(Lambda, int, int);
      };
    };
  };
};
template <typename Lambda>
void Component(Lambda&&) {
  Eval<Call(F(Lambda))>()();
}
void f() {
  Component([] {});
}
}
```


## Comments

### Comment 1 - alexfh

@cor3ntin @mizvekov 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes with an assertion failure on valid code:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4579: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl*, clang::Decl*): Assertion `cast&lt;Decl *&gt;(Stored) == Inst &amp;&amp; "Already instantiated this local"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:55:6: instantiating function definition '(anonymous namespace)::Component&lt;(lambda at &lt;source&gt;:59:13)&gt;'
3.	&lt;source&gt;:36:12: instantiating function definition '(anonymous namespace)::S::apply&lt;(lambda at &lt;source&gt;:59:13), int, int&gt;::Op::operator()'
 #<!-- -->0 0x0000000003f9b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b2a8)
 #<!-- -->1 0x0000000003f98f34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98f34)
 #<!-- -->2 0x0000000003eddb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b3979442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b39794969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b3979442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b39794287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b397942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b3979439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000071e68d5 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e68d5)
#<!-- -->10 0x0000000007221aa4 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7221aa4)
#<!-- -->11 0x0000000007222996 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;*, clang::Sema::ExtParameterInfoBuilder&amp;, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000071fd83c clang::QualType clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType&lt;clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)&gt;(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000071feaf7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000720d7bb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000072109f8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000071ff733 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x000000000721ecc4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000071fe982 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x0000000007212dc5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTypeWithDeducedTST(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x00000000072132ff clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x00000000071ed69c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000071ef3b7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x00000000071fa209 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x00000000071ed6d8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x000000000722d777 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x000000000722e00e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x00000000071ec7c6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->31 0x00000000071ed2d2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x00000000071ed85f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x000000000720b41e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCStyleCastExpr(clang::CStyleCastExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x00000000071ed6ec clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->35 0x000000000722d777 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->36 0x000000000722e00e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->37 0x0000000007237334 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7237334)
#<!-- -->38 0x0000000007296ac7 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7296ac7)
#<!-- -->39 0x0000000007294916 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294916)
#<!-- -->40 0x0000000007296d7b clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7296d7b)
#<!-- -->41 0x0000000007294916 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294916)
#<!-- -->42 0x00000000067df402 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->43 0x00000000067dfca2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67dfca2)
#<!-- -->44 0x00000000066400cb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66400cb)
#<!-- -->45 0x00000000066323ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66323ba)
#<!-- -->46 0x0000000004946b78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946b78)
#<!-- -->47 0x0000000004c3eb45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3eb45)
#<!-- -->48 0x0000000004bbd78e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbd78e)
#<!-- -->49 0x0000000004d31629 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d31629)
#<!-- -->50 0x0000000000dace0f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdace0f)
#<!-- -->51 0x0000000000da2fda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->52 0x00000000049ade69 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->53 0x0000000003ede004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede004)
#<!-- -->54 0x00000000049ae47f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->55 0x000000000497024d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497024d)
#<!-- -->56 0x00000000049712de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49712de)
#<!-- -->57 0x00000000049795c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49795c5)
#<!-- -->58 0x0000000000da8d78 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8d78)
#<!-- -->59 0x0000000000c2e774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e774)
#<!-- -->60 0x00007b3979429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->61 0x00007b3979429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->62 0x0000000000da2a85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2a85)
```

Reduced test case: https://gcc.godbolt.org/z/sdxq1aP89
```
namespace {
struct Call {
  template &lt;typename F&gt;
  struct apply : F::template apply&lt;void&gt; {};
};
template &lt;typename MetaExpr&gt;
struct DoEval {
  using type = MetaExpr;
};
template &lt;typename MetaFun, typename... MetaExprs&gt;
struct DoEval&lt;MetaFun(MetaExprs...)&gt; {
  using type = typename DoEval&lt;typename MetaFun::template apply&lt;
      typename DoEval&lt;MetaExprs&gt;::type...&gt;::type&gt;::type;
};
template &lt;typename MetaFun, typename... MetaExprs&gt;
struct DoEval&lt;MetaFun (*)(MetaExprs...)&gt; {
  using type = typename DoEval&lt;typename MetaFun::template apply&lt;
      typename DoEval&lt;MetaExprs&gt;::type...&gt;::type&gt;::type;
};
template &lt;typename MetaExpr&gt;
using Eval = typename DoEval&lt;MetaExpr&gt;::type;
struct GetNthType {
  template &lt;typename N&gt;
  struct apply {
    using type = int;
  };
};
template &lt;typename F, typename... Args&gt;
static auto invoke(Args...) -&gt; void;
template &lt;typename&gt;
struct Arg {};
struct S {
  template &lt;typename Lambda, typename... Indexes&gt;
  struct apply {
    struct Op {
      void operator()() {
        (void)[] { invoke&lt;Lambda&gt;(Arg&lt;GetNthType(Indexes)&gt;()...); };
      }
    };
    using type = Op;
  };
};
struct F {
  template &lt;typename Lambda&gt;
  struct apply {
    struct type {
      template&lt;typename C&gt;
      struct apply {
        using type = S(Lambda, int, int);
      };
    };
  };
};
template &lt;typename Lambda&gt;
void Component(Lambda&amp;&amp;) {
  Eval&lt;Call(F(Lambda))&gt;()();
}
void f() {
  Component([] {});
}
}
```

</details>


---

### Comment 3 - mizvekov

This is fixed by https://github.com/llvm/llvm-project/pull/107942, but I don't know when I'll be back to working on that patch yet.

---

