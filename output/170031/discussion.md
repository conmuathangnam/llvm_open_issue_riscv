# [clang][bytecode] Assertion `Offset != PastEndMark && "invalid offset"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170031
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-12-01T11:43:37Z

## Body

Reproducer:
https://godbolt.org/z/1E19xvsqW
```cpp
namespace std {

struct source_location {
  struct __impl {
    const char *_M_file_name;
    const char *_M_function_name;
    unsigned _M_line;
    unsigned _M_column;
  } const *_M_impl;

  static constexpr source_location
  current(const void *p = __builtin_source_location()) {
    return source_location{static_cast<const __impl *>(p)};
  }

  constexpr const char *function_name() const {
    return _M_impl ? _M_impl->_M_function_name : "";
  }
};

struct char_traits {
  static constexpr long length(const char *s) { return __builtin_strlen(s); }
};

template <typename C, typename Traits = char_traits> class basic_string_view {
public:
  using size_type = unsigned long;

  constexpr basic_string_view(const C *s)
      : _M_len{Traits::length(s)}, _M_str{s} {}

  constexpr size_type find(const C *s, size_type, size_type n) const noexcept {
    const C *it = _M_str;
    size_type len = _M_len;
    while (len >= n) {
      if (__builtin_memcmp(it, s, n) == 0)
        return it - _M_str;
      len = _M_str - ++it;
    }
    return size_type(-1);
  }

  constexpr size_type find(const C *s) { return find(s, 0, Traits::length(s)); }

  long _M_len;
  const C *_M_str;
};

using string_view = basic_string_view<const char>;

} // namespace std

template <typename> constexpr auto f() {
  return std::string_view{std::source_location::current().function_name()};
}

int foo() { constexpr auto s = f<int>().find("int"); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Pointer.h:382: unsigned int clang::interp::Pointer::getOffset() const: Assertion `Offset != PastEndMark && "invalid offset"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:57:52: current parser token ';'
2.	<source>:57:11: parsing function body 'foo'
3.	<source>:57:11: in compound statement ('{}')
 #0 0x0000000004279728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4279728)
 #1 0x0000000004276b54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4276b54)
 #2 0x00000000041ba888 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dcbe2e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007dcbe2e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007dcbe2e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007dcbe2e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007dcbe2e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007dcbe2e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007e2764d enumerateData(clang::interp::Pointer const&, clang::interp::Context const&, clang::interp::Bits, clang::interp::Bits, llvm::function_ref<bool (clang::interp::Pointer const&, clang::interp::PrimType, clang::interp::Bits, clang::interp::Bits, bool)>) InterpBuiltinBitCast.cpp:0:0
#10 0x0000000007e27777 clang::interp::readPointerToBuffer(clang::interp::Context const&, clang::interp::Pointer const&, clang::interp::BitcastBuffer&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e27777)
#11 0x0000000007e0cefe clang::interp::interp__builtin_memcmp(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::InterpFrame const*, clang::CallExpr const*, unsigned int) (.isra.0) InterpBuiltin.cpp:0:0
#12 0x0000000007e1e186 clang::interp::InterpretBuiltin(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e1e186)
#13 0x0000000007eae5ac clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eae5ac)
#14 0x0000000007ebf5b7 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ebf5b7)
#15 0x0000000007eba925 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eba925)
#16 0x0000000007ebf5b7 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ebf5b7)
#17 0x00000000081559ad clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81559ad)
#18 0x0000000008161b35 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitExprWithCleanups(clang::ExprWithCleanups const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8161b35)
#19 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#20 0x0000000008150b81 clang::interp::Compiler<clang::interp::EvalEmitter>::visitVarDecl(clang::VarDecl const*, clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8150b81)
#21 0x0000000008150f12 clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8150f12)
#22 0x0000000007ce49ad clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ce49ad)
#23 0x0000000007cc2fe6 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cc2fe6)
#24 0x0000000007cabf26 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cabf26)
#25 0x0000000007a947c2 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a947c2)
#26 0x0000000007a94b71 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a94b71)
#27 0x0000000006d52b8b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d52b8b)
#28 0x0000000006d5a916 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d5a916)
#29 0x00000000069ab291 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ab291)
#30 0x00000000069b84ba clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b84ba)
#31 0x00000000069c11cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c11cf)
#32 0x00000000069c169c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c169c)
#33 0x0000000006a7341a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7341a)
#34 0x0000000006a73c76 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73c76)
#35 0x0000000006a7c3fe clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7c3fe)
#36 0x0000000006a7cc4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7cc4a)
#37 0x0000000006982aab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6982aab)
#38 0x00000000069b8ecd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b8ecd)
#39 0x000000000697625e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697625e)
#40 0x00000000069769ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69769ff)
#41 0x000000000697e2b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697e2b3)
#42 0x000000000697f1c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697f1c5)
#43 0x00000000069607aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69607aa)
#44 0x0000000004bc8be8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc8be8)
#45 0x0000000004eb4455 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eb4455)
#46 0x0000000004e3460e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e3460e)
#47 0x0000000004fad48d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fad48d)
#48 0x0000000000dc7780 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7780)
#49 0x0000000000dbe1fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dbe37d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x0000000004c313d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x00000000041bad24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41bad24)
#53 0x0000000004c319ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x0000000004bf21e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf21e2)
#55 0x0000000004bf318e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf318e)
#56 0x0000000004bfa5c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfa5c5)
#57 0x0000000000dc3ba1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3ba1)
#58 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72574)
#59 0x00007dcbe2e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x00007dcbe2e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000dbdc95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdc95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

This is trunk only:
https://godbolt.org/z/dYjPjerbr

---

