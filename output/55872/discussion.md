# Clang crashes with "undeduced type in IR-generation" on recursive template instantiation

**Author:** joeri-hu
**URL:** https://github.com/llvm/llvm-project/issues/55872
**Status:** Closed
**Labels:** c++17, c++20, clang:frontend, crash-on-invalid
**Closed Date:** 2025-05-07T00:35:13Z

## Body

Clang produces a crash report with the code example that can be found [here](https://godbolt.org/z/qrEd1cq4c).

```console
Stack dump:
0.  Program arguments: /opt/compiler-explorer/clang-14.0.0/bin/clang-14 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-14.0.0/lib/clang/14.0.0 -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0 -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0/backward -internal-isystem /opt/compiler-explorer/clang-14.0.0/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -pedantic -std=c++20 -fdeprecated-macro -fdebug-compilation-dir=/app -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-817158.o -x c++ <source>
1.  <eof> parser at end of file
2.  <source>:13:6: LLVM IR generation of declaration 'main'
3.  <source>:13:6: Generating code for declaration 'main'
 #0 0x0000560abff7de8f PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #1 0x0000560abff7b74d SignalHandler(int) Signals.cpp:0:0
 #2 0x00007f7bfe4343c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x143c0)
 #3 0x0000560ac2824f13 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e24f13)
 #4 0x0000560ac28182ed clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e182ed)
 #5 0x0000560ac28184f5 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e184f5)
 #6 0x0000560ac043ac38 (anonymous namespace)::X86_64ABIInfo::getIndirectReturnResult(clang::QualType) const TargetInfo.cpp:0:0
 #7 0x0000560ac045bbc3 (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&) const TargetInfo.cpp:0:0
 #8 0x0000560ac05648ca clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, bool, bool, llvm::ArrayRef<clang::CanQual<clang::Type> >, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3b648ca)
 #9 0x0000560ac0565c1b arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type> >&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#10 0x0000560ac0566268 clang::CodeGen::CodeGenTypes::arrangeCXXMethodDeclaration(clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3b66268)
#11 0x0000560ac05f3d30 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bf3d30)
#12 0x0000560ac05f551e clang::CodeGen::CodeGenFunction::EmitCXXOperatorMemberCallExpr(clang::CXXOperatorCallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bf551e)
#13 0x0000560ac05dec2a clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bdec2a)
#14 0x0000560ac05f88e8 (anonymous namespace)::ComplexExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprComplex.cpp:0:0
#15 0x0000560ac05fd89d (anonymous namespace)::ComplexExprEmitter::Visit(clang::Expr*) CGExprComplex.cpp:0:0
#16 0x0000560ac05fdd90 (anonymous namespace)::ComplexExprEmitter::Visit(clang::Expr*) CGExprComplex.cpp:0:0
#17 0x0000560ac05ff504 clang::CodeGen::CodeGenFunction::EmitComplexExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bff504)
#18 0x0000560ac05c52c1 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bc52c1)
#19 0x0000560ac05dd416 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bdd416)
#20 0x0000560ac031061c clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x391061c)
#21 0x0000560ac031627c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x391627c)
#22 0x0000560ac0366433 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3966433)
#23 0x0000560ac036fe7a clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x396fe7a)
#24 0x0000560ac03b6918 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b6918)
#25 0x0000560ac03b3a55 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b3a55)
#26 0x0000560ac03b4193 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b4193)
#27 0x0000560ac03ba531 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.5348) CodeGenModule.cpp:0:0
#28 0x0000560ac0f07ab1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#29 0x0000560ac0efc802 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x44fc802)
#30 0x0000560ac1d9a854 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x539a854)
#31 0x0000560ac0f05b02 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x4505b02)
#32 0x0000560ac0901161 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3f01161)
#33 0x0000560ac089c2f2 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3e9c2f2)
#34 0x0000560ac09cc853 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3fcc853)
#35 0x0000560abdb9b1d4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x119b1d4)
#36 0x0000560abdb9718d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x0000560abdaa6f07 main (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x10a6f07)
#38 0x00007f7bfdee20b3 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x240b3)
#39 0x0000560abdb96d0a _start (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x1196d0a)
clang-14: error: unable to execute command: Segmentation fault (core dumped)
clang-14: error: clang frontend command failed due to signal (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-c-20

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - DimitryAndric

```
Starting program: /home/dim/ins/llvmorg-15-init-11439-g5c9f3ec4ad5/bin/clang -std=c++20 -Wall -Wextra -pedantic -c pr55872.cpp
undeduced type in IR-generation
UNREACHABLE executed at /home/dim/src/llvm/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:230!

Program received signal SIGABRT, Aborted.
Sent by thr_kill() from pid 60933 and user 1236.
thr_kill () at thr_kill.S:4
4       thr_kill.S: No such file or directory.
(gdb) bt
#0  thr_kill () at thr_kill.S:4
#1  0x0000000806c72544 in __raise (s=s@entry=6) at /usr/src/lib/libc/gen/raise.c:52
#2  0x0000000806d276a9 in abort () at /usr/src/lib/libc/stdlib/abort.c:67
#3  0x0000000003c4d713 in llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) ()
#4  0x000000000416098b in clang::CodeGen::CodeGenFunction::getEvaluationKind(clang::QualType) ()
#5  0x000000000417e4b5 in clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) ()
#6  0x000000000417e459 in clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) ()
#7  0x000000000425220a in clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) ()
#8  0x000000000425d6e0 in clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) ()
#9  0x0000000004169242 in clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) ()
#10 0x0000000004169eda in clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) ()
#11 0x00000000040f82a8 in clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) ()
#12 0x00000000040f0bf4 in clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) ()
#13 0x00000000040fb873 in clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) ()
#14 0x0000000004807690 in (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ()
#15 0x00000000048049a5 in clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) ()
#16 0x00000000051894a4 in clang::ParseAST(clang::Sema&, bool, bool) ()
#17 0x000000000474f773 in clang::FrontendAction::Execute() ()
#18 0x00000000046d53f9 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) ()
#19 0x00000000047fe141 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) ()
#20 0x000000000214ce2d in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) ()
#21 0x000000000214a5ea in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) ()
#22 0x00000000045768a7 in void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >*, bool*) const::$_1>(long) ()
#23 0x0000000003c47e80 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) ()
#24 0x00000000045763b2 in clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >*, bool*) const ()
#25 0x000000000453dd1d in clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&) const ()
#26 0x000000000453e2cf in clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*> >&) const ()
#27 0x000000000455711c in clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*> >&) ()
#28 0x0000000002149f7e in main ()
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-codegen

---

### Comment 5 - llvmbot

@llvm/issue-subscribers-c-17

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: joeri (joeri-hu)

<details>
Clang produces a crash report with the code example that can be found [here](https://godbolt.org/z/qrEd1cq4c).

```console
Stack dump:
0.  Program arguments: /opt/compiler-explorer/clang-14.0.0/bin/clang-14 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-14.0.0/lib/clang/14.0.0 -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0 -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../include/c++/11.2.0/backward -internal-isystem /opt/compiler-explorer/clang-14.0.0/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-11.2.0/lib/gcc/x86_64-linux-gnu/11.2.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -pedantic -std=c++20 -fdeprecated-macro -fdebug-compilation-dir=/app -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-817158.o -x c++ &lt;source&gt;
1.  &lt;eof&gt; parser at end of file
2.  &lt;source&gt;:13:6: LLVM IR generation of declaration 'main'
3.  &lt;source&gt;:13:6: Generating code for declaration 'main'
 #<!-- -->0 0x0000560abff7de8f PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->1 0x0000560abff7b74d SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007f7bfe4343c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x143c0)
 #<!-- -->3 0x0000560ac2824f13 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e24f13)
 #<!-- -->4 0x0000560ac28182ed clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e182ed)
 #<!-- -->5 0x0000560ac28184f5 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x5e184f5)
 #<!-- -->6 0x0000560ac043ac38 (anonymous namespace)::X86_64ABIInfo::getIndirectReturnResult(clang::QualType) const TargetInfo.cpp:0:0
 #<!-- -->7 0x0000560ac045bbc3 (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const TargetInfo.cpp:0:0
 #<!-- -->8 0x0000560ac05648ca clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, bool, bool, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt; &gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3b648ca)
 #<!-- -->9 0x0000560ac0565c1b arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt; &gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->10 0x0000560ac0566268 clang::CodeGen::CodeGenTypes::arrangeCXXMethodDeclaration(clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3b66268)
#<!-- -->11 0x0000560ac05f3d30 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bf3d30)
#<!-- -->12 0x0000560ac05f551e clang::CodeGen::CodeGenFunction::EmitCXXOperatorMemberCallExpr(clang::CXXOperatorCallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bf551e)
#<!-- -->13 0x0000560ac05dec2a clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bdec2a)
#<!-- -->14 0x0000560ac05f88e8 (anonymous namespace)::ComplexExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprComplex.cpp:0:0
#<!-- -->15 0x0000560ac05fd89d (anonymous namespace)::ComplexExprEmitter::Visit(clang::Expr*) CGExprComplex.cpp:0:0
#<!-- -->16 0x0000560ac05fdd90 (anonymous namespace)::ComplexExprEmitter::Visit(clang::Expr*) CGExprComplex.cpp:0:0
#<!-- -->17 0x0000560ac05ff504 clang::CodeGen::CodeGenFunction::EmitComplexExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bff504)
#<!-- -->18 0x0000560ac05c52c1 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bc52c1)
#<!-- -->19 0x0000560ac05dd416 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3bdd416)
#<!-- -->20 0x0000560ac031061c clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x391061c)
#<!-- -->21 0x0000560ac031627c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x391627c)
#<!-- -->22 0x0000560ac0366433 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3966433)
#<!-- -->23 0x0000560ac036fe7a clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x396fe7a)
#<!-- -->24 0x0000560ac03b6918 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b6918)
#<!-- -->25 0x0000560ac03b3a55 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b3a55)
#<!-- -->26 0x0000560ac03b4193 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x39b4193)
#<!-- -->27 0x0000560ac03ba531 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.5348) CodeGenModule.cpp:0:0
#<!-- -->28 0x0000560ac0f07ab1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->29 0x0000560ac0efc802 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x44fc802)
#<!-- -->30 0x0000560ac1d9a854 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x539a854)
#<!-- -->31 0x0000560ac0f05b02 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x4505b02)
#<!-- -->32 0x0000560ac0901161 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3f01161)
#<!-- -->33 0x0000560ac089c2f2 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3e9c2f2)
#<!-- -->34 0x0000560ac09cc853 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x3fcc853)
#<!-- -->35 0x0000560abdb9b1d4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x119b1d4)
#<!-- -->36 0x0000560abdb9718d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->37 0x0000560abdaa6f07 main (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x10a6f07)
#<!-- -->38 0x00007f7bfdee20b3 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x240b3)
#<!-- -->39 0x0000560abdb96d0a _start (/opt/compiler-explorer/clang-14.0.0/bin/clang-14+0x1196d0a)
clang-14: error: unable to execute command: Segmentation fault (core dumped)
clang-14: error: clang frontend command failed due to signal (use -v to see invocation)
```
</details>


---

