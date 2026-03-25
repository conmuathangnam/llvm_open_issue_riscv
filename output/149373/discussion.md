# clang++ ICE in DataflowAnalysis when using -Weverything

**Author:** matthew-f
**URL:** https://github.com/llvm/llvm-project/issues/149373
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, needs-reduction, regression:21
**Closed Date:** 2025-07-22T19:43:19Z

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/llvm-latest/bin/clang++ -fPIC -fdiagnostics-show-option -fsigned-char -std=c++20 -mtune=generic -g --gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13 -Wall -Wextra -Weverything -Wno-c++98-compat-pedantic -Wno-cast-align -Wno-date-time -Wno-disabled-macro-expansion -Wno-documentation-unknown-command -Wno-documentation -Wno-double-promotion -Wno-exit-time-destructors -Wno-global-constructors -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-braces -Wno-missing-noreturn -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-nested-anon-types -Wno-padded -Wno-redundant-parens -Wno-reserved-id-macro -Wno-sign-conversion -Wno-switch-enum -Wno-undefined-reinterpret-cast -Wno-unknown-pragmas -Wno-unused-member-function -Wno-unused-parameter -Wno-unused-template -Wno-used-but-marked-unused -Wno-zero-length-array -Wno-zero-as-null-pointer-constant -Wno- -Wno-c++2a-compat -Wno- -Wno-extra-semi-stmt -Wno-return-std-move -Wno- -Wno-enum-enum-conversion -Wno-suggest-destructor-override -Wno- -Wno-suggest-override -Wno-reserved-identifier -Wno-null-pointer-subtraction -Wno-unaligned-access -Wno-unsafe-buffer-usage -Wno-ctad-maybe-unsupported -Wno-switch-default -Wno-covered-switch-default -Wno-float-equal -Wno-weak-vtables -O3 -DCAT_CPP20_OPERATORS -DCAT_PSUBJECT_FOREACH -DCAT_POWNER_FOREACH -DCAT_GLIST_ITERATORS -DCAT_SUPPORT_GCM_256 -DCAT_ABSL_BACKTRACE=1 -DCAPTURESTATION -DRELEASE -DTAGGED -DVMS_BRANCH=2026 -DENT_BRANCH=8 -DPRODUCT=everything -DPLATFORM=Ubuntu_24_04 -DPLATFORM_Ubuntu_24_04=1 -DARCH=x86_64 -DARCH_x86_64=1 -DWORDSIZE64 -DBUILD_VERSION=trunk -D_FILE_OFFSET_BITS=64 -DBUILD_CORELIB -DLIB_UNITTEST_ASSETS_DIR=\"assets/unittest/core\" -Ibuild_x86_64_tagged -I./ -I../../../pkg/absl/build -isystem ../../../pkg/gsl/include -isystem ../../../pkg/absl/lib -c -fcolor-diagnostics -o build_x86_64_tagged/core_paths.o core_paths.cpp
1.      <eof> parser at end of file
2.      core_paths.cpp:122:1: parsing function body 'core::Paths::getPathName'
 #0 0x00005fc5bcc3f042 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/llvm-latest/bin/clang+++0x3c8d042)
 #1 0x00005fc5bcc3caec llvm::sys::CleanupOnSignal(unsigned long) (/usr/local/llvm-latest/bin/clang+++0x3c8aaec)
 #2 0x00005fc5bcb805f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071e450645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00005fc5bf9e59b4 clang::(anonymous namespace)::DataflowAnalysis<clang::(anonymous namespace)::LoanPropagationAnalysis, clang::(anonymous namespace)::LoanPropagationLattice, (clang::(anonymous namespace)::Direction)0>::run() LifetimeSafety.cpp:0:0
 #5 0x00005fc5bf9e8fb5 clang::runLifetimeSafetyAnalysis(clang::DeclContext const&, clang::CFG const&, clang::AnalysisDeclContext&) (/usr/local/llvm-latest/bin/clang+++0x6a36fb5)
 #6 0x00005fc5bf931406 clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/usr/local/llvm-latest/bin/clang+++0x697f406)
 #7 0x00005fc5bef710fd clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) (/usr/local/llvm-latest/bin/clang+++0x5fbf0fd)
 #8 0x00005fc5bf188bb0 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/usr/local/llvm-latest/bin/clang+++0x61d6bb0)
 #9 0x00005fc5bef093f5 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/local/llvm-latest/bin/clang+++0x5f573f5)
#10 0x00005fc5bedf9427 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/llvm-latest/bin/clang+++0x5e47427)
#11 0x00005fc5bee41129 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/llvm-latest/bin/clang+++0x5e8f129)
#12 0x00005fc5bedf2d31 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/llvm-latest/bin/clang+++0x5e40d31)
#13 0x00005fc5bedf38d3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/llvm-latest/bin/clang+++0x5e418d3)
#14 0x00005fc5bedfae7d clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/llvm-latest/bin/clang+++0x5e48e7d)
#15 0x00005fc5bedfc67d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/llvm-latest/bin/clang+++0x5e4a67d)
#16 0x00005fc5bedec1fa clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/llvm-latest/bin/clang+++0x5e3a1fa)
#17 0x00005fc5bd8387fe clang::FrontendAction::Execute() (/usr/local/llvm-latest/bin/clang+++0x48867fe)
#18 0x00005fc5bd7b71e3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/llvm-latest/bin/clang+++0x48051e3)
#19 0x00005fc5bd91395b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/llvm-latest/bin/clang+++0x496195b)
#20 0x00005fc5b9e427be cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/llvm-latest/bin/clang+++0xe907be)
#21 0x00005fc5b9e3911a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#22 0x00005fc5bd58b20d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#23 0x00005fc5bcb80a27 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/local/llvm-latest/bin/clang+++0x3bcea27)
#24 0x00005fc5bd58b901 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#25 0x00005fc5bd54fc7a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/local/llvm-latest/bin/clang+++0x459dc7a)
#26 0x00005fc5bd550cf9 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/local/llvm-latest/bin/clang+++0x459ecf9)
#27 0x00005fc5bd56561c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/local/llvm-latest/bin/clang+++0x45b361c)
#28 0x00005fc5b9e3df28 clang_main(int, char**, llvm::ToolContext const&) (/usr/local/llvm-latest/bin/clang+++0xe8bf28)
#29 0x00005fc5b9ce46ea main (/usr/local/llvm-latest/bin/clang+++0xd326ea)
#30 0x000071e45062a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x000071e45062a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x000071e45062a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00005fc5b9e38b55 _start (/usr/local/llvm-latest/bin/clang+++0xe86b55)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/matthew-f/llvm-project.git 9898c5fb92dca652a56cfdf7e21ada3437cafa56)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - matthew-f

This is the contents of the .sh file (can't seem to attach .sh files)

```
# Crash reproducer for clang version 22.0.0git (https://github.com/matthew-f/llvm-project.git 9898c5fb92dca652a56cfdf7e21ada3437cafa56)
# Driver args: "--driver-mode=g++" "-fPIC" "-fdiagnostics-show-option" "-fsigned-char" "-std=c++20" "-mtune=generic" "-g" "--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13" "-Wall" "-Wextra" "-Weverything" "-Wno-c++98-compat-pedantic" "-Wno-cast-align" "-Wno-date-time" "-Wno-disabled-macro-expansion" "-Wno-documentation-unknown-command" "-Wno-documentation" "-Wno-double-promotion" "-Wno-exit-time-destructors" "-Wno-global-constructors" "-Wno-gnu-zero-variadic-macro-arguments" "-Wno-missing-braces" "-Wno-missing-noreturn" "-Wno-missing-prototypes" "-Wno-missing-variable-declarations" "-Wno-nested-anon-types" "-Wno-padded" "-Wno-redundant-parens" "-Wno-reserved-id-macro" "-Wno-sign-conversion" "-Wno-switch-enum" "-Wno-undefined-reinterpret-cast" "-Wno-unknown-pragmas" "-Wno-unused-member-function" "-Wno-unused-parameter" "-Wno-unused-template" "-Wno-used-but-marked-unused" "-Wno-zero-length-array" "-Wno-zero-as-null-pointer-constant" "-Wno-" "-Wno-c++2a-compat" "-Wno-" "-Wno-extra-semi-stmt" "-Wno-return-std-move" "-Wno-" "-Wno-enum-enum-conversion" "-Wno-suggest-destructor-override" "-Wno-" "-Wno-suggest-override" "-Wno-reserved-identifier" "-Wno-null-pointer-subtraction" "-Wno-unaligned-access" "-Wno-unsafe-buffer-usage" "-Wno-ctad-maybe-unsupported" "-Wno-switch-default" "-Wno-covered-switch-default" "-Wno-float-equal" "-Wno-weak-vtables" "-O3" "-D" "CAT_CPP20_OPERATORS" "-D" "CAT_PSUBJECT_FOREACH" "-D" "CAT_POWNER_FOREACH" "-D" "CAT_GLIST_ITERATORS" "-D" "CAT_SUPPORT_GCM_256" "-D" "CAT_ABSL_BACKTRACE=1" "-D" "CAPTURESTATION" "-D" "RELEASE" "-D" "TAGGED" "-D" "VMS_BRANCH=2026" "-D" "ENT_BRANCH=8" "-D" "PRODUCT=everything" "-D" "PLATFORM=Ubuntu_24_04" "-D" "PLATFORM_Ubuntu_24_04=1" "-D" "ARCH=x86_64" "-D" "ARCH_x86_64=1" "-D" "WORDSIZE64" "-D" "BUILD_VERSION=trunk" "-D" "_FILE_OFFSET_BITS=64" "-D" "BUILD_CORELIB" "-D" "LIB_UNITTEST_ASSETS_DIR=\"assets/unittest/core\"" "-I" "build_x86_64_tagged" "-I" "./" "-I" "../../../pkg/absl/build" "-isystem" "../../../pkg/gsl/include" "-isystem" "../../../pkg/absl/lib" "-c" "-fcolor-diagnostics" "-o" "build_x86_64_tagged/core_paths.o" "core_paths.cpp"
# Original command:  "/usr/massstore/cathexis/llvm/github/active/bin/clang-20" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-O3" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "core_paths.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-fhalf-no-semantic-interposition" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/usr/massstore/cathexis/source/trunk/500_clang/cdl/libs/core" "-fcoverage-compilation-dir=/usr/massstore/cathexis/source/trunk/500_clang/cdl/libs/core" "-resource-dir" "/usr/massstore/cathexis/llvm/github/active/lib/clang/22" "-isystem" "../../../pkg/gsl/include" "-isystem" "../../../pkg/absl/lib" "-D" "CAT_CPP20_OPERATORS" "-D" "CAT_PSUBJECT_FOREACH" "-D" "CAT_POWNER_FOREACH" "-D" "CAT_GLIST_ITERATORS" "-D" "CAT_SUPPORT_GCM_256" "-D" "CAT_ABSL_BACKTRACE=1" "-D" "CAPTURESTATION" "-D" "RELEASE" "-D" "TAGGED" "-D" "VMS_BRANCH=2026" "-D" "ENT_BRANCH=8" "-D" "PRODUCT=everything" "-D" "PLATFORM=Ubuntu_24_04" "-D" "PLATFORM_Ubuntu_24_04=1" "-D" "ARCH=x86_64" "-D" "ARCH_x86_64=1" "-D" "WORDSIZE64" "-D" "BUILD_VERSION=trunk" "-D" "_FILE_OFFSET_BITS=64" "-D" "BUILD_CORELIB" "-D" "LIB_UNITTEST_ASSETS_DIR=\"assets/unittest/core\"" "-I" "build_x86_64_tagged" "-I" "./" "-I" "../../../pkg/absl/build" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward" "-internal-isystem" "/usr/massstore/cathexis/llvm/github/active/lib/clang/22/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wall" "-Wextra" "-Weverything" "-Wno-c++98-compat-pedantic" "-Wno-cast-align" "-Wno-date-time" "-Wno-disabled-macro-expansion" "-Wno-documentation-unknown-command" "-Wno-documentation" "-Wno-double-promotion" "-Wno-exit-time-destructors" "-Wno-global-constructors" "-Wno-gnu-zero-variadic-macro-arguments" "-Wno-missing-braces" "-Wno-missing-noreturn" "-Wno-missing-prototypes" "-Wno-missing-variable-declarations" "-Wno-nested-anon-types" "-Wno-padded" "-Wno-redundant-parens" "-Wno-reserved-id-macro" "-Wno-sign-conversion" "-Wno-switch-enum" "-Wno-undefined-reinterpret-cast" "-Wno-unknown-pragmas" "-Wno-unused-member-function" "-Wno-unused-parameter" "-Wno-unused-template" "-Wno-used-but-marked-unused" "-Wno-zero-length-array" "-Wno-zero-as-null-pointer-constant" "-Wno-" "-Wno-c++2a-compat" "-Wno-" "-Wno-extra-semi-stmt" "-Wno-return-std-move" "-Wno-" "-Wno-enum-enum-conversion" "-Wno-suggest-destructor-override" "-Wno-" "-Wno-suggest-override" "-Wno-reserved-identifier" "-Wno-null-pointer-subtraction" "-Wno-unaligned-access" "-Wno-unsafe-buffer-usage" "-Wno-ctad-maybe-unsupported" "-Wno-switch-default" "-Wno-covered-switch-default" "-Wno-float-equal" "-Wno-weak-vtables" "-std=c++20" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "build_x86_64_tagged/core_paths.o" "-x" "c++" "core_paths.cpp"
 "/usr/massstore/cathexis/llvm/github/active/bin/clang-20" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-O3" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "core_paths.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-fhalf-no-semantic-interposition" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/usr/massstore/cathexis/source/trunk/500_clang/cdl/libs/core" "-fcoverage-compilation-dir=/usr/massstore/cathexis/source/trunk/500_clang/cdl/libs/core" "-D" "CAT_CPP20_OPERATORS" "-D" "CAT_PSUBJECT_FOREACH" "-D" "CAT_POWNER_FOREACH" "-D" "CAT_GLIST_ITERATORS" "-D" "CAT_SUPPORT_GCM_256" "-D" "CAT_ABSL_BACKTRACE=1" "-D" "CAPTURESTATION" "-D" "RELEASE" "-D" "TAGGED" "-D" "VMS_BRANCH=2026" "-D" "ENT_BRANCH=8" "-D" "PRODUCT=everything" "-D" "PLATFORM=Ubuntu_24_04" "-D" "PLATFORM_Ubuntu_24_04=1" "-D" "ARCH=x86_64" "-D" "ARCH_x86_64=1" "-D" "WORDSIZE64" "-D" "BUILD_VERSION=trunk" "-D" "_FILE_OFFSET_BITS=64" "-D" "BUILD_CORELIB" "-D" "LIB_UNITTEST_ASSETS_DIR=\"assets/unittest/core\"" "-Wall" "-Wextra" "-Weverything" "-Wno-c++98-compat-pedantic" "-Wno-cast-align" "-Wno-date-time" "-Wno-disabled-macro-expansion" "-Wno-documentation-unknown-command" "-Wno-documentation" "-Wno-double-promotion" "-Wno-exit-time-destructors" "-Wno-global-constructors" "-Wno-gnu-zero-variadic-macro-arguments" "-Wno-missing-braces" "-Wno-missing-noreturn" "-Wno-missing-prototypes" "-Wno-missing-variable-declarations" "-Wno-nested-anon-types" "-Wno-padded" "-Wno-redundant-parens" "-Wno-reserved-id-macro" "-Wno-sign-conversion" "-Wno-switch-enum" "-Wno-undefined-reinterpret-cast" "-Wno-unknown-pragmas" "-Wno-unused-member-function" "-Wno-unused-parameter" "-Wno-unused-template" "-Wno-used-but-marked-unused" "-Wno-zero-length-array" "-Wno-zero-as-null-pointer-constant" "-Wno-" "-Wno-c++2a-compat" "-Wno-" "-Wno-extra-semi-stmt" "-Wno-return-std-move" "-Wno-" "-Wno-enum-enum-conversion" "-Wno-suggest-destructor-override" "-Wno-" "-Wno-suggest-override" "-Wno-reserved-identifier" "-Wno-null-pointer-subtraction" "-Wno-unaligned-access" "-Wno-unsafe-buffer-usage" "-Wno-ctad-maybe-unsupported" "-Wno-switch-default" "-Wno-covered-switch-default" "-Wno-float-equal" "-Wno-weak-vtables" "-std=c++20" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "core_paths-fb9dc0.cpp"
```


---

### Comment 2 - matthew-f

This is the contents of the cpp file to reproduce (renamed .cpp to .txt to attach)
[core_paths-fb9dc0.txt](https://github.com/user-attachments/files/21302740/core_paths-fb9dc0.txt)

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (matthew-f)

<details>
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/llvm-latest/bin/clang++ -fPIC -fdiagnostics-show-option -fsigned-char -std=c++20 -mtune=generic -g --gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13 -Wall -Wextra -Weverything -Wno-c++98-compat-pedantic -Wno-cast-align -Wno-date-time -Wno-disabled-macro-expansion -Wno-documentation-unknown-command -Wno-documentation -Wno-double-promotion -Wno-exit-time-destructors -Wno-global-constructors -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-braces -Wno-missing-noreturn -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-nested-anon-types -Wno-padded -Wno-redundant-parens -Wno-reserved-id-macro -Wno-sign-conversion -Wno-switch-enum -Wno-undefined-reinterpret-cast -Wno-unknown-pragmas -Wno-unused-member-function -Wno-unused-parameter -Wno-unused-template -Wno-used-but-marked-unused -Wno-zero-length-array -Wno-zero-as-null-pointer-constant -Wno- -Wno-c++2a-compat -Wno- -Wno-extra-semi-stmt -Wno-return-std-move -Wno- -Wno-enum-enum-conversion -Wno-suggest-destructor-override -Wno- -Wno-suggest-override -Wno-reserved-identifier -Wno-null-pointer-subtraction -Wno-unaligned-access -Wno-unsafe-buffer-usage -Wno-ctad-maybe-unsupported -Wno-switch-default -Wno-covered-switch-default -Wno-float-equal -Wno-weak-vtables -O3 -DCAT_CPP20_OPERATORS -DCAT_PSUBJECT_FOREACH -DCAT_POWNER_FOREACH -DCAT_GLIST_ITERATORS -DCAT_SUPPORT_GCM_256 -DCAT_ABSL_BACKTRACE=1 -DCAPTURESTATION -DRELEASE -DTAGGED -DVMS_BRANCH=2026 -DENT_BRANCH=8 -DPRODUCT=everything -DPLATFORM=Ubuntu_24_04 -DPLATFORM_Ubuntu_24_04=1 -DARCH=x86_64 -DARCH_x86_64=1 -DWORDSIZE64 -DBUILD_VERSION=trunk -D_FILE_OFFSET_BITS=64 -DBUILD_CORELIB -DLIB_UNITTEST_ASSETS_DIR=\"assets/unittest/core\" -Ibuild_x86_64_tagged -I./ -I../../../pkg/absl/build -isystem ../../../pkg/gsl/include -isystem ../../../pkg/absl/lib -c -fcolor-diagnostics -o build_x86_64_tagged/core_paths.o core_paths.cpp
1.      &lt;eof&gt; parser at end of file
2.      core_paths.cpp:122:1: parsing function body 'core::Paths::getPathName'
 #<!-- -->0 0x00005fc5bcc3f042 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/llvm-latest/bin/clang+++0x3c8d042)
 #<!-- -->1 0x00005fc5bcc3caec llvm::sys::CleanupOnSignal(unsigned long) (/usr/local/llvm-latest/bin/clang+++0x3c8aaec)
 #<!-- -->2 0x00005fc5bcb805f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071e450645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00005fc5bf9e59b4 clang::(anonymous namespace)::DataflowAnalysis&lt;clang::(anonymous namespace)::LoanPropagationAnalysis, clang::(anonymous namespace)::LoanPropagationLattice, (clang::(anonymous namespace)::Direction)0&gt;::run() LifetimeSafety.cpp:0:0
 #<!-- -->5 0x00005fc5bf9e8fb5 clang::runLifetimeSafetyAnalysis(clang::DeclContext const&amp;, clang::CFG const&amp;, clang::AnalysisDeclContext&amp;) (/usr/local/llvm-latest/bin/clang+++0x6a36fb5)
 #<!-- -->6 0x00005fc5bf931406 clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/usr/local/llvm-latest/bin/clang+++0x697f406)
 #<!-- -->7 0x00005fc5bef710fd clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) (/usr/local/llvm-latest/bin/clang+++0x5fbf0fd)
 #<!-- -->8 0x00005fc5bf188bb0 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/usr/local/llvm-latest/bin/clang+++0x61d6bb0)
 #<!-- -->9 0x00005fc5bef093f5 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/llvm-latest/bin/clang+++0x5f573f5)
#<!-- -->10 0x00005fc5bedf9427 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/llvm-latest/bin/clang+++0x5e47427)
#<!-- -->11 0x00005fc5bee41129 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/llvm-latest/bin/clang+++0x5e8f129)
#<!-- -->12 0x00005fc5bedf2d31 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/llvm-latest/bin/clang+++0x5e40d31)
#<!-- -->13 0x00005fc5bedf38d3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/llvm-latest/bin/clang+++0x5e418d3)
#<!-- -->14 0x00005fc5bedfae7d clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/llvm-latest/bin/clang+++0x5e48e7d)
#<!-- -->15 0x00005fc5bedfc67d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/llvm-latest/bin/clang+++0x5e4a67d)
#<!-- -->16 0x00005fc5bedec1fa clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/llvm-latest/bin/clang+++0x5e3a1fa)
#<!-- -->17 0x00005fc5bd8387fe clang::FrontendAction::Execute() (/usr/local/llvm-latest/bin/clang+++0x48867fe)
#<!-- -->18 0x00005fc5bd7b71e3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/llvm-latest/bin/clang+++0x48051e3)
#<!-- -->19 0x00005fc5bd91395b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/llvm-latest/bin/clang+++0x496195b)
#<!-- -->20 0x00005fc5b9e427be cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/llvm-latest/bin/clang+++0xe907be)
#<!-- -->21 0x00005fc5b9e3911a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->22 0x00005fc5bd58b20d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x00005fc5bcb80a27 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/local/llvm-latest/bin/clang+++0x3bcea27)
#<!-- -->24 0x00005fc5bd58b901 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x00005fc5bd54fc7a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/local/llvm-latest/bin/clang+++0x459dc7a)
#<!-- -->26 0x00005fc5bd550cf9 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/local/llvm-latest/bin/clang+++0x459ecf9)
#<!-- -->27 0x00005fc5bd56561c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/local/llvm-latest/bin/clang+++0x45b361c)
#<!-- -->28 0x00005fc5b9e3df28 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/llvm-latest/bin/clang+++0xe8bf28)
#<!-- -->29 0x00005fc5b9ce46ea main (/usr/local/llvm-latest/bin/clang+++0xd326ea)
#<!-- -->30 0x000071e45062a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x000071e45062a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x000071e45062a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00005fc5b9e38b55 _start (/usr/local/llvm-latest/bin/clang+++0xe86b55)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/matthew-f/llvm-project.git 9898c5fb92dca652a56cfdf7e21ada3437cafa56)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```
</details>


---

### Comment 4 - matthew-f

This code built successfully with llvm from yesterday, but hit this error with llvm built this morning (SAST timezone - I could probably get more accurate times if necessary). So the trigger was committed sometime yesterday or before roughly 8am SAST this morning.

---

### Comment 5 - kglindemann

This could be caused partially by using -Weverything, I have a similar (same?) problem with this simple program:
// clang frontent crash clang++ 22.0.0git: 89f09a8 :  clang++ -Weverything -c crash.cpp
```cpp
void f()
{
   for (;;)
   {
   }
}
```
// end of crash.cpp


---

### Comment 6 - cor3ntin

Confirmed https://compiler-explorer.com/z/xv3xE9o46 

@usx95 

---

### Comment 7 - usx95

Apologies for breaking `-Weverything`. This analysis is in its early days.
https://github.com/llvm/llvm-project/pull/149592 should fix this and not uninentionally turn on the analysis with `-Weverything` !
(For unblocking immediately, this can also be turned off using `-Wno-experimental-lifetime-safety` https://compiler-explorer.com/z/vMd618v6P)

---

### Comment 8 - usx95

This should be fixed with https://github.com/llvm/llvm-project/commit/0d0478903474b2e53c874427e3d6eb2ed7567e50

Filed https://github.com/llvm/llvm-project/issues/150095 to track this separately.

---

