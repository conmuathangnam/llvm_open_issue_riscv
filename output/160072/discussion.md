# clangd crash inside VS code while parsing mpunits unit declaration in C++23

**Author:** alex-tee
**URL:** https://github.com/llvm/llvm-project/issues/160072
**Status:** Open
**Labels:** clangd, crash

## Body

It crashes in the line `using tick_t = mp_units::quantity<tick, int64_t>;` below (code extracted from my project, I don't know if it's reproducible with just this). MP units is here: https://github.com/mpusz/mp-units (I'm using commit 8489cc2299a58cdf603f2521f982359959d907ea).

```cpp
#include <mp-units/framework/quantity_spec.h>
#include <mp-units/framework/unit.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

QUANTITY_SPEC (tick_count, mp_units::dimensionless, mp_units::is_kind);

inline constexpr struct tick final
    : mp_units::named_unit<"tick", mp_units::one, mp_units::kind_of<tick_count>>
{
} tick;

using tick_t = mp_units::quantity<tick, int64_t>;
```

System: Arch Linux

Version (distro-installed clang): 
```
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

Backtrace:
```
I[19:52:25.208] Built preamble of size 95786236 for file /home/alex/Documents/git/zrythm/src/utils/types.h version 76 in 7.61 seconds
I[19:52:25.208] --> workspace/semanticTokens/refresh(1)
I[19:52:25.208] Indexing c++23 standard library in the context of /home/alex/Documents/git/zrythm/src/utils/types.h
I[19:52:25.208] --> textDocument/clangd.fileStatus
I[19:52:25.209] <-- reply(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/alex/Documents/git/zrythm/src/utils/types.h:111:26: current parser token 'quantity'
1.	/home/alex/Documents/git/zrythm/src/utils/types.h:80:1: parsing namespace 'zrythm'
 #0 0x00000000005556e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:799:13
 #1 0x00000000005535ae llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #2 0x000000000055602f SignalHandler(int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:415:1
 #3 0x00007fe56be3e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x000000000430d8e0 getTypeClass /__w/clangd/clangd/llvm-project/clang/include/clang/AST/Type.h:2341:75
 #5 0x000000000430d8e0 getTypeLocClass /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLoc.h:118:38
 #6 0x000000000430d8e0 clang::TypeLocVisitor<clang::TypeLocReader, void>::Visit(clang::TypeLoc) /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLocVisitor.h:28:19
 #7 0x000000000430d89e getNextTypeLoc /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLoc.h:171:12
 #8 0x000000000430d89e clang::ASTRecordReader::readTypeLoc(clang::TypeLoc, clang::SourceLocationSequence*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:7395:32
 #9 0x000000000434972d getKind /__w/clangd/clangd/llvm-project/clang/include/clang/AST/DeclBase.h:445:51
#10 0x000000000434972d classof /__w/clangd/clangd/llvm-project/clang/include/clang/AST/Decl.h:3429:62
#11 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#12 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#13 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#14 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#15 0x000000000434972d isPossible /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#16 0x000000000434972d doCastIfPossible /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
#17 0x000000000434972d dyn_cast<clang::TypeDecl, clang::Decl> /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
#18 0x000000000434972d clang::ASTDeclReader::Visit(clang::Decl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:538:18
#19 0x00000000046b65fb clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /__w/clangd/clangd/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#20 0x00000000043753a8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:4221:40
#21 0x0000000004306e33 clang::ASTReader::GetDecl(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8208:9
#22 0x000000000433ff01 clang::NamedDecl* clang::ASTReader::ReadDeclAs<clang::NamedDecl>(clang::serialization::ModuleFile&, llvm::SmallVectorImpl<unsigned long> const&, unsigned int&) /__w/clangd/clangd/llvm-project/clang/include/clang/Serialization/ASTReader.h:2132:5
#23 0x0000000004318467 readDeclAs<clang::NamedDecl> /__w/clangd/clangd/llvm-project/clang/include/clang/Serialization/ASTRecordReader.h:203:20
#24 0x0000000004318467 clang::ASTRecordReader::readTemplateParameterList() /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:9781:22
#25 0x0000000004356e72 VisitTemplateDecl /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2404:21
#26 0x0000000004356e72 clang::ASTDeclReader::VisitRedeclarableTemplateDecl(clang::RedeclarableTemplateDecl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2463:3
#27 0x0000000004356a02 clang::ASTDeclReader::VisitClassTemplateDecl(clang::ClassTemplateDecl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2471:3
#28 0x00000000043496bf clang::ASTDeclReader::Visit(clang::Decl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:530:34
#29 0x00000000046b65fb clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /__w/clangd/clangd/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#30 0x00000000043753a8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:4221:40
#31 0x0000000004306e33 clang::ASTReader::GetDecl(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8208:9
#32 0x000000000431151f clang::ASTReader::FindExternalVisibleDeclsByName(clang::DeclContext const*, clang::DeclarationName, clang::DeclContext const*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8495:39
#33 0x00000000006b431c clang::DeclContext::lookupImpl(clang::DeclarationName, clang::DeclContext const*) const /__w/clangd/clangd/llvm-project/clang/lib/AST/DeclBase.cpp:1913:78
#34 0x0000000000c1ba29 operator!=<clang::NamedDecl *, clang::DeclListNode *> /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:214:31
#35 0x0000000000c1ba29 operator!= /__w/clangd/clangd/llvm-project/clang/include/clang/AST/DeclBase.h:1350:59
#36 0x0000000000c1ba29 LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaLookup.cpp:1128:21
#37 0x0000000000c17ff3 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaLookup.cpp:2451:7
#38 0x0000000000c8a2e9 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:5
#39 0x0000000000c89e79 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplate.cpp:210:7
#40 0x00000000044a5796 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:545:42
#41 0x000000000444da55 clang::Parser::TryAnnotateCXXScopeToken(bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:2252:7
#42 0x0000000004469e45 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:4053:13
#43 0x0000000004469748 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:2961:23
#44 0x000000000445a357 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:63:7
#45 0x00000000044825a3 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::Parser::UsingDeclarator&, clang::SourceLocation&, clang::AccessSpecifier, clang::ParsedAttributes&, clang::Decl**) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#46 0x000000000448142c clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:853:16
#47 0x0000000004480968 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&, clang::ParsedAttributes&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:533:10
#48 0x0000000004465b05 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:2081:12
#49 0x0000000004448593 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#50 0x000000000447f76b ~AttributePool /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:724:22
#51 0x000000000447f76b ~ParsedAttributes /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:956:7
#52 0x000000000447f76b clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:265:5
#53 0x000000000447f86b Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#54 0x000000000447f86b clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:288:18
#55 0x000000000447f150 Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#56 0x000000000447f150 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:244:18
#57 0x0000000004465a72 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#58 0x0000000004448593 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#59 0x0000000004447569 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:12
#60 0x0000000004443bfe clang::ParseAST(clang::Sema&, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#61 0x00000000040aaefd clang::FrontendAction::Execute() /__w/clangd/clangd/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:10
#62 0x00000000015ba9a4 getPtr /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:282:12
#63 0x00000000015ba9a4 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#64 0x00000000015ba9a4 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#65 0x0000000001639edf ~__shared_count /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/shared_ptr_base.h:729:6
#66 0x0000000001639edf ~__shared_ptr /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/shared_ptr_base.h:1169:31
#67 0x0000000001639edf clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#68 0x000000000163996e clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_2::operator()() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:0:5
#69 0x0000000001636343 operator() /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#70 0x0000000001636343 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#71 0x000000000163531d run /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1463:5
#72 0x000000000163531d operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#73 0x000000000163531d void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_7>(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#74 0x00000000017ade06 operator= /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:48:5
#75 0x00000000017ade06 PunnedPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37:60
#76 0x00000000017ade06 PointerIntPair /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86:13
#77 0x00000000017ade06 UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:343:3
#78 0x00000000017ade06 unique_function /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:372:3
#79 0x00000000017ade06 operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:103:14
#80 0x00000000017ade06 operator()<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:43:11
#81 0x00000000017ade06 __invoke_impl<void, (lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:60:14
#82 0x00000000017ade06 __invoke<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:95:14
#83 0x00000000017ade06 __apply_impl<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> &, 0> /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/tuple:1684:14
#84 0x00000000017ade06 apply<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> &> /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/tuple:1694:14
#85 0x00000000017ade06 GenericThreadProxy<std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> > /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:41:5
#86 0x00000000017ade06 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:55:5
#87 0x00007fe56be969cb start_thread /usr/src/debug/glibc/glibc/nptl/pthread_create.c:448:8
#88 0x00007fe56bf1aa0c __GI___clone3 /usr/src/debug/glibc/glibc/misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: /home/alex/Documents/git/zrythm/src/utils/types.h
  Directory: /home/alex/Documents/git/zrythm/builddir_cmake
  Command Line: /usr/bin/clang++ --driver-mode=g++ -DBACKWARD_HAS_DW=1 -DBACKWARD_HAS_UNWIND=1 -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DDEBUG=1 -DHAVE_LITTLE_ENDIAN=1 -DJSON_USE_IMPLICIT_CONVERSIONS=0 -DJUCE_DEBUG=1 -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_PLUGINHOST_AU=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCE_PLUGINHOST_LV2=1 -DJUCE_PLUGINHOST_VST3=1 -DJUCE_USE_MP3AUDIOFORMAT=1 -DJUCE_WEB_BROWSER=0 -DMP_UNITS_API_CONTRACTS=0 -DMP_UNITS_API_NATURAL_UNITS=1 -DMP_UNITS_API_NO_CRTP=0 -DMP_UNITS_API_STD_FORMAT=0 -DMP_UNITS_HOSTED=0 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_ENABLE_STRICT_MODE_UP_TO=0x060900 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_KEYWORDS -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DQT_WIDGETS_LIB -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FUNCTION=__PRETTY_FUNCTION__ -DTBB_USE_DEBUG -DTYPE_SAFE_ARITHMETIC_POLICY=1 -DTYPE_SAFE_ENABLE_ASSERTIONS=1 -DTYPE_SAFE_ENABLE_PRECONDITION_CHECKS=1 -DTYPE_SAFE_ENABLE_WRAPPER=1 -D_DEBUG=1 -I/home/alex/Documents/git/zrythm/builddir_cmake/src/utils/zrythm_utils_lib_autogen/include -I/home/alex/Documents/git/zrythm/src -I/home/alex/Documents/git/zrythm/builddir_cmake/src -I/home/alex/Documents/git/zrythm/ext/rubberband -I/home/alex/Documents/git/zrythm/.cache/CPM/juce/4e98/modules -I/home/alex/Documents/git/zrythm/ext/ladspa -I/home/alex/Documents/git/zrythm/ext/kissfft -isystem /home/alex/Documents/git/zrythm/ext/zita-resampler -isystem /home/alex/Documents/git/zrythm/ext/soxr/src -isystem /home/alex/Documents/git/zrythm/.cache/CPM/zstd/e8f3/build/cmake/../../lib -isystem /home/alex/Documents/git/zrythm/.cache/CPM/xxhash/c13d/cmake_unofficial/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/nlohmann_json/26c2/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/magic_enum/2c54/include/magic_enum -isystem /home/alex/Documents/git/zrythm/ext/backward-cpp -isystem /home/alex/Documents/git/zrythm/ext/crill/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/fmt/6173/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/scnlib/f754/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/spdlog/514a/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/external/debug_assert -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/describe/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/mp11/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/stl_interfaces/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/assert/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/config/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/container/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/intrusive/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/move/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/unordered/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/container_hash/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/core/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/static_assert/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/throw_exception/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/predef/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/multi_index/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/bind/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/integer/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/type_traits/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/iterator/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/concept_check/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/preprocessor/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/detail/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/fusion/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/function_types/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/mpl/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/utility/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/io/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/tuple/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/typeof/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/functional/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/function/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/optional/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/smart_ptr/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gsl-lite/1f8f/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap-helpers/c8f1/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap/60c7/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/tbb/4990/src/tbb/../../include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/farbot/c1e9/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/mp-units/b278/src/core/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/mp-units/b278/src/systems/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtCore -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/mkspecs/linux-g++ -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtGui -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtWidgets -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuick -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQml -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlIntegration -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtNetwork -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlMeta -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlModels -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlWorkerScript -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtOpenGL -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuickControls2 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtConcurrent -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest -isystem /home/alex/Documents/git/zrythm/ext/moodycamel/.. -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -g -fPIC -g -O0 -DSCN_REGEX_BACKEND=0 -Wno-unused-result -fPIC -fsanitize-address-use-after-scope -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Wextra -Wformat=2 -Wno-missing-field-initializers -Wno-unused-parameter -Wno-cast-function-type -Wno-attributes -Werror=format-overflow -Werror=format-truncation -Werror=return-local-addr -Werror=alloca -Werror=cast-align -Werror=cast-qual -Werror=ctor-dtor-privacy -Werror=deprecated -Werror=format-signedness -Wframe-larger-than=65536 -Werror=multichar -Werror=non-virtual-dtor -Werror=range-loop-construct -Werror=disabled-optimization -Werror=pointer-arith -Werror=enum-conversion -Werror=overlength-strings -Werror=int-to-pointer-cast -Werror=shadow -Werror=address -Werror=undef -Werror=unused -Werror=vla -fstrict-aliasing -Wstrict-aliasing=2 -Werror=strict-aliasing -Werror=strict-overflow -Wstrict-overflow=2 -fstrict-overflow -Werror=init-self -Werror=write-strings -Werror=sign-compare -Werror=float-conversion -Werror=uninitialized -Werror=return-type -Werror=switch -Werror=overflow -Werror=array-bounds -Werror=enum-compare -Werror=misleading-indentation -Werror=int-in-bool-context -Werror=type-limits -Werror=endif-labels -Werror=logical-not-parentheses -Werror=parentheses -Werror=comment -Werror=sizeof-pointer-div -Werror=shift-count-overflow -Werror=free-nonheap-object -Werror=nonnull -Werror=tautological-compare -Werror=unused-result -Werror=inline -Werror=sizeof-array-argument -Werror=odr -Werror=narrowing -Werror=reorder -Werror=sequence-point -Werror=invalid-constexpr -Werror=pessimizing-move -pedantic-errors -Werror=pedantic -Werror=vla-cxx-extension -Werror=sentinel -Werror=return-stack-address -Werror=abstract-final-class -Werror=null-dereference -Werror=defaulted-function-deleted -Wexit-time-destructors -Wglobal-constructors -fsafe-buffer-usage-suggestions -Wperf-constraint-implies-noexcept -Winvalid-pch -Xclang -include-pch -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/src/utils/CMakeFiles/zrythm_utils_lib.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/src/utils/CMakeFiles/zrythm_utils_lib.dir/cmake_pch.hxx -c -x c++-header -std=c++23 -ferror-limit=20 -resource-dir=/home/alex/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/20.1.8/clangd_20.1.8/lib/clang/20 -- /home/alex/Documents/git/zrythm/src/utils/types.h
  Version: 76
[Info  - 19:52:59] Connection to server got closed. Server will restart.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (alex-tee)

<details>
It crashes in the line `using tick_t = mp_units::quantity&lt;tick, int64_t&gt;;` below (code extracted from my project, I don't know if it's reproducible with just this). MP units is here: https://github.com/mpusz/mp-units (I'm using commit 8489cc2299a58cdf603f2521f982359959d907ea).

```cpp
#include &lt;mp-units/framework/quantity_spec.h&gt;
#include &lt;mp-units/framework/unit.h&gt;
#include &lt;mp-units/systems/isq.h&gt;
#include &lt;mp-units/systems/si.h&gt;

QUANTITY_SPEC (tick_count, mp_units::dimensionless, mp_units::is_kind);

inline constexpr struct tick final
    : mp_units::named_unit&lt;"tick", mp_units::one, mp_units::kind_of&lt;tick_count&gt;&gt;
{
} tick;

using tick_t = mp_units::quantity&lt;tick, int64_t&gt;;
```

System: Arch Linux

Version (distro-installed clang): 
```
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

Backtrace:
```
I[19:52:25.208] Built preamble of size 95786236 for file /home/alex/Documents/git/zrythm/src/utils/types.h version 76 in 7.61 seconds
I[19:52:25.208] --&gt; workspace/semanticTokens/refresh(1)
I[19:52:25.208] Indexing c++23 standard library in the context of /home/alex/Documents/git/zrythm/src/utils/types.h
I[19:52:25.208] --&gt; textDocument/clangd.fileStatus
I[19:52:25.209] &lt;-- reply(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/alex/Documents/git/zrythm/src/utils/types.h:111:26: current parser token 'quantity'
1.	/home/alex/Documents/git/zrythm/src/utils/types.h:80:1: parsing namespace 'zrythm'
 #<!-- -->0 0x00000000005556e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:799:13
 #<!-- -->1 0x00000000005535ae llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x000000000055602f SignalHandler(int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:415:1
 #<!-- -->3 0x00007fe56be3e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x000000000430d8e0 getTypeClass /__w/clangd/clangd/llvm-project/clang/include/clang/AST/Type.h:2341:75
 #<!-- -->5 0x000000000430d8e0 getTypeLocClass /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLoc.h:118:38
 #<!-- -->6 0x000000000430d8e0 clang::TypeLocVisitor&lt;clang::TypeLocReader, void&gt;::Visit(clang::TypeLoc) /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLocVisitor.h:28:19
 #<!-- -->7 0x000000000430d89e getNextTypeLoc /__w/clangd/clangd/llvm-project/clang/include/clang/AST/TypeLoc.h:171:12
 #<!-- -->8 0x000000000430d89e clang::ASTRecordReader::readTypeLoc(clang::TypeLoc, clang::SourceLocationSequence*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:7395:32
 #<!-- -->9 0x000000000434972d getKind /__w/clangd/clangd/llvm-project/clang/include/clang/AST/DeclBase.h:445:51
#<!-- -->10 0x000000000434972d classof /__w/clangd/clangd/llvm-project/clang/include/clang/AST/Decl.h:3429:62
#<!-- -->11 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:64:53
#<!-- -->12 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:110:12
#<!-- -->13 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:137:12
#<!-- -->14 0x000000000434972d doit /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:127:12
#<!-- -->15 0x000000000434972d isPossible /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:255:12
#<!-- -->16 0x000000000434972d doCastIfPossible /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:493:10
#<!-- -->17 0x000000000434972d dyn_cast&lt;clang::TypeDecl, clang::Decl&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Casting.h:663:10
#<!-- -->18 0x000000000434972d clang::ASTDeclReader::Visit(clang::Decl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:538:18
#<!-- -->19 0x00000000046b65fb clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /__w/clangd/clangd/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#<!-- -->20 0x00000000043753a8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:4221:40
#<!-- -->21 0x0000000004306e33 clang::ASTReader::GetDecl(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8208:9
#<!-- -->22 0x000000000433ff01 clang::NamedDecl* clang::ASTReader::ReadDeclAs&lt;clang::NamedDecl&gt;(clang::serialization::ModuleFile&amp;, llvm::SmallVectorImpl&lt;unsigned long&gt; const&amp;, unsigned int&amp;) /__w/clangd/clangd/llvm-project/clang/include/clang/Serialization/ASTReader.h:2132:5
#<!-- -->23 0x0000000004318467 readDeclAs&lt;clang::NamedDecl&gt; /__w/clangd/clangd/llvm-project/clang/include/clang/Serialization/ASTRecordReader.h:203:20
#<!-- -->24 0x0000000004318467 clang::ASTRecordReader::readTemplateParameterList() /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:9781:22
#<!-- -->25 0x0000000004356e72 VisitTemplateDecl /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2404:21
#<!-- -->26 0x0000000004356e72 clang::ASTDeclReader::VisitRedeclarableTemplateDecl(clang::RedeclarableTemplateDecl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2463:3
#<!-- -->27 0x0000000004356a02 clang::ASTDeclReader::VisitClassTemplateDecl(clang::ClassTemplateDecl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:2471:3
#<!-- -->28 0x00000000043496bf clang::ASTDeclReader::Visit(clang::Decl*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:530:34
#<!-- -->29 0x00000000046b65fb clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /__w/clangd/clangd/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#<!-- -->30 0x00000000043753a8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReaderDecl.cpp:4221:40
#<!-- -->31 0x0000000004306e33 clang::ASTReader::GetDecl(clang::GlobalDeclID) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8208:9
#<!-- -->32 0x000000000431151f clang::ASTReader::FindExternalVisibleDeclsByName(clang::DeclContext const*, clang::DeclarationName, clang::DeclContext const*) /__w/clangd/clangd/llvm-project/clang/lib/Serialization/ASTReader.cpp:8495:39
#<!-- -->33 0x00000000006b431c clang::DeclContext::lookupImpl(clang::DeclarationName, clang::DeclContext const*) const /__w/clangd/clangd/llvm-project/clang/lib/AST/DeclBase.cpp:1913:78
#<!-- -->34 0x0000000000c1ba29 operator!=&lt;clang::NamedDecl *, clang::DeclListNode *&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:214:31
#<!-- -->35 0x0000000000c1ba29 operator!= /__w/clangd/clangd/llvm-project/clang/include/clang/AST/DeclBase.h:1350:59
#<!-- -->36 0x0000000000c1ba29 LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaLookup.cpp:1128:21
#<!-- -->37 0x0000000000c17ff3 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaLookup.cpp:2451:7
#<!-- -->38 0x0000000000c8a2e9 clang::Sema::LookupTemplateName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec&amp;, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplate.cpp:0:5
#<!-- -->39 0x0000000000c89e79 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&amp;, bool, clang::UnqualifiedId const&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, clang::OpaquePtr&lt;clang::TemplateName&gt;&amp;, bool&amp;, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplate.cpp:210:7
#<!-- -->40 0x00000000044a5796 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:545:42
#<!-- -->41 0x000000000444da55 clang::Parser::TryAnnotateCXXScopeToken(bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:2252:7
#<!-- -->42 0x0000000004469e45 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:4053:13
#<!-- -->43 0x0000000004469748 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:2961:23
#<!-- -->44 0x000000000445a357 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:63:7
#<!-- -->45 0x00000000044825a3 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::Parser::UsingDeclarator&amp;, clang::SourceLocation&amp;, clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Decl**) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#<!-- -->46 0x000000000448142c clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:853:16
#<!-- -->47 0x0000000004480968 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:533:10
#<!-- -->48 0x0000000004465b05 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:2081:12
#<!-- -->49 0x0000000004448593 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#<!-- -->50 0x000000000447f76b ~AttributePool /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:724:22
#<!-- -->51 0x000000000447f76b ~ParsedAttributes /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:956:7
#<!-- -->52 0x000000000447f76b clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:265:5
#<!-- -->53 0x000000000447f86b Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#<!-- -->54 0x000000000447f86b clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:288:18
#<!-- -->55 0x000000000447f150 Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#<!-- -->56 0x000000000447f150 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:244:18
#<!-- -->57 0x0000000004465a72 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->58 0x0000000004448593 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#<!-- -->59 0x0000000004447569 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->60 0x0000000004443bfe clang::ParseAST(clang::Sema&amp;, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->61 0x00000000040aaefd clang::FrontendAction::Execute() /__w/clangd/clangd/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:10
#<!-- -->62 0x00000000015ba9a4 getPtr /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->63 0x00000000015ba9a4 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->64 0x00000000015ba9a4 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#<!-- -->65 0x0000000001639edf ~__shared_count /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/shared_ptr_base.h:729:6
#<!-- -->66 0x0000000001639edf ~__shared_ptr /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/shared_ptr_base.h:1169:31
#<!-- -->67 0x0000000001639edf clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#<!-- -->68 0x000000000163996e clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_2::operator()() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:0:5
#<!-- -->69 0x0000000001636343 operator() /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->70 0x0000000001636343 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#<!-- -->71 0x000000000163531d run /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1463:5
#<!-- -->72 0x000000000163531d operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#<!-- -->73 0x000000000163531d void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_7&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#<!-- -->74 0x00000000017ade06 operator= /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:48:5
#<!-- -->75 0x00000000017ade06 PunnedPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37:60
#<!-- -->76 0x00000000017ade06 PointerIntPair /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86:13
#<!-- -->77 0x00000000017ade06 UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:343:3
#<!-- -->78 0x00000000017ade06 unique_function /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:372:3
#<!-- -->79 0x00000000017ade06 operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->80 0x00000000017ade06 operator()&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->81 0x00000000017ade06 __invoke_impl&lt;void, (lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:60:14
#<!-- -->82 0x00000000017ade06 __invoke&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:95:14
#<!-- -->83 0x00000000017ade06 __apply_impl&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;, 0&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/tuple:1684:14
#<!-- -->84 0x00000000017ade06 apply&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/tuple:1694:14
#<!-- -->85 0x00000000017ade06 GenericThreadProxy&lt;std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->86 0x00000000017ade06 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->87 0x00007fe56be969cb start_thread /usr/src/debug/glibc/glibc/nptl/pthread_create.c:448:8
#<!-- -->88 0x00007fe56bf1aa0c __GI___clone3 /usr/src/debug/glibc/glibc/misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: /home/alex/Documents/git/zrythm/src/utils/types.h
  Directory: /home/alex/Documents/git/zrythm/builddir_cmake
  Command Line: /usr/bin/clang++ --driver-mode=g++ -DBACKWARD_HAS_DW=1 -DBACKWARD_HAS_UNWIND=1 -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DDEBUG=1 -DHAVE_LITTLE_ENDIAN=1 -DJSON_USE_IMPLICIT_CONVERSIONS=0 -DJUCE_DEBUG=1 -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_PLUGINHOST_AU=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCE_PLUGINHOST_LV2=1 -DJUCE_PLUGINHOST_VST3=1 -DJUCE_USE_MP3AUDIOFORMAT=1 -DJUCE_WEB_BROWSER=0 -DMP_UNITS_API_CONTRACTS=0 -DMP_UNITS_API_NATURAL_UNITS=1 -DMP_UNITS_API_NO_CRTP=0 -DMP_UNITS_API_STD_FORMAT=0 -DMP_UNITS_HOSTED=0 -DQT_CONCURRENT_LIB -DQT_CORE_LIB -DQT_ENABLE_STRICT_MODE_UP_TO=0x060900 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_KEYWORDS -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKCONTROLS2_LIB -DQT_QUICK_LIB -DQT_WIDGETS_LIB -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FUNCTION=__PRETTY_FUNCTION__ -DTBB_USE_DEBUG -DTYPE_SAFE_ARITHMETIC_POLICY=1 -DTYPE_SAFE_ENABLE_ASSERTIONS=1 -DTYPE_SAFE_ENABLE_PRECONDITION_CHECKS=1 -DTYPE_SAFE_ENABLE_WRAPPER=1 -D_DEBUG=1 -I/home/alex/Documents/git/zrythm/builddir_cmake/src/utils/zrythm_utils_lib_autogen/include -I/home/alex/Documents/git/zrythm/src -I/home/alex/Documents/git/zrythm/builddir_cmake/src -I/home/alex/Documents/git/zrythm/ext/rubberband -I/home/alex/Documents/git/zrythm/.cache/CPM/juce/4e98/modules -I/home/alex/Documents/git/zrythm/ext/ladspa -I/home/alex/Documents/git/zrythm/ext/kissfft -isystem /home/alex/Documents/git/zrythm/ext/zita-resampler -isystem /home/alex/Documents/git/zrythm/ext/soxr/src -isystem /home/alex/Documents/git/zrythm/.cache/CPM/zstd/e8f3/build/cmake/../../lib -isystem /home/alex/Documents/git/zrythm/.cache/CPM/xxhash/c13d/cmake_unofficial/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/nlohmann_json/26c2/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/magic_enum/2c54/include/magic_enum -isystem /home/alex/Documents/git/zrythm/ext/backward-cpp -isystem /home/alex/Documents/git/zrythm/ext/crill/.. -isystem /home/alex/Documents/git/zrythm/.cache/CPM/fmt/6173/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/scnlib/f754/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/spdlog/514a/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/type_safe/b200/external/debug_assert -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/describe/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/mp11/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/stl_interfaces/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/assert/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/config/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/container/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/intrusive/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/move/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/unordered/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/container_hash/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/core/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/static_assert/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/throw_exception/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/predef/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/multi_index/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/bind/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/integer/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/type_traits/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/iterator/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/concept_check/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/preprocessor/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/detail/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/fusion/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/function_types/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/mpl/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/utility/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/io/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/tuple/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/typeof/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/functional/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/function/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/optional/include -isystem /home/alex/Documents/git/zrythm/builddir_cmake/_deps/boost-src/libs/smart_ptr/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gsl-lite/1f8f/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap-helpers/c8f1/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/clap/60c7/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/tbb/4990/src/tbb/../../include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/farbot/c1e9/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/mp-units/b278/src/core/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/mp-units/b278/src/systems/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtCore -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/mkspecs/linux-g++ -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtGui -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtWidgets -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuick -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQml -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlIntegration -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtNetwork -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlMeta -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlModels -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQmlWorkerScript -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtOpenGL -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtQuickControls2 -isystem /home/alex/opt/Qt6.10.0-beta3-Debug/include/QtConcurrent -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest/include -isystem /home/alex/Documents/git/zrythm/.cache/CPM/gtest/a317/googletest -isystem /home/alex/Documents/git/zrythm/ext/moodycamel/.. -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -g -fPIC -g -O0 -DSCN_REGEX_BACKEND=0 -Wno-unused-result -fPIC -fsanitize-address-use-after-scope -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Wextra -Wformat=2 -Wno-missing-field-initializers -Wno-unused-parameter -Wno-cast-function-type -Wno-attributes -Werror=format-overflow -Werror=format-truncation -Werror=return-local-addr -Werror=alloca -Werror=cast-align -Werror=cast-qual -Werror=ctor-dtor-privacy -Werror=deprecated -Werror=format-signedness -Wframe-larger-than=65536 -Werror=multichar -Werror=non-virtual-dtor -Werror=range-loop-construct -Werror=disabled-optimization -Werror=pointer-arith -Werror=enum-conversion -Werror=overlength-strings -Werror=int-to-pointer-cast -Werror=shadow -Werror=address -Werror=undef -Werror=unused -Werror=vla -fstrict-aliasing -Wstrict-aliasing=2 -Werror=strict-aliasing -Werror=strict-overflow -Wstrict-overflow=2 -fstrict-overflow -Werror=init-self -Werror=write-strings -Werror=sign-compare -Werror=float-conversion -Werror=uninitialized -Werror=return-type -Werror=switch -Werror=overflow -Werror=array-bounds -Werror=enum-compare -Werror=misleading-indentation -Werror=int-in-bool-context -Werror=type-limits -Werror=endif-labels -Werror=logical-not-parentheses -Werror=parentheses -Werror=comment -Werror=sizeof-pointer-div -Werror=shift-count-overflow -Werror=free-nonheap-object -Werror=nonnull -Werror=tautological-compare -Werror=unused-result -Werror=inline -Werror=sizeof-array-argument -Werror=odr -Werror=narrowing -Werror=reorder -Werror=sequence-point -Werror=invalid-constexpr -Werror=pessimizing-move -pedantic-errors -Werror=pedantic -Werror=vla-cxx-extension -Werror=sentinel -Werror=return-stack-address -Werror=abstract-final-class -Werror=null-dereference -Werror=defaulted-function-deleted -Wexit-time-destructors -Wglobal-constructors -fsafe-buffer-usage-suggestions -Wperf-constraint-implies-noexcept -Winvalid-pch -Xclang -include-pch -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/src/utils/CMakeFiles/zrythm_utils_lib.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/alex/Documents/git/zrythm/builddir_cmake/src/utils/CMakeFiles/zrythm_utils_lib.dir/cmake_pch.hxx -c -x c++-header -std=c++23 -ferror-limit=20 -resource-dir=/home/alex/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/20.1.8/clangd_20.1.8/lib/clang/20 -- /home/alex/Documents/git/zrythm/src/utils/types.h
  Version: 76
[Info  - 19:52:59] Connection to server got closed. Server will restart.
```
</details>


---

### Comment 2 - HighCommander4

If you're also building the project with clang (or if the project is capable of being built with clang as an option), it's worth checking whether the crash also occurs when building with the same clang version as your clangd.

---

### Comment 3 - alex-tee

@HighCommander4 I'm actually building my project with the same clang compiler and it compiles and runs without issues. Seems like a clangd-specific problem to me.

---

### Comment 4 - HighCommander4

Thanks for checking.

It is indeed possible for clangd to exhibit crashes that the compiler does not, even when calling into the clang parser, because clangd exercises codepaths in the clang parser related to serializing and deserializing ASTs that the compiler typically does not (though it can, e.g. if you're using precompiled headers).

The next step here would be to reduce the crashing testcase across its library dependencies to arrive at a reduced testcase that's small enough to feasibly debug. If you're able to help with this step, that's likely to result in the issue being fixed sooner :)

---

