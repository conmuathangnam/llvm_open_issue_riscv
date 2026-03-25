# Assertion failure/bad codegen: too many args w/inherited constructor from virtual base in ctor-initializer

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/33749
**Status:** Open
**Labels:** c++17, clang:codegen, bugzilla, miscompilation, confirmed

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [34401](https://llvm.org/bz34401) |
| Version | trunk |
| OS | All |
| CC | @AaronBallman |

## Extended Description 
When a mem-initializer selects an inherited constructor from a virtual base and the object it constructs is not a complete object, Clang tries to pass more arguments than it should.

### SOURCE (`<stdin>`):
```cpp
struct V { V() = default; V(int); };
struct A : virtual V {
  using V::V;
  A() : A(19) { }
};
struct B : A { } b;
```

### COMPILER INVOCATION:
```
clang++ -cc1 -xc++ -std=c++1z -emit-llvm -
```

### EXPECTED OUTPUT:
(Clean compile)


### ACTUAL OUTPUT:
```
clang++: /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCall.cpp:4112: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitCall(const clang::CodeGen::CGFunctionInfo&, const clang::CodeGen::CGCallee&, clang::CodeGen::ReturnValueSlot, const clang::CodeGen::CallArgList&, llvm::Instruction**): Assertion `IRCallArgs.size() == IRFuncTy->getNumParams() || IRFuncTy->isVarArg()' failed.
#0 0x000000001481a998 llvm::sys::PrintStackTrace(llvm::raw_ostream&) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:398:0
#1 0x000000001481aa9c PrintStackTraceSignalHandler(void*) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:461:0
#2 0x000000001481877c llvm::sys::RunSignalHandlers() /llvm_trunk/llvm/lib/Support/Signals.cpp:50:0
#3 0x0000000014819f40 SignalHandler(int) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:242:0
#4 0x00003fff840e0478  0x478 __GI_abort
#5 0x00003fff840e0478
#6 0x00003fff840e0478 __assert_fail_base (+0x478)
#7 0x00003fff83b30d70 __GI___assert_fail (/lib64/libc.so.6+0x40d70)
#8 0x00003fff83b248a4 clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::CGCallee const&, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&, llvm::Instruction**) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCall.cpp:4112:0
#9 0x00003fff83b24994 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::Address, clang::CodeGen::CallArgList&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:2061:0
#10 0x0000000015057448 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::Address, clang::CXXConstructExpr const*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:1979:0
#11 0x0000000015080cd4 clang::CodeGen::CodeGenFunction::EmitCXXConstructExpr(clang::CXXConstructExpr const*, clang::CodeGen::AggValueSlot) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprCXX.cpp:622:0
#12 0x0000000015080518 (anonymous namespace)::AggExprEmitter::VisitCXXConstructExpr(clang::CXXConstructExpr const*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:1001:0
#13 0x000000001511be9c clang::StmtVisitorBase<clang::make_ptr, (anonymous namespace)::AggExprEmitter, void>::Visit(clang::Stmt*) /tools/clang/include/clang/AST/StmtNodes.inc:213:0
#14 0x000000001510c0a8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:104:0
#15 0x00000000151103d0 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:1532:0
#16 0x000000001510791c clang::CodeGen::CodeGenFunction::EmitDelegatingCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CodeGen::FunctionArgList const&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:2289:0
#17 0x000000001510efb8 clang::CodeGen::CodeGenFunction::EmitCtorPrologue(clang::CXXConstructorDecl const*, clang::CXXCtorType, clang::CodeGen::FunctionArgList&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:1243:0
#18 0x000000001508242c clang::CodeGen::CodeGenFunction::EmitConstructorBody(clang::CodeGen::FunctionArgList&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:839:0
#19 0x000000001507d230 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:1218:0
#20 0x000000001507aef0 clang::CodeGen::CodeGenModule::codegenCXXStructor(clang::CXXMethodDecl const*, clang::CodeGen::StructorType) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCXX.cpp:234:0
#21 0x0000000014d88f18 (anonymous namespace)::ItaniumCXXABI::emitCXXStructor(clang::CXXMethodDecl const*, clang::CodeGen::StructorType) /llvm_trunk/llvm/tools/clang/lib/CodeGen/ItaniumCXXABI.cpp:3668:0
#22 0x000000001503bcf8 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:2011:0
#23 0x0000000014f92b50 clang::CodeGen::CodeGenModule::EmitDeferred() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:1440:0
#24 0x0000000014daf498 clang::CodeGen::CodeGenModule::EmitDeferred() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:1447:0
#25 0x0000000014dabff8 clang::CodeGen::CodeGenModule::Release() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:385:0
#26 0x0000000014dac068 (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/ModuleBuilder.cpp:265:0
#27 0x0000000014da5ea0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenAction.cpp:204:0
#28 0x0000000015b478a0 clang::ParseAST(clang::Sema&, bool, bool) /llvm_trunk/llvm/tools/clang/lib/Parse/ParseAST.cpp:159:0
#29 0x0000000015b3bf14 clang::ASTFrontendAction::ExecuteAction() /llvm_trunk/llvm/tools/clang/lib/Frontend/FrontendAction.cpp:1004:0
#30 0x0000000016d027d8 clang::CodeGenAction::ExecuteAction() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenAction.cpp:992:0
#31 0x0000000015482c6c clang::FrontendAction::Execute() /llvm_trunk/llvm/tools/clang/lib/Frontend/FrontendAction.cpp:902:0
#32 0x0000000015b3936c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /llvm_trunk/llvm/tools/clang/lib/Frontend/CompilerInstance.cpp:986:0
#33 0x00000000154824d4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm_trunk/llvm/tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:252:0
#34 0x00000000153fb814 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /llvm_trunk/llvm/tools/clang/tools/driver/cc1_main.cpp:221:0
#35 0x0000000015657598 ExecuteCC1Tool(llvm::ArrayRef<char const*>, llvm::StringRef) /llvm_trunk/llvm/tools/clang/tools/driver/driver.cpp:302:0
#36 0x0000000011bd2e04 main /llvm_trunk/llvm/tools/clang/tools/driver/driver.cpp:381:0
#37 0x0000000011bc137c generic_start_main.isra.0 (/bin/clang+++0x11bc137c)
#38 0x0000000011bc1fb8 __libc_start_main (/bin/clang+++0x11bc1fb8)
/bin/clang++(_ZN4llvm3sys15PrintStackTraceERNS_11raw_ostreamE+0x58)[0x1481a998]
/bin/clang++[0x1481aa9c]
/bin/clang++(_ZN4llvm3sys17RunSignalHandlersEv+0xb4)[0x1481877c]
/bin/clang++[0x14819f40]
[0x3fff840e0478]
/lib64/libc.so.6(abort+0x280)[0x3fff83b30d70]
/lib64/libc.so.6(+0x348a4)[0x3fff83b248a4]
/lib64/libc.so.6(__assert_fail+0x64)[0x3fff83b24994]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction8EmitCallERKNS0_14CGFunctionInfoERKNS0_8CGCalleeENS0_15ReturnValueSlotERKNS0_11CallArgListEPPN4llvm11InstructionE+0x2738)[0x15057448]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction22EmitCXXConstructorCallEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeEbbNS0_7AddressERNS0_11CallArgListE+0x5ac)[0x15080cd4]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction22EmitCXXConstructorCallEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeEbbNS0_7AddressEPKNS_16CXXConstructExprE+0x2b4)[0x15080518]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction20EmitCXXConstructExprEPKNS_16CXXConstructExprENS0_12AggValueSlotE+0x454)[0x1511be9c]
/bin/clang++[0x1510c0a8]
/bin/clang++[0x151103d0]
/bin/clang++[0x1510791c]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction11EmitAggExprEPKNS_4ExprENS0_12AggValueSlotE+0x164)[0x1510efb8]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction32EmitDelegatingCXXConstructorCallEPKNS_18CXXConstructorDeclERKNS0_15FunctionArgListE+0xf4)[0x1508242c]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction16EmitCtorPrologueEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeERNS0_15FunctionArgListE+0x58)[0x1507d230]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction19EmitConstructorBodyERNS0_15FunctionArgListE+0x294)[0x1507aef0]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction12GenerateCodeENS_10GlobalDeclEPN4llvm8FunctionERKNS0_14CGFunctionInfoE+0x2d4)[0x14d88f18]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule18codegenCXXStructorEPKNS_13CXXMethodDeclENS0_12StructorTypeE+0x17c)[0x1503bcf8]
/bin/clang++[0x14f92b50]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule20EmitGlobalDefinitionENS_10GlobalDeclEPN4llvm11GlobalValueE+0x168)[0x14daf498]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule12EmitDeferredEv+0x1fc)[0x14dabff8]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule12EmitDeferredEv+0x26c)[0x14dac068]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule7ReleaseEv+0x30)[0x14da5ea0]
/bin/clang++[0x15b478a0]
/bin/clang++[0x15b3bf14]
/bin/clang++(_ZN5clang8ParseASTERNS_4SemaEbb+0x364)[0x16d027d8]
/bin/clang++(_ZN5clang17ASTFrontendAction13ExecuteActionEv+0x1d4)[0x15482c6c]
/bin/clang++(_ZN5clang13CodeGenAction13ExecuteActionEv+0x4a4)[0x15b3936c]
/bin/clang++(_ZN5clang14FrontendAction7ExecuteEv+0xbc)[0x154824d4]
/bin/clang++(_ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE+0x5f0)[0x153fb814]
/bin/clang++(_ZN5clang25ExecuteCompilerInvocationEPNS_16CompilerInstanceE+0x7b8)[0x15657598]
/bin/clang++(_Z8cc1_mainN4llvm8ArrayRefIPKcEES2_Pv+0x4a8)[0x11bd2e04]
/bin/clang++[0x11bc137c]
/bin/clang++(main+0x9b8)[0x11bc1fb8]
/lib64/libc.so.6(+0x24700)[0x3fff83b14700]
/lib64/libc.so.6(__libc_start_main+0xc4)[0x3fff83b148f4]
Stack dump:
0.      Program arguments: /bin/clang++ -cc1 -xc++ -std=c++1z -emit-llvm -
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      <stdin>:4:3: Generating code for declaration 'A::A'
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 6.0.0 (trunk)
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /bin
Found candidate GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.2
Found candidate GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.5
Selected GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.5
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - AaronBallman


> This reproduces (with a slightly different assertion) with the same
> instructions as was originally provided using a build with assertions
> enabled:

Can confirm -- I had missed that the original was an assertion (despite it being in the title), so I was testing a release without asserts build. When I test on an asserts build, I get the miscompile. Thanks for reopening!

---

### Comment 2 - hubert-reinterpretcast

This reproduces (with a slightly different assertion) with the same instructions as was originally provided using a build with assertions enabled:
```
/llvm/clang/lib/CodeGen/CGCall.cpp:4669: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitCall(const clang::CodeGen::CGFunctionInfo &, const clang::CodeGen::CGCallee &, clang::CodeGen::ReturnValueSlot, const clang::CodeGen::CallArgList &, llvm::CallBase **, bool, clang::SourceLocation): Assertion `IRFuncTy == TypeFromVal' failed.
```
The arguments to the inherited constructor are "passed into the void".

It seems that a different symptom of the same problem is how such arguments with non-trivial destruction fail to get destroyed when compiling for the MS ABI.

### ONLINE COMPILER LINK:
https://godbolt.org/z/d4a5x8MYq

### SOURCE:
```cpp
struct NonTriv {
  NonTriv(int);
  ~NonTriv();
};
extern "C" void LookForThis(void);
NonTriv::~NonTriv() {
  LookForThis();
}
NonTriv LookForThat(void);
struct V { V() = default; V(NonTriv); };
struct A : virtual V {
  using V::V;
  A() : A(LookForThat()) { }
};
struct B : A { };
void foo() { B b; }
```
### OPTIONS:
```
-target x86_64-pc-windows -emit-llvm -O -g0
```
### IR output (snippet):
```llvm
define dso_local void @"?foo@@YAXXZ"() local_unnamed_addr #2 personality i32 (...)* @__CxxFrameHandler3 {
  %1 = alloca %struct.NonTriv, align 1
  %2 = getelementptr inbounds %struct.NonTriv, %struct.NonTriv* %1, i64 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 1, i8* nonnull %2)
  call void @"?LookForThat@@YA?AUNonTriv@@XZ"(%struct.NonTriv* nonnull sret(%struct.NonTriv) align 1 %1)
  call void @llvm.lifetime.end.p0i8(i64 1, i8* nonnull %2)
  ret void
}
```
### OBSERVATIONS:
It appears that there is no mechanism in the above by which the `NonTriv` temporary is destroyed.

Aside: I will open a GCC defect for its failure to process the side-effects of the argument initialization.

---

### Comment 3 - AaronBallman

I can no longer reproduce this issue with Clang trunk. If you're still seeing the problem with Clang trunk, please reopen this bug with additional details on how to reproduce.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Hubert Tong (hubert-reinterpretcast)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [34401](https://llvm.org/bz34401) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->AaronBallman |

## Extended Description 
When a mem-initializer selects an inherited constructor from a virtual base and the object it constructs is not a complete object, Clang tries to pass more arguments than it should.

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct V { V() = default; V(int); };
struct A : virtual V {
  using V::V;
  A() : A(19) { }
};
struct B : A { } b;
```

### COMPILER INVOCATION:
```
clang++ -cc1 -xc++ -std=c++1z -emit-llvm -
```

### EXPECTED OUTPUT:
(Clean compile)


### ACTUAL OUTPUT:
```
clang++: /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCall.cpp:4112: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitCall(const clang::CodeGen::CGFunctionInfo&amp;, const clang::CodeGen::CGCallee&amp;, clang::CodeGen::ReturnValueSlot, const clang::CodeGen::CallArgList&amp;, llvm::Instruction**): Assertion `IRCallArgs.size() == IRFuncTy-&gt;getNumParams() || IRFuncTy-&gt;isVarArg()' failed.
#<!-- -->0 0x000000001481a998 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:398:0
#<!-- -->1 0x000000001481aa9c PrintStackTraceSignalHandler(void*) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:461:0
#<!-- -->2 0x000000001481877c llvm::sys::RunSignalHandlers() /llvm_trunk/llvm/lib/Support/Signals.cpp:50:0
#<!-- -->3 0x0000000014819f40 SignalHandler(int) /llvm_trunk/llvm/lib/Support/Unix/Signals.inc:242:0
#<!-- -->4 0x00003fff840e0478  0x478 __GI_abort
#<!-- -->5 0x00003fff840e0478
#<!-- -->6 0x00003fff840e0478 __assert_fail_base (+0x478)
#<!-- -->7 0x00003fff83b30d70 __GI___assert_fail (/lib64/libc.so.6+0x40d70)
#<!-- -->8 0x00003fff83b248a4 clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&amp;, llvm::Instruction**) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCall.cpp:4112:0
#<!-- -->9 0x00003fff83b24994 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::Address, clang::CodeGen::CallArgList&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:2061:0
#<!-- -->10 0x0000000015057448 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::Address, clang::CXXConstructExpr const*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:1979:0
#<!-- -->11 0x0000000015080cd4 clang::CodeGen::CodeGenFunction::EmitCXXConstructExpr(clang::CXXConstructExpr const*, clang::CodeGen::AggValueSlot) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprCXX.cpp:622:0
#<!-- -->12 0x0000000015080518 (anonymous namespace)::AggExprEmitter::VisitCXXConstructExpr(clang::CXXConstructExpr const*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:1001:0
#<!-- -->13 0x000000001511be9c clang::StmtVisitorBase&lt;clang::make_ptr, (anonymous namespace)::AggExprEmitter, void&gt;::Visit(clang::Stmt*) /tools/clang/include/clang/AST/StmtNodes.inc:213:0
#<!-- -->14 0x000000001510c0a8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:104:0
#<!-- -->15 0x00000000151103d0 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:1532:0
#<!-- -->16 0x000000001510791c clang::CodeGen::CodeGenFunction::EmitDelegatingCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CodeGen::FunctionArgList const&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:2289:0
#<!-- -->17 0x000000001510efb8 clang::CodeGen::CodeGenFunction::EmitCtorPrologue(clang::CXXConstructorDecl const*, clang::CXXCtorType, clang::CodeGen::FunctionArgList&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:1243:0
#<!-- -->18 0x000000001508242c clang::CodeGen::CodeGenFunction::EmitConstructorBody(clang::CodeGen::FunctionArgList&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGClass.cpp:839:0
#<!-- -->19 0x000000001507d230 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:1218:0
#<!-- -->20 0x000000001507aef0 clang::CodeGen::CodeGenModule::codegenCXXStructor(clang::CXXMethodDecl const*, clang::CodeGen::StructorType) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CGCXX.cpp:234:0
#<!-- -->21 0x0000000014d88f18 (anonymous namespace)::ItaniumCXXABI::emitCXXStructor(clang::CXXMethodDecl const*, clang::CodeGen::StructorType) /llvm_trunk/llvm/tools/clang/lib/CodeGen/ItaniumCXXABI.cpp:3668:0
#<!-- -->22 0x000000001503bcf8 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:2011:0
#<!-- -->23 0x0000000014f92b50 clang::CodeGen::CodeGenModule::EmitDeferred() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:1440:0
#<!-- -->24 0x0000000014daf498 clang::CodeGen::CodeGenModule::EmitDeferred() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:1447:0
#<!-- -->25 0x0000000014dabff8 clang::CodeGen::CodeGenModule::Release() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:385:0
#<!-- -->26 0x0000000014dac068 (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/ModuleBuilder.cpp:265:0
#<!-- -->27 0x0000000014da5ea0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenAction.cpp:204:0
#<!-- -->28 0x0000000015b478a0 clang::ParseAST(clang::Sema&amp;, bool, bool) /llvm_trunk/llvm/tools/clang/lib/Parse/ParseAST.cpp:159:0
#<!-- -->29 0x0000000015b3bf14 clang::ASTFrontendAction::ExecuteAction() /llvm_trunk/llvm/tools/clang/lib/Frontend/FrontendAction.cpp:1004:0
#<!-- -->30 0x0000000016d027d8 clang::CodeGenAction::ExecuteAction() /llvm_trunk/llvm/tools/clang/lib/CodeGen/CodeGenAction.cpp:992:0
#<!-- -->31 0x0000000015482c6c clang::FrontendAction::Execute() /llvm_trunk/llvm/tools/clang/lib/Frontend/FrontendAction.cpp:902:0
#<!-- -->32 0x0000000015b3936c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /llvm_trunk/llvm/tools/clang/lib/Frontend/CompilerInstance.cpp:986:0
#<!-- -->33 0x00000000154824d4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm_trunk/llvm/tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:252:0
#<!-- -->34 0x00000000153fb814 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /llvm_trunk/llvm/tools/clang/tools/driver/cc1_main.cpp:221:0
#<!-- -->35 0x0000000015657598 ExecuteCC1Tool(llvm::ArrayRef&lt;char const*&gt;, llvm::StringRef) /llvm_trunk/llvm/tools/clang/tools/driver/driver.cpp:302:0
#<!-- -->36 0x0000000011bd2e04 main /llvm_trunk/llvm/tools/clang/tools/driver/driver.cpp:381:0
#<!-- -->37 0x0000000011bc137c generic_start_main.isra.0 (/bin/clang+++0x11bc137c)
#<!-- -->38 0x0000000011bc1fb8 __libc_start_main (/bin/clang+++0x11bc1fb8)
/bin/clang++(_ZN4llvm3sys15PrintStackTraceERNS_11raw_ostreamE+0x58)[0x1481a998]
/bin/clang++[0x1481aa9c]
/bin/clang++(_ZN4llvm3sys17RunSignalHandlersEv+0xb4)[0x1481877c]
/bin/clang++[0x14819f40]
[0x3fff840e0478]
/lib64/libc.so.6(abort+0x280)[0x3fff83b30d70]
/lib64/libc.so.6(+0x348a4)[0x3fff83b248a4]
/lib64/libc.so.6(__assert_fail+0x64)[0x3fff83b24994]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction8EmitCallERKNS0_14CGFunctionInfoERKNS0_8CGCalleeENS0_15ReturnValueSlotERKNS0_11CallArgListEPPN4llvm11InstructionE+0x2738)[0x15057448]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction22EmitCXXConstructorCallEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeEbbNS0_7AddressERNS0_11CallArgListE+0x5ac)[0x15080cd4]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction22EmitCXXConstructorCallEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeEbbNS0_7AddressEPKNS_16CXXConstructExprE+0x2b4)[0x15080518]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction20EmitCXXConstructExprEPKNS_16CXXConstructExprENS0_12AggValueSlotE+0x454)[0x1511be9c]
/bin/clang++[0x1510c0a8]
/bin/clang++[0x151103d0]
/bin/clang++[0x1510791c]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction11EmitAggExprEPKNS_4ExprENS0_12AggValueSlotE+0x164)[0x1510efb8]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction32EmitDelegatingCXXConstructorCallEPKNS_18CXXConstructorDeclERKNS0_15FunctionArgListE+0xf4)[0x1508242c]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction16EmitCtorPrologueEPKNS_18CXXConstructorDeclENS_11CXXCtorTypeERNS0_15FunctionArgListE+0x58)[0x1507d230]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction19EmitConstructorBodyERNS0_15FunctionArgListE+0x294)[0x1507aef0]
/bin/clang++(_ZN5clang7CodeGen15CodeGenFunction12GenerateCodeENS_10GlobalDeclEPN4llvm8FunctionERKNS0_14CGFunctionInfoE+0x2d4)[0x14d88f18]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule18codegenCXXStructorEPKNS_13CXXMethodDeclENS0_12StructorTypeE+0x17c)[0x1503bcf8]
/bin/clang++[0x14f92b50]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule20EmitGlobalDefinitionENS_10GlobalDeclEPN4llvm11GlobalValueE+0x168)[0x14daf498]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule12EmitDeferredEv+0x1fc)[0x14dabff8]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule12EmitDeferredEv+0x26c)[0x14dac068]
/bin/clang++(_ZN5clang7CodeGen13CodeGenModule7ReleaseEv+0x30)[0x14da5ea0]
/bin/clang++[0x15b478a0]
/bin/clang++[0x15b3bf14]
/bin/clang++(_ZN5clang8ParseASTERNS_4SemaEbb+0x364)[0x16d027d8]
/bin/clang++(_ZN5clang17ASTFrontendAction13ExecuteActionEv+0x1d4)[0x15482c6c]
/bin/clang++(_ZN5clang13CodeGenAction13ExecuteActionEv+0x4a4)[0x15b3936c]
/bin/clang++(_ZN5clang14FrontendAction7ExecuteEv+0xbc)[0x154824d4]
/bin/clang++(_ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE+0x5f0)[0x153fb814]
/bin/clang++(_ZN5clang25ExecuteCompilerInvocationEPNS_16CompilerInstanceE+0x7b8)[0x15657598]
/bin/clang++(_Z8cc1_mainN4llvm8ArrayRefIPKcEES2_Pv+0x4a8)[0x11bd2e04]
/bin/clang++[0x11bc137c]
/bin/clang++(main+0x9b8)[0x11bc1fb8]
/lib64/libc.so.6(+0x24700)[0x3fff83b14700]
/lib64/libc.so.6(__libc_start_main+0xc4)[0x3fff83b148f4]
Stack dump:
0.      Program arguments: /bin/clang++ -cc1 -xc++ -std=c++1z -emit-llvm -
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      &lt;stdin&gt;:4:3: Generating code for declaration 'A::A'
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 6.0.0 (trunk)
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /bin
Found candidate GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.2
Found candidate GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.5
Selected GCC installation: /usr/lib/gcc/ppc64le-redhat-linux/4.8.5
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 5 - shafik

Looks like this stopped asserting around clang-15: https://godbolt.org/z/bhbja4b54

CC @hubert-reinterpretcast 

---

### Comment 6 - hubert-reinterpretcast

The MS ABI problem described in https://github.com/llvm/llvm-project/issues/33749#issuecomment-980985362 still exists: https://godbolt.org/z/fMxadMea8

```cpp
struct NonTriv {
  NonTriv(int);
  ~NonTriv();
};
extern "C" void LookForThis(void) noexcept;
inline NonTriv::~NonTriv() {
  LookForThis();
}
extern "C" NonTriv LookForThat(void) noexcept;
struct V { V() = default; V(NonTriv); };
struct A : virtual V {
  using V::V;
  A() : A(LookForThat()) { }
};
struct B : A { };
extern "C" void foo() { B b; }  // `NonTriv` object constructed but never destroyed
```

However, https://wg21.link/CWG2504 further clarifies that the call to `LookForThat()` is not supposed to occur.

@shafik, how do you want to track this? Ignoring CWG 2504, there is a bug (that does not need fixing if we implement CWG 2504).

---

### Comment 7 - shafik

@hubert-reinterpretcast I think we should open a new issue then. The original issue looks fixed and the rest are incidental to it.

---

### Comment 8 - hubert-reinterpretcast

> [@hubert-reinterpretcast](https://github.com/hubert-reinterpretcast) I think we should open a new issue then. The original issue looks fixed and the rest are incidental to it.

For the new issue, which aspect do you believe the main focus should be on? That CWG 2504 is not implemented or the MS ABI bug with the "pre-CWG 2504" semantics?

---

### Comment 9 - shafik

> > [@hubert-reinterpretcast](https://github.com/hubert-reinterpretcast) I think we should open a new issue then. The original issue looks fixed and the rest are incidental to it.
> 
> For the new issue, which aspect do you believe the main focus should be on? That CWG 2504 is not implemented or the MS ABI bug with the "pre-CWG 2504" semantics?

Probably the MS ABI bug, we should eventually get to defects we don't implement when we go through our DR testing, which we do periodically. 

---

