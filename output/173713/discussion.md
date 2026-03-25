# [clang-fuzzer] Crash in DecltypeType

**Author:** zczc66
**URL:** https://github.com/llvm/llvm-project/issues/173713

## Body

Hi, while testing clang by AFL++, it found a crashing case:
version : llvmorg-21.1.8

I’ve edited the post.

Reduced and modified from the original:
https://godbolt.org/z/dP9e8cW91

PoC:
```
void fn_4(double *a, double *b, double *c) {
  int i = 0;
  while (i < 10)
    a[10] = c[({
      struct tree_el {
      } state_t[4][foo()] = 0;
      0 < 1;
    })];
  i++;
}
```

backtrace:
```
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:39: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T->isDependentType() && "should not see dependent types here"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:8: current parser token ';'
2.	<source>:1:44: parsing function body 'fn_4'
3.	<source>:1:44: in compound statement ('{}')
 #0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078e79c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078e79c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078e79c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078e79c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078e79c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078e79c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007985ac9 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7985ac9)
#10 0x0000000007981f67 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7981f67)
#11 0x0000000007985ddb clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7985ddb)
#12 0x0000000007986afb clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7986afb)
#13 0x0000000008148829 clang::ConstantArrayType::getNumAddressingBits(clang::ASTContext const&, clang::QualType, llvm::APInt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8148829)
#14 0x0000000008148b83 clang::ConstantArrayType::getNumAddressingBits(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8148b83)
#15 0x0000000007ce2b31 CheckArraySize((anonymous namespace)::EvalInfo&, clang::ConstantArrayType const*, clang::SourceLocation) ExprConstant.cpp:0:0
#16 0x0000000007d2b91d HandleDestructionImpl((anonymous namespace)::EvalInfo&, clang::SourceRange, (anonymous namespace)::LValue const&, clang::APValue&, clang::QualType) ExprConstant.cpp:0:0
#17 0x0000000007d2e082 (anonymous namespace)::ScopeRAII<((anonymous namespace)::ScopeKind)0>::cleanup((anonymous namespace)::EvalInfo&, bool, unsigned int) ExprConstant.cpp:0:0
#18 0x0000000007d37ce4 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#19 0x0000000007d0f87e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#20 0x0000000007d1017b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#21 0x0000000007d1722e EvaluateAsInt(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#22 0x0000000007d175b2 clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d175b2)
#23 0x0000000006c609b9 clang::Sema::CheckArrayAccess(clang::Expr const*, clang::Expr const*, clang::ArraySubscriptExpr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c609b9)
#24 0x0000000006c61b57 clang::Sema::CheckArrayAccess(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c61b57)
#25 0x0000000006f9a09a clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9a09a)
#26 0x0000000006f9be5a clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9be5a)
#27 0x0000000006f9c396 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c396)
#28 0x0000000006a9eea0 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9eea0)
#29 0x0000000006aa2339 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2339)
#30 0x0000000006b27bf9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b27bf9)
#31 0x0000000006b1f0a4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1f0a4)
#32 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#33 0x0000000006b20a8b clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b20a8b)
#34 0x0000000006b23d97 clang::Parser::ParseWhileStatement(clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b23d97)
#35 0x0000000006b1eb0e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1eb0e)
#36 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#37 0x0000000006b283ce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b283ce)
#38 0x0000000006b28c1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b28c1a)
#39 0x0000000006a2eb7b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2eb7b)
#40 0x0000000006a6503d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6503d)
#41 0x0000000006a21cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21cdb)
#42 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#43 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#44 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#45 0x0000000006a2b740 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b740)
#46 0x0000000006a0c2e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c2e3)
#47 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#48 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#49 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#50 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#51 0x0000000000de5c4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#52 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#53 0x0000000000ddc81d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#54 0x0000000004cc3049 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#55 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#56 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#57 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#58 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#59 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#60 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#61 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#62 0x000078e79c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#63 0x000078e79c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#64 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (zczc66)

<details>
Hi, while testing clang by AFL++, it found a crashing case:
version : llvmorg-21.1.8

Flags:
```
export LLVM_CC_NAME=/home/user/hlpfuzz_aflpp/afl-clang-fast LLVM_CXX_NAME=/home/user/hlpfuzz_aflpp/afl-clang-fast++ CC=gclang CXX=gclang++
cmake -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_SANITIZE_COVERAGE=On -DLLVM_BUILD_RUNTIME=Off -G "Unix Makefiles" ../llvm
make clang-fuzzer
```

PoC:
```
void fn_4 ( double * a , double * b , double * c ) { int i = 0 ; while ( i &lt; 10 ) a [ sizeof ( 0x11 ) / 0xbf ] = i ++ * c [ ( { struct tree_el { int val ; int insert ( const MyType Rcon [ thousands [ 0xec ] != ( ( y &gt;&gt; 2 &amp; ( sizeof ( foo ( tempa [ printf ( "sum = %d\n" , ( cnt ) [ foo ( tempa [ printf ( "sum = %d\n" , ( cnt ) [ a3 = ( CustomType * ) ( b3 ) ] [ 2 ] ) ] ) ] [ 2 ] ) ] ) ) ) ) * xtime ( xtime ( x ) ) ) [ i ] * c [ i ] ] , int ( * f2 ) ( node * * tree , node * item ) ) ( node * tree ) ; } i , state_t [ 4 ] [ ( ( 0x52 &gt;&gt; 7 ) &amp; 1 / ( 1 / ( 1 / ( ( ( 0x52 &gt;&gt; 7 ) &amp; 1 / ( ( ( 0x52 &gt;&gt; 7 ) &amp; 1 / ( 1 + ( sizeof ( set_up_dialog ( printf ( "\nrefined value of n and h are:%d  %f\n" , printf ( "sum = %d\n" , Nr + 1 ) , h ) , 0 , 0 ) ) ) ) ) + ( 1 / ( ( 0 ) ) ) ) ) * 0x1b ) ) ) ) + ( 1 / ( ( 0 ) ) ) ] [ 4 ] = 0 ; 0x97 &lt; 10000 ; } ) &lt;&lt; 1 ] ; i ++ ; }
```

Reproduction(Since make with ASan causes errors, I use gdb.):
```
gdb -q --batch \
    -x gdb_bt.cmd \
    --args /home/user/repo/llvm-project/gllvm_build/bin/clang-fuzzer poc

```
gdb_bt.cmd:
```
set pagination off 
set confirm off
set print thread-events off
handle SIGSTOP nostop noprint pass
handle SIGUSR1 nostop noprint pass
run
bt
quit
```

Crashing thread backtrace:
```
Program received signal SIGSEGV, Segmentation fault.
clang::DecltypeType::isSugared (this=0x55555e176240) at /home/user/repo/llvm-project/clang/lib/AST/Type.cpp:4142
4142    bool DecltypeType::isSugared() const { return !E-&gt;isInstantiationDependent(); }
#<!-- -->0  clang::DecltypeType::isSugared (this=0x55555e176240) at /home/user/repo/llvm-project/clang/lib/AST/Type.cpp:4142
#<!-- -->1  clang::DecltypeType::desugar (this=0x55555e176240) at /home/user/repo/llvm-project/clang/lib/AST/Type.cpp:4145
#<!-- -->2  0x00005555596f1336 in clang::ASTContext::getTypeInfoImpl (this=this@<!-- -->entry=0x55555e0d59a0, T=T@<!-- -->entry=0x55555e176240) at /home/user/repo/llvm-project/gllvm_build/tools/clang/include/clang/AST/TypeNodes.inc:39
#<!-- -->3  0x00005555596f4f30 in clang::ASTContext::getTypeInfo (this=this@<!-- -->entry=0x55555e0d59a0, T=T@<!-- -->entry=0x55555e176240) at /home/user/repo/llvm-project/clang/lib/AST/ASTContext.cpp:2056
#<!-- -->4  0x00005555596f4abb in clang::ASTContext::getTypeInfoInChars (T=0x55555e176240, this=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/AST/ASTContext.cpp:1970
#<!-- -->5  clang::ASTContext::getTypeInfoInChars (this=0x55555e0d59a0, T=...) at /home/user/repo/llvm-project/clang/lib/AST/ASTContext.cpp:1976
#<!-- -->6  0x00005555596f5f4d in clang::ASTContext::getTypeSizeInChars (this=0x2a0ce64, T=...) at /home/user/repo/llvm-project/clang/lib/AST/ASTContext.cpp:2653
#<!-- -->7  0x000055555a21c6f7 in clang::ConstantArrayType::getNumAddressingBits (Context=..., ElementType=..., ElementType@<!-- -->entry=..., NumElements=...) at /home/user/repo/llvm-project/clang/lib/AST/Type.cpp:217
#<!-- -->8  0x000055555a21cc05 in clang::ConstantArrayType::getNumAddressingBits (this=&lt;optimized out&gt;, Context=...) at /home/user/repo/llvm-project/clang/lib/AST/Type.cpp:251
#<!-- -->9  0x0000555559c1cd27 in CheckArraySize (Info=..., CAT=0x55555e1762c0, CallLoc=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:3860
#<!-- -->10 HandleDestructionImpl (Info=..., CallRange=..., This=..., Value=..., T=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:6929
#<!-- -->11 0x0000555559c1c571 in HandleDestruction (Info=..., Loc=..., LVBase=..., Value=..., T=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:7123
#<!-- -->12 (anonymous namespace)::Cleanup::endLifetime (this=&lt;optimized out&gt;, Info=..., RunDestructors=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:750
#<!-- -->13 0x0000555559c26f3b in (anonymous namespace)::ScopeRAII&lt;((anonymous namespace)::ScopeKind)0&gt;::cleanup (Info=..., RunDestructors=true, OldStackSize=0) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:1453
#<!-- -->14 (anonymous namespace)::ScopeRAII&lt;((anonymous namespace)::ScopeKind)0&gt;::destroy (RunDestructors=true, this=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:1431
#<!-- -->15 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitStmtExpr (this=0x7fffffff3520, E=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:8652
#<!-- -->16 0x0000555559cb2cd9 in (anonymous namespace)::IntExprEvaluator::VisitCastExpr (this=0x7fffffff3520, E=0x55555e1764c0) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:15228
#<!-- -->17 0x0000555559bd8fa1 in Evaluate (Result=..., Info=..., E=E@<!-- -->entry=0x55555e1764c0) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16641
#<!-- -->18 0x0000555559bd40bf in EvaluateAsRValue (Info=..., E=E@<!-- -->entry=0x55555e1764c0, Result=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16763
#<!-- -->19 0x0000555559bcde64 in EvaluateAsRValue (E=0x55555e1764c0, Result=..., Ctx=..., Info=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16846
#<!-- -->20 EvaluateAsInt (E=E@<!-- -->entry=0x55555e1764c0, ExprResult=..., Ctx=..., AllowSideEffects=AllowSideEffects@<!-- -->entry=clang::Expr::SE_NoSideEffects, Info=...) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16857
#<!-- -->21 0x0000555559bcdbfe in clang::Expr::EvaluateAsInt (this=0x55555e1764c0, Result=..., Ctx=..., AllowSideEffects=clang::Expr::SE_NoSideEffects, InConstantContext=64) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16916
#<!-- -->22 0x0000555558733bab in DiagnoseBadShiftValues (S=..., LHS=..., RHS=..., Loc=Loc@<!-- -->entry=..., Opc=Opc@<!-- -->entry=clang::BO_Shl, LHSType=...) at /home/user/repo/llvm-project/clang/lib/Sema/SemaExpr.cpp:11459
#<!-- -->23 0x0000555558732a0d in clang::Sema::CheckShiftOperands (this=this@<!-- -->entry=0x55555e13c730, LHS=..., RHS=..., Loc=..., Loc@<!-- -->entry=..., Opc=clang::BO_Shl, IsCompAssign=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Sema/SemaExpr.cpp:11753
#<!-- -->24 0x0000555558742adb in clang::Sema::CreateBuiltinBinOp (this=0x55555e13c730, OpLoc=..., Opc=&lt;optimized out&gt;, LHSExpr=0x55555e176480, RHSExpr=&lt;optimized out&gt;, ForFoldExpression=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Sema/SemaExpr.cpp:15073
#<!-- -->25 0x00005555586eff52 in clang::Sema::ActOnBinOp (this=0x55555e13c730, S=0x2a0ce64, TokLoc=..., Kind=&lt;optimized out&gt;, LHSExpr=&lt;optimized out&gt;, RHSExpr=0x55555e1764a0) at /home/user/repo/llvm-project/clang/lib/Sema/SemaExpr.cpp:15440
#<!-- -->26 0x0000555557e92478 in clang::Parser::ParseRHSOfBinaryExpression (this=this@<!-- -->entry=0x55555e1497e0, LHS=LHS@<!-- -->entry=..., MinPrec=MinPrec@<!-- -->entry=clang::prec::Assignment) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:533
#<!-- -->27 0x0000555557e910be in clang::Parser::ParseAssignmentExpression (this=this@<!-- -->entry=0x55555e1497e0, CorrectionBehavior=CorrectionBehavior@<!-- -->entry=clang::TypoCorrectionTypeBehavior::AllowNonTypes) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:92
#<!-- -->28 0x0000555557e9483e in clang::Parser::ParseExpression (this=0x55555e1497e0, CorrectionBehavior=clang::TypoCorrectionTypeBehavior::AllowNonTypes) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:48
#<!-- -->29 clang::Parser::ParsePostfixExpressionSuffix (this=this@<!-- -->entry=0x55555e1497e0, LHS=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:1709
#<!-- -->30 0x0000555557e98861 in clang::Parser::ParseCastExpression (this=this@<!-- -->entry=0x55555e1497e0, ParseKind=ParseKind@<!-- -->entry=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=&lt;optimized out&gt;, NotCastExpr=@<!-- -->0x7fffffff7500: false, CorrectionBehavior=CorrectionBehavior@<!-- -->entry=clang::TypoCorrectionTypeBehavior::AllowNonTypes, isVectorLiteral=&lt;optimized out&gt;, NotPrimaryExpression=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:1622
#<!-- -->31 0x0000555557e91d32 in clang::Parser::ParseCastExpression (this=0x55555e1497e0, ParseKind=clang::CastParseKind::AnyCastExpr, isAddressOfOperand=false, CorrectionBehavior=clang::TypoCorrectionTypeBehavior::AllowNonTypes, isVectorLiteral=false, NotPrimaryExpression=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:567
#<!-- -->32 clang::Parser::ParseRHSOfBinaryExpression (this=this@<!-- -->entry=0x55555e1497e0, LHS=LHS@<!-- -->entry=..., MinPrec=MinPrec@<!-- -->entry=clang::prec::Assignment) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:461
#<!-- -->33 0x0000555557e910be in clang::Parser::ParseAssignmentExpression (this=this@<!-- -->entry=0x55555e1497e0, CorrectionBehavior=CorrectionBehavior@<!-- -->entry=clang::TypoCorrectionTypeBehavior::AllowNonTypes) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:92
#<!-- -->34 0x0000555557e91d03 in clang::Parser::ParseRHSOfBinaryExpression (this=this@<!-- -->entry=0x55555e1497e0, LHS=LHS@<!-- -->entry=..., MinPrec=MinPrec@<!-- -->entry=clang::prec::Assignment) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:459
#<!-- -->35 0x0000555557e910be in clang::Parser::ParseAssignmentExpression (this=0x55555e1497e0, CorrectionBehavior=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:92
#<!-- -->36 0x0000555557e90e86 in clang::Parser::ParseExpression (this=0x7ffff4f60010, CorrectionBehavior=(unknown: 0x2a0ce64)) at /home/user/repo/llvm-project/clang/lib/Parse/ParseExpr.cpp:48
#<!-- -->37 0x0000555557f3db87 in clang::Parser::ParseExprStatement (this=0x7ffff4f60010, this@<!-- -->entry=0x55555e1497e0, StmtCtx=StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::SubStmt) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:521
#<!-- -->38 0x0000555557f3a141 in clang::Parser::ParseStatementOrDeclarationAfterAttributes (this=this@<!-- -->entry=0x55555e1497e0, Stmts=..., StmtCtx=44093028, StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::SubStmt, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0, CXX11Attrs=..., GNUAttrs=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:249
#<!-- -->39 0x0000555557f39498 in clang::Parser::ParseStatementOrDeclaration (this=this@<!-- -->entry=0x55555e1497e0, Stmts=..., StmtCtx=StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::SubStmt, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:75
#<!-- -->40 0x0000555557f410c0 in clang::Parser::ParseStatement (this=0x55555e1497e0, TrailingElseLoc=0x0, StmtCtx=clang::Parser::ParsedStmtContext::SubStmt) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:47
#<!-- -->41 clang::Parser::ParseWhileStatement (this=this@<!-- -->entry=0x55555e1497e0, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:1767
#<!-- -->42 0x0000555557f3a541 in clang::Parser::ParseStatementOrDeclarationAfterAttributes (this=this@<!-- -->entry=0x55555e1497e0, Stmts=..., StmtCtx=StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=0x2a0ce64, TrailingElseLoc@<!-- -->entry=0x0, CXX11Attrs=..., GNUAttrs=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:284
#<!-- -->43 0x0000555557f39498 in clang::Parser::ParseStatementOrDeclaration (this=this@<!-- -->entry=0x55555e1497e0, Stmts=..., StmtCtx=StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:75
#<!-- -->44 0x0000555557f48a2b in clang::Parser::ParseCompoundStatementBody (this=this@<!-- -->entry=0x55555e1497e0, isStmtExpr=44) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:1161
#<!-- -->45 0x0000555557f49dfd in clang::Parser::ParseFunctionStatementBody (this=0x55555e1497e0, Decl=0x55555e1500d0, BodyScope=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:2393
#<!-- -->46 0x0000555557e0048d in clang::Parser::ParseFunctionDefinition (this=0x55555e1497e0, D=..., TemplateInfo=..., LateParsedAttrs=0x7fffffff86e8) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1449
#<!-- -->47 0x0000555557e3d1da in clang::Parser::ParseDeclGroup (this=0x55555e1497e0, DS=..., Context=clang::DeclaratorContext::File, Attrs=..., TemplateInfo=..., DeclEnd=0x0, FRI=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2238
#<!-- -->48 0x0000555557dfe72d in clang::Parser::ParseDeclOrFunctionDefInternal (this=this@<!-- -->entry=0x55555e1497e0, Attrs=..., DeclSpecAttrs=..., DS=..., AS=AS@<!-- -->entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1187
#<!-- -->49 0x0000555557dfd9ee in clang::Parser::ParseDeclarationOrFunctionDefinition (this=this@<!-- -->entry=0x55555e1497e0, Attrs=..., DeclSpecAttrs=..., DS=DS@<!-- -->entry=0x55555e1497e0, AS=AS@<!-- -->entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1209
#<!-- -->50 0x0000555557dfbfef in clang::Parser::ParseExternalDeclaration (this=this@<!-- -->entry=0x55555e1497e0, Attrs=..., DeclSpecAttrs=..., DS=DS@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1032
#<!-- -->51 0x0000555557df9357 in clang::Parser::ParseTopLevelDecl (this=this@<!-- -->entry=0x55555e1497e0, Result=..., ImportState=@<!-- -->0x7fffffff9df4: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:745
#<!-- -->52 0x0000555557df896e in clang::Parser::ParseFirstTopLevelDecl (this=0x55555e1497e0, Result=..., ImportState=@<!-- -->0x7fffffff9df4: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:601
#<!-- -->53 0x0000555557df184b in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Parse/ParseAST.cpp:169
#<!-- -->54 0x0000555557c8064c in clang::FrontendAction::Execute (this=0x2a0ce64) at /home/user/repo/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1221
#<!-- -->55 0x0000555557b51601 in clang::CompilerInstance::ExecuteAction (this=0x7fffffff9fb0, Act=...) at /home/user/repo/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055
#<!-- -->56 0x0000555557b085d7 in clang::tooling::FrontendActionFactory::runInvocation (this=0x55555e0a4e80, Invocation=..., Files=0x55555e091790, PCHContainerOps=..., DiagConsumer=0x7fffffffa190) at /home/user/repo/llvm-project/clang/lib/Tooling/Tooling.cpp:463
#<!-- -->57 0x00005555558d1a8e in clang_fuzzer::HandleCXX (S="void fn_4 ( double * a , double * b , double * c ) { int i = 0 ; while ( i &lt; 10 ) a [ sizeof ( 0x11 ) / 0xbf ] = i ++ * c [ ( { struct tree_el { int val ; int insert ( const MyType Rcon [ thousands [ "..., FileName=&lt;optimized out&gt;, ExtraArgs=std::vector of length 1, capacity 1 = {...}) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/handle-cxx/handle_cxx.cpp:49
#<!-- -->58 0x00005555558d1434 in LLVMFuzzerTestOneInput (data=0x7ffff4e5f010 "void fn_4 ( double * a , double * b , double * c ) { int i = 0 ; while ( i &lt; 10 ) a [ sizeof ( 0x11 ) / 0xbf ] = i ++ * c [ ( { struct tree_el { int val ; int insert ( const MyType Rcon [ thousands [ "..., size=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
#<!-- -->59 0x000055555c55452e in ExecuteFilesOnyByOne (argc=2, argv=0x7fffffffe198, callback=callback@<!-- -->entry=0x5555558d12f0 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:256
#<!-- -->60 0x000055555c55431e in LLVMFuzzerRunDriver (argcp=argcp@<!-- -->entry=0x7fffffffe064, argvp=argvp@<!-- -->entry=0x7fffffffe068, callback=0x5555558d12f0 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:377
#<!-- -->61 0x000055555c553e5e in main (argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe198) at aflpp_driver.c:312
#<!-- -->62 0x00007ffff7a64d90 in __libc_start_call_main (main=main@<!-- -->entry=0x55555c553da0 &lt;main&gt;, argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe198) at ../sysdeps/nptl/libc_start_call_main.h:58
#<!-- -->63 0x00007ffff7a64e40 in __libc_start_main_impl (main=0x55555c553da0 &lt;main&gt;, argc=2, argv=0x7fffffffe198, init=&lt;optimized out&gt;, fini=&lt;optimized out&gt;, rtld_fini=&lt;optimized out&gt;, stack_end=0x7fffffffe188) at ../csu/libc-start.c:392
#<!-- -->64 0x00005555558d1215 in _start ()
```


</details>


---

### Comment 2 - shafik

You have too much irrelevant information in this report. If flags and configurations don't matter to reproducing the issue than adding all that additions information is not helpful and actually is a big distraction.

Second if you can't format the code properly using for example clang-format then it really demotivates us from triaging the issue. Formatting is the lowest bar.

You should also include a godbolt link, simple reproducers are better and a godbolt link helps avoid confusion. gdb is not necessary to reproduce this bug, adding those steps is a distraction from the real bug.

Also, please make sure you are looking for duplicates, you should not open a new bug report if an existing issue covers it. If the code is unique then you should add the example as a comment to the existing bug.

---

### Comment 3 - zczc66

> You have too much irrelevant information in this report. If flags and configurations don't matter to reproducing the issue than adding all that additions information is not helpful and actually is a big distraction.[/+]
>
> Second if you can't format the code properly using for example clang-format then it really demotivates us from triaging the issue. Formatting is the lowest bar.[/+]
>
> You should also include a godbolt link, simple reproducers are better and a godbolt link helps avoid confusion. gdb is not necessary to reproduce this bug, adding those steps is a distraction from the real bug.[/+]
>
> Also, please make sure you are looking for duplicates, you should not open a new bug report if an existing issue covers it. If the code is unique then you should add the example as a comment to the existing bug.

Thank you for your reply! This was indeed my mistake, and I will fix it and resubmit.

---

### Comment 4 - shafik

> Thank you for your reply! This was indeed my mistake, and I will fix it and resubmit.

You don't need to resubmit, just edit the issues.


---

### Comment 5 - zczc66





> > Thank you for your reply! This was indeed my mistake, and I will fix it and resubmit.
> 
> You don't need to resubmit, just edit the issues.

I’ve edited the relevant post and appreciate your suggestions. Please take another look.

---

### Comment 6 - shafik

@cor3ntin this bisect to 45ab2b48bd55e50a86f6026ed31d2b60a118bdce

---

