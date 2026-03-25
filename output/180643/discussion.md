# [clangd] Segfault in clang::DeclarationNameTable::getCXXConstructorName(clang::CanQual<clang::Type>)

**Author:** k4lizen
**URL:** https://github.com/llvm/llvm-project/issues/180643
**Status:** Open
**Labels:** clangd, crash

## Body

I was looking at a build of the linux kernel. Here is a backtrace:
```
pwndbg> bt
#0  0x00007fb3db2a9ba5 in clang::DeclarationNameTable::getCXXConstructorName(clang::CanQual<clang::Type>) () from /usr/lib/libclang-cpp.so.21.1
#1  0x00007fb3dbfcca5f in clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) () from /usr/lib/libclang-cpp.so.21.1
#2  0x00007fb3dbdc2307 in ?? () from /usr/lib/libclang-cpp.so.21.1
#3  0x00007fb3dbdc3b9f in ?? () from /usr/lib/libclang-cpp.so.21.1
#4  0x00007fb3dbdc3fae in ?? () from /usr/lib/libclang-cpp.so.21.1
#5  0x00007fb3dbdfcc35 in clang::Sema::DeclareImplicitMoveConstructor(clang::CXXRecordDecl*) () from /usr/lib/libclang-cpp.so.21.1
#6  0x00007fb3dbdcca31 in clang::Sema::AddImplicitlyDeclaredMembersToClass(clang::CXXRecordDecl*) () from /usr/lib/libclang-cpp.so.21.1
#7  0x00007fb3dbd7c7ba in clang::Sema::ActOnFields(clang::Scope*, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::Decl*>, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&) ()
   from /usr/lib/libclang-cpp.so.21.1
#8  0x00007fb3dbdd9590 in clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&) ()
   from /usr/lib/libclang-cpp.so.21.1
#9  0x00007fb3dafdfdff in clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) () from /usr/lib/libclang-cpp.so.21.1
#10 0x00007fb3dafe292f in clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) () from /usr/lib/libclang-cpp.so.21.1
#11 0x00007fb3daf7e76e in clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) () from /usr/lib/libclang-cpp.so.21.1
#12 0x00007fb3db065570 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) () from /usr/lib/libclang-cpp.so.21.1
#13 0x00007fb3db065f3d in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) () from /usr/lib/libclang-cpp.so.21.1
#14 0x00007fb3db079268 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) () from /usr/lib/libclang-cpp.so.21.1
#15 0x00007fb3db07a2bb in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) () from /usr/lib/libclang-cpp.so.21.1
#16 0x00007fb3daf69e0b in clang::ParseAST(clang::Sema&, bool, bool) () from /usr/lib/libclang-cpp.so.21.1
#17 0x00007fb3dcfb5aa8 in clang::FrontendAction::Execute() () from /usr/lib/libclang-cpp.so.21.1
#18 0x0000563157c254e5 in ?? ()
#19 0x0000563157cafeb1 in ?? ()
#20 0x0000563157ca077e in ?? ()
#21 0x0000563157caa982 in ?? ()
#22 0x0000563157cac58d in ?? ()
#23 0x0000563157e4ca9e in ?? ()
#24 0x00007fb3d0aa55c5 in start_thread (arg=<optimized out>) at pthread_create.c:448
#25 0x00007fb3d0b264cc in __GI___clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78
```

Here is the coredump along with the clangd and libclangd-cpp.so.21.1 binaries: https://files.catbox.moe/p0qxc3.zip . Password for the zipfile is `meow123456`.

Version:
```
~❯ clangd --version
clangd version 21.1.6
Features: linux
Platform: x86_64-pc-linux-gnu
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (k4lizen)

<details>
I was looking at a build of the linux kernel. Here is a backtrace:
```
pwndbg&gt; bt
#<!-- -->0  0x00007fb3db2a9ba5 in clang::DeclarationNameTable::getCXXConstructorName(clang::CanQual&lt;clang::Type&gt;) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->1  0x00007fb3dbfcca5f in clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->2  0x00007fb3dbdc2307 in ?? () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->3  0x00007fb3dbdc3b9f in ?? () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->4  0x00007fb3dbdc3fae in ?? () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->5  0x00007fb3dbdfcc35 in clang::Sema::DeclareImplicitMoveConstructor(clang::CXXRecordDecl*) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->6  0x00007fb3dbdcca31 in clang::Sema::AddImplicitlyDeclaredMembersToClass(clang::CXXRecordDecl*) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->7  0x00007fb3dbd7c7ba in clang::Sema::ActOnFields(clang::Scope*, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::Decl*&gt;, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&amp;) ()
   from /usr/lib/libclang-cpp.so.21.1
#<!-- -->8  0x00007fb3dbdd9590 in clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&amp;) ()
   from /usr/lib/libclang-cpp.so.21.1
#<!-- -->9  0x00007fb3dafdfdff in clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->10 0x00007fb3dafe292f in clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->11 0x00007fb3daf7e76e in clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->12 0x00007fb3db065570 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->13 0x00007fb3db065f3d in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->14 0x00007fb3db079268 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->15 0x00007fb3db07a2bb in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->16 0x00007fb3daf69e0b in clang::ParseAST(clang::Sema&amp;, bool, bool) () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->17 0x00007fb3dcfb5aa8 in clang::FrontendAction::Execute() () from /usr/lib/libclang-cpp.so.21.1
#<!-- -->18 0x0000563157c254e5 in ?? ()
#<!-- -->19 0x0000563157cafeb1 in ?? ()
#<!-- -->20 0x0000563157ca077e in ?? ()
#<!-- -->21 0x0000563157caa982 in ?? ()
#<!-- -->22 0x0000563157cac58d in ?? ()
#<!-- -->23 0x0000563157e4ca9e in ?? ()
#<!-- -->24 0x00007fb3d0aa55c5 in start_thread (arg=&lt;optimized out&gt;) at pthread_create.c:448
#<!-- -->25 0x00007fb3d0b264cc in __GI___clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78
```

Here is the coredump along with the clangd and libclangd-cpp.so.21.1 binaries: https://files.catbox.moe/p0qxc3.zip . Password for the zipfile is `meow123456`.

Version:
```
~❯ clangd --version
clangd version 21.1.6
Features: linux
Platform: x86_64-pc-linux-gnu
```
</details>


---

### Comment 3 - k4lizen

@EugeneZelenko not really since I don't have a reproducer.

---

