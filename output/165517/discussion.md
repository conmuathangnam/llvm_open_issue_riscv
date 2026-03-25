# [CLANG]  Clang crashes when target_clones attribute is only on a declaration

**Author:** EbinJose2002
**URL:** https://github.com/llvm/llvm-project/issues/165517
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, crash
**Closed Date:** 2025-11-24T18:49:43Z

## Body

According to the Clang docs , [https://clang.llvm.org/docs/AttributeReference.html#target-clones](url)  for target_clones attribute:
`"This attribute may be attached to a function declaration and causes function multiversioning," `
However, with a simple test, Clang appears to require that the attribute be on the function definition, not just the declaration. When the attribute is only on the declaration and the definition lacks it, Clang crashes in CodeGen with “None multiversion type isn't valid here” and hits an UNREACHABLE.

Clang Version: 21.1.0
```
__attribute__((target_clones("arch=znver4", "default")))
void foo(int n, double*);


void foo(int n, double* X) {
    for ( int i = 0 ; i < n ; i++ ) {
        X[i] *= 2.0;
    }
}
```
```
clang -O3 -c foo.c 

None multiversion type isn't valid here
UNREACHABLE executed at /llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:2089!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ../llvm_upstream/install_dir/bin/clang -O3 -c foo.c
1.      <eof> parser at end of file
2.      foo.c:4:6: LLVM IR generation of declaration 'foo'
3.      foo.c:4:6: Generating code for declaration 'foo'
 #0 0x00007f70faf8f880 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f8880)
 #1 0x00007f70faf8c52f llvm::sys::RunSignalHandlers() (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f552f)
 #2 0x00007f70fae7bf98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f70fa642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f70fa6969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f70fa6969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f70fa6969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f70fa642476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f70fa6287f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f70fae9917e (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x10217e)
#10 0x00007f70ff3ee8a7 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#11 0x00007f70ff4023f4 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5983f4)
#12 0x00007f70ff42e8fd clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c48fd)
#13 0x00007f70ff42ef01 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c4f01)
#14 0x00007f70ff4320f7 clang::CodeGen::CodeGenModule::EmitMultiVersionFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c80f7)
#15 0x00007f70ff4299c5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5bf9c5)
#16 0x00007f70ff42a790 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c0790)
#17 0x00007f70ff4342a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#18 0x00007f70ff4dbdd0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#19 0x00007f70ff3a07b1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5367b1)
#20 0x00007f70f7bf8c74 clang::ParseAST(clang::Sema&, bool, bool) (/install_dir/bin/../lib/../lib/libclangParse.so.21.1+0x42c74)
#21 0x00007f70fd80b5c9 clang::FrontendAction::Execute() (/install_dir/bin/../lib/libclangFrontend.so.21.1+0x1455c9)
#22 0x00007f70fd77be05 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/install_dir/bin/../lib/libclangFrontend.so.21.1+0xb5e05)
#23 0x00007f71000a8c08 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/install_dir/bin/../lib/libclangFrontendTool.so.21.1+0x5c08)
#24 0x0000562a543a85fe cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (../llvm_upstream/install_dir/bin/clang+0x175fe)
#25 0x0000562a5439e81a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00007f70fd39569d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x00007f70fae7c440 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0xe5440)
#28 0x00007f70fd395f3e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x00007f70fd34db91 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x79b91)
#30 0x00007f70fd34ecb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x7acb1)
#31 0x00007f70fd35fce4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/install_dir/bin/../lib/libclangDriver.so.21.1+0x8bce4)
#32 0x0000562a543a40d7 clang_main(int, char**, llvm::ToolContext const&) (../llvm_upstream/install_dir/bin/clang+0x130d7)
#33 0x0000562a5439deeb main (../llvm_upstream/install_dir/bin/clang+0xceeb)
#34 0x00007f70fa629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#35 0x00007f70fa629e40 call_init ./csu/../csu/libc-start.c:128:20
#36 0x00007f70fa629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#37 0x0000562a5439df45 _start (../llvm_upstream/install_dir/bin/clang+0xcf45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.0 (https://github.com/llvm/llvm-project.git 3623fe661ae35c6c80ac221f14d85be76aa870f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /install_dir/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/foo-1c1aca.c
clang: note: diagnostic msg: /tmp/foo-1c1aca.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Ebin-McW (EbinJose2002)

<details>
According to the Clang docs , [https://clang.llvm.org/docs/AttributeReference.html#target-clones](url)  for target_clones attribute:
`"This attribute may be attached to a function declaration and causes function multiversioning," `
However, with a simple test, Clang appears to require that the attribute be on the function definition, not just the declaration. When the attribute is only on the declaration and the definition lacks it, Clang crashes in CodeGen with “None multiversion type isn't valid here” and hits an UNREACHABLE.

Clang Version: 21.1.0
```
__attribute__((target_clones("arch=znver4", "default")))
void foo(int n, double*);


void foo(int n, double* X) {
    for ( int i = 0 ; i &lt; n ; i++ ) {
        X[i] *= 2.0;
    }
}
```
```
clang -O3 -c foo.c 

None multiversion type isn't valid here
UNREACHABLE executed at /llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:2089!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ../llvm_upstream/install_dir/bin/clang -O3 -c foo.c
1.      &lt;eof&gt; parser at end of file
2.      foo.c:4:6: LLVM IR generation of declaration 'foo'
3.      foo.c:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00007f70faf8f880 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f8880)
 #<!-- -->1 0x00007f70faf8c52f llvm::sys::RunSignalHandlers() (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f552f)
 #<!-- -->2 0x00007f70fae7bf98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f70fa642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f70fa6969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f70fa6969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f70fa6969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f70fa642476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f70fa6287f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f70fae9917e (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x10217e)
#<!-- -->10 0x00007f70ff3ee8a7 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->11 0x00007f70ff4023f4 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5983f4)
#<!-- -->12 0x00007f70ff42e8fd clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c48fd)
#<!-- -->13 0x00007f70ff42ef01 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c4f01)
#<!-- -->14 0x00007f70ff4320f7 clang::CodeGen::CodeGenModule::EmitMultiVersionFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c80f7)
#<!-- -->15 0x00007f70ff4299c5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5bf9c5)
#<!-- -->16 0x00007f70ff42a790 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c0790)
#<!-- -->17 0x00007f70ff4342a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->18 0x00007f70ff4dbdd0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->19 0x00007f70ff3a07b1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5367b1)
#<!-- -->20 0x00007f70f7bf8c74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/install_dir/bin/../lib/../lib/libclangParse.so.21.1+0x42c74)
#<!-- -->21 0x00007f70fd80b5c9 clang::FrontendAction::Execute() (/install_dir/bin/../lib/libclangFrontend.so.21.1+0x1455c9)
#<!-- -->22 0x00007f70fd77be05 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/install_dir/bin/../lib/libclangFrontend.so.21.1+0xb5e05)
#<!-- -->23 0x00007f71000a8c08 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/install_dir/bin/../lib/libclangFrontendTool.so.21.1+0x5c08)
#<!-- -->24 0x0000562a543a85fe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (../llvm_upstream/install_dir/bin/clang+0x175fe)
#<!-- -->25 0x0000562a5439e81a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00007f70fd39569d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00007f70fae7c440 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0xe5440)
#<!-- -->28 0x00007f70fd395f3e clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x00007f70fd34db91 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x79b91)
#<!-- -->30 0x00007f70fd34ecb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x7acb1)
#<!-- -->31 0x00007f70fd35fce4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/install_dir/bin/../lib/libclangDriver.so.21.1+0x8bce4)
#<!-- -->32 0x0000562a543a40d7 clang_main(int, char**, llvm::ToolContext const&amp;) (../llvm_upstream/install_dir/bin/clang+0x130d7)
#<!-- -->33 0x0000562a5439deeb main (../llvm_upstream/install_dir/bin/clang+0xceeb)
#<!-- -->34 0x00007f70fa629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->35 0x00007f70fa629e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007f70fa629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->37 0x0000562a5439df45 _start (../llvm_upstream/install_dir/bin/clang+0xcf45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.0 (https://github.com/llvm/llvm-project.git 3623fe661ae35c6c80ac221f14d85be76aa870f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /install_dir/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/foo-1c1aca.c
clang: note: diagnostic msg: /tmp/foo-1c1aca.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - shafik

This looks like a modified version of https://github.com/llvm/llvm-project/issues/130850 and https://github.com/llvm/llvm-project/issues/129483

CC @erichkeane 

---

### Comment 3 - erichkeane

Hmm.... looks like we're doing a bad job inherit'ing this attribute during redeclaration?  Though #130850 looks a little different.  I wonder if this broke during various refactors of how that works.

The `getMultiVersionKind` is checking the attribute, and despite us having set the `isMultiVersion` appropriately, we're not getting the attribute inherited or something?  I would very much expect it to be attached as an inherited version.

I'm going to mark this as 'good-first-issue', since I suspect it is just a case of bad-attribute-merging, and should be reasonably easy to track down?  Perhaps we're just opting-out of the normal inherit mechanism by accident?

Either way, I'll help whoever starts it as best i can.

---

### Comment 4 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 5 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Ebin-McW (EbinJose2002)

<details>
According to the Clang docs , [https://clang.llvm.org/docs/AttributeReference.html#target-clones](url)  for target_clones attribute:
`"This attribute may be attached to a function declaration and causes function multiversioning," `
However, with a simple test, Clang appears to require that the attribute be on the function definition, not just the declaration. When the attribute is only on the declaration and the definition lacks it, Clang crashes in CodeGen with “None multiversion type isn't valid here” and hits an UNREACHABLE.

Clang Version: 21.1.0
```
__attribute__((target_clones("arch=znver4", "default")))
void foo(int n, double*);


void foo(int n, double* X) {
    for ( int i = 0 ; i &lt; n ; i++ ) {
        X[i] *= 2.0;
    }
}
```
```
clang -O3 -c foo.c 

None multiversion type isn't valid here
UNREACHABLE executed at /llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:2089!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ../llvm_upstream/install_dir/bin/clang -O3 -c foo.c
1.      &lt;eof&gt; parser at end of file
2.      foo.c:4:6: LLVM IR generation of declaration 'foo'
3.      foo.c:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00007f70faf8f880 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f8880)
 #<!-- -->1 0x00007f70faf8c52f llvm::sys::RunSignalHandlers() (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f552f)
 #<!-- -->2 0x00007f70fae7bf98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f70fa642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f70fa6969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f70fa6969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f70fa6969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f70fa642476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f70fa6287f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f70fae9917e (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x10217e)
#<!-- -->10 0x00007f70ff3ee8a7 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->11 0x00007f70ff4023f4 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5983f4)
#<!-- -->12 0x00007f70ff42e8fd clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c48fd)
#<!-- -->13 0x00007f70ff42ef01 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c4f01)
#<!-- -->14 0x00007f70ff4320f7 clang::CodeGen::CodeGenModule::EmitMultiVersionFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c80f7)
#<!-- -->15 0x00007f70ff4299c5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5bf9c5)
#<!-- -->16 0x00007f70ff42a790 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c0790)
#<!-- -->17 0x00007f70ff4342a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->18 0x00007f70ff4dbdd0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->19 0x00007f70ff3a07b1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5367b1)
#<!-- -->20 0x00007f70f7bf8c74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/install_dir/bin/../lib/../lib/libclangParse.so.21.1+0x42c74)
#<!-- -->21 0x00007f70fd80b5c9 clang::FrontendAction::Execute() (/install_dir/bin/../lib/libclangFrontend.so.21.1+0x1455c9)
#<!-- -->22 0x00007f70fd77be05 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/install_dir/bin/../lib/libclangFrontend.so.21.1+0xb5e05)
#<!-- -->23 0x00007f71000a8c08 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/install_dir/bin/../lib/libclangFrontendTool.so.21.1+0x5c08)
#<!-- -->24 0x0000562a543a85fe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (../llvm_upstream/install_dir/bin/clang+0x175fe)
#<!-- -->25 0x0000562a5439e81a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00007f70fd39569d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00007f70fae7c440 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0xe5440)
#<!-- -->28 0x00007f70fd395f3e clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x00007f70fd34db91 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x79b91)
#<!-- -->30 0x00007f70fd34ecb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x7acb1)
#<!-- -->31 0x00007f70fd35fce4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/install_dir/bin/../lib/libclangDriver.so.21.1+0x8bce4)
#<!-- -->32 0x0000562a543a40d7 clang_main(int, char**, llvm::ToolContext const&amp;) (../llvm_upstream/install_dir/bin/clang+0x130d7)
#<!-- -->33 0x0000562a5439deeb main (../llvm_upstream/install_dir/bin/clang+0xceeb)
#<!-- -->34 0x00007f70fa629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->35 0x00007f70fa629e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007f70fa629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->37 0x0000562a5439df45 _start (../llvm_upstream/install_dir/bin/clang+0xcf45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.0 (https://github.com/llvm/llvm-project.git 3623fe661ae35c6c80ac221f14d85be76aa870f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /install_dir/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/foo-1c1aca.c
clang: note: diagnostic msg: /tmp/foo-1c1aca.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 6 - henrybw

@erichkeane Hi! I would like to work on this.

Looks like we should probably be handling this somewhere in Sema? I’m not sure what the normal inherit mechanism here is supposed to be, though.

---

### Comment 7 - henrybw

> Hmm.... looks like we're doing a bad job inherit'ing this attribute during redeclaration? Though [#130850](https://github.com/llvm/llvm-project/issues/130850) looks a little different. I wonder if this broke during various refactors of how that works.
> 
> The `getMultiVersionKind` is checking the attribute, and despite us having set the `isMultiVersion` appropriately, we're not getting the attribute inherited or something? I would very much expect it to be attached as an inherited version.

@erichkeane I tried the repro from this issue on both 9deab60 ("Implement target_clones multiversioning") and fc53eb6 ("Reapply 'Implement target_clones multiversioning'"), and it still hits the same `llvm_unreachable` call, so I don't think this is a regression.

It appears that, when we set the multiversion-ness of the second function declaration (i.e. the function definition) in `CheckMultiVersionAdditionalDecl`, we also set `Redeclaration` to false, which seems incorrect. This prevents `Sema::MergeFunctionDecl` from being called, which prevents the attribute from being inherited.

(More specifically, in `CheckMultiVersionAdditionalDecl`, we're reaching the [last case at the bottom of the function](https://github.com/llvm/llvm-project/blob/c9b41699978132e83abf2ce4dd63eb21e69f846a/clang/lib/Sema/SemaDecl.cpp#L12015-L12019) that sets `Redeclaration` to false, but also sets `isMultiVersion` on the new function declaration and clears the previous declaration.)

I think we can fix this by modifying [this case](https://github.com/llvm/llvm-project/blob/c9b41699978132e83abf2ce4dd63eb21e69f846a/clang/lib/Sema/SemaDecl.cpp#L12006-L12013) near the end of `CheckMultiVersionAdditionalDecl` to also accept `MultiVersionKind::None` redeclarations, since we've [already asserted](https://github.com/llvm/llvm-project/blob/c9b41699978132e83abf2ce4dd63eb21e69f846a/clang/lib/Sema/SemaDecl.cpp#L11829-L11832) that the old declaration was `MultiVersionKind::TargetClones`.

In other words, something like this:
```diff
--- a/clang/lib/Sema/SemaDecl.cpp
+++ b/clang/lib/Sema/SemaDecl.cpp
@@ -12005,7 +12005,7 @@ static bool CheckMultiVersionAdditionalDecl(
   }
 
   // Permit forward declarations in the case where these two are compatible.
-  if (!OldFD->isMultiVersion()) {
+  if (!OldFD->isMultiVersion() || NewMVKind == MultiVersionKind::None) {
     OldFD->setIsMultiVersion();
     NewFD->setIsMultiVersion();
     Redeclaration = true;
```

Does this make sense and seem reasonable to you?

---

### Comment 8 - erichkeane

Hmm... The idea is that the functions are NOT redeclarations of eachother in the cases where they are 'different' function versions.  For example, `cpu_dispatch`/`cpu_specific`/`target`. In those cases, we treat each individual function as a different declaration, thus they are not redeclarations.

I presume the problem here is specific to `target_clones`, where each declaration IS part of the same declaration chain.  

So I think your `something like this` is at least close to correct?  The intent was to allow cases where we had the old declaration with NO annotation (that itself wasn't a definition), but added multiversioning.  So for example:

```
void foo();
void foo() __attribute__((cpu_dispatch("whatever")));`
```

Can you make sure you do a number of similar tests/redeclarations on the various kinds of multiversioning?  

Please add me to any such review!  

---

### Comment 9 - henrybw

> Hmm... The idea is that the functions are NOT redeclarations of eachother in the cases where they are 'different' function versions. For example, `cpu_dispatch`/`cpu_specific`/`target`. In those cases, we treat each individual function as a different declaration, thus they are not redeclarations.
> 
> I presume the problem here is specific to `target_clones`, where each declaration IS part of the same declaration chain.

Ah, I didn't realize that the "different" function versions are generally not supposed to be considered redeclarations of each other. Thanks for clarifying that.

Yeah, the problem here is indeed specific to `target_clones` (on non-AArch64/RISC-V targets), where each declaration should refer to the same group of functions:

```c
__attribute__((target_clones("whatever"))) void foo(void);
void foo(void) {} // should have the same `__attribute__((target_clones("whatever")))`
```

So that's why I was thinking that the first declaration (with the annotation) is effectively a "compatible forward declaration" for the second.

As for the other kinds of multiversioning, I don't think we really have any other cases that are comparable to this? Most of the other attributes that cause multiversioning don't seem to allow implicit attribute inheritance, because they either do not allow redeclarations, or require that any redeclarations explicitly have either the same (or a compatible) attribute.

That said, there is the implicit "default" `target_version` case on AArch64/RISC-V:

```c
__attribute__((target_version("default"))) void foo(void);
void foo(void) {} // has an implicit `__attribute__((target_version("default")))`
```

However, this seems like a special case: even though these ARE redeclarations of each other, the implicit attribute is added by a special `patchDefaultTargetVersion` function, not the usual attribute inheritance/merging for redeclarations.

Anyway, I have also been consulting the various `clang/test/Sema/attr-*` tests (`attr-target-clones.c`, `attr-cpuspecific.c`, etc.) to understand how the other kinds of function multiversioning are supposed to work, and I will be sure to update the relevant tests when I have a patch ready (and include you on the review as well).

Thank you for the help so far!

---

### Comment 10 - erichkeane

> > Hmm... The idea is that the functions are NOT redeclarations of eachother in the cases where they are 'different' function versions. For example, `cpu_dispatch`/`cpu_specific`/`target`. In those cases, we treat each individual function as a different declaration, thus they are not redeclarations.
> > I presume the problem here is specific to `target_clones`, where each declaration IS part of the same declaration chain.
> 
> Ah, I didn't realize that the "different" function versions are generally not supposed to be considered redeclarations of each other. Thanks for clarifying that.

Yeah, this bit is a little odd, but it only applies to the 'target' and 'cpu-dispatch/cpu-specific' behavior.  For target_clones (IIRC), there is only ONE variant allowed per 'chain', so those ARE all redeclarations of eachother.
> 
> Yeah, the problem here is indeed specific to `target_clones` (on non-AArch64/RISC-V targets), where each declaration should refer to the same group of functions:
> 
> __attribute__((target_clones("whatever"))) void foo(void);
> void foo(void) {} // should have the same `__attribute__((target_clones("whatever")))`
> 
> So that's why I was thinking that the first declaration (with the annotation) is effectively a "compatible forward declaration" for the second.
> 

Yeah, I think thats right.  TBH, target_clones was implemented way after the rest, so I could definitely believe I'd forgotten some of the details of the implementation when doing that one.

> As for the other kinds of multiversioning, I don't think we really have any other cases that are comparable to this? Most of the other attributes that cause multiversioning don't seem to allow implicit attribute inheritance, because they either do not allow redeclarations, or require that any redeclarations explicitly have either the same (or a compatible) attribute.

`target` is a little weird (it is actually only multiversioning when certain strings are seen, or the SECOND 'target' is seen, else they are redeclarations), but yeah, basically this.
> 
> That said, there is the implicit "default" `target_version` case on AArch64/RISC-V:
> 
> __attribute__((target_version("default"))) void foo(void);
> void foo(void) {} // has an implicit `__attribute__((target_version("default")))`
> 
> However, this seems like a special case: even though these ARE redeclarations of each other, the implicit attribute is added by a special `patchDefaultTargetVersion` function, not the usual attribute inheritance/merging for redeclarations.
> 
> Anyway, I have also been consulting the various `clang/test/Sema/attr-*` tests (`attr-target-clones.c`, `attr-cpuspecific.c`, etc.) to understand how the other kinds of function multiversioning are supposed to work, and I will be sure to update the relevant tests when I have a patch ready (and include you on the review as well).
> 
> Thank you for the help so far!

Thank you!  Let me know if you get stuck, and feel free to reach out.



---

### Comment 11 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ebin-McW (EbinJose2002)

<details>
According to the Clang docs , [https://clang.llvm.org/docs/AttributeReference.html#target-clones](url)  for target_clones attribute:
`"This attribute may be attached to a function declaration and causes function multiversioning," `
However, with a simple test, Clang appears to require that the attribute be on the function definition, not just the declaration. When the attribute is only on the declaration and the definition lacks it, Clang crashes in CodeGen with “None multiversion type isn't valid here” and hits an UNREACHABLE.

Clang Version: 21.1.0
```
__attribute__((target_clones("arch=znver4", "default")))
void foo(int n, double*);


void foo(int n, double* X) {
    for ( int i = 0 ; i &lt; n ; i++ ) {
        X[i] *= 2.0;
    }
}
```
```
clang -O3 -c foo.c 

None multiversion type isn't valid here
UNREACHABLE executed at /llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:2089!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ../llvm_upstream/install_dir/bin/clang -O3 -c foo.c
1.      &lt;eof&gt; parser at end of file
2.      foo.c:4:6: LLVM IR generation of declaration 'foo'
3.      foo.c:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00007f70faf8f880 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f8880)
 #<!-- -->1 0x00007f70faf8c52f llvm::sys::RunSignalHandlers() (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x1f552f)
 #<!-- -->2 0x00007f70fae7bf98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f70fa642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f70fa6969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f70fa6969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f70fa6969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f70fa642476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f70fa6287f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f70fae9917e (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0x10217e)
#<!-- -->10 0x00007f70ff3ee8a7 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->11 0x00007f70ff4023f4 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5983f4)
#<!-- -->12 0x00007f70ff42e8fd clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c48fd)
#<!-- -->13 0x00007f70ff42ef01 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c4f01)
#<!-- -->14 0x00007f70ff4320f7 clang::CodeGen::CodeGenModule::EmitMultiVersionFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c80f7)
#<!-- -->15 0x00007f70ff4299c5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5bf9c5)
#<!-- -->16 0x00007f70ff42a790 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5c0790)
#<!-- -->17 0x00007f70ff4342a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->18 0x00007f70ff4dbdd0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->19 0x00007f70ff3a07b1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/install_dir/bin/../lib/libclangCodeGen.so.21.1+0x5367b1)
#<!-- -->20 0x00007f70f7bf8c74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/install_dir/bin/../lib/../lib/libclangParse.so.21.1+0x42c74)
#<!-- -->21 0x00007f70fd80b5c9 clang::FrontendAction::Execute() (/install_dir/bin/../lib/libclangFrontend.so.21.1+0x1455c9)
#<!-- -->22 0x00007f70fd77be05 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/install_dir/bin/../lib/libclangFrontend.so.21.1+0xb5e05)
#<!-- -->23 0x00007f71000a8c08 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/install_dir/bin/../lib/libclangFrontendTool.so.21.1+0x5c08)
#<!-- -->24 0x0000562a543a85fe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (../llvm_upstream/install_dir/bin/clang+0x175fe)
#<!-- -->25 0x0000562a5439e81a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00007f70fd39569d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00007f70fae7c440 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/install_dir/bin/../lib/libLLVMSupport.so.21.1+0xe5440)
#<!-- -->28 0x00007f70fd395f3e clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x00007f70fd34db91 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x79b91)
#<!-- -->30 0x00007f70fd34ecb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/install_dir/bin/../lib/libclangDriver.so.21.1+0x7acb1)
#<!-- -->31 0x00007f70fd35fce4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/install_dir/bin/../lib/libclangDriver.so.21.1+0x8bce4)
#<!-- -->32 0x0000562a543a40d7 clang_main(int, char**, llvm::ToolContext const&amp;) (../llvm_upstream/install_dir/bin/clang+0x130d7)
#<!-- -->33 0x0000562a5439deeb main (../llvm_upstream/install_dir/bin/clang+0xceeb)
#<!-- -->34 0x00007f70fa629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->35 0x00007f70fa629e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007f70fa629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->37 0x0000562a5439df45 _start (../llvm_upstream/install_dir/bin/clang+0xcf45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.0 (https://github.com/llvm/llvm-project.git 3623fe661ae35c6c80ac221f14d85be76aa870f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /install_dir/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/foo-1c1aca.c
clang: note: diagnostic msg: /tmp/foo-1c1aca.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

