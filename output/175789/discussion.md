# Using `__funcref` crashes Clang

**Author:** geraintluff
**URL:** https://github.com/llvm/llvm-project/issues/175789
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash
**Closed Date:** 2026-01-13T21:48:52Z

## Body

While trying to get Clang to export a function to a `.wasm` module, I added `__funcref` (and reflexively saved, so it attempted to build):

```cpp
extern "C" {
	__funcref
	HostedWclap * makeHosted(Instance *instance) {
		return HostedWclap::create(instance);
	}
	//...
}
```

It crashed, and asked me to create this report/issue.

[stack-dump.txt](https://github.com/user-attachments/files/24594927/stack-dump.txt)

```console
 #0 0x000000010abd4934 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libLLVM.dylib+0xf8934)
 #1 0x000000010abd4378 llvm::sys::CleanupOnSignal(unsigned long) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libLLVM.dylib+0xf8378)
 #2 0x000000010ab44100 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libLLVM.dylib+0x68100)
 #3 0x000000010ab442f8 CrashRecoverySignalHandler(int) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libLLVM.dylib+0x682f8)
 #4 0x00000001836816a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x00000001086a0a50 processTypeAttrs((anonymous namespace)::TypeProcessingState&, clang::QualType&, TypeAttrLocation, clang::ParsedAttributesView const&, clang::CUDAFunctionTarget) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0xdd4a50)
 #6 0x00000001086a0a50 processTypeAttrs((anonymous namespace)::TypeProcessingState&, clang::QualType&, TypeAttrLocation, clang::ParsedAttributesView const&, clang::CUDAFunctionTarget) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0xdd4a50)
 #7 0x0000000108696034 GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::TypeSourceInfo*&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0xdca034)
 #8 0x0000000108694988 clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0xdc8988)
 #9 0x0000000108285090 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x9b9090)
#10 0x000000010829aa70 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x9cea70)
#11 0x0000000107acff08 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x203f08)
#12 0x0000000107a4c4a4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x1804a4)
#13 0x0000000107acf660 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x203660)
#14 0x0000000107acf100 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x203100)
#15 0x0000000107ace974 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x202974)
#16 0x0000000107a5fda8 clang::Parser::ParseLinkage(clang::ParsingDeclSpec&, clang::DeclaratorContext) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x193da8)
#17 0x0000000107acf634 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x203634)
#18 0x0000000107acf100 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x203100)
#19 0x0000000107ace974 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x202974)
#20 0x0000000107acd568 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x201568)
#21 0x0000000107a3c1e4 clang::ParseAST(clang::Sema&, bool, bool) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x1701e4)
#22 0x0000000108de161c clang::FrontendAction::Execute() (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x151561c)
#23 0x0000000108d8e7ec clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x14c27ec)
#24 0x0000000108e3c62c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x157062c)
#25 0x00000001042f9b04 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/bin/clang-21+0x100005b04)
#26 0x00000001042f78f0 clang_main(int, char**, llvm::ToolContext const&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/bin/clang-21+0x1000038f0)
#27 0x0000000108b6a97c void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_1>(long) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x129e97c)
#28 0x000000010ab44088 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libLLVM.dylib+0x68088)
#29 0x0000000108b6a1f4 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x129e1f4)
#30 0x0000000108b42564 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x1276564)
#31 0x0000000108b42688 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x1276688)
#32 0x0000000108b53a54 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/lib/libclang-cpp.dylib+0x1287a54)
#33 0x00000001042f7210 clang_main(int, char**, llvm::ToolContext const&) (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/bin/clang-21+0x100003210)
#34 0x0000000104301b88 main (/Users/geraintluff/Development/others/wasi-sdk/wasi-sdk-29.0-arm64-macos/bin/clang-21+0x10000db88)
#35 0x00000001832a6b98
```

[host-20b392.sh](https://github.com/user-attachments/files/24594997/host-20b392.sh)

[host-20b392.cpp.gz](https://github.com/user-attachments/files/24595017/host-20b392.cpp.gz)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Geraint (geraintluff)

<details>
While trying to get Clang to export a function to a `.wasm` module, I added `__funcref` (and reflexively saved, so it attempted to build):

```cpp
extern "C" {
	__funcref
	HostedWclap * makeHosted(Instance *instance) {
		return HostedWclap::create(instance);
	}
	//...
}
```

It crashed, and asked me to create this report/issue.

[stack-dump.txt](https://github.com/user-attachments/files/24594927/stack-dump.txt)

[host-20b392.sh](https://github.com/user-attachments/files/24594997/host-20b392.sh)

[host-20b392.cpp.gz](https://github.com/user-attachments/files/24595017/host-20b392.cpp.gz)
</details>


---

### Comment 2 - shafik

Maybe dup: https://github.com/llvm/llvm-project/issues/118233

---

### Comment 3 - AaronBallman

It reproduces with:
```
__funcref int foo();
```
https://godbolt.org/z/1oqEeaxq1

---

### Comment 4 - AaronBallman

Yeah, this is a duplicate of #118233 so closing as dup.

---

