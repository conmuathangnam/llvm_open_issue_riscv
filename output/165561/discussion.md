# [clang-fuzzer] Crash in `numStructUnionElements`

**Author:** gal1ium
**URL:** https://github.com/llvm/llvm-project/issues/165561

## Body

Hi, while testing clang by the fuzzing driver `clang-fuzzer`, it found a crashing case:

Version: 531fd45e9238d0485e3268aaf14ae15d01c7740f

Flags:
```
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="lld;clang;compiler-rt" ../llvm -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_BUILD_RUNTIME=Off -DLLVM_BUILD_INSTRUMENTED_COVERAGE=On -DCLANG_ENABLE_PROTO_FUZZER=ON
ninja clang-fuzzer
```

PoC:
```c
truct g{truct t o0}g[]{[0].o0[0]{}}
```

Reproduction:
`./bin/clang-fuzzer ./poc`

Crashing thread backtrace:
```
#0  0x000055555b758531 in (anonymous namespace)::InitListChecker::numStructUnionElements (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1137

#1  0x000055555b7591f1 in (anonymous namespace)::InitListChecker::createInitListExpr (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3525

#2  0x000055555b765ccb in (anonymous namespace)::InitListChecker::getStructuredSubobjectInit (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3489

#3  0x000055555b7b81fb in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:2869

#4  0x000055555b7b8824 in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3187

#5  0x000055555b7b77f0 in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3398

#6  0x000055555b7a314f in (anonymous namespace)::InitListChecker::CheckArrayType (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:2231

#7  0x000055555b7a6e88 in (anonymous namespace)::InitListChecker::CheckListElementTypes (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1463

#8  0x000055555b7a8659 in (anonymous namespace)::InitListChecker::CheckExplicitInitList (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1345

#9  0x000055555b7aa27d in (anonymous namespace)::InitListChecker::InitListChecker (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1108

#10 0x000055555b778c68 in TryListInitialization (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:5171

#11 0x000055555b77cb28 in clang::InitializationSequence::InitializeFrom (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:6641

#12 0x000055555af5610f in clang::Sema::AddInitializerToDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaDecl.cpp:13953

#13 0x000055555a03324e in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/include/clang/AST/TemplateBase.h:44

#14 0x000055555a05ca4b in clang::Parser::ParseDeclGroup (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/ParseDecl.cpp:2356

#15 0x0000555559fea9b0 in clang::Parser::ParseDeclOrFunctionDefInternal (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1187

#16 0x0000555559fec23b in clang::Parser::ParseDeclarationOrFunctionDefinition (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1209

#17 0x000055555a0024f0 in clang::Parser::ParseExternalDeclaration (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1032

#18 0x000055555a006d15 in clang::Parser::ParseTopLevelDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:745

#19 0x000055555a0079a0 in clang::Parser::ParseFirstTopLevelDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:601

#20 0x0000555559fbff86 in clang::ParseAST (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/ParseAST.cpp:169

#21 0x0000555559ec1b28 in clang::ASTFrontendAction::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/FrontendAction.cpp:1432

#22 0x00005555564f8aa2 in clang::CodeGenAction::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/CodeGen/CodeGenAction.cpp:1109

#23 0x0000555559ed30d2 in clang::FrontendAction::Execute (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/FrontendAction.cpp:1312

#24 0x0000555559da3af8 in clang::CompilerInstance::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/CompilerInstance.cpp:1003

#25 0x0000555559d46f8e in clang::tooling::FrontendActionFactory::runInvocation (fuzz-binaries/clang-fuzzer)
                       443: bool clang::tooling::FrontendActionFactory::runInvocation(this = (clang::tooling::FrontendActionFactory * const)0x5555617751a0, Invocation = (std::shared_ptr<clang::CompilerInvocation>)std::shared_ptr<clang::CompilerInvocation> (empty) = {get() = 0x0}, Files = (clang::FileManager *)0x5555617715c0, PCHContainerOps = (std::shared_ptr<clang::PCHContainerOperations>)std::shared_ptr<clang::PCHContainerOperations> (empty) = {get() = 0x0}, DiagConsumer = (clang::DiagnosticConsumer *)0x7fffffff9990) {
                       |||:
                       ---: }
                       at /usr/include/c++/9/bits/unique_ptr.h:154

#26 0x00005555564db128 in clang_fuzzer::HandleCXX (fuzz-binaries/clang-fuzzer)
                         23: void clang_fuzzer::HandleCXX(S = (const std::string &)"truct g{truct t o0}g[]{[0].o0[0]{}}", FileName = (const char *)0x5555558e74b7 "./test.cc", ExtraArgs = (const std::vector<char const*, std::allocator<char const*> > &)std::vector of length 1, capacity 1 = {0x5555559613a3 "-O2"}) {
                       ||||:
                       1386:       template<typename _Yp, typename _Yp2 = typename remove_cv<_Yp>::type>
                       1387: 	typename enable_if<!__has_esft_base<_Yp2>::value>::type
                       1388: 	_M_enable_shared_from_this_with(_Yp*) noexcept
                       ||||:
                       ----: }
                       at /usr/include/c++/9/bits/shared_ptr_base.h:1388

#27 0x00005555564d3832 in LLVMFuzzerTestOneInput (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (gal1ium)

<details>
Hi, while testing clang by the fuzzing driver `clang-fuzzer`, it found a crashing case:

Version: 531fd45e9238d0485e3268aaf14ae15d01c7740f

Flags:
```
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="lld;clang;compiler-rt" ../llvm -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_BUILD_RUNTIME=Off -DLLVM_BUILD_INSTRUMENTED_COVERAGE=On -DCLANG_ENABLE_PROTO_FUZZER=ON
ninja clang-fuzzer
```

PoC:
```c
truct g{truct t o0}g[]{[0].o0[0]{}}
```

Reproduction:
`./bin/clang-fuzzer ./poc`

Crashing thread backtrace:
```
#<!-- -->0  0x000055555b758531 in (anonymous namespace)::InitListChecker::numStructUnionElements (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1137

#<!-- -->1  0x000055555b7591f1 in (anonymous namespace)::InitListChecker::createInitListExpr (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3525

#<!-- -->2  0x000055555b765ccb in (anonymous namespace)::InitListChecker::getStructuredSubobjectInit (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3489

#<!-- -->3  0x000055555b7b81fb in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:2869

#<!-- -->4  0x000055555b7b8824 in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3187

#<!-- -->5  0x000055555b7b77f0 in (anonymous namespace)::InitListChecker::CheckDesignatedInitializer (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:3398

#<!-- -->6  0x000055555b7a314f in (anonymous namespace)::InitListChecker::CheckArrayType (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:2231

#<!-- -->7  0x000055555b7a6e88 in (anonymous namespace)::InitListChecker::CheckListElementTypes (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1463

#<!-- -->8  0x000055555b7a8659 in (anonymous namespace)::InitListChecker::CheckExplicitInitList (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1345

#<!-- -->9  0x000055555b7aa27d in (anonymous namespace)::InitListChecker::InitListChecker (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:1108

#<!-- -->10 0x000055555b778c68 in TryListInitialization (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:5171

#<!-- -->11 0x000055555b77cb28 in clang::InitializationSequence::InitializeFrom (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaInit.cpp:6641

#<!-- -->12 0x000055555af5610f in clang::Sema::AddInitializerToDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Sema/SemaDecl.cpp:13953

#<!-- -->13 0x000055555a03324e in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/include/clang/AST/TemplateBase.h:44

#<!-- -->14 0x000055555a05ca4b in clang::Parser::ParseDeclGroup (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/ParseDecl.cpp:2356

#<!-- -->15 0x0000555559fea9b0 in clang::Parser::ParseDeclOrFunctionDefInternal (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1187

#<!-- -->16 0x0000555559fec23b in clang::Parser::ParseDeclarationOrFunctionDefinition (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1209

#<!-- -->17 0x000055555a0024f0 in clang::Parser::ParseExternalDeclaration (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:1032

#<!-- -->18 0x000055555a006d15 in clang::Parser::ParseTopLevelDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:745

#<!-- -->19 0x000055555a0079a0 in clang::Parser::ParseFirstTopLevelDecl (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/Parser.cpp:601

#<!-- -->20 0x0000555559fbff86 in clang::ParseAST (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Parse/ParseAST.cpp:169

#<!-- -->21 0x0000555559ec1b28 in clang::ASTFrontendAction::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/FrontendAction.cpp:1432

#<!-- -->22 0x00005555564f8aa2 in clang::CodeGenAction::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/CodeGen/CodeGenAction.cpp:1109

#<!-- -->23 0x0000555559ed30d2 in clang::FrontendAction::Execute (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/FrontendAction.cpp:1312

#<!-- -->24 0x0000555559da3af8 in clang::CompilerInstance::ExecuteAction (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/lib/Frontend/CompilerInstance.cpp:1003

#<!-- -->25 0x0000555559d46f8e in clang::tooling::FrontendActionFactory::runInvocation (fuzz-binaries/clang-fuzzer)
                       443: bool clang::tooling::FrontendActionFactory::runInvocation(this = (clang::tooling::FrontendActionFactory * const)0x5555617751a0, Invocation = (std::shared_ptr&lt;clang::CompilerInvocation&gt;)std::shared_ptr&lt;clang::CompilerInvocation&gt; (empty) = {get() = 0x0}, Files = (clang::FileManager *)0x5555617715c0, PCHContainerOps = (std::shared_ptr&lt;clang::PCHContainerOperations&gt;)std::shared_ptr&lt;clang::PCHContainerOperations&gt; (empty) = {get() = 0x0}, DiagConsumer = (clang::DiagnosticConsumer *)0x7fffffff9990) {
                       |||:
                       ---: }
                       at /usr/include/c++/9/bits/unique_ptr.h:154

#<!-- -->26 0x00005555564db128 in clang_fuzzer::HandleCXX (fuzz-binaries/clang-fuzzer)
                         23: void clang_fuzzer::HandleCXX(S = (const std::string &amp;)"truct g{truct t o0}g[]{[0].o0[0]{}}", FileName = (const char *)0x5555558e74b7 "./test.cc", ExtraArgs = (const std::vector&lt;char const*, std::allocator&lt;char const*&gt; &gt; &amp;)std::vector of length 1, capacity 1 = {0x5555559613a3 "-O2"}) {
                       ||||:
                       1386:       template&lt;typename _Yp, typename _Yp2 = typename remove_cv&lt;_Yp&gt;::type&gt;
                       1387: 	typename enable_if&lt;!__has_esft_base&lt;_Yp2&gt;::value&gt;::type
                       1388: 	_M_enable_shared_from_this_with(_Yp*) noexcept
                       ||||:
                       ----: }
                       at /usr/include/c++/9/bits/shared_ptr_base.h:1388

#<!-- -->27 0x00005555564d3832 in LLVMFuzzerTestOneInput (fuzz-binaries/clang-fuzzer)
                       at /src/llvm/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
```
</details>


---

### Comment 2 - shafik

So if you are going to submit bug reports based on fuzzing, we need you to do a few things to make this more effective:

- godbolt links w/ reproducer really make a huge difference
- How far back does this go, looks like clang-10: https://godbolt.org/z/PshzjMf4n knowing if this is a recent regression is important 
- You should look for duplicates and point them out in the report
- You need to use an assertions build so we can see any assertions, they are more useful because usually they fail earlier and point to the source of the bug better.
- We need you to include the assertion and the backtrace as it shows up in godbolt, this helps us to discover duplicates later on 
- Please point out this was generated via fuzzing

Finally we can't really process a large amount of fuzzing based reports, so you should prioritize recent regressions if you must.


---

### Comment 3 - gal1ium

> So if you are going to submit bug reports based on fuzzing, we need you to do a few things to make this more effective:
> 
> * godbolt links w/ reproducer really make a huge difference
> * How far back does this go, looks like clang-10: https://godbolt.org/z/PshzjMf4n knowing if this is a recent regression is important
> * You should look for duplicates and point them out in the report
> * You need to use an assertions build so we can see any assertions, they are more useful because usually they fail earlier and point to the source of the bug better.
> * We need you to include the assertion and the backtrace as it shows up in godbolt, this helps us to discover duplicates later on
> * Please point out this was generated via fuzzing
> 
> Finally we can't really process a large amount of fuzzing based reports, so you should prioritize recent regressions if you must.

Got it, thanks! I'll revise the reports and prioritize recent ones.

---

