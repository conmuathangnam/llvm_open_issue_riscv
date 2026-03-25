# [clang] Assertion `EnumDeclBits.NumPositiveBits == Num && "can't store this bitcount"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140346

## Body

Reproducer:
https://godbolt.org/z/oYsceKfW7
```c
enum A { A0 = 99999999999999999999999999999999999999999999999999999999999999999999999999999wb};
```

Diagnostics:

```console
<source>:1:15: warning: '_BitInt' suffix for literals is a C23 extension [-Wc23-extensions]
    1 | enum A { A0 = 99999999999999999999999999999999999999999999999999999999999999999999999999999wb};
      |               ^
<source>:1:6: warning: enumeration values exceed range of largest integer [-Wenum-too-large]
    1 | enum A { A0 = 99999999999999999999999999999999999999999999999999999999999999999999999999999wb};
      |      ^
```

Backtrace:

```console
clang: /root/llvm-project/llvm/tools/clang/include/clang/AST/Decl.h:3980: void clang::EnumDecl::setNumPositiveBits(unsigned int): Assertion `EnumDeclBits.NumPositiveBits == Num && "can't store this bitcount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:95: current parser token ';'
 #0 0x0000000003f99d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f99d48)
 #1 0x0000000003f979d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f979d4)
 #2 0x0000000003edc608 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000733c83042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000733c830969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000733c83042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000733c830287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000733c8302871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000733c83039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000774ed3f clang::EnumDecl::completeDefinition(clang::QualType, clang::QualType, unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x774ed3f)
#10 0x00000000069d8635 clang::Sema::ActOnEnumBody(clang::SourceLocation, clang::SourceRange, clang::Decl*, llvm::ArrayRef<clang::Decl*>, clang::Scope*, clang::ParsedAttributesView const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69d8635)
#11 0x0000000006672402 clang::Parser::ParseEnumBody(clang::SourceLocation, clang::Decl*, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6672402)
#12 0x000000000667af0c clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667af0c)
#13 0x000000000667702e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667702e)
#14 0x0000000006631a44 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6631a44)
#15 0x00000000066324a9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66324a9)
#16 0x0000000006639dba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6639dba)
#17 0x000000000663ad5d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663ad5d)
#18 0x000000000663b210 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b210)
#19 0x000000000662d0f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662d0f3)
#20 0x0000000004944fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4944fd8)
#21 0x0000000004c3cea5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c3cea5)
#22 0x0000000004bbbb3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bbbb3e)
#23 0x0000000004d30a49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d30a49)
#24 0x0000000000dac29f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdac29f)
#25 0x0000000000da246a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049ac2c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003edcaa4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3edcaa4)
#28 0x00000000049ac8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000496e6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496e6ad)
#30 0x000000000496f73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496f73e)
#31 0x0000000004977a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4977a25)
#32 0x0000000000da8208 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda8208)
#33 0x0000000000c2df44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2df44)
#34 0x0000733c83029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x0000733c83029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000da1f15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda1f15)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/oYsceKfW7
```c
enum A { A0 = 99999999999999999999999999999999999999999999999999999999999999999999999999999wb};
```
</details>


---

### Comment 2 - shafik

These are fuzzer generated right? Please add the generated by fuzzer label to them.

---

### Comment 3 - k-arrows

> These are fuzzer generated right? Please add the generated by fuzzer label to them.

No. The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.dg/bitint-14.c

---

### Comment 4 - shafik

> > These are fuzzer generated right? Please add the generated by fuzzer label to them.
> 
> No. The original reproducer of this issue is the following: https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.dg/bitint-14.c

So your recent bug reports have all been from the test suite or do you have a bigger corpus you are working from?

---

### Comment 5 - shafik

CC @AaronBallman it looks like _BitInt is required to make this happen

---

### Comment 6 - k-arrows

> > > These are fuzzer generated right? Please add the generated by fuzzer label to them.
> > 
> > 
> > No. The original reproducer of this issue is the following: https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.dg/bitint-14.c
> 
> So your recent bug reports have all been from the test suite or do you have a bigger corpus you are working from?

Not everything, but there are many that come from the test suite. What should I do? For example, if there is a label like 'from gcc test suite', I would use that.

---

### Comment 7 - k-arrows

As a pattern that does not come from the test suite, I have recently been using the method of generating mutants and variants with an LLM, based on known bugs or the test suite. I am not sure if this method should be called fuzzing.

---

### Comment 8 - k-arrows

>What should I do? For example, if there is a label like 'from gcc test suite', I would use that.

One more suggestion I can make is to create a project to record the issues found with the GCC test suite. For example, Flang has a project that deals with issues from the GCC test suite and the Fujitsu test suite.
https://github.com/orgs/llvm/projects/17
https://github.com/orgs/llvm/projects/20

---

### Comment 9 - AaronBallman

We cannot add GCC's tests to our test suite due to licensing issues (in fact, we probably should not be taking GCC test suite contents and adding them to our issue tracker). I have absolutely no idea how GPL interacts with LLM-based fuzzers in terms of "derivative" works, either. CC @llvm-beanz for input, we may need a lawyer.

---

### Comment 10 - zwuis

> As a pattern that does not come from the test suite, I have recently been using the method of generating mutants and variants with an LLM, based on known bugs or the test suite. I am not sure if this method should be called fuzzing.

CC @tbaederr 

---

### Comment 11 - tbaederr

> > As a pattern that does not come from the test suite, I have recently been using the method of generating mutants and variants with an LLM, based on known bugs or the test suite. I am not sure if this method should be called fuzzing.
> 
> CC [@tbaederr](https://github.com/tbaederr)

Low priority bug reports that don't come from real-world code and are produced en masse. How they are generated in detail isn't really the interesting point IMO.

---

### Comment 12 - shafik

> > > As a pattern that does not come from the test suite, I have recently been using the method of generating mutants and variants with an LLM, based on known bugs or the test suite. I am not sure if this method should be called fuzzing.
> > 
> > 
> > CC [@tbaederr](https://github.com/tbaederr)
> 
> Low priority bug reports that don't come from real-world code and are produced en masse. How they are generated in detail isn't really the interesting point IMO.

We should avoid absolutes here. 

Some of the fuzzing and related work have had some success in finding regressions in recent releases. Finding regressions in recent releases should be considered very helpful work. 

---

