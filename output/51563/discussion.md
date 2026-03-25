# clang crash in clang::Sema::CheckParameterPacksForExpansion

**Author:** cmtice
**URL:** https://github.com/llvm/llvm-project/issues/51563
**Status:** Closed
**Labels:** c++14, clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2025-07-31T20:50:21Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [52221](https://llvm.org/bz52221) |
| Version | unspecified |
| OS | All |
| CC | @cmtice |

## Comments

### Comment 1 - cmtice

When compiling this file (which contains invalid source code), clang crashes.  

Before it crashes, the compiler gives the error: 
/usr/local/google/home/cmtice/crash-test.cc:9:37: error: use of undeclared identifier 'f'

  d([](auto... g) { int h(g...); }, f);



Based on the stack dump, I looked at the code and it looks like clang::Sema::CheckParameterPacksForExpansion is

calling clang::LocalInstantiationScope::findInstantiationOf, which is returning a null pointer (because "declaration not instantiated in this scope"), and the caller is immediately trying to dereference the pointer without checking it first to see if it's null or not:

// Code from SemaTemplateVariadic.cpp, lines 710-713

      llvm::PointerUnion<Decl *, DeclArgumentPack *> *Instantiation
        = CurrentInstantiationScope->findInstantiationOf(
                                        i->first.get<NamedDecl *>());
      if (Instantiation->is<DeclArgumentPack *>()) {


Here's the actual stack dump:
```
$ llvm-project/build3/bin/clang++ ~/crash-test.cc
/usr/local/google/home/cmtice/crash-test.cc:9:37: error: use of undeclared identifier 'f'
  d([](auto... g) { int h(g...); }, f);
                                    ^
clang-14: /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3611: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/local/google3/cmtice/llvm-project/build3/bin/clang-14 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -main-file-name crash-test.cc -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fcoverage-compilation-dir=/usr/local/google/home/cmtice -resource-dir /usr/local/google3/cmtice/llvm-project/build3/lib/clang/14.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/backward -internal-isystem /usr/local/google3/cmtice/llvm-project/build3/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir=/usr/local/google/home/cmtice -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/crash-test-08be6d.o -x c++ /usr/local/google/home/cmtice/crash-test.cc
1.	<eof> parser at end of file
2.	/usr/local/google/home/cmtice/crash-test.cc:3:14: instantiating function definition 'a<int>::b'
 #&#8203;0 0x000056451e9b3c1b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google3/cmtice/llvm-project/llvm/lib/Support/Unix/Signals.inc:565:22
 #&#8203;1 0x000056451e9b3cd2 PrintStackTraceSignalHandler(void*) /usr/local/google3/cmtice/llvm-project/llvm/lib/Support/Unix/Signals.inc:632:1
 #&#8203;2 0x000056451e9b1cc8 llvm::sys::RunSignalHandlers() /usr/local/google3/cmtice/llvm-project/llvm/lib/Support/Signals.cpp:97:20
 #&#8203;3 0x000056451e9b366e SignalHandler(int) /usr/local/google3/cmtice/llvm-project/llvm/lib/Support/Unix/Signals.inc:407:1
 #&#8203;4 0x00007f6f87ddf8e0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x138e0)
 #&#8203;5 0x00007f6f87884e71 raise ./signal/../sysdeps/unix/sysv/linux/raise.c:50:1
 #&#8203;6 0x00007f6f8786e536 abort ./stdlib/abort.c:81:7
 #&#8203;7 0x00007f6f8786e41f get_sysdep_segment_value ./intl/loadmsgcat.c:509:8
 #&#8203;8 0x00007f6f8786e41f _nl_load_domain ./intl/loadmsgcat.c:970:34
 #&#8203;9 0x00007f6f8787d7f2 (/lib/x86_64-linux-gnu/libc.so.6+0x357f2)
#&#8203;10 0x0000564522787cd9 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3611:3
#&#8203;11 0x0000564522837e1b clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation> >, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, llvm::Optional<unsigned int>&) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateVariadic.cpp:711:57
#&#8203;12 0x000056452277e6c4 (anonymous namespace)::TemplateInstantiator::TryExpandParameterPacks(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation> >, bool&, bool&, llvm::Optional<unsigned int>&) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:953:55
#&#8203;13 0x0000564522791ade clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:3945:47
#&#8203;14 0x000056452279fce1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenListExpr(clang::ParenListExpr*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:11315:3
#&#8203;15 0x000056452278b878 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:1347:1
#&#8203;16 0x0000564522791616 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:3881:38
#&#8203;17 0x000056452278767e clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3495:63
#&#8203;18 0x00005645227ead24 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5292:74
#&#8203;19 0x00005645227eab28 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5261:29
#&#8203;20 0x00005645227d9e4b clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:1083:37
#&#8203;21 0x00005645227d9a62 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:1037:1
#&#8203;22 0x00005645227ca02e clang::declvisitor::Base<std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(clang::Decl*) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/DeclNodes.inc:473:1
#&#8203;23 0x00005645227e54cc clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()::operator()() const /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:3839:12
#&#8203;24 0x00005645227ef339 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) /usr/local/google3/cmtice/llvm-project/llvm/include/llvm/ADT/STLExtras.h:178:40
#&#8203;25 0x000056451e8dae46 llvm::function_ref<void ()>::operator()() const /usr/local/google3/cmtice/llvm-project/llvm/include/llvm/ADT/STLExtras.h:200:62
#&#8203;26 0x0000564521b64e83 clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /usr/local/google3/cmtice/llvm-project/clang/include/clang/Basic/Stack.h:52:3
#&#8203;27 0x0000564521b55e30 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/Sema.cpp:489:1
#&#8203;28 0x00005645227e55a2 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:3841:10
#&#8203;29 0x000056452277f2ba (anonymous namespace)::TemplateInstantiator::TransformDefinition(clang::SourceLocation, clang::Decl*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1248:35
#&#8203;30 0x00005645227acc7b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:7608:57
#&#8203;31 0x000056452278fb8d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:97:1
#&#8203;32 0x00005645227b80c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:7208:51
#&#8203;33 0x00005645227ac13d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:7195:1
#&#8203;34 0x000056452278fae1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:73:1
#&#8203;35 0x000056452279383a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaBody(clang::LambdaExpr*, clang::Stmt*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:13022:1
#&#8203;36 0x00005645227899cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:12994:63
#&#8203;37 0x000056452277eb96 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1129:72
#&#8203;38 0x000056452278b343 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:1153:1
#&#8203;39 0x0000564522791482 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:3864:38
#&#8203;40 0x0000564522791f6b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:4016:49
#&#8203;41 0x00005645227990af clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:10806:3
#&#8203;42 0x000056452278ac8b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:871:1
#&#8203;43 0x0000564522790a3a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:3785:48
#&#8203;44 0x00005645227b80c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:7208:51
#&#8203;45 0x00005645227ac13d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/TreeTransform.h:7195:1
#&#8203;46 0x000056452278fae1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /usr/local/google3/cmtice/llvm-project/build3/tools/clang/include/clang/AST/StmtNodes.inc:73:1
#&#8203;47 0x0000564522787491 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3464:38
#&#8203;48 0x00005645227e9cf9 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5030:45
#&#8203;49 0x00005645227ee0ac clang::Sema::PerformPendingInstantiations(bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6276:32
#&#8203;50 0x0000564521b5809a clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/Sema.cpp:1058:66
#&#8203;51 0x0000564521b582f9 clang::Sema::ActOnEndOfTranslationUnit() /usr/local/google3/cmtice/llvm-project/clang/lib/Sema/Sema.cpp:1101:9
#&#8203;52 0x0000564521a01113 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Parse/Parser.cpp:696:12
#&#8203;53 0x00005645219fc89e clang::ParseAST(clang::Sema&, bool, bool) /usr/local/google3/cmtice/llvm-project/clang/lib/Parse/ParseAST.cpp:158:37
#&#8203;54 0x000056451f726835 clang::ASTFrontendAction::ExecuteAction() /usr/local/google3/cmtice/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1064:11
#&#8203;55 0x00005645200d0433 clang::CodeGenAction::ExecuteAction() /usr/local/google3/cmtice/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1105:5
#&#8203;56 0x000056451f726165 clang::FrontendAction::Execute() /usr/local/google3/cmtice/llvm-project/clang/lib/Frontend/FrontendAction.cpp:961:38
#&#8203;57 0x000056451f66e651 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/local/google3/cmtice/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1030:42
#&#8203;58 0x000056451f8adc35 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google3/cmtice/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:261:38
#&#8203;59 0x000056451b850f42 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/local/google3/cmtice/llvm-project/clang/tools/driver/cc1_main.cpp:246:40
#&#8203;60 0x000056451b845e99 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) /usr/local/google3/cmtice/llvm-project/clang/tools/driver/driver.cpp:317:20
#&#8203;61 0x000056451b8464c4 main /usr/local/google3/cmtice/llvm-project/clang/tools/driver/driver.cpp:388:26
#&#8203;62 0x00007f6f8786fe4a __libc_start_main ./csu/../csu/libc-start.c:314:16
#&#8203;63 0x000056451b84480a _start (/usr/local/google3/cmtice/llvm-project/build3/bin/clang-14+0x179980a)
clang-14: error: unable to execute command: Aborted
clang-14: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 14.0.0 (git@github.com:llvm/llvm-project.git 3af474c0a15a642e5d73981d5cdf465bbdfc3d51)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google3/cmtice/llvm-project/build3/bin
clang-14: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-14: note: diagnostic msg: /tmp/crash-test-cd776e.cpp
clang-14: note: diagnostic msg: /tmp/crash-test-cd776e.sh
clang-14: note: diagnostic msg: 

********************

```

---

### Comment 2 - cmtice

[Crashing command](https://user-images.githubusercontent.com/5561162/143762648-ad2ba0d8-65f4-4893-af66-4dc997091d8d.gz)
This is the other file the crash told me to upload when filing the bug.

---

### Comment 3 - cmtice

[Crashing test case](https://user-images.githubusercontent.com/5561162/143762649-5ef4ec52-0941-4e92-8301-1df7036963a9.gz)
This is the crashing source file.  It's one of the two files the crash told me to upload when filing a bug.

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-bug

---

### Comment 5 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 6 - shafik

Still crashes: https://godbolt.org/z/69hv6oE6r

Maybe related to: https://github.com/llvm/llvm-project/issues/39263

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-c-14

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [52221](https://llvm.org/bz52221) |
| Version | unspecified |
| OS | All |
| CC | @<!-- -->cmtice |
</details>


---

### Comment 8 - shafik

Fixed in trunk: https://godbolt.org/z/9se1ea9f7

---

