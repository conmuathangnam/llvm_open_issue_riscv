# [clang] Assertion "predeclared global operator new/delete is missing" hit at `CGExprCXX.cpp:1384` when using C++20 modules

**Author:** jiixyj
**URL:** https://github.com/llvm/llvm-project/issues/166068
**Status:** Closed
**Labels:** clang:modules, crash
**Closed Date:** 2025-11-12T05:37:37Z

## Body

I'm getting this assertion failure when compiling a project using C++20 modules and reduced BMI (which is the default). The version is 8e14e2704e0c63e925211c68c061a56f6d281e41 .

```txt
/freebsd/data/linux/git-net/llvm-project/prefix/bin/clang++  -I/home/jan/git/ldgr/src -isystem /home/jan/.conan2/p/b/value72f009ff7e596/p/include -isystem /home/jan/.conan2/p/b/re268e6b7476ccd3/p/include -isystem /home/jan/.conan2/p/b/absei22c7762699c27/p/include -isystem /home/jan/.conan2/p/b/boost478921d8e99f2/p/include -m64 -stdlib=libc++ -fexperimental-library -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics -Wall -Wextra -Wconversion -Wsign-conversion -Wundef -Werror=format -Werror=return-type -Werror=undef -Werror=missing-field-initializers -fdiagnostics-color=always -Wno-non-template-friend -Wno-builtin-declaration-mismatch -Wno-unknown-warning-option -Wno-read-modules-implicitly -Wno-c++26-extensions -fmacro-prefix-map=/home/jan/git/ldgr/src/= -MD -MT src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -MF src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.d @src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.modmap -o src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -c /home/jan/git/ldgr/src/ldgr/core.cppm
predeclared global operator new/delete is missing
UNREACHABLE executed at /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprCXX.cpp:1384!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /freebsd/data/linux/git-net/llvm-project/prefix/bin/clang++ -I/home/jan/git/ldgr/src -isystem /home/jan/.conan2/p/b/value72f009ff7e596/p/include -isystem /home/jan/.conan2/p/b/re268e6b7476ccd3/p/include -isystem /home/jan/.conan2/p/b/absei22c7762699c27/p/include -isystem /home/jan/.conan2/p/b/boost478921d8e99f2/p/include -m64 -stdlib=libc++ -fexperimental-library -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics -Wall -Wextra -Wconversion -Wsign-conversion -Wundef -Werror=format -Werror=return-type -Werror=undef -Werror=missing-field-initializers -fdiagnostics-color=always -Wno-non-template-friend -Wno-builtin-declaration-mismatch -Wno-unknown-warning-option -Wno-read-modules-implicitly -Wno-c++26-extensions -fmacro-prefix-map=/home/jan/git/ldgr/src/= -MD -MT src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -MF src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.d @src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.modmap -o src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -c /home/jan/git/ldgr/src/ldgr/core.cppm
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	/freebsd/data/linux/git-net/llvm-project/prefix/bin/../include/c++/v1/__new/allocate.h:36:1: Generating code for declaration 'std::__libcpp_allocate'
 #0 0x000056144fb98678 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #1 0x000056144fb961f3 llvm::sys::RunSignalHandlers() /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x000056144fb0d816 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x000056144fb0d816 CrashRecoverySignalHandler(int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x00007f435fa56110 __restore_rt libc_sigaction.c:0:0
 #5 0x00007f435faa51ac __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f435fa55fe2 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007f435fa3eefc abort ./stdlib/abort.c:81:3
 #8 0x000056144fb1556f (/freebsd/data/linux/git-net/llvm-project/prefix/bin/clang+++0x1ec956f)
 #9 0x000056144fe4b09b clang::CodeGen::CodeGenFunction::EmitBuiltinNewDeleteCall(clang::FunctionProtoType const*, clang::CallExpr const*, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprCXX.cpp:0:3
#10 0x00005614501ae7e9 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGBuiltin.cpp:5617:5
#11 0x000056144fe2a4a4 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExpr.cpp:6012:5
#12 0x000056144fe850da clang::CodeGen::RValue::isScalar() const /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGValue.h:64:41
#13 0x000056144fe850da clang::CodeGen::RValue::getScalarVal() const /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGValue.h:72:5
#14 0x000056144fe850da (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:637:36
#15 0x000056144fe8cc8a (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#16 0x000056144fe8cc8a (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:2480:18
#17 0x000056144fe7353d (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#18 0x000056144fe7353d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5890:8
#19 0x000056144ff622dd clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1698:26
#20 0x000056144ff5eb81 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:462:5
#21 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::RunCleanupsScope::ForceCleanup(std::initializer_list<llvm::Value**>) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:1075:28
#22 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::RunCleanupsScope::~RunCleanupsScope() /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:1058:9
#23 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:982:3
#24 0x000056144ff5e9fb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:462:5
#25 0x000056144ff6a74c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:591:22
#26 0x000056144ffcedca clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:0:5
#27 0x000056144ffcedca clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1625:5
#28 0x000056144fff7ba9 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6468:3
#29 0x000056144fff053f llvm::TimeTraceScope::~TimeTraceScope() /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#30 0x000056144fff053f clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4552:3
```

There are a lot of module partitions importing each other, but I could narrow it down to a `#include <boost/flyweight.hpp>` in the GMF of one of the leaf partition units.

It seems like in reduced BMI mode, the global `operator new` and `operator delete` decls are not written to the BMI. So when `CodeGenFunction::EmitBuiltinNewDeleteCall` tries to look them up when generating code for `__builtin_operator_new`/`__builtin_operator_delete`, it cannot find them.

With `-fno-modules-reduced-bmi` there is no issue.

This patch here fixes the problem for me:

```patch
diff --git a/clang/lib/Serialization/ASTWriter.cpp b/clang/lib/Serialization/ASTWriter.cpp
index 3ac338e013de..2db4cc044321 100644
--- a/clang/lib/Serialization/ASTWriter.cpp
+++ b/clang/lib/Serialization/ASTWriter.cpp
@@ -3425,6 +3425,14 @@ static bool IsInternalDeclFromFileContext(const Decl *D) {
   return ND->getFormalLinkage() == Linkage::Internal;
 }
 
+static bool IsAllocationFunction(const Decl *D) {
+  if (auto *FD = dyn_cast<FunctionDecl>(D))
+    if (FD->getDeclName().isAnyOperatorNewOrDelete())
+      return true;
+
+  return false;
+}
+
 /// Write the block containing all of the declaration IDs
 /// lexically declared within the given DeclContext.
 ///
@@ -5647,7 +5655,7 @@ void ASTWriter::PrepareWritingSpecialDecls(Sema &SemaRef) {
       continue;
 
     if (GeneratingReducedBMI) {
-      if (D->isFromExplicitGlobalModule())
+      if (D->isFromExplicitGlobalModule() && !IsAllocationFunction(D))
         continue;
 
       // Don't force emitting static entities.
```

...but I'm not sure if this is the right way to fix this.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Jan Kokemüller (jiixyj)

<details>
I'm getting this assertion failure when compiling a project using C++20 modules and reduced BMI (which is the default). The version is 8e14e2704e0c63e925211c68c061a56f6d281e41 .

```txt
/freebsd/data/linux/git-net/llvm-project/prefix/bin/clang++  -I/home/jan/git/ldgr/src -isystem /home/jan/.conan2/p/b/value72f009ff7e596/p/include -isystem /home/jan/.conan2/p/b/re268e6b7476ccd3/p/include -isystem /home/jan/.conan2/p/b/absei22c7762699c27/p/include -isystem /home/jan/.conan2/p/b/boost478921d8e99f2/p/include -m64 -stdlib=libc++ -fexperimental-library -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics -Wall -Wextra -Wconversion -Wsign-conversion -Wundef -Werror=format -Werror=return-type -Werror=undef -Werror=missing-field-initializers -fdiagnostics-color=always -Wno-non-template-friend -Wno-builtin-declaration-mismatch -Wno-unknown-warning-option -Wno-read-modules-implicitly -Wno-c++26-extensions -fmacro-prefix-map=/home/jan/git/ldgr/src/= -MD -MT src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -MF src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.d @<!-- -->src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.modmap -o src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -c /home/jan/git/ldgr/src/ldgr/core.cppm
predeclared global operator new/delete is missing
UNREACHABLE executed at /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprCXX.cpp:1384!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /freebsd/data/linux/git-net/llvm-project/prefix/bin/clang++ -I/home/jan/git/ldgr/src -isystem /home/jan/.conan2/p/b/value72f009ff7e596/p/include -isystem /home/jan/.conan2/p/b/re268e6b7476ccd3/p/include -isystem /home/jan/.conan2/p/b/absei22c7762699c27/p/include -isystem /home/jan/.conan2/p/b/boost478921d8e99f2/p/include -m64 -stdlib=libc++ -fexperimental-library -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics -Wall -Wextra -Wconversion -Wsign-conversion -Wundef -Werror=format -Werror=return-type -Werror=undef -Werror=missing-field-initializers -fdiagnostics-color=always -Wno-non-template-friend -Wno-builtin-declaration-mismatch -Wno-unknown-warning-option -Wno-read-modules-implicitly -Wno-c++26-extensions -fmacro-prefix-map=/home/jan/git/ldgr/src/= -MD -MT src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -MF src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.d @<!-- -->src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o.modmap -o src/CMakeFiles/ldgr.dir/ldgr/core.cppm.o -c /home/jan/git/ldgr/src/ldgr/core.cppm
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
3.	/freebsd/data/linux/git-net/llvm-project/prefix/bin/../include/c++/v1/__new/allocate.h:36:1: Generating code for declaration 'std::__libcpp_allocate'
 #<!-- -->0 0x000056144fb98678 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->1 0x000056144fb961f3 llvm::sys::RunSignalHandlers() /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->2 0x000056144fb0d816 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x000056144fb0d816 CrashRecoverySignalHandler(int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #<!-- -->4 0x00007f435fa56110 __restore_rt libc_sigaction.c:0:0
 #<!-- -->5 0x00007f435faa51ac __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f435fa55fe2 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->7 0x00007f435fa3eefc abort ./stdlib/abort.c:81:3
 #<!-- -->8 0x000056144fb1556f (/freebsd/data/linux/git-net/llvm-project/prefix/bin/clang+++0x1ec956f)
 #<!-- -->9 0x000056144fe4b09b clang::CodeGen::CodeGenFunction::EmitBuiltinNewDeleteCall(clang::FunctionProtoType const*, clang::CallExpr const*, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprCXX.cpp:0:3
#<!-- -->10 0x00005614501ae7e9 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGBuiltin.cpp:5617:5
#<!-- -->11 0x000056144fe2a4a4 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExpr.cpp:6012:5
#<!-- -->12 0x000056144fe850da clang::CodeGen::RValue::isScalar() const /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGValue.h:64:41
#<!-- -->13 0x000056144fe850da clang::CodeGen::RValue::getScalarVal() const /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGValue.h:72:5
#<!-- -->14 0x000056144fe850da (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:637:36
#<!-- -->15 0x000056144fe8cc8a (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#<!-- -->16 0x000056144fe8cc8a (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:2480:18
#<!-- -->17 0x000056144fe7353d (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:453:52
#<!-- -->18 0x000056144fe7353d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5890:8
#<!-- -->19 0x000056144ff622dd clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1698:26
#<!-- -->20 0x000056144ff5eb81 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:462:5
#<!-- -->21 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::RunCleanupsScope::ForceCleanup(std::initializer_list&lt;llvm::Value**&gt;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:1075:28
#<!-- -->22 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::RunCleanupsScope::~RunCleanupsScope() /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:1058:9
#<!-- -->23 0x000056144ff5f986 clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&amp;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:982:3
#<!-- -->24 0x000056144ff5e9fb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:462:5
#<!-- -->25 0x000056144ff6a74c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CGStmt.cpp:591:22
#<!-- -->26 0x000056144ffcedca clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:0:5
#<!-- -->27 0x000056144ffcedca clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1625:5
#<!-- -->28 0x000056144fff7ba9 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6468:3
#<!-- -->29 0x000056144fff053f llvm::TimeTraceScope::~TimeTraceScope() /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#<!-- -->30 0x000056144fff053f clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /freebsd/data/linux/git-net/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4552:3
```

There are a lot of module partitions importing each other, but I could narrow it down to a `#include &lt;boost/flyweight.hpp&gt;` in the GMF of one of the leaf partition units.

It seems like in reduced BMI mode, the global `operator new` and `operator delete` decls are not written to the BMI. So when `CodeGenFunction::EmitBuiltinNewDeleteCall` tries to look them up when generating code for `__builtin_operator_new`/`__builtin_operator_delete`, it cannot find them.

With `-fno-modules-reduced-bmi` there is no issue.

This patch here fixes the problem for me:

```patch
diff --git a/clang/lib/Serialization/ASTWriter.cpp b/clang/lib/Serialization/ASTWriter.cpp
index 3ac338e013de..2db4cc044321 100644
--- a/clang/lib/Serialization/ASTWriter.cpp
+++ b/clang/lib/Serialization/ASTWriter.cpp
@@ -3425,6 +3425,14 @@ static bool IsInternalDeclFromFileContext(const Decl *D) {
   return ND-&gt;getFormalLinkage() == Linkage::Internal;
 }
 
+static bool IsAllocationFunction(const Decl *D) {
+  if (auto *FD = dyn_cast&lt;FunctionDecl&gt;(D))
+    if (FD-&gt;getDeclName().isAnyOperatorNewOrDelete())
+      return true;
+
+  return false;
+}
+
 /// Write the block containing all of the declaration IDs
 /// lexically declared within the given DeclContext.
 ///
@@ -5647,7 +5655,7 @@ void ASTWriter::PrepareWritingSpecialDecls(Sema &amp;SemaRef) {
       continue;
 
     if (GeneratingReducedBMI) {
-      if (D-&gt;isFromExplicitGlobalModule())
+      if (D-&gt;isFromExplicitGlobalModule() &amp;&amp; !IsAllocationFunction(D))
         continue;
 
       // Don't force emitting static entities.
```

...but I'm not sure if this is the right way to fix this.
</details>


---

### Comment 2 - jiixyj

I'm currently working on a minimal reproducer.

---

### Comment 3 - jiixyj

Here is a reproducer:

flyweight.iim:
```cpp
module;
template <typename> struct flyweight_core {
  static bool init() { (void)__builtin_operator_new(2); return true; }
  static bool static_initializer;
};
template <typename T> bool flyweight_core<T>::static_initializer = init();
module ldgr:flyweight;
template <class> void flyweight() {
  (void)flyweight_core<int>::static_initializer;
}
```

account.iim:
```cpp
module ldgr:account;
import :flyweight;
void account() {
  (void)::flyweight<int>;
}
```

core.iim:
```cpp
module ldgr:core;
import :account;
```

to build:
```sh
_clang="clang++"

_mods="
-fmodule-file=ldgr:account=account.pcm
-fmodule-file=ldgr:flyweight=flyweight.pcm
"

"$_clang" -m64 -nostdlib -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics \
	-x c++-module -fmodule-output=flyweight.pcm $_mods -c flyweight.iim
"$_clang" -m64 -nostdlib -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics \
	-x c++-module -fmodule-output=account.pcm $_mods -c account.iim
# this will trigger the assert:
"$_clang" -m64 -nostdlib -O2 -g -DNDEBUG -std=c++26 -fcolor-diagnostics \
	-x c++-module -fmodule-output=core.pcm $_mods -c core.iim
```


---

### Comment 4 - jiixyj

[166068.zip](https://github.com/user-attachments/files/23325478/166068.zip)

---

### Comment 5 - ChuanqiXu9

@jiixyj If you like, you can give a test for https://github.com/llvm/llvm-project/pull/167468. That PR is actually an optimization. So it might optimize the performance further if your code base is large enough.

---

### Comment 6 - jiixyj

Thank you! The reproducer now compiles fine with your fix. I'm hitting the same issue related to `__builtin_operator_new` now a bit later in the compilation process, but I'll open a separate issue for this.

---

### Comment 7 - ChuanqiXu9

> Thank you! The reproducer now compiles fine with your fix. I'm hitting the same issue related to `__builtin_operator_new` now a bit later in the compilation process, but I'll open a separate issue for this.

This is more or less expected. Since that "fix" is actually an optimization. It just optimizes away the problem case in the reproducer. And to be honest, I feel much better to see such optimization chances.

---

