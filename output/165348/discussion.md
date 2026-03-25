# clang 21.1.4: C++23 compilation crash

**Author:** alex-tee
**URL:** https://github.com/llvm/llvm-project/issues/165348
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

On Arch Linux with latest distro-provided clang:

```
[build] PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang++ -DBACKWARD_HAS_DW=1 -DBACKWARD_HAS_UNWIND=1 -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DDEBUG=1 -DHAVE_LITTLE_ENDIAN=1 -DJSON_USE_IMPLICIT_CONVERSIONS=0 -DJUCE_DEBUG=1 -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_PLUGINHOST_AU=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCE_PLUGINHOST_LV2=1 -DJUCE_PLUGINHOST_VST3=1 -DJUCE_USE_MP3AUDIOFORMAT=1 -DJUCE_WEB_BROWSER=0 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_ENABLE_STRICT_MODE_UP_TO=0x060900 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_KEYWORDS -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DQT_TESTCASE_BUILDDIR=\"/home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils\" -DQT_TESTCASE_SOURCEDIR=\"/home/alex/Documents/git/zrythm/tests/unit/utils\" -DQT_TESTLIB_LIB -DQT_WIDGETS_LIB -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FUNCTION=__PRETTY_FUNCTION__ -DTBB_USE_DEBUG -DTEST_WAV_FILE_PATH=\"/home/alex/Documents/git/zrythm/tests/test.wav\" -DTYPE_SAFE_ARITHMETIC_POLICY=1 -DTYPE_SAFE_ENABLE_ASSERTIONS=1 -DTYPE_SAFE_ENABLE_PRECONDITION_CHECKS=1 -DTYPE_SAFE_ENABLE_WRAPPER=1 -D_DEBUG=1 -I/home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/zrythm_utils_unit_tests_autogen/include -I/home/alex/Documents/git/zrythm/src/utils/.. -I/home/alex/Documents/git/zrythm/src -I/home/alex/Documents/git/zrythm/builddir_cmake/src -I/home/alex/Documents/git/zrythm/ext/rubberband -I/home/alex/Documents/git/zrythm/.cache/CPM/juce/4e98/modules -I/home/alex/Documents/git/zrythm/ext/ladspa -I/home/alex/Documents/git/zrythm/ext/kissfft -I/home/alex/Documents/git/zrythm/tests -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googlemock/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googlemock -isystem /home/alex/Documents/git/zrythm/ext/zita-resampler -isystem /home/alex/Documents/git/zrythm/ext/soxr/src -isystem /home/alex/Documents/git/zrythm/.cache/CPM/zstd/e8f3/build/cmake/../../lib -isystem /home/alex/Documents/git/zrythm/.cache/CPM/xxhash/c13d/cmake_unofficial/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/nlohmann_json/26c2/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/magic_enum/2c54/include/magic_enum -isystem /home/alex/Documents/git/zrythm/ext/backward-cpp -isystem /home/alex/Documents/git/zrythm/ext/crill/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/fmt/6173/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/scnlib/f754/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/spdlog/514a/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/external/debug_assert -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/describe/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/mp11/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/stl_interfaces/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/assert/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/config/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/container/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/intrusive/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/move/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/unordered/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/container_hash/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/core/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/static_assert/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/throw_exception/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/predef/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/multi_index/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/bind/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/integer/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/type_traits/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/iterator/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/concept_check/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/preprocessor/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/detail/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/fusion/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/function_types/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/mpl/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/utility/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/io/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/tuple/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/typeof/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/functional/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/function/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/optional/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/smart_ptr/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gsl-lite/1f8f/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap-helpers/c8f1/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap/60c7/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/tbb/4990/src/tbb/../../include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/farbot/c1e9/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/au/235d -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtCore -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/mkspecs/linux-g++ -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtGui -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtWidgets -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuick -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQml -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlIntegration -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtNetwork -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlMeta -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlModels -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlWorkerScript -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtOpenGL -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuickControls2 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtConcurrent -isystem /home/alex/Documents/git/zrythm/ext/moodycamel/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/cpp_httplib/e6b6 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtTest -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -g -std=c++23 -fPIE -pipe -g -O0 -DSCN_REGEX_BACKEND=0 -fPIC -fsanitize-address-use-after-scope -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Wextra -Wformat=2 -Wno-missing-field-initializers -Wno-unused-parameter -Wno-cast-function-type -Wno-attributes -Werror=format-overflow -Werror=format-truncation -Werror=return-local-addr -Werror=alloca -Werror=cast-align -Werror=cast-qual -Werror=ctor-dtor-privacy -Wdeprecated -Werror=format-signedness -Wframe-larger-than=65536 -Werror=multichar -Werror=non-virtual-dtor -Werror=range-loop-construct -Werror=disabled-optimization -Werror=pointer-arith -Werror=enum-conversion -Werror=overlength-strings -Werror=int-to-pointer-cast -Werror=shadow -Werror=address -Werror=undef -Werror=unused -Werror=vla -fstrict-aliasing -Wstrict-aliasing=2 -Werror=strict-aliasing -Werror=strict-overflow -Wstrict-overflow=2 -fstrict-overflow -Werror=init-self -Werror=write-strings -Werror=sign-compare -Werror=float-conversion -Werror=uninitialized -Werror=return-type -Werror=switch -Werror=overflow -Werror=array-bounds -Werror=enum-compare -Werror=misleading-indentation -Werror=int-in-bool-context -Werror=type-limits -Werror=endif-labels -Werror=logical-not-parentheses -Werror=parentheses -Werror=comment -Werror=sizeof-pointer-div -Werror=shift-count-overflow -Werror=free-nonheap-object -Werror=nonnull -Werror=tautological-compare -Werror=unused-result -Werror=inline -Werror=sizeof-array-argument -Werror=odr -Werror=narrowing -Werror=reorder -Werror=sequence-point -Werror=invalid-constexpr -Werror=pessimizing-move -pedantic-errors -Werror=pedantic -Werror=vla-cxx-extension -Werror=sentinel -Werror=return-stack-address -Werror=abstract-final-class -Werror=null-dereference -Werror=defaulted-function-deleted -fsafe-buffer-usage-suggestions -Werror=mismatched-tags -Wperf-constraint-implies-noexcept -Winvalid-pch -Xclang -include-pch -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/cmake_pch.hxx -MD -MT tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o -MF tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o.d -o tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o -c /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx
[build] 1.	<eof> parser at end of file
[build] 2.	/home/alex/Documents/git/zrythm/src/utils/../utils/variant_helpers.h:198:30: instantiating function definition 'QVariantToStdVariant<std::variant<int *, QString *>>::convert'
[build] 3.	/home/alex/Documents/git/zrythm/src/utils/../utils/traits.h:104:3: LLVM IR generation of declaration '(anonymous class)::operator()'
[build] 4.	/home/alex/Documents/git/zrythm/src/utils/../utils/traits.h:104:3: Mangling declaration '(anonymous class)::operator()'
[build]  #0 0x00007fce8d9082ad llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Unix/Signals.inc:834:22
[build]  #1 0x00007fce8d905d87 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Signals.cpp:104:20
[build]  #2 0x00007fce8d905d87 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Unix/Signals.inc:373:31
[build]  #3 0x00007fce8d7da779 HandleCrash /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:73:5
[build]  #4 0x00007fce8d7da779 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:390:62
[build]  #5 0x00007fce8c83e540 (/usr/lib/libc.so.6+0x3e540)
[build]  #6 0x00007fce97759082 clang::NamedDecl::getDeclName() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:339:48
[build]  #7 0x00007fce97759082 mangleUnqualifiedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:1634:38
[build]  #8 0x00007fce97759b97 mangleUnqualifiedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:510:26
[build]  #9 0x00007fce97759b97 manglePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2262:26
[build] #10 0x00007fce97759b6d manglePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2262:27
[build] #11 0x00007fce97759c8e mangleTemplatePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2317:38
[build] #12 0x00007fce97759fb6 mangleNestedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:1829:23
[build] #13 0x00007fce97754032 pop /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:277:12
[build] #14 0x00007fce97754032 mangleFunctionEncoding /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:852:26
[build] #15 0x00007fce977573f6 mangleCXXName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:7309:3
[build] #16 0x00007fce98819ade isUniqueInternalLinkageDecl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2001:67
[build] #17 0x00007fce98819ade getMangledNameImpl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2048:50
[build] #18 0x00007fce9881a656 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_Alloc_hider::_Alloc_hider(char*, std::allocator<char>&&) /usr/include/c++/15.2.1/bits/basic_string.h:209:36
[build] #19 0x00007fce9881a656 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&) /usr/include/c++/15.2.1/bits/basic_string.h:614:9
[build] #20 0x00007fce9881a656 std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, clang::GlobalDecl>::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, clang::GlobalDecl&, true>(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, clang::GlobalDecl&) /usr/include/c++/15.2.1/bits/stl_pair.h:902:4
[build] #21 0x00007fce9881a656 std::pair<std::__strip_reference_wrapper<std::decay<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&>::type>::__type, std::__strip_reference_wrapper<std::decay<clang::GlobalDecl&>::type>::__type> std::make_pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, clang::GlobalDecl&>(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, clang::GlobalDecl&) /usr/include/c++/15.2.1/bits/stl_pair.h:1169:72
[build] #22 0x00007fce9881a656 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2205:48
[build] #23 0x00007fce9882dbb9 clang::CodeGen::CodeGenModule::GetGlobalValue(llvm::StringRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2241:35
[build] #24 0x00007fce9882dbb9 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:4139:21
[build] #25 0x00007fce988632d9 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:7213:37
[build] #26 0x00007fce988d0e71 HandleTopLevelDecl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/ModuleBuilder.cpp:189:22
[build] #27 0x00007fce987e87bc clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenAction.cpp:172:22
[build] #28 0x00007fce987e87bc clang::BackendConsumer::HandleInterestingDecl(clang::DeclGroupRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenAction.cpp:194:23
[build] #29 0x00007fce98eaab0e clang::ASTReader::PassInterestingDeclsToConsumer() /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReaderDecl.cpp:4343:36
[build] #30 0x00007fce98e48f4d clang::ASTReader::FinishedDeserializing() /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReader.cpp:10906:3
[build] #31 0x00007fce98e48f4d clang::ExternalASTSource::Deserializing::~Deserializing() /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/ExternalASTSource.h:89:36
[build] #32 0x00007fce98e48f4d clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseMapPair<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable>>&, clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReader.cpp:8413:1
[build] #33 0x00007fce98e49048 non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) /usr/src/debug/clang/clang-21.1.4.src/include/clang/Serialization/ASTReader.h:2186:3
[build] #34 0x00007fce9731c1e7 clang::RedeclarableTemplateDecl::SpecEntryTraits<clang::VarTemplateSpecializationDecl>::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl<clang::VarTemplateSpecializationDecl, llvm::ArrayRef<clang::TemplateArgument>>(llvm::FoldingSetVector<clang::VarTemplateSpecializationDecl, llvm::SmallVector<clang::VarTemplateSpecializationDecl*, 8u>>&, void*&, llvm::ArrayRef<clang::TemplateArgument>) /usr/src/debug/clang/clang-21.1.4.src/lib/AST/DeclTemplate.cpp:401:3
[build] #35 0x00007fce9731c1e7 clang::VarTemplateDecl::findSpecialization(llvm::ArrayRef<clang::TemplateArgument>, void*&) /usr/src/debug/clang/clang-21.1.4.src/lib/AST/DeclTemplate.cpp:1338:32
[build] #36 0x00007fce98264fcc clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4522:39
[build] #37 0x00007fce98267866 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4682:3
[build] #38 0x00007fce98267866 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4798:40
[build] #39 0x00007fce983bfbb9 RebuildTemplateIdExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:3548:41
[build] #40 0x00007fce983bfbb9 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:14840:44
[build] #41 0x00007fce97cb7870 llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #42 0x00007fce97cb7870 ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #43 0x00007fce97cb7870 ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #44 0x00007fce97cb7870 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4488:1
[build] #45 0x00007fce97cb7870 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4480:1
[build] #46 0x00007fce97cb7870 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4506:19
[build] #47 0x00007fce97cb7870 EvaluateAtomicConstraint /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:236:30
[build] #48 0x00007fce97cb7870 calculateConstraintSatisfaction /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:494:62
[build] #49 0x00007fce97cb92ca CheckConstraintSatisfaction /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:595:5
[build] #50 0x00007fce97cb9886 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:654:3
[build] #51 0x00007fce97cbc448 llvm::SmallVectorTemplateCommon<clang::Expr*, void>::isSmall() const /usr/include/llvm/ADT/SmallVector.h:144:39
[build] #52 0x00007fce97cbc448 llvm::SmallVectorImpl<clang::Expr*>::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:604:23
[build] #53 0x00007fce97cbc448 llvm::SmallVector<clang::Expr*, 4u>::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1204:3
[build] #54 0x00007fce97cbc448 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Sema.h:14667:3
[build] #55 0x00007fce97cbc448 CheckFunctionConstraintsWithoutInstantiation /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:1100:45
[build] #56 0x00007fce97cbc448 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:1112:58
[build] #57 0x00007fce98313a0c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateDeduction.cpp:3945:5
[build] #58 0x00007fce98314825 callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool(llvm::ArrayRef<clang::QualType>, bool)>)::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:47:40
[build] #59 0x00007fce96d7cd72 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /usr/src/debug/clang/clang-21.1.4.src/lib/Basic/StackExhaustionHandler.cpp:21:1
[build] #60 0x00007fce98322463 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateDeduction.cpp:4695:10
[build] #61 0x00007fce981cdf72 AddTemplateOverloadCandidateImmediately /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:8010:3
[build] #62 0x00007fce981ce801 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:8092:5
[build] #63 0x00007fce981cfcc1 AddOverloadedCallCandidate /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14248:5
[build] #64 0x00007fce981cfcc1 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14298:31
[build] #65 0x00007fce981da62b clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14606:21
[build] #66 0x00007fce981ebe59 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14816:3
[build] #67 0x00007fce97eb23dc clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaExpr.cpp:6661:66
[build] #68 0x00007fce97eb31d1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaExpr.cpp:6517:3
[build] #69 0x00007fce983a5256 clang::Sema::FPFeaturesStateRAII::~FPFeaturesStateRAII() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:2905:19
[build] #70 0x00007fce983a5256 TransformCallExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:13434:1
[build] #71 0x00007fce983a716e TransformCXXFoldExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:16444:5
[build] #72 0x00007fce983a166f llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #73 0x00007fce983a166f ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #74 0x00007fce983a166f ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #75 0x00007fce983a166f clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4526:1
[build] #76 0x00007fce983e2c3c clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6128:5
[build] #77 0x00007fce983f665f clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6079:35
[build] #78 0x00007fce983f6bb2 clang::Decl::getKind() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/DeclBase.h:445:51
[build] #79 0x00007fce983f6bb2 clang::ParmVarDecl::classof(clang::Decl const*) /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:1931:58
[build] #80 0x00007fce983f6bb2 llvm::isa_impl<clang::ParmVarDecl, clang::VarDecl, void>::doit(clang::VarDecl const&) /usr/include/llvm/Support/Casting.h:64:64
[build] #81 0x00007fce983f6bb2 llvm::isa_impl_cl<clang::ParmVarDecl, clang::VarDecl const*>::doit(clang::VarDecl const*) /usr/include/llvm/Support/Casting.h:110:36
[build] #82 0x00007fce983f6bb2 llvm::isa_impl_wrap<clang::ParmVarDecl, clang::VarDecl const*, clang::VarDecl const*>::doit(clang::VarDecl const* const&) /usr/include/llvm/Support/Casting.h:137:41
[build] #83 0x00007fce983f6bb2 llvm::isa_impl_wrap<clang::ParmVarDecl, clang::VarDecl const* const, clang::VarDecl const*>::doit(clang::VarDecl const* const&) /usr/include/llvm/Support/Casting.h:129:13
[build] #84 0x00007fce983f6bb2 llvm::CastIsPossible<clang::ParmVarDecl, clang::VarDecl const*, void>::isPossible(clang::VarDecl const* const&) /usr/include/llvm/Support/Casting.h:257:62
[build] #85 0x00007fce983f6bb2 llvm::CastInfo<clang::ParmVarDecl, clang::VarDecl const* const, void>::isPossible(clang::VarDecl const* const&) /usr/include/llvm/Support/Casting.h:509:38
[build] #86 0x00007fce983f6bb2 bool llvm::isa<clang::ParmVarDecl, clang::VarDecl const*>(clang::VarDecl const* const&) /usr/include/llvm/Support/Casting.h:549:46
[build] #87 0x00007fce983f6bb2 clang::VarDecl::isNRVOVariable() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:1492:28
[build] #88 0x00007fce983f6bb2 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:1692:24
[build] #89 0x00007fce983b8ba0 operator() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:4646:12
[build] #90 0x00007fce983b8ba0 callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, const clang::MultiLevelTemplateArgumentList&)::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
[build] #91 0x00007fce983ba240 llvm::SmallVectorTemplateCommon<std::pair<clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*>, void>::isSmall() const /usr/include/llvm/ADT/SmallVector.h:144:39
[build] #92 0x00007fce983ba240 llvm::SmallVectorImpl<std::pair<clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*>>::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:604:23
[build] #93 0x00007fce983ba240 llvm::SmallVector<std::pair<clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*>, 1u>::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1204:3
[build] #94 0x00007fce983ba240 clang::TemplateDeclInstantiator::~TemplateDeclInstantiator() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Template.h:574:9
[build] #95 0x00007fce983ba240 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:4649:1
[build] #96 0x00007fce983bb4f4 TransformDefinition /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:2007:35
[build] #97 0x00007fce983bb4f4 TransformDeclStmt /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:8579:57
[build] #98 0x00007fce983be9e8 TransformCompoundStmt /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:8124:5
[build] #99 0x00007fce98393393 llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #100 0x00007fce98393393 ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #101 0x00007fce98393393 ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #102 0x00007fce98393393 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4459:1
[build] #103 0x00007fce98393393 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4451:1
[build] #104 0x00007fce983ed082 clang::ActionResult<clang::Stmt*, true>::get() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Ownership.h:204:47
[build] #105 0x00007fce983ed082 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5854:28
[build] #106 0x00007fce983fa2d2 clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:2257:21
[build] #107 0x00007fce983fa2d2 clang::Sema::PerformPendingInstantiations(bool, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:7105:32
[build] #108 0x00007fce97b9b1a4 llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:200:5
[build] #109 0x00007fce97b9b1a4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:1223:3
[build] #110 0x00007fce97b9b501 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:1252:9
[build] #111 0x00007fce97083ad7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang/clang-21.1.4.src/lib/Parse/Parser.cpp:714:12
[build] #112 0x00007fce96f71589 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Parse/ParseAST.cpp:169:69
[build] #113 0x00007fce9904e85c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/CompilerInstance.cpp:86:58
[build] #114 0x00007fce9904e85c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/FrontendAction.cpp:1225:38
[build] #115 0x00007fce98fafccf llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
[build] #116 0x00007fce98fafccf llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
[build] #117 0x00007fce98fafccf clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/CompilerInstance.cpp:1055:42
[build] #118 0x00007fce990abaac std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1673:16
[build] #119 0x00007fce990abaac std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1370:69
[build] #120 0x00007fce990abaac std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1358:40
[build] #121 0x00007fce990abaac clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Frontend/CompilerInvocation.h:259:48
[build] #122 0x00007fce990abaac clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Frontend/CompilerInstance.h:304:39
[build] #123 0x00007fce990abaac clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-21.1.4.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
[build] #124 0x000055c6bf43ae9a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-21.1.4.src/tools/driver/cc1_main.cpp:297:40
[build] #125 0x000055c6bf43eb61 ExecuteCC1Tool /usr/src/debug/clang/clang-21.1.4.src/tools/driver/driver.cpp:223:20
[build] #126 0x00007fce98bf3035 operator() /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Job.cpp:436:32
[build] #127 0x00007fce98bf3035 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
[build] #128 0x00007fce8d7dabb8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:428:1
[build] #129 0x00007fce98bfebf5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Job.cpp:440:10
[build] #130 0x00007fce98c1204b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Compilation.cpp:196:22
[build] #131 0x00007fce98c1244c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Compilation.cpp:251:5
[build] #132 0x00007fce98c24c25 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:82:46
[build] #133 0x00007fce98c24c25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Driver.cpp:2246:28
[build] #134 0x000055c6bf440b85 llvm::SmallVectorBase<unsigned int>::size() const /usr/include/llvm/ADT/SmallVector.h:79:32
[build] #135 0x000055c6bf440b85 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/include/llvm/ADT/SmallVector.h:270:41
[build] #136 0x000055c6bf440b85 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-21.1.4.src/tools/driver/driver.cpp:408:26
[build] #137 0x000055c6bf4322b0 main /usr/src/debug/clang/clang-21.1.4.src/build/tools/driver/clang-driver.cpp:18:1
[build] #138 0x00007fce8c827675 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #139 0x00007fce8c827729 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
[build] #140 0x00007fce8c827729 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
[build] #141 0x000055c6bf432315 _start (/usr/bin/clang+++0xb315)
[build] clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
[build] clang version 21.1.4
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: /tmp/unity_3_cxx-7aaa8d.cpp
[build] clang++: note: diagnostic msg: /tmp/unity_3_cxx-7aaa8d.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
```

Possibly related code:
```cpp

// Helper to check if QVariant can convert to type T
template <typename T>
concept QVariantConvertible = requires (const QVariant &qvar) {
  { qvar.template value<T> () } -> std::same_as<T>;
};

// Primary template (undefined)
template <typename Variant> struct QVariantToStdVariant;

// Specialization for std::variant
template <typename... Ts> struct QVariantToStdVariant<std::variant<Ts...>>
{
  static std::variant<Ts...> convert (const QVariant &qvar)
  {
    std::variant<Ts...> result;
    bool                success = (tryConvert<Ts> (qvar, result) || ...);
    if (!success)
      {
        throw std::runtime_error ("QVariant type not supported in std::variant");
      }
    return result;
  }

private:
  // Non-pointer specialization
  template <typename T>
  static bool tryConvert (const QVariant &qvar, std::variant<Ts...> &out)
  {
    if constexpr (QVariantConvertible<T>)
      {
        if (qvar.userType () == QMetaType::fromType<T> ().id ())
          {
            out = qvar.template value<T> ();
            return true;
          }
      }
    return false;
  }

  // Pointer specialization
  template <typename T>
  static bool tryConvert (const QVariant &qvar, std::variant<Ts...> &out)
    requires (std::is_pointer_v<T>)
  {
    if constexpr (QVariantConvertible<T>)
      {
        if (qvar.template canConvert<T> ())
          {
            auto ptr = qvar.value<T> ();
            if (ptr != nullptr)
              {
                out = ptr;
                return true;
              }
          }
      }
    return false;
  }
};

// Helper function for direct usage
template <typename Variant>
auto
qvariantToStdVariant (const QVariant &qvar)
{
  return QVariantToStdVariant<Variant>::convert (qvar);
}
```

[unity_3_cxx-7aaa8d.cpp](https://github.com/user-attachments/files/23178788/unity_3_cxx-7aaa8d.cpp)
[unity_3_cxx-7aaa8d.sh](https://github.com/user-attachments/files/23178787/unity_3_cxx-7aaa8d.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (alex-tee)

<details>
On Arch Linux with latest distro-provided clang:

```
[build] PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang++ -DBACKWARD_HAS_DW=1 -DBACKWARD_HAS_UNWIND=1 -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DDEBUG=1 -DHAVE_LITTLE_ENDIAN=1 -DJSON_USE_IMPLICIT_CONVERSIONS=0 -DJUCE_DEBUG=1 -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_PLUGINHOST_AU=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCE_PLUGINHOST_LV2=1 -DJUCE_PLUGINHOST_VST3=1 -DJUCE_USE_MP3AUDIOFORMAT=1 -DJUCE_WEB_BROWSER=0 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_ENABLE_STRICT_MODE_UP_TO=0x060900 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_KEYWORDS -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DQT_TESTCASE_BUILDDIR=\"/home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils\" -DQT_TESTCASE_SOURCEDIR=\"/home/alex/Documents/git/zrythm/tests/unit/utils\" -DQT_TESTLIB_LIB -DQT_WIDGETS_LIB -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FUNCTION=__PRETTY_FUNCTION__ -DTBB_USE_DEBUG -DTEST_WAV_FILE_PATH=\"/home/alex/Documents/git/zrythm/tests/test.wav\" -DTYPE_SAFE_ARITHMETIC_POLICY=1 -DTYPE_SAFE_ENABLE_ASSERTIONS=1 -DTYPE_SAFE_ENABLE_PRECONDITION_CHECKS=1 -DTYPE_SAFE_ENABLE_WRAPPER=1 -D_DEBUG=1 -I/home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/zrythm_utils_unit_tests_autogen/include -I/home/alex/Documents/git/zrythm/src/utils/.. -I/home/alex/Documents/git/zrythm/src -I/home/alex/Documents/git/zrythm/builddir_cmake/src -I/home/alex/Documents/git/zrythm/ext/rubberband -I/home/alex/Documents/git/zrythm/.cache/CPM/juce/4e98/modules -I/home/alex/Documents/git/zrythm/ext/ladspa -I/home/alex/Documents/git/zrythm/ext/kissfft -I/home/alex/Documents/git/zrythm/tests -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googlemock/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googlemock -isystem /home/alex/Documents/git/zrythm/ext/zita-resampler -isystem /home/alex/Documents/git/zrythm/ext/soxr/src -isystem /home/alex/Documents/git/zrythm/.cache/CPM/zstd/e8f3/build/cmake/../../lib -isystem /home/alex/Documents/git/zrythm/.cache/CPM/xxhash/c13d/cmake_unofficial/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/nlohmann_json/26c2/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/magic_enum/2c54/include/magic_enum -isystem /home/alex/Documents/git/zrythm/ext/backward-cpp -isystem /home/alex/Documents/git/zrythm/ext/crill/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/fmt/6173/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/scnlib/f754/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/spdlog/514a/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/external/debug_assert -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/describe/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/mp11/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/stl_interfaces/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/assert/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/config/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/container/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/intrusive/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/move/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/unordered/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/container_hash/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/core/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/static_assert/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/throw_exception/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/predef/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/multi_index/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/bind/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/integer/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/type_traits/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/iterator/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/concept_check/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/preprocessor/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/detail/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/fusion/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/function_types/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/mpl/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/utility/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/io/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/tuple/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/typeof/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/functional/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/function/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/optional/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/boost/90cc/libs/smart_ptr/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gsl-lite/1f8f/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap-helpers/c8f1/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap/60c7/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/tbb/4990/src/tbb/../../include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/farbot/c1e9/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/au/235d -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtCore -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/mkspecs/linux-g++ -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtGui -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtWidgets -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuick -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQml -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlIntegration -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtNetwork -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlMeta -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlModels -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlWorkerScript -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtOpenGL -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuickControls2 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtConcurrent -isystem /home/alex/Documents/git/zrythm/ext/moodycamel/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/cpp_httplib/e6b6 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtTest -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -g -std=c++23 -fPIE -pipe -g -O0 -DSCN_REGEX_BACKEND=0 -fPIC -fsanitize-address-use-after-scope -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Wextra -Wformat=2 -Wno-missing-field-initializers -Wno-unused-parameter -Wno-cast-function-type -Wno-attributes -Werror=format-overflow -Werror=format-truncation -Werror=return-local-addr -Werror=alloca -Werror=cast-align -Werror=cast-qual -Werror=ctor-dtor-privacy -Wdeprecated -Werror=format-signedness -Wframe-larger-than=65536 -Werror=multichar -Werror=non-virtual-dtor -Werror=range-loop-construct -Werror=disabled-optimization -Werror=pointer-arith -Werror=enum-conversion -Werror=overlength-strings -Werror=int-to-pointer-cast -Werror=shadow -Werror=address -Werror=undef -Werror=unused -Werror=vla -fstrict-aliasing -Wstrict-aliasing=2 -Werror=strict-aliasing -Werror=strict-overflow -Wstrict-overflow=2 -fstrict-overflow -Werror=init-self -Werror=write-strings -Werror=sign-compare -Werror=float-conversion -Werror=uninitialized -Werror=return-type -Werror=switch -Werror=overflow -Werror=array-bounds -Werror=enum-compare -Werror=misleading-indentation -Werror=int-in-bool-context -Werror=type-limits -Werror=endif-labels -Werror=logical-not-parentheses -Werror=parentheses -Werror=comment -Werror=sizeof-pointer-div -Werror=shift-count-overflow -Werror=free-nonheap-object -Werror=nonnull -Werror=tautological-compare -Werror=unused-result -Werror=inline -Werror=sizeof-array-argument -Werror=odr -Werror=narrowing -Werror=reorder -Werror=sequence-point -Werror=invalid-constexpr -Werror=pessimizing-move -pedantic-errors -Werror=pedantic -Werror=vla-cxx-extension -Werror=sentinel -Werror=return-stack-address -Werror=abstract-final-class -Werror=null-dereference -Werror=defaulted-function-deleted -fsafe-buffer-usage-suggestions -Werror=mismatched-tags -Wperf-constraint-implies-noexcept -Winvalid-pch -Xclang -include-pch -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/cmake_pch.hxx -MD -MT tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o -MF tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o.d -o tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx.o -c /home/alex/Documents/git/zrythm/builddir_cmake/tests/unit/utils/CMakeFiles/zrythm_utils_unit_tests.dir/Unity/unity_3_cxx.cxx
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	/home/alex/Documents/git/zrythm/src/utils/../utils/variant_helpers.h:198:30: instantiating function definition 'QVariantToStdVariant&lt;std::variant&lt;int *, QString *&gt;&gt;::convert'
[build] 3.	/home/alex/Documents/git/zrythm/src/utils/../utils/traits.h:104:3: LLVM IR generation of declaration '(anonymous class)::operator()'
[build] 4.	/home/alex/Documents/git/zrythm/src/utils/../utils/traits.h:104:3: Mangling declaration '(anonymous class)::operator()'
[build]  #<!-- -->0 0x00007fce8d9082ad llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Unix/Signals.inc:834:22
[build]  #<!-- -->1 0x00007fce8d905d87 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Signals.cpp:104:20
[build]  #<!-- -->2 0x00007fce8d905d87 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/Unix/Signals.inc:373:31
[build]  #<!-- -->3 0x00007fce8d7da779 HandleCrash /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:73:5
[build]  #<!-- -->4 0x00007fce8d7da779 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:390:62
[build]  #<!-- -->5 0x00007fce8c83e540 (/usr/lib/libc.so.6+0x3e540)
[build]  #<!-- -->6 0x00007fce97759082 clang::NamedDecl::getDeclName() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:339:48
[build]  #<!-- -->7 0x00007fce97759082 mangleUnqualifiedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:1634:38
[build]  #<!-- -->8 0x00007fce97759b97 mangleUnqualifiedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:510:26
[build]  #<!-- -->9 0x00007fce97759b97 manglePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2262:26
[build] #<!-- -->10 0x00007fce97759b6d manglePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2262:27
[build] #<!-- -->11 0x00007fce97759c8e mangleTemplatePrefix /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:2317:38
[build] #<!-- -->12 0x00007fce97759fb6 mangleNestedName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:1829:23
[build] #<!-- -->13 0x00007fce97754032 pop /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:277:12
[build] #<!-- -->14 0x00007fce97754032 mangleFunctionEncoding /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:852:26
[build] #<!-- -->15 0x00007fce977573f6 mangleCXXName /usr/src/debug/clang/clang-21.1.4.src/lib/AST/ItaniumMangle.cpp:7309:3
[build] #<!-- -->16 0x00007fce98819ade isUniqueInternalLinkageDecl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2001:67
[build] #<!-- -->17 0x00007fce98819ade getMangledNameImpl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2048:50
[build] #<!-- -->18 0x00007fce9881a656 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_Alloc_hider::_Alloc_hider(char*, std::allocator&lt;char&gt;&amp;&amp;) /usr/include/c++/15.2.1/bits/basic_string.h:209:36
[build] #<!-- -->19 0x00007fce9881a656 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::basic_string(std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt; const&amp;) /usr/include/c++/15.2.1/bits/basic_string.h:614:9
[build] #<!-- -->20 0x00007fce9881a656 std::pair&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;, clang::GlobalDecl&gt;::pair&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;, clang::GlobalDecl&amp;, true&gt;(std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;, clang::GlobalDecl&amp;) /usr/include/c++/15.2.1/bits/stl_pair.h:902:4
[build] #<!-- -->21 0x00007fce9881a656 std::pair&lt;std::__strip_reference_wrapper&lt;std::decay&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;&gt;::type&gt;::__type, std::__strip_reference_wrapper&lt;std::decay&lt;clang::GlobalDecl&amp;&gt;::type&gt;::__type&gt; std::make_pair&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;, clang::GlobalDecl&amp;&gt;(std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;, clang::GlobalDecl&amp;) /usr/include/c++/15.2.1/bits/stl_pair.h:1169:72
[build] #<!-- -->22 0x00007fce9881a656 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2205:48
[build] #<!-- -->23 0x00007fce9882dbb9 clang::CodeGen::CodeGenModule::GetGlobalValue(llvm::StringRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:2241:35
[build] #<!-- -->24 0x00007fce9882dbb9 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:4139:21
[build] #<!-- -->25 0x00007fce988632d9 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenModule.cpp:7213:37
[build] #<!-- -->26 0x00007fce988d0e71 HandleTopLevelDecl /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/ModuleBuilder.cpp:189:22
[build] #<!-- -->27 0x00007fce987e87bc clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenAction.cpp:172:22
[build] #<!-- -->28 0x00007fce987e87bc clang::BackendConsumer::HandleInterestingDecl(clang::DeclGroupRef) /usr/src/debug/clang/clang-21.1.4.src/lib/CodeGen/CodeGenAction.cpp:194:23
[build] #<!-- -->29 0x00007fce98eaab0e clang::ASTReader::PassInterestingDeclsToConsumer() /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReaderDecl.cpp:4343:36
[build] #<!-- -->30 0x00007fce98e48f4d clang::ASTReader::FinishedDeserializing() /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReader.cpp:10906:3
[build] #<!-- -->31 0x00007fce98e48f4d clang::ExternalASTSource::Deserializing::~Deserializing() /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/ExternalASTSource.h:89:36
[build] #<!-- -->32 0x00007fce98e48f4d clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/debug/clang/clang-21.1.4.src/lib/Serialization/ASTReader.cpp:8413:1
[build] #<!-- -->33 0x00007fce98e49048 non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/debug/clang/clang-21.1.4.src/include/clang/Serialization/ASTReader.h:2186:3
[build] #<!-- -->34 0x00007fce9731c1e7 clang::RedeclarableTemplateDecl::SpecEntryTraits&lt;clang::VarTemplateSpecializationDecl&gt;::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl&lt;clang::VarTemplateSpecializationDecl, llvm::ArrayRef&lt;clang::TemplateArgument&gt;&gt;(llvm::FoldingSetVector&lt;clang::VarTemplateSpecializationDecl, llvm::SmallVector&lt;clang::VarTemplateSpecializationDecl*, 8u&gt;&gt;&amp;, void*&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /usr/src/debug/clang/clang-21.1.4.src/lib/AST/DeclTemplate.cpp:401:3
[build] #<!-- -->35 0x00007fce9731c1e7 clang::VarTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/AST/DeclTemplate.cpp:1338:32
[build] #<!-- -->36 0x00007fce98264fcc clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4522:39
[build] #<!-- -->37 0x00007fce98267866 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4682:3
[build] #<!-- -->38 0x00007fce98267866 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplate.cpp:4798:40
[build] #<!-- -->39 0x00007fce983bfbb9 RebuildTemplateIdExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:3548:41
[build] #<!-- -->40 0x00007fce983bfbb9 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:14840:44
[build] #<!-- -->41 0x00007fce97cb7870 llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #<!-- -->42 0x00007fce97cb7870 ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #<!-- -->43 0x00007fce97cb7870 ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #<!-- -->44 0x00007fce97cb7870 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4488:1
[build] #<!-- -->45 0x00007fce97cb7870 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4480:1
[build] #<!-- -->46 0x00007fce97cb7870 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4506:19
[build] #<!-- -->47 0x00007fce97cb7870 EvaluateAtomicConstraint /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:236:30
[build] #<!-- -->48 0x00007fce97cb7870 calculateConstraintSatisfaction /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:494:62
[build] #<!-- -->49 0x00007fce97cb92ca CheckConstraintSatisfaction /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:595:5
[build] #<!-- -->50 0x00007fce97cb9886 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:654:3
[build] #<!-- -->51 0x00007fce97cbc448 llvm::SmallVectorTemplateCommon&lt;clang::Expr*, void&gt;::isSmall() const /usr/include/llvm/ADT/SmallVector.h:144:39
[build] #<!-- -->52 0x00007fce97cbc448 llvm::SmallVectorImpl&lt;clang::Expr*&gt;::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:604:23
[build] #<!-- -->53 0x00007fce97cbc448 llvm::SmallVector&lt;clang::Expr*, 4u&gt;::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1204:3
[build] #<!-- -->54 0x00007fce97cbc448 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Sema.h:14667:3
[build] #<!-- -->55 0x00007fce97cbc448 CheckFunctionConstraintsWithoutInstantiation /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:1100:45
[build] #<!-- -->56 0x00007fce97cbc448 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaConcept.cpp:1112:58
[build] #<!-- -->57 0x00007fce98313a0c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateDeduction.cpp:3945:5
[build] #<!-- -->58 0x00007fce98314825 callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool(llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:47:40
[build] #<!-- -->59 0x00007fce96d7cd72 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /usr/src/debug/clang/clang-21.1.4.src/lib/Basic/StackExhaustionHandler.cpp:21:1
[build] #<!-- -->60 0x00007fce98322463 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateDeduction.cpp:4695:10
[build] #<!-- -->61 0x00007fce981cdf72 AddTemplateOverloadCandidateImmediately /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:8010:3
[build] #<!-- -->62 0x00007fce981ce801 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:8092:5
[build] #<!-- -->63 0x00007fce981cfcc1 AddOverloadedCallCandidate /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14248:5
[build] #<!-- -->64 0x00007fce981cfcc1 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14298:31
[build] #<!-- -->65 0x00007fce981da62b clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14606:21
[build] #<!-- -->66 0x00007fce981ebe59 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaOverload.cpp:14816:3
[build] #<!-- -->67 0x00007fce97eb23dc clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaExpr.cpp:6661:66
[build] #<!-- -->68 0x00007fce97eb31d1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaExpr.cpp:6517:3
[build] #<!-- -->69 0x00007fce983a5256 clang::Sema::FPFeaturesStateRAII::~FPFeaturesStateRAII() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:2905:19
[build] #<!-- -->70 0x00007fce983a5256 TransformCallExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:13434:1
[build] #<!-- -->71 0x00007fce983a716e TransformCXXFoldExpr /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:16444:5
[build] #<!-- -->72 0x00007fce983a166f llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #<!-- -->73 0x00007fce983a166f ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #<!-- -->74 0x00007fce983a166f ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #<!-- -->75 0x00007fce983a166f clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4526:1
[build] #<!-- -->76 0x00007fce983e2c3c clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6128:5
[build] #<!-- -->77 0x00007fce983f665f clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6079:35
[build] #<!-- -->78 0x00007fce983f6bb2 clang::Decl::getKind() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/DeclBase.h:445:51
[build] #<!-- -->79 0x00007fce983f6bb2 clang::ParmVarDecl::classof(clang::Decl const*) /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:1931:58
[build] #<!-- -->80 0x00007fce983f6bb2 llvm::isa_impl&lt;clang::ParmVarDecl, clang::VarDecl, void&gt;::doit(clang::VarDecl const&amp;) /usr/include/llvm/Support/Casting.h:64:64
[build] #<!-- -->81 0x00007fce983f6bb2 llvm::isa_impl_cl&lt;clang::ParmVarDecl, clang::VarDecl const*&gt;::doit(clang::VarDecl const*) /usr/include/llvm/Support/Casting.h:110:36
[build] #<!-- -->82 0x00007fce983f6bb2 llvm::isa_impl_wrap&lt;clang::ParmVarDecl, clang::VarDecl const*, clang::VarDecl const*&gt;::doit(clang::VarDecl const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
[build] #<!-- -->83 0x00007fce983f6bb2 llvm::isa_impl_wrap&lt;clang::ParmVarDecl, clang::VarDecl const* const, clang::VarDecl const*&gt;::doit(clang::VarDecl const* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
[build] #<!-- -->84 0x00007fce983f6bb2 llvm::CastIsPossible&lt;clang::ParmVarDecl, clang::VarDecl const*, void&gt;::isPossible(clang::VarDecl const* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
[build] #<!-- -->85 0x00007fce983f6bb2 llvm::CastInfo&lt;clang::ParmVarDecl, clang::VarDecl const* const, void&gt;::isPossible(clang::VarDecl const* const&amp;) /usr/include/llvm/Support/Casting.h:509:38
[build] #<!-- -->86 0x00007fce983f6bb2 bool llvm::isa&lt;clang::ParmVarDecl, clang::VarDecl const*&gt;(clang::VarDecl const* const&amp;) /usr/include/llvm/Support/Casting.h:549:46
[build] #<!-- -->87 0x00007fce983f6bb2 clang::VarDecl::isNRVOVariable() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:1492:28
[build] #<!-- -->88 0x00007fce983f6bb2 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:1692:24
[build] #<!-- -->89 0x00007fce983b8ba0 operator() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:4646:12
[build] #<!-- -->90 0x00007fce983b8ba0 callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, const clang::MultiLevelTemplateArgumentList&amp;)::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
[build] #<!-- -->91 0x00007fce983ba240 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*&gt;, void&gt;::isSmall() const /usr/include/llvm/ADT/SmallVector.h:144:39
[build] #<!-- -->92 0x00007fce983ba240 llvm::SmallVectorImpl&lt;std::pair&lt;clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*&gt;&gt;::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:604:23
[build] #<!-- -->93 0x00007fce983ba240 llvm::SmallVector&lt;std::pair&lt;clang::VarTemplateDecl*, clang::VarTemplatePartialSpecializationDecl*&gt;, 1u&gt;::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1204:3
[build] #<!-- -->94 0x00007fce983ba240 clang::TemplateDeclInstantiator::~TemplateDeclInstantiator() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Template.h:574:9
[build] #<!-- -->95 0x00007fce983ba240 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:4649:1
[build] #<!-- -->96 0x00007fce983bb4f4 TransformDefinition /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:2007:35
[build] #<!-- -->97 0x00007fce983bb4f4 TransformDeclStmt /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:8579:57
[build] #<!-- -->98 0x00007fce983be9e8 TransformCompoundStmt /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:8124:5
[build] #<!-- -->99 0x00007fce98393393 llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:817:50
[build] #<!-- -->100 0x00007fce98393393 ~TreeTransform /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/TreeTransform.h:110:7
[build] #<!-- -->101 0x00007fce98393393 ~TemplateInstantiator /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:1385:9
[build] #<!-- -->102 0x00007fce98393393 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4459:1
[build] #<!-- -->103 0x00007fce98393393 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiate.cpp:4451:1
[build] #<!-- -->104 0x00007fce983ed082 clang::ActionResult&lt;clang::Stmt*, true&gt;::get() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/Sema/Ownership.h:204:47
[build] #<!-- -->105 0x00007fce983ed082 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5854:28
[build] #<!-- -->106 0x00007fce983fa2d2 clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-21.1.4.src/include/clang/AST/Decl.h:2257:21
[build] #<!-- -->107 0x00007fce983fa2d2 clang::Sema::PerformPendingInstantiations(bool, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:7105:32
[build] #<!-- -->108 0x00007fce97b9b1a4 llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:200:5
[build] #<!-- -->109 0x00007fce97b9b1a4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:1223:3
[build] #<!-- -->110 0x00007fce97b9b501 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-21.1.4.src/lib/Sema/Sema.cpp:1252:9
[build] #<!-- -->111 0x00007fce97083ad7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Parse/Parser.cpp:714:12
[build] #<!-- -->112 0x00007fce96f71589 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.4.src/lib/Parse/ParseAST.cpp:169:69
[build] #<!-- -->113 0x00007fce9904e85c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/CompilerInstance.cpp:86:58
[build] #<!-- -->114 0x00007fce9904e85c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/FrontendAction.cpp:1225:38
[build] #<!-- -->115 0x00007fce98fafccf llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
[build] #<!-- -->116 0x00007fce98fafccf llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
[build] #<!-- -->117 0x00007fce98fafccf clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Frontend/CompilerInstance.cpp:1055:42
[build] #<!-- -->118 0x00007fce990abaac std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1673:16
[build] #<!-- -->119 0x00007fce990abaac std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1370:69
[build] #<!-- -->120 0x00007fce990abaac std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1358:40
[build] #<!-- -->121 0x00007fce990abaac clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Frontend/CompilerInvocation.h:259:48
[build] #<!-- -->122 0x00007fce990abaac clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-21.1.4.src/include/clang/Frontend/CompilerInstance.h:304:39
[build] #<!-- -->123 0x00007fce990abaac clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-21.1.4.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
[build] #<!-- -->124 0x000055c6bf43ae9a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-21.1.4.src/tools/driver/cc1_main.cpp:297:40
[build] #<!-- -->125 0x000055c6bf43eb61 ExecuteCC1Tool /usr/src/debug/clang/clang-21.1.4.src/tools/driver/driver.cpp:223:20
[build] #<!-- -->126 0x00007fce98bf3035 operator() /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Job.cpp:436:32
[build] #<!-- -->127 0x00007fce98bf3035 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
[build] #<!-- -->128 0x00007fce8d7dabb8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm/llvm-21.1.4.src/lib/Support/CrashRecoveryContext.cpp:428:1
[build] #<!-- -->129 0x00007fce98bfebf5 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Job.cpp:440:10
[build] #<!-- -->130 0x00007fce98c1204b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Compilation.cpp:196:22
[build] #<!-- -->131 0x00007fce98c1244c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Compilation.cpp:251:5
[build] #<!-- -->132 0x00007fce98c24c25 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/include/llvm/ADT/SmallVector.h:82:46
[build] #<!-- -->133 0x00007fce98c24c25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang/clang-21.1.4.src/lib/Driver/Driver.cpp:2246:28
[build] #<!-- -->134 0x000055c6bf440b85 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/include/llvm/ADT/SmallVector.h:79:32
[build] #<!-- -->135 0x000055c6bf440b85 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/include/llvm/ADT/SmallVector.h:270:41
[build] #<!-- -->136 0x000055c6bf440b85 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-21.1.4.src/tools/driver/driver.cpp:408:26
[build] #<!-- -->137 0x000055c6bf4322b0 main /usr/src/debug/clang/clang-21.1.4.src/build/tools/driver/clang-driver.cpp:18:1
[build] #<!-- -->138 0x00007fce8c827675 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->139 0x00007fce8c827729 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
[build] #<!-- -->140 0x00007fce8c827729 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
[build] #<!-- -->141 0x000055c6bf432315 _start (/usr/bin/clang+++0xb315)
[build] clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
[build] clang version 21.1.4
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: /tmp/unity_3_cxx-7aaa8d.cpp
[build] clang++: note: diagnostic msg: /tmp/unity_3_cxx-7aaa8d.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
```

Possibly related code:
```cpp

// Helper to check if QVariant can convert to type T
template &lt;typename T&gt;
concept QVariantConvertible = requires (const QVariant &amp;qvar) {
  { qvar.template value&lt;T&gt; () } -&gt; std::same_as&lt;T&gt;;
};

// Primary template (undefined)
template &lt;typename Variant&gt; struct QVariantToStdVariant;

// Specialization for std::variant
template &lt;typename... Ts&gt; struct QVariantToStdVariant&lt;std::variant&lt;Ts...&gt;&gt;
{
  static std::variant&lt;Ts...&gt; convert (const QVariant &amp;qvar)
  {
    std::variant&lt;Ts...&gt; result;
    bool                success = (tryConvert&lt;Ts&gt; (qvar, result) || ...);
    if (!success)
      {
        throw std::runtime_error ("QVariant type not supported in std::variant");
      }
    return result;
  }

private:
  // Non-pointer specialization
  template &lt;typename T&gt;
  static bool tryConvert (const QVariant &amp;qvar, std::variant&lt;Ts...&gt; &amp;out)
  {
    if constexpr (QVariantConvertible&lt;T&gt;)
      {
        if (qvar.userType () == QMetaType::fromType&lt;T&gt; ().id ())
          {
            out = qvar.template value&lt;T&gt; ();
            return true;
          }
      }
    return false;
  }

  // Pointer specialization
  template &lt;typename T&gt;
  static bool tryConvert (const QVariant &amp;qvar, std::variant&lt;Ts...&gt; &amp;out)
    requires (std::is_pointer_v&lt;T&gt;)
  {
    if constexpr (QVariantConvertible&lt;T&gt;)
      {
        if (qvar.template canConvert&lt;T&gt; ())
          {
            auto ptr = qvar.value&lt;T&gt; ();
            if (ptr != nullptr)
              {
                out = ptr;
                return true;
              }
          }
      }
    return false;
  }
};

// Helper function for direct usage
template &lt;typename Variant&gt;
auto
qvariantToStdVariant (const QVariant &amp;qvar)
{
  return QVariantToStdVariant&lt;Variant&gt;::convert (qvar);
}
```

[unity_3_cxx-7aaa8d.cpp](https://github.com/user-attachments/files/23178788/unity_3_cxx-7aaa8d.cpp)
[unity_3_cxx-7aaa8d.sh](https://github.com/user-attachments/files/23178787/unity_3_cxx-7aaa8d.sh)
</details>


---

