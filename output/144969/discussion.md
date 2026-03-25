# After compiling llvm project, use Clang to compile a cuda kernel which calls cutlass gemm interface, but it report a compile error

**Author:** zhangzh33
**URL:** https://github.com/llvm/llvm-project/issues/144969

## Body

command is: 
/data/llvm/build/bin/clang-19 -I/data/cutlass/include -I/usr/local/cuda-12.2/include --cuda-gpu-arch=sm_70 -O2 -std=c++17 --cuda-device-only -Xcuda-ptxas -v -o test.s test.cu

compile error:
clang-19: /data/llvm/clang/lib/AST/ExprClassification.cpp:72: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isPRValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/llvm/build/bin/clang-19 -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -dumpdir test.ptx- -disable-free -clear-ast-before-backend -main-file-name test.cu -mrelocation-model static -mframe-pointer=all -fno-rounding-math -no-integrated-as -aux-target-cpu x86-64 -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fcuda-allow-variadic-functions -mlink-builtin-bitcode /usr/local/cuda-12.2/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.2 -target-cpu sm_70 -target-feature +ptx82 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/xp6/demo_cutlass -resource-dir /data/llvm/build/lib/clang/19 -internal-isystem /data/llvm/build/lib/clang/19/include/cuda_wrappers -include __clang_cuda_runtime_wrapper.h -I /data/cutlass/include -I /usr/local/cuda-12.2/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /data/llvm/build/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /usr/local/cuda-12.2/include -internal-isystem /data/llvm/build/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fno-autolink -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -cuid=27acb42ef6f99229 -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-sm_70-0e74d8.s -x cuda test.cu
1.      /data/cutlass/include/cute/numeric/integral_constant.hpp:227:1 <Spelling=/data/cutlass/include/cute/numeric/integral_constant.hpp:207:3>: at annotation token
2.      /data/cutlass/include/cute/numeric/integral_constant.hpp:37:1: parsing namespace 'cute'
 #0 0x0000598773feab36 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/llvm/llvm/lib/Support/Unix/Signals.inc:723:22
 #1 0x0000598773feaf57 PrintStackTraceSignalHandler(void*) /data/llvm/llvm/lib/Support/Unix/Signals.inc:798:1
 #2 0x0000598773fe83a7 llvm::sys::RunSignalHandlers() /data/llvm/llvm/lib/Support/Signals.cpp:105:20
 #3 0x0000598773fea3ce SignalHandler(int) /data/llvm/llvm/lib/Support/Unix/Signals.inc:413:1
 #4 0x0000703d15b65520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x0000703d15bb99fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000703d15bb99fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000703d15bb99fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000703d15b65476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000703d15b4b7f3 abort ./stdlib/abort.c:81:7
#10 0x0000703d15b4b71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000703d15b5ce96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#12 0x000059877a36646b clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const /data/llvm/clang/lib/AST/ExprClassification.cpp:58:5
#13 0x0000598778aae760 clang::Expr::Classify(clang::ASTContext&) const /data/llvm/clang/include/clang/AST/Expr.h:407:3
#14 0x0000598779553d85 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) /data/llvm/clang/lib/Sema/SemaTemplateDeduction.cpp:5280:61
#15 0x0000598779320431 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, clang::Sema::CheckTemplateArgumentKind) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:7879:25
#16 0x0000598779319c83 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::Sema::CheckTemplateArgumentKind) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:6374:45
#17 0x000059877931ac62 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, bool, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, bool, bool*, bool) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:6665:32
#18 0x0000598779311f04 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:4617:32
#19 0x0000598779313a48 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:4930:40
#20 0x00005987784251ca clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:1364:40
#21 0x000059877832827a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /data/llvm/clang/lib/Parse/ParseDecl.cpp:4165:7
#22 0x00005987782f9c39 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) /data/llvm/clang/include/clang/Parse/Parser.h:2500:38
#23 0x0000598778420f8d clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:238:13
#24 0x0000598778420bfb clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:178:39
#25 0x000059877842067f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:50:50
#26 0x000059877831fda2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /data/llvm/clang/lib/Parse/ParseDecl.cpp:1984:48
#27 0x00005987782ed983 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /data/llvm/clang/lib/Parse/Parser.cpp:990:30
#28 0x000059877834ac8c clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /data/llvm/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#29 0x000059877834aaa2 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /data/llvm/clang/lib/Parse/ParseDeclCXX.cpp:252:22
#30 0x000059877831ffbe clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /data/llvm/clang/lib/Parse/ParseDecl.cpp:2003:26
#31 0x00005987782ed983 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /data/llvm/clang/lib/Parse/Parser.cpp:990:30
#32 0x00005987782ecca6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /data/llvm/clang/lib/Parse/Parser.cpp:763:36
#33 0x00005987782e7ce0 clang::ParseAST(clang::Sema&, bool, bool) /data/llvm/clang/lib/Parse/ParseAST.cpp:163:37
#34 0x000059877513d33a clang::ASTFrontendAction::ExecuteAction() /data/llvm/clang/lib/Frontend/FrontendAction.cpp:1192:11
#35 0x0000598774e0578a clang::CodeGenAction::ExecuteAction() /data/llvm/clang/lib/CodeGen/CodeGenAction.cpp:1147:5
#36 0x000059877513cbe4 clang::FrontendAction::Execute() /data/llvm/clang/lib/Frontend/FrontendAction.cpp:1082:38
#37 0x000059877504758f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/llvm/clang/lib/Frontend/CompilerInstance.cpp:1061:42
#38 0x00005987752f462f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/llvm/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:38
#39 0x0000598772057449 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/llvm/clang/tools/driver/cc1_main.cpp:232:40
#40 0x00005987720482d6 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /data/llvm/clang/tools/driver/driver.cpp:215:20
#41 0x000059877204882b clang_main(int, char**, llvm::ToolContext const&) /data/llvm/clang/tools/driver/driver.cpp:256:26
#42 0x0000598772083e28 main /data/llvm/build/tools/clang/tools/driver/clang-driver.cpp:17:20
#43 0x0000703d15b4cd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#44 0x0000703d15b4ce40 call_init ./csu/../csu/libc-start.c:128:20
#45 0x0000703d15b4ce40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#46 0x0000598772047665 _start (/data/llvm/build/bin/clang-19+0x1079665)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (zhangzh33)

<details>
command is: 
/data/llvm/build/bin/clang-19 -I/data/cutlass/include -I/usr/local/cuda-12.2/include --cuda-gpu-arch=sm_70 -O2 -std=c++17 --cuda-device-only -Xcuda-ptxas -v -o test.s test.cu

compile error:
clang-19: /data/llvm/clang/lib/AST/ExprClassification.cpp:72: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isPRValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/llvm/build/bin/clang-19 -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -dumpdir test.ptx- -disable-free -clear-ast-before-backend -main-file-name test.cu -mrelocation-model static -mframe-pointer=all -fno-rounding-math -no-integrated-as -aux-target-cpu x86-64 -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fcuda-allow-variadic-functions -mlink-builtin-bitcode /usr/local/cuda-12.2/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.2 -target-cpu sm_70 -target-feature +ptx82 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/xp6/demo_cutlass -resource-dir /data/llvm/build/lib/clang/19 -internal-isystem /data/llvm/build/lib/clang/19/include/cuda_wrappers -include __clang_cuda_runtime_wrapper.h -I /data/cutlass/include -I /usr/local/cuda-12.2/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /data/llvm/build/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /usr/local/cuda-12.2/include -internal-isystem /data/llvm/build/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fno-autolink -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -cuid=27acb42ef6f99229 -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-sm_70-0e74d8.s -x cuda test.cu
1.      /data/cutlass/include/cute/numeric/integral_constant.hpp:227:1 &lt;Spelling=/data/cutlass/include/cute/numeric/integral_constant.hpp:207:3&gt;: at annotation token
2.      /data/cutlass/include/cute/numeric/integral_constant.hpp:37:1: parsing namespace 'cute'
 #<!-- -->0 0x0000598773feab36 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /data/llvm/llvm/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x0000598773feaf57 PrintStackTraceSignalHandler(void*) /data/llvm/llvm/lib/Support/Unix/Signals.inc:798:1
 #<!-- -->2 0x0000598773fe83a7 llvm::sys::RunSignalHandlers() /data/llvm/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->3 0x0000598773fea3ce SignalHandler(int) /data/llvm/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->4 0x0000703d15b65520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x0000703d15bb99fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000703d15bb99fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000703d15bb99fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000703d15b65476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000703d15b4b7f3 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000703d15b4b71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000703d15b5ce96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->12 0x000059877a36646b clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const /data/llvm/clang/lib/AST/ExprClassification.cpp:58:5
#<!-- -->13 0x0000598778aae760 clang::Expr::Classify(clang::ASTContext&amp;) const /data/llvm/clang/include/clang/AST/Expr.h:407:3
#<!-- -->14 0x0000598779553d85 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) /data/llvm/clang/lib/Sema/SemaTemplateDeduction.cpp:5280:61
#<!-- -->15 0x0000598779320431 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, clang::Sema::CheckTemplateArgumentKind) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:7879:25
#<!-- -->16 0x0000598779319c83 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::Sema::CheckTemplateArgumentKind) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:6374:45
#<!-- -->17 0x000059877931ac62 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, bool, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, bool, bool*, bool) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:6665:32
#<!-- -->18 0x0000598779311f04 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:4617:32
#<!-- -->19 0x0000598779313a48 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) /data/llvm/clang/lib/Sema/SemaTemplate.cpp:4930:40
#<!-- -->20 0x00005987784251ca clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&amp;, clang::ImplicitTypenameContext, bool) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:1364:40
#<!-- -->21 0x000059877832827a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /data/llvm/clang/lib/Parse/ParseDecl.cpp:4165:7
#<!-- -->22 0x00005987782f9c39 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) /data/llvm/clang/include/clang/Parse/Parser.h:2500:38
#<!-- -->23 0x0000598778420f8d clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:238:13
#<!-- -->24 0x0000598778420bfb clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:178:39
#<!-- -->25 0x000059877842067f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) /data/llvm/clang/lib/Parse/ParseTemplate.cpp:50:50
#<!-- -->26 0x000059877831fda2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /data/llvm/clang/lib/Parse/ParseDecl.cpp:1984:48
#<!-- -->27 0x00005987782ed983 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /data/llvm/clang/lib/Parse/Parser.cpp:990:30
#<!-- -->28 0x000059877834ac8c clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /data/llvm/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->29 0x000059877834aaa2 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /data/llvm/clang/lib/Parse/ParseDeclCXX.cpp:252:22
#<!-- -->30 0x000059877831ffbe clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /data/llvm/clang/lib/Parse/ParseDecl.cpp:2003:26
#<!-- -->31 0x00005987782ed983 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /data/llvm/clang/lib/Parse/Parser.cpp:990:30
#<!-- -->32 0x00005987782ecca6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /data/llvm/clang/lib/Parse/Parser.cpp:763:36
#<!-- -->33 0x00005987782e7ce0 clang::ParseAST(clang::Sema&amp;, bool, bool) /data/llvm/clang/lib/Parse/ParseAST.cpp:163:37
#<!-- -->34 0x000059877513d33a clang::ASTFrontendAction::ExecuteAction() /data/llvm/clang/lib/Frontend/FrontendAction.cpp:1192:11
#<!-- -->35 0x0000598774e0578a clang::CodeGenAction::ExecuteAction() /data/llvm/clang/lib/CodeGen/CodeGenAction.cpp:1147:5
#<!-- -->36 0x000059877513cbe4 clang::FrontendAction::Execute() /data/llvm/clang/lib/Frontend/FrontendAction.cpp:1082:38
#<!-- -->37 0x000059877504758f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /data/llvm/clang/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->38 0x00005987752f462f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/llvm/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:38
#<!-- -->39 0x0000598772057449 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /data/llvm/clang/tools/driver/cc1_main.cpp:232:40
#<!-- -->40 0x00005987720482d6 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /data/llvm/clang/tools/driver/driver.cpp:215:20
#<!-- -->41 0x000059877204882b clang_main(int, char**, llvm::ToolContext const&amp;) /data/llvm/clang/tools/driver/driver.cpp:256:26
#<!-- -->42 0x0000598772083e28 main /data/llvm/build/tools/clang/tools/driver/clang-driver.cpp:17:20
#<!-- -->43 0x0000703d15b4cd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->44 0x0000703d15b4ce40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->45 0x0000703d15b4ce40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->46 0x0000598772047665 _start (/data/llvm/build/bin/clang-19+0x1079665)
</details>


---

### Comment 3 - zhangzh33

> Could you please try 20 or `main` branch?

thx for reply, and this is a version error? 

---

### Comment 4 - zwuis

Please provide reproducer.

> thx for reply, and this is a version error?

Just to confirm that this bug still exists in latest major release.

---

### Comment 5 - zhangzh33

> Could you please try 20 or `main` branch?

I just install clang-20 release pkg，and the cuda kernel is:

#include <cuda_runtime.h>
#include <cutlass/gemm/device/gemm.h>

using GemmOperator = cutlass::gemm::device::Gemm<
    float,
    cutlass::layout::RowMajor,
    float,
    cutlass::layout::RowMajor,
    float,
    cutlass::layout::RowMajor,
    float,
    cutlass::arch::Sm70
>;

__global__ void launch_cutlass_gemm(
    const float* A, const float* B, float* C,
    int M, int N, int K, int lda, int ldb, int ldc) {

    GemmOperator gemm_op;
    cutlass::Status status = gemm_op({
        {M, N, K},
        A, lda,
        B, ldb,
        C, ldc,
        C, ldc,
        {1.0f, 0.0f}
    });
}

int main() {
    const int M = 1024, N = 768, K = 512;
    const int lda = K, ldb = N, ldc = N;

    float *h_A = new float[M*K];
    float *h_B = new float[K*N];
    float *h_C = new float[M*N];

    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, M*K*sizeof(float));
    cudaMalloc(&d_B, K*N*sizeof(float));
    cudaMalloc(&d_C, M*N*sizeof(float));

    std::fill(h_A, h_A + M*K, 1.0f);
    std::fill(h_B, h_B + K*N, 1.0f);

    cudaMemcpy(d_A, h_A, M*K*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, K*N*sizeof(float), cudaMemcpyHostToDevice);

    dim3 grid(128, 1);
    dim3 block(256, 1);
    launch_cutlass_gemm<<<grid, block>>>(d_A, d_B, d_C, M, N, K, lda, ldb, ldc);

    cudaMemcpy(h_C, d_C, M*N*sizeof(float), cudaMemcpyDeviceToHost);

    delete[] h_A;
        delete[] h_B;
        delete[] h_C;
    cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);

    return 0;
}

command line:  clang-20 -I/data/cutlass/include -I/usr/local/cuda-12.2/include --cuda-gpu-arch=sm_70 -O2 -std=c++17 --cuda-device-only -Xcuda-ptxas -v -o test.ptx test.cu

compile error is as below:
/data/cutlass/include/cutlass/gemm/device/gemm.h:189:43: error: implicit instantiation of undefined template 'cutlass::gemm::device::DefaultGemmConfiguration<cutlass::arch::Sm70, cutlass::arch::Sm70, float, float, float, float>'

test.cu:19:5: error: unknown type name 'GemmOperator'
   19 |     GemmOperator gemm_op;

please help me to solve this error, thx

---

### Comment 6 - zhangzh33

> Please provide reproducer.
> 
> > thx for reply, and this is a version error?
> 
> Just to confirm that this bug still exists in latest major release.

I install clang-20 release pkg and the error changes

---

