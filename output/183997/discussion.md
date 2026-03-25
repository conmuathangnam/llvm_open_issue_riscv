# [X86AsmParser] Assertion `*FirstTokOffset == FirstCharOffset' failed with -fms-extensions

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183997
**Status:** Open
**Labels:** backend:X86, llvm:asmparser, crash

## Body

Reproducer:
https://godbolt.org/z/cG5azrjKK
```cpp
void foo() {
  __asm mov eax, 1uwb
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Parse/ParseStmtAsm.cpp:150: void {anonymous}::ClangAsmParserCallback::findTokensForString(llvm::StringRef, llvm::SmallVectorImpl<clang::Token>&, const clang::Token*&) const: Assertion `*FirstTokOffset == FirstCharOffset' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fms-extensions <source>
1.	<source>:3:1: current parser token '}'
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
 #0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076ef8b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076ef8b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076ef8b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076ef8b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076ef8b62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076ef8b639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006cef1d5 (anonymous namespace)::ClangAsmParserCallback::LookupInlineAsmIdentifier(llvm::StringRef&, llvm::InlineAsmIdentifierInfo&, bool) ParseStmtAsm.cpp:0:0
#10 0x0000000002f7de24 (anonymous namespace)::X86AsmParser::ParseIntelInlineAsmIdentifier(llvm::MCExpr const*&, llvm::StringRef&, llvm::InlineAsmIdentifierInfo&, bool, llvm::SMLoc&, bool) X86AsmParser.cpp:0:0
#11 0x0000000002f80750 (anonymous namespace)::X86AsmParser::ParseIntelExpression((anonymous namespace)::X86AsmParser::IntelExprStateMachine&, llvm::SMLoc&) X86AsmParser.cpp:0:0
#12 0x0000000002f84ed2 (anonymous namespace)::X86AsmParser::parseIntelOperand(llvm::SmallVectorImpl<std::unique_ptr<llvm::MCParsedAsmOperand, std::default_delete<llvm::MCParsedAsmOperand>>>&, llvm::StringRef) X86AsmParser.cpp:0:0
#13 0x0000000002f8a79d (anonymous namespace)::X86AsmParser::parseInstruction(llvm::ParseInstructionInfo&, llvm::StringRef, llvm::SMLoc, llvm::SmallVectorImpl<std::unique_ptr<llvm::MCParsedAsmOperand, std::default_delete<llvm::MCParsedAsmOperand>>>&) X86AsmParser.cpp:0:0
#14 0x000000000414b7f2 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#15 0x000000000415ffe3 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#16 0x0000000004154de8 (anonymous namespace)::AsmParser::parseMSInlineAsm(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, unsigned int&, unsigned int&, llvm::SmallVectorImpl<std::pair<void*, bool>>&, llvm::SmallVectorImpl<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>&, llvm::SmallVectorImpl<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>&, llvm::MCInstrInfo const*, llvm::MCInstPrinter*, llvm::MCAsmParserSemaCallback&) AsmParser.cpp:0:0
#17 0x0000000006cf1a32 clang::Parser::ParseMicrosoftAsmStatement(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf1a32)
#18 0x0000000006cf49d0 clang::Parser::ParseAsmStatement(bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf49d0)
#19 0x0000000006ce23d7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce23d7)
#20 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#21 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#22 0x0000000006cec25a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec25a)
#23 0x0000000006bf18c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf18c3)
#24 0x0000000006c27f5d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27f5d)
#25 0x0000000006be52bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be52bb)
#26 0x0000000006be5abf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5abf)
#27 0x0000000006bed0c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed0c1)
#28 0x0000000006bedff5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bedff5)
#29 0x0000000006bee3e0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee3e0)
#30 0x0000000006bcf6a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6a3)
#31 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#32 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#33 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#34 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#35 0x0000000000df0afe cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#36 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000de75ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004f58f39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#40 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#42 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#43 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#44 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#45 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#46 0x000076ef8b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000076ef8b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/cG5azrjKK
```cpp
void foo() {
  __asm mov eax, 1uwb
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Parse/ParseStmtAsm.cpp:150: void {anonymous}::ClangAsmParserCallback::findTokensForString(llvm::StringRef, llvm::SmallVectorImpl&lt;clang::Token&gt;&amp;, const clang::Token*&amp;) const: Assertion `*FirstTokOffset == FirstCharOffset' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fms-extensions &lt;source&gt;
1.	&lt;source&gt;:3:1: current parser token '}'
2.	&lt;source&gt;:1:12: parsing function body 'foo'
3.	&lt;source&gt;:1:12: in compound statement ('{}')
 #<!-- -->0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #<!-- -->1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #<!-- -->2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076ef8b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076ef8b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076ef8b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076ef8b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076ef8b62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076ef8b639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006cef1d5 (anonymous namespace)::ClangAsmParserCallback::LookupInlineAsmIdentifier(llvm::StringRef&amp;, llvm::InlineAsmIdentifierInfo&amp;, bool) ParseStmtAsm.cpp:0:0
#<!-- -->10 0x0000000002f7de24 (anonymous namespace)::X86AsmParser::ParseIntelInlineAsmIdentifier(llvm::MCExpr const*&amp;, llvm::StringRef&amp;, llvm::InlineAsmIdentifierInfo&amp;, bool, llvm::SMLoc&amp;, bool) X86AsmParser.cpp:0:0
#<!-- -->11 0x0000000002f80750 (anonymous namespace)::X86AsmParser::ParseIntelExpression((anonymous namespace)::X86AsmParser::IntelExprStateMachine&amp;, llvm::SMLoc&amp;) X86AsmParser.cpp:0:0
#<!-- -->12 0x0000000002f84ed2 (anonymous namespace)::X86AsmParser::parseIntelOperand(llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;, llvm::StringRef) X86AsmParser.cpp:0:0
#<!-- -->13 0x0000000002f8a79d (anonymous namespace)::X86AsmParser::parseInstruction(llvm::ParseInstructionInfo&amp;, llvm::StringRef, llvm::SMLoc, llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;) X86AsmParser.cpp:0:0
#<!-- -->14 0x000000000414b7f2 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&amp;, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#<!-- -->15 0x000000000415ffe3 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&amp;, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#<!-- -->16 0x0000000004154de8 (anonymous namespace)::AsmParser::parseMSInlineAsm(std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;, unsigned int&amp;, unsigned int&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;void*, bool&gt;&gt;&amp;, llvm::SmallVectorImpl&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&amp;, llvm::SmallVectorImpl&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&amp;, llvm::MCInstrInfo const*, llvm::MCInstPrinter*, llvm::MCAsmParserSemaCallback&amp;) AsmParser.cpp:0:0
#<!-- -->17 0x0000000006cf1a32 clang::Parser::ParseMicrosoftAsmStatement(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf1a32)
#<!-- -->18 0x0000000006cf49d0 clang::Parser::ParseAsmStatement(bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf49d0)
#<!-- -->19 0x0000000006ce23d7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce23d7)
#<!-- -->20 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->21 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#<!-- -->22 0x0000000006cec25a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec25a)
#<!-- -->23 0x0000000006bf18c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf18c3)
#<!-- -->24 0x0000000006c27f5d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27f5d)
#<!-- -->25 0x0000000006be52bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be52bb)
#<!-- -->26 0x0000000006be5abf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5abf)
#<!-- -->27 0x0000000006bed0c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed0c1)
#<!-- -->28 0x0000000006bedff5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bedff5)
#<!-- -->29 0x0000000006bee3e0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee3e0)
#<!-- -->30 0x0000000006bcf6a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6a3)
#<!-- -->31 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#<!-- -->32 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#<!-- -->33 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#<!-- -->34 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#<!-- -->35 0x0000000000df0afe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#<!-- -->36 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->37 0x0000000000de75ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004f58f39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#<!-- -->40 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#<!-- -->42 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#<!-- -->43 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#<!-- -->44 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#<!-- -->45 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#<!-- -->46 0x000076ef8b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000076ef8b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-3.3:
https://godbolt.org/z/xKK3G5Ga4

---

