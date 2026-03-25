# Tiny concepts code crashes compiler

**Author:** avocadoboi
**URL:** https://github.com/llvm/llvm-project/issues/171509
**Status:** Closed
**Labels:** clang:frontend, obsolete, concepts, crash-on-valid
**Closed Date:** 2025-12-10T04:31:03Z

## Body

The following piece of code
```
template<typename T>
concept integral = true;

template<typename Value_>
struct Degrees { Value_ value; };

template<integral A, integral B, template<integral> class Class_>
constexpr bool operator==(Class_<A> const first, Class_<B> const second) {
	return first.value == second.value;
}

static_assert(Degrees{80} == Degrees{80});
```
crashes the compiler:
```
$ clang++ -std=c++20 main.cpp 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-ll
vm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno
-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bjorn/Dokument/cl
angd-error-reproduce -fcoverage-compilation-dir=/home/bjorn/Dokument/clangd-error-reproduce -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/l
ib/gcc/x86_64-linux-gnu/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/15/../../../../include/x86_64-linux-gnu/c++/15 -internal-i
system /usr/lib/gcc/x86_64-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local
/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/15/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -intern
al-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fs
kip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/main-184ea7.o -x c++ main.cpp
1.      main.cpp:12:41: current parser token ')'
 #0 0x00007f519e85f4ea llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5f4ea)
 #1 0x00007f519e85d094 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5d094)
 #2 0x00007f519e85fbab (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5fbab)
 #3 0x00007f519a449df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #4 0x00007f51a4e13ebc (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2813ebc)
 #5 0x00007f51a4e1693a (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x281693a)
 #6 0x00007f51a4e09456 (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2809456)
 #7 0x00007f51a4e16490 (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2816490)
 #8 0x00007f51a4e0fa9d (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x280fa9d)
 #9 0x00007f51a4e0ea38 clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::Templat
ePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x280ea38)
#10 0x00007f51a4cf4c9e clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, c
lang::OverloadCandidateSet::CandidateSetKind) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x26f4c9e)
#11 0x00007f51a4ce8ffe clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/lib/x86_64-linux-gnu/l
ibclang-cpp.so.19.1+0x26e8ffe)
#12 0x00007f51a4d00dee clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang
::Expr*, bool, bool, clang::FunctionDecl*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2700dee)
#13 0x00007f51a49da8cf (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x23da8cf)
#14 0x00007f51a49da5f3 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/lib/x86_64-linux-g
nu/libclang-cpp.so.19.1+0x23da5f3)
#15 0x00007f51a49ad6ac clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/lib/x86_64-linux-gnu/l
ibclang-cpp.so.19.1+0x23ad6ac)
#16 0x00007f51a3da5fc5 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/lib/x86_64-linux-gnu/libclang-cp
p.so.19.1+0x17a5fc5)
#17 0x00007f51a3da83d9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x17a83
d9)
#18 0x00007f51a3d8ff95 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x178ff95)
#19 0x00007f51a3d6d75f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, cla
ng::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x176d75f)
#20 0x00007f51a3e2caa4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/lib/x86_64-linux-g
nu/libclang-cpp.so.19.1+0x182caa4)
#21 0x00007f51a3e2b657 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/lib/x86_64-linux-gnu/libclang
-cpp.so.19.1+0x182b657)
#22 0x00007f51a3d5811e clang::ParseAST(clang::Sema&, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x175811e)
#23 0x00007f51a5e57379 clang::FrontendAction::Execute() (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x3857379)
#24 0x00007f51a5dc8794 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x37c8794)
#25 0x00007f51a5ed548e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x38d548e)
#26 0x000055829504a9a6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x129a6)
#27 0x0000558295047835 (/usr/lib/llvm-19/bin/clang+0xf835)
#28 0x00005582950469df clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xe9df)
#29 0x00005582950559ea main (/usr/lib/llvm-19/bin/clang+0x1d9ea)
#30 0x00007f519a433ca8 (/lib/x86_64-linux-gnu/libc.so.6+0x29ca8)
#31 0x00007f519a433d65 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29d65)
#32 0x0000558295045461 _start (/usr/lib/llvm-19/bin/clang+0xd461)
clang++: error: unable to execute command: Segmentation fault
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Debian clang version 19.1.7 (19)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-73d1d4.cpp
clang++: note: diagnostic msg: /tmp/main-73d1d4.sh
clang++: note: diagnostic msg: 

********************
```

I doubt you need the preprocessed source and run scripts but here they are anyways:
[main-73d1d4.sh](https://github.com/user-attachments/files/24064170/main-73d1d4.sh)
[main-73d1d4.cpp](https://github.com/user-attachments/files/24064169/main-73d1d4.cpp)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Björn Sundin (avocadoboi)

<details>
The following piece of code
```
template&lt;typename T&gt;
concept integral = true;

template&lt;typename Value_&gt;
struct Degrees { Value_ value; };

template&lt;integral A, integral B, template&lt;integral&gt; class Class_&gt;
constexpr bool operator==(Class_&lt;A&gt; const first, Class_&lt;B&gt; const second) {
	return first.value == second.value;
}

static_assert(Degrees{80} == Degrees{80});
```
crashes the compiler:
```
$ clang++ -std=c++20 main.cpp 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-ll
vm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno
-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bjorn/Dokument/cl
angd-error-reproduce -fcoverage-compilation-dir=/home/bjorn/Dokument/clangd-error-reproduce -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/l
ib/gcc/x86_64-linux-gnu/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/15/../../../../include/x86_64-linux-gnu/c++/15 -internal-i
system /usr/lib/gcc/x86_64-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local
/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/15/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -intern
al-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fs
kip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/main-184ea7.o -x c++ main.cpp
1.      main.cpp:12:41: current parser token ')'
 #<!-- -->0 0x00007f519e85f4ea llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5f4ea)
 #<!-- -->1 0x00007f519e85d094 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5d094)
 #<!-- -->2 0x00007f519e85fbab (/lib/x86_64-linux-gnu/libLLVM.so.19.1+0x3e5fbab)
 #<!-- -->3 0x00007f519a449df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->4 0x00007f51a4e13ebc (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2813ebc)
 #<!-- -->5 0x00007f51a4e1693a (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x281693a)
 #<!-- -->6 0x00007f51a4e09456 (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2809456)
 #<!-- -->7 0x00007f51a4e16490 (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2816490)
 #<!-- -->8 0x00007f51a4e0fa9d (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x280fa9d)
 #<!-- -->9 0x00007f51a4e0ea38 clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::Templat
ePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x280ea38)
#<!-- -->10 0x00007f51a4cf4c9e clang::isBetterOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;, clang::SourceLocation, c
lang::OverloadCandidateSet::CandidateSetKind) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x26f4c9e)
#<!-- -->11 0x00007f51a4ce8ffe clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/lib/x86_64-linux-gnu/l
ibclang-cpp.so.19.1+0x26e8ffe)
#<!-- -->12 0x00007f51a4d00dee clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang
::Expr*, bool, bool, clang::FunctionDecl*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x2700dee)
#<!-- -->13 0x00007f51a49da8cf (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x23da8cf)
#<!-- -->14 0x00007f51a49da5f3 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/lib/x86_64-linux-g
nu/libclang-cpp.so.19.1+0x23da5f3)
#<!-- -->15 0x00007f51a49ad6ac clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/lib/x86_64-linux-gnu/l
ibclang-cpp.so.19.1+0x23ad6ac)
#<!-- -->16 0x00007f51a3da5fc5 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/lib/x86_64-linux-gnu/libclang-cp
p.so.19.1+0x17a5fc5)
#<!-- -->17 0x00007f51a3da83d9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x17a83
d9)
#<!-- -->18 0x00007f51a3d8ff95 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x178ff95)
#<!-- -->19 0x00007f51a3d6d75f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, cla
ng::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x176d75f)
#<!-- -->20 0x00007f51a3e2caa4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib/x86_64-linux-g
nu/libclang-cpp.so.19.1+0x182caa4)
#<!-- -->21 0x00007f51a3e2b657 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib/x86_64-linux-gnu/libclang
-cpp.so.19.1+0x182b657)
#<!-- -->22 0x00007f51a3d5811e clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x175811e)
#<!-- -->23 0x00007f51a5e57379 clang::FrontendAction::Execute() (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x3857379)
#<!-- -->24 0x00007f51a5dc8794 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x37c8794)
#<!-- -->25 0x00007f51a5ed548e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib/x86_64-linux-gnu/libclang-cpp.so.19.1+0x38d548e)
#<!-- -->26 0x000055829504a9a6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x129a6)
#<!-- -->27 0x0000558295047835 (/usr/lib/llvm-19/bin/clang+0xf835)
#<!-- -->28 0x00005582950469df clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe9df)
#<!-- -->29 0x00005582950559ea main (/usr/lib/llvm-19/bin/clang+0x1d9ea)
#<!-- -->30 0x00007f519a433ca8 (/lib/x86_64-linux-gnu/libc.so.6+0x29ca8)
#<!-- -->31 0x00007f519a433d65 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29d65)
#<!-- -->32 0x0000558295045461 _start (/usr/lib/llvm-19/bin/clang+0xd461)
clang++: error: unable to execute command: Segmentation fault
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Debian clang version 19.1.7 (19)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-73d1d4.cpp
clang++: note: diagnostic msg: /tmp/main-73d1d4.sh
clang++: note: diagnostic msg: 

********************
```

I doubt you need the preprocessed source and run scripts but here they are anyways:
[main-73d1d4.sh](https://github.com/user-attachments/files/24064170/main-73d1d4.sh)
[main-73d1d4.cpp](https://github.com/user-attachments/files/24064169/main-73d1d4.cpp)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 3 - frederick-vs-ja

Only crashed in Clang 19 [Godbolt link](https://godbolt.org/z/MxvbT4KW9). Possibly fixed by #124137 in Clang 20.

---

