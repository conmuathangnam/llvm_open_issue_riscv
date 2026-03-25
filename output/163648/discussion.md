# [HLSL] Assert when using `out` parameter modifier on a template function

**Author:** hekota
**URL:** https://github.com/llvm/llvm-project/issues/163648
**Status:** Closed
**Labels:** clang:frontend, crash, HLSL
**Closed Date:** 2025-10-17T03:31:42Z

## Body

The code below hits an assert in `SemaHLSL::getInoutParameterType` when using `out` parameter modifier on a template function on an argument that is not instantiation-dependent. This issue is blocking https://github.com/llvm/llvm-project/pull/161929.

https://godbolt.org/z/bGMda7E1K

```
RWStructuredBuffer<int> Out : register(u0, space1);

template <typename T> void foo(out int i, T x) {
    i = 10 * x;
}

[numthreads(4,1,1)]
void main(uint GI : SV_GroupIndex) {
    int a;
    float f = 6;
    foo(a, f);
    Out[0] = a;
}
```

Assert:
```
clang: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaHLSL.cpp:3685: clang::QualType clang::SemaHLSL::getInoutParameterType(clang::QualType): Assertion `!Ty->isReferenceType() && "Pointer and reference types cannot be inout or out parameters"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T cs_6_6 <source>
1.	<source>:11:13: current parser token ')'
2.	<source>:8:36: parsing function body 'main'
3.	<source>:8:36: in compound statement ('{}')
 #0 0x00000000041f8598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f8598)
 #1 0x00000000041f59c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f59c4)
 #2 0x0000000004139df8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073be5d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073be5d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073be5d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073be5d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073be5d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073be5d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f6fa5d clang::SemaHLSL::getInoutParameterType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f6fa5d)
#10 0x00000000074d1525 clang::Sema::InstantiateAttrs(clang::MultiLevelTemplateArgumentList const&, clang::Decl const*, clang::Decl*, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74d1525)
#11 0x000000000743dda3 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x743dda3)
#12 0x000000000744e127 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007461184 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7461184)
#14 0x00000000074756df clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74756df)
#15 0x00000000074d61cd clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74d61cd)
#16 0x00000000074dabf4 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#17 0x00000000081e0911 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x81e0911)
#18 0x000000000747549a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x747549a)
#19 0x00000000073b41f7 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b41f7)
#20 0x00000000073b559a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#21 0x00000000081e0911 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x81e0911)
#22 0x000000000739457b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x739457b)
#23 0x00000000071e508b AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#24 0x00000000071e5b7f clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71e5b7f)
#25 0x00000000071e5d53 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71e5d53)
#26 0x00000000071ff07b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71ff07b)
#27 0x0000000006e685ba clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6e685ba)
...
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Helena Kotas (hekota)

<details>
The code below hits an assert in `SemaHLSL::getInoutParameterType` when using `out` parameter modifier on a template function. This issue is blocking https://github.com/llvm/llvm-project/pull/161929.

https://godbolt.org/z/bGMda7E1K

```
RWStructuredBuffer&lt;int&gt; Out : register(u0, space1);

template &lt;typename T&gt; void foo(out int i, T x) {
    i = 10 * x;
}

[numthreads(4,1,1)]
void main(uint GI : SV_GroupIndex) {
    int a;
    float f = 6;
    foo(a, f);
    Out[0] = a;
}
```

Assert:
```
clang: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaHLSL.cpp:3685: clang::QualType clang::SemaHLSL::getInoutParameterType(clang::QualType): Assertion `!Ty-&gt;isReferenceType() &amp;&amp; "Pointer and reference types cannot be inout or out parameters"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T cs_6_6 &lt;source&gt;
1.	&lt;source&gt;:11:13: current parser token ')'
2.	&lt;source&gt;:8:36: parsing function body 'main'
3.	&lt;source&gt;:8:36: in compound statement ('{}')
 #<!-- -->0 0x00000000041f8598 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f8598)
 #<!-- -->1 0x00000000041f59c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f59c4)
 #<!-- -->2 0x0000000004139df8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073be5d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073be5d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073be5d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073be5d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073be5d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073be5d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f6fa5d clang::SemaHLSL::getInoutParameterType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f6fa5d)
#<!-- -->10 0x00000000074d1525 clang::Sema::InstantiateAttrs(clang::MultiLevelTemplateArgumentList const&amp;, clang::Decl const*, clang::Decl*, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74d1525)
#<!-- -->11 0x000000000743dda3 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x743dda3)
#<!-- -->12 0x000000000744e127 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;*, clang::Sema::ExtParameterInfoBuilder&amp;, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007461184 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7461184)
#<!-- -->14 0x00000000074756df clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74756df)
#<!-- -->15 0x00000000074d61cd clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74d61cd)
#<!-- -->16 0x00000000074dabf4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->17 0x00000000081e0911 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x81e0911)
#<!-- -->18 0x000000000747549a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x747549a)
#<!-- -->19 0x00000000073b41f7 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b41f7)
#<!-- -->20 0x00000000073b559a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->21 0x00000000081e0911 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x81e0911)
#<!-- -->22 0x000000000739457b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x739457b)
#<!-- -->23 0x00000000071e508b AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->24 0x00000000071e5b7f clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71e5b7f)
#<!-- -->25 0x00000000071e5d53 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71e5d53)
#<!-- -->26 0x00000000071ff07b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71ff07b)
#<!-- -->27 0x0000000006e685ba clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6e685ba)
...
```

</details>


---

