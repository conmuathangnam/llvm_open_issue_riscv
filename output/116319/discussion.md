# clang-20 crashed with -g. Assertion `!DT.isNull() && "Undeduced types shouldn't reach here."' failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/116319
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-09-29T19:30:09Z

## Body

clang-20 crashed with ```-g``` option.

Compiler explorer: https://godbolt.org/z/73x3Wzj57
Since compiler explorer enables ```-g``` by default, if clang compiles this code with ```-g0```, it does not crash.

```
$cat mutant.C
template <int = 0> struct a {
  template <class> static constexpr auto b = 2;
  template <class> static void c() noexcept(noexcept(b<int>)) {}
};
int main() { a<>::c<int>(); }
```
Also crashed at Clang-19.

```
$clang-19 -g mutant.C
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.C -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-3b9cb5.o -x c++ mutant.C
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      mutant.C:3:32: Generating code for declaration 'a<>::c'
 #0 0x00007f845ded7246 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #1 0x00007f845ded4e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #2 0x00007f845ded790b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #3 0x00007f845cb0f520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f8466b53cb5 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91cb5)
 #5 0x00007f8466b5ba82 clang::CodeGen::CGDebugInfo::CreateRecordStaticField(clang::VarDecl const*, llvm::DIType*, clang::RecordDecl const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f99a82)
 #6 0x00007f8466b5c1da clang::CodeGen::CGDebugInfo::CollectRecordFields(clang::RecordDecl const*, llvm::DIFile*, llvm::SmallVectorImpl<llvm::Metadata*>&, llvm::DICompositeType*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9a1da)
 #7 0x00007f8466b5f1c3 clang::CodeGen::CGDebugInfo::CreateTypeDefinition(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d1c3)
 #8 0x00007f8466b5f6f6 clang::CodeGen::CGDebugInfo::CreateType(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d6f6)
 #9 0x00007f8466b53e2b clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91e2b)
#10 0x00007f8466b639c2 clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&, llvm::StringRef&, llvm::DIScope*&, llvm::MDTupleTypedArrayWrapper<llvm::DINode>&, llvm::DINode::DIFlags&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa19c2)
#11 0x00007f8466b65b46 clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa3b46)
#12 0x00007f8466d95acd clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::FunctionArgList const&, clang::SourceLocation, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d3acd)
#13 0x00007f8466d97fc9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d5fc9)
#14 0x00007f8466db99af clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f79af)
#15 0x00007f8466db3379 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f1379)
#16 0x00007f8466da75e2 clang::CodeGen::CodeGenModule::EmitDeferred() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e55e2)
#17 0x00007f8466da4c4b clang::CodeGen::CodeGenModule::Release() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e2c4b)
#18 0x00007f8466e527fb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x22907fb)
#19 0x00007f8466d87a68 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21c5a68)
#20 0x00007f84657f1739 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f739)
#21 0x00007f84678840b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#22 0x00007f84677f42f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#23 0x00007f8467900dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#24 0x0000557ae222dbc5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#25 0x0000557ae222aad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#26 0x0000557ae2229c9b clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#27 0x0000557ae2237f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#28 0x00007f845caf6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#29 0x00007f845caf6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#30 0x0000557ae2228765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/mutant-c6c626.cpp
clang: note: diagnostic msg: /tmp/mutant-c6c626.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Anonymous (iamanonymouscs)

<details>
clang-20 crashed with ```-g``` option.

Compiler explorer: https://godbolt.org/z/73x3Wzj57
Since compiler explorer enables ```-g``` by default, if clang compiles this code with ```-g0```, it does not crash.

```
$cat mutant.C
template &lt;int = 0&gt; struct a {
  template &lt;class&gt; static constexpr auto b = 2;
  template &lt;class&gt; static void c() noexcept(noexcept(b&lt;int&gt;)) {}
};
int main() { a&lt;&gt;::c&lt;int&gt;(); }
```
Also crashed at Clang-19.

```
$clang-19 -g mutant.C
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.C -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-3b9cb5.o -x c++ mutant.C
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      mutant.C:3:32: Generating code for declaration 'a&lt;&gt;::c'
 #<!-- -->0 0x00007f845ded7246 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #<!-- -->1 0x00007f845ded4e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #<!-- -->2 0x00007f845ded790b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #<!-- -->3 0x00007f845cb0f520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f8466b53cb5 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91cb5)
 #<!-- -->5 0x00007f8466b5ba82 clang::CodeGen::CGDebugInfo::CreateRecordStaticField(clang::VarDecl const*, llvm::DIType*, clang::RecordDecl const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f99a82)
 #<!-- -->6 0x00007f8466b5c1da clang::CodeGen::CGDebugInfo::CollectRecordFields(clang::RecordDecl const*, llvm::DIFile*, llvm::SmallVectorImpl&lt;llvm::Metadata*&gt;&amp;, llvm::DICompositeType*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9a1da)
 #<!-- -->7 0x00007f8466b5f1c3 clang::CodeGen::CGDebugInfo::CreateTypeDefinition(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d1c3)
 #<!-- -->8 0x00007f8466b5f6f6 clang::CodeGen::CGDebugInfo::CreateType(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d6f6)
 #<!-- -->9 0x00007f8466b53e2b clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91e2b)
#<!-- -->10 0x00007f8466b639c2 clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&amp;, llvm::StringRef&amp;, llvm::DIScope*&amp;, llvm::MDTupleTypedArrayWrapper&lt;llvm::DINode&gt;&amp;, llvm::DINode::DIFlags&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa19c2)
#<!-- -->11 0x00007f8466b65b46 clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa3b46)
#<!-- -->12 0x00007f8466d95acd clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::FunctionArgList const&amp;, clang::SourceLocation, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d3acd)
#<!-- -->13 0x00007f8466d97fc9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d5fc9)
#<!-- -->14 0x00007f8466db99af clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f79af)
#<!-- -->15 0x00007f8466db3379 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f1379)
#<!-- -->16 0x00007f8466da75e2 clang::CodeGen::CodeGenModule::EmitDeferred() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e55e2)
#<!-- -->17 0x00007f8466da4c4b clang::CodeGen::CodeGenModule::Release() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e2c4b)
#<!-- -->18 0x00007f8466e527fb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x22907fb)
#<!-- -->19 0x00007f8466d87a68 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21c5a68)
#<!-- -->20 0x00007f84657f1739 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f739)
#<!-- -->21 0x00007f84678840b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#<!-- -->22 0x00007f84677f42f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#<!-- -->23 0x00007f8467900dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#<!-- -->24 0x0000557ae222dbc5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#<!-- -->25 0x0000557ae222aad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#<!-- -->26 0x0000557ae2229c9b clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#<!-- -->27 0x0000557ae2237f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#<!-- -->28 0x00007f845caf6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->29 0x00007f845caf6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->30 0x0000557ae2228765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/mutant-c6c626.cpp
clang: note: diagnostic msg: /tmp/mutant-c6c626.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - MagentaTreehouse

Looks like a duplicate of #47375, which is considered a duplicate of #56652.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang-20 crashed with ```-g``` option.

Compiler explorer: https://godbolt.org/z/73x3Wzj57
Since compiler explorer enables ```-g``` by default, if clang compiles this code with ```-g0```, it does not crash.

```
$cat mutant.C
template &lt;int = 0&gt; struct a {
  template &lt;class&gt; static constexpr auto b = 2;
  template &lt;class&gt; static void c() noexcept(noexcept(b&lt;int&gt;)) {}
};
int main() { a&lt;&gt;::c&lt;int&gt;(); }
```
Also crashed at Clang-19.

```
$clang-19 -g mutant.C
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.C -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-3b9cb5.o -x c++ mutant.C
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      mutant.C:3:32: Generating code for declaration 'a&lt;&gt;::c'
 #<!-- -->0 0x00007f845ded7246 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #<!-- -->1 0x00007f845ded4e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #<!-- -->2 0x00007f845ded790b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #<!-- -->3 0x00007f845cb0f520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f8466b53cb5 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91cb5)
 #<!-- -->5 0x00007f8466b5ba82 clang::CodeGen::CGDebugInfo::CreateRecordStaticField(clang::VarDecl const*, llvm::DIType*, clang::RecordDecl const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f99a82)
 #<!-- -->6 0x00007f8466b5c1da clang::CodeGen::CGDebugInfo::CollectRecordFields(clang::RecordDecl const*, llvm::DIFile*, llvm::SmallVectorImpl&lt;llvm::Metadata*&gt;&amp;, llvm::DICompositeType*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9a1da)
 #<!-- -->7 0x00007f8466b5f1c3 clang::CodeGen::CGDebugInfo::CreateTypeDefinition(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d1c3)
 #<!-- -->8 0x00007f8466b5f6f6 clang::CodeGen::CGDebugInfo::CreateType(clang::RecordType const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f9d6f6)
 #<!-- -->9 0x00007f8466b53e2b clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1f91e2b)
#<!-- -->10 0x00007f8466b639c2 clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&amp;, llvm::StringRef&amp;, llvm::DIScope*&amp;, llvm::MDTupleTypedArrayWrapper&lt;llvm::DINode&gt;&amp;, llvm::DINode::DIFlags&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa19c2)
#<!-- -->11 0x00007f8466b65b46 clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1fa3b46)
#<!-- -->12 0x00007f8466d95acd clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::FunctionArgList const&amp;, clang::SourceLocation, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d3acd)
#<!-- -->13 0x00007f8466d97fc9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21d5fc9)
#<!-- -->14 0x00007f8466db99af clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f79af)
#<!-- -->15 0x00007f8466db3379 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21f1379)
#<!-- -->16 0x00007f8466da75e2 clang::CodeGen::CodeGenModule::EmitDeferred() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e55e2)
#<!-- -->17 0x00007f8466da4c4b clang::CodeGen::CodeGenModule::Release() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21e2c4b)
#<!-- -->18 0x00007f8466e527fb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x22907fb)
#<!-- -->19 0x00007f8466d87a68 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21c5a68)
#<!-- -->20 0x00007f84657f1739 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f739)
#<!-- -->21 0x00007f84678840b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#<!-- -->22 0x00007f84677f42f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#<!-- -->23 0x00007f8467900dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#<!-- -->24 0x0000557ae222dbc5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#<!-- -->25 0x0000557ae222aad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#<!-- -->26 0x0000557ae2229c9b clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#<!-- -->27 0x0000557ae2237f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#<!-- -->28 0x00007f845caf6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->29 0x00007f845caf6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->30 0x0000557ae2228765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/mutant-c6c626.cpp
clang: note: diagnostic msg: /tmp/mutant-c6c626.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

