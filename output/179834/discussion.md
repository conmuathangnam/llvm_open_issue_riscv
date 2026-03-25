# [HLSL] Define Matrix cbuffer alignment

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/179834
**Status:** Closed
**Labels:** clang:frontend, crash, HLSL
**Closed Date:** 2026-02-18T18:42:33Z

## Body

Today if we define a matrix inside a struct the scalar bit width of the  type is over 64bits (in my testing 256). This is because we are not limiting them to 16 the way we should be. This causes us to hit an assert. 

```
Assertion failed: (Context.getTypeSize(T) <= 64 && "Scalar bit widths larger than 64 not supported"), function calculateLegacyCbufferFieldAlign, file SemaHLSL.cpp, line 231.
Process 79983 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #4: 0x0000000103f89b54 clang`calculateLegacyCbufferSize(clang::ASTContext const&, clang::QualType) (.cold.6) [inlined] calculateLegacyCbufferFieldAlign(Context=<unavailable>, T=<unavailable>) at SemaHLSL.cpp:230:3 [opt]
   227    if (const VectorType *VT = T->getAs<VectorType>())
   228      return calculateLegacyCbufferFieldAlign(Context, VT->getElementType());
   229 
-> 230    assert(Context.getTypeSize(T) <= 64 &&
   231           "Scalar bit widths larger than 64 not supported");
   232 
   233    // Scalar types are aligned to their byte width
error: libclangSema.a(SemaHLSL.cpp.o) 0x0000000000201090: DW_TAG_variable has an invalid location: DW_OP_breg8 +0, DW_OP_convert 0x3a, DW_OP_convert 0x34, DW_OP_stack_value
* thread #1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #0: 0x000000019c20e388 libsystem_kernel.dylib`__pthread_kill + 8
    frame #1: 0x000000019c247848 libsystem_pthread.dylib`pthread_kill + 296
    frame #2: 0x000000019c1509e4 libsystem_c.dylib`abort + 124
    frame #3: 0x000000019c14fc18 libsystem_c.dylib`__assert_rtn + 284
  * frame #4: 0x0000000103f89b54 clang`calculateLegacyCbufferSize(clang::ASTContext const&, clang::QualType) (.cold.6) [inlined] calculateLegacyCbufferFieldAlign(Context=<unavailable>, T=<unavailable>) at SemaHLSL.cpp:230:3 [opt]
    frame #5: 0x0000000103f89b34 clang`calculateLegacyCbufferSize(clang::ASTContext const&, clang::QualType) (.cold.6) at SemaHLSL.cpp:247:29 [opt]
    frame #6: 0x0000000102e7148c clang`calculateLegacyCbufferSize(clang::ASTContext const&, clang::QualType) [inlined] calculateLegacyCbufferFieldAlign(Context=0x000000014e01a800, T=<unavailable>) at SemaHLSL.cpp:230:3 [opt]
    frame #7: 0x0000000102e71488 clang`calculateLegacyCbufferSize(Context=<unavailable>, T=<unavailable>) at SemaHLSL.cpp:247:29 [opt]
    frame #8: 0x0000000102e62ac8 clang`clang::SemaHLSL::ActOnFinishBuffer(clang::Decl*, clang::SourceLocation) at SemaHLSL.cpp:324:21 [opt]
    frame #9: 0x0000000102e62a90 clang`clang::SemaHLSL::ActOnFinishBuffer(this=<unavailable>, Dcl=<unavailable>, RBrace=<unavailable>) at SemaHLSL.cpp:600:3 [opt]
    frame #10: 0x0000000102a7dcdc clang`clang::Parser::ParseHLSLBuffer(this=<unavailable>, DeclEnd=0x000000016fdf6330, Attrs=<unavailable>) at ParseHLSL.cpp:100:18 [opt]
    frame #11: 0x0000000102a380c0 clang`clang::Parser::ParseDeclaration(this=0x000000014e045600, Context=File, DeclEnd=0x000000016fdf6330, DeclAttrs=0x000000016fdf6568, DeclSpecAttrs=0x000000016fdf6518, DeclSpecStart=0x0000000000000000) at ParseDecl.cpp:1903:18 [opt]
    frame #12: 0x0000000102ad8c3c clang`clang::Parser::ParseExternalDeclaration(this=0x000000014e045600, Attrs=0x000000016fdf6568, DeclSpecAttrs=0x000000016fdf6518, DS=0x0000000000000000) at Parser.cpp:0 [opt]
    frame #13: 0x0000000102ad7a48 clang`clang::Parser::ParseTopLevelDecl(this=0x000000014e045600, Result=0x000000016fdf6630, ImportState=0x000000016fdf662c) at Parser.cpp:706:12 [opt]
    frame #14: 0x0000000102a27120 clang`clang::ParseAST(S=0x000000014e039000, PrintStats=<unavailable>, SkipFunctionBodies=<unavailable>) at ParseAST.cpp:170:20 [opt]
    frame #15: 0x0000000101f55bb4 clang`clang::HLSLFrontendAction::ExecuteAction(this=0x000000014cf0e330) at FrontendActions.cpp:1325:35 [opt]
    frame #16: 0x0000000101f39dcc clang`clang::FrontendAction::Execute(this=<unavailable>) at FrontendAction.cpp:1313:3 [opt]
    frame #17: 0x0000000101ed3f10 clang`clang::CompilerInstance::ExecuteAction(this=0x000000014cf0d470, Act=0x000000014cf0e330) at CompilerInstance.cpp:1007:33 [opt]
    frame #18: 0x0000000101fbca48 clang`clang::ExecuteCompilerInvocation(Clang=0x000000014cf0d470) at ExecuteCompilerInvocation.cpp:310:25 [opt]
    frame #19: 0x0000000100008144 clang`cc1_main(Argv=<unavailable>, Argv0=<unavailable>, MainAddr=0x0000000100003bf4) at cc1_main.cpp:304:15 [opt]
    frame #20: 0x0000000100005b14 clang`ExecuteCC1Tool(ArgV=0x000000016fdfddc0, ToolContext=0x000000016fdfe658, VFS=<unavailable>) at driver.cpp:226:12 [opt]
    frame #21: 0x0000000100005230 clang`clang_main(Argc=<unavailable>, Argv=<unavailable>, ToolContext=0x000000016fdfe658) at driver.cpp:274:12 [opt]
    frame #22: 0x0000000100010a94 clang`main(argc=<unavailable>, argv=<unavailable>) at clang-driver.cpp:17:10 [opt]
    frame #23: 0x000000019bea6b98 dyld`start + 6076
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Farzon Lotfi (farzonl)

<details>
Today if we define a matrix inside a struct the scalar bit width of the  type is over 64bits (in my testing 256). This is because we are not limiting them to 16 the way we should be. This causes us to hit an assert. 

```
Assertion failed: (Context.getTypeSize(T) &lt;= 64 &amp;&amp; "Scalar bit widths larger than 64 not supported"), function calculateLegacyCbufferFieldAlign, file SemaHLSL.cpp, line 231.
Process 79983 stopped
* thread #<!-- -->1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #<!-- -->4: 0x0000000103f89b54 clang`calculateLegacyCbufferSize(clang::ASTContext const&amp;, clang::QualType) (.cold.6) [inlined] calculateLegacyCbufferFieldAlign(Context=&lt;unavailable&gt;, T=&lt;unavailable&gt;) at SemaHLSL.cpp:230:3 [opt]
   227    if (const VectorType *VT = T-&gt;getAs&lt;VectorType&gt;())
   228      return calculateLegacyCbufferFieldAlign(Context, VT-&gt;getElementType());
   229 
-&gt; 230    assert(Context.getTypeSize(T) &lt;= 64 &amp;&amp;
   231           "Scalar bit widths larger than 64 not supported");
   232 
   233    // Scalar types are aligned to their byte width
error: libclangSema.a(SemaHLSL.cpp.o) 0x0000000000201090: DW_TAG_variable has an invalid location: DW_OP_breg8 +0, DW_OP_convert 0x3a, DW_OP_convert 0x34, DW_OP_stack_value
* thread #<!-- -->1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #<!-- -->0: 0x000000019c20e388 libsystem_kernel.dylib`__pthread_kill + 8
    frame #<!-- -->1: 0x000000019c247848 libsystem_pthread.dylib`pthread_kill + 296
    frame #<!-- -->2: 0x000000019c1509e4 libsystem_c.dylib`abort + 124
    frame #<!-- -->3: 0x000000019c14fc18 libsystem_c.dylib`__assert_rtn + 284
  * frame #<!-- -->4: 0x0000000103f89b54 clang`calculateLegacyCbufferSize(clang::ASTContext const&amp;, clang::QualType) (.cold.6) [inlined] calculateLegacyCbufferFieldAlign(Context=&lt;unavailable&gt;, T=&lt;unavailable&gt;) at SemaHLSL.cpp:230:3 [opt]
    frame #<!-- -->5: 0x0000000103f89b34 clang`calculateLegacyCbufferSize(clang::ASTContext const&amp;, clang::QualType) (.cold.6) at SemaHLSL.cpp:247:29 [opt]
    frame #<!-- -->6: 0x0000000102e7148c clang`calculateLegacyCbufferSize(clang::ASTContext const&amp;, clang::QualType) [inlined] calculateLegacyCbufferFieldAlign(Context=0x000000014e01a800, T=&lt;unavailable&gt;) at SemaHLSL.cpp:230:3 [opt]
    frame #<!-- -->7: 0x0000000102e71488 clang`calculateLegacyCbufferSize(Context=&lt;unavailable&gt;, T=&lt;unavailable&gt;) at SemaHLSL.cpp:247:29 [opt]
    frame #<!-- -->8: 0x0000000102e62ac8 clang`clang::SemaHLSL::ActOnFinishBuffer(clang::Decl*, clang::SourceLocation) at SemaHLSL.cpp:324:21 [opt]
    frame #<!-- -->9: 0x0000000102e62a90 clang`clang::SemaHLSL::ActOnFinishBuffer(this=&lt;unavailable&gt;, Dcl=&lt;unavailable&gt;, RBrace=&lt;unavailable&gt;) at SemaHLSL.cpp:600:3 [opt]
    frame #<!-- -->10: 0x0000000102a7dcdc clang`clang::Parser::ParseHLSLBuffer(this=&lt;unavailable&gt;, DeclEnd=0x000000016fdf6330, Attrs=&lt;unavailable&gt;) at ParseHLSL.cpp:100:18 [opt]
    frame #<!-- -->11: 0x0000000102a380c0 clang`clang::Parser::ParseDeclaration(this=0x000000014e045600, Context=File, DeclEnd=0x000000016fdf6330, DeclAttrs=0x000000016fdf6568, DeclSpecAttrs=0x000000016fdf6518, DeclSpecStart=0x0000000000000000) at ParseDecl.cpp:1903:18 [opt]
    frame #<!-- -->12: 0x0000000102ad8c3c clang`clang::Parser::ParseExternalDeclaration(this=0x000000014e045600, Attrs=0x000000016fdf6568, DeclSpecAttrs=0x000000016fdf6518, DS=0x0000000000000000) at Parser.cpp:0 [opt]
    frame #<!-- -->13: 0x0000000102ad7a48 clang`clang::Parser::ParseTopLevelDecl(this=0x000000014e045600, Result=0x000000016fdf6630, ImportState=0x000000016fdf662c) at Parser.cpp:706:12 [opt]
    frame #<!-- -->14: 0x0000000102a27120 clang`clang::ParseAST(S=0x000000014e039000, PrintStats=&lt;unavailable&gt;, SkipFunctionBodies=&lt;unavailable&gt;) at ParseAST.cpp:170:20 [opt]
    frame #<!-- -->15: 0x0000000101f55bb4 clang`clang::HLSLFrontendAction::ExecuteAction(this=0x000000014cf0e330) at FrontendActions.cpp:1325:35 [opt]
    frame #<!-- -->16: 0x0000000101f39dcc clang`clang::FrontendAction::Execute(this=&lt;unavailable&gt;) at FrontendAction.cpp:1313:3 [opt]
    frame #<!-- -->17: 0x0000000101ed3f10 clang`clang::CompilerInstance::ExecuteAction(this=0x000000014cf0d470, Act=0x000000014cf0e330) at CompilerInstance.cpp:1007:33 [opt]
    frame #<!-- -->18: 0x0000000101fbca48 clang`clang::ExecuteCompilerInvocation(Clang=0x000000014cf0d470) at ExecuteCompilerInvocation.cpp:310:25 [opt]
    frame #<!-- -->19: 0x0000000100008144 clang`cc1_main(Argv=&lt;unavailable&gt;, Argv0=&lt;unavailable&gt;, MainAddr=0x0000000100003bf4) at cc1_main.cpp:304:15 [opt]
    frame #<!-- -->20: 0x0000000100005b14 clang`ExecuteCC1Tool(ArgV=0x000000016fdfddc0, ToolContext=0x000000016fdfe658, VFS=&lt;unavailable&gt;) at driver.cpp:226:12 [opt]
    frame #<!-- -->21: 0x0000000100005230 clang`clang_main(Argc=&lt;unavailable&gt;, Argv=&lt;unavailable&gt;, ToolContext=0x000000016fdfe658) at driver.cpp:274:12 [opt]
    frame #<!-- -->22: 0x0000000100010a94 clang`main(argc=&lt;unavailable&gt;, argv=&lt;unavailable&gt;) at clang-driver.cpp:17:10 [opt]
    frame #<!-- -->23: 0x000000019bea6b98 dyld`start + 6076
```
</details>


---

