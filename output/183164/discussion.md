# [clang-tidy] Segmentation fault on Linux

**Author:** Bobini1
**URL:** https://github.com/llvm/llvm-project/issues/183164
**Status:** Open
**Labels:** clang:static analyzer, crash

## Body

Hi, I'm running clang-tidy on Linux (via CMake) and I started getting crashes today.
The error message asked to report the crash so here I am.


```
$ clang-tidy --version
LLVM (http://llvm.org/):
  LLVM version 21.1.8
  Optimized build.
```

```
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling boost::pfr::get(struct gameplay_logic::BmsResult::DTO &) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#1 Calling support::get(struct gameplay_logic::BmsResult::DTO &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, gameplay_logic::BmsResult::DTO &, int &)::(anonymous class)::operator()(std::integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27>) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, struct gameplay_logic::BmsResult::DTO &, int &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:22
	#4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, std::tuple<gameplay_logic::BmsResult::DTO, gameplay_logic::BmsReplayData::DTO, gameplay_logic::BmsGaugeHistory::DTO> &, int &)::(anonymous class)::operator()(std::integer_sequence<int, 0, 1, 2>) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#5 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, class std::tuple<struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO> &, int &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#6 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, class std::tuple<struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO> &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#7 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 154
	#8 Calling qml_components::ScoreDb::getScoresForCourseIdImpl(const QList<QString> &) at line 391
	#9 Calling qml_components::ScoreDb::getScores(const resource_managers::Table &)::(anonymous class)::operator()()
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #0 0x00007fc5921081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #1 0x00007fc5921052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #2 0x00007fc5921052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #3 0x00007fc59124d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #4 0x00007fc59db40db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #5 0x00007fc59db40db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #6 0x00007fc59db40db3 llvm::isa_impl<clang::MemberExpr, clang::Expr, void>::doit(clang::Expr const&) /usr/include/llvm/Support/Casting.h:64:64
 #7 0x00007fc59db40db3 llvm::isa_impl_cl<clang::MemberExpr, clang::Expr const*>::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #8 0x00007fc59db40db3 llvm::isa_impl_wrap<clang::MemberExpr, clang::Expr const*, clang::Expr const*>::doit(clang::Expr const* const&) /usr/include/llvm/Support/Casting.h:137:41
 #9 0x00007fc59db40db3 llvm::isa_impl_wrap<clang::MemberExpr, clang::Expr* const, clang::Expr const*>::doit(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:129:13
#10 0x00007fc59db40db3 llvm::CastIsPossible<clang::MemberExpr, clang::Expr*, void>::isPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:257:62
#11 0x00007fc59db40db3 llvm::CastInfo<clang::MemberExpr, clang::Expr*, void>::doCastIfPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:493:26
#12 0x00007fc59db40db3 decltype(auto) llvm::dyn_cast<clang::MemberExpr, clang::Expr>(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#13 0x00007fc59db40db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#14 0x00007fc59db467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#15 0x00007fc59db467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#16 0x00007fc59db46f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#17 0x00007fc59db46f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#18 0x00007fc59db47b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#19 0x00007fc59db47b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#20 0x00007fc59db4809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#21 0x00007fc59db4809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#22 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#23 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#24 0x00007fc59dfff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#25 0x00007fc59dfff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#26 0x00007fc59dfff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#27 0x00007fc59e00af8e llvm::DenseMapBase<llvm::DenseMap<clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#28 0x00007fc59e00af8e llvm::detail::DenseSetImpl<clang::Decl const*, llvm::DenseMap<clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>, llvm::DenseMapInfo<clang::Decl const*, void>>::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#29 0x00007fc59e00af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#30 0x00007fc59e00af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#31 0x00007fc59e00af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#32 0x00007fc59d83f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#33 0x00007fc59b771736 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#34 0x00007fc59b771736 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#35 0x00007fc59b771736 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#36 0x00007fc59b771736 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#37 0x00007fc59d84ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#38 0x00007fc59d84ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#39 0x00007fc59d7aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#40 0x00007fc59d7aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#41 0x00007fc59d7aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#42 0x00007fc59d9fd6b1 std::__uniq_ptr_impl<clang::FileSystemStatCache, std::default_delete<clang::FileSystemStatCache>>::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#43 0x00007fc59d9fd6b1 std::unique_ptr<clang::FileSystemStatCache, std::default_delete<clang::FileSystemStatCache>>::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#44 0x00007fc59d9fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#45 0x00007fc59d9fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#46 0x00005577094c475c std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#47 0x00005577094c475c std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#48 0x00005577094c475c std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#49 0x00005577094c475c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#50 0x00007fc59d9fc03d std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#51 0x00007fc59d9fc03d std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#52 0x00007fc59d9fc03d std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#53 0x00007fc59d9fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#54 0x00007fc59d9fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#55 0x00007fc59d9fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#56 0x00005577094cc772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#57 0x0000557708f107a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#58 0x00007fc5912366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#59 0x00007fc5912367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#60 0x00007fc5912367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#61 0x0000557708f04635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:818: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o] Błąd 1
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling boost::pfr::get(struct gameplay_logic::ChartData::DTO &) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#1 Calling support::get(struct gameplay_logic::ChartData::DTO &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, gameplay_logic::ChartData::DTO &, int &)::(anonymous class)::operator()(std::integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35>) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, struct gameplay_logic::ChartData::DTO &, int &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &, struct gameplay_logic::ChartData::DTO &) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#5 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 463
	#6 Calling qml_components::ChartLoader::loadChartDataFromDb(QList<QString>)
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #0 0x00007f7342f081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #1 0x00007f7342f052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #2 0x00007f7342f052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #3 0x00007f734204d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #4 0x00007f734e940db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #5 0x00007f734e940db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #6 0x00007f734e940db3 llvm::isa_impl<clang::MemberExpr, clang::Expr, void>::doit(clang::Expr const&) /usr/include/llvm/Support/Casting.h:64:64
 #7 0x00007f734e940db3 llvm::isa_impl_cl<clang::MemberExpr, clang::Expr const*>::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #8 0x00007f734e940db3 llvm::isa_impl_wrap<clang::MemberExpr, clang::Expr const*, clang::Expr const*>::doit(clang::Expr const* const&) /usr/include/llvm/Support/Casting.h:137:41
 #9 0x00007f734e940db3 llvm::isa_impl_wrap<clang::MemberExpr, clang::Expr* const, clang::Expr const*>::doit(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:129:13
#10 0x00007f734e940db3 llvm::CastIsPossible<clang::MemberExpr, clang::Expr*, void>::isPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:257:62
#11 0x00007f734e940db3 llvm::CastInfo<clang::MemberExpr, clang::Expr*, void>::doCastIfPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:493:26
#12 0x00007f734e940db3 decltype(auto) llvm::dyn_cast<clang::MemberExpr, clang::Expr>(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#13 0x00007f734e940db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#14 0x00007f734e9467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#15 0x00007f734e9467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#16 0x00007f734e946f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#17 0x00007f734e946f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#18 0x00007f734e947b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#19 0x00007f734e947b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#20 0x00007f734e94809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#21 0x00007f734e94809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#22 0x00007f734edff1cb llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#23 0x00007f734edff1cb llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#24 0x00007f734edff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#25 0x00007f734edff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#26 0x00007f734edff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#27 0x00007f734ee0af8e llvm::DenseMapBase<llvm::DenseMap<clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#28 0x00007f734ee0af8e llvm::detail::DenseSetImpl<clang::Decl const*, llvm::DenseMap<clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseSetPair<clang::Decl const*>>, llvm::DenseMapInfo<clang::Decl const*, void>>::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#29 0x00007f734ee0af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#30 0x00007f734ee0af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#31 0x00007f734ee0af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#32 0x00007f734e63f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#33 0x00007f734c571736 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#34 0x00007f734c571736 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#35 0x00007f734c571736 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#36 0x00007f734c571736 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#37 0x00007f734e64ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#38 0x00007f734e64ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#39 0x00007f734e5aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#40 0x00007f734e5aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#41 0x00007f734e5aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#42 0x00007f734e7fd6b1 std::__uniq_ptr_impl<clang::FileSystemStatCache, std::default_delete<clang::FileSystemStatCache>>::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#43 0x00007f734e7fd6b1 std::unique_ptr<clang::FileSystemStatCache, std::default_delete<clang::FileSystemStatCache>>::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#44 0x00007f734e7fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#45 0x00007f734e7fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#46 0x0000560d9c60575c std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#47 0x0000560d9c60575c std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#48 0x0000560d9c60575c std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#49 0x0000560d9c60575c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#50 0x00007f734e7fc03d std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#51 0x00007f734e7fc03d std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#52 0x00007f734e7fc03d std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#53 0x00007f734e7fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#54 0x00007f734e7fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#55 0x00007f734e7fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#56 0x0000560d9c60d772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#57 0x0000560d9c0517a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#58 0x00007f73420366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#59 0x00007f73420367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#60 0x00007f73420367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#61 0x0000560d9c045635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:636: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o] Błąd 1
make[2]: *** [CMakeFiles/Makefile2:216: CMakeFiles/RhythmGame_lib.dir/all] Błąd 2
make[1]: *** [CMakeFiles/Makefile2:264: CMakeFiles/RhythmGame_exe.dir/rule] Błąd 2
make: *** [Makefile:212: RhythmGame_exe] Błąd 2
```


My project:
https://github.com/Bobini1/RhythmGame/tree/3168ee935a780ff89d08d064f22640dd7e805762

My CMakeUserPresets (I was trying to build with the `dev-rel` preset):
```
{
  "version": 2,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 14,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev-common",
      "hidden": true,
      "inherits": [
        "dev-mode",
        "vcpkg",
        "clang-tidy"
      ],
      "cacheVariables": {
        "CMAKE_EXE_LINKER_FLAGS": "-fuse-ld=mold",
        "CMAKE_SHARED_LINKER_FLAGS": "-fuse-ld=mold"
      }
    },
    {
      "name": "dev-linux",
      "binaryDir": "${sourceDir}/build/dev-linux",
      "inherits": [
        "dev-common",
        "ci-linux"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "VCPKG_TARGET_TRIPLET": "x64-linux-sharedqt",
        "VCPKG_HOST_TRIPLET": "x64-linux-sharedqt"
      }
    },
    {
      "name": "dev-darwin",
      "binaryDir": "${sourceDir}/build/dev-darwin",
      "inherits": [
        "dev-common",
        "ci-darwin",
        "vcpkg-osx-sharedqt"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "dev-win64",
      "binaryDir": "${sourceDir}/build/dev-win64",
      "inherits": [
        "dev-common",
        "ci-win64",
        "vcpkg-win64-sharedqt"
      ],
      "environment": {
        "UseMultiToolTask": "true",
        "EnforceProcessCountAcrossBuilds": "true"
      }
    },
    {
      "name": "dev",
      "binaryDir": "${sourceDir}/build/dev",
      "inherits": "dev-linux",
      "environment": {
        "VCPKG_ROOT": "/home/bobini/vcpkg"
      },
      "cacheVariables": {
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install-debug"
      }
    },
    {
      "name": "dev-rel",
      "binaryDir": "${sourceDir}/build/dev-rel",
      "inherits": "dev",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install"
      }
    },
    {
      "name": "dev-coverage",
      "binaryDir": "${sourceDir}/build/coverage",
      "inherits": [
        "dev-mode",
        "coverage-linux",
        "vcpkg"
      ]
    }
  ],
  "buildPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "jobs": 16
    },
    {
      "name": "dev-rel",
      "configurePreset": "dev-rel",
      "configuration": "Release",
      "jobs": 16
    }
  ],
  "testPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      },
      "execution": {
        "jobs": 16,
        "noTestsAction": "error"
      }
    }
  ]
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Tomasz Kalisiak (Bobini1)

<details>
Hi, I'm running clang-tidy on Linux (via CMake) and I started getting crashes today.
The error message asked to report the crash so here I am.


```
$ clang-tidy --version
LLVM (http://llvm.org/):
  LLVM version 21.1.8
  Optimized build.
```

```
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling boost::pfr::get(struct gameplay_logic::BmsResult::DTO &amp;) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#<!-- -->1 Calling support::get(struct gameplay_logic::BmsResult::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#<!-- -->2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, gameplay_logic::BmsResult::DTO &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::BmsResult::DTO &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:22
	#<!-- -->4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, std::tuple&lt;gameplay_logic::BmsResult::DTO, gameplay_logic::BmsReplayData::DTO, gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->5 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, class std::tuple&lt;struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#<!-- -->6 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, class std::tuple&lt;struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#<!-- -->7 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 154
	#<!-- -->8 Calling qml_components::ScoreDb::getScoresForCourseIdImpl(const QList&lt;QString&gt; &amp;) at line 391
	#<!-- -->9 Calling qml_components::ScoreDb::getScores(const resource_managers::Table &amp;)::(anonymous class)::operator()()
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #<!-- -->0 0x00007fc5921081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007fc5921052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007fc5921052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #<!-- -->3 0x00007fc59124d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #<!-- -->4 0x00007fc59db40db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #<!-- -->5 0x00007fc59db40db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #<!-- -->6 0x00007fc59db40db3 llvm::isa_impl&lt;clang::MemberExpr, clang::Expr, void&gt;::doit(clang::Expr const&amp;) /usr/include/llvm/Support/Casting.h:64:64
 #<!-- -->7 0x00007fc59db40db3 llvm::isa_impl_cl&lt;clang::MemberExpr, clang::Expr const*&gt;::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #<!-- -->8 0x00007fc59db40db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr const*, clang::Expr const*&gt;::doit(clang::Expr const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
 #<!-- -->9 0x00007fc59db40db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr* const, clang::Expr const*&gt;::doit(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
#<!-- -->10 0x00007fc59db40db3 llvm::CastIsPossible&lt;clang::MemberExpr, clang::Expr*, void&gt;::isPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
#<!-- -->11 0x00007fc59db40db3 llvm::CastInfo&lt;clang::MemberExpr, clang::Expr*, void&gt;::doCastIfPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:493:26
#<!-- -->12 0x00007fc59db40db3 decltype(auto) llvm::dyn_cast&lt;clang::MemberExpr, clang::Expr&gt;(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#<!-- -->13 0x00007fc59db40db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#<!-- -->14 0x00007fc59db467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#<!-- -->15 0x00007fc59db467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#<!-- -->16 0x00007fc59db46f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#<!-- -->17 0x00007fc59db46f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#<!-- -->18 0x00007fc59db47b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#<!-- -->19 0x00007fc59db47b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#<!-- -->20 0x00007fc59db4809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#<!-- -->21 0x00007fc59db4809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#<!-- -->22 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->23 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->24 0x00007fc59dfff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#<!-- -->25 0x00007fc59dfff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#<!-- -->26 0x00007fc59dfff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#<!-- -->27 0x00007fc59e00af8e llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#<!-- -->28 0x00007fc59e00af8e llvm::detail::DenseSetImpl&lt;clang::Decl const*, llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#<!-- -->29 0x00007fc59e00af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#<!-- -->30 0x00007fc59e00af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#<!-- -->31 0x00007fc59e00af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#<!-- -->32 0x00007fc59d83f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#<!-- -->33 0x00007fc59b771736 __gnu_cxx::__normal_iterator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;*, std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;::__normal_iterator(std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;* const&amp;) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#<!-- -->34 0x00007fc59b771736 std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#<!-- -->35 0x00007fc59b771736 void clang::finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;(std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&amp;, clang::Sema const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#<!-- -->36 0x00007fc59b771736 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#<!-- -->37 0x00007fc59d84ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#<!-- -->38 0x00007fc59d84ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->39 0x00007fc59d7aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#<!-- -->40 0x00007fc59d7aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#<!-- -->41 0x00007fc59d7aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->42 0x00007fc59d9fd6b1 std::__uniq_ptr_impl&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#<!-- -->43 0x00007fc59d9fd6b1 std::unique_ptr&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#<!-- -->44 0x00007fc59d9fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#<!-- -->45 0x00007fc59d9fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#<!-- -->46 0x00005577094c475c std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->47 0x00005577094c475c std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->48 0x00005577094c475c std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->49 0x00005577094c475c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#<!-- -->50 0x00007fc59d9fc03d std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->51 0x00007fc59d9fc03d std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->52 0x00007fc59d9fc03d std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->53 0x00007fc59d9fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#<!-- -->54 0x00007fc59d9fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#<!-- -->55 0x00007fc59d9fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#<!-- -->56 0x00005577094cc772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#<!-- -->57 0x0000557708f107a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#<!-- -->58 0x00007fc5912366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->59 0x00007fc5912367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007fc5912367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->61 0x0000557708f04635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:818: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o] Błąd 1
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling boost::pfr::get(struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#<!-- -->1 Calling support::get(struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#<!-- -->2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, gameplay_logic::ChartData::DTO &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::ChartData::DTO &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#<!-- -->4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#<!-- -->5 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 463
	#<!-- -->6 Calling qml_components::ChartLoader::loadChartDataFromDb(QList&lt;QString&gt;)
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #<!-- -->0 0x00007f7342f081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007f7342f052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007f7342f052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #<!-- -->3 0x00007f734204d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #<!-- -->4 0x00007f734e940db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #<!-- -->5 0x00007f734e940db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #<!-- -->6 0x00007f734e940db3 llvm::isa_impl&lt;clang::MemberExpr, clang::Expr, void&gt;::doit(clang::Expr const&amp;) /usr/include/llvm/Support/Casting.h:64:64
 #<!-- -->7 0x00007f734e940db3 llvm::isa_impl_cl&lt;clang::MemberExpr, clang::Expr const*&gt;::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #<!-- -->8 0x00007f734e940db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr const*, clang::Expr const*&gt;::doit(clang::Expr const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
 #<!-- -->9 0x00007f734e940db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr* const, clang::Expr const*&gt;::doit(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
#<!-- -->10 0x00007f734e940db3 llvm::CastIsPossible&lt;clang::MemberExpr, clang::Expr*, void&gt;::isPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
#<!-- -->11 0x00007f734e940db3 llvm::CastInfo&lt;clang::MemberExpr, clang::Expr*, void&gt;::doCastIfPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:493:26
#<!-- -->12 0x00007f734e940db3 decltype(auto) llvm::dyn_cast&lt;clang::MemberExpr, clang::Expr&gt;(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#<!-- -->13 0x00007f734e940db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#<!-- -->14 0x00007f734e9467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#<!-- -->15 0x00007f734e9467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#<!-- -->16 0x00007f734e946f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#<!-- -->17 0x00007f734e946f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#<!-- -->18 0x00007f734e947b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#<!-- -->19 0x00007f734e947b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#<!-- -->20 0x00007f734e94809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#<!-- -->21 0x00007f734e94809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#<!-- -->22 0x00007f734edff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->23 0x00007f734edff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->24 0x00007f734edff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#<!-- -->25 0x00007f734edff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#<!-- -->26 0x00007f734edff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#<!-- -->27 0x00007f734ee0af8e llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#<!-- -->28 0x00007f734ee0af8e llvm::detail::DenseSetImpl&lt;clang::Decl const*, llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#<!-- -->29 0x00007f734ee0af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#<!-- -->30 0x00007f734ee0af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#<!-- -->31 0x00007f734ee0af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#<!-- -->32 0x00007f734e63f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#<!-- -->33 0x00007f734c571736 __gnu_cxx::__normal_iterator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;*, std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;::__normal_iterator(std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;* const&amp;) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#<!-- -->34 0x00007f734c571736 std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#<!-- -->35 0x00007f734c571736 void clang::finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;(std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&amp;, clang::Sema const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#<!-- -->36 0x00007f734c571736 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#<!-- -->37 0x00007f734e64ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#<!-- -->38 0x00007f734e64ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->39 0x00007f734e5aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#<!-- -->40 0x00007f734e5aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#<!-- -->41 0x00007f734e5aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->42 0x00007f734e7fd6b1 std::__uniq_ptr_impl&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#<!-- -->43 0x00007f734e7fd6b1 std::unique_ptr&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#<!-- -->44 0x00007f734e7fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#<!-- -->45 0x00007f734e7fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#<!-- -->46 0x0000560d9c60575c std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->47 0x0000560d9c60575c std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->48 0x0000560d9c60575c std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->49 0x0000560d9c60575c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#<!-- -->50 0x00007f734e7fc03d std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->51 0x00007f734e7fc03d std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->52 0x00007f734e7fc03d std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->53 0x00007f734e7fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#<!-- -->54 0x00007f734e7fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#<!-- -->55 0x00007f734e7fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#<!-- -->56 0x0000560d9c60d772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#<!-- -->57 0x0000560d9c0517a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#<!-- -->58 0x00007f73420366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->59 0x00007f73420367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007f73420367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->61 0x0000560d9c045635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:636: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o] Błąd 1
make[2]: *** [CMakeFiles/Makefile2:216: CMakeFiles/RhythmGame_lib.dir/all] Błąd 2
make[1]: *** [CMakeFiles/Makefile2:264: CMakeFiles/RhythmGame_exe.dir/rule] Błąd 2
make: *** [Makefile:212: RhythmGame_exe] Błąd 2
```


My project:
https://github.com/Bobini1/RhythmGame/tree/3168ee935a780ff89d08d064f22640dd7e805762

My CMakeUserPresets (I was trying to build with the `dev-rel` preset):
```
{
  "version": 2,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 14,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev-common",
      "hidden": true,
      "inherits": [
        "dev-mode",
        "vcpkg",
        "clang-tidy"
      ],
      "cacheVariables": {
        "CMAKE_EXE_LINKER_FLAGS": "-fuse-ld=mold",
        "CMAKE_SHARED_LINKER_FLAGS": "-fuse-ld=mold"
      }
    },
    {
      "name": "dev-linux",
      "binaryDir": "${sourceDir}/build/dev-linux",
      "inherits": [
        "dev-common",
        "ci-linux"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "VCPKG_TARGET_TRIPLET": "x64-linux-sharedqt",
        "VCPKG_HOST_TRIPLET": "x64-linux-sharedqt"
      }
    },
    {
      "name": "dev-darwin",
      "binaryDir": "${sourceDir}/build/dev-darwin",
      "inherits": [
        "dev-common",
        "ci-darwin",
        "vcpkg-osx-sharedqt"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "dev-win64",
      "binaryDir": "${sourceDir}/build/dev-win64",
      "inherits": [
        "dev-common",
        "ci-win64",
        "vcpkg-win64-sharedqt"
      ],
      "environment": {
        "UseMultiToolTask": "true",
        "EnforceProcessCountAcrossBuilds": "true"
      }
    },
    {
      "name": "dev",
      "binaryDir": "${sourceDir}/build/dev",
      "inherits": "dev-linux",
      "environment": {
        "VCPKG_ROOT": "/home/bobini/vcpkg"
      },
      "cacheVariables": {
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install-debug"
      }
    },
    {
      "name": "dev-rel",
      "binaryDir": "${sourceDir}/build/dev-rel",
      "inherits": "dev",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install"
      }
    },
    {
      "name": "dev-coverage",
      "binaryDir": "${sourceDir}/build/coverage",
      "inherits": [
        "dev-mode",
        "coverage-linux",
        "vcpkg"
      ]
    }
  ],
  "buildPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "jobs": 16
    },
    {
      "name": "dev-rel",
      "configurePreset": "dev-rel",
      "configuration": "Release",
      "jobs": 16
    }
  ],
  "testPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      },
      "execution": {
        "jobs": 16,
        "noTestsAction": "error"
      }
    }
  ]
}
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: Tomasz Kalisiak (Bobini1)

<details>
Hi, I'm running clang-tidy on Linux (via CMake) and I started getting crashes today.
The error message asked to report the crash so here I am.


```
$ clang-tidy --version
LLVM (http://llvm.org/):
  LLVM version 21.1.8
  Optimized build.
```

```
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ScoreDb.cpp
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling boost::pfr::get(struct gameplay_logic::BmsResult::DTO &amp;) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#<!-- -->1 Calling support::get(struct gameplay_logic::BmsResult::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#<!-- -->2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, gameplay_logic::BmsResult::DTO &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::BmsResult::DTO &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:22
	#<!-- -->4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, std::tuple&lt;gameplay_logic::BmsResult::DTO, gameplay_logic::BmsReplayData::DTO, gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->5 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, class std::tuple&lt;struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#<!-- -->6 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, class std::tuple&lt;struct gameplay_logic::BmsResult::DTO, struct gameplay_logic::BmsReplayData::DTO, struct gameplay_logic::BmsGaugeHistory::DTO&gt; &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#<!-- -->7 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 154
	#<!-- -->8 Calling qml_components::ScoreDb::getScoresForCourseIdImpl(const QList&lt;QString&gt; &amp;) at line 391
	#<!-- -->9 Calling qml_components::ScoreDb::getScores(const resource_managers::Table &amp;)::(anonymous class)::operator()()
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #<!-- -->0 0x00007fc5921081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007fc5921052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007fc5921052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #<!-- -->3 0x00007fc59124d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #<!-- -->4 0x00007fc59db40db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #<!-- -->5 0x00007fc59db40db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #<!-- -->6 0x00007fc59db40db3 llvm::isa_impl&lt;clang::MemberExpr, clang::Expr, void&gt;::doit(clang::Expr const&amp;) /usr/include/llvm/Support/Casting.h:64:64
 #<!-- -->7 0x00007fc59db40db3 llvm::isa_impl_cl&lt;clang::MemberExpr, clang::Expr const*&gt;::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #<!-- -->8 0x00007fc59db40db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr const*, clang::Expr const*&gt;::doit(clang::Expr const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
 #<!-- -->9 0x00007fc59db40db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr* const, clang::Expr const*&gt;::doit(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
#<!-- -->10 0x00007fc59db40db3 llvm::CastIsPossible&lt;clang::MemberExpr, clang::Expr*, void&gt;::isPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
#<!-- -->11 0x00007fc59db40db3 llvm::CastInfo&lt;clang::MemberExpr, clang::Expr*, void&gt;::doCastIfPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:493:26
#<!-- -->12 0x00007fc59db40db3 decltype(auto) llvm::dyn_cast&lt;clang::MemberExpr, clang::Expr&gt;(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#<!-- -->13 0x00007fc59db40db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#<!-- -->14 0x00007fc59db467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#<!-- -->15 0x00007fc59db467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#<!-- -->16 0x00007fc59db46f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#<!-- -->17 0x00007fc59db46f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#<!-- -->18 0x00007fc59db47b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#<!-- -->19 0x00007fc59db47b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#<!-- -->20 0x00007fc59db4809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#<!-- -->21 0x00007fc59db4809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#<!-- -->22 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->23 0x00007fc59dfff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->24 0x00007fc59dfff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#<!-- -->25 0x00007fc59dfff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#<!-- -->26 0x00007fc59dfff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#<!-- -->27 0x00007fc59e00af8e llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#<!-- -->28 0x00007fc59e00af8e llvm::detail::DenseSetImpl&lt;clang::Decl const*, llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#<!-- -->29 0x00007fc59e00af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#<!-- -->30 0x00007fc59e00af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#<!-- -->31 0x00007fc59e00af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#<!-- -->32 0x00007fc59d83f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#<!-- -->33 0x00007fc59b771736 __gnu_cxx::__normal_iterator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;*, std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;::__normal_iterator(std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;* const&amp;) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#<!-- -->34 0x00007fc59b771736 std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#<!-- -->35 0x00007fc59b771736 void clang::finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;(std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&amp;, clang::Sema const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#<!-- -->36 0x00007fc59b771736 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#<!-- -->37 0x00007fc59d84ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#<!-- -->38 0x00007fc59d84ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->39 0x00007fc59d7aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#<!-- -->40 0x00007fc59d7aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#<!-- -->41 0x00007fc59d7aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->42 0x00007fc59d9fd6b1 std::__uniq_ptr_impl&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#<!-- -->43 0x00007fc59d9fd6b1 std::unique_ptr&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#<!-- -->44 0x00007fc59d9fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#<!-- -->45 0x00007fc59d9fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#<!-- -->46 0x00005577094c475c std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->47 0x00005577094c475c std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->48 0x00005577094c475c std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->49 0x00005577094c475c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#<!-- -->50 0x00007fc59d9fc03d std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->51 0x00007fc59d9fc03d std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->52 0x00007fc59d9fc03d std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->53 0x00007fc59d9fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#<!-- -->54 0x00007fc59d9fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#<!-- -->55 0x00007fc59d9fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#<!-- -->56 0x00005577094cc772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#<!-- -->57 0x0000557708f107a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#<!-- -->58 0x00007fc5912366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->59 0x00007fc5912367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007fc5912367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->61 0x0000557708f04635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:818: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ScoreDb.cpp.o] Błąd 1
Error running 'clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-tidy --header-filter=/home/bobini/RhythmGame/* --extra-arg-before=--driver-mode=g++ /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp -- /usr/bin/c++ -DLEXY_HAS_UNICODE_DATABASE=1 -DLLFIO_EXCLUDE_DYNAMIC_THREAD_POOL_GROUP=1 -DLLFIO_EXCLUDE_NETWORKING=1 -DNOMINMAX -DOUTCOME_USE_SYSTEM_STATUS_CODE=1 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_GUI_LIB -DQT_INTERFACEFRAMEWORK_LIB -DQT_MULTIMEDIA_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DRHYTHMGAME_DATA_DIR=\"share\" -DRHYTHMGAME_VERSION_MAJOR=1 -DRHYTHMGAME_VERSION_MINOR=2 -DRHYTHMGAME_VERSION_PATCH=7 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSQLITE_ENABLE_COLUMN_METADATA -I/home/bobini/RhythmGame/build/dev-rel/RhythmGame_lib_autogen/include -I/home/bobini/RhythmGame/src -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/SDL2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuick -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtCore -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/share/Qt6/mkspecs/linux-g++ -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtGui -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQml -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlIntegration -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtNetwork -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlMeta -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlModels -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQmlWorkerScript -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtOpenGL -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtQuickControls2 -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtConcurrent -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtMultimedia -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/Qt6/QtInterfaceFramework -isystem /home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/libxml2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast -O3 -DNDEBUG -std=c++23 -flto=auto -fno-fat-lto-objects -fdiagnostics-color=always -fPIC -MD -MT CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -MF CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o.d -o CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o -c /home/bobini/RhythmGame/src/qml_components/ChartLoader.cpp
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling boost::pfr::get(struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/support/get.h:39:12
	#<!-- -->1 Calling support::get(struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:125:37
	#<!-- -->2 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, gameplay_logic::ChartData::DTO &amp;, int &amp;)::(anonymous class)::operator()(std::integer_sequence&lt;int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35&gt;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:124:17
	#<!-- -->3 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::ChartData::DTO &amp;, int &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:112:13
	#<!-- -->4 Calling db::SqliteCppDb::Statement::writeRow(SQLite::Statement &amp;, struct gameplay_logic::ChartData::DTO &amp;) at line /home/bobini/RhythmGame/src/db/SqliteCppDb.h:90:17
	#<!-- -->5 Calling db::SqliteCppDb::Statement::executeAndGetAll() at line 463
	#<!-- -->6 Calling qml_components::ChartLoader::loadChartDataFromDb(QList&lt;QString&gt;)
3.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
4.	/home/bobini/RhythmGame/build/dev-rel/vcpkg_installed/x64-linux-sharedqt/include/boost/pfr/core.hpp:72:35: Error evaluating statement
 #<!-- -->0 0x00007f7342f081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007f7342f052c7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007f7342f052c7 SignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:409:31
 #<!-- -->3 0x00007f734204d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #<!-- -->4 0x00007f734e940db3 clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Stmt.h:1484:44
 #<!-- -->5 0x00007f734e940db3 clang::MemberExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.8.src/include/clang/AST/Expr.h:3514:27
 #<!-- -->6 0x00007f734e940db3 llvm::isa_impl&lt;clang::MemberExpr, clang::Expr, void&gt;::doit(clang::Expr const&amp;) /usr/include/llvm/Support/Casting.h:64:64
 #<!-- -->7 0x00007f734e940db3 llvm::isa_impl_cl&lt;clang::MemberExpr, clang::Expr const*&gt;::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
 #<!-- -->8 0x00007f734e940db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr const*, clang::Expr const*&gt;::doit(clang::Expr const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
 #<!-- -->9 0x00007f734e940db3 llvm::isa_impl_wrap&lt;clang::MemberExpr, clang::Expr* const, clang::Expr const*&gt;::doit(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
#<!-- -->10 0x00007f734e940db3 llvm::CastIsPossible&lt;clang::MemberExpr, clang::Expr*, void&gt;::isPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
#<!-- -->11 0x00007f734e940db3 llvm::CastInfo&lt;clang::MemberExpr, clang::Expr*, void&gt;::doCastIfPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:493:26
#<!-- -->12 0x00007f734e940db3 decltype(auto) llvm::dyn_cast&lt;clang::MemberExpr, clang::Expr&gt;(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#<!-- -->13 0x00007f734e940db3 clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:3278:46
#<!-- -->14 0x00007f734e9467d9 clang::ento::NodeBuilder::addNodes(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/CoreEngine.h:341:60
#<!-- -->15 0x00007f734e9467d9 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:2274:20
#<!-- -->16 0x00007f734e946f9b clang::ento::ExplodedNodeSet::insert(clang::ento::ExplodedNodeSet const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h:473:5
#<!-- -->17 0x00007f734e946f9b clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/ExprEngine.cpp:1136:15
#<!-- -->18 0x00007f734e947b40 clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:531:30
#<!-- -->19 0x00007f734e947b40 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:255:21
#<!-- -->20 0x00007f734e94809a operator() /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:159:23
#<!-- -->21 0x00007f734e94809a clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Core/CoreEngine.cpp:163:41
#<!-- -->22 0x00007f734edff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->23 0x00007f734edff1cb llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->24 0x00007f734edff1cb clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) /usr/src/debug/clang/clang-21.1.8.src/include/clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h:192:45
#<!-- -->25 0x00007f734edff1cb RunPathSensitiveChecks /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:768:22
#<!-- -->26 0x00007f734edff1cb HandleCode /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:737:27
#<!-- -->27 0x00007f734ee0af8e llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;::begin() /usr/include/llvm/ADT/DenseMap.h:80:5
#<!-- -->28 0x00007f734ee0af8e llvm::detail::DenseSetImpl&lt;clang::Decl const*, llvm::DenseMap&lt;clang::Decl const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseSetPair&lt;clang::Decl const*&gt;&gt;, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;::begin() /usr/include/llvm/ADT/DenseSet.h:187:50
#<!-- -->29 0x00007f734ee0af8e HandleDeclsCallGraph /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:512:31
#<!-- -->30 0x00007f734ee0af8e runAnalysisOnTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:580:25
#<!-- -->31 0x00007f734ee0af8e HandleTranslationUnit /usr/src/debug/clang/clang-21.1.8.src/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:643:31
#<!-- -->32 0x00007f734e63f360 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/MultiplexConsumer.cpp:338:25
#<!-- -->33 0x00007f734c571736 __gnu_cxx::__normal_iterator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;*, std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;::__normal_iterator(std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;* const&amp;) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#<!-- -->34 0x00007f734c571736 std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#<!-- -->35 0x00007f734c571736 void clang::finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&gt;(std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt;&gt;&gt;&gt;&amp;, clang::Sema const&amp;) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#<!-- -->36 0x00007f734c571736 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#<!-- -->37 0x00007f734e64ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#<!-- -->38 0x00007f734e64ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->39 0x00007f734e5aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#<!-- -->40 0x00007f734e5aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#<!-- -->41 0x00007f734e5aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->42 0x00007f734e7fd6b1 std::__uniq_ptr_impl&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:201:16
#<!-- -->43 0x00007f734e7fd6b1 std::unique_ptr&lt;clang::FileSystemStatCache, std::default_delete&lt;clang::FileSystemStatCache&gt;&gt;::reset(clang::FileSystemStatCache*) /usr/include/c++/15.2.1/bits/unique_ptr.h:511:12
#<!-- -->44 0x00007f734e7fd6b1 clang::FileManager::clearStatCache() /usr/src/debug/clang/clang-21.1.8.src/lib/Basic/FileManager.cpp:61:53
#<!-- -->45 0x00007f734e7fd6b1 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:465:24
#<!-- -->46 0x0000560d9c60575c std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->47 0x0000560d9c60575c std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->48 0x0000560d9c60575c std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->49 0x0000560d9c60575c runInvocation /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:593:50
#<!-- -->50 0x00007f734e7fc03d std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#<!-- -->51 0x00007f734e7fc03d std::__shared_ptr&lt;clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#<!-- -->52 0x00007f734e7fc03d std::shared_ptr&lt;clang::CompilerInvocation&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#<!-- -->53 0x00007f734e7fc03d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:438:31
#<!-- -->54 0x00007f734e7fc84d clang::tooling::ToolInvocation::run() /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:423:23
#<!-- -->55 0x00007f734e7fe7d3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /usr/src/debug/clang/clang-21.1.8.src/lib/Tooling/Tooling.cpp:621:7
#<!-- -->56 0x0000560d9c60d772 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) /usr/src/debug/clang/clang-tools-extra/clang-tidy/ClangTidy.cpp:615:28
#<!-- -->57 0x0000560d9c0517a1 clang::tidy::clangTidyMain(int, char const**) /usr/src/debug/clang/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:715:63
#<!-- -->58 0x00007f73420366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->59 0x00007f73420367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007f73420367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->61 0x0000560d9c045635 _start (/usr/bin/clang-tidy+0xdd635)
Segmentation fault
make[3]: *** [CMakeFiles/RhythmGame_lib.dir/build.make:636: CMakeFiles/RhythmGame_lib.dir/src/qml_components/ChartLoader.cpp.o] Błąd 1
make[2]: *** [CMakeFiles/Makefile2:216: CMakeFiles/RhythmGame_lib.dir/all] Błąd 2
make[1]: *** [CMakeFiles/Makefile2:264: CMakeFiles/RhythmGame_exe.dir/rule] Błąd 2
make: *** [Makefile:212: RhythmGame_exe] Błąd 2
```


My project:
https://github.com/Bobini1/RhythmGame/tree/3168ee935a780ff89d08d064f22640dd7e805762

My CMakeUserPresets (I was trying to build with the `dev-rel` preset):
```
{
  "version": 2,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 14,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev-common",
      "hidden": true,
      "inherits": [
        "dev-mode",
        "vcpkg",
        "clang-tidy"
      ],
      "cacheVariables": {
        "CMAKE_EXE_LINKER_FLAGS": "-fuse-ld=mold",
        "CMAKE_SHARED_LINKER_FLAGS": "-fuse-ld=mold"
      }
    },
    {
      "name": "dev-linux",
      "binaryDir": "${sourceDir}/build/dev-linux",
      "inherits": [
        "dev-common",
        "ci-linux"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "VCPKG_TARGET_TRIPLET": "x64-linux-sharedqt",
        "VCPKG_HOST_TRIPLET": "x64-linux-sharedqt"
      }
    },
    {
      "name": "dev-darwin",
      "binaryDir": "${sourceDir}/build/dev-darwin",
      "inherits": [
        "dev-common",
        "ci-darwin",
        "vcpkg-osx-sharedqt"
      ],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "dev-win64",
      "binaryDir": "${sourceDir}/build/dev-win64",
      "inherits": [
        "dev-common",
        "ci-win64",
        "vcpkg-win64-sharedqt"
      ],
      "environment": {
        "UseMultiToolTask": "true",
        "EnforceProcessCountAcrossBuilds": "true"
      }
    },
    {
      "name": "dev",
      "binaryDir": "${sourceDir}/build/dev",
      "inherits": "dev-linux",
      "environment": {
        "VCPKG_ROOT": "/home/bobini/vcpkg"
      },
      "cacheVariables": {
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install-debug"
      }
    },
    {
      "name": "dev-rel",
      "binaryDir": "${sourceDir}/build/dev-rel",
      "inherits": "dev",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INSTALL_PREFIX": "/home/bobini/RhythmGame/install"
      }
    },
    {
      "name": "dev-coverage",
      "binaryDir": "${sourceDir}/build/coverage",
      "inherits": [
        "dev-mode",
        "coverage-linux",
        "vcpkg"
      ]
    }
  ],
  "buildPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "jobs": 16
    },
    {
      "name": "dev-rel",
      "configurePreset": "dev-rel",
      "configuration": "Release",
      "jobs": 16
    }
  ],
  "testPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      },
      "execution": {
        "jobs": 16,
        "noTestsAction": "error"
      }
    }
  ]
}
```
</details>


---

### Comment 3 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.


---

### Comment 4 - steakhal

Could you please attach the preprocessed file of the file that crashed?

BTW let me guess, there is a structured binding pack expression a the crash location?
We had recently another crash using `boost::pfr`. This suggests to me that this is a duplicate of that one https://github.com/llvm/llvm-project/issues/182346

---

### Comment 5 - steakhal

Hmm, so it's coming from this one, which has nothing to indicate the structured binding pack expressions.
```c++
template<std::size_t N, typename T>
constexpr auto
get(T&& aggregate) -> auto&
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
{
    return boost::pfr::get<N>(std::forward<T>(aggregate)); // <--- crashes - according to your stack trace.
}
```

---

### Comment 6 - Bobini1

Good news, I have repro on Compiler Explorer.

https://clang-tidy.godbolt.org/z/nd8TY6qre

Edit: Actually, here is a shorter one: https://clang-tidy.godbolt.org/z/685EGnzf4

---

### Comment 7 - steakhal

Perfect! I'll look into this tomorrow.

---

### Comment 8 - steakhal

The repro can be further simplified by inlining `pfr::get` - which internally uses structure-binding pack indexing.
https://clang-tidy.godbolt.org/z/9aWf3re1b
```c++
template <unsigned I, class T>
constexpr decltype(auto) get(T& val) noexcept {
    auto& [... members] = val;
    return static_cast<decltype(members...[I])&>(members...[I]);
}

struct DTO {
    int id;
};

void repro() {
    auto dto = DTO{};
    auto& member = get<0>(dto);
}
```

This confirms that this is a duplicate.

---

