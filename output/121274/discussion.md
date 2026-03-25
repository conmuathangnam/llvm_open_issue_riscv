# Front-end crashes when `__PRETTY_FUNCTION__` (or `__FUNCSIG__`) appears in trailing return type

**Author:** jhcarl0814
**URL:** https://github.com/llvm/llvm-project/issues/121274
**Status:** Closed
**Labels:** clang:frontend, crash, lambda
**Closed Date:** 2025-01-12T14:42:05Z

## Body

On Compiler Explorer `clang` crashes when `__PRETTY_FUNCTION__` appears in trailing return type. On my computer `clang-cl.exe` crashes when `__FUNCSIG__` appears in trailing return type.

[Compiler Explorer, x64-64 clang (trunk), -std=c++26 -O2 -Wall -pedantic -pthread](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:86,endLineNumber:4,positionColumn:86,positionLineNumber:4,selectionStartColumn:86,selectionStartLineNumber:4,startColumn:86,startLineNumber:4),source:'%0Aint+main()%0A%7B%0A++++%5B%5D()+-%3E+decltype(static_cast%3Cchar+const*%3E(__PRETTY_FUNCTION__))+%7B+return+nullptr%3B+%7D%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:38.67141162514828,n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang_trunk,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23+-O2+-Wall+-pedantic+-pthread',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+(trunk)+(Editor+%231)',t:'0'),(h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B26+-O2+-Wall+-pedantic+-pthread',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',tree:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),l:'4',m:61.32858837485172,n:'0',o:'',s:1,t:'0')),k:100,l:'3',n:'0',o:'',t:'0')),version:4):
```cpp
int main()
{
    []() -> decltype(static_cast<char const*>(__PRETTY_FUNCTION__)) { return nullptr; };
}
```
Could not execute the program
Build failed
Compiler returned: 254
Compiler stderr
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241228/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241228/lib/clang/20 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20241228/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -Wall -pedantic -std=c++26 -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-b47b85.o -x c++ <source>
1.	<source>:4:47: current parser token '__PRETTY_FUNCTION__'
2.	<source>:3:1: parsing function body 'main'
3.	<source>:3:1: in compound statement ('{}')
4.	<source>:4:5: lambda expression parsing
 #0 0x0000000003a32498 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x3a32498)
 #1 0x0000000003a3005c SignalHandler(int) Signals.cpp:0:0
 #2 0x00007eaa64842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00000000073767a4 clang::PredefinedExpr::ComputeName[abi:cxx11](clang::PredefinedIdentKind, clang::Decl const*, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x73767a4)
 #4 0x00000000067639bf clang::Sema::BuildPredefinedExpr(clang::SourceLocation, clang::PredefinedIdentKind) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x67639bf)
 #5 0x00000000062ef620 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62ef620)
 #6 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
 #7 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
 #8 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
 #9 0x000000000630ab32 clang::Parser::ParseCXXCasts() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x630ab32)
#10 0x00000000062ef70e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62ef70e)
#11 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
#12 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
#13 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
#14 0x00000000062d1cf6 clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62d1cf6)
#15 0x000000000630d037 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x630d037)
#16 0x000000000626d522 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626d522)
#17 0x00000000062b24c4 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b24c4)
#18 0x00000000062b3832 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b3832)
#19 0x00000000062b3cef clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b3cef)
#20 0x00000000063128bc clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x63128bc)
#21 0x0000000006312e0b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6312e0b)
#22 0x00000000062f0805 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f0805)
#23 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
#24 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
#25 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
#26 0x00000000063833c7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x63833c7)
#27 0x0000000006376b57 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6376b57)
#28 0x000000000637754d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x637754d)
#29 0x0000000006378864 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6378864)
#30 0x0000000006379a1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6379a1a)
#31 0x0000000006275c56 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6275c56)
#32 0x00000000062bfbdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62bfbdf)
#33 0x000000000626e207 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626e207)
#34 0x000000000626f01e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626f01e)
#35 0x0000000006278932 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6278932)
#36 0x000000000627a1c8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x627a1c8)
#37 0x000000000627a710 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x627a710)
#38 0x0000000006268833 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6268833)
#39 0x000000000436e385 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x436e385)
#40 0x000000000463a081 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x463a081)
#41 0x00000000045b9fcb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x45b9fcb)
#42 0x00000000047240b3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x47240b3)
#43 0x0000000000d01daa cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xd01daa)
#44 0x0000000000cfa5fd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#45 0x0000000000cfd3cf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xcfd3cf)
#46 0x0000000000bc6874 main (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xbc6874)
#47 0x00007eaa64829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007eaa64829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000cfa095 _start (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xcfa095)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Han Jiang (jhcarl0814)

<details>
On Compiler Explorer `clang` crashes when `__PRETTY_FUNCTION__` appears in trailing return type. On my computer `clangcl.exe` crashes when `__FUNCSIG__` appears in trailing return type.

[Compiler Explorer, x64-64 clang (trunk), -std=c++26 -O2 -Wall -pedantic -pthread](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:86,endLineNumber:4,positionColumn:86,positionLineNumber:4,selectionStartColumn:86,selectionStartLineNumber:4,startColumn:86,startLineNumber:4),source:'%0Aint+main()%0A%7B%0A++++%5B%5D()+-%3E+decltype(static_cast%3Cchar+const*%3E(__PRETTY_FUNCTION__))+%7B+return+nullptr%3B+%7D%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:38.67141162514828,n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang_trunk,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23+-O2+-Wall+-pedantic+-pthread',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+(trunk)+(Editor+%231)',t:'0'),(h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B26+-O2+-Wall+-pedantic+-pthread',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',tree:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),l:'4',m:61.32858837485172,n:'0',o:'',s:1,t:'0')),k:100,l:'3',n:'0',o:'',t:'0')),version:4):
```cpp
int main()
{
    []() -&gt; decltype(static_cast&lt;char const*&gt;(__PRETTY_FUNCTION__)) { return nullptr; };
}
```
Could not execute the program
Build failed
Compiler returned: 254
Compiler stderr
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241228/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241228/lib/clang/20 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20241228/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -Wall -pedantic -std=c++26 -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-b47b85.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:4:47: current parser token '__PRETTY_FUNCTION__'
2.	&lt;source&gt;:3:1: parsing function body 'main'
3.	&lt;source&gt;:3:1: in compound statement ('{}')
4.	&lt;source&gt;:4:5: lambda expression parsing
 #<!-- -->0 0x0000000003a32498 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x3a32498)
 #<!-- -->1 0x0000000003a3005c SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007eaa64842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000073767a4 clang::PredefinedExpr::ComputeName[abi:cxx11](clang::PredefinedIdentKind, clang::Decl const*, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x73767a4)
 #<!-- -->4 0x00000000067639bf clang::Sema::BuildPredefinedExpr(clang::SourceLocation, clang::PredefinedIdentKind) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x67639bf)
 #<!-- -->5 0x00000000062ef620 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62ef620)
 #<!-- -->6 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
 #<!-- -->7 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
 #<!-- -->8 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
 #<!-- -->9 0x000000000630ab32 clang::Parser::ParseCXXCasts() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x630ab32)
#<!-- -->10 0x00000000062ef70e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62ef70e)
#<!-- -->11 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
#<!-- -->12 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
#<!-- -->13 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
#<!-- -->14 0x00000000062d1cf6 clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62d1cf6)
#<!-- -->15 0x000000000630d037 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x630d037)
#<!-- -->16 0x000000000626d522 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626d522)
#<!-- -->17 0x00000000062b24c4 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b24c4)
#<!-- -->18 0x00000000062b3832 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b3832)
#<!-- -->19 0x00000000062b3cef clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62b3cef)
#<!-- -->20 0x00000000063128bc clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x63128bc)
#<!-- -->21 0x0000000006312e0b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6312e0b)
#<!-- -->22 0x00000000062f0805 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f0805)
#<!-- -->23 0x00000000062f10ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f10ca)
#<!-- -->24 0x00000000062f1299 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f1299)
#<!-- -->25 0x00000000062f6539 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62f6539)
#<!-- -->26 0x00000000063833c7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x63833c7)
#<!-- -->27 0x0000000006376b57 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6376b57)
#<!-- -->28 0x000000000637754d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x637754d)
#<!-- -->29 0x0000000006378864 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6378864)
#<!-- -->30 0x0000000006379a1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6379a1a)
#<!-- -->31 0x0000000006275c56 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6275c56)
#<!-- -->32 0x00000000062bfbdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x62bfbdf)
#<!-- -->33 0x000000000626e207 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626e207)
#<!-- -->34 0x000000000626f01e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x626f01e)
#<!-- -->35 0x0000000006278932 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6278932)
#<!-- -->36 0x000000000627a1c8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x627a1c8)
#<!-- -->37 0x000000000627a710 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x627a710)
#<!-- -->38 0x0000000006268833 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x6268833)
#<!-- -->39 0x000000000436e385 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x436e385)
#<!-- -->40 0x000000000463a081 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x463a081)
#<!-- -->41 0x00000000045b9fcb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x45b9fcb)
#<!-- -->42 0x00000000047240b3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0x47240b3)
#<!-- -->43 0x0000000000d01daa cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xd01daa)
#<!-- -->44 0x0000000000cfa5fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->45 0x0000000000cfd3cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xcfd3cf)
#<!-- -->46 0x0000000000bc6874 main (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xbc6874)
#<!-- -->47 0x00007eaa64829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007eaa64829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000cfa095 _start (/opt/compiler-explorer/clang-trunk-20241228/bin/clang-20+0xcfa095)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```
</details>


---

