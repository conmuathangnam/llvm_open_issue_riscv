# [HLSL] Assert record is already complete triggered when compiler can't distinguish RWStructuredBuffer `a` and `b`

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/153619
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2026-02-02T21:40:25Z

## Body

The code below is very much not expected to compile. However it is also not expected to trigger an assert aswell

https://hlsl.godbolt.org/z/eYM3njEYr

## HLSL Code
```hlsl
RWStructuredBuffer<float16_t>;
RWStructuredBuffer<uint> a RWStructuredBuffer<float16_t> b
```
## Repro command
`clang-dxc bug.hlsl -E CSMain -T cs_6_2 -enable-16bit-types -O3`

## Crash Stack
```gdb
bug.hlsl:1:1: warning: declaration does not declare anything [-Wmissing-declarations]
    1 | RWStructuredBuffer<float16_t>;
      | ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bug.hlsl:2:27: error: expected ';' after top level declarator
    2 | RWStructuredBuffer<uint> a RWStructuredBuffer<float16_t> b
      |                           ^
      |                           ;
Assertion failed: (!Record->isCompleteDefinition() && "record is already complete"), function addHandleMember, file HLSLBuiltinTypeDeclBuilder.cpp, line 599.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-dxc bug.hlsl -E CSMain -T cs_6_2 -enable-16bit-types -O3
1.	<eof> parser at end of file
 #0 0x000000010623d3ec llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336d3ec)
 #1 0x000000010623da0c PrintStackTraceSignalHandler(void*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336da0c)
 #2 0x000000010623b2a8 llvm::sys::RunSignalHandlers() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336b2a8)
 #3 0x000000010623cbc0 llvm::sys::CleanupOnSignal(unsigned long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336cbc0)
 #4 0x00000001060dac68 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320ac68)
 #5 0x00000001060db14c CrashRecoverySignalHandler(int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320b14c)
 #6 0x00000001861c16a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #7 0x000000018618788c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #8 0x0000000186090a3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #9 0x000000018608fc70 (/usr/lib/system/libsystem_c.dylib+0x18037bc70)
#10 0x0000000109e45844 clang::hlsl::BuiltinTypeDeclBuilder::addHandleMember(llvm::dxil::ResourceClass, bool, bool, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f75844)
#11 0x0000000109e4f6ac setupBufferType(clang::CXXRecordDecl*, clang::Sema&, llvm::dxil::ResourceClass, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7f6ac)
#12 0x0000000109e55948 clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4::operator()(clang::CXXRecordDecl*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f85948)
#13 0x0000000109e558f8 decltype(std::declval<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&>()(std::declval<clang::CXXRecordDecl*>())) std::__1::__invoke[abi:nn180100]<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&, clang::CXXRecordDecl*>(clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&, clang::CXXRecordDecl*&&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f858f8)
#14 0x0000000109e558a4 void std::__1::__invoke_void_return_wrapper<void, true>::__call[abi:nn180100]<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&, clang::CXXRecordDecl*>(clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&, clang::CXXRecordDecl*&&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f858a4)
#15 0x0000000109e55878 std::__1::__function::__alloc_func<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4, std::__1::allocator<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4>, void (clang::CXXRecordDecl*)>::operator()[abi:nn180100](clang::CXXRecordDecl*&&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f85878)
#16 0x0000000109e547cc std::__1::__function::__func<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4, std::__1::allocator<clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4>, void (clang::CXXRecordDecl*)>::operator()(clang::CXXRecordDecl*&&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f847cc)
#17 0x0000000109e5f754 std::__1::__function::__value_func<void (clang::CXXRecordDecl*)>::operator()[abi:nn180100](clang::CXXRecordDecl*&&) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f8f754)
#18 0x0000000109e4d194 std::__1::function<void (clang::CXXRecordDecl*)>::operator()(clang::CXXRecordDecl*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7d194)
#19 0x0000000109e4cdc0 clang::HLSLExternalSemaSource::CompleteType(clang::TagDecl*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7cdc0)
#20 0x000000010ade33f4 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f133f4)
#21 0x000000010ade2db0 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f12db0)
#22 0x000000010ade4260 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f14260)
#23 0x0000000109ef0a00 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, unsigned int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107020a00)
#24 0x000000010a5ec554 clang::SemaHLSL::ActOnVariableDeclarator(clang::VarDecl*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10771c554)
#25 0x000000010a1f787c clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10732787c)
#26 0x000000010a1ef160 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10731f160)
#27 0x000000010a1ee470 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10731e470)
#28 0x0000000109ca67ac clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106dd67ac)
#29 0x0000000109ca4220 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106dd4220)
#30 0x0000000109dc3c7c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef3c7c)
#31 0x0000000109dc31b4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef31b4)
#32 0x0000000109dc2370 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef2370)
#33 0x0000000109dc0304 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef0304)
#34 0x0000000109c874e8 clang::ParseAST(clang::Sema&, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106db74e8)
#35 0x0000000107f49bb8 clang::ASTFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x105079bb8)
#36 0x000000010738dc38 clang::CodeGenAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1044bdc38)
#37 0x0000000107f493c8 clang::FrontendAction::Execute() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1050793c8)
#38 0x0000000107e5edb8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104f8edb8)
#39 0x0000000108091440 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1051c1440)
#40 0x0000000102ee3054 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100013054)
#41 0x0000000102ed527c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10000527c)
#42 0x0000000102ee1a60 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100011a60)
#43 0x0000000102ee1a30 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100011a30)
#44 0x0000000107b584cc llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c884cc)
#45 0x0000000107b5848c clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0::operator()() const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c8848c)
#46 0x0000000107b58458 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c88458)
#47 0x0000000104102a54 llvm::function_ref<void ()>::operator()() const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x101232a54)
#48 0x00000001060daab8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320aab8)
#49 0x0000000107b549a0 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c849a0)
#50 0x0000000107ae7fc8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c17fc8)
#51 0x0000000107ae8258 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c18258)
#52 0x0000000107b06640 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c36640)
#53 0x0000000102ed4a00 clang_main(int, char**, llvm::ToolContext const&) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100004a00)
#54 0x0000000102f10178 main (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100040178)
#55 0x0000000185de6b98
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (git@github.com:llvm/llvm-project.git b8e4232bd2fb326cca994dd88cfb249266d6c53e)
Target: dxilv1.2-unknown-shadermodel6.2-compute
Thread model: posix
InstalledDir: /Users/farzonlotfi/Projects/llvm_debug_build/bin
Build config: +unoptimized, +assertions
clang-dxc: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/bug-03eb5e.hlsl
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/bug-03eb5e.sh
clang-dxc: note: diagnostic msg: Crash backtrace is located in
clang-dxc: note: diagnostic msg: /Users/farzonlotfi/Library/Logs/DiagnosticReports/clang-20_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang-dxc: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang-dxc: note: diagnostic msg: 

********************

```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Farzon Lotfi (farzonl)

<details>
The code below is very much not expected to compile. However it is also not expected to trigger an assert aswell

https://hlsl.godbolt.org/z/eYM3njEYr

## HLSL Code
```hlsl
RWStructuredBuffer&lt;float16_t&gt;;
RWStructuredBuffer&lt;uint&gt; a RWStructuredBuffer&lt;float16_t&gt; b
```
## Repro command
`clang-dxc bug.hlsl -E CSMain -T cs_6_2 -enable-16bit-types -O3`

## Crash Stack
```gdb
bug.hlsl:1:1: warning: declaration does not declare anything [-Wmissing-declarations]
    1 | RWStructuredBuffer&lt;float16_t&gt;;
      | ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bug.hlsl:2:27: error: expected ';' after top level declarator
    2 | RWStructuredBuffer&lt;uint&gt; a RWStructuredBuffer&lt;float16_t&gt; b
      |                           ^
      |                           ;
Assertion failed: (!Record-&gt;isCompleteDefinition() &amp;&amp; "record is already complete"), function addHandleMember, file HLSLBuiltinTypeDeclBuilder.cpp, line 599.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-dxc bug.hlsl -E CSMain -T cs_6_2 -enable-16bit-types -O3
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x000000010623d3ec llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336d3ec)
 #<!-- -->1 0x000000010623da0c PrintStackTraceSignalHandler(void*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336da0c)
 #<!-- -->2 0x000000010623b2a8 llvm::sys::RunSignalHandlers() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336b2a8)
 #<!-- -->3 0x000000010623cbc0 llvm::sys::CleanupOnSignal(unsigned long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10336cbc0)
 #<!-- -->4 0x00000001060dac68 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320ac68)
 #<!-- -->5 0x00000001060db14c CrashRecoverySignalHandler(int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320b14c)
 #<!-- -->6 0x00000001861c16a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->7 0x000000018618788c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #<!-- -->8 0x0000000186090a3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #<!-- -->9 0x000000018608fc70 (/usr/lib/system/libsystem_c.dylib+0x18037bc70)
#<!-- -->10 0x0000000109e45844 clang::hlsl::BuiltinTypeDeclBuilder::addHandleMember(llvm::dxil::ResourceClass, bool, bool, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f75844)
#<!-- -->11 0x0000000109e4f6ac setupBufferType(clang::CXXRecordDecl*, clang::Sema&amp;, llvm::dxil::ResourceClass, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7f6ac)
#<!-- -->12 0x0000000109e55948 clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4::operator()(clang::CXXRecordDecl*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f85948)
#<!-- -->13 0x0000000109e558f8 decltype(std::declval&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&amp;&gt;()(std::declval&lt;clang::CXXRecordDecl*&gt;())) std::__1::__invoke[abi:nn180100]&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&amp;, clang::CXXRecordDecl*&gt;(clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&amp;, clang::CXXRecordDecl*&amp;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f858f8)
#<!-- -->14 0x0000000109e558a4 void std::__1::__invoke_void_return_wrapper&lt;void, true&gt;::__call[abi:nn180100]&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&amp;, clang::CXXRecordDecl*&gt;(clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&amp;, clang::CXXRecordDecl*&amp;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f858a4)
#<!-- -->15 0x0000000109e55878 std::__1::__function::__alloc_func&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4, std::__1::allocator&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&gt;, void (clang::CXXRecordDecl*)&gt;::operator()[abi:nn180100](clang::CXXRecordDecl*&amp;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f85878)
#<!-- -->16 0x0000000109e547cc std::__1::__function::__func&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4, std::__1::allocator&lt;clang::HLSLExternalSemaSource::defineHLSLTypesWithForwardDeclarations()::$_4&gt;, void (clang::CXXRecordDecl*)&gt;::operator()(clang::CXXRecordDecl*&amp;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f847cc)
#<!-- -->17 0x0000000109e5f754 std::__1::__function::__value_func&lt;void (clang::CXXRecordDecl*)&gt;::operator()[abi:nn180100](clang::CXXRecordDecl*&amp;&amp;) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f8f754)
#<!-- -->18 0x0000000109e4d194 std::__1::function&lt;void (clang::CXXRecordDecl*)&gt;::operator()(clang::CXXRecordDecl*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7d194)
#<!-- -->19 0x0000000109e4cdc0 clang::HLSLExternalSemaSource::CompleteType(clang::TagDecl*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106f7cdc0)
#<!-- -->20 0x000000010ade33f4 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f133f4)
#<!-- -->21 0x000000010ade2db0 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f12db0)
#<!-- -->22 0x000000010ade4260 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107f14260)
#<!-- -->23 0x0000000109ef0a00 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, unsigned int) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x107020a00)
#<!-- -->24 0x000000010a5ec554 clang::SemaHLSL::ActOnVariableDeclarator(clang::VarDecl*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10771c554)
#<!-- -->25 0x000000010a1f787c clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10732787c)
#<!-- -->26 0x000000010a1ef160 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10731f160)
#<!-- -->27 0x000000010a1ee470 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10731e470)
#<!-- -->28 0x0000000109ca67ac clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106dd67ac)
#<!-- -->29 0x0000000109ca4220 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106dd4220)
#<!-- -->30 0x0000000109dc3c7c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef3c7c)
#<!-- -->31 0x0000000109dc31b4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef31b4)
#<!-- -->32 0x0000000109dc2370 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef2370)
#<!-- -->33 0x0000000109dc0304 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106ef0304)
#<!-- -->34 0x0000000109c874e8 clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x106db74e8)
#<!-- -->35 0x0000000107f49bb8 clang::ASTFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x105079bb8)
#<!-- -->36 0x000000010738dc38 clang::CodeGenAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1044bdc38)
#<!-- -->37 0x0000000107f493c8 clang::FrontendAction::Execute() (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1050793c8)
#<!-- -->38 0x0000000107e5edb8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104f8edb8)
#<!-- -->39 0x0000000108091440 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x1051c1440)
#<!-- -->40 0x0000000102ee3054 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100013054)
#<!-- -->41 0x0000000102ed527c ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10000527c)
#<!-- -->42 0x0000000102ee1a60 clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100011a60)
#<!-- -->43 0x0000000102ee1a30 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100011a30)
#<!-- -->44 0x0000000107b584cc llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c884cc)
#<!-- -->45 0x0000000107b5848c clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c8848c)
#<!-- -->46 0x0000000107b58458 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c88458)
#<!-- -->47 0x0000000104102a54 llvm::function_ref&lt;void ()&gt;::operator()() const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x101232a54)
#<!-- -->48 0x00000001060daab8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x10320aab8)
#<!-- -->49 0x0000000107b549a0 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c849a0)
#<!-- -->50 0x0000000107ae7fc8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c17fc8)
#<!-- -->51 0x0000000107ae8258 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c18258)
#<!-- -->52 0x0000000107b06640 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x104c36640)
#<!-- -->53 0x0000000102ed4a00 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100004a00)
#<!-- -->54 0x0000000102f10178 main (/Users/farzonlotfi/Projects/llvm_debug_build/bin/clang-20+0x100040178)
#<!-- -->55 0x0000000185de6b98
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git b8e4232bd2fb326cca994dd88cfb249266d6c53e)
Target: dxilv1.2-unknown-shadermodel6.2-compute
Thread model: posix
InstalledDir: /Users/farzonlotfi/Projects/llvm_debug_build/bin
Build config: +unoptimized, +assertions
clang-dxc: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/bug-03eb5e.hlsl
clang-dxc: note: diagnostic msg: /var/folders/c5/8k67fxsn7gq5d835vtqt_rcm0000gn/T/bug-03eb5e.sh
clang-dxc: note: diagnostic msg: Crash backtrace is located in
clang-dxc: note: diagnostic msg: /Users/farzonlotfi/Library/Logs/DiagnosticReports/clang-20_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang-dxc: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang-dxc: note: diagnostic msg: 

********************

```


</details>


---

