# clang-21 crashes when trying to compile

**Author:** DrDesten
**URL:** https://github.com/llvm/llvm-project/issues/174342
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

clang crashes when trying to compile my code.
I've attached the preprocessed source files.

Shell command:
```
clang-21 -std=c23 -Wall -Wextra -Wformat=0 -Wno-unused-parameter -Wno-unused-function -fsanitize=address -g ./lexer.c ./irparser.c ./globals.c ./main.c ./value.c -o tachyon
```

Stack dump:
```
0.      Program arguments: /usr/lib/llvm-21/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir tachyon- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name irparser.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/raketil/Documents/Programming/tachyon-c -fcoverage-compilation-dir=/home/raketil/Documents/Programming/tachyon-c -resource-dir /usr/lib/llvm-21/lib/clang/21 -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Wformat=0 -Wno-unused-parameter -Wno-unused-function -std=c23 -ferror-limit 19 -fmessage-length=170 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -fsanitize=address -fsanitize-system-ignorelist=/usr/lib/llvm-21/lib/clang/21/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/irparser-195cd9.o -x c ./irparser.c
1.      <eof> parser at end of file
2.      ./irparser.c:858:4: LLVM IR generation of declaration 'parse'
3.      ./irparser.c:858:4: Generating code for declaration 'parse'
 #0 0x00007fc4fd3f2c9a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f2c9a)
 #1 0x00007fc4fd3f0487 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f0487)
 #2 0x00007fc4fd3f3a64 (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f3a64)
 #3 0x00007fc4f8849df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #4 0x00007fc502a71d40 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1671d40)
 #5 0x00007fc502a72c92 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1672c92)
 #6 0x00007fc502a73d2d clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1673d2d)
 #7 0x00007fc503c26aff clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2826aff)
 #8 0x00007fc503be86e9 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27e86e9)
 #9 0x00007fc503be5ca1 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27e5ca1)
#10 0x00007fc503bdfd8b clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27dfd8b)
#11 0x00007fc503bdf9e2 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27df9e2)
#12 0x00007fc503d8fad0 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x298fad0)
#13 0x00007fc503d8f077 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x298f077)
#14 0x00007fc503d9aafc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x299aafc)
#15 0x00007fc503dfc84e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x29fc84e)
#16 0x00007fc503e1f6b8 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a1f6b8)
#17 0x00007fc503e18c9f clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a18c9f)
#18 0x00007fc503e1cb65 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a1cb65)
#19 0x00007fc503e178b5 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a178b5)
#20 0x00007fc503ec072c (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ac072c)
#21 0x00007fc503dec60d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x29ec60d)
#22 0x00007fc50298909a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x158909a)
#23 0x00007fc50459a26f clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x319a26f)
#24 0x00007fc50450bf74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x310bf74)
#25 0x00007fc504622cce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3222cce)
#26 0x0000559a70bde88d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x1388d)
#27 0x0000559a70bdb435 (/usr/lib/llvm-21/bin/clang+0x10435)
#28 0x0000559a70bda7c4 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-21/bin/clang+0xf7c4)
#29 0x0000559a70be995a main (/usr/lib/llvm-21/bin/clang+0x1e95a)
#30 0x00007fc4f8833ca8 (/lib/x86_64-linux-gnu/libc.so.6+0x29ca8)
#31 0x00007fc4f8833d65 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29d65)
#32 0x0000559a70bd9101 _start (/usr/lib/llvm-21/bin/clang+0xe101)
```

Diagnostic message:
```
Debian clang version 21.1.8 (++20251221033036+2078da43e25a-1~exp1~20251221153213.50)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-21: note: diagnostic msg: /tmp/lexer-d5038d.c
clang-21: note: diagnostic msg: /tmp/irparser-b1b69e.c
clang-21: note: diagnostic msg: /tmp/globals-9f618e.c
clang-21: note: diagnostic msg: /tmp/main-4158b4.c
clang-21: note: diagnostic msg: /tmp/value-903192.c
clang-21: note: diagnostic msg: /tmp/lexer-d5038d.sh
clang-21: note: diagnostic msg: 

********************
```

[lexer-d5038d.sh](https://github.com/user-attachments/files/24423517/lexer-d5038d.sh)
[value-903192.c](https://github.com/user-attachments/files/24423519/value-903192.c)
[main-4158b4.c](https://github.com/user-attachments/files/24423518/main-4158b4.c)
[lexer-d5038d.c](https://github.com/user-attachments/files/24423520/lexer-d5038d.c)
[irparser-b1b69e.c](https://github.com/user-attachments/files/24423521/irparser-b1b69e.c)
[globals-9f618e.c](https://github.com/user-attachments/files/24423522/globals-9f618e.c)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: DrDesten (DrDesten)

<details>
clang crashes when trying to compile my code.
I've attached the preprocessed source files.

Shell command:
```
clang-21 -std=c23 -Wall -Wextra -Wformat=0 -Wno-unused-parameter -Wno-unused-function -fsanitize=address -g ./lexer.c ./irparser.c ./globals.c ./main.c ./value.c -o tachyon
```

Stack dump:
```
0.      Program arguments: /usr/lib/llvm-21/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir tachyon- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name irparser.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/raketil/Documents/Programming/tachyon-c -fcoverage-compilation-dir=/home/raketil/Documents/Programming/tachyon-c -resource-dir /usr/lib/llvm-21/lib/clang/21 -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wextra -Wformat=0 -Wno-unused-parameter -Wno-unused-function -std=c23 -ferror-limit 19 -fmessage-length=170 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -fsanitize=address -fsanitize-system-ignorelist=/usr/lib/llvm-21/lib/clang/21/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/irparser-195cd9.o -x c ./irparser.c
1.      &lt;eof&gt; parser at end of file
2.      ./irparser.c:858:4: LLVM IR generation of declaration 'parse'
3.      ./irparser.c:858:4: Generating code for declaration 'parse'
 #<!-- -->0 0x00007fc4fd3f2c9a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f2c9a)
 #<!-- -->1 0x00007fc4fd3f0487 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f0487)
 #<!-- -->2 0x00007fc4fd3f3a64 (/lib/x86_64-linux-gnu/libLLVM.so.21.1+0x45f3a64)
 #<!-- -->3 0x00007fc4f8849df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->4 0x00007fc502a71d40 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1671d40)
 #<!-- -->5 0x00007fc502a72c92 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1672c92)
 #<!-- -->6 0x00007fc502a73d2d clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1673d2d)
 #<!-- -->7 0x00007fc503c26aff clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2826aff)
 #<!-- -->8 0x00007fc503be86e9 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27e86e9)
 #<!-- -->9 0x00007fc503be5ca1 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27e5ca1)
#<!-- -->10 0x00007fc503bdfd8b clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27dfd8b)
#<!-- -->11 0x00007fc503bdf9e2 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27df9e2)
#<!-- -->12 0x00007fc503d8fad0 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x298fad0)
#<!-- -->13 0x00007fc503d8f077 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x298f077)
#<!-- -->14 0x00007fc503d9aafc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x299aafc)
#<!-- -->15 0x00007fc503dfc84e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x29fc84e)
#<!-- -->16 0x00007fc503e1f6b8 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a1f6b8)
#<!-- -->17 0x00007fc503e18c9f clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a18c9f)
#<!-- -->18 0x00007fc503e1cb65 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a1cb65)
#<!-- -->19 0x00007fc503e178b5 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2a178b5)
#<!-- -->20 0x00007fc503ec072c (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ac072c)
#<!-- -->21 0x00007fc503dec60d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x29ec60d)
#<!-- -->22 0x00007fc50298909a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x158909a)
#<!-- -->23 0x00007fc50459a26f clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x319a26f)
#<!-- -->24 0x00007fc50450bf74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x310bf74)
#<!-- -->25 0x00007fc504622cce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3222cce)
#<!-- -->26 0x0000559a70bde88d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x1388d)
#<!-- -->27 0x0000559a70bdb435 (/usr/lib/llvm-21/bin/clang+0x10435)
#<!-- -->28 0x0000559a70bda7c4 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-21/bin/clang+0xf7c4)
#<!-- -->29 0x0000559a70be995a main (/usr/lib/llvm-21/bin/clang+0x1e95a)
#<!-- -->30 0x00007fc4f8833ca8 (/lib/x86_64-linux-gnu/libc.so.6+0x29ca8)
#<!-- -->31 0x00007fc4f8833d65 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29d65)
#<!-- -->32 0x0000559a70bd9101 _start (/usr/lib/llvm-21/bin/clang+0xe101)
```

Diagnostic message:
```
Debian clang version 21.1.8 (++20251221033036+2078da43e25a-1~exp1~20251221153213.50)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-21: note: diagnostic msg: /tmp/lexer-d5038d.c
clang-21: note: diagnostic msg: /tmp/irparser-b1b69e.c
clang-21: note: diagnostic msg: /tmp/globals-9f618e.c
clang-21: note: diagnostic msg: /tmp/main-4158b4.c
clang-21: note: diagnostic msg: /tmp/value-903192.c
clang-21: note: diagnostic msg: /tmp/lexer-d5038d.sh
clang-21: note: diagnostic msg: 

********************
```

[lexer-d5038d.sh](https://github.com/user-attachments/files/24423517/lexer-d5038d.sh)
[value-903192.c](https://github.com/user-attachments/files/24423519/value-903192.c)
[main-4158b4.c](https://github.com/user-attachments/files/24423518/main-4158b4.c)
[lexer-d5038d.c](https://github.com/user-attachments/files/24423520/lexer-d5038d.c)
[irparser-b1b69e.c](https://github.com/user-attachments/files/24423521/irparser-b1b69e.c)
[globals-9f618e.c](https://github.com/user-attachments/files/24423522/globals-9f618e.c)
</details>


---

### Comment 2 - DrDesten

Maybe this helps reducing the complexity:

iirc the crash doesn't happen anymore when using vec(Value) behind a typedef in here (irparser.c):

```c
typedef struct Proc {
    vec_t(Inst);
    vec(InstInfo) debug;
    u64           stackSize;
    vec(Value)    values;
    string*       name;
} Proc;

typedef struct {
    Proc       main;
    vec(Value) globals;
} IR;
```

vec() being defined as:
```c
#define vec(Type) \
    struct vec_##Type##_t { Type* data; i64 size; i64 cap; }
```

---

