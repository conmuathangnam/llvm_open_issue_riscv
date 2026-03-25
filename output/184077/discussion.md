# [Clang] assertion failure in name lookup when parsing enum after inline namespace

**Author:** YuchangSu
**URL:** https://github.com/llvm/llvm-project/issues/184077

## Body

Code:
```CPP
inline namespace a {
    namespace a {}
} enum a
```
See Compiler Explorer: [https://godbolt.org/z/6qcadhr78](https://godbolt.org/z/6qcadhr78)

Backtrace:
```
 #0 0x000000000440a158 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a158)
 #1 0x00000000044075a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44075a4)
 #2 0x00000000043473a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d0a8e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d0a8e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d0a8e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d0a8e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d0a8e82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d0a8e839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e1e7aa (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1e7aa)
#10 0x0000000006fb7118 clang::Sema::ActOnTag(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::CXXScopeSpec&, clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributesView const&, clang::AccessSpecifier, clang::SourceLocation, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, bool&, clang::SourceLocation, bool, clang::ActionResult<clang::OpaquePtr<clang::QualType>, false>, bool, bool, clang::OffsetOfKind, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb7118)
#11 0x0000000006c2de38 clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2de38)
#12 0x0000000006c2a065 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2a065)
#13 0x0000000006be51a7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be51a7)
#14 0x0000000006be5c2f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5c2f)
#15 0x0000000006bed231 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed231)
#16 0x0000000006bee165 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee165)
#17 0x0000000006bcf84a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf84a)
#18 0x0000000004ee3c28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee3c28)
#19 0x00000000051ea5c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51ea5c5)
#20 0x000000000516881e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x516881e)
#21 0x00000000052e627d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e627d)
#22 0x0000000000df0dce cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0dce)
#23 0x0000000000de773a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#24 0x0000000000de78bd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#25 0x0000000004f58389 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x00000000043477e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43477e4)
#27 0x0000000004f5899f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004f18682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f18682)
#29 0x0000000004f1962e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1962e)
#30 0x0000000004f21375 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21375)
#31 0x0000000000ded13e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xded13e)
#32 0x0000000000ca4fca main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4fca)
#33 0x00007d0a8e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x00007d0a8e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000de71d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde71d5)
```

The test case was generated by a fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (YuchangSu)

<details>
Code:
```CPP
inline namespace a {
    namespace a {}
} enum a
```
See Compiler Explorer: [https://godbolt.org/z/6qcadhr78](https://godbolt.org/z/6qcadhr78)

Backtrace:
```
 #<!-- -->0 0x000000000440a158 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a158)
 #<!-- -->1 0x00000000044075a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44075a4)
 #<!-- -->2 0x00000000043473a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d0a8e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d0a8e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d0a8e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d0a8e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007d0a8e82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007d0a8e839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006e1e7aa (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1e7aa)
#<!-- -->10 0x0000000006fb7118 clang::Sema::ActOnTag(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributesView const&amp;, clang::AccessSpecifier, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, bool&amp;, clang::SourceLocation, bool, clang::ActionResult&lt;clang::OpaquePtr&lt;clang::QualType&gt;, false&gt;, bool, bool, clang::OffsetOfKind, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb7118)
#<!-- -->11 0x0000000006c2de38 clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2de38)
#<!-- -->12 0x0000000006c2a065 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2a065)
#<!-- -->13 0x0000000006be51a7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be51a7)
#<!-- -->14 0x0000000006be5c2f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5c2f)
#<!-- -->15 0x0000000006bed231 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed231)
#<!-- -->16 0x0000000006bee165 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee165)
#<!-- -->17 0x0000000006bcf84a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf84a)
#<!-- -->18 0x0000000004ee3c28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee3c28)
#<!-- -->19 0x00000000051ea5c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51ea5c5)
#<!-- -->20 0x000000000516881e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x516881e)
#<!-- -->21 0x00000000052e627d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e627d)
#<!-- -->22 0x0000000000df0dce cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0dce)
#<!-- -->23 0x0000000000de773a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->24 0x0000000000de78bd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004f58389 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x00000000043477e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43477e4)
#<!-- -->27 0x0000000004f5899f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x0000000004f18682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f18682)
#<!-- -->29 0x0000000004f1962e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1962e)
#<!-- -->30 0x0000000004f21375 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21375)
#<!-- -->31 0x0000000000ded13e clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xded13e)
#<!-- -->32 0x0000000000ca4fca main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4fca)
#<!-- -->33 0x00007d0a8e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x00007d0a8e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000de71d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde71d5)
```

The test case was generated by a fuzzer.
</details>


---

### Comment 2 - shafik

As I have noted with others who are doing fuzzing against the clang frontend. We now have a large number of folks doing this type of work and so the reports really need to be high quality.

They should be focused on recent regressions (the last five releases). You need to indicate which release the regression started in.

You also need to provide a godbolt link w/ the reproducing case, as well as add the assertion/trap if any and the backtrace.

Please make sure they are not duplicates of already existing issues, if you find a duplicate and the code is sufficiently different then add your example to the existing issue. Issues they are duplicates will just be closed.

---

