# [Clang] Clang crashes when attempting to call a member function via CRTP in a default argument expression

**Author:** venkatrao1
**URL:** https://github.com/llvm/llvm-project/issues/123471

## Body

I'm using this [clang19 docker image](https://hub.docker.com/layers/silkeh/clang/19/images/sha256-12b393a8a03b2b2296a28e01651c0be35d5fc7c612df71a93ed2be4a61f46598) to reproduce, here's a [godbolt reproduction](https://godbolt.org/z/jcorvY5K6) as well.

Reproduction file:
```cpp
template<typename Derived>
struct CRTPBase {
  auto& self() { return static_cast<Derived&>(*this); }
};

template<typename T = int>
struct Foo : CRTPBase<Foo<T>> {
  int x() const { return 5; }
 
  // illegal to call a non-static member fn "self" here, I think
  int callXByDefault(int val = self().x()) { return val; }

private:
  using CRTPBase<Foo<T>>::self;
};

int main() {
  return Foo{}.callXByDefault();
}
```
Note: removing the template parameter T from Foo produces an error instead of crashing, which I think should be the correct behavior with the template parameter as well.

I get the following in stderr:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes repro.cpp
1.	<eof> parser at end of file
2.	repro.cpp:17:5: LLVM IR generation of declaration 'main'
3.	repro.cpp:17:5: Generating code for declaration 'main'
 #0 0x00007f72f89ca3c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb73c6)
 #1 0x00007f72f89c8070 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb5070)
 #2 0x00007f72f8910210 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfd210)
 #3 0x00007f72f764f050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #4 0x00007f730169fb5f clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1fa4b5f)
 #5 0x00007f73017403b4 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20453b4)
 #6 0x00007f730173ef7d clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2043f7d)
 #7 0x00007f730176a5d9 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f5d9)
 #8 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
 #9 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#10 0x00007f7301743999 clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2048999)
#11 0x00007f7301742aa5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047aa5)
#12 0x00007f730174b88c clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205088c)
#13 0x00007f7301742a95 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047a95)
#14 0x00007f730173768d clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c68d)
#15 0x00007f730176a7a4 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f7a4)
#16 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#17 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#18 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#19 0x00007f73017a10bf (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a60bf)
#20 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#21 0x00007f7301737210 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c210)
#22 0x00007f7301737856 clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c856)
#23 0x00007f7301692162 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&, clang::Expr const*, clang::QualType) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f97162)
#24 0x00007f730169174f clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range<clang::Stmt::CastIterator<clang::Expr, clang::Expr const* const, clang::Stmt const* const>>, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f9674f)
#25 0x00007f7301768f7f (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206df7f)
#26 0x00007f7301768a75 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorCall(clang::CXXMethodDecl const*, clang::CodeGen::CGCallee const&, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206da75)
#27 0x00007f730176b2fb clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20702fb)
#28 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#29 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#30 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#31 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#32 0x00007f730189a3fe clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x219f3fe)
#33 0x00007f73018a2b7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21a7b7c)
#34 0x00007f7301901063 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2206063)
#35 0x00007f73019228d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22278d0)
#36 0x00007f730191c0f4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22210f4)
#37 0x00007f730191fe68 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2224e68)
#38 0x00007f730191aef1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x221fef1)
#39 0x00007f73019bd13c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22c213c)
#40 0x00007f73018f03fe clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21f53fe)
#41 0x00007f730033121a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3621a)
#42 0x00007f73023f87a5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cfd7a5)
#43 0x00007f73023666f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c6b6f4)
#44 0x00007f730247657e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b57e)
#45 0x000055bbf1bb9bc9 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc9)
#46 0x000055bbf1bb6b55 (/usr/lib/llvm-19/bin/clang+0xeb55)
#47 0x00007f7301ff9369 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fe369)
#48 0x00007f72f890ffa8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfcfa8)
#49 0x00007f7301ff8cdd clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fdcdd)
#50 0x00007f7301fbde05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c2e05)
#51 0x00007f7301fbe06e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c306e)
#52 0x00007f7301fdb64c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28e064c)
#53 0x000055bbf1bb65a1 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xe5a1)
#54 0x000055bbf1bc4356 main (/usr/lib/llvm-19/bin/clang+0x1c356)
#55 0x00007f72f763a24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#56 0x00007f72f763a305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#57 0x000055bbf1bb47b1 _start (/usr/lib/llvm-19/bin/clang+0xc7b1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (++20250114103228+cd708029e0b2-1~exp1~20250114103334.78)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/repro-b39b7a.cpp
clang++: note: diagnostic msg: /tmp/repro-b39b7a.sh
clang++: note: diagnostic msg: 

********************
```
`repro-b39b7a.cpp` and `repro-b39b7a.sh` are attached, with .txt extensions as the original file types are not allowed.
[stderr.txt](https://github.com/user-attachments/files/18465707/stderr.txt)
[repro-b39b7a.cpp.txt](https://github.com/user-attachments/files/18465709/repro-b39b7a.cpp.txt)
[repro-b39b7a.sh.txt](https://github.com/user-attachments/files/18465708/repro-b39b7a.sh.txt)

Sorry if the formatting's a bit off, this is my first time reporting an issue here.

## Comments

### Comment 1 - DimitryAndric

Asserts with `llvmorg-20-init-18265-g`0a33532500a:

```text
Assertion failed: (CXXThisValue && "no 'this' value for this function"), function LoadCXXThis, file /home/dim/src/llvm/llvm-project/clang/lib/CodeGen/CodeGenFunction.h, line 3081.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name repro.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/mnt/clangbug -fcoverage-compilation-dir=/mnt/clangbug -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -disable-llvm-passes -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ repro-b39b7a.cpp
1.      <eof> parser at end of file
2.      repro.cpp:17:5: LLVM IR generation of declaration 'main'
3.      repro.cpp:17:5: Generating code for declaration 'main'
 #0 0x00000000047d3a18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x47d3a18)
 #1 0x00000000047d1539 llvm::sys::RunSignalHandlers() (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x47d1539)
 #2 0x00000000047d421e SignalHandler(int) Signals.cpp:0:0
 #3 0x000000082bad2310 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x000000082bad18db thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #5 0x000000082985a2d3 ([vdso]+0x2d3)
 #6 0x000000082ef2632a __sys_thr_kill /usr/obj/usr/src/lib/libc/thr_kill.S:4:0
 #7 0x000000082ee9f6b4 __raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082ef50179 abort /usr/src/lib/libc/stdlib/abort.c:71:17
 #9 0x000000082ee82771 (/lib/libc.so.7+0x93771)
#10 0x0000000004b87ef4 clang::StmtVisitorBase<std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#11 0x0000000004b7e82b clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7e82b)
#12 0x0000000004b5c2cd clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b5c2cd)
#13 0x0000000004b5c1c4 clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b5c1c4)
#14 0x0000000004d25e03 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d25e03)
#15 0x0000000004d24efe clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d24efe)
#16 0x0000000004b7865f clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7865f)
#17 0x0000000004b60c0b clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b60c0b)
#18 0x0000000004b5fbda clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b5fbda)
#19 0x0000000004b7dac1 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0>(long) CGExpr.cpp:0:0
#20 0x00000000051c904f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x51c904f)
#21 0x0000000004b53a21 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b53a21)
#22 0x0000000004b69689 clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b69689)
#23 0x0000000004b5fbc5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b5fbc5)
#24 0x0000000004b7dac1 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0>(long) CGExpr.cpp:0:0
#25 0x00000000051c904f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x51c904f)
#26 0x0000000004b53a21 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b53a21)
#27 0x0000000004d25fc2 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d25fc2)
#28 0x0000000004d24efe clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d24efe)
#29 0x0000000004b7865f clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7865f)
#30 0x0000000004b91836 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#31 0x0000000004b91c69 (anonymous namespace)::ScalarExprEmitter::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) CGExprScalar.cpp:0:0
#32 0x0000000004b7e82b clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7e82b)
#33 0x0000000004b5325a clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b5325a)
#34 0x0000000004b53bf0 clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b53bf0)
#35 0x0000000004a71e48 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&, clang::Expr const*, clang::QualType) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4a71e48)
#36 0x0000000004a70b42 clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range<clang::Stmt::CastIterator<clang::Expr, clang::Expr const* const, clang::Stmt const* const>>, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4a70b42)
#37 0x0000000004d24370 commonEmitCXXMemberOrOperatorCall(clang::CodeGen::CodeGenFunction&, clang::GlobalDecl, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList&, clang::CodeGen::CallArgList*) CGExprCXX.cpp:0:0
#38 0x0000000004d23e02 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorCall(clang::CXXMethodDecl const*, clang::CodeGen::CGCallee const&, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList*, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d23e02)
#39 0x0000000004d26c71 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d26c71)
#40 0x0000000004d24efe clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4d24efe)
#41 0x0000000004b7865f clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7865f)
#42 0x0000000004b91836 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#43 0x0000000004b7e82b clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b7e82b)
#44 0x0000000004b41842 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b41842)
#45 0x0000000004b4a3b1 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b4a3b1)
#46 0x0000000004b29255 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4b29255)
#47 0x0000000004a00910 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4a00910)
#48 0x00000000049f87c4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x49f87c4)
#49 0x00000000049fd483 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x49fd483)
#50 0x00000000049f7380 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x49f7380)
#51 0x0000000004ff407c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#52 0x0000000004febd85 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x4febd85)
#53 0x0000000006535119 clang::ParseAST(clang::Sema&, bool, bool) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x6535119)
#54 0x000000000545772a clang::FrontendAction::Execute() (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x545772a)
#55 0x00000000053d270d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x53d270d)
#56 0x0000000005533909 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x5533909)
#57 0x0000000002ab04ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x2ab04ff)
#58 0x0000000002aad10e ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#59 0x0000000002aac338 clang_main(int, char**, llvm::ToolContext const&) (/home/dim/ins/llvmorg-20-init-18265-g0a33532500a/bin/clang+0x2aac338)
Abort trap
```


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: Venkat Rao (venkatrao1)

<details>
I'm using this [clang19 docker image](https://hub.docker.com/layers/silkeh/clang/19/images/sha256-12b393a8a03b2b2296a28e01651c0be35d5fc7c612df71a93ed2be4a61f46598) to reproduce, here's a [godbolt reproduction](https://godbolt.org/z/jcorvY5K6) as well.

Reproduction file:
```cpp
template&lt;typename Derived&gt;
struct CRTPBase {
  auto&amp; self() { return static_cast&lt;Derived&amp;&gt;(*this); }
};

template&lt;typename T = int&gt;
struct Foo : CRTPBase&lt;Foo&lt;T&gt;&gt; {
  int x() const { return 5; }
 
  // illegal to call a non-static member fn "self" here, I think
  int callXByDefault(int val = self().x()) { return val; }

private:
  using CRTPBase&lt;Foo&lt;T&gt;&gt;::self;
};

int main() {
  return Foo{}.callXByDefault();
}
```
Note: removing the template parameter T from Foo produces an error instead of crashing, which I think should be the correct behavior with the template parameter as well.

I get the following in stderr:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes repro.cpp
1.	&lt;eof&gt; parser at end of file
2.	repro.cpp:17:5: LLVM IR generation of declaration 'main'
3.	repro.cpp:17:5: Generating code for declaration 'main'
 #<!-- -->0 0x00007f72f89ca3c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb73c6)
 #<!-- -->1 0x00007f72f89c8070 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb5070)
 #<!-- -->2 0x00007f72f8910210 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfd210)
 #<!-- -->3 0x00007f72f764f050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->4 0x00007f730169fb5f clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1fa4b5f)
 #<!-- -->5 0x00007f73017403b4 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20453b4)
 #<!-- -->6 0x00007f730173ef7d clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2043f7d)
 #<!-- -->7 0x00007f730176a5d9 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f5d9)
 #<!-- -->8 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
 #<!-- -->9 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->10 0x00007f7301743999 clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2048999)
#<!-- -->11 0x00007f7301742aa5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047aa5)
#<!-- -->12 0x00007f730174b88c clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205088c)
#<!-- -->13 0x00007f7301742a95 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047a95)
#<!-- -->14 0x00007f730173768d clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c68d)
#<!-- -->15 0x00007f730176a7a4 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f7a4)
#<!-- -->16 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->17 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->18 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->19 0x00007f73017a10bf (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a60bf)
#<!-- -->20 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->21 0x00007f7301737210 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c210)
#<!-- -->22 0x00007f7301737856 clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c856)
#<!-- -->23 0x00007f7301692162 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&amp;, clang::Expr const*, clang::QualType) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f97162)
#<!-- -->24 0x00007f730169174f clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&amp;, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range&lt;clang::Stmt::CastIterator&lt;clang::Expr, clang::Expr const* const, clang::Stmt const* const&gt;&gt;, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f9674f)
#<!-- -->25 0x00007f7301768f7f (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206df7f)
#<!-- -->26 0x00007f7301768a75 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorCall(clang::CXXMethodDecl const*, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206da75)
#<!-- -->27 0x00007f730176b2fb clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20702fb)
#<!-- -->28 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->29 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->30 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->31 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->32 0x00007f730189a3fe clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x219f3fe)
#<!-- -->33 0x00007f73018a2b7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21a7b7c)
#<!-- -->34 0x00007f7301901063 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2206063)
#<!-- -->35 0x00007f73019228d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22278d0)
#<!-- -->36 0x00007f730191c0f4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22210f4)
#<!-- -->37 0x00007f730191fe68 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2224e68)
#<!-- -->38 0x00007f730191aef1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x221fef1)
#<!-- -->39 0x00007f73019bd13c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22c213c)
#<!-- -->40 0x00007f73018f03fe clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21f53fe)
#<!-- -->41 0x00007f730033121a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3621a)
#<!-- -->42 0x00007f73023f87a5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cfd7a5)
#<!-- -->43 0x00007f73023666f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c6b6f4)
#<!-- -->44 0x00007f730247657e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b57e)
#<!-- -->45 0x000055bbf1bb9bc9 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc9)
#<!-- -->46 0x000055bbf1bb6b55 (/usr/lib/llvm-19/bin/clang+0xeb55)
#<!-- -->47 0x00007f7301ff9369 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fe369)
#<!-- -->48 0x00007f72f890ffa8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfcfa8)
#<!-- -->49 0x00007f7301ff8cdd clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fdcdd)
#<!-- -->50 0x00007f7301fbde05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c2e05)
#<!-- -->51 0x00007f7301fbe06e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c306e)
#<!-- -->52 0x00007f7301fdb64c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28e064c)
#<!-- -->53 0x000055bbf1bb65a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe5a1)
#<!-- -->54 0x000055bbf1bc4356 main (/usr/lib/llvm-19/bin/clang+0x1c356)
#<!-- -->55 0x00007f72f763a24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#<!-- -->56 0x00007f72f763a305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#<!-- -->57 0x000055bbf1bb47b1 _start (/usr/lib/llvm-19/bin/clang+0xc7b1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (++20250114103228+cd708029e0b2-1~exp1~20250114103334.78)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/repro-b39b7a.cpp
clang++: note: diagnostic msg: /tmp/repro-b39b7a.sh
clang++: note: diagnostic msg: 

********************
```
`repro-b39b7a.cpp` and `repro-b39b7a.sh` are attached, with .txt extensions as the original file types are not allowed.
[stderr.txt](https://github.com/user-attachments/files/18465707/stderr.txt)
[repro-b39b7a.cpp.txt](https://github.com/user-attachments/files/18465709/repro-b39b7a.cpp.txt)
[repro-b39b7a.sh.txt](https://github.com/user-attachments/files/18465708/repro-b39b7a.sh.txt)

Sorry if the formatting's a bit off, this is my first time reporting an issue here.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Venkat Rao (venkatrao1)

<details>
I'm using this [clang19 docker image](https://hub.docker.com/layers/silkeh/clang/19/images/sha256-12b393a8a03b2b2296a28e01651c0be35d5fc7c612df71a93ed2be4a61f46598) to reproduce, here's a [godbolt reproduction](https://godbolt.org/z/jcorvY5K6) as well.

Reproduction file:
```cpp
template&lt;typename Derived&gt;
struct CRTPBase {
  auto&amp; self() { return static_cast&lt;Derived&amp;&gt;(*this); }
};

template&lt;typename T = int&gt;
struct Foo : CRTPBase&lt;Foo&lt;T&gt;&gt; {
  int x() const { return 5; }
 
  // illegal to call a non-static member fn "self" here, I think
  int callXByDefault(int val = self().x()) { return val; }

private:
  using CRTPBase&lt;Foo&lt;T&gt;&gt;::self;
};

int main() {
  return Foo{}.callXByDefault();
}
```
Note: removing the template parameter T from Foo produces an error instead of crashing, which I think should be the correct behavior with the template parameter as well.

I get the following in stderr:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes repro.cpp
1.	&lt;eof&gt; parser at end of file
2.	repro.cpp:17:5: LLVM IR generation of declaration 'main'
3.	repro.cpp:17:5: Generating code for declaration 'main'
 #<!-- -->0 0x00007f72f89ca3c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb73c6)
 #<!-- -->1 0x00007f72f89c8070 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb5070)
 #<!-- -->2 0x00007f72f8910210 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfd210)
 #<!-- -->3 0x00007f72f764f050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->4 0x00007f730169fb5f clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1fa4b5f)
 #<!-- -->5 0x00007f73017403b4 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20453b4)
 #<!-- -->6 0x00007f730173ef7d clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2043f7d)
 #<!-- -->7 0x00007f730176a5d9 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f5d9)
 #<!-- -->8 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
 #<!-- -->9 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->10 0x00007f7301743999 clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2048999)
#<!-- -->11 0x00007f7301742aa5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047aa5)
#<!-- -->12 0x00007f730174b88c clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205088c)
#<!-- -->13 0x00007f7301742a95 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047a95)
#<!-- -->14 0x00007f730173768d clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c68d)
#<!-- -->15 0x00007f730176a7a4 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f7a4)
#<!-- -->16 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->17 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->18 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->19 0x00007f73017a10bf (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a60bf)
#<!-- -->20 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->21 0x00007f7301737210 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c210)
#<!-- -->22 0x00007f7301737856 clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c856)
#<!-- -->23 0x00007f7301692162 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&amp;, clang::Expr const*, clang::QualType) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f97162)
#<!-- -->24 0x00007f730169174f clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&amp;, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range&lt;clang::Stmt::CastIterator&lt;clang::Expr, clang::Expr const* const, clang::Stmt const* const&gt;&gt;, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f9674f)
#<!-- -->25 0x00007f7301768f7f (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206df7f)
#<!-- -->26 0x00007f7301768a75 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorCall(clang::CXXMethodDecl const*, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206da75)
#<!-- -->27 0x00007f730176b2fb clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20702fb)
#<!-- -->28 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->29 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->30 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->31 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->32 0x00007f730189a3fe clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x219f3fe)
#<!-- -->33 0x00007f73018a2b7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21a7b7c)
#<!-- -->34 0x00007f7301901063 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2206063)
#<!-- -->35 0x00007f73019228d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22278d0)
#<!-- -->36 0x00007f730191c0f4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22210f4)
#<!-- -->37 0x00007f730191fe68 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2224e68)
#<!-- -->38 0x00007f730191aef1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x221fef1)
#<!-- -->39 0x00007f73019bd13c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22c213c)
#<!-- -->40 0x00007f73018f03fe clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21f53fe)
#<!-- -->41 0x00007f730033121a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3621a)
#<!-- -->42 0x00007f73023f87a5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cfd7a5)
#<!-- -->43 0x00007f73023666f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c6b6f4)
#<!-- -->44 0x00007f730247657e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b57e)
#<!-- -->45 0x000055bbf1bb9bc9 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc9)
#<!-- -->46 0x000055bbf1bb6b55 (/usr/lib/llvm-19/bin/clang+0xeb55)
#<!-- -->47 0x00007f7301ff9369 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fe369)
#<!-- -->48 0x00007f72f890ffa8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfcfa8)
#<!-- -->49 0x00007f7301ff8cdd clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fdcdd)
#<!-- -->50 0x00007f7301fbde05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c2e05)
#<!-- -->51 0x00007f7301fbe06e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c306e)
#<!-- -->52 0x00007f7301fdb64c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28e064c)
#<!-- -->53 0x000055bbf1bb65a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe5a1)
#<!-- -->54 0x000055bbf1bc4356 main (/usr/lib/llvm-19/bin/clang+0x1c356)
#<!-- -->55 0x00007f72f763a24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#<!-- -->56 0x00007f72f763a305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#<!-- -->57 0x000055bbf1bb47b1 _start (/usr/lib/llvm-19/bin/clang+0xc7b1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (++20250114103228+cd708029e0b2-1~exp1~20250114103334.78)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/repro-b39b7a.cpp
clang++: note: diagnostic msg: /tmp/repro-b39b7a.sh
clang++: note: diagnostic msg: 

********************
```
`repro-b39b7a.cpp` and `repro-b39b7a.sh` are attached, with .txt extensions as the original file types are not allowed.
[stderr.txt](https://github.com/user-attachments/files/18465707/stderr.txt)
[repro-b39b7a.cpp.txt](https://github.com/user-attachments/files/18465709/repro-b39b7a.cpp.txt)
[repro-b39b7a.sh.txt](https://github.com/user-attachments/files/18465708/repro-b39b7a.sh.txt)

Sorry if the formatting's a bit off, this is my first time reporting an issue here.
</details>


---

### Comment 4 - shafik

I believe we should be rejecting this: https://godbolt.org/z/6nnoEPdxv

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Venkat Rao (venkatrao1)

<details>
I'm using this [clang19 docker image](https://hub.docker.com/layers/silkeh/clang/19/images/sha256-12b393a8a03b2b2296a28e01651c0be35d5fc7c612df71a93ed2be4a61f46598) to reproduce, here's a [godbolt reproduction](https://godbolt.org/z/jcorvY5K6) as well.

Reproduction file:
```cpp
template&lt;typename Derived&gt;
struct CRTPBase {
  auto&amp; self() { return static_cast&lt;Derived&amp;&gt;(*this); }
};

template&lt;typename T = int&gt;
struct Foo : CRTPBase&lt;Foo&lt;T&gt;&gt; {
  int x() const { return 5; }
 
  // illegal to call a non-static member fn "self" here, I think
  int callXByDefault(int val = self().x()) { return val; }

private:
  using CRTPBase&lt;Foo&lt;T&gt;&gt;::self;
};

int main() {
  return Foo{}.callXByDefault();
}
```
Note: removing the template parameter T from Foo produces an error instead of crashing, which I think should be the correct behavior with the template parameter as well.

I get the following in stderr:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes repro.cpp
1.	&lt;eof&gt; parser at end of file
2.	repro.cpp:17:5: LLVM IR generation of declaration 'main'
3.	repro.cpp:17:5: Generating code for declaration 'main'
 #<!-- -->0 0x00007f72f89ca3c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb73c6)
 #<!-- -->1 0x00007f72f89c8070 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb5070)
 #<!-- -->2 0x00007f72f8910210 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfd210)
 #<!-- -->3 0x00007f72f764f050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->4 0x00007f730169fb5f clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1fa4b5f)
 #<!-- -->5 0x00007f73017403b4 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20453b4)
 #<!-- -->6 0x00007f730173ef7d clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2043f7d)
 #<!-- -->7 0x00007f730176a5d9 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f5d9)
 #<!-- -->8 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
 #<!-- -->9 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->10 0x00007f7301743999 clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2048999)
#<!-- -->11 0x00007f7301742aa5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047aa5)
#<!-- -->12 0x00007f730174b88c clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205088c)
#<!-- -->13 0x00007f7301742a95 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2047a95)
#<!-- -->14 0x00007f730173768d clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c68d)
#<!-- -->15 0x00007f730176a7a4 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206f7a4)
#<!-- -->16 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->17 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->18 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->19 0x00007f73017a10bf (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a60bf)
#<!-- -->20 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->21 0x00007f7301737210 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c210)
#<!-- -->22 0x00007f7301737856 clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x203c856)
#<!-- -->23 0x00007f7301692162 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&amp;, clang::Expr const*, clang::QualType) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f97162)
#<!-- -->24 0x00007f730169174f clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&amp;, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range&lt;clang::Stmt::CastIterator&lt;clang::Expr, clang::Expr const* const, clang::Stmt const* const&gt;&gt;, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1f9674f)
#<!-- -->25 0x00007f7301768f7f (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206df7f)
#<!-- -->26 0x00007f7301768a75 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorCall(clang::CXXMethodDecl const*, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::Value*, clang::QualType, clang::CallExpr const*, clang::CodeGen::CallArgList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206da75)
#<!-- -->27 0x00007f730176b2fb clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20702fb)
#<!-- -->28 0x00007f7301769819 clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x206e819)
#<!-- -->29 0x00007f7301757031 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x205c031)
#<!-- -->30 0x00007f73017a0a3e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x20a5a3e)
#<!-- -->31 0x00007f730178f59d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x209459d)
#<!-- -->32 0x00007f730189a3fe clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x219f3fe)
#<!-- -->33 0x00007f73018a2b7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21a7b7c)
#<!-- -->34 0x00007f7301901063 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2206063)
#<!-- -->35 0x00007f73019228d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22278d0)
#<!-- -->36 0x00007f730191c0f4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22210f4)
#<!-- -->37 0x00007f730191fe68 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2224e68)
#<!-- -->38 0x00007f730191aef1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x221fef1)
#<!-- -->39 0x00007f73019bd13c (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x22c213c)
#<!-- -->40 0x00007f73018f03fe clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21f53fe)
#<!-- -->41 0x00007f730033121a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3621a)
#<!-- -->42 0x00007f73023f87a5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cfd7a5)
#<!-- -->43 0x00007f73023666f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c6b6f4)
#<!-- -->44 0x00007f730247657e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d7b57e)
#<!-- -->45 0x000055bbf1bb9bc9 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc9)
#<!-- -->46 0x000055bbf1bb6b55 (/usr/lib/llvm-19/bin/clang+0xeb55)
#<!-- -->47 0x00007f7301ff9369 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fe369)
#<!-- -->48 0x00007f72f890ffa8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfcfa8)
#<!-- -->49 0x00007f7301ff8cdd clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fdcdd)
#<!-- -->50 0x00007f7301fbde05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c2e05)
#<!-- -->51 0x00007f7301fbe06e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28c306e)
#<!-- -->52 0x00007f7301fdb64c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28e064c)
#<!-- -->53 0x000055bbf1bb65a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe5a1)
#<!-- -->54 0x000055bbf1bc4356 main (/usr/lib/llvm-19/bin/clang+0x1c356)
#<!-- -->55 0x00007f72f763a24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#<!-- -->56 0x00007f72f763a305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#<!-- -->57 0x000055bbf1bb47b1 _start (/usr/lib/llvm-19/bin/clang+0xc7b1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (++20250114103228+cd708029e0b2-1~exp1~20250114103334.78)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/repro-b39b7a.cpp
clang++: note: diagnostic msg: /tmp/repro-b39b7a.sh
clang++: note: diagnostic msg: 

********************
```
`repro-b39b7a.cpp` and `repro-b39b7a.sh` are attached, with .txt extensions as the original file types are not allowed.
[stderr.txt](https://github.com/user-attachments/files/18465707/stderr.txt)
[repro-b39b7a.cpp.txt](https://github.com/user-attachments/files/18465709/repro-b39b7a.cpp.txt)
[repro-b39b7a.sh.txt](https://github.com/user-attachments/files/18465708/repro-b39b7a.sh.txt)

Sorry if the formatting's a bit off, this is my first time reporting an issue here.
</details>


---

