# Crash in c++20 code when lambda is present

**Author:** emmenlau
**URL:** https://github.com/llvm/llvm-project/issues/126938

## Body

Clang kindly asked me to create this bug report:
```
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang++ -DBDADATABASEHANDLER_DEBUG -DBDADATABASEHANDLER_HAVE_POSTGRESQL -DBDADATABASEHANDLER_HAVE_SQLITE -DBDAHDF5_DEBUG -DBDAIMAGEANALYSIS_HAVE_BDAIMAGEFILEHANDLER=1 -DBDAIMAGEANALYSIS_HAVE_BLAS=1 -DBDAIMAGEANALYSIS_HAVE_FFTW3=0 -DBDAIMAGEANALYSIS_HAVE_FFTW3XC=1 -DBDAIMAGEANALYSIS_HAVE_IPP=1 -DBDAIMAGEANALYSIS_HAVE_ITK=1 -DBDAIMAGEANALYSIS_HAVE_OPENCV=1 -DBDAIMAGEANALYSIS_HAVE_RANGES=0 -DBDAIMAGEANALYSIS_HAVE_VIGRA=1 -DBDAIMAGEFILEHANDLER_HAVE_BDAHDF5=1 -DBDAIMAGEFILEHANDLER_HAVE_BIOIMAGE=1 -DBDAPIPELINEPROCESSING_DEBUG -DBDAPIPELINEPROCESSING_ENABLE_THREAD_SAFETY=0 -DBDAPIPELINEPROCESSING_HAVE_QT=1 -DBDAQTBASEGUI_HAVE_PDFWRITER=1 -DBDAQTBASE_HAVE_QT_GUI=1 -DBDASAMPLEHANDLING_DEBUG -DBDASAMPLEHANDLING_THREAD_SAFETY_IMPLEMENTATION=2 -DBDA_HAVE_BIOIMAGE -DBDA_HAVE_BOOST -DBDA_HAVE_EIGEN3 -DBDA_HAVE_IPP -DBDA_HAVE_ITK -DBDA_HAVE_LEMON -DBDA_HAVE_MKL -DBDA_HAVE_OPENCV -DBDA_HAVE_VIGRA -DBDA_HISTOGRAM_BOUNDS_CHECK -DBIC_ENABLE_THREADSAFE -DBIM_COMPOSITE_FORMAT -DBIM_GEOTIFF_FORMAT -DBIM_HDF5_FORMAT -DBIM_JP2_FORMAT -DBIM_ND2_FORMAT -DBIM_NIFTI_FORMAT -DBIM_PNG_FORMAT -DBIM_PROJ4_FORMAT -DBIM_TIFF_FORMAT -DBIM_USE_CODECVT -DBIM_USE_EIGEN -DBOOST_ALL_NO_LIB -DBOOST_ALL_NO_LIB=1 -DBOOST_DATE_TIME_NO_LIB -DBOOST_FILESYSTEM_NO_DEPRECATED=1 -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_VERSION=4 -DBOOST_NOWIDE_NO_LIB -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_SYSTEM_NO_LIB -DDEBUG -DFFTWMKLWRAPPERS_ENABLE_CUSTOM_PREFIX -DH5_USE_BOOST -DH5_USE_EIGEN -DH5_USE_XTENSOR -DHAVE_CBLAS=1 -DHIGHFIVE_HAS_CONCEPTS=1 -DHasZLIB -DMPI_NO_CPPBIND -DOPENCV_DISABLE_THREAD_SUPPORT=1 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_QML_DEBUG -DSQLITE_ENABLE_COLUMN_METADATA -DTBB_DEPRECATED=0 -DTBB_USE_CAPTURED_EXCEPTION=0 -DTBB_USE_DEBUG -DVIGRA_STATIC_LIB -DXSIMD_ENABLE_XTL_COMPLEX=1 -DXTENSOR_ENABLE_ASSERT -DXTENSOR_USE_XSIMD -D_DEBUG -D_DEBUG_ -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_POSIX_C_SOURCE=200809L -D_USE_MATH_DEFINES -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing/BDAPipelineProcessing_autogen/include -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/include -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/ipp/2021.5.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/eigen3 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/ITK-5.4 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include/fftw -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/openjpeg-2.5 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/server -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtGui -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtCore -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/mkspecs/linux-clang -DDEBUG -DQT_DISABLE_DEPRECATED_BEFORE=0x050F00 -Wall -pedantic -Wextra -Werror=return-type -Werror=uninitialized -Werror=unknown-pragmas -Werror=shadow -Werror=undef -Werror=pointer-arith -Werror=shift-count-overflow -Werror=overloaded-virtual -Werror=old-style-cast -Werror=format -Werror=comment -Werror=char-subscripts -Werror=implicit-int -Werror=implicit-function-declaration -Werror=parentheses -Werror=nan-infinity-disabled -Werror=float-equal -Werror=implicit-int-float-conversion -Werror=reserved-identifier -Werror=reserved-macro-identifier -Werror=cast-qual -Werror=string-plus-int -Werror=nonportable-include-path -Werror=inconsistent-missing-override -Werror=self-assign -Werror=self-assign-field -Werror=self-move -Werror=braced-scalar-init -Werror=many-braces-around-scalar-init -Werror=deprecated-enum-enum-conversion -Werror=infinite-recursion -Werror=exceptions -Werror=excess-initializers -Werror=return-stack-address -Werror=delete-non-abstract-non-virtual-dtor -march=skylake -std=gnu++20 -O0 -glldb -fno-omit-frame-pointer -ftemplate-backtrace-limit=40 -ftime-trace -Werror=pessimizing-move -Werror=redundant-move -Werror=range-loop-construct -g -std=gnu++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -MD -MT CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -MF CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o.d -o CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -c /home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc
[build] 1.	<eof> parser at end of file
[build] 2.	/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc:57:41: instantiating function definition 'bda::PipelineActionImageMath::process(bda::pipe::PipelineProcessorIterationStatus &)::(anonymous class)::operator()<xt::xtensor_container<xt::uvector<double, xsimd::aligned_allocator<double, 32>>, 2, xt::layout_type::row_major>>'
[build]  #0 0x0000773356ebd42f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd42f)
[build]  #1 0x0000773356ebb139 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebb139)
[build]  #2 0x0000773356e04227 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe04227)
[build]  #3 0x0000773355845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
[build]  #4 0x000077335f4f4b05 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18f4b05)
[build]  #5 0x000077335f4c08a6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c08a6)
[build]  #6 0x000077335f4c0748 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c0748)
[build]  #7 0x000077335fa044cc (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e044cc)
[build]  #8 0x000077335fa0e3ad (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0e3ad)
[build]  #9 0x000077335f9e01d1 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1de01d1)
[build] #10 0x000077335f30cb8c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170cb8c)
[build] #11 0x000077335f30b9ca (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170b9ca)
[build] #12 0x000077335f304a25 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1704a25)
[build] #13 0x000077335f30477e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170477e)
[build] #14 0x000077335f3078d4 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x17078d4)
[build] #15 0x000077335f926678 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d26678)
[build] #16 0x000077335f983ba7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d83ba7)
[build] #17 0x000077335f1bc160 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15bc160)
[build] #18 0x000077335f9280f0 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d280f0)
[build] #19 0x000077335f809d66 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c09d66)
[build] #20 0x000077335f81a2bd (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a2bd)
[build] #21 0x000077335f81a17e clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a17e)
[build] #22 0x000077335f81a4dc clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a4dc)
[build] #23 0x000077335f81a7dd clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a7dd)
[build] #24 0x000077335f4bb707 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18bb707)
[build] #25 0x000077335f4cf5d1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18cf5d1)
[build] #26 0x000077335fa0a661 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0a661)
[build] #27 0x000077335f9de737 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde737)
[build] #28 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #29 0x000077335fa25f2d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e25f2d)
[build] #30 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #31 0x000077335f9de6c3 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde6c3)
[build] #32 0x000077335fa46451 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e46451)
[build] #33 0x000077335fa48bfe clang::Sema::PerformPendingInstantiations(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e48bfe)
[build] #34 0x000077335f1be023 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be023)
[build] #35 0x000077335f1be6a0 clang::Sema::ActOnEndOfTranslationUnit() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be6a0)
[build] #36 0x000077335e91f554 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd1f554)
[build] #37 0x000077335e84bb8e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4bb8e)
[build] #38 0x000077336097b67c clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b67c)
[build] #39 0x00007733608ee9f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cee9f4)
[build] #40 0x00007733609fad2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dfad2c)
[build] #41 0x000060731c5041d2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0x131d2)
[build] #42 0x000060731c5011a4 (/usr/bin/clang+++0x101a4)
[build] #43 0x000077336057d272 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297d272)
[build] #44 0x0000773356e03f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe03f77)
[build] #45 0x000077336057cb06 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297cb06)
[build] #46 0x0000773360541f89 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2941f89)
[build] #47 0x00007733605421ff clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29421ff)
[build] #48 0x000077336055f4d0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x295f4d0)
[build] #49 0x000060731c500bcb clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang+++0xfbcb)
[build] #50 0x000060731c50e703 main (/usr/bin/clang+++0x1d703)
[build] #51 0x000077335582a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #52 0x000077335582a28b call_init ./csu/../csu/libc-start.c:128:20
[build] #53 0x000077335582a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #54 0x000060731c4feda5 _start (/usr/bin/clang+++0xdda5)
[build] clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
[build] Ubuntu clang version 19.1.7 (++20250114103332+cd708029e0b2-1~exp1~20250114103446.78)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/lib/llvm-19/bin
[build] clang++: note: diagnostic msg: 
```


## Comments

### Comment 1 - emmenlau

[clang-crash-files.zip](https://github.com/user-attachments/files/18771067/clang-crash-files.zip)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mario Emmenlauer (emmenlau)

<details>
Clang kindly asked me to create this bug report:
```
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang++ -DBDADATABASEHANDLER_DEBUG -DBDADATABASEHANDLER_HAVE_POSTGRESQL -DBDADATABASEHANDLER_HAVE_SQLITE -DBDAHDF5_DEBUG -DBDAIMAGEANALYSIS_HAVE_BDAIMAGEFILEHANDLER=1 -DBDAIMAGEANALYSIS_HAVE_BLAS=1 -DBDAIMAGEANALYSIS_HAVE_FFTW3=0 -DBDAIMAGEANALYSIS_HAVE_FFTW3XC=1 -DBDAIMAGEANALYSIS_HAVE_IPP=1 -DBDAIMAGEANALYSIS_HAVE_ITK=1 -DBDAIMAGEANALYSIS_HAVE_OPENCV=1 -DBDAIMAGEANALYSIS_HAVE_RANGES=0 -DBDAIMAGEANALYSIS_HAVE_VIGRA=1 -DBDAIMAGEFILEHANDLER_HAVE_BDAHDF5=1 -DBDAIMAGEFILEHANDLER_HAVE_BIOIMAGE=1 -DBDAPIPELINEPROCESSING_DEBUG -DBDAPIPELINEPROCESSING_ENABLE_THREAD_SAFETY=0 -DBDAPIPELINEPROCESSING_HAVE_QT=1 -DBDAQTBASEGUI_HAVE_PDFWRITER=1 -DBDAQTBASE_HAVE_QT_GUI=1 -DBDASAMPLEHANDLING_DEBUG -DBDASAMPLEHANDLING_THREAD_SAFETY_IMPLEMENTATION=2 -DBDA_HAVE_BIOIMAGE -DBDA_HAVE_BOOST -DBDA_HAVE_EIGEN3 -DBDA_HAVE_IPP -DBDA_HAVE_ITK -DBDA_HAVE_LEMON -DBDA_HAVE_MKL -DBDA_HAVE_OPENCV -DBDA_HAVE_VIGRA -DBDA_HISTOGRAM_BOUNDS_CHECK -DBIC_ENABLE_THREADSAFE -DBIM_COMPOSITE_FORMAT -DBIM_GEOTIFF_FORMAT -DBIM_HDF5_FORMAT -DBIM_JP2_FORMAT -DBIM_ND2_FORMAT -DBIM_NIFTI_FORMAT -DBIM_PNG_FORMAT -DBIM_PROJ4_FORMAT -DBIM_TIFF_FORMAT -DBIM_USE_CODECVT -DBIM_USE_EIGEN -DBOOST_ALL_NO_LIB -DBOOST_ALL_NO_LIB=1 -DBOOST_DATE_TIME_NO_LIB -DBOOST_FILESYSTEM_NO_DEPRECATED=1 -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_VERSION=4 -DBOOST_NOWIDE_NO_LIB -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_SYSTEM_NO_LIB -DDEBUG -DFFTWMKLWRAPPERS_ENABLE_CUSTOM_PREFIX -DH5_USE_BOOST -DH5_USE_EIGEN -DH5_USE_XTENSOR -DHAVE_CBLAS=1 -DHIGHFIVE_HAS_CONCEPTS=1 -DHasZLIB -DMPI_NO_CPPBIND -DOPENCV_DISABLE_THREAD_SUPPORT=1 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_QML_DEBUG -DSQLITE_ENABLE_COLUMN_METADATA -DTBB_DEPRECATED=0 -DTBB_USE_CAPTURED_EXCEPTION=0 -DTBB_USE_DEBUG -DVIGRA_STATIC_LIB -DXSIMD_ENABLE_XTL_COMPLEX=1 -DXTENSOR_ENABLE_ASSERT -DXTENSOR_USE_XSIMD -D_DEBUG -D_DEBUG_ -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_POSIX_C_SOURCE=200809L -D_USE_MATH_DEFINES -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing/BDAPipelineProcessing_autogen/include -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/include -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/ipp/2021.5.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/eigen3 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/ITK-5.4 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include/fftw -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/openjpeg-2.5 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/server -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtGui -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtCore -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/mkspecs/linux-clang -DDEBUG -DQT_DISABLE_DEPRECATED_BEFORE=0x050F00 -Wall -pedantic -Wextra -Werror=return-type -Werror=uninitialized -Werror=unknown-pragmas -Werror=shadow -Werror=undef -Werror=pointer-arith -Werror=shift-count-overflow -Werror=overloaded-virtual -Werror=old-style-cast -Werror=format -Werror=comment -Werror=char-subscripts -Werror=implicit-int -Werror=implicit-function-declaration -Werror=parentheses -Werror=nan-infinity-disabled -Werror=float-equal -Werror=implicit-int-float-conversion -Werror=reserved-identifier -Werror=reserved-macro-identifier -Werror=cast-qual -Werror=string-plus-int -Werror=nonportable-include-path -Werror=inconsistent-missing-override -Werror=self-assign -Werror=self-assign-field -Werror=self-move -Werror=braced-scalar-init -Werror=many-braces-around-scalar-init -Werror=deprecated-enum-enum-conversion -Werror=infinite-recursion -Werror=exceptions -Werror=excess-initializers -Werror=return-stack-address -Werror=delete-non-abstract-non-virtual-dtor -march=skylake -std=gnu++20 -O0 -glldb -fno-omit-frame-pointer -ftemplate-backtrace-limit=40 -ftime-trace -Werror=pessimizing-move -Werror=redundant-move -Werror=range-loop-construct -g -std=gnu++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -MD -MT CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -MF CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o.d -o CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -c /home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc:57:41: instantiating function definition 'bda::PipelineActionImageMath::process(bda::pipe::PipelineProcessorIterationStatus &amp;)::(anonymous class)::operator()&lt;xt::xtensor_container&lt;xt::uvector&lt;double, xsimd::aligned_allocator&lt;double, 32&gt;&gt;, 2, xt::layout_type::row_major&gt;&gt;'
[build]  #<!-- -->0 0x0000773356ebd42f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd42f)
[build]  #<!-- -->1 0x0000773356ebb139 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebb139)
[build]  #<!-- -->2 0x0000773356e04227 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe04227)
[build]  #<!-- -->3 0x0000773355845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
[build]  #<!-- -->4 0x000077335f4f4b05 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18f4b05)
[build]  #<!-- -->5 0x000077335f4c08a6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c08a6)
[build]  #<!-- -->6 0x000077335f4c0748 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c0748)
[build]  #<!-- -->7 0x000077335fa044cc (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e044cc)
[build]  #<!-- -->8 0x000077335fa0e3ad (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0e3ad)
[build]  #<!-- -->9 0x000077335f9e01d1 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1de01d1)
[build] #<!-- -->10 0x000077335f30cb8c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170cb8c)
[build] #<!-- -->11 0x000077335f30b9ca (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170b9ca)
[build] #<!-- -->12 0x000077335f304a25 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1704a25)
[build] #<!-- -->13 0x000077335f30477e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170477e)
[build] #<!-- -->14 0x000077335f3078d4 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x17078d4)
[build] #<!-- -->15 0x000077335f926678 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d26678)
[build] #<!-- -->16 0x000077335f983ba7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d83ba7)
[build] #<!-- -->17 0x000077335f1bc160 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15bc160)
[build] #<!-- -->18 0x000077335f9280f0 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d280f0)
[build] #<!-- -->19 0x000077335f809d66 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c09d66)
[build] #<!-- -->20 0x000077335f81a2bd (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a2bd)
[build] #<!-- -->21 0x000077335f81a17e clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a17e)
[build] #<!-- -->22 0x000077335f81a4dc clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a4dc)
[build] #<!-- -->23 0x000077335f81a7dd clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a7dd)
[build] #<!-- -->24 0x000077335f4bb707 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18bb707)
[build] #<!-- -->25 0x000077335f4cf5d1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18cf5d1)
[build] #<!-- -->26 0x000077335fa0a661 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0a661)
[build] #<!-- -->27 0x000077335f9de737 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde737)
[build] #<!-- -->28 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #<!-- -->29 0x000077335fa25f2d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e25f2d)
[build] #<!-- -->30 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #<!-- -->31 0x000077335f9de6c3 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde6c3)
[build] #<!-- -->32 0x000077335fa46451 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e46451)
[build] #<!-- -->33 0x000077335fa48bfe clang::Sema::PerformPendingInstantiations(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e48bfe)
[build] #<!-- -->34 0x000077335f1be023 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be023)
[build] #<!-- -->35 0x000077335f1be6a0 clang::Sema::ActOnEndOfTranslationUnit() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be6a0)
[build] #<!-- -->36 0x000077335e91f554 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd1f554)
[build] #<!-- -->37 0x000077335e84bb8e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4bb8e)
[build] #<!-- -->38 0x000077336097b67c clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b67c)
[build] #<!-- -->39 0x00007733608ee9f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cee9f4)
[build] #<!-- -->40 0x00007733609fad2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dfad2c)
[build] #<!-- -->41 0x000060731c5041d2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x131d2)
[build] #<!-- -->42 0x000060731c5011a4 (/usr/bin/clang+++0x101a4)
[build] #<!-- -->43 0x000077336057d272 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297d272)
[build] #<!-- -->44 0x0000773356e03f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe03f77)
[build] #<!-- -->45 0x000077336057cb06 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297cb06)
[build] #<!-- -->46 0x0000773360541f89 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2941f89)
[build] #<!-- -->47 0x00007733605421ff clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29421ff)
[build] #<!-- -->48 0x000077336055f4d0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x295f4d0)
[build] #<!-- -->49 0x000060731c500bcb clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0xfbcb)
[build] #<!-- -->50 0x000060731c50e703 main (/usr/bin/clang+++0x1d703)
[build] #<!-- -->51 0x000077335582a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->52 0x000077335582a28b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->53 0x000077335582a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->54 0x000060731c4feda5 _start (/usr/bin/clang+++0xdda5)
[build] clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
[build] Ubuntu clang version 19.1.7 (++20250114103332+cd708029e0b2-1~exp1~20250114103446.78)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/lib/llvm-19/bin
[build] clang++: note: diagnostic msg: 
```

</details>


---

### Comment 3 - Endilll

Confirmed on Clang 21.0 (trunk): https://godbolt.org/z/9oTY9KM5b
This seems to be a regression in Clang 18. However, since Clang 19 we crash only in C++20 mode, but not C++23 and C++26 modes.
Reduced by me and C-Reduce:
```cpp
struct shared_ptr {
  shared_ptr();
};
void dispatchBDAImage(auto... aImagePtrs)
  requires(sizeof...(aImagePtrs) >= 0);
void PipelineActionImageMathprocess() {
  [&]< int = 0>() -> shared_ptr {
    dispatchBDAImage([] {});
  }();
}
```
Clang 21.0 crash:
```
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:291:
T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = clang::sema::FunctionScopeInfo*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = clang::sema::FunctionScopeInfo*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]:
Assertion `idx < size()' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:7:3: instantiating function definition 'PipelineActionImageMathprocess()::(anonymous class)::operator()<0>'
 #0 0x0000000003e87c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e87c38)
 #1 0x0000000003e858c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e858c4)
 #2 0x0000000003dca758 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078632c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078632c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078632c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078632c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078632c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078632c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000069bfa14 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bfa14)
#10 0x00000000069c1ab6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c1ab6)
#11 0x00000000069c8b67 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c8b67)
#12 0x000000000706adf2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSizeOfPackExpr(clang::SizeOfPackExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000705132e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007063dd8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007050aa8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000070793f2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70793f2)
#17 0x000000000679d370 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#18 0x000000000679f17e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#19 0x000000000679f7dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679f7dd)
#20 0x00000000067a0828 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a0828)
#21 0x0000000006fbff55 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#22 0x0000000006fc0397 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#23 0x0000000007d8dcb1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8dcb1)
#24 0x0000000006f98475 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f98475)
#25 0x0000000006dd75dc clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#26 0x0000000006dd7dab AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#27 0x0000000006dd80c1 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd80c1)
#28 0x0000000006dd83be clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd83be)
#29 0x0000000006de4bed clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6de4bed)
#30 0x00000000069f6716 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f6716)
#31 0x00000000069f758c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f758c)
#32 0x0000000007062378 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x0000000007050d42 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#34 0x000000000707ee57 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#35 0x000000000707fa6e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#36 0x0000000007088ab4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7088ab4)
#37 0x000000000710403d clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710403d)
#38 0x0000000007101fc6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7101fc6)
#39 0x000000000664faf2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#40 0x0000000006650392 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6650392)
#41 0x00000000064bc5bb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bc5bb)
#42 0x00000000064aea8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aea8a)
#43 0x0000000004842da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4842da8)
#44 0x0000000004b127f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b127f5)
#45 0x0000000004a94a2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a94a2e)
#46 0x0000000004c024fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c024fe)
#47 0x0000000000d50baf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd50baf)
#48 0x0000000000d4836a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#49 0x000000000488b309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#50 0x0000000003dcabf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dcabf4)
#51 0x000000000488b91f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#52 0x000000000484e38d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484e38d)
#53 0x000000000484f41e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484f41e)
#54 0x0000000004856af5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4856af5)
#55 0x0000000000d4d9a8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4d9a8)
#56 0x0000000000c146c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc146c4)
#57 0x000078632c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#58 0x000078632c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#59 0x0000000000d47e15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd47e15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

Author: Mario Emmenlauer (emmenlau)

<details>
Clang kindly asked me to create this bug report:
```
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang++ -DBDADATABASEHANDLER_DEBUG -DBDADATABASEHANDLER_HAVE_POSTGRESQL -DBDADATABASEHANDLER_HAVE_SQLITE -DBDAHDF5_DEBUG -DBDAIMAGEANALYSIS_HAVE_BDAIMAGEFILEHANDLER=1 -DBDAIMAGEANALYSIS_HAVE_BLAS=1 -DBDAIMAGEANALYSIS_HAVE_FFTW3=0 -DBDAIMAGEANALYSIS_HAVE_FFTW3XC=1 -DBDAIMAGEANALYSIS_HAVE_IPP=1 -DBDAIMAGEANALYSIS_HAVE_ITK=1 -DBDAIMAGEANALYSIS_HAVE_OPENCV=1 -DBDAIMAGEANALYSIS_HAVE_RANGES=0 -DBDAIMAGEANALYSIS_HAVE_VIGRA=1 -DBDAIMAGEFILEHANDLER_HAVE_BDAHDF5=1 -DBDAIMAGEFILEHANDLER_HAVE_BIOIMAGE=1 -DBDAPIPELINEPROCESSING_DEBUG -DBDAPIPELINEPROCESSING_ENABLE_THREAD_SAFETY=0 -DBDAPIPELINEPROCESSING_HAVE_QT=1 -DBDAQTBASEGUI_HAVE_PDFWRITER=1 -DBDAQTBASE_HAVE_QT_GUI=1 -DBDASAMPLEHANDLING_DEBUG -DBDASAMPLEHANDLING_THREAD_SAFETY_IMPLEMENTATION=2 -DBDA_HAVE_BIOIMAGE -DBDA_HAVE_BOOST -DBDA_HAVE_EIGEN3 -DBDA_HAVE_IPP -DBDA_HAVE_ITK -DBDA_HAVE_LEMON -DBDA_HAVE_MKL -DBDA_HAVE_OPENCV -DBDA_HAVE_VIGRA -DBDA_HISTOGRAM_BOUNDS_CHECK -DBIC_ENABLE_THREADSAFE -DBIM_COMPOSITE_FORMAT -DBIM_GEOTIFF_FORMAT -DBIM_HDF5_FORMAT -DBIM_JP2_FORMAT -DBIM_ND2_FORMAT -DBIM_NIFTI_FORMAT -DBIM_PNG_FORMAT -DBIM_PROJ4_FORMAT -DBIM_TIFF_FORMAT -DBIM_USE_CODECVT -DBIM_USE_EIGEN -DBOOST_ALL_NO_LIB -DBOOST_ALL_NO_LIB=1 -DBOOST_DATE_TIME_NO_LIB -DBOOST_FILESYSTEM_NO_DEPRECATED=1 -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_VERSION=4 -DBOOST_NOWIDE_NO_LIB -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_SYSTEM_NO_LIB -DDEBUG -DFFTWMKLWRAPPERS_ENABLE_CUSTOM_PREFIX -DH5_USE_BOOST -DH5_USE_EIGEN -DH5_USE_XTENSOR -DHAVE_CBLAS=1 -DHIGHFIVE_HAS_CONCEPTS=1 -DHasZLIB -DMPI_NO_CPPBIND -DOPENCV_DISABLE_THREAD_SUPPORT=1 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_QML_DEBUG -DSQLITE_ENABLE_COLUMN_METADATA -DTBB_DEPRECATED=0 -DTBB_USE_CAPTURED_EXCEPTION=0 -DTBB_USE_DEBUG -DVIGRA_STATIC_LIB -DXSIMD_ENABLE_XTL_COMPLEX=1 -DXTENSOR_ENABLE_ASSERT -DXTENSOR_USE_XSIMD -D_DEBUG -D_DEBUG_ -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_POSIX_C_SOURCE=200809L -D_USE_MATH_DEFINES -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing/BDAPipelineProcessing_autogen/include -I/data/memmenlauer/bda/usr-tmp-U2404Sk64c191/Debug/BDAPipelineProcessing -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/include -I/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/ipp/2021.5.2/include -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/eigen3 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/ITK-5.4 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Extras/oneapi/mkl/2022.0.2/include/fftw -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/openjpeg-2.5 -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/server -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtGui -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/include/QtCore -isystem /data/memmenlauer/bda/usr-dst-U2404Sk64c191/Debug/mkspecs/linux-clang -DDEBUG -DQT_DISABLE_DEPRECATED_BEFORE=0x050F00 -Wall -pedantic -Wextra -Werror=return-type -Werror=uninitialized -Werror=unknown-pragmas -Werror=shadow -Werror=undef -Werror=pointer-arith -Werror=shift-count-overflow -Werror=overloaded-virtual -Werror=old-style-cast -Werror=format -Werror=comment -Werror=char-subscripts -Werror=implicit-int -Werror=implicit-function-declaration -Werror=parentheses -Werror=nan-infinity-disabled -Werror=float-equal -Werror=implicit-int-float-conversion -Werror=reserved-identifier -Werror=reserved-macro-identifier -Werror=cast-qual -Werror=string-plus-int -Werror=nonportable-include-path -Werror=inconsistent-missing-override -Werror=self-assign -Werror=self-assign-field -Werror=self-move -Werror=braced-scalar-init -Werror=many-braces-around-scalar-init -Werror=deprecated-enum-enum-conversion -Werror=infinite-recursion -Werror=exceptions -Werror=excess-initializers -Werror=return-stack-address -Werror=delete-non-abstract-non-virtual-dtor -march=skylake -std=gnu++20 -O0 -glldb -fno-omit-frame-pointer -ftemplate-backtrace-limit=40 -ftime-trace -Werror=pessimizing-move -Werror=redundant-move -Werror=range-loop-construct -g -std=gnu++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -MD -MT CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -MF CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o.d -o CMakeFiles/BDAPipelineProcessing.dir/src/PipelineActionImageMath.cc.o -c /home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	/home/memmenlauer/BDA/Src/BDAPipelineProcessing/src/PipelineActionImageMath.cc:57:41: instantiating function definition 'bda::PipelineActionImageMath::process(bda::pipe::PipelineProcessorIterationStatus &amp;)::(anonymous class)::operator()&lt;xt::xtensor_container&lt;xt::uvector&lt;double, xsimd::aligned_allocator&lt;double, 32&gt;&gt;, 2, xt::layout_type::row_major&gt;&gt;'
[build]  #<!-- -->0 0x0000773356ebd42f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd42f)
[build]  #<!-- -->1 0x0000773356ebb139 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebb139)
[build]  #<!-- -->2 0x0000773356e04227 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe04227)
[build]  #<!-- -->3 0x0000773355845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
[build]  #<!-- -->4 0x000077335f4f4b05 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18f4b05)
[build]  #<!-- -->5 0x000077335f4c08a6 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c08a6)
[build]  #<!-- -->6 0x000077335f4c0748 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18c0748)
[build]  #<!-- -->7 0x000077335fa044cc (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e044cc)
[build]  #<!-- -->8 0x000077335fa0e3ad (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0e3ad)
[build]  #<!-- -->9 0x000077335f9e01d1 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1de01d1)
[build] #<!-- -->10 0x000077335f30cb8c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170cb8c)
[build] #<!-- -->11 0x000077335f30b9ca (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170b9ca)
[build] #<!-- -->12 0x000077335f304a25 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1704a25)
[build] #<!-- -->13 0x000077335f30477e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x170477e)
[build] #<!-- -->14 0x000077335f3078d4 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x17078d4)
[build] #<!-- -->15 0x000077335f926678 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d26678)
[build] #<!-- -->16 0x000077335f983ba7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d83ba7)
[build] #<!-- -->17 0x000077335f1bc160 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15bc160)
[build] #<!-- -->18 0x000077335f9280f0 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d280f0)
[build] #<!-- -->19 0x000077335f809d66 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c09d66)
[build] #<!-- -->20 0x000077335f81a2bd (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a2bd)
[build] #<!-- -->21 0x000077335f81a17e clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a17e)
[build] #<!-- -->22 0x000077335f81a4dc clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a4dc)
[build] #<!-- -->23 0x000077335f81a7dd clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c1a7dd)
[build] #<!-- -->24 0x000077335f4bb707 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18bb707)
[build] #<!-- -->25 0x000077335f4cf5d1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x18cf5d1)
[build] #<!-- -->26 0x000077335fa0a661 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e0a661)
[build] #<!-- -->27 0x000077335f9de737 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde737)
[build] #<!-- -->28 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #<!-- -->29 0x000077335fa25f2d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e25f2d)
[build] #<!-- -->30 0x000077335fa10024 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e10024)
[build] #<!-- -->31 0x000077335f9de6c3 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dde6c3)
[build] #<!-- -->32 0x000077335fa46451 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e46451)
[build] #<!-- -->33 0x000077335fa48bfe clang::Sema::PerformPendingInstantiations(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e48bfe)
[build] #<!-- -->34 0x000077335f1be023 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be023)
[build] #<!-- -->35 0x000077335f1be6a0 clang::Sema::ActOnEndOfTranslationUnit() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x15be6a0)
[build] #<!-- -->36 0x000077335e91f554 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd1f554)
[build] #<!-- -->37 0x000077335e84bb8e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4bb8e)
[build] #<!-- -->38 0x000077336097b67c clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b67c)
[build] #<!-- -->39 0x00007733608ee9f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cee9f4)
[build] #<!-- -->40 0x00007733609fad2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dfad2c)
[build] #<!-- -->41 0x000060731c5041d2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x131d2)
[build] #<!-- -->42 0x000060731c5011a4 (/usr/bin/clang+++0x101a4)
[build] #<!-- -->43 0x000077336057d272 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297d272)
[build] #<!-- -->44 0x0000773356e03f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe03f77)
[build] #<!-- -->45 0x000077336057cb06 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x297cb06)
[build] #<!-- -->46 0x0000773360541f89 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2941f89)
[build] #<!-- -->47 0x00007733605421ff clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29421ff)
[build] #<!-- -->48 0x000077336055f4d0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x295f4d0)
[build] #<!-- -->49 0x000060731c500bcb clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0xfbcb)
[build] #<!-- -->50 0x000060731c50e703 main (/usr/bin/clang+++0x1d703)
[build] #<!-- -->51 0x000077335582a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->52 0x000077335582a28b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->53 0x000077335582a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->54 0x000060731c4feda5 _start (/usr/bin/clang+++0xdda5)
[build] clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
[build] Ubuntu clang version 19.1.7 (++20250114103332+cd708029e0b2-1~exp1~20250114103446.78)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/lib/llvm-19/bin
[build] clang++: note: diagnostic msg: 
```

</details>


---

### Comment 5 - shafik

@cor3ntin git bisect points to 3ed9e9e3ace6f9ce320cf4e75cffa04a7c7241b5 as the cause of this regression

---

