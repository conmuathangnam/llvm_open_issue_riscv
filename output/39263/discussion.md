# Invalid code generation and ICE when template references local static constexpr variable by pointer

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/39263
**Status:** Closed
**Labels:** c++17, clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2025-05-08T21:42:46Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [39916](https://llvm.org/bz39916) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @zygoloid |

## Extended Description 
The following minimised code fragment generates incorrect code in all clang version from 4.0 (oldest I have) up to trunk (tested using godbolt and -O2 -std=c++1z):

```cpp
  template <const int* X>
  struct S
    {
    static int Value() { return *X; }
    };

  template <typename T>
  int fn()
    {
    static constexpr int value = 10;
    return S<&value>::Value();
    }

  int test()
    {
    return fn<int>();   // will return 0 not 10
    }
```

There are ways that the code can be made to produce correct values:

  1. Move the 'static constexpr int value = 10' line up to the global scope
  2. Remove the template from 'int fn()' to make it a plain function
  3. Change the template parameter in struct S from 'const int* X' to 'const int& X'

So, for example, the following compiles correctly...

```cpp
  template <const int& X>
  struct S
    {
    static int Value() { return X; }
    };

  template <typename T>
  int fn()
    {
    static constexpr int value = 10;
    return S<value>::Value();
    }

  int test()
    {
    return fn<int>();   // will return 0 not 10
    }
```

The following, slightly more complex example, crashes the compiler:

```cpp
  template <const int* X>
  struct S
    {
    static int Value() { return *X; }
    };

  template <typename T>
  int fn()
    {
    const int c = ([]() {
        static constexpr int value = 10;
        return S<&value>::Value();
      })();
    return c;
    }

  int test()
    {
    return fn<int>();
    }
```

Again, any one of the three "solutions" above solves the problem.  So the issue seems to be in the specific use of pointers in the template for S when called from another templated function.

However, while we're here, this also crashes the compiler:

```cpp
 template <const int& X>
  struct S
    {
    static int Value() { return X; }
    };

  template <typename T>
  int fn()
    {
    const int c = ([]() {
        static int value = 10;     // not const or constexpr!
        return S<value>::Value();
      })();
    return c;
    }

  int test()
    {
    return fn<int>();
    }
```

But utilising a second "solution" from above means this is ok again:

```cpp
 template <const int& X>
  struct S
    {
    static int Value() { return X; }
    };

  int fn()
    {
    const int c = ([]() {
        static int value = 10;
        return S<value>::Value();
      })();
    return c;
    }

  int test()
    {
    return fn();
    }
```

## Comments

### Comment 1 - llvmbot

I don't know if it is helpful, but in case it is, at the point that the assert is fired, it is because "D" in "LocalInstantiationScope::findInstantiationOf(const clang::Decl *)" is a non-instantiated VarDecl.

---

### Comment 2 - llvmbot

Having now run these code fragments in a local build of clang 7.0.0, I can confirm that all but the final code fragment will crash a compiler built with release-asserts.  It seems the godbolt compiler, which I used to test most of the above fragments, is built without asserts (or there really is some weird difference between their builds and mine which I cannot determine).

The following code fragment (a variant on the above)...

```cpp
  template <const int& X>
  constexpr auto fn1() { return X; }

  template <typename>
  auto fn2()
    {
    return ([]() {
        static constexpr auto value = 12;
        return fn1<value>();
      })();
    }

  auto test()
    {
    return fn2<int>();
    }
```

will produce the following crash log...

```console
~/Toolchains/new/build$ final/bin/clang++ -std=c++1z -O2 -v test.cpp

clang version 7.0.0 (branches/release_70 342383)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/andyg/Toolchains/new/build/final/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6.3.0
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6.3.0
Candidate multilib: .;@m64
Selected multilib: .;@m64
 "/home/andyg/Toolchains/new/build/final/bin/clang-7" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -main-file-name test.cpp -mrelocation-model static -mthread-model posix -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -momit-leaf-frame-pointer -v -resource-dir /home/andyg/Toolchains/new/build/final/lib/clang/7.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0/backward -internal-isystem /usr/local/include -internal-isystem /home/andyg/Toolchains/new/build/final/lib/clang/7.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++1z -fdeprecated-macro -fdebug-compilation-dir /home/andyg/Toolchains/new/build -ferror-limit 19 -fmessage-length 271 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -vectorize-loops -vectorize-slp -o /tmp/test-2afeb0.o -x c++ test.cpp -faddrsig
clang -cc1 version 7.0.0 based upon LLVM 7.0.0 default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0
 /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0
 /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0/backward
 /usr/local/include
 /home/andyg/Toolchains/new/build/final/lib/clang/7.0.0/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
clang-7: /home/andyg/Toolchains/new/source/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2920: llvm::PointerUnion<Decl *, LocalInstantiationScope::DeclArgumentPack *> *clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl *): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
Stack dump:
0.      Program arguments: /home/andyg/Toolchains/new/build/final/bin/clang-7 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -main-file-name test.cpp -mrelocation-model static -mthread-model posix -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -momit-leaf-frame-pointer -v -resource-dir /home/andyg/Toolchains/new/build/final/lib/clang/7.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/x86_64-linux-gnu/c++/6.3.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/6.3.0/../../../../include/c++/6.3.0/backward -internal-isystem /usr/local/include -internal-isystem /home/andyg/Toolchains/new/build/final/lib/clang/7.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++1z -fdeprecated-macro -fdebug-compilation-dir /home/andyg/Toolchains/new/build -ferror-limit 19 -fmessage-length 271 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -vectorize-loops -vectorize-slp -o /tmp/test-2afeb0.o -x c++ test.cpp -faddrsig 
1.      test.cpp:9:27: current parser token ')'
2.      test.cpp:6:5: parsing function body 'fn2'
3.      test.cpp:6:5: in compound statement ('{}')
4.      test.cpp:7:13: lambda expression parsing
5.      test.cpp:7:18: in compound statement ('{}')
6.      test.cpp:2:18: instantiating function definition 'fn1<&value>'
#&#8203;0 0x0000000002135834 PrintStackTraceSignalHandler(void*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x2135834)
#&#8203;1 0x00000000021335be llvm::sys::RunSignalHandlers() (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x21335be)
#&#8203;2 0x00000000021359f2 SignalHandler(int) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x21359f2)
#&#8203;3 0x00007fd4e06060c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x110c0)
#&#8203;4 0x00007fd4df19bfff gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x32fff)
#&#8203;5 0x00007fd4df19d42a abort (/lib/x86_64-linux-gnu/libc.so.6+0x3442a)
#&#8203;6 0x00007fd4df194e67 (/lib/x86_64-linux-gnu/libc.so.6+0x2be67)
#&#8203;7 0x00007fd4df194f12 (/lib/x86_64-linux-gnu/libc.so.6+0x2bf12)
#&#8203;8 0x0000000003a2923a clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a2923a)
#&#8203;9 0x0000000003a4dbbc clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a4dbbc)
#&#8203;10 0x0000000003a3b12c (anonymous namespace)::TemplateInstantiator::transformNonTypeTemplateParmRef(clang::NonTypeTemplateParmDecl*, clang::SourceLocation, clang::TemplateArgument) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a3b12c)
#&#8203;11 0x0000000003a3a91d (anonymous namespace)::TemplateInstantiator::TransformTemplateParmRefExpr(clang::DeclRefExpr*, clang::NonTypeTemplateParmDecl*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a3a91d)
#&#8203;12 0x0000000003a448df clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a448df)
#&#8203;13 0x0000000003a3d481 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a3d481)
#&#8203;14 0x0000000003a25b09 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a25b09)
#&#8203;15 0x0000000003a5f0e4 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3a5f0e4)
#&#8203;16 0x00000000039e31d6 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x39e31d6)
#&#8203;17 0x00000000036bdb25 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x36bdb25)
#&#8203;18 0x00000000038e8097 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x38e8097)
#&#8203;19 0x00000000038e7fba clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x38e7fba)
#&#8203;20 0x00000000036c5930 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x36c5930)
#&#8203;21 0x000000000323235f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x323235f)
#&#8203;22 0x0000000003234526 clang::Parser::ParseCastExpression(bool, bool, bool&, clang::Parser::TypeCastState, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3234526)
#&#8203;23 0x0000000003237b31 clang::Parser::ParseCastExpression(bool, bool, bool&, clang::Parser::TypeCastState, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3237b31)
#&#8203;24 0x000000000322fde2 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x322fde2)
#&#8203;25 0x000000000322fd29 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x322fd29)
#&#8203;26 0x000000000327c72c clang::Parser::ParseReturnStatement() (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x327c72c)
#&#8203;27 0x0000000003277790 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*, clang::Parser::ParsedAttributesWithRange&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3277790)
#&#8203;28 0x0000000003276bc9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3276bc9)
#&#8203;29 0x000000000327e2e1 clang::Parser::ParseCompoundStatementBody(bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x327e2e1)
#&#8203;30 0x00000000032448ef clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x32448ef)
#&#8203;31 0x0000000003243467 clang::Parser::ParseLambdaExpression() (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3243467)
#&#8203;32 0x0000000003234511 clang::Parser::ParseCastExpression(bool, bool, bool&, clang::Parser::TypeCastState, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3234511)
#&#8203;33 0x000000000322fde2 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x322fde2)
#&#8203;34 0x000000000323884f clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x323884f)
#&#8203;35 0x0000000003233e2e clang::Parser::ParseCastExpression(bool, bool, bool&, clang::Parser::TypeCastState, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3233e2e)
#&#8203;36 0x000000000322fde2 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x322fde2)
#&#8203;37 0x000000000322fd29 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x322fd29)
#&#8203;38 0x000000000327c72c clang::Parser::ParseReturnStatement() (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x327c72c)
#&#8203;39 0x0000000003277790 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*, clang::Parser::ParsedAttributesWithRange&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3277790)
#&#8203;40 0x0000000003276bc9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3276bc9)
#&#8203;41 0x000000000327e2e1 clang::Parser::ParseCompoundStatementBody(bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x327e2e1)
#&#8203;42 0x000000000327eb22 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x327eb22)
#&#8203;43 0x00000000031f15a5 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x31f15a5)
#&#8203;44 0x00000000032863c0 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x32863c0)
#&#8203;45 0x0000000003284f4e clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3284f4e)
#&#8203;46 0x0000000003284916 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x3284916)
#&#8203;47 0x000000000320573f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::Parser::ParsedAttributesWithRange&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x320573f)
#&#8203;48 0x00000000031ee34d clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x31ee34d)
#&#8203;49 0x00000000031edb8b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x31edb8b)
#&#8203;50 0x00000000031e9597 clang::ParseAST(clang::Sema&, bool, bool) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x31e9597)
#&#8203;51 0x00000000027bc6b0 clang::FrontendAction::Execute() (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x27bc6b0)
#&#8203;52 0x0000000002766f41 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x2766f41)
#&#8203;53 0x000000000285fc86 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x285fc86)
#&#8203;54 0x0000000000984f3a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x984f3a)
#&#8203;55 0x0000000000983075 main (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x983075)
#&#8203;56 0x00007fd4df1892e1 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x202e1)
#&#8203;57 0x000000000098024a _start (/home/andyg/Toolchains/new/build/final/bin/clang-7+0x98024a)
clang-7: error: unable to execute command: Aborted
clang-7: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 7.0.0 (branches/release_70 342383)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

---

### Comment 3 - shafik

The original posted code now looks like it works but the last snippet still crashes clang and also gcc as well: https://godbolt.org/z/TojrozTz6

Assertion:

```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4063:
 llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): 
Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
```

backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++1z -O2 <source>
1.	<source>:9:27: current parser token ')'
2.	<source>:6:5: parsing function body 'fn2'
3.	<source>:6:5: in compound statement ('{}')
4.	<source>:7:13: lambda expression parsing
5.	<source>:7:18: in compound statement ('{}')
6.	<source>:2:18: instantiating function definition 'fn1<value>'
 #0 0x000055e949f7fc7f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40a7c7f)
 #1 0x000055e949f7dc1c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40a5c1c)
 #2 0x000055e949ecb1a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc41caf8420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fc41c5c500b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fc41c5a4859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fc41c5a4729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fc41c5b5fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055e94d0b5395 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71dd395)
 #9 0x000055e94d141417 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7269417)
#10 0x000055e94d0d6c29 (anonymous namespace)::TemplateInstantiator::transformNonTypeTemplateParmRef(clang::Decl*, clang::NonTypeTemplateParmDecl const*, clang::SourceLocation, clang::TemplateArgument, std::optional<unsigned int>) SemaTemplateInstantiate.cpp:0:0
#11 0x000055e94d0e37e1 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000055e94d0bfec7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000055e94d0c08d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#14 0x000055e94d0c0c5c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#15 0x000055e94d0f6049 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#16 0x000055e94d0fb67e clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722367e)
#17 0x000055e94d14901a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x727101a)
#18 0x000055e94c7362c5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685e2c5)
#19 0x000055e94d03787f clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715f87f)
#20 0x000055e94caef9ba clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c179ba)
#21 0x000055e94cec15ff FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#22 0x000055e94cec23dd clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fea3dd)
#23 0x000055e94cb519b5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c799b5)
#24 0x000055e94cb7444b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9c44b)
#25 0x000055e94c659039 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6781039)
#26 0x000055e94c651187 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6779187)
#27 0x000055e94c6524b4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677a4b4)
#28 0x000055e94c65426b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c26b)
#29 0x000055e94c65430d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c30d)
#30 0x000055e94c6585bd clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67805bd)
#31 0x000055e94c6c573a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ed73a)
#32 0x000055e94c6c8a40 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0a40)
#33 0x000055e94c6ca025 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f2025)
#34 0x000055e94c6caffa clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f2ffa)
#35 0x000055e94c67200a clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679a00a)
#36 0x000055e94c672a7b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679aa7b)
#37 0x000055e94c652212 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677a212)
#38 0x000055e94c65426b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c26b)
#39 0x000055e94c65430d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c30d)
#40 0x000055e94c6585bd clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67805bd)
#41 0x000055e94c65c79d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678479d)
#42 0x000055e94c65238f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677a38f)
#43 0x000055e94c65426b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c26b)
#44 0x000055e94c65430d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677c30d)
#45 0x000055e94c6585bd clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67805bd)
#46 0x000055e94c6c573a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ed73a)
#47 0x000055e94c6c8a40 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0a40)
#48 0x000055e94c6ca025 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f2025)
#49 0x000055e94c6caffa clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f2ffa)
#50 0x000055e94c6cc9aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f49aa)
#51 0x000055e94c5fc441 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6724441)
#52 0x000055e94c6de546 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6806546)
#53 0x000055e94c6ec0ae clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#54 0x000055e94c6ec343 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6814343)
#55 0x000055e94c629fc0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6751fc0)
#56 0x000055e94c5f6e33 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671ee33)
#57 0x000055e94c5f7d4d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671fd4d)
#58 0x000055e94c5ebc9a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713c9a)
#59 0x000055e94b2f4db8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x541cdb8)
#60 0x000055e94ab6adf9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c92df9)
#61 0x000055e94aaf15de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c195de)
#62 0x000055e94ac50823 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d78823)
#63 0x000055e9474294ed cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x15514ed)
#64 0x000055e947425607 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#65 0x000055e94a95b4d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#66 0x000055e949ecb690 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff3690)
#67 0x000055e94a95bd8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#68 0x000055e94a92496c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a4c96c)
#69 0x000055e94a9253ed clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a4d3ed)
#70 0x000055e94a92e68c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a5668c)
#71 0x000055e947427c52 clang_main(int, char**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x154fc52)
#72 0x00007fc41c5a6083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#73 0x000055e94742030e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x154830e)
clang-16: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 5 - shafik

Slightly simpler reproducer here: https://github.com/llvm/llvm-project/issues/47472

---

### Comment 6 - andyg1001

Tidying up an old bug report of mine.  All the code samples in this report are correctly compiling in clang 20.  This issue can be closed please.

---

