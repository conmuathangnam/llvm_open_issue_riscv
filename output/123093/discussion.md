# clang static analyzer crash with CTU Analysis enabled

**Author:** tomrittervg
**URL:** https://github.com/llvm/llvm-project/issues/123093

## Body

The following program (creduce-d from real Firefox source code, originally 133,189 lines long) and command produces a segfault in clang static analyzer. I'm more confident it is in the Analyzer, but not 100% sure, but it outputs "CTU loaded AST file" and disabling CTU makes the segfault go away.

> clang version 20.0.0git (https://github.com/llvm/llvm-project.git 9d5299eb61a64cd4df5fefa0299b0cf8d917978f)

```
clang \
  --analyze \
  -Xclang \
  -analyzer-checker=optin.taint.GenericTaint \
  -Xclang \
  -analyzer-config \
  -Xclang \
  experimental-enable-naive-ctu-analysis=true \
  -Xclang \
  -analyzer-config \
  -Xclang \
  ctu-dir=/segfault-2/repro \
  -Xclang \
  -analyzer-config \
  -Xclang \
  display-ctu-progress=true \
  -x c++ \
  VTuneWrapper.cpp
```

VTuneWrapper.cpp:
```
typedef char uint8_t;
namespace JS {
class a;
template <typename> class b;
typedef b<a> c;
enum RootKind {};
template <typename> class b {};
extern const c UndefinedHandleValue;
namespace {
class d {
  c e() {}
};
} // namespace
} // namespace JS
namespace js {
class Nursery {};
} // namespace js

```

It does require two AST files and an externalDef file that I don't know how to minimize.  I've hosted them here: https://ritter.vg/misc/transient/taint-2/


Here is the top of the stack:

```
(gdb) bt
#0  0x0000555556d1678c in llvm::detail::PunnedPointer<void*>::operator= (this=<error reading variable: Cannot access memory at address 0x7fffff7feff8>, 
    V=<error reading variable: Cannot access memory at address 0x7fffff7feff0>) at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:47
#1  0x0000555556d112a1 in llvm::detail::PunnedPointer<void*>::PunnedPointer (this=0x7fffff7ff100, i=0)
    at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37
#2  0x000055555bb0e8c1 in llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > >::PointerIntPair (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86
#3  0x000055555bb0e8e0 in llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > >, 2>::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:62
#4  0x000055555bb0e900 in llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > >, 1, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75
#5  0x000055555bb0e920 in llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > >, 0, clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75
#6  0x000055555bb0e940 in llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>::PointerUnion (this=0x7fffff7ff100) at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:135
#7  0x000055555bb25a6d in llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>::PointerUnion(decltype(nullptr)) (this=0x7fffff7ff100) at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:137
#8  0x000055555bb196c7 in llvm::ValueIsPresent<llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>, void>::isPresent (t=...) at /llvm-project/llvm/include/llvm/Support/Casting.h:622
#9  0x000055555bb0eb9b in llvm::detail::isPresent<llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > (t=...) at /llvm-project/llvm/include/llvm/Support/Casting.h:630
#10 0x000055555bafdc37 in llvm::dyn_cast_if_present<clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*, llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> > (Val=...)
    at /llvm-project/llvm/include/llvm/Support/Casting.h:739
#11 0x000055555bafdc78 in llvm::PointerUnion<clang::Decl*, clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*>::dyn_cast<clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::LazyData*> (this=0x7fffff7ff1e0)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:163
#12 0x000055555baf2abd in clang::LazyGenerationalUpdatePtr<clang::Decl const*, clang::Decl*, &clang::ExternalASTSource::CompleteRedeclChain>::get (this=0x7fffff7ff1e0, O=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/ExternalASTSource.h:481
#13 0x000055555ce5e17c in clang::Redeclarable<clang::RedeclarableTemplateDecl>::DeclLink::getPrevious (this=0x555576aae088, D=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:133
#14 0x000055555ce58dff in clang::Redeclarable<clang::RedeclarableTemplateDecl>::getNextRedeclaration (this=0x555576aae088)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:188
#15 0x000055555ce58e62 in clang::Redeclarable<clang::RedeclarableTemplateDecl>::getMostRecentDecl (this=0x555576aae088)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:226
#16 0x0000555560fcac3c in clang::RedeclarableTemplateDecl::getMostRecentDeclImpl (this=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/DeclTemplate.h:733
#17 0x000055555bea899f in clang::Decl::getMostRecentDecl (this=0x555576aae048) at /llvm-project/clang/include/clang/AST/DeclBase.h:1070
#18 0x0000555560f7e950 in clang::Decl::isUsed (this=0x555576aae048, CheckUsedAttr=false) at /llvm-project/clang/lib/AST/DeclBase.cpp:561
#19 0x0000555560d11dad in clang::updateFlags (From=0x555576aae048, To=0x555573cb0970) at /llvm-project/clang/lib/AST/ASTImporter.cpp:132
#20 0x0000555560d52dd1 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576aae048)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9482
#21 0x0000555560d61263 in clang::ASTNodeImporter::importInto<clang::FunctionTemplateDecl> (this=0x7fffff7ffd40, To=@0x7fffff7ff690: 0x0, From=0x555576aae048)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:190
#22 0x0000555560d11fd8 in clang::ASTNodeImporter::ImportFunctionTemplateWithTemplateArgsFromSpecialization (this=0x7fffff7ffd40, FromFD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:782
#23 0x0000555560d2996a in clang::ASTNodeImporter::FindFunctionTemplateSpecialization (this=0x7fffff7ffd40, FromFD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3498
#24 0x0000555560d2ad43 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff7ffd40, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3788
#25 0x0000555560d7b8f6 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffff7ffd40, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#26 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#27 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#28 0x0000555560d63107 in clang::ASTNodeImporter::import<clang::Decl> (this=0x7fffff800128, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#29 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff800128, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#30 0x0000555560d7c8d4 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff800128, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#31 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#32 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#33 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff800538, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#34 0x0000555560d12616 in clang::ASTNodeImporter::import<clang::TemplateArgument> (this=0x7fffff800538, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#35 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff800538, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#36 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff800538, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#37 0x0000555560d7c928 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff800538, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#38 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
--Type <RET> for more, q to quit, c to continue without paging--c
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#39 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#40 0x0000555560d65bcf in clang::ASTNodeImporter::import<clang::NestedNameSpecifier> (this=0x7fffff8007a8, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#41 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff8007a8, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#42 0x0000555560d7c688 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff8007a8, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#43 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#44 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#45 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff800a28, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#46 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff800a28, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#47 0x0000555560d7c5a8 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff800a28, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#48 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#49 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#50 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff800df8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#51 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff800df8, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#52 0x0000555560d7c6c0 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff800df8, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#53 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#54 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#55 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff8013f0, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#56 0x0000555560d641ec in clang::ASTNodeImporter::importChecked<clang::QualType> (this=0x7fffff8013f0, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#57 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff8013f0, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#58 0x0000555560d7b8f6 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffff8013f0, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#59 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#60 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#61 0x0000555560d63107 in clang::ASTNodeImporter::import<clang::Decl> (this=0x7fffff8017d8, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#62 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff8017d8, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#63 0x0000555560d7c8d4 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff8017d8, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#64 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#65 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#66 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff801be8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#67 0x0000555560d12616 in clang::ASTNodeImporter::import<clang::TemplateArgument> (this=0x7fffff801be8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#68 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff801be8, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#69 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff801be8, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#70 0x0000555560d7c928 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff801be8, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#71 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#72 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#73 0x0000555560d65bcf in clang::ASTNodeImporter::import<clang::NestedNameSpecifier> (this=0x7fffff801e58, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#74 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff801e58, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#75 0x0000555560d7c688 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff801e58, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#76 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#77 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#78 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff8020d8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#79 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff8020d8, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#80 0x0000555560d7c5a8 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff8020d8, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#81 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#82 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#83 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff8024a8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#84 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff8024a8, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#85 0x0000555560d7c6c0 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff8024a8, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#86 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#87 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#88 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff802aa0, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#89 0x0000555560d641ec in clang::ASTNodeImporter::importChecked<clang::QualType> (this=0x7fffff802aa0, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#90 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff802aa0, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#91 0x0000555560d7b8f6 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffff802aa0, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#92 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#93 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#94 0x0000555560d63107 in clang::ASTNodeImporter::import<clang::Decl> (this=0x7fffff802e88, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#95 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff802e88, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#96 0x0000555560d7c8d4 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff802e88, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#97 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#98 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#99 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff803298, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#100 0x0000555560d12616 in clang::ASTNodeImporter::import<clang::TemplateArgument> (this=0x7fffff803298, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#101 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff803298, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#102 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff803298, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#103 0x0000555560d7c928 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff803298, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#104 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#105 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#106 0x0000555560d65bcf in clang::ASTNodeImporter::import<clang::NestedNameSpecifier> (this=0x7fffff803508, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#107 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff803508, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#108 0x0000555560d7c688 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff803508, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#109 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#110 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#111 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff803788, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#112 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff803788, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#113 0x0000555560d7c5a8 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff803788, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#114 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#115 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#116 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff803b58, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#117 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff803b58, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#118 0x0000555560d7c6c0 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff803b58, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#119 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#120 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#121 0x0000555560d61d92 in clang::ASTNodeImporter::import<clang::QualType> (this=0x7fffff804150, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#122 0x0000555560d641ec in clang::ASTNodeImporter::importChecked<clang::QualType> (this=0x7fffff804150, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#123 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff804150, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#124 0x0000555560d7b8f6 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffff804150, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#125 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#126 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#127 0x0000555560d63107 in clang::ASTNodeImporter::import<clang::Decl> (this=0x7fffff804538, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#128 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff804538, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#129 0x0000555560d7c8d4 in clang::TypeVisitor<clang::ASTNodeImporter, llvm::Expected<clang::QualType> >::Visit (this=0x7fffff804538, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#130 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127

```

Here is the bottom:

```
#46735 0x0000555560d21325 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff18d0, FromDC=0x5555759cbcf8, ForceImport=false)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2178
#46736 0x0000555560d24ad7 in clang::ASTNodeImporter::VisitNamespaceDecl (this=0x7fffffff18d0, D=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2702
#46737 0x0000555560d7b7de in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffffff18d0, D=0x5555759cbcc8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46738 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#46739 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#46740 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x5555759cbcf8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#46741 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff1f90, FromD=0x555570ec19c0, ToDC=@0x7fffffff1e18: 0x550156d112a1, 
    ToLexicalDC=@0x7fffffff1e20: 0x555577940028) at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#46742 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff1f90, D=0x555570ec19c0, DC=@0x7fffffff1e18: 0x550156d112a1, LexicalDC=@0x7fffffff1e20: 0x555577940028, 
    Name=..., ToD=@0x7fffffff1e30: 0x0, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#46743 0x0000555560d24433 in clang::ASTNodeImporter::VisitNamespaceDecl (this=0x7fffffff1f90, D=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2631
#46744 0x0000555560d7b7de in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffffff1f90, D=0x555570ec19c0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46745 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#46746 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#46747 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x555570ec19f0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#46748 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff2770, FromD=0x555570ec1910, ToDC=@0x7fffffff24f0: 0x0, ToLexicalDC=@0x7fffffff24f8: 0x0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#46749 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff2770, D=0x555570ec1910, DC=@0x7fffffff24f0: 0x0, LexicalDC=@0x7fffffff24f8: 0x0, Name=..., 
    ToD=@0x7fffffff2508: 0x0, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#46750 0x0000555560d271ac in clang::ASTNodeImporter::VisitRecordDecl (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3092
#46751 0x0000555560d9bf44 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::VisitCXXRecordDecl (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#46752 0x0000555560d7bbb2 in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#46753 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#46754 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#46755 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x555570ec1950)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#46756 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff2ee0, FromD=0x5555748c0e80, ToDC=@0x7fffffff2cc8: 0x7fffffff2e08, 
    ToLexicalDC=@0x7fffffff2cd0: 0x7fffffff2cf0) at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#46757 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff2ee0, D=0x5555748c0e80, DC=@0x7fffffff2cc8: 0x7fffffff2e08, LexicalDC=@0x7fffffff2cd0: 0x7fffffff2cf0, 
    Name=..., ToD=@0x7fffffff2ce0: 0x7fffffff2d20, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#46758 0x0000555560d2fb0b in clang::ASTNodeImporter::VisitVarDecl (this=0x7fffffff2ee0, D=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:4520
#46759 0x0000555560d7b99e in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffffff2ee0, D=0x5555748c0e80)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46760 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#46761 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#46762 0x0000555560d63107 in clang::ASTNodeImporter::import<clang::Decl> (this=0x7fffffff3420, From=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#46763 0x0000555560d2f9bc in clang::ASTNodeImporter::VisitVarDecl (this=0x7fffffff3420, D=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:4509
#46764 0x0000555560d7b99e in clang::declvisitor::Base<std::add_pointer, clang::ASTNodeImporter, llvm::Expected<clang::Decl*> >::Visit (this=0x7fffffff3420, D=0x5555748c0e10)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46765 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#46766 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#46767 0x000055555f741170 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/include/clang/AST/ASTImporter.h:352
#46768 0x000055555f744864 in clang::cross_tu::CrossTranslationUnitContext::importDefinitionImpl<clang::VarDecl> (this=0x55556d5d1c80, D=0x5555748c0e10, Unit=0x555573ab69a0)
    at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:736
#46769 0x000055555f73ff6f in clang::cross_tu::CrossTranslationUnitContext::importDefinition (this=0x55556d5d1c80, VD=0x5555748c0e10, Unit=0x555573ab69a0)
    at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:772
#46770 0x000055555f742dff in clang::cross_tu::CrossTranslationUnitContext::getCrossTUDefinitionImpl<clang::VarDecl> (this=0x55556d5d1c80, D=0x555574a83dc0, CrossTUDir=..., IndexName=..., 
    DisplayCTUProgress=true) at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:348
#46771 0x000055555f73da7f in clang::cross_tu::CrossTranslationUnitContext::getCrossTUDefinition (this=0x55556d5d1c80, VD=0x555574a83dc0, CrossTUDir=..., IndexName=..., 
    DisplayCTUProgress=true) at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:367
#46772 0x000055555f0d1b11 in (anonymous namespace)::AnalysisConsumer::VisitVarDecl (this=0x55556d5d1bf0, VD=0x555574a83dc0)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:297
#46773 0x0000555560fd28b4 in (anonymous namespace)::Impl::VisitVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46774 0x0000555561007e2a in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::WalkUpFromVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46775 0x0000555561007c3f in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2332
#46776 0x0000555560fe1249 in clang::DynamicRecursiveASTVisitor::TraverseVarDecl (this=0x55556d5d1c00, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46777 0x0000555560fd14cc in (anonymous namespace)::Impl::TraverseVarDecl (this=0x7fffffff3b10, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46778 0x0000555560ffb4f8 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDecl (this=0x7fffffff3b10, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#46779 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x555574a83dc0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#46780 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3c40, D=0x555574a83dc0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#46781 0x000055556105eb95 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDeclContextHelper (this=0x7fffffff3c40, DC=0x5555749809f8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#46782 0x000055556100aec1 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseCXXRecordDecl (this=0x7fffffff3c40, D=0x5555749809b8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2059
#46783 0x0000555560fe2089 in clang::DynamicRecursiveASTVisitor::TraverseCXXRecordDecl (this=0x55556d5d1c00, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#46784 0x0000555560fd18f4 in (anonymous namespace)::Impl::TraverseCXXRecordDecl (this=0x7fffffff3ce0, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#46785 0x0000555560ffb8c1 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDecl (this=0x7fffffff3ce0, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#46786 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x5555749809b8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#46787 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3e10, D=0x5555749809b8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#46788 0x000055556105eb95 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDeclContextHelper (this=0x7fffffff3e10, DC=0x555572c99810)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#46789 0x0000555561005886 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseNamespaceDecl (this=0x7fffffff3e10, D=0x555572c997e0)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1679
#46790 0x0000555560fe0649 in clang::DynamicRecursiveASTVisitor::TraverseNamespaceDecl (this=0x55556d5d1c00, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46791 0x0000555560fd114c in (anonymous namespace)::Impl::TraverseNamespaceDecl (this=0x7fffffff3eb0, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46792 0x0000555560ffb1c8 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDecl (this=0x7fffffff3eb0, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46793 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x555572c997e0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#46794 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3fe0, D=0x555572c997e0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#46795 0x000055556105eb95 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDeclContextHelper (this=0x7fffffff3fe0, DC=0x55556d6a8118)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#46796 0x0000555561005886 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseNamespaceDecl (this=0x7fffffff3fe0, D=0x55556d6a80e8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1679
#46797 0x0000555560fe0649 in clang::DynamicRecursiveASTVisitor::TraverseNamespaceDecl (this=0x55556d5d1c00, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46798 0x0000555560fd114c in (anonymous namespace)::Impl::TraverseNamespaceDecl (this=0x7fffffff4080, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46799 0x0000555560ffb1c8 in clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDecl (this=0x7fffffff4080, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#46800 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x55556d6a80e8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#46801 0x000055555f0d2948 in (anonymous namespace)::AnalysisConsumer::runAnalysisOnTranslationUnit (this=0x55556d5d1bf0, C=...)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:554
#46802 0x000055555f0d2d17 in (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit (this=0x55556d5d1bf0, C=...)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:613
#46803 0x000055555f7dec90 in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=false)
    at /llvm-project/clang/lib/Parse/ParseAST.cpp:184
#46804 0x000055555c9f6070 in clang::ASTFrontendAction::ExecuteAction (this=0x55556d5d6950)
    at /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1191
#46805 0x000055555c9f5952 in clang::FrontendAction::Execute (this=0x55556d5d6950) at /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1077
#46806 0x000055555c909259 in clang::CompilerInstance::ExecuteAction (this=0x55556d5cf6a0, Act=...)
    at /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1062
#46807 0x000055555cbc2a4e in clang::ExecuteCompilerInvocation (Clang=0x55556d5cf6a0)
    at /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296
#46808 0x0000555556c7cab8 in cc1_main (Argv=..., Argv0=0x55556d5cba10 "
/llvm-project/mybuild/bin/clang-20", 
    MainAddr=0x555556c6c66a <GetExecutablePath[abi:cxx11](char const*, bool)>) at /llvm-project/clang/tools/driver/cc1_main.cpp:286
#46809 0x0000555556c6d26e in ExecuteCC1Tool (ArgV=..., ToolContext=...) at /llvm-project/clang/tools/driver/driver.cpp:218
#46810 0x0000555556c6d492 in operator() (__closure=0x7fffffff8b78, ArgV=...) at /llvm-project/clang/tools/driver/driver.cpp:360
#46811 0x0000555556c6eb99 in llvm::function_ref<int(llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, const llvm::ToolContext&)::<lambda(llvm::SmallVectorImpl<char const*>&)> >(intptr_t, llvm::SmallVectorImpl<char const*> &) (callable=140737488325496, params#0=...)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
#46812 0x000055555c7362f1 in llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const (this=0x7fffffff9168, params#0=...)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
#46813 0x000055555c734e84 in operator() (__closure=0x7fffffff8430) at /llvm-project/clang/lib/Driver/Job.cpp:437
#46814 0x000055555c735338 in llvm::function_ref<void()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> >(intptr_t) (callable=140737488323632) at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
#46815 0x000055555a07c8ce in llvm::function_ref<void ()>::operator()() const (this=0x7fffffff8360)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
#46816 0x000055555b6083e3 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (this=0x7fffffff8410, Fn=...)
    at /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426
#46817 0x000055555c7350a1 in clang::driver::CC1Command::Execute (this=0x55556d5cfa50, Redirects=..., ErrMsg=0x7fffffff88f0, ExecutionFailed=0x7fffffff88d0)
    at /llvm-project/clang/lib/Driver/Job.cpp:437
#46818 0x000055555c6cae21 in clang::driver::Compilation::ExecuteCommand (this=0x55556d5cdf10, C=..., FailingCommand=@0x7fffffff89d0: 0x0, LogOnly=false)
    at /llvm-project/clang/lib/Driver/Compilation.cpp:196
#46819 0x000055555c6cb1c4 in clang::driver::Compilation::ExecuteJobs (this=0x55556d5cdf10, Jobs=..., FailingCommands=..., LogOnly=false)
    at /llvm-project/clang/lib/Driver/Compilation.cpp:251
#46820 0x000055555c6e025c in clang::driver::Driver::ExecuteCompilation (this=0x7fffffff8de0, C=..., FailingCommands=...)
    at /llvm-project/clang/lib/Driver/Driver.cpp:2121
#46821 0x0000555556c6e6c1 in clang_main (Argc=23, Argv=0x7fffffffd888, ToolContext=...) at /llvm-project/clang/tools/driver/driver.cpp:398
#46822 0x0000555556ca951a in main (argc=23, argv=0x7fffffffd888) at /llvm-project/mybuild/tools/clang/tools/driver/clang-driver.cpp:17

```

cc @steakhal 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tom Ritter (tomrittervg)

<details>
The following program (creduce-d from real Firefox source code, originally 133,189 lines long) and command produces a segfault in clang static analyzer. I'm more confident it is in the Analyzer, but not 100% sure, but it outputs "CTU loaded AST file" and disabling CTU makes the segfault go away.

&gt; clang version 20.0.0git (https://github.com/llvm/llvm-project.git 9d5299eb61a64cd4df5fefa0299b0cf8d917978f)

```
clang \
  --analyze \
  -Xclang \
  -analyzer-checker=optin.taint.GenericTaint \
  -Xclang \
  -analyzer-config \
  -Xclang \
  experimental-enable-naive-ctu-analysis=true \
  -Xclang \
  -analyzer-config \
  -Xclang \
  ctu-dir=/segfault-2/repro \
  -Xclang \
  -analyzer-config \
  -Xclang \
  display-ctu-progress=true \
  -x c++ \
  VTuneWrapper.cpp
```

VTuneWrapper.cpp:
```
typedef char uint8_t;
namespace JS {
class a;
template &lt;typename&gt; class b;
typedef b&lt;a&gt; c;
enum RootKind {};
template &lt;typename&gt; class b {};
extern const c UndefinedHandleValue;
namespace {
class d {
  c e() {}
};
} // namespace
} // namespace JS
namespace js {
class Nursery {};
} // namespace js

```

It does require two AST files and an externalDef file that I don't know how to minimize.  I've hosted them here: https://ritter.vg/misc/transient/taint-2/


Here is the top of the stack:

```
(gdb) bt
#<!-- -->0  0x0000555556d1678c in llvm::detail::PunnedPointer&lt;void*&gt;::operator= (this=&lt;error reading variable: Cannot access memory at address 0x7fffff7feff8&gt;, 
    V=&lt;error reading variable: Cannot access memory at address 0x7fffff7feff0&gt;) at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:47
#<!-- -->1  0x0000555556d112a1 in llvm::detail::PunnedPointer&lt;void*&gt;::PunnedPointer (this=0x7fffff7ff100, i=0)
    at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37
#<!-- -->2  0x000055555bb0e8c1 in llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; &gt;::PointerIntPair (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86
#<!-- -->3  0x000055555bb0e8e0 in llvm::pointer_union_detail::PointerUnionMembers&lt;llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; &gt;, 2&gt;::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:62
#<!-- -->4  0x000055555bb0e900 in llvm::pointer_union_detail::PointerUnionMembers&lt;llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; &gt;, 1, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75
#<!-- -->5  0x000055555bb0e920 in llvm::pointer_union_detail::PointerUnionMembers&lt;llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; &gt;, 0, clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;::PointerUnionMembers (this=0x7fffff7ff100)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75
#<!-- -->6  0x000055555bb0e940 in llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;::PointerUnion (this=0x7fffff7ff100) at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:135
#<!-- -->7  0x000055555bb25a6d in llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;::PointerUnion(decltype(nullptr)) (this=0x7fffff7ff100) at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:137
#<!-- -->8  0x000055555bb196c7 in llvm::ValueIsPresent&lt;llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;, void&gt;::isPresent (t=...) at /llvm-project/llvm/include/llvm/Support/Casting.h:622
#<!-- -->9  0x000055555bb0eb9b in llvm::detail::isPresent&lt;llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; (t=...) at /llvm-project/llvm/include/llvm/Support/Casting.h:630
#<!-- -->10 0x000055555bafdc37 in llvm::dyn_cast_if_present&lt;clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*, llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; &gt; (Val=...)
    at /llvm-project/llvm/include/llvm/Support/Casting.h:739
#<!-- -->11 0x000055555bafdc78 in llvm::PointerUnion&lt;clang::Decl*, clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt;::dyn_cast&lt;clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::LazyData*&gt; (this=0x7fffff7ff1e0)
    at /llvm-project/llvm/include/llvm/ADT/PointerUnion.h:163
#<!-- -->12 0x000055555baf2abd in clang::LazyGenerationalUpdatePtr&lt;clang::Decl const*, clang::Decl*, &amp;clang::ExternalASTSource::CompleteRedeclChain&gt;::get (this=0x7fffff7ff1e0, O=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/ExternalASTSource.h:481
#<!-- -->13 0x000055555ce5e17c in clang::Redeclarable&lt;clang::RedeclarableTemplateDecl&gt;::DeclLink::getPrevious (this=0x555576aae088, D=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:133
#<!-- -->14 0x000055555ce58dff in clang::Redeclarable&lt;clang::RedeclarableTemplateDecl&gt;::getNextRedeclaration (this=0x555576aae088)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:188
#<!-- -->15 0x000055555ce58e62 in clang::Redeclarable&lt;clang::RedeclarableTemplateDecl&gt;::getMostRecentDecl (this=0x555576aae088)
    at /llvm-project/clang/include/clang/AST/Redeclarable.h:226
#<!-- -->16 0x0000555560fcac3c in clang::RedeclarableTemplateDecl::getMostRecentDeclImpl (this=0x555576aae048)
    at /llvm-project/clang/include/clang/AST/DeclTemplate.h:733
#<!-- -->17 0x000055555bea899f in clang::Decl::getMostRecentDecl (this=0x555576aae048) at /llvm-project/clang/include/clang/AST/DeclBase.h:1070
#<!-- -->18 0x0000555560f7e950 in clang::Decl::isUsed (this=0x555576aae048, CheckUsedAttr=false) at /llvm-project/clang/lib/AST/DeclBase.cpp:561
#<!-- -->19 0x0000555560d11dad in clang::updateFlags (From=0x555576aae048, To=0x555573cb0970) at /llvm-project/clang/lib/AST/ASTImporter.cpp:132
#<!-- -->20 0x0000555560d52dd1 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576aae048)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9482
#<!-- -->21 0x0000555560d61263 in clang::ASTNodeImporter::importInto&lt;clang::FunctionTemplateDecl&gt; (this=0x7fffff7ffd40, To=@<!-- -->0x7fffff7ff690: 0x0, From=0x555576aae048)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:190
#<!-- -->22 0x0000555560d11fd8 in clang::ASTNodeImporter::ImportFunctionTemplateWithTemplateArgsFromSpecialization (this=0x7fffff7ffd40, FromFD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:782
#<!-- -->23 0x0000555560d2996a in clang::ASTNodeImporter::FindFunctionTemplateSpecialization (this=0x7fffff7ffd40, FromFD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3498
#<!-- -->24 0x0000555560d2ad43 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff7ffd40, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3788
#<!-- -->25 0x0000555560d7b8f6 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffff7ffd40, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#<!-- -->26 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->27 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->28 0x0000555560d63107 in clang::ASTNodeImporter::import&lt;clang::Decl&gt; (this=0x7fffff800128, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->29 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff800128, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#<!-- -->30 0x0000555560d7c8d4 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff800128, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#<!-- -->31 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->32 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->33 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff800538, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->34 0x0000555560d12616 in clang::ASTNodeImporter::import&lt;clang::TemplateArgument&gt; (this=0x7fffff800538, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#<!-- -->35 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff800538, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#<!-- -->36 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff800538, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#<!-- -->37 0x0000555560d7c928 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff800538, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#<!-- -->38 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
--Type &lt;RET&gt; for more, q to quit, c to continue without paging--c
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->39 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#<!-- -->40 0x0000555560d65bcf in clang::ASTNodeImporter::import&lt;clang::NestedNameSpecifier&gt; (this=0x7fffff8007a8, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->41 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff8007a8, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#<!-- -->42 0x0000555560d7c688 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff8007a8, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#<!-- -->43 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->44 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->45 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff800a28, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->46 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff800a28, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#<!-- -->47 0x0000555560d7c5a8 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff800a28, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#<!-- -->48 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->49 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->50 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff800df8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->51 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff800df8, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#<!-- -->52 0x0000555560d7c6c0 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff800df8, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#<!-- -->53 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->54 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->55 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff8013f0, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->56 0x0000555560d641ec in clang::ASTNodeImporter::importChecked&lt;clang::QualType&gt; (this=0x7fffff8013f0, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#<!-- -->57 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff8013f0, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#<!-- -->58 0x0000555560d7b8f6 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffff8013f0, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#<!-- -->59 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->60 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->61 0x0000555560d63107 in clang::ASTNodeImporter::import&lt;clang::Decl&gt; (this=0x7fffff8017d8, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->62 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff8017d8, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#<!-- -->63 0x0000555560d7c8d4 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff8017d8, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#<!-- -->64 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->65 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->66 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff801be8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->67 0x0000555560d12616 in clang::ASTNodeImporter::import&lt;clang::TemplateArgument&gt; (this=0x7fffff801be8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#<!-- -->68 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff801be8, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#<!-- -->69 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff801be8, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#<!-- -->70 0x0000555560d7c928 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff801be8, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#<!-- -->71 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->72 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#<!-- -->73 0x0000555560d65bcf in clang::ASTNodeImporter::import&lt;clang::NestedNameSpecifier&gt; (this=0x7fffff801e58, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->74 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff801e58, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#<!-- -->75 0x0000555560d7c688 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff801e58, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#<!-- -->76 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->77 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->78 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff8020d8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->79 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff8020d8, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#<!-- -->80 0x0000555560d7c5a8 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff8020d8, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#<!-- -->81 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->82 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->83 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff8024a8, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->84 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff8024a8, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#<!-- -->85 0x0000555560d7c6c0 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff8024a8, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#<!-- -->86 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->87 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->88 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff802aa0, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->89 0x0000555560d641ec in clang::ASTNodeImporter::importChecked&lt;clang::QualType&gt; (this=0x7fffff802aa0, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#<!-- -->90 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff802aa0, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#<!-- -->91 0x0000555560d7b8f6 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffff802aa0, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#<!-- -->92 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->93 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->94 0x0000555560d63107 in clang::ASTNodeImporter::import&lt;clang::Decl&gt; (this=0x7fffff802e88, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->95 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff802e88, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#<!-- -->96 0x0000555560d7c8d4 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff802e88, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#<!-- -->97 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->98 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->99 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff803298, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->100 0x0000555560d12616 in clang::ASTNodeImporter::import&lt;clang::TemplateArgument&gt; (this=0x7fffff803298, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:831
#<!-- -->101 0x0000555560d2345d in clang::ASTNodeImporter::ImportTemplateArguments (this=0x7fffff803298, FromArgs=..., ToArgs=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2452
#<!-- -->102 0x0000555560d1de52 in clang::ASTNodeImporter::VisitTemplateSpecializationType (this=0x7fffff803298, T=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1655
#<!-- -->103 0x0000555560d7c928 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff803298, T=0x555576ba9d70)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:77
#<!-- -->104 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9d70)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->105 0x0000555560d546f4 in clang::ASTImporter::Import (this=0x5555778fb970, FromNNS=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9758
#<!-- -->106 0x0000555560d65bcf in clang::ASTNodeImporter::import&lt;clang::NestedNameSpecifier&gt; (this=0x7fffff803508, From=0x555576ba9db8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->107 0x0000555560d1e0e6 in clang::ASTNodeImporter::VisitElaboratedType (this=0x7fffff803508, T=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1674
#<!-- -->108 0x0000555560d7c688 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff803508, T=0x555576ba9ef0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:49
#<!-- -->109 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9ef0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->110 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->111 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff803788, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->112 0x0000555560d1cb86 in clang::ASTNodeImporter::VisitAutoType (this=0x7fffff803788, T=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1511
#<!-- -->113 0x0000555560d7c5a8 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff803788, T=0x555576ba9f20)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:41
#<!-- -->114 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f20)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->115 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->116 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff803b58, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->117 0x0000555560d1b84e in clang::ASTNodeImporter::VisitFunctionProtoType (this=0x7fffff803b58, T=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1361
#<!-- -->118 0x0000555560d7c6c0 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff803b58, T=0x555576ba9f90)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:52
#<!-- -->119 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576ba9f90)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127
#<!-- -->120 0x0000555560d5164d in clang::ASTImporter::Import (this=0x5555778fb970, FromT=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:9141
#<!-- -->121 0x0000555560d61d92 in clang::ASTNodeImporter::import&lt;clang::QualType&gt; (this=0x7fffff804150, From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:216
#<!-- -->122 0x0000555560d641ec in clang::ASTNodeImporter::importChecked&lt;clang::QualType&gt; (this=0x7fffff804150, Err=..., From=...)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:691
#<!-- -->123 0x0000555560d2b775 in clang::ASTNodeImporter::VisitFunctionDecl (this=0x7fffff804150, D=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3917
#<!-- -->124 0x0000555560d7b8f6 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffff804150, D=0x555576c68ae8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:238
#<!-- -->125 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->126 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->127 0x0000555560d63107 in clang::ASTNodeImporter::import&lt;clang::Decl&gt; (this=0x7fffff804538, From=0x555576c68ae8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->128 0x0000555560d1d9e4 in clang::ASTNodeImporter::VisitSubstTemplateTypeParmType (this=0x7fffff804538, T=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:1620
#<!-- -->129 0x0000555560d7c8d4 in clang::TypeVisitor&lt;clang::ASTNodeImporter, llvm::Expected&lt;clang::QualType&gt; &gt;::Visit (this=0x7fffff804538, T=0x555576c68d40)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/TypeNodes.inc:73
#<!-- -->130 0x0000555560d514db in clang::ASTImporter::Import (this=0x5555778fb970, FromT=0x555576c68d40)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9127

```

Here is the bottom:

```
#<!-- -->46735 0x0000555560d21325 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff18d0, FromDC=0x5555759cbcf8, ForceImport=false)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2178
#<!-- -->46736 0x0000555560d24ad7 in clang::ASTNodeImporter::VisitNamespaceDecl (this=0x7fffffff18d0, D=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2702
#<!-- -->46737 0x0000555560d7b7de in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffffff18d0, D=0x5555759cbcc8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46738 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->46739 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555759cbcc8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->46740 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x5555759cbcf8)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#<!-- -->46741 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff1f90, FromD=0x555570ec19c0, ToDC=@<!-- -->0x7fffffff1e18: 0x550156d112a1, 
    ToLexicalDC=@<!-- -->0x7fffffff1e20: 0x555577940028) at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#<!-- -->46742 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff1f90, D=0x555570ec19c0, DC=@<!-- -->0x7fffffff1e18: 0x550156d112a1, LexicalDC=@<!-- -->0x7fffffff1e20: 0x555577940028, 
    Name=..., ToD=@<!-- -->0x7fffffff1e30: 0x0, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#<!-- -->46743 0x0000555560d24433 in clang::ASTNodeImporter::VisitNamespaceDecl (this=0x7fffffff1f90, D=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2631
#<!-- -->46744 0x0000555560d7b7de in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffffff1f90, D=0x555570ec19c0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46745 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->46746 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555570ec19c0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->46747 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x555570ec19f0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#<!-- -->46748 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff2770, FromD=0x555570ec1910, ToDC=@<!-- -->0x7fffffff24f0: 0x0, ToLexicalDC=@<!-- -->0x7fffffff24f8: 0x0)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#<!-- -->46749 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff2770, D=0x555570ec1910, DC=@<!-- -->0x7fffffff24f0: 0x0, LexicalDC=@<!-- -->0x7fffffff24f8: 0x0, Name=..., 
    ToD=@<!-- -->0x7fffffff2508: 0x0, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#<!-- -->46750 0x0000555560d271ac in clang::ASTNodeImporter::VisitRecordDecl (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:3092
#<!-- -->46751 0x0000555560d9bf44 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::VisitCXXRecordDecl (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#<!-- -->46752 0x0000555560d7bbb2 in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffffff2770, D=0x555570ec1910)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#<!-- -->46753 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->46754 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x555570ec1910)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->46755 0x0000555560d539d7 in clang::ASTImporter::ImportContext (this=0x5555778fb970, FromDC=0x555570ec1950)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9618
#<!-- -->46756 0x0000555560d21629 in clang::ASTNodeImporter::ImportDeclContext (this=0x7fffffff2ee0, FromD=0x5555748c0e80, ToDC=@<!-- -->0x7fffffff2cc8: 0x7fffffff2e08, 
    ToLexicalDC=@<!-- -->0x7fffffff2cd0: 0x7fffffff2cf0) at /llvm-project/clang/lib/AST/ASTImporter.cpp:2222
#<!-- -->46757 0x0000555560d20286 in clang::ASTNodeImporter::ImportDeclParts (this=0x7fffffff2ee0, D=0x5555748c0e80, DC=@<!-- -->0x7fffffff2cc8: 0x7fffffff2e08, LexicalDC=@<!-- -->0x7fffffff2cd0: 0x7fffffff2cf0, 
    Name=..., ToD=@<!-- -->0x7fffffff2ce0: 0x7fffffff2d20, Loc=...) at /llvm-project/clang/lib/AST/ASTImporter.cpp:1962
#<!-- -->46758 0x0000555560d2fb0b in clang::ASTNodeImporter::VisitVarDecl (this=0x7fffffff2ee0, D=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:4520
#<!-- -->46759 0x0000555560d7b99e in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffffff2ee0, D=0x5555748c0e80)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46760 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->46761 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->46762 0x0000555560d63107 in clang::ASTNodeImporter::import&lt;clang::Decl&gt; (this=0x7fffffff3420, From=0x5555748c0e80)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:202
#<!-- -->46763 0x0000555560d2f9bc in clang::ASTNodeImporter::VisitVarDecl (this=0x7fffffff3420, D=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:4509
#<!-- -->46764 0x0000555560d7b99e in clang::declvisitor::Base&lt;std::add_pointer, clang::ASTNodeImporter, llvm::Expected&lt;clang::Decl*&gt; &gt;::Visit (this=0x7fffffff3420, D=0x5555748c0e10)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46765 0x0000555560d51257 in clang::ASTImporter::ImportImpl (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9096
#<!-- -->46766 0x0000555560d52e97 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/lib/AST/ASTImporter.cpp:9491
#<!-- -->46767 0x000055555f741170 in clang::ASTImporter::Import (this=0x5555778fb970, FromD=0x5555748c0e10)
    at /llvm-project/clang/include/clang/AST/ASTImporter.h:352
#<!-- -->46768 0x000055555f744864 in clang::cross_tu::CrossTranslationUnitContext::importDefinitionImpl&lt;clang::VarDecl&gt; (this=0x55556d5d1c80, D=0x5555748c0e10, Unit=0x555573ab69a0)
    at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:736
#<!-- -->46769 0x000055555f73ff6f in clang::cross_tu::CrossTranslationUnitContext::importDefinition (this=0x55556d5d1c80, VD=0x5555748c0e10, Unit=0x555573ab69a0)
    at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:772
#<!-- -->46770 0x000055555f742dff in clang::cross_tu::CrossTranslationUnitContext::getCrossTUDefinitionImpl&lt;clang::VarDecl&gt; (this=0x55556d5d1c80, D=0x555574a83dc0, CrossTUDir=..., IndexName=..., 
    DisplayCTUProgress=true) at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:348
#<!-- -->46771 0x000055555f73da7f in clang::cross_tu::CrossTranslationUnitContext::getCrossTUDefinition (this=0x55556d5d1c80, VD=0x555574a83dc0, CrossTUDir=..., IndexName=..., 
    DisplayCTUProgress=true) at /llvm-project/clang/lib/CrossTU/CrossTranslationUnit.cpp:367
#<!-- -->46772 0x000055555f0d1b11 in (anonymous namespace)::AnalysisConsumer::VisitVarDecl (this=0x55556d5d1bf0, VD=0x555574a83dc0)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:297
#<!-- -->46773 0x0000555560fd28b4 in (anonymous namespace)::Impl::VisitVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46774 0x0000555561007e2a in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::WalkUpFromVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46775 0x0000555561007c3f in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseVarDecl (this=0x7fffffff3a70, D=0x555574a83dc0)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2332
#<!-- -->46776 0x0000555560fe1249 in clang::DynamicRecursiveASTVisitor::TraverseVarDecl (this=0x55556d5d1c00, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46777 0x0000555560fd14cc in (anonymous namespace)::Impl::TraverseVarDecl (this=0x7fffffff3b10, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46778 0x0000555560ffb4f8 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDecl (this=0x7fffffff3b10, D=0x555574a83dc0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:278
#<!-- -->46779 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x555574a83dc0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#<!-- -->46780 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3c40, D=0x555574a83dc0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#<!-- -->46781 0x000055556105eb95 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDeclContextHelper (this=0x7fffffff3c40, DC=0x5555749809f8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#<!-- -->46782 0x000055556100aec1 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseCXXRecordDecl (this=0x7fffffff3c40, D=0x5555749809b8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2059
#<!-- -->46783 0x0000555560fe2089 in clang::DynamicRecursiveASTVisitor::TraverseCXXRecordDecl (this=0x55556d5d1c00, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#<!-- -->46784 0x0000555560fd18f4 in (anonymous namespace)::Impl::TraverseCXXRecordDecl (this=0x7fffffff3ce0, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#<!-- -->46785 0x0000555560ffb8c1 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDecl (this=0x7fffffff3ce0, D=0x5555749809b8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:414
#<!-- -->46786 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x5555749809b8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#<!-- -->46787 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3e10, D=0x5555749809b8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#<!-- -->46788 0x000055556105eb95 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDeclContextHelper (this=0x7fffffff3e10, DC=0x555572c99810)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#<!-- -->46789 0x0000555561005886 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseNamespaceDecl (this=0x7fffffff3e10, D=0x555572c997e0)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1679
#<!-- -->46790 0x0000555560fe0649 in clang::DynamicRecursiveASTVisitor::TraverseNamespaceDecl (this=0x55556d5d1c00, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46791 0x0000555560fd114c in (anonymous namespace)::Impl::TraverseNamespaceDecl (this=0x7fffffff3eb0, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46792 0x0000555560ffb1c8 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDecl (this=0x7fffffff3eb0, D=0x555572c997e0)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46793 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x555572c997e0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#<!-- -->46794 0x0000555560fd0544 in (anonymous namespace)::Impl::TraverseDecl (this=0x7fffffff3fe0, D=0x555572c997e0)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:117
#<!-- -->46795 0x000055556105eb95 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDeclContextHelper (this=0x7fffffff3fe0, DC=0x55556d6a8118)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1543
#<!-- -->46796 0x0000555561005886 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseNamespaceDecl (this=0x7fffffff3fe0, D=0x55556d6a80e8)
    at /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1679
#<!-- -->46797 0x0000555560fe0649 in clang::DynamicRecursiveASTVisitor::TraverseNamespaceDecl (this=0x55556d5d1c00, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46798 0x0000555560fd114c in (anonymous namespace)::Impl::TraverseNamespaceDecl (this=0x7fffffff4080, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46799 0x0000555560ffb1c8 in clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&gt;::TraverseDecl (this=0x7fffffff4080, D=0x55556d6a80e8)
    at /llvm-project/mybuild/tools/clang/include/clang/AST/DeclNodes.inc:170
#<!-- -->46800 0x0000555560fded0b in clang::DynamicRecursiveASTVisitor::TraverseDecl (this=0x55556d5d1c00, D=0x55556d6a80e8)
    at /llvm-project/clang/lib/AST/DynamicRecursiveASTVisitor.cpp:286
#<!-- -->46801 0x000055555f0d2948 in (anonymous namespace)::AnalysisConsumer::runAnalysisOnTranslationUnit (this=0x55556d5d1bf0, C=...)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:554
#<!-- -->46802 0x000055555f0d2d17 in (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit (this=0x55556d5d1bf0, C=...)
    at /llvm-project/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:613
#<!-- -->46803 0x000055555f7dec90 in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=false)
    at /llvm-project/clang/lib/Parse/ParseAST.cpp:184
#<!-- -->46804 0x000055555c9f6070 in clang::ASTFrontendAction::ExecuteAction (this=0x55556d5d6950)
    at /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1191
#<!-- -->46805 0x000055555c9f5952 in clang::FrontendAction::Execute (this=0x55556d5d6950) at /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1077
#<!-- -->46806 0x000055555c909259 in clang::CompilerInstance::ExecuteAction (this=0x55556d5cf6a0, Act=...)
    at /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1062
#<!-- -->46807 0x000055555cbc2a4e in clang::ExecuteCompilerInvocation (Clang=0x55556d5cf6a0)
    at /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296
#<!-- -->46808 0x0000555556c7cab8 in cc1_main (Argv=..., Argv0=0x55556d5cba10 "
/llvm-project/mybuild/bin/clang-20", 
    MainAddr=0x555556c6c66a &lt;GetExecutablePath[abi:cxx11](char const*, bool)&gt;) at /llvm-project/clang/tools/driver/cc1_main.cpp:286
#<!-- -->46809 0x0000555556c6d26e in ExecuteCC1Tool (ArgV=..., ToolContext=...) at /llvm-project/clang/tools/driver/driver.cpp:218
#<!-- -->46810 0x0000555556c6d492 in operator() (__closure=0x7fffffff8b78, ArgV=...) at /llvm-project/clang/tools/driver/driver.cpp:360
#<!-- -->46811 0x0000555556c6eb99 in llvm::function_ref&lt;int(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, const llvm::ToolContext&amp;)::&lt;lambda(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt; &gt;(intptr_t, llvm::SmallVectorImpl&lt;char const*&gt; &amp;) (callable=140737488325496, params#<!-- -->0=...)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
#<!-- -->46812 0x000055555c7362f1 in llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const (this=0x7fffffff9168, params#<!-- -->0=...)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
#<!-- -->46813 0x000055555c734e84 in operator() (__closure=0x7fffffff8430) at /llvm-project/clang/lib/Driver/Job.cpp:437
#<!-- -->46814 0x000055555c735338 in llvm::function_ref&lt;void()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt;(intptr_t) (callable=140737488323632) at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
#<!-- -->46815 0x000055555a07c8ce in llvm::function_ref&lt;void ()&gt;::operator()() const (this=0x7fffffff8360)
    at /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
#<!-- -->46816 0x000055555b6083e3 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (this=0x7fffffff8410, Fn=...)
    at /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426
#<!-- -->46817 0x000055555c7350a1 in clang::driver::CC1Command::Execute (this=0x55556d5cfa50, Redirects=..., ErrMsg=0x7fffffff88f0, ExecutionFailed=0x7fffffff88d0)
    at /llvm-project/clang/lib/Driver/Job.cpp:437
#<!-- -->46818 0x000055555c6cae21 in clang::driver::Compilation::ExecuteCommand (this=0x55556d5cdf10, C=..., FailingCommand=@<!-- -->0x7fffffff89d0: 0x0, LogOnly=false)
    at /llvm-project/clang/lib/Driver/Compilation.cpp:196
#<!-- -->46819 0x000055555c6cb1c4 in clang::driver::Compilation::ExecuteJobs (this=0x55556d5cdf10, Jobs=..., FailingCommands=..., LogOnly=false)
    at /llvm-project/clang/lib/Driver/Compilation.cpp:251
#<!-- -->46820 0x000055555c6e025c in clang::driver::Driver::ExecuteCompilation (this=0x7fffffff8de0, C=..., FailingCommands=...)
    at /llvm-project/clang/lib/Driver/Driver.cpp:2121
#<!-- -->46821 0x0000555556c6e6c1 in clang_main (Argc=23, Argv=0x7fffffffd888, ToolContext=...) at /llvm-project/clang/tools/driver/driver.cpp:398
#<!-- -->46822 0x0000555556ca951a in main (argc=23, argv=0x7fffffffd888) at /llvm-project/mybuild/tools/clang/tools/driver/clang-driver.cpp:17

```

cc @<!-- -->steakhal 
</details>


---

### Comment 2 - steakhal

Yea, this one looks like an infinite recursion in astimporter.
Its not fault of CSA, we just trip on this bug of the astimporter.
That said, I understand it impacts the analysis quality.

Unfortunately, i dont know of a general solution to containt all these effectively for once and all.
Maybe we could have some detection mechanism for identical recursion steps, thus fail earlier with the import instead of just running out of stack and crash.

That containment wouldnt help with the import, which would then just fail instead. I acknowledge that would be many magnitudes better than crashing.

---

### Comment 3 - steakhal

I came back to this, and realized that you are sharing the raw ast files.
Those are not really portable, thus can't be used for minimzation by other peers.

I'd suggest sharing the input files, aka. `VTuneWrapper.cpp` and `Value.cpp`.

The usually way of reducing CTU crashes is by first reducing the primary TU, then the secondary TU.
Why this way? Because you only need to dump the secondary TU as an ast file once, and reuse that while reducing the primary TU. So you avoid dumping it again and again for each round of interestingness test.
Once the primary TU is minimized, you need to adjust the interestingness test by always reusing the same already minimized primary TU, and dump the secondary TU as an ast file, call extdefmapping on it to get the mapping file, and only then you can run the actual analysis starting from the primary TU.
This way you minimize the secondary TU. This is an extremely time consuming process.

Usually I try to add a static counter to some of the import entry points to pin down the root cause of the infinite recursion.
This is to avoid reducing the case into a different infinite recursion, because that wouldn't be really useful for you for the original issue. Here is a concrete example patch for that:
```diff
diff --git a/clang/lib/AST/ASTImporter.cpp b/clang/lib/AST/ASTImporter.cpp
@@ -3753,6 +3753,12 @@ ASTNodeImporter::importExplicitSpecifier(Error &Err, ExplicitSpecifier ESpec) {
 }
 
 ExpectedDecl ASTNodeImporter::VisitFunctionDecl(FunctionDecl *D) {
+  //llvm::errs() << "ASTNodeImporter::VisitFunctionDecl  " << D->getQualifiedNameAsString() << "\n";
+  static int counter = 0;
+  if (D->getQualifiedNameAsString() == StringRef("mozilla::UnderlyingValue")) {
+    ++counter;
+    assert(counter < 3 && "UnderlyingValue is imported multiple times");
+  }
 
   SmallVector<Decl *, 2> Redecls = getCanonicalForwardRedeclChain(D);
   auto RedeclIt = Redecls.begin();
```
There I made sure that the same decl is requested for import (`mozilla::UnderlyingValue` in this case) for a couple of times (let's say 3). You can see the original `llvm::errs()` dumps above the code that helped me to infer that we infinitely try to import the `mozilla::UnderlyingValue` function. Once you do this, you can hop into a debugger to look around and actually print stuff, because unlike in the original case, now you have stack space to print stuff.

I'm not an expert on the ASTImporter though, so they may have a more sophisticated process for debugging these.
/cc @balazske 

Unfortunately, with the input files, nobody can help you with this case.

---

### Comment 4 - tomrittervg

Thanks, that's really helpful and I will work on it.

---

### Comment 5 - balazske

My usual way to debug this type of problem if the stack dump is available:
Check the `VisitXXXDecl` (or `VisitXXXType`, `VisitXXXStmt`) lines in the crash dump, there should be a repetitive sequence of these (but it may be a long one). Then check the lines (at the stack frames) in the source code and see why the infinite loop can happen. Usually this type of problem happens when there are import statements before a create (`GetImportedOrCreateDecl`) in the (declaration) visit functions at two different declarations that call each other.

---

### Comment 6 - steakhal

I think I was reducing separately the exact same inf import.
Here is a self-contained test.
This demonstrates that the import of `remote()` triggers an import chain, that will eventually trigger an infinite recursion, importing again and again the `mozilla::UnderlyingValue()`.

From what I've seen in the debugger, the import of the FunctionDecl `mozilla::UnderlyingValue()` triggers the import of its type specifier info. That thing is a `FunctionProtoType` of type `typename std::underlying_type<enum RootKind>::type`.
The import of that type triggers the import of the AutoType, which imports the deduced type. That is an ElaboratedType `typename std::underlying_type<enum RootKind>::type`. This in turn imports the qualifier part first, aka. `std::underlying_type<enum RootKind>::`.
That type is a TemplateSpecialization type, which will import its template arguments, so the `enum RootKind` type will be requested next.
That somehow triggers the import of a SubstTemplateTypeParmType, which first includes the "associated" decl - which happens to be the same as the FunctionDecl we begin with, infinitely recursing.

Test, that fails on current main (382d3599c203573388b82717dc17e3db4039916a):
```
cat clang/test/Analysis/ctu-crash.cpp
// RUN: rm -rf %t && mkdir %t
// RUN: split-file %s %t

// RUN: %clang_cc1 -emit-pch -o %t/remote.ast %t/remote.cpp

// RUN: %clang_cc1 -analyze \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-config experimental-enable-naive-ctu-analysis=true \
// RUN:   -analyzer-config display-ctu-progress=true \
// RUN:   -analyzer-config ctu-dir=%t \
// RUN:   -verify %t/entrypoint.cpp

//--- entrypoint.cpp
namespace mozilla {
void remote();
} // namespace mozilla

void entrypoint() {
  mozilla::remote();
}

//--- remote.cpp
template <typename _Tp> struct underlying_type {
  using type = __underlying_type(_Tp);
};

namespace mozilla {
template <typename Enum> auto UnderlyingValue(Enum v) {
  return static_cast<typename underlying_type<Enum>::type>(v);
}

enum class RootKind : char { Nothing };

void remote() {
  UnderlyingValue(RootKind::Nothing);
}
} // namespace mozilla

//--- externalDefMap.txt
22:c:@N@mozilla@F@remote# remote.ast
```

---

### Comment 7 - tomrittervg

I just finished minimizing things from my side as well, so hopefully another test case is helpful:

Value-full.cpp
```
typedef int a;
typedef unsigned uint8_t;
typedef int b;
namespace aa {
enum c { ab };
template <typename, c> class d;
} // namespace aa
class e;
class f;
namespace JS {
class g;
template <typename> class h;
template <typename> class ac;
typedef h<g> ad;
typedef ac<e> i;
} // namespace JS
namespace js {
namespace gc {
class af;
using j = aa::d<b, aa::ab>;
class l {
  j k(af);
};
} // namespace gc
template <typename, typename> class ag {};
} // namespace js
namespace JS {
template <typename> class h {};
template <typename> class ac : js::ag<a, a> {};
} // namespace JS
namespace {
namespace {
enum ap : uint8_t;
}
} // namespace
namespace js::gc {
class AllocSite {
  static f *const WasmScript;
  ap aq;
};
} // namespace js::gc
namespace js {
namespace gc {
class af {
  b ar(AllocSite);
};
} // namespace gc
} // namespace js
namespace JS {
extern const ad UndefinedHandleValue;
class a {
  i av;
};
const ad UndefinedHandleValue;
} // namespace JS
```

Pretenuring-full.cpp
```
namespace a {
template <typename b, b c> struct d {
  static constexpr b e = c;
};
typedef d<bool, false> f;
template <bool, typename> struct g;
template <typename b> struct h {
  typedef b i;
};
template <bool j, typename b = void> using k = g<j, b>;
template <typename> bool l;
} // namespace a
typedef unsigned m;
typedef int n;
namespace ab {
template <typename p> constexpr auto o(p r) {
  a::l<p>;
  return static_cast<a::h<p>::i>(r);
}
template <typename p> struct q {
  static constexpr p e = p();
};
template <typename ac, typename, m> class ad {
  static_assert(o(q<ac>::e) == 0);
};
} // namespace ab
class s;
namespace js::gc {
class AllocSite {
  static n t;
  static s *const WasmScript;
};
} // namespace js::gc
namespace ab {
template <typename, m, class> class w;
}
namespace js {
namespace ae {
template <typename> struct u : a::f {};
} // namespace ae
template <typename p, m x, class v = a::k<ae::u<p>::e>>
using w = ab::w<p, x, v>;
namespace {
enum ag { ah };
}
class ai {};
} // namespace js
class s : js::ai {};
namespace js {
namespace {
struct aj;
using ak = w<aj, 0>;
struct a : ab::ad<ag, ak, ah> {};
} // namespace
} // namespace js
using namespace js::gc;
s *const AllocSite::WasmScript = reinterpret_cast<s *>(t);
```

VtuneWrapper.cpp
```
typedef char uint8_t;
namespace JS {
class a;
template <typename> class b;
typedef b<a> c;
enum RootKind {};
template <typename> class b {};
extern const c UndefinedHandleValue;
namespace {
class d {
  c e() {}
};
} // namespace
} // namespace JS
namespace js {
class Nursery {};
} // namespace js
```


Command
```
#!/usr/bin/env bash

# The Clang command that triggers a potential segfault:
CC=/usr/bin

rm -f externalDefMap.txt
touch externalDefMap.txt

$CC/clang-20 \
-c \
-x \
c++ \
Pretenuring-full.cpp \
-emit-ast \
-D__clang_analyzer__ \
-w \
-o Pretenuring-full.cpp.ast


$CC/clang-extdef-mapping Pretenuring-full.cpp -- -c -x c++ 2>/dev/null >> externalDefMap.txt


$CC/clang-20 \
-c \
-x \
c++ \
Value-full.cpp \
-emit-ast \
-D__clang_analyzer__ \
-w  \
-o \
Value-full.cpp.ast

$CC/clang-extdef-mapping Value-full.cpp -- -c -x c++ 2>/dev/null >> externalDefMap.txt

sed -i -e 's/\.cpp$/\.cpp\.ast/g' externalDefMap.txt
sed -i -e "s|$(pwd)/||g" externalDefMap.txt


$CC/clang-20 \
--analyze \
-Xclang \
-analyzer-config \
-Xclang \
optin.taint.TaintPropagation:Config=/media/tom/b032d6f1-a2e4-4c59-8c52-4cb0c18f69ce/ctu-analysis-38/myconfig.yaml \
-Xclang \
-analyzer-checker=optin.taint.GenericTaint \
-Xclang \
-analyzer-config \
-Xclang \
experimental-enable-naive-ctu-analysis=true \
-Xclang \
-analyzer-config \
-Xclang \
ctu-dir=. \
-Xclang \
-analyzer-config \
-Xclang \
display-ctu-progress=true \
-x \
c++ \
VTuneWrapper.cpp 
```

---

### Comment 8 - steakhal

```c++
template <typename Enum> auto UnderlyingValue(Enum v) {
  return static_cast<typename underlying_type<Enum>::type>(v);
}
```
What if the function prototype type refers to a function that has an auto return type that depends on the type of a template parameter?
Right now, if I'm not mistaken, the import of the function prototype of `UnderlyingValue()` would need to resolve the `auto` return type,
which would eventually trigger the import of `underlying_type<Enum>`, and that one has the `Enum` type template parameter.
That will have the type `SubstTemplateTypeParmType`, denoting the template subtitution of `Enum=enum mozilla::RootKind`.
This `SubstTemplateTypeParmType` would have an "associated decl" referring to `UnderlyingValue()`.

@balazske My question is, how do we usually break this circular dependency between an auto return type and a type template parameter? I suppose, we should follow the same tactic here.

---

### Comment 9 - steakhal

I think I found two other places where we had a similar issue:
 - https://reviews.llvm.org/D112013
 - https://github.com/llvm/llvm-project/commit/25234fd69e32e51bc800686075265f535d69d990

---

### Comment 10 - balazske

#69724 looks related too.
I think a similar solution should be possible here, add something to `hasAutoReturnTypeDeclaredInside` or `IsTypeDeclaredInsideVisitor`.

---

### Comment 11 - steakhal

> [#69724](https://github.com/llvm/llvm-project/pull/69724) looks related too. I think a similar solution should be possible here, add something to `hasAutoReturnTypeDeclaredInside` or `IsTypeDeclaredInsideVisitor`.

I was thinking about this for a while, but I couldn't find a solution.
My problem is that `VisitSubstNonTypeTemplateParmExpr` wants to import the `AssociatedDecl` (which is the templ specialization of the function we were in progress importing (`auto UnderlyingValue(RootKind)`), and I can't delay the import of the `AssociatedDecl` because I need to create one `SubstNonTypeTemplateParmExpr` that needs a non-null `AssociatedDecl`. So, I'm not sure how to continue here.

---

### Comment 12 - steakhal

Let me reiterate the events:
1) We request the import of `remote()`, that has a CallExpr that has an ImplicitCastExpr with type referring to FunctionProtoType of `auto UnderlyingValue(RootKind)`.
2) This requests the import of the FunctionProtoType, that will request the import of the AutoType (the return type of the FnProtoType).
3) The import of AutoType imports the DeducedType of it, which in turns imports the ElaboratedType of it.
4) The ElaboratedType has a `getQualifier()`, which will be requested for import next. This is basically the `typename std::underlying_type<RootKind>::`.
5) That ElaboratedType refers to a template specialization: `std::underlying_type of RootKind`, so when importing it, it will also import the `SubstNonTypeTemplateParmExpr` representing the `Enum=RootKind` type template argument.
6) The twist is that to create a `SubstNonTypeTemplateParmExpr`, it also needs the `AssociatedDecl`, which is the decl to which this template substitution attaches, which is the decl of `UnderlyingValue(RootKind)`.

I can't see right now where to break this circle.
The only think I can think of is to somehow "skip" the problematic Stmts when importing the body of the function.
That would "avoid" getting into this import chain.
So, in this case it would mean that I'd avoid importing a Stmt if that Stmt has a type that depends on the current function for which I'm importing the Stmts. WDYT? @balazske 

---

### Comment 13 - steakhal

I came up with a patch like this, that avoids the crash, but it doesn't fix the import of course:
```diff
diff --git a/clang/lib/AST/ASTImporter.cpp b/clang/lib/AST/ASTImporter.cpp
index c9f2f905d213..8451f2063e2b 100644
--- a/clang/lib/AST/ASTImporter.cpp
+++ b/clang/lib/AST/ASTImporter.cpp
@@ -3586,7 +3586,7 @@ namespace {
 class IsTypeDeclaredInsideVisitor
     : public TypeVisitor<IsTypeDeclaredInsideVisitor, std::optional<bool>> {
 public:
-  IsTypeDeclaredInsideVisitor(const FunctionDecl *ParentDC)
+  IsTypeDeclaredInsideVisitor(const DeclContext *ParentDC)
       : ParentDC(ParentDC) {}
 
   bool CheckType(QualType T) {
@@ -3636,6 +3636,13 @@ public:
     return {};
   }
 
+  std::optional<bool> VisitFunctionProtoType(const FunctionProtoType *T) {
+    for (QualType Param : T->param_types())
+      if (CheckType(Param))
+        return true;
+    return CheckType(T->getReturnType());
+  }
+
   std::optional<bool>
   VisitTemplateSpecializationType(const TemplateSpecializationType *T) {
     for (const auto &Arg : T->template_arguments())
@@ -7960,6 +7967,18 @@ ASTNodeImporter::ImportCastPath(CastExpr *CE) {
 }
 
 ExpectedStmt ASTNodeImporter::VisitImplicitCastExpr(ImplicitCastExpr *E) {
+  if (const auto *DRE = dyn_cast<DeclRefExpr>(E->getSubExpr())) {
+    if (const auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
+      const DeclContext *DC = FD->isTemplateInstantiation()
+                                  ? FD->getPrimaryTemplate()->getDeclContext()
+                                  : FD;
+      IsTypeDeclaredInsideVisitor Visitor(DC);
+      if (Visitor.CheckType(E->getType())) {
+        return make_error<ASTImportError>(ASTImportError::UnsupportedConstruct);
+      }
+    }
+  }
+
   ExpectedType ToTypeOrErr = import(E->getType());
   if (!ToTypeOrErr)
     return ToTypeOrErr.takeError();
```

---

### Comment 14 - balazske

This is the return type of the function `UnderlyingValue`:
```
AutoType 0x5555557b5990 'typename underlying_type<enum RootKind>::type' sugar imported
`-ElaboratedType 0x5555557b5960 'typename underlying_type<enum RootKind>::type' sugar imported
  `-TypedefType 0x5555557b5930 'underlying_type<enum mozilla::RootKind>::type' sugar imported
    |-TypeAlias 0x5555557b5838 'type'
    `-UnaryTransformType 0x5555557b58d0 '__underlying_type(enum mozilla::RootKind)' sugar imported underlying_type
      |-SubstTemplateTypeParmType 0x5555557b58a0 'enum mozilla::RootKind' sugar imported typename depth 0 index 0 _Tp
      | |-ClassTemplateSpecialization 0x5555557b5658 'underlying_type'
      | `-EnumType 0x5555557a2c70 'enum mozilla::RootKind' imported
      |   `-Enum 0x5555557a2ba8 'RootKind'
      `-BuiltinType 0x55555576d6e0 'char'
```
In this case the "associated declaration" in the `SubstTemplateTypeParmType` is the `ClassTemplateSpecialization 0x5555557b5658 'underlying_type'`. Here `hasReturnTypeDeclaredInside` does not return true.

The type `typename underlying_type<enum RootKind>::type` appears with two versions in the AST:
 * `'typename underlying_type<RootKind>::type (mozilla::RootKind)'`
 * `'typename underlying_type<RootKind>::type':'char'`

The second one is the problematic because there the associated declaration is function `UnderlyingType`. It appears in the function body at return statement, but not in the prototype, so it can not be detected in the current way. (The real problem may be that this type appears also at the `CallExpr` where `UnderlyingType` is called.)

```
FunctionDecl 0x5555557b53f8 <remote.cpp:6:26, line:8:1> line:6:31 imported used UnderlyingValue 'typename underlying_type<RootKind>::type (mozilla::RootKind)' implicit_instantiation
|-TemplateArgument type 'mozilla::RootKind'
| `-EnumType 0x5555557a2c70 'mozilla::RootKind' imported
|   `-Enum 0x5555557a2ba8 'RootKind'
|-ParmVarDecl 0x5555557b55a0 <col:47, col:52> col:52 imported used v 'mozilla::RootKind'
`-CompoundStmt 0x5555557b5c40 <col:55, line:8:1>
  `-ReturnStmt 0x5555557b5c30 <line:7:3, col:61>
    `-CXXStaticCastExpr 0x5555557b5ba0 <col:10, col:61> 'typename underlying_type<RootKind>::type':'char' static_cast<typename underlying_type<enum RootKind>::type> <IntegralCast>
      `-ImplicitCastExpr 0x5555557b5b88 <col:60> 'mozilla::RootKind' <LValueToRValue> part_of_explicit_cast
        `-DeclRefExpr 0x5555557b5b68 <col:60> 'mozilla::RootKind' lvalue ParmVar 0x5555557b55a0 'v' 'mozilla::RootKind'
```

---

### Comment 15 - balazske

My previous comment is not exactly true. I found out that there is a problem in `IsTypeDeclaredInsideVisitor::VisitTypedefType` and that for `ElaboratedType` the qualifier part should be checked (in `IsTypeDeclaredInsideVisitor`) if it is a type. This qualifier part has the reference to the function `UnderlyingValue`. With these additions I could fix the crash.

---

### Comment 16 - balazske

These are the changes to fix the crash:
```diff
diff --git a/clang/lib/AST/ASTImporter.cpp b/clang/lib/AST/ASTImporter.cpp
index c27ebbf838ad..db6bc3ebbbcf 100644
--- a/clang/lib/AST/ASTImporter.cpp
+++ b/clang/lib/AST/ASTImporter.cpp
@@ -3614,6 +3614,14 @@ public:
     return isAncestorDeclContextOf(ParentDC, T->getDecl());
   }
 
+  std::optional<bool> VisitElaboratedType(const ElaboratedType *T) {
+    if (NestedNameSpecifier *NNS = T->getQualifier())
+      if (const Type *T = NNS->getAsType())
+        if (CheckType(QualType(T, 0)))
+          return true;
+    return {};
+  }
+
   std::optional<bool> VisitPointerType(const PointerType *T) {
     return CheckType(T->getPointeeType());
   }
@@ -3625,7 +3633,9 @@ public:
   std::optional<bool> VisitTypedefType(const TypedefType *T) {
     const TypedefNameDecl *TD = T->getDecl();
     assert(TD);
-    return isAncestorDeclContextOf(ParentDC, TD);
+    if (isAncestorDeclContextOf(ParentDC, TD))
+      return true;
+    return {};
   }
 
   std::optional<bool> VisitUsingType(const UsingType *T) {
```

---

### Comment 17 - steakhal

I'll have a look, many thanks! <3 @balazske 

---

### Comment 18 - steakhal

>> These are the changes to fix the crash:
>> [[diff]]

> I'll have a look, many thanks!

So, I looked at the case again, and I'm not completely satisfied. It's better to not crash, but it would also stop the import of `remote`, which isn't ideal.

I was thinking of a solution, and I come back again and again to the idea of delaying imports of subexpressions of the function body.
To me the problem is that sometimes some of the nodes of a function body may depend on the function itself.
So, if we could recognize those, and put them into a "delay" list, and finish their imports once the function itself finished should theoretically break the circular dependency, right?

The next problem of course that imports of functions may be nested, so these "delay lists" must be also handled separately, but I don't think that should be too hard to handle. WDYT? @balazske 

---

### Comment 19 - balazske

This change (my fix) should not stop the import. It only delays import of the function prototype type until the body was imported. If `remote` is not imported, it can be because other (related or not) import error happens.

About the new idea, it looks difficult to first find all the statement nodes that should be delayed (a loop over all statements is needed and a recursive visit of each statement and sub-expression). The data is difficult to pass to import functions of expressions (which is necessary if sub-expressions are to be delayed). What looks almost impossible is to import and set the delayed data where it is needed at the final import phase. Additionally there may be other unexpected dependencies between `Stmt` nodes (and probably types which depend on them) which can make a selective import more difficult. `ASTImporter` has generally the problem with interdependency of nodes that is now solved with special cases and "hacks", a better way is only to have more than one import passes (would be a re-design of the whole `ASTImporter`).

---

