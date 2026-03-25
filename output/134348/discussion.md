# Crash when compiling multiply nested templates with Constrained NTTP

**Author:** AdrArtGar
**URL:** https://github.com/llvm/llvm-project/issues/134348

## Body

The following code (which compiles and runs properly on GCC and on clang 15) [godbolt](https://godbolt.org/z/boE3dc6oT)
```c++
template <typename T>
concept Any = true;

template <typename T = void> constexpr auto crash_clang() {
	return []<typename...>() {
		return []<Any /*<- remove and it compiles*/ auto>() {
			return 0;
		}.template operator()<0>();
	}();
}

int main() {
	return crash_clang();
}
```
crashes clang with the following output
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-19 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name CrashClang.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/adrian -fcoverage-compilation-dir=/home/adrian -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/CrashClang-1dd804.o -x c++ CrashClang.cpp
1.	CrashClang.cpp:13:21: current parser token ')'
2.	CrashClang.cpp:12:12: parsing function body 'main'
3.	CrashClang.cpp:12:12: in compound statement ('{}')
4.	CrashClang.cpp:4:45: instantiating function definition 'crash_clang<void>'
5.	CrashClang.cpp:5:9: instantiating function definition 'crash_clang()::(anonymous class)::operator()<>'
 #0 0x00007fbe4f21a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.19.1+0x81a730)
 #1 0x00007fbe4f2176bd (/usr/lib/libLLVM.so.19.1+0x8176bd)
 #2 0x00007fbe4e44bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #3 0x00007fbe59f0f27f (/usr/lib/libclang-cpp.so.19.1+0x2d0f27f)
 #4 0x00007fbe5887bf2f (/usr/lib/libclang-cpp.so.19.1+0x167bf2f)
 #5 0x00007fbe5887597c (/usr/lib/libclang-cpp.so.19.1+0x167597c)
 #6 0x00007fbe58877742 (/usr/lib/libclang-cpp.so.19.1+0x1677742)
 #7 0x00007fbe5887caec (/usr/lib/libclang-cpp.so.19.1+0x167caec)
 #8 0x00007fbe59cdc96b (/usr/lib/libclang-cpp.so.19.1+0x2adc96b)
 #9 0x00007fbe58849c4d (/usr/lib/libclang-cpp.so.19.1+0x1649c4d)
#10 0x00007fbe5824d317 (/usr/lib/libclang-cpp.so.19.1+0x104d317)
#11 0x00007fbe5824ec0b (/usr/lib/libclang-cpp.so.19.1+0x104ec0b)
#12 0x00007fbe5824f0d1 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/usr/lib/libclang-cpp.so.19.1+0x104f0d1)
#13 0x00007fbe58250231 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/usr/lib/libclang-cpp.so.19.1+0x1050231)
#14 0x00007fbe587e88f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/usr/lib/libclang-cpp.so.19.1+0x15e88f0)
#15 0x00007fbe587e99f5 (/usr/lib/libclang-cpp.so.19.1+0x15e99f5)
#16 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#17 0x00007fbe587f3128 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/usr/lib/libclang-cpp.so.19.1+0x15f3128)
#18 0x00007fbe59ce6af2 (/usr/lib/libclang-cpp.so.19.1+0x2ae6af2)
#19 0x00007fbe586f939a clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f939a)
#20 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#21 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#22 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#23 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#24 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#25 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#26 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#27 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#28 0x00007fbe586d1f2c (/usr/lib/libclang-cpp.so.19.1+0x14d1f2c)
#29 0x00007fbe586f7b79 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/usr/lib/libclang-cpp.so.19.1+0x14f7b79)
#30 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#31 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#32 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#33 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#34 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#35 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#36 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#37 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#38 0x00007fbe586f540c (/usr/lib/libclang-cpp.so.19.1+0x14f540c)
#39 0x00007fbe586f64f0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f64f0)
#40 0x00007fbe583e72b1 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x11e72b1)
#41 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#42 0x00007fbe57836a2a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/usr/lib/libclang-cpp.so.19.1+0x636a2a)
#43 0x00007fbe5783e457 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.19.1+0x63e457)
#44 0x00007fbe59ccf43a (/usr/lib/libclang-cpp.so.19.1+0x2acf43a)
#45 0x00007fbe59ccf50d (/usr/lib/libclang-cpp.so.19.1+0x2acf50d)
#46 0x00007fbe59ccf6a5 (/usr/lib/libclang-cpp.so.19.1+0x2acf6a5)
#47 0x00007fbe5788c5fb clang::Parser::ParseReturnStatement() (/usr/lib/libclang-cpp.so.19.1+0x68c5fb)
#48 0x00007fbe57891e9f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/libclang-cpp.so.19.1+0x691e9f)
#49 0x00007fbe578922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.19.1+0x6922a7)
#50 0x00007fbe5789a59a clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/libclang-cpp.so.19.1+0x69a59a)
#51 0x00007fbe5789ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/libclang-cpp.so.19.1+0x69ddac)
#52 0x00007fbe578bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/libclang-cpp.so.19.1+0x6bf6ef)
#53 0x00007fbe577ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.19.1+0x5ee83f)
#54 0x00007fbe578bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bc9d8)
#55 0x00007fbe578bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bd3ba)
#56 0x00007fbe578cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.19.1+0x6cd632)
#57 0x00007fbe578ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.19.1+0x6ce345)
#58 0x00007fbe577d9c18 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x5d9c18)
#59 0x00007fbe594fa221 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.19.1+0x22fa221)
#60 0x00007fbe5949008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/libclang-cpp.so.19.1+0x229008f)
#61 0x00007fbe59554ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.19.1+0x2354ba5)
#62 0x00005973f9813832 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#63 0x00005973f981804e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#64 0x00005973f9818de3 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#65 0x00005973f980bf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#66 0x00007fbe4e435488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#67 0x00007fbe4e43554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#68 0x00007fbe4e43554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#69 0x00005973f980bfc5 _start (/usr/bin/clang-19+0xafc5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

I did a git bisect and the first bad commit was found to be babdef27c503c0bbbcc017e9f88affddda90ea4e



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (AdrArtGar)

<details>
The following code (which compiles and runs properly on GCC and on clang 15) [godbolt](https://godbolt.org/z/boE3dc6oT)
```c++
template &lt;typename T&gt;
concept Any = true;

template &lt;typename T = void&gt; constexpr auto crash_clang() {
	return []&lt;typename...&gt;() {
		return []&lt;Any /*&lt;- remove and it compiles*/ auto&gt;() {
			return 0;
		}.template operator()&lt;0&gt;();
	}();
}

int main() {
	return crash_clang();
}
```
crashes clang with the following output
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-19 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name CrashClang.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/adrian -fcoverage-compilation-dir=/home/adrian -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/CrashClang-1dd804.o -x c++ CrashClang.cpp
1.	CrashClang.cpp:13:21: current parser token ')'
2.	CrashClang.cpp:12:12: parsing function body 'main'
3.	CrashClang.cpp:12:12: in compound statement ('{}')
4.	CrashClang.cpp:4:45: instantiating function definition 'crash_clang&lt;void&gt;'
5.	CrashClang.cpp:5:9: instantiating function definition 'crash_clang()::(anonymous class)::operator()&lt;&gt;'
 #<!-- -->0 0x00007fbe4f21a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.19.1+0x81a730)
 #<!-- -->1 0x00007fbe4f2176bd (/usr/lib/libLLVM.so.19.1+0x8176bd)
 #<!-- -->2 0x00007fbe4e44bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->3 0x00007fbe59f0f27f (/usr/lib/libclang-cpp.so.19.1+0x2d0f27f)
 #<!-- -->4 0x00007fbe5887bf2f (/usr/lib/libclang-cpp.so.19.1+0x167bf2f)
 #<!-- -->5 0x00007fbe5887597c (/usr/lib/libclang-cpp.so.19.1+0x167597c)
 #<!-- -->6 0x00007fbe58877742 (/usr/lib/libclang-cpp.so.19.1+0x1677742)
 #<!-- -->7 0x00007fbe5887caec (/usr/lib/libclang-cpp.so.19.1+0x167caec)
 #<!-- -->8 0x00007fbe59cdc96b (/usr/lib/libclang-cpp.so.19.1+0x2adc96b)
 #<!-- -->9 0x00007fbe58849c4d (/usr/lib/libclang-cpp.so.19.1+0x1649c4d)
#<!-- -->10 0x00007fbe5824d317 (/usr/lib/libclang-cpp.so.19.1+0x104d317)
#<!-- -->11 0x00007fbe5824ec0b (/usr/lib/libclang-cpp.so.19.1+0x104ec0b)
#<!-- -->12 0x00007fbe5824f0d1 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x104f0d1)
#<!-- -->13 0x00007fbe58250231 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x1050231)
#<!-- -->14 0x00007fbe587e88f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0x15e88f0)
#<!-- -->15 0x00007fbe587e99f5 (/usr/lib/libclang-cpp.so.19.1+0x15e99f5)
#<!-- -->16 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->17 0x00007fbe587f3128 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/libclang-cpp.so.19.1+0x15f3128)
#<!-- -->18 0x00007fbe59ce6af2 (/usr/lib/libclang-cpp.so.19.1+0x2ae6af2)
#<!-- -->19 0x00007fbe586f939a clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f939a)
#<!-- -->20 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->21 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#<!-- -->22 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#<!-- -->23 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#<!-- -->24 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#<!-- -->25 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->26 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#<!-- -->27 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#<!-- -->28 0x00007fbe586d1f2c (/usr/lib/libclang-cpp.so.19.1+0x14d1f2c)
#<!-- -->29 0x00007fbe586f7b79 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/usr/lib/libclang-cpp.so.19.1+0x14f7b79)
#<!-- -->30 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->31 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#<!-- -->32 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#<!-- -->33 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#<!-- -->34 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#<!-- -->35 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->36 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#<!-- -->37 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#<!-- -->38 0x00007fbe586f540c (/usr/lib/libclang-cpp.so.19.1+0x14f540c)
#<!-- -->39 0x00007fbe586f64f0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f64f0)
#<!-- -->40 0x00007fbe583e72b1 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x11e72b1)
#<!-- -->41 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->42 0x00007fbe57836a2a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/lib/libclang-cpp.so.19.1+0x636a2a)
#<!-- -->43 0x00007fbe5783e457 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.19.1+0x63e457)
#<!-- -->44 0x00007fbe59ccf43a (/usr/lib/libclang-cpp.so.19.1+0x2acf43a)
#<!-- -->45 0x00007fbe59ccf50d (/usr/lib/libclang-cpp.so.19.1+0x2acf50d)
#<!-- -->46 0x00007fbe59ccf6a5 (/usr/lib/libclang-cpp.so.19.1+0x2acf6a5)
#<!-- -->47 0x00007fbe5788c5fb clang::Parser::ParseReturnStatement() (/usr/lib/libclang-cpp.so.19.1+0x68c5fb)
#<!-- -->48 0x00007fbe57891e9f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/libclang-cpp.so.19.1+0x691e9f)
#<!-- -->49 0x00007fbe578922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.19.1+0x6922a7)
#<!-- -->50 0x00007fbe5789a59a clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/libclang-cpp.so.19.1+0x69a59a)
#<!-- -->51 0x00007fbe5789ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/libclang-cpp.so.19.1+0x69ddac)
#<!-- -->52 0x00007fbe578bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/libclang-cpp.so.19.1+0x6bf6ef)
#<!-- -->53 0x00007fbe577ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.19.1+0x5ee83f)
#<!-- -->54 0x00007fbe578bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bc9d8)
#<!-- -->55 0x00007fbe578bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bd3ba)
#<!-- -->56 0x00007fbe578cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.19.1+0x6cd632)
#<!-- -->57 0x00007fbe578ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/libclang-cpp.so.19.1+0x6ce345)
#<!-- -->58 0x00007fbe577d9c18 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x5d9c18)
#<!-- -->59 0x00007fbe594fa221 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.19.1+0x22fa221)
#<!-- -->60 0x00007fbe5949008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x229008f)
#<!-- -->61 0x00007fbe59554ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.19.1+0x2354ba5)
#<!-- -->62 0x00005973f9813832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->63 0x00005973f981804e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->64 0x00005973f9818de3 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#<!-- -->65 0x00005973f980bf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->66 0x00007fbe4e435488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->67 0x00007fbe4e43554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->68 0x00007fbe4e43554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->69 0x00005973f980bfc5 _start (/usr/bin/clang-19+0xafc5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

I did a git bisect and the first bad commit was found to be babdef27c503c0bbbcc017e9f88affddda90ea4e


</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AdrArtGar)

<details>
The following code (which compiles and runs properly on GCC and on clang 15) [godbolt](https://godbolt.org/z/boE3dc6oT)
```c++
template &lt;typename T&gt;
concept Any = true;

template &lt;typename T = void&gt; constexpr auto crash_clang() {
	return []&lt;typename...&gt;() {
		return []&lt;Any /*&lt;- remove and it compiles*/ auto&gt;() {
			return 0;
		}.template operator()&lt;0&gt;();
	}();
}

int main() {
	return crash_clang();
}
```
crashes clang with the following output
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-19 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name CrashClang.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/adrian -fcoverage-compilation-dir=/home/adrian -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/CrashClang-1dd804.o -x c++ CrashClang.cpp
1.	CrashClang.cpp:13:21: current parser token ')'
2.	CrashClang.cpp:12:12: parsing function body 'main'
3.	CrashClang.cpp:12:12: in compound statement ('{}')
4.	CrashClang.cpp:4:45: instantiating function definition 'crash_clang&lt;void&gt;'
5.	CrashClang.cpp:5:9: instantiating function definition 'crash_clang()::(anonymous class)::operator()&lt;&gt;'
 #<!-- -->0 0x00007fbe4f21a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.19.1+0x81a730)
 #<!-- -->1 0x00007fbe4f2176bd (/usr/lib/libLLVM.so.19.1+0x8176bd)
 #<!-- -->2 0x00007fbe4e44bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->3 0x00007fbe59f0f27f (/usr/lib/libclang-cpp.so.19.1+0x2d0f27f)
 #<!-- -->4 0x00007fbe5887bf2f (/usr/lib/libclang-cpp.so.19.1+0x167bf2f)
 #<!-- -->5 0x00007fbe5887597c (/usr/lib/libclang-cpp.so.19.1+0x167597c)
 #<!-- -->6 0x00007fbe58877742 (/usr/lib/libclang-cpp.so.19.1+0x1677742)
 #<!-- -->7 0x00007fbe5887caec (/usr/lib/libclang-cpp.so.19.1+0x167caec)
 #<!-- -->8 0x00007fbe59cdc96b (/usr/lib/libclang-cpp.so.19.1+0x2adc96b)
 #<!-- -->9 0x00007fbe58849c4d (/usr/lib/libclang-cpp.so.19.1+0x1649c4d)
#<!-- -->10 0x00007fbe5824d317 (/usr/lib/libclang-cpp.so.19.1+0x104d317)
#<!-- -->11 0x00007fbe5824ec0b (/usr/lib/libclang-cpp.so.19.1+0x104ec0b)
#<!-- -->12 0x00007fbe5824f0d1 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x104f0d1)
#<!-- -->13 0x00007fbe58250231 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x1050231)
#<!-- -->14 0x00007fbe587e88f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0x15e88f0)
#<!-- -->15 0x00007fbe587e99f5 (/usr/lib/libclang-cpp.so.19.1+0x15e99f5)
#<!-- -->16 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->17 0x00007fbe587f3128 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/libclang-cpp.so.19.1+0x15f3128)
#<!-- -->18 0x00007fbe59ce6af2 (/usr/lib/libclang-cpp.so.19.1+0x2ae6af2)
#<!-- -->19 0x00007fbe586f939a clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f939a)
#<!-- -->20 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->21 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#<!-- -->22 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#<!-- -->23 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#<!-- -->24 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#<!-- -->25 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->26 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#<!-- -->27 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#<!-- -->28 0x00007fbe586d1f2c (/usr/lib/libclang-cpp.so.19.1+0x14d1f2c)
#<!-- -->29 0x00007fbe586f7b79 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/usr/lib/libclang-cpp.so.19.1+0x14f7b79)
#<!-- -->30 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->31 0x00007fbe5888a15a (/usr/lib/libclang-cpp.so.19.1+0x168a15a)
#<!-- -->32 0x00007fbe58884b08 (/usr/lib/libclang-cpp.so.19.1+0x1684b08)
#<!-- -->33 0x00007fbe5889f8de (/usr/lib/libclang-cpp.so.19.1+0x169f8de)
#<!-- -->34 0x00007fbe588c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x16c88cb)
#<!-- -->35 0x00007fbe580ee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/libclang-cpp.so.19.1+0xeee5cd)
#<!-- -->36 0x00007fbe587ebe24 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.19.1+0x15ebe24)
#<!-- -->37 0x00007fbe583cc8ee clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/usr/lib/libclang-cpp.so.19.1+0x11cc8ee)
#<!-- -->38 0x00007fbe586f540c (/usr/lib/libclang-cpp.so.19.1+0x14f540c)
#<!-- -->39 0x00007fbe586f64f0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x14f64f0)
#<!-- -->40 0x00007fbe583e72b1 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x11e72b1)
#<!-- -->41 0x00007fbe583ea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/libclang-cpp.so.19.1+0x11ea0d8)
#<!-- -->42 0x00007fbe57836a2a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/lib/libclang-cpp.so.19.1+0x636a2a)
#<!-- -->43 0x00007fbe5783e457 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.19.1+0x63e457)
#<!-- -->44 0x00007fbe59ccf43a (/usr/lib/libclang-cpp.so.19.1+0x2acf43a)
#<!-- -->45 0x00007fbe59ccf50d (/usr/lib/libclang-cpp.so.19.1+0x2acf50d)
#<!-- -->46 0x00007fbe59ccf6a5 (/usr/lib/libclang-cpp.so.19.1+0x2acf6a5)
#<!-- -->47 0x00007fbe5788c5fb clang::Parser::ParseReturnStatement() (/usr/lib/libclang-cpp.so.19.1+0x68c5fb)
#<!-- -->48 0x00007fbe57891e9f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/libclang-cpp.so.19.1+0x691e9f)
#<!-- -->49 0x00007fbe578922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.19.1+0x6922a7)
#<!-- -->50 0x00007fbe5789a59a clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/libclang-cpp.so.19.1+0x69a59a)
#<!-- -->51 0x00007fbe5789ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/libclang-cpp.so.19.1+0x69ddac)
#<!-- -->52 0x00007fbe578bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/libclang-cpp.so.19.1+0x6bf6ef)
#<!-- -->53 0x00007fbe577ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.19.1+0x5ee83f)
#<!-- -->54 0x00007fbe578bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bc9d8)
#<!-- -->55 0x00007fbe578bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.19.1+0x6bd3ba)
#<!-- -->56 0x00007fbe578cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.19.1+0x6cd632)
#<!-- -->57 0x00007fbe578ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/libclang-cpp.so.19.1+0x6ce345)
#<!-- -->58 0x00007fbe577d9c18 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/libclang-cpp.so.19.1+0x5d9c18)
#<!-- -->59 0x00007fbe594fa221 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.19.1+0x22fa221)
#<!-- -->60 0x00007fbe5949008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/libclang-cpp.so.19.1+0x229008f)
#<!-- -->61 0x00007fbe59554ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.19.1+0x2354ba5)
#<!-- -->62 0x00005973f9813832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->63 0x00005973f981804e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->64 0x00005973f9818de3 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#<!-- -->65 0x00005973f980bf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->66 0x00007fbe4e435488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->67 0x00007fbe4e43554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->68 0x00007fbe4e43554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->69 0x00005973f980bfc5 _start (/usr/bin/clang-19+0xafc5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

I did a git bisect and the first bad commit was found to be babdef27c503c0bbbcc017e9f88affddda90ea4e


</details>


---

