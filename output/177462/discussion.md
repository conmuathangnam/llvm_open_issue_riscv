# [clang] occasional frontend crashes on version 22.1.0-rc1

**Author:** xarblu
**URL:** https://github.com/llvm/llvm-project/issues/177462
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Since I started using LLVM 22.1.0-rc1 my custom Linux kernel builds started occasionally crashing clang.

Some builds complete perfectly fine but those that fail seem to always crash on `fs/bcachefs/data/ec/init.c` with an error like the following:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-22 -Wp,-MMD,fs/bcachefs/data/ec/.init.o.d -nostdinc -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/arch/x86/include -I./arch/x86/include/generated -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include -I./include -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/uapi -I./include/generated/uapi -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/compiler-version.h -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/kconfig.h -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/compiler_types.h -D__KERNEL__ --target=x86_64-linux-gnu -fintegrated-as -Werror=ignored-optimization-argument -Werror=option-ignored -fmacro-prefix-map=/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/= -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-sse4a -fcf-protection=branch -fno-jump-tables -m64 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mstack-alignment=8 -mskip-rax-setup -march=x86-64-v3 -mno-red-zone -mcmodel=kernel -mstack-protector-guard-reg=gs -mstack-protector-guard-symbol=__ref_stack_chk_guard -Wno-sign-compare -fno-asynchronous-unwind-tables -mretpoline-external-thunk -mindirect-branch-cs-prefix -mfunction-return=thunk-extern -mharden-sls=all -fpatchable-function-entry=11,11 -fno-delete-null-pointer-checks -O3 -fstack-protector-strong -ftrivial-auto-var-init=zero -fno-stack-clash-protection -pg -mfentry -DCC_USING_NOP_MCOUNT -DCC_USING_FENTRY -fno-lto -flto=thin -fsplit-lto-unit -fvisibility=hidden -fsanitize=kcfi -fsanitize-kcfi-arity -falign-functions=16 -fstrict-flex-arrays=3 -fms-extensions -fno-strict-overflow -fno-stack-check -fno-builtin-wcslen -Wall -Wextra -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-microsoft-anon-tag -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wno-default-const-init-unsafe -Wno-pointer-sign -Wcast-function-type -Wno-unterminated-string-initialization -Wimplicit-fallthrough -Werror=date-time -Wenum-conversion -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -Wno-psabi -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs -Ifs/bcachefs -DMODULE -DKBUILD_BASENAME=\"init\" -DKBUILD_MODNAME=\"bcachefs\" -D__KBUILD_MODNAME=bcachefs -c -o fs/bcachefs/data/ec/init.o /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>))),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>)))" on module "/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c"
5.	Running pass "jump-threading" on function "check_bucket_to_stripe_ref"
  CC      drivers/base/module.o
  CC [M]  drivers/tty/rpmsg_tty.o
  CC [M]  fs/smb/server/server.o
  LD [M]  fs/nfs/blocklayout/blocklayoutdriver.o
  LD [M]  drivers/nfc/st21nfca/st21nfca_hci.o
 #0 0x00007fbe42fb62da llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4bb62da)
 #1 0x00007fbe42fb2a8d llvm::sys::RunSignalHandlers() (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4bb2a8d)
 #2 0x00007fbe42eaa144 (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4aaa144)
 #3 0x00007fbe3e044220 (/usr/lib64/libc.so.6+0x44220)
 #4 0x00007fbe43146a27 llvm::Argument::getRange() const (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4d46a27)
 #5 0x00007fbe4503f758 llvm::LazyValueInfoImpl::solveBlockValueNonLocal(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3f758)
 #6 0x00007fbe4503f37c llvm::LazyValueInfoImpl::solveBlockValueImpl(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3f37c)
 #7 0x00007fbe4503e6e7 llvm::LazyValueInfoImpl::solveBlockValue(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3e6e7)
 #8 0x00007fbe4503dc2e llvm::LazyValueInfoImpl::solve() (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3dc2e)
 #9 0x00007fbe45049313 llvm::LazyValueInfoImpl::getValueOnEdge(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c49313)
#10 0x00007fbe4504b78a llvm::LazyValueInfo::getConstantOnEdge(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c4b78a)
#11 0x00007fbe446cbe24 llvm::JumpThreadingPass::computeValueKnownInPredecessorsImpl(llvm::Value*, llvm::BasicBlock*, llvm::SmallVectorImpl<std::__1::pair<llvm::Constant*, llvm::BasicBlock*>>&, llvm::jumpthreading::ConstantPreference, llvm::SmallPtrSet<llvm::Value*, 4u>&, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cbe24)
#12 0x00007fbe446cc265 llvm::JumpThreadingPass::computeValueKnownInPredecessorsImpl(llvm::Value*, llvm::BasicBlock*, llvm::SmallVectorImpl<std::__1::pair<llvm::Constant*, llvm::BasicBlock*>>&, llvm::jumpthreading::ConstantPreference, llvm::SmallPtrSet<llvm::Value*, 4u>&, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cc265)
#13 0x00007fbe446ce9ec llvm::JumpThreadingPass::processThreadableEdges(llvm::Value*, llvm::BasicBlock*, llvm::jumpthreading::ConstantPreference, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62ce9ec)
#14 0x00007fbe446cbabc llvm::JumpThreadingPass::processBlock(llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cbabc)
#15 0x00007fbe446caa4a llvm::JumpThreadingPass::runImpl(llvm::Function&, llvm::AnalysisManager<llvm::Function>*, llvm::TargetLibraryInfo*, llvm::TargetTransformInfo*, llvm::LazyValueInfo*, llvm::AAResults*, std::__1::unique_ptr<llvm::DomTreeUpdater, std::__1::default_delete<llvm::DomTreeUpdater>>, llvm::BlockFrequencyInfo*, llvm::BranchProbabilityInfo*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62caa4a)
#16 0x00007fbe446ca517 llvm::JumpThreadingPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62ca517)
#17 0x00007fbe431ea3eb llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4dea3eb)
#18 0x00007fbe44f182bf llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b182bf)
#19 0x00007fbe44f140cc llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b140cc)
#20 0x00007fbe44f16af6 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b16af6)
#21 0x00007fbe44f15b0d llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b15b0d)
#22 0x00007fbe431e9025 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4de9025)
#23 0x00007fbe44a478ae llvm::ModuleInlinerWrapperPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x66478ae)
#24 0x00007fbe431e9025 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4de9025)
#25 0x00007fbe4b0944da (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x30944da)
#26 0x00007fbe4b08b314 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x308b314)
#27 0x00007fbe4b441639 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3441639)
#28 0x00007fbe495ece59 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x15ece59)
#29 0x00007fbe4bdfa37f clang::FrontendAction::Execute() (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3dfa37f)
#30 0x00007fbe4bd2f4a0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3d2f4a0)
#31 0x00007fbe4be88e6c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3e88e6c)
#32 0x000055a3236ba003 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm/22/bin/clang-22+0x14003)
#33 0x000055a3236b6f42 (/usr/lib/llvm/22/bin/clang-22+0x10f42)
#34 0x000055a3236b8c99 (/usr/lib/llvm/22/bin/clang-22+0x12c99)
#35 0x00007fbe4b8e055d (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38e055d)
#36 0x00007fbe42ea9e30 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4aa9e30)
#37 0x00007fbe4b8dfaa5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38dfaa5)
#38 0x00007fbe4b88ef0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x388ef0d)
#39 0x00007fbe4b88f1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x388f1de)
#40 0x00007fbe4b8bad8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38bad8c)
#41 0x000055a3236b5e30 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm/22/bin/clang-22+0xfe30)
#42 0x000055a3236c73f8 main (/usr/lib/llvm/22/bin/clang-22+0x213f8)
#43 0x00007fbe3e02764b (/usr/lib64/libc.so.6+0x2764b)
#44 0x00007fbe3e02770a __libc_start_main (/usr/lib64/libc.so.6+0x2770a)
#45 0x000055a3236b4255 _start (/usr/lib/llvm/22/bin/clang-22+0xe255)
clang-22: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.1.0+libcxx
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm/22/bin
Configuration file: /etc/clang/22/x86_64-unknown-linux-gnu-clang.cfg
  CC      drivers/mfd/max8998-irq.o
  AR      drivers/scsi/device_handler/built-in.a
  CC [M]  drivers/scsi/device_handler/scsi_dh_rdac.o
  AR      net/mctp/built-in.a
  LD [M]  drivers/nfc/st21nfca/st21nfca_i2c.o
  CC      fs/xfs/xfs_dahash_test.o
  LD [M]  fs/nfs/nfs.o
  CC      net/mptcp/pm_kernel.o
  CC [M]  fs/ocfs2/cluster/tcp.o
clang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-22: note: diagnostic msg: /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/temp/init-c8d266.c
clang-22: note: diagnostic msg: /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/temp/init-c8d266.sh
clang-22: note: diagnostic msg:

********************
make[5]: *** [/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/scripts/Makefile.build:287: fs/bcachefs/data/ec/init.o] Error 1
make[4]: *** [/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/scripts/Makefile.build:544: fs/bcachefs] Error 2
make[4]: *** Waiting for unfinished jobs....
```

The same source files build fine with LLVM  `21.1.8` and GCC `15.2.1` so I'm assuming this is a regression of some sort.

Here the requested reproducer files (for me these reproduce the crash roughly 1/3 of runs):
[init-c8d266.sh](https://github.com/user-attachments/files/24806834/init-c8d266.sh)
[init-c8d266.c](https://github.com/user-attachments/files/24806835/init-c8d266.c)


## Comments

### Comment 1 - nikic

Reduced:
```c
struct {
  int v;
} foo();
void test() {
  ({ foo().v ? ({ goto label; 0; }) : 0; });
label:
  ;
}
```
Run with `clang -O3 -Xclang -disable-llvm-optzns`:
```
Instruction does not dominate all uses!
  %cond = phi i32 [ %2, %1 ], [ 0, %cond.false ]
  store i32 %cond, ptr %tmp, align 4, !tbaa !5
fatal error: error in backend: Broken module found, compilation aborted!
```
The crash in LLVM is due to invalid input IR.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Xarblu (xarblu)

<details>
Since I started using LLVM 22.1.0-rc1 my custom Linux kernel builds started occasionally crashing clang.

Some builds complete perfectly fine but those that fail seem to always crash on `fs/bcachefs/data/ec/init.c` with an error like the following:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-22 -Wp,-MMD,fs/bcachefs/data/ec/.init.o.d -nostdinc -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/arch/x86/include -I./arch/x86/include/generated -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include -I./include -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/uapi -I./include/generated/uapi -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/compiler-version.h -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/kconfig.h -include /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/include/linux/compiler_types.h -D__KERNEL__ --target=x86_64-linux-gnu -fintegrated-as -Werror=ignored-optimization-argument -Werror=option-ignored -fmacro-prefix-map=/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/= -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-sse4a -fcf-protection=branch -fno-jump-tables -m64 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mstack-alignment=8 -mskip-rax-setup -march=x86-64-v3 -mno-red-zone -mcmodel=kernel -mstack-protector-guard-reg=gs -mstack-protector-guard-symbol=__ref_stack_chk_guard -Wno-sign-compare -fno-asynchronous-unwind-tables -mretpoline-external-thunk -mindirect-branch-cs-prefix -mfunction-return=thunk-extern -mharden-sls=all -fpatchable-function-entry=11,11 -fno-delete-null-pointer-checks -O3 -fstack-protector-strong -ftrivial-auto-var-init=zero -fno-stack-clash-protection -pg -mfentry -DCC_USING_NOP_MCOUNT -DCC_USING_FENTRY -fno-lto -flto=thin -fsplit-lto-unit -fvisibility=hidden -fsanitize=kcfi -fsanitize-kcfi-arity -falign-functions=16 -fstrict-flex-arrays=3 -fms-extensions -fno-strict-overflow -fno-stack-check -fno-builtin-wcslen -Wall -Wextra -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-microsoft-anon-tag -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wno-default-const-init-unsafe -Wno-pointer-sign -Wcast-function-type -Wno-unterminated-string-initialization -Wimplicit-fallthrough -Werror=date-time -Wenum-conversion -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -Wno-psabi -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs -I/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs -Ifs/bcachefs -DMODULE -DKBUILD_BASENAME=\"init\" -DKBUILD_MODNAME=\"bcachefs\" -D__KBUILD_MODNAME=bcachefs -c -o fs/bcachefs/data/ec/init.o /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "require&lt;globals-aa&gt;,function(invalidate&lt;aa&gt;),require&lt;profile-summary&gt;,cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;))),function(invalidate&lt;should-not-run-function-passes&gt;),cgscc(devirt&lt;4&gt;())" on module "/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c"
4.	Running pass "cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;)))" on module "/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/fs/bcachefs/data/ec/init.c"
5.	Running pass "jump-threading" on function "check_bucket_to_stripe_ref"
  CC      drivers/base/module.o
  CC [M]  drivers/tty/rpmsg_tty.o
  CC [M]  fs/smb/server/server.o
  LD [M]  fs/nfs/blocklayout/blocklayoutdriver.o
  LD [M]  drivers/nfc/st21nfca/st21nfca_hci.o
 #<!-- -->0 0x00007fbe42fb62da llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4bb62da)
 #<!-- -->1 0x00007fbe42fb2a8d llvm::sys::RunSignalHandlers() (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4bb2a8d)
 #<!-- -->2 0x00007fbe42eaa144 (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4aaa144)
 #<!-- -->3 0x00007fbe3e044220 (/usr/lib64/libc.so.6+0x44220)
 #<!-- -->4 0x00007fbe43146a27 llvm::Argument::getRange() const (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4d46a27)
 #<!-- -->5 0x00007fbe4503f758 llvm::LazyValueInfoImpl::solveBlockValueNonLocal(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3f758)
 #<!-- -->6 0x00007fbe4503f37c llvm::LazyValueInfoImpl::solveBlockValueImpl(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3f37c)
 #<!-- -->7 0x00007fbe4503e6e7 llvm::LazyValueInfoImpl::solveBlockValue(llvm::Value*, llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3e6e7)
 #<!-- -->8 0x00007fbe4503dc2e llvm::LazyValueInfoImpl::solve() (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c3dc2e)
 #<!-- -->9 0x00007fbe45049313 llvm::LazyValueInfoImpl::getValueOnEdge(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c49313)
#<!-- -->10 0x00007fbe4504b78a llvm::LazyValueInfo::getConstantOnEdge(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6c4b78a)
#<!-- -->11 0x00007fbe446cbe24 llvm::JumpThreadingPass::computeValueKnownInPredecessorsImpl(llvm::Value*, llvm::BasicBlock*, llvm::SmallVectorImpl&lt;std::__1::pair&lt;llvm::Constant*, llvm::BasicBlock*&gt;&gt;&amp;, llvm::jumpthreading::ConstantPreference, llvm::SmallPtrSet&lt;llvm::Value*, 4u&gt;&amp;, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cbe24)
#<!-- -->12 0x00007fbe446cc265 llvm::JumpThreadingPass::computeValueKnownInPredecessorsImpl(llvm::Value*, llvm::BasicBlock*, llvm::SmallVectorImpl&lt;std::__1::pair&lt;llvm::Constant*, llvm::BasicBlock*&gt;&gt;&amp;, llvm::jumpthreading::ConstantPreference, llvm::SmallPtrSet&lt;llvm::Value*, 4u&gt;&amp;, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cc265)
#<!-- -->13 0x00007fbe446ce9ec llvm::JumpThreadingPass::processThreadableEdges(llvm::Value*, llvm::BasicBlock*, llvm::jumpthreading::ConstantPreference, llvm::Instruction*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62ce9ec)
#<!-- -->14 0x00007fbe446cbabc llvm::JumpThreadingPass::processBlock(llvm::BasicBlock*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62cbabc)
#<!-- -->15 0x00007fbe446caa4a llvm::JumpThreadingPass::runImpl(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;*, llvm::TargetLibraryInfo*, llvm::TargetTransformInfo*, llvm::LazyValueInfo*, llvm::AAResults*, std::__1::unique_ptr&lt;llvm::DomTreeUpdater, std::__1::default_delete&lt;llvm::DomTreeUpdater&gt;&gt;, llvm::BlockFrequencyInfo*, llvm::BranchProbabilityInfo*) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62caa4a)
#<!-- -->16 0x00007fbe446ca517 llvm::JumpThreadingPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x62ca517)
#<!-- -->17 0x00007fbe431ea3eb llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4dea3eb)
#<!-- -->18 0x00007fbe44f182bf llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b182bf)
#<!-- -->19 0x00007fbe44f140cc llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b140cc)
#<!-- -->20 0x00007fbe44f16af6 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b16af6)
#<!-- -->21 0x00007fbe44f15b0d llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x6b15b0d)
#<!-- -->22 0x00007fbe431e9025 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4de9025)
#<!-- -->23 0x00007fbe44a478ae llvm::ModuleInlinerWrapperPass::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x66478ae)
#<!-- -->24 0x00007fbe431e9025 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4de9025)
#<!-- -->25 0x00007fbe4b0944da (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x30944da)
#<!-- -->26 0x00007fbe4b08b314 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::__1::unique_ptr&lt;llvm::raw_pwrite_stream, std::__1::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x308b314)
#<!-- -->27 0x00007fbe4b441639 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3441639)
#<!-- -->28 0x00007fbe495ece59 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x15ece59)
#<!-- -->29 0x00007fbe4bdfa37f clang::FrontendAction::Execute() (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3dfa37f)
#<!-- -->30 0x00007fbe4bd2f4a0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3d2f4a0)
#<!-- -->31 0x00007fbe4be88e6c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x3e88e6c)
#<!-- -->32 0x000055a3236ba003 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm/22/bin/clang-22+0x14003)
#<!-- -->33 0x000055a3236b6f42 (/usr/lib/llvm/22/bin/clang-22+0x10f42)
#<!-- -->34 0x000055a3236b8c99 (/usr/lib/llvm/22/bin/clang-22+0x12c99)
#<!-- -->35 0x00007fbe4b8e055d (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38e055d)
#<!-- -->36 0x00007fbe42ea9e30 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm/22/bin/../lib64/libLLVM.so.22.1+libcxx+0x4aa9e30)
#<!-- -->37 0x00007fbe4b8dfaa5 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38dfaa5)
#<!-- -->38 0x00007fbe4b88ef0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x388ef0d)
#<!-- -->39 0x00007fbe4b88f1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x388f1de)
#<!-- -->40 0x00007fbe4b8bad8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm/22/bin/../lib64/libclang-cpp.so.22.1+libcxx+0x38bad8c)
#<!-- -->41 0x000055a3236b5e30 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm/22/bin/clang-22+0xfe30)
#<!-- -->42 0x000055a3236c73f8 main (/usr/lib/llvm/22/bin/clang-22+0x213f8)
#<!-- -->43 0x00007fbe3e02764b (/usr/lib64/libc.so.6+0x2764b)
#<!-- -->44 0x00007fbe3e02770a __libc_start_main (/usr/lib64/libc.so.6+0x2770a)
#<!-- -->45 0x000055a3236b4255 _start (/usr/lib/llvm/22/bin/clang-22+0xe255)
clang-22: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.1.0+libcxx
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm/22/bin
Configuration file: /etc/clang/22/x86_64-unknown-linux-gnu-clang.cfg
  CC      drivers/mfd/max8998-irq.o
  AR      drivers/scsi/device_handler/built-in.a
  CC [M]  drivers/scsi/device_handler/scsi_dh_rdac.o
  AR      net/mctp/built-in.a
  LD [M]  drivers/nfc/st21nfca/st21nfca_i2c.o
  CC      fs/xfs/xfs_dahash_test.o
  LD [M]  fs/nfs/nfs.o
  CC      net/mptcp/pm_kernel.o
  CC [M]  fs/ocfs2/cluster/tcp.o
clang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-22: note: diagnostic msg: /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/temp/init-c8d266.c
clang-22: note: diagnostic msg: /var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/temp/init-c8d266.sh
clang-22: note: diagnostic msg:

********************
make[5]: *** [/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/scripts/Makefile.build:287: fs/bcachefs/data/ec/init.o] Error 1
make[4]: *** [/var/tmp/portage/sys-kernel/cachyos-kernel-6.19_rc6/work/linux-6.18/scripts/Makefile.build:544: fs/bcachefs] Error 2
make[4]: *** Waiting for unfinished jobs....
```

The same source files build fine with LLVM  `21.1.8` and GCC `15.2.1` so I'm assuming this is a regression of some sort.

Here the requested reproducer files (for me these reproduce the crash roughly 1/3 of runs):
[init-c8d266.sh](https://github.com/user-attachments/files/24806834/init-c8d266.sh)
[init-c8d266.c](https://github.com/user-attachments/files/24806835/init-c8d266.c)

</details>


---

