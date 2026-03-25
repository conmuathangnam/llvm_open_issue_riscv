# partial specialization selection treated as immediate context, resulting in invalid decls with no diagnostic

**Author:** ec04fc15-fa35-46f2-80e1-5d271f2ef708
**URL:** https://github.com/llvm/llvm-project/issues/50067

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [50723](https://llvm.org/bz50723) |
| Version | unspecified |
| OS | All |
| CC | @CaseyCarter,@DougGregor,@zygoloid |

## Extended Description 
Clang incorrectly accepts this code, despite `S<int*, int*>` having no member named `foo`:
```cpp
template<typename T, typename U> struct S {};
template<typename T> struct S<T, T> {};
template<typename T, typename U> struct S<T*, U*> {};
template<typename ... Ts> using V = void;
template<typename T, typename U = void> struct X {}; // #&#8203;1
template<typename T> struct X<T, V<typename S<T, T>::type>>; // #&#8203;2
X<int*> xpi;
S<int*, int*>::foo x = "hello";
```
What's happening here is that we try to instantiate `S<int*, int*>` as part of considering `#2`, but fail because the partial specialization selection for `S` results in an ambiguity. We mark `S<int*, int*>` as invalid and produce a diagnostic. However, because this all happens in the immediate context of the partial specialization selection for `#2`, we swallow the diagnostic and instead select `#1`.

Then when we try to form `S<int*, int*>::foo`, because `S<int*, int*>` is marked as invalid we form an invalid type and don't produce any diagnostic (incorrectly thinking we already produced one).

Probably the right approach here is to treat ambiguity during partial specialization selection as not being part of the immediate context. That's what GCC and EDG do. (MSVC, like Clang, treats this ambiguity as being in the immediate context, and has very similar kinds of brokenness for examples like this one.)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ec04fc15-fa35-46f2-80e1-5d271f2ef708)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [50723](https://llvm.org/bz50723) |
| Version | unspecified |
| OS | All |
| CC | @<!-- -->CaseyCarter,@<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
Clang incorrectly accepts this code, despite `S&lt;int*, int*&gt;` having no member named `foo`:
```cpp
template&lt;typename T, typename U&gt; struct S {};
template&lt;typename T&gt; struct S&lt;T, T&gt; {};
template&lt;typename T, typename U&gt; struct S&lt;T*, U*&gt; {};
template&lt;typename ... Ts&gt; using V = void;
template&lt;typename T, typename U = void&gt; struct X {}; // #&amp;#<!-- -->8203;1
template&lt;typename T&gt; struct X&lt;T, V&lt;typename S&lt;T, T&gt;::type&gt;&gt;; // #&amp;#<!-- -->8203;2
X&lt;int*&gt; xpi;
S&lt;int*, int*&gt;::foo x = "hello";
```
What's happening here is that we try to instantiate `S&lt;int*, int*&gt;` as part of considering `#<!-- -->2`, but fail because the partial specialization selection for `S` results in an ambiguity. We mark `S&lt;int*, int*&gt;` as invalid and produce a diagnostic. However, because this all happens in the immediate context of the partial specialization selection for `#<!-- -->2`, we swallow the diagnostic and instead select `#<!-- -->1`.

Then when we try to form `S&lt;int*, int*&gt;::foo`, because `S&lt;int*, int*&gt;` is marked as invalid we form an invalid type and don't produce any diagnostic (incorrectly thinking we already produced one).

Probably the right approach here is to treat ambiguity during partial specialization selection as not being part of the immediate context. That's what GCC and EDG do. (MSVC, like Clang, treats this ambiguity as being in the immediate context, and has very similar kinds of brokenness for examples like this one.)
</details>


---

### Comment 2 - shafik

Sadly this now crashes: https://godbolt.org/z/q9TTzdKMK

Looks like a clang-18 regression.

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/Decl.cpp:2549:
clang::APValue* clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic> >&, bool) const:
Assertion `!Init->isValueDependent()' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:8:20: LLVM IR generation of declaration 'x'
3.	<source>:8:20: Generating code for declaration 'x'
 #0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
 #1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
 #2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000794346c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000794346c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000794346c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000794346c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000794346c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000794346c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077c93c3 (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x77c93c3)
#10 0x00000000077c944b clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x77c944b)
#11 0x0000000003e2021b clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e2021b)
#12 0x0000000003e205c2 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e205c2)
#13 0x0000000003f05f1c clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f05f1c)
#14 0x0000000003f28cc9 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f28cc9)
#15 0x0000000003f292fb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f292fb)
#16 0x0000000003f335fb clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#17 0x00000000043fb906 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#18 0x00000000043ebe68 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43ebe68)
#19 0x00000000064cc324 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc324)
#20 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#21 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#22 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#23 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#24 0x0000000000c82b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#25 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x000000000443cba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#28 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#30 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#31 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#32 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#33 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#34 0x0000794346c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x0000794346c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

CC @zyn0217 

---

### Comment 4 - shafik

@HighCommander4 git bisect points to this commit as the cause for the regression: d5308949cf884d8e4b971d51a8b4f73584c4adec

---

### Comment 5 - HighCommander4

Thanks, I'll take a took.

It probably makes sense to track the crash regression separately from the original accepts-invalid bug, as a fix for the crash probably won't fix the original bug.

---

### Comment 6 - HighCommander4

> It probably makes sense to track the crash regression separately from the original accepts-invalid bug, as a fix for the crash probably won't fix the original bug.

Actually, having looked into it a bit, it occurs to me that the crash might be an **improvement** over silently accepting the invalid code:

 * In the original testcase, we have an invalid variable decl for which we do not emit a diagnostic.
   * Prior to https://github.com/llvm/llvm-project/pull/88645, we did not retain the initializer of an invalid variable decl in the AST at all.
   * Therefore, we were not emitting the initializer during codegen.
   * Therefore, the original bug is not just "accepts invalid" in the sense that we failed to issue a diagnostic, it's also a miscompilation.
 * In https://github.com/llvm/llvm-project/pull/88645, we started to retain the initializers of invalid decls, wrapped in a `RecoveryExpr`, for tooling use.
   * Normally, if the decl is invalid, there should have been an error diagnostic and we don't try to codegen the decl (we take [this early exit](https://searchfox.org/llvm/rev/0735537ec8954d185cda3c6fd126431f16cea133/clang/lib/CodeGen/ModuleBuilder.cpp#183-184)), so it doesn't matter whether it has an initializer.
   * But due to the original bug here, there was no diagnostic, so now we try to emit the `RecoveryExpr` initializer, leading to an assertion failure.

Now, we could do something like [this](https://github.com/llvm/llvm-project/pull/131278) to teach codegen to not try to emit an initializer that contains a `RecoveryExpr`, thereby restoring the previous behaviour of silently accepting the code... but given that the result of the compilation is not correct, I would argue a crash is actually preferable to silently generating invalid code?

---

