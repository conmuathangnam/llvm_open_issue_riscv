# [clang] segfault with memcpy on incomplete template specification in C++23 mode

**Author:** zhscn
**URL:** https://github.com/llvm/llvm-project/issues/132943
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash, c++23, regression:20
**Closed Date:** 2025-03-25T17:13:32Z

## Body

clang++ crashed with `clang++ -std=c++23 foo.cc`
```foo.cc
#include "system_error2-nowindows.hpp"

using namespace system_error2;

int main() {
  auto code = make_status_code(errc::invalid_argument);
}
```

However, `clang++ -std=c++20 foo.cc` could compile it successfully.

macOS 15.3.2 with M4 chip

clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf) release/20.x branch.

The header comes from https://github.com/ned14/status-code/blob/master/single-header/system_error2-nowindows.hpp

```
Stack dump:
0.	Program arguments: /Users/zhangsong/.local/llvm/bin/clang-20 -cc1 -triple arm64-apple-macosx15.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name foo.cc -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=1 -target-sdk-version=15.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -target-cpu apple-m1 -target-feature +zcm -target-feature +zcz -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debugger-tuning=lldb -fdebug-compilation-dir=/Users/zhangsong/code -target-linker-version 1115.7.3 -fcoverage-compilation-dir=/Users/zhangsong/code -resource-dir /Users/zhangsong/.local/llvm/lib/clang/20 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1 -internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/local/include -internal-isystem /Users/zhangsong/.local/llvm/lib/clang/20/include -internal-externc-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-08fa8f.o -x c++ foo.cc
1.	./system_error2-nowindows.hpp:993:30: current parser token ')'
2.	./system_error2-nowindows.hpp:598:1 <Spelling=./system_error2-nowindows.hpp:382:39>: parsing namespace 'system_error2'
3.	./system_error2-nowindows.hpp:667:1: parsing struct/union/class body 'system_error2::status_code_domain'
4.	./system_error2-nowindows.hpp:985:3: parsing function body 'system_error2::status_code_domain::_do_erased_copy'
5.	./system_error2-nowindows.hpp:985:3: in compound statement ('{}')
 #0 0x0000000113bd9e58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14de58)
 #1 0x0000000113bd80a8 llvm::sys::RunSignalHandlers() (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14c0a8)
 #2 0x0000000113bda4c4 SignalHandler(int) (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14e4c4)
 #3 0x0000000182c92de4 (/usr/lib/system/libsystem_platform.dylib+0x180482de4)
 #4 0x0000000108d27a78 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x807a78)
 #5 0x00000001087eaef8 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2caef8)
 #6 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
 #7 0x00000001087eae4c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cae4c)
 #8 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
 #9 0x00000001087eae4c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cae4c)
#10 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
#11 0x00000001087ebbe4 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbbe4)
#12 0x00000001087ec70c clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cc70c)
#13 0x0000000108a85b9c HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x565b9c)
#14 0x0000000108a76dd0 tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long long&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x556dd0)
#15 0x0000000108a76828 clang::Expr::tryEvaluateObjectSize(unsigned long long&, clang::ASTContext&, unsigned int) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x556828)
#16 0x0000000109041d60 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::$_2::operator()(unsigned int) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xb21d60)
#17 0x0000000109041100 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xb21100)
#18 0x00000001091d55ec clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xcb55ec)
#19 0x000000010943fe80 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xf1fe80)
#20 0x000000010943fcac clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xf1fcac)
#21 0x00000001091c1c90 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xca1c90)
#22 0x00000001091d3904 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xcb3904)
#23 0x000000010873962c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x21962c)
#24 0x000000010873b338 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x21b338)
#25 0x0000000108737240 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x217240)
#26 0x0000000108737138 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x217138)
#27 0x0000000108793634 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x273634)
#28 0x000000010879192c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27192c)
#29 0x000000010879127c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27127c)
#30 0x0000000108799954 clang::Parser::ParseCompoundStatementBody(bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x279954)
#31 0x000000010879a5ec clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27a5ec)
#32 0x00000001086f9080 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d9080)
#33 0x00000001086f7f7c clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d7f7c)
#34 0x000000010872b3a4 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x20b3a4)
#35 0x0000000108728c1c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x208c1c)
#36 0x000000010870ce44 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1ece44)
#37 0x00000001087b0988 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x290988)
#38 0x00000001087b06ac clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2906ac)
#39 0x00000001087afac8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28fac8)
#40 0x0000000108722380 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x202380)
#41 0x0000000108721d4c clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x201d4c)
#42 0x0000000108707f04 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1e7f04)
#43 0x00000001087af400 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28f400)
#44 0x00000001087ae18c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28e18c)
#45 0x00000001086f3480 clang::ParseAST(clang::Sema&, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d3480)
#46 0x000000010a1335bc clang::FrontendAction::Execute() (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1c135bc)
#47 0x000000010a0c6664 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1ba6664)
#48 0x000000010a1a1c9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1c81c9c)
#49 0x000000010471e048 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/zhangsong/.local/llvm/bin/clang-20+0x10000a048)
#50 0x000000010471bac8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/zhangsong/.local/llvm/bin/clang-20+0x100007ac8)
#51 0x000000010471ae34 clang_main(int, char**, llvm::ToolContext const&) (/Users/zhangsong/.local/llvm/bin/clang-20+0x100006e34)
#52 0x00000001047264a0 main (/Users/zhangsong/.local/llvm/bin/clang-20+0x1000124a0)
#53 0x00000001828dc274 
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf)
Target: arm64-apple-darwin24.3.0
Thread model: posix
InstalledDir: /Users/zhangsong/.local/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-9599b6.cpp
clang++: note: diagnostic msg: /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/

clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/zhangsong/Library/Logs/DiagnosticReports/clang-20_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

foo-9599b6.sh:
```
# Crash reproducer for clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf)
# Driver args: "--driver-mode=g++" "-std=c++23" "foo.cc"
# Original command:  "/Users/zhangsong/.local/llvm/bin/clang-20" "-cc1" "-triple" "arm64-apple-macosx15.0.0" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "foo.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=15.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-target-cpu" "apple-m1" "-target-feature" "+zcm" "-target-feature" "+zcz" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/zhangsong/code" "-target-linker-version" "1115.7.3" "-fcoverage-compilation-dir=/Users/zhangsong/code" "-resource-dir" "/Users/zhangsong/.local/llvm/lib/clang/20" "-isysroot" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk" "-internal-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1" "-internal-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/local/include" "-internal-isystem" "/Users/zhangsong/.local/llvm/lib/clang/20/include" "-internal-externc-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include" "-std=c++23" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "1" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fmax-type-align=16" "-fcolor-diagnostics" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-08fa8f.o" "-x" "c++" "foo.cc"
 "/Users/zhangsong/.local/llvm/bin/clang-20" "-cc1" "-triple" "arm64-apple-macosx15.0.0" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "foo.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=15.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-target-cpu" "apple-m1" "-target-feature" "+zcm" "-target-feature" "+zcz" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/zhangsong/code" "-target-linker-version" "1115.7.3" "-fcoverage-compilation-dir=/Users/zhangsong/code" "-std=c++23" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "1" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fmax-type-align=16" "-fcolor-diagnostics" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "foo-9599b6.cpp"
```

[foo-9599b6.cpp.txt](https://github.com/user-attachments/files/19450377/foo-9599b6.cpp.txt)

cc @ned14

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (zhscn)

<details>
clang++ crashed with `clang++ -std=c++23 foo.cc`
```foo.cc
#include "system_error2-nowindows.hpp"

using namespace system_error2;

int main() {
  auto code = make_status_code(errc::invalid_argument);
}
```

However, `clang++ -std=c++20 foo.cc` could compile it successfully.

macOS 15.3.2 with M4 chip

clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf) release/20.x branch.

The header comes from https://github.com/ned14/status-code/blob/master/single-header/system_error2-nowindows.hpp

```
Stack dump:
0.	Program arguments: /Users/zhangsong/.local/llvm/bin/clang-20 -cc1 -triple arm64-apple-macosx15.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name foo.cc -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=1 -target-sdk-version=15.2 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -fdefine-target-os-macros -fno-modulemap-allow-subdirectory-search -target-cpu apple-m1 -target-feature +zcm -target-feature +zcz -target-feature +v8.4a -target-feature +aes -target-feature +altnzcv -target-feature +ccdp -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +flagm -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fptoint -target-feature +fullfp16 -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +predres -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sb -target-feature +sha2 -target-feature +sha3 -target-feature +specrestrict -target-feature +ssbs -target-abi darwinpcs -debugger-tuning=lldb -fdebug-compilation-dir=/Users/zhangsong/code -target-linker-version 1115.7.3 -fcoverage-compilation-dir=/Users/zhangsong/code -resource-dir /Users/zhangsong/.local/llvm/lib/clang/20 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1 -internal-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/local/include -internal-isystem /Users/zhangsong/.local/llvm/lib/clang/20/include -internal-externc-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-08fa8f.o -x c++ foo.cc
1.	./system_error2-nowindows.hpp:993:30: current parser token ')'
2.	./system_error2-nowindows.hpp:598:1 &lt;Spelling=./system_error2-nowindows.hpp:382:39&gt;: parsing namespace 'system_error2'
3.	./system_error2-nowindows.hpp:667:1: parsing struct/union/class body 'system_error2::status_code_domain'
4.	./system_error2-nowindows.hpp:985:3: parsing function body 'system_error2::status_code_domain::_do_erased_copy'
5.	./system_error2-nowindows.hpp:985:3: in compound statement ('{}')
 #<!-- -->0 0x0000000113bd9e58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14de58)
 #<!-- -->1 0x0000000113bd80a8 llvm::sys::RunSignalHandlers() (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14c0a8)
 #<!-- -->2 0x0000000113bda4c4 SignalHandler(int) (/Users/zhangsong/.local/llvm/lib/libLLVM.dylib+0x14e4c4)
 #<!-- -->3 0x0000000182c92de4 (/usr/lib/system/libsystem_platform.dylib+0x180482de4)
 #<!-- -->4 0x0000000108d27a78 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x807a78)
 #<!-- -->5 0x00000001087eaef8 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2caef8)
 #<!-- -->6 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
 #<!-- -->7 0x00000001087eae4c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cae4c)
 #<!-- -->8 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
 #<!-- -->9 0x00000001087eae4c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cae4c)
#<!-- -->10 0x00000001087ebecc clang::ASTContext::getTypeInfo(clang::Type const*) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbecc)
#<!-- -->11 0x00000001087ebbe4 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cbbe4)
#<!-- -->12 0x00000001087ec70c clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2cc70c)
#<!-- -->13 0x0000000108a85b9c HandleSizeof((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::CharUnits&amp;, SizeOfType) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x565b9c)
#<!-- -->14 0x0000000108a76dd0 tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&amp;, unsigned long long&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x556dd0)
#<!-- -->15 0x0000000108a76828 clang::Expr::tryEvaluateObjectSize(unsigned long long&amp;, clang::ASTContext&amp;, unsigned int) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x556828)
#<!-- -->16 0x0000000109041d60 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::$_2::operator()(unsigned int) const (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xb21d60)
#<!-- -->17 0x0000000109041100 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xb21100)
#<!-- -->18 0x00000001091d55ec clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xcb55ec)
#<!-- -->19 0x000000010943fe80 FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xf1fe80)
#<!-- -->20 0x000000010943fcac clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xf1fcac)
#<!-- -->21 0x00000001091c1c90 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xca1c90)
#<!-- -->22 0x00000001091d3904 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0xcb3904)
#<!-- -->23 0x000000010873962c clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x21962c)
#<!-- -->24 0x000000010873b338 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x21b338)
#<!-- -->25 0x0000000108737240 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x217240)
#<!-- -->26 0x0000000108737138 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x217138)
#<!-- -->27 0x0000000108793634 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x273634)
#<!-- -->28 0x000000010879192c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27192c)
#<!-- -->29 0x000000010879127c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27127c)
#<!-- -->30 0x0000000108799954 clang::Parser::ParseCompoundStatementBody(bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x279954)
#<!-- -->31 0x000000010879a5ec clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x27a5ec)
#<!-- -->32 0x00000001086f9080 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d9080)
#<!-- -->33 0x00000001086f7f7c clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d7f7c)
#<!-- -->34 0x000000010872b3a4 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x20b3a4)
#<!-- -->35 0x0000000108728c1c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x208c1c)
#<!-- -->36 0x000000010870ce44 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1ece44)
#<!-- -->37 0x00000001087b0988 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x290988)
#<!-- -->38 0x00000001087b06ac clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x2906ac)
#<!-- -->39 0x00000001087afac8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28fac8)
#<!-- -->40 0x0000000108722380 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x202380)
#<!-- -->41 0x0000000108721d4c clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x201d4c)
#<!-- -->42 0x0000000108707f04 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1e7f04)
#<!-- -->43 0x00000001087af400 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28f400)
#<!-- -->44 0x00000001087ae18c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x28e18c)
#<!-- -->45 0x00000001086f3480 clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1d3480)
#<!-- -->46 0x000000010a1335bc clang::FrontendAction::Execute() (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1c135bc)
#<!-- -->47 0x000000010a0c6664 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1ba6664)
#<!-- -->48 0x000000010a1a1c9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/zhangsong/.local/llvm/lib/libclang-cpp.dylib+0x1c81c9c)
#<!-- -->49 0x000000010471e048 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/zhangsong/.local/llvm/bin/clang-20+0x10000a048)
#<!-- -->50 0x000000010471bac8 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/zhangsong/.local/llvm/bin/clang-20+0x100007ac8)
#<!-- -->51 0x000000010471ae34 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/zhangsong/.local/llvm/bin/clang-20+0x100006e34)
#<!-- -->52 0x00000001047264a0 main (/Users/zhangsong/.local/llvm/bin/clang-20+0x1000124a0)
#<!-- -->53 0x00000001828dc274 
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf)
Target: arm64-apple-darwin24.3.0
Thread model: posix
InstalledDir: /Users/zhangsong/.local/llvm/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-9599b6.cpp
clang++: note: diagnostic msg: /var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/

clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/zhangsong/Library/Logs/DiagnosticReports/clang-20_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

foo-9599b6.sh:
```
# Crash reproducer for clang version 20.1.2 (https://github.com/llvm/llvm-project.git 3f957cc67cff4e337f56fa2dbfdb037d3a997baf)
# Driver args: "--driver-mode=g++" "-std=c++23" "foo.cc"
# Original command:  "/Users/zhangsong/.local/llvm/bin/clang-20" "-cc1" "-triple" "arm64-apple-macosx15.0.0" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "foo.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=15.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-target-cpu" "apple-m1" "-target-feature" "+zcm" "-target-feature" "+zcz" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/zhangsong/code" "-target-linker-version" "1115.7.3" "-fcoverage-compilation-dir=/Users/zhangsong/code" "-resource-dir" "/Users/zhangsong/.local/llvm/lib/clang/20" "-isysroot" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk" "-internal-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1" "-internal-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/local/include" "-internal-isystem" "/Users/zhangsong/.local/llvm/lib/clang/20/include" "-internal-externc-isystem" "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include" "-std=c++23" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "1" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fmax-type-align=16" "-fcolor-diagnostics" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/var/folders/sl/mydhk66n1d1641jxr3cl67dr0000gn/T/foo-08fa8f.o" "-x" "c++" "foo.cc"
 "/Users/zhangsong/.local/llvm/bin/clang-20" "-cc1" "-triple" "arm64-apple-macosx15.0.0" "-Wundef-prefix=TARGET_OS_" "-Werror=undef-prefix" "-Wdeprecated-objc-isa-usage" "-Werror=deprecated-objc-isa-usage" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "foo.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-mframe-pointer=non-leaf" "-ffp-contract=on" "-fno-rounding-math" "-funwind-tables=1" "-target-sdk-version=15.2" "-fcompatibility-qualified-id-block-type-checking" "-fvisibility-inlines-hidden-static-local-var" "-fdefine-target-os-macros" "-fno-modulemap-allow-subdirectory-search" "-target-cpu" "apple-m1" "-target-feature" "+zcm" "-target-feature" "+zcz" "-target-feature" "+v8.4a" "-target-feature" "+aes" "-target-feature" "+altnzcv" "-target-feature" "+ccdp" "-target-feature" "+ccpp" "-target-feature" "+complxnum" "-target-feature" "+crc" "-target-feature" "+dotprod" "-target-feature" "+flagm" "-target-feature" "+fp-armv8" "-target-feature" "+fp16fml" "-target-feature" "+fptoint" "-target-feature" "+fullfp16" "-target-feature" "+jsconv" "-target-feature" "+lse" "-target-feature" "+neon" "-target-feature" "+pauth" "-target-feature" "+perfmon" "-target-feature" "+predres" "-target-feature" "+ras" "-target-feature" "+rcpc" "-target-feature" "+rdm" "-target-feature" "+sb" "-target-feature" "+sha2" "-target-feature" "+sha3" "-target-feature" "+specrestrict" "-target-feature" "+ssbs" "-target-abi" "darwinpcs" "-debugger-tuning=lldb" "-fdebug-compilation-dir=/Users/zhangsong/code" "-target-linker-version" "1115.7.3" "-fcoverage-compilation-dir=/Users/zhangsong/code" "-std=c++23" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "1" "-fblocks" "-fencode-extended-block-signature" "-fregister-global-dtors-with-atexit" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fmax-type-align=16" "-fcolor-diagnostics" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "foo-9599b6.cpp"
```

[foo-9599b6.cpp.txt](https://github.com/user-attachments/files/19450377/foo-9599b6.cpp.txt)

cc @<!-- -->ned14
</details>


---

### Comment 2 - zhscn

Sorry for the vague issue title, but I'm not sure how to create a minimal reproducible example 😢

---

### Comment 3 - zhscn

The minimal example:
```cpp
#include <string.h>

template <class DomainType>
class status_code;

void _do_erased_copy(status_code<void> &dst, const status_code<void> &src) {
  memcpy(&dst, &src, 1);
}
```

https://godbolt.org/z/7nxce4ExY

---

### Comment 4 - shafik

Looks like a clang-20 regression: https://godbolt.org/z/cWEobcGrx

FYI, assertion builds are more useful, generally they point to the source of the problem better.

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/Type.cpp:2387:
bool clang::Type::isConstantSizeType() const:
Assertion `!isIncompleteType() && "This doesn't make sense for incomplete types"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:7:23: current parser token ')'
2.	<source>:6:76: parsing function body '_do_erased_copy'
3.	<source>:6:76: in compound statement ('{}')
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f764c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f764c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f764c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f764c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f764c82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f764c839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007e2184f clang::Type::isConstantSizeType() const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7e2184f)
#10 0x0000000007a0cc5c HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#11 0x0000000007a51cba tryEvaluateBuiltinObjectSize(clang::Expr const*, unsigned int, (anonymous namespace)::EvalInfo&, unsigned long&) ExprConstant.cpp:0:0
#12 0x0000000007a528a3 clang::Expr::tryEvaluateObjectSize(unsigned long&, clang::ASTContext&, unsigned int) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7a528a3)
#13 0x0000000006a72562 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda1'(unsigned int)::operator()(unsigned int) const SemaChecking.cpp:0:0
#14 0x0000000006a7c7a7 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6a7c7a7)
#15 0x0000000006d72c1b clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d72c1b)
#16 0x000000000713b04a FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#17 0x000000000713bf6c clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bf6c)
#18 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#19 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#20 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#21 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#22 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#23 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#24 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#25 0x0000000006945929 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6945929)
#26 0x000000000693bc9e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693bc9e)
#27 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#28 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#29 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#30 0x0000000006850a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6850a53)
#31 0x00000000068856ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68856ad)
#32 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#33 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#34 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#35 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#36 0x000000000683fa5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#37 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#38 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#39 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#40 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#41 0x0000000000cfe40f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#42 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#43 0x000000000483e749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#45 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#47 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#48 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#49 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#50 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#51 0x00007f764c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x00007f764c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - MagentaTreehouse

@shafik Looks like a duplicate of #129397.

---

### Comment 6 - ned14

@zhscn Many thanks for reporting this to LLVM and chasing its fix. Appreciated.

---

