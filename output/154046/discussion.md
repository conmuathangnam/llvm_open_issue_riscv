# [Crash] error: incompatible pointer to integer conversion initializing 'const char' with an expression of type 'char[8]'

**Author:** 8ss-boop
**URL:** https://github.com/llvm/llvm-project/issues/154046
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:14
**Closed Date:** 2025-08-18T14:22:32Z

## Body

Compiler Explorer: https://godbolt.org/z/MrYdbWe1z

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
_Noreturn void handle_error(int code)
{
    const char *msg = (const char[]) {
        [0] = "Success",
        [1] = "General error",
        [2] = "Invalid input"
    }[code];
    fprintf(stderr, "Error: %s (%d)\n", msg ? msg : "Unknown", code);
    exit(code);
}
int main(void)
{
    volatile int state = 0;
    int result = 0;
    for (int i = 0; i < 3; ++i) {
        if (signal(SIGINT, SIG_IGN) == SIG_ERR)
            handle_error(2);
        for (int j = i + 1; j < 5; ++j) {
            state ^= j * (i + 1);
            if (state % 3 == 0) {
                result += abs(state - j);
                continue;
            }
            else if (state % 2 == 0 && result > 0)
                result /= 2;
            else
                result += state % 4;
        }
    }
    printf("Final result: %d\n", result);
    return result > 10 ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

Crash output:
```
test.c:7:15: error: incompatible pointer to integer conversion initializing 'const char' with an expression of type
      'char[8]' [-Wint-conversion]
    7 |         [0] = "Success",
      |               ^~~~~~~~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=143 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-73502f.o -x c test.c
1.      test.c:10:6: current parser token '['
2.      test.c:5:1: parsing function body 'handle_error'
3.      test.c:5:1: in compound statement ('{}')
 #0 0x00007f50ba39fd01 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #1 0x00007f50ba39cd27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f50b9c47520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f50b6d603e0 clang::ConstantArrayType::getSize() const (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x7b73e0)
 #4 0x00007f50b6e1e11a (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) (.constprop.0) SemaInit.cpp:0:0
 #5 0x00007f50b6e26b22 (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) SemaInit.cpp:0:0
 #6 0x00007f50b6e21592 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
 #7 0x00007f50b6e21f78 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
 #8 0x00007f50b6e22867 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
 #9 0x00007f50b6e171c6 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x86e1c6)
#10 0x00007f50b6c27497 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x67e497)
#11 0x00007f50b6c27cfe clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::Expr*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x67ecfe)
#12 0x00007f50b76d4e85 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xbae85)
#13 0x00007f50b76c8a2e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xaea2e)
#14 0x00007f50b76ca6e0 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xb06e0)
#15 0x00007f50b76ca8fa clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xb08fa)
#16 0x00007f50b767ca62 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x62a62)
#17 0x00007f50b7692cdd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x78cdd)
#18 0x00007f50b7694c2a clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7ac2a)
#19 0x00007f50b76950e3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7b0e3)
#20 0x00007f50b776d4e3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1534e3)
#21 0x00007f50b776de5a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x153e5a)
#22 0x00007f50b776fb84 clang::Parser::ParseCompoundStatementBody(bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x155b84)
#23 0x00007f50b7770613 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x156613)
#24 0x00007f50b77a8664 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x18e664)
#25 0x00007f50b769327f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7927f)
#26 0x00007f50b77a1f86 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x187f86)
#27 0x00007f50b77a2d2b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x188d2b)
#28 0x00007f50b77ac473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x192473)
#29 0x00007f50b77ad6d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1936d5)
#30 0x00007f50b765a02c clang::ParseAST(clang::Sema&, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x4002c)
#31 0x00007f50bc6ae236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#32 0x00007f50bc624a9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#33 0x00007f50be3a47a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#34 0x0000000000414d9b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#35 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#37 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#38 0x00007f50b9c2ed90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007f50b9c2ee40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-96ad0c.c
clang: note: diagnostic msg: /tmp/test-96ad0c.sh
clang: note: diagnostic msg: 

********************
```

[test.zip](https://github.com/user-attachments/files/21826380/test.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (8ss-boop)

<details>
Compiler Explorer: https://godbolt.org/z/MrYdbWe1z

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;signal.h&gt;
_Noreturn void handle_error(int code)
{
    const char *msg = (const char[]) {
        [0] = "Success",
        [1] = "General error",
        [2] = "Invalid input"
    }[code];
    fprintf(stderr, "Error: %s (%d)\n", msg ? msg : "Unknown", code);
    exit(code);
}
int main(void)
{
    volatile int state = 0;
    int result = 0;
    for (int i = 0; i &lt; 3; ++i) {
        if (signal(SIGINT, SIG_IGN) == SIG_ERR)
            handle_error(2);
        for (int j = i + 1; j &lt; 5; ++j) {
            state ^= j * (i + 1);
            if (state % 3 == 0) {
                result += abs(state - j);
                continue;
            }
            else if (state % 2 == 0 &amp;&amp; result &gt; 0)
                result /= 2;
            else
                result += state % 4;
        }
    }
    printf("Final result: %d\n", result);
    return result &gt; 10 ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

Crash output:
```
test.c:7:15: error: incompatible pointer to integer conversion initializing 'const char' with an expression of type
      'char[8]' [-Wint-conversion]
    7 |         [0] = "Success",
      |               ^~~~~~~~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=143 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-73502f.o -x c test.c
1.      test.c:10:6: current parser token '['
2.      test.c:5:1: parsing function body 'handle_error'
3.      test.c:5:1: in compound statement ('{}')
 #<!-- -->0 0x00007f50ba39fd01 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #<!-- -->1 0x00007f50ba39cd27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f50b9c47520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007f50b6d603e0 clang::ConstantArrayType::getSize() const (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x7b73e0)
 #<!-- -->4 0x00007f50b6e1e11a (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->5 0x00007f50b6e26b22 (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) SemaInit.cpp:0:0
 #<!-- -->6 0x00007f50b6e21592 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->7 0x00007f50b6e21f78 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->8 0x00007f50b6e22867 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
 #<!-- -->9 0x00007f50b6e171c6 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x86e1c6)
#<!-- -->10 0x00007f50b6c27497 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x67e497)
#<!-- -->11 0x00007f50b6c27cfe clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::Expr*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x67ecfe)
#<!-- -->12 0x00007f50b76d4e85 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xbae85)
#<!-- -->13 0x00007f50b76c8a2e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xaea2e)
#<!-- -->14 0x00007f50b76ca6e0 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xb06e0)
#<!-- -->15 0x00007f50b76ca8fa clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0xb08fa)
#<!-- -->16 0x00007f50b767ca62 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x62a62)
#<!-- -->17 0x00007f50b7692cdd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x78cdd)
#<!-- -->18 0x00007f50b7694c2a clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7ac2a)
#<!-- -->19 0x00007f50b76950e3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7b0e3)
#<!-- -->20 0x00007f50b776d4e3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1534e3)
#<!-- -->21 0x00007f50b776de5a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x153e5a)
#<!-- -->22 0x00007f50b776fb84 clang::Parser::ParseCompoundStatementBody(bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x155b84)
#<!-- -->23 0x00007f50b7770613 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x156613)
#<!-- -->24 0x00007f50b77a8664 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x18e664)
#<!-- -->25 0x00007f50b769327f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7927f)
#<!-- -->26 0x00007f50b77a1f86 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x187f86)
#<!-- -->27 0x00007f50b77a2d2b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x188d2b)
#<!-- -->28 0x00007f50b77ac473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x192473)
#<!-- -->29 0x00007f50b77ad6d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1936d5)
#<!-- -->30 0x00007f50b765a02c clang::ParseAST(clang::Sema&amp;, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x4002c)
#<!-- -->31 0x00007f50bc6ae236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#<!-- -->32 0x00007f50bc624a9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#<!-- -->33 0x00007f50be3a47a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#<!-- -->34 0x0000000000414d9b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#<!-- -->35 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#<!-- -->37 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#<!-- -->38 0x00007f50b9c2ed90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007f50b9c2ee40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-96ad0c.c
clang: note: diagnostic msg: /tmp/test-96ad0c.sh
clang: note: diagnostic msg: 

********************
```

[test.zip](https://github.com/user-attachments/files/21826380/test.zip)
</details>


---

### Comment 2 - frederick-vs-ja

Slightly reduced:
```C++
int main(void)
{
    (void)(const char[]) {
        [0] = "",
        [1] = ""
    }[1];
}
```

Looks like a regression since Clang 14 ([Godbolt link](https://godbolt.org/z/9sMra3ejP)).

---

