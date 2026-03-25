# clang crash on x86_64-linux-gnu: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/120084
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash, regression:14
**Closed Date:** 2024-12-16T22:35:36Z

## Body

Compiler Explorer: https://godbolt.org/z/4Prz7vEfW
```console
<source>:3:2: error: expected ';' after union
    3 | }    long double __mutated_field__;
      |  ^
      |  ;
<source>:1:1: warning: declaration does not declare anything [-Wmissing-declarations]
    1 | union  {
      | ^
<source>:4:2: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    4 |  a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
      |  ^
      |  int
<source>:4:10: error: incompatible pointer to integer conversion initializing 'int' with an expression of type 'unsigned short[3]' [-Wint-conversion]
    4 |  a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
      |          ^~~~~
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ConstantArrayType; From = const clang::ArrayType]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:79: current parser token ';'
 #0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007489e0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007489e06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007489e0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007489e06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007489e062871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007489e0639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006c8182f decltype(auto) llvm::cast<clang::ConstantArrayType, clang::ArrayType const>(clang::ArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c8182f)
#10 0x0000000006ca858e (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#11 0x0000000006cab020 (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) SemaInit.cpp:0:0
#12 0x0000000006ca4b59 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
#13 0x0000000006c9327a (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#14 0x0000000006c93c7e (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#15 0x0000000006c94005 diagnoseListInit(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*) SemaInit.cpp:0:0
#16 0x0000000006c9eeb4 clang::InitializationSequence::Diagnose(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::ArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c9eeb4)
#17 0x0000000006c945b5 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c945b5)
#18 0x00000000069675d9 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69675d9)
#19 0x000000000660005a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660005a)
#20 0x000000000660f0f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660f0f9)
#21 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#22 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#23 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#24 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#25 0x00000000065ca20a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#26 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#27 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#28 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#29 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#30 0x0000000000ce1c7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#31 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000000004624ad9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#34 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#36 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#37 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#38 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#39 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#40 0x00007489e0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x00007489e0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
union  {
  __CHAR16_TYPE__ s[2];
}    long double __mutated_field__;
 a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/4Prz7vEfW
```console
&lt;source&gt;:3:2: error: expected ';' after union
    3 | }    long double __mutated_field__;
      |  ^
      |  ;
&lt;source&gt;:1:1: warning: declaration does not declare anything [-Wmissing-declarations]
    1 | union  {
      | ^
&lt;source&gt;:4:2: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    4 |  a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
      |  ^
      |  int
&lt;source&gt;:4:10: error: incompatible pointer to integer conversion initializing 'int' with an expression of type 'unsigned short[3]' [-Wint-conversion]
    4 |  a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
      |          ^~~~~
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ConstantArrayType; From = const clang::ArrayType]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:79: current parser token ';'
 #<!-- -->0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #<!-- -->1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #<!-- -->2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007489e0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007489e06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007489e0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007489e06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007489e062871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007489e0639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006c8182f decltype(auto) llvm::cast&lt;clang::ConstantArrayType, clang::ArrayType const&gt;(clang::ArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c8182f)
#<!-- -->10 0x0000000006ca858e (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->11 0x0000000006cab020 (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) SemaInit.cpp:0:0
#<!-- -->12 0x0000000006ca4b59 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->13 0x0000000006c9327a (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->14 0x0000000006c93c7e (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
#<!-- -->15 0x0000000006c94005 diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) SemaInit.cpp:0:0
#<!-- -->16 0x0000000006c9eeb4 clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c9eeb4)
#<!-- -->17 0x0000000006c945b5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c945b5)
#<!-- -->18 0x00000000069675d9 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69675d9)
#<!-- -->19 0x000000000660005a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660005a)
#<!-- -->20 0x000000000660f0f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660f0f9)
#<!-- -->21 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#<!-- -->22 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#<!-- -->23 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#<!-- -->24 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#<!-- -->25 0x00000000065ca20a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#<!-- -->26 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#<!-- -->27 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#<!-- -->28 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#<!-- -->29 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#<!-- -->30 0x0000000000ce1c7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#<!-- -->31 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004624ad9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#<!-- -->34 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#<!-- -->36 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#<!-- -->37 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#<!-- -->38 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#<!-- -->39 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#<!-- -->40 0x00007489e0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007489e0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
union  {
  __CHAR16_TYPE__ s[2];
}    long double __mutated_field__;
 a[] = { u"ff", [0].s[0] = u'x', [1] = u"\u1234\u4567", [1].s[0] = u'\u89ab' };
```
</details>


---

### Comment 2 - shafik

This goes back to clang-14: https://godbolt.org/z/Y81zsv3vb

---

### Comment 3 - shafik

duplicate https://github.com/llvm/llvm-project/issues/110775

---

### Comment 4 - shafik

It looks like you are hitting similar bugs to @yijan4845 if you are using similar techniques please figure out how to coordinate your work to prevent duplicating work which eats llvm community resources.

---

