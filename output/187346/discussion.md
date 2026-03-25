# [clang] Assertion `EC.isDependent() && "delaying non-dependent access"' failed.

**Author:** nuudlman
**URL:** https://github.com/llvm/llvm-project/issues/187346
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid
**Closed Date:** 2026-03-24T14:16:45Z

## Body

[Reproducer:](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIABykrgAyeAyYAHI%2BAEaYxBJcpAAOqAqETgwe3r4ByanpAqHhUSyx8VyJdpgOGUIETMQEWT5%2BgVU1AnUNBEWRMXEJtvWNzTltwz1hfaUDFQCUtqhexMjsHASYLEkGGwDUJgDMbrsEAJ5JjKyYu6Qn55ds%2BwfYJhoAgsgGCgq7TK9vu12zDYCiSTFWgJMAHYrO9AYCNlsdtdDsczhdgddok8XnD4Z8mN9fqjdtFUJ5btjDthITD/vCGaYDrCAQzdlRiHhXICmE8WQzoQARE6bbZMPYk9EPLE4%2BnwxFiiVHXboWV4wEAN1QeFVf2VZIppJxIBAmAgc32dPV%2BI4C1onAArLw/BwtKRUJw3NZrLsFEsViizAceKQCJpbQsANYgA4HAB0scTSeTADZ9JxJM7w%2B7OLwFCANKHwws4LAkGgtnQ4uRKBWklX4gSjGYKoSlI0%2BHQNsR8xBotnomEGqdOCHB8xiKcAPLRbTVMPcXgVtiCKcMWgj128LDRLzANxiWj5xekLAsQzAcRb094YjzvAazDHt2YVTVLwbUe8MIbe3X2h4NExDDh4WDZgQnIsF%2BpCPsQZJKIKmwXgBRjFnwBjAAoABqXIAO5Thi0H8IIIhiOwUgyIIigqOo166IkBioaYljWPogH5pACyoEkjgCMevCoLBnJYBx5q2Jg9i8X4ECuKMfiJCEUwlGUegpGkUlyap%2BRSb0ymzOJkm1BMmmVBJ96dBMun9OUQzdCZtmNFZMzlAsfrLKsEh2o6WbXh6HC7Ko/gpgAtCmki7E2wC7C2cZcHGGi7BAbZxFJCgWhAuCECQ%2BxBlwcy8AuWhzFGMbxsm5WJmmf6ZqQLpun5eYFkWW4ljAiAgEsBBJB%2BNYQHWDYRFcnCBSFYURYxUUxXFCVJd8KUZGlvCYPgRDCXoxHCKI4gURt1FqNm9GkLhwFJF%2BXkcE6tXZn5U4ft1BC7KgVABUFoXhZF0VxdNiXJY0C3pR4lb0MQOXBvlzVFSVsYJhV5XphwNV1QJua2E1hURvDZg%2BfVKPo8VMFxGkziSEAA%3D%3D)

```cpp
template < typename , typename >
class a
  namespace  {
    template < typename b >
    class a< bool, b >  {
       c;
      friend  a ;
    } template < typename b >
    template < d >
    void a< bool, b >::e() {
      c
```

Backtrace:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaAccess.cpp:1417: void DelayDependentAccess(clang::Sema&, const {anonymous}::EffectiveContext&, clang::SourceLocation, const {anonymous}::AccessTarget&): Assertion `EC.isDependent() && "delaying non-dependent access"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:11:7: at annotation token
2.	<source>:3:3: parsing namespace '(anonymous)'
3.	<source>:10:28: parsing function body
4.	<source>:10:28: in compound statement ('{}')
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072fa36042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072fa360969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072fa36042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072fa360287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072fa3602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072fa36039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000068ce034 CheckEffectiveAccess(clang::Sema&, (anonymous namespace)::EffectiveContext const&, clang::SourceLocation, (anonymous namespace)::AccessTarget&) SemaAccess.cpp:0:0
#10 0x00000000068ce8d6 CheckAccess(clang::Sema&, clang::SourceLocation, (anonymous namespace)::AccessTarget&) SemaAccess.cpp:0:0
#11 0x00000000068d1e76 clang::Sema::CheckLookupAccess(clang::LookupResult const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x68d1e76)
#12 0x00000000068e7782 clang::LookupResult::~LookupResult() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x68e7782)
#13 0x0000000006a35db8 clang::Sema::ActOnNameClassifiedAsOverloadSet(clang::Scope*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6a35db8)
#14 0x000000000674a88f clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674a88f)
#15 0x000000000674ad37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674ad37)
#16 0x000000000674adc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674adc9)
#17 0x000000000674f479 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674f479)
#18 0x00000000067e3f89 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e3f89)
#19 0x00000000067db70c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67db70c)
#20 0x00000000067dc549 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67dc549)
#21 0x00000000067e467b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e467b)
#22 0x00000000067e4eca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e4eca)
#23 0x00000000066dc7c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66dc7c3)
#24 0x00000000067125bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67125bd)
#25 0x00000000067f0698 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67f0698)
#26 0x00000000067fe2cf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#27 0x00000000067fe67a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67fe67a)
#28 0x000000000671aee0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671aee0)
#29 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#30 0x0000000006734ba5 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6734ba5)
#31 0x0000000006735826 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6735826)
#32 0x000000000671af20 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671af20)
#33 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#34 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#35 0x00000000066ca73a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#36 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#37 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#38 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#39 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#40 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#41 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#44 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#46 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#47 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#48 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#49 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#50 0x000072fa36029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x000072fa36029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Isaac Nudelman (nuudlman)

<details>
[Reproducer:](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIABykrgAyeAyYAHI%2BAEaYxBJcpAAOqAqETgwe3r4ByanpAqHhUSyx8VyJdpgOGUIETMQEWT5%2BgVU1AnUNBEWRMXEJtvWNzTltwz1hfaUDFQCUtqhexMjsHASYLEkGGwDUJgDMbrsEAJ5JjKyYu6Qn55ds%2BwfYJhoAgsgGCgq7TK9vu12zDYCiSTFWgJMAHYrO9AYCNlsdtdDsczhdgddok8XnD4Z8mN9fqjdtFUJ5btjDthITD/vCGaYDrCAQzdlRiHhXICmE8WQzoQARE6bbZMPYk9EPLE4%2BnwxFiiVHXboWV4wEAN1QeFVf2VZIppJxIBAmAgc32dPV%2BI4C1onAArLw/BwtKRUJw3NZrLsFEsViizAceKQCJpbQsANYgA4HAB0scTSeTADZ9JxJM7w%2B7OLwFCANKHwws4LAkGgtnQ4uRKBWklX4gSjGYKoSlI0%2BHQNsR8xBotnomEGqdOCHB8xiKcAPLRbTVMPcXgVtiCKcMWgj128LDRLzANxiWj5xekLAsQzAcRb094YjzvAazDHt2YVTVLwbUe8MIbe3X2h4NExDDh4WDZgQnIsF%2BpCPsQZJKIKmwXgBRjFnwBjAAoABqXIAO5Thi0H8IIIhiOwUgyIIigqOo166IkBioaYljWPogH5pACyoEkjgCMevCoLBnJYBx5q2Jg9i8X4ECuKMfiJCEUwlGUegpGkUlyap%2BRSb0ymzOJkm1BMmmVBJ96dBMun9OUQzdCZtmNFZMzlAsfrLKsEh2o6WbXh6HC7Ko/gpgAtCmki7E2wC7C2cZcHGGi7BAbZxFJCgWhAuCECQ%2BxBlwcy8AuWhzFGMbxsm5WJmmf6ZqQLpun5eYFkWW4ljAiAgEsBBJB%2BNYQHWDYRFcnCBSFYURYxUUxXFCVJd8KUZGlvCYPgRDCXoxHCKI4gURt1FqNm9GkLhwFJF%2BXkcE6tXZn5U4ft1BC7KgVABUFoXhZF0VxdNiXJY0C3pR4lb0MQOXBvlzVFSVsYJhV5XphwNV1QJua2E1hURvDZg%2BfVKPo8VMFxGkziSEAA%3D%3D)

```cpp
template &lt; typename , typename &gt;
class a
  namespace  {
    template &lt; typename b &gt;
    class a&lt; bool, b &gt;  {
       c;
      friend  a ;
    } template &lt; typename b &gt;
    template &lt; d &gt;
    void a&lt; bool, b &gt;::e() {
      c
```

Backtrace:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaAccess.cpp:1417: void DelayDependentAccess(clang::Sema&amp;, const {anonymous}::EffectiveContext&amp;, clang::SourceLocation, const {anonymous}::AccessTarget&amp;): Assertion `EC.isDependent() &amp;&amp; "delaying non-dependent access"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:11:7: at annotation token
2.	&lt;source&gt;:3:3: parsing namespace '(anonymous)'
3.	&lt;source&gt;:10:28: parsing function body
4.	&lt;source&gt;:10:28: in compound statement ('{}')
 #<!-- -->0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #<!-- -->1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #<!-- -->2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072fa36042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072fa360969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072fa36042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072fa360287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072fa3602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072fa36039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000068ce034 CheckEffectiveAccess(clang::Sema&amp;, (anonymous namespace)::EffectiveContext const&amp;, clang::SourceLocation, (anonymous namespace)::AccessTarget&amp;) SemaAccess.cpp:0:0
#<!-- -->10 0x00000000068ce8d6 CheckAccess(clang::Sema&amp;, clang::SourceLocation, (anonymous namespace)::AccessTarget&amp;) SemaAccess.cpp:0:0
#<!-- -->11 0x00000000068d1e76 clang::Sema::CheckLookupAccess(clang::LookupResult const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x68d1e76)
#<!-- -->12 0x00000000068e7782 clang::LookupResult::~LookupResult() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x68e7782)
#<!-- -->13 0x0000000006a35db8 clang::Sema::ActOnNameClassifiedAsOverloadSet(clang::Scope*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6a35db8)
#<!-- -->14 0x000000000674a88f clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674a88f)
#<!-- -->15 0x000000000674ad37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674ad37)
#<!-- -->16 0x000000000674adc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674adc9)
#<!-- -->17 0x000000000674f479 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x674f479)
#<!-- -->18 0x00000000067e3f89 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e3f89)
#<!-- -->19 0x00000000067db70c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67db70c)
#<!-- -->20 0x00000000067dc549 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67dc549)
#<!-- -->21 0x00000000067e467b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e467b)
#<!-- -->22 0x00000000067e4eca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67e4eca)
#<!-- -->23 0x00000000066dc7c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66dc7c3)
#<!-- -->24 0x00000000067125bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67125bd)
#<!-- -->25 0x00000000067f0698 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67f0698)
#<!-- -->26 0x00000000067fe2cf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->27 0x00000000067fe67a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67fe67a)
#<!-- -->28 0x000000000671aee0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671aee0)
#<!-- -->29 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#<!-- -->30 0x0000000006734ba5 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6734ba5)
#<!-- -->31 0x0000000006735826 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6735826)
#<!-- -->32 0x000000000671af20 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671af20)
#<!-- -->33 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#<!-- -->34 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#<!-- -->35 0x00000000066ca73a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#<!-- -->36 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#<!-- -->37 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#<!-- -->38 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#<!-- -->39 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#<!-- -->40 0x0000000000d93cff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#<!-- -->41 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x0000000004a36b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#<!-- -->44 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#<!-- -->46 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#<!-- -->47 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#<!-- -->48 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#<!-- -->49 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#<!-- -->50 0x000072fa36029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x000072fa36029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
```
</details>


---

### Comment 2 - hokein

no reproducible in clang v22.1.0.

---

