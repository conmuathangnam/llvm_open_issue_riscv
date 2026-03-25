# [OpenMP] clang crashed when openmp's collapse clause argument is unknown

**Author:** fel-cab
**URL:** https://github.com/llvm/llvm-project/issues/63581

## Body

The testcase (compiled with Clang):
```cpp
void f() {
    #pragma omp for collapse(UNKNOWN)
    for (int i = 0; i < 10; ++i);
}
```
https://godbolt.org/z/hb7TKb5GY

Compilation error:
```console
test.c:5:30: error: use of undeclared identifier 'UNKNOWN'
    5 |     #pragma omp for collapse(UNKNOWN)
      |                              ^
clang-17: /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Sema/SemaOpenMP.cpp:10348: clang::StmtResult clang::Sema::ActOnOpenMPForDirective(ArrayRef<clang::OMPClause *>, clang::Stmt *, clang::SourceLocation, clang::SourceLocation, clang::Sema::VarsWithInheritedDSAType &): Assertion `(CurContext->isDependentContext() || B.builtAll()) && "omp for loop exprs were not built"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /autofs/nccs-svm1_sw/frontier/ums/ums012/llvm/17.0.0-20230626/bin/clang-17 -cc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -emit-llvm-uselists -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fcoverage-compilation-dir=/ccs/home/cabarcasf/REDUCTIONS -resource-dir /autofs/nccs-svm1_sw/frontier/ums/ums012/llvm/17.0.0-20230626/lib/clang/17 -c-isystem /sw/frontier/ums/ums012/llvm/17.0.0-20230626/include -cxx-isystem /sw/frontier/ums/ums012/llvm/17.0.0-20230626/include -internal-isystem /autofs/nccs-svm1_sw/frontier/ums/ums012/llvm/17.0.0-20230626/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-suse-linux/7/../../../../x86_64-suse-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /autofs/nccs-svm1_sw/frontier/ums/ums012/llvm/17.0.0-20230626/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-suse-linux/7/../../../../x86_64-suse-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdebug-compilation-dir=/ccs/home/cabarcasf/REDUCTIONS -ferror-limit 19 -fopenmp -fgnuc-version=4.2.1 -fcolor-diagnostics -disable-llvm-passes -fopenmp-targets=amdgcn-amd-amdhsa -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-72c531.bc -x c test.c
1.	test.c:7:1: current parser token '}'
2.	test.c:4:10: parsing function body 'f'
3.	test.c:4:10: in compound statement ('{}')
 #0 0x000000000208f9b7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/llvm/lib/Support/Unix/Signals.inc:602:13
 #1 0x000000000208db20 llvm::sys::RunSignalHandlers() /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x000000000209005f SignalHandler(int) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #3 0x00007f0bef7088c0 __restore_rt (/lib64/libpthread.so.0+0x168c0)
 #4 0x00007f0bee168cbb raise (/lib64/libc.so.6+0x4acbb)
 #5 0x00007f0bee16a355 abort (/lib64/libc.so.6+0x4c355)
 #6 0x00007f0bee160cba __assert_fail_base (/lib64/libc.so.6+0x42cba)
 #7 0x00007f0bee160d42 (/lib64/libc.so.6+0x42d42)
 #8 0x000000000442e241 clang::Sema::ActOnOpenMPForDirective(llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation, llvm::SmallDenseMap<clang::ValueDecl const*, clang::Expr const*, 4u, llvm::DenseMapInfo<clang::ValueDecl const*, void>, llvm::detail::DenseMapPair<clang::ValueDecl const*, clang::Expr const*>>&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Sema/SemaOpenMP.cpp:10337:3
 #9 0x0000000004420d41 clang::Sema::ActOnOpenMPExecutableDirective(llvm::omp::Directive, clang::DeclarationNameInfo const&, llvm::omp::Directive, llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Sema/SemaOpenMP.cpp:0:11
#10 0x0000000003e59d79 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseOpenMP.cpp:2970:25
#11 0x0000000003e7d30f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseStmt.cpp:475:12
#12 0x0000000003e7b8c3 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseStmt.cpp:117:20
#13 0x0000000003e85aa1 clang::Parser::ParseCompoundStatementBody(bool) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseStmt.cpp:1205:11
#14 0x0000000003e86e75 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseStmt.cpp:2467:21
#15 0x0000000003db71e5 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#16 0x0000000003dd14e4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseDecl.cpp:2197:18
#17 0x0000000003db5d6b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/Parser.cpp:0:10
#18 0x0000000003db5600 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/Parser.cpp:1225:12
#19 0x0000000003db4521 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/Parser.cpp:0:14
#20 0x0000000003db1ecf clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/Parser.cpp:0:12
#21 0x0000000003dac8ce clang::ParseAST(clang::Sema&, bool, bool) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Parse/ParseAST.cpp:162:5
#22 0x0000000002988971 clang::FrontendAction::Execute() /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1062:10
#23 0x00000000029022ef llvm::Error::getPtr() const /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/llvm/include/llvm/Support/Error.h:272:42
#24 0x00000000029022ef llvm::Error::operator bool() /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/llvm/include/llvm/Support/Error.h:235:16
#25 0x00000000029022ef clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1049:23
#26 0x0000000002a49c82 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:264:25
#27 0x0000000000ad729d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/tools/driver/cc1_main.cpp:249:15
#28 0x0000000000ad30ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/tools/driver/driver.cpp:366:12
#29 0x0000000000ad2267 clang_main(int, char**, llvm::ToolContext const&) /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/llvm-project/clang/tools/driver/driver.cpp:407:12
#30 0x0000000000ae1c91 main /lustre/orion/csc372/scratch/cabarcasf/ums012/llvm17_git/build_frontier/tools/clang/tools/driver/clang-driver.cpp:15:3
#31 0x00007f0bee15329d __libc_start_main (/lib64/libc.so.6+0x3529d)
#32 0x0000000000acf00a _start /home/abuild/rpmbuild/BUILD/glibc-2.31/csu/../sysdeps/x86_64/start.S:122:0
clang: error: unable to execute command: Aborted
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 17.0.0 (https://github.com/llvm/llvm-project.git ae54f01dd8c53d18c276420b23f0d0ab7afefff1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-openmp

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

  1) Assign the issue to you.
  2) Fix the issue locally.
  3) [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally.
    3.1) Remember that the subdirectories under `test/` create fine-grained testing targets, so you can
         e.g. use `make check-clang-ast` to only run Clang's AST tests.
  4) Create a `git` commit
  5) Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
  6) Submit the patch to [Phabricator](https://reviews.llvm.org/).
    6.1) Detailed instructions can be found [here](https://llvm.org/docs/Phabricator.html#requesting-a-review-via-the-web-interface)

For more instructions on how to submit a patch to LLVM, see our [documentation](https://llvm.org/docs/Contributing.html).

If you have any further questions about this issue, don't hesitate to ask via a comment on this Github issue.

@llvm/issue-subscribers-good-first-issue

---

### Comment 4 - aikatz

Hi @fel-cab, I'd like to take a look at this. Could you please assign it to me? It looks like I don't have permissions to assign myself.

---

### Comment 5 - fel-cab

> Hi @fel-cab, I'd like to take a look at this. Could you please assign it to me? It looks like I don't have permissions to assign myself.

@EugeneZelenko assigned it to you before me. Thanks.

---

### Comment 6 - erichkeane

Unassigning as there is no progress on this for quite a while?  Anyone wants to work on this, it is open.

---

### Comment 7 - MillePlateaux

@erichkeane I can try to solve this problem, and I browsed the relevant code and got some initial ideas.

Before:
```cpp
StmtResult SemaOpenMP::ActOnOpenMPForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc, VarsWithInheritedDSAType &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();

  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopBasedDirective::HelperExprs B; 
}
```

We can add a statement to check if the parameter passed to the function is a constant integer to catch the error of illegal UNKNOWN parameters and prevent clang from crashing.

After:

```cpp
StmtResult SemaOpenMP::ActOnOpenMPForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc, VarsWithInheritedDSAType &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();

  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopBasedDirective::HelperExprs B;

 OMPClause * GetClause ; 
 for (OMPClause *Clause : Clauses) {
        if (auto * CollapseClause = dyn_cast<OMPCollapseClause>(Clause)) { 
           GetClause = CollapseClause ;
        }
 }
  Expr *CollapseExpr = getCollapseNumberExpr(GetClause)
  if (! (auto *ConstExpr = dyn_cast<IntegerLiteral>(CollapseExpr)))
 }
```
Can this issue be assigned to me?


---

