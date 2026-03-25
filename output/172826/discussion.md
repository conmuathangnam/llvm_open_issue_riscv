# [Clang] Clang crashes when compiling tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp

**Author:** cabbaken
**URL:** https://github.com/llvm/llvm-project/issues/172826
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

The crash backtrace and files may help:
```
[43/337] Building CXX object tools/fla...s/FortranParser.dir/expr-parsers.cpp.o
FAILED: tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o 
/home/ruoyu/llvm_install/bin/clang++ -DFLANG_INCLUDE_TESTS=1 -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/include -I/home/ruoyu/my/llvm-project/build/tools/flang/include -I/home/ruoyu/my/llvm-project/build/include -I/home/ruoyu/my/llvm-project/llvm/include -isystem /home/ruoyu/my/llvm-project/flang/../mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/clang/include -isystem /home/ruoyu/my/llvm-project/llvm/../clang/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fno-omit-frame-pointer -O1 -fsanitize=address -fdiagnostics-color -Wno-deprecated-copy -Wno-string-conversion -Wno-ctad-maybe-unsupported -Wno-unused-command-line-argument -Wstring-conversion           -Wcovered-switch-default -Wno-nested-anon-types -Xclang -fno-pch-timestamp -g -DDEBUGF18  -fno-exceptions -funwind-tables -fno-rtti -std=c++17 -Winvalid-pch -Xclang -include-pch -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx -MD -MT tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -MF tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o.d -o tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -c /home/ruoyu/my/llvm-project/flang/lib/Parser/expr-parsers.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/ruoyu/llvm_install/bin/clang++ -DFLANG_INCLUDE_TESTS=1 -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/include -I/home/ruoyu/my/llvm-project/build/tools/flang/include -I/home/ruoyu/my/llvm-project/build/include -I/home/ruoyu/my/llvm-project/llvm/include -isystem /home/ruoyu/my/llvm-project/flang/../mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/clang/include -isystem /home/ruoyu/my/llvm-project/llvm/../clang/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fno-omit-frame-pointer -O1 -fsanitize=address -fdiagnostics-color -Wno-deprecated-copy -Wno-string-conversion -Wno-ctad-maybe-unsupported -Wno-unused-command-line-argument -Wstring-conversion -Wcovered-switch-default -Wno-nested-anon-types -Xclang -fno-pch-timestamp -g -DDEBUGF18 -fno-exceptions -funwind-tables -fno-rtti -std=c++17 -Winvalid-pch -Xclang -include-pch -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx -MD -MT tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -MF tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o.d -o tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -c /home/ruoyu/my/llvm-project/flang/lib/Parser/expr-parsers.cpp
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      /home/ruoyu/my/llvm-project/flang/lib/Parser/basic-parsers.h:915:13: Generating code for declaration 'Fortran::parser::SourcedParser<Fortran::parser::AlternativesParser<Fortran::parser::SequenceParser<Fortran::parser::TokenStringMatch<>, Fortran::parser::ApplyFunction<Fortran::parser::ApplicableFunctionObject, Fortran::parser::Expr, Fortran::parser::EquivOperand>>, Fortran::parser::SequenceParser<Fortran::parser::AlternativesParser<Fortran::parser::TokenStringMatch<>, Fortran::parser::NonstandardParser<Fortran::common::LanguageFeature::XOROperator, Fortran::parser::AlternativesParser<Fortran::parser::TokenStringMatch<>, Fortran::parser::NonstandardParser<Fortran::common::LanguageFeature::LogicalAbbreviations, Fortran::parser::TokenStringMatch<>>>>>, Fortran::parser::ApplyFunction<Fortran::parser::ApplicableFunctionObject, Fortran::parser::Expr, Fortran::parser::EquivOperand>>>>::SourcedParser'
 #0 0x00005596bb4faa18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/ruoyu/llvm_install/bin/clang+++0x73b6a18)
 #1 0x00005596bb4f8475 llvm::sys::RunSignalHandlers() (/home/ruoyu/llvm_install/bin/clang+++0x73b4475)
 #2 0x00005596bb46fd59 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fca5f42a520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00005596bbb48b44 clang::CodeGen::CodeGenTBAA::CollectFields(unsigned long, clang::QualType, llvm::SmallVectorImpl<llvm::MDBuilder::TBAAStructField>&, bool) (/home/ruoyu/llvm_install/bin/clang+++0x7a04b44)
 #5 0x00005596bbb48fc5 clang::CodeGen::CodeGenTBAA::getTBAAStructInfo(clang::QualType) (/home/ruoyu/llvm_install/bin/clang+++0x7a04fc5)
 #6 0x00005596bb98cc27 clang::CodeGen::CodeGenFunction::EmitAggregateCopy(clang::CodeGen::LValue, clang::CodeGen::LValue, clang::QualType, clang::CodeGen::AggValueSlot::Overlap_t, bool) (/home/ruoyu/llvm_install/bin/clang+++0x7848c27)
 #7 0x00005596bb839511 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::AggValueSlot, clang::CXXConstructExpr const*) (/home/ruoyu/llvm_install/bin/clang+++0x76f5511)
 #8 0x00005596bb9e48f5 clang::CodeGen::CodeGenFunction::EmitCXXConstructExpr(clang::CXXConstructExpr const*, clang::CodeGen::AggValueSlot) (/home/ruoyu/llvm_install/bin/clang+++0x78a08f5)
 #9 0x00005596bb9922d7 (anonymous namespace)::AggExprEmitter::VisitCXXConstructExpr(clang::CXXConstructExpr const*) CGExprAgg.cpp:0:0
#10 0x00005596bb98b9fe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/home/ruoyu/llvm_install/bin/clang+++0x78479fe)
#11 0x00005596bb834806 clang::CodeGen::CodeGenFunction::EmitInitializerForField(clang::FieldDecl*, clang::CodeGen::LValue, clang::Expr*) (/home/ruoyu/llvm_install/bin/clang+++0x76f0806)
#12 0x00005596bb83f4fa EmitMemberInitializer(clang::CodeGen::CodeGenFunction&, clang::CXXRecordDecl const*, clang::CXXCtorInitializer*, clang::CXXConstructorDecl const*, clang::CodeGen::FunctionArgList&) CGClass.cpp:0:0
#13 0x00005596bb835c36 clang::CodeGen::CodeGenFunction::EmitCtorPrologue(clang::CXXConstructorDecl const*, clang::CXXCtorType, clang::CodeGen::FunctionArgList&) (/home/ruoyu/llvm_install/bin/clang+++0x76f1c36)
#14 0x00005596bb835070 clang::CodeGen::CodeGenFunction::EmitConstructorBody(clang::CodeGen::FunctionArgList&) (/home/ruoyu/llvm_install/bin/clang+++0x76f1070)
#15 0x00005596bb799844 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/home/ruoyu/llvm_install/bin/clang+++0x7655844)
#16 0x00005596bb9ee0fc clang::CodeGen::CodeGenModule::codegenCXXStructor(clang::GlobalDecl) (/home/ruoyu/llvm_install/bin/clang+++0x78aa0fc)
#17 0x00005596bb73fe15 (anonymous namespace)::ItaniumCXXABI::emitCXXStructor(clang::GlobalDecl) ItaniumCXXABI.cpp:0:0
#18 0x00005596bb6f4b5d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/ruoyu/llvm_install/bin/clang+++0x75b0b5d)
#19 0x00005596bb6e7a12 clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a12)
#20 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#21 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#22 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#23 0x00005596bb6e49ea clang::CodeGen::CodeGenModule::Release() (/home/ruoyu/llvm_install/bin/clang+++0x75a09ea)
#24 0x00005596bbb72c4b (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#25 0x00005596bbb6c688 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/ruoyu/llvm_install/bin/clang+++0x7a28688)
#26 0x00005596bd467d79 clang::ParseAST(clang::Sema&, bool, bool) (/home/ruoyu/llvm_install/bin/clang+++0x9323d79)
#27 0x00005596bc0021dd clang::FrontendAction::Execute() (/home/ruoyu/llvm_install/bin/clang+++0x7ebe1dd)
#28 0x00005596bbf72ea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/ruoyu/llvm_install/bin/clang+++0x7e2eea4)
#29 0x00005596bc0d7db9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/ruoyu/llvm_install/bin/clang+++0x7f93db9)
#30 0x00005596b912df61 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/ruoyu/llvm_install/bin/clang+++0x4fe9f61)
#31 0x00005596b912a947 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x00005596b912c1ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x00005596bbdfb5b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#34 0x00005596bb46fa98 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/ruoyu/llvm_install/bin/clang+++0x732ba98)
#35 0x00005596bbdfaf65 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/home/ruoyu/llvm_install/bin/clang+++0x7cb6f65)
#36 0x00005596bbdc026f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/ruoyu/llvm_install/bin/clang+++0x7c7c26f)
#37 0x00005596bbdc040e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/ruoyu/llvm_install/bin/clang+++0x7c7c40e)
#38 0x00005596bbdd80fd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/ruoyu/llvm_install/bin/clang+++0x7c940fd)
#39 0x00005596b912a319 clang_main(int, char**, llvm::ToolContext const&) (/home/ruoyu/llvm_install/bin/clang+++0x4fe6319)
#40 0x00005596b91388c7 main (/home/ruoyu/llvm_install/bin/clang+++0x4ff48c7)
#41 0x00007fca5f411d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#42 0x00007fca5f411e40 call_init ./csu/../csu/libc-start.c:128:20
#43 0x00007fca5f411e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#44 0x00005596b9128525 _start (/home/ruoyu/llvm_install/bin/clang+++0x4fe4525)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/ruoyu/llvm_install/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/expr-parsers-44a349.cpp
clang++: note: diagnostic msg: /tmp/expr-parsers-44a349.sh
```
[expr-parsers-44a349.sh](https://github.com/user-attachments/files/24232093/expr-parsers-44a349.sh)
[expr-parsers-44a349.cpp](https://github.com/user-attachments/files/24232092/expr-parsers-44a349.cpp)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Ruoyu Qiu (cabbaken)

<details>
The crash backtrace and files may help:
```
[43/337] Building CXX object tools/fla...s/FortranParser.dir/expr-parsers.cpp.o
FAILED: tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o 
/home/ruoyu/llvm_install/bin/clang++ -DFLANG_INCLUDE_TESTS=1 -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/include -I/home/ruoyu/my/llvm-project/build/tools/flang/include -I/home/ruoyu/my/llvm-project/build/include -I/home/ruoyu/my/llvm-project/llvm/include -isystem /home/ruoyu/my/llvm-project/flang/../mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/clang/include -isystem /home/ruoyu/my/llvm-project/llvm/../clang/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fno-omit-frame-pointer -O1 -fsanitize=address -fdiagnostics-color -Wno-deprecated-copy -Wno-string-conversion -Wno-ctad-maybe-unsupported -Wno-unused-command-line-argument -Wstring-conversion           -Wcovered-switch-default -Wno-nested-anon-types -Xclang -fno-pch-timestamp -g -DDEBUGF18  -fno-exceptions -funwind-tables -fno-rtti -std=c++17 -Winvalid-pch -Xclang -include-pch -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx -MD -MT tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -MF tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o.d -o tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -c /home/ruoyu/my/llvm-project/flang/lib/Parser/expr-parsers.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/ruoyu/llvm_install/bin/clang++ -DFLANG_INCLUDE_TESTS=1 -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/lib/Parser -I/home/ruoyu/my/llvm-project/flang/include -I/home/ruoyu/my/llvm-project/build/tools/flang/include -I/home/ruoyu/my/llvm-project/build/include -I/home/ruoyu/my/llvm-project/llvm/include -isystem /home/ruoyu/my/llvm-project/flang/../mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/mlir/include -isystem /home/ruoyu/my/llvm-project/build/tools/clang/include -isystem /home/ruoyu/my/llvm-project/llvm/../clang/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fno-omit-frame-pointer -O1 -fsanitize=address -fdiagnostics-color -Wno-deprecated-copy -Wno-string-conversion -Wno-ctad-maybe-unsupported -Wno-unused-command-line-argument -Wstring-conversion -Wcovered-switch-default -Wno-nested-anon-types -Xclang -fno-pch-timestamp -g -DDEBUGF18 -fno-exceptions -funwind-tables -fno-rtti -std=c++17 -Winvalid-pch -Xclang -include-pch -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/ruoyu/my/llvm-project/build/tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/cmake_pch.hxx -MD -MT tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -MF tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o.d -o tools/flang/lib/Parser/CMakeFiles/FortranParser.dir/expr-parsers.cpp.o -c /home/ruoyu/my/llvm-project/flang/lib/Parser/expr-parsers.cpp
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      /home/ruoyu/my/llvm-project/flang/lib/Parser/basic-parsers.h:915:13: Generating code for declaration 'Fortran::parser::SourcedParser&lt;Fortran::parser::AlternativesParser&lt;Fortran::parser::SequenceParser&lt;Fortran::parser::TokenStringMatch&lt;&gt;, Fortran::parser::ApplyFunction&lt;Fortran::parser::ApplicableFunctionObject, Fortran::parser::Expr, Fortran::parser::EquivOperand&gt;&gt;, Fortran::parser::SequenceParser&lt;Fortran::parser::AlternativesParser&lt;Fortran::parser::TokenStringMatch&lt;&gt;, Fortran::parser::NonstandardParser&lt;Fortran::common::LanguageFeature::XOROperator, Fortran::parser::AlternativesParser&lt;Fortran::parser::TokenStringMatch&lt;&gt;, Fortran::parser::NonstandardParser&lt;Fortran::common::LanguageFeature::LogicalAbbreviations, Fortran::parser::TokenStringMatch&lt;&gt;&gt;&gt;&gt;&gt;, Fortran::parser::ApplyFunction&lt;Fortran::parser::ApplicableFunctionObject, Fortran::parser::Expr, Fortran::parser::EquivOperand&gt;&gt;&gt;&gt;::SourcedParser'
 #<!-- -->0 0x00005596bb4faa18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/ruoyu/llvm_install/bin/clang+++0x73b6a18)
 #<!-- -->1 0x00005596bb4f8475 llvm::sys::RunSignalHandlers() (/home/ruoyu/llvm_install/bin/clang+++0x73b4475)
 #<!-- -->2 0x00005596bb46fd59 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fca5f42a520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00005596bbb48b44 clang::CodeGen::CodeGenTBAA::CollectFields(unsigned long, clang::QualType, llvm::SmallVectorImpl&lt;llvm::MDBuilder::TBAAStructField&gt;&amp;, bool) (/home/ruoyu/llvm_install/bin/clang+++0x7a04b44)
 #<!-- -->5 0x00005596bbb48fc5 clang::CodeGen::CodeGenTBAA::getTBAAStructInfo(clang::QualType) (/home/ruoyu/llvm_install/bin/clang+++0x7a04fc5)
 #<!-- -->6 0x00005596bb98cc27 clang::CodeGen::CodeGenFunction::EmitAggregateCopy(clang::CodeGen::LValue, clang::CodeGen::LValue, clang::QualType, clang::CodeGen::AggValueSlot::Overlap_t, bool) (/home/ruoyu/llvm_install/bin/clang+++0x7848c27)
 #<!-- -->7 0x00005596bb839511 clang::CodeGen::CodeGenFunction::EmitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CodeGen::AggValueSlot, clang::CXXConstructExpr const*) (/home/ruoyu/llvm_install/bin/clang+++0x76f5511)
 #<!-- -->8 0x00005596bb9e48f5 clang::CodeGen::CodeGenFunction::EmitCXXConstructExpr(clang::CXXConstructExpr const*, clang::CodeGen::AggValueSlot) (/home/ruoyu/llvm_install/bin/clang+++0x78a08f5)
 #<!-- -->9 0x00005596bb9922d7 (anonymous namespace)::AggExprEmitter::VisitCXXConstructExpr(clang::CXXConstructExpr const*) CGExprAgg.cpp:0:0
#<!-- -->10 0x00005596bb98b9fe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/home/ruoyu/llvm_install/bin/clang+++0x78479fe)
#<!-- -->11 0x00005596bb834806 clang::CodeGen::CodeGenFunction::EmitInitializerForField(clang::FieldDecl*, clang::CodeGen::LValue, clang::Expr*) (/home/ruoyu/llvm_install/bin/clang+++0x76f0806)
#<!-- -->12 0x00005596bb83f4fa EmitMemberInitializer(clang::CodeGen::CodeGenFunction&amp;, clang::CXXRecordDecl const*, clang::CXXCtorInitializer*, clang::CXXConstructorDecl const*, clang::CodeGen::FunctionArgList&amp;) CGClass.cpp:0:0
#<!-- -->13 0x00005596bb835c36 clang::CodeGen::CodeGenFunction::EmitCtorPrologue(clang::CXXConstructorDecl const*, clang::CXXCtorType, clang::CodeGen::FunctionArgList&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x76f1c36)
#<!-- -->14 0x00005596bb835070 clang::CodeGen::CodeGenFunction::EmitConstructorBody(clang::CodeGen::FunctionArgList&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x76f1070)
#<!-- -->15 0x00005596bb799844 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x7655844)
#<!-- -->16 0x00005596bb9ee0fc clang::CodeGen::CodeGenModule::codegenCXXStructor(clang::GlobalDecl) (/home/ruoyu/llvm_install/bin/clang+++0x78aa0fc)
#<!-- -->17 0x00005596bb73fe15 (anonymous namespace)::ItaniumCXXABI::emitCXXStructor(clang::GlobalDecl) ItaniumCXXABI.cpp:0:0
#<!-- -->18 0x00005596bb6f4b5d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/ruoyu/llvm_install/bin/clang+++0x75b0b5d)
#<!-- -->19 0x00005596bb6e7a12 clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a12)
#<!-- -->20 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#<!-- -->21 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#<!-- -->22 0x00005596bb6e7a2e clang::CodeGen::CodeGenModule::EmitDeferred() (/home/ruoyu/llvm_install/bin/clang+++0x75a3a2e)
#<!-- -->23 0x00005596bb6e49ea clang::CodeGen::CodeGenModule::Release() (/home/ruoyu/llvm_install/bin/clang+++0x75a09ea)
#<!-- -->24 0x00005596bbb72c4b (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->25 0x00005596bbb6c688 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x7a28688)
#<!-- -->26 0x00005596bd467d79 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/ruoyu/llvm_install/bin/clang+++0x9323d79)
#<!-- -->27 0x00005596bc0021dd clang::FrontendAction::Execute() (/home/ruoyu/llvm_install/bin/clang+++0x7ebe1dd)
#<!-- -->28 0x00005596bbf72ea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x7e2eea4)
#<!-- -->29 0x00005596bc0d7db9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/ruoyu/llvm_install/bin/clang+++0x7f93db9)
#<!-- -->30 0x00005596b912df61 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/ruoyu/llvm_install/bin/clang+++0x4fe9f61)
#<!-- -->31 0x00005596b912a947 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x00005596b912c1ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x00005596bbdfb5b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00005596bb46fa98 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/ruoyu/llvm_install/bin/clang+++0x732ba98)
#<!-- -->35 0x00005596bbdfaf65 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/home/ruoyu/llvm_install/bin/clang+++0x7cb6f65)
#<!-- -->36 0x00005596bbdc026f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/ruoyu/llvm_install/bin/clang+++0x7c7c26f)
#<!-- -->37 0x00005596bbdc040e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/ruoyu/llvm_install/bin/clang+++0x7c7c40e)
#<!-- -->38 0x00005596bbdd80fd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x7c940fd)
#<!-- -->39 0x00005596b912a319 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/ruoyu/llvm_install/bin/clang+++0x4fe6319)
#<!-- -->40 0x00005596b91388c7 main (/home/ruoyu/llvm_install/bin/clang+++0x4ff48c7)
#<!-- -->41 0x00007fca5f411d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->42 0x00007fca5f411e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->43 0x00007fca5f411e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->44 0x00005596b9128525 _start (/home/ruoyu/llvm_install/bin/clang+++0x4fe4525)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/ruoyu/llvm_install/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/expr-parsers-44a349.cpp
clang++: note: diagnostic msg: /tmp/expr-parsers-44a349.sh
```
[expr-parsers-44a349.sh](https://github.com/user-attachments/files/24232093/expr-parsers-44a349.sh)
[expr-parsers-44a349.cpp](https://github.com/user-attachments/files/24232092/expr-parsers-44a349.cpp)
</details>


---

