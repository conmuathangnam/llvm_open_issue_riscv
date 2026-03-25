# ICE on 'requires' for recursive function template

**Author:** rollbear
**URL:** https://github.com/llvm/llvm-project/issues/64462

## Body

The following invalid code ICEs Clang++ 16.0.6. This is a reduction from a case I believe to be valid, and that was accepted by Clang++-15 (https://godbolt.org/z/axxY8TGWz)

```c++
auto c(auto, auto ... fs){return [](auto) requires(c(fs...)){};}
auto f = c([]{})(5);
```

with the backtrace:
```console
$ clang++-16 -std=c++2b err.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-16/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name err.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -fcoverage-compilation-dir=/ -resource-dir /usr/lib/llvm-16/lib/clang/16 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward -internal-isystem /usr/lib/llvm-16/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++2b -fdeprecated-macro -fdebug-compilation-dir=/ -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/err-6f78a7.o -x c++ err.cpp
1.      err.cpp:2:19: current parser token ')'
 #0 0x00007f899cf08c16 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfc1c16)
 #1 0x00007f899cf06e60 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfbfe60)
 #2 0x00007f899cf093eb (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfc23eb)
 #3 0x00007f899b9ff4b0 (/lib/x86_64-linux-gnu/libc.so.6+0x3c4b0)
 #4 0x00007f89a500b3ee clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, std::optional<unsigned int>&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1aa13ee)
 #5 0x00007f89a4fa2571 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a38571)
 #6 0x00007f89a4fa85ab (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a3e5ab)
 #7 0x00007f89a4fa1f82 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a37f82)
 #8 0x00007f89a49cf387 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1465387)
 #9 0x00007f89a49ce279 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1464279)
#10 0x00007f89a49c94df (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x145f4df)
#11 0x00007f89a49c8e8c clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x145ee8c)
#12 0x00007f89a49cb785 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1461785)
#13 0x00007f89a4f415df clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x19d75df)
#14 0x00007f89a4f94ab7 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a2aab7)
#15 0x00007f89a48966dd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x132c6dd)
#16 0x00007f89a4f42a81 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x19d8a81)
#17 0x00007f89a4e1f5f0 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18b55f0)
#18 0x00007f89a4e1ffc3 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18b5fc3)
#19 0x00007f89a4e36e67 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18cce67)
#20 0x00007f89a4b50681 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x15e6681)
#21 0x00007f89a4b687fd clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x15fe7fd)
#22 0x00007f89a417fa4c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc15a4c)
#23 0x00007f89a4180965 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc16965)
#24 0x00007f89a417cbe1 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc12be1)
#25 0x00007f89a414f26b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbe526b)
#26 0x00007f89a414c637 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbe2637)
#27 0x00007f89a41fc557 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc92557)
#28 0x00007f89a41fbf79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc91f79)
#29 0x00007f89a41fb217 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc91217)
#30 0x00007f89a41f95be clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc8f5be)
#31 0x00007f89a413933e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbcf33e)
#32 0x00007f89a5d503b5 clang::FrontendAction::Execute() (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x27e63b5)
#33 0x00007f89a5ccac94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x2760c94)
#34 0x00007f89a5dc9434 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x285f434)
#35 0x000055dc45f3b116 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-16/bin/clang+0x15116)
#36 0x000055dc45f38280 (/usr/lib/llvm-16/bin/clang+0x12280)
#37 0x000055dc45f37f89 clang_main(int, char**) (/usr/lib/llvm-16/bin/clang+0x11f89)
#38 0x00007f899b9e6a90 (/lib/x86_64-linux-gnu/libc.so.6+0x23a90)
#39 0x00007f899b9e6b49 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x23b49)
#40 0x000055dc45f348b5 _start (/usr/lib/llvm-16/bin/clang+0xe8b5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 16.0.6 (++20230710042020+7cbf1a259152-1~exp1~20230710162037.55)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/err-c1b700.cpp
clang: note: diagnostic msg: /tmp/err-c1b700.sh
clang: note: diagnostic msg: 

********************
```

`/tmp/err-c1b700.cpp`:
```cpp
# 1 "err.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 451 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "err.cpp" 2
auto c(auto, auto ... fs){return [](auto) requires(c(fs...)){};}
auto f = c([]{})(5);
```

`/tmp/err-c1b700.sh`:
```shell
# Crash reproducer for Ubuntu clang version 16.0.6 (++20230710042020+7cbf1a259152-1~exp1~20230710162037.55)
# Driver args: "--driver-mode=g++" "-std=c++2b" "err.cpp"
# Original command:  "/usr/lib/llvm-16/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "err.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/" "-resource-dir" "/usr/lib/llvm-16/lib/clang/16" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward" "-internal-isystem" "/usr/lib/llvm-16/lib/clang/16/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-std=c++2b" "-fdeprecated-macro" "-fdebug-compilation-dir=/" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/err-6f78a7.o" "-x" "c++" "err.cpp"
 "/usr/lib/llvm-16/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "err.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/" "-std=c++2b" "-fdeprecated-macro" "-fdebug-compilation-dir=/" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "err-c1b700.cpp"
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - 0x59616e

The first bad commit is babdef27c503, but the diagnostic message before this commit is weird, as it doesn't point out that the substitution failure is due to the unsatisfied constraint. So this may not be a regression. The issue has always been there, but had not been exhumed until now.

I will keep investigating.

---

### Comment 3 - 0x59616e

By the way, should we add a C++20 tag ?

---

### Comment 4 - EugeneZelenko

Godbolt reproducer is compiled with `C++23`.

---

### Comment 5 - AaronBallman

Reduced slightly more:
```
inline constexpr auto neg = []<typename T>(T t) { return -t; };

inline constexpr auto add = []<typename T, typename ... Ts>(T t, Ts ... ts) {
        return (t + ... + ts);
};

template <typename F, typename ... Fs>
constexpr decltype(auto) compose(F f, Fs ... fs)
{
    if constexpr (sizeof...(fs) == 0)
    {
        return [f]<typename T>(T&& t) { f(t); };
    }
    else
    {
        return [=]<typename T>(T&& t) requires(requires{f(compose(fs...)(t));}) { return f(compose(fs...)(t)); };
    }
};

constexpr auto x = compose(add(3), neg)(-5);
```
https://godbolt.org/z/f6Garf4rE

---

### Comment 6 - llvmbot

@llvm/issue-subscribers-c-20

---

### Comment 7 - 0x59616e

My conjecture is that `Sema::SetupConstraintScope` does not do its job comprehensively.

Also, I want to kindly ask where in the C++ standard has the specification of checking the contraint of a lambda defined within a template function.

Thanks !

---

### Comment 8 - MagentaTreehouse

Requesting to reopen this issue. The original repro now causes Clang to segfault.

See https://compiler-explorer.com/z/shKs3Pvv1.

<details>

<summary>Stack dump (nondeterministic on Compiler Explorer)</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20250211/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20250211/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20250211/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O3 -Wall -Wextra -Wpedantic -Wconversion -std=c++2b -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-8c05ff.o -x c++ <source>
1.	<source>:69:43: current parser token ')'
2.	<source>:58:16: instantiating function definition 'compose((lambda at <source>:29:29))::(anonymous class)::operator()<int>'
3.	<source>:58:16: LLVM IR generation of declaration 'compose((lambda at <source>:29:29))::(anonymous class)::operator()'
4.	<source>:58:16: Mangling declaration 'compose((lambda at <source>:29:29))::(anonymous class)::operator()'
  #0 0x0000000003e72298 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20250211/bin/clang-21+0x3e72298)
  #1 0x0000000003e6fc54 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x0000785a76c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x0000000007937b22 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #4 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #5 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #6 0x0000000007926a07 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #7 0x0000000007937b7b (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #8 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #9 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #10 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #11 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #12 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #13 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #14 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #15 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #16 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #17 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #18 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #19 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #20 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #21 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #22 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #23 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #24 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #25 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #26 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #27 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #28 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #29 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #30 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #31 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #32 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #33 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #34 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #35 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #36 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #37 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #38 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #39 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #40 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #41 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #42 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #43 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #44 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #45 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #46 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #47 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #48 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #49 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #50 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #51 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #52 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #53 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #54 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #55 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #56 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #57 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #58 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #59 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #60 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #61 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #62 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #63 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #64 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #65 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #66 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #67 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #68 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #69 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #70 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #71 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #72 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #73 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #74 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #75 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #76 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #77 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #78 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #79 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #80 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #81 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #82 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #83 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #84 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #85 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #86 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #87 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #88 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #89 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
 #90 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #91 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #92 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #93 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #94 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #95 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #96 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #97 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #98 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #99 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#100 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#101 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#102 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#103 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#104 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#105 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#106 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#107 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#108 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#109 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#110 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#111 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#112 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#113 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#114 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#115 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#116 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#117 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#118 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#119 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#120 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#121 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#122 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#123 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#124 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#125 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#126 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#127 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#128 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#129 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#130 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#131 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#132 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#133 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#134 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#135 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#136 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#137 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#138 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#139 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#140 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#141 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#142 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#143 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#144 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#145 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#146 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#147 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#148 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#149 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#150 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#151 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#152 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#153 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#154 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#155 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#156 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#157 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#158 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#159 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#160 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#161 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#162 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#163 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#164 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#165 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#166 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#167 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#168 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#169 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#170 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#171 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#172 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#173 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#174 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#175 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#176 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#177 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#178 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#179 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#180 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#181 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#182 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#183 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#184 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#185 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#186 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#187 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#188 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#189 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#190 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#191 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#192 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#193 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#194 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#195 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#196 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#197 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#198 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#199 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#200 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#201 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#202 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#203 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#204 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#205 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#206 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#207 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#208 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#209 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#210 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#211 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#212 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#213 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#214 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#215 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#216 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#217 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#218 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#219 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#220 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#221 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#222 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#223 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#224 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#225 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#226 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#227 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#228 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#229 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#230 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#231 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#232 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#233 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#234 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#235 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#236 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#237 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#238 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#239 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#240 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#241 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#242 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#243 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#244 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#245 0x00000000079338c2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#246 0x0000000007942332 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#247 0x0000000007933b85 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#248 0x000000000793669b (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#249 0x0000000007937ba0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#250 0x00000000079254a4 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#251 0x0000000007925dd3 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#252 0x0000000007925ff0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#253 0x0000000007938cbf (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#254 0x00000000079392f0 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#255 0x0000000007931d7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
```

</details>

In addition:

* Segfault only happens in Clang trunk.

* The original repro was claimed to be valid, and is accepted by GCC and MSVC, but it has not been accepted by Clang since 16.

CC @shafik

---

### Comment 9 - shafik

It looks like we reregressed this bug, it looks @0x59616e did not fully cover the test cases in their PR.

---

### Comment 10 - shafik

@LYP951018 git bisect points to as being responsible for this regression: 52126dc72c3f6f4d27e3835b0ad53e162af25e53


---

### Comment 11 - cor3ntin

This is fixed in trunk, but we should ad a test

---

### Comment 12 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 13 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Björn Fahller (rollbear)

<details>
The following invalid code ICEs Clang++ 16.0.6. This is a reduction from a case I believe to be valid, and that was accepted by Clang++-15 (https://godbolt.org/z/axxY8TGWz)

```c++
auto c(auto, auto ... fs){return [](auto) requires(c(fs...)){};}
auto f = c([]{})(5);
```

with the backtrace:
```console
$ clang++-16 -std=c++2b err.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-16/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name err.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -fcoverage-compilation-dir=/ -resource-dir /usr/lib/llvm-16/lib/clang/16 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward -internal-isystem /usr/lib/llvm-16/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++2b -fdeprecated-macro -fdebug-compilation-dir=/ -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/err-6f78a7.o -x c++ err.cpp
1.      err.cpp:2:19: current parser token ')'
 #<!-- -->0 0x00007f899cf08c16 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfc1c16)
 #<!-- -->1 0x00007f899cf06e60 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfbfe60)
 #<!-- -->2 0x00007f899cf093eb (/usr/lib/llvm-16/bin/../lib/libLLVM-16.so.1+0xfc23eb)
 #<!-- -->3 0x00007f899b9ff4b0 (/lib/x86_64-linux-gnu/libc.so.6+0x3c4b0)
 #<!-- -->4 0x00007f89a500b3ee clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef&lt;std::pair&lt;llvm::PointerUnion&lt;clang::TemplateTypeParmType const*, clang::NamedDecl*&gt;, clang::SourceLocation&gt;&gt;, clang::MultiLevelTemplateArgumentList const&amp;, bool&amp;, bool&amp;, std::optional&lt;unsigned int&gt;&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1aa13ee)
 #<!-- -->5 0x00007f89a4fa2571 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a38571)
 #<!-- -->6 0x00007f89a4fa85ab (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a3e5ab)
 #<!-- -->7 0x00007f89a4fa1f82 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a37f82)
 #<!-- -->8 0x00007f89a49cf387 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1465387)
 #<!-- -->9 0x00007f89a49ce279 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1464279)
#<!-- -->10 0x00007f89a49c94df (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x145f4df)
#<!-- -->11 0x00007f89a49c8e8c clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x145ee8c)
#<!-- -->12 0x00007f89a49cb785 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1461785)
#<!-- -->13 0x00007f89a4f415df clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x19d75df)
#<!-- -->14 0x00007f89a4f94ab7 (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x1a2aab7)
#<!-- -->15 0x00007f89a48966dd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x132c6dd)
#<!-- -->16 0x00007f89a4f42a81 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x19d8a81)
#<!-- -->17 0x00007f89a4e1f5f0 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18b55f0)
#<!-- -->18 0x00007f89a4e1ffc3 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18b5fc3)
#<!-- -->19 0x00007f89a4e36e67 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x18cce67)
#<!-- -->20 0x00007f89a4b50681 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x15e6681)
#<!-- -->21 0x00007f89a4b687fd clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x15fe7fd)
#<!-- -->22 0x00007f89a417fa4c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc15a4c)
#<!-- -->23 0x00007f89a4180965 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc16965)
#<!-- -->24 0x00007f89a417cbe1 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc12be1)
#<!-- -->25 0x00007f89a414f26b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbe526b)
#<!-- -->26 0x00007f89a414c637 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbe2637)
#<!-- -->27 0x00007f89a41fc557 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc92557)
#<!-- -->28 0x00007f89a41fbf79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc91f79)
#<!-- -->29 0x00007f89a41fb217 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc91217)
#<!-- -->30 0x00007f89a41f95be clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xc8f5be)
#<!-- -->31 0x00007f89a413933e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0xbcf33e)
#<!-- -->32 0x00007f89a5d503b5 clang::FrontendAction::Execute() (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x27e63b5)
#<!-- -->33 0x00007f89a5ccac94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x2760c94)
#<!-- -->34 0x00007f89a5dc9434 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-16/bin/../lib/libclang-cpp.so.16+0x285f434)
#<!-- -->35 0x000055dc45f3b116 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-16/bin/clang+0x15116)
#<!-- -->36 0x000055dc45f38280 (/usr/lib/llvm-16/bin/clang+0x12280)
#<!-- -->37 0x000055dc45f37f89 clang_main(int, char**) (/usr/lib/llvm-16/bin/clang+0x11f89)
#<!-- -->38 0x00007f899b9e6a90 (/lib/x86_64-linux-gnu/libc.so.6+0x23a90)
#<!-- -->39 0x00007f899b9e6b49 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x23b49)
#<!-- -->40 0x000055dc45f348b5 _start (/usr/lib/llvm-16/bin/clang+0xe8b5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 16.0.6 (++20230710042020+7cbf1a259152-1~exp1~20230710162037.55)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/err-c1b700.cpp
clang: note: diagnostic msg: /tmp/err-c1b700.sh
clang: note: diagnostic msg: 

********************
```

`/tmp/err-c1b700.cpp`:
```cpp
# 1 "err.cpp"
# 1 "&lt;built-in&gt;" 1
# 1 "&lt;built-in&gt;" 3
# 451 "&lt;built-in&gt;" 3
# 1 "&lt;command line&gt;" 1
# 1 "&lt;built-in&gt;" 2
# 1 "err.cpp" 2
auto c(auto, auto ... fs){return [](auto) requires(c(fs...)){};}
auto f = c([]{})(5);
```

`/tmp/err-c1b700.sh`:
```shell
# Crash reproducer for Ubuntu clang version 16.0.6 (++20230710042020+7cbf1a259152-1~exp1~20230710162037.55)
# Driver args: "--driver-mode=g++" "-std=c++2b" "err.cpp"
# Original command:  "/usr/lib/llvm-16/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "err.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/" "-resource-dir" "/usr/lib/llvm-16/lib/clang/16" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward" "-internal-isystem" "/usr/lib/llvm-16/lib/clang/16/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-std=c++2b" "-fdeprecated-macro" "-fdebug-compilation-dir=/" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/err-6f78a7.o" "-x" "c++" "err.cpp"
 "/usr/lib/llvm-16/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "err.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/" "-std=c++2b" "-fdeprecated-macro" "-fdebug-compilation-dir=/" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "err-c1b700.cpp"
```
</details>


---

### Comment 14 - shafik

I added this as a good first issue since all that is left is writing up the test.

---

### Comment 15 - zyn0217

This still segfaults on trunk

---

### Comment 16 - shafik

> This still segfaults on trunk

Fixed in clang-19 and regressed in clang-20: https://godbolt.org/z/8Y6TrxGsP



---

### Comment 17 - shafik

@LYP951018 git bisect points to 52126dc72c3f6f4d27e3835b0ad53e162af25e53 as the responsible commit.

I wonder if this is just uncovering another problem.

---

