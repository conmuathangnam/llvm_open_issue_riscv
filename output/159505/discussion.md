# Clang parser crash with -fptrauth-intrinsics enabled - Assertion failed: (!T->isDependentType() && "should not see dependent types here")

**Author:** porglezomp
**URL:** https://github.com/llvm/llvm-project/issues/159505
**Status:** Closed
**Labels:** clang:frontend, crash-on-valid, regression:21
**Closed Date:** 2025-09-20T01:34:11Z

## Body

Tested on latest main. Build with `clang++ -fptrauth-intrinsics example.cpp`:
```cpp
class A {
  virtual void f();
};

template <int N> class B {
  class C : A {
    A a[N];
  } d;
};
```

This crashes with:
```
Assertion failed: (!T->isDependentType() && "should not see dependent types here"), function getTypeInfoImpl, file TypeNodes.inc, line 39.
Stack dump:
0.	Program arguments: /Users/cassie/Development/oss/swift-project/llvm-project/build/bin/clang-22 -cc1 -triple arm64-apple-macosx15.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name fptrauth-intrinsics.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=1 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fbuiltin-headers-in-system-modules -fdefine-target-os-macros -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -fptrauth-intrinsics -debugger-tuning=lldb -fdebug-compilation-dir=/Users/cassie/Development/oss/swift-project/llvm-project -target-linker-version 1167.5 -fcoverage-compilation-dir=/Users/cassie/Development/oss/swift-project/llvm-project -resource-dir /Users/cassie/Development/oss/swift-project/llvm-project/build/lib/clang/22 -internal-isystem /usr/local/include -internal-isystem /Users/cassie/Development/oss/swift-project/llvm-project/build/lib/clang/22/include -internal-externc-isystem /usr/include -internal-iframework /System/Library/Frameworks -internal-iframework /System/Library/SubFrameworks -internal-iframework /Library/Frameworks -fdeprecated-macro -ferror-limit 19 -fmessage-length=198 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /var/folders/57/2mg3850x1cj1fb_9yhr0r3jh0000gn/T/fptrauth-intrinsics-3cf885.o -x c++ /tmp/fptrauth-intrinsics.cpp
1.	/tmp/fptrauth-intrinsics.cpp:8:6: current parser token ';'
2.	/tmp/fptrauth-intrinsics.cpp:5:18: parsing struct/union/class body 'B'
 #0 0x00000001052580cc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (clang-22+0x1010500cc)
 #1 0x000000010525623c llvm::sys::RunSignalHandlers() (clang-22+0x10104e23c)
 #2 0x0000000105258b2c SignalHandler(int, __siginfo*, void*) (clang-22+0x101050b2c)
 #3 0x000000018e9556a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #4 0x0000000106f6e054 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (clang-22+0x102d66054)
 #5 0x0000000106f6f8f0 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (clang-22+0x102d678f0)
 #6 0x00000001074b965c (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) (clang-22+0x1032b165c)
 #7 0x00000001074b7c98 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) (clang-22+0x1032afc98)
 #8 0x00000001074b1be0 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (clang-22+0x1032a9be0)
 #9 0x0000000106f6d0d4 clang::ASTContext::findPointerAuthContent(clang::QualType) const (clang-22+0x102d650d4)
#10 0x0000000107548850 clang::QualType::isCXX98PODType(clang::ASTContext const&) const (clang-22+0x103340850)
#11 0x00000001071488fc clang::CXXRecordDecl::addedMember(clang::Decl*) (clang-22+0x102f408fc)
#12 0x00000001071429ac clang::DeclContext::addHiddenDecl(clang::Decl*) (clang-22+0x102f3a9ac)
#13 0x0000000107142a24 clang::DeclContext::addDecl(clang::Decl*) (clang-22+0x102f3aa24)
#14 0x0000000106836dc8 clang::Sema::PushOnScopeChains(clang::NamedDecl*, clang::Scope*, bool) (clang-22+0x10262edc8)
#15 0x0000000106882e24 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (clang-22+0x10267ae24)
#16 0x00000001068fb398 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (clang-22+0x1026f3398)
#17 0x00000001065698ec clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (clang-22+0x1023618ec)
#18 0x000000010656b3c8 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (clang-22+0x1023633c8)
#19 0x0000000106566528 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (clang-22+0x10235e528)
#20 0x0000000106563f9c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (clang-22+0x10235bf9c)
#21 0x0000000106548f44 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (clang-22+0x102340f44)
#22 0x00000001065dcce0 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (clang-22+0x1023d4ce0)
#23 0x00000001065dc220 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (clang-22+0x1023d4220)
#24 0x00000001065dbc48 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (clang-22+0x1023d3c48)
#25 0x000000010654429c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (clang-22+0x10233c29c)
#26 0x00000001065ea85c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (clang-22+0x1023e285c)
#27 0x00000001065e95fc clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (clang-22+0x1023e15fc)
#28 0x000000010652faa8 clang::ParseAST(clang::Sema&, bool, bool) (clang-22+0x102327aa8)
#29 0x0000000105b908ec clang::FrontendAction::Execute() (clang-22+0x1019888ec)
#30 0x0000000105b2c508 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (clang-22+0x101924508)
#31 0x0000000105bfecb4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang-22+0x1019f6cb4)
#32 0x000000010420d9c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (clang-22+0x1000059c4)
#33 0x000000010420b918 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (clang-22+0x100003918)
#34 0x000000010420b0e8 clang_main(int, char**, llvm::ToolContext const&) (clang-22+0x1000030e8)
#35 0x00000001042164c4 main (clang-22+0x10000e4c4)
#36 0x000000018e57ab98
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (git@github.com:llvm/llvm-project.git 8548fa00f1d57c39119a24cfda4ef84232ead7e0)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/cassie/Development/oss/swift-project/llvm-project/build/bin
```

Crash reproducer files attached:
[fptrauth-intrinsics-dda544.sh](https://github.com/user-attachments/files/22399280/fptrauth-intrinsics-dda544.sh)
[fptrauth-intrinsics-dda544.cpp](https://github.com/user-attachments/files/22399279/fptrauth-intrinsics-dda544.cpp)
[clang-22-2025-09-17-220452.ips.zip](https://github.com/user-attachments/files/22399284/clang-22-2025-09-17-220452.ips.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Cassie Jones (porglezomp)

<details>
Build with `clang++ -fptrauth-intrinsics example.cpp`:
```cpp
class A {
  virtual void f();
};

template &lt;int N&gt; class B {
  class C : A {
    A a[N];
  } d;
};
```

This crashes with:
```
Stack dump:
0.	Program arguments: /Users/cassie/Development/oss/swift-project/llvm-project/build/bin/clang-22 -cc1 -triple arm64-apple-macosx15.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name fptrauth-intrinsics.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=1 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fbuiltin-headers-in-system-modules -fdefine-target-os-macros -enable-tlsdesc -target-cpu apple-m1 -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -fptrauth-intrinsics -debugger-tuning=lldb -fdebug-compilation-dir=/Users/cassie/Development/oss/swift-project/llvm-project -target-linker-version 1167.5 -fcoverage-compilation-dir=/Users/cassie/Development/oss/swift-project/llvm-project -resource-dir /Users/cassie/Development/oss/swift-project/llvm-project/build/lib/clang/22 -internal-isystem /usr/local/include -internal-isystem /Users/cassie/Development/oss/swift-project/llvm-project/build/lib/clang/22/include -internal-externc-isystem /usr/include -internal-iframework /System/Library/Frameworks -internal-iframework /System/Library/SubFrameworks -internal-iframework /Library/Frameworks -fdeprecated-macro -ferror-limit 19 -fmessage-length=198 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /var/folders/57/2mg3850x1cj1fb_9yhr0r3jh0000gn/T/fptrauth-intrinsics-3cf885.o -x c++ /tmp/fptrauth-intrinsics.cpp
1.	/tmp/fptrauth-intrinsics.cpp:8:6: current parser token ';'
2.	/tmp/fptrauth-intrinsics.cpp:5:18: parsing struct/union/class body 'B'
 #<!-- -->0 0x00000001052580cc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (clang-22+0x1010500cc)
 #<!-- -->1 0x000000010525623c llvm::sys::RunSignalHandlers() (clang-22+0x10104e23c)
 #<!-- -->2 0x0000000105258b2c SignalHandler(int, __siginfo*, void*) (clang-22+0x101050b2c)
 #<!-- -->3 0x000000018e9556a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->4 0x0000000106f6e054 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (clang-22+0x102d66054)
 #<!-- -->5 0x0000000106f6f8f0 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (clang-22+0x102d678f0)
 #<!-- -->6 0x00000001074b965c (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) (clang-22+0x1032b165c)
 #<!-- -->7 0x00000001074b7c98 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) (clang-22+0x1032afc98)
 #<!-- -->8 0x00000001074b1be0 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (clang-22+0x1032a9be0)
 #<!-- -->9 0x0000000106f6d0d4 clang::ASTContext::findPointerAuthContent(clang::QualType) const (clang-22+0x102d650d4)
#<!-- -->10 0x0000000107548850 clang::QualType::isCXX98PODType(clang::ASTContext const&amp;) const (clang-22+0x103340850)
#<!-- -->11 0x00000001071488fc clang::CXXRecordDecl::addedMember(clang::Decl*) (clang-22+0x102f408fc)
#<!-- -->12 0x00000001071429ac clang::DeclContext::addHiddenDecl(clang::Decl*) (clang-22+0x102f3a9ac)
#<!-- -->13 0x0000000107142a24 clang::DeclContext::addDecl(clang::Decl*) (clang-22+0x102f3aa24)
#<!-- -->14 0x0000000106836dc8 clang::Sema::PushOnScopeChains(clang::NamedDecl*, clang::Scope*, bool) (clang-22+0x10262edc8)
#<!-- -->15 0x0000000106882e24 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&amp;, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (clang-22+0x10267ae24)
#<!-- -->16 0x00000001068fb398 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (clang-22+0x1026f3398)
#<!-- -->17 0x00000001065698ec clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (clang-22+0x1023618ec)
#<!-- -->18 0x000000010656b3c8 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (clang-22+0x1023633c8)
#<!-- -->19 0x0000000106566528 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (clang-22+0x10235e528)
#<!-- -->20 0x0000000106563f9c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (clang-22+0x10235bf9c)
#<!-- -->21 0x0000000106548f44 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (clang-22+0x102340f44)
#<!-- -->22 0x00000001065dcce0 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (clang-22+0x1023d4ce0)
#<!-- -->23 0x00000001065dc220 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (clang-22+0x1023d4220)
#<!-- -->24 0x00000001065dbc48 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (clang-22+0x1023d3c48)
#<!-- -->25 0x000000010654429c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (clang-22+0x10233c29c)
#<!-- -->26 0x00000001065ea85c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (clang-22+0x1023e285c)
#<!-- -->27 0x00000001065e95fc clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (clang-22+0x1023e15fc)
#<!-- -->28 0x000000010652faa8 clang::ParseAST(clang::Sema&amp;, bool, bool) (clang-22+0x102327aa8)
#<!-- -->29 0x0000000105b908ec clang::FrontendAction::Execute() (clang-22+0x1019888ec)
#<!-- -->30 0x0000000105b2c508 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (clang-22+0x101924508)
#<!-- -->31 0x0000000105bfecb4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang-22+0x1019f6cb4)
#<!-- -->32 0x000000010420d9c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (clang-22+0x1000059c4)
#<!-- -->33 0x000000010420b918 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (clang-22+0x100003918)
#<!-- -->34 0x000000010420b0e8 clang_main(int, char**, llvm::ToolContext const&amp;) (clang-22+0x1000030e8)
#<!-- -->35 0x00000001042164c4 main (clang-22+0x10000e4c4)
#<!-- -->36 0x000000018e57ab98
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 8548fa00f1d57c39119a24cfda4ef84232ead7e0)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/cassie/Development/oss/swift-project/llvm-project/build/bin
```

Crash reproducer files attached:
[fptrauth-intrinsics-dda544.sh](https://github.com/user-attachments/files/22399280/fptrauth-intrinsics-dda544.sh)
[fptrauth-intrinsics-dda544.cpp](https://github.com/user-attachments/files/22399279/fptrauth-intrinsics-dda544.cpp)
[clang-22-2025-09-17-220452.ips.zip](https://github.com/user-attachments/files/22399284/clang-22-2025-09-17-220452.ips.zip)
</details>


---

### Comment 2 - porglezomp

Checking if this reproduces on release branches or if this is only on main…
Confirmed this is in 21.x, this reproduces in the `release/21.x` branch:
```
clang version 21.1.1 (git@github.com:llvm/llvm-project.git f4907049285ca0875cc91770e3ceb3f162ec7c48)
```

---

### Comment 3 - porglezomp

Does not reproduced in `release/20.x`, so this is a (recent?) regression in 21.

---

### Comment 4 - porglezomp

Bisected to:
```
commit 27f0e6e579b7bd47cf67523db7b0cc75fdab380a
Author: Oliver Hunt <oliver@apple.com>
Date:   Tue Aug 26 15:59:03 2025 -0700

    [clang][PAC] Fix builtins that claim address discriminated types are bitwise compatible (#154490)
    
    A number of builtins report some variation of "this type is compatibile
    with some bitwise equivalent operation", but this is not true for
    address discriminated values. We had address a number of cases, but not
    all of them. This PR corrects the remaining builtins.
    
    Fixes #154394
```
This is apparently caused by #154490 while fixing #154394.

---

### Comment 5 - zwuis

CC @ojhunt 

---

### Comment 6 - porglezomp

Some of the newly added `containsAddressDiscriminatedPointerAuth` calls end up trying to compute the layout of the dependent type which fails with the assertion.

---

