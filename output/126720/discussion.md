# [Clang] Frontend crash compiling struct/class method with explicit object parameters

**Author:** R-Goc
**URL:** https://github.com/llvm/llvm-project/issues/126720
**Status:** Closed
**Labels:** clang:frontend, regression, c++23, crash-on-valid
**Closed Date:** 2025-02-13T23:32:13Z

## Body

Frontend crash regression when compiling this code. Required flags: -std=c++23/26 (for explicit object parameters)
Works on 19.1.0 even with assertions, crashes on master(both with assertions and without)
```cpp
void operator*(auto) {}

template <typename T>
struct Quaternionz {
    void inverse(this Quaternionz<T>& a) {
        a.inverse() * 0;
    }
};

int main() {}
```
Quaternionz can be a class as well, and inverse can just accept ``` (this Quaternionz& a)``` and still crash.  Function/object names can be shortened. 
Looks like it is an issue with parsing explicit object parameters.

Godbolt link: https://clang.godbolt.org/z/1cYvc66j4
[Full godbolt link](https://clang.godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxBJcpAAOqAqETgwe3r56icmOAkEh4SxRMVxxdpgOqUIETMQE6T5%2BZZj2uQzVtQT5YZHRsbY1dQ2ZZYNdwT1FfaUAlLaoXsTI7BwAbqh46ADUqPHRTETEAFQQTF5EM1smAOxW1wAiJhoAgk/PBJgs8QYfVwDMbgIAE89sw2FsAComP7YN4KAjELwOLYARS8B2iDFSAC8rrc3ltCVt1pstsFVtElBACAg8ApUeiPsQsQJsdC3FCYeYAGxbJiXG5WF5EkV8gB05MpmAglyOWw00KFzxFN0eL1Vireb2CBC2LCYwRleLuaueHDmtE4AFZeH4OFpSKhOG5rNYtgoFktMFczH8eKQCJpzXMANYgP5WsVmSR/ACcAA4/kmuFaY9c/vpOJJbUHHZxeAoQBoA0G5nBYDBEChUF86NFyJQ0LX6DFkAYjFxY1wmAolHU%2BHQmYWIBFcxFgrUgZx/ePmMQgQB5CLaCqB7i8JtsQQLhi0Kf23hYCJeYBuMS0Qvr0hYfVGcQH694YirvAUy8OzCqCrnFYOnUtXNaDwCJiEnDwsFzBE8BYadeApYgIiSTB7k%2BQxgCAoxSz4AxgAUAA1PBMAAdwXUFYJkQQRDEdgpAo%2BQlDUXNdDidtjFdSx9GAwtIDmXY2kvABaeF0Ghe5TEsaxfV4VB4OITZMG4mVbBaF9nAgVxhiaUhAgmQpiiyJIUgETSDJyVJuj06ZlNaKoxhM5obIEDo6gs3oSgGTp7I8lzdLciQ5g9RZln8zMOBtUg7QdJ0OC2VR425ATuUkLY2zQrZOzFLgxQ0LZTl7aJ%2BMuCBcEIEgfT9GZeDXLQZlDcMsvjaN03jVNEw0a4uDMULs1IGCrWLSLpPzWwixLA8y0rCAkAWAh4nOBsICbeI62IUJWBWOKEqSlLWNyhEvAYENKu0/BDk2PR%2BEo0RxC4bk6MUFR1AfZjSCI0D4lgi1rRzB9ooXc45t1VAqFi%2BLEuS1KjHS2NMuy3Kez7Qrco8ZtonKrhjuq4NSAQTAmCwGIlMtDger6gbc2igtRqx2rSDDP4zCjDQuHjeNrmjSQrW5P5uWuUK/h%2BqLhpp0hyygSakCWlaFqllsUFYgB9BGCtSBQFf2w6B1oIdKFHB9Z0nciDfnJcVwccjN0YAgdz3XMjxPM9aAvcibzQ%2B8HXwZ9KjfXNP2/D5yP/YmHSAkCwIwX8qrkmCr3gxClBQ290OCUBxuwphcII4jSMYcjLuEa6aOkfOHsY56QC61iUHYmxQ8U3j4n4zghIIES/jEmupMdWT5Pr6zVL8dSGHcTxGn8YfXKmdzsiMtJR5GBJDLaSf9IcgfnPqeetPKSonLGFerPhTyt70I%2BfIKPyMfmIKaK%2BsLBaGmLNvBna0ryxHVa2DWjtykrDnRzGpY6oMzFFaVMnVYySF9PGWMZg7rE1JiAfqEUKbDULMWEWYspogBmoDGWNZlotjWmwTgz9tqQ2AHtREh1jqYFOiQc6cR85URusXWQpcnoOl0F1N6TAPrrjvuFQaeYOD/VmucHYIMyEQ12u/FWAh6TfyKijQhaNzAM0AeNOYuN8Z9CJlmXgZMUG/TQdTIBdNwyMzAZICBUC/gwLgfzB%2BIiqrmOJmYZxlMxo1TmPBZIzhJBAA)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (R-Goc)

<details>
Frontend crash regression when compiling this code. Required flags: -std=c++23/26 (for explicit object parameters)
Works on 19.1.0 even with assertions, crashes on master(both with assertions and without)
```cpp
void operator*(auto) {}

template &lt;typename T&gt;
struct Quaternionz {
    void inverse(this Quaternionz&lt;T&gt;&amp; a) {
        a.inverse() * 0;
    }
};

int main() {}
```
Quaternionz can be a class as well, and inverse can just accept ``` (this Quaternionz&amp; a)``` and still crash.  Function/object names can be shortened. 
Looks like it is an issue with parsing explicit object parameters.

Godbolt link: https://clang.godbolt.org/z/1cYvc66j4
[Full godbolt link](https://clang.godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxBJcpAAOqAqETgwe3r56icmOAkEh4SxRMVxxdpgOqUIETMQE6T5%2BZZj2uQzVtQT5YZHRsbY1dQ2ZZYNdwT1FfaUAlLaoXsTI7BwAbqh46ADUqPHRTETEAFQQTF5EM1smAOxW1wAiJhoAgk/PBJgs8QYfVwDMbgIAE89sw2FsAComP7YN4KAjELwOLYARS8B2iDFSAC8rrc3ltCVt1pstsFVtElBACAg8ApUeiPsQsQJsdC3FCYeYAGxbJiXG5WF5EkV8gB05MpmAglyOWw00KFzxFN0eL1Vireb2CBC2LCYwRleLuaueHDmtE4AFZeH4OFpSKhOG5rNYtgoFktMFczH8eKQCJpzXMANYgP5WsVmSR/ACcAA4/kmuFaY9c/vpOJJbUHHZxeAoQBoA0G5nBYDBEChUF86NFyJQ0LX6DFkAYjFxY1wmAolHU%2BHQmYWIBFcxFgrUgZx/ePmMQgQB5CLaCqB7i8JtsQQLhi0Kf23hYCJeYBuMS0Qvr0hYfVGcQH694YirvAUy8OzCqCrnFYOnUtXNaDwCJiEnDwsFzBE8BYadeApYgIiSTB7k%2BQxgCAoxSz4AxgAUAA1PBMAAdwXUFYJkQQRDEdgpAo%2BQlDUXNdDidtjFdSx9GAwtIDmXY2kvABaeF0Ghe5TEsaxfV4VB4OITZMG4mVbBaF9nAgVxhiaUhAgmQpiiyJIUgETSDJyVJuj06ZlNaKoxhM5obIEDo6gs3oSgGTp7I8lzdLciQ5g9RZln8zMOBtUg7QdJ0OC2VR425ATuUkLY2zQrZOzFLgxQ0LZTl7aJ%2BMuCBcEIEgfT9GZeDXLQZlDcMsvjaN03jVNEw0a4uDMULs1IGCrWLSLpPzWwixLA8y0rCAkAWAh4nOBsICbeI62IUJWBWOKEqSlLWNyhEvAYENKu0/BDk2PR%2BEo0RxC4bk6MUFR1AfZjSCI0D4lgi1rRzB9ooXc45t1VAqFi%2BLEuS1KjHS2NMuy3Kez7Qrco8ZtonKrhjuq4NSAQTAmCwGIlMtDger6gbc2igtRqx2rSDDP4zCjDQuHjeNrmjSQrW5P5uWuUK/h%2BqLhpp0hyygSakCWlaFqllsUFYgB9BGCtSBQFf2w6B1oIdKFHB9Z0nciDfnJcVwccjN0YAgdz3XMjxPM9aAvcibzQ%2B8HXwZ9KjfXNP2/D5yP/YmHSAkCwIwX8qrkmCr3gxClBQ290OCUBxuwphcII4jSMYcjLuEa6aOkfOHsY56QC61iUHYmxQ8U3j4n4zghIIES/jEmupMdWT5Pr6zVL8dSGHcTxGn8YfXKmdzsiMtJR5GBJDLaSf9IcgfnPqeetPKSonLGFerPhTyt70I%2BfIKPyMfmIKaK%2BsLBaGmLNvBna0ryxHVa2DWjtykrDnRzGpY6oMzFFaVMnVYySF9PGWMZg7rE1JiAfqEUKbDULMWEWYspogBmoDGWNZlotjWmwTgz9tqQ2AHtREh1jqYFOiQc6cR85URusXWQpcnoOl0F1N6TAPrrjvuFQaeYOD/VmucHYIMyEQ12u/FWAh6TfyKijQhaNzAM0AeNOYuN8Z9CJlmXgZMUG/TQdTIBdNwyMzAZICBUC/gwLgfzB%2BIiqrmOJmYZxlMxo1TmPBZIzhJBAA)
</details>


---

### Comment 2 - R-Goc

This is also a regression. 19.1.0 doesn't crash.

---

### Comment 3 - zyn0217

The crash stackstace suggests https://github.com/llvm/llvm-project/pull/117841 might be related
@HighCommander4 

---

### Comment 4 - R-Goc

Checked out 2b855dd97092e2178ac5c470a804a17ec440d7e5 (the one before #117841), built a debug build and it compiles

---

### Comment 5 - R-Goc

On 9ccde12f5eeb (#117841) it fails to compile. log:
```
Assertion failed: getNumArgs() > 0 && getArg(0), file D:\lib\llvm\clang\lib\AST\Expr.cpp, line 1645
1.      main.cpp:4:57: current parser token ';'
2.      main.cpp:3:23: parsing struct/union/class body 'Quaternionz'
3.      main.cpp:4:40: parsing function body 'Quaternionz::inverse'
4.      main.cpp:4:40: in compound statement ('{}')
Exception Code: 0xC000001D
 #0 0x00007ff60b9d1e2c HandleAbort D:\lib\llvm\llvm\lib\Support\Windows\Signals.inc:429:0
 #1 0x00007ffba31b9ea7 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x99ea7)
 #2 0x00007ffba31bbae9 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x9bae9)
 #3 0x00007ffba31c1745 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa1745)
 #4 0x00007ffba31c13b7 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa13b7)
 #5 0x00007ffba31bf381 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x9f381)
 #6 0x00007ffba31c1caf (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa1caf)
 #7 0x00007ff60d2a7954 clang::CallExpr::getBeginLoc(void) const D:\lib\llvm\clang\lib\AST\Expr.cpp:1646:0
 #8 0x00007ff60d27c00d clang::Stmt::getBeginLoc(void) const D:\lib\llvm\build\tools\clang\include\clang\AST\StmtNodes.inc:614:0
 #9 0x00007ff60d25f1a1 clang::CXXOperatorCallExpr::getSourceRangeImpl(void) const D:\lib\llvm\clang\lib\AST\ExprCXX.cpp:656:0
#10 0x00007ff60d25eef2 clang::CXXOperatorCallExpr::CXXOperatorCallExpr(enum clang::OverloadedOperatorKind, class clang::Expr *, class llvm::ArrayRef<class clang::Expr *>, class clang::QualType, enum clang::ExprValueKind, class clang::SourceLocation, class clang::FPOptionsOverride, enum clang::CallExpr::ADLCallKind) D:\lib\llvm\clang\lib\AST\ExprCXX.cpp:602:0
#11 0x00007ff60d25f39f clang::CXXOperatorCallExpr::Create(class clang::ASTContext const &, enum clang::OverloadedOperatorKind, class clang::Expr *, class llvm::ArrayRef<class clang::Expr *>, class clang::QualType, enum clang::ExprValueKind, class clang::SourceLocation, class clang::FPOptionsOverride, enum clang::CallExpr::ADLCallKind) D:\lib\llvm\clang\lib\AST\ExprCXX.cpp:622:0
#12 0x00007ff6102ced6c clang::Sema::CreateOverloadedBinOp(class clang::SourceLocation, enum clang::BinaryOperatorKind, class clang::UnresolvedSetImpl const &, class clang::Expr *, class clang::Expr *, bool, bool, class clang::FunctionDecl *) D:\lib\llvm\clang\lib\Sema\SemaOverload.cpp:14583:0
#13 0x00007ff61035aa16 BuildOverloadedBinOp D:\lib\llvm\clang\lib\Sema\SemaExpr.cpp:15286:0
#14 0x00007ff61035a42f clang::Sema::BuildBinOp(class clang::Scope *, class clang::SourceLocation, enum clang::BinaryOperatorKind, class clang::Expr *, class clang::Expr *) D:\lib\llvm\clang\lib\Sema\SemaExpr.cpp:15384:0
#15 0x00007ff6103115d5 clang::Sema::ActOnBinOp(class clang::Scope *, class clang::SourceLocation, enum clang::tok::TokenKind, class clang::Expr *, class clang::Expr *) D:\lib\llvm\clang\lib\Sema\SemaExpr.cpp:15236:0
#16 0x00007ff6142a8757 clang::Parser::ParseRHSOfBinaryExpression(class clang::ActionResult<class clang::Expr *, 1>, enum clang::prec::Level) D:\lib\llvm\clang\lib\Parse\ParseExpr.cpp:660:0
#17 0x00007ff6142a7228 clang::Parser::ParseAssignmentExpression(enum clang::Parser::TypeCastState) D:\lib\llvm\clang\lib\Parse\ParseExpr.cpp:188:0
#18 0x00007ff6142a7048 clang::Parser::ParseExpression(enum clang::Parser::TypeCastState) D:\lib\llvm\clang\lib\Parse\ParseExpr.cpp:135:0
#19 0x00007ff6142953f6 clang::Parser::ParseExprStatement(enum clang::Parser::ParsedStmtContext) D:\lib\llvm\clang\lib\Parse\ParseStmt.cpp:565:0
#20 0x00007ff61429378b clang::Parser::ParseStatementOrDeclarationAfterAttributes(class llvm::SmallVector<class clang::Stmt *, 32> &, enum clang::Parser::ParsedStmtContext, class clang::SourceLocation *, class clang::ParsedAttributes &, class clang::ParsedAttributes &) D:\lib\llvm\clang\lib\Parse\ParseStmt.cpp:293:0
#21 0x00007ff614292de8 clang::Parser::ParseStatementOrDeclaration(class llvm::SmallVector<class clang::Stmt *, 32> &, enum clang::Parser::ParsedStmtContext, class clang::SourceLocation *) D:\lib\llvm\clang\lib\Parse\ParseStmt.cpp:125:0
#22 0x00007ff61429d0e5 clang::Parser::ParseCompoundStatementBody(bool) D:\lib\llvm\clang\lib\Parse\ParseStmt.cpp:1267:0
#23 0x00007ff61429ec15 clang::Parser::ParseFunctionStatementBody(class clang::Decl *, class clang::Parser::ParseScope &) D:\lib\llvm\clang\lib\Parse\ParseStmt.cpp:2577:0
#24 0x00007ff6142a5405 clang::Parser::ParseLexedMethodDef(struct clang::Parser::LexedMethod &) D:\lib\llvm\clang\lib\Parse\ParseCXXInlineMethods.cpp:662:0
#25 0x00007ff6142a4f37 clang::Parser::LexedMethod::ParseLexedMethodDefs(void) D:\lib\llvm\clang\lib\Parse\ParseCXXInlineMethods.cpp:321:0
#26 0x00007ff6142a39f3 clang::Parser::ParseLexedMethodDefs(struct clang::Parser::ParsingClass &) D:\lib\llvm\clang\lib\Parse\ParseCXXInlineMethods.cpp:592:0
#27 0x00007ff6141e629a clang::Parser::ParseCXXMemberSpecification(class clang::SourceLocation, class clang::SourceLocation, class clang::ParsedAttributes &, unsigned int, class clang::Decl *) D:\lib\llvm\clang\lib\Parse\ParseDeclCXX.cpp:3993:0
#28 0x00007ff6141e3d29 clang::Parser::ParseClassSpecifier(enum clang::tok::TokenKind, class clang::SourceLocation, class clang::DeclSpec &, struct clang::Parser::ParsedTemplateInfo &, enum clang::AccessSpecifier, bool, enum clang::Parser::DeclSpecContext, class clang::ParsedAttributes &) D:\lib\llvm\clang\lib\Parse\ParseDeclCXX.cpp:2354:0
#29 0x00007ff614228667 clang::Parser::ParseDeclarationSpecifiers(class clang::DeclSpec &, struct clang::Parser::ParsedTemplateInfo &, enum clang::AccessSpecifier, enum clang::Parser::DeclSpecContext, class clang::Parser::LateParsedAttrList *, enum clang::ImplicitTypenameContext) D:\lib\llvm\clang\lib\Parse\ParseDecl.cpp:4697:0
#30 0x00007ff612edd5e4 clang::Parser::ParseDeclarationSpecifiers(class clang::DeclSpec &, struct clang::Parser::ParsedTemplateInfo &, enum clang::AccessSpecifier, enum clang::Parser::DeclSpecContext, class clang::Parser::LateParsedAttrList *) D:\lib\llvm\clang\include\clang\Parse\Parser.h:2515:0
#31 0x00007ff6141ceb54 clang::Parser::ParseDeclarationAfterTemplate(enum clang::DeclaratorContext, struct clang::Parser::ParsedTemplateInfo &, class clang::ParsingDeclRAIIObject &, class clang::SourceLocation &, class clang::ParsedAttributes &, enum clang::AccessSpecifier) D:\lib\llvm\clang\lib\Parse\ParseTemplate.cpp:235:0
#32 0x00007ff6141cdc65 clang::Parser::ParseTemplateDeclarationOrSpecialization(enum clang::DeclaratorContext, class clang::SourceLocation &, class clang::ParsedAttributes &, enum clang::AccessSpecifier) D:\lib\llvm\clang\lib\Parse\ParseTemplate.cpp:178:0
#33 0x00007ff6141cd5a7 clang::Parser::ParseDeclarationStartingWithTemplate(enum clang::DeclaratorContext, class clang::SourceLocation &, class clang::ParsedAttributes &) D:\lib\llvm\clang\lib\Parse\ParseTemplate.cpp:50:0
#34 0x00007ff61421e1a0 clang::Parser::ParseDeclaration(enum clang::DeclaratorContext, class clang::SourceLocation &, class clang::ParsedAttributes &, class clang::ParsedAttributes &, class clang::SourceLocation *) D:\lib\llvm\clang\lib\Parse\ParseDecl.cpp:2057:0
#35 0x00007ff612ed4116 clang::Parser::ParseExternalDeclaration(class clang::ParsedAttributes &, class clang::ParsedAttributes &, class clang::ParsingDeclSpec *) D:\lib\llvm\clang\lib\Parse\Parser.cpp:985:0
#36 0x00007ff612ed1f9f clang::Parser::ParseTopLevelDecl(class clang::OpaquePtr<class clang::DeclGroupRef> &, enum clang::Sema::ModuleImportState &) D:\lib\llvm\clang\lib\Parse\Parser.cpp:758:0
#37 0x00007ff610645e91 clang::ParseAST(class clang::Sema &, bool, bool) D:\lib\llvm\clang\lib\Parse\ParseAST.cpp:171:0
#38 0x00007ff60d7acbbd clang::ASTFrontendAction::ExecuteAction(void) D:\lib\llvm\clang\lib\Frontend\FrontendAction.cpp:1193:0
#39 0x00007ff60da1b93b clang::CodeGenAction::ExecuteAction(void) D:\lib\llvm\clang\lib\CodeGen\CodeGenAction.cpp:1140:0
#40 0x00007ff60d7ac4e4 clang::FrontendAction::Execute(void) D:\lib\llvm\clang\lib\Frontend\FrontendAction.cpp:1081:0
#41 0x00007ff60bc83ae2 clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) D:\lib\llvm\clang\lib\Frontend\CompilerInstance.cpp:1062:0
#42 0x00007ff60bda427f clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) D:\lib\llvm\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:296:0
#43 0x00007ff60b1d1945 cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) D:\lib\llvm\clang\tools\driver\cc1_main.cpp:286:0
#44 0x00007ff60b1c2d48 ExecuteCC1Tool D:\lib\llvm\clang\tools\driver\driver.cpp:218:0
#45 0x00007ff60b1c150e clang_main(int, char **, struct llvm::ToolContext const &) D:\lib\llvm\clang\tools\driver\driver.cpp:259:0
#46 0x00007ff60b1faa51 main D:\lib\llvm\build\tools\clang\tools\driver\clang-driver.cpp:17:0
#47 0x00007ff614afb4e9 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
#48 0x00007ff614afb622 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#49 0x00007ff614afb6ae __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
#50 0x00007ff614afb6ce mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
#51 0x00007ffc14f2e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#52 0x00007ffc156dbf6c (C:\WINDOWS\SYSTEM32\ntdll.dll+0xbbf6c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git 9ccde12f5eeb91152900082a2ae839e2a9702b31)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\bin\LLVM-DBG2\bin
Build config: +unoptimized, +assertions
```
So this can be considered confirmed and bisected (though the "bisect" was two commits. Thanks @zyn0217)

---

### Comment 6 - R-Goc

This reverts cleanly.

---

### Comment 7 - HighCommander4

Thanks for the report! I will investigate.

---

### Comment 8 - shafik

Folks, we have regression labels, so please use them when you identify a regression.

---

### Comment 9 - R-Goc

I don't have triage rights, not sure if I should apply for them or not.

---

### Comment 10 - shafik

> I don't have triage rights, not sure if I should apply for them or not.

Apologies, I was aiming more for @zyn0217 and @HighCommander4 

---

### Comment 11 - HighCommander4

The logic added in https://github.com/llvm/llvm-project/pull/117841 assumed that a call to a method with an explicit object parameter was modelled as a `CallExpr` with the explicit object parameter being the first argument.

It looks like this is the case for **non-dependent** calls, but for the dependent calls, the modelling is different (the explicit object parameter is represented as part of the callee expression).

Some AST dumps to illustrate the difference follow.

---

Non-dependent case:

```c++
struct Foo {
  void bar(this Foo&);
};
void f() {
  Foo f;
  f.bar();
}
```

```
    `-CallExpr 0x55b06432ca78 <line:6:5, col:9> 'void'
      |-ImplicitCastExpr 0x55b06432ca60 <col:5> 'void (*)(Foo &)' <FunctionToPointerDecay>
      | `-DeclRefExpr 0x55b06432c9e0 <col:5> 'void (Foo &)' lvalue CXXMethod 0x55b06430a768 'bar' 'void (Foo &)'
      `-DeclRefExpr 0x55b06430b148 <col:3> 'Foo' lvalue Var 0x55b06430a9d0 'f' 'Foo'
```

Here, the explicit object parameter is the first argument of the `CallExpr` (the second child expr in the dump, after the callee).

---

Dependent case:

```c++
template <typename T>
struct Foo {
  void bar(this Foo&);
};
template <typename T>
void f() {
  Foo<T> f;
  f.bar();
}
```

```
      `-CallExpr 0x55ad328253b8 <line:8:3, col:9> '<dependent type>'
        `-CXXDependentScopeMemberExpr 0x55ad32825370 <col:3, col:5> '<dependent type>' lvalue .bar
          `-DeclRefExpr 0x55ad32825338 <col:3> 'Foo<T>' lvalue Var 0x55ad328252b8 'f' 'Foo<T>'
```

Here, the explicit object parameter is part of the callee expression, and there are no arguments.

Note, in this case we get a `CXXDependentScopeMemberExpr` which `getCalleeDecl()` does not see through, but there are dependent cases where we can get a plain `MemberExpr` as well:

```c++
template <typename T>
struct Foo {
  void bar(this Foo&);
  void f(Foo other) {
    other.bar();
  }
};
```

```
        `-CallExpr 0x55ccde20f098 <line:5:5, col:15> '<dependent type>'
          `-MemberExpr 0x55ccde20f068 <col:5, col:11> '<bound member function type>' .bar 0x55ccde20ed58
            `-DeclRefExpr 0x55ccde20f048 <col:5> 'Foo<T>' lvalue ParmVar 0x55ccde20ee38 'other' 'Foo<T>'
```

---

In the original testcase, the role of the `void operator*(auto) {}`, and the `*` expression which involves the affected dependent `CallExpr`, is to trigger a call to `CallExpr::getBeginLoc()` on it.

---

### Comment 12 - HighCommander4

Proposed fix: https://github.com/llvm/llvm-project/pull/126868

---

### Comment 13 - HighCommander4

Fixed on trunk. I filed an issue for backporting to 20.x at https://github.com/llvm/llvm-project/issues/127146.

---

