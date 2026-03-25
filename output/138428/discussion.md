# [clang] Crash due to lambdas in decltype (?)

**Author:** dankmeme01
**URL:** https://github.com/llvm/llvm-project/issues/138428

## Body

Managed to reduce the code to the following:

```cpp
template <typename F>
auto test(F&& f) {}

int main() {
    auto _x = []() -> decltype(test([] {})) {};
}
```

```
$ clang++ /tmp/test.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/llvm/bin/clang-19 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk -fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk -resource-dir /opt/llvm/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward -internal-isystem /opt/llvm/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-db0ac7.o -x c++ /tmp/test.cpp
1.	/tmp/test.cpp:5:42: current parser token ')'
2.	/tmp/test.cpp:4:12: parsing function body 'main'
3.	/tmp/test.cpp:4:12: in compound statement ('{}')
4.	/tmp/test.cpp:5:15: lambda expression parsing
5.	/tmp/test.cpp:2:6: instantiating function definition 'test<(lambda at /tmp/test.cpp:5:37)>'
6.	/tmp/test.cpp:2:6: LLVM IR generation of declaration 'test'
7.	/tmp/test.cpp:2:6: Mangling declaration 'test'
 #0 0x000000000322b3b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm/bin/clang-19+0x322b3b8)
 #1 0x0000000003228707 SignalHandler(int) Signals.cpp:0:0
 #2 0x00007fc6a6d39c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #3 0x0000000006a4ef44 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #4 0x0000000006a44521 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #5 0x0000000006a4751d (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #6 0x0000000006a56a20 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #7 0x0000000006a53e2d (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) ItaniumMangle.cpp:0:0
 #8 0x0000000006a4ed17 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #9 0x0000000006a51442 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#10 0x0000000003617c97 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#11 0x000000000361da24 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/llvm/bin/clang-19+0x361da24)
#12 0x0000000003639472 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/llvm/bin/clang-19+0x3639472)
#13 0x00000000036429bb clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#14 0x0000000003b2ddf0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#15 0x0000000003b1ef70 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/llvm/bin/clang-19+0x3b1ef70)
#16 0x0000000006276c90 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/llvm/bin/clang-19+0x6276c90)
#17 0x000000000574f032 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/llvm/bin/clang-19+0x574f032)
#18 0x0000000006104227 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/llvm/bin/clang-19+0x6104227)
#19 0x0000000005b1332a clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/llvm/bin/clang-19+0x5b1332a)
#20 0x0000000005f91058 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#21 0x0000000005f92523 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/llvm/bin/clang-19+0x5f92523)
#22 0x0000000005bb6021 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/llvm/bin/clang-19+0x5bb6021)
#23 0x0000000005bb855b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/llvm/bin/clang-19+0x5bb855b)
#24 0x000000000565785c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/llvm/bin/clang-19+0x565785c)
#25 0x00000000056502e1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x56502e1)
#26 0x0000000005652b99 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x5652b99)
#27 0x0000000005652d5a clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x5652d5a)
#28 0x000000000565752e clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x565752e)
#29 0x0000000005630c10 clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&) (/opt/llvm/bin/clang-19+0x5630c10)
#30 0x000000000566dd67 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool) (/opt/llvm/bin/clang-19+0x566dd67)
#31 0x00000000055c870c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/llvm/bin/clang-19+0x55c870c)
#32 0x000000000561029b clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/llvm/bin/clang-19+0x561029b)
#33 0x00000000056120df clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/llvm/bin/clang-19+0x56120df)
#34 0x00000000056125c4 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/llvm/bin/clang-19+0x56125c4)
#35 0x0000000005673f63 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/llvm/bin/clang-19+0x5673f63)
#36 0x0000000005674c0b clang::Parser::ParseLambdaExpression() (/opt/llvm/bin/clang-19+0x5674c0b)
#37 0x000000000565256d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x565256d)
#38 0x0000000005652b99 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x5652b99)
#39 0x0000000005652d5a clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x5652d5a)
#40 0x000000000560600d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x560600d)
#41 0x000000000561e886 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x561e886)
#42 0x00000000056202cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x56202cf)
#43 0x0000000005620773 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x5620773)
#44 0x00000000056d433b clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/llvm/bin/clang-19+0x56d433b)
#45 0x00000000056d4d05 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x56d4d05)
#46 0x00000000056d603a clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm/bin/clang-19+0x56d603a)
#47 0x00000000056d6bf7 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/llvm/bin/clang-19+0x56d6bf7)
#48 0x00000000055cfeb2 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/llvm/bin/clang-19+0x55cfeb2)
#49 0x000000000561ed7f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x561ed7f)
#50 0x00000000055c92b7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/llvm/bin/clang-19+0x55c92b7)
#51 0x00000000055ca100 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm/bin/clang-19+0x55ca100)
#52 0x00000000055d347d clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/llvm/bin/clang-19+0x55d347d)
#53 0x00000000055d4555 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/llvm/bin/clang-19+0x55d4555)
#54 0x00000000055c328c clang::ParseAST(clang::Sema&, bool, bool) (/opt/llvm/bin/clang-19+0x55c328c)
#55 0x0000000003e009c9 clang::FrontendAction::Execute() (/opt/llvm/bin/clang-19+0x3e009c9)
#56 0x0000000003d7fb32 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/llvm/bin/clang-19+0x3d7fb32)
#57 0x0000000003ed1401 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/bin/clang-19+0x3ed1401)
#58 0x000000000058e0ad cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/llvm/bin/clang-19+0x58e0ad)
#59 0x0000000000587004 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#60 0x00000000005899cb clang_main(int, char**, llvm::ToolContext const&) (/opt/llvm/bin/clang-19+0x5899cb)
#61 0x000000000044b45f main (/opt/llvm/bin/clang-19+0x44b45f)
#62 0x00007fc6a6d235f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#63 0x00007fc6a6d236a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#64 0x0000000000586a45 _start (/opt/llvm/bin/clang-19+0x586a45)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7 (https://github.com/llvm/llvm-project.git cd708029e0b2869e80abe31ddb175f7c35361f90)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-43376c.cpp
clang++: note: diagnostic msg: /tmp/test-43376c.sh
clang++: note: diagnostic msg: 

********************
```

Compile script:
```
# Crash reproducer for clang version 19.1.7 (https://github.com/llvm/llvm-project.git cd708029e0b2869e80abe31ddb175f7c35361f90)
# Driver args: "--driver-mode=g++" "/tmp/test.cpp"
# Original command:  "/opt/llvm/bin/clang-19" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "test.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk" "-resource-dir" "/opt/llvm/lib/clang/19" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward" "-internal-isystem" "/opt/llvm/lib/clang/19/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/test-db0ac7.o" "-x" "c++" "/tmp/test.cpp"
 "/opt/llvm/bin/clang-19" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "test.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "test-43376c.cpp"
```

Preprocessed source:
```
# 1 "<built-in>"
# 1 "/tmp/test.cpp"
template <typename F>
auto test(F&& f) {}

int main() {
    auto _x = []() -> decltype(test([] {})) {};
}
```

Tested this with the latest Clang on godbolt.org (20.1.0) and the crash persists in that version too: https://godbolt.org/z/YMszedzdx

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (dankmeme01)

<details>
Managed to reduce the code to the following:

```cpp
template &lt;typename F&gt;
auto test(F&amp;&amp; f) {}

int main() {
    auto _x = []() -&gt; decltype(test([] {})) {};
}
```

```
$ clang++ /tmp/test.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/llvm/bin/clang-19 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk -fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk -resource-dir /opt/llvm/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward -internal-isystem /opt/llvm/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-db0ac7.o -x c++ /tmp/test.cpp
1.	/tmp/test.cpp:5:42: current parser token ')'
2.	/tmp/test.cpp:4:12: parsing function body 'main'
3.	/tmp/test.cpp:4:12: in compound statement ('{}')
4.	/tmp/test.cpp:5:15: lambda expression parsing
5.	/tmp/test.cpp:2:6: instantiating function definition 'test&lt;(lambda at /tmp/test.cpp:5:37)&gt;'
6.	/tmp/test.cpp:2:6: LLVM IR generation of declaration 'test'
7.	/tmp/test.cpp:2:6: Mangling declaration 'test'
 #<!-- -->0 0x000000000322b3b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/bin/clang-19+0x322b3b8)
 #<!-- -->1 0x0000000003228707 SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007fc6a6d39c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #<!-- -->3 0x0000000006a4ef44 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->4 0x0000000006a44521 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
 #<!-- -->5 0x0000000006a4751d (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #<!-- -->6 0x0000000006a56a20 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #<!-- -->7 0x0000000006a53e2d (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) ItaniumMangle.cpp:0:0
 #<!-- -->8 0x0000000006a4ed17 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->9 0x0000000006a51442 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
#<!-- -->10 0x0000000003617c97 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->11 0x000000000361da24 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/llvm/bin/clang-19+0x361da24)
#<!-- -->12 0x0000000003639472 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/llvm/bin/clang-19+0x3639472)
#<!-- -->13 0x00000000036429bb clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->14 0x0000000003b2ddf0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->15 0x0000000003b1ef70 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/llvm/bin/clang-19+0x3b1ef70)
#<!-- -->16 0x0000000006276c90 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/llvm/bin/clang-19+0x6276c90)
#<!-- -->17 0x000000000574f032 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/llvm/bin/clang-19+0x574f032)
#<!-- -->18 0x0000000006104227 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/llvm/bin/clang-19+0x6104227)
#<!-- -->19 0x0000000005b1332a clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/llvm/bin/clang-19+0x5b1332a)
#<!-- -->20 0x0000000005f91058 FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#<!-- -->21 0x0000000005f92523 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/llvm/bin/clang-19+0x5f92523)
#<!-- -->22 0x0000000005bb6021 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/llvm/bin/clang-19+0x5bb6021)
#<!-- -->23 0x0000000005bb855b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/llvm/bin/clang-19+0x5bb855b)
#<!-- -->24 0x000000000565785c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/llvm/bin/clang-19+0x565785c)
#<!-- -->25 0x00000000056502e1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x56502e1)
#<!-- -->26 0x0000000005652b99 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x5652b99)
#<!-- -->27 0x0000000005652d5a clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x5652d5a)
#<!-- -->28 0x000000000565752e clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x565752e)
#<!-- -->29 0x0000000005630c10 clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&amp;) (/opt/llvm/bin/clang-19+0x5630c10)
#<!-- -->30 0x000000000566dd67 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool) (/opt/llvm/bin/clang-19+0x566dd67)
#<!-- -->31 0x00000000055c870c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/llvm/bin/clang-19+0x55c870c)
#<!-- -->32 0x000000000561029b clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/llvm/bin/clang-19+0x561029b)
#<!-- -->33 0x00000000056120df clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/llvm/bin/clang-19+0x56120df)
#<!-- -->34 0x00000000056125c4 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/llvm/bin/clang-19+0x56125c4)
#<!-- -->35 0x0000000005673f63 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/llvm/bin/clang-19+0x5673f63)
#<!-- -->36 0x0000000005674c0b clang::Parser::ParseLambdaExpression() (/opt/llvm/bin/clang-19+0x5674c0b)
#<!-- -->37 0x000000000565256d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x565256d)
#<!-- -->38 0x0000000005652b99 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang-19+0x5652b99)
#<!-- -->39 0x0000000005652d5a clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang-19+0x5652d5a)
#<!-- -->40 0x000000000560600d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x560600d)
#<!-- -->41 0x000000000561e886 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x561e886)
#<!-- -->42 0x00000000056202cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x56202cf)
#<!-- -->43 0x0000000005620773 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x5620773)
#<!-- -->44 0x00000000056d433b clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/llvm/bin/clang-19+0x56d433b)
#<!-- -->45 0x00000000056d4d05 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/llvm/bin/clang-19+0x56d4d05)
#<!-- -->46 0x00000000056d603a clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm/bin/clang-19+0x56d603a)
#<!-- -->47 0x00000000056d6bf7 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/llvm/bin/clang-19+0x56d6bf7)
#<!-- -->48 0x00000000055cfeb2 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/llvm/bin/clang-19+0x55cfeb2)
#<!-- -->49 0x000000000561ed7f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang-19+0x561ed7f)
#<!-- -->50 0x00000000055c92b7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/llvm/bin/clang-19+0x55c92b7)
#<!-- -->51 0x00000000055ca100 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm/bin/clang-19+0x55ca100)
#<!-- -->52 0x00000000055d347d clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/llvm/bin/clang-19+0x55d347d)
#<!-- -->53 0x00000000055d4555 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/llvm/bin/clang-19+0x55d4555)
#<!-- -->54 0x00000000055c328c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/llvm/bin/clang-19+0x55c328c)
#<!-- -->55 0x0000000003e009c9 clang::FrontendAction::Execute() (/opt/llvm/bin/clang-19+0x3e009c9)
#<!-- -->56 0x0000000003d7fb32 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/llvm/bin/clang-19+0x3d7fb32)
#<!-- -->57 0x0000000003ed1401 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/bin/clang-19+0x3ed1401)
#<!-- -->58 0x000000000058e0ad cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/llvm/bin/clang-19+0x58e0ad)
#<!-- -->59 0x0000000000587004 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->60 0x00000000005899cb clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/llvm/bin/clang-19+0x5899cb)
#<!-- -->61 0x000000000044b45f main (/opt/llvm/bin/clang-19+0x44b45f)
#<!-- -->62 0x00007fc6a6d235f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#<!-- -->63 0x00007fc6a6d236a8 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#<!-- -->64 0x0000000000586a45 _start (/opt/llvm/bin/clang-19+0x586a45)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7 (https://github.com/llvm/llvm-project.git cd708029e0b2869e80abe31ddb175f7c35361f90)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-43376c.cpp
clang++: note: diagnostic msg: /tmp/test-43376c.sh
clang++: note: diagnostic msg: 

********************
```

Compile script:
```
# Crash reproducer for clang version 19.1.7 (https://github.com/llvm/llvm-project.git cd708029e0b2869e80abe31ddb175f7c35361f90)
# Driver args: "--driver-mode=g++" "/tmp/test.cpp"
# Original command:  "/opt/llvm/bin/clang-19" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "test.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk" "-resource-dir" "/opt/llvm/lib/clang/19" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward" "-internal-isystem" "/opt/llvm/lib/clang/19/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/test-db0ac7.o" "-x" "c++" "/tmp/test.cpp"
 "/opt/llvm/bin/clang-19" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "test.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fcoverage-compilation-dir=/home/dankpc/programming/geode3/sdk" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "test-43376c.cpp"
```

Preprocessed source:
```
# 1 "&lt;built-in&gt;"
# 1 "/tmp/test.cpp"
template &lt;typename F&gt;
auto test(F&amp;&amp; f) {}

int main() {
    auto _x = []() -&gt; decltype(test([] {})) {};
}
```

Tested this with the latest Clang on godbolt.org (20.1.0) and the crash persists in that version too: https://godbolt.org/z/v7v1oPdxT
</details>


---

### Comment 2 - shafik

Maybe: https://github.com/llvm/llvm-project/issues/132076

---

### Comment 3 - AaronBallman

> Maybe: [#132076](https://github.com/llvm/llvm-project/issues/132076)

I don't think they're related; for this issue, I get the same stack trace on Windows as Ubuntu.

---

### Comment 4 - jongmyeong-choi

I think the expected behavior should be as shown in the ast dump:

```
$ clang++ -Xclang -ast-dump -fsyntax-only test2.cpp
test2.cpp:5:37: error: lambda expression in an unevaluated operand
    5 |     auto _x = []() -> decltype(test([] {})) {};
      |                                     ^
```

---

### Comment 5 - jongmyeong-choi

The crash occurs in `makeFunctionReturnTypeTags` at:
```cpp
const FunctionProtoType *Proto =
    cast<FunctionProtoType>(FD->getType()->getAs<FunctionType>());
```

- `FD` is the lambda's `operator()` (function call operator)
- `FD->getType()` returns a NULL type
- The cast operation fails when trying to access a NULL type pointer

The lambda's function call operator type appears to be incomplete when name mangling is attempted, possibly due to template instantiation occurring in an unevaluated context before lambda type setup is complete. There's a relevant FIXME comment in `SemaTemplateInstantiateDecl.cpp`:
```cpp
// FIXME: finishing the function body while in an expression evaluation
// context seems wrong. Investigate more.
```

---

### Comment 6 - shafik

CC @cor3ntin 

---

