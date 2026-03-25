# [clang] Clang crashes when parsing invalid typedef with overloaded operator syntax and unnamed class type

**Author:** YuchangSu
**URL:** https://github.com/llvm/llvm-project/issues/177549

## Body

```c++
typedef(( operator | <> ) ) ( class  { private : });
```

See Compiler Explorer: [https://godbolt.org/z/7oYbzj55e](https://godbolt.org/z/7oYbzj55e)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (YuchangSu)

<details>
`typedef(( operator | &lt;&gt; ) ) ( class  { private : }); `
See Compiler Explorer: [https://godbolt.org/z/7oYbzj55e](https://godbolt.org/z/7oYbzj55e)
</details>


---

### Comment 2 - nehaGautam07

I am working on this. Can anyone assign this to me.

---

### Comment 3 - shafik

> I am working on this. Can anyone assign this to me.

We assign issues after there is a PR and we have verified it is moving in the right direction. Commenting on the issue is helpful to let others know your intentions.

---

### Comment 4 - shafik

goes back to clang-11.

Backtrace:

```console
 #0 0x00000000043cb438 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cb438)
 #1 0x00000000043c8894 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c8894)
 #2 0x0000000004308ba8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d4041442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000082a5bf5 clang::TemplateName::getKind() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x82a5bf5)
 #5 0x0000000007aca5f6 clang::ASTContext::getNameForTemplate(clang::TemplateName, clang::SourceLocation) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7aca5f6)
 #6 0x0000000006f22a95 clang::Sema::GetNameFromUnqualifiedId(clang::UnqualifiedId const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f22a95)
 #7 0x0000000006f7fe4b clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7fe4b)
 #8 0x0000000006f811f0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f811f0)
 #9 0x0000000006bb3d53 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb3d53)
#10 0x0000000006bc1b7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc1b7a)
#11 0x0000000006bca87f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bca87f)
#12 0x0000000006bcad4c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcad4c)
#13 0x0000000006b87906 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b87906)
#14 0x0000000006b88815 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b88815)
#15 0x0000000006b88c00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b88c00)
#16 0x0000000006b69eb3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b69eb3)
#17 0x0000000004d458e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d458e8)
#18 0x00000000050439c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50439c5)
#19 0x0000000004fc2cde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fc2cde)
#20 0x000000000513f02d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513f02d)
#21 0x0000000000de86de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde86de)
#22 0x0000000000ddf0aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#23 0x0000000000ddf22d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#24 0x0000000004db9319 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000000004309044 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4309044)
#26 0x0000000004db992f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x0000000004d7a322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7a322)
#28 0x0000000004d7b2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7b2ce)
#29 0x0000000004d82585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d82585)
#30 0x0000000000de4a51 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4a51)
#31 0x0000000000c9e9b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9e9b4)
#32 0x00007d4041429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x00007d4041429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000ddeb45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddeb45)
c
```

---

### Comment 5 - shafik

Looks strongly related if not a duplicate of: https://github.com/llvm/llvm-project/issues/139387

---

