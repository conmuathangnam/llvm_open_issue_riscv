# [clangd] Crash

**Author:** olemayu
**URL:** https://github.com/llvm/llvm-project/issues/155507
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-08-27T10:55:57Z

## Body

CFLAGS = -std=c23
Version = Ubuntu 22.0.0 (++20250812082130+296e057d0bf2-1\~exp1\~20250812082146.1091)

```c
#include <stdint.h>

typedef bool __vbool2  __attribute__((ext_vector_type(2)));
typedef bool __vbool4  __attribute__((ext_vector_type(4)));
typedef bool __vbool8  __attribute__((ext_vector_type(8)));
typedef bool __vbool16 __attribute__((ext_vector_type(16)));
typedef bool __vbool32 __attribute__((ext_vector_type(32)));
typedef bool __vbool64 __attribute__((ext_vector_type(64)));

#define __vbit_cast2  uint8_t
#define __vbit_cast4  uint8_t
#define __vbit_cast8  uint8_t
#define __vbit_cast16 uint16_t
#define __vbit_cast32 uint32_t
#define __vbit_cast64 uint64_t

#define VMASK(bits, x) __builtin_bit_cast(__vbit_cast##bits, __builtin_convertvector(x, __vbool##bits))

typedef int16_t v2int16_t __attribute__((ext_vector_type(2)));

bool v2int16_eq(v2int16_t a, v2int16_t b) {
    return VMASK(2, a == b) == 0b11;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (olemayu)

<details>
CFLAGS = -std=c23
Version = Ubuntu 22.0.0 (++20250812082130+296e057d0bf2-1\~exp1\~20250812082146.1091)

```c
#include &lt;stdint.h&gt;

typedef bool __vbool2  __attribute__((ext_vector_type(2)));
typedef bool __vbool4  __attribute__((ext_vector_type(4)));
typedef bool __vbool8  __attribute__((ext_vector_type(8)));
typedef bool __vbool16 __attribute__((ext_vector_type(16)));
typedef bool __vbool32 __attribute__((ext_vector_type(32)));
typedef bool __vbool64 __attribute__((ext_vector_type(64)));

#define __vbit_cast2  uint8_t
#define __vbit_cast4  uint8_t
#define __vbit_cast8  uint8_t
#define __vbit_cast16 uint16_t
#define __vbit_cast32 uint32_t
#define __vbit_cast64 uint64_t

#define VMASK(bits, x) __builtin_bit_cast(__vbit_cast##bits, __builtin_convertvector(x, __vbool##bits))

typedef int16_t v2int16_t __attribute__((ext_vector_type(2)));

bool v2int16_eq(v2int16_t a, v2int16_t b) {
    return VMASK(2, a == b) == 0b11;
}
```
</details>


---

### Comment 2 - keinflue

Not actually a clangd issue specifically, also fails when compiling properly: https://godbolt.org/z/jdxcTz4z6

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (olemayu)

<details>
CFLAGS = -std=c23
Version = Ubuntu 22.0.0 (++20250812082130+296e057d0bf2-1\~exp1\~20250812082146.1091)

```c
#include &lt;stdint.h&gt;

typedef bool __vbool2  __attribute__((ext_vector_type(2)));
typedef bool __vbool4  __attribute__((ext_vector_type(4)));
typedef bool __vbool8  __attribute__((ext_vector_type(8)));
typedef bool __vbool16 __attribute__((ext_vector_type(16)));
typedef bool __vbool32 __attribute__((ext_vector_type(32)));
typedef bool __vbool64 __attribute__((ext_vector_type(64)));

#define __vbit_cast2  uint8_t
#define __vbit_cast4  uint8_t
#define __vbit_cast8  uint8_t
#define __vbit_cast16 uint16_t
#define __vbit_cast32 uint32_t
#define __vbit_cast64 uint64_t

#define VMASK(bits, x) __builtin_bit_cast(__vbit_cast##bits, __builtin_convertvector(x, __vbool##bits))

typedef int16_t v2int16_t __attribute__((ext_vector_type(2)));

bool v2int16_eq(v2int16_t a, v2int16_t b) {
    return VMASK(2, a == b) == 0b11;
}
```
</details>


---

### Comment 4 - Mr-Anyone

Bacektrace 

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/vhe/vscode/llvm-project/build-debug/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name main.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all
-fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/vhe/vscode/llvm-project/build-debug -fcoverage-compilation-dir=/home/vhe/vscode/llvm-project/bui
ld-debug -resource-dir /home/vhe/vscode/llvm-project/build-debug/lib/clang/22 -internal-isystem /home/vhe/vscode/llvm-project/build-debug/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-g
nu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c23 -ferror-limit 19 -fmessage-length=273 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HA
VE_DWARF2_CFI_ASM=1 -o /tmp/main-3f311f.o -x c main.c
1.      main.c:22:36: current parser token ';'
2.      main.c:21:43: parsing function body 'v2int16_eq'
3.      main.c:21:43: in compound statement ('{}')
 #0 0x000059877524e0c1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/vhe/vscode/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000059877524e64b PrintStackTraceSignalHandler(void*) /home/vhe/vscode/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x000059877524c396 llvm::sys::RunSignalHandlers() /home/vhe/vscode/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000059877524edcd SignalHandler(int, siginfo_t*, void*) /home/vhe/vscode/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007e47df045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000059877ad76fb5 clang::interp::State::diag(clang::SourceLocation, unsigned int, unsigned int, bool) /home/vhe/vscode/llvm-project/clang/lib/AST/ByteCode/State.cpp:87:34
 #6 0x000059877ad76f80 clang::interp::State::FFDiag(clang::SourceLocation, unsigned int, unsigned int) /home/vhe/vscode/llvm-project/clang/lib/AST/ByteCode/State.cpp:23:10
 #7 0x000059877aa46483 (anonymous namespace)::checkBitCastConstexprEligibilityType(clang::SourceLocation, clang::QualType, (anonymous namespace)::EvalInfo*, clang::ASTContext const&, bool) /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:7978:13
 #8 0x000059877aa45d03 (anonymous namespace)::checkBitCastConstexprEligibility((anonymous namespace)::EvalInfo*, clang::ASTContext const&, clang::CastExpr const*) /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:8002:29
 #9 0x000059877aa0329d CheckICE(clang::Expr const*, clang::ASTContext const&) /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:18077:9
#10 0x000059877aa02e91 CheckICE(clang::Expr const*, clang::ASTContext const&) /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:18021:14
#11 0x000059877aa02098 clang::Expr::isIntegerConstantExpr(clang::ASTContext const&) const /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:18113:15
#12 0x000059877aa0340a clang::Expr::getIntegerConstantExpr(clang::ASTContext const&) const /home/vhe/vscode/llvm-project/clang/lib/AST/ExprConstant.cpp:18133:7
#13 0x00005987791b5b60 AnalyzeComparison(clang::Sema&, clang::BinaryOperator*) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaChecking.cpp:11510:9
#14 0x00005987791b4dd7 AnalyzeImplicitConversions(clang::Sema&, (anonymous namespace)::AnalyzeImplicitConversionsWorkItem, llvm::SmallVectorImpl<(anonymous namespace)::AnalyzeImplicitConversionsWorkItem>&) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaChecking.cpp:12955
:7
#15 0x0000598779198663 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaChecking.cpp:13026:3
#16 0x0000598779198273 clang::Sema::CheckImplicitConversions(clang::Expr*, clang::SourceLocation) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaChecking.cpp:13293:1
#17 0x0000598779198d56 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaChecking.cpp:14222:8
#18 0x0000598779703bf1 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaExprCXX.cpp:7722:3
#19 0x0000598779b219e3 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaStmt.cpp:4159:11
#20 0x0000598779b20350 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) /home/vhe/vscode/llvm-project/clang/lib/Sema/SemaStmt.cpp:3864:7
#21 0x0000598778f53cd2 clang::Parser::ParseReturnStatement() /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseStmt.cpp:2336:18
#22 0x0000598778f4c9e4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /home/vhe/vscode/llvm-project/clang/lib/Par
se/ParseStmt.cpp:305:11
#23 0x0000598778f4bd40 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseStmt.cpp:75:20
#24 0x0000598778f559f9 clang::Parser::ParseCompoundStatementBody(bool) /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseStmt.cpp:1162:11
#25 0x0000598778f56db4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseStmt.cpp:2394:21
#26 0x0000598778e88b1e clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /home/vhe/vscode/llvm-project/clang/lib/Parse/Parser.cpp:1449:3
#27 0x0000598778f23445 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseDe
cl.cpp:2262:21
#28 0x0000598778e878cb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/vhe/vscode/llvm-project/clang/lib/Parse/Parser.cpp:1187:10
#29 0x0000598778e86dc2 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/vhe/vscode/llvm-project/clang/lib/Parse/Parser.cpp:1209:12
#30 0x0000598778e86636 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/vhe/vscode/llvm-project/clang/lib/Parse/Parser.cpp:1032:14
#31 0x0000598778e8436d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/vhe/vscode/llvm-project/clang/lib/Parse/Parser.cpp:745:12
#32 0x0000598778e7edac clang::ParseAST(clang::Sema&, bool, bool) /home/vhe/vscode/llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
#33 0x00005987769bedd3 clang::ASTFrontendAction::ExecuteAction() /home/vhe/vscode/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1430:1
#34 0x000059877611a6a0 clang::CodeGenAction::ExecuteAction() /home/vhe/vscode/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1114:5
#35 0x00005987769be746 clang::FrontendAction::Execute() /home/vhe/vscode/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1312:7
#36 0x00005987768d767e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/vhe/vscode/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1066:23
#37 0x0000598776bc9c57 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/vhe/vscode/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:8
#38 0x0000598770ca3f28 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/vhe/vscode/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#39 0x0000598770c96d92 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/vhe/vscode/llvm-project/clang/tools/driver/driver.cpp:223:5
#40 0x0000598770c95b1d clang_main(int, char**, llvm::ToolContext const&) /home/vhe/vscode/llvm-project/clang/tools/driver/driver.cpp:264:5
#41 0x0000598770ccb595 main /home/vhe/vscode/llvm-project/build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#42 0x00007e47df02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#43 0x00007e47df02a28b call_init ./csu/../csu/libc-start.c:128:20
#44 0x00007e47df02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#45 0x0000598770c955a5 _start (/home/vhe/vscode/llvm-project/build-debug/bin/clang-20+0x8d505a5```

---

### Comment 5 - Mr-Anyone

reduced a bit: 

```c
#include <stdint.h>

typedef bool __vbool2  __attribute__((ext_vector_type(2)));
typedef int16_t v2int16_t __attribute__((ext_vector_type(2)));

bool v2int16_eq(v2int16_t a, v2int16_t b) {
    return __builtin_bit_cast(uint8_t, __builtin_convertvector(a == b, __vbool2)) == 0b11;
}
```

---

### Comment 6 - Mr-Anyone

It seems that the problem is diagnostics: 

`Info` is a null pointer here, and when we emit diagnostics, the entire program segfaults.

https://github.com/llvm/llvm-project/blob/316004764fe39fd1a273ebec050c749e2176b098/clang/lib/AST/ExprConstant.cpp#L7972-L7980

The `Info` is first parameter being passed here: 

https://github.com/llvm/llvm-project/blob/316004764fe39fd1a273ebec050c749e2176b098/clang/lib/AST/ExprConstant.cpp#L18108-L18109




---

### Comment 7 - shafik

@tbaederr the diagnostic where are crashing was brought in via https://github.com/llvm/llvm-project/pull/119366

it looks like you missed that we can't assume `Info` is valid in `checkBitCastConstexprEligibilityType` and the function has special handling via `diag` and `note` lambdas. 

---

