# Clang rejects a valid generic lamba within a conditional explicit for a constructor template of a class template

**Author:** AlisdairM
**URL:** https://github.com/llvm/llvm-project/issues/134861

## Body

In the following example, all 5 variable declarations should succeed, but the three instantiations of a class template fail, leaving only the non-template classes to succeed.  This interpretation has been validated against current gcc compilers.

Code example also available at Godbolt: https://godbolt.org/z/d488W9xGn

```
#include <type_traits>
#include <utility>

// Plain classes work fine

struct Best {
   template <class ...U>
   explicit( []<std::size_t ...I>(std::index_sequence<I...>) consteval
               { return sizeof...(U) > 1; }
               ( std::index_sequence<1uz>{} )
           )
   Best(U const &...)
   {}
};

struct Jest {
   template <class ...U>
   explicit( []<std::size_t I>(std::index_sequence<I>) consteval
               { return sizeof...(U) > 1; }
               ( std::index_sequence<1uz>{} )
           )
   Jest(U const &...)
   {}
};

// class templates do not support constrained polymorphic lambdas

template <typename ...T>
struct Test {
   template <class ...U>
   explicit( []<std::size_t ...I>(std::index_sequence<I...>) consteval
               { return sizeof...(U) > 1; }
               ( std::index_sequence_for<T...>{} )
           )
   Test(U const &...)
   {}
};

template <typename T>
struct Yest {
   template <class ...U>
   explicit( []<std::size_t ...I>(std::index_sequence<I...>) consteval
               { return sizeof...(U) > 1; }
               ( std::index_sequence<1uz>{} )
           )
   Yest(U const &...)
   {}
};

template <typename T>
struct Zest {
   template <class ...U>
   explicit( []<std::size_t I>(std::index_sequence<I>) consteval
               { return sizeof...(U) > 1; }
               ( std::index_sequence<1uz>{} )
           )
   Zest(U const &...)
   {}
};

int main() {

   Best         a = 3.14;
   Jest         b = 3.14;
   Test<double> x = 3.14;
   Yest<double> y = 3.14;
   Zest<double> z = 3.14;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alisdair Meredith (AlisdairM)

<details>
In the following example, all 5 variable declarations should succeed, but the three instantiations of a class template fail, leaving only the non-template classes to succeed.  This interpretation has been validated against current gcc compilers.

Code example also available at Godbolt: https://godbolt.org/z/d488W9xGn

```
#include &lt;type_traits&gt;
#include &lt;utility&gt;

// Plain classes work fine

struct Best {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Best(U const &amp;...)
   {}
};

struct Jest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t I&gt;(std::index_sequence&lt;I&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Jest(U const &amp;...)
   {}
};

// class templates do not support constrained polymorphic lambdas

template &lt;typename ...T&gt;
struct Test {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence_for&lt;T...&gt;{} )
           )
   Test(U const &amp;...)
   {}
};

template &lt;typename T&gt;
struct Yest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Yest(U const &amp;...)
   {}
};

template &lt;typename T&gt;
struct Zest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t I&gt;(std::index_sequence&lt;I&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Zest(U const &amp;...)
   {}
};

int main() {

   Best         a = 3.14;
   Jest         b = 3.14;
   Test&lt;double&gt; x = 3.14;
   Yest&lt;double&gt; y = 3.14;
   Zest&lt;double&gt; z = 3.14;
}
```
</details>


---

### Comment 2 - Sirraide

Looks like we’re hitting an assertion during template deduction:
```console
clang-21: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:886: 
unsigned int {anonymous}::PackDeductionScope::addPacks(clang::TemplateArgument): 
Assertion `!Packs.empty() && "Pack expansion without unexpanded packs?"' failed.
```

Stack trace
```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20250408/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20250408/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-6e0456.o -x c++ <source>
1.	<source>:65:25: current parser token ';'
2.	<source>:61:12: parsing function body 'main'
3.	<source>:61:12: in compound statement ('{}')
4.	<source>:29:8: instantiating class definition 'Test<double>'
 #0 0x0000000003ea8238 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x3ea8238)
 #1 0x0000000003ea5bc4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000769f28c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000769f28c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000769f28c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000769f28c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000769f28c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000769f28c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000006fa7a06 (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6fa7a06)
 #9 0x0000000006fa8459 DeduceTemplateSpecArguments(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool*) SemaTemplateDeduction.cpp:0:0
#10 0x0000000006fa43d4 DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#11 0x0000000006fc82e0 DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#12 0x0000000006fc91ee clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda0'(clang::QualType, unsigned int, bool)::operator()(clang::QualType, unsigned int, bool) const SemaTemplateDeduction.cpp:0:0
#13 0x0000000006fca1ed clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6fca1ed)
#14 0x0000000006e093e1 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#15 0x0000000006e09d31 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6e09d31)
#16 0x0000000006e20f52 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6e20f52)
#17 0x0000000006a2aed3 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6a2aed3)
#18 0x0000000006a2b7de clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6a2b7de)
#19 0x000000000706a76c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000705d8da clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000070990d2 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x70990d2)
#22 0x00000000070b0b8d clang::Sema::instantiateExplicitSpecifier(clang::MultiLevelTemplateArgumentList const&, clang::ExplicitSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x70b0b8d)
#23 0x000000000711282c clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x711282c)
#24 0x0000000007116e25 clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x7116e25)
#25 0x00000000070857cb clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x70857cb)
#26 0x00000000070a8860 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x70a8860)
#27 0x0000000007126ace void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#28 0x0000000007d97221 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x7d97221)
#29 0x000000000712fe9a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x712fe9a)
#30 0x0000000007130495 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x7130495)
#31 0x0000000007130615 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x7130615)
#32 0x0000000006891467 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6891467)
#33 0x0000000006518d9a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6518d9a)
#34 0x0000000006527f49 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6527f49)
#35 0x00000000065309b7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x65309b7)
#36 0x0000000006530e79 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x6530e79)
#37 0x00000000065e0632 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x65e0632)
#38 0x00000000065e133d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x65e133d)
#39 0x00000000065e8fd3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x65e8fd3)
#40 0x00000000065e979a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x65e979a)
#41 0x00000000064f2e63 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64f2e63)
#42 0x000000000652894d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x652894d)
#43 0x00000000064e653e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64e653e)
#44 0x00000000064e6cf9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64e6cf9)
#45 0x00000000064eeb03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64eeb03)
#46 0x00000000064efa1d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64efa1d)
#47 0x00000000064e19aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x64e19aa)
#48 0x0000000004840bc8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x4840bc8)
#49 0x0000000004b2f4e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x4b2f4e5)
#50 0x0000000004ab13be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x4ab13be)
#51 0x0000000004c1f70e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0x4c1f70e)
#52 0x0000000000d6234f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0xd6234f)
#53 0x0000000000d59b0a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#54 0x0000000000d5dcc6 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0xd5dcc6)
#55 0x0000000000c24e44 main (/opt/compiler-explorer/clang-assertions-trunk-20250408/bin/clang-21+0xc24e44)
```

---

### Comment 3 - shafik

Sadly clang assertion crashes on this: https://godbolt.org/z/GYdjqKhdb

but worth noting only gcc accepts this while edg/MSVC do not. So can you justify why you believe this should work w/ some quotes?

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:886:
unsigned int {anonymous}::PackDeductionScope::addPacks(clang::TemplateArgument):
Assertion `!Packs.empty() && "Pack expansion without unexpanded packs?"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:65:25: current parser token ';'
2.	<source>:61:12: parsing function body 'main'
3.	<source>:61:12: in compound statement ('{}')
4.	<source>:29:8: instantiating class definition 'Test<double>'
 #0 0x0000000003ea8238 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea8238)
 #1 0x0000000003ea5ec4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea5ec4)
 #2 0x0000000003deaa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bdf9de42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bdf9de969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bdf9de42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bdf9de287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007bdf9de2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007bdf9de39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006fa7a06 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa7a06)
#10 0x0000000006fa8459 DeduceTemplateSpecArguments(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool*) SemaTemplateDeduction.cpp:0:0
#11 0x0000000006fa43d4 DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#12 0x0000000006fc82e0 DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#13 0x0000000006fc91ee clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda0'(clang::QualType, unsigned int, bool)::operator()(clang::QualType, unsigned int, bool) const SemaTemplateDeduction.cpp:0:0
#14 0x0000000006fca1ed clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fca1ed)
#15 0x0000000006e093e1 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#16 0x0000000006e09d31 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e09d31)
#17 0x0000000006e20f52 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e20f52)
#18 0x0000000006a2aed3 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2aed3)
#19 0x0000000006a2b7de clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b7de)
#20 0x000000000706a76c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x000000000705d8da clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x00000000070990d2 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70990d2)
#23 0x00000000070b0b8d clang::Sema::instantiateExplicitSpecifier(clang::MultiLevelTemplateArgumentList const&, clang::ExplicitSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b0b8d)
#24 0x000000000711282c clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711282c)
#25 0x0000000007116e25 clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7116e25)
#26 0x00000000070857cb clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70857cb)
#27 0x00000000070a8860 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a8860)
#28 0x0000000007126ace void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#29 0x0000000007d97221 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d97221)
#30 0x000000000712fe9a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712fe9a)
#31 0x0000000007130495 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7130495)
#32 0x0000000007130615 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7130615)
#33 0x0000000006891467 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6891467)
#34 0x0000000006518d9a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6518d9a)
#35 0x0000000006527f49 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6527f49)
#36 0x00000000065309b7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65309b7)
#37 0x0000000006530e79 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6530e79)
#38 0x00000000065e0632 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e0632)
#39 0x00000000065e133d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e133d)
#40 0x00000000065e8fd3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e8fd3)
#41 0x00000000065e979a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e979a)
#42 0x00000000064f2e63 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f2e63)
#43 0x000000000652894d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652894d)
#44 0x00000000064e653e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e653e)
#45 0x00000000064e6cf9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e6cf9)
#46 0x00000000064eeb03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64eeb03)
#47 0x00000000064efa1d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64efa1d)
#48 0x00000000064e19aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e19aa)
#49 0x0000000004840bc8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4840bc8)
#50 0x0000000004b2f4e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2f4e5)
#51 0x0000000004ab13be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab13be)
#52 0x0000000004c1f70e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1f70e)
#53 0x0000000000d6234f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd6234f)
#54 0x0000000000d59b0a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#55 0x00000000048a6639 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#56 0x0000000003deaea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3deaea4)
#57 0x00000000048a6c4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#58 0x00000000048696ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48696ed)
#59 0x000000000486a77e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486a77e)
#60 0x0000000004871e85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4871e85)
#61 0x0000000000d5f148 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5f148)
#62 0x0000000000c24e44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc24e44)
#63 0x00007bdf9de29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#64 0x00007bdf9de29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#65 0x0000000000d595b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd595b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alisdair Meredith (AlisdairM)

<details>
In the following example, all 5 variable declarations should succeed, but the three instantiations of a class template fail, leaving only the non-template classes to succeed.  This interpretation has been validated against current gcc compilers.

Code example also available at Godbolt: https://godbolt.org/z/d488W9xGn

```
#include &lt;type_traits&gt;
#include &lt;utility&gt;

// Plain classes work fine

struct Best {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Best(U const &amp;...)
   {}
};

struct Jest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t I&gt;(std::index_sequence&lt;I&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Jest(U const &amp;...)
   {}
};

// class templates do not support constrained polymorphic lambdas

template &lt;typename ...T&gt;
struct Test {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence_for&lt;T...&gt;{} )
           )
   Test(U const &amp;...)
   {}
};

template &lt;typename T&gt;
struct Yest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t ...I&gt;(std::index_sequence&lt;I...&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Yest(U const &amp;...)
   {}
};

template &lt;typename T&gt;
struct Zest {
   template &lt;class ...U&gt;
   explicit( []&lt;std::size_t I&gt;(std::index_sequence&lt;I&gt;) consteval
               { return sizeof...(U) &gt; 1; }
               ( std::index_sequence&lt;1uz&gt;{} )
           )
   Zest(U const &amp;...)
   {}
};

int main() {

   Best         a = 3.14;
   Jest         b = 3.14;
   Test&lt;double&gt; x = 3.14;
   Yest&lt;double&gt; y = 3.14;
   Zest&lt;double&gt; z = 3.14;
}
```
</details>


---

### Comment 5 - shafik

We have bunch of crashes all somewhat similar but not identical: https://github.com/llvm/llvm-project/issues/114755 and https://github.com/llvm/llvm-project/issues/112096 are two of them.

CC @cor3ntin 

Let's focus on language lawyering the original issue and maybe there is a new issue specifically for the crash, if folks don't believe it is just another variant of the others.

---

