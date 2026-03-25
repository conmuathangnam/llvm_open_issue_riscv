# [clang] crash on the simplest code.

**Author:** yaoxinliu
**URL:** https://github.com/llvm/llvm-project/issues/141693
**Status:** Closed
**Labels:** clang:frontend, crash, regression:16
**Closed Date:** 2026-01-22T15:43:10Z

## Body

```c++
#include <concepts>

template<typename T>
concept IsOK = []<typename U> consteval -> bool {    
    return requires { requires std::same_as<typename U::InnerType, void>; };
}.template operator()<T>();

static_assert(!IsOK<int>);

int main() {}
```

**The above code is simple and legal. However, clangd will crash when parsing it.**

This is a regression back to clang-16. clang-15 is ok, see [https://godbolt.org/z/e5jof54eo](https://godbolt.org/z/e5jof54eo)

Related issue: [https://github.com/llvm/llvm-project/issues/141732](https://github.com/llvm/llvm-project/issues/141732)



## Comments

### Comment 1 - tbaederr

This also happens with clang: https://godbolt.org/z/YhYhcMv4Y

```
 #0 0x0000000003c7e538 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c7e538)
 #1 0x0000000003c7c67c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c7c67c)
 #2 0x0000000003bcce48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000752e6b042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000074c610c clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #5 0x000000000748b83e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #6 0x00000000074a419a EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
 #7 0x00000000074ac694 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74ac694)
 #8 0x000000000654c6a9 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #9 0x000000000654de99 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#10 0x000000000654e59e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x654e59e)
#11 0x0000000006e3cbbb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006e0b50e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006e0d0d5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006e0d2c7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006e46255 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006e0a846 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000006e0b9b8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006e32d85 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006e0af8f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006e14e84 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000006e0b05a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000006e173c5 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e173c5)
#23 0x000000000654be66 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#24 0x000000000654de99 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#25 0x000000000654e3c7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x654e3c7)
#26 0x0000000006ccf7d0 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
#27 0x0000000006ce22a6 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ce22a6)
#28 0x0000000006799d55 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6799d55)
```

CC @cor3ntin 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (yaoxinliu)

<details>
```
#include &lt;concepts&gt;

template&lt;typename T&gt;
concept IsOK = []&lt;typename U&gt; consteval -&gt; bool {    
    return requires { requires std::same_as&lt;typename U::InnerType, void&gt;; };
}.template operator()&lt;T&gt;();

static_assert(!IsOK&lt;int&gt;);

int main() {}
```

**The above code is simple and legal. However, clangd will crash when parsing it.**

clangd version:

&gt; Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114)
&gt; Features: linux+grpc
&gt; Platform: x86_64-pc-linux-gnu

Attached is the debug info:

```
I[07:57:39.277] &lt;-- textDocument/semanticTokens/full(3)
V[07:57:39.282] Build dynamic index for header symbols with estimated memory usage of 1275412 bytes
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/data/repo/cmn/main.cpp:28:25: current parser token ')'
V[07:57:39.340] &lt;&lt;&lt; {
  "id": 4,
  "jsonrpc": "2.0",
  "method": "textDocument/foldingRange",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:39.340] &lt;-- textDocument/foldingRange(4)
I[07:57:39.341] --&gt; reply:textDocument/foldingRange(4) 0 ms
V[07:57:39.341] &gt;&gt;&gt; {
  "id": 4,
  "jsonrpc": "2.0",
  "result": [
    {
      "endLine": 24,
      "kind": "region",
      "startCharacter": 49,
      "startLine": 23
    },
    {
      "endLine": 30,
      "kind": "region",
      "startCharacter": 12,
      "startLine": 29
    },
    {
      "endCharacter": 20,
      "endLine": 4,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 2
    },
    {
      "endCharacter": 4,
      "endLine": 10,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 8
    },
    {
      "endCharacter": 4,
      "endLine": 14,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 12
    },
    {
      "endCharacter": 4,
      "endLine": 18,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 16
    }
  ]
}

I[07:57:42.200] Indexed c++23 standard library: 14259 symbols, 1797 filtered
V[07:57:42.241] Build dynamic index for header symbols with estimated memory usage of 10554604 bytes
 #<!-- -->0 0x00007feb8fd6d37f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x103337f)
 #<!-- -->1 0x00007feb8fd6b089 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1031089)
 #<!-- -->2 0x00007feb8fd6da90 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1033a90)
 #<!-- -->3 0x00007feb8de8e330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007feb987ffafd (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1133afd)
 #<!-- -->5 0x00007feb987a8e33 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10dce33)
 #<!-- -->6 0x00007feb987a4ad3 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d8ad3)
 #<!-- -->7 0x00007feb987a454b clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d854b)
 #<!-- -->8 0x00007feb98ec67d6 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa7d6)
 #<!-- -->9 0x00007feb98ebf8b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->10 0x00007feb994b0322 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de4322)
#<!-- -->11 0x00007feb994a326b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1dd726b)
#<!-- -->12 0x00007feb994c0378 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1df4378)
#<!-- -->13 0x00007feb994ae414 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de2414)
#<!-- -->14 0x00007feb994b3d09 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de7d09)
#<!-- -->15 0x00007feb994a6a24 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddaa24)
#<!-- -->16 0x00007feb994ab58b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddf58b)
#<!-- -->17 0x00007feb994a8ae4 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddcae4)
#<!-- -->18 0x00007feb99472f47 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1da6f47)
#<!-- -->19 0x00007feb98ec786d (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fb86d)
#<!-- -->20 0x00007feb98ec6743 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa743)
#<!-- -->21 0x00007feb98ebf8b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->22 0x00007feb98ebf60e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f360e)
#<!-- -->23 0x00007feb99383589 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7589)
#<!-- -->24 0x00007feb99383a34 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7a34)
#<!-- -->25 0x00007feb98fffc62 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1933c62)
#<!-- -->26 0x00007feb98491547 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc5547)
#<!-- -->27 0x00007feb9849275d clang::Parser::ParseCXXIdExpression(bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc675d)
#<!-- -->28 0x00007feb98482b32 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6b32)
#<!-- -->29 0x00007feb98482bdc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6bdc)
#<!-- -->30 0x00007feb9848131b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb531b)
#<!-- -->31 0x00007feb984805ec clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb45ec)
#<!-- -->32 0x00007feb9846be8b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd9fe8b)
#<!-- -->33 0x00007feb9844dc91 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd81c91)
#<!-- -->34 0x00007feb984f535e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2935e)
#<!-- -->35 0x00007feb984f4318 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe28318)
#<!-- -->36 0x00007feb9843a76e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd6e76e)
#<!-- -->37 0x00007feb9a39cc42 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cd0c42)
#<!-- -->38 0x000055f9a3bf6fd3 (/usr/lib/llvm-20/bin/clangd+0x3f9fd3)
#<!-- -->39 0x000055f9a3c9a818 (/usr/lib/llvm-20/bin/clangd+0x49d818)
#<!-- -->40 0x000055f9a3c9a260 (/usr/lib/llvm-20/bin/clangd+0x49d260)
#<!-- -->41 0x000055f9a3c96aee (/usr/lib/llvm-20/bin/clangd+0x499aee)
#<!-- -->42 0x000055f9a3c9668b (/usr/lib/llvm-20/bin/clangd+0x49968b)
#<!-- -->43 0x000055f9a3e014e1 (/usr/lib/llvm-20/bin/clangd+0x6044e1)
#<!-- -->44 0x00007feb8dee5aa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->45 0x00007feb8df72c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: main.cpp
  Directory: /data/repo/cmn
  Command Line: /usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
  Version: 1
[Info  - 7:57:44 AM] Connection to server got closed. Server will restart.
[Error - 7:57:44 AM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 7:57:44 AM] Request textDocument/codeAction failed.
[object Object]
[Error - 7:57:44 AM] Request textDocument/semanticTokens/full failed.
[object Object]
I[07:57:44.484] Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114)
I[07:57:44.484] Features: linux+grpc
I[07:57:44.484] PID: 3409294
I[07:57:44.484] Working directory: /data/repo/cmn
I[07:57:44.484] argv[0]: /usr/lib/llvm-20/bin/clangd
I[07:57:44.484] argv[1]: --log=verbose
I[07:57:44.484] argv[2]: --pretty
I[07:57:44.484] argv[3]: --background-index
I[07:57:44.484] argv[4]: --header-insertion=iwyu
I[07:57:44.484] argv[5]: --pch-storage=memory
I[07:57:44.484] argv[6]: --enable-config
I[07:57:44.484] argv[7]: --limit-results=5000
I[07:57:44.484] argv[8]: --query-driver=clang++
I[07:57:44.484] argv[9]: --clang-tidy
I[07:57:44.484] argv[10]: --completion-style=detailed
I[07:57:44.484] argv[11]: -j=12
I[07:57:44.484] argv[12]: --all-scopes-completion
V[07:57:44.484] User config file is /home/liuyaoxin/.config/clangd/config.yaml
I[07:57:44.485] Starting LSP over stdin/stdout
V[07:57:44.485] &lt;&lt;&lt; {
  "id": 0,
  "jsonrpc": "2.0",
  "method": "initialize",
  "params": {
    "capabilities": {
      "general": {
        "markdown": {
          "parser": "marked",
          "version": "1.1.0"
        },
        "positionEncodings": [
          "utf-16"
        ],
        "regularExpressions": {
          "engine": "ECMAScript",
          "version": "ES2020"
        },
        "staleRequestSupport": {
          "cancel": true,
          "retryOnContentModified": [
            "textDocument/semanticTokens/full",
            "textDocument/semanticTokens/range",
            "textDocument/semanticTokens/full/delta"
          ]
        }
      },
      "notebookDocument": {
        "synchronization": {
          "dynamicRegistration": true,
          "executionSummarySupport": true
        }
      },
      "textDocument": {
        "callHierarchy": {
          "dynamicRegistration": true
        },
        "codeAction": {
          "codeActionLiteralSupport": {
            "codeActionKind": {
              "valueSet": [
                "",
                "quickfix",
                "refactor",
                "refactor.extract",
                "refactor.inline",
                "refactor.rewrite",
                "source",
                "source.organizeImports"
              ]
            }
          },
          "dataSupport": true,
          "disabledSupport": true,
          "dynamicRegistration": true,
          "honorsChangeAnnotations": false,
          "isPreferredSupport": true,
          "resolveSupport": {
            "properties": [
              "edit"
            ]
          }
        },
        "codeLens": {
          "dynamicRegistration": true
        },
        "colorProvider": {
          "dynamicRegistration": true
        },
        "completion": {
          "completionItem": {
            "commitCharactersSupport": true,
            "deprecatedSupport": true,
            "documentationFormat": [
              "markdown",
              "plaintext"
            ],
            "insertReplaceSupport": true,
            "insertTextModeSupport": {
              "valueSet": [
                1,
                2
              ]
            },
            "labelDetailsSupport": true,
            "preselectSupport": true,
            "resolveSupport": {
              "properties": [
                "documentation",
                "detail",
                "additionalTextEdits"
              ]
            },
            "snippetSupport": true,
            "tagSupport": {
              "valueSet": [
                1
              ]
            }
          },
          "completionItemKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25
            ]
          },
          "completionList": {
            "itemDefaults": [
              "commitCharacters",
              "editRange",
              "insertTextFormat",
              "insertTextMode"
            ]
          },
          "contextSupport": true,
          "dynamicRegistration": true,
          "editsNearCursor": true,
          "insertTextMode": 2
        },
        "declaration": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "definition": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "diagnostic": {
          "dynamicRegistration": true,
          "relatedDocumentSupport": false
        },
        "documentHighlight": {
          "dynamicRegistration": true
        },
        "documentLink": {
          "dynamicRegistration": true,
          "tooltipSupport": true
        },
        "documentSymbol": {
          "dynamicRegistration": true,
          "hierarchicalDocumentSymbolSupport": true,
          "labelSupport": true,
          "symbolKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25,
              26
            ]
          },
          "tagSupport": {
            "valueSet": [
              1
            ]
          }
        },
        "foldingRange": {
          "dynamicRegistration": true,
          "foldingRange": {
            "collapsedText": false
          },
          "foldingRangeKind": {
            "valueSet": [
              "comment",
              "imports",
              "region"
            ]
          },
          "lineFoldingOnly": true,
          "rangeLimit": 5000
        },
        "formatting": {
          "dynamicRegistration": true
        },
        "hover": {
          "contentFormat": [
            "markdown",
            "plaintext"
          ],
          "dynamicRegistration": true
        },
        "implementation": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "inactiveRegionsCapabilities": {
          "inactiveRegions": true
        },
        "inlayHint": {
          "dynamicRegistration": true,
          "resolveSupport": {
            "properties": [
              "tooltip",
              "textEdits",
              "label.tooltip",
              "label.location",
              "label.command"
            ]
          }
        },
        "inlineValue": {
          "dynamicRegistration": true
        },
        "linkedEditingRange": {
          "dynamicRegistration": true
        },
        "onTypeFormatting": {
          "dynamicRegistration": true
        },
        "publishDiagnostics": {
          "codeDescriptionSupport": true,
          "dataSupport": true,
          "relatedInformation": true,
          "tagSupport": {
            "valueSet": [
              1,
              2
            ]
          },
          "versionSupport": false
        },
        "rangeFormatting": {
          "dynamicRegistration": true
        },
        "references": {
          "dynamicRegistration": true
        },
        "rename": {
          "dynamicRegistration": true,
          "honorsChangeAnnotations": true,
          "prepareSupport": true,
          "prepareSupportDefaultBehavior": 1
        },
        "selectionRange": {
          "dynamicRegistration": true
        },
        "semanticTokens": {
          "augmentsSyntaxTokens": true,
          "dynamicRegistration": true,
          "formats": [
            "relative"
          ],
          "multilineTokenSupport": false,
          "overlappingTokenSupport": false,
          "requests": {
            "full": {
              "delta": true
            },
            "range": true
          },
          "serverCancelSupport": true,
          "tokenModifiers": [
            "declaration",
            "definition",
            "readonly",
            "static",
            "deprecated",
            "abstract",
            "async",
            "modification",
            "documentation",
            "defaultLibrary"
          ],
          "tokenTypes": [
            "namespace",
            "type",
            "class",
            "enum",
            "interface",
            "struct",
            "typeParameter",
            "parameter",
            "variable",
            "property",
            "enumMember",
            "event",
            "function",
            "method",
            "macro",
            "keyword",
            "modifier",
            "comment",
            "string",
            "number",
            "regexp",
            "operator",
            "decorator"
          ]
        },
        "signatureHelp": {
          "contextSupport": true,
          "dynamicRegistration": true,
          "signatureInformation": {
            "activeParameterSupport": true,
            "documentationFormat": [
              "markdown",
              "plaintext"
            ],
            "parameterInformation": {
              "labelOffsetSupport": true
            }
          }
        },
        "synchronization": {
          "didSave": true,
          "dynamicRegistration": true,
          "willSave": true,
          "willSaveWaitUntil": true
        },
        "typeDefinition": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "typeHierarchy": {
          "dynamicRegistration": true
        }
      },
      "window": {
        "showDocument": {
          "support": true
        },
        "showMessage": {
          "messageActionItem": {
            "additionalPropertiesSupport": true
          }
        },
        "workDoneProgress": true
      },
      "workspace": {
        "applyEdit": true,
        "codeLens": {
          "refreshSupport": true
        },
        "configuration": true,
        "diagnostics": {
          "refreshSupport": true
        },
        "didChangeConfiguration": {
          "dynamicRegistration": true
        },
        "didChangeWatchedFiles": {
          "dynamicRegistration": true,
          "relativePatternSupport": true
        },
        "executeCommand": {
          "dynamicRegistration": true
        },
        "fileOperations": {
          "didCreate": true,
          "didDelete": true,
          "didRename": true,
          "dynamicRegistration": true,
          "willCreate": true,
          "willDelete": true,
          "willRename": true
        },
        "inlayHint": {
          "refreshSupport": true
        },
        "inlineValue": {
          "refreshSupport": true
        },
        "semanticTokens": {
          "refreshSupport": true
        },
        "symbol": {
          "dynamicRegistration": true,
          "resolveSupport": {
            "properties": [
              "location.range"
            ]
          },
          "symbolKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25,
              26
            ]
          },
          "tagSupport": {
            "valueSet": [
              1
            ]
          }
        },
        "workspaceEdit": {
          "changeAnnotationSupport": {
            "groupsOnLabel": true
          },
          "documentChanges": true,
          "failureHandling": "textOnlyTransactional",
          "normalizesLineEndings": true,
          "resourceOperations": [
            "create",
            "rename",
            "delete"
          ]
        },
        "workspaceFolders": true
      }
    },
    "clientInfo": {
      "name": "Visual Studio Code",
      "version": "1.100.2"
    },
    "initializationOptions": {
      "clangdFileStatus": true,
      "fallbackFlags": []
    },
    "locale": "en",
    "processId": 3409019,
    "rootPath": "/data/repo/cmn",
    "rootUri": "file:///data/repo/cmn",
    "trace": "off",
    "workspaceFolders": [
      {
        "name": "cmn",
        "uri": "file:///data/repo/cmn"
      }
    ]
  }
}

I[07:57:44.485] &lt;-- initialize(0)
I[07:57:44.516] --&gt; reply:initialize(0) 30 ms
V[07:57:44.516] &gt;&gt;&gt; {
  "id": 0,
  "jsonrpc": "2.0",
  "result": {
    "capabilities": {
      "astProvider": true,
      "callHierarchyProvider": true,
      "clangdInlayHintsProvider": true,
      "codeActionProvider": {
        "codeActionKinds": [
          "quickfix",
          "refactor",
          "info"
        ]
      },
      "compilationDatabase": {
        "automaticReload": true
      },
      "completionProvider": {
        "resolveProvider": false,
        "triggerCharacters": [
          ".",
          "&lt;",
          "&gt;",
          ":",
          "\"",
          "/",
          "*"
        ]
      },
      "declarationProvider": true,
      "definitionProvider": true,
      "documentFormattingProvider": true,
      "documentHighlightProvider": true,
      "documentLinkProvider": {
        "resolveProvider": false
      },
      "documentOnTypeFormattingProvider": {
        "firstTriggerCharacter": "\n",
        "moreTriggerCharacter": []
      },
      "documentRangeFormattingProvider": true,
      "documentSymbolProvider": true,
      "executeCommandProvider": {
        "commands": [
          "clangd.applyFix",
          "clangd.applyRename",
          "clangd.applyTweak"
        ]
      },
      "foldingRangeProvider": true,
      "hoverProvider": true,
      "implementationProvider": true,
      "inactiveRegionsProvider": true,
      "inlayHintProvider": true,
      "memoryUsageProvider": true,
      "referencesProvider": true,
      "renameProvider": {
        "prepareProvider": true
      },
      "selectionRangeProvider": true,
      "semanticTokensProvider": {
        "full": {
          "delta": true
        },
        "legend": {
          "tokenModifiers": [
            "declaration",
            "definition",
            "deprecated",
            "deduced",
            "readonly",
            "static",
            "abstract",
            "virtual",
            "dependentName",
            "defaultLibrary",
            "usedAsMutableReference",
            "usedAsMutablePointer",
            "constructorOrDestructor",
            "userDefined",
            "functionScope",
            "classScope",
            "fileScope",
            "globalScope"
          ],
          "tokenTypes": [
            "variable",
            "variable",
            "parameter",
            "function",
            "method",
            "function",
            "property",
            "variable",
            "class",
            "interface",
            "enum",
            "enumMember",
            "type",
            "type",
            "unknown",
            "namespace",
            "typeParameter",
            "concept",
            "type",
            "macro",
            "modifier",
            "operator",
            "bracket",
            "label",
            "comment"
          ]
        },
        "range": false
      },
      "signatureHelpProvider": {
        "triggerCharacters": [
          "(",
          ")",
          "{",
          "}",
          "&lt;",
          "&gt;",
          ","
        ]
      },
      "standardTypeHierarchyProvider": true,
      "textDocumentSync": {
        "change": 2,
        "openClose": true,
        "save": true
      },
      "typeDefinitionProvider": true,
      "typeHierarchyProvider": true,
      "workspaceSymbolProvider": true
    },
    "serverInfo": {
      "name": "clangd",
      "version": "Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114) linux+grpc x86_64-pc-linux-gnu"
    }
  }
}

V[07:57:44.518] &lt;&lt;&lt; {
  "jsonrpc": "2.0",
  "method": "initialized",
  "params": {}
}

I[07:57:44.518] &lt;-- initialized
V[07:57:44.521] &lt;&lt;&lt; {
  "jsonrpc": "2.0",
  "method": "textDocument/didOpen",
  "params": {
    "textDocument": {
      "languageId": "cpp",
      "text": "// #include &lt;cmn/tester.hpp&gt;\n\n// #include &lt;chrono&gt;\n// #include &lt;stdexcept&gt;\n// #include &lt;thread&gt;\n\n// #include &lt;cmn/test/test_all.hpp&gt;\n\n// TEST_CASE (test1) {\n//     std::this_thread::sleep_for(2s);\n// }\n\n// TEST_CASE (test2) {\n//     std::this_thread::sleep_for(3s);\n// }\n\n// TEST_CASE (test3) {\n//     std::this_thread::sleep_for(1s);\n// }\n\n#include &lt;concepts&gt;\n\ntemplate&lt;typename T&gt;\nconcept IsOK = []&lt;typename U&gt; consteval -&gt; bool {    \n    return requires { requires std::same_as&lt;typename U::InnerType, void&gt;; };\n}.template operator()&lt;T&gt;();\n\nstatic_assert(!IsOK&lt;int&gt;);\n\nint main() {\n    // cmn::run_test_cases();\n}\n",
      "uri": "file:///data/repo/cmn/main.cpp",
      "version": 1
    }
  }
}

I[07:57:44.521] &lt;-- textDocument/didOpen
V[07:57:44.552] System include extraction: driver clang expanded to /usr/lib/llvm-20/bin/clang
V[07:57:44.552] System include extraction: not allowed driver /usr/lib/llvm-20/bin/clang
V[07:57:44.552] &lt;&lt;&lt; {
  "id": 1,
  "jsonrpc": "2.0",
  "method": "textDocument/documentSymbol",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.552] &lt;-- textDocument/documentSymbol(1)
V[07:57:44.552] &lt;&lt;&lt; {
  "id": 2,
  "jsonrpc": "2.0",
  "method": "textDocument/codeAction",
  "params": {
    "context": {
      "diagnostics": [],
      "triggerKind": 2
    },
    "range": {
      "end": {
        "character": 49,
        "line": 23
      },
      "start": {
        "character": 49,
        "line": 23
      }
    },
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.552] &lt;-- textDocument/codeAction(2)
V[07:57:44.553] config note at /data/repo/cmn/.clangd:4:0: Parsing config fragment
V[07:57:44.553] config note at /data/repo/cmn/.clangd:1:0: Parsed 1 fragments from file
V[07:57:44.553] Config fragment: compiling /data/repo/cmn/.clangd:4 -&gt; 0x00007F3790002C60 (trusted=false)
I[07:57:44.553] config warning at :-1:-1: Invalid MissingIncludes value 'true'. Valid values are Strict, None.
I[07:57:44.553] --&gt; textDocument/publishDiagnostics
V[07:57:44.553] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/publishDiagnostics",
  "params": {
    "diagnostics": [],
    "uri": "file:///data/repo/cmn/.clangd"
  }
}

I[07:57:44.554] Failed to find compilation database for /data/repo/cmn/main.cpp
V[07:57:44.580] System include extraction: driver clang expanded to /usr/lib/llvm-20/bin/clang
V[07:57:44.580] System include extraction: not allowed driver /usr/lib/llvm-20/bin/clang
I[07:57:44.580] ASTWorker building file /data/repo/cmn/main.cpp version 1 with command clangd fallback
[/data/repo/cmn]
/usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
V[07:57:44.581] Driver produced command: cc1 -cc1 -triple x86_64-pc-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -target-feature +avx2 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/data/repo/cmn -fcoverage-compilation-dir=/data/repo/cmn -resource-dir /usr/lib/llvm-20/lib/clang/20 -isystem /usr/include/c++/v1 -isystem /usr/include/c++/v1 -isystem /usr/include/x86_64-linux-gnu -isystem /usr/include -isystem . -D _FILE_OFFSET_BITS=64 -D _TIME_BITS=64 -D IS_DEBUGGING=true -internal-isystem /usr/lib/llvm-20/bin/../include/c++/v1 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -std=c++2c -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ /data/repo/cmn/main.cpp
I[07:57:44.581] --&gt; textDocument/clangd.fileStatus
V[07:57:44.581] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/clangd.fileStatus",
  "params": {
    "state": "parsing includes, running Update",
    "uri": "file:///data/repo/cmn/main.cpp"
  }
}

V[07:57:44.581] Building first preamble for /data/repo/cmn/main.cpp version 1
I[07:57:44.630] Built preamble of size 636760 for file /data/repo/cmn/main.cpp version 1 in 0.05 seconds
I[07:57:44.631] --&gt; workspace/semanticTokens/refresh(0)
I[07:57:44.631] Indexing c++23 standard library in the context of /data/repo/cmn/main.cpp
V[07:57:44.631] &gt;&gt;&gt; {
  "id": 0,
  "jsonrpc": "2.0",
  "method": "workspace/semanticTokens/refresh",
  "params": null
}

I[07:57:44.631] --&gt; textDocument/clangd.fileStatus
V[07:57:44.631] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/clangd.fileStatus",
  "params": {
    "state": "parsing includes, running Build AST",
    "uri": "file:///data/repo/cmn/main.cpp"
  }
}

V[07:57:44.631] &lt;&lt;&lt; {
  "id": 0,
  "jsonrpc": "2.0",
  "result": null
}

I[07:57:44.631] &lt;-- reply(0)
V[07:57:44.636] &lt;&lt;&lt; {
  "id": 3,
  "jsonrpc": "2.0",
  "method": "textDocument/semanticTokens/full",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.636] &lt;-- textDocument/semanticTokens/full(3)
V[07:57:44.639] Ignored diagnostic. invalid configuration value 'lower_space' for option 'readability-identifier-naming.NamespaceCase' [clang-tidy-config]
V[07:57:44.641] indexed preamble AST for /data/repo/cmn/main.cpp version 1:
  symbol slab: 739 symbols, 204336 bytes
  ref slab: 0 symbols, 0 refs, 128 bytes
  relations slab: 58 relations, 1112 bytes
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/data/repo/cmn/main.cpp:28:25: current parser token ')'
V[07:57:44.647] Build dynamic index for header symbols with estimated memory usage of 1275412 bytes
V[07:57:44.725] &lt;&lt;&lt; {
  "id": 4,
  "jsonrpc": "2.0",
  "method": "textDocument/foldingRange",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.725] &lt;-- textDocument/foldingRange(4)
I[07:57:44.725] --&gt; reply:textDocument/foldingRange(4) 0 ms
V[07:57:44.725] &gt;&gt;&gt; {
  "id": 4,
  "jsonrpc": "2.0",
  "result": [
    {
      "endLine": 24,
      "kind": "region",
      "startCharacter": 49,
      "startLine": 23
    },
    {
      "endLine": 30,
      "kind": "region",
      "startCharacter": 12,
      "startLine": 29
    },
    {
      "endCharacter": 20,
      "endLine": 4,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 2
    },
    {
      "endCharacter": 4,
      "endLine": 10,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 8
    },
    {
      "endCharacter": 4,
      "endLine": 14,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 12
    },
    {
      "endCharacter": 4,
      "endLine": 18,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 16
    }
  ]
}

I[07:57:47.622] Indexed c++23 standard library: 14259 symbols, 1797 filtered
V[07:57:47.664] Build dynamic index for header symbols with estimated memory usage of 10554860 bytes
 #<!-- -->0 0x00007f37ce32137f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x103337f)
 #<!-- -->1 0x00007f37ce31f089 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1031089)
 #<!-- -->2 0x00007f37ce321a90 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1033a90)
 #<!-- -->3 0x00007f37cc442330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f37d6db3afd (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1133afd)
 #<!-- -->5 0x00007f37d6d5ce33 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10dce33)
 #<!-- -->6 0x00007f37d6d58ad3 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d8ad3)
 #<!-- -->7 0x00007f37d6d5854b clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d854b)
 #<!-- -->8 0x00007f37d747a7d6 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa7d6)
 #<!-- -->9 0x00007f37d74738b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->10 0x00007f37d7a64322 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de4322)
#<!-- -->11 0x00007f37d7a5726b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1dd726b)
#<!-- -->12 0x00007f37d7a74378 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1df4378)
#<!-- -->13 0x00007f37d7a62414 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de2414)
#<!-- -->14 0x00007f37d7a67d09 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de7d09)
#<!-- -->15 0x00007f37d7a5aa24 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddaa24)
#<!-- -->16 0x00007f37d7a5f58b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddf58b)
#<!-- -->17 0x00007f37d7a5cae4 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddcae4)
#<!-- -->18 0x00007f37d7a26f47 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1da6f47)
#<!-- -->19 0x00007f37d747b86d (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fb86d)
#<!-- -->20 0x00007f37d747a743 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa743)
#<!-- -->21 0x00007f37d74738b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->22 0x00007f37d747360e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f360e)
#<!-- -->23 0x00007f37d7937589 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7589)
#<!-- -->24 0x00007f37d7937a34 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7a34)
#<!-- -->25 0x00007f37d75b3c62 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1933c62)
#<!-- -->26 0x00007f37d6a45547 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc5547)
#<!-- -->27 0x00007f37d6a4675d clang::Parser::ParseCXXIdExpression(bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc675d)
#<!-- -->28 0x00007f37d6a36b32 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6b32)
#<!-- -->29 0x00007f37d6a36bdc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6bdc)
#<!-- -->30 0x00007f37d6a3531b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb531b)
#<!-- -->31 0x00007f37d6a345ec clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb45ec)
#<!-- -->32 0x00007f37d6a1fe8b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd9fe8b)
#<!-- -->33 0x00007f37d6a01c91 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd81c91)
#<!-- -->34 0x00007f37d6aa935e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2935e)
#<!-- -->35 0x00007f37d6aa8318 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe28318)
#<!-- -->36 0x00007f37d69ee76e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd6e76e)
#<!-- -->37 0x00007f37d8950c42 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cd0c42)
#<!-- -->38 0x0000563bd4ecefd3 (/usr/lib/llvm-20/bin/clangd+0x3f9fd3)
#<!-- -->39 0x0000563bd4f72818 (/usr/lib/llvm-20/bin/clangd+0x49d818)
#<!-- -->40 0x0000563bd4f72260 (/usr/lib/llvm-20/bin/clangd+0x49d260)
#<!-- -->41 0x0000563bd4f6eaee (/usr/lib/llvm-20/bin/clangd+0x499aee)
#<!-- -->42 0x0000563bd4f6e68b (/usr/lib/llvm-20/bin/clangd+0x49968b)
#<!-- -->43 0x0000563bd50d94e1 (/usr/lib/llvm-20/bin/clangd+0x6044e1)
#<!-- -->44 0x00007f37cc499aa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->45 0x00007f37cc526c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: main.cpp
  Directory: /data/repo/cmn
  Command Line: /usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
  Version: 1
[Error - 7:57:49 AM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 7:57:49 AM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 7:57:49 AM] Request textDocument/codeAction failed.
[object Object]
[Error - 7:57:49 AM] Request textDocument/semanticTokens/full failed.
[object Object]
```

</details>


---

### Comment 3 - kadircet

oops, nvm missed https://github.com/llvm/llvm-project/issues/141693#issuecomment-2914882043.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (yaoxinliu)

<details>
```
#include &lt;concepts&gt;

template&lt;typename T&gt;
concept IsOK = []&lt;typename U&gt; consteval -&gt; bool {    
    return requires { requires std::same_as&lt;typename U::InnerType, void&gt;; };
}.template operator()&lt;T&gt;();

static_assert(!IsOK&lt;int&gt;);

int main() {}
```

**The above code is simple and legal. However, clangd will crash when parsing it.**

clangd version:

&gt; Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114)
&gt; Features: linux+grpc
&gt; Platform: x86_64-pc-linux-gnu

Attached is the debug info:

```
I[07:57:39.277] &lt;-- textDocument/semanticTokens/full(3)
V[07:57:39.282] Build dynamic index for header symbols with estimated memory usage of 1275412 bytes
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/data/repo/cmn/main.cpp:28:25: current parser token ')'
V[07:57:39.340] &lt;&lt;&lt; {
  "id": 4,
  "jsonrpc": "2.0",
  "method": "textDocument/foldingRange",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:39.340] &lt;-- textDocument/foldingRange(4)
I[07:57:39.341] --&gt; reply:textDocument/foldingRange(4) 0 ms
V[07:57:39.341] &gt;&gt;&gt; {
  "id": 4,
  "jsonrpc": "2.0",
  "result": [
    {
      "endLine": 24,
      "kind": "region",
      "startCharacter": 49,
      "startLine": 23
    },
    {
      "endLine": 30,
      "kind": "region",
      "startCharacter": 12,
      "startLine": 29
    },
    {
      "endCharacter": 20,
      "endLine": 4,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 2
    },
    {
      "endCharacter": 4,
      "endLine": 10,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 8
    },
    {
      "endCharacter": 4,
      "endLine": 14,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 12
    },
    {
      "endCharacter": 4,
      "endLine": 18,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 16
    }
  ]
}

I[07:57:42.200] Indexed c++23 standard library: 14259 symbols, 1797 filtered
V[07:57:42.241] Build dynamic index for header symbols with estimated memory usage of 10554604 bytes
 #<!-- -->0 0x00007feb8fd6d37f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x103337f)
 #<!-- -->1 0x00007feb8fd6b089 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1031089)
 #<!-- -->2 0x00007feb8fd6da90 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1033a90)
 #<!-- -->3 0x00007feb8de8e330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007feb987ffafd (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1133afd)
 #<!-- -->5 0x00007feb987a8e33 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10dce33)
 #<!-- -->6 0x00007feb987a4ad3 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d8ad3)
 #<!-- -->7 0x00007feb987a454b clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d854b)
 #<!-- -->8 0x00007feb98ec67d6 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa7d6)
 #<!-- -->9 0x00007feb98ebf8b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->10 0x00007feb994b0322 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de4322)
#<!-- -->11 0x00007feb994a326b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1dd726b)
#<!-- -->12 0x00007feb994c0378 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1df4378)
#<!-- -->13 0x00007feb994ae414 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de2414)
#<!-- -->14 0x00007feb994b3d09 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de7d09)
#<!-- -->15 0x00007feb994a6a24 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddaa24)
#<!-- -->16 0x00007feb994ab58b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddf58b)
#<!-- -->17 0x00007feb994a8ae4 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddcae4)
#<!-- -->18 0x00007feb99472f47 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1da6f47)
#<!-- -->19 0x00007feb98ec786d (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fb86d)
#<!-- -->20 0x00007feb98ec6743 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa743)
#<!-- -->21 0x00007feb98ebf8b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->22 0x00007feb98ebf60e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f360e)
#<!-- -->23 0x00007feb99383589 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7589)
#<!-- -->24 0x00007feb99383a34 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7a34)
#<!-- -->25 0x00007feb98fffc62 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1933c62)
#<!-- -->26 0x00007feb98491547 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc5547)
#<!-- -->27 0x00007feb9849275d clang::Parser::ParseCXXIdExpression(bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc675d)
#<!-- -->28 0x00007feb98482b32 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6b32)
#<!-- -->29 0x00007feb98482bdc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6bdc)
#<!-- -->30 0x00007feb9848131b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb531b)
#<!-- -->31 0x00007feb984805ec clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb45ec)
#<!-- -->32 0x00007feb9846be8b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd9fe8b)
#<!-- -->33 0x00007feb9844dc91 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd81c91)
#<!-- -->34 0x00007feb984f535e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2935e)
#<!-- -->35 0x00007feb984f4318 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe28318)
#<!-- -->36 0x00007feb9843a76e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd6e76e)
#<!-- -->37 0x00007feb9a39cc42 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cd0c42)
#<!-- -->38 0x000055f9a3bf6fd3 (/usr/lib/llvm-20/bin/clangd+0x3f9fd3)
#<!-- -->39 0x000055f9a3c9a818 (/usr/lib/llvm-20/bin/clangd+0x49d818)
#<!-- -->40 0x000055f9a3c9a260 (/usr/lib/llvm-20/bin/clangd+0x49d260)
#<!-- -->41 0x000055f9a3c96aee (/usr/lib/llvm-20/bin/clangd+0x499aee)
#<!-- -->42 0x000055f9a3c9668b (/usr/lib/llvm-20/bin/clangd+0x49968b)
#<!-- -->43 0x000055f9a3e014e1 (/usr/lib/llvm-20/bin/clangd+0x6044e1)
#<!-- -->44 0x00007feb8dee5aa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->45 0x00007feb8df72c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: main.cpp
  Directory: /data/repo/cmn
  Command Line: /usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
  Version: 1
[Info  - 7:57:44 AM] Connection to server got closed. Server will restart.
[Error - 7:57:44 AM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 7:57:44 AM] Request textDocument/codeAction failed.
[object Object]
[Error - 7:57:44 AM] Request textDocument/semanticTokens/full failed.
[object Object]
I[07:57:44.484] Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114)
I[07:57:44.484] Features: linux+grpc
I[07:57:44.484] PID: 3409294
I[07:57:44.484] Working directory: /data/repo/cmn
I[07:57:44.484] argv[0]: /usr/lib/llvm-20/bin/clangd
I[07:57:44.484] argv[1]: --log=verbose
I[07:57:44.484] argv[2]: --pretty
I[07:57:44.484] argv[3]: --background-index
I[07:57:44.484] argv[4]: --header-insertion=iwyu
I[07:57:44.484] argv[5]: --pch-storage=memory
I[07:57:44.484] argv[6]: --enable-config
I[07:57:44.484] argv[7]: --limit-results=5000
I[07:57:44.484] argv[8]: --query-driver=clang++
I[07:57:44.484] argv[9]: --clang-tidy
I[07:57:44.484] argv[10]: --completion-style=detailed
I[07:57:44.484] argv[11]: -j=12
I[07:57:44.484] argv[12]: --all-scopes-completion
V[07:57:44.484] User config file is /home/liuyaoxin/.config/clangd/config.yaml
I[07:57:44.485] Starting LSP over stdin/stdout
V[07:57:44.485] &lt;&lt;&lt; {
  "id": 0,
  "jsonrpc": "2.0",
  "method": "initialize",
  "params": {
    "capabilities": {
      "general": {
        "markdown": {
          "parser": "marked",
          "version": "1.1.0"
        },
        "positionEncodings": [
          "utf-16"
        ],
        "regularExpressions": {
          "engine": "ECMAScript",
          "version": "ES2020"
        },
        "staleRequestSupport": {
          "cancel": true,
          "retryOnContentModified": [
            "textDocument/semanticTokens/full",
            "textDocument/semanticTokens/range",
            "textDocument/semanticTokens/full/delta"
          ]
        }
      },
      "notebookDocument": {
        "synchronization": {
          "dynamicRegistration": true,
          "executionSummarySupport": true
        }
      },
      "textDocument": {
        "callHierarchy": {
          "dynamicRegistration": true
        },
        "codeAction": {
          "codeActionLiteralSupport": {
            "codeActionKind": {
              "valueSet": [
                "",
                "quickfix",
                "refactor",
                "refactor.extract",
                "refactor.inline",
                "refactor.rewrite",
                "source",
                "source.organizeImports"
              ]
            }
          },
          "dataSupport": true,
          "disabledSupport": true,
          "dynamicRegistration": true,
          "honorsChangeAnnotations": false,
          "isPreferredSupport": true,
          "resolveSupport": {
            "properties": [
              "edit"
            ]
          }
        },
        "codeLens": {
          "dynamicRegistration": true
        },
        "colorProvider": {
          "dynamicRegistration": true
        },
        "completion": {
          "completionItem": {
            "commitCharactersSupport": true,
            "deprecatedSupport": true,
            "documentationFormat": [
              "markdown",
              "plaintext"
            ],
            "insertReplaceSupport": true,
            "insertTextModeSupport": {
              "valueSet": [
                1,
                2
              ]
            },
            "labelDetailsSupport": true,
            "preselectSupport": true,
            "resolveSupport": {
              "properties": [
                "documentation",
                "detail",
                "additionalTextEdits"
              ]
            },
            "snippetSupport": true,
            "tagSupport": {
              "valueSet": [
                1
              ]
            }
          },
          "completionItemKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25
            ]
          },
          "completionList": {
            "itemDefaults": [
              "commitCharacters",
              "editRange",
              "insertTextFormat",
              "insertTextMode"
            ]
          },
          "contextSupport": true,
          "dynamicRegistration": true,
          "editsNearCursor": true,
          "insertTextMode": 2
        },
        "declaration": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "definition": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "diagnostic": {
          "dynamicRegistration": true,
          "relatedDocumentSupport": false
        },
        "documentHighlight": {
          "dynamicRegistration": true
        },
        "documentLink": {
          "dynamicRegistration": true,
          "tooltipSupport": true
        },
        "documentSymbol": {
          "dynamicRegistration": true,
          "hierarchicalDocumentSymbolSupport": true,
          "labelSupport": true,
          "symbolKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25,
              26
            ]
          },
          "tagSupport": {
            "valueSet": [
              1
            ]
          }
        },
        "foldingRange": {
          "dynamicRegistration": true,
          "foldingRange": {
            "collapsedText": false
          },
          "foldingRangeKind": {
            "valueSet": [
              "comment",
              "imports",
              "region"
            ]
          },
          "lineFoldingOnly": true,
          "rangeLimit": 5000
        },
        "formatting": {
          "dynamicRegistration": true
        },
        "hover": {
          "contentFormat": [
            "markdown",
            "plaintext"
          ],
          "dynamicRegistration": true
        },
        "implementation": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "inactiveRegionsCapabilities": {
          "inactiveRegions": true
        },
        "inlayHint": {
          "dynamicRegistration": true,
          "resolveSupport": {
            "properties": [
              "tooltip",
              "textEdits",
              "label.tooltip",
              "label.location",
              "label.command"
            ]
          }
        },
        "inlineValue": {
          "dynamicRegistration": true
        },
        "linkedEditingRange": {
          "dynamicRegistration": true
        },
        "onTypeFormatting": {
          "dynamicRegistration": true
        },
        "publishDiagnostics": {
          "codeDescriptionSupport": true,
          "dataSupport": true,
          "relatedInformation": true,
          "tagSupport": {
            "valueSet": [
              1,
              2
            ]
          },
          "versionSupport": false
        },
        "rangeFormatting": {
          "dynamicRegistration": true
        },
        "references": {
          "dynamicRegistration": true
        },
        "rename": {
          "dynamicRegistration": true,
          "honorsChangeAnnotations": true,
          "prepareSupport": true,
          "prepareSupportDefaultBehavior": 1
        },
        "selectionRange": {
          "dynamicRegistration": true
        },
        "semanticTokens": {
          "augmentsSyntaxTokens": true,
          "dynamicRegistration": true,
          "formats": [
            "relative"
          ],
          "multilineTokenSupport": false,
          "overlappingTokenSupport": false,
          "requests": {
            "full": {
              "delta": true
            },
            "range": true
          },
          "serverCancelSupport": true,
          "tokenModifiers": [
            "declaration",
            "definition",
            "readonly",
            "static",
            "deprecated",
            "abstract",
            "async",
            "modification",
            "documentation",
            "defaultLibrary"
          ],
          "tokenTypes": [
            "namespace",
            "type",
            "class",
            "enum",
            "interface",
            "struct",
            "typeParameter",
            "parameter",
            "variable",
            "property",
            "enumMember",
            "event",
            "function",
            "method",
            "macro",
            "keyword",
            "modifier",
            "comment",
            "string",
            "number",
            "regexp",
            "operator",
            "decorator"
          ]
        },
        "signatureHelp": {
          "contextSupport": true,
          "dynamicRegistration": true,
          "signatureInformation": {
            "activeParameterSupport": true,
            "documentationFormat": [
              "markdown",
              "plaintext"
            ],
            "parameterInformation": {
              "labelOffsetSupport": true
            }
          }
        },
        "synchronization": {
          "didSave": true,
          "dynamicRegistration": true,
          "willSave": true,
          "willSaveWaitUntil": true
        },
        "typeDefinition": {
          "dynamicRegistration": true,
          "linkSupport": true
        },
        "typeHierarchy": {
          "dynamicRegistration": true
        }
      },
      "window": {
        "showDocument": {
          "support": true
        },
        "showMessage": {
          "messageActionItem": {
            "additionalPropertiesSupport": true
          }
        },
        "workDoneProgress": true
      },
      "workspace": {
        "applyEdit": true,
        "codeLens": {
          "refreshSupport": true
        },
        "configuration": true,
        "diagnostics": {
          "refreshSupport": true
        },
        "didChangeConfiguration": {
          "dynamicRegistration": true
        },
        "didChangeWatchedFiles": {
          "dynamicRegistration": true,
          "relativePatternSupport": true
        },
        "executeCommand": {
          "dynamicRegistration": true
        },
        "fileOperations": {
          "didCreate": true,
          "didDelete": true,
          "didRename": true,
          "dynamicRegistration": true,
          "willCreate": true,
          "willDelete": true,
          "willRename": true
        },
        "inlayHint": {
          "refreshSupport": true
        },
        "inlineValue": {
          "refreshSupport": true
        },
        "semanticTokens": {
          "refreshSupport": true
        },
        "symbol": {
          "dynamicRegistration": true,
          "resolveSupport": {
            "properties": [
              "location.range"
            ]
          },
          "symbolKind": {
            "valueSet": [
              1,
              2,
              3,
              4,
              5,
              6,
              7,
              8,
              9,
              10,
              11,
              12,
              13,
              14,
              15,
              16,
              17,
              18,
              19,
              20,
              21,
              22,
              23,
              24,
              25,
              26
            ]
          },
          "tagSupport": {
            "valueSet": [
              1
            ]
          }
        },
        "workspaceEdit": {
          "changeAnnotationSupport": {
            "groupsOnLabel": true
          },
          "documentChanges": true,
          "failureHandling": "textOnlyTransactional",
          "normalizesLineEndings": true,
          "resourceOperations": [
            "create",
            "rename",
            "delete"
          ]
        },
        "workspaceFolders": true
      }
    },
    "clientInfo": {
      "name": "Visual Studio Code",
      "version": "1.100.2"
    },
    "initializationOptions": {
      "clangdFileStatus": true,
      "fallbackFlags": []
    },
    "locale": "en",
    "processId": 3409019,
    "rootPath": "/data/repo/cmn",
    "rootUri": "file:///data/repo/cmn",
    "trace": "off",
    "workspaceFolders": [
      {
        "name": "cmn",
        "uri": "file:///data/repo/cmn"
      }
    ]
  }
}

I[07:57:44.485] &lt;-- initialize(0)
I[07:57:44.516] --&gt; reply:initialize(0) 30 ms
V[07:57:44.516] &gt;&gt;&gt; {
  "id": 0,
  "jsonrpc": "2.0",
  "result": {
    "capabilities": {
      "astProvider": true,
      "callHierarchyProvider": true,
      "clangdInlayHintsProvider": true,
      "codeActionProvider": {
        "codeActionKinds": [
          "quickfix",
          "refactor",
          "info"
        ]
      },
      "compilationDatabase": {
        "automaticReload": true
      },
      "completionProvider": {
        "resolveProvider": false,
        "triggerCharacters": [
          ".",
          "&lt;",
          "&gt;",
          ":",
          "\"",
          "/",
          "*"
        ]
      },
      "declarationProvider": true,
      "definitionProvider": true,
      "documentFormattingProvider": true,
      "documentHighlightProvider": true,
      "documentLinkProvider": {
        "resolveProvider": false
      },
      "documentOnTypeFormattingProvider": {
        "firstTriggerCharacter": "\n",
        "moreTriggerCharacter": []
      },
      "documentRangeFormattingProvider": true,
      "documentSymbolProvider": true,
      "executeCommandProvider": {
        "commands": [
          "clangd.applyFix",
          "clangd.applyRename",
          "clangd.applyTweak"
        ]
      },
      "foldingRangeProvider": true,
      "hoverProvider": true,
      "implementationProvider": true,
      "inactiveRegionsProvider": true,
      "inlayHintProvider": true,
      "memoryUsageProvider": true,
      "referencesProvider": true,
      "renameProvider": {
        "prepareProvider": true
      },
      "selectionRangeProvider": true,
      "semanticTokensProvider": {
        "full": {
          "delta": true
        },
        "legend": {
          "tokenModifiers": [
            "declaration",
            "definition",
            "deprecated",
            "deduced",
            "readonly",
            "static",
            "abstract",
            "virtual",
            "dependentName",
            "defaultLibrary",
            "usedAsMutableReference",
            "usedAsMutablePointer",
            "constructorOrDestructor",
            "userDefined",
            "functionScope",
            "classScope",
            "fileScope",
            "globalScope"
          ],
          "tokenTypes": [
            "variable",
            "variable",
            "parameter",
            "function",
            "method",
            "function",
            "property",
            "variable",
            "class",
            "interface",
            "enum",
            "enumMember",
            "type",
            "type",
            "unknown",
            "namespace",
            "typeParameter",
            "concept",
            "type",
            "macro",
            "modifier",
            "operator",
            "bracket",
            "label",
            "comment"
          ]
        },
        "range": false
      },
      "signatureHelpProvider": {
        "triggerCharacters": [
          "(",
          ")",
          "{",
          "}",
          "&lt;",
          "&gt;",
          ","
        ]
      },
      "standardTypeHierarchyProvider": true,
      "textDocumentSync": {
        "change": 2,
        "openClose": true,
        "save": true
      },
      "typeDefinitionProvider": true,
      "typeHierarchyProvider": true,
      "workspaceSymbolProvider": true
    },
    "serverInfo": {
      "name": "clangd",
      "version": "Ubuntu clangd version 20.1.5 (++20250510053315+4370072022e5-1~exp1~20250510173335.114) linux+grpc x86_64-pc-linux-gnu"
    }
  }
}

V[07:57:44.518] &lt;&lt;&lt; {
  "jsonrpc": "2.0",
  "method": "initialized",
  "params": {}
}

I[07:57:44.518] &lt;-- initialized
V[07:57:44.521] &lt;&lt;&lt; {
  "jsonrpc": "2.0",
  "method": "textDocument/didOpen",
  "params": {
    "textDocument": {
      "languageId": "cpp",
      "text": "// #include &lt;cmn/tester.hpp&gt;\n\n// #include &lt;chrono&gt;\n// #include &lt;stdexcept&gt;\n// #include &lt;thread&gt;\n\n// #include &lt;cmn/test/test_all.hpp&gt;\n\n// TEST_CASE (test1) {\n//     std::this_thread::sleep_for(2s);\n// }\n\n// TEST_CASE (test2) {\n//     std::this_thread::sleep_for(3s);\n// }\n\n// TEST_CASE (test3) {\n//     std::this_thread::sleep_for(1s);\n// }\n\n#include &lt;concepts&gt;\n\ntemplate&lt;typename T&gt;\nconcept IsOK = []&lt;typename U&gt; consteval -&gt; bool {    \n    return requires { requires std::same_as&lt;typename U::InnerType, void&gt;; };\n}.template operator()&lt;T&gt;();\n\nstatic_assert(!IsOK&lt;int&gt;);\n\nint main() {\n    // cmn::run_test_cases();\n}\n",
      "uri": "file:///data/repo/cmn/main.cpp",
      "version": 1
    }
  }
}

I[07:57:44.521] &lt;-- textDocument/didOpen
V[07:57:44.552] System include extraction: driver clang expanded to /usr/lib/llvm-20/bin/clang
V[07:57:44.552] System include extraction: not allowed driver /usr/lib/llvm-20/bin/clang
V[07:57:44.552] &lt;&lt;&lt; {
  "id": 1,
  "jsonrpc": "2.0",
  "method": "textDocument/documentSymbol",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.552] &lt;-- textDocument/documentSymbol(1)
V[07:57:44.552] &lt;&lt;&lt; {
  "id": 2,
  "jsonrpc": "2.0",
  "method": "textDocument/codeAction",
  "params": {
    "context": {
      "diagnostics": [],
      "triggerKind": 2
    },
    "range": {
      "end": {
        "character": 49,
        "line": 23
      },
      "start": {
        "character": 49,
        "line": 23
      }
    },
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.552] &lt;-- textDocument/codeAction(2)
V[07:57:44.553] config note at /data/repo/cmn/.clangd:4:0: Parsing config fragment
V[07:57:44.553] config note at /data/repo/cmn/.clangd:1:0: Parsed 1 fragments from file
V[07:57:44.553] Config fragment: compiling /data/repo/cmn/.clangd:4 -&gt; 0x00007F3790002C60 (trusted=false)
I[07:57:44.553] config warning at :-1:-1: Invalid MissingIncludes value 'true'. Valid values are Strict, None.
I[07:57:44.553] --&gt; textDocument/publishDiagnostics
V[07:57:44.553] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/publishDiagnostics",
  "params": {
    "diagnostics": [],
    "uri": "file:///data/repo/cmn/.clangd"
  }
}

I[07:57:44.554] Failed to find compilation database for /data/repo/cmn/main.cpp
V[07:57:44.580] System include extraction: driver clang expanded to /usr/lib/llvm-20/bin/clang
V[07:57:44.580] System include extraction: not allowed driver /usr/lib/llvm-20/bin/clang
I[07:57:44.580] ASTWorker building file /data/repo/cmn/main.cpp version 1 with command clangd fallback
[/data/repo/cmn]
/usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
V[07:57:44.581] Driver produced command: cc1 -cc1 -triple x86_64-pc-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -target-feature +avx2 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/data/repo/cmn -fcoverage-compilation-dir=/data/repo/cmn -resource-dir /usr/lib/llvm-20/lib/clang/20 -isystem /usr/include/c++/v1 -isystem /usr/include/c++/v1 -isystem /usr/include/x86_64-linux-gnu -isystem /usr/include -isystem . -D _FILE_OFFSET_BITS=64 -D _TIME_BITS=64 -D IS_DEBUGGING=true -internal-isystem /usr/lib/llvm-20/bin/../include/c++/v1 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -std=c++2c -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ /data/repo/cmn/main.cpp
I[07:57:44.581] --&gt; textDocument/clangd.fileStatus
V[07:57:44.581] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/clangd.fileStatus",
  "params": {
    "state": "parsing includes, running Update",
    "uri": "file:///data/repo/cmn/main.cpp"
  }
}

V[07:57:44.581] Building first preamble for /data/repo/cmn/main.cpp version 1
I[07:57:44.630] Built preamble of size 636760 for file /data/repo/cmn/main.cpp version 1 in 0.05 seconds
I[07:57:44.631] --&gt; workspace/semanticTokens/refresh(0)
I[07:57:44.631] Indexing c++23 standard library in the context of /data/repo/cmn/main.cpp
V[07:57:44.631] &gt;&gt;&gt; {
  "id": 0,
  "jsonrpc": "2.0",
  "method": "workspace/semanticTokens/refresh",
  "params": null
}

I[07:57:44.631] --&gt; textDocument/clangd.fileStatus
V[07:57:44.631] &gt;&gt;&gt; {
  "jsonrpc": "2.0",
  "method": "textDocument/clangd.fileStatus",
  "params": {
    "state": "parsing includes, running Build AST",
    "uri": "file:///data/repo/cmn/main.cpp"
  }
}

V[07:57:44.631] &lt;&lt;&lt; {
  "id": 0,
  "jsonrpc": "2.0",
  "result": null
}

I[07:57:44.631] &lt;-- reply(0)
V[07:57:44.636] &lt;&lt;&lt; {
  "id": 3,
  "jsonrpc": "2.0",
  "method": "textDocument/semanticTokens/full",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.636] &lt;-- textDocument/semanticTokens/full(3)
V[07:57:44.639] Ignored diagnostic. invalid configuration value 'lower_space' for option 'readability-identifier-naming.NamespaceCase' [clang-tidy-config]
V[07:57:44.641] indexed preamble AST for /data/repo/cmn/main.cpp version 1:
  symbol slab: 739 symbols, 204336 bytes
  ref slab: 0 symbols, 0 refs, 128 bytes
  relations slab: 58 relations, 1112 bytes
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/data/repo/cmn/main.cpp:28:25: current parser token ')'
V[07:57:44.647] Build dynamic index for header symbols with estimated memory usage of 1275412 bytes
V[07:57:44.725] &lt;&lt;&lt; {
  "id": 4,
  "jsonrpc": "2.0",
  "method": "textDocument/foldingRange",
  "params": {
    "textDocument": {
      "uri": "file:///data/repo/cmn/main.cpp"
    }
  }
}

I[07:57:44.725] &lt;-- textDocument/foldingRange(4)
I[07:57:44.725] --&gt; reply:textDocument/foldingRange(4) 0 ms
V[07:57:44.725] &gt;&gt;&gt; {
  "id": 4,
  "jsonrpc": "2.0",
  "result": [
    {
      "endLine": 24,
      "kind": "region",
      "startCharacter": 49,
      "startLine": 23
    },
    {
      "endLine": 30,
      "kind": "region",
      "startCharacter": 12,
      "startLine": 29
    },
    {
      "endCharacter": 20,
      "endLine": 4,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 2
    },
    {
      "endCharacter": 4,
      "endLine": 10,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 8
    },
    {
      "endCharacter": 4,
      "endLine": 14,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 12
    },
    {
      "endCharacter": 4,
      "endLine": 18,
      "kind": "comment",
      "startCharacter": 2,
      "startLine": 16
    }
  ]
}

I[07:57:47.622] Indexed c++23 standard library: 14259 symbols, 1797 filtered
V[07:57:47.664] Build dynamic index for header symbols with estimated memory usage of 10554860 bytes
 #<!-- -->0 0x00007f37ce32137f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x103337f)
 #<!-- -->1 0x00007f37ce31f089 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1031089)
 #<!-- -->2 0x00007f37ce321a90 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1033a90)
 #<!-- -->3 0x00007f37cc442330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f37d6db3afd (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1133afd)
 #<!-- -->5 0x00007f37d6d5ce33 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10dce33)
 #<!-- -->6 0x00007f37d6d58ad3 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d8ad3)
 #<!-- -->7 0x00007f37d6d5854b clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x10d854b)
 #<!-- -->8 0x00007f37d747a7d6 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa7d6)
 #<!-- -->9 0x00007f37d74738b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->10 0x00007f37d7a64322 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de4322)
#<!-- -->11 0x00007f37d7a5726b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1dd726b)
#<!-- -->12 0x00007f37d7a74378 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1df4378)
#<!-- -->13 0x00007f37d7a62414 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de2414)
#<!-- -->14 0x00007f37d7a67d09 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1de7d09)
#<!-- -->15 0x00007f37d7a5aa24 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddaa24)
#<!-- -->16 0x00007f37d7a5f58b (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddf58b)
#<!-- -->17 0x00007f37d7a5cae4 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1ddcae4)
#<!-- -->18 0x00007f37d7a26f47 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1da6f47)
#<!-- -->19 0x00007f37d747b86d (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fb86d)
#<!-- -->20 0x00007f37d747a743 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17fa743)
#<!-- -->21 0x00007f37d74738b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f38b5)
#<!-- -->22 0x00007f37d747360e clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x17f360e)
#<!-- -->23 0x00007f37d7937589 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7589)
#<!-- -->24 0x00007f37d7937a34 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1cb7a34)
#<!-- -->25 0x00007f37d75b3c62 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1933c62)
#<!-- -->26 0x00007f37d6a45547 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc5547)
#<!-- -->27 0x00007f37d6a4675d clang::Parser::ParseCXXIdExpression(bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdc675d)
#<!-- -->28 0x00007f37d6a36b32 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6b32)
#<!-- -->29 0x00007f37d6a36bdc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb6bdc)
#<!-- -->30 0x00007f37d6a3531b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb531b)
#<!-- -->31 0x00007f37d6a345ec clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdb45ec)
#<!-- -->32 0x00007f37d6a1fe8b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd9fe8b)
#<!-- -->33 0x00007f37d6a01c91 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd81c91)
#<!-- -->34 0x00007f37d6aa935e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2935e)
#<!-- -->35 0x00007f37d6aa8318 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe28318)
#<!-- -->36 0x00007f37d69ee76e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd6e76e)
#<!-- -->37 0x00007f37d8950c42 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cd0c42)
#<!-- -->38 0x0000563bd4ecefd3 (/usr/lib/llvm-20/bin/clangd+0x3f9fd3)
#<!-- -->39 0x0000563bd4f72818 (/usr/lib/llvm-20/bin/clangd+0x49d818)
#<!-- -->40 0x0000563bd4f72260 (/usr/lib/llvm-20/bin/clangd+0x49d260)
#<!-- -->41 0x0000563bd4f6eaee (/usr/lib/llvm-20/bin/clangd+0x499aee)
#<!-- -->42 0x0000563bd4f6e68b (/usr/lib/llvm-20/bin/clangd+0x49968b)
#<!-- -->43 0x0000563bd50d94e1 (/usr/lib/llvm-20/bin/clangd+0x6044e1)
#<!-- -->44 0x00007f37cc499aa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->45 0x00007f37cc526c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: main.cpp
  Directory: /data/repo/cmn
  Command Line: /usr/lib/llvm-20/bin/clang -std=c++2c -stdlib=libc++ -isystem/usr/include/c++/v1 -g -pipe -m64 -mavx2 -pthread -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64 -DIS_DEBUGGING=true -Wall -Wextra -Werror -Wconversion -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wformat -isystem/usr/include/c++/v1 -isystem/usr/include/x86_64-linux-gnu -isystem/usr/include -isystem. -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /data/repo/cmn/main.cpp
  Version: 1
[Error - 7:57:49 AM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 7:57:49 AM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 7:57:49 AM] Request textDocument/codeAction failed.
[object Object]
[Error - 7:57:49 AM] Request textDocument/semanticTokens/full failed.
[object Object]
```

</details>


---

### Comment 5 - yaoxinliu

@cor3ntin

This is a regression back to clang-16. clang-15 is ok, see [https://godbolt.org/z/e5jof54eo](https://godbolt.org/z/e5jof54eo)

Related issue: [https://github.com/llvm/llvm-project/issues/141732](https://github.com/llvm/llvm-project/issues/141732)

---

### Comment 6 - Thibault-Monnier

This seems to have been fixed in trunk.

---

