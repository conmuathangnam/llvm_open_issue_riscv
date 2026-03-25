# [ICE] Inline assembly with struct containing flexible array member as register operand

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169882
**Status:** Open
**Labels:** clang:codegen, crash, inline-asm

## Body

### Link for quick verification:  https://godbolt.org/z/7nv8socrc
**The compiler's internal assertion failed when trying to generate code for an inline assembly statement that claimed to have one output operand (via "=r"constraint) and Flexible array member as the only member of the struct.**
```c
#include <stdio.h>
struct S {
    char data[];
};

struct S foo(const char *a, const char *b, const char *c) {
    struct S y;
    __asm__ volatile ("mov %0, %%rax" : "=r"(y));  
    return y;
}
```

## The stack dump is:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 26639.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/26639-f15d82.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:17:1: current parser token 'class'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:9:10: LLVM IR generation of declaration 'foo'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:9:10: Generating code for declaration 'foo'
 #0 0x00005b5cb2f35c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005b5cb2f36457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005b5cb2f2ff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005b5cb2f301c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007901e7e1d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007901e7e76b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007901e7e76b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007901e7e76b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007901e7e1d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007901e7e008ff abort ./stdlib/abort.c:81:7
#10 0x00005b5cb2e2d6de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#11 0x00005b5cb23d7b57 llvm::InlineAsm::InlineAsm(llvm::FunctionType*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool, bool, llvm::InlineAsm::AsmDialect, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b33b57)
#12 0x00005b5cb23d8b16 llvm::ConstantUniqueMap<llvm::InlineAsm>::getOrCreate(llvm::PointerType*, llvm::InlineAsmKeyType) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b34b16)
#13 0x00005b5cb23d9e8b llvm::InlineAsm::get(llvm::FunctionType*, llvm::StringRef, llvm::StringRef, bool, bool, llvm::InlineAsm::AsmDialect, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b35e8b)
#14 0x00005b5cb36bde31 clang::CodeGen::CodeGenFunction::EmitAsmStmt(clang::AsmStmt const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e19e31)
#15 0x00005b5cb36c25a7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1e5a7)
#16 0x00005b5cb36cb01d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#17 0x00005b5cb37650ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#18 0x00005b5cb37853f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#19 0x00005b5cb382c5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#20 0x00005b5cb383207d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#21 0x00005b5cb3837278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#22 0x00005b5cb383836a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#23 0x00005b5cb3d51be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#24 0x00005b5cb3d3b475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#25 0x00005b5cb64159b2 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#26 0x00005b5cb415d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#27 0x00005b5cb3d4b360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#28 0x00005b5cb41659cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#29 0x00005b5cb40abf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#30 0x00005b5cb42edf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#31 0x00005b5cae05c523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#32 0x00005b5cae04cc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00005b5cae052b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#34 0x00005b5cadeadb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#35 0x00007901e7e021ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#36 0x00007901e7e0228b call_init ./csu/../csu/libc-start.c:128:20
#37 0x00007901e7e0228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#38 0x00005b5cae04af55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```

## The clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (AbyssStaror)

<details>
### Link for quick verification:  https://godbolt.org/z/7nv8socrc
**The compiler's internal assertion failed when trying to generate code for an inline assembly statement that claimed to have one output operand (via "=r"constraint) and Flexible array member as the only member of the struct.**
```c
#include &lt;stdio.h&gt;
struct S {
    char data[];
};

struct S foo(const char *a, const char *b, const char *c) {
    struct S y;
    __asm__ volatile ("mov %0, %%rax" : "=r"(y));  
    return y;
}
```

## The stack dump is:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 26639.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/26639-f15d82.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:17:1: current parser token 'class'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:9:10: LLVM IR generation of declaration 'foo'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26639.fuzz:9:10: Generating code for declaration 'foo'
 #<!-- -->0 0x00005b5cb2f35c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005b5cb2f36457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005b5cb2f2ff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005b5cb2f301c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007901e7e1d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007901e7e76b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007901e7e76b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007901e7e76b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007901e7e1d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007901e7e008ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005b5cb2e2d6de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#<!-- -->11 0x00005b5cb23d7b57 llvm::InlineAsm::InlineAsm(llvm::FunctionType*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt; const&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt; const&amp;, bool, bool, llvm::InlineAsm::AsmDialect, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b33b57)
#<!-- -->12 0x00005b5cb23d8b16 llvm::ConstantUniqueMap&lt;llvm::InlineAsm&gt;::getOrCreate(llvm::PointerType*, llvm::InlineAsmKeyType) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b34b16)
#<!-- -->13 0x00005b5cb23d9e8b llvm::InlineAsm::get(llvm::FunctionType*, llvm::StringRef, llvm::StringRef, bool, bool, llvm::InlineAsm::AsmDialect, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b35e8b)
#<!-- -->14 0x00005b5cb36bde31 clang::CodeGen::CodeGenFunction::EmitAsmStmt(clang::AsmStmt const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e19e31)
#<!-- -->15 0x00005b5cb36c25a7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1e5a7)
#<!-- -->16 0x00005b5cb36cb01d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#<!-- -->17 0x00005b5cb37650ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#<!-- -->18 0x00005b5cb37853f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#<!-- -->19 0x00005b5cb382c5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#<!-- -->20 0x00005b5cb383207d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#<!-- -->21 0x00005b5cb3837278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#<!-- -->22 0x00005b5cb383836a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#<!-- -->23 0x00005b5cb3d51be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->24 0x00005b5cb3d3b475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#<!-- -->25 0x00005b5cb64159b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#<!-- -->26 0x00005b5cb415d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->27 0x00005b5cb3d4b360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->28 0x00005b5cb41659cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->29 0x00005b5cb40abf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->30 0x00005b5cb42edf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->31 0x00005b5cae05c523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->32 0x00005b5cae04cc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x00005b5cae052b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->34 0x00005b5cadeadb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->35 0x00007901e7e021ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->36 0x00007901e7e0228b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->37 0x00007901e7e0228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->38 0x00005b5cae04af55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```

## The clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

