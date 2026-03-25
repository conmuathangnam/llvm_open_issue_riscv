# [Clang][OpenMP] Long / indefinite compile times on unexpected OpenMP clause

**Author:** Thyre
**URL:** https://github.com/llvm/llvm-project/issues/183613

## Body

Godbolt: https://godbolt.org/z/YjGnETc89

---

Some, as far as I know unimplemented, constructs from OpenMP 6.0 cause the following error to appear:

```
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
   15 | #pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
      |                                               ^
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
<source>:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
fatal error: too many errors emitted, stopping now [-ferror-limit=]
```

e.g. with the following source code, shortened from the OpenMP examples:

```c
// Taken from https://github.com/OpenMP/Examples/blob/3e4757ae2b52e51df6cd5d363d6fc1d894509719/data_environment/sources/induction.1.c

#include <stdio.h>

void comp_poly(int N, double x, double c[]) {
    // x:    input: value of x for which to eval the polynomial
    // c[N]: input: the coefficients
    double x0 = 1.0;       // initial value x^0 == 1
    double xi;             // x^i
    double result;         // accumulator for the result

    // Case 1: induction clause
    xi = x0;
    result = 0.0;
#pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
    for (int i = 0; i < N; i++) {
        result += c[i] * xi;
        xi *= x;
    }
    printf("C1: result = %f, xn = %f\n", result, xi);
} 
```

built with: `clang -c -fopenmp induction.1.c`.

----

While that is fine in itself, Clang will not terminate and has to be terminated manually.
A backtrace attached to the executing process yields:

```
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
0x000055555c509dc1 in llvm::omp::getOpenMPClauseKindAndVersions(llvm::StringRef) ()
(gdb) bt
#0  0x000055555c509dc1 in llvm::omp::getOpenMPClauseKindAndVersions(llvm::StringRef) ()
#1  0x000055555a9827bd in clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) ()
#2  0x000055555a98f17d in clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) ()
#3  0x000055555a9b14df in clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) ()
#4  0x000055555a9b2b2e in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) ()
#5  0x000055555a9ba537 in clang::Parser::ParseCompoundStatementBody(bool) ()
#6  0x000055555a9badbf in clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) ()
#7  0x000055555a8b44ef in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) ()
#8  0x000055555a8f859a in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) ()
#9  0x000055555a8add7c in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) ()
#10 0x000055555a8ae55f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) ()
#11 0x000055555a8b8a61 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) ()
#12 0x000055555a8b9a9f in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) ()
#13 0x000055555a896aca in clang::ParseAST(clang::Sema&, bool, bool) ()
#14 0x000055555902f4c9 in clang::FrontendAction::Execute() ()
#15 0x0000555558fabe25 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) ()
#16 0x0000555559129193 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) ()
#17 0x00005555563e2e30 in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) ()
#18 0x00005555563d939a in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) ()
#19 0x00005555563dd688 in clang_main(int, char**, llvm::ToolContext const&) ()
#20 0x00005555562ea141 in main ()
```

As far as I can tell, this only affects LLVM 22 and trunk. LLVM 21.1.8 simply ignores these directives and yields:

```
warning: extra tokens at the end of '#pragma omp parallel for' are ignored [-Wextra-tokens]
   15 | #pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
      |                                               ^
1 warning generated.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jan André Reuter (Thyre)

<details>
Godbolt: https://godbolt.org/z/YjGnETc89

---

Some, as far as I know unimplemented, constructs from OpenMP 6.0 cause the following error to appear:

```
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
   15 | #pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
      |                                               ^
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
&lt;source&gt;:15:47: error: unexpected OpenMP clause 'induction' in directive '#pragma omp parallel for'
fatal error: too many errors emitted, stopping now [-ferror-limit=]
```

e.g. with the following source code, shortened from the OpenMP examples:

```c
// Taken from https://github.com/OpenMP/Examples/blob/3e4757ae2b52e51df6cd5d363d6fc1d894509719/data_environment/sources/induction.1.c

#include &lt;stdio.h&gt;

void comp_poly(int N, double x, double c[]) {
    // x:    input: value of x for which to eval the polynomial
    // c[N]: input: the coefficients
    double x0 = 1.0;       // initial value x^0 == 1
    double xi;             // x^i
    double result;         // accumulator for the result

    // Case 1: induction clause
    xi = x0;
    result = 0.0;
#pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
    for (int i = 0; i &lt; N; i++) {
        result += c[i] * xi;
        xi *= x;
    }
    printf("C1: result = %f, xn = %f\n", result, xi);
} 
```

built with: `clang -c -fopenmp induction.1.c`.

----

While that is fine in itself, Clang will not terminate and has to be terminated manually.
A backtrace attached to the executing process yields:

```
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
0x000055555c509dc1 in llvm::omp::getOpenMPClauseKindAndVersions(llvm::StringRef) ()
(gdb) bt
#<!-- -->0  0x000055555c509dc1 in llvm::omp::getOpenMPClauseKindAndVersions(llvm::StringRef) ()
#<!-- -->1  0x000055555a9827bd in clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) ()
#<!-- -->2  0x000055555a98f17d in clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) ()
#<!-- -->3  0x000055555a9b14df in clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) ()
#<!-- -->4  0x000055555a9b2b2e in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) ()
#<!-- -->5  0x000055555a9ba537 in clang::Parser::ParseCompoundStatementBody(bool) ()
#<!-- -->6  0x000055555a9badbf in clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) ()
#<!-- -->7  0x000055555a8b44ef in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) ()
#<!-- -->8  0x000055555a8f859a in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) ()
#<!-- -->9  0x000055555a8add7c in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) ()
#<!-- -->10 0x000055555a8ae55f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) ()
#<!-- -->11 0x000055555a8b8a61 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) ()
#<!-- -->12 0x000055555a8b9a9f in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) ()
#<!-- -->13 0x000055555a896aca in clang::ParseAST(clang::Sema&amp;, bool, bool) ()
#<!-- -->14 0x000055555902f4c9 in clang::FrontendAction::Execute() ()
#<!-- -->15 0x0000555558fabe25 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) ()
#<!-- -->16 0x0000555559129193 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) ()
#<!-- -->17 0x00005555563e2e30 in cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) ()
#<!-- -->18 0x00005555563d939a in ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) ()
#<!-- -->19 0x00005555563dd688 in clang_main(int, char**, llvm::ToolContext const&amp;) ()
#<!-- -->20 0x00005555562ea141 in main ()
```

As far as I can tell, this only affects LLVM 22 and trunk. LLVM 21.1.8 simply ignores these directives and yields:

```
warning: extra tokens at the end of '#pragma omp parallel for' are ignored [-Wextra-tokens]
   15 | #pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
      |                                               ^
1 warning generated.
```
</details>


---

### Comment 2 - shafik

CC @zahiraam 

---

### Comment 3 - shafik

Is this fuzzer generated?

---

### Comment 4 - Thyre

> Is this fuzzer generated?

I don't know how the OpenMP ARB creates their OpenMP examples unfortunately. I reduced their example found [here](https://github.com/OpenMP/Examples/blob/3e4757ae2b52e51df6cd5d363d6fc1d894509719/data_environment/sources/induction.1.c) to only include a reduced part reproducing the issue I've found while using the OpenMP Examples as a testing ground for our performance tool work. 

---

### Comment 5 - Thyre

Given that the example is part of their [examples document](https://www.openmp.org/wp-content/uploads/openmp-examples-6-0-1.pdf) and explained there (OpenMP Examples v6.0.1, p.543), I think it's unlikely that this is generated by a fuzzer. 

---

### Comment 6 - zahiraam

Was able to reproduce the issue. Confirming that the test case was compiling into a warning with `clang.21.0`. Assigning the issue to myself.

---

### Comment 7 - zahiraam

Looks like the guilty commit is f530c161a7eeabc0c4b3f0f7c95900bff3be15c5. Tagging @Krzysztof for awareness.

---

