# clang crashes at -O{s,2,3} on x86_64-linux-gnu running pass 'Register Coalescer'

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/151659
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

It appears to be a recent regression as the latest build on Compiller Explorer doesn't crash. 

```
[507] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 3e2fadf3beff93433f125eb1ea1abe470db5aa12)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Candidate multilib: .;@m64
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.9
[508] % 
[508] % clangtk -O3 small.c
clang-22: /local/suz-local/software/clangbuild/llvm-project/llvm/lib/CodeGen/RegisterCoalescer.cpp:1500: bool {anonymous}::RegisterCoalescer::reMaterializeTrivialDef(const llvm::CoalescerPair&, llvm::MachineInstr*, bool&): Assertion `MO.isImplicit() && MO.getReg().isPhysical() && (MO.isDead() || (DefSubIdx && ((TRI->getSubReg(MO.getReg(), DefSubIdx) == MCRegister((unsigned)NewMI.getOperand(0).getReg())) || TRI->isSubRegisterEq(NewMI.getOperand(0).getReg(), MO.getReg()))))' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/home/suz/software/local/clang-trunk/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/suz/20250801-clangtk-m64-O3-build-003550/delta -fcoverage-compilation-dir=/home/suz/20250801-clangtk-m64-O3-build-003550/delta -resource-dir /local/home/suz/software/local/clang-trunk/lib/clang/22 -I /usr/local/include -I /local/suz-local/software/local/include -internal-isystem /local/home/suz/software/local/clang-trunk/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=100 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/small-c8a24d.o -x c small.c
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'small.c'.
4.	Running pass 'Register Coalescer' on function '@main'
 #0 0x00005b8b26ed93e2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x42ab3e2)
 #1 0x00005b8b26ed60df llvm::sys::RunSignalHandlers() (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x42a80df)
 #2 0x00005b8b26ed622c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007144aa445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007144aa49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007144aa49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007144aa49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007144aa44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007144aa4288ff abort ./stdlib/abort.c:81:7
 #9 0x00007144aa42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007144aa43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005b8b2636b86f (anonymous namespace)::RegisterCoalescer::reMaterializeTrivialDef(llvm::CoalescerPair const&, llvm::MachineInstr*, bool&) RegisterCoalescer.cpp:0:0
#12 0x00005b8b2636c9a7 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&) RegisterCoalescer.cpp:0:0
#13 0x00005b8b2636eab2 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef<llvm::MachineInstr*>) RegisterCoalescer.cpp:0:0
#14 0x00005b8b26372369 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#15 0x00005b8b263734cb (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#16 0x00005b8b26135623 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x00005b8b267bd281 llvm::FPPassManager::runOnFunction(llvm::Function&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x3b8f281)
#18 0x00005b8b267bd6d1 llvm::FPPassManager::runOnModule(llvm::Module&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x3b8f6d1)
#19 0x00005b8b267be024 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x3b90024)
#20 0x00005b8b2719181c (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) BackendUtil.cpp:0:0
#21 0x00005b8b271924ec clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x45644ec)
#22 0x00005b8b2788920e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x4c5b20e)
#23 0x00005b8b293354ac clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x67074ac)
#24 0x00005b8b27bb6617 clang::FrontendAction::Execute() (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x4f88617)
#25 0x00005b8b27b3776d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x4f0976d)
#26 0x00005b8b27ca7b7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0x5079b7b)
#27 0x00005b8b23af6f1b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0xec8f1b)
#28 0x00005b8b23aec687 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00005b8b23af10da clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/software/local/clang-trunk/bin/clang-22+0xec30da)
#30 0x00005b8b2398c4fa main (/local/home/suz/software/local/clang-trunk/bin/clang-22+0xd5e4fa)
#31 0x00007144aa42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#32 0x00007144aa42a28b call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007144aa42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#34 0x00005b8b23aebc05 _start (/local/home/suz/software/local/clang-trunk/bin/clang-22+0xebdc05)
clangtk: error: unable to execute command: Aborted
clangtk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 3e2fadf3beff93433f125eb1ea1abe470db5aa12)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-f29223.c
clangtk: note: diagnostic msg: /tmp/small-f29223.sh
clangtk: note: diagnostic msg: 

********************
[509] % 
[509] % cat small.c
int printf(const char *, ...);
int a, b;
short c = 1, f;
long d, e;
int main() {
  long g = 0, h = 1 % c;
  short i = 0;
  if (b && !(d < f || h < e))
  j:
    i = c;
  long k = d & g;
  short l = i + k;
  if (l - 1)
    while (b) {
      printf("0\n");
      if (a)
        goto j;
      g = b;
    }
}
```

## Comments

### Comment 1 - dtcxzyw

Reduced:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @main(i16 %0, ptr %p) {
entry:
  %.pre22 = load i64, ptr %p, align 8
  %rem18 = srem i16 1, %0
  %conv1 = zext i16 %rem18 to i64
  %cmp4 = icmp sgt i64 0, %conv1
  br i1 %cmp4, label %if.end, label %j

j:                                                ; preds = %if.end, %entry
  %g.0 = phi i64 [ 0, %entry ], [ 1, %if.end ]
  br label %if.end

if.end:                                           ; preds = %j, %entry
  %1 = phi i64 [ %.pre22, %entry ], [ 0, %j ]
  %i.0 = phi i16 [ 0, %entry ], [ 1, %j ]
  %g.1 = phi i64 [ 0, %entry ], [ %g.0, %j ]
  %and = and i64 %1, %g.1
  %2 = trunc i64 %and to i16
  %conv7 = or i16 %i.0, %2
  %tobool9 = icmp ne i16 %conv7, 1
  br i1 %tobool9, label %j, label %if.end16

if.end16:                                         ; preds = %if.end
  ret void
}

```
llvm version: 16a0892a9db1825ffa5e42b801e13215418d93b9

---

