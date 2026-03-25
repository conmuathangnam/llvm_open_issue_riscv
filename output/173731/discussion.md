# [clang-fuzzer] Crash in llvm::APSInt

**Author:** zczc66
**URL:** https://github.com/llvm/llvm-project/issues/173731

## Body

Hi, while testing clang by AFL++, it found a crashing case:
version : llvmorg-21.1.8

PoC:
```
void fn_40_b(const char *(*a4(int *__cdecl f2()))[(__builtin_expect(
    -((sizeof(bool[1 / (((sizeof(__builtin_printf("Setup")) % 2)))]))), 0))]

) {}
```
Reduced and modified from the original:
https://godbolt.org/z/qMMG71Pj8

 backtrace:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:20808: llvm::APSInt clang::Expr::EvaluateKnownConstInt(const clang::ASTContext&) const: Assertion `Result && "Could not evaluate expression"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'fn_40_b'
3.	<source>:1:6: Generating code for declaration 'fn_40_b'
4.	<source>:1:6: Mangling declaration 'fn_40_b'
 #0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000703a8f842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000703a8f8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000703a8f842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000703a8f8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000703a8f82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000703a8f839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007d164a9 clang::Expr::EvaluateKnownConstInt(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d164a9)
#10 0x0000000007fe745e (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#11 0x0000000007fe6b32 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#12 0x0000000007fe82fc (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#13 0x0000000007fdd83b (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#14 0x0000000007fdd865 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#15 0x0000000007feb036 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#16 0x0000000007febb35 (anonymous namespace)::CXXNameMangler::mangleType(clang::FunctionProtoType const*) ItaniumMangle.cpp:0:0
#17 0x0000000007fde0d0 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#18 0x0000000007fdd865 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#19 0x0000000007feb0de (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#20 0x0000000007fec705 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#21 0x0000000007fed817 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#22 0x0000000007fefeb1 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#23 0x000000000801573c clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x801573c)
#24 0x00000000048949e5 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#25 0x00000000048ac02d clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ac02d)
#26 0x00000000048e8ce5 clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e8ce5)
#27 0x00000000048e9261 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e9261)
#28 0x00000000048e3fe4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e3fe4)
#29 0x00000000048e47c3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e47c3)
#30 0x00000000048efd53 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#31 0x0000000004c53569 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#32 0x0000000004c44094 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c44094)
#33 0x0000000006a0c304 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c304)
#34 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#35 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#36 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#37 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#38 0x0000000000de5c4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#39 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000ddc81d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004cc3049 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#43 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#45 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#46 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#47 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#48 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#49 0x0000703a8f829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x0000703a8f829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (zczc66)

<details>
Hi, while testing clang by AFL++, it found a crashing case:
version : llvmorg-21.1.8

Flags:
```
export LLVM_CC_NAME=/home/user/hlpfuzz_aflpp/afl-clang-fast LLVM_CXX_NAME=/home/user/hlpfuzz_aflpp/afl-clang-fast++ CC=gclang CXX=gclang++
cmake -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_SANITIZE_COVERAGE=On -DLLVM_BUILD_RUNTIME=Off -G "Unix Makefiles" ../llvm
make clang-fuzzer
```

PoC:
```
void fn_40_b ( const char * ( * a4 ( int * __cdecl f2 ( int * state_t [ 4 ] [ 4 ] , char * argv [ ] ) , ... ) ) [ 1 / ( ( 1 + ( __FILE__ [ 10 ] ) &gt;&gt; ( 0x52 &gt;&gt; ( ( ( ( ( __builtin_FILE ( ) [ 0 ] ) &amp; ( ( 0x52 &gt;&gt; 0x5b ) &amp; ( ( __builtin_expect ( - ( __builtin_expect ( - ( ( sizeof ( * "%f%f%f" ) ) ) , 0 ) ) - ( ( ( ( ( 1 + - ( sizeof ( 0x1c ) ) ) + ( 0.0 ) ) * ( ( ( ( sizeof ( bool [ 1 / ( ( ( sizeof ( __builtin_printf ( "Setup" , 0 , 0 ) / ( 1 + ( ( bool ( 1 ) ) ) ) ) % 2 ) ) ) ] ) ) * 0x1b ) ) * 0xc6 ) ) * 4 ) * 4 ) , 0 ) ) ) ) / ( 1 + ( 1 / ( ( 0 ) ) ) ) ) * 4 ) * 0x1b ) ) ) ) ) ] , const char ( * f2 ) ( double * a , double * b , void * c ) , int thousands_len ) { struct tree_el { struct test44_d { struct tree_el { struct list_el { int state_t [ 0x11 ] [ 4 ] ; int ( n7 ) [ 4 ] [ 10000 ] ; } val ; struct fn_12 * right , * left ; } Rcon [ ! ( 0x85 &gt;= ( 0xaa ) ) ] ; int se , foo [ 4 ] ; } \u00b5_var ; int fn_9 ; } cnt ; ! ( * "%f%f%f" ) ; }
```

Reproduction(Since make with ASan causes errors, I use gdb.):
```
gdb -q --batch \
    -x gdb_bt.cmd \
    --args /home/user/repo/llvm-project/gllvm_build/bin/clang-fuzzer poc
```

gdb_bt.cmd:
```
set pagination off 
set confirm off
set print thread-events off
handle SIGSTOP nostop noprint pass
handle SIGUSR1 nostop noprint pass
run
bt
quit
```

Crashing thread backtrace:
```
Running LLVMFuzzerInitialize ...
continue...

Program received signal SIGSEGV, Segmentation fault.
__memmove_avx_unaligned () at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:222
222     ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S: No such file or directory.
#<!-- -->0  __memmove_avx_unaligned () at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:222
#<!-- -->1  0x000055555a0ab7b9 in llvm::APInt::APInt (this=0x7fffffff7e50, that=...) at /home/user/repo/llvm-project/llvm/include/llvm/ADT/APInt.h:160
#<!-- -->2  llvm::APSInt::APSInt (this=0x7fffffff7e50) at /home/user/repo/llvm-project/llvm/include/llvm/ADT/APSInt.h:23
#<!-- -->3  clang::Expr::EvaluateKnownConstInt (this=0x55555e37a9b0, Ctx=..., Diag=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/AST/ExprConstant.cpp:16278
#<!-- -->4  0x000055555a40a7db in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37a9b0, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5134
#<!-- -->5  0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37aa28, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->6  0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37aac0, Arity=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->7  0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37ab18, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->8  0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37ab90, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->9  0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37ac08, Arity=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->10 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37ac60, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->11 0x000055555a408a14 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37acb8, Arity=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4916
#<!-- -->12 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37ad70, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->13 0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37bf18, Arity=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->14 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37bf68, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->15 0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37bfe0, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->16 0x000055555a40967f in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37c058, Arity=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->17 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37c0e0, Arity=&lt;optimized out&gt;, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->18 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=0x7fffffff9d00, E=0x55555e37c120, Arity=&lt;optimized out&gt;, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->19 0x000055555a409693 in (anonymous namespace)::CXXNameMangler::mangleExpression (this=this@<!-- -->entry=0x7fffffff9d00, E=0x55555e37c180, Arity=Arity@<!-- -->entry=4294967295, AsTemplateArg=false) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5307
#<!-- -->20 0x000055555a3f7990 in (anonymous namespace)::CXXNameMangler::mangleType (this=this@<!-- -->entry=0x7fffffff9d00, T=T@<!-- -->entry=0x55555e37c1c0) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3679
#<!-- -->21 0x000055555a3f5d08 in (anonymous namespace)::CXXNameMangler::mangleType (this=0x7fffffff9d00, T=...) at /home/user/repo/llvm-project/gllvm_build/tools/clang/include/clang/AST/TypeNodes.inc:29
#<!-- -->22 0x000055555a3f607e in (anonymous namespace)::CXXNameMangler::mangleType (this=this@<!-- -->entry=0x7fffffff9d00, T=...) at /home/user/repo/llvm-project/gllvm_build/tools/clang/include/clang/AST/TypeNodes.inc:67
#<!-- -->23 0x000055555a41c4f3 in (anonymous namespace)::CXXNameMangler::mangleBareFunctionType (this=this@<!-- -->entry=0x7fffffff9d00, Proto=Proto@<!-- -->entry=0x55555e37c290, MangleReturnType=&lt;optimized out&gt;, FD=FD@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3604
#<!-- -->24 0x000055555a401a3b in (anonymous namespace)::CXXNameMangler::mangleType (this=this@<!-- -->entry=0x7fffffff9d00, T=T@<!-- -->entry=0x55555e37c290) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3558
#<!-- -->25 0x000055555a3f5f1f in (anonymous namespace)::CXXNameMangler::mangleType (this=0x7fffffff9d00, T=...) at /home/user/repo/llvm-project/gllvm_build/tools/clang/include/clang/AST/TypeNodes.inc:52
#<!-- -->26 0x000055555a3f607e in (anonymous namespace)::CXXNameMangler::mangleType (this=this@<!-- -->entry=0x7fffffff9d00, T=...) at /home/user/repo/llvm-project/gllvm_build/tools/clang/include/clang/AST/TypeNodes.inc:67
#<!-- -->27 0x000055555a41caae in (anonymous namespace)::CXXNameMangler::mangleBareFunctionType (this=0x7fffffff9d00, Proto=0x55555e37c870, MangleReturnType=&lt;optimized out&gt;, FD=0x55555e37c8f0) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3622
#<!-- -->28 0x000055555a3f2635 in (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding (this=0x7fffffff9d00, GD=...) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855
#<!-- -->29 0x000055555a3ed5ec in (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName (this=0x55555e3115b0, GD=..., Out=...) at /home/user/repo/llvm-project/clang/lib/AST/ItaniumMangle.cpp:7039
#<!-- -->30 0x0000555555f6e8b3 in getMangledNameImpl[abi:cxx11](clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) (CGM=..., GD=..., ND=ND@<!-- -->entry=0x55555e37c8f0, OmitMultiVersionMangling=212) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1845
#<!-- -->31 0x0000555555f607eb in clang::CodeGen::CodeGenModule::getMangledName (this=this@<!-- -->entry=0x55555e3105d0, GD=...) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:1998
#<!-- -->32 0x0000555555f901cc in clang::CodeGen::CodeGenModule::GetAddrOfFunction (this=this@<!-- -->entry=0x55555e3105d0, GD=..., Ty=0x55555e312fd8, ForVTable=&lt;optimized out&gt;, DontDefer=true, IsForDefinition=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4782
#<!-- -->33 0x0000555555f8c658 in clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition (this=0x7fffe8f53010, this@<!-- -->entry=0x55555e3105d0, GD=..., GV=GV@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:5971
#<!-- -->34 0x0000555555f7e68d in clang::CodeGen::CodeGenModule::EmitGlobalDefinition (this=0x7fffe8f53010, this@<!-- -->entry=0x55555e3105d0, GD=..., GV=0x7ffff7b58a97 &lt;__GI___mmap64+23&gt;, GV@<!-- -->entry=0x0) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4151
#<!-- -->35 0x0000555555f85673 in clang::CodeGen::CodeGenModule::EmitGlobal (this=this@<!-- -->entry=0x55555e3105d0, GD=...) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:3862
#<!-- -->36 0x0000555555f7c976 in clang::CodeGen::CodeGenModule::EmitTopLevelDecl (this=0x55555e3105d0, D=0x55555e37c8f0) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6878
#<!-- -->37 0x0000555555b0db41 in (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl (this=0x55555e310410, DG=...) at /home/user/repo/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:190
#<!-- -->38 0x0000555555ad087d in clang::BackendConsumer::HandleTopLevelDecl (this=0x55555e2c9e00, D=...) at /home/user/repo/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:199
#<!-- -->39 0x0000555557cdffda in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/lib/Parse/ParseAST.cpp:175
#<!-- -->40 0x0000555557b850e6 in clang::FrontendAction::Execute (this=0x55555e2c9a80) at /home/user/repo/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1078
#<!-- -->41 0x0000555557a4ae01 in clang::CompilerInstance::ExecuteAction (this=0x7fffffffd8f8, Act=...) at /home/user/repo/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061
#<!-- -->42 0x0000555557a02502 in clang::tooling::FrontendActionFactory::runInvocation (this=0x55555e2c1ef0, Invocation=..., Files=0x55555e2c6910, PCHContainerOps=..., DiagConsumer=0x7fffffffdac0) at /home/user/repo/llvm-project/clang/lib/Tooling/Tooling.cpp:465
#<!-- -->43 0x0000555555acd136 in clang_fuzzer::HandleCXX (S="void fn_40_b ( const char * ( * a4 ( int * __cdecl f2 ( int * state_t [ 4 ] [ 4 ] , char * argv [ ] ) , ... ) ) [ 1 / ( ( 1 + ( __FILE__ [ 10 ] ) &gt;&gt; ( 0x52 &gt;&gt; ( ( ( ( ( __builtin_FILE ( ) [ 0 ] ) &amp; ( "..., FileName=&lt;optimized out&gt;, ExtraArgs=std::vector of length 1, capacity 1 = {...}) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/handle-cxx/handle_cxx.cpp:49
#<!-- -->44 0x0000555555accad4 in LLVMFuzzerTestOneInput (data=0x7ffff4be4010 "void fn_40_b ( const char * ( * a4 ( int * __cdecl f2 ( int * state_t [ 4 ] [ 4 ] , char * argv [ ] ) , ... ) ) [ 1 / ( ( 1 + ( __FILE__ [ 10 ] ) &gt;&gt; ( 0x52 &gt;&gt; ( ( ( ( ( __builtin_FILE ( ) [ 0 ] ) &amp; ( "..., size=&lt;optimized out&gt;) at /home/user/repo/llvm-project/clang/tools/clang-fuzzer/ClangFuzzer.cpp:23
#<!-- -->45 0x000055555c6f96ee in ExecuteFilesOnyByOne (argc=2, argv=0x7fffffffe348, callback=callback@<!-- -->entry=0x555555acc990 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:256
#<!-- -->46 0x000055555c6f94de in LLVMFuzzerRunDriver (argcp=argcp@<!-- -->entry=0x7fffffffe214, argvp=argvp@<!-- -->entry=0x7fffffffe218, callback=0x555555acc990 &lt;LLVMFuzzerTestOneInput(uint8_t*, size_t)&gt;) at aflpp_driver.c:377
#<!-- -->47 0x000055555c6f901e in main (argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe348) at aflpp_driver.c:312
#<!-- -->48 0x00007ffff7a63d90 in __libc_start_call_main (main=main@<!-- -->entry=0x55555c6f8f60 &lt;main&gt;, argc=argc@<!-- -->entry=2, argv=argv@<!-- -->entry=0x7fffffffe348) at ../sysdeps/nptl/libc_start_call_main.h:58
#<!-- -->49 0x00007ffff7a63e40 in __libc_start_main_impl (main=0x55555c6f8f60 &lt;main&gt;, argc=2, argv=0x7fffffffe348, init=&lt;optimized out&gt;, fini=&lt;optimized out&gt;, rtld_fini=&lt;optimized out&gt;, stack_end=0x7fffffffe338) at ../csu/libc-start.c:392
#<!-- -->50 0x0000555555acc8b5 in _start ()
```

</details>


---

### Comment 2 - shafik

@zczc66 I am curious is this fuzzing part of a larger project or effort?

---

### Comment 3 - zczc66

> I am curious is this fuzzing part of a larger project or effort?

It’s not part of a large project , just a small experiment.

---

