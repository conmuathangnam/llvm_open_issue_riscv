# Crash when C++ coroutine await_suspend is consteval

**Author:** JesseBusman
**URL:** https://github.com/llvm/llvm-project/issues/128965

## Body

[https://godbolt.org/z/Y3f41exY9](https://godbolt.org/z/Y3f41exY9)
```c++
#include <coroutine>

struct CrashClangCoroutine {
	struct promise_type {
		struct Awaiter {
			void await_resume() const noexcept { }
			bool await_ready() const noexcept { return false; }
			consteval void await_suspend(std::coroutine_handle<>) const noexcept { }
		};
		std::suspend_always initial_suspend() { return {}; }
		Awaiter final_suspend() noexcept { return {}; }
		void unhandled_exception() { }
		CrashClangCoroutine get_return_object() { return {}; }
		void return_void() { }
	};
};

CrashClangCoroutine co() { co_return; }

int main() {
	co();
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name clangd_crash_test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Projects/Test -fcoverage-compilation-dir=/home/user/Projects/Test -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/clangd_crash_test-e592fe.o -x c++ clangd_crash_test.cpp
1.      clangd_crash_test.cpp:18:37: current parser token ';'
2.      clangd_crash_test.cpp:18:26: parsing function body 'co'
3.      clangd_crash_test.cpp:18:26: in compound statement ('{}')
 #0 0x0000722bdee1a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #1 0x0000722bdee176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #2 0x0000722bdee176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
 #3 0x0000722bde04bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #4 0x0000722be771dbe9 clang::QualType::getCommonPtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:959:0
 #5 0x0000722be771dbe9 clang::QualType::getTypePtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:7747:0
 #6 0x0000722be771dbe9 clang::QualType::operator->() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:1001:0
 #7 0x0000722be771dbe9 clang::CallExpr::getCallReturnType(clang::ASTContext const&) const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/Expr.cpp:1593:0
 #8 0x0000722be7e8033f clang::ReferenceType::classof(clang::Type const*) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:3440:0
 #9 0x0000722be7e8033f llvm::isa_impl<clang::ReferenceType, clang::Type, void>::doit(clang::Type const&) /usr/include/llvm/Support/Casting.h:64:0
#10 0x0000722be7e8033f llvm::isa_impl_cl<clang::ReferenceType, clang::Type const*>::doit(clang::Type const*) /usr/include/llvm/Support/Casting.h:110:0
#11 0x0000722be7e8033f llvm::isa_impl_wrap<clang::ReferenceType, clang::Type const*, clang::Type const*>::doit(clang::Type const* const&) /usr/include/llvm/Support/Casting.h:137:0
#12 0x0000722be7e8033f llvm::isa_impl_wrap<clang::ReferenceType, clang::Type const* const, clang::Type const*>::doit(clang::Type const* const&) /usr/include/llvm/Support/Casting.h:129:0
#13 0x0000722be7e8033f llvm::CastIsPossible<clang::ReferenceType, clang::Type const*, void>::isPossible(clang::Type const* const&) /usr/include/llvm/Support/Casting.h:257:0
#14 0x0000722be7e8033f llvm::CastInfo<clang::ReferenceType, clang::QualType const, void>::isPossible(clang::QualType const&) /usr/include/llvm/Support/Casting.h:509:0
#15 0x0000722be7e8033f bool llvm::isa<clang::ReferenceType, clang::QualType>(clang::QualType const&) /usr/include/llvm/Support/Casting.h:549:0
#16 0x0000722be7e8033f clang::Type::isReferenceType() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:8014:0
#17 0x0000722be7e8033f maybeTailCall /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:334:0
#18 0x0000722be7e8033f buildCoawaitCalls /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:434:0
#19 0x0000722be7eab9a5 clang::Sema::BuildResolvedCoawaitExpr(clang::SourceLocation, clang::Expr*, clang::Expr*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:902:0
#20 0x0000722be7eabe43 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:718:0
#21 0x0000722be7eabe43 clang::Sema::ActOnCoroutineBodyStart(clang::Scope*, clang::SourceLocation, llvm::StringRef) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:732:0
#22 0x0000722be748c728 clang::Sema::ActOnCoreturnStmt(clang::Scope*, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:971:3
#23 0x0000722be748c728 clang::Parser::ParseReturnStatement() /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2470:37
#24 0x0000722be7491d4f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:346:15
#25 0x0000722be74922a7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#26 0x0000722be74922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#27 0x0000722be749a59a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#28 0x0000722be749ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#29 0x0000722be74bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#30 0x0000722be73ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#31 0x0000722be74bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#32 0x0000722be74bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#33 0x0000722be74cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#34 0x0000722be74ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#35 0x0000722be73d9c18 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:0
#36 0x0000722be90fa221 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#37 0x0000722be909008f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#38 0x0000722be909008f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#39 0x0000722be909008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#40 0x0000722be9154ba5 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#41 0x0000722be9154ba5 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#42 0x0000722be9154ba5 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#43 0x0000722be9154ba5 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#44 0x0000722be9154ba5 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#45 0x0000722be9154ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#46 0x000056dae01d2832 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#47 0x000056dae01d704e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#48 0x000056dae01d7de3 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#49 0x000056dae01caf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#50 0x0000722bde035488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#51 0x0000722bde03554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#52 0x0000722bde03554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#53 0x000056dae01cafc5 _start (/usr/bin/clang+++0xafc5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jesse Busman (JesseBusman)

<details>
[https://godbolt.org/z/Y3f41exY9](https://godbolt.org/z/Y3f41exY9)
```
#include &lt;coroutine&gt;

struct CrashClangCoroutine {
	struct promise_type {
		struct Awaiter {
			void await_resume() const noexcept { }
			bool await_ready() const noexcept { return false; }
			consteval void await_suspend(std::coroutine_handle&lt;&gt;) const noexcept { }
		};
		std::suspend_always initial_suspend() { return {}; }
		Awaiter final_suspend() noexcept { return {}; }
		void unhandled_exception() { }
		CrashClangCoroutine get_return_object() { return {}; }
		void return_void() { }
	};
};

CrashClangCoroutine co() { co_return; }

int main() {
	co();
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name clangd_crash_test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Projects/Test -fcoverage-compilation-dir=/home/user/Projects/Test -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/clangd_crash_test-e592fe.o -x c++ clangd_crash_test.cpp
1.      clangd_crash_test.cpp:18:37: current parser token ';'
2.      clangd_crash_test.cpp:18:26: parsing function body 'co'
3.      clangd_crash_test.cpp:18:26: in compound statement ('{}')
 #<!-- -->0 0x0000722bdee1a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x0000722bdee176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x0000722bdee176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
 #<!-- -->3 0x0000722bde04bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->4 0x0000722be771dbe9 clang::QualType::getCommonPtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:959:0
 #<!-- -->5 0x0000722be771dbe9 clang::QualType::getTypePtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:7747:0
 #<!-- -->6 0x0000722be771dbe9 clang::QualType::operator-&gt;() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:1001:0
 #<!-- -->7 0x0000722be771dbe9 clang::CallExpr::getCallReturnType(clang::ASTContext const&amp;) const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/Expr.cpp:1593:0
 #<!-- -->8 0x0000722be7e8033f clang::ReferenceType::classof(clang::Type const*) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:3440:0
 #<!-- -->9 0x0000722be7e8033f llvm::isa_impl&lt;clang::ReferenceType, clang::Type, void&gt;::doit(clang::Type const&amp;) /usr/include/llvm/Support/Casting.h:64:0
#<!-- -->10 0x0000722be7e8033f llvm::isa_impl_cl&lt;clang::ReferenceType, clang::Type const*&gt;::doit(clang::Type const*) /usr/include/llvm/Support/Casting.h:110:0
#<!-- -->11 0x0000722be7e8033f llvm::isa_impl_wrap&lt;clang::ReferenceType, clang::Type const*, clang::Type const*&gt;::doit(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:137:0
#<!-- -->12 0x0000722be7e8033f llvm::isa_impl_wrap&lt;clang::ReferenceType, clang::Type const* const, clang::Type const*&gt;::doit(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:129:0
#<!-- -->13 0x0000722be7e8033f llvm::CastIsPossible&lt;clang::ReferenceType, clang::Type const*, void&gt;::isPossible(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:257:0
#<!-- -->14 0x0000722be7e8033f llvm::CastInfo&lt;clang::ReferenceType, clang::QualType const, void&gt;::isPossible(clang::QualType const&amp;) /usr/include/llvm/Support/Casting.h:509:0
#<!-- -->15 0x0000722be7e8033f bool llvm::isa&lt;clang::ReferenceType, clang::QualType&gt;(clang::QualType const&amp;) /usr/include/llvm/Support/Casting.h:549:0
#<!-- -->16 0x0000722be7e8033f clang::Type::isReferenceType() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:8014:0
#<!-- -->17 0x0000722be7e8033f maybeTailCall /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:334:0
#<!-- -->18 0x0000722be7e8033f buildCoawaitCalls /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:434:0
#<!-- -->19 0x0000722be7eab9a5 clang::Sema::BuildResolvedCoawaitExpr(clang::SourceLocation, clang::Expr*, clang::Expr*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:902:0
#<!-- -->20 0x0000722be7eabe43 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:718:0
#<!-- -->21 0x0000722be7eabe43 clang::Sema::ActOnCoroutineBodyStart(clang::Scope*, clang::SourceLocation, llvm::StringRef) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:732:0
#<!-- -->22 0x0000722be748c728 clang::Sema::ActOnCoreturnStmt(clang::Scope*, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:971:3
#<!-- -->23 0x0000722be748c728 clang::Parser::ParseReturnStatement() /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2470:37
#<!-- -->24 0x0000722be7491d4f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:346:15
#<!-- -->25 0x0000722be74922a7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#<!-- -->26 0x0000722be74922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#<!-- -->27 0x0000722be749a59a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#<!-- -->28 0x0000722be749ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#<!-- -->29 0x0000722be74bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#<!-- -->30 0x0000722be73ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#<!-- -->31 0x0000722be74bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#<!-- -->32 0x0000722be74bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#<!-- -->33 0x0000722be74cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#<!-- -->34 0x0000722be74ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#<!-- -->35 0x0000722be73d9c18 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:0
#<!-- -->36 0x0000722be90fa221 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#<!-- -->37 0x0000722be909008f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#<!-- -->38 0x0000722be909008f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#<!-- -->39 0x0000722be909008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->40 0x0000722be9154ba5 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->41 0x0000722be9154ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->42 0x0000722be9154ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->43 0x0000722be9154ba5 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#<!-- -->44 0x0000722be9154ba5 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#<!-- -->45 0x0000722be9154ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#<!-- -->46 0x000056dae01d2832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->47 0x000056dae01d704e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->48 0x000056dae01d7de3 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#<!-- -->49 0x000056dae01caf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->50 0x0000722bde035488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x0000722bde03554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->52 0x0000722bde03554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->53 0x000056dae01cafc5 _start (/usr/bin/clang+++0xafc5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-coroutines

Author: Jesse Busman (JesseBusman)

<details>
[https://godbolt.org/z/Y3f41exY9](https://godbolt.org/z/Y3f41exY9)
```
#include &lt;coroutine&gt;

struct CrashClangCoroutine {
	struct promise_type {
		struct Awaiter {
			void await_resume() const noexcept { }
			bool await_ready() const noexcept { return false; }
			consteval void await_suspend(std::coroutine_handle&lt;&gt;) const noexcept { }
		};
		std::suspend_always initial_suspend() { return {}; }
		Awaiter final_suspend() noexcept { return {}; }
		void unhandled_exception() { }
		CrashClangCoroutine get_return_object() { return {}; }
		void return_void() { }
	};
};

CrashClangCoroutine co() { co_return; }

int main() {
	co();
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name clangd_crash_test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Projects/Test -fcoverage-compilation-dir=/home/user/Projects/Test -resource-dir /usr/lib/clang/19 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/backward -internal-isystem /usr/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/clangd_crash_test-e592fe.o -x c++ clangd_crash_test.cpp
1.      clangd_crash_test.cpp:18:37: current parser token ';'
2.      clangd_crash_test.cpp:18:26: parsing function body 'co'
3.      clangd_crash_test.cpp:18:26: in compound statement ('{}')
 #<!-- -->0 0x0000722bdee1a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x0000722bdee176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x0000722bdee176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
 #<!-- -->3 0x0000722bde04bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->4 0x0000722be771dbe9 clang::QualType::getCommonPtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:959:0
 #<!-- -->5 0x0000722be771dbe9 clang::QualType::getTypePtr() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:7747:0
 #<!-- -->6 0x0000722be771dbe9 clang::QualType::operator-&gt;() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:1001:0
 #<!-- -->7 0x0000722be771dbe9 clang::CallExpr::getCallReturnType(clang::ASTContext const&amp;) const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/Expr.cpp:1593:0
 #<!-- -->8 0x0000722be7e8033f clang::ReferenceType::classof(clang::Type const*) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:3440:0
 #<!-- -->9 0x0000722be7e8033f llvm::isa_impl&lt;clang::ReferenceType, clang::Type, void&gt;::doit(clang::Type const&amp;) /usr/include/llvm/Support/Casting.h:64:0
#<!-- -->10 0x0000722be7e8033f llvm::isa_impl_cl&lt;clang::ReferenceType, clang::Type const*&gt;::doit(clang::Type const*) /usr/include/llvm/Support/Casting.h:110:0
#<!-- -->11 0x0000722be7e8033f llvm::isa_impl_wrap&lt;clang::ReferenceType, clang::Type const*, clang::Type const*&gt;::doit(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:137:0
#<!-- -->12 0x0000722be7e8033f llvm::isa_impl_wrap&lt;clang::ReferenceType, clang::Type const* const, clang::Type const*&gt;::doit(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:129:0
#<!-- -->13 0x0000722be7e8033f llvm::CastIsPossible&lt;clang::ReferenceType, clang::Type const*, void&gt;::isPossible(clang::Type const* const&amp;) /usr/include/llvm/Support/Casting.h:257:0
#<!-- -->14 0x0000722be7e8033f llvm::CastInfo&lt;clang::ReferenceType, clang::QualType const, void&gt;::isPossible(clang::QualType const&amp;) /usr/include/llvm/Support/Casting.h:509:0
#<!-- -->15 0x0000722be7e8033f bool llvm::isa&lt;clang::ReferenceType, clang::QualType&gt;(clang::QualType const&amp;) /usr/include/llvm/Support/Casting.h:549:0
#<!-- -->16 0x0000722be7e8033f clang::Type::isReferenceType() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:8014:0
#<!-- -->17 0x0000722be7e8033f maybeTailCall /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:334:0
#<!-- -->18 0x0000722be7e8033f buildCoawaitCalls /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:434:0
#<!-- -->19 0x0000722be7eab9a5 clang::Sema::BuildResolvedCoawaitExpr(clang::SourceLocation, clang::Expr*, clang::Expr*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:902:0
#<!-- -->20 0x0000722be7eabe43 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:718:0
#<!-- -->21 0x0000722be7eabe43 clang::Sema::ActOnCoroutineBodyStart(clang::Scope*, clang::SourceLocation, llvm::StringRef) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:732:0
#<!-- -->22 0x0000722be748c728 clang::Sema::ActOnCoreturnStmt(clang::Scope*, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaCoroutine.cpp:971:3
#<!-- -->23 0x0000722be748c728 clang::Parser::ParseReturnStatement() /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2470:37
#<!-- -->24 0x0000722be7491d4f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:346:15
#<!-- -->25 0x0000722be74922a7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#<!-- -->26 0x0000722be74922a7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#<!-- -->27 0x0000722be749a59a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#<!-- -->28 0x0000722be749ddac clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#<!-- -->29 0x0000722be74bf6ef clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#<!-- -->30 0x0000722be73ee83f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#<!-- -->31 0x0000722be74bc9d8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#<!-- -->32 0x0000722be74bd3ba clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#<!-- -->33 0x0000722be74cd632 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#<!-- -->34 0x0000722be74ce345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#<!-- -->35 0x0000722be73d9c18 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:0
#<!-- -->36 0x0000722be90fa221 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#<!-- -->37 0x0000722be909008f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#<!-- -->38 0x0000722be909008f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#<!-- -->39 0x0000722be909008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->40 0x0000722be9154ba5 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->41 0x0000722be9154ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->42 0x0000722be9154ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->43 0x0000722be9154ba5 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#<!-- -->44 0x0000722be9154ba5 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#<!-- -->45 0x0000722be9154ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#<!-- -->46 0x000056dae01d2832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->47 0x000056dae01d704e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->48 0x000056dae01d7de3 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:256:26
#<!-- -->49 0x000056dae01caf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->50 0x0000722bde035488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x0000722bde03554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->52 0x0000722bde03554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->53 0x000056dae01cafc5 _start (/usr/bin/clang+++0xafc5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

</details>


---

### Comment 3 - yuxuanchen1997

This is ill-formed as far as I can tell, and we should have diagnosed it somewhere. Otherwise, what do you expect `consteval` await to do?

---

### Comment 4 - JesseBusman

I also think it is ill-formed. Clangd crashed when I accidentally typed `consteval` instead of `constexpr`

---

### Comment 5 - frederick-vs-ja

GCC complains ([Godbolt link](https://godbolt.org/z/qKjhjajcG)):
```
<source>: In function 'void co(_Z2cov.Frame*)':
<source>:18:39: error: call to consteval function 'Fs.CrashClangCoroutine::promise_type::Awaiter::await_suspend(_Coro_self_handle.std::__n4861::coroutine_handle<CrashClangCoroutine::promise_type>::operator std::__n4861::coroutine_handle<void>())' is not a constant expression
   18 | CrashClangCoroutine co() { co_return; }
      |                                       ^
<source>:18:21: error: 'frame_ptr' is not a constant expression
   18 | CrashClangCoroutine co() { co_return; }
      |                     ^~
<source>:18:21: error: 'frame_ptr' is not a constant expression
Compiler returned: 1
```

---

