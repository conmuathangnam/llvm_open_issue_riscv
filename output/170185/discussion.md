# [clang]Clang crash in Itanium mangling for lambda with NTTP of incomplete type

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/170185

## Body



Clang crashes (assertions build) on the following code starting from 15.0.0, and it is still reproducible on trunk:

```cpp
struct Foo;

template <Foo X>
struct Arr;

Foo f;

void foo() {
    []() {
        [&](Arr<f>) {

        };
    }();
}
```

https://godbolt.org/z/dqfqvzYr6

GCC/EDG/MSVC all correctly reject the program without crashing.

I (naively) think: Clang crashes while mangling a lambda whose parameter type is a class template instantiated with a non-type template argument of an incomplete type.  

Output:

```text
<source>:3:15: error: non-type template parameter has incomplete type 'Foo'
    3 | template <Foo X>
      |               ^
<source>:1:8: note: forward declaration of 'Foo'
    1 | struct Foo;
      |        ^
<source>:5:5: error: variable has incomplete type 'Foo'
    5 | Foo f;
      |     ^
<source>:1:8: note: forward declaration of 'Foo'
    1 | struct Foo;
      |        ^
unexpected statement kind
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:4943!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:21: current parser token '{'
2.	<source>:6:12: parsing function body 'foo'
3.	<source>:6:12: in compound statement ('{}')
4.	<source>:7:5: lambda expression parsing
5.	<source>:7:10: in compound statement ('{}')
6.	<source>:8:9: lambda expression parsing
 #0 0x000000000427b8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x427b8f8)
 #1 0x0000000004278d24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4278d24)
 #2 0x00000000041bca58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e196dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e196dc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e196dc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e196dc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000041c872a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c872a)
 #8 0x0000000007efcdfc (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #9 0x0000000007f04b22 (anonymous namespace)::CXXNameMangler::mangleTemplateArgExpr(clang::Expr const*) ItaniumMangle.cpp:0:0
#10 0x0000000007f0c8c0 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#11 0x0000000007f0b423 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>) ItaniumMangle.cpp:0:0
#12 0x0000000007f0cd4e (anonymous namespace)::CXXNameMangler::mangleTemplateName(clang::TemplateDecl const*, llvm::ArrayRef<clang::TemplateArgument>) ItaniumMangle.cpp:0:0
#13 0x0000000007ef48f0 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#14 0x0000000007f019ce (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#15 0x0000000007f0d51e (anonymous namespace)::ItaniumMangleContextImpl::mangleLambdaSig(clang::CXXRecordDecl const*, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#16 0x0000000007ee5a49 (anonymous namespace)::ItaniumNumberingContext::getManglingNumber(clang::CXXMethodDecl const*) ItaniumCXXABI.cpp:0:0
#17 0x00000000070447a5 clang::Sema::handleLambdaNumbering(clang::CXXRecordDecl*, clang::CXXMethodDecl*, std::optional<clang::CXXRecordDecl::LambdaNumbering>) (.part.0) SemaLambda.cpp:0:0
#18 0x000000000704a34f clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&, clang::Declarator&, clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x704a34f)
#19 0x0000000006a11f78 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11f78)
#20 0x0000000006a1334b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1334b)
#21 0x00000000069f3935 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f3935)
#22 0x00000000069f4357 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f4357)
#23 0x00000000069f43e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f43e9)
#24 0x00000000069f89c9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f89c9)
#25 0x0000000006a7e039 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e039)
#26 0x0000000006a756c2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a756c2)
#27 0x0000000006a76086 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a76086)
#28 0x0000000006a7e80e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e80e)
#29 0x0000000006a12262 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12262)
#30 0x0000000006a1334b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1334b)
#31 0x00000000069f3935 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f3935)
#32 0x00000000069f4357 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f4357)
#33 0x00000000069f43e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f43e9)
#34 0x00000000069f89c9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f89c9)
#35 0x0000000006a7e039 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e039)
#36 0x0000000006a756c2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a756c2)
#37 0x0000000006a76086 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a76086)
#38 0x0000000006a7e80e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e80e)
#39 0x0000000006a7f05a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f05a)
#40 0x0000000006984ebb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6984ebb)
#41 0x00000000069bb2dd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bb2dd)
#42 0x000000000697866e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697866e)
#43 0x0000000006978e0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6978e0f)
#44 0x00000000069806c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69806c3)
#45 0x00000000069815d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69815d5)
#46 0x0000000006962bba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6962bba)
#47 0x0000000004bcb0d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcb0d8)
#48 0x0000000004eb6945 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eb6945)
#49 0x0000000004e36afe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e36afe)
#50 0x0000000004faf97d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4faf97d)
#51 0x0000000000dc7740 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7740)
#52 0x0000000000dbe1ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#53 0x0000000000dbe33d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#54 0x0000000004c338c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#55 0x00000000041bcef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41bcef4)
#56 0x0000000004c33edf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#57 0x0000000004bf46d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf46d2)
#58 0x0000000004bf567e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf567e)
#59 0x0000000004bfcab5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfcab5)
#60 0x0000000000dc3b61 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3b61)
#61 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72574)
#62 0x00007e196dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#63 0x00007e196dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#64 0x0000000000dbdc55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdc55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>


Clang crashes (assertions build) on the following code starting from 15.0.0, and it is still reproducible on trunk:

```cpp
struct Foo;

template &lt;Foo X&gt;
struct Arr;

Foo f;

void foo() {
    []() {
        [&amp;](Arr&lt;f&gt;) {

        };
    }();
}
```

https://godbolt.org/z/dqfqvzYr6

GCC/EDG/MSVC all correctly reject the program without crashing.

I (naively) think: Clang crashes while mangling a lambda whose parameter type is a class template instantiated with a non-type template argument of an incomplete type.  

Output:

```text
&lt;source&gt;:3:15: error: non-type template parameter has incomplete type 'Foo'
    3 | template &lt;Foo X&gt;
      |               ^
&lt;source&gt;:1:8: note: forward declaration of 'Foo'
    1 | struct Foo;
      |        ^
&lt;source&gt;:5:5: error: variable has incomplete type 'Foo'
    5 | Foo f;
      |     ^
&lt;source&gt;:1:8: note: forward declaration of 'Foo'
    1 | struct Foo;
      |        ^
unexpected statement kind
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:4943!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:8:21: current parser token '{'
2.	&lt;source&gt;:6:12: parsing function body 'foo'
3.	&lt;source&gt;:6:12: in compound statement ('{}')
4.	&lt;source&gt;:7:5: lambda expression parsing
5.	&lt;source&gt;:7:10: in compound statement ('{}')
6.	&lt;source&gt;:8:9: lambda expression parsing
 #<!-- -->0 0x000000000427b8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x427b8f8)
 #<!-- -->1 0x0000000004278d24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4278d24)
 #<!-- -->2 0x00000000041bca58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e196dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e196dc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e196dc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e196dc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000041c872a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c872a)
 #<!-- -->8 0x0000000007efcdfc (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #<!-- -->9 0x0000000007f04b22 (anonymous namespace)::CXXNameMangler::mangleTemplateArgExpr(clang::Expr const*) ItaniumMangle.cpp:0:0
#<!-- -->10 0x0000000007f0c8c0 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#<!-- -->11 0x0000000007f0b423 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) ItaniumMangle.cpp:0:0
#<!-- -->12 0x0000000007f0cd4e (anonymous namespace)::CXXNameMangler::mangleTemplateName(clang::TemplateDecl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) ItaniumMangle.cpp:0:0
#<!-- -->13 0x0000000007ef48f0 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#<!-- -->14 0x0000000007f019ce (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000007f0d51e (anonymous namespace)::ItaniumMangleContextImpl::mangleLambdaSig(clang::CXXRecordDecl const*, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
#<!-- -->16 0x0000000007ee5a49 (anonymous namespace)::ItaniumNumberingContext::getManglingNumber(clang::CXXMethodDecl const*) ItaniumCXXABI.cpp:0:0
#<!-- -->17 0x00000000070447a5 clang::Sema::handleLambdaNumbering(clang::CXXRecordDecl*, clang::CXXMethodDecl*, std::optional&lt;clang::CXXRecordDecl::LambdaNumbering&gt;) (.part.0) SemaLambda.cpp:0:0
#<!-- -->18 0x000000000704a34f clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&amp;, clang::Declarator&amp;, clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x704a34f)
#<!-- -->19 0x0000000006a11f78 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11f78)
#<!-- -->20 0x0000000006a1334b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1334b)
#<!-- -->21 0x00000000069f3935 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f3935)
#<!-- -->22 0x00000000069f4357 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f4357)
#<!-- -->23 0x00000000069f43e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f43e9)
#<!-- -->24 0x00000000069f89c9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f89c9)
#<!-- -->25 0x0000000006a7e039 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e039)
#<!-- -->26 0x0000000006a756c2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a756c2)
#<!-- -->27 0x0000000006a76086 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a76086)
#<!-- -->28 0x0000000006a7e80e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e80e)
#<!-- -->29 0x0000000006a12262 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12262)
#<!-- -->30 0x0000000006a1334b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1334b)
#<!-- -->31 0x00000000069f3935 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f3935)
#<!-- -->32 0x00000000069f4357 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f4357)
#<!-- -->33 0x00000000069f43e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f43e9)
#<!-- -->34 0x00000000069f89c9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f89c9)
#<!-- -->35 0x0000000006a7e039 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e039)
#<!-- -->36 0x0000000006a756c2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a756c2)
#<!-- -->37 0x0000000006a76086 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a76086)
#<!-- -->38 0x0000000006a7e80e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e80e)
#<!-- -->39 0x0000000006a7f05a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f05a)
#<!-- -->40 0x0000000006984ebb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6984ebb)
#<!-- -->41 0x00000000069bb2dd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bb2dd)
#<!-- -->42 0x000000000697866e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697866e)
#<!-- -->43 0x0000000006978e0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6978e0f)
#<!-- -->44 0x00000000069806c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69806c3)
#<!-- -->45 0x00000000069815d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69815d5)
#<!-- -->46 0x0000000006962bba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6962bba)
#<!-- -->47 0x0000000004bcb0d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcb0d8)
#<!-- -->48 0x0000000004eb6945 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eb6945)
#<!-- -->49 0x0000000004e36afe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e36afe)
#<!-- -->50 0x0000000004faf97d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4faf97d)
#<!-- -->51 0x0000000000dc7740 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7740)
#<!-- -->52 0x0000000000dbe1ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->53 0x0000000000dbe33d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->54 0x0000000004c338c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->55 0x00000000041bcef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41bcef4)
#<!-- -->56 0x0000000004c33edf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->57 0x0000000004bf46d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf46d2)
#<!-- -->58 0x0000000004bf567e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf567e)
#<!-- -->59 0x0000000004bfcab5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfcab5)
#<!-- -->60 0x0000000000dc3b61 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3b61)
#<!-- -->61 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72574)
#<!-- -->62 0x00007e196dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->63 0x00007e196dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->64 0x0000000000dbdc55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdc55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - efriedma-quic

The "unexpected statement kind" is a RecoveryExpr.  Probably handleLambdaNumbering should be bailing out on errors, or something like that.

---

