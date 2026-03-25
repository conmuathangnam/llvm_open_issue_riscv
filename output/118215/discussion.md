# [llvm][Mips] After c4a60c9d343, Assertion failed: (PhysReg.isPhysical() && "Unallocated register?!"), function useOrDefCSROrFI

**Author:** DimitryAndric
**URL:** https://github.com/llvm/llvm-project/issues/118215
**Status:** Open
**Labels:** backend:MIPS, llvm:codegen, crash-on-valid

## Body

After `llvmorg-17-init-12447-g`c4a60c9d343 ("[CodeGen][ShrinkWrap] Enable PostShrinkWrap by default") by @sushgokh, building parts of libzfs on FreeBSD for mips asserts with:

```text
Assertion failed: (PhysReg.isPhysical() && "Unallocated register?!"), function useOrDefCSROrFI, file /home/dim/src/llvm/llvm-project/llvm/lib/CodeGen/ShrinkWrap.cpp, line 341.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang -cc1 -triple mips-unknown-freebsd13.4 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name arc.c -mrelocation-model static -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu mips2 -target-feature -noabicalls -target-feature +soft-float -target-abi o32 -msoft-float -mfloat-abi soft -mllvm -mips-ssection-threshold=0 -debug-info-kind=standalone -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/home/dim/obj/home/dim/src/freebsd/stable-13-llvm-19/mips.mips/cddl/lib/libzpool -fcoverage-compilation-dir=/home/dim/obj/home/dim/src/freebsd/stable-13-llvm-19/mips.mips/cddl/lib/libzpool -nobuiltininc -sys-header-deps -D IN_BASE -D HAVE_ISSETUGID -D LIB_ZPOOL_BUILD -D ZFS_DEBUG -D WANTS_MUTEX_OWNED -D DEBUG=1 -D NEED_SOLARIS_BOOLEAN -O2 -Wno-format-zero-length -Wsystem-headers -Werror -Wall -Wno-format-y2k -Wno-uninitialized -Wno-pointer-sign -Wno-unknown-pragmas -Wno-empty-body -Wno-string-plus-int -Wno-unused-const-variable -Wno-error=unused-but-set-variable -Wno-error=array-parameter -Wno-error=deprecated-non-prototype -Wno-error=unused-but-set-parameter -Wno-tautological-compare -Wno-unused-value -Wno-parentheses-equality -Wno-unused-function -Wno-enum-conversion -Wno-unused-local-typedef -Wno-address-of-packed-member -Wno-switch -Wno-switch-enum -Wno-knr-promoted-parameter -std=iso9899:1999 -ferror-limit 19 -fgnuc-version=4.2.1 -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c arc-cacfd2.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'arc-cacfd2.c'.
4.      Running pass 'Shrink Wrapping analysis' on function '@arc_init'
 #0 0x000000000469d6d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x469d6d8)
 #1 0x000000000469b4b9 llvm::sys::RunSignalHandlers() (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x469b4b9)
 #2 0x000000000469de9e SignalHandler(int) Signals.cpp:0:0
 #3 0x0000000829e9257c handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x0000000829e91b4b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #5 0x00000008297e32d3 ([vdso]+0x2d3)
 #6 0x000000082a44a5ca __sys_thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #7 0x000000082e1df0b4 __raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082e2934f9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #9 0x000000082e1c2871 (/lib/libc.so.7+0x9c871)
#10 0x0000000003f6ee7c (anonymous namespace)::ShrinkWrap::useOrDefCSROrFI(llvm::MachineInstr const&, llvm::RegScavenger*, bool) const ShrinkWrap.cpp:0:0
#11 0x0000000003f6c2cc (anonymous namespace)::ShrinkWrap::runOnMachineFunction(llvm::MachineFunction&) ShrinkWrap.cpp:0:0
#12 0x0000000003bc9cee llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x3bc9cee)
#13 0x0000000004181a50 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4181a50)
#14 0x0000000004189ed4 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4189ed4)
#15 0x0000000004182552 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4182552)
#16 0x0000000004e92f2f clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4e92f2f)
#17 0x0000000004eb790d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4eb790d)
#18 0x000000000636b569 clang::ParseAST(clang::Sema&, bool, bool) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x636b569)
#19 0x00000000052edc93 clang::FrontendAction::Execute() (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x52edc93)
#20 0x0000000005279e3d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x5279e3d)
#21 0x00000000053cdefe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x53cdefe)
#22 0x00000000029e8d64 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x29e8d64)
#23 0x00000000029e58d5 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x00000000029e4b82 clang_main(int, char**, llvm::ToolContext const&) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x29e4b82)
Abort trap
```

Minimized test case:

```c
// clang -cc1 -triple mips-- -S -mrelocation-model static -target-cpu mips2 -O2 arc-min.c
long arc_all_memory, arc_stats_4_0_0;
long long arc_stats_3_0_0, arc_stats_2_0_0, arc_stats_1_0_0, arc_stats_0_0_0;
void zfs_arc_dnode_limit_percent() {
  arc_stats_0_0_0 = arc_stats_1_0_0 / 2 ?: 4;
  arc_stats_2_0_0 = arc_stats_4_0_0 * arc_stats_1_0_0;
  arc_stats_3_0_0 = arc_all_memory * arc_stats_2_0_0 / 100;
}
```

Note that disabling shrink wrap (using `-mllvm -enable-shrink-wrap-region-split=false`) papers over the problem, but with the full test case I get another regression, which I will submit separately.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Dimitry Andric (DimitryAndric)

<details>
After `llvmorg-17-init-12447-g`c4a60c9d343 ("[CodeGen][ShrinkWrap] Enable PostShrinkWrap by default") by @<!-- -->sushgokh, building parts of libzfs on FreeBSD for mips asserts with:

```text
Assertion failed: (PhysReg.isPhysical() &amp;&amp; "Unallocated register?!"), function useOrDefCSROrFI, file /home/dim/src/llvm/llvm-project/llvm/lib/CodeGen/ShrinkWrap.cpp, line 341.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang -cc1 -triple mips-unknown-freebsd13.4 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name arc.c -mrelocation-model static -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu mips2 -target-feature -noabicalls -target-feature +soft-float -target-abi o32 -msoft-float -mfloat-abi soft -mllvm -mips-ssection-threshold=0 -debug-info-kind=standalone -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/home/dim/obj/home/dim/src/freebsd/stable-13-llvm-19/mips.mips/cddl/lib/libzpool -fcoverage-compilation-dir=/home/dim/obj/home/dim/src/freebsd/stable-13-llvm-19/mips.mips/cddl/lib/libzpool -nobuiltininc -sys-header-deps -D IN_BASE -D HAVE_ISSETUGID -D LIB_ZPOOL_BUILD -D ZFS_DEBUG -D WANTS_MUTEX_OWNED -D DEBUG=1 -D NEED_SOLARIS_BOOLEAN -O2 -Wno-format-zero-length -Wsystem-headers -Werror -Wall -Wno-format-y2k -Wno-uninitialized -Wno-pointer-sign -Wno-unknown-pragmas -Wno-empty-body -Wno-string-plus-int -Wno-unused-const-variable -Wno-error=unused-but-set-variable -Wno-error=array-parameter -Wno-error=deprecated-non-prototype -Wno-error=unused-but-set-parameter -Wno-tautological-compare -Wno-unused-value -Wno-parentheses-equality -Wno-unused-function -Wno-enum-conversion -Wno-unused-local-typedef -Wno-address-of-packed-member -Wno-switch -Wno-switch-enum -Wno-knr-promoted-parameter -std=iso9899:1999 -ferror-limit 19 -fgnuc-version=4.2.1 -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c arc-cacfd2.c
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'arc-cacfd2.c'.
4.      Running pass 'Shrink Wrapping analysis' on function '@<!-- -->arc_init'
 #<!-- -->0 0x000000000469d6d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x469d6d8)
 #<!-- -->1 0x000000000469b4b9 llvm::sys::RunSignalHandlers() (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x469b4b9)
 #<!-- -->2 0x000000000469de9e SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x0000000829e9257c handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #<!-- -->4 0x0000000829e91b4b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:244:1
 #<!-- -->5 0x00000008297e32d3 ([vdso]+0x2d3)
 #<!-- -->6 0x000000082a44a5ca __sys_thr_kill /usr/obj/usr/src/amd64.amd64/lib/libsys/thr_kill.S:4:0
 #<!-- -->7 0x000000082e1df0b4 __raise /usr/src/lib/libc/gen/raise.c:0:10
 #<!-- -->8 0x000000082e2934f9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #<!-- -->9 0x000000082e1c2871 (/lib/libc.so.7+0x9c871)
#<!-- -->10 0x0000000003f6ee7c (anonymous namespace)::ShrinkWrap::useOrDefCSROrFI(llvm::MachineInstr const&amp;, llvm::RegScavenger*, bool) const ShrinkWrap.cpp:0:0
#<!-- -->11 0x0000000003f6c2cc (anonymous namespace)::ShrinkWrap::runOnMachineFunction(llvm::MachineFunction&amp;) ShrinkWrap.cpp:0:0
#<!-- -->12 0x0000000003bc9cee llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x3bc9cee)
#<!-- -->13 0x0000000004181a50 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4181a50)
#<!-- -->14 0x0000000004189ed4 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4189ed4)
#<!-- -->15 0x0000000004182552 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4182552)
#<!-- -->16 0x0000000004e92f2f clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::__1::unique_ptr&lt;llvm::raw_pwrite_stream, std::__1::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4e92f2f)
#<!-- -->17 0x0000000004eb790d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x4eb790d)
#<!-- -->18 0x000000000636b569 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x636b569)
#<!-- -->19 0x00000000052edc93 clang::FrontendAction::Execute() (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x52edc93)
#<!-- -->20 0x0000000005279e3d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x5279e3d)
#<!-- -->21 0x00000000053cdefe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x53cdefe)
#<!-- -->22 0x00000000029e8d64 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x29e8d64)
#<!-- -->23 0x00000000029e58d5 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->24 0x00000000029e4b82 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/dim/ins/llvmorg-20-init-12945-ga7427410dd7/bin/clang+0x29e4b82)
Abort trap
```

Minimized test case:

```c
// clang -cc1 -triple mips-- -S -mrelocation-model static -target-cpu mips2 -O2 arc-min.c
long arc_all_memory, arc_stats_4_0_0;
long long arc_stats_3_0_0, arc_stats_2_0_0, arc_stats_1_0_0, arc_stats_0_0_0;
void zfs_arc_dnode_limit_percent() {
  arc_stats_0_0_0 = arc_stats_1_0_0 / 2 ?: 4;
  arc_stats_2_0_0 = arc_stats_4_0_0 * arc_stats_1_0_0;
  arc_stats_3_0_0 = arc_all_memory * arc_stats_2_0_0 / 100;
}
```

Note that disabling shrink wrap (using `-mllvm -enable-shrink-wrap-region-split=false`) papers over the problem, but with the full test case I get another regression, which I will submit separately.

</details>


---

### Comment 2 - brad0

@yingopq @wzssyqa  Has this been fixed or does it need something further?

---

### Comment 3 - yingopq

No, this assert was different with issue #118223.
I also can reproduce this case in the environment which has fixed #118223.


---

