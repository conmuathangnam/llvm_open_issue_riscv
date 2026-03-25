# [clang] Crash with dependent default template argument and decltype of static variable template

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/161196
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-09-29T19:30:09Z

## Body

The following code causes clang to crash,This issue has existed since ```x86-64 Clang 9.0.0``` and is still reproducible on trunk.:

```cpp


template <typename> struct A { static constexpr int digits = 0; };

template <typename> struct B {
  template <int, typename MaskInt = int, int = A<MaskInt>::digits>
  static constexpr auto XBitMask = 0;
};

struct C {
  using ReferenceHost = B<int>;
  template <int> static decltype(ReferenceHost::XBitMask<0>) XBitMask;
};

int main() { C::XBitMask<0>; }

```

For more information please see:https://godbolt.org/z/8Kf94Gzod

Stack Dump:

```
<source>:14:14: warning: expression result unused [-Wunused-value]
int main() { C::XBitMask<0>; }
             ^~~~~~~~~~~~~~
Stack dump:
0.	Program arguments: /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9 -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -coverage-notes-file /app/output.gcno -resource-dir /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/backward -internal-isystem /usr/local/include -internal-isystem /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /app -ferror-limit 19 -fmessage-length 0 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -mllvm --x86-asm-syntax=intel -faddrsig -o /app/output.s -x c++ <source> 
1.	<eof> parser at end of file
2.	<source>:14:5: LLVM IR generation of declaration 'main'
3.	<source>:14:5: Generating code for declaration 'main'
 #0 0x00005d2a3c5db76a llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27db76a)
 #1 0x00005d2a3c5d9524 llvm::sys::RunSignalHandlers() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9524)
 #2 0x00005d2a3c5d9662 SignalHandler(int) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9662)
 #3 0x00007b047a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00005d2a3c8fbc6a clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afbc6a)
 #5 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #6 0x00005d2a3c8fc3d8 clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afc3d8)
 #7 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #8 0x00005d2a3c8b5447 clang::CodeGen::CodeGenModule::GetAddrOfGlobalVar(clang::VarDecl const*, llvm::Type*, clang::CodeGen::ForDefinition_t) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ab5447)
 #9 0x00005d2a3ca323fc clang::CodeGen::CodeGenFunction::EmitDeclRefLValue(clang::DeclRefExpr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c323fc)
#10 0x00005d2a3ca315be clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c315be)
#11 0x00005d2a3ca31b0b clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c31b0b)
#12 0x00005d2a3c84c913 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4c913)
#13 0x00005d2a3c84ceec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4ceec)
#14 0x00005d2a3c888181 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a88181)
#15 0x00005d2a3c8900ee clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a900ee)
#16 0x00005d2a3c8cd089 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acd089)
#17 0x00005d2a3c8caef5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acaef5)
#18 0x00005d2a3c8cb5c3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acb5c3)
#19 0x00005d2a3c8d05a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.4556) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ad05a7)
#20 0x00005d2a3d3343a1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35343a1)
#21 0x00005d2a3d3286a6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35286a6)
#22 0x00005d2a3db05c54 clang::ParseAST(clang::Sema&, bool, bool) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3d05c54)
#23 0x00005d2a3d330a47 clang::CodeGenAction::ExecuteAction() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3530a47)
#24 0x00005d2a3ccac529 clang::FrontendAction::Execute() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2eac529)
#25 0x00005d2a3cc6fbf5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2e6fbf5)
#26 0x00005d2a3cd6ff23 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2f6ff23)
#27 0x00005d2a3ab0e140 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0e140)
#28 0x00005d2a3aa6c882 main (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xc6c882)
#29 0x00007b047a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x00007b047a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x00005d2a3ab0b7fa _start (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0b7fa)
clang-9: error: unable to execute command: Segmentation fault (core dumped)
clang-9: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (Attacker23)

<details>
The following code causes clang to crash,This issue has existed since ```x86-64 Clang 9.0.0``` and is still reproducible on trunk.:

```cpp


template &lt;typename&gt; struct A { static constexpr int digits = 0; };

template &lt;typename&gt; struct B {
  template &lt;int, typename MaskInt = int, int = A&lt;MaskInt&gt;::digits&gt;
  static constexpr auto XBitMask = 0;
};

struct C {
  using ReferenceHost = B&lt;int&gt;;
  template &lt;int&gt; static decltype(ReferenceHost::XBitMask&lt;0&gt;) XBitMask;
};

int main() { C::XBitMask&lt;0&gt;; }

```

For more information please see:https://godbolt.org/z/8Kf94Gzod

Stack Dump:

```
&lt;source&gt;:14:14: warning: expression result unused [-Wunused-value]
int main() { C::XBitMask&lt;0&gt;; }
             ^~~~~~~~~~~~~~
Stack dump:
0.	Program arguments: /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9 -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -coverage-notes-file /app/output.gcno -resource-dir /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/backward -internal-isystem /usr/local/include -internal-isystem /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /app -ferror-limit 19 -fmessage-length 0 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -mllvm --x86-asm-syntax=intel -faddrsig -o /app/output.s -x c++ &lt;source&gt; 
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:14:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:14:5: Generating code for declaration 'main'
 #<!-- -->0 0x00005d2a3c5db76a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27db76a)
 #<!-- -->1 0x00005d2a3c5d9524 llvm::sys::RunSignalHandlers() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9524)
 #<!-- -->2 0x00005d2a3c5d9662 SignalHandler(int) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9662)
 #<!-- -->3 0x00007b047a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00005d2a3c8fbc6a clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afbc6a)
 #<!-- -->5 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #<!-- -->6 0x00005d2a3c8fc3d8 clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afc3d8)
 #<!-- -->7 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #<!-- -->8 0x00005d2a3c8b5447 clang::CodeGen::CodeGenModule::GetAddrOfGlobalVar(clang::VarDecl const*, llvm::Type*, clang::CodeGen::ForDefinition_t) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ab5447)
 #<!-- -->9 0x00005d2a3ca323fc clang::CodeGen::CodeGenFunction::EmitDeclRefLValue(clang::DeclRefExpr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c323fc)
#<!-- -->10 0x00005d2a3ca315be clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c315be)
#<!-- -->11 0x00005d2a3ca31b0b clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c31b0b)
#<!-- -->12 0x00005d2a3c84c913 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4c913)
#<!-- -->13 0x00005d2a3c84ceec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4ceec)
#<!-- -->14 0x00005d2a3c888181 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a88181)
#<!-- -->15 0x00005d2a3c8900ee clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a900ee)
#<!-- -->16 0x00005d2a3c8cd089 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acd089)
#<!-- -->17 0x00005d2a3c8caef5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acaef5)
#<!-- -->18 0x00005d2a3c8cb5c3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acb5c3)
#<!-- -->19 0x00005d2a3c8d05a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.4556) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ad05a7)
#<!-- -->20 0x00005d2a3d3343a1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35343a1)
#<!-- -->21 0x00005d2a3d3286a6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35286a6)
#<!-- -->22 0x00005d2a3db05c54 clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3d05c54)
#<!-- -->23 0x00005d2a3d330a47 clang::CodeGenAction::ExecuteAction() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3530a47)
#<!-- -->24 0x00005d2a3ccac529 clang::FrontendAction::Execute() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2eac529)
#<!-- -->25 0x00005d2a3cc6fbf5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2e6fbf5)
#<!-- -->26 0x00005d2a3cd6ff23 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2f6ff23)
#<!-- -->27 0x00005d2a3ab0e140 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0e140)
#<!-- -->28 0x00005d2a3aa6c882 main (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xc6c882)
#<!-- -->29 0x00007b047a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x00007b047a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x00005d2a3ab0b7fa _start (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0b7fa)
clang-9: error: unable to execute command: Segmentation fault (core dumped)
clang-9: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code causes clang to crash,This issue has existed since ```x86-64 Clang 9.0.0``` and is still reproducible on trunk.:

```cpp


template &lt;typename&gt; struct A { static constexpr int digits = 0; };

template &lt;typename&gt; struct B {
  template &lt;int, typename MaskInt = int, int = A&lt;MaskInt&gt;::digits&gt;
  static constexpr auto XBitMask = 0;
};

struct C {
  using ReferenceHost = B&lt;int&gt;;
  template &lt;int&gt; static decltype(ReferenceHost::XBitMask&lt;0&gt;) XBitMask;
};

int main() { C::XBitMask&lt;0&gt;; }

```

For more information please see:https://godbolt.org/z/8Kf94Gzod

Stack Dump:

```
&lt;source&gt;:14:14: warning: expression result unused [-Wunused-value]
int main() { C::XBitMask&lt;0&gt;; }
             ^~~~~~~~~~~~~~
Stack dump:
0.	Program arguments: /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9 -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -coverage-notes-file /app/output.gcno -resource-dir /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0 -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-9.3.0/lib/gcc/x86_64-linux-gnu/9.3.0/../../../../include/c++/9.3.0/backward -internal-isystem /usr/local/include -internal-isystem /cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/lib/clang/9.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /app -ferror-limit 19 -fmessage-length 0 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -mllvm --x86-asm-syntax=intel -faddrsig -o /app/output.s -x c++ &lt;source&gt; 
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:14:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:14:5: Generating code for declaration 'main'
 #<!-- -->0 0x00005d2a3c5db76a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27db76a)
 #<!-- -->1 0x00005d2a3c5d9524 llvm::sys::RunSignalHandlers() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9524)
 #<!-- -->2 0x00005d2a3c5d9662 SignalHandler(int) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x27d9662)
 #<!-- -->3 0x00007b047a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00005d2a3c8fbc6a clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afbc6a)
 #<!-- -->5 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #<!-- -->6 0x00005d2a3c8fc3d8 clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afc3d8)
 #<!-- -->7 0x00005d2a3c8fccbf clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2afccbf)
 #<!-- -->8 0x00005d2a3c8b5447 clang::CodeGen::CodeGenModule::GetAddrOfGlobalVar(clang::VarDecl const*, llvm::Type*, clang::CodeGen::ForDefinition_t) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ab5447)
 #<!-- -->9 0x00005d2a3ca323fc clang::CodeGen::CodeGenFunction::EmitDeclRefLValue(clang::DeclRefExpr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c323fc)
#<!-- -->10 0x00005d2a3ca315be clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c315be)
#<!-- -->11 0x00005d2a3ca31b0b clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2c31b0b)
#<!-- -->12 0x00005d2a3c84c913 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4c913)
#<!-- -->13 0x00005d2a3c84ceec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a4ceec)
#<!-- -->14 0x00005d2a3c888181 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a88181)
#<!-- -->15 0x00005d2a3c8900ee clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2a900ee)
#<!-- -->16 0x00005d2a3c8cd089 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acd089)
#<!-- -->17 0x00005d2a3c8caef5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acaef5)
#<!-- -->18 0x00005d2a3c8cb5c3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2acb5c3)
#<!-- -->19 0x00005d2a3c8d05a7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.4556) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2ad05a7)
#<!-- -->20 0x00005d2a3d3343a1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35343a1)
#<!-- -->21 0x00005d2a3d3286a6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x35286a6)
#<!-- -->22 0x00005d2a3db05c54 clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3d05c54)
#<!-- -->23 0x00005d2a3d330a47 clang::CodeGenAction::ExecuteAction() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x3530a47)
#<!-- -->24 0x00005d2a3ccac529 clang::FrontendAction::Execute() (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2eac529)
#<!-- -->25 0x00005d2a3cc6fbf5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2e6fbf5)
#<!-- -->26 0x00005d2a3cd6ff23 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0x2f6ff23)
#<!-- -->27 0x00005d2a3ab0e140 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0e140)
#<!-- -->28 0x00005d2a3aa6c882 main (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xc6c882)
#<!-- -->29 0x00007b047a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x00007b047a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x00005d2a3ab0b7fa _start (/cefs/f0/f03e5f34867580d0ae719b6e_consolidated/compilers_c++_clang_9.0.0/bin/clang-9+0xd0b7fa)
clang-9: error: unable to execute command: Segmentation fault (core dumped)
clang-9: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```
</details>


---

