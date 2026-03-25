# [ice-on-invalid] crash when compiling in-class initialization of constinit static member

**Author:** ericniebler
**URL:** https://github.com/llvm/llvm-project/issues/151562

## Body

the following invalid code causes clang trunk to ICE (compile with `-std=c++20`):

```c++
#include <atomic>
#include <thread>

// An atomic queue that supports multiple producers and a single consumer.
template <auto _NextPtr>
class  atomic_intrusive_queue;

template <class _Tp, _Tp* _Tp::* _NextPtr>
class alignas(64) atomic_intrusive_queue<_NextPtr>
{
public:
  using node_pointer         = _Tp*;
  using atomic_node_pointer  = std::atomic<_Tp*>;

  auto push(node_pointer node) noexcept
  {
    node_pointer old_head = head_.load();
    do
    {
      node->*_NextPtr = old_head;
    } while (!head_.compare_exchange_weak(old_head, node));

    // There can be only one consumer thread, so we can use notify_one here
    // instead of notify_all:
    head_.notify_one();
  }

  void wait_for_item() noexcept
  {
    head_.wait(nullptr);
  }

  [[nodiscard]]
  auto pop_all() noexcept -> atomic_node_pointer
  {
    return head_.exchange(nullptr);
  }

private:
  atomic_node_pointer head_{nullptr};
};


struct run_loop
{
  void run()
  {

  }

  template <class Fn>
  auto enqueue(Fn fn)
  {
    return op(fn, queue_);
  }
private:
  struct item
  {
    virtual void execute() = 0;
    item* next_{};
  };

  template <class Fn>
  struct op : item
  {
    explicit op(Fn fn, atomic_intrusive_queue<&item::next_>& queue)
      : fn_(fn)
    {
      queue.push(this);
    }

    op(op&&) = delete;

    void execute() override
    {
      fn_();
    }

    Fn fn_;
  };

  struct noop : item
  {
    void execute() override
    {}
  };
  //static constinit noop noop_;
  static constinit noop noop_{};
  atomic_intrusive_queue<&item::next_> queue_;
};
``` 

result:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:70:13: current parser token 'push'
2.	<source>:45:1: parsing struct/union/class body 'run_loop'
3.	<source>:69:5: parsing function body 'run_loop::op::op<Fn>'
4.	<source>:69:5: in compound statement ('{}')
 #0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #2 0x0000000003bf3cb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072c499c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007797f8f clang::Stmt::getBeginLoc() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7797f8f)
 #5 0x0000000006c72aac clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c72aac)
 #6 0x0000000006c753a4 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c753a4)
 #7 0x0000000006d35562 ConvertDeducedTemplateArguments(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, clang::Sema::CheckTemplateArgumentInfo&, clang::LocalInstantiationScope*, unsigned int, bool*) SemaTemplateDeduction.cpp:0:0
 #8 0x0000000006d36d82 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
 #9 0x0000000006d379a2 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#10 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#11 0x0000000006d41e4b _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#12 0x0000000006db1812 getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006df9401 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6df9401)
#14 0x0000000006e745de void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#15 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#16 0x0000000006e78f87 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e78f87)
#17 0x0000000006e78fe5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e78fe5)
#18 0x0000000006e790c0 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e790c0)
#19 0x000000000687807b clang::Sema::ActOnStartCXXMemberReference(clang::Scope*, clang::Expr*, clang::SourceLocation, clang::tok::TokenKind, clang::OpaquePtr<clang::QualType>&, bool&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x687807b)
#20 0x00000000062ea376 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ea376)
#21 0x00000000062e266a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e266a)
#22 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#23 0x00000000062e4af9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e4af9)
#24 0x00000000062e9aa9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9aa9)
#25 0x000000000638cac7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638cac7)
#26 0x0000000006383616 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6383616)
#27 0x0000000006383ff3 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6383ff3)
#28 0x000000000638d985 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638d985)
#29 0x000000000638e3ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638e3ba)
#30 0x000000000627fe3a clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627fe3a)
#31 0x000000000627f7cd clang::Parser::LateParsedClass::ParseLexedMethodDefs() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627f7cd)
#32 0x000000000627fb0d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627fb0d)
#33 0x00000000062d7dc6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d7dc6)
#34 0x00000000062da473 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62da473)
#35 0x00000000062a2b2a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a2b2a)
#36 0x000000000625e7ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625e7ff)
#37 0x000000000625f997 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625f997)
#38 0x000000000626858f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626858f)
#39 0x0000000006269da0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6269da0)
#40 0x000000000625851a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625851a)
#41 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#42 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#43 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#44 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#45 0x0000000000dbabf5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#46 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#47 0x00000000046628f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#49 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#51 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#52 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#53 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#54 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#55 0x000072c499c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x000072c499c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

repro:
https://godbolt.org/z/Pbs9Y7T6o


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Niebler (ericniebler)

<details>
the following invalid code causes clang trunk to ICE (compile with `-std=c++20`):

```c++
#include &lt;atomic&gt;
#include &lt;thread&gt;

// An atomic queue that supports multiple producers and a single consumer.
template &lt;auto _NextPtr&gt;
class  atomic_intrusive_queue;

template &lt;class _Tp, _Tp* _Tp::* _NextPtr&gt;
class alignas(64) atomic_intrusive_queue&lt;_NextPtr&gt;
{
public:
  using node_pointer         = _Tp*;
  using atomic_node_pointer  = std::atomic&lt;_Tp*&gt;;

  auto push(node_pointer node) noexcept
  {
    node_pointer old_head = head_.load();
    do
    {
      node-&gt;*_NextPtr = old_head;
    } while (!head_.compare_exchange_weak(old_head, node));

    // There can be only one consumer thread, so we can use notify_one here
    // instead of notify_all:
    head_.notify_one();
  }

  void wait_for_item() noexcept
  {
    head_.wait(nullptr);
  }

  [[nodiscard]]
  auto pop_all() noexcept -&gt; atomic_node_pointer
  {
    return head_.exchange(nullptr);
  }

private:
  atomic_node_pointer head_{nullptr};
};


struct run_loop
{
  void run()
  {

  }

  template &lt;class Fn&gt;
  auto enqueue(Fn fn)
  {
    return op(fn, queue_);
  }
private:
  struct item
  {
    virtual void execute() = 0;
    item* next_{};
  };

  template &lt;class Fn&gt;
  struct op : item
  {
    explicit op(Fn fn, atomic_intrusive_queue&lt;&amp;item::next_&gt;&amp; queue)
      : fn_(fn)
    {
      queue.push(this);
    }

    op(op&amp;&amp;) = delete;

    void execute() override
    {
      fn_();
    }

    Fn fn_;
  };

  struct noop : item
  {
    void execute() override
    {}
  };
  //static constinit noop noop_;
  static constinit noop noop_{};
  atomic_intrusive_queue&lt;&amp;item::next_&gt; queue_;
};
``` 

result:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:70:13: current parser token 'push'
2.	&lt;source&gt;:45:1: parsing struct/union/class body 'run_loop'
3.	&lt;source&gt;:69:5: parsing function body 'run_loop::op::op&lt;Fn&gt;'
4.	&lt;source&gt;:69:5: in compound statement ('{}')
 #<!-- -->0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #<!-- -->1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #<!-- -->2 0x0000000003bf3cb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072c499c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007797f8f clang::Stmt::getBeginLoc() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7797f8f)
 #<!-- -->5 0x0000000006c72aac clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c72aac)
 #<!-- -->6 0x0000000006c753a4 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c753a4)
 #<!-- -->7 0x0000000006d35562 ConvertDeducedTemplateArguments(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::LocalInstantiationScope*, unsigned int, bool*) SemaTemplateDeduction.cpp:0:0
 #<!-- -->8 0x0000000006d36d82 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) SemaTemplateDeduction.cpp:0:0
 #<!-- -->9 0x0000000006d379a2 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#<!-- -->10 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#<!-- -->11 0x0000000006d41e4b _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x0000000006db1812 getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000006df9401 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6df9401)
#<!-- -->14 0x0000000006e745de void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->15 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#<!-- -->16 0x0000000006e78f87 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e78f87)
#<!-- -->17 0x0000000006e78fe5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e78fe5)
#<!-- -->18 0x0000000006e790c0 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e790c0)
#<!-- -->19 0x000000000687807b clang::Sema::ActOnStartCXXMemberReference(clang::Scope*, clang::Expr*, clang::SourceLocation, clang::tok::TokenKind, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, bool&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x687807b)
#<!-- -->20 0x00000000062ea376 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ea376)
#<!-- -->21 0x00000000062e266a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e266a)
#<!-- -->22 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#<!-- -->23 0x00000000062e4af9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e4af9)
#<!-- -->24 0x00000000062e9aa9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9aa9)
#<!-- -->25 0x000000000638cac7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638cac7)
#<!-- -->26 0x0000000006383616 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6383616)
#<!-- -->27 0x0000000006383ff3 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6383ff3)
#<!-- -->28 0x000000000638d985 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638d985)
#<!-- -->29 0x000000000638e3ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x638e3ba)
#<!-- -->30 0x000000000627fe3a clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627fe3a)
#<!-- -->31 0x000000000627f7cd clang::Parser::LateParsedClass::ParseLexedMethodDefs() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627f7cd)
#<!-- -->32 0x000000000627fb0d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627fb0d)
#<!-- -->33 0x00000000062d7dc6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d7dc6)
#<!-- -->34 0x00000000062da473 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62da473)
#<!-- -->35 0x00000000062a2b2a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a2b2a)
#<!-- -->36 0x000000000625e7ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625e7ff)
#<!-- -->37 0x000000000625f997 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625f997)
#<!-- -->38 0x000000000626858f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626858f)
#<!-- -->39 0x0000000006269da0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6269da0)
#<!-- -->40 0x000000000625851a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625851a)
#<!-- -->41 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#<!-- -->42 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#<!-- -->43 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#<!-- -->44 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#<!-- -->45 0x0000000000dbabf5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#<!-- -->46 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->47 0x00000000046628f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->48 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#<!-- -->49 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->50 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#<!-- -->51 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#<!-- -->52 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#<!-- -->53 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#<!-- -->54 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#<!-- -->55 0x000072c499c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->56 0x000072c499c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->57 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

repro:
https://godbolt.org/z/Pbs9Y7T6o

</details>


---

### Comment 2 - shafik

Goes back to clang-10: https://godbolt.org/z/ocsoW8Mco

---

