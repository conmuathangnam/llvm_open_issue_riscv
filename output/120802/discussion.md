# Clang crashes when using lambda captures inside a trailing return decltype

**Author:** fahadnayyar
**URL:** https://github.com/llvm/llvm-project/issues/120802

## Body

Clang crashed on this example:

```
template<typename T>
consteval auto matches(T t)
{
    return [](auto u) -> decltype([u](){}()){}(t);
}

int main() {
    matches(0);
    return 0;
}
```

This issue was introduced from clang16.0.0 ( released on March 17, 2023) to clang 17.0.1 (released on September 9, 2023)
https://godbolt.org/z/q9v9q31f4

Gcc works fine for this example: https://godbolt.org/z/x8z41EKr4 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (fahadnayyar)

<details>
Clang crashed on this example:

```
template&lt;typename T&gt;
consteval auto matches(T t)
{
    return [](auto u) -&gt; decltype([u](){}()){}(t);
}

int main() {
    matches(0);
    return 0;
}
```

This issue was introduced from clang16.0.0 ( released on March 17, 2023) to clang 17.0.1 (released on September 9, 2023)
https://godbolt.org/z/q9v9q31f4

Gcc works fine for this example: https://godbolt.org/z/x8z41EKr4 
</details>


---

### Comment 2 - fahadnayyar

@shafik can you please help me triage this issue? I can try to find the faulty commit between clang16.0.0 -> clang 17.0.1 in a few days. 

---

### Comment 3 - tbaederr

```
    #5: TypeNodes.inc:79  clang::ASTContext::getTypeInfoImpl(this=0x00007e9f75de0200, T=0x00007e0f75e806b0) const
    #6: ASTContext.cpp:1959  clang::ASTContext::getTypeInfo(this=0x00007e9f75de0200, T=0x00007e0f75e806b0) const
    #7: ASTContext.cpp:1873  clang::ASTContext::getTypeInfoInChars(this=0x00007e9f75de0200, T=0x00007e0f75e806b0) const
    #8: ASTContext.cpp:1879  clang::ASTContext::getTypeInfoInChars(this=0x00007e9f75de0200, T=QualType @ 0x00007bff74986b20) const
    #9: RecordLayoutBuilder.cpp:1908  (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool)::$_0::operator()(this=0x00007bff74edd170, IsIncompleteArrayType=false) const
    #10: RecordLayoutBuilder.cpp:1920  (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(this=0x00007bff74fe5560, D=0x00007e0f75e80d00, InsertExtraPadding=false)
    #11: RecordLayoutBuilder.cpp:1460  (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(this=0x00007bff74fe5560, D=0x00007e0f75e80808)
    #12: RecordLayoutBuilder.cpp:1395  (anonymous namespace)::ItaniumRecordLayoutBuilder::Layout(this=0x00007bff74fe5560, RD=0x00007e0f75e80808)
    #13: RecordLayoutBuilder.cpp:3384  clang::ASTContext::getASTRecordLayout(this=0x00007e9f75de0200, D=0x00007e0f75e80808) const
    #14: ASTContext.cpp:2399  clang::ASTContext::getTypeInfoImpl(this=0x00007e9f75de0200, T=0x00007e0f75e80c50) const
    #15: ASTContext.cpp:1959  clang::ASTContext::getTypeInfo(this=0x00007e9f75de0200, T=0x00007e0f75e80c50) const
    #16: ASTContext.h:2476  clang::ASTContext::getTypeInfo(this=0x00007e9f75de0200, T=QualType @ 0x00007bff74981160) const
    #17: ASTContext.h:2513  clang::ASTContext::getTypeAlign(this=0x00007e9f75de0200, T=QualType @ 0x00007ffffffec860) const
    #18: ASTContext.cpp:2552  clang::ASTContext::getTypeAlignInChars(this=0x00007e9f75de0200, T=QualType @ 0x00007ffffffec8c0) const
    #19: SemaChecking.cpp:3215  clang::Sema::CheckArgAlignment(this=0x00007e8f75dea200, Loc=(ID = 95), FDecl=0x00007e0f75e80990, ParamName=(Data = "'this'", Length = 6), ArgTy=QualType @ 0x00007bff74c77240, ParamTy=QualType @ 0x00007bff74c77260)
    #20: SemaChecking.cpp:3517  clang::Sema::CheckFunctionCall(this=0x00007e8f75dea200, FDecl=0x00007e0f75e80990, TheCall=0x00007e0f75e80f48, Proto=0x00007e0f75e80c10)
    #21: SemaOverload.cpp:15921  clang::Sema::BuildCallToObjectOfClassType(this=0x00007e8f75dea200, S=0x0000000000000000, Obj=0x00007e0f75e80dd8, LParenLoc=(ID = 87), Args=clang::MultiExprArg @ 0x00007bff751c0050, RParenLoc=(ID = 95))
    #22: SemaExpr.cpp:6540  clang::Sema::BuildCallExpr(this=0x00007e8f75dea200, Scope=0x0000000000000000, Fn=0x00007e0f75e80dd8, LParenLoc=(ID = 87), ArgExprs=clang::MultiExprArg @ 0x00007bff74ed6840, RParenLoc=(ID = 95), ExecConfig=0x0000000000000000, IsExecConfig=false, AllowRecovery=true)
    #23: SemaExpr.cpp:6447  clang::Sema::ActOnCallExpr(this=0x00007e8f75dea200, Scope=0x0000000000000000, Fn=0x00007e0f75e80dd8, LParenLoc=(ID = 87), ArgExprs=clang::MultiExprArg @ 0x00007ffffffee448, RParenLoc=(ID = 95), ExecConfig=0x0000000000000000)
    #24: TreeTransform.h:2871  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildCallExpr(this=0x00007bff74ba1b40, Callee=0x00007e0f75e80dd8, LParenLoc=(ID = 87), Args=clang::MultiExprArg @ 0x00007ffffffee4e0, RParenLoc=(ID = 95), ExecConfig=0x0000000000000000)
    #25: TreeTransform.h:13013  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7deb0)
    #26: StmtNodes.inc:614  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7deb0)
    #27: TreeTransform.h:6725  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecltypeType(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, TL=DecltypeTypeLoc @ 0x00007bff74ba5840)
    #28: TypeNodes.inc:39  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, T=(Ty = 0x00007e0f75e7df20, Data = 0x00007e0f75e7e010))
    #29: TreeTransform.h:6352  clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, TL=FunctionProtoTypeLoc @ 0x00007bff74eca080, ThisContext=0x0000000000000000, ThisTypeQuals=(Mask = 0), TransformExceptionSpec=(unnamed class) @ 0x00007bff74eca020)
    #30: SemaTemplateInstantiate.cpp:2518  clang::QualType (anonymous namespace)::TemplateInstantiator::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, TL=FunctionProtoTypeLoc @ 0x00007ffffffefb48, ThisContext=0x0000000000000000, ThisTypeQuals=(Mask = 0), TransformExceptionSpec=(unnamed class) @ 0x00007bff74c6b220)
    #31: TreeTransform.h:6306  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, TL=FunctionProtoTypeLoc @ 0x00007bff74ba4f20)
    #32: SemaTemplateInstantiate.cpp:1575  (anonymous namespace)::TemplateInstantiator::TransformFunctionProtoType(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, TL=FunctionProtoTypeLoc @ 0x00007ffffffefcf8)
    #33: TypeNodes.inc:52  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(this=0x00007bff74ba1b40, TLB=0x00007bff750e6690, T=(Ty = 0x00007e0f75e7dfb0, Data = 0x00007e0f75e7dff8))
    #34: TreeTransform.h:15277  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7e600)
    #35: SemaTemplateInstantiate.cpp:1703  (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7e600)
    #36: StmtNodes.inc:346  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7e600)
    #37: TreeTransform.h:12985  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7e658)
    #38: StmtNodes.inc:614  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(this=0x00007bff74ba1b40, E=0x00007e0f75e7e658)
    #39: TreeTransform.h:4306  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(this=0x00007bff74ba1b40, Init=0x00007e0f75e7e658, NotCopyInit=false)
    #40: TreeTransform.h:8433  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(this=0x00007bff74ba1b40, S=0x00007e0f75e7e688)
    #41: StmtNodes.inc:938  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(this=0x00007bff74ba1b40, S=0x00007e0f75e7e688, SDK=SDK_Discarded)
    #42: TreeTransform.h:7992  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(this=0x00007bff74ba1b40, S=0x00007e0f75e7e6a0, IsStmtExpr=false)
    #43: TreeTransform.h:7974  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(this=0x00007bff74ba1b40, S=0x00007e0f75e7e6a0)
    #44: StmtNodes.inc:1604  clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(this=0x00007bff74ba1b40, S=0x00007e0f75e7e6a0, SDK=SDK_Discarded)
    #45: SemaTemplateInstantiate.cpp:4475  clang::Sema::SubstStmt(this=0x00007e8f75dea200, S=0x00007e0f75e7e6a0, TemplateArgs=0x00007bff74fdb7d0)
    #46: SemaTemplateInstantiateDecl.cpp:5270  clang::Sema::InstantiateFunctionDefinition(this=0x00007e8f75dea200, PointOfInstantiation=(ID = 124), Function=0x00007e0f75e802b0, Recursive=false, DefinitionRequired=false, AtEndOfTU=false)
    #47: SemaTemplateDeduction.cpp:5438  clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool)::$_1::operator()(this=0x00007bff74d669f0) const
    #48: STLFunctionalExtras.h:46  void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool)::$_1>(callable=136337107085808)
    #49: STLFunctionalExtras.h:69  llvm::function_ref<void ()>::operator()(this=0x00007bff749b6360) const
    #50: Stack.h:46  clang::runWithSufficientStackSpace(Diag=function_ref<void ()> @ 0x00007fffffff3818, Fn=function_ref<void ()> @ 0x00007bff749b6360)
    #51: StackExhaustionHandler.cpp:20  clang::StackExhaustionHandler::runWithSufficientStackSpace(this=0x00007e8f75dea468, Loc=(ID = 124), Fn=function_ref<void ()> @ 0x00007fffffff3918)
```

---

### Comment 4 - shafik

I would just end up doing a git bisect for a regression against a recent release. So you can do the same thing, let me know if you start the bisect.

---

### Comment 5 - cor3ntin

@mizvekov 

---

