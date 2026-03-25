# Out of memory or taking too much time in compiling an invalid decltype declaration

**Author:** haoxintu
**URL:** https://github.com/llvm/llvm-project/issues/46215
**Status:** Closed
**Labels:** c++11, clang:frontend, bugzilla, confirmed, crash-on-valid
**Closed Date:** 2025-06-13T10:45:42Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46871](https://llvm.org/bz46871) |
| Version | trunk |
| OS | All |
| CC | @zygoloid |

## Extended Description 
Hi, all.

This invalid code makes Clang out of memory or takes too much time in compiling. I have tested this in Clang-6.0 and upwards versions, and they behave the same.

Input:
//small.cc
short g_const_short ;
short g_volatile_ushort ;
bool g_cv_bool ;
char g_cv_char ;
short g_cv_ushort ;
int g_cv_int ;
long g_cv_long ;
decltype(0 ? g_cv_uchar or g_var_wchar or g_var_wchar 
or g_cv_uchar : g_var_int or g_cv_short or     
g_volatile_bool || g_const_long or g_volatile_bool or
g_var_llong or g_const_ullong || g_cv_llong || 
g_cv_short or g_volatile_wchar ||                                      
g_var_ushort || g_var_bool || g_const_int                                  
|| a) var = 0;

Command:
time clang++ small.cc

Output: (in a 32G RAM, 8 cores machine)
//errors
real    0m23.615s
user    0m22.771s
sys     0m0.843s

Reproduced in Godbolt: https://godbolt.org/z/Enq96z

It says:
LLVM ERROR: out of memory
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20200728/bin/clang-12 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -fno-split-dwarf-inlining -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -resource-dir /opt/compiler-explorer/clang-trunk-20200728/lib/clang/12.0.0 -internal-isystem /opt/compiler-explorer/gcc-9.2.0/lib/gcc/x86_64-linux-gnu/9.2.0/../../../../include/c++/9.2.0 -internal-isystem /opt/compiler-explorer/gcc-9.2.0/lib/gcc/x86_64-linux-gnu/9.2.0/../../../../include/c++/9.2.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-9.2.0/lib/gcc/x86_64-linux-gnu/9.2.0/../../../../include/c++/9.2.0/backward -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/clang-trunk-20200728/lib/clang/12.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/ce -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/example-18b01c.o -x c++ <source> 
1.	<source>:14:5: current parser token ')'
 #&#8203;0 0x0000555d8e7a9eca llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x2ceeeca)
 #&#8203;1 0x0000555d8e7a7c74 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x2cecc74)
 #&#8203;2 0x0000555d8e7a7dc3 SignalHandler(int) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x2cecdc3)
 #&#8203;3 0x00007f1e49f6f8a0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x128a0)
 #&#8203;4 0x00007f1e48e4af47 raise (/lib/x86_64-linux-gnu/libc.so.6+0x3ef47)
 #&#8203;5 0x00007f1e48e4c8b1 abort (/lib/x86_64-linux-gnu/libc.so.6+0x408b1)
 #&#8203;6 0x0000555d8e7234a1 llvm::report_bad_alloc_error(char const*, bool) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x2c684a1)
 #&#8203;7 0x0000555d8e7234d5 (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x2c684d5)
 #&#8203;8 0x00007f1e498462ac operator new(unsigned long) (/usr/lib/x86_64-linux-gnu/libstdc++.so.6+0x932ac)
 #&#8203;9 0x0000555d90e5c24d llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::Allocate(unsigned long, llvm::Align) (.constprop.1155) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x53a124d)
#&#8203;10 0x0000555d90e639c7 clang::ImplicitCastExpr::Create(clang::ASTContext const&, clang::QualType, clang::CastKind, clang::Expr*, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::ExprValueKind) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x53a89c7)
#&#8203;11 0x0000555d9070c8a9 clang::Sema::DefaultLvalueConversion(clang::Expr*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4c518a9)
#&#8203;12 0x0000555d907a83ed clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::StandardConversionSequence const&, clang::Sema::AssignmentAction, clang::Sema::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4ced3ed)
#&#8203;13 0x0000555d907aad77 clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&, clang::Sema::AssignmentAction, clang::Sema::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4cefd77)
#&#8203;14 0x0000555d9097adcf clang::Sema::PerformContextuallyConvertToBool(clang::Expr*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4ebfdcf)
#&#8203;15 0x0000555d90712740 clang::Sema::CheckLogicalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4c57740)
#&#8203;16 0x0000555d9071cd34 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4c61d34)
#&#8203;17 0x0000555d9071dc44 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4c62c44)
#&#8203;18 0x0000555d907ff9c2 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d449c2)
#&#8203;19 0x0000555d907ff8bd clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d448bd)
#&#8203;20 0x0000555d907ff8bd clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d448bd)
#&#8203;21 0x0000555d907f3f3f clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformConditionalOperator(clang::ConditionalOperator*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d38f3f)
#&#8203;22 0x0000555d907d09bb (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d159bb)
#&#8203;23 0x0000555d907d20ee clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x4d170ee)
#&#8203;24 0x0000555d903d7717 clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x491c717)
#&#8203;25 0x0000555d90405b09 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo**, bool, bool) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x494ab09)
#&#8203;26 0x0000555d903a3dfc clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48e8dfc)
#&#8203;27 0x0000555d903c9914 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x490e914)
#&#8203;28 0x0000555d903a4968 clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48e9968)
#&#8203;29 0x0000555d903a5841 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.228) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48ea841)
#&#8203;30 0x0000555d903ab719 clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48f0719)
#&#8203;31 0x0000555d903ace29 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48f1e29)
#&#8203;32 0x0000555d903a0a69 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x48e5a69)
#&#8203;33 0x0000555d8ef9bee9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x34e0ee9)
#&#8203;34 0x0000555d8ef55b4b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x349ab4b)
#&#8203;35 0x0000555d8f06230b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0x35a730b)
#&#8203;36 0x0000555d8c96f01c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0xeb401c)
#&#8203;37 0x0000555d8c96bb1d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0xeb0b1d)
#&#8203;38 0x0000555d8c8a9ba1 main (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0xdeeba1)
#&#8203;39 0x00007f1e48e2db97 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b97)
#&#8203;40 0x0000555d8c96b69a _start (/opt/compiler-explorer/clang-trunk-20200728/bin/clang-12+0xeb069a)
clang-12: error: unable to execute command: Aborted (core dumped)
clang-12: error: clang frontend command failed due to signal (use -v to see invocation)


Clang version in Godbolt:
clang version 12.0.0 (https://github.com/llvm/llvm-project.git 8120eba5fce378083ef22651f2b7b6dcaa54a098)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-9.2.0/lib/gcc/x86_64-linux-gnu/9.2.0
Selected GCC installation: /opt/compiler-explorer/gcc-9.2.0/lib/gcc/x86_64-linux-gnu/9.2.0
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64

## Comments

### Comment 1 - AaronBallman

This issue reproduces as of Clang 16.0.0: https://gcc.godbolt.org/z/5o37f6vvq

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - llvmbot

@llvm/issue-subscribers-c-11

---

### Comment 4 - AaronBallman

The issue seems to be that we never break out of the loop in `RecursiveTransformLoop()`: https://github.com/llvm/llvm-project/blob/19c26a7c03559a6c7ab1968fcf98e39fd4a86714/clang/lib/Sema/SemaExprCXX.cpp#L8611

CC @DavidGoldman 

---

