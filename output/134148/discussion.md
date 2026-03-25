# Assertion failed: (!Current->LocalDecls.contains(D) && "Instantiated local in inner and outer scopes"), function InstantiatedLocal, file clang/lib/Sema/SemaTemplateInstantiate.cpp, line 4695

**Author:** DimitryAndric
**URL:** https://github.com/llvm/llvm-project/issues/134148

## Body

As reported in https://bugs.freebsd.org/276265, clang crashes with an assertion failure while building consumers of https://github.com/stephenberry/glaze. For example:

```c++
// clang++ -std=c++20 -c glaze-test.cpp
#include <glaze/reflection/get_name.hpp>
```

results in:

```text
Assertion failed: (!Current->LocalDecls.contains(D) && "Instantiated local in inner and outer scopes"), function InstantiatedLocal, file /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp, line 4418.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-pch -dumpdir /dev/null- -disable-free -clear-ast-before-backend -main-file-name get_name.hpp -mrelocation-model static -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/dim -fcoverage-compilation-dir=/home/dim -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++-header get_name-0dda50.hh
1.      /usr/local/include/glaze/reflection/get_name.hpp:65:69: current parser token ';'
2.      /usr/local/include/glaze/reflection/get_name.hpp:21:1: parsing namespace 'glz'
3.      /usr/local/include/glaze/reflection/get_name.hpp:63:4: parsing struct/union/class body 'glz::detail::reflect_field'
4.      /usr/local/include/glaze/reflection/get_name.hpp:48:31: instantiating variable definition 'glz::detail::get_name_impl<0, glz::detail::GLAZE_REFLECTOR>'
5.      /usr/local/include/glaze/reflection/to_tuple.hpp:1278:22: instantiating function definition 'glz::detail::get_ptr<0UL, const glz::detail::GLAZE_REFLECTOR &>'
  #0 0x0000000005f53e31 PrintStackTrace /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
  #1 0x0000000005f51c95 RunSignalHandlers /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
  #2 0x0000000005f5443a SignalHandler /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
  #3 0x000000082c9e55bc handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
  #4 0x000000082c9e4b8b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:243:1
  #5 0x00000008292712d3 ([vdso]+0x2d3)
  #6 0x000000082e55b65a __sys_thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
  #7 0x000000082f875784 raise /usr/src/lib/libc/gen/raise.c:0:10
  #8 0x000000082f92afe9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
  #9 0x000000082f858f61 (/lib/libc.so.7+0x9cf61)
 #10 0x00000000049b2ff3 reserveForParamAndGetAddressImpl<llvm::SmallVectorTemplateBase<clang::VarDecl *, true> > /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:245:35
 #11 0x00000000049b2ff3 reserveForParamAndGetAddress /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:551:9
 #12 0x00000000049b2ff3 push_back /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:576:23
 #13 0x00000000049b2ff3 InstantiatedLocal /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4423:11
 #14 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
 #15 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
 #16 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
 #17 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
 #18 0x00000000049f334f isPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
 #19 0x00000000049f334f doCastIfPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
 #20 0x00000000049f334f dyn_cast<clang::DeclContext, clang::Decl> /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
 #21 0x00000000049f334f transformedLocalDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1478:22
 #22 0x00000000049f3b0e getSema /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:152:34
 #23 0x00000000049f3b0e TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14404:7
 #24 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #25 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #26 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #27 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #28 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #29 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #30 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #31 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #32 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #33 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #34 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #35 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #36 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #37 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #38 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #39 0x0000000004a02508 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #40 0x0000000004a02508 TransformReturnStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8252:14
 #41 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #42 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #43 0x0000000004a06d58 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #44 0x0000000004a06d58 TransformIfStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8035:14
 #45 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #46 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #47 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14713:10
 #48 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1701:36
 #49 0x00000000049f4cc4 TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14644:44
 #50 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #51 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #52 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #53 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #54 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #55 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #56 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #57 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #58 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #59 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #60 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #61 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #62 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #63 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #64 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #65 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
 #66 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
 #67 0x00000000049b78d4 SubstTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4242:23
 #68 0x000000000484f397 SubstDefaultTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4836:18
 #69 0x000000000484eb1e SubstDefaultTemplateArgumentIfAvailable /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:0
 #70 0x00000000048ea184 ConvertDeducedTemplateArguments<clang::FunctionTemplateDecl> /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3004:14
 #71 0x00000000048ea184 FinishTemplateArgumentDeduction /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3835:21
 #72 0x000000000495238e operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4577:5
 #73 0x000000000495238e callback_fn<(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4576:51)> /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #74 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #75 0x00000000048ec3d3 DeduceTemplateArguments /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4584:10
 #76 0x00000000047b37aa AddTemplateOverloadCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:7687:14
 #77 0x00000000047c497a AddOverloadedCallCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13479:7
 #78 0x00000000047c47df AddOverloadedCallCandidates /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:0:5
 #79 0x00000000047c4ced getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
 #80 0x00000000047c4ced buildOverloadedCallSet /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13841:7
 #81 0x00000000047c50ac BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14047:7
 #82 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
 #83 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
 #84 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
 #85 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
 #86 0x00000000049b9486 TransformExprs /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4344:7
 #87 0x00000000049e752b TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12292:7
 #88 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #89 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #90 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #91 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #92 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #93 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #94 0x0000000004a1a969 VisitVarDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:1231:11
 #95 0x0000000004a60659 operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4099:5
 #96 0x0000000004a60659 callback_fn<(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4098:49)> /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #97 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #98 0x0000000004a2944a SubstDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4101:10
 #99 0x0000000004a074d0 TransformDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1847:8
#100 0x0000000004a074d0 TransformDeclStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8266:38
#101 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#102 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
#103 0x00000000049b725a SubstStmt /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4233:23
#104 0x0000000004a2b572 InstantiateFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5186:14
#105 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#106 0x00000000048f05f1 DeduceReturnType /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5343:25
#107 0x00000000043e3587 DiagnoseUseOfDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:301:9
#108 0x00000000047c5406 FinishOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13929:9
#109 0x00000000047c51cf BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14076:10
#110 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
#111 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
#112 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
#113 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
#114 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
#115 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
#116 0x00000000049b8514 TransformTemplateArguments<const clang::TemplateArgumentLoc *> /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4933:9
#117 0x00000000049ed580 TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13695:7
#118 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
#119 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
#120 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
#121 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
#122 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
#123 0x0000000004a2d17d getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#124 0x0000000004a2d17d CompleteVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5285:7
#125 0x0000000004a2d893 InstantiateVariableDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
#126 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#127 0x000000000442eced DoMarkVarDeclReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19463:3
#128 0x000000000442f171 getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#129 0x000000000442f171 MarkExprReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19714:17
#130 0x00000000043ef45a MarkDeclRefReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:3
#131 0x00000000043eecec isNull /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:142:33
#132 0x00000000043eecec isNull /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1009:31
#133 0x00000000043eecec getCommonPtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:957:5
#134 0x00000000043eecec getTypePtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:7747:10
#135 0x00000000043eecec operator-> /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1001:12
#136 0x00000000043eecec BuildDeclRefExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2288:25
#137 0x00000000043eebc5 /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:10
#138 0x00000000043f2e4b BuildDeclarationNameExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:3443:7
#139 0x000000000484cb68 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:10
#140 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
#141 0x00000000043e8921 ActOnIdExpression /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2864:12
#142 0x0000000003e1308a tryParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:658:17
#143 0x0000000003e148c1 isUnset /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:200:39
#144 0x0000000003e148c1 ParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:752:14
#145 0x0000000003e02a4e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#146 0x0000000003e027c0 ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#147 0x0000000003dfd29e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:20
#148 0x0000000003dfd29e ParseAssignmentExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:182:20
#149 0x0000000003e3e4d9 ParseInitializer /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:0:0
#150 0x0000000003e3e4d9 ParseCXXMemberInitializer /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3466:10
#151 0x0000000003e3d111 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#152 0x0000000003e3d111 ParseCXXClassMemberDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3305:16
#153 0x0000000003e3f3d2 ParseCXXClassMemberDeclarationWithPragmas /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:0
#154 0x0000000003e38434 MaybeDestroyTemplateIds /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:328:9
#155 0x0000000003e38434 ParseCXXMemberSpecification /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3838:7
#156 0x0000000003e36026 ParseClassSpecifier /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#157 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:95:46
#158 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:843:40
#159 0x0000000003e5b6fb ParseDeclarationSpecifiers /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:4668:23
#160 0x0000000003dda738 ParseDeclOrFunctionDefInternal /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1158:10
#161 0x0000000003dda332 ParseDeclarationOrFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#162 0x0000000003dd907c ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:14
#163 0x0000000003e2c9cd ~AttributePool /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#164 0x0000000003e2c9cd ~ParsedAttributes /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#165 0x0000000003e2c9cd ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#166 0x0000000003e2caeb Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#167 0x0000000003e2caeb ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:296:18
#168 0x0000000003e2c076 Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#169 0x0000000003e2c076 ParseNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#170 0x0000000003e54f2a ObjCDeclContextSwitch /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1093:11
#171 0x0000000003e54f2a ParseDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:2017:25
#172 0x0000000003dd8c8a ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#173 0x0000000003dd70f8 ParseTopLevelDecl /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:763:10
#174 0x0000000003dd20ee ParseAST /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#175 0x000000000371bbcf Execute /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#176 0x000000000363561d getPtr /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:279:42
#177 0x000000000363561d operator bool /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#178 0x000000000363561d ExecuteAction /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#179 0x00000000037ea5f9 ExecuteCompilerInvocation /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#180 0x000000000291632e cc1_main /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:284:15
#181 0x0000000002924367 ExecuteCC1Tool /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:215:12
#182 0x0000000002923369 clang_main /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:256:12
#183 0x0000000002921efd main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#184 0x000000082f84a494 __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:172:2
Abort trap
```

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=c++20 get_name-min.cpp
namespace {
template <class _Tp> using __remove_cvref_t = __remove_cvref(_Tp);
template <class _Tp> using remove_cvref_t = __remove_cvref_t<_Tp>;
struct GLAZE_REFLECTOR {};
template <class T, class... Args>
constexpr auto count_members = [] {
  count_members<remove_cvref_t<T>>;
  return sizeof...(Args);
}();
template <class T, int = count_members<T>> void to_tie(T &&);
GLAZE_REFLECTOR get_ptr_t() { to_tie(get_ptr_t); }
} // namespace
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Dimitry Andric (DimitryAndric)

<details>
As reported in https://bugs.freebsd.org/276265, clang crashes with an assertion failure while building consumers of https://github.com/stephenberry/glaze. For example:

```c++
// clang++ -std=c++20 -c glaze-test.cpp
#include &lt;glaze/reflection/get_name.hpp&gt;
```

results in:

```text
Assertion failed: (!Current-&gt;LocalDecls.contains(D) &amp;&amp; "Instantiated local in inner and outer scopes"), function InstantiatedLocal, file /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp, line 4418.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-pch -dumpdir /dev/null- -disable-free -clear-ast-before-backend -main-file-name get_name.hpp -mrelocation-model static -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/dim -fcoverage-compilation-dir=/home/dim -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++-header get_name-0dda50.hh
1.      /usr/local/include/glaze/reflection/get_name.hpp:65:69: current parser token ';'
2.      /usr/local/include/glaze/reflection/get_name.hpp:21:1: parsing namespace 'glz'
3.      /usr/local/include/glaze/reflection/get_name.hpp:63:4: parsing struct/union/class body 'glz::detail::reflect_field'
4.      /usr/local/include/glaze/reflection/get_name.hpp:48:31: instantiating variable definition 'glz::detail::get_name_impl&lt;0, glz::detail::GLAZE_REFLECTOR&gt;'
5.      /usr/local/include/glaze/reflection/to_tuple.hpp:1278:22: instantiating function definition 'glz::detail::get_ptr&lt;0UL, const glz::detail::GLAZE_REFLECTOR &amp;&gt;'
  #<!-- -->0 0x0000000005f53e31 PrintStackTrace /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
  #<!-- -->1 0x0000000005f51c95 RunSignalHandlers /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
  #<!-- -->2 0x0000000005f5443a SignalHandler /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
  #<!-- -->3 0x000000082c9e55bc handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
  #<!-- -->4 0x000000082c9e4b8b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:243:1
  #<!-- -->5 0x00000008292712d3 ([vdso]+0x2d3)
  #<!-- -->6 0x000000082e55b65a __sys_thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
  #<!-- -->7 0x000000082f875784 raise /usr/src/lib/libc/gen/raise.c:0:10
  #<!-- -->8 0x000000082f92afe9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
  #<!-- -->9 0x000000082f858f61 (/lib/libc.so.7+0x9cf61)
 #<!-- -->10 0x00000000049b2ff3 reserveForParamAndGetAddressImpl&lt;llvm::SmallVectorTemplateBase&lt;clang::VarDecl *, true&gt; &gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:245:35
 #<!-- -->11 0x00000000049b2ff3 reserveForParamAndGetAddress /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:551:9
 #<!-- -->12 0x00000000049b2ff3 push_back /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:576:23
 #<!-- -->13 0x00000000049b2ff3 InstantiatedLocal /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4423:11
 #<!-- -->14 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
 #<!-- -->15 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
 #<!-- -->16 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
 #<!-- -->17 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
 #<!-- -->18 0x00000000049f334f isPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
 #<!-- -->19 0x00000000049f334f doCastIfPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
 #<!-- -->20 0x00000000049f334f dyn_cast&lt;clang::DeclContext, clang::Decl&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
 #<!-- -->21 0x00000000049f334f transformedLocalDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1478:22
 #<!-- -->22 0x00000000049f3b0e getSema /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:152:34
 #<!-- -->23 0x00000000049f3b0e TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14404:7
 #<!-- -->24 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #<!-- -->25 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #<!-- -->26 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->27 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->28 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->29 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->30 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->31 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->32 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #<!-- -->33 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #<!-- -->34 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #<!-- -->35 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #<!-- -->36 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #<!-- -->37 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #<!-- -->38 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #<!-- -->39 0x0000000004a02508 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->40 0x0000000004a02508 TransformReturnStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8252:14
 #<!-- -->41 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->42 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #<!-- -->43 0x0000000004a06d58 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->44 0x0000000004a06d58 TransformIfStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8035:14
 #<!-- -->45 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->46 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #<!-- -->47 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14713:10
 #<!-- -->48 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1701:36
 #<!-- -->49 0x00000000049f4cc4 TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14644:44
 #<!-- -->50 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #<!-- -->51 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #<!-- -->52 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->53 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->54 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->55 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->56 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->57 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->58 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #<!-- -->59 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #<!-- -->60 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #<!-- -->61 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #<!-- -->62 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #<!-- -->63 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #<!-- -->64 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #<!-- -->65 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
 #<!-- -->66 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
 #<!-- -->67 0x00000000049b78d4 SubstTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4242:23
 #<!-- -->68 0x000000000484f397 SubstDefaultTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4836:18
 #<!-- -->69 0x000000000484eb1e SubstDefaultTemplateArgumentIfAvailable /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:0
 #<!-- -->70 0x00000000048ea184 ConvertDeducedTemplateArguments&lt;clang::FunctionTemplateDecl&gt; /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3004:14
 #<!-- -->71 0x00000000048ea184 FinishTemplateArgumentDeduction /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3835:21
 #<!-- -->72 0x000000000495238e operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4577:5
 #<!-- -->73 0x000000000495238e callback_fn&lt;(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4576:51)&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #<!-- -->74 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #<!-- -->75 0x00000000048ec3d3 DeduceTemplateArguments /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4584:10
 #<!-- -->76 0x00000000047b37aa AddTemplateOverloadCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:7687:14
 #<!-- -->77 0x00000000047c497a AddOverloadedCallCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13479:7
 #<!-- -->78 0x00000000047c47df AddOverloadedCallCandidates /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:0:5
 #<!-- -->79 0x00000000047c4ced getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
 #<!-- -->80 0x00000000047c4ced buildOverloadedCallSet /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13841:7
 #<!-- -->81 0x00000000047c50ac BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14047:7
 #<!-- -->82 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
 #<!-- -->83 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
 #<!-- -->84 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
 #<!-- -->85 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
 #<!-- -->86 0x00000000049b9486 TransformExprs /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4344:7
 #<!-- -->87 0x00000000049e752b TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12292:7
 #<!-- -->88 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->89 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->90 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->91 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->92 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->93 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->94 0x0000000004a1a969 VisitVarDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:1231:11
 #<!-- -->95 0x0000000004a60659 operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4099:5
 #<!-- -->96 0x0000000004a60659 callback_fn&lt;(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4098:49)&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #<!-- -->97 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #<!-- -->98 0x0000000004a2944a SubstDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4101:10
 #<!-- -->99 0x0000000004a074d0 TransformDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1847:8
#<!-- -->100 0x0000000004a074d0 TransformDeclStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8266:38
#<!-- -->101 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->102 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
#<!-- -->103 0x00000000049b725a SubstStmt /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4233:23
#<!-- -->104 0x0000000004a2b572 InstantiateFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5186:14
#<!-- -->105 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->106 0x00000000048f05f1 DeduceReturnType /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5343:25
#<!-- -->107 0x00000000043e3587 DiagnoseUseOfDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:301:9
#<!-- -->108 0x00000000047c5406 FinishOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13929:9
#<!-- -->109 0x00000000047c51cf BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14076:10
#<!-- -->110 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
#<!-- -->111 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
#<!-- -->112 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
#<!-- -->113 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
#<!-- -->114 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
#<!-- -->115 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
#<!-- -->116 0x00000000049b8514 TransformTemplateArguments&lt;const clang::TemplateArgumentLoc *&gt; /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4933:9
#<!-- -->117 0x00000000049ed580 TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13695:7
#<!-- -->118 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
#<!-- -->119 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
#<!-- -->120 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
#<!-- -->121 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
#<!-- -->122 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
#<!-- -->123 0x0000000004a2d17d getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#<!-- -->124 0x0000000004a2d17d CompleteVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5285:7
#<!-- -->125 0x0000000004a2d893 InstantiateVariableDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
#<!-- -->126 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->127 0x000000000442eced DoMarkVarDeclReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19463:3
#<!-- -->128 0x000000000442f171 getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#<!-- -->129 0x000000000442f171 MarkExprReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19714:17
#<!-- -->130 0x00000000043ef45a MarkDeclRefReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:3
#<!-- -->131 0x00000000043eecec isNull /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:142:33
#<!-- -->132 0x00000000043eecec isNull /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1009:31
#<!-- -->133 0x00000000043eecec getCommonPtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:957:5
#<!-- -->134 0x00000000043eecec getTypePtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:7747:10
#<!-- -->135 0x00000000043eecec operator-&gt; /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1001:12
#<!-- -->136 0x00000000043eecec BuildDeclRefExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2288:25
#<!-- -->137 0x00000000043eebc5 /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:10
#<!-- -->138 0x00000000043f2e4b BuildDeclarationNameExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:3443:7
#<!-- -->139 0x000000000484cb68 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:10
#<!-- -->140 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
#<!-- -->141 0x00000000043e8921 ActOnIdExpression /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2864:12
#<!-- -->142 0x0000000003e1308a tryParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:658:17
#<!-- -->143 0x0000000003e148c1 isUnset /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:200:39
#<!-- -->144 0x0000000003e148c1 ParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:752:14
#<!-- -->145 0x0000000003e02a4e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#<!-- -->146 0x0000000003e027c0 ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#<!-- -->147 0x0000000003dfd29e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:20
#<!-- -->148 0x0000000003dfd29e ParseAssignmentExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:182:20
#<!-- -->149 0x0000000003e3e4d9 ParseInitializer /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:0:0
#<!-- -->150 0x0000000003e3e4d9 ParseCXXMemberInitializer /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3466:10
#<!-- -->151 0x0000000003e3d111 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->152 0x0000000003e3d111 ParseCXXClassMemberDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3305:16
#<!-- -->153 0x0000000003e3f3d2 ParseCXXClassMemberDeclarationWithPragmas /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:0
#<!-- -->154 0x0000000003e38434 MaybeDestroyTemplateIds /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:328:9
#<!-- -->155 0x0000000003e38434 ParseCXXMemberSpecification /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3838:7
#<!-- -->156 0x0000000003e36026 ParseClassSpecifier /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#<!-- -->157 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:95:46
#<!-- -->158 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:843:40
#<!-- -->159 0x0000000003e5b6fb ParseDeclarationSpecifiers /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:4668:23
#<!-- -->160 0x0000000003dda738 ParseDeclOrFunctionDefInternal /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1158:10
#<!-- -->161 0x0000000003dda332 ParseDeclarationOrFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#<!-- -->162 0x0000000003dd907c ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->163 0x0000000003e2c9cd ~AttributePool /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#<!-- -->164 0x0000000003e2c9cd ~ParsedAttributes /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->165 0x0000000003e2c9cd ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->166 0x0000000003e2caeb Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->167 0x0000000003e2caeb ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:296:18
#<!-- -->168 0x0000000003e2c076 Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->169 0x0000000003e2c076 ParseNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#<!-- -->170 0x0000000003e54f2a ObjCDeclContextSwitch /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1093:11
#<!-- -->171 0x0000000003e54f2a ParseDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:2017:25
#<!-- -->172 0x0000000003dd8c8a ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->173 0x0000000003dd70f8 ParseTopLevelDecl /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:763:10
#<!-- -->174 0x0000000003dd20ee ParseAST /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->175 0x000000000371bbcf Execute /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#<!-- -->176 0x000000000363561d getPtr /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:279:42
#<!-- -->177 0x000000000363561d operator bool /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->178 0x000000000363561d ExecuteAction /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#<!-- -->179 0x00000000037ea5f9 ExecuteCompilerInvocation /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#<!-- -->180 0x000000000291632e cc1_main /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:284:15
#<!-- -->181 0x0000000002924367 ExecuteCC1Tool /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:215:12
#<!-- -->182 0x0000000002923369 clang_main /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:256:12
#<!-- -->183 0x0000000002921efd main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#<!-- -->184 0x000000082f84a494 __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:172:2
Abort trap
```

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=c++20 get_name-min.cpp
namespace {
template &lt;class _Tp&gt; using __remove_cvref_t = __remove_cvref(_Tp);
template &lt;class _Tp&gt; using remove_cvref_t = __remove_cvref_t&lt;_Tp&gt;;
struct GLAZE_REFLECTOR {};
template &lt;class T, class... Args&gt;
constexpr auto count_members = [] {
  count_members&lt;remove_cvref_t&lt;T&gt;&gt;;
  return sizeof...(Args);
}();
template &lt;class T, int = count_members&lt;T&gt;&gt; void to_tie(T &amp;&amp;);
GLAZE_REFLECTOR get_ptr_t() { to_tie(get_ptr_t); }
} // namespace
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dimitry Andric (DimitryAndric)

<details>
As reported in https://bugs.freebsd.org/276265, clang crashes with an assertion failure while building consumers of https://github.com/stephenberry/glaze. For example:

```c++
// clang++ -std=c++20 -c glaze-test.cpp
#include &lt;glaze/reflection/get_name.hpp&gt;
```

results in:

```text
Assertion failed: (!Current-&gt;LocalDecls.contains(D) &amp;&amp; "Instantiated local in inner and outer scopes"), function InstantiatedLocal, file /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp, line 4418.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-pch -dumpdir /dev/null- -disable-free -clear-ast-before-backend -main-file-name get_name.hpp -mrelocation-model static -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/dim -fcoverage-compilation-dir=/home/dim -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++-header get_name-0dda50.hh
1.      /usr/local/include/glaze/reflection/get_name.hpp:65:69: current parser token ';'
2.      /usr/local/include/glaze/reflection/get_name.hpp:21:1: parsing namespace 'glz'
3.      /usr/local/include/glaze/reflection/get_name.hpp:63:4: parsing struct/union/class body 'glz::detail::reflect_field'
4.      /usr/local/include/glaze/reflection/get_name.hpp:48:31: instantiating variable definition 'glz::detail::get_name_impl&lt;0, glz::detail::GLAZE_REFLECTOR&gt;'
5.      /usr/local/include/glaze/reflection/to_tuple.hpp:1278:22: instantiating function definition 'glz::detail::get_ptr&lt;0UL, const glz::detail::GLAZE_REFLECTOR &amp;&gt;'
  #<!-- -->0 0x0000000005f53e31 PrintStackTrace /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
  #<!-- -->1 0x0000000005f51c95 RunSignalHandlers /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
  #<!-- -->2 0x0000000005f5443a SignalHandler /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
  #<!-- -->3 0x000000082c9e55bc handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
  #<!-- -->4 0x000000082c9e4b8b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:243:1
  #<!-- -->5 0x00000008292712d3 ([vdso]+0x2d3)
  #<!-- -->6 0x000000082e55b65a __sys_thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
  #<!-- -->7 0x000000082f875784 raise /usr/src/lib/libc/gen/raise.c:0:10
  #<!-- -->8 0x000000082f92afe9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
  #<!-- -->9 0x000000082f858f61 (/lib/libc.so.7+0x9cf61)
 #<!-- -->10 0x00000000049b2ff3 reserveForParamAndGetAddressImpl&lt;llvm::SmallVectorTemplateBase&lt;clang::VarDecl *, true&gt; &gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:245:35
 #<!-- -->11 0x00000000049b2ff3 reserveForParamAndGetAddress /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:551:9
 #<!-- -->12 0x00000000049b2ff3 push_back /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:576:23
 #<!-- -->13 0x00000000049b2ff3 InstantiatedLocal /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4423:11
 #<!-- -->14 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
 #<!-- -->15 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
 #<!-- -->16 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
 #<!-- -->17 0x00000000049f334f doit /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
 #<!-- -->18 0x00000000049f334f isPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
 #<!-- -->19 0x00000000049f334f doCastIfPossible /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
 #<!-- -->20 0x00000000049f334f dyn_cast&lt;clang::DeclContext, clang::Decl&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
 #<!-- -->21 0x00000000049f334f transformedLocalDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1478:22
 #<!-- -->22 0x00000000049f3b0e getSema /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:152:34
 #<!-- -->23 0x00000000049f3b0e TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14404:7
 #<!-- -->24 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #<!-- -->25 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #<!-- -->26 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->27 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->28 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->29 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->30 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->31 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->32 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #<!-- -->33 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #<!-- -->34 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #<!-- -->35 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #<!-- -->36 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #<!-- -->37 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #<!-- -->38 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #<!-- -->39 0x0000000004a02508 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->40 0x0000000004a02508 TransformReturnStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8252:14
 #<!-- -->41 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->42 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #<!-- -->43 0x0000000004a06d58 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->44 0x0000000004a06d58 TransformIfStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8035:14
 #<!-- -->45 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
 #<!-- -->46 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
 #<!-- -->47 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14713:10
 #<!-- -->48 0x00000000049f4cc4 TransformLambdaBody /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1701:36
 #<!-- -->49 0x00000000049f4cc4 TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:14644:44
 #<!-- -->50 0x00000000049e4b7b TransformLambdaExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:0:38
 #<!-- -->51 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
 #<!-- -->52 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->53 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->54 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->55 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->56 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->57 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->58 0x0000000004a291a5 VisitVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
 #<!-- -->59 0x0000000004a2cf8b BuildVarTemplateInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5259:20
 #<!-- -->60 0x000000000484c3e0 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4273:8
 #<!-- -->61 0x000000000484cb0b CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4305:21
 #<!-- -->62 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
 #<!-- -->63 0x00000000049ed62e RebuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:22
 #<!-- -->64 0x00000000049ed62e TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13720:23
 #<!-- -->65 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
 #<!-- -->66 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
 #<!-- -->67 0x00000000049b78d4 SubstTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4242:23
 #<!-- -->68 0x000000000484f397 SubstDefaultTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4836:18
 #<!-- -->69 0x000000000484eb1e SubstDefaultTemplateArgumentIfAvailable /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:0
 #<!-- -->70 0x00000000048ea184 ConvertDeducedTemplateArguments&lt;clang::FunctionTemplateDecl&gt; /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3004:14
 #<!-- -->71 0x00000000048ea184 FinishTemplateArgumentDeduction /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3835:21
 #<!-- -->72 0x000000000495238e operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4577:5
 #<!-- -->73 0x000000000495238e callback_fn&lt;(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4576:51)&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #<!-- -->74 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #<!-- -->75 0x00000000048ec3d3 DeduceTemplateArguments /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4584:10
 #<!-- -->76 0x00000000047b37aa AddTemplateOverloadCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:7687:14
 #<!-- -->77 0x00000000047c497a AddOverloadedCallCandidate /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13479:7
 #<!-- -->78 0x00000000047c47df AddOverloadedCallCandidates /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:0:5
 #<!-- -->79 0x00000000047c4ced getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
 #<!-- -->80 0x00000000047c4ced buildOverloadedCallSet /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13841:7
 #<!-- -->81 0x00000000047c50ac BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14047:7
 #<!-- -->82 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
 #<!-- -->83 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
 #<!-- -->84 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
 #<!-- -->85 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
 #<!-- -->86 0x00000000049b9486 TransformExprs /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4344:7
 #<!-- -->87 0x00000000049e752b TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12292:7
 #<!-- -->88 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
 #<!-- -->89 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
 #<!-- -->90 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
 #<!-- -->91 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
 #<!-- -->92 0x0000000004a1b21b isInvalidDecl /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:594:46
 #<!-- -->93 0x0000000004a1b21b BuildVariableInstantiation /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5415:16
 #<!-- -->94 0x0000000004a1a969 VisitVarDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:1231:11
 #<!-- -->95 0x0000000004a60659 operator() /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4099:5
 #<!-- -->96 0x0000000004a60659 callback_fn&lt;(lambda at /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4098:49)&gt; /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
 #<!-- -->97 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
 #<!-- -->98 0x0000000004a2944a SubstDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4101:10
 #<!-- -->99 0x0000000004a074d0 TransformDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1847:8
#<!-- -->100 0x0000000004a074d0 TransformDeclStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:8266:38
#<!-- -->101 0x00000000049ee604 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->102 0x00000000049ee604 TransformCompoundStmt /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:7831:16
#<!-- -->103 0x00000000049b725a SubstStmt /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4233:23
#<!-- -->104 0x0000000004a2b572 InstantiateFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5186:14
#<!-- -->105 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->106 0x00000000048f05f1 DeduceReturnType /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5343:25
#<!-- -->107 0x00000000043e3587 DiagnoseUseOfDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:301:9
#<!-- -->108 0x00000000047c5406 FinishOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:13929:9
#<!-- -->109 0x00000000047c51cf BuildOverloadedCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaOverload.cpp:14076:10
#<!-- -->110 0x00000000043e916f BuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6487:16
#<!-- -->111 0x00000000043ff7fb ActOnCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:6373:7
#<!-- -->112 0x00000000049e76a0 RebuildCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:2837:22
#<!-- -->113 0x00000000049e76a0 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:12313:23
#<!-- -->114 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4754:9
#<!-- -->115 0x00000000049b7a30 TransformTemplateArgument /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1605:25
#<!-- -->116 0x00000000049b8514 TransformTemplateArguments&lt;const clang::TemplateArgumentLoc *&gt; /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:4933:9
#<!-- -->117 0x00000000049ed580 TransformUnresolvedLookupExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:13695:7
#<!-- -->118 0x00000000049e74c8 TransformCallExpr /usr/src/contrib/llvm-project/clang/lib/Sema/TreeTransform.h:0:36
#<!-- -->119 0x00000000049b58e5 SubstInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4289:23
#<!-- -->120 0x0000000004a27c74 pop /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3036:12
#<!-- -->121 0x0000000004a27c74 ~ContextRAII /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:3046:22
#<!-- -->122 0x0000000004a27c74 InstantiateVariableInitializer /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5447:5
#<!-- -->123 0x0000000004a2d17d getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#<!-- -->124 0x0000000004a2d17d CompleteVarTemplateSpecializationDecl /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5285:7
#<!-- -->125 0x0000000004a2d893 InstantiateVariableDefinition /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:11
#<!-- -->126 0x0000000003f493ba runWithSufficientStackSpace /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->127 0x000000000442eced DoMarkVarDeclReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19463:3
#<!-- -->128 0x000000000442f171 getLangOpts /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:593:51
#<!-- -->129 0x000000000442f171 MarkExprReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:19714:17
#<!-- -->130 0x00000000043ef45a MarkDeclRefReferenced /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:3
#<!-- -->131 0x00000000043eecec isNull /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:142:33
#<!-- -->132 0x00000000043eecec isNull /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1009:31
#<!-- -->133 0x00000000043eecec getCommonPtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:957:5
#<!-- -->134 0x00000000043eecec getTypePtr /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:7747:10
#<!-- -->135 0x00000000043eecec operator-&gt; /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:1001:12
#<!-- -->136 0x00000000043eecec BuildDeclRefExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2288:25
#<!-- -->137 0x00000000043eebc5 /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:0:10
#<!-- -->138 0x00000000043f2e4b BuildDeclarationNameExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:3443:7
#<!-- -->139 0x000000000484cb68 CheckVarTemplateId /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:10
#<!-- -->140 0x000000000484d54f BuildTemplateIdExpr /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplate.cpp:4418:22
#<!-- -->141 0x00000000043e8921 ActOnIdExpression /usr/src/contrib/llvm-project/clang/lib/Sema/SemaExpr.cpp:2864:12
#<!-- -->142 0x0000000003e1308a tryParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:658:17
#<!-- -->143 0x0000000003e148c1 isUnset /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:200:39
#<!-- -->144 0x0000000003e148c1 ParseCXXIdExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:752:14
#<!-- -->145 0x0000000003e02a4e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#<!-- -->146 0x0000000003e027c0 ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:0
#<!-- -->147 0x0000000003dfd29e ParseCastExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:0:20
#<!-- -->148 0x0000000003dfd29e ParseAssignmentExpression /usr/src/contrib/llvm-project/clang/lib/Parse/ParseExpr.cpp:182:20
#<!-- -->149 0x0000000003e3e4d9 ParseInitializer /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:0:0
#<!-- -->150 0x0000000003e3e4d9 ParseCXXMemberInitializer /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3466:10
#<!-- -->151 0x0000000003e3d111 isInvalid /usr/src/contrib/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->152 0x0000000003e3d111 ParseCXXClassMemberDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3305:16
#<!-- -->153 0x0000000003e3f3d2 ParseCXXClassMemberDeclarationWithPragmas /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:0
#<!-- -->154 0x0000000003e38434 MaybeDestroyTemplateIds /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:328:9
#<!-- -->155 0x0000000003e38434 ParseCXXMemberSpecification /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3838:7
#<!-- -->156 0x0000000003e36026 ParseClassSpecifier /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#<!-- -->157 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/SmallVector.h:95:46
#<!-- -->158 0x0000000003e5b6fb empty /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:843:40
#<!-- -->159 0x0000000003e5b6fb ParseDeclarationSpecifiers /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:4668:23
#<!-- -->160 0x0000000003dda738 ParseDeclOrFunctionDefInternal /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1158:10
#<!-- -->161 0x0000000003dda332 ParseDeclarationOrFunctionDefinition /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#<!-- -->162 0x0000000003dd907c ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->163 0x0000000003e2c9cd ~AttributePool /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#<!-- -->164 0x0000000003e2c9cd ~ParsedAttributes /usr/src/contrib/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->165 0x0000000003e2c9cd ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->166 0x0000000003e2caeb Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->167 0x0000000003e2caeb ParseInnerNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:296:18
#<!-- -->168 0x0000000003e2c076 Exit /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->169 0x0000000003e2c076 ParseNamespace /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#<!-- -->170 0x0000000003e54f2a ObjCDeclContextSwitch /usr/src/contrib/llvm-project/clang/include/clang/Parse/Parser.h:1093:11
#<!-- -->171 0x0000000003e54f2a ParseDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/ParseDecl.cpp:2017:25
#<!-- -->172 0x0000000003dd8c8a ParseExternalDeclaration /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->173 0x0000000003dd70f8 ParseTopLevelDecl /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:763:10
#<!-- -->174 0x0000000003dd20ee ParseAST /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->175 0x000000000371bbcf Execute /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#<!-- -->176 0x000000000363561d getPtr /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:279:42
#<!-- -->177 0x000000000363561d operator bool /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->178 0x000000000363561d ExecuteAction /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#<!-- -->179 0x00000000037ea5f9 ExecuteCompilerInvocation /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#<!-- -->180 0x000000000291632e cc1_main /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:284:15
#<!-- -->181 0x0000000002924367 ExecuteCC1Tool /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:215:12
#<!-- -->182 0x0000000002923369 clang_main /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:256:12
#<!-- -->183 0x0000000002921efd main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#<!-- -->184 0x000000082f84a494 __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:172:2
Abort trap
```

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=c++20 get_name-min.cpp
namespace {
template &lt;class _Tp&gt; using __remove_cvref_t = __remove_cvref(_Tp);
template &lt;class _Tp&gt; using remove_cvref_t = __remove_cvref_t&lt;_Tp&gt;;
struct GLAZE_REFLECTOR {};
template &lt;class T, class... Args&gt;
constexpr auto count_members = [] {
  count_members&lt;remove_cvref_t&lt;T&gt;&gt;;
  return sizeof...(Args);
}();
template &lt;class T, int = count_members&lt;T&gt;&gt; void to_tie(T &amp;&amp;);
GLAZE_REFLECTOR get_ptr_t() { to_tie(get_ptr_t); }
} // namespace
```

</details>


---

### Comment 3 - zwuis

It looks like a duplicate of #97881.

---

### Comment 4 - shafik

> It looks like a duplicate of [#97881](https://github.com/llvm/llvm-project/issues/97881).

It could be the same but not 💯 sure

---

### Comment 5 - DimitryAndric

Still reproduces as of `llvmorg-22-init-05731-g`6111c9cfdcc. :)

---

