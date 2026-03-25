# Clang ICE: `invalid parameter index` on some attributes with invalid indices applied to varargs functions

**Author:** langston-barrett
**URL:** https://github.com/llvm/llvm-project/issues/61635
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash-on-invalid, awaiting-review, confirmed
**Closed Date:** 2025-04-01T11:56:38Z

## Body

This is a fuzzer-generated bug. Feel free to close if you're not interested in such reports. If you would like more reports of fuzzer-generated bugs, let me know and I can post them. I read [the bug report guidelines](https://llvm.org/docs/HowToSubmitABug.html), and did not find information on whether or not fuzzer-generated bugs were welcome.

Clang crashes on this program:
```c
__attribute__ ((__format_arg__(2)))
void test (int i, ...) { }
```
with this message ([Godbolt](https://godbolt.org/z/oszqjxGzn)):
```console
clang++: /root/llvm-project/clang/include/clang/AST/Type.h:4249: clang::QualType clang::FunctionProtoType::getParamType(unsigned int) const: Assertion `i < getNumParams() && "invalid parameter index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -c -O0 <source>
1.	<source>:2:24: current parser token '{'
 #0 0x000055767e07d78f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x400478f)
 #1 0x000055767e07b4cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40024cc)
 #2 0x000055767dfc8b38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6519e42420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f651990f00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f65198ee859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f65198ee729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f65198fffd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000557680ac32d5 getFunctionOrMethodParamType(clang::Decl const*, unsigned int) SemaDeclAttr.cpp:0:0
 #9 0x0000557680af2357 handleFormatArgAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
#10 0x0000557680b1dcee ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#11 0x0000557680b1ebeb clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa5beb)
#12 0x0000557680ab5470 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3c470)
#13 0x0000557680abb0cf clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a420cf)
#14 0x0000557680abbc13 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Sema::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a42c13)
#15 0x0000557680754792 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66db792)
#16 0x00005576807805a0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67075a0)
#17 0x00005576807486f2 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cf6f2)
#18 0x0000557680748faf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#19 0x000055768074fa39 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d6a39)
#20 0x000055768075038d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d738d)
#21 0x0000557680750854 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d7854)
#22 0x000055768074422a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cb22a)
#23 0x000055767f2a4798 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x522b798)
#24 0x000055767eb09979 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a90979)
#25 0x000055767ea8dea6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a14ea6)
#26 0x000055767ebedb37 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b74b37)
#27 0x000055767b608ef6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x158fef6)
#28 0x000055767b604d1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x000055767e8eeb0d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x000055767dfc9020 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f50020)
#31 0x000055767e8ef3cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x000055767e8b6c0c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483dc0c)
#33 0x000055767e8b76ad clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483e6ad)
#34 0x000055767e8bf35d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484635d)
#35 0x000055767b6073a0 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x158e3a0)
#36 0x000055767b5134b5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x149a4b5)
#37 0x00007f65198f0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#38 0x000055767b5ffa2e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1586a2e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This looks like a Clang problem, because it still happens with `-emit-llvm -Xclang -disable-llvm-passes`.

I searched like so for duplicates, but didn't find any:

- https://github.com/llvm/llvm-project/issues?q=is%3Aissue+is%3Aopen+label%3Aclang%3Afrontend+__format_arg__+
- https://github.com/llvm/llvm-project/issues?q=is%3Aissue+is%3Aopen+label%3Aclang%3Afrontend+invalid+parameter+index+

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

This is a good bug report while the code is invalid we should generate a proper diagnostic like gcc does: https://godbolt.org/z/PrEvnfKK8

This is probably an easy mistake for someone to make if they are using the attribute the first time or typo the index etc 

On the other hand we don't have the bandwidth for mass submission of fuzzer bugs but carefully curated bug submissions are definitely welcome @AaronBallman do you have additional feedback on this?

---

### Comment 3 - tbaederr

Should be an easy fix, just compare the `Idx` with `getFunctionOrMethodNumParams(D)`.

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-good-first-issue

---

### Comment 5 - AaronBallman

> This is a good bug report while the code is invalid we should generate a proper diagnostic like gcc does: https://godbolt.org/z/PrEvnfKK8
> 
> This is probably an easy mistake for someone to make if they are using the attribute the first time or typo the index etc

Agreed, this is a good issue to fix (and shouldn't be too challenging).

> On the other hand we don't have the bandwidth for mass submission of fuzzer bugs but carefully curated bug submissions are definitely welcome @AaronBallman do you have additional feedback on this?

You nailed it -- if the fuzzer produces code that could be reasonably plausible for a user to write (like this one), then it's a great bug report to file. For implausible code, we'd be fine accepting a patch to fix the issue if it's a crash but would probably not prioritize the bug too highly otherwise. (If the code is borderline, I'd say go ahead and add an issue for it.)

---

### Comment 6 - langston-barrett

Thanks for the feedback @shafik, @AaronBallman! I'll endeavor to post useful bugs as I find them, feel free to let me know if they're straying outside of the realm of reports you want.

This test case exhibits a similar issue; not sure whether to file a separate bug or just note it here.
```c
void __attribute__ ((alloc_size (2, 3))) *test(int i, ...);
```

---

### Comment 7 - AaronBallman

Eh, let's note it here -- it's a semi-distinct issue, but it's the same kind of problem between both attributes (so the fix for one might fix the other, depending on how we decide to address it).

Would you mind editing the summary and title though, so it's clear there's two-ish issues here?

---

### Comment 8 - samuelmaina

Hello @tbaederr , what does this mean >`Should be an easy fix, just compare the Idx with getFunctionOrMethodNumParams(D)`.I am able to track to the function where the error is emerging which is this 
```
static QualType getFunctionOrMethodParamType(const Decl *D, unsigned Idx) {
  if (const FunctionType *FnTy = D->getFunctionType())
    return cast<FunctionProtoType>(FnTy)->getParamType(Idx);
  if (const auto *BD = dyn_cast<BlockDecl>(D))
    return BD->getParamDecl(Idx)->getType();

  return cast<ObjCMethodDecl>(D)->parameters()[Idx]->getType();
}
```

What changes should I make for the comparison?

---

### Comment 9 - tbaederr

That is not the function where the error is reported though. You need to find the place where that happens and check if the given index is greater than the number of parameters.

---

### Comment 10 - samuelmaina

The crash emerges from the assertion failing in the  getParamType after being called in the above function. 
``` 
QualType getParamType(unsigned i) const {
    assert(i < getNumParams() && "invalid parameter index");
    return param_type_begin()[i];
  }
  ```
  I guess my task is to return a nice warning or error message  maybe > `  attribute argument value '2' exceeds the number of function parameters 1` .


---

### Comment 11 - samuelmaina

Where can I find classes and function for creating Warmng or Error messages?
I am a begginer that why I may ask very basic questions. Bare with me.

---

### Comment 12 - samuelmaina

Even if I compare the Idx and the result of getFunctionOrMethodNumParams(D) in the getFunctionOrMethodParamType function above , I still can't use an assertion since it will cause a crush.

---

### Comment 13 - tbaederr

Yes, you need to do that _before_ the assertion is reached, and from the function that calls `getFunctionOrMethodParamType`

---

### Comment 14 - ipriyanshi1708

> Even if I compare the Idx and the result of getFunctionOrMethodNumParams(D) in the getFunctionOrMethodParamType function above , I still can't use an assertion since it will cause a crush.

I am also working on this issue. Can we collaborate?

---

### Comment 15 - samuelmaina

Sure.


---

### Comment 16 - SamTebbs33

From brief investigation it looks like we want the `bad` function here to produce the same error message as GCC: https://godbolt.org/z/761MhbPYP

---

### Comment 17 - xgupta

Adding posted patch link  - https://reviews.llvm.org/D147037

---

### Comment 18 - disconnect3d

Ping, can we get this fixed anytime _soon_? :)

---

