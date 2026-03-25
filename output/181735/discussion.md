# Clang Frontend Crash - Compiling Malformed Input Containing Recursive Include and Unbalanced Parentheses

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/181735

## Body

Clang crashes when compiling the following malformed C input. This issue was discovered via fuzzing with AFL++ and minimized by hand. The crash is reproducible.

Tested on Clang 21.1.8 on Ubuntu 25 and Clang 21.1.0 on CompilerExplorer.

### Code:
```c
func() {

#include __FILE__
(((((((
```
### Command:
```
clang ./crashing_file.c
```
### Expected Output:
Syntax Error and safe termination

### Actual Behavior
Clang crashes with segmentation fault during parsing.

### Portion of the Stack Trace:
```
"Stacktrace": [
    "#0  0x000055555ea2e3c4 in clang::Lexer::LexTokenInternal (this=0x55556218b810, Result=..., TokAtPhysicalStartOfLine=<error reading variable: Cannot access memory at address 0x7fffff7fed37>) at clang/lib/Lex/Lexer.cpp:3740",
    "#1  0x000055555eb199ba in clang::Preprocessor::Lex (this=0x1, Result=...) at clang/lib/Lex/Preprocessor.cpp:896",
    "#2  0x000055555c3d82aa in clang::Parser::ConsumeParen (this=<optimized out>) at clang/include/clang/Parse/Parser.h:642",
    "#3  clang::BalancedDelimiterTracker::consumeOpen (this=0x7fffff7ff128) at clang/include/clang/Parse/RAIIObjectsForParser.h:471",
    "#4  clang::Parser::ParseParenExpression (this=this@entry=0x555562159280, ExprType=@0x7fffff7ffec0: clang::ParenParseOption::CastExpr, StopIfCastExpr=false, ParenBehavior=ParenBehavior@entry=clang::ParenExprKind::Unknown, CorrectionBehavior=clang::TypoCorrectionTypeBehavior::AllowBoth, CastTy=..., RParenLoc=...) at clang/lib/Parse/ParseExpr.cpp:2618",
    "#5  0x000055555c3d58ed in clang::Parser::ParseCastExpression (this=0x555562159280, ParseKind=ParseKind@entry=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=<optimized out>, NotCastExpr=@0x7fffff800d37: false, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), isVectorLiteral=false, NotPrimaryExpression=<optimized out>) at clang/lib/Parse/ParseExpr.cpp:763",
    "#6  0x000055555c3cad9d in clang::Parser::ParseCastExpression (this=0x1, ParseKind=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=false, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), isVectorLiteral=false, NotPrimaryExpression=0x0) at clang/lib/Parse/ParseExpr.cpp:567",
    "#7  clang::Parser::ParseAssignmentExpression (this=0x1, this@entry=0x555562159280, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), CorrectionBehavior@entry=clang::TypoCorrectionTypeBehavior::AllowBoth) at clang/lib/Parse/ParseExpr.cpp:90",
```

### Environment:
clang version 21.1.8
OS: Ubuntu 25.04
Architecture: x86_64
RAM: 8GB


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
Clang crashes when compiling the following malformed C input. This issue was discovered via fuzzing with AFL++ and minimized by hand. The crash is reproducible.

Tested on Clang 21.1.8 on Ubuntu 25 and Clang 21.1.0 on CompilerExplorer.

### Code:
```
func() {

#include __FILE__
(((((((
```
### Command:
```
clang ./crashing_file.c
```
### Expected Output:
Syntax Error and safe termination

### Actual Behavior
Clang crashes with segmentation fault during parsing.

### Portion of the Stack Trace:
```
"Stacktrace": [
    "#<!-- -->0  0x000055555ea2e3c4 in clang::Lexer::LexTokenInternal (this=0x55556218b810, Result=..., TokAtPhysicalStartOfLine=&lt;error reading variable: Cannot access memory at address 0x7fffff7fed37&gt;) at clang/lib/Lex/Lexer.cpp:3740",
    "#<!-- -->1  0x000055555eb199ba in clang::Preprocessor::Lex (this=0x1, Result=...) at clang/lib/Lex/Preprocessor.cpp:896",
    "#<!-- -->2  0x000055555c3d82aa in clang::Parser::ConsumeParen (this=&lt;optimized out&gt;) at clang/include/clang/Parse/Parser.h:642",
    "#<!-- -->3  clang::BalancedDelimiterTracker::consumeOpen (this=0x7fffff7ff128) at clang/include/clang/Parse/RAIIObjectsForParser.h:471",
    "#<!-- -->4  clang::Parser::ParseParenExpression (this=this@<!-- -->entry=0x555562159280, ExprType=@<!-- -->0x7fffff7ffec0: clang::ParenParseOption::CastExpr, StopIfCastExpr=false, ParenBehavior=ParenBehavior@<!-- -->entry=clang::ParenExprKind::Unknown, CorrectionBehavior=clang::TypoCorrectionTypeBehavior::AllowBoth, CastTy=..., RParenLoc=...) at clang/lib/Parse/ParseExpr.cpp:2618",
    "#<!-- -->5  0x000055555c3d58ed in clang::Parser::ParseCastExpression (this=0x555562159280, ParseKind=ParseKind@<!-- -->entry=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=&lt;optimized out&gt;, NotCastExpr=@<!-- -->0x7fffff800d37: false, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), isVectorLiteral=false, NotPrimaryExpression=&lt;optimized out&gt;) at clang/lib/Parse/ParseExpr.cpp:763",
    "#<!-- -->6  0x000055555c3cad9d in clang::Parser::ParseCastExpression (this=0x1, ParseKind=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=false, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), isVectorLiteral=false, NotPrimaryExpression=0x0) at clang/lib/Parse/ParseExpr.cpp:567",
    "#<!-- -->7  clang::Parser::ParseAssignmentExpression (this=0x1, this@<!-- -->entry=0x555562159280, CorrectionBehavior=(clang::TypoCorrectionTypeBehavior::AllowTypes | unknown: 0xfc), CorrectionBehavior@<!-- -->entry=clang::TypoCorrectionTypeBehavior::AllowBoth) at clang/lib/Parse/ParseExpr.cpp:90",
```

### Environment:
clang version 21.1.8
OS: Ubuntu 25.04
Architecture: x86_64
RAM: 8GB

</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 3 - apbenson00

Sorry about that. This is the output from CompilerExplorer on the trunk assertion build:
https://godbolt.org/z/9fn8Gs8xe


---

