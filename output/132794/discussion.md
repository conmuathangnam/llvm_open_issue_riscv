# [C++ Modules] crash of clang

**Author:** Arthapz
**URL:** https://github.com/llvm/llvm-project/issues/132794
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-03-30T14:36:31Z

## Body

Hello, i noticed a crash when building my project, i made a repro here https://github.com/Arthapz/clang-module-crash
the crash only happend on linux, not on windows / macOS
```cpp
// Foo.mpp
export module Foo;

import std;

template<typename T, typename ... Args>
concept EnableCtor = sizeof...(Args) != 1 || (sizeof...(Args) == 1 && !std::same_as<T, typename std::remove_cvref<Args...>::type>);

export {
template<class T, bool A = false>
class Foo {
  public:
    template<class... Args>
        requires (EnableCtor<Foo<T, A>, Args...>)
    explicit (sizeof...(Args) == 1) Foo(Args&&... args);

    template<class... Args>
    auto init(Args&&... args) -> void;
};
}

template<class T, bool A>
template<class... Args>
    requires (EnableCtor<Foo<T, A>, Args...>)
inline Foo<T, A>::Foo(Args&&... args) {
    init(std::forward<Args>(args)...);
}

template<class T, bool A>
template<class... Args>
inline auto Foo<T, A>::init(Args&&... args) -> void {
    auto _ = std::make_unique<T>(std::forward<Args>(args)...);
}
```

```cpp
// main.cpp
import Foo;

struct Bar {
  
};

int main() {
  auto foo = Foo<Bar>{};
  return 0;
}
```


```
$ rm -rf .xmake build; xmake f --toolchain=llvm --runtimes="c++_shared" --sdk=/opt/llvm-git/; xmake b -vD | save -f README.md
checking for flags (clang_print_library_module_manifest_path) ... ok
> clang "-print-library-module-manifest-path" "-Qunused-arguments" "-m64"
checking for flags (-std=c++26) ... ok
> clang "-std=c++26" "-Qunused-arguments" "-m64"
[  0%]: <Foo> generating.module.deps src/Foo.mpp
checking for /opt/llvm-git/bin/clang-scan-deps ... ok
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c src/Foo.mpp -o build/.objs/Foo/linux/x86_64/release/src/Foo.mpp.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: <Foo> generating.module.deps src/main.cpp
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c src/main.cpp -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: <Foo> generating.module.deps /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.compat.cppm.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: <Foo> generating.module.deps /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.cppm.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[ 33%]: <Foo> compiling.module.release std
checking for flags (clang_module_output) ... ok
> clang "-fmodule-output=" "-Qunused-arguments" "-m64"
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -Wno-include-angled-in-module-purview -Wno-reserved-module-identifier -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.cppm.o /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm
checking for flags (clang_module_file) ... ok
> clang "-fmodule-file=/tmp/.xmake1000/250324/_5AEC64D4F4EE43008365A2C298B95140.pcm" "-Qunused-arguments" "-m64"
[ 55%]: <Foo> compiling.module.release std.compat
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.compat.pcm -Wno-include-angled-in-module-purview -Wno-reserved-module-identifier -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.compat.cppm.o /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm
[ 55%]: <Foo> compiling.module.release Foo
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -o build/.objs/Foo/linux/x86_64/release/src/Foo.mpp.o src/Foo.mpp
[ 77%]: compiling.release src/main.cpp
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=Foo=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o src/main.cpp
checking for flags (-MMD -MF) ... ok
> clang "-MMD" "-MF" "/dev/null" "-Qunused-arguments" "-m64"
error: @programdir/core/main.lua:329: @programdir/actions/build/main.lua:146: @programdir/modules/async/runjobs.lua:330: @programdir/modules/private/action/build/object.lua:100: @programdir/modules/core/tools/gcc.lua:980: clang: /home/arthapz/Development/AUR/llvm-git/src/llvm-project/clang/include/clang/AST/DeclCXX.h:2648: void clang::CXXConstructorDecl::setExplicitSpecifier(ExplicitSpecifier): Assertion `(!ES.getExpr() || CXXConstructorDeclBits.HasTrailingExplicitSpecifier) && "cannot set this explicit specifier. no trail-allocated space for " "explicit"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=Foo=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -MMD -MF /tmp/.xmake1000/250324/_6C557976DCA64A2082224442AF497340 -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o src/main.cpp
1.	src/main.cpp:8:24: current parser token ';'
2.	src/main.cpp:7:12: parsing function body 'main'
3.	src/main.cpp:7:12: in compound statement ('{}')
4.	/home/arthapz/Development/Repos/crash-clang/src/Foo.mpp:10:7: instantiating class definition 'Foo<Bar>'
 #0 0x00007c6857eab98a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0xaab98a)
 #1 0x00007c6857ea9262 llvm::sys::RunSignalHandlers() (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0xaa9262)
 #2 0x00007c6857dda961 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c6856e4bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #4 0x00007c6856ea5624 (/usr/lib/libc.so.6+0x97624)
 #5 0x00007c6856e4bba0 raise (/usr/lib/libc.so.6+0x3dba0)
 #6 0x00007c6856e33582 abort (/usr/lib/libc.so.6+0x25582)
 #7 0x00007c6856e334eb __assert_perror_fail (/usr/lib/libc.so.6+0x254eb)
 #8 0x00007c685d989755 clang::CXXConstructorDecl::setExplicitSpecifier(clang::ExplicitSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb89755)
 #9 0x00007c685f72f1f6 clang::ASTDeclReader::VisitCXXConstructorDecl(clang::CXXConstructorDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x292f1f6)
#10 0x00007c685f71f633 clang::ASTDeclReader::Visit(clang::Decl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x291f633)
#11 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#12 0x00007c685f7483c8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x29483c8)
#13 0x00007c685f6c8c29 clang::ASTReader::GetDecl(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28c8c29)
#14 0x00007c685f72faea clang::ASTDeclReader::VisitTemplateDecl(clang::TemplateDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x292faea)
#15 0x00007c685f7306b0 clang::ASTDeclReader::VisitRedeclarableTemplateDecl(clang::RedeclarableTemplateDecl*) ASTReaderDecl.cpp:0:0
#16 0x00007c685f73144f clang::ASTDeclReader::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x293144f)
#17 0x00007c685f71f633 clang::ASTDeclReader::Visit(clang::Decl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x291f633)
#18 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#19 0x00007c685f7483c8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x29483c8)
#20 0x00007c685f6c8c29 clang::ASTReader::GetDecl(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28c8c29)
#21 0x00007c685f74c9c8 clang::ASTReader::loadPendingDeclChain(clang::Decl*, unsigned long) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x294c9c8)
#22 0x00007c685f6e2c83 clang::ASTReader::finishPendingActions() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28e2c83)
#23 0x00007c685f6e9770 clang::ASTReader::FinishedDeserializing() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28e9770)
#24 0x00007c685d9788ca clang::DeclContext::LoadLexicalDeclsFromExternalStorage() const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb788ca)
#25 0x00007c685d978eb6 clang::DeclContext::decls_begin() const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb78eb6)
#26 0x00007c685eaf6bb5 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1cf6bb5)
#27 0x00007c685eaf937a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1cf937a)
#28 0x00007c685ec0c8d2 void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0>(long) SemaType.cpp:0:0
#29 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#30 0x00007c685ebf3928 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1df3928)
#31 0x00007c685ebf31c4 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1df31c4)
#32 0x00007c685e6601af clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x18601af)
#33 0x00007c685e65fa1a clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x185fa1a)
#34 0x00007c685d677502 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x877502)
#35 0x00007c685d660aaa clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x860aaa)
#36 0x00007c685d660522 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x860522)
#37 0x00007c685d65b216 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x85b216)
#38 0x00007c685d6222ab clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8222ab)
#39 0x00007c685d61f4d2 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81f4d2)
#40 0x00007c685d61e496 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81e496)
#41 0x00007c685d61dccc clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81dccc)
#42 0x00007c685d6cbcdd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8cbcdd)
#43 0x00007c685d6cb3bc clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8cb3bc)
#44 0x00007c685d6d5902 clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8d5902)
#45 0x00007c685d6d6614 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8d6614)
#46 0x00007c685d6f25da clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f25da)
#47 0x00007c685d620541 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x820541)
#48 0x00007c685d6f1711 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f1711)
#49 0x00007c685d6f0f05 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f0f05)
#50 0x00007c685d6efdf9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8efdf9)
#51 0x00007c685d6ee153 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8ee153)
#52 0x00007c685d60656e clang::ParseAST(clang::Sema&, bool, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x80656e)
#53 0x00007c685f9d24f8 clang::FrontendAction::Execute() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2bd24f8)
#54 0x00007c685f93a79d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2b3a79d)
#55 0x00007c685fa6e85d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2c6e85d)
#56 0x0000599cf8ac945b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/llvm-git/bin/clang+0x1245b)
#57 0x0000599cf8ac54e9 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#58 0x00007c685f4f1f6d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#59 0x00007c6857dda666 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0x9da666)
#60 0x00007c685f4f1798 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26f1798)
#61 0x00007c685f4b197c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26b197c)
#62 0x00007c685f4b1bc7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26b1bc7)
#63 0x00007c685f4d0b69 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26d0b69)
#64 0x0000599cf8ac4d47 clang_main(int, char**, llvm::ToolContext const&) (/opt/llvm-git/bin/clang+0xdd47)
#65 0x0000599cf8ad445a main (/opt/llvm-git/bin/clang+0x1d45a)
#66 0x00007c6856e35488 (/usr/lib/libc.so.6+0x27488)
#67 0x00007c6856e3554c __libc_start_main (/usr/lib/libc.so.6+0x2754c)
#68 0x0000599cf8ac2e55 _start (/opt/llvm-git/bin/clang+0xbe55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0 (/home/arthapz/Development/AUR/llvm-git/llvm-project ed022d93b2fbfe52b7bdee786aa5cc49fa2323c4)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm-git/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************
PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-e02457.cpp
clang: note: diagnostic msg: /tmp/main-e02457.sh
clang: note: diagnostic msg: 
********************
stack traceback:
    [C]: in function 'error'
    [@programdir/core/base/os.lua:1075]:
    [@programdir/modules/core/tools/gcc.lua:980]: in function 'catch'
    [@programdir/core/sandbox/modules/try.lua:123]: in function 'try'
    [@programdir/modules/core/tools/gcc.lua:924]:
    [C]: in function 'xpcall'
    [@programdir/core/base/utils.lua:246]:
    [@programdir/core/tool/compiler.lua:288]: in function 'compile'
    [@programdir/modules/private/action/build/object.lua:100]: in function 'script'
    [@programdir/modules/private/action/build/object.lua:131]: in function 'build_object'
    [@programdir/modules/private/action/build/object.lua:156]: in function 'jobfunc'
    [@programdir/modules/async/runjobs.lua:246]:
    [C]: in function 'xpcall'
    [@programdir/core/base/utils.lua:246]: in function 'trycall'
    [@programdir/core/sandbox/modules/try.lua:117]: in function 'try'
    [@programdir/modules/async/runjobs.lua:229]: in function 'cotask'
    [@programdir/core/base/scheduler.lua:406]:

stack traceback:
	[C]: in function 'error'
	@programdir/core/base/os.lua:1075: in function 'os.raiselevel'
	(...tail calls...)
	@programdir/core/main.lua:329: in upvalue 'cotask'
	@programdir/core/base/scheduler.lua:406: in function <@programdir/core/base/scheduler.lua:399>
```

## Comments

### Comment 1 - Arthapz

when removing ` explicit (sizeof...(Args) == 1)` it doesn't crash

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Arthur Laurent (Arthapz)

<details>
Hello, i noticed a crash when building my project, i made a repro here https://github.com/Arthapz/clang-module-crash

```cpp
// Foo.mpp
export module Foo;

import std;

template&lt;typename T, typename ... Args&gt;
concept EnableCtor = sizeof...(Args) != 1 || (sizeof...(Args) == 1 &amp;&amp; !std::same_as&lt;T, typename std::remove_cvref&lt;Args...&gt;::type&gt;);

export {
template&lt;class T, bool A = false&gt;
class Foo {
  public:
    template&lt;class... Args&gt;
        requires (EnableCtor&lt;Foo&lt;T, A&gt;, Args...&gt;)
    explicit (sizeof...(Args) == 1) Foo(Args&amp;&amp;... args);

    template&lt;class... Args&gt;
    auto init(Args&amp;&amp;... args) -&gt; void;
};
}

template&lt;class T, bool A&gt;
template&lt;class... Args&gt;
    requires (EnableCtor&lt;Foo&lt;T, A&gt;, Args...&gt;)
inline Foo&lt;T, A&gt;::Foo(Args&amp;&amp;... args) {
    init(std::forward&lt;Args&gt;(args)...);
}

template&lt;class T, bool A&gt;
template&lt;class... Args&gt;
inline auto Foo&lt;T, A&gt;::init(Args&amp;&amp;... args) -&gt; void {
    auto _ = std::make_unique&lt;T&gt;(std::forward&lt;Args&gt;(args)...);
}
```

```cpp
// main.cpp
import Foo;

struct Bar {
  
};

int main() {
  auto foo = Foo&lt;Bar&gt;{};
  return 0;
}
```


```
$ rm -rf .xmake build; xmake f --toolchain=llvm --runtimes="c++_shared" --sdk=/opt/llvm-git/; xmake b -vD | save -f README.md
checking for flags (clang_print_library_module_manifest_path) ... ok
&gt; clang "-print-library-module-manifest-path" "-Qunused-arguments" "-m64"
checking for flags (-std=c++26) ... ok
&gt; clang "-std=c++26" "-Qunused-arguments" "-m64"
[  0%]: &lt;Foo&gt; generating.module.deps src/Foo.mpp
checking for /opt/llvm-git/bin/clang-scan-deps ... ok
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c src/Foo.mpp -o build/.objs/Foo/linux/x86_64/release/src/Foo.mpp.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: &lt;Foo&gt; generating.module.deps src/main.cpp
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c src/main.cpp -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: &lt;Foo&gt; generating.module.deps /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.compat.cppm.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[  0%]: &lt;Foo&gt; generating.module.deps /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm
/opt/llvm-git/bin/clang-scan-deps --format=p1689 -- /opt/llvm-git/bin/clang -x c++ -c /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.cppm.o -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1
[ 33%]: &lt;Foo&gt; compiling.module.release std
checking for flags (clang_module_output) ... ok
&gt; clang "-fmodule-output=" "-Qunused-arguments" "-m64"
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -Wno-include-angled-in-module-purview -Wno-reserved-module-identifier -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.cppm.o /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.cppm
checking for flags (clang_module_file) ... ok
&gt; clang "-fmodule-file=/tmp/.xmake1000/250324/_5AEC64D4F4EE43008365A2C298B95140.pcm" "-Qunused-arguments" "-m64"
[ 55%]: &lt;Foo&gt; compiling.module.release std.compat
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.compat.pcm -Wno-include-angled-in-module-purview -Wno-reserved-module-identifier -o build/.objs/Foo/linux/x86_64/release/opt/llvm-git/bin/__/lib/x86_64-pc-linux-gnu/__/__/share/libc++/v1/std.compat.cppm.o /opt/llvm-git/bin/../lib/x86_64-pc-linux-gnu/../../share/libc++/v1/std.compat.cppm
[ 55%]: &lt;Foo&gt; compiling.module.release Foo
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -x c++-module -fmodule-output=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -o build/.objs/Foo/linux/x86_64/release/src/Foo.mpp.o src/Foo.mpp
[ 77%]: compiling.release src/main.cpp
/opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=Foo=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o src/main.cpp
checking for flags (-MMD -MF) ... ok
&gt; clang "-MMD" "-MF" "/dev/null" "-Qunused-arguments" "-m64"
error: @<!-- -->programdir/core/main.lua:329: @<!-- -->programdir/actions/build/main.lua:146: @<!-- -->programdir/modules/async/runjobs.lua:330: @<!-- -->programdir/modules/private/action/build/object.lua:100: @<!-- -->programdir/modules/core/tools/gcc.lua:980: clang: /home/arthapz/Development/AUR/llvm-git/src/llvm-project/clang/include/clang/AST/DeclCXX.h:2648: void clang::CXXConstructorDecl::setExplicitSpecifier(ExplicitSpecifier): Assertion `(!ES.getExpr() || CXXConstructorDeclBits.HasTrailingExplicitSpecifier) &amp;&amp; "cannot set this explicit specifier. no trail-allocated space for " "explicit"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/llvm-git/bin/clang -c -Qunused-arguments -m64 -std=c++26 -stdlib=libc++ -cxx-isystem/opt/llvm-git/include/c++/v1 -fmodule-file=Foo=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/Foo.pcm -fmodule-file=std=build/.gens/Foo/linux/x86_64/release/rules/bmi/cache/modules/f55c7a20/std.pcm -MMD -MF /tmp/.xmake1000/250324/_6C557976DCA64A2082224442AF497340 -o build/.objs/Foo/linux/x86_64/release/src/main.cpp.o src/main.cpp
1.	src/main.cpp:8:24: current parser token ';'
2.	src/main.cpp:7:12: parsing function body 'main'
3.	src/main.cpp:7:12: in compound statement ('{}')
4.	/home/arthapz/Development/Repos/crash-clang/src/Foo.mpp:10:7: instantiating class definition 'Foo&lt;Bar&gt;'
 #<!-- -->0 0x00007c6857eab98a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0xaab98a)
 #<!-- -->1 0x00007c6857ea9262 llvm::sys::RunSignalHandlers() (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0xaa9262)
 #<!-- -->2 0x00007c6857dda961 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c6856e4bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->4 0x00007c6856ea5624 (/usr/lib/libc.so.6+0x97624)
 #<!-- -->5 0x00007c6856e4bba0 raise (/usr/lib/libc.so.6+0x3dba0)
 #<!-- -->6 0x00007c6856e33582 abort (/usr/lib/libc.so.6+0x25582)
 #<!-- -->7 0x00007c6856e334eb __assert_perror_fail (/usr/lib/libc.so.6+0x254eb)
 #<!-- -->8 0x00007c685d989755 clang::CXXConstructorDecl::setExplicitSpecifier(clang::ExplicitSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb89755)
 #<!-- -->9 0x00007c685f72f1f6 clang::ASTDeclReader::VisitCXXConstructorDecl(clang::CXXConstructorDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x292f1f6)
#<!-- -->10 0x00007c685f71f633 clang::ASTDeclReader::Visit(clang::Decl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x291f633)
#<!-- -->11 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#<!-- -->12 0x00007c685f7483c8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x29483c8)
#<!-- -->13 0x00007c685f6c8c29 clang::ASTReader::GetDecl(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28c8c29)
#<!-- -->14 0x00007c685f72faea clang::ASTDeclReader::VisitTemplateDecl(clang::TemplateDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x292faea)
#<!-- -->15 0x00007c685f7306b0 clang::ASTDeclReader::VisitRedeclarableTemplateDecl(clang::RedeclarableTemplateDecl*) ASTReaderDecl.cpp:0:0
#<!-- -->16 0x00007c685f73144f clang::ASTDeclReader::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x293144f)
#<!-- -->17 0x00007c685f71f633 clang::ASTDeclReader::Visit(clang::Decl*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x291f633)
#<!-- -->18 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#<!-- -->19 0x00007c685f7483c8 clang::ASTReader::ReadDeclRecord(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x29483c8)
#<!-- -->20 0x00007c685f6c8c29 clang::ASTReader::GetDecl(clang::GlobalDeclID) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28c8c29)
#<!-- -->21 0x00007c685f74c9c8 clang::ASTReader::loadPendingDeclChain(clang::Decl*, unsigned long) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x294c9c8)
#<!-- -->22 0x00007c685f6e2c83 clang::ASTReader::finishPendingActions() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28e2c83)
#<!-- -->23 0x00007c685f6e9770 clang::ASTReader::FinishedDeserializing() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x28e9770)
#<!-- -->24 0x00007c685d9788ca clang::DeclContext::LoadLexicalDeclsFromExternalStorage() const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb788ca)
#<!-- -->25 0x00007c685d978eb6 clang::DeclContext::decls_begin() const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0xb78eb6)
#<!-- -->26 0x00007c685eaf6bb5 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1cf6bb5)
#<!-- -->27 0x00007c685eaf937a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1cf937a)
#<!-- -->28 0x00007c685ec0c8d2 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0&gt;(long) SemaType.cpp:0:0
#<!-- -->29 0x00007c685d478f31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x678f31)
#<!-- -->30 0x00007c685ebf3928 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1df3928)
#<!-- -->31 0x00007c685ebf31c4 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x1df31c4)
#<!-- -->32 0x00007c685e6601af clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x18601af)
#<!-- -->33 0x00007c685e65fa1a clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x185fa1a)
#<!-- -->34 0x00007c685d677502 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x877502)
#<!-- -->35 0x00007c685d660aaa clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x860aaa)
#<!-- -->36 0x00007c685d660522 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x860522)
#<!-- -->37 0x00007c685d65b216 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x85b216)
#<!-- -->38 0x00007c685d6222ab clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8222ab)
#<!-- -->39 0x00007c685d61f4d2 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81f4d2)
#<!-- -->40 0x00007c685d61e496 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81e496)
#<!-- -->41 0x00007c685d61dccc clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x81dccc)
#<!-- -->42 0x00007c685d6cbcdd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8cbcdd)
#<!-- -->43 0x00007c685d6cb3bc clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8cb3bc)
#<!-- -->44 0x00007c685d6d5902 clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8d5902)
#<!-- -->45 0x00007c685d6d6614 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8d6614)
#<!-- -->46 0x00007c685d6f25da clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f25da)
#<!-- -->47 0x00007c685d620541 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x820541)
#<!-- -->48 0x00007c685d6f1711 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f1711)
#<!-- -->49 0x00007c685d6f0f05 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8f0f05)
#<!-- -->50 0x00007c685d6efdf9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8efdf9)
#<!-- -->51 0x00007c685d6ee153 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x8ee153)
#<!-- -->52 0x00007c685d60656e clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x80656e)
#<!-- -->53 0x00007c685f9d24f8 clang::FrontendAction::Execute() (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2bd24f8)
#<!-- -->54 0x00007c685f93a79d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2b3a79d)
#<!-- -->55 0x00007c685fa6e85d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x2c6e85d)
#<!-- -->56 0x0000599cf8ac945b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/llvm-git/bin/clang+0x1245b)
#<!-- -->57 0x0000599cf8ac54e9 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->58 0x00007c685f4f1f6d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->59 0x00007c6857dda666 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/llvm-git/bin/../lib/libLLVM.so.21.0+0x9da666)
#<!-- -->60 0x00007c685f4f1798 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26f1798)
#<!-- -->61 0x00007c685f4b197c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26b197c)
#<!-- -->62 0x00007c685f4b1bc7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26b1bc7)
#<!-- -->63 0x00007c685f4d0b69 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/llvm-git/bin/../lib/libclang-cpp.so.21.0+0x26d0b69)
#<!-- -->64 0x0000599cf8ac4d47 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/llvm-git/bin/clang+0xdd47)
#<!-- -->65 0x0000599cf8ad445a main (/opt/llvm-git/bin/clang+0x1d45a)
#<!-- -->66 0x00007c6856e35488 (/usr/lib/libc.so.6+0x27488)
#<!-- -->67 0x00007c6856e3554c __libc_start_main (/usr/lib/libc.so.6+0x2754c)
#<!-- -->68 0x0000599cf8ac2e55 _start (/opt/llvm-git/bin/clang+0xbe55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0 (/home/arthapz/Development/AUR/llvm-git/llvm-project ed022d93b2fbfe52b7bdee786aa5cc49fa2323c4)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm-git/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************
PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-e02457.cpp
clang: note: diagnostic msg: /tmp/main-e02457.sh
clang: note: diagnostic msg: 
********************
stack traceback:
    [C]: in function 'error'
    [@<!-- -->programdir/core/base/os.lua:1075]:
    [@<!-- -->programdir/modules/core/tools/gcc.lua:980]: in function 'catch'
    [@<!-- -->programdir/core/sandbox/modules/try.lua:123]: in function 'try'
    [@<!-- -->programdir/modules/core/tools/gcc.lua:924]:
    [C]: in function 'xpcall'
    [@<!-- -->programdir/core/base/utils.lua:246]:
    [@<!-- -->programdir/core/tool/compiler.lua:288]: in function 'compile'
    [@<!-- -->programdir/modules/private/action/build/object.lua:100]: in function 'script'
    [@<!-- -->programdir/modules/private/action/build/object.lua:131]: in function 'build_object'
    [@<!-- -->programdir/modules/private/action/build/object.lua:156]: in function 'jobfunc'
    [@<!-- -->programdir/modules/async/runjobs.lua:246]:
    [C]: in function 'xpcall'
    [@<!-- -->programdir/core/base/utils.lua:246]: in function 'trycall'
    [@<!-- -->programdir/core/sandbox/modules/try.lua:117]: in function 'try'
    [@<!-- -->programdir/modules/async/runjobs.lua:229]: in function 'cotask'
    [@<!-- -->programdir/core/base/scheduler.lua:406]:

stack traceback:
	[C]: in function 'error'
	@<!-- -->programdir/core/base/os.lua:1075: in function 'os.raiselevel'
	(...tail calls...)
	@<!-- -->programdir/core/main.lua:329: in upvalue 'cotask'
	@<!-- -->programdir/core/base/scheduler.lua:406: in function &lt;@<!-- -->programdir/core/base/scheduler.lua:399&gt;
```
</details>


---

### Comment 3 - koplas

@Arthapz Can you provide a reproducer without `import std`?

---

### Comment 4 - Nya-Enzo

i think i figured it out... Crash of Clang is actually Clash of Clans... holy fart.

---

### Comment 5 - Arthapz

> [@Arthapz](https://github.com/Arthapz) Can you provide a reproducer without `import std`?

i updated the repo to remove import std

---

### Comment 6 - koplas

Note for me, the crash happens because in `ASTDeclWriter::VisitCXXConstructorDecl`, an `ExplicitSpecifier` with an Expression gets written that does not have the `CXXConstructorDeclBits.HasTrailingExplicitSpecifier` set.
![Image](https://github.com/user-attachments/assets/f7b9c293-84b9-4747-92f7-4d0aaef7ff0a)

---

### Comment 7 - koplas

The reason for this crash is here:
https://github.com/llvm/llvm-project/blob/1a7402d3d831940fc04cc0fb5731239744ce5435/clang/include/clang/AST/DeclCXX.h#L2655-L2660

`ASTDeclWriter::VisitCXXConstructorDecl` calls `D->getExplicitSpecifier()`, which returns the `ExplicitSpecifier` of the canonical declaration and not the current one.

The canonical declaration has the `CXXConstructorDeclBits.HasTrailingExplicitSpecifier` bit set, but the current declaration doesn't. The result is that the `ExplicitSpecifier` of the canonical declaration is used, but the `AllocKind` flags of the current declaration are used.

Using the AllocKind flags from the canonical declaration could be enough. Here is an untested patch:
```diff
diff --git a/clang/lib/Serialization/ASTWriterDecl.cpp b/clang/lib/Serialization/ASTWriterDecl.cpp
--- a/clang/lib/Serialization/ASTWriterDecl.cpp	(revision 5f58f3dda8b17f664a85d4e5e3c808edde41ff46)
+++ b/clang/lib/Serialization/ASTWriterDecl.cpp	(date 1742986625385)
@@ -1725,7 +1725,7 @@
                 "You need to update the serializer after you change the "
                 "CXXConstructorDeclBits");
 
-  Record.push_back(D->getTrailingAllocKind());
+  Record.push_back(D->getCanonicalDecl()->getTrailingAllocKind());
   addExplicitSpecifier(D->getExplicitSpecifier(), Record);
   if (auto Inherited = D->getInheritedConstructor()) {
     Record.AddDeclRef(Inherited.getShadowDecl());
```

@Arthapz can you test if this fixes your problem? It would be kind if you could also reduce the reproducer by textual including the system headers and removing all code that is not needed for the crash.

Update: It can make more sense to fix `getTrailingAllocKind`, like it is done for `getExplicitSpecifier`: https://github.com/llvm/llvm-project/commit/914c4c306d1206670c3486987b55fedc11eccee4.

```diff
diff --git a/clang/include/clang/AST/DeclCXX.h b/clang/include/clang/AST/DeclCXX.h
--- a/clang/include/clang/AST/DeclCXX.h	(revision 5f58f3dda8b17f664a85d4e5e3c808edde41ff46)
+++ b/clang/include/clang/AST/DeclCXX.h	(date 1742988295120)
@@ -2601,10 +2601,10 @@
   void anchor() override;
 
   size_t numTrailingObjects(OverloadToken<InheritedConstructor>) const {
-    return CXXConstructorDeclBits.IsInheritingConstructor;
+    return getCanonicalDecl()->CXXConstructorDeclBits.IsInheritingConstructor;
   }
   size_t numTrailingObjects(OverloadToken<ExplicitSpecifier>) const {
-    return CXXConstructorDeclBits.HasTrailingExplicitSpecifier;
+    return getCanonicalDecl()->CXXConstructorDeclBits.HasTrailingExplicitSpecifier;
   }
 
   ExplicitSpecifier getExplicitSpecifierInternal() const {
```

---

### Comment 8 - Arthapz

the patch it seems to work on my side (repro and my main module project) 
and i've simplified the repro to remove std headers

---

### Comment 9 - koplas

Sent #133077

---

### Comment 10 - Arthapz

now that [#133077](https://github.com/llvm/llvm-project/pull/133077) is merged should i close the issue ? i updated my llvm build and it work

---

