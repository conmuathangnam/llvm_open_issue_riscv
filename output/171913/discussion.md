# LLDB DAP assertion failure

**Author:** sedymrak
**URL:** https://github.com/llvm/llvm-project/issues/171913
**Status:** Closed
**Labels:** lldb, crash
**Closed Date:** 2026-01-07T22:56:55Z

## Body

I am using VSCode IDE.
Until recently, I was using `codelldb` VSCode extension for debugging C++ code.
Now I am experimenting with "LLDB DAP" VSCode extension.
I have compiled `lldb` with debugging symbols so that I can debug it in VSCode (using the "LLDB DAP" VSCode extension).
I have noticed that whenever the control flow enters the `ValueObject::GetExpressionPath` method (defined in the `lldb/source/ValueObject/ValueObject.cpp` file), as soon as I start typing something in the "DEBUG  CONSOLE" (it does not matter what), the following error appears in the "DEBUG CONSOLE":
```
lldb-dap: /home/me/git/other/llvm-project/main/0/clang/lib/Sema/SemaAccess.cpp:750: AccessResult HasAccess(Sema &, const EffectiveContext &, const CXXRecordDecl *, AccessSpecifier, const AccessTarget &): Assertion `Access == AS_private || Access == AS_protected' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/me/git/other/llvm-project/main/0/build--main--Debug--mold--make/bin/lldb-dap
```
and the following message appears in the "LLDB-DAP" console (accessible via the combo-box in the "OUTPUT" pane:
```
2025-12-11 22:30:49.196 [error] Error: read error
	at Socket.<anonymous> (file:///usr/share/code/resources/app/out/vs/workbench/api/node/extensionHostProcess.js:132:1948)
	at Socket.emit (node:events:531:35)
	at Pipe.<anonymous> (node:net:346:12)
```
Afterwards, the "DEBUG CONSOLE" is unresponsive.

I have configured VSCode to use `lldb-dap` built from the `main` branch by adding the following entry:
```
"lldb-dap.executable-path": "/home/me/git/other/llvm-project/main/1/build--main--Release--ld--make/bin/lldb-dap",
```
to the corresponding `.vscode/settings.json` file.

I **suspect** that the bug is in the "LLDB DAP" VSCode extension and not in the `lldb-dap` program.
This suspicion is based on my experiment where I sent and received DAP messages directly to/from the `lldb-dap` program. I have instructed it to stop in the `ValueObject::GetExpressionPath` method and when the control flow reached that method, the `lldb-dap` remained responsive in the usual way.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb-dap

Author: Matej Košík (sedymrak)

<details>
I am using VSCode IDE.
Until recently, I was using `codelldb` VSCode extension for debugging C++ code.
Now I am experimenting with "LLDB DAP" VSCode extension.
I have compiled `lldb` with debugging symbols so that I can debug it in VSCode (using the "LLDB DAP" VSCode extension).
I have noticed that whenever the control flow enters the `ValueObject::GetExpressionPath` method (defined in the `lldb/source/ValueObject/ValueObject.cpp` file), as soon as I start typing something in the "DEBUG  CONSOLE" (it does not matter what), the following error appears in the "DEBUG CONSOLE":
```
lldb-dap: /home/me/git/other/llvm-project/main/0/clang/lib/Sema/SemaAccess.cpp:750: AccessResult HasAccess(Sema &amp;, const EffectiveContext &amp;, const CXXRecordDecl *, AccessSpecifier, const AccessTarget &amp;): Assertion `Access == AS_private || Access == AS_protected' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/me/git/other/llvm-project/main/0/build--main--Debug--mold--make/bin/lldb-dap
```
and the following message appears in the "LLDB-DAP" console (accessible via the combo-box in the "OUTPUT" pane:
```
2025-12-11 22:30:49.196 [error] Error: read error
	at Socket.&lt;anonymous&gt; (file:///usr/share/code/resources/app/out/vs/workbench/api/node/extensionHostProcess.js:132:1948)
	at Socket.emit (node:events:531:35)
	at Pipe.&lt;anonymous&gt; (node:net:346:12)
```
Afterwards, the "DEBUG CONSOLE" is unresponsive.

I have configured VSCode to use `lldb-dap` built from the `main` branch by adding the following entry:
```
"lldb-dap.executable-path": "/home/me/git/other/llvm-project/main/1/build--main--Release--ld--make/bin/lldb-dap",
```
to the corresponding `.vscode/settings.json` file.

I **suspect** that the bug is in the "LLDB DAP" VSCode extension and not in the `lldb-dap` program.
This suspicion is based on my experiment where I sent and received DAP messages directly to/from the `lldb-dap` program. I have instructed it to stop in the `ValueObject::GetExpressionPath` method and when the control flow reached that method, the `lldb-dap` remained responsive in the usual way.
</details>


---

### Comment 2 - ashgti

Does this also happen if you build and run `lldb` in the CLI and try to autocomplete a `e <variable>`?

---

### Comment 3 - sedymrak

If I have a following C program:
```
int main() {
  int foo[2][3][4];

  foo[0][0][0] = 1;
  foo[0][0][1] = 2;
  foo[0][0][2] = 3;
  foo[0][0][3] = 4;
  foo[0][1][0] = 11;
  foo[0][1][1] = 12;
  foo[0][1][2] = 13;
  foo[0][1][3] = 14;
  foo[0][2][0] = 21;
  foo[0][2][1] = 22;
  foo[0][2][2] = 23;
  foo[0][2][3] = 24;
  foo[1][0][0] = 101;
  foo[1][0][1] = 102;
  foo[1][0][2] = 103;
  foo[1][0][3] = 104;
  foo[1][1][0] = 111;
  foo[1][1][1] = 112;
  foo[1][1][2] = 113;
  foo[1][1][3] = 114;
  foo[1][2][0] = 121;
  foo[1][2][1] = 122;
  foo[1][2][2] = 123;
  foo[1][2][3] = 124;

  int (*bar)[3][4] = foo;
  int (*baz)[4] = bar[0];

  write(1, "DEBUG/0\n", 8);
  return 0;
}
```
the following `.lldbinit` file:
```
target create ./main
breakpoint set --file main.c --line 32
process launch
script lldb.frame.FindVariable("bar").GetChildAtIndex(0).get_expr_path()
```
and start  LLDB (built from the `main` branch in the following way:
```
~/git/other/llvm-project/main/0/install--main--Release--ld--make/bin/lldb --source .lldbinit
```
then I was not able to observe any problems with the autocompletion of variables that follow the `expression` LLDB command:
```(lldb) command source -s 0 '.lldbinit'
Executing commands in '/home/me/Documents/computer/software/compiler/llvm/github/pull-requests/2025-12-09-fix-ValueObject-GetExpressionPath-met
hod/.lldbinit'.
(lldb) target create ./main
Current executable set to '/home/me/Documents/computer/software/compiler/llvm/github/pull-requests/2025-12-09-fix-ValueObject-GetExpressionPath
-method/main' (x86_64).
(lldb) breakpoint set --file main.c --line 32
Breakpoint 1: where = main`main + 192 at main.c:32:3, address = 0x00000000000011f9
(lldb) process launch
Process 1550012 launched: '/home/me/Documents/computer/software/compiler/llvm/github/pull-requests/2025-12-09-fix-ValueObject-GetExpressionPath
-method/main' (x86_64)
Process 1550012 stopped
* thread #1, name = 'main', stop reason = breakpoint 1.1
    frame #0: 0x00005555555551f9 main`main at main.c:32:3
(lldb) script lldb.frame.FindVariable("bar").GetChildAtIndex(0).get_expr_path()
'bar[0][0]'
(lldb) e f
Available completions:
        foo   -- int (&)[2][3][4]
        float
(lldb) e foo 
(int[2][3][4]) $0 = {
  [0] = {
    [0] = ([0] = 1, [1] = 2, [2] = 3, [3] = 4)
    [1] = ([0] = 11, [1] = 12, [2] = 13, [3] = 14)
    [2] = ([0] = 21, [1] = 22, [2] = 23, [3] = 24)
  }
  [1] = {
    [0] = ([0] = 101, [1] = 102, [2] = 103, [3] = 104)
    [1] = ([0] = 111, [1] = 112, [2] = 113, [3] = 114)
    [2] = ([0] = 121, [1] = 122, [2] = 123, [3] = 124)
  }
}
(lldb) e b
Available completions:
        bar  -- int (*&)[3][4]
        baz  -- int (*&)[4]
        bool
(lldb) e b
Available completions:
        bar  -- int (*&)[3][4]
        baz  -- int (*&)[4]
        bool
(lldb) e ba
Available completions:
        bar -- int (*&)[3][4]
        baz -- int (*&)[4]
(lldb) e ba
Available completions:
        bar -- int (*&)[3][4]
        baz -- int (*&)[4]
(lldb) e bar
(int (*)[3][4]) $1 = 0x00007fffffffd800
```

---

### Comment 4 - sedymrak

That, however, is not surprising. I think that the unexpected behavior can only be reproduced when using the "LLDB DAP" VSCode plugin.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-lldb

Author: Matej Košík (sedymrak)

<details>
I am using VSCode IDE.
Until recently, I was using `codelldb` VSCode extension for debugging C++ code.
Now I am experimenting with "LLDB DAP" VSCode extension.
I have compiled `lldb` with debugging symbols so that I can debug it in VSCode (using the "LLDB DAP" VSCode extension).
I have noticed that whenever the control flow enters the `ValueObject::GetExpressionPath` method (defined in the `lldb/source/ValueObject/ValueObject.cpp` file), as soon as I start typing something in the "DEBUG  CONSOLE" (it does not matter what), the following error appears in the "DEBUG CONSOLE":
```
lldb-dap: /home/me/git/other/llvm-project/main/0/clang/lib/Sema/SemaAccess.cpp:750: AccessResult HasAccess(Sema &amp;, const EffectiveContext &amp;, const CXXRecordDecl *, AccessSpecifier, const AccessTarget &amp;): Assertion `Access == AS_private || Access == AS_protected' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/me/git/other/llvm-project/main/0/build--main--Debug--mold--make/bin/lldb-dap
```
and the following message appears in the "LLDB-DAP" console (accessible via the combo-box in the "OUTPUT" pane:
```
2025-12-11 22:30:49.196 [error] Error: read error
	at Socket.&lt;anonymous&gt; (file:///usr/share/code/resources/app/out/vs/workbench/api/node/extensionHostProcess.js:132:1948)
	at Socket.emit (node:events:531:35)
	at Pipe.&lt;anonymous&gt; (node:net:346:12)
```
Afterwards, the "DEBUG CONSOLE" is unresponsive.

I have configured VSCode to use `lldb-dap` built from the `main` branch by adding the following entry:
```
"lldb-dap.executable-path": "/home/me/git/other/llvm-project/main/1/build--main--Release--ld--make/bin/lldb-dap",
```
to the corresponding `.vscode/settings.json` file.

I **suspect** that the bug is in the "LLDB DAP" VSCode extension and not in the `lldb-dap` program.
This suspicion is based on my experiment where I sent and received DAP messages directly to/from the `lldb-dap` program. I have instructed it to stop in the `ValueObject::GetExpressionPath` method and when the control flow reached that method, the `lldb-dap` remained responsive in the usual way.
</details>


---

### Comment 6 - da-viper

it is reproducible when debugging lldb with lldb

It crashes here `Acess == AS_none`.
https://github.com/llvm/llvm-project/blob/44c0469e5f5e7ff214690140a1142d4f05d782ce/clang/lib/Sema/SemaAccess.cpp#L749-L751

```sh
# the lldb to debug
$ lldb --source .lldbinit
```

```sh
# lldb debugger
$ lldb --attach-pid <the_pid_of_lldb_to_debug>
(lldb) b ValueObject::GetExpressionPath
(lldb) process continue
```
```sh
# the lldb to debug
(lldb_debuggee) script lldb.frame.FindVariable("bar").GetChildAtIndex(0).get_expr_path()
```

```sh
# lldb debugger
(lldb) script
>>> inte = lldb.dbg.GetCommandInterpreter()
>>> matches = lldb.SBStringList()
>>> desc = lldb.SBStringList()
>>> inte.HandleCompletionWithDescriptions("expression -- t", 15, 0 ,100, matches, desc)
```

---

### Comment 7 - sedymrak

You are right. I was able to reproduce the problem in `lldb` proper as well:
```
# Terminal #1
$ ~/git/other/llvm-project/main/1/install--main--Debug--mold--make/bin/lldb
  (lldb) process attach --waitfor --name lldb

# Terminal #2
$ /home/me/git/other/llvm-project/main/1/install--main--Debug--mold--make/bin/lldb /home/me/Documents/computer/software/compiler/llvm/github/issues/171913-LLDB-DAP-assert-failure/c-program/main

# Terminal #1
  (lldb) breakpoint set --method ValueObject::GetExpressionPath
  (lldb) process continue

# Terminal #2

  (lldb) breakpoint set --name main
  (lldb) process launch

    Process 1956501 launched: '/home/me/Documents/computer/software/compiler/llvm/github/issues/171913-LLDB-DAP-assert-failure/c-program/main' (x86
    _64)
    Process 1956501 stopped
    * thread #1, name = 'main', stop reason = breakpoint 1.1
        frame #0: 0x0000555555555142 main`main(argc=1, argv=0x00007fffffffd9f8) at main.c:2:3
       1   	int main(int argc, char** argv) {
    -> 2   	  return 0;
       3   	}

  (lldb) script lldb.frame.FindVariable("argc").get_expr_path()

# Terminal #1

  (lldb) process continue
  (lldb) process continue

# Terminal #2

  (lldb) script lldb.frame.FindVariable("argc").get_expr_path()

# Terminal #1
  # "lldb" process now stops in the "ValueObject::GetExpressionPath" method.
  (lldb) script
    >>> inte = lldb.debugger.GetCommandInterpreter()
    >>> matches = lldb.SBStringList()
    >>> desc = lldb.SBStringList()
    >>> inte.HandleCompletionWithDescriptions("expression -- t", 15, 0 ,100, matches, desc)

lldb: /home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaAccess.cpp:750: AccessResult HasAccess(Sema &, const EffectiveContext &, const 
CXXRecordDecl *, AccessSpecifier, const AccessTarget &): Assertion `Access == AS_private || Access == AS_protected' failed.
LLDB diagnostics will be written to /tmp/diagnostics-6cd57b
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/me/git/other/llvm-project/main/1/install--main--Debug--mold--make/bin/lldb
 #0 0x000055d33f609559 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/me/git/other/llvm-project/main/1/llvm/lib/Support/Unix/Signals
.inc:834:11
 #1 0x000055d33f609a7b PrintStackTraceSignalHandler(void*) /home/me/git/other/llvm-project/main/1/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055d33f60798f llvm::sys::RunSignalHandlers() /home/me/git/other/llvm-project/main/1/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055d33f60a139 SignalHandler(int, siginfo_t*, void*) /home/me/git/other/llvm-project/main/1/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fa713e3a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #5 0x00007fa713eff819 syscall ./misc/../sysdeps/unix/sysv/linux/x86_64/syscall.S:38:0
 #6 0x00007fa71e5bb3ec SignalHandler(int, siginfo_t*, void*) /home/me/git/other/llvm-project/main/1/llvm/lib/Support/Unix/Signals.inc:425:7
 #7 0x00007fa713e3a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #8 0x00007fa713e88eec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #9 0x00007fa713e39fb2 raise ./signal/../sysdeps/posix/raise.c:27:6
#10 0x00007fa713e24472 abort ./stdlib/abort.c:81:7
#11 0x00007fa713e24395 _nl_load_domain ./intl/loadmsgcat.c:1177:9
#12 0x00007fa713e32ec2 (/lib/x86_64-linux-gnu/libc.so.6+0x34ec2)
#13 0x00007fa724b18d82 HasAccess(clang::Sema&, (anonymous namespace)::EffectiveContext const&, clang::CXXRecordDecl const*, clang::AccessSpecif
ier, (anonymous namespace)::AccessTarget const&) /home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaAccess.cpp:752:16
#14 0x00007fa724b17f0c IsAccessible(clang::Sema&, (anonymous namespace)::EffectiveContext const&, (anonymous namespace)::AccessTarget&) /home/m
e/git/other/llvm-project/main/1/clang/lib/Sema/SemaAccess.cpp:1372:5
#15 0x00007fa724b17bda clang::Sema::IsSimplyAccessible(clang::NamedDecl*, clang::CXXRecordDecl*, clang::QualType) /home/me/git/other/llvm-proje
ct/main/1/clang/lib/Sema/SemaAccess.cpp:1936:46
#16 0x00007fa724c1e421 (anonymous namespace)::CodeCompletionDeclConsumer::IsAccessible(clang::NamedDecl*, clang::DeclContext*) /home/me/git/oth
er/llvm-project/main/1/clang/lib/Sema/SemaCodeComplete.cpp:1819:5
#17 0x00007fa724c1e1fc (anonymous namespace)::CodeCompletionDeclConsumer::FoundDecl(clang::NamedDecl*, clang::NamedDecl*, clang::DeclContext*, 
bool) /home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaCodeComplete.cpp:1785:34
#18 0x00007fa7252212c1 (anonymous namespace)::LookupVisibleHelper::lookupInDeclContext(clang::DeclContext*, clang::LookupResult&, bool, bool) /
home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaLookup.cpp:4217:18
#19 0x00007fa7252207a8 (anonymous namespace)::LookupVisibleHelper::lookupInScope(clang::Scope*, clang::LookupResult&, (anonymous namespace)::Un
qualUsingDirectiveSet&) /home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaLookup.cpp:4375:18
#20 0x00007fa725211561 (anonymous namespace)::LookupVisibleHelper::lookupVisibleDecls(clang::Sema&, clang::Scope*, clang::Sema::LookupNameKind,
 bool) /home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaLookup.cpp:4137:3
#21 0x00007fa725211320 clang::Sema::LookupVisibleDecls(clang::Scope*, clang::Sema::LookupNameKind, clang::VisibleDeclConsumer&, bool, bool) /ho
me/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaLookup.cpp:4446:1
#22 0x00007fa724bf6b91 clang::SemaCodeCompletion::CodeCompleteOrdinaryName(clang::Scope*, clang::SemaCodeCompletion::ParserCompletionContext) /
home/me/git/other/llvm-project/main/1/clang/lib/Sema/SemaCodeComplete.cpp:4693:11
#23 0x00007fa72468ff25 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedSt
mtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) /home/me/git/other/llvm-project/main/
1/clang/lib/Parse/ParseStmt.cpp:158:12
#24 0x00007fa72468fbf7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clan
g::SourceLocation*, clang::LabelDecl*) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/ParseStmt.cpp:77:20
#25 0x00007fa724699136 clang::Parser::ParseCompoundStatementBody(bool) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/ParseStmt.cpp:116
2:11
#26 0x00007fa72469a602 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /home/me/git/other/llvm-project/main
/1/clang/lib/Parse/ParseStmt.cpp:2430:21
#27 0x00007fa7246ba761 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parse
r::LateParsedAttrList*) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/Parser.cpp:1443:3
#28 0x00007fa7245a34bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser
::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/ParseDecl.c
pp:2265:21
#29 0x00007fa7246b96ed clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec
&, clang::AccessSpecifier) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/Parser.cpp:1182:10
#30 0x00007fa7246b8bbb clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDe
clSpec*, clang::AccessSpecifier) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/Parser.cpp:1204:12
#31 0x00007fa7246b84b4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /ho
me/me/git/other/llvm-project/main/1/clang/lib/Parse/Parser.cpp:1032:14
#32 0x00007fa7246b6334 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/me/git/o
ther/llvm-project/main/1/clang/lib/Parse/Parser.cpp:745:12
#33 0x00007fa72458988c clang::ParseAST(clang::Sema&, bool, bool) /home/me/git/other/llvm-project/main/1/clang/lib/Parse/ParseAST.cpp:170:16
#34 0x00007fa72152eeb4 lldb_private::ClangExpressionParser::ParseInternal(lldb_private::DiagnosticManager&, clang::CodeCompleteConsumer*, unsig
ned int, unsigned int) /home/me/git/other/llvm-project/main/1/lldb/source/Plugins/ExpressionParser/Clang/ClangExpressionParser.cpp:1333:3
#35 0x00007fa72152e115 lldb_private::ClangExpressionParser::Complete(lldb_private::CompletionRequest&, unsigned int, unsigned int, unsigned int
) /home/me/git/other/llvm-project/main/1/lldb/source/Plugins/ExpressionParser/Clang/ClangExpressionParser.cpp:1182:21
#36 0x00007fa721564c89 lldb_private::ClangUserExpression::Complete(lldb_private::ExecutionContext&, lldb_private::CompletionRequest&, unsigned 
int) /home/me/git/other/llvm-project/main/1/lldb/source/Plugins/ExpressionParser/Clang/ClangUserExpression.cpp:837:3
#37 0x00007fa723f39721 lldb_private::CommandObjectExpression::HandleCompletion(lldb_private::CompletionRequest&) /home/me/git/other/llvm-projec
t/main/1/lldb/source/Commands/CommandObjectExpression.cpp:389:1
#38 0x00007fa71d9366c4 lldb_private::CommandInterpreter::HandleCompletionMatches(lldb_private::CompletionRequest&) /home/me/git/other/llvm-proj
ect/main/1/lldb/source/Interpreter/CommandInterpreter.cpp:2276:3
#39 0x00007fa71d9367ed lldb_private::CommandInterpreter::HandleCompletion(lldb_private::CompletionRequest&) /home/me/git/other/llvm-project/mai
n/1/lldb/source/Interpreter/CommandInterpreter.cpp:2297:1
#40 0x00007fa71d3ea789 lldb::SBCommandInterpreter::HandleCompletionWithDescriptions(char const*, char const*, char const*, int, int, lldb::SBSt
ringList&, lldb::SBStringList&) /home/me/git/other/llvm-project/main/1/lldb/source/API/SBCommandInterpreter.cpp:275:10
#41 0x00007fa71d3eab61 lldb::SBCommandInterpreter::HandleCompletionWithDescriptions(char const*, unsigned int, int, int, lldb::SBStringList&, l
ldb::SBStringList&) /home/me/git/other/llvm-project/main/1/lldb/source/API/SBCommandInterpreter.cpp:331:10
#42 0x00007fa71d548a14 _wrap_SBCommandInterpreter_HandleCompletionWithDescriptions(_object*, _object*) /home/me/git/other/llvm-project/main/1/b
uild--main--Debug--mold--make/tools/lldb/bindings/python/LLDBWrapPython.cpp:16743:12
#43 0x00007fa7137c9bc1 (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x1c9bc1)
#44 0x00007fa71377be8d _PyObject_MakeTpCall (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x17be8d)
#45 0x00007fa713709c63 _PyEval_EvalFrameDefault (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x109c63)
#46 0x00007fa7139cce8c (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x3cce8c)
#47 0x00007fa7138688b1 PyEval_EvalCode (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x2688b1)
#48 0x00007fa713868fd0 (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x268fd0)
#49 0x00007fa71370ca25 _PyEval_EvalFrameDefault (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x10ca25)
#50 0x00007fa7139cce8c (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x3cce8c)
#51 0x00007fa7138688b1 PyEval_EvalCode (/lib/x86_64-linux-gnu/libpython3.11.so.1.0+0x2688b1)
#52 0x00007fa71e1d9ee2 lldb_private::python::RunString(char const*, int, _object*, _object*) /home/me/git/other/llvm-project/main/1/lldb/source
/Plugins/ScriptInterpreter/Python/PythonDataObjects.cpp:1470:13
#53 0x00007fa71e1da30f lldb_private::python::RunSimpleString(char const*) /home/me/git/other/llvm-project/main/1/lldb/source/Plugins/ScriptInte
rpreter/Python/PythonDataObjects.cpp:1487:13
#54 0x00007fa71e1f1a1c lldb_private::IOHandlerPythonInterpreter::Run() /home/me/git/other/llvm-project/main/1/lldb/source/Plugins/ScriptInterpr
eter/Python/Interfaces/../ScriptInterpreterPythonImpl.h:484:7
#55 0x00007fa71d6bedba lldb_private::Debugger::RunIOHandlers() /home/me/git/other/llvm-project/main/1/lldb/source/Core/Debugger.cpp:1277:16
#56 0x00007fa71d93b62c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&) /home/me/git/other/
llvm-project/main/1/lldb/source/Interpreter/CommandInterpreter.cpp:3603:16
#57 0x00007fa71d412701 lldb::SBDebugger::RunCommandInterpreter(bool, bool) /home/me/git/other/llvm-project/main/1/lldb/source/API/SBDebugger.cp
p:1188:42
#58 0x000055d33f4a9032 Driver::MainLoop() /home/me/git/other/llvm-project/main/1/lldb/tools/driver/Driver.cpp:720:3
#59 0x000055d33f4a99b2 main /home/me/git/other/llvm-project/main/1/lldb/tools/driver/Driver.cpp:930:17
#60 0x00007fa713e2524a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#61 0x00007fa713e25305 call_init ./csu/../csu/libc-start.c:128:20
#62 0x00007fa713e25305 __libc_start_main ./csu/../csu/libc-start.c:347:5
#63 0x000055d33f4a6501 _start (/home/me/git/other/llvm-project/main/1/install--main--Debug--mold--make/bin/lldb+0x157501)
Aborted
```

---

### Comment 8 - Michael137

```
struct Foo {
  enum SomeEnum { First };

  void bar(SomeEnum e = {}) {}
};

int main() { Foo{}.bar(); }
```

```
./bin/lldb access-enum.out -o "b bar" -o run -o 'script -- lldb.debugger.GetCommandInterpreter().HandleCompletion("expression -- x", 15, 0, -1, lldb.SBStringList())'
```

The gist of this is that we're going down a clang codepath which checks the access specifier for the enum decl. This is specifically only hit for code-completion. But LLDB doesn't set access specifiers on this enum decl...we probably should do that. Though I think we should just allow unconditional access, instead of relying on setting the access specifier manually

---

