# [Clang 18] ICE on generic lambda

**Author:** mhx
**URL:** https://github.com/llvm/llvm-project/issues/165440
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash, needs-reduction, lambda
**Closed Date:** 2025-10-28T17:49:47Z

## Body

```
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -O2 -g -std=c++20 -fPIC -Wall -Wextra -pedantic -Wold-style-cast -Wnon-virtual-dtor -Wsuggest-override -Wpessimizing-move -Wfloat-equal -Wcast-align -Wpointer-arith -Wformat=2 -Wnull-dereference -fcolor-diagnostics -DBOOST_ATOMIC_DYN_LINK -DBOOST_ATOMIC_NO_LIB -DBOOST_CHRONO_DYN_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_PROGRAM_OPTIONS_NO_LIB -DDWARFS_HAVE_CLOSE_RANGE=1 -DFMT_HEADER_ONLY -DFOLLY_CFG_NO_COROUTINES -DGFLAGS_IS_A_DLL=0 -DGLOG_NO_ABBREVIATED_SEVERITIES -DGLOG_USE_GLOG_EXPORT -DNOGDI -DNOMINMAX -I/workspace/include -I/home/mhx/build/ubuntu/include -I/home/mhx/build/ubuntu/thrift -isystem /home/mhx/build/ubuntu/_deps/fmt-src/include -isystem /workspace/fbthrift -isystem /home/mhx/build/ubuntu -isystem /home/mhx/build/ubuntu/folly -isystem /workspace/folly -isystem /usr/include/libiberty -DNDEBUG -c -MD -MT CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o -MF CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o.d -fcolor-diagnostics -o CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o /workspace/src/writer/categorizer/binary_categorizer.cpp
1.	<eof> parser at end of file
2.	/workspace/src/writer/categorizer/binary_categorizer.cpp:308:24: instantiating function definition 'dwarfs::writer::(anonymous namespace)::minimal_macho_fat_header::check(inode_fragments &, std::span<const uint8_t>, const file_view &, const category_mapper &, sync_subcat_map &)::(anonymous class)::operator()<dwarfs::writer::(anonymous namespace)::minimal_macho_fat_header::arch64>'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007ff232a5d3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.18.1      0x00007ff232a5b4f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007ff2329a7227
3  libc.so.6            0x00007ff231865330
4  libclang-cpp.so.18.1 0x00007ff23a9daf6a clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) + 698
5  libclang-cpp.so.18.1 0x00007ff23a9a30a6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) + 134
6  libclang-cpp.so.18.1 0x00007ff23a9a3005 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) + 149
7  libclang-cpp.so.18.1 0x00007ff23a9a733c clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) + 1372
8  libclang-cpp.so.18.1 0x00007ff23ae39871
9  libclang-cpp.so.18.1 0x00007ff23ae3008d
10 libclang-cpp.so.18.1 0x00007ff23ae2d4b4
11 libclang-cpp.so.18.1 0x00007ff23ae3142d
12 libclang-cpp.so.18.1 0x00007ff23ae34670
13 libclang-cpp.so.18.1 0x00007ff23ae27c77
14 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
15 libclang-cpp.so.18.1 0x00007ff23a7ec828
16 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
17 libclang-cpp.so.18.1 0x00007ff23a7e6109
18 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
19 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) + 857
20 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) + 3138
21 libclang-cpp.so.18.1 0x00007ff23ae155c7
22 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 64
23 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) + 2379
24 libclang-cpp.so.18.1 0x00007ff23ac8e1eb clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) + 683
25 libclang-cpp.so.18.1 0x00007ff23ac95012 clang::Sema::AddArgumentDependentLookupCandidates(clang::DeclarationName, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::TemplateArgumentListInfo*, clang::OverloadCandidateSet&, bool) + 1058
26 libclang-cpp.so.18.1 0x00007ff23aca15bc clang::Sema::LookupOverloadedBinOp(clang::OverloadCandidateSet&, clang::OverloadedOperatorKind, clang::UnresolvedSetImpl const&, llvm::ArrayRef<clang::Expr*>, bool) + 428
27 libclang-cpp.so.18.1 0x00007ff23aca1939 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) + 745
28 libclang-cpp.so.18.1 0x00007ff23a9d2086
29 libclang-cpp.so.18.1 0x00007ff23a9d1c2f clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) + 1263
30 libclang-cpp.so.18.1 0x00007ff23ae315db
31 libclang-cpp.so.18.1 0x00007ff23ae34670
32 libclang-cpp.so.18.1 0x00007ff23ae27c77
33 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
34 libclang-cpp.so.18.1 0x00007ff23a7ec828
35 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
36 libclang-cpp.so.18.1 0x00007ff23a7e6109
37 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
38 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
39 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
40 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
41 libclang-cpp.so.18.1 0x00007ff23a7ec828
42 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
43 libclang-cpp.so.18.1 0x00007ff23a7eb711
44 libclang-cpp.so.18.1 0x00007ff23a7e6109
45 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
46 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
47 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
48 libclang-cpp.so.18.1 0x00007ff23ae22791 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
49 libclang-cpp.so.18.1 0x00007ff23aad4bdf clang::Sema::BuildExprRequirement(clang::Expr*, bool, clang::SourceLocation, clang::concepts::ExprRequirement::ReturnTypeRequirement) + 575
50 libclang-cpp.so.18.1 0x00007ff23ae34703
51 libclang-cpp.so.18.1 0x00007ff23ae27c77
52 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
53 libclang-cpp.so.18.1 0x00007ff23a7ec828
54 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
55 libclang-cpp.so.18.1 0x00007ff23a7e6109
56 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
57 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
58 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
59 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
60 libclang-cpp.so.18.1 0x00007ff23a7ec828
61 libclang-cpp.so.18.1 0x00007ff23a7ebc91
62 libclang-cpp.so.18.1 0x00007ff23a7ebdf1
63 libclang-cpp.so.18.1 0x00007ff23a7eb6be
64 libclang-cpp.so.18.1 0x00007ff23a7e6109
65 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
66 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) + 857
67 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) + 3138
68 libclang-cpp.so.18.1 0x00007ff23ae155c7
69 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 64
70 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) + 2379
71 libclang-cpp.so.18.1 0x00007ff23ac8dc43 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) + 643
72 libclang-cpp.so.18.1 0x00007ff23ac8e5c9 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) + 201
73 libclang-cpp.so.18.1 0x00007ff23aca6f16 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) + 950
74 libclang-cpp.so.18.1 0x00007ff23a99db7d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) + 2509
75 libclang-cpp.so.18.1 0x00007ff23a9b80a1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) + 81
76 libclang-cpp.so.18.1 0x00007ff23ae2d65d
77 libclang-cpp.so.18.1 0x00007ff23ae34670
78 libclang-cpp.so.18.1 0x00007ff23ae27c77
79 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
80 libclang-cpp.so.18.1 0x00007ff23a7ec828
81 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
82 libclang-cpp.so.18.1 0x00007ff23a7e6109
83 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
84 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
85 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
86 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
87 libclang-cpp.so.18.1 0x00007ff23a7ec828
88 libclang-cpp.so.18.1 0x00007ff23a7ebc91
89 libclang-cpp.so.18.1 0x00007ff23a7eb6be
90 libclang-cpp.so.18.1 0x00007ff23a7e6109
91 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
92 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
93 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
94 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
95 libclang-cpp.so.18.1 0x00007ff23a7ec828
96 libclang-cpp.so.18.1 0x00007ff23a7ebc91
97 libclang-cpp.so.18.1 0x00007ff23a7eb6be
98 libclang-cpp.so.18.1 0x00007ff23a7e6109
99 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
100 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
101 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
102 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
103 libclang-cpp.so.18.1 0x00007ff23a7ec828
104 libclang-cpp.so.18.1 0x00007ff23a7ebc91
105 libclang-cpp.so.18.1 0x00007ff23a7eb6be
106 libclang-cpp.so.18.1 0x00007ff23a7e6109
107 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
108 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
109 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
110 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
111 libclang-cpp.so.18.1 0x00007ff23a7ec828
112 libclang-cpp.so.18.1 0x00007ff23a7ebc91
113 libclang-cpp.so.18.1 0x00007ff23a7eb6be
114 libclang-cpp.so.18.1 0x00007ff23a7e6109
115 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
116 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
117 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
118 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
119 libclang-cpp.so.18.1 0x00007ff23a7ec828
120 libclang-cpp.so.18.1 0x00007ff23a7ebc91
121 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
122 libclang-cpp.so.18.1 0x00007ff23a7eb6be
123 libclang-cpp.so.18.1 0x00007ff23a7e6109
124 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
125 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
126 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
127 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) + 81
128 libclang-cpp.so.18.1 0x00007ff23a7ec828
129 libclang-cpp.so.18.1 0x00007ff23a7ebc91
130 libclang-cpp.so.18.1 0x00007ff23a7ebdf1
131 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
132 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
133 libclang-cpp.so.18.1 0x00007ff23a7eb6be
134 libclang-cpp.so.18.1 0x00007ff23a7e6109
135 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 805
136 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) + 857
137 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) + 3138
138 libclang-cpp.so.18.1 0x00007ff23ae155c7
139 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 64
140 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) + 2379
141 libclang-cpp.so.18.1 0x00007ff23ac8e1eb clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) + 683
142 libclang-cpp.so.18.1 0x00007ff23ab7f432
143 libclang-cpp.so.18.1 0x00007ff23ab6698a
144 libclang-cpp.so.18.1 0x00007ff23ab6493d
145 libclang-cpp.so.18.1 0x00007ff23ab62e34 clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) + 532
146 libclang-cpp.so.18.1 0x00007ff23aabe624 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) + 2116
147 libclang-cpp.so.18.1 0x00007ff23ae2dfa6
148 libclang-cpp.so.18.1 0x00007ff23ae230c6
149 libclang-cpp.so.18.1 0x00007ff23ae2d517
150 libclang-cpp.so.18.1 0x00007ff23ae21a77
151 libclang-cpp.so.18.1 0x00007ff23ae32669
152 libclang-cpp.so.18.1 0x00007ff23ae21a03 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) + 83
153 libclang-cpp.so.18.1 0x00007ff23ae63f26 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) + 2662
154 libclang-cpp.so.18.1 0x00007ff23ae66744 clang::Sema::PerformPendingInstantiations(bool) + 628
155 libclang-cpp.so.18.1 0x00007ff23a6b090b clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) + 427
156 libclang-cpp.so.18.1 0x00007ff23a6b0f7b clang::Sema::ActOnEndOfTranslationUnit() + 363
157 libclang-cpp.so.18.1 0x00007ff239f21e2e clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1166
158 libclang-cpp.so.18.1 0x00007ff239e5947e clang::ParseAST(clang::Sema&, bool, bool) + 766
159 libclang-cpp.so.18.1 0x00007ff23bcc862c clang::FrontendAction::Execute() + 92
160 libclang-cpp.so.18.1 0x00007ff23bc450b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 708
161 libclang-cpp.so.18.1 0x00007ff23bd4463d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
162 clang++-18           0x00005569d13b642e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 3694
163 clang++-18           0x00005569d13b3894
164 libclang-cpp.so.18.1 0x00007ff23b8f5972
165 libLLVM.so.18.1      0x00007ff2329a6f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
166 libclang-cpp.so.18.1 0x00007ff23b8f5237 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 407
167 libclang-cpp.so.18.1 0x00007ff23b8bd518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 888
168 libclang-cpp.so.18.1 0x00007ff23b8bd77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
169 libclang-cpp.so.18.1 0x00007ff23b8d9c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 352
170 clang++-18           0x00005569d13b31ec clang_main(int, char**, llvm::ToolContext const&) + 11180
171 clang++-18           0x00005569d13c0383 main + 131
172 libc.so.6            0x00007ff23184a1ca
173 libc.so.6            0x00007ff23184a28b __libc_start_main + 139
174 clang++-18           0x00005569d13b0255 _start + 37
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/binary_categorizer-36102d.cpp
clang++-18: note: diagnostic msg: /tmp/binary_categorizer-36102d.sh
clang++-18: note: diagnostic msg: 

********************
```

[binary_categorizer-36102d.cpp](https://github.com/user-attachments/files/23193992/binary_categorizer-36102d.cpp)
[binary_categorizer-36102d.sh](https://github.com/user-attachments/files/23193991/binary_categorizer-36102d.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: mhx (mhx)

<details>
```
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -O2 -g -std=c++20 -fPIC -Wall -Wextra -pedantic -Wold-style-cast -Wnon-virtual-dtor -Wsuggest-override -Wpessimizing-move -Wfloat-equal -Wcast-align -Wpointer-arith -Wformat=2 -Wnull-dereference -fcolor-diagnostics -DBOOST_ATOMIC_DYN_LINK -DBOOST_ATOMIC_NO_LIB -DBOOST_CHRONO_DYN_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_PROGRAM_OPTIONS_NO_LIB -DDWARFS_HAVE_CLOSE_RANGE=1 -DFMT_HEADER_ONLY -DFOLLY_CFG_NO_COROUTINES -DGFLAGS_IS_A_DLL=0 -DGLOG_NO_ABBREVIATED_SEVERITIES -DGLOG_USE_GLOG_EXPORT -DNOGDI -DNOMINMAX -I/workspace/include -I/home/mhx/build/ubuntu/include -I/home/mhx/build/ubuntu/thrift -isystem /home/mhx/build/ubuntu/_deps/fmt-src/include -isystem /workspace/fbthrift -isystem /home/mhx/build/ubuntu -isystem /home/mhx/build/ubuntu/folly -isystem /workspace/folly -isystem /usr/include/libiberty -DNDEBUG -c -MD -MT CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o -MF CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o.d -fcolor-diagnostics -o CMakeFiles/dwarfs_writer.dir/src/writer/categorizer/binary_categorizer.cpp.o /workspace/src/writer/categorizer/binary_categorizer.cpp
1.	&lt;eof&gt; parser at end of file
2.	/workspace/src/writer/categorizer/binary_categorizer.cpp:308:24: instantiating function definition 'dwarfs::writer::(anonymous namespace)::minimal_macho_fat_header::check(inode_fragments &amp;, std::span&lt;const uint8_t&gt;, const file_view &amp;, const category_mapper &amp;, sync_subcat_map &amp;)::(anonymous class)::operator()&lt;dwarfs::writer::(anonymous namespace)::minimal_macho_fat_header::arch64&gt;'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007ff232a5d3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM.so.18.1      0x00007ff232a5b4f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007ff2329a7227
3  libc.so.6            0x00007ff231865330
4  libclang-cpp.so.18.1 0x00007ff23a9daf6a clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) + 698
5  libclang-cpp.so.18.1 0x00007ff23a9a30a6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) + 134
6  libclang-cpp.so.18.1 0x00007ff23a9a3005 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) + 149
7  libclang-cpp.so.18.1 0x00007ff23a9a733c clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) + 1372
8  libclang-cpp.so.18.1 0x00007ff23ae39871
9  libclang-cpp.so.18.1 0x00007ff23ae3008d
10 libclang-cpp.so.18.1 0x00007ff23ae2d4b4
11 libclang-cpp.so.18.1 0x00007ff23ae3142d
12 libclang-cpp.so.18.1 0x00007ff23ae34670
13 libclang-cpp.so.18.1 0x00007ff23ae27c77
14 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
15 libclang-cpp.so.18.1 0x00007ff23a7ec828
16 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
17 libclang-cpp.so.18.1 0x00007ff23a7e6109
18 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
19 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) + 857
20 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) + 3138
21 libclang-cpp.so.18.1 0x00007ff23ae155c7
22 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 64
23 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) + 2379
24 libclang-cpp.so.18.1 0x00007ff23ac8e1eb clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) + 683
25 libclang-cpp.so.18.1 0x00007ff23ac95012 clang::Sema::AddArgumentDependentLookupCandidates(clang::DeclarationName, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::TemplateArgumentListInfo*, clang::OverloadCandidateSet&amp;, bool) + 1058
26 libclang-cpp.so.18.1 0x00007ff23aca15bc clang::Sema::LookupOverloadedBinOp(clang::OverloadCandidateSet&amp;, clang::OverloadedOperatorKind, clang::UnresolvedSetImpl const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, bool) + 428
27 libclang-cpp.so.18.1 0x00007ff23aca1939 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) + 745
28 libclang-cpp.so.18.1 0x00007ff23a9d2086
29 libclang-cpp.so.18.1 0x00007ff23a9d1c2f clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) + 1263
30 libclang-cpp.so.18.1 0x00007ff23ae315db
31 libclang-cpp.so.18.1 0x00007ff23ae34670
32 libclang-cpp.so.18.1 0x00007ff23ae27c77
33 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
34 libclang-cpp.so.18.1 0x00007ff23a7ec828
35 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
36 libclang-cpp.so.18.1 0x00007ff23a7e6109
37 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
38 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
39 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
40 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
41 libclang-cpp.so.18.1 0x00007ff23a7ec828
42 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
43 libclang-cpp.so.18.1 0x00007ff23a7eb711
44 libclang-cpp.so.18.1 0x00007ff23a7e6109
45 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
46 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
47 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
48 libclang-cpp.so.18.1 0x00007ff23ae22791 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
49 libclang-cpp.so.18.1 0x00007ff23aad4bdf clang::Sema::BuildExprRequirement(clang::Expr*, bool, clang::SourceLocation, clang::concepts::ExprRequirement::ReturnTypeRequirement) + 575
50 libclang-cpp.so.18.1 0x00007ff23ae34703
51 libclang-cpp.so.18.1 0x00007ff23ae27c77
52 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
53 libclang-cpp.so.18.1 0x00007ff23a7ec828
54 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
55 libclang-cpp.so.18.1 0x00007ff23a7e6109
56 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
57 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
58 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
59 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
60 libclang-cpp.so.18.1 0x00007ff23a7ec828
61 libclang-cpp.so.18.1 0x00007ff23a7ebc91
62 libclang-cpp.so.18.1 0x00007ff23a7ebdf1
63 libclang-cpp.so.18.1 0x00007ff23a7eb6be
64 libclang-cpp.so.18.1 0x00007ff23a7e6109
65 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
66 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) + 857
67 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) + 3138
68 libclang-cpp.so.18.1 0x00007ff23ae155c7
69 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 64
70 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) + 2379
71 libclang-cpp.so.18.1 0x00007ff23ac8dc43 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) + 643
72 libclang-cpp.so.18.1 0x00007ff23ac8e5c9 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) + 201
73 libclang-cpp.so.18.1 0x00007ff23aca6f16 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) + 950
74 libclang-cpp.so.18.1 0x00007ff23a99db7d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) + 2509
75 libclang-cpp.so.18.1 0x00007ff23a9b80a1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) + 81
76 libclang-cpp.so.18.1 0x00007ff23ae2d65d
77 libclang-cpp.so.18.1 0x00007ff23ae34670
78 libclang-cpp.so.18.1 0x00007ff23ae27c77
79 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
80 libclang-cpp.so.18.1 0x00007ff23a7ec828
81 libclang-cpp.so.18.1 0x00007ff23a7eb5b1
82 libclang-cpp.so.18.1 0x00007ff23a7e6109
83 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
84 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
85 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
86 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
87 libclang-cpp.so.18.1 0x00007ff23a7ec828
88 libclang-cpp.so.18.1 0x00007ff23a7ebc91
89 libclang-cpp.so.18.1 0x00007ff23a7eb6be
90 libclang-cpp.so.18.1 0x00007ff23a7e6109
91 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
92 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
93 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
94 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
95 libclang-cpp.so.18.1 0x00007ff23a7ec828
96 libclang-cpp.so.18.1 0x00007ff23a7ebc91
97 libclang-cpp.so.18.1 0x00007ff23a7eb6be
98 libclang-cpp.so.18.1 0x00007ff23a7e6109
99 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
100 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
101 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
102 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
103 libclang-cpp.so.18.1 0x00007ff23a7ec828
104 libclang-cpp.so.18.1 0x00007ff23a7ebc91
105 libclang-cpp.so.18.1 0x00007ff23a7eb6be
106 libclang-cpp.so.18.1 0x00007ff23a7e6109
107 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
108 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
109 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
110 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
111 libclang-cpp.so.18.1 0x00007ff23a7ec828
112 libclang-cpp.so.18.1 0x00007ff23a7ebc91
113 libclang-cpp.so.18.1 0x00007ff23a7eb6be
114 libclang-cpp.so.18.1 0x00007ff23a7e6109
115 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
116 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
117 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
118 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
119 libclang-cpp.so.18.1 0x00007ff23a7ec828
120 libclang-cpp.so.18.1 0x00007ff23a7ebc91
121 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
122 libclang-cpp.so.18.1 0x00007ff23a7eb6be
123 libclang-cpp.so.18.1 0x00007ff23a7e6109
124 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
125 libclang-cpp.so.18.1 0x00007ff23ad18020 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) + 704
126 libclang-cpp.so.18.1 0x00007ff23ae2cf8d
127 libclang-cpp.so.18.1 0x00007ff23ae22b51 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) + 81
128 libclang-cpp.so.18.1 0x00007ff23a7ec828
129 libclang-cpp.so.18.1 0x00007ff23a7ebc91
130 libclang-cpp.so.18.1 0x00007ff23a7ebdf1
131 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
132 libclang-cpp.so.18.1 0x00007ff23a7ebd9e
133 libclang-cpp.so.18.1 0x00007ff23a7eb6be
134 libclang-cpp.so.18.1 0x00007ff23a7e6109
135 libclang-cpp.so.18.1 0x00007ff23a7e5d85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 805
136 libclang-cpp.so.18.1 0x00007ff23a7e8899 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) + 857
137 libclang-cpp.so.18.1 0x00007ff23adbf442 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) + 3138
138 libclang-cpp.so.18.1 0x00007ff23ae155c7
139 libclang-cpp.so.18.1 0x00007ff23a6aede0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 64
140 libclang-cpp.so.18.1 0x00007ff23adc0d4b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) + 2379
141 libclang-cpp.so.18.1 0x00007ff23ac8e1eb clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) + 683
142 libclang-cpp.so.18.1 0x00007ff23ab7f432
143 libclang-cpp.so.18.1 0x00007ff23ab6698a
144 libclang-cpp.so.18.1 0x00007ff23ab6493d
145 libclang-cpp.so.18.1 0x00007ff23ab62e34 clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) + 532
146 libclang-cpp.so.18.1 0x00007ff23aabe624 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) + 2116
147 libclang-cpp.so.18.1 0x00007ff23ae2dfa6
148 libclang-cpp.so.18.1 0x00007ff23ae230c6
149 libclang-cpp.so.18.1 0x00007ff23ae2d517
150 libclang-cpp.so.18.1 0x00007ff23ae21a77
151 libclang-cpp.so.18.1 0x00007ff23ae32669
152 libclang-cpp.so.18.1 0x00007ff23ae21a03 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) + 83
153 libclang-cpp.so.18.1 0x00007ff23ae63f26 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) + 2662
154 libclang-cpp.so.18.1 0x00007ff23ae66744 clang::Sema::PerformPendingInstantiations(bool) + 628
155 libclang-cpp.so.18.1 0x00007ff23a6b090b clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) + 427
156 libclang-cpp.so.18.1 0x00007ff23a6b0f7b clang::Sema::ActOnEndOfTranslationUnit() + 363
157 libclang-cpp.so.18.1 0x00007ff239f21e2e clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1166
158 libclang-cpp.so.18.1 0x00007ff239e5947e clang::ParseAST(clang::Sema&amp;, bool, bool) + 766
159 libclang-cpp.so.18.1 0x00007ff23bcc862c clang::FrontendAction::Execute() + 92
160 libclang-cpp.so.18.1 0x00007ff23bc450b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 708
161 libclang-cpp.so.18.1 0x00007ff23bd4463d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
162 clang++-18           0x00005569d13b642e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 3694
163 clang++-18           0x00005569d13b3894
164 libclang-cpp.so.18.1 0x00007ff23b8f5972
165 libLLVM.so.18.1      0x00007ff2329a6f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 151
166 libclang-cpp.so.18.1 0x00007ff23b8f5237 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 407
167 libclang-cpp.so.18.1 0x00007ff23b8bd518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 888
168 libclang-cpp.so.18.1 0x00007ff23b8bd77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 159
169 libclang-cpp.so.18.1 0x00007ff23b8d9c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 352
170 clang++-18           0x00005569d13b31ec clang_main(int, char**, llvm::ToolContext const&amp;) + 11180
171 clang++-18           0x00005569d13c0383 main + 131
172 libc.so.6            0x00007ff23184a1ca
173 libc.so.6            0x00007ff23184a28b __libc_start_main + 139
174 clang++-18           0x00005569d13b0255 _start + 37
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/binary_categorizer-36102d.cpp
clang++-18: note: diagnostic msg: /tmp/binary_categorizer-36102d.sh
clang++-18: note: diagnostic msg: 

********************
```

[binary_categorizer-36102d.cpp](https://github.com/user-attachments/files/23193992/binary_categorizer-36102d.cpp)
[binary_categorizer-36102d.sh](https://github.com/user-attachments/files/23193991/binary_categorizer-36102d.sh)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.

---

### Comment 3 - mhx

> Could you please try 21 or `main` branch? Only most recent release is maintained.

It's fixed in 19/20/21, so feel free to close this if it's not relevant.

---

