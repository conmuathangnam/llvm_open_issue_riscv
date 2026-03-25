# Clang crashes (SIGSEGV) in CTAD deduction guide generation when constructing FilteredPotentialEdgeIterator without explicit template argument

**Author:** sm5ve
**URL:** https://github.com/llvm/llvm-project/issues/181090

## Body

Clang crashes with a SIGSEGV during template argument deduction. The crash was originally encountered with valid C++, where a class template was constructed without explicit template arguments in a function whose return type already specifies them. A minimal reproducer was subsequently found via creduce. The crash can be observed in this [godbolt project](https://godbolt.org/z/czo4j4Ec4) using the `x86-64 clang (assertions trunk)` compiler. The problematic code is reproduced here:

```c++
template< typename > class a {
template< b>     class c {
c( a ) :  
namespace struct : a< int> 
template <bool b>          using d = c< b>;
e() {
             d()
}
}
}
```

I encountered this bug while working on unit tests for my hobby operating system, building for an arm64 macOS target (and host). In particular, I used clang 21.1.8_1 as distributed via homebrew. As shown in [this commit](https://github.com/sm5ve/CroCOS/commit/01d77ecb2f84c5723cb5351e5c1047f780d5b9f3#diff-1de442555f369155ea4971f6a238eab84d40286bcd68ffe4e27be98e9303f3ceR614) I was able to avoid triggering this crash by explicitly specifying the template parameters in the constructors of my return values (see InterruptGraphManager.cpp‎ lines 614 and 620). That is, I changed

```c++
return FilteredPotentialEdgeIterator(move(baseIterator), *this, target);
```

to

```c++
return FilteredPotentialEdgeIterator<false>(move(baseIterator), *this, target);
```

Below is the stack trace as produced on my machine:

<details>
<code>PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21 -cc1 -triple arm64-apple-macosx26.0.0 -O0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name InterruptGraphManager.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=2 -target-sdk-version=26.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debug-info-kind=standalone -dwarf-version=5 -debugger-tuning=lldb -fdebug-compilation-dir=/Users/user/Documents/CroCOS/tests/build/kernel -target-linker-version 1230.1 -fcoverage-compilation-dir=/Users/user/Documents/CroCOS/tests/build/kernel -sys-header-deps -D CORE_KERNEL_TESTING -D CORE_LINKED_WITH_KERNEL -D CORE_LIBRARY_TESTING -D CROCOS_TESTING -D CROCOS_TEST_INSTRUMENT_ALLOCATORS -c-isystem /opt/homebrew/Cellar/gmp/6.2.1/include -c-isystem /opt/homebrew/Cellar/mpfr/4.1.0/include -c-isystem . -Wall -Wextra -Wno-c++26-extensions -std=c++2c -fdeprecated-macro -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -fsanitize=address,leak -fsanitize-system-ignorelist=/opt/homebrew/Cellar/llvm/21.1.8_1/lib/clang/21/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ InterruptGraphManager-ce392a.cpp
1.	InterruptGraphManager-ce392a.cpp:8:1: current parser token '}'
2.	InterruptGraphManager-ce392a.cpp:4:11: parsing struct/union/class body '(unnamed struct at InterruptGraphManager-ce392a.cpp:4:11)'
3.	InterruptGraphManager-ce392a.cpp:6:5: parsing function body '(anonymous struct)::e'
4.	InterruptGraphManager-ce392a.cpp:6:5: in compound statement ('{}')
 #0 0x00000001159fcb04 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e48b04)
 #1 0x00000001159fd334 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e49334)
 #2 0x000000019403f744 (/usr/lib/system/libsystem_platform.dylib+0x1804e3744)
 #3 0x0000000105c0b078 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1157078)
 #4 0x0000000105c54a30 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11a0a30)
 #5 0x0000000105be1d90 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x112dd90)
 #6 0x0000000105bec8dc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11388dc)
 #7 0x0000000105be6304 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1132304)
 #8 0x0000000105c42db4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x118edb4)
 #9 0x0000000105bf12d0 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x113d2d0)
#10 0x0000000105c953e4 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11e13e4)
#11 0x0000000105c87664 clang::declvisitor::Base<std::__1::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11d3664)
#12 0x0000000105cb3f04 clang::Sema::InstantiateFunctionDeclaration(clang::FunctionTemplateDecl*, clang::TemplateArgumentList const*, clang::SourceLocation, clang::Sema::CodeSynthesisContext::SynthesisKind) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11fff04)
#13 0x0000000105bce0bc (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x111a0bc)
#14 0x0000000105bcc1c4 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11181c4)
#15 0x00000001059159e0 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xe619e0)
#16 0x00000001058e46cc clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xe306cc)
#17 0x0000000105855d38 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xda1d38)
#18 0x0000000104d6ec9c clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x2bac9c)
#19 0x0000000104d495a8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x2955a8)
#20 0x0000000104dbf054 clang::Parser::ParseCompoundStatementBody(bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x30b054)
#21 0x0000000104c96dc4 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1e2dc4)
#22 0x0000000104d2f874 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x27b874)
#23 0x0000000104d29b9c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x275b9c)
#24 0x0000000104cc9ae4 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x215ae4)
#25 0x0000000104dfcd44 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x348d44)
#26 0x0000000104c8ba48 clang::ParseAST(clang::Sema&, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1d7a48)
#27 0x00000001064d3b44 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a1fb44)
#28 0x000000010646af64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x19b6f64)
#29 0x0000000106508dd0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a54dd0)
#30 0x000000010076d3a0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x1000053a0)
#31 0x000000010076b550 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100003550)
#32 0x0000000100769ad4 clang_main(int, char**, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100001ad4)
#33 0x0000000100775810 main (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x10000d810)
#34 0x0000000193c6dd54</code>
</details>

Please let me know if there is any further context or information I can provide to assist with this (or any further guidance for how I can better organize and submit future bug reports)!

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (sm5ve)

<details>
Clang crashes with a SIGSEGV during template argument deduction. The crash was originally encountered with valid C++, where a class template was constructed without explicit template arguments in a function whose return type already specifies them. A minimal reproducer was subsequently found via creduce. The crash can be observed in this [godbolt project](https://godbolt.org/z/czo4j4Ec4) using the `x86-64 clang (assertions trunk)` compiler. The problematic code is reproduced here:

```c++
template&lt; typename &gt; class a {
template&lt; b&gt;     class c {
c( a ) :  
namespace struct : a&lt; int&gt; 
template &lt;bool b&gt;          using d = c&lt; b&gt;;
e() {
             d()
}
}
}
```

I encountered this bug while working on unit tests for my hobby operating system, building for an arm64 macOS target (and host). In particular, I used clang 21.1.8_1 as distributed via homebrew. As shown in [this commit](https://github.com/sm5ve/CroCOS/commit/01d77ecb2f84c5723cb5351e5c1047f780d5b9f3#diff-1de442555f369155ea4971f6a238eab84d40286bcd68ffe4e27be98e9303f3ceR614) I was able to avoid triggering this crash by explicitly specifying the template parameters in the constructors of my return values (see InterruptGraphManager.cpp‎ lines 614 and 620). That is, I changed

```c++
return FilteredPotentialEdgeIterator(move(baseIterator), *this, target);
```

to

```c++
return FilteredPotentialEdgeIterator&lt;false&gt;(move(baseIterator), *this, target);
```

Below is the stack trace as produced on my machine:

&lt;details&gt;
&lt;code&gt;PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21 -cc1 -triple arm64-apple-macosx26.0.0 -O0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name InterruptGraphManager.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=2 -target-sdk-version=26.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debug-info-kind=standalone -dwarf-version=5 -debugger-tuning=lldb -fdebug-compilation-dir=/Users/user/Documents/CroCOS/tests/build/kernel -target-linker-version 1230.1 -fcoverage-compilation-dir=/Users/user/Documents/CroCOS/tests/build/kernel -sys-header-deps -D CORE_KERNEL_TESTING -D CORE_LINKED_WITH_KERNEL -D CORE_LIBRARY_TESTING -D CROCOS_TESTING -D CROCOS_TEST_INSTRUMENT_ALLOCATORS -c-isystem /opt/homebrew/Cellar/gmp/6.2.1/include -c-isystem /opt/homebrew/Cellar/mpfr/4.1.0/include -c-isystem . -Wall -Wextra -Wno-c++26-extensions -std=c++2c -fdeprecated-macro -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -fsanitize=address,leak -fsanitize-system-ignorelist=/opt/homebrew/Cellar/llvm/21.1.8_1/lib/clang/21/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ InterruptGraphManager-ce392a.cpp
1.	InterruptGraphManager-ce392a.cpp:8:1: current parser token '}'
2.	InterruptGraphManager-ce392a.cpp:4:11: parsing struct/union/class body '(unnamed struct at InterruptGraphManager-ce392a.cpp:4:11)'
3.	InterruptGraphManager-ce392a.cpp:6:5: parsing function body '(anonymous struct)::e'
4.	InterruptGraphManager-ce392a.cpp:6:5: in compound statement ('{}')
 #<!-- -->0 0x00000001159fcb04 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e48b04)
 #<!-- -->1 0x00000001159fd334 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e49334)
 #<!-- -->2 0x000000019403f744 (/usr/lib/system/libsystem_platform.dylib+0x1804e3744)
 #<!-- -->3 0x0000000105c0b078 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1157078)
 #<!-- -->4 0x0000000105c54a30 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11a0a30)
 #<!-- -->5 0x0000000105be1d90 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x112dd90)
 #<!-- -->6 0x0000000105bec8dc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11388dc)
 #<!-- -->7 0x0000000105be6304 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1132304)
 #<!-- -->8 0x0000000105c42db4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;*, clang::Sema::ExtParameterInfoBuilder&amp;, unsigned int*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x118edb4)
 #<!-- -->9 0x0000000105bf12d0 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x113d2d0)
#<!-- -->10 0x0000000105c953e4 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11e13e4)
#<!-- -->11 0x0000000105c87664 clang::declvisitor::Base&lt;std::__1::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*&gt;::Visit(clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11d3664)
#<!-- -->12 0x0000000105cb3f04 clang::Sema::InstantiateFunctionDeclaration(clang::FunctionTemplateDecl*, clang::TemplateArgumentList const*, clang::SourceLocation, clang::Sema::CodeSynthesisContext::SynthesisKind) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11fff04)
#<!-- -->13 0x0000000105bce0bc (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&amp;, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x111a0bc)
#<!-- -->14 0x0000000105bcc1c4 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x11181c4)
#<!-- -->15 0x00000001059159e0 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xe619e0)
#<!-- -->16 0x00000001058e46cc clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xe306cc)
#<!-- -->17 0x0000000105855d38 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0xda1d38)
#<!-- -->18 0x0000000104d6ec9c clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x2bac9c)
#<!-- -->19 0x0000000104d495a8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x2955a8)
#<!-- -->20 0x0000000104dbf054 clang::Parser::ParseCompoundStatementBody(bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x30b054)
#<!-- -->21 0x0000000104c96dc4 clang::Parser::LexedMethod::ParseLexedMethodDefs() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1e2dc4)
#<!-- -->22 0x0000000104d2f874 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x27b874)
#<!-- -->23 0x0000000104d29b9c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x275b9c)
#<!-- -->24 0x0000000104cc9ae4 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x215ae4)
#<!-- -->25 0x0000000104dfcd44 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x348d44)
#<!-- -->26 0x0000000104c8ba48 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1d7a48)
#<!-- -->27 0x00000001064d3b44 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a1fb44)
#<!-- -->28 0x000000010646af64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x19b6f64)
#<!-- -->29 0x0000000106508dd0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a54dd0)
#<!-- -->30 0x000000010076d3a0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x1000053a0)
#<!-- -->31 0x000000010076b550 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100003550)
#<!-- -->32 0x0000000100769ad4 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100001ad4)
#<!-- -->33 0x0000000100775810 main (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x10000d810)
#<!-- -->34 0x0000000193c6dd54&lt;/code&gt;
&lt;/details&gt;

Please let me know if there is any further context or information I can provide to assist with this (or any further guidance for how I can better organize and submit future bug reports)!
</details>


---

### Comment 2 - zyn0217

If possible, can you please provide us with a valid reduced example? (Like, one can be compiled with GCC but can crash clang) The reduced one above doesn't make sense to me so it's hard to figure out where the actual problem was.

---

### Comment 3 - sm5ve

I will try to find a valid reduced example, but so far it is causing me some trouble. Please forgive me if it takes some time! I don't suppose there's a tool like creduce that only produces GCC-compilable examples, is there?

To be explicit, the unreduced code did compile under GCC.

---

### Comment 4 - zyn0217

> I will try to find a valid reduced example, but so far it is causing me some trouble. Please forgive me if it takes some time! I don't suppose there's a tool like creduce that only produces GCC-compilable examples, is there?
> 
> To be explicit, the unreduced code did compile under GCC.

You can always instruct creduce/cvise to test the code that compiles with GCC but reproduces crash with Clang, with the interestingness test script. Unless you're directly reducing from Clang preprocessed files which are usually not compatible with GCC.

---

