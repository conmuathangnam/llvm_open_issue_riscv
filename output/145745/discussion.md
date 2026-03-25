# Clang 20.1.0 Compiler Internal Error (Crash) during AST Parsing of C++23

**Author:** 0xSqualo
**URL:** https://github.com/llvm/llvm-project/issues/145745
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, c++23
**Closed Date:** 2025-06-25T18:33:10Z

## Body

Clang 20.1.0 crashes with an unhandled internal error, when compiling a specific C++23 source file, and requests that this issue be reported to the LLVM project.

This issue was discovered using Boost 1.83.0 and Clang 20.1.0, which highlights the issue is in the AST parsing logic. 

It seems that this issue could potentially be due to AST parsing during the [`_do_erased_copy(...)` call internal to `status_code_domain.hpp`](https://github.com/boostorg/outcome/blob/b3bc4b9360e1b2110c33716a2616a54d60c1de72/include/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp#L451), however this root cause is not certain.

### PoC

See example with full crash on godbolt: https://godbolt.org/z/crG13rjo9

This issue can otherwise be reproduced with:
- `clang-20`: version `20.1.0`
- Boost: version `1.83.0`

And using a basic entrypoint that includes the boost header which triggers the AST issue:

```c++
// main.cpp
#include <boost/outcome/experimental/status-code/status-code/nested_status_code.hpp>

int main() {
    return 0;
}
```

For full reproducability locally, I used (on Ubuntu `25.04`):

`CMakeLists.txt`: 

```CMakeLists.txt
cmake_minimum_required(VERSION 3.30)

project(clang-20-issue)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED EXACT 1.83.0)

set(TARGET_NAME a.out)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
```

And this `build.sh` build script:

```bash
mkdir -p build
CC=clang-20 CXX=clang++-20 cmake . -B build
cmake --build build
```

Which reproduces the same AST crash as on godbolt.

For reference:

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++23 <source> -isystem/opt/compiler-explorer/libs/boost_1_83_0
1.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:451:30: current parser token ')'
2.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:38:1 <Spelling=/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/config.hpp:212:3>: parsing namespace 'system_error2'
3.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:110:1: parsing struct/union/class body 'system_error2::status_code_domain'
4.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: parsing function body 'system_error2::status_code_domain::_do_erased_copy'
5.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: in compound statement ('{}')
 #0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000793ca9242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007769b20 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x7769b20)
 #5 0x00000000071b4651 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b4651)
 #6 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #7 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #8 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #9 0x00000000071b556b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b556b)
#10 0x00000000071b5ec9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b5ec9)
#11 0x0000000007444ad3 HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#12 0x000000000749f0cb tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long&) ExprConstant.cpp:0:0
#13 0x000000000749fb17 clang::Expr::tryEvaluateObjectSize(unsigned long&, clang::ASTContext&, unsigned int) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x749fb17)
#14 0x0000000006506839 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda1'(unsigned int)::operator()(unsigned int) const SemaChecking.cpp:0:0
#15 0x000000000651d7b0 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x651d7b0)
#16 0x00000000068266e2 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x68266e2)
```





## Comments

### Comment 1 - EugeneZelenko

`main` seems OK: https://godbolt.org/z/fzzGbbYx5

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (0xSqualo)

<details>
Clang 20.1.0 crashes with an unhandled internal error, when compiling a specific C++23 source file, and requests that this issue be reported to the LLVM project.

This issue was discovered using Boost 1.83.0 and Clang 20.1.0, which highlights the issue is in the AST parsing logic. 

It seems that this issue could potentially be due to AST parsing during the [`_do_erased_copy(...)` call internal to `status_code_domain.hpp`](https://github.com/boostorg/outcome/blob/b3bc4b9360e1b2110c33716a2616a54d60c1de72/include/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp#L451), however this root cause is not certain.

### PoC

See example with full crash on godbolt: https://godbolt.org/z/crG13rjo9

This issue can otherwise be reproduced with:
- `clang-20`: version `20.1.0`
- Boost: version `1.83.0`

And using a basic entrypoint that includes the boost header which triggers the AST issue:

```c++
// main.cpp
#include &lt;boost/outcome/experimental/status-code/status-code/nested_status_code.hpp&gt;

int main() {
    return 0;
}
```

For full reproducability locally, I used (on Ubuntu `25.04`):

`CMakeLists.txt`: 

```CMakeLists.txt
cmake_minimum_required(VERSION 3.30)

project(clang-20-issue)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED EXACT 1.83.0)

set(TARGET_NAME a.out)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
```

And this `build.sh` build script:

```bash
mkdir -p build
CC=clang-20 CXX=clang++-20 cmake . -B build
cmake --build build
```

Which reproduces the same AST crash as on godbolt.

For reference:

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++23 &lt;source&gt; -isystem/opt/compiler-explorer/libs/boost_1_83_0
1.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:451:30: current parser token ')'
2.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:38:1 &lt;Spelling=/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/config.hpp:212:3&gt;: parsing namespace 'system_error2'
3.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:110:1: parsing struct/union/class body 'system_error2::status_code_domain'
4.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: parsing function body 'system_error2::status_code_domain::_do_erased_copy'
5.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: in compound statement ('{}')
 #<!-- -->0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #<!-- -->1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #<!-- -->2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000793ca9242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007769b20 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x7769b20)
 #<!-- -->5 0x00000000071b4651 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b4651)
 #<!-- -->6 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #<!-- -->7 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #<!-- -->8 0x000000000719e68b clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x719e68b)
 #<!-- -->9 0x00000000071b556b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b556b)
#<!-- -->10 0x00000000071b5ec9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x71b5ec9)
#<!-- -->11 0x0000000007444ad3 HandleSizeof((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::CharUnits&amp;, SizeOfType) ExprConstant.cpp:0:0
#<!-- -->12 0x000000000749f0cb tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&amp;, unsigned long&amp;) ExprConstant.cpp:0:0
#<!-- -->13 0x000000000749fb17 clang::Expr::tryEvaluateObjectSize(unsigned long&amp;, clang::ASTContext&amp;, unsigned int) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x749fb17)
#<!-- -->14 0x0000000006506839 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda1'(unsigned int)::operator()(unsigned int) const SemaChecking.cpp:0:0
#<!-- -->15 0x000000000651d7b0 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x651d7b0)
#<!-- -->16 0x00000000068266e2 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x68266e2)
```




</details>


---

### Comment 3 - shafik

Looks like something introduced in clang-20 and fixed in trunk.

for clang-20: https://godbolt.org/z/GM516svb5

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/Type.cpp:2387:
bool clang::Type::isConstantSizeType() const:
Assertion `!isIncompleteType() && "This doesn't make sense for incomplete types"' failed.
```

backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++23 -std=c++23 <source> -isystem/opt/compiler-explorer/libs/boost_1_83_0
1.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:451:30: current parser token ')'
2.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:38:1 <Spelling=/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/config.hpp:212:3>: parsing namespace 'system_error2'
3.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:110:1: parsing struct/union/class body 'system_error2::status_code_domain'
4.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: parsing function body 'system_error2::status_code_domain::_do_erased_copy'
5.	/opt/compiler-explorer/libs/boost_1_83_0/boost/outcome/experimental/status-code/status-code/status_code_domain.hpp:443:3: in compound statement ('{}')
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d1778442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d17784969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d1778442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d17784287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d177842871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d1778439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007e2184f clang::Type::isConstantSizeType() const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7e2184f)
#10 0x0000000007a0cc5c HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#11 0x0000000007a51cba tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long&) ExprConstant.cpp:0:0
#12 0x0000000007a528a3 clang::Expr::tryEvaluateObjectSize(unsigned long&, clang::ASTContext&, unsigned int) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7a528a3)
#13 0x0000000006a72562 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda1'(unsigned int)::operator()(unsigned int) const SemaChecking.cpp:0:0
#14 0x0000000006a7c7a7 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6a7c7a7)
#15 0x0000000006d72c1b clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d72c1b)
#16 0x000000000713b04a FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#17 0x000000000713bf6c clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bf6c)
#18 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#19 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#20 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#21 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#22 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#23 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#24 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#25 0x0000000006945929 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6945929)
#26 0x000000000693bc9e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693bc9e)
#27 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#28 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#29 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#30 0x000000000685e8b0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x685e8b0)
#31 0x000000000685e59d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x685e59d)
#32 0x00000000068b04ae clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68b04ae)
#33 0x00000000068b2aa3 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68b2aa3)
#34 0x0000000006887018 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6887018)
#35 0x0000000006844514 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844514)
#36 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#37 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#38 0x00000000068a8154 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68a8154)
#39 0x00000000068a8d83 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68a8d83)
#40 0x000000000688dd67 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x688dd67)
#41 0x000000000684c767 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c767)
#42 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#43 0x000000000683fa5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#44 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#45 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#46 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#47 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#48 0x0000000000cfe40f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#49 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#50 0x000000000483e749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#51 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#52 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#53 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#54 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#55 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#56 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#57 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#58 0x00007d1778429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#59 0x00007d1778429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#60 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

