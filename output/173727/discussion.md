# [clang-fuzzer] Crash in clang::QualType::hasQualifiers

**Author:** zczc66
**URL:** https://github.com/llvm/llvm-project/issues/173727

## Body

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
int main ( ) { int a = "hello" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - - ( * "\n Enter values of x0,xn,h:\n" ) ) >> 10 ) - ( 1 / ( ( ( ( ( 0x52 >> 0x5b ) & 1 ) / ( ( ( 0x52 >> ( ( 0x52 >> 7 ) & ! ( 0x1c ^ ( ! 1 < 10000 ) ) ) ) > 0 ? 0 : ( * "Setup" + "\nfinal integration is %f" ) ) [ 10 ] ) ) ) ) + ( 1 / ( - - ( * "\n Enter values of x0,xn,h:\n" ) ) ) ) ) * 0x1b ) + ( 0.0 ) ) , 0 ) ) * 0x1b ) ) ) + ( 0.0 ) ) ) * 4 ) * ( ( 1 + - ( 1 + - ( ( ( 1 / ( 1 + ( 1 / ( ( ( ( 1 + ( 1 / ( ( ( 0.0 ) ) - 1 ) ) ) / ( 0x97 + ( 0.0 ) ) ) ) - 1 ) ) ) ) ) * ( 0 ) ) ) ) ) ) ) < 10000 ) ) ) + ( 1 / ( ( 0 ) ) ) >> 4 ) ) ] , volatile char * thousands , int thousands_len ) ( ) ( char * sbox [ sizeof ( ( 0x52 >> 7 ) & 1 / ( 1 + ( ( int ) ( 0x74 ) ) ) ) ] ) ) = 3.14 ; char * c = "literal" ; __builtin_printf ( "%d" , c ) ; int unused ; return a ; }
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
clang::QualType::hasQualifiers (this=<optimized out>) at /home/user/repo/llvm-project/clang/include/clang/AST/Type.h:7837
7837             getCommonPtr()->CanonicalType.hasLocalQualifiers();
#0  clang::QualType::hasQualifiers (this=<optimized out>) at /home/user/repo/llvm-project/clang/include/clang/AST/Type.h:7837
#1  AdjustFunctionParmAndArgTypesForDeduction (Arg=<optimized out>, FailedTSC=<optimized out>, S=..., TemplateParams=<optimized out>, FirstInnerIndex=<optimized out>, ParamType=..., ArgType=..., ArgClassification=..., TDF=<optimized out>) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4154
#2  DeduceTemplateArgumentsFromCallArgument (S=..., TemplateParams=TemplateParams@entry=0x7fffffff6de0, FirstInnerIndex=FirstInnerIndex@entry=0, ParamType=..., ArgType=..., ArgClassification=..., Arg=0x55555e37dd58, Info=..., Deduced=..., OriginalCallArgs=..., DecomposedParam=<optimized out>, ArgIdx=0, TDF=0, FailedTSC=0x0) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4340
#3  0x0000555558fb6c4c in clang::Sema::DeduceAutoType (this=0x55555e349790, Type=..., Init=<optimized out>, Result=..., Info=..., DependentDeduction=<optimized out>, IgnoreConstraints=<optimized out>, FailedTSC=<optimized out>) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5178
#4  0x000055555845b2fd in clang::Sema::deduceVarTypeFromInitializer (this=this@entry=0x55555e349790, VDecl=<optimized out>, VDecl@entry=0x55555e37dcf0, Name=..., Name@entry=..., Type=..., TSI=0x0, TSI@entry=0x55555e37dcb0, Range=..., DirectInit=false, Init=<optimized out>) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:12904
#5  0x000055555845bc01 in clang::Sema::DeduceVariableDeclarationType (this=this@entry=0x55555e349790, VDecl=VDecl@entry=0x55555e37dcf0, DirectInit=<optimized out>, Init=<optimized out>, Init@entry=0x55555e37dd58) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:12941
#6  0x000055555845c990 in clang::Sema::AddInitializerToDecl (this=0x55555e349790, RealDecl=0x55555e37dcf0, Init=0x55555e37dd58, DirectInit=164) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:13311
#7  0x0000555557d358e7 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes (this=this@entry=0x55555e356b60, D=..., TemplateInfo=..., FRI=FRI@entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2823
#8  0x0000555557d30890 in clang::Parser::ParseDeclGroup (this=this@entry=0x55555e356b60, DS=..., Context=Context@entry=clang::DeclaratorContext::Block, Attrs=..., TemplateInfo=..., DeclEnd=0x7fffffffb020, FRI=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2516
#9  0x0000555557d2e5d0 in clang::Parser::ParseSimpleDeclaration (this=this@entry=0x55555e356b60, Context=clang::DeclaratorContext::Block, Context@entry=clang::DeclaratorContext::File, DeclEnd=..., DeclAttrs=..., DeclSpecAttrs=..., RequireSemi=52, FRI=0x0, DeclSpecStart=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2135
#10 0x0000555557d2dabc in clang::Parser::ParseDeclaration (this=0x55555e356b60, Context=4294929888, DeclEnd=..., DeclAttrs=..., DeclSpecAttrs=..., DeclSpecStart=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2028
#11 0x0000555557e50a55 in clang::Parser::ParseStatementOrDeclarationAfterAttributes (this=this@entry=0x55555e356b60, Stmts=..., StmtCtx=StmtCtx@entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=TrailingElseLoc@entry=0x0, CXX11Attrs=..., GNUAttrs=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:259
#12 0x0000555557e4e4f5 in clang::Parser::ParseStatementOrDeclaration (this=this@entry=0x55555e356b60, Stmts=..., StmtCtx=clang::Parser::ParsedStmtContext::SubStmt, StmtCtx@entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=TrailingElseLoc@entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:124
#13 0x0000555557e5f14c in clang::Parser::ParseCompoundStatementBody (this=this@entry=0x55555e356b60, isStmtExpr=96) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:1248
#14 0x0000555557e612da in clang::Parser::ParseFunctionStatementBody (this=0x55555e356b60, Decl=0x55555e35d198, BodyScope=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:2526
#15 0x0000555557cf1009 in clang::Parser::ParseFunctionDefinition (this=0x55555e356b60, D=..., TemplateInfo=..., LateParsedAttrs=0x7fffffffba80) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1525
#16 0x0000555557d31fe5 in clang::Parser::ParseDeclGroup (this=0x55555e356b60, DS=..., Context=clang::DeclaratorContext::File, Attrs=..., TemplateInfo=..., DeclEnd=0x0, FRI=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2402
#17 0x0000555557ceed0b in clang::Parser::ParseDeclOrFunctionDefInternal (this=this@entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=..., AS=AS@entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1249
#18 0x0000555557cedee1 in clang::Parser::ParseDeclarationOrFunctionDefinition (this=this@entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=DS@entry=0x55555e356b60, AS=1580191984, AS@entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1271
#19 0x0000555557cec26d in clang::Parser::ParseExternalDeclaration (this=this@entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=DS@entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1074
#20 0x0000555557ce8f6b in clang::Parser::ParseTopLevelDecl (this=this@entry=0x55555e356b60, Result=..., ImportState=@0x7fffffffd724: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:763
#21 0x0000555557ce824f in clang::Parser::ParseFirstTopLevelDecl (this=0x55555e356b60, Result=..., ImportState=@0x7fffffffd724: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:608
#22 0x0000555557cdff8d in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=<optimized out>) at /home/user/repo/llvm-project/clang/lib/Parse/ParseAST.cpp:170
#23 0x0000555557b850e6 in clang::FrontendAction::Execute (this=0x55555e2c9a90) at /home/user/repo/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1078
#24 0x0000555557a4ae01 in clang::CompilerInstance::ExecuteAction (this=0x7fffffffd8d8, Act=...) at /home/user/repo/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061
#25 0x0000555557a02502 in clang::tooling::FrontendActionFactory::runInvocation (this=0x55555e2c1ef0, Invocation=..., Files=0x55555e2c6920, PCHContainerOps=..., DiagConsumer=0x7fffffffdaa0) at /home/user/repo/llvm-project/clang/lib/Tooling/Tooling.cpp:465
#26 0x0000555555acd136 in clang_fuzzer::HandleCXX (S="int main ( ) { int a = \"hello\" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - -"..., FileName=<optimized out>, ExtraArgs=std::vector of length 1, capacity 1 = {...}) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/handle-cxx/handle_cxx.cpp:49
#27 0x0000555555accad4 in LLVMFuzzerTestOneInput (data=0x7ffff4be4010 "int main ( ) { int a = \"hello\" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - -"..., size=<optimized out>) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
#28 0x000055555c6f96ee in ExecuteFilesOnyByOne (argc=2, argv=0x7fffffffe328, callback=callback@entry=0x555555acc990 <LLVMFuzzerTestOneInput(uint8_t*, size_t)>) at aflpp_driver.c:256
#29 0x000055555c6f94de in LLVMFuzzerRunDriver (argcp=argcp@entry=0x7fffffffe1f4, argvp=argvp@entry=0x7fffffffe1f8, callback=0x555555acc990 <LLVMFuzzerTestOneInput(uint8_t*, size_t)>) at aflpp_driver.c:377
#30 0x000055555c6f901e in main (argc=argc@entry=2, argv=argv@entry=0x7fffffffe328) at aflpp_driver.c:312
#31 0x00007ffff7a63d90 in __libc_start_call_main (main=main@entry=0x55555c6f8f60 <main>, argc=argc@entry=2, argv=argv@entry=0x7fffffffe328) at ../sysdeps/nptl/libc_start_call_main.h:58
#32 0x00007ffff7a63e40 in __libc_start_main_impl (main=0x55555c6f8f60 <main>, argc=2, argv=0x7fffffffe328, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffe318) at ../csu/libc-start.c:392
#33 0x0000555555acc8b5 in _start ()
```


## Comments

### Comment 1 - shafik

Please see my comment in your previous report and update this one accordingly, thank you.

---

### Comment 2 - llvmbot


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
int main ( ) { int a = "hello" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - - ( * "\n Enter values of x0,xn,h:\n" ) ) &gt;&gt; 10 ) - ( 1 / ( ( ( ( ( 0x52 &gt;&gt; 0x5b ) &amp; 1 ) / ( ( ( 0x52 &gt;&gt; ( ( 0x52 &gt;&gt; 7 ) &amp; ! ( 0x1c ^ ( ! 1 &lt; 10000 ) ) ) ) &gt; 0 ? 0 : ( * "Setup" + "\nfinal integration is %f" ) ) [ 10 ] ) ) ) ) + ( 1 / ( - - ( * "\n Enter values of x0,xn,h:\n" ) ) ) ) ) * 0x1b ) + ( 0.0 ) ) , 0 ) ) * 0x1b ) ) ) + ( 0.0 ) ) ) * 4 ) * ( ( 1 + - ( 1 + - ( ( ( 1 / ( 1 + ( 1 / ( ( ( ( 1 + ( 1 / ( ( ( 0.0 ) ) - 1 ) ) ) / ( 0x97 + ( 0.0 ) ) ) ) - 1 ) ) ) ) ) * ( 0 ) ) ) ) ) ) ) &lt; 10000 ) ) ) + ( 1 / ( ( 0 ) ) ) &gt;&gt; 4 ) ) ] , volatile char * thousands , int thousands_len ) ( ) ( char * sbox [ sizeof ( ( 0x52 &gt;&gt; 7 ) &amp; 1 / ( 1 + ( ( int ) ( 0x74 ) ) ) ) ] ) ) = 3.14 ; char * c = "literal" ; __builtin_printf ( "%d" , c ) ; int unused ; return a ; }
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
clang::QualType::hasQualifiers (this=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/include/clang/AST/Type.h:7837
7837             getCommonPtr()-&gt;CanonicalType.hasLocalQualifiers();
#<!-- -->0  clang::QualType::hasQualifiers (this=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/include/clang/AST/Type.h:7837
#<!-- -->1  AdjustFunctionParmAndArgTypesForDeduction (Arg=&lt;optimized out&gt;, FailedTSC=&lt;optimized out&gt;, S=..., TemplateParams=&lt;optimized out&gt;, FirstInnerIndex=&lt;optimized out&gt;, ParamType=..., ArgType=..., ArgClassification=..., TDF=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4154
#<!-- -->2  DeduceTemplateArgumentsFromCallArgument (S=..., TemplateParams=TemplateParams@<!-- -->entry=0x7fffffff6de0, FirstInnerIndex=FirstInnerIndex@<!-- -->entry=0, ParamType=..., ArgType=..., ArgClassification=..., Arg=0x55555e37dd58, Info=..., Deduced=..., OriginalCallArgs=..., DecomposedParam=&lt;optimized out&gt;, ArgIdx=0, TDF=0, FailedTSC=0x0) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4340
#<!-- -->3  0x0000555558fb6c4c in clang::Sema::DeduceAutoType (this=0x55555e349790, Type=..., Init=&lt;optimized out&gt;, Result=..., Info=..., DependentDeduction=&lt;optimized out&gt;, IgnoreConstraints=&lt;optimized out&gt;, FailedTSC=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5178
#<!-- -->4  0x000055555845b2fd in clang::Sema::deduceVarTypeFromInitializer (this=this@<!-- -->entry=0x55555e349790, VDecl=&lt;optimized out&gt;, VDecl@<!-- -->entry=0x55555e37dcf0, Name=..., Name@<!-- -->entry=..., Type=..., TSI=0x0, TSI@<!-- -->entry=0x55555e37dcb0, Range=..., DirectInit=false, Init=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:12904
#<!-- -->5  0x000055555845bc01 in clang::Sema::DeduceVariableDeclarationType (this=this@<!-- -->entry=0x55555e349790, VDecl=VDecl@<!-- -->entry=0x55555e37dcf0, DirectInit=&lt;optimized out&gt;, Init=&lt;optimized out&gt;, Init@<!-- -->entry=0x55555e37dd58) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:12941
#<!-- -->6  0x000055555845c990 in clang::Sema::AddInitializerToDecl (this=0x55555e349790, RealDecl=0x55555e37dcf0, Init=0x55555e37dd58, DirectInit=164) at /home/user/repo/llvm-project/clang/lib/Sema/SemaDecl.cpp:13311
#<!-- -->7  0x0000555557d358e7 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes (this=this@<!-- -->entry=0x55555e356b60, D=..., TemplateInfo=..., FRI=FRI@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2823
#<!-- -->8  0x0000555557d30890 in clang::Parser::ParseDeclGroup (this=this@<!-- -->entry=0x55555e356b60, DS=..., Context=Context@<!-- -->entry=clang::DeclaratorContext::Block, Attrs=..., TemplateInfo=..., DeclEnd=0x7fffffffb020, FRI=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2516
#<!-- -->9  0x0000555557d2e5d0 in clang::Parser::ParseSimpleDeclaration (this=this@<!-- -->entry=0x55555e356b60, Context=clang::DeclaratorContext::Block, Context@<!-- -->entry=clang::DeclaratorContext::File, DeclEnd=..., DeclAttrs=..., DeclSpecAttrs=..., RequireSemi=52, FRI=0x0, DeclSpecStart=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2135
#<!-- -->10 0x0000555557d2dabc in clang::Parser::ParseDeclaration (this=0x55555e356b60, Context=4294929888, DeclEnd=..., DeclAttrs=..., DeclSpecAttrs=..., DeclSpecStart=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2028
#<!-- -->11 0x0000555557e50a55 in clang::Parser::ParseStatementOrDeclarationAfterAttributes (this=this@<!-- -->entry=0x55555e356b60, Stmts=..., StmtCtx=StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0, CXX11Attrs=..., GNUAttrs=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:259
#<!-- -->12 0x0000555557e4e4f5 in clang::Parser::ParseStatementOrDeclaration (this=this@<!-- -->entry=0x55555e356b60, Stmts=..., StmtCtx=clang::Parser::ParsedStmtContext::SubStmt, StmtCtx@<!-- -->entry=clang::Parser::ParsedStmtContext::Compound, TrailingElseLoc=TrailingElseLoc@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:124
#<!-- -->13 0x0000555557e5f14c in clang::Parser::ParseCompoundStatementBody (this=this@<!-- -->entry=0x55555e356b60, isStmtExpr=96) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:1248
#<!-- -->14 0x0000555557e612da in clang::Parser::ParseFunctionStatementBody (this=0x55555e356b60, Decl=0x55555e35d198, BodyScope=...) at /home/user/repo/llvm-project/clang/lib/Parse/ParseStmt.cpp:2526
#<!-- -->15 0x0000555557cf1009 in clang::Parser::ParseFunctionDefinition (this=0x55555e356b60, D=..., TemplateInfo=..., LateParsedAttrs=0x7fffffffba80) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1525
#<!-- -->16 0x0000555557d31fe5 in clang::Parser::ParseDeclGroup (this=0x55555e356b60, DS=..., Context=clang::DeclaratorContext::File, Attrs=..., TemplateInfo=..., DeclEnd=0x0, FRI=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/ParseDecl.cpp:2402
#<!-- -->17 0x0000555557ceed0b in clang::Parser::ParseDeclOrFunctionDefInternal (this=this@<!-- -->entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=..., AS=AS@<!-- -->entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1249
#<!-- -->18 0x0000555557cedee1 in clang::Parser::ParseDeclarationOrFunctionDefinition (this=this@<!-- -->entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=DS@<!-- -->entry=0x55555e356b60, AS=1580191984, AS@<!-- -->entry=clang::AS_none) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1271
#<!-- -->19 0x0000555557cec26d in clang::Parser::ParseExternalDeclaration (this=this@<!-- -->entry=0x55555e356b60, Attrs=..., DeclSpecAttrs=..., DS=DS@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:1074
#<!-- -->20 0x0000555557ce8f6b in clang::Parser::ParseTopLevelDecl (this=this@<!-- -->entry=0x55555e356b60, Result=..., ImportState=@<!-- -->0x7fffffffd724: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:763
#<!-- -->21 0x0000555557ce824f in clang::Parser::ParseFirstTopLevelDecl (this=0x55555e356b60, Result=..., ImportState=@<!-- -->0x7fffffffd724: clang::Sema::ModuleImportState::FirstDecl) at /home/user/repo/llvm-project/clang/lib/Parse/Parser.cpp:608
#<!-- -->22 0x0000555557cdff8d in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Parse/ParseAST.cpp:170
#<!-- -->23 0x0000555557b850e6 in clang::FrontendAction::Execute (this=0x55555e2c9a90) at /home/user/repo/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1078
#<!-- -->24 0x0000555557a4ae01 in clang::CompilerInstance::ExecuteAction (this=0x7fffffffd8d8, Act=...) at /home/user/repo/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061
#<!-- -->25 0x0000555557a02502 in clang::tooling::FrontendActionFactory::runInvocation (this=0x55555e2c1ef0, Invocation=..., Files=0x55555e2c6920, PCHContainerOps=..., DiagConsumer=0x7fffffffdaa0) at /home/user/repo/llvm-project/clang/lib/Tooling/Tooling.cpp:465
#<!-- -->26 0x0000555555acd136 in clang_fuzzer::HandleCXX (S="int main ( ) { int a = \"hello\" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - -"..., FileName=&lt;optimized out&gt;, ExtraArgs=std::vector of length 1, capacity 1 = {...}) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/handle-cxx/handle_cxx.cpp:49
#<!-- -->27 0x0000555555accad4 in LLVMFuzzerTestOneInput (data=0x7ffff4be4010 "int main ( ) { int a = \"hello\" ; auto ( * * f2 ) ( float ( * f2 ) ( const char * state_t [ 11 ] [ 1 / ( ( ( 1 / ( ( ( 1 + ( ( ( 1.234567e10 / ( ! ( ( 0.0 / ( ( __builtin_expect ( ( ( 1 + ( ( 1 + ( - -"..., size=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
#<!-- -->28 0x000055555c6f96ee in ExecuteFilesOnyByOne (argc=2, argv=0x7fffffffe328, callback=callback@<!-- -->entry=0x555555acc990 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:256
#<!-- -->29 0x000055555c6f94de in LLVMFuzzerRunDriver (argcp=argcp@<!-- -->entry=0x7fffffffe1f4, argvp=argvp@<!-- -->entry=0x7fffffffe1f8, callback=0x555555acc990 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:377
#<!-- -->30 0x000055555c6f901e in main (argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe328) at aflpp_driver.c:312
#<!-- -->31 0x00007ffff7a63d90 in __libc_start_call_main (main=main@<!-- -->entry=0x55555c6f8f60 &lt;main&gt;, argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe328) at ../sysdeps/nptl/libc_start_call_main.h:58
#<!-- -->32 0x00007ffff7a63e40 in __libc_start_main_impl (main=0x55555c6f8f60 &lt;main&gt;, argc=2, argv=0x7fffffffe328, init=&lt;optimized out&gt;, fini=&lt;optimized out&gt;, rtld_fini=&lt;optimized out&gt;, stack_end=0x7fffffffe318) at ../csu/libc-start.c:392
#<!-- -->33 0x0000555555acc8b5 in _start ()
```

</details>


---

### Comment 3 - k-arrows

Reduced and modified from the original:
https://godbolt.org/z/z6KjP5d56
```cpp
void foo() {
  auto (*f2)(float (*f2)(char *s1[*"" + ""[10]])()(char *s2[4])) = 0;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:5322: clang::TemplateDeductionResult clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*): Assertion `!FuncParam.isNull() && "substituting template parameter for 'auto' failed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:69: current parser token ';'
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
 #0 0x00000000042ed758 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ed758)
 #1 0x00000000042eab84 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab84)
 #2 0x000000000422f928 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007611e6442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007611e64969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007611e6442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007611e64287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007611e642871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007611e6439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000074e8e81 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74e8e81)
#10 0x0000000006dd23f5 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd23f5)
#11 0x0000000006dd8497 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd8497)
#12 0x0000000006e062be clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e062be)
#13 0x0000000006a55201 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55201)
#14 0x0000000006a6245a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6245a)
#15 0x0000000006a6b16f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6b16f)
#16 0x0000000006a6b63c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6b63c)
#17 0x0000000006b1d024 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1d024)
#18 0x0000000006b1d8b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1d8b6)
#19 0x0000000006b261fe clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b261fe)
#20 0x0000000006b26a4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b26a4a)
#21 0x0000000006a2c9ab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2c9ab)
#22 0x0000000006a62e6d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a62e6d)
#23 0x0000000006a1fb0b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1fb0b)
#24 0x0000000006a202ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a202ff)
#25 0x0000000006a2812a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2812a)
#26 0x0000000006a290c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a290c5)
#27 0x0000000006a29570 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a29570)
#28 0x0000000006a0a113 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0a113)
#29 0x0000000004c4ea88 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4ea88)
#30 0x0000000004f45385 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f45385)
#31 0x0000000004ec561e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec561e)
#32 0x000000000503f70d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503f70d)
#33 0x0000000000de5b8c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b8c)
#34 0x0000000000ddc5da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000ddc75d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004cc13d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x000000000422fdc4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422fdc4)
#38 0x0000000004cc19ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004c824d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c824d2)
#40 0x0000000004c8347e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8347e)
#41 0x0000000004c8a8d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8a8d5)
#42 0x0000000000de1f81 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f81)
#43 0x0000000000c8cfd4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cfd4)
#44 0x00007611e6429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007611e6429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000ddc075 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc075)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - shafik

@k-arrows thank you, I do appreciate you making the effort to get these issues into shape but that does not change my original comment that folk posting fuzzing issues to this project need to put the effort into posting it well the first time and not rely on folks coming in to get it into shape.

---

### Comment 5 - shafik

Goes back to clang-5: https://godbolt.org/z/69KKf9r6q

---

