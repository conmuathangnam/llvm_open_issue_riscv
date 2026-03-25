# Parser crash when template argument is missing from designated initializer list for a union of structs

**Author:** cogwheel
**URL:** https://github.com/llvm/llvm-project/issues/67173
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash
**Closed Date:** 2025-05-21T02:02:35Z

## Body

The following invalid code compiled with `-std=c++2b` causes the parser to crash in clang++ 17 and 18, but not 16. Here's a [live example](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TKQB2cncAGQImbAA5PwAjbFIQAE5yAAd0JWIXJi9ffyC0jKyhcMiYtnjElIdsJ2yRIhZSIly/AJ5gmrqhBqaiUui4hOT7RubW/I7RvoHyyuSASnt0H1JUTi4ibDZUoy2AajMAZg9UIyUlfYAVY9wzLQBBHyZs/YA1WotMQ8CbR/2AfslERSD4nD8rNd9upjpCrvsAJ6wn4AEVh9weZkCaKOf0xjy2Oz22EOJzOLAu11uGIwTGB2HUqVI70%2BmGOHhuR1w%2B3QqQSLBIpFsEA%2BqC%2B7M53NpwP2LHILLFbJOkv20qI%2B1iCwhGMB%2BwA9Hr9h4kMZgCSSIcAGxaUXi5XU637C10dROpAkjA4HWA0jYIirJgK8W/AB0ruOKNlYcO1g1YflIYRpMjLETMchsUTWJxeOzGIxEXVbBYEQgWqxeN1tqVHkLt32gnQFbzuO9AOr7LrXI1TQr0Yj%2BwArAmkwOeFoW5WfX6A/taOh0Ome6QFmH0Y8848uEt6NxB/wAlwdOR0NwPLZbECVmsSZYjnxyERtNulgBrEAWQchixJH8ADiOP8tD/QI/ySQceGHXcuGkfg2BAQctHIQ9j1PLh%2BCUEAkKfI9t3IOBYBQT08EIQVKGoOhGFYDhuAfQRhDECROBkORhGUNRNFw8h9AsQxTTQC9rHsbBHGcVwIHcCZ2hCJhMFmIZEimdJMjEnJvDaAxlOKJh5IqYYeGE0T6jGFp1MmQzalU3pml0%2BYpmBPopIMBybIiQY9MUpYlGvdZuEJXYBVvMlzkuSUMWeV5q21f5AWBUFwQrKEYVxKEkRSyd82xdd8QefziVJU4QqpLkaSEelGWZDt7W7Xl%2BUFYUqo5es1VleVGpVFrNWih5dQNI0TRMc1FzMa12odLQnUXF03Q9LBsDbfZfX9UhAw7UNwyOFN%2B1jTN1BHZMo1HHasyy1sNyyjdHkLfZi1LctfgWxqu25Rtm1Oqd21ZTthHrWJe3Wg7h32NMxwnd6FqW2d52GnamlXZLcwuh48Og/dkOfE8z0EyFvNWdYYyOCx%2BBwnQFiWd0WBwRIy0MbhYPIeDEPRri0IwrDH2fMnyHfI4jhDS0LEte9AhkCweAsCw/yg7gjgPDHWY53ClgI5A0HQHYGASciIAwDXGESckTA6LQDJwAA3Ah1jeAhsAAdwAeT5Q86IYLZSEwiBYgx2IIiaBFaP4H3WFIBF7diXRLID8hdY4YR7aYeh/a4nBYh8YAPAkehMN4fgcGLExJGTghfTqM3sGz48GVqHwtijwsRIx%2BgCFiUg/a8HAMZBAh4JzpYaCMYAlGtu3HeYKP6NEcRJBYif2I0DH9AMowTAE6xbEMZvMMgJZeVU7OAFp7cuffgSVFFzDX6wLFifh0DL0hSAIHAt5prpVLcWSnIMsI3LmfTChUtkL%2BADtK2X/m/YyjkzLSQgT0EyYDFLTHGNA5y8Df4KSkF5HyzEdx7jlizbg0I/yWn3paaQqpl7AH2B0EMWgQw8DnPgYgZACY8AWMTTm%2BEiLq1SJrCgVAdY8L4WgShPBrRIXNpbbAw8HZO3Hq7BIHsvZcSDn7KOqiQ5hwjk4KOMdmBEHjonDGKc04Z3oFnKOedTSF2PIQEuzgy4V34FXVANcNgPnrtBY8TcW5tzmp3R%2BPc%2BB9wHkPG2six451YpPJiUhZCzxUPPLiPE%2BIrwvjYISPiX471SHvbgJ8iBn3SbYa%2Bt976P2fvALyIlLLZA/p4FB39ZIIM0kUVSwCtKqRaQZWBTBrKmTyDAmp3Q%2BloLKBgnpJlgEuX6OgjymDlh404BYXBXA0YoVvoQ9QxDSHkMNlQsRtDaGMNIiwu8KyOFKzfFIP8IZpB/mkOBQI0hxwWHuYBWQ0F6YbMxuhew7MSZ4RVhAFAhAaA0G1hPRi094nyDnpxbxSBMK8XoEisFNAiAIj5B%2BQwSKGQghYLvOpfzSBIoMKSpQ6LMXYt4hS/FrciWuHQqs9Z8tuAogIOC944SEhEJIWQihppqGWiORNM2lxtn8r2ZQ6hgQjkPkBVzd80g5V/jVYET80gVUCxAsEL5%2BDULcDZthTmqyibM0NX8xVSx76ZFcNIIAA%3D).

```c++
template <class T>
union Vec2d {
    struct { T x; T y; };
};

template <class T>
constexpr Vec2d<T> operator+(Vec2d<T> const a, Vec2d<T> const b) {
    // Change to `Vec2d<T>` to fix the code
    return Vec2d{.x = a.x + b.x, .y = a.y + b.y};
}

int main() {
    Vec2d<int> foo{};
    Vec2d<int> bar{.x = 5, .y = 10};
    return (foo + bar).x;
}
```

## Comments

### Comment 1 - shafik

I reduced this a bunch to remove things not involved:  https://godbolt.org/z/YceqcxosP

```cpp
template <class T>
union Vec2d {
    struct { T x; };
};

Vec2d<int> f(Vec2d<int> const a) {
    // Change to `Vec2d<int>` to fix the code
    return Vec2d{.x = a.x + a.x};
}

void g() {
    f(Vec2d<int>{});
}
```

Assertion:

```console
clang++: /root/llvm-project/clang/lib/Sema/SemaInit.cpp:1449:
void {anonymous}::InitListChecker::CheckSubElementType(const clang::InitializedEntity&, clang::InitListExpr*, clang::QualType, unsigned int&, clang::InitListExpr*, unsigned int&, bool):
Assertion `AggrDeductionCandidateParamTypes' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:8:33: current parser token ';'
2.	<source>:6:34: parsing function body 'f'
3.	<source>:6:34: in compound statement ('{}')
 #0 0x00000000036e2ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36e2ff8)
 #1 0x00000000036e0cbc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36e0cbc)
 #2 0x000000000362a078 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f1515578420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f151503b00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f151501a859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f151501a729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f151502bfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000000000659fb63 (anonymous namespace)::InitListChecker::CheckSubElementType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
 #9 0x00000000065a3ecd (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#10 0x00000000065a4bc0 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#11 0x00000000065a4bc0 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#12 0x00000000065a1a72 (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, llvm::iterator_range<clang::CXXBaseSpecifier const*>, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
#13 0x000000000659fead (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
#14 0x000000000658fffa (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#15 0x000000000659097e (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#16 0x0000000006598eca TryListInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, clang::InitListExpr*, clang::InitializationSequence&, bool) SemaInit.cpp:0:0
#17 0x0000000006596e6b clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6596e6b)
#18 0x00000000064b1931 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b1931)
#19 0x000000000653cc6d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653cc6d)
#20 0x0000000005ec7846 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ec7846)
#21 0x0000000005ea8b85 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ea8b85)
#22 0x0000000005ea89cd clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ea89cd)
#23 0x0000000005eaa70a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5eaa70a)
#24 0x0000000005eaa8a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5eaa8a9)
#25 0x0000000005eaf5a9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5eaf5a9)
#26 0x0000000005f1d2e2 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f1d2e2)
#27 0x0000000005f205fb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f205fb)
#28 0x0000000005f21bf8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f21bf8)
#29 0x0000000005f22b39 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f22b39)
#30 0x0000000005f2445a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f2445a)
#31 0x0000000005e51b41 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e51b41)
#32 0x0000000005e79128 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e79128)
#33 0x0000000005e45a2b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e45a2b)
#34 0x0000000005e4615f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#35 0x0000000005e4c874 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e4c874)
#36 0x0000000005e4d09d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e4d09d)
#37 0x0000000005e413da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e413da)
#38 0x0000000004946658 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946658)
#39 0x00000000041ac459 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ac459)
#40 0x000000000412d13e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412d13e)
#41 0x000000000428b79e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x428b79e)
#42 0x0000000000bdb92e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbdb92e)
#43 0x0000000000bd34ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#44 0x0000000003f8d929 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x000000000362a524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x362a524)
#46 0x0000000003f8df1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000003f56265 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f56265)
#48 0x0000000003f56ccd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f56ccd)
#49 0x0000000003f5ebf5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f5ebf5)
#50 0x0000000000bd909c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd909c)
#51 0x0000000000ad4921 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xad4921)
#52 0x00007f151501c083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#53 0x0000000000bd2f9e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd2f9e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

<details>
The following invalid code compiled with `-std=c++2b` causes the parser to crash in clang++ 17 and 18, but not 16. Here's a [live example](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TKQB2cncAGQImbAA5PwAjbFIQAE5yAAd0JWIXJi9ffyC0jKyhcMiYtnjElIdsJ2yRIhZSIly/AJ5gmrqhBqaiUui4hOT7RubW/I7RvoHyyuSASnt0H1JUTi4ibDZUoy2AajMAZg9UIyUlfYAVY9wzLQBBHyZs/YA1WotMQ8CbR/2AfslERSD4nD8rNd9upjpCrvsAJ6wn4AEVh9weZkCaKOf0xjy2Oz22EOJzOLAu11uGIwTGB2HUqVI70%2BmGOHhuR1w%2B3QqQSLBIpFsEA%2BqC%2B7M53NpwP2LHILLFbJOkv20qI%2B1iCwhGMB%2BwA9Hr9h4kMZgCSSIcAGxaUXi5XU637C10dROpAkjA4HWA0jYIirJgK8W/AB0ruOKNlYcO1g1YflIYRpMjLETMchsUTWJxeOzGIxEXVbBYEQgWqxeN1tqVHkLt32gnQFbzuO9AOr7LrXI1TQr0Yj%2BwArAmkwOeFoW5WfX6A/taOh0Ome6QFmH0Y8848uEt6NxB/wAlwdOR0NwPLZbECVmsSZYjnxyERtNulgBrEAWQchixJH8ADiOP8tD/QI/ySQceGHXcuGkfg2BAQctHIQ9j1PLh%2BCUEAkKfI9t3IOBYBQT08EIQVKGoOhGFYDhuAfQRhDECROBkORhGUNRNFw8h9AsQxTTQC9rHsbBHGcVwIHcCZ2hCJhMFmIZEimdJMjEnJvDaAxlOKJh5IqYYeGE0T6jGFp1MmQzalU3pml0%2BYpmBPopIMBybIiQY9MUpYlGvdZuEJXYBVvMlzkuSUMWeV5q21f5AWBUFwQrKEYVxKEkRSyd82xdd8QefziVJU4QqpLkaSEelGWZDt7W7Xl%2BUFYUqo5es1VleVGpVFrNWih5dQNI0TRMc1FzMa12odLQnUXF03Q9LBsDbfZfX9UhAw7UNwyOFN%2B1jTN1BHZMo1HHasyy1sNyyjdHkLfZi1LctfgWxqu25Rtm1Oqd21ZTthHrWJe3Wg7h32NMxwnd6FqW2d52GnamlXZLcwuh48Og/dkOfE8z0EyFvNWdYYyOCx%2BBwnQFiWd0WBwRIy0MbhYPIeDEPRri0IwrDH2fMnyHfI4jhDS0LEte9AhkCweAsCw/yg7gjgPDHWY53ClgI5A0HQHYGASciIAwDXGESckTA6LQDJwAA3Ah1jeAhsAAdwAeT5Q86IYLZSEwiBYgx2IIiaBFaP4H3WFIBF7diXRLID8hdY4YR7aYeh/a4nBYh8YAPAkehMN4fgcGLExJGTghfTqM3sGz48GVqHwtijwsRIx%2BgCFiUg/a8HAMZBAh4JzpYaCMYAlGtu3HeYKP6NEcRJBYif2I0DH9AMowTAE6xbEMZvMMgJZeVU7OAFp7cuffgSVFFzDX6wLFifh0DL0hSAIHAt5prpVLcWSnIMsI3LmfTChUtkL%2BADtK2X/m/YyjkzLSQgT0EyYDFLTHGNA5y8Df4KSkF5HyzEdx7jlizbg0I/yWn3paaQqpl7AH2B0EMWgQw8DnPgYgZACY8AWMTTm%2BEiLq1SJrCgVAdY8L4WgShPBrRIXNpbbAw8HZO3Hq7BIHsvZcSDn7KOqiQ5hwjk4KOMdmBEHjonDGKc04Z3oFnKOedTSF2PIQEuzgy4V34FXVANcNgPnrtBY8TcW5tzmp3R%2BPc%2BB9wHkPG2six451YpPJiUhZCzxUPPLiPE%2BIrwvjYISPiX471SHvbgJ8iBn3SbYa%2Bt976P2fvALyIlLLZA/p4FB39ZIIM0kUVSwCtKqRaQZWBTBrKmTyDAmp3Q%2BloLKBgnpJlgEuX6OgjymDlh404BYXBXA0YoVvoQ9QxDSHkMNlQsRtDaGMNIiwu8KyOFKzfFIP8IZpB/mkOBQI0hxwWHuYBWQ0F6YbMxuhew7MSZ4RVhAFAhAaA0G1hPRi094nyDnpxbxSBMK8XoEisFNAiAIj5B%2BQwSKGQghYLvOpfzSBIoMKSpQ6LMXYt4hS/FrciWuHQqs9Z8tuAogIOC944SEhEJIWQihppqGWiORNM2lxtn8r2ZQ6hgQjkPkBVzd80g5V/jVYET80gVUCxAsEL5%2BDULcDZthTmqyibM0NX8xVSx76ZFcNIIAA%3D).

```c++
template &lt;class T&gt;
union Vec2d {
    struct { T x; T y; };
};

template &lt;class T&gt;
constexpr Vec2d&lt;T&gt; operator+(Vec2d&lt;T&gt; const a, Vec2d&lt;T&gt; const b) {
    // Change to `Vec2d&lt;T&gt;` to fix the code
    return Vec2d{.x = a.x + b.x, .y = a.y + b.y};
}

int main() {
    Vec2d&lt;int&gt; foo{};
    Vec2d&lt;int&gt; bar{.x = 5, .y = 10};
    return (foo + bar).x;
}
```
</details>


---

### Comment 3 - shafik

This is a regression I suspect it is related to:  https://reviews.llvm.org/D139837 

CC @yuanfang-chen

Also CC @Fznamznon who has looked at designated init before and may wish to look at this as well.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

<details>
The following invalid code compiled with `-std=c++2b` causes the parser to crash in clang++ 17 and 18, but not 16. Here's a [live example](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TKQB2cncAGQImbAA5PwAjbFIQAE5yAAd0JWIXJi9ffyC0jKyhcMiYtnjElIdsJ2yRIhZSIly/AJ5gmrqhBqaiUui4hOT7RubW/I7RvoHyyuSASnt0H1JUTi4ibDZUoy2AajMAZg9UIyUlfYAVY9wzLQBBHyZs/YA1WotMQ8CbR/2AfslERSD4nD8rNd9upjpCrvsAJ6wn4AEVh9weZkCaKOf0xjy2Oz22EOJzOLAu11uGIwTGB2HUqVI70%2BmGOHhuR1w%2B3QqQSLBIpFsEA%2BqC%2B7M53NpwP2LHILLFbJOkv20qI%2B1iCwhGMB%2BwA9Hr9h4kMZgCSSIcAGxaUXi5XU637C10dROpAkjA4HWA0jYIirJgK8W/AB0ruOKNlYcO1g1YflIYRpMjLETMchsUTWJxeOzGIxEXVbBYEQgWqxeN1tqVHkLt32gnQFbzuO9AOr7LrXI1TQr0Yj%2BwArAmkwOeFoW5WfX6A/taOh0Ome6QFmH0Y8848uEt6NxB/wAlwdOR0NwPLZbECVmsSZYjnxyERtNulgBrEAWQchixJH8ADiOP8tD/QI/ySQceGHXcuGkfg2BAQctHIQ9j1PLh%2BCUEAkKfI9t3IOBYBQT08EIQVKGoOhGFYDhuAfQRhDECROBkORhGUNRNFw8h9AsQxTTQC9rHsbBHGcVwIHcCZ2hCJhMFmIZEimdJMjEnJvDaAxlOKJh5IqYYeGE0T6jGFp1MmQzalU3pml0%2BYpmBPopIMBybIiQY9MUpYlGvdZuEJXYBVvMlzkuSUMWeV5q21f5AWBUFwQrKEYVxKEkRSyd82xdd8QefziVJU4QqpLkaSEelGWZDt7W7Xl%2BUFYUqo5es1VleVGpVFrNWih5dQNI0TRMc1FzMa12odLQnUXF03Q9LBsDbfZfX9UhAw7UNwyOFN%2B1jTN1BHZMo1HHasyy1sNyyjdHkLfZi1LctfgWxqu25Rtm1Oqd21ZTthHrWJe3Wg7h32NMxwnd6FqW2d52GnamlXZLcwuh48Og/dkOfE8z0EyFvNWdYYyOCx%2BBwnQFiWd0WBwRIy0MbhYPIeDEPRri0IwrDH2fMnyHfI4jhDS0LEte9AhkCweAsCw/yg7gjgPDHWY53ClgI5A0HQHYGASciIAwDXGESckTA6LQDJwAA3Ah1jeAhsAAdwAeT5Q86IYLZSEwiBYgx2IIiaBFaP4H3WFIBF7diXRLID8hdY4YR7aYeh/a4nBYh8YAPAkehMN4fgcGLExJGTghfTqM3sGz48GVqHwtijwsRIx%2BgCFiUg/a8HAMZBAh4JzpYaCMYAlGtu3HeYKP6NEcRJBYif2I0DH9AMowTAE6xbEMZvMMgJZeVU7OAFp7cuffgSVFFzDX6wLFifh0DL0hSAIHAt5prpVLcWSnIMsI3LmfTChUtkL%2BADtK2X/m/YyjkzLSQgT0EyYDFLTHGNA5y8Df4KSkF5HyzEdx7jlizbg0I/yWn3paaQqpl7AH2B0EMWgQw8DnPgYgZACY8AWMTTm%2BEiLq1SJrCgVAdY8L4WgShPBrRIXNpbbAw8HZO3Hq7BIHsvZcSDn7KOqiQ5hwjk4KOMdmBEHjonDGKc04Z3oFnKOedTSF2PIQEuzgy4V34FXVANcNgPnrtBY8TcW5tzmp3R%2BPc%2BB9wHkPG2six451YpPJiUhZCzxUPPLiPE%2BIrwvjYISPiX471SHvbgJ8iBn3SbYa%2Bt976P2fvALyIlLLZA/p4FB39ZIIM0kUVSwCtKqRaQZWBTBrKmTyDAmp3Q%2BloLKBgnpJlgEuX6OgjymDlh404BYXBXA0YoVvoQ9QxDSHkMNlQsRtDaGMNIiwu8KyOFKzfFIP8IZpB/mkOBQI0hxwWHuYBWQ0F6YbMxuhew7MSZ4RVhAFAhAaA0G1hPRi094nyDnpxbxSBMK8XoEisFNAiAIj5B%2BQwSKGQghYLvOpfzSBIoMKSpQ6LMXYt4hS/FrciWuHQqs9Z8tuAogIOC944SEhEJIWQihppqGWiORNM2lxtn8r2ZQ6hgQjkPkBVzd80g5V/jVYET80gVUCxAsEL5%2BDULcDZthTmqyibM0NX8xVSx76ZFcNIIAA%3D).

```c++
template &lt;class T&gt;
union Vec2d {
    struct { T x; T y; };
};

template &lt;class T&gt;
constexpr Vec2d&lt;T&gt; operator+(Vec2d&lt;T&gt; const a, Vec2d&lt;T&gt; const b) {
    // Change to `Vec2d&lt;T&gt;` to fix the code
    return Vec2d{.x = a.x + b.x, .y = a.y + b.y};
}

int main() {
    Vec2d&lt;int&gt; foo{};
    Vec2d&lt;int&gt; bar{.x = 5, .y = 10};
    return (foo + bar).x;
}
```
</details>


---

### Comment 5 - Fznamznon

Reduced further:
```
template <class T>
struct Vec2d {
    struct { T x; };
};

void f() {
    // Change to `Vec2d<int>` to fix the code
    Vec2d v{.x = 1};
}
```
It is only important to have a designated init for a nested member and CTAD. Not quite clear what happens yet. The failing assertion is introduced by https://reviews.llvm.org/D139837 . https://godbolt.org/z/b4EzefEsa

---

### Comment 6 - cor3ntin

@hokein 

---

