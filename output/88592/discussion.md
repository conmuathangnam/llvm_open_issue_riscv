# After 4b163e343cf, Assertion failed: (result && "no existing substitution for template name"), function mangleExistingSubstitution

**Author:** DimitryAndric
**URL:** https://github.com/llvm/llvm-project/issues/88592

## Body

As reported in https://bugs.freebsd.org/278333, clang 18 asserts on the `audio/noise-suppression-for-voice-lv2` port, with:

```text
Assertion failed: (result && "no existing substitution for template name"), function mangleExistingSubstitution, file /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp, line 6697.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-obj -disable-free -clear-ast-before-backend -main-file-name RnNoiseLadspaPlugin.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -relaxed-aliasing -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -fcoverage-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -sys-header-deps -D rnnoise_ladspa_EXPORTS -D JUCE_CUSTOM_VST3_SDK=1 -D JUCE_CUSTOM_VST3_SDK=1 -D NDEBUG -O3 -std=gnu++14 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ RnNoiseLadspaPlugin-f211a9.cpp
1.      <eof> parser at end of file
2.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:884:30: instantiating function definition 'ladspa::builder<RnNoiseMono>::_instantiate<RnNoiseMono>'
3.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: instantiating function definition 'ladspa::plugin_holder_t<RnNoiseMono>::plugin_holder_t<RnNoiseMono, nullptr>'
4.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: LLVM IR generation of declaration 'ladspa::plugin_holder_t<RnNoiseMono>::plugin_holder_t'
5.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: Mangling declaration 'ladspa::plugin_holder_t<RnNoiseMono>::plugin_holder_t'
 #0 0x0000000005aa5a41 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x0000000005aa3a35 llvm::sys::RunSignalHandlers() /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #2 0x0000000005aa6042 SignalHandler(int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #3 0x000000082adb3440 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x000000082adb29fb thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #5 0x00000008289f62d3 ([vdso]+0x2d3)
 #6 0x000000082f96519a thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #7 0x000000082d641714 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082d6f4cc9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #9 0x000000082d625091 (/lib/libc.so.7+0x9c091)
#10 0x0000000002aae818 (/usr/bin/clang+0x2aae818)
#11 0x0000000002aa9e3f (anonymous namespace)::CXXNameMangler::mangleUnresolvedTypeOrSimpleId(clang::QualType, llvm::StringRef) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#12 0x0000000002aa988f (anonymous namespace)::CXXNameMangler::mangleUnresolvedPrefix(clang::NestedNameSpecifier*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:9
#13 0x0000000002aaa0fe (anonymous namespace)::CXXNameMangler::mangleUnresolvedName(clang::NestedNameSpecifier*, clang::DeclarationName, clang::TemplateArgumentLoc const*, unsigned int, unsigned int) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:18
#14 0x0000000002aa6be3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#15 0x0000000002aac304 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#16 0x0000000002aae8d1 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5994:24
#17 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::QualType) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#18 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2164:5
#19 0x0000000002aa0ccf clang::DependentNameType::getIdentifier() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:5951:12
#20 0x0000000002aa0ccf (anonymous namespace)::CXXNameMangler::mangleType(clang::DependentNameType const*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4296:23
#21 0x0000000002a9cf09 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#22 0x0000000002a9ced9 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#23 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#24 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5983:5
#25 0x0000000002aafbc8 (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector<llvm::StringRef, 4u> const*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1802:3
#26 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::getDepth() const /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:256:14
#27 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::pop((anonymous namespace)::CXXNameMangler::FunctionTypeDepthState) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:279:7
#28 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:23
#29 0x0000000002a9816e llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>::~DenseMap() /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/DenseMap.h:782:23
#30 0x0000000002a9816e (anonymous namespace)::CXXNameMangler::~CXXNameMangler() /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:218:7
#31 0x0000000002a9816e (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#32 0x00000000027a7b4a std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>::__is_long[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1773:29
#33 0x00000000027a7b4a std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>::size[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1177:12
#34 0x00000000027a7b4a std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>::empty[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1217:12
#35 0x00000000027a7b4a isUniqueInternalLinkageDecl(clang::GlobalDecl, clang::CodeGen::CodeGenModule&) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1774:35
#36 0x00000000027a7b4a getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1851:23
#37 0x000000000279fa4a clang::Decl::getKind() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:447:51
#38 0x000000000279fa4a clang::FunctionDecl::classof(clang::Decl const*) /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:3004:62
#39 0x000000000279fa4a llvm::isa_impl<clang::FunctionDecl, clang::NamedDecl, void>::doit(clang::NamedDecl const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#40 0x000000000279fa4a llvm::isa_impl_cl<clang::FunctionDecl, clang::NamedDecl const*>::doit(clang::NamedDecl const*) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#41 0x000000000279fa4a llvm::isa_impl_wrap<clang::FunctionDecl, clang::NamedDecl const*, clang::NamedDecl const*>::doit(clang::NamedDecl const* const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#42 0x000000000279fa4a llvm::isa_impl_wrap<clang::FunctionDecl, clang::NamedDecl const* const, clang::NamedDecl const*>::doit(clang::NamedDecl const* const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#43 0x000000000279fa4a llvm::CastIsPossible<clang::FunctionDecl, clang::NamedDecl const*, void>::isPossible(clang::NamedDecl const* const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#44 0x000000000279fa4a llvm::CastInfo<clang::FunctionDecl, clang::NamedDecl const* const, void>::isPossible(clang::NamedDecl const* const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:509:12
#45 0x000000000279fa4a bool llvm::isa<clang::FunctionDecl, clang::NamedDecl const*>(clang::NamedDecl const* const&) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:549:10
#46 0x000000000279fa4a clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1972:3
#47 0x00000000027b5d39 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:3822:27
#48 0x0000000002ceff84 clang::CXXMethodDecl::getParent() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclCXX.h:2180:46
#49 0x0000000002ceff84 (anonymous namespace)::ItaniumCXXABI::EmitCXXConstructors(clang::CXXConstructorDecl const*) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ItaniumCXXABI.cpp:1661:11
#50 0x000000000315d47c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:73
#51 0x00000000031539b6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:208:7
#52 0x0000000004620d7f clang::Sema::LocalEagerInstantiationScope::perform() /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:10200:22
#53 0x0000000004620d7f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5207:23
#54 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#55 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#56 0x0000000004620da8 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:24
#57 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#58 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#59 0x0000000003c45b1f llvm::TimeTraceScope::~TimeTraceScope() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:155:9
#60 0x0000000003c45b1f clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1089:3
#61 0x0000000003c462ad clang::Sema::ActOnEndOfTranslationUnit() /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1130:9
#62 0x0000000003aeff75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:13
#63 0x0000000003aea32e clang::ParseAST(clang::Sema&, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:162:5
#64 0x000000000341cc7f clang::FrontendAction::Execute() /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1073:10
#65 0x000000000334d28d llvm::Error::getPtr() const /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:276:42
#66 0x000000000334d28d llvm::Error::operator bool() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:239:16
#67 0x000000000334d28d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#68 0x00000000034e7c1c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#69 0x0000000002729621 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:294:15
#70 0x00000000027389ab ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:365:12
#71 0x0000000002737a97 clang_main(int, char**, llvm::ToolContext const&) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:405:12
#72 0x00000000027351ad main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#73 0x000000082d6165ea __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:157:2
Abort trap
```

This regressed with 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 ("Implement mangling rules for C++20 concepts and requires-expressions") by @zygoloid, cc @erichkeane @AaronBallman @rjmccall .

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=gnu++14 RnNoiseLadspaPlugin-min.cpp
template <bool> struct enable_if;
template <class _Tp> struct array {
  _Tp __elems_;
};
typedef void LADSPA_Handle;
typedef struct {
  LADSPA_Handle (*instantiate)(const int *, unsigned long);
} LADSPA_Descriptor;
template <typename> struct has_ctor_1_args {
  static constexpr bool value = 0;
};
template <class T, template <class> class HaveClass>
using en_if_doesnt_have = enable_if<HaveClass<T>::value>;
struct plugin_holder_t {
  template <class _Plugin,
            en_if_doesnt_have<_Plugin, has_ctor_1_args> * = nullptr>
  plugin_holder_t(_Plugin, long) {}
};
template <class> struct builder {
  template <class>
  static LADSPA_Handle _instantiate(const int *, unsigned long s) {
    plugin_holder_t(int(), s);
  }
  static constexpr LADSPA_Descriptor descriptor_for_ladspa{_instantiate<int>};
  static const LADSPA_Descriptor &get_ladspa_descriptor();
};
template <class... Args> struct collection {
  struct caller {
    const LADSPA_Descriptor &(&callback)();
  };
  array<caller> init_callers() {
    return {builder<Args>::get_ladspa_descriptor...};
  }
  array<caller> callers = init_callers();
};
void ladspa_descriptor() { collection<int> get_ladspa_descriptor; }
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-14

Author: Dimitry Andric (DimitryAndric)

<details>
As reported in https://bugs.freebsd.org/278333, clang 18 asserts on the `audio/noise-suppression-for-voice-lv2` port, with:

```text
Assertion failed: (result &amp;&amp; "no existing substitution for template name"), function mangleExistingSubstitution, file /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp, line 6697.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-obj -disable-free -clear-ast-before-backend -main-file-name RnNoiseLadspaPlugin.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -relaxed-aliasing -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -fcoverage-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -sys-header-deps -D rnnoise_ladspa_EXPORTS -D JUCE_CUSTOM_VST3_SDK=1 -D JUCE_CUSTOM_VST3_SDK=1 -D NDEBUG -O3 -std=gnu++14 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ RnNoiseLadspaPlugin-f211a9.cpp
1.      &lt;eof&gt; parser at end of file
2.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:884:30: instantiating function definition 'ladspa::builder&lt;RnNoiseMono&gt;::_instantiate&lt;RnNoiseMono&gt;'
3.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: instantiating function definition 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t&lt;RnNoiseMono, nullptr&gt;'
4.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: LLVM IR generation of declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
5.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: Mangling declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
 #<!-- -->0 0x0000000005aa5a41 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x0000000005aa3a35 llvm::sys::RunSignalHandlers() /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x0000000005aa6042 SignalHandler(int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #<!-- -->3 0x000000082adb3440 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #<!-- -->4 0x000000082adb29fb thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #<!-- -->5 0x00000008289f62d3 ([vdso]+0x2d3)
 #<!-- -->6 0x000000082f96519a thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #<!-- -->7 0x000000082d641714 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #<!-- -->8 0x000000082d6f4cc9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #<!-- -->9 0x000000082d625091 (/lib/libc.so.7+0x9c091)
#<!-- -->10 0x0000000002aae818 (/usr/bin/clang+0x2aae818)
#<!-- -->11 0x0000000002aa9e3f (anonymous namespace)::CXXNameMangler::mangleUnresolvedTypeOrSimpleId(clang::QualType, llvm::StringRef) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#<!-- -->12 0x0000000002aa988f (anonymous namespace)::CXXNameMangler::mangleUnresolvedPrefix(clang::NestedNameSpecifier*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:9
#<!-- -->13 0x0000000002aaa0fe (anonymous namespace)::CXXNameMangler::mangleUnresolvedName(clang::NestedNameSpecifier*, clang::DeclarationName, clang::TemplateArgumentLoc const*, unsigned int, unsigned int) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:18
#<!-- -->14 0x0000000002aa6be3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000002aac304 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->16 0x0000000002aae8d1 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5994:24
#<!-- -->17 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::QualType) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->18 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2164:5
#<!-- -->19 0x0000000002aa0ccf clang::DependentNameType::getIdentifier() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:5951:12
#<!-- -->20 0x0000000002aa0ccf (anonymous namespace)::CXXNameMangler::mangleType(clang::DependentNameType const*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4296:23
#<!-- -->21 0x0000000002a9cf09 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->22 0x0000000002a9ced9 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->23 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->24 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5983:5
#<!-- -->25 0x0000000002aafbc8 (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1802:3
#<!-- -->26 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::getDepth() const /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:256:14
#<!-- -->27 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::pop((anonymous namespace)::CXXNameMangler::FunctionTypeDepthState) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:279:7
#<!-- -->28 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:23
#<!-- -->29 0x0000000002a9816e llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;::~DenseMap() /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/DenseMap.h:782:23
#<!-- -->30 0x0000000002a9816e (anonymous namespace)::CXXNameMangler::~CXXNameMangler() /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:218:7
#<!-- -->31 0x0000000002a9816e (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->32 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::__is_long[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1773:29
#<!-- -->33 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::size[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1177:12
#<!-- -->34 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::empty[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1217:12
#<!-- -->35 0x00000000027a7b4a isUniqueInternalLinkageDecl(clang::GlobalDecl, clang::CodeGen::CodeGenModule&amp;) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1774:35
#<!-- -->36 0x00000000027a7b4a getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1851:23
#<!-- -->37 0x000000000279fa4a clang::Decl::getKind() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:447:51
#<!-- -->38 0x000000000279fa4a clang::FunctionDecl::classof(clang::Decl const*) /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:3004:62
#<!-- -->39 0x000000000279fa4a llvm::isa_impl&lt;clang::FunctionDecl, clang::NamedDecl, void&gt;::doit(clang::NamedDecl const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#<!-- -->40 0x000000000279fa4a llvm::isa_impl_cl&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const*) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#<!-- -->41 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const*, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#<!-- -->42 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const* const, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#<!-- -->43 0x000000000279fa4a llvm::CastIsPossible&lt;clang::FunctionDecl, clang::NamedDecl const*, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#<!-- -->44 0x000000000279fa4a llvm::CastInfo&lt;clang::FunctionDecl, clang::NamedDecl const* const, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:509:12
#<!-- -->45 0x000000000279fa4a bool llvm::isa&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:549:10
#<!-- -->46 0x000000000279fa4a clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1972:3
#<!-- -->47 0x00000000027b5d39 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:3822:27
#<!-- -->48 0x0000000002ceff84 clang::CXXMethodDecl::getParent() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclCXX.h:2180:46
#<!-- -->49 0x0000000002ceff84 (anonymous namespace)::ItaniumCXXABI::EmitCXXConstructors(clang::CXXConstructorDecl const*) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ItaniumCXXABI.cpp:1661:11
#<!-- -->50 0x000000000315d47c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:73
#<!-- -->51 0x00000000031539b6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:208:7
#<!-- -->52 0x0000000004620d7f clang::Sema::LocalEagerInstantiationScope::perform() /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:10200:22
#<!-- -->53 0x0000000004620d7f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5207:23
#<!-- -->54 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->55 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->56 0x0000000004620da8 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:24
#<!-- -->57 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->58 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->59 0x0000000003c45b1f llvm::TimeTraceScope::~TimeTraceScope() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:155:9
#<!-- -->60 0x0000000003c45b1f clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1089:3
#<!-- -->61 0x0000000003c462ad clang::Sema::ActOnEndOfTranslationUnit() /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1130:9
#<!-- -->62 0x0000000003aeff75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:13
#<!-- -->63 0x0000000003aea32e clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:162:5
#<!-- -->64 0x000000000341cc7f clang::FrontendAction::Execute() /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1073:10
#<!-- -->65 0x000000000334d28d llvm::Error::getPtr() const /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:276:42
#<!-- -->66 0x000000000334d28d llvm::Error::operator bool() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:239:16
#<!-- -->67 0x000000000334d28d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#<!-- -->68 0x00000000034e7c1c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#<!-- -->69 0x0000000002729621 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:294:15
#<!-- -->70 0x00000000027389ab ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:365:12
#<!-- -->71 0x0000000002737a97 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:405:12
#<!-- -->72 0x00000000027351ad main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#<!-- -->73 0x000000082d6165ea __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:157:2
Abort trap
```

This regressed with 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 ("Implement mangling rules for C++20 concepts and requires-expressions") by @<!-- -->zygoloid, cc @<!-- -->erichkeane @<!-- -->AaronBallman @<!-- -->rjmccall .

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=gnu++14 RnNoiseLadspaPlugin-min.cpp
template &lt;bool&gt; struct enable_if;
template &lt;class _Tp&gt; struct array {
  _Tp __elems_;
};
typedef void LADSPA_Handle;
typedef struct {
  LADSPA_Handle (*instantiate)(const int *, unsigned long);
} LADSPA_Descriptor;
template &lt;typename&gt; struct has_ctor_1_args {
  static constexpr bool value = 0;
};
template &lt;class T, template &lt;class&gt; class HaveClass&gt;
using en_if_doesnt_have = enable_if&lt;HaveClass&lt;T&gt;::value&gt;;
struct plugin_holder_t {
  template &lt;class _Plugin,
            en_if_doesnt_have&lt;_Plugin, has_ctor_1_args&gt; * = nullptr&gt;
  plugin_holder_t(_Plugin, long) {}
};
template &lt;class&gt; struct builder {
  template &lt;class&gt;
  static LADSPA_Handle _instantiate(const int *, unsigned long s) {
    plugin_holder_t(int(), s);
  }
  static constexpr LADSPA_Descriptor descriptor_for_ladspa{_instantiate&lt;int&gt;};
  static const LADSPA_Descriptor &amp;get_ladspa_descriptor();
};
template &lt;class... Args&gt; struct collection {
  struct caller {
    const LADSPA_Descriptor &amp;(&amp;callback)();
  };
  array&lt;caller&gt; init_callers() {
    return {builder&lt;Args&gt;::get_ladspa_descriptor...};
  }
  array&lt;caller&gt; callers = init_callers();
};
void ladspa_descriptor() { collection&lt;int&gt; get_ladspa_descriptor; }
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: Dimitry Andric (DimitryAndric)

<details>
As reported in https://bugs.freebsd.org/278333, clang 18 asserts on the `audio/noise-suppression-for-voice-lv2` port, with:

```text
Assertion failed: (result &amp;&amp; "no existing substitution for template name"), function mangleExistingSubstitution, file /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp, line 6697.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-obj -disable-free -clear-ast-before-backend -main-file-name RnNoiseLadspaPlugin.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -relaxed-aliasing -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -fcoverage-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -sys-header-deps -D rnnoise_ladspa_EXPORTS -D JUCE_CUSTOM_VST3_SDK=1 -D JUCE_CUSTOM_VST3_SDK=1 -D NDEBUG -O3 -std=gnu++14 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ RnNoiseLadspaPlugin-f211a9.cpp
1.      &lt;eof&gt; parser at end of file
2.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:884:30: instantiating function definition 'ladspa::builder&lt;RnNoiseMono&gt;::_instantiate&lt;RnNoiseMono&gt;'
3.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: instantiating function definition 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t&lt;RnNoiseMono, nullptr&gt;'
4.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: LLVM IR generation of declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
5.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: Mangling declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
 #<!-- -->0 0x0000000005aa5a41 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x0000000005aa3a35 llvm::sys::RunSignalHandlers() /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x0000000005aa6042 SignalHandler(int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #<!-- -->3 0x000000082adb3440 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #<!-- -->4 0x000000082adb29fb thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #<!-- -->5 0x00000008289f62d3 ([vdso]+0x2d3)
 #<!-- -->6 0x000000082f96519a thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #<!-- -->7 0x000000082d641714 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #<!-- -->8 0x000000082d6f4cc9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #<!-- -->9 0x000000082d625091 (/lib/libc.so.7+0x9c091)
#<!-- -->10 0x0000000002aae818 (/usr/bin/clang+0x2aae818)
#<!-- -->11 0x0000000002aa9e3f (anonymous namespace)::CXXNameMangler::mangleUnresolvedTypeOrSimpleId(clang::QualType, llvm::StringRef) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#<!-- -->12 0x0000000002aa988f (anonymous namespace)::CXXNameMangler::mangleUnresolvedPrefix(clang::NestedNameSpecifier*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:9
#<!-- -->13 0x0000000002aaa0fe (anonymous namespace)::CXXNameMangler::mangleUnresolvedName(clang::NestedNameSpecifier*, clang::DeclarationName, clang::TemplateArgumentLoc const*, unsigned int, unsigned int) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:18
#<!-- -->14 0x0000000002aa6be3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000002aac304 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->16 0x0000000002aae8d1 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5994:24
#<!-- -->17 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::QualType) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->18 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2164:5
#<!-- -->19 0x0000000002aa0ccf clang::DependentNameType::getIdentifier() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:5951:12
#<!-- -->20 0x0000000002aa0ccf (anonymous namespace)::CXXNameMangler::mangleType(clang::DependentNameType const*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4296:23
#<!-- -->21 0x0000000002a9cf09 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->22 0x0000000002a9ced9 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->23 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->24 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5983:5
#<!-- -->25 0x0000000002aafbc8 (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1802:3
#<!-- -->26 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::getDepth() const /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:256:14
#<!-- -->27 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::pop((anonymous namespace)::CXXNameMangler::FunctionTypeDepthState) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:279:7
#<!-- -->28 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:23
#<!-- -->29 0x0000000002a9816e llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;::~DenseMap() /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/DenseMap.h:782:23
#<!-- -->30 0x0000000002a9816e (anonymous namespace)::CXXNameMangler::~CXXNameMangler() /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:218:7
#<!-- -->31 0x0000000002a9816e (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->32 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::__is_long[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1773:29
#<!-- -->33 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::size[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1177:12
#<!-- -->34 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::empty[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1217:12
#<!-- -->35 0x00000000027a7b4a isUniqueInternalLinkageDecl(clang::GlobalDecl, clang::CodeGen::CodeGenModule&amp;) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1774:35
#<!-- -->36 0x00000000027a7b4a getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1851:23
#<!-- -->37 0x000000000279fa4a clang::Decl::getKind() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:447:51
#<!-- -->38 0x000000000279fa4a clang::FunctionDecl::classof(clang::Decl const*) /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:3004:62
#<!-- -->39 0x000000000279fa4a llvm::isa_impl&lt;clang::FunctionDecl, clang::NamedDecl, void&gt;::doit(clang::NamedDecl const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#<!-- -->40 0x000000000279fa4a llvm::isa_impl_cl&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const*) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#<!-- -->41 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const*, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#<!-- -->42 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const* const, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#<!-- -->43 0x000000000279fa4a llvm::CastIsPossible&lt;clang::FunctionDecl, clang::NamedDecl const*, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#<!-- -->44 0x000000000279fa4a llvm::CastInfo&lt;clang::FunctionDecl, clang::NamedDecl const* const, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:509:12
#<!-- -->45 0x000000000279fa4a bool llvm::isa&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:549:10
#<!-- -->46 0x000000000279fa4a clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1972:3
#<!-- -->47 0x00000000027b5d39 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:3822:27
#<!-- -->48 0x0000000002ceff84 clang::CXXMethodDecl::getParent() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclCXX.h:2180:46
#<!-- -->49 0x0000000002ceff84 (anonymous namespace)::ItaniumCXXABI::EmitCXXConstructors(clang::CXXConstructorDecl const*) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ItaniumCXXABI.cpp:1661:11
#<!-- -->50 0x000000000315d47c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:73
#<!-- -->51 0x00000000031539b6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:208:7
#<!-- -->52 0x0000000004620d7f clang::Sema::LocalEagerInstantiationScope::perform() /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:10200:22
#<!-- -->53 0x0000000004620d7f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5207:23
#<!-- -->54 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->55 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->56 0x0000000004620da8 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:24
#<!-- -->57 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->58 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->59 0x0000000003c45b1f llvm::TimeTraceScope::~TimeTraceScope() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:155:9
#<!-- -->60 0x0000000003c45b1f clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1089:3
#<!-- -->61 0x0000000003c462ad clang::Sema::ActOnEndOfTranslationUnit() /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1130:9
#<!-- -->62 0x0000000003aeff75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:13
#<!-- -->63 0x0000000003aea32e clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:162:5
#<!-- -->64 0x000000000341cc7f clang::FrontendAction::Execute() /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1073:10
#<!-- -->65 0x000000000334d28d llvm::Error::getPtr() const /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:276:42
#<!-- -->66 0x000000000334d28d llvm::Error::operator bool() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:239:16
#<!-- -->67 0x000000000334d28d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#<!-- -->68 0x00000000034e7c1c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#<!-- -->69 0x0000000002729621 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:294:15
#<!-- -->70 0x00000000027389ab ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:365:12
#<!-- -->71 0x0000000002737a97 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:405:12
#<!-- -->72 0x00000000027351ad main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#<!-- -->73 0x000000082d6165ea __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:157:2
Abort trap
```

This regressed with 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 ("Implement mangling rules for C++20 concepts and requires-expressions") by @<!-- -->zygoloid, cc @<!-- -->erichkeane @<!-- -->AaronBallman @<!-- -->rjmccall .

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=gnu++14 RnNoiseLadspaPlugin-min.cpp
template &lt;bool&gt; struct enable_if;
template &lt;class _Tp&gt; struct array {
  _Tp __elems_;
};
typedef void LADSPA_Handle;
typedef struct {
  LADSPA_Handle (*instantiate)(const int *, unsigned long);
} LADSPA_Descriptor;
template &lt;typename&gt; struct has_ctor_1_args {
  static constexpr bool value = 0;
};
template &lt;class T, template &lt;class&gt; class HaveClass&gt;
using en_if_doesnt_have = enable_if&lt;HaveClass&lt;T&gt;::value&gt;;
struct plugin_holder_t {
  template &lt;class _Plugin,
            en_if_doesnt_have&lt;_Plugin, has_ctor_1_args&gt; * = nullptr&gt;
  plugin_holder_t(_Plugin, long) {}
};
template &lt;class&gt; struct builder {
  template &lt;class&gt;
  static LADSPA_Handle _instantiate(const int *, unsigned long s) {
    plugin_holder_t(int(), s);
  }
  static constexpr LADSPA_Descriptor descriptor_for_ladspa{_instantiate&lt;int&gt;};
  static const LADSPA_Descriptor &amp;get_ladspa_descriptor();
};
template &lt;class... Args&gt; struct collection {
  struct caller {
    const LADSPA_Descriptor &amp;(&amp;callback)();
  };
  array&lt;caller&gt; init_callers() {
    return {builder&lt;Args&gt;::get_ladspa_descriptor...};
  }
  array&lt;caller&gt; callers = init_callers();
};
void ladspa_descriptor() { collection&lt;int&gt; get_ladspa_descriptor; }
```

</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dimitry Andric (DimitryAndric)

<details>
As reported in https://bugs.freebsd.org/278333, clang 18 asserts on the `audio/noise-suppression-for-voice-lv2` port, with:

```text
Assertion failed: (result &amp;&amp; "no existing substitution for template name"), function mangleExistingSubstitution, file /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp, line 6697.
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd15.0 -emit-obj -disable-free -clear-ast-before-backend -main-file-name RnNoiseLadspaPlugin.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -relaxed-aliasing -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -fcoverage-compilation-dir=/wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/.build -sys-header-deps -D rnnoise_ladspa_EXPORTS -D JUCE_CUSTOM_VST3_SDK=1 -D JUCE_CUSTOM_VST3_SDK=1 -D NDEBUG -O3 -std=gnu++14 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ RnNoiseLadspaPlugin-f211a9.cpp
1.      &lt;eof&gt; parser at end of file
2.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:884:30: instantiating function definition 'ladspa::builder&lt;RnNoiseMono&gt;::_instantiate&lt;RnNoiseMono&gt;'
3.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: instantiating function definition 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t&lt;RnNoiseMono, nullptr&gt;'
4.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: LLVM IR generation of declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
5.      /wrkdirs/share/dim/ports/audio/noise-suppression-for-voice-lv2/work/noise-suppression-for-voice-1.03/src/ladspa_plugin/ladspa++.h:823:9: Mangling declaration 'ladspa::plugin_holder_t&lt;RnNoiseMono&gt;::plugin_holder_t'
 #<!-- -->0 0x0000000005aa5a41 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x0000000005aa3a35 llvm::sys::RunSignalHandlers() /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x0000000005aa6042 SignalHandler(int) /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #<!-- -->3 0x000000082adb3440 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #<!-- -->4 0x000000082adb29fb thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #<!-- -->5 0x00000008289f62d3 ([vdso]+0x2d3)
 #<!-- -->6 0x000000082f96519a thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #<!-- -->7 0x000000082d641714 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #<!-- -->8 0x000000082d6f4cc9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #<!-- -->9 0x000000082d625091 (/lib/libc.so.7+0x9c091)
#<!-- -->10 0x0000000002aae818 (/usr/bin/clang+0x2aae818)
#<!-- -->11 0x0000000002aa9e3f (anonymous namespace)::CXXNameMangler::mangleUnresolvedTypeOrSimpleId(clang::QualType, llvm::StringRef) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#<!-- -->12 0x0000000002aa988f (anonymous namespace)::CXXNameMangler::mangleUnresolvedPrefix(clang::NestedNameSpecifier*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:9
#<!-- -->13 0x0000000002aaa0fe (anonymous namespace)::CXXNameMangler::mangleUnresolvedName(clang::NestedNameSpecifier*, clang::DeclarationName, clang::TemplateArgumentLoc const*, unsigned int, unsigned int) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:18
#<!-- -->14 0x0000000002aa6be3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000002aac304 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->16 0x0000000002aae8d1 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5994:24
#<!-- -->17 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::QualType) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->18 0x0000000002ab44d6 (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2164:5
#<!-- -->19 0x0000000002aa0ccf clang::DependentNameType::getIdentifier() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Type.h:5951:12
#<!-- -->20 0x0000000002aa0ccf (anonymous namespace)::CXXNameMangler::mangleType(clang::DependentNameType const*) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4296:23
#<!-- -->21 0x0000000002a9cf09 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->22 0x0000000002a9ced9 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /usr/obj/usr/src/amd64.amd64/lib/clang/libclang/clang/AST/TypeNodes.inc:0:1
#<!-- -->23 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
#<!-- -->24 0x0000000002ab1402 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5983:5
#<!-- -->25 0x0000000002aafbc8 (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*, bool) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1802:3
#<!-- -->26 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::getDepth() const /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:256:14
#<!-- -->27 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::FunctionTypeDepthState::pop((anonymous namespace)::CXXNameMangler::FunctionTypeDepthState) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:279:7
#<!-- -->28 0x0000000002a9b157 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:23
#<!-- -->29 0x0000000002a9816e llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;::~DenseMap() /usr/src/contrib/llvm-project/llvm/include/llvm/ADT/DenseMap.h:782:23
#<!-- -->30 0x0000000002a9816e (anonymous namespace)::CXXNameMangler::~CXXNameMangler() /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:218:7
#<!-- -->31 0x0000000002a9816e (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) /usr/src/contrib/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:0
#<!-- -->32 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::__is_long[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1773:29
#<!-- -->33 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::size[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1177:12
#<!-- -->34 0x00000000027a7b4a std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;::empty[abi:sn180100]() const /usr/obj/usr/src/amd64.amd64/tmp/usr/include/c++/v1/string:1217:12
#<!-- -->35 0x00000000027a7b4a isUniqueInternalLinkageDecl(clang::GlobalDecl, clang::CodeGen::CodeGenModule&amp;) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1774:35
#<!-- -->36 0x00000000027a7b4a getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1851:23
#<!-- -->37 0x000000000279fa4a clang::Decl::getKind() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclBase.h:447:51
#<!-- -->38 0x000000000279fa4a clang::FunctionDecl::classof(clang::Decl const*) /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:3004:62
#<!-- -->39 0x000000000279fa4a llvm::isa_impl&lt;clang::FunctionDecl, clang::NamedDecl, void&gt;::doit(clang::NamedDecl const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#<!-- -->40 0x000000000279fa4a llvm::isa_impl_cl&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const*) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#<!-- -->41 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const*, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#<!-- -->42 0x000000000279fa4a llvm::isa_impl_wrap&lt;clang::FunctionDecl, clang::NamedDecl const* const, clang::NamedDecl const*&gt;::doit(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#<!-- -->43 0x000000000279fa4a llvm::CastIsPossible&lt;clang::FunctionDecl, clang::NamedDecl const*, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#<!-- -->44 0x000000000279fa4a llvm::CastInfo&lt;clang::FunctionDecl, clang::NamedDecl const* const, void&gt;::isPossible(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:509:12
#<!-- -->45 0x000000000279fa4a bool llvm::isa&lt;clang::FunctionDecl, clang::NamedDecl const*&gt;(clang::NamedDecl const* const&amp;) /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Casting.h:549:10
#<!-- -->46 0x000000000279fa4a clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1972:3
#<!-- -->47 0x00000000027b5d39 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:3822:27
#<!-- -->48 0x0000000002ceff84 clang::CXXMethodDecl::getParent() const /usr/src/contrib/llvm-project/clang/include/clang/AST/DeclCXX.h:2180:46
#<!-- -->49 0x0000000002ceff84 (anonymous namespace)::ItaniumCXXABI::EmitCXXConstructors(clang::CXXConstructorDecl const*) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ItaniumCXXABI.cpp:1661:11
#<!-- -->50 0x000000000315d47c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:73
#<!-- -->51 0x00000000031539b6 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/contrib/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:208:7
#<!-- -->52 0x0000000004620d7f clang::Sema::LocalEagerInstantiationScope::perform() /usr/src/contrib/llvm-project/clang/include/clang/Sema/Sema.h:10200:22
#<!-- -->53 0x0000000004620d7f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5207:23
#<!-- -->54 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->55 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->56 0x0000000004620da8 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:0:24
#<!-- -->57 0x0000000004623b52 clang::FunctionDecl::isDefined() const /usr/src/contrib/llvm-project/clang/include/clang/AST/Decl.h:2222:12
#<!-- -->58 0x0000000004623b52 clang::Sema::PerformPendingInstantiations(bool) /usr/src/contrib/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6446:23
#<!-- -->59 0x0000000003c45b1f llvm::TimeTraceScope::~TimeTraceScope() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:155:9
#<!-- -->60 0x0000000003c45b1f clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1089:3
#<!-- -->61 0x0000000003c462ad clang::Sema::ActOnEndOfTranslationUnit() /usr/src/contrib/llvm-project/clang/lib/Sema/Sema.cpp:1130:9
#<!-- -->62 0x0000000003aeff75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/contrib/llvm-project/clang/lib/Parse/Parser.cpp:0:13
#<!-- -->63 0x0000000003aea32e clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/contrib/llvm-project/clang/lib/Parse/ParseAST.cpp:162:5
#<!-- -->64 0x000000000341cc7f clang::FrontendAction::Execute() /usr/src/contrib/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1073:10
#<!-- -->65 0x000000000334d28d llvm::Error::getPtr() const /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:276:42
#<!-- -->66 0x000000000334d28d llvm::Error::operator bool() /usr/src/contrib/llvm-project/llvm/include/llvm/Support/Error.h:239:16
#<!-- -->67 0x000000000334d28d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/contrib/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#<!-- -->68 0x00000000034e7c1c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/contrib/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#<!-- -->69 0x0000000002729621 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/contrib/llvm-project/clang/tools/driver/cc1_main.cpp:294:15
#<!-- -->70 0x00000000027389ab ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:365:12
#<!-- -->71 0x0000000002737a97 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/contrib/llvm-project/clang/tools/driver/driver.cpp:405:12
#<!-- -->72 0x00000000027351ad main /usr/src/usr.bin/clang/clang/clang-driver.cpp:17:10
#<!-- -->73 0x000000082d6165ea __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:157:2
Abort trap
```

This regressed with 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 ("Implement mangling rules for C++20 concepts and requires-expressions") by @<!-- -->zygoloid, cc @<!-- -->erichkeane @<!-- -->AaronBallman @<!-- -->rjmccall .

Minimized test case:

```c++
// clang -cc1 -triple x86_64-- -S -std=gnu++14 RnNoiseLadspaPlugin-min.cpp
template &lt;bool&gt; struct enable_if;
template &lt;class _Tp&gt; struct array {
  _Tp __elems_;
};
typedef void LADSPA_Handle;
typedef struct {
  LADSPA_Handle (*instantiate)(const int *, unsigned long);
} LADSPA_Descriptor;
template &lt;typename&gt; struct has_ctor_1_args {
  static constexpr bool value = 0;
};
template &lt;class T, template &lt;class&gt; class HaveClass&gt;
using en_if_doesnt_have = enable_if&lt;HaveClass&lt;T&gt;::value&gt;;
struct plugin_holder_t {
  template &lt;class _Plugin,
            en_if_doesnt_have&lt;_Plugin, has_ctor_1_args&gt; * = nullptr&gt;
  plugin_holder_t(_Plugin, long) {}
};
template &lt;class&gt; struct builder {
  template &lt;class&gt;
  static LADSPA_Handle _instantiate(const int *, unsigned long s) {
    plugin_holder_t(int(), s);
  }
  static constexpr LADSPA_Descriptor descriptor_for_ladspa{_instantiate&lt;int&gt;};
  static const LADSPA_Descriptor &amp;get_ladspa_descriptor();
};
template &lt;class... Args&gt; struct collection {
  struct caller {
    const LADSPA_Descriptor &amp;(&amp;callback)();
  };
  array&lt;caller&gt; init_callers() {
    return {builder&lt;Args&gt;::get_ladspa_descriptor...};
  }
  array&lt;caller&gt; callers = init_callers();
};
void ladspa_descriptor() { collection&lt;int&gt; get_ladspa_descriptor; }
```

</details>


---

### Comment 4 - DimitryAndric

If there's no movement on this issue, can we revert 4b163e343cf for now?

---

### Comment 5 - cor3ntin

@zygoloid 

---

### Comment 6 - zygoloid

> If there's no movement on this issue, can we revert [4b163e3](https://github.com/llvm/llvm-project/commit/4b163e343cfa54c8d55c9da73c70d58f55ea9df2) for now?

I don't think that would be a good idea -- it would introduce a lot of ABI churn to revert and then later reapply an ABI change we've already shipped.

Can you work around this with `-fclang-abi-compat=17`?

---

### Comment 7 - DimitryAndric

`-fclang-abi-compat=17` makes both the reduced and the original full test case work, so I guess for the FreeBSD port that is a good enough workaround. But it would be nice to have the assertion fixed at some point. :)


---

### Comment 8 - zygoloid

If someone wants to take a look, here's a further reduced testcase:
```c++
template<bool> struct A;

template<typename> struct B {
  static constexpr bool value = 0;
};

template<class T, template <class> class TT>
  using C = A<TT<T>::value>;

template<class T, C<T, B>* = nullptr> void f() {}

template void f<int>();
```

---

### Comment 9 - zygoloid

A clue:
```c++
template<bool> struct A;

template<typename> struct B {
  static constexpr bool value = false;
};

template<
#if BAD
    class T,
#endif
    template <class> class TT>
  using C = A<TT<int>::value>;

template<template<class> class TT,
         C<
#if BAD
           int,
#endif
           TT>* = nullptr> void f() {}

template void f<B>();
```
... crashes with `#define BAD` but not without. This probably suggests some confusion between the template parameters of `C` and the template parameters of one of the other involved templates in mangling, likely because expansion of the alias template has muddled together template parameter references into different templates.

---

### Comment 10 - zyn0217

Seems to have been fixed somewhere in trunk https://gcc.godbolt.org/z/KWvhen5TW

@mizvekov related to any of your patches?

---

### Comment 11 - DimitryAndric

It looks like `llvmorg-20-init-01320-g`7f78f99fe5a fixed this. Would be a good candidate to merge to the 19.x branch, unless the revisions before that are prerequisites?


---

### Comment 12 - mizvekov

That patch is intended to help preserve type sugar, which is optional and not supposed to be required for correct compilation. This deserves greater scrutiny, as it's likely that the bug is still there, but harder to reproduce now.

---

### Comment 13 - DimitryAndric

> This deserves greater scrutiny, as it's likely that the bug is still there, but harder to reproduce now.

Probably; in any case both my original full test case, and the minimized one, are regressed by `llvmorg-18-init-06331-g`4b163e343cf, and 'fixed' by `llvmorg-20-init-01320-g`7f78f99fe5a.

---

### Comment 14 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/132076

---

### Comment 15 - aokblast

I just did some experiment and it shows that the problem fixed at the latest trunk.

Update:
Oh, sorry that I didn't see the fixed patch.

---

