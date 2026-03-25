# [12 regression] error: unable to execute command: Segmentation fault (core dumped)

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/50498
**Status:** Closed
**Labels:** c, clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-03-11T01:33:57Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [51154](https://llvm.org/bz51154) |
| Version | 12.0 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@zygoloid |

## Extended Description 
```console
$ cat 10.c 
```
```cpp
struct S
{
  char data[16];
};

struct S sx;

int y;

char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];

char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
```
--------------------------------
```console
$ clang-sp11 --version
clang version 11.0.0 (https://github.com/llvm/llvm-project.git 0160ad802e899c2922bc9b29564080c22eb0908c)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
```
--------------------------------
```console
$ clang-sp11 10.c 
10.c:11:29: error: member reference base type 'int' is not a structure or union
char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];
                   ~~~~~~~~~^~~~~
10.c:13:24: error: assigning to 'struct S' from incompatible type 'int'
char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
                       ^ ~
2 errors generated.
```
--------------------------------
```console
$ clang-12 --version
clang version 12.0.0 (https://github.com/llvm/llvm-project/ b978a93635b584db380274d7c8963c73989944a1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
```
--------------------------------
```console
$ clang-12 10.c 
10.c:11:29: error: member reference base type 'int' is not a structure or union
char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];
                   ~~~~~~~~~^~~~~
10.c:13:24: error: assigning to 'struct S' from incompatible type 'int'
char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
                       ^ ~
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -disable-llvm-verifier -discard-value-names -main-file-name 10.c -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -tune-cpu generic -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/lib/clang/12.0.0 -internal-isystem /usr/local/include -internal-isystem /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/lib/clang/12.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdebug-compilation-dir /media/Store/Project/C-compiler/ZSmith/data/bug-llvm-20210721/bug_dir/proc.0/deal -ferror-limit 19 -fgnuc-version=4.2.1 -fcolor-diagnostics -faddrsig -o /tmp/10-54d374.o -x c 10.c
1.	10.c:13:70: current parser token ';'
#0 0x00000000024ab943 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24ab943)
#1 0x00000000024a989e llvm::sys::RunSignalHandlers() (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24a989e)
#2 0x00000000024abdef SignalHandler(int) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24abdef)
#3 0x00007f3017c7b3c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x153c0)
#4 0x00000000043fc561 clang::ASTContext::propertyTypesAreCompatible(clang::QualType, clang::QualType) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x43fc561)
#5 0x0000000008ef55e8 
clang-12: error: unable to execute command: Segmentation fault (core dumped)
clang-12: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 12.0.0 (https://github.com/llvm/llvm-project/ b978a93635b584db380274d7c8963c73989944a1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
clang-12: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-12: note: diagnostic msg: /tmp/10-053b13.c
clang-12: note: diagnostic msg: /tmp/10-053b13.sh
clang-12: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [51154](https://llvm.org/bz51154) |
| Version | 12.0 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
```console
$ cat 10.c 
```
```cpp
struct S
{
  char data[16];
};

struct S sx;

int y;

char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];

char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
```
--------------------------------
```console
$ clang-sp11 --version
clang version 11.0.0 (https://github.com/llvm/llvm-project.git 0160ad802e899c2922bc9b29564080c22eb0908c)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
```
--------------------------------
```console
$ clang-sp11 10.c 
10.c:11:29: error: member reference base type 'int' is not a structure or union
char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];
                   ~~~~~~~~~^~~~~
10.c:13:24: error: assigning to 'struct S' from incompatible type 'int'
char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
                       ^ ~
2 errors generated.
```
--------------------------------
```console
$ clang-12 --version
clang version 12.0.0 (https://github.com/llvm/llvm-project/ b978a93635b584db380274d7c8963c73989944a1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
```
--------------------------------
```console
$ clang-12 10.c 
10.c:11:29: error: member reference base type 'int' is not a structure or union
char z[((sizeof(0, ((sx, y)).data)) == (sizeof(char *))) ? (1) : (-1)];
                   ~~~~~~~~~^~~~~
10.c:13:24: error: assigning to 'struct S' from incompatible type 'int'
char z[((sizeof(0, (sx = y).data)) == (sizeof(char *))) ? (1) : (-1)];
                       ^ ~
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -disable-llvm-verifier -discard-value-names -main-file-name 10.c -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -tune-cpu generic -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/lib/clang/12.0.0 -internal-isystem /usr/local/include -internal-isystem /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/lib/clang/12.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdebug-compilation-dir /media/Store/Project/C-compiler/ZSmith/data/bug-llvm-20210721/bug_dir/proc.0/deal -ferror-limit 19 -fgnuc-version=4.2.1 -fcolor-diagnostics -faddrsig -o /tmp/10-54d374.o -x c 10.c
1.	10.c:13:70: current parser token ';'
#<!-- -->0 0x00000000024ab943 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24ab943)
#<!-- -->1 0x00000000024a989e llvm::sys::RunSignalHandlers() (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24a989e)
#<!-- -->2 0x00000000024abdef SignalHandler(int) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x24abdef)
#<!-- -->3 0x00007f3017c7b3c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x153c0)
#<!-- -->4 0x00000000043fc561 clang::ASTContext::propertyTypesAreCompatible(clang::QualType, clang::QualType) (/home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang-12+0x43fc561)
#<!-- -->5 0x0000000008ef55e8 
clang-12: error: unable to execute command: Segmentation fault (core dumped)
clang-12: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 12.0.0 (https://github.com/llvm/llvm-project/ b978a93635b584db380274d7c8963c73989944a1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/tom/Documents/llvm/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
clang-12: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-12: note: diagnostic msg: /tmp/10-053b13.c
clang-12: note: diagnostic msg: /tmp/10-053b13.sh
clang-12: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - Endilll

Appears to be fixed in Clang 17: https://godbolt.org/z/x6qhvjfes
Clang 16 crash:
```
Non-canonical and dependent types shouldn't get here
UNREACHABLE executed at /root/llvm-project/clang/lib/AST/ASTContext.cpp:10535!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-16.0.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-12.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:12:70: current parser token ';'
 #0 0x000061780a23332f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x412e32f)
 #1 0x000061780a2312cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x412c2cc)
 #2 0x000061780a17dfb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f57d2442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f57d24969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f57d2442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f57d24287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000061780a1897ce (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x40847ce)
 #8 0x000061780d71c4cb clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x76174cb)
 #9 0x000061780cbd8d8b clang::Sema::MergeVarDeclTypes(clang::VarDecl*, clang::VarDecl*, bool) (.part.0) SemaDecl.cpp:0:0
#10 0x000061780cbff90b clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x6afa90b)
#11 0x000061780cc00ae9 clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x6afbae9)
#12 0x000061780cc01982 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x6afc982)
#13 0x000061780cc1bfd4 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x6b16fd4)
#14 0x000061780cc1caa5 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x6b17aa5)
#15 0x000061780c8dd300 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67d8300)
#16 0x000061780c8eea28 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67e9a28)
#17 0x000061780c8b8e82 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67b3e82)
#18 0x000061780c8b972f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#19 0x000061780c8bee89 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67b9e89)
#20 0x000061780c8bfe2d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67bae2d)
#21 0x000061780c8b41ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x67af1ba)
#22 0x000061780b5b53c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x54b03c8)
#23 0x000061780ae20659 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4d1b659)
#24 0x000061780ada71ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4ca21ee)
#25 0x000061780af07a83 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4e02a83)
#26 0x000061780765e2a6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x15592a6)
#27 0x000061780765a397 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x000061780ac0f3b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x000061780a17e4a0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x40794a0)
#30 0x000061780ac0fc6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x000061780abd830c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4ad330c)
#32 0x000061780abd8d8d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4ad3d8d)
#33 0x000061780abe1f5c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x4adcf5c)
#34 0x000061780765c9f2 clang_main(int, char**) (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x15579f2)
#35 0x00007f57d2429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007f57d2429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x000061780765507e _start (/opt/compiler-explorer/clang-assertions-16.0.0/bin/clang+++0x155007e)
clang-16: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

