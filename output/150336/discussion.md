# [clang][Sema][regression] compiling gromacs hangs after introduction of #144408

**Author:** pawosm-arm
**URL:** https://github.com/llvm/llvm-project/issues/150336
**Status:** Closed
**Labels:** clang:frontend, regression, crash
**Closed Date:** 2025-07-30T13:40:09Z

## Body

After the merger of the PR #144408 I can't compile gromacs, the compiler hangs when trying to compile this file: https://github.com/gromacs/gromacs/blob/main/src/gromacs/selection/selelem.cpp

The symptom is the `clang++` hanging forever. I've sent `SIGQUIT` to the hung process, and managed to obtain this trace:

```
[ 32%] Building CXX object src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ -DGMX_DOUBLE=1 -DHAVE_CONFIG_H -DMUPARSER_STATIC -DTMPI_EXPORTS -DTMPI_USE_VISIBILITY -DUSE_STD_INTTYPES_H -Isrc/include -Ibuild/src/include -Isrc -Isrc/external/tng_io/include -Ibuild/tng/include -Iapi/legacy/include -Ibuild/api/legacy/include -Isrc/gromacs/math/include -Isrc/gromacs/timing/include -Isrc/gromacs/utility/include -Isrc/gromacs/pbcutil/include -Isrc/gromacs/pulling/include -Isrc/gromacs/topology/include -Isrc/gromacs/serialization/include -Isrc/gromacs/linearalgebra/include -Isrc/gromacs/simd/include -Isrc/gromacs/taskassignment/include -isystem src/external/thread_mpi/include -isystem fftw/include -isystem src/external -isystem src/gromacs/../external/rpc_xdr -isystem src/external/muparser/include -isystem src/external/lmfit -isystem src/external/colvars -isystem src/external/plumed -O3 -DNDEBUG -std=c++17 -Wall -Wextra -Wpointer-arith -Wmissing-prototypes -Wpedantic -Wdeprecated -Wno-unused-function -Wno-reserved-identifier -Wno-missing-field-initializers -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-source-uses-openmp -Wno-c++17-extensions -Wno-documentation-unknown-command -Wno-covered-switch-default -Wno-switch-enum -Wno-switch-default -Wno-extra-semi-stmt -Wno-weak-vtables -Wno-shadow -Wno-padded -Wno-reserved-id-macro -Wno-double-promotion -Wno-exit-time-destructors -Wno-global-constructors -Wno-documentation -Wno-format-nonliteral -Wno-used-but-marked-unused -Wno-float-equal -Wno-conditional-uninitialized -Wno-conversion -Wno-disabled-macro-expansion -Wno-unused-macros -Wno-unsafe-buffer-usage -Wno-cast-function-type-strict -fopenmp=libomp -MD -MT src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o -MF CMakeFiles/libgromacs.dir/selection/selelem.cpp.o.d -o CMakeFiles/libgromacs.dir/selection/selelem.cpp.o -c src/gromacs/selection/selelem.cpp
1.      src/gromacs/selection/selelem.cpp:563:1: current parser token 'void'
2.      src/gromacs/selection/selelem.cpp:518:1: parsing function body '_gmx_selelem_free_method'
 #0 0x0000ffffaa0c4bd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x0000ffffaa0c5014 PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x0000ffffaa0c2544 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:104:20
 #3 0x0000ffffaa0c4374 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x0000ffffb4fe87dc (linux-vdso.so.1+0x7dc)
 #5 0x0000ffffa2da429c llvm::DenseMapBase<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>, clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>::begin() llvm/include/llvm/ADT/DenseMap.h:85:3
 #6 0x0000ffffa2d9f0c0 decltype(fp.begin()) std::begin<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>>(llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&) /usr/include/c++/11/bits/range_access.h:52:27
 #7 0x0000ffffa2d97114 decltype(begin(forward<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&>(fp))) llvm::adl_detail::begin_impl<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&>(llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&) llvm/include/llvm/ADT/ADL.h:29:43
 #8 0x0000ffffa2d8cedc decltype(begin_impl(forward<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&>(fp))) llvm::adl_begin<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&>(llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&) llvm/include/llvm/ADT/ADL.h:80:60
 #9 0x0000ffffa2d6772c bool llvm::all_of<llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&, areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&, clang::AnalysisDeclContext&)::'lambda'(llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>)>(llvm::DenseMap<clang::CFGBlock const*, std::optional<bool>, llvm::DenseMapInfo<clang::CFGBlock const*, void>, llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>>&, areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&, clang::AnalysisDeclContext&)::'lambda'(llvm::detail::DenseMapPair<clang::CFGBlock const*, std::optional<bool>>)) llvm/include/llvm/ADT/STLExtras.h:1745:21
#10 0x0000ffffa2d5b92c areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&, clang::AnalysisDeclContext&) clang/lib/Sema/AnalysisBasedWarnings.cpp:525:21
#11 0x0000ffffa2d5bf68 CheckFallThrough(clang::AnalysisDeclContext&) clang/lib/Sema/AnalysisBasedWarnings.cpp:674:37
#12 0x0000ffffa2d5c610 CheckFallThroughForBody(clang::Sema&, clang::Decl const*, clang::Stmt const*, clang::QualType, (anonymous namespace)::CheckFallThroughDiagnostics const&, clang::AnalysisDeclContext&) clang/lib/Sema/AnalysisBasedWarnings.cpp:825:52
#13 0x0000ffffa2d6566c clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) clang/lib/Sema/AnalysisBasedWarnings.cpp:2968:9
#14 0x0000ffffa2ea188c clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) clang/lib/Sema/Sema.cpp:2457:35
#15 0x0000ffffa3239938 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) clang/lib/Sema/SemaDecl.cpp:16679:23
#16 0x0000ffffa3237330 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*) clang/lib/Sema/SemaDecl.cpp:16149:1
#17 0x0000ffffa401620c clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) clang/lib/Parse/ParseStmt.cpp:2403:41
#18 0x0000ffffa403c238 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) clang/lib/Parse/Parser.cpp:1449:36
#19 0x0000ffffa3f185f4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2263:40
#20 0x0000ffffa403b28c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1187:73
#21 0x0000ffffa403b430 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1209:42
#22 0x0000ffffa403a830 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1032:50
#23 0x0000ffffa4039998 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:745:36
#24 0x0000ffffa3ef31d4 clang::ParseAST(clang::Sema&, bool, bool) clang/lib/Parse/ParseAST.cpp:170:37
#25 0x0000ffffaf5daabc clang::ASTFrontendAction::ExecuteAction() clang/lib/Frontend/FrontendAction.cpp:1342:11
#26 0x0000ffffb3833bd8 clang::CodeGenAction::ExecuteAction() clang/lib/CodeGen/CodeGenAction.cpp:1112:5
#27 0x0000ffffaf5da48c clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1226:38
#28 0x0000ffffaf4e0144 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) clang/lib/Frontend/CompilerInstance.cpp:1056:42
#29 0x0000ffffb4296ba4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:38
#30 0x0000aaaac1b5c2d4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) clang/tools/driver/cc1_main.cpp:297:40
#31 0x0000aaaac1b4dcbc ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) clang/tools/driver/driver.cpp:223:20
#32 0x0000aaaac1b4de68 clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)::operator()(llvm::SmallVectorImpl<char const*>&) const clang/tools/driver/driver.cpp:369:7
#33 0x0000aaaac1b4ef34 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) llvm/include/llvm/ADT/STLFunctionalExtras.h:48:3
#34 0x0000ffffaed8d470 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#35 0x0000ffffaed8b8e8 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()::operator()() const clang/lib/Driver/Job.cpp:436:32
#36 0x0000ffffaed8bd04 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:47:40
#37 0x0000ffffa9f22cc8 llvm::function_ref<void ()>::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:62
#38 0x0000ffffa9f3c52c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#39 0x0000ffffaed8ba48 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const clang/lib/Driver/Job.cpp:436:21
#40 0x0000ffffaed01e60 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const clang/lib/Driver/Compilation.cpp:196:22
#41 0x0000ffffaed02214 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const clang/lib/Driver/Compilation.cpp:251:62
#42 0x0000ffffaed1bbb8 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) clang/lib/Driver/Driver.cpp:2242:28
#43 0x0000aaaac1b4eadc clang_main(int, char**, llvm::ToolContext const&) clang/tools/driver/driver.cpp:406:39
#44 0x0000aaaac1b84018 main tools/clang/tools/driver/clang-driver.cpp:17:20
#45 0x0000ffffa98c73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#46 0x0000ffffa98c74cc call_init ./csu/../csu/libc-start.c:128:20
#47 0x0000ffffa98c74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#48 0x0000aaaac1b4d1b0 _start (clang+++0x4d1b0)
Quit (core dumped)
make[2]: *** [src/gromacs/CMakeFiles/libgromacs.dir/build.make:11244: src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o] Error 131
make[1]: *** [CMakeFiles/Makefile2:4812: src/gromacs/CMakeFiles/libgromacs.dir/all] Error 2
make: *** [Makefile:166: all] Error 2
```


## Comments

### Comment 1 - pawosm-arm

Reverting the commit introduced by PR #144408 (with resolving merge conflicts) makes the problem go away.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Paul Osmialowski (pawosm-arm)

<details>
After the merger of the PR #<!-- -->144408 I can't compile gromacs, the compiler hangs when trying to compile this file: https://github.com/gromacs/gromacs/blob/main/src/gromacs/selection/selelem.cpp

The symptom is the `clang++` hanging forever. I've sent `SIGQUIT` to the hung process, and managed to obtain this trace:

```
[ 32%] Building CXX object src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ -DGMX_DOUBLE=1 -DHAVE_CONFIG_H -DMUPARSER_STATIC -DTMPI_EXPORTS -DTMPI_USE_VISIBILITY -DUSE_STD_INTTYPES_H -Isrc/include -Ibuild/src/include -Isrc -Isrc/external/tng_io/include -Ibuild/tng/include -Iapi/legacy/include -Ibuild/api/legacy/include -Isrc/gromacs/math/include -Isrc/gromacs/timing/include -Isrc/gromacs/utility/include -Isrc/gromacs/pbcutil/include -Isrc/gromacs/pulling/include -Isrc/gromacs/topology/include -Isrc/gromacs/serialization/include -Isrc/gromacs/linearalgebra/include -Isrc/gromacs/simd/include -Isrc/gromacs/taskassignment/include -isystem src/external/thread_mpi/include -isystem fftw/include -isystem src/external -isystem src/gromacs/../external/rpc_xdr -isystem src/external/muparser/include -isystem src/external/lmfit -isystem src/external/colvars -isystem src/external/plumed -O3 -DNDEBUG -std=c++17 -Wall -Wextra -Wpointer-arith -Wmissing-prototypes -Wpedantic -Wdeprecated -Wno-unused-function -Wno-reserved-identifier -Wno-missing-field-initializers -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-source-uses-openmp -Wno-c++17-extensions -Wno-documentation-unknown-command -Wno-covered-switch-default -Wno-switch-enum -Wno-switch-default -Wno-extra-semi-stmt -Wno-weak-vtables -Wno-shadow -Wno-padded -Wno-reserved-id-macro -Wno-double-promotion -Wno-exit-time-destructors -Wno-global-constructors -Wno-documentation -Wno-format-nonliteral -Wno-used-but-marked-unused -Wno-float-equal -Wno-conditional-uninitialized -Wno-conversion -Wno-disabled-macro-expansion -Wno-unused-macros -Wno-unsafe-buffer-usage -Wno-cast-function-type-strict -fopenmp=libomp -MD -MT src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o -MF CMakeFiles/libgromacs.dir/selection/selelem.cpp.o.d -o CMakeFiles/libgromacs.dir/selection/selelem.cpp.o -c src/gromacs/selection/selelem.cpp
1.      src/gromacs/selection/selelem.cpp:563:1: current parser token 'void'
2.      src/gromacs/selection/selelem.cpp:518:1: parsing function body '_gmx_selelem_free_method'
 #<!-- -->0 0x0000ffffaa0c4bd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x0000ffffaa0c5014 PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:918:1
 #<!-- -->2 0x0000ffffaa0c2544 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->3 0x0000ffffaa0c4374 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->4 0x0000ffffb4fe87dc (linux-vdso.so.1+0x7dc)
 #<!-- -->5 0x0000ffffa2da429c llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;, clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;::begin() llvm/include/llvm/ADT/DenseMap.h:85:3
 #<!-- -->6 0x0000ffffa2d9f0c0 decltype(fp.begin()) std::begin&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&gt;(llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;) /usr/include/c++/11/bits/range_access.h:52:27
 #<!-- -->7 0x0000ffffa2d97114 decltype(begin(forward&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;&gt;(fp))) llvm::adl_detail::begin_impl&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;&gt;(llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;) llvm/include/llvm/ADT/ADL.h:29:43
 #<!-- -->8 0x0000ffffa2d8cedc decltype(begin_impl(forward&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;&gt;(fp))) llvm::adl_begin&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;&gt;(llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;) llvm/include/llvm/ADT/ADL.h:80:60
 #<!-- -->9 0x0000ffffa2d6772c bool llvm::all_of&lt;llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;, areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&amp;, clang::AnalysisDeclContext&amp;)::'lambda'(llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;)&gt;(llvm::DenseMap&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;, llvm::DenseMapInfo&lt;clang::CFGBlock const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;&gt;&amp;, areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&amp;, clang::AnalysisDeclContext&amp;)::'lambda'(llvm::detail::DenseMapPair&lt;clang::CFGBlock const*, std::optional&lt;bool&gt;&gt;)) llvm/include/llvm/ADT/STLExtras.h:1745:21
#<!-- -->10 0x0000ffffa2d5b92c areAllValuesNoReturn(clang::VarDecl const*, clang::CFGBlock const&amp;, clang::AnalysisDeclContext&amp;) clang/lib/Sema/AnalysisBasedWarnings.cpp:525:21
#<!-- -->11 0x0000ffffa2d5bf68 CheckFallThrough(clang::AnalysisDeclContext&amp;) clang/lib/Sema/AnalysisBasedWarnings.cpp:674:37
#<!-- -->12 0x0000ffffa2d5c610 CheckFallThroughForBody(clang::Sema&amp;, clang::Decl const*, clang::Stmt const*, clang::QualType, (anonymous namespace)::CheckFallThroughDiagnostics const&amp;, clang::AnalysisDeclContext&amp;) clang/lib/Sema/AnalysisBasedWarnings.cpp:825:52
#<!-- -->13 0x0000ffffa2d6566c clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) clang/lib/Sema/AnalysisBasedWarnings.cpp:2968:9
#<!-- -->14 0x0000ffffa2ea188c clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) clang/lib/Sema/Sema.cpp:2457:35
#<!-- -->15 0x0000ffffa3239938 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) clang/lib/Sema/SemaDecl.cpp:16679:23
#<!-- -->16 0x0000ffffa3237330 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*) clang/lib/Sema/SemaDecl.cpp:16149:1
#<!-- -->17 0x0000ffffa401620c clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) clang/lib/Parse/ParseStmt.cpp:2403:41
#<!-- -->18 0x0000ffffa403c238 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) clang/lib/Parse/Parser.cpp:1449:36
#<!-- -->19 0x0000ffffa3f185f4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2263:40
#<!-- -->20 0x0000ffffa403b28c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1187:73
#<!-- -->21 0x0000ffffa403b430 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1209:42
#<!-- -->22 0x0000ffffa403a830 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1032:50
#<!-- -->23 0x0000ffffa4039998 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:745:36
#<!-- -->24 0x0000ffffa3ef31d4 clang::ParseAST(clang::Sema&amp;, bool, bool) clang/lib/Parse/ParseAST.cpp:170:37
#<!-- -->25 0x0000ffffaf5daabc clang::ASTFrontendAction::ExecuteAction() clang/lib/Frontend/FrontendAction.cpp:1342:11
#<!-- -->26 0x0000ffffb3833bd8 clang::CodeGenAction::ExecuteAction() clang/lib/CodeGen/CodeGenAction.cpp:1112:5
#<!-- -->27 0x0000ffffaf5da48c clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1226:38
#<!-- -->28 0x0000ffffaf4e0144 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->29 0x0000ffffb4296ba4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:38
#<!-- -->30 0x0000aaaac1b5c2d4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) clang/tools/driver/cc1_main.cpp:297:40
#<!-- -->31 0x0000aaaac1b4dcbc ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:223:20
#<!-- -->32 0x0000aaaac1b4de68 clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const clang/tools/driver/driver.cpp:369:7
#<!-- -->33 0x0000aaaac1b4ef34 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) llvm/include/llvm/ADT/STLFunctionalExtras.h:48:3
#<!-- -->34 0x0000ffffaed8d470 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#<!-- -->35 0x0000ffffaed8b8e8 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()::operator()() const clang/lib/Driver/Job.cpp:436:32
#<!-- -->36 0x0000ffffaed8bd04 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:47:40
#<!-- -->37 0x0000ffffa9f22cc8 llvm::function_ref&lt;void ()&gt;::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:62
#<!-- -->38 0x0000ffffa9f3c52c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#<!-- -->39 0x0000ffffaed8ba48 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const clang/lib/Driver/Job.cpp:436:21
#<!-- -->40 0x0000ffffaed01e60 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const clang/lib/Driver/Compilation.cpp:196:22
#<!-- -->41 0x0000ffffaed02214 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const clang/lib/Driver/Compilation.cpp:251:62
#<!-- -->42 0x0000ffffaed1bbb8 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) clang/lib/Driver/Driver.cpp:2242:28
#<!-- -->43 0x0000aaaac1b4eadc clang_main(int, char**, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:406:39
#<!-- -->44 0x0000aaaac1b84018 main tools/clang/tools/driver/clang-driver.cpp:17:20
#<!-- -->45 0x0000ffffa98c73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->46 0x0000ffffa98c74cc call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->47 0x0000ffffa98c74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->48 0x0000aaaac1b4d1b0 _start (clang+++0x4d1b0)
Quit (core dumped)
make[2]: *** [src/gromacs/CMakeFiles/libgromacs.dir/build.make:11244: src/gromacs/CMakeFiles/libgromacs.dir/selection/selelem.cpp.o] Error 131
make[1]: *** [CMakeFiles/Makefile2:4812: src/gromacs/CMakeFiles/libgromacs.dir/all] Error 2
make: *** [Makefile:166: all] Error 2
```

</details>


---

### Comment 3 - pawosm-arm

I've turned on the `VERBOSE` output in `make` when building gromacs and spotted this:

```
-O3 -DNDEBUG -Wall -Wextra -Wpointer-arith -Wmissing-prototypes -Wpedantic -Wdeprecated -Wno-unused-function -Wno-reserved-identifier -Wno-missing-field-initializers -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-source-uses-openmp -Wno-c++17-extensions -Wno-documentation-unknown-command -Wno-covered-switch-default -Wno-switch-enum -Wno-switch-default -Wno-extra-semi-stmt -Wno-weak-vtables -Wno-shadow -Wno-padded -Wno-reserved-id-macro -Wno-double-promotion -Wno-exit-time-destructors -Wno-global-constructors -Wno-documentation -Wno-format-nonliteral -Wno-used-but-marked-unused -Wno-float-equal -Wno-conditional-uninitialized -Wno-conversion -Wno-disabled-macro-expansion -Wno-unused-macros -Wno-unsafe-buffer-usage -Wno-cast-function-type-strict
```

The critical part of this is the `-Weverything`, mentioned in many recent bug reports.

I've extracted the offending C++ function into a small C file, `gmx_hang.c`:

```
#include <stddef.h>
#include <stdlib.h>

typedef void (*sel_freefunc)(void *);

struct gmx_ana_selmethod_t
{
  sel_freefunc free;
  int nparams;
  int *param;
};

void gmx_selelem_free_method(struct gmx_ana_selmethod_t* method, void* mdata);

void gmx_selelem_free_method(struct gmx_ana_selmethod_t* method, void* mdata)
{
    sel_freefunc free_func = NULL;

    /* Save the pointer to the free function. */
    if (method && method->free)
    {
        free_func = method->free;
    }

    /* Free the method itself.
     * Has to be done before freeing the method data, because parameter
     * values are typically stored in the method data, and here we may
     * access them. */
    if (method)
    {
        /* Free the memory allocated for the parameters that are not managed
         * by the selection method itself. */
        for (int i = 0; i < method->nparams; ++i)
        {
            free(&method->param[i]);
        }
        free(method->param);
        free(method);
    }
    /* Free method data. */
    if (mdata)
    {
        if (free_func)
            free_func(mdata);
    }
}
```

When compiling as such:

```
$ clang -Weverything -c gmx_hang.c
```

It hangs like this:

```
0.      Program arguments: clang -Weverything -c gmx_hang.c
1.      <eof> parser at end of file
2.      gmx_hang.c:16:1: parsing function body 'gmx_selelem_free_method'

```

Does not hang when replaced `-Weverything` with `-Wall`. Also does not hang after removal of the `Free method data` part of this function.


---

### Comment 4 - shafik

CC @spavloff @erichkeane 

---

