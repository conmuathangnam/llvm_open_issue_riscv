# Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/164330
**Status:** Closed
**Labels:** clang:frontend, crash-on-valid
**Closed Date:** 2025-10-21T02:00:39Z

## Body

The following (originally valid, hopefully, this wasn't over-reduced) code triggers an assertion failure in Clang (https://gcc.godbolt.org/z/Y1e5Pd7fG):

```
template <int __v> struct integral_constant {
  static constexpr int value = __v;
};
template <bool _Val> using _BoolConstant = integral_constant<_Val>;
template <int, class> struct tuple_element;
template <class...> class tuple;
template <int _Ip, class... _Tp> struct tuple_element<_Ip, tuple<_Tp...>> {
  using type = __type_pack_element<_Ip, _Tp...>;
};
template <class> struct tuple_size;
template <bool> using __enable_if_t = int;
template <template <class> class _BaseType, class _Tp, _Tp _SequenceSize>
using __make_integer_sequence_impl =
    __make_integer_seq<_BaseType, _Tp, _SequenceSize>;
template <class _Tp, _Tp...> struct __integer_sequence;
template <int... _Indices>
using __index_sequence = __integer_sequence<int, _Indices...>;
template <int _SequenceSize>
using __make_index_sequence =
    __make_integer_sequence_impl<__integer_sequence, int, _SequenceSize>;
template <class _Tp, _Tp...> struct integer_sequence {};
template <int... _Ip> using index_sequence = integer_sequence<int, _Ip...>;
template <class _Tp, _Tp _Ep>
using make_integer_sequence =
    __make_integer_sequence_impl<integer_sequence, _Tp, _Ep>;
template <int _Np> using make_index_sequence = make_integer_sequence<int, _Np>;
enum __element_count : int;
constexpr void __constexpr_memmove(char *__dest, const char *__src,
                                   __element_count __n) {
  __builtin_memmove(__dest, __src, __n);
}
template <class _Tp> using __underlying_type_t = __underlying_type(_Tp);
template <class _Tp> using underlying_type_t = __underlying_type_t<_Tp>;
template <class _Tp, class> using __enable_if_tuple_size_imp = _Tp;
template <class _Tp>
struct tuple_size<__enable_if_tuple_size_imp<
    const _Tp, __enable_if_t<_BoolConstant<__is_volatile(int)>::value>>>
    : integral_constant<tuple_size<_Tp>::value> {};
template <class... _Tp>
struct tuple_size<tuple<_Tp...>> : integral_constant<sizeof...(_Tp)> {};
template <class _Tp> constexpr int tuple_size_v = tuple_size<_Tp>::value;
template <class _T1, class _T2> struct pair {
  _T1 first;
  _T2 second;
};
template <class _T1> constexpr pair<_T1, char *> make_pair(_T1, char *__t2) {
  return pair(_T1(), __t2);
}
template <int, class _Hp> struct __tuple_leaf {
  _Hp __value_;
  constexpr const _Hp &get() const { return __value_; }
};
template <class...> struct __tuple_impl;
template <int... _Indx, class... _Tp>
struct __tuple_impl<__index_sequence<_Indx...>, _Tp...>
    : __tuple_leaf<_Indx, _Tp>... {
  template <class... _Args>
  constexpr __tuple_impl(int, _Args... __args)
      : __tuple_leaf<_Indx, _Tp>(__args)... {}
};
template <class... _Tp> struct tuple {
  __tuple_impl<__make_index_sequence<sizeof...(_Tp)>, _Tp...> __base_;
  template <class... _Up> constexpr tuple(_Up... __u) : __base_({}, __u...) {}
};
template <int _Ip, class... _Tp>
constexpr const tuple_element<_Ip, tuple<_Tp...>>::type &
get(const tuple<_Tp...> &__t) noexcept {
  return static_cast<const __tuple_leaf<
      _Ip, typename tuple_element<_Ip, tuple<_Tp...>>::type> &>(__t.__base_)
      .get();
}
template <class... _Tp> constexpr tuple<_Tp...> make_tuple(_Tp... __t) {
  return tuple<_Tp...>(__t...);
}
constexpr int __char_traits_length_checked(const char *__s) {
  return __builtin_strlen(__s);
}
struct basic_string_view {
  constexpr basic_string_view() {}
  constexpr basic_string_view(const char *__s)
      : __data_(__s), __size_(__char_traits_length_checked(__s)) {}
  constexpr const char *begin() { return __data_; }
  constexpr const char *end() {
    return __data_ + __size_;
  }
  const char *__data_;
  int __size_;
};
template <class _Algorithm>
constexpr pair<const char *, char *>
__copy_move_unwrap_iters(const char *__first, const char *__last,
                         char *__out_first) {
  pair<const char *, const char *> __range = {__first, __last};
  auto __result = _Algorithm()(__range.first, __range.second, __out_first);
  return make_pair(__result.first, __result.second);
}
struct __copy_impl {
  constexpr pair<const char *, char *>
  operator()(const char *__first, const char *__last, char *__result) {
    const int __n(__last - __first);
    __constexpr_memmove(__result, __first, __element_count(__n));
    return make_pair(__last, __result);
  }
};
constexpr char *copy(const char *__first, const char *__last, char *__result) {
  return __copy_move_unwrap_iters<__copy_impl>(__first, __last, __result).second;
}
constexpr char *copy_n(const char *__first, int __orig_n, char *__result) {
  return copy(__first, __first + __orig_n, __result);
}
template <int _Size> struct array {
  basic_string_view __elems_[_Size];
  constexpr basic_string_view &operator[](int __n) { return __elems_[__n]; }
  constexpr basic_string_view operator[](int __n) const {
    return __elems_[__n];
  }
};

template <typename> struct FieldId;

template <FieldId field> constexpr auto FieldIdToInnerValue() {
  return field.template ToInnerValue<field>();
}
struct FieldNameEnum {
  enum class type;
};
template <int N> using FieldName = FieldNameEnum::type;
template <typename, auto> struct GetParentMessageAtIndexImpl;
template <typename, auto> struct FieldInfoHelper;
template <FieldId...> struct PathImpl;
template <int N> struct LongPathLiteral {
  consteval LongPathLiteral(const char (&s)[N]) {
    copy_n(s, N, long_path)[N] = field_count = long_path_size = 1;
  }
  consteval basic_string_view to_string_view() const { return long_path; }
  char long_path[N + 1];
  int long_path_size;
  int field_count;
};
template <LongPathLiteral kLongPath> consteval auto get_field_components() {
  basic_string_view long_path(kLongPath.to_string_view());
  array<kLongPath.field_count> ret;
  for (int i = 0; i < kLongPath.field_count; ++i)
    ret[i] = long_path;
  return ret;
}
template <LongPathLiteral kLongPath>
constexpr auto kFieldComponents = get_field_components<kLongPath>();
template <LongPathLiteral kLongPath> struct LongPathHelper {
  template <int... I>
  static PathImpl<kFieldComponents<kLongPath>[I]...>
      PathForLongPath(index_sequence<I...>);
  using type =
      decltype(PathForLongPath(make_index_sequence<kLongPath.field_count>{}));
};
template <typename T> struct PathFieldId {
  template <typename Arg> constexpr PathFieldId(Arg &arg) : value(arg) {}
  T value;
};
template <PathFieldId...> constexpr auto PathImplHelper();

template <int N> using FieldName = FieldName<N>;
enum class FieldNumber;
template <PathFieldId... fields>
constexpr auto Path = PathImplHelper<fields...>();
template <typename Proto, FieldId field>
using FieldInfo =
    FieldInfoHelper<Proto, FieldIdToInnerValue<field>()>::type;
template <> struct FieldId<FieldNameEnum::type> {
  constexpr FieldId(basic_string_view);
  int size;
  long hash;
  template <auto field> static constexpr auto ToInnerValue() {
    return static_cast<FieldNameEnum::type>(field.hash);
  }
};
FieldId(basic_string_view) -> FieldId<FieldNameEnum::type>;
template <typename Proto, FieldId field, int index>
using GetParentMessageAtIndex = GetParentMessageAtIndexImpl<
    Proto, FieldIdToInnerValue<field>()>::type;

template <typename T>
PathFieldId(T &t) -> PathFieldId<decltype(LongPathLiteral(t))>;
template <FieldId... fields1, FieldId... fields2>
constexpr PathImpl<fields1..., fields2...> *ConcatPath(PathImpl<fields1...> *,
                                                       PathImpl<fields2...> *) {
  return nullptr;
}
template <LongPathLiteral long_path_literal>
constexpr LongPathHelper<long_path_literal>::type *SinglePath() {
  return nullptr;
}
template <PathFieldId... fields> constexpr auto PathImplHelper() {
  return ConcatPath(SinglePath<fields.value>()...);
}
template <auto hash_prime, auto offset_bias>
constexpr auto Fnv1a(basic_string_view str) {
  auto hash = offset_bias;
  for (char c : str) {
    hash ^= c;
    hash *= hash_prime;
  }
  return hash;
}
constexpr auto HashField(basic_string_view str) {
  return Fnv1a<1099511628211u, 1039346656037>(str);
}
template <typename FI> struct FieldInfoValueTypeAlias : FI {};
template <typename Proto, auto field> struct FieldInfoHelperBase {
  static constexpr auto MaskFieldNameHash() {
    using FieldEnum = decltype(field);
    return FieldEnum{static_cast<underlying_type_t<FieldEnum>>(field) & 31};
  }
  using internal_type =
      Proto::template FieldInfoImpl<decltype(field), MaskFieldNameHash()>;
};
template <typename Proto, auto field> struct FieldInfoHelper {
  using type = FieldInfoValueTypeAlias<
      typename FieldInfoHelperBase<Proto, field>::internal_type>;
};

template <auto... fields>
struct FieldId<const PathImpl<fields...> *> {
  constexpr FieldId(PathImpl<fields...> *) : path() {}
  template <auto field> static constexpr auto ToInnerValue() {
    return field.path;
  }
  const PathImpl<fields...> *path;
};
template <auto... fields>
FieldId(PathImpl<fields...> *)
    -> FieldId<const PathImpl<fields...> *>;

template <auto> struct UnpackedField {
  static constexpr bool is_path = false;
};
template <auto... fields, const PathImpl<fields...> *path>
struct UnpackedField<path> {
  static constexpr auto value = make_tuple(fields...);
  static constexpr bool is_path = true;
};
template <typename Proto, FieldId... fields, const PathImpl<fields...> *path>
struct GetParentMessageAtIndexImpl<Proto, path> {
  using type = Proto;
};

constexpr FieldId<FieldNameEnum::type>::FieldId(basic_string_view str)
    : size(), hash(HashField(str)) {}
template <FieldId field> constexpr bool IsPath() {
  return UnpackedField<
      FieldIdToInnerValue<field>()>::is_path;
}
template <FieldId field> constexpr auto UnpackFieldToTuple() {
  return UnpackedField<FieldIdToInnerValue<field>()>::value;
}
template <int> struct CompileTimeString {
  consteval CompileTimeString(basic_string_view &v) : internal_view_(v) {}
  basic_string_view &internal_view_;
};
CompileTimeString(basic_string_view) -> CompileTimeString<0>;

template <CompileTimeString... parts> struct NameJoiner {
  template <CompileTimeString... after>
  NameJoiner<parts...> operator+(NameJoiner<after...>);
};
template <FieldId> struct FieldNameBuilder;
template <FieldId field>
  requires(!IsPath<field>())
struct FieldNameBuilder<field> {
  template <typename Proto> static auto Get() {
    return NameJoiner<FieldInfo<Proto, field>::name>();
  }
};
template <FieldId field>
  requires(IsPath<field>())
struct FieldNameBuilder<field> {
  static constexpr auto kTuple = UnpackFieldToTuple<field>();
  static constexpr int kTupleSize = tuple_size_v<decltype(kTuple)>;
  template <typename Proto, int... Is> static void Get(index_sequence<Is...>) {
    (FieldNameBuilder<get<Is>(
         kTuple)>::template Get<GetParentMessageAtIndex<Proto, field, Is>>() +
     ...);
  }
  template <typename Proto> static void Get() {
    Get<Proto>(make_index_sequence<kTupleSize>());
  }
};

struct T {
  template <typename FieldType, FieldType> struct FieldInfoImpl;
};
void AddPathsToFieldMask() {
  FieldNameBuilder<Path<"message_field", "int32_field">>::Get<T>();
}
template <> struct T::FieldInfoImpl<FieldNumber, FieldNumber{1}> {
  static basic_string_view name;
};
template <>
struct T::FieldInfoImpl<FieldName<1>, FieldName<1>{12}>
    : FieldInfoImpl<FieldNumber, FieldNumber{1}> {};
template <> struct T::FieldInfoImpl<FieldNumber, FieldNumber{10}> {
  static basic_string_view name;
};
template <>
struct T::FieldInfoImpl<FieldName<3>, FieldName<3>{11}>
    : FieldInfoImpl<FieldNumber, FieldNumber{10}> {};
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeBase.h:953: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:284:41: instantiating function definition 'FieldNameBuilder<FieldId<const PathImpl<FieldId<FieldNameEnum::type>{0, -4724390119873347861}, FieldId<FieldNameEnum::type>{0, -4761221134017270484}> *>{nullptr}>::Get<T>'
3.	<source>:279:52: instantiating function definition 'FieldNameBuilder<FieldId<const PathImpl<FieldId<FieldNameEnum::type>{0, -4724390119873347861}, FieldId<FieldNameEnum::type>{0, -4761221134017270484}> *>{nullptr}>::Get<T, 0, 1>'
 #0 0x00000000041f5e08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f5e08)
 #1 0x00000000041f3234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f3234)
 #2 0x0000000004137498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f94f8842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f94f88969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f94f8842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f94f88287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f94f882871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f94f8839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004531044 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531044)
#10 0x0000000007438c7d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000741db35 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000074346e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007449077 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000742840d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007453418 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7453418)
#16 0x00000000074a83df clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a83df)
#17 0x000000000750be62 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750be62)
#18 0x000000000750de94 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#19 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#20 0x00000000074a819a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a819a)
#21 0x00000000073b5137 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73b5137)
#22 0x00000000073b64da void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#23 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#24 0x000000000739122b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x739122b)
#25 0x00000000071e4d6d AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#26 0x00000000071e7aa3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7aa3)
#27 0x00000000071e7cd3 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7cd3)
#28 0x000000000720290a clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720290a)
#29 0x0000000006e11fa2 BuildOverloadedBinOp(clang::Sema&, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#30 0x0000000006e67757 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e67757)
#31 0x0000000007443c7f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x000000000741d572 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000745a357 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#34 0x000000000745aa0e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#35 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
#36 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
#37 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#38 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
#39 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#40 0x0000000006a90ad2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#41 0x0000000006a90e52 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90e52)
#42 0x00000000068f9fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9fd0)
#43 0x00000000068db57a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db57a)
#44 0x0000000004b3e668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e668)
#45 0x0000000004e2bcf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2bcf5)
#46 0x0000000004dacd8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dacd8e)
#47 0x0000000004f2377d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2377d)
#48 0x0000000000dc3800 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3800)
#49 0x0000000000dba36a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dba4ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x0000000004ba7819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x0000000004137934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4137934)
#53 0x0000000004ba7e2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x0000000004b69142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b69142)
#55 0x0000000004b6a0ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6a0ee)
#56 0x0000000004b71825 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b71825)
#57 0x0000000000dbfd29 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfd29)
#58 0x0000000000c6fab4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6fab4)
#59 0x00007f94f8829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x00007f94f8829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000db9e05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9e05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
The following (originally valid, hopefully, this wasn't over-reduced) code triggers an assertion failure in Clang (https://gcc.godbolt.org/z/Y1e5Pd7fG):

```
template &lt;int __v&gt; struct integral_constant {
  static constexpr int value = __v;
};
template &lt;bool _Val&gt; using _BoolConstant = integral_constant&lt;_Val&gt;;
template &lt;int, class&gt; struct tuple_element;
template &lt;class...&gt; class tuple;
template &lt;int _Ip, class... _Tp&gt; struct tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt; {
  using type = __type_pack_element&lt;_Ip, _Tp...&gt;;
};
template &lt;class&gt; struct tuple_size;
template &lt;bool&gt; using __enable_if_t = int;
template &lt;template &lt;class&gt; class _BaseType, class _Tp, _Tp _SequenceSize&gt;
using __make_integer_sequence_impl =
    __make_integer_seq&lt;_BaseType, _Tp, _SequenceSize&gt;;
template &lt;class _Tp, _Tp...&gt; struct __integer_sequence;
template &lt;int... _Indices&gt;
using __index_sequence = __integer_sequence&lt;int, _Indices...&gt;;
template &lt;int _SequenceSize&gt;
using __make_index_sequence =
    __make_integer_sequence_impl&lt;__integer_sequence, int, _SequenceSize&gt;;
template &lt;class _Tp, _Tp...&gt; struct integer_sequence {};
template &lt;int... _Ip&gt; using index_sequence = integer_sequence&lt;int, _Ip...&gt;;
template &lt;class _Tp, _Tp _Ep&gt;
using make_integer_sequence =
    __make_integer_sequence_impl&lt;integer_sequence, _Tp, _Ep&gt;;
template &lt;int _Np&gt; using make_index_sequence = make_integer_sequence&lt;int, _Np&gt;;
enum __element_count : int;
constexpr void __constexpr_memmove(char *__dest, const char *__src,
                                   __element_count __n) {
  __builtin_memmove(__dest, __src, __n);
}
template &lt;class _Tp&gt; using __underlying_type_t = __underlying_type(_Tp);
template &lt;class _Tp&gt; using underlying_type_t = __underlying_type_t&lt;_Tp&gt;;
template &lt;class _Tp, class&gt; using __enable_if_tuple_size_imp = _Tp;
template &lt;class _Tp&gt;
struct tuple_size&lt;__enable_if_tuple_size_imp&lt;
    const _Tp, __enable_if_t&lt;_BoolConstant&lt;__is_volatile(int)&gt;::value&gt;&gt;&gt;
    : integral_constant&lt;tuple_size&lt;_Tp&gt;::value&gt; {};
template &lt;class... _Tp&gt;
struct tuple_size&lt;tuple&lt;_Tp...&gt;&gt; : integral_constant&lt;sizeof...(_Tp)&gt; {};
template &lt;class _Tp&gt; constexpr int tuple_size_v = tuple_size&lt;_Tp&gt;::value;
template &lt;class _T1, class _T2&gt; struct pair {
  _T1 first;
  _T2 second;
};
template &lt;class _T1&gt; constexpr pair&lt;_T1, char *&gt; make_pair(_T1, char *__t2) {
  return pair(_T1(), __t2);
}
template &lt;int, class _Hp&gt; struct __tuple_leaf {
  _Hp __value_;
  constexpr const _Hp &amp;get() const { return __value_; }
};
template &lt;class...&gt; struct __tuple_impl;
template &lt;int... _Indx, class... _Tp&gt;
struct __tuple_impl&lt;__index_sequence&lt;_Indx...&gt;, _Tp...&gt;
    : __tuple_leaf&lt;_Indx, _Tp&gt;... {
  template &lt;class... _Args&gt;
  constexpr __tuple_impl(int, _Args... __args)
      : __tuple_leaf&lt;_Indx, _Tp&gt;(__args)... {}
};
template &lt;class... _Tp&gt; struct tuple {
  __tuple_impl&lt;__make_index_sequence&lt;sizeof...(_Tp)&gt;, _Tp...&gt; __base_;
  template &lt;class... _Up&gt; constexpr tuple(_Up... __u) : __base_({}, __u...) {}
};
template &lt;int _Ip, class... _Tp&gt;
constexpr const tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt;::type &amp;
get(const tuple&lt;_Tp...&gt; &amp;__t) noexcept {
  return static_cast&lt;const __tuple_leaf&lt;
      _Ip, typename tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt;::type&gt; &amp;&gt;(__t.__base_)
      .get();
}
template &lt;class... _Tp&gt; constexpr tuple&lt;_Tp...&gt; make_tuple(_Tp... __t) {
  return tuple&lt;_Tp...&gt;(__t...);
}
constexpr int __char_traits_length_checked(const char *__s) {
  return __builtin_strlen(__s);
}
struct basic_string_view {
  constexpr basic_string_view() {}
  constexpr basic_string_view(const char *__s)
      : __data_(__s), __size_(__char_traits_length_checked(__s)) {}
  constexpr const char *begin() { return __data_; }
  constexpr const char *end() {
    return __data_ + __size_;
  }
  const char *__data_;
  int __size_;
};
template &lt;class _Algorithm&gt;
constexpr pair&lt;const char *, char *&gt;
__copy_move_unwrap_iters(const char *__first, const char *__last,
                         char *__out_first) {
  pair&lt;const char *, const char *&gt; __range = {__first, __last};
  auto __result = _Algorithm()(__range.first, __range.second, __out_first);
  return make_pair(__result.first, __result.second);
}
struct __copy_impl {
  constexpr pair&lt;const char *, char *&gt;
  operator()(const char *__first, const char *__last, char *__result) {
    const int __n(__last - __first);
    __constexpr_memmove(__result, __first, __element_count(__n));
    return make_pair(__last, __result);
  }
};
constexpr char *copy(const char *__first, const char *__last, char *__result) {
  return __copy_move_unwrap_iters&lt;__copy_impl&gt;(__first, __last, __result).second;
}
constexpr char *copy_n(const char *__first, int __orig_n, char *__result) {
  return copy(__first, __first + __orig_n, __result);
}
template &lt;int _Size&gt; struct array {
  basic_string_view __elems_[_Size];
  constexpr basic_string_view &amp;operator[](int __n) { return __elems_[__n]; }
  constexpr basic_string_view operator[](int __n) const {
    return __elems_[__n];
  }
};

template &lt;typename&gt; struct FieldId;

template &lt;FieldId field&gt; constexpr auto FieldIdToInnerValue() {
  return field.template ToInnerValue&lt;field&gt;();
}
struct FieldNameEnum {
  enum class type;
};
template &lt;int N&gt; using FieldName = FieldNameEnum::type;
template &lt;typename, auto&gt; struct GetParentMessageAtIndexImpl;
template &lt;typename, auto&gt; struct FieldInfoHelper;
template &lt;FieldId...&gt; struct PathImpl;
template &lt;int N&gt; struct LongPathLiteral {
  consteval LongPathLiteral(const char (&amp;s)[N]) {
    copy_n(s, N, long_path)[N] = field_count = long_path_size = 1;
  }
  consteval basic_string_view to_string_view() const { return long_path; }
  char long_path[N + 1];
  int long_path_size;
  int field_count;
};
template &lt;LongPathLiteral kLongPath&gt; consteval auto get_field_components() {
  basic_string_view long_path(kLongPath.to_string_view());
  array&lt;kLongPath.field_count&gt; ret;
  for (int i = 0; i &lt; kLongPath.field_count; ++i)
    ret[i] = long_path;
  return ret;
}
template &lt;LongPathLiteral kLongPath&gt;
constexpr auto kFieldComponents = get_field_components&lt;kLongPath&gt;();
template &lt;LongPathLiteral kLongPath&gt; struct LongPathHelper {
  template &lt;int... I&gt;
  static PathImpl&lt;kFieldComponents&lt;kLongPath&gt;[I]...&gt;
      PathForLongPath(index_sequence&lt;I...&gt;);
  using type =
      decltype(PathForLongPath(make_index_sequence&lt;kLongPath.field_count&gt;{}));
};
template &lt;typename T&gt; struct PathFieldId {
  template &lt;typename Arg&gt; constexpr PathFieldId(Arg &amp;arg) : value(arg) {}
  T value;
};
template &lt;PathFieldId...&gt; constexpr auto PathImplHelper();

template &lt;int N&gt; using FieldName = FieldName&lt;N&gt;;
enum class FieldNumber;
template &lt;PathFieldId... fields&gt;
constexpr auto Path = PathImplHelper&lt;fields...&gt;();
template &lt;typename Proto, FieldId field&gt;
using FieldInfo =
    FieldInfoHelper&lt;Proto, FieldIdToInnerValue&lt;field&gt;()&gt;::type;
template &lt;&gt; struct FieldId&lt;FieldNameEnum::type&gt; {
  constexpr FieldId(basic_string_view);
  int size;
  long hash;
  template &lt;auto field&gt; static constexpr auto ToInnerValue() {
    return static_cast&lt;FieldNameEnum::type&gt;(field.hash);
  }
};
FieldId(basic_string_view) -&gt; FieldId&lt;FieldNameEnum::type&gt;;
template &lt;typename Proto, FieldId field, int index&gt;
using GetParentMessageAtIndex = GetParentMessageAtIndexImpl&lt;
    Proto, FieldIdToInnerValue&lt;field&gt;()&gt;::type;

template &lt;typename T&gt;
PathFieldId(T &amp;t) -&gt; PathFieldId&lt;decltype(LongPathLiteral(t))&gt;;
template &lt;FieldId... fields1, FieldId... fields2&gt;
constexpr PathImpl&lt;fields1..., fields2...&gt; *ConcatPath(PathImpl&lt;fields1...&gt; *,
                                                       PathImpl&lt;fields2...&gt; *) {
  return nullptr;
}
template &lt;LongPathLiteral long_path_literal&gt;
constexpr LongPathHelper&lt;long_path_literal&gt;::type *SinglePath() {
  return nullptr;
}
template &lt;PathFieldId... fields&gt; constexpr auto PathImplHelper() {
  return ConcatPath(SinglePath&lt;fields.value&gt;()...);
}
template &lt;auto hash_prime, auto offset_bias&gt;
constexpr auto Fnv1a(basic_string_view str) {
  auto hash = offset_bias;
  for (char c : str) {
    hash ^= c;
    hash *= hash_prime;
  }
  return hash;
}
constexpr auto HashField(basic_string_view str) {
  return Fnv1a&lt;1099511628211u, 1039346656037&gt;(str);
}
template &lt;typename FI&gt; struct FieldInfoValueTypeAlias : FI {};
template &lt;typename Proto, auto field&gt; struct FieldInfoHelperBase {
  static constexpr auto MaskFieldNameHash() {
    using FieldEnum = decltype(field);
    return FieldEnum{static_cast&lt;underlying_type_t&lt;FieldEnum&gt;&gt;(field) &amp; 31};
  }
  using internal_type =
      Proto::template FieldInfoImpl&lt;decltype(field), MaskFieldNameHash()&gt;;
};
template &lt;typename Proto, auto field&gt; struct FieldInfoHelper {
  using type = FieldInfoValueTypeAlias&lt;
      typename FieldInfoHelperBase&lt;Proto, field&gt;::internal_type&gt;;
};

template &lt;auto... fields&gt;
struct FieldId&lt;const PathImpl&lt;fields...&gt; *&gt; {
  constexpr FieldId(PathImpl&lt;fields...&gt; *) : path() {}
  template &lt;auto field&gt; static constexpr auto ToInnerValue() {
    return field.path;
  }
  const PathImpl&lt;fields...&gt; *path;
};
template &lt;auto... fields&gt;
FieldId(PathImpl&lt;fields...&gt; *)
    -&gt; FieldId&lt;const PathImpl&lt;fields...&gt; *&gt;;

template &lt;auto&gt; struct UnpackedField {
  static constexpr bool is_path = false;
};
template &lt;auto... fields, const PathImpl&lt;fields...&gt; *path&gt;
struct UnpackedField&lt;path&gt; {
  static constexpr auto value = make_tuple(fields...);
  static constexpr bool is_path = true;
};
template &lt;typename Proto, FieldId... fields, const PathImpl&lt;fields...&gt; *path&gt;
struct GetParentMessageAtIndexImpl&lt;Proto, path&gt; {
  using type = Proto;
};

constexpr FieldId&lt;FieldNameEnum::type&gt;::FieldId(basic_string_view str)
    : size(), hash(HashField(str)) {}
template &lt;FieldId field&gt; constexpr bool IsPath() {
  return UnpackedField&lt;
      FieldIdToInnerValue&lt;field&gt;()&gt;::is_path;
}
template &lt;FieldId field&gt; constexpr auto UnpackFieldToTuple() {
  return UnpackedField&lt;FieldIdToInnerValue&lt;field&gt;()&gt;::value;
}
template &lt;int&gt; struct CompileTimeString {
  consteval CompileTimeString(basic_string_view &amp;v) : internal_view_(v) {}
  basic_string_view &amp;internal_view_;
};
CompileTimeString(basic_string_view) -&gt; CompileTimeString&lt;0&gt;;

template &lt;CompileTimeString... parts&gt; struct NameJoiner {
  template &lt;CompileTimeString... after&gt;
  NameJoiner&lt;parts...&gt; operator+(NameJoiner&lt;after...&gt;);
};
template &lt;FieldId&gt; struct FieldNameBuilder;
template &lt;FieldId field&gt;
  requires(!IsPath&lt;field&gt;())
struct FieldNameBuilder&lt;field&gt; {
  template &lt;typename Proto&gt; static auto Get() {
    return NameJoiner&lt;FieldInfo&lt;Proto, field&gt;::name&gt;();
  }
};
template &lt;FieldId field&gt;
  requires(IsPath&lt;field&gt;())
struct FieldNameBuilder&lt;field&gt; {
  static constexpr auto kTuple = UnpackFieldToTuple&lt;field&gt;();
  static constexpr int kTupleSize = tuple_size_v&lt;decltype(kTuple)&gt;;
  template &lt;typename Proto, int... Is&gt; static void Get(index_sequence&lt;Is...&gt;) {
    (FieldNameBuilder&lt;get&lt;Is&gt;(
         kTuple)&gt;::template Get&lt;GetParentMessageAtIndex&lt;Proto, field, Is&gt;&gt;() +
     ...);
  }
  template &lt;typename Proto&gt; static void Get() {
    Get&lt;Proto&gt;(make_index_sequence&lt;kTupleSize&gt;());
  }
};

struct T {
  template &lt;typename FieldType, FieldType&gt; struct FieldInfoImpl;
};
void AddPathsToFieldMask() {
  FieldNameBuilder&lt;Path&lt;"message_field", "int32_field"&gt;&gt;::Get&lt;T&gt;();
}
template &lt;&gt; struct T::FieldInfoImpl&lt;FieldNumber, FieldNumber{1}&gt; {
  static basic_string_view name;
};
template &lt;&gt;
struct T::FieldInfoImpl&lt;FieldName&lt;1&gt;, FieldName&lt;1&gt;{12}&gt;
    : FieldInfoImpl&lt;FieldNumber, FieldNumber{1}&gt; {};
template &lt;&gt; struct T::FieldInfoImpl&lt;FieldNumber, FieldNumber{10}&gt; {
  static basic_string_view name;
};
template &lt;&gt;
struct T::FieldInfoImpl&lt;FieldName&lt;3&gt;, FieldName&lt;3&gt;{11}&gt;
    : FieldInfoImpl&lt;FieldNumber, FieldNumber{10}&gt; {};
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeBase.h:953: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:284:41: instantiating function definition 'FieldNameBuilder&lt;FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNameEnum::type&gt;{0, -4724390119873347861}, FieldId&lt;FieldNameEnum::type&gt;{0, -4761221134017270484}&gt; *&gt;{nullptr}&gt;::Get&lt;T&gt;'
3.	&lt;source&gt;:279:52: instantiating function definition 'FieldNameBuilder&lt;FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNameEnum::type&gt;{0, -4724390119873347861}, FieldId&lt;FieldNameEnum::type&gt;{0, -4761221134017270484}&gt; *&gt;{nullptr}&gt;::Get&lt;T, 0, 1&gt;'
 #<!-- -->0 0x00000000041f5e08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f5e08)
 #<!-- -->1 0x00000000041f3234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f3234)
 #<!-- -->2 0x0000000004137498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f94f8842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f94f88969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f94f8842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f94f88287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f94f882871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f94f8839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004531044 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531044)
#<!-- -->10 0x0000000007438c7d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x000000000741db35 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000074346e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007449077 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000742840d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000007453418 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7453418)
#<!-- -->16 0x00000000074a83df clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a83df)
#<!-- -->17 0x000000000750be62 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750be62)
#<!-- -->18 0x000000000750de94 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->19 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#<!-- -->20 0x00000000074a819a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a819a)
#<!-- -->21 0x00000000073b5137 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73b5137)
#<!-- -->22 0x00000000073b64da void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->23 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#<!-- -->24 0x000000000739122b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x739122b)
#<!-- -->25 0x00000000071e4d6d AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#<!-- -->26 0x00000000071e7aa3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7aa3)
#<!-- -->27 0x00000000071e7cd3 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7cd3)
#<!-- -->28 0x000000000720290a clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720290a)
#<!-- -->29 0x0000000006e11fa2 BuildOverloadedBinOp(clang::Sema&amp;, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#<!-- -->30 0x0000000006e67757 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e67757)
#<!-- -->31 0x0000000007443c7f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x000000000741d572 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x000000000745a357 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x000000000745aa0e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->35 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
#<!-- -->36 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
#<!-- -->37 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#<!-- -->38 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
#<!-- -->39 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#<!-- -->40 0x0000000006a90ad2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->41 0x0000000006a90e52 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90e52)
#<!-- -->42 0x00000000068f9fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9fd0)
#<!-- -->43 0x00000000068db57a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db57a)
#<!-- -->44 0x0000000004b3e668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e668)
#<!-- -->45 0x0000000004e2bcf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2bcf5)
#<!-- -->46 0x0000000004dacd8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dacd8e)
#<!-- -->47 0x0000000004f2377d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2377d)
#<!-- -->48 0x0000000000dc3800 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3800)
#<!-- -->49 0x0000000000dba36a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->50 0x0000000000dba4ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->51 0x0000000004ba7819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->52 0x0000000004137934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4137934)
#<!-- -->53 0x0000000004ba7e2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->54 0x0000000004b69142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b69142)
#<!-- -->55 0x0000000004b6a0ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6a0ee)
#<!-- -->56 0x0000000004b71825 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b71825)
#<!-- -->57 0x0000000000dbfd29 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfd29)
#<!-- -->58 0x0000000000c6fab4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6fab4)
#<!-- -->59 0x00007f94f8829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->60 0x00007f94f8829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->61 0x0000000000db9e05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9e05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Alexander Kornienko (alexfh)

<details>
The following (originally valid, hopefully, this wasn't over-reduced) code triggers an assertion failure in Clang (https://gcc.godbolt.org/z/Y1e5Pd7fG):

```
template &lt;int __v&gt; struct integral_constant {
  static constexpr int value = __v;
};
template &lt;bool _Val&gt; using _BoolConstant = integral_constant&lt;_Val&gt;;
template &lt;int, class&gt; struct tuple_element;
template &lt;class...&gt; class tuple;
template &lt;int _Ip, class... _Tp&gt; struct tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt; {
  using type = __type_pack_element&lt;_Ip, _Tp...&gt;;
};
template &lt;class&gt; struct tuple_size;
template &lt;bool&gt; using __enable_if_t = int;
template &lt;template &lt;class&gt; class _BaseType, class _Tp, _Tp _SequenceSize&gt;
using __make_integer_sequence_impl =
    __make_integer_seq&lt;_BaseType, _Tp, _SequenceSize&gt;;
template &lt;class _Tp, _Tp...&gt; struct __integer_sequence;
template &lt;int... _Indices&gt;
using __index_sequence = __integer_sequence&lt;int, _Indices...&gt;;
template &lt;int _SequenceSize&gt;
using __make_index_sequence =
    __make_integer_sequence_impl&lt;__integer_sequence, int, _SequenceSize&gt;;
template &lt;class _Tp, _Tp...&gt; struct integer_sequence {};
template &lt;int... _Ip&gt; using index_sequence = integer_sequence&lt;int, _Ip...&gt;;
template &lt;class _Tp, _Tp _Ep&gt;
using make_integer_sequence =
    __make_integer_sequence_impl&lt;integer_sequence, _Tp, _Ep&gt;;
template &lt;int _Np&gt; using make_index_sequence = make_integer_sequence&lt;int, _Np&gt;;
enum __element_count : int;
constexpr void __constexpr_memmove(char *__dest, const char *__src,
                                   __element_count __n) {
  __builtin_memmove(__dest, __src, __n);
}
template &lt;class _Tp&gt; using __underlying_type_t = __underlying_type(_Tp);
template &lt;class _Tp&gt; using underlying_type_t = __underlying_type_t&lt;_Tp&gt;;
template &lt;class _Tp, class&gt; using __enable_if_tuple_size_imp = _Tp;
template &lt;class _Tp&gt;
struct tuple_size&lt;__enable_if_tuple_size_imp&lt;
    const _Tp, __enable_if_t&lt;_BoolConstant&lt;__is_volatile(int)&gt;::value&gt;&gt;&gt;
    : integral_constant&lt;tuple_size&lt;_Tp&gt;::value&gt; {};
template &lt;class... _Tp&gt;
struct tuple_size&lt;tuple&lt;_Tp...&gt;&gt; : integral_constant&lt;sizeof...(_Tp)&gt; {};
template &lt;class _Tp&gt; constexpr int tuple_size_v = tuple_size&lt;_Tp&gt;::value;
template &lt;class _T1, class _T2&gt; struct pair {
  _T1 first;
  _T2 second;
};
template &lt;class _T1&gt; constexpr pair&lt;_T1, char *&gt; make_pair(_T1, char *__t2) {
  return pair(_T1(), __t2);
}
template &lt;int, class _Hp&gt; struct __tuple_leaf {
  _Hp __value_;
  constexpr const _Hp &amp;get() const { return __value_; }
};
template &lt;class...&gt; struct __tuple_impl;
template &lt;int... _Indx, class... _Tp&gt;
struct __tuple_impl&lt;__index_sequence&lt;_Indx...&gt;, _Tp...&gt;
    : __tuple_leaf&lt;_Indx, _Tp&gt;... {
  template &lt;class... _Args&gt;
  constexpr __tuple_impl(int, _Args... __args)
      : __tuple_leaf&lt;_Indx, _Tp&gt;(__args)... {}
};
template &lt;class... _Tp&gt; struct tuple {
  __tuple_impl&lt;__make_index_sequence&lt;sizeof...(_Tp)&gt;, _Tp...&gt; __base_;
  template &lt;class... _Up&gt; constexpr tuple(_Up... __u) : __base_({}, __u...) {}
};
template &lt;int _Ip, class... _Tp&gt;
constexpr const tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt;::type &amp;
get(const tuple&lt;_Tp...&gt; &amp;__t) noexcept {
  return static_cast&lt;const __tuple_leaf&lt;
      _Ip, typename tuple_element&lt;_Ip, tuple&lt;_Tp...&gt;&gt;::type&gt; &amp;&gt;(__t.__base_)
      .get();
}
template &lt;class... _Tp&gt; constexpr tuple&lt;_Tp...&gt; make_tuple(_Tp... __t) {
  return tuple&lt;_Tp...&gt;(__t...);
}
constexpr int __char_traits_length_checked(const char *__s) {
  return __builtin_strlen(__s);
}
struct basic_string_view {
  constexpr basic_string_view() {}
  constexpr basic_string_view(const char *__s)
      : __data_(__s), __size_(__char_traits_length_checked(__s)) {}
  constexpr const char *begin() { return __data_; }
  constexpr const char *end() {
    return __data_ + __size_;
  }
  const char *__data_;
  int __size_;
};
template &lt;class _Algorithm&gt;
constexpr pair&lt;const char *, char *&gt;
__copy_move_unwrap_iters(const char *__first, const char *__last,
                         char *__out_first) {
  pair&lt;const char *, const char *&gt; __range = {__first, __last};
  auto __result = _Algorithm()(__range.first, __range.second, __out_first);
  return make_pair(__result.first, __result.second);
}
struct __copy_impl {
  constexpr pair&lt;const char *, char *&gt;
  operator()(const char *__first, const char *__last, char *__result) {
    const int __n(__last - __first);
    __constexpr_memmove(__result, __first, __element_count(__n));
    return make_pair(__last, __result);
  }
};
constexpr char *copy(const char *__first, const char *__last, char *__result) {
  return __copy_move_unwrap_iters&lt;__copy_impl&gt;(__first, __last, __result).second;
}
constexpr char *copy_n(const char *__first, int __orig_n, char *__result) {
  return copy(__first, __first + __orig_n, __result);
}
template &lt;int _Size&gt; struct array {
  basic_string_view __elems_[_Size];
  constexpr basic_string_view &amp;operator[](int __n) { return __elems_[__n]; }
  constexpr basic_string_view operator[](int __n) const {
    return __elems_[__n];
  }
};

template &lt;typename&gt; struct FieldId;

template &lt;FieldId field&gt; constexpr auto FieldIdToInnerValue() {
  return field.template ToInnerValue&lt;field&gt;();
}
struct FieldNameEnum {
  enum class type;
};
template &lt;int N&gt; using FieldName = FieldNameEnum::type;
template &lt;typename, auto&gt; struct GetParentMessageAtIndexImpl;
template &lt;typename, auto&gt; struct FieldInfoHelper;
template &lt;FieldId...&gt; struct PathImpl;
template &lt;int N&gt; struct LongPathLiteral {
  consteval LongPathLiteral(const char (&amp;s)[N]) {
    copy_n(s, N, long_path)[N] = field_count = long_path_size = 1;
  }
  consteval basic_string_view to_string_view() const { return long_path; }
  char long_path[N + 1];
  int long_path_size;
  int field_count;
};
template &lt;LongPathLiteral kLongPath&gt; consteval auto get_field_components() {
  basic_string_view long_path(kLongPath.to_string_view());
  array&lt;kLongPath.field_count&gt; ret;
  for (int i = 0; i &lt; kLongPath.field_count; ++i)
    ret[i] = long_path;
  return ret;
}
template &lt;LongPathLiteral kLongPath&gt;
constexpr auto kFieldComponents = get_field_components&lt;kLongPath&gt;();
template &lt;LongPathLiteral kLongPath&gt; struct LongPathHelper {
  template &lt;int... I&gt;
  static PathImpl&lt;kFieldComponents&lt;kLongPath&gt;[I]...&gt;
      PathForLongPath(index_sequence&lt;I...&gt;);
  using type =
      decltype(PathForLongPath(make_index_sequence&lt;kLongPath.field_count&gt;{}));
};
template &lt;typename T&gt; struct PathFieldId {
  template &lt;typename Arg&gt; constexpr PathFieldId(Arg &amp;arg) : value(arg) {}
  T value;
};
template &lt;PathFieldId...&gt; constexpr auto PathImplHelper();

template &lt;int N&gt; using FieldName = FieldName&lt;N&gt;;
enum class FieldNumber;
template &lt;PathFieldId... fields&gt;
constexpr auto Path = PathImplHelper&lt;fields...&gt;();
template &lt;typename Proto, FieldId field&gt;
using FieldInfo =
    FieldInfoHelper&lt;Proto, FieldIdToInnerValue&lt;field&gt;()&gt;::type;
template &lt;&gt; struct FieldId&lt;FieldNameEnum::type&gt; {
  constexpr FieldId(basic_string_view);
  int size;
  long hash;
  template &lt;auto field&gt; static constexpr auto ToInnerValue() {
    return static_cast&lt;FieldNameEnum::type&gt;(field.hash);
  }
};
FieldId(basic_string_view) -&gt; FieldId&lt;FieldNameEnum::type&gt;;
template &lt;typename Proto, FieldId field, int index&gt;
using GetParentMessageAtIndex = GetParentMessageAtIndexImpl&lt;
    Proto, FieldIdToInnerValue&lt;field&gt;()&gt;::type;

template &lt;typename T&gt;
PathFieldId(T &amp;t) -&gt; PathFieldId&lt;decltype(LongPathLiteral(t))&gt;;
template &lt;FieldId... fields1, FieldId... fields2&gt;
constexpr PathImpl&lt;fields1..., fields2...&gt; *ConcatPath(PathImpl&lt;fields1...&gt; *,
                                                       PathImpl&lt;fields2...&gt; *) {
  return nullptr;
}
template &lt;LongPathLiteral long_path_literal&gt;
constexpr LongPathHelper&lt;long_path_literal&gt;::type *SinglePath() {
  return nullptr;
}
template &lt;PathFieldId... fields&gt; constexpr auto PathImplHelper() {
  return ConcatPath(SinglePath&lt;fields.value&gt;()...);
}
template &lt;auto hash_prime, auto offset_bias&gt;
constexpr auto Fnv1a(basic_string_view str) {
  auto hash = offset_bias;
  for (char c : str) {
    hash ^= c;
    hash *= hash_prime;
  }
  return hash;
}
constexpr auto HashField(basic_string_view str) {
  return Fnv1a&lt;1099511628211u, 1039346656037&gt;(str);
}
template &lt;typename FI&gt; struct FieldInfoValueTypeAlias : FI {};
template &lt;typename Proto, auto field&gt; struct FieldInfoHelperBase {
  static constexpr auto MaskFieldNameHash() {
    using FieldEnum = decltype(field);
    return FieldEnum{static_cast&lt;underlying_type_t&lt;FieldEnum&gt;&gt;(field) &amp; 31};
  }
  using internal_type =
      Proto::template FieldInfoImpl&lt;decltype(field), MaskFieldNameHash()&gt;;
};
template &lt;typename Proto, auto field&gt; struct FieldInfoHelper {
  using type = FieldInfoValueTypeAlias&lt;
      typename FieldInfoHelperBase&lt;Proto, field&gt;::internal_type&gt;;
};

template &lt;auto... fields&gt;
struct FieldId&lt;const PathImpl&lt;fields...&gt; *&gt; {
  constexpr FieldId(PathImpl&lt;fields...&gt; *) : path() {}
  template &lt;auto field&gt; static constexpr auto ToInnerValue() {
    return field.path;
  }
  const PathImpl&lt;fields...&gt; *path;
};
template &lt;auto... fields&gt;
FieldId(PathImpl&lt;fields...&gt; *)
    -&gt; FieldId&lt;const PathImpl&lt;fields...&gt; *&gt;;

template &lt;auto&gt; struct UnpackedField {
  static constexpr bool is_path = false;
};
template &lt;auto... fields, const PathImpl&lt;fields...&gt; *path&gt;
struct UnpackedField&lt;path&gt; {
  static constexpr auto value = make_tuple(fields...);
  static constexpr bool is_path = true;
};
template &lt;typename Proto, FieldId... fields, const PathImpl&lt;fields...&gt; *path&gt;
struct GetParentMessageAtIndexImpl&lt;Proto, path&gt; {
  using type = Proto;
};

constexpr FieldId&lt;FieldNameEnum::type&gt;::FieldId(basic_string_view str)
    : size(), hash(HashField(str)) {}
template &lt;FieldId field&gt; constexpr bool IsPath() {
  return UnpackedField&lt;
      FieldIdToInnerValue&lt;field&gt;()&gt;::is_path;
}
template &lt;FieldId field&gt; constexpr auto UnpackFieldToTuple() {
  return UnpackedField&lt;FieldIdToInnerValue&lt;field&gt;()&gt;::value;
}
template &lt;int&gt; struct CompileTimeString {
  consteval CompileTimeString(basic_string_view &amp;v) : internal_view_(v) {}
  basic_string_view &amp;internal_view_;
};
CompileTimeString(basic_string_view) -&gt; CompileTimeString&lt;0&gt;;

template &lt;CompileTimeString... parts&gt; struct NameJoiner {
  template &lt;CompileTimeString... after&gt;
  NameJoiner&lt;parts...&gt; operator+(NameJoiner&lt;after...&gt;);
};
template &lt;FieldId&gt; struct FieldNameBuilder;
template &lt;FieldId field&gt;
  requires(!IsPath&lt;field&gt;())
struct FieldNameBuilder&lt;field&gt; {
  template &lt;typename Proto&gt; static auto Get() {
    return NameJoiner&lt;FieldInfo&lt;Proto, field&gt;::name&gt;();
  }
};
template &lt;FieldId field&gt;
  requires(IsPath&lt;field&gt;())
struct FieldNameBuilder&lt;field&gt; {
  static constexpr auto kTuple = UnpackFieldToTuple&lt;field&gt;();
  static constexpr int kTupleSize = tuple_size_v&lt;decltype(kTuple)&gt;;
  template &lt;typename Proto, int... Is&gt; static void Get(index_sequence&lt;Is...&gt;) {
    (FieldNameBuilder&lt;get&lt;Is&gt;(
         kTuple)&gt;::template Get&lt;GetParentMessageAtIndex&lt;Proto, field, Is&gt;&gt;() +
     ...);
  }
  template &lt;typename Proto&gt; static void Get() {
    Get&lt;Proto&gt;(make_index_sequence&lt;kTupleSize&gt;());
  }
};

struct T {
  template &lt;typename FieldType, FieldType&gt; struct FieldInfoImpl;
};
void AddPathsToFieldMask() {
  FieldNameBuilder&lt;Path&lt;"message_field", "int32_field"&gt;&gt;::Get&lt;T&gt;();
}
template &lt;&gt; struct T::FieldInfoImpl&lt;FieldNumber, FieldNumber{1}&gt; {
  static basic_string_view name;
};
template &lt;&gt;
struct T::FieldInfoImpl&lt;FieldName&lt;1&gt;, FieldName&lt;1&gt;{12}&gt;
    : FieldInfoImpl&lt;FieldNumber, FieldNumber{1}&gt; {};
template &lt;&gt; struct T::FieldInfoImpl&lt;FieldNumber, FieldNumber{10}&gt; {
  static basic_string_view name;
};
template &lt;&gt;
struct T::FieldInfoImpl&lt;FieldName&lt;3&gt;, FieldName&lt;3&gt;{11}&gt;
    : FieldInfoImpl&lt;FieldNumber, FieldNumber{10}&gt; {};
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeBase.h:953: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:284:41: instantiating function definition 'FieldNameBuilder&lt;FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNameEnum::type&gt;{0, -4724390119873347861}, FieldId&lt;FieldNameEnum::type&gt;{0, -4761221134017270484}&gt; *&gt;{nullptr}&gt;::Get&lt;T&gt;'
3.	&lt;source&gt;:279:52: instantiating function definition 'FieldNameBuilder&lt;FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNameEnum::type&gt;{0, -4724390119873347861}, FieldId&lt;FieldNameEnum::type&gt;{0, -4761221134017270484}&gt; *&gt;{nullptr}&gt;::Get&lt;T, 0, 1&gt;'
 #<!-- -->0 0x00000000041f5e08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f5e08)
 #<!-- -->1 0x00000000041f3234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f3234)
 #<!-- -->2 0x0000000004137498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f94f8842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f94f88969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f94f8842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f94f88287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f94f882871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f94f8839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004531044 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531044)
#<!-- -->10 0x0000000007438c7d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x000000000741db35 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000074346e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007449077 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000742840d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000007453418 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7453418)
#<!-- -->16 0x00000000074a83df clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a83df)
#<!-- -->17 0x000000000750be62 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750be62)
#<!-- -->18 0x000000000750de94 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->19 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#<!-- -->20 0x00000000074a819a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a819a)
#<!-- -->21 0x00000000073b5137 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73b5137)
#<!-- -->22 0x00000000073b64da void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->23 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
#<!-- -->24 0x000000000739122b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x739122b)
#<!-- -->25 0x00000000071e4d6d AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#<!-- -->26 0x00000000071e7aa3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7aa3)
#<!-- -->27 0x00000000071e7cd3 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e7cd3)
#<!-- -->28 0x000000000720290a clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720290a)
#<!-- -->29 0x0000000006e11fa2 BuildOverloadedBinOp(clang::Sema&amp;, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#<!-- -->30 0x0000000006e67757 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e67757)
#<!-- -->31 0x0000000007443c7f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x000000000741d572 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x000000000745a357 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x000000000745aa0e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->35 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
#<!-- -->36 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
#<!-- -->37 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#<!-- -->38 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
#<!-- -->39 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
#<!-- -->40 0x0000000006a90ad2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->41 0x0000000006a90e52 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90e52)
#<!-- -->42 0x00000000068f9fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9fd0)
#<!-- -->43 0x00000000068db57a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db57a)
#<!-- -->44 0x0000000004b3e668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e668)
#<!-- -->45 0x0000000004e2bcf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2bcf5)
#<!-- -->46 0x0000000004dacd8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dacd8e)
#<!-- -->47 0x0000000004f2377d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2377d)
#<!-- -->48 0x0000000000dc3800 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3800)
#<!-- -->49 0x0000000000dba36a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->50 0x0000000000dba4ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->51 0x0000000004ba7819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->52 0x0000000004137934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4137934)
#<!-- -->53 0x0000000004ba7e2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->54 0x0000000004b69142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b69142)
#<!-- -->55 0x0000000004b6a0ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6a0ee)
#<!-- -->56 0x0000000004b71825 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b71825)
#<!-- -->57 0x0000000000dbfd29 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfd29)
#<!-- -->58 0x0000000000c6fab4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6fab4)
#<!-- -->59 0x00007f94f8829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->60 0x00007f94f8829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->61 0x0000000000db9e05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9e05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - alexfh

https://github.com/llvm/llvm-project/pull/163243 doesn't seem to fix this.

---

### Comment 4 - alexfh

I've bisected this to https://github.com/llvm/llvm-project/pull/162587. @mizvekov please take a look.

---

