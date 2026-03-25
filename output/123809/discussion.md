# [BOLT] --use-aggr-reg-reassign with BOLT on Clang/LLD leads to Segmentation Faults when using these binaries

**Author:** ms178
**URL:** https://github.com/llvm/llvm-project/issues/123809
**Status:** Closed
**Labels:** backend:X86, miscompilation, BOLT, crash
**Closed Date:** 2025-05-10T18:47:12Z

## Body

**Description:**

I am encountering compiler errors while compiling various projects with BOLTed Clang/LLD binaries that were built with the `--use-aggr-reg-reassign` option, e.g. in the `asn1_compiler` component of the Linux kernel 6.13. There, the faulty binaries crash during the loop vectorization optimization pass. Without that option, a BOLTed Clang/LLD with the same revision worked fine for the same task.

**Environment:**

*   **LLVM Version:** 20.0.0git (commit: e4f03b158c97098e1835cc1f00d0175398974f98)
*   **Target:** x86_64-pc-linux-gnu
*   **Host System:**  CachyOS
*   **Target CPU:** Intel Raptor Lake
*   **Kernel:** Linux Kernel 6.13 (CachyOS patched)

**Steps to Reproduce:**

1.  Clone the CachyOS Linux kernel 6.13 source code. I am using some extra patches available [in my repo](https://github.com/ms178/archpkgbuilds/tree/main/packages/linux-cachyos).
2.  Configure the kernel build with LLVM as the compiler.
3.  Build the kernel using a BOLTed LLVM toolchain which was build with these BOLT options:

```
        --dyno-stats \
        --icf=safe \
        --plt=all \
        --hugify \
        --peepholes=all \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit \
        --reg-reassign \
        --use-aggr-reg-reassign || return 1
```

**Expected Behavior:**

The `asn1_compiler` should compile successfully without any errors.

**Actual Behavior:**

The compilation crashes with a segmentation fault during the optimization of `scripts/asn1_compiler.c`.

Stack trace:

```
HOSTCC  scripts/asn1_compiler
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/marcus/llvm20/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir scripts/asn1_compiler- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name asn1_compiler.c -function-alignment 5 -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -ffp-contract=fast -fno-rounding-math -ffp-exception-behavior=ignore -mconstructor-aliases -funwind-tables=2 -target-cpu raptorlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +sahf -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +aes -target-feature +bmi2 -target-feature +pclmul -tune-cpu raptorlake -debugger-tuning=gdb -fdebug-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -ffunction-sections -fdata-sections -fsplit-machine-functions -fcoverage-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -resource-dir /home/marcus/llvm20/lib/clang/20 -dependency-file scripts/.asn1_compiler.d -MT scripts/asn1_compiler -I ./scripts/include -I ./include -internal-isystem /home/marcus/llvm20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -source-date-epoch 1737487879 -O3 -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-error -w -std=gnu11 -ferror-limit 19 -fcf-protection=none -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mprefer-vector-width=256 -mllvm -inline-threshold=1000 -mllvm -extra-vectorizer-passes -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement -mllvm -enable-gvn-hoist -mllvm -enable-dfa-jump-thread -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/asn1_compiler-456ed7.o -x c scripts/asn1_compiler.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,extra-vector-passes,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,early-cse<>,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-unroll-and-jam),loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "scripts/asn1_compiler.c"
4.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "main"
 #0 0x00005e1b5aed88c2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) Signals.cpp:0:0
 #1 0x00005e1b5aed8ccf SignalHandler(int) Signals.cpp:0:0
 #2 0x00007762de845f50 (/usr/lib/libc.so.6+0x45f50)
 #3 0x00005e1b58dd7acd llvm::X86TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) (.cold) X86TargetTransformInfo.cpp:0:0
 #4 0x00005e1b5bc88c83 llvm::VPWidenCastRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&) const (/home/marcus/llvm20/bin/clang-20+0xce88c83)
 #5 0x00005e1b5bc8322b llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce8322b)
 #6 0x00005e1b5bc761c3 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce761c3)
 #7 0x00005e1b5bc7637d llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce7637d)
 #8 0x00005e1b5bc23e9c llvm::LoopVectorizationPlanner::cost(llvm::VPlan&, llvm::ElementCount) const LoopVectorize.cpp:0:0
 #9 0x00005e1b5bc24265 llvm::LoopVectorizationPlanner::computeBestVF() LoopVectorize.cpp:0:0
#10 0x00005e1b5bc52312 llvm::LoopVectorizePass::processLoop(llvm::Loop*) LoopVectorize.cpp:0:0
#11 0x00005e1b59b252c1 llvm::LoopVectorizePass::runImpl(llvm::Function&) LoopVectorize.cpp:0:0
#12 0x00005e1b599aed27 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (.cold) bolt-pseudo.o:0:0
#13 0x00005e1b58af07cd llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilder.cpp:0:0
#14 0x00005e1b57cfc769 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/marcus/llvm20/bin/clang-20+0x8efc769)
#15 0x00005e1b57cfc11b llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) AMDGPUTargetMachine.cpp:0:0
#16 0x00005e1b57faea24 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/marcus/llvm20/bin/clang-20+0x91aea24)
#17 0x00005e1b57fae6a9 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) AMDGPUTargetMachine.cpp:0:0
#18 0x00005e1b57fb7744 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/marcus/llvm20/bin/clang-20+0x91b7744)
#19 0x00005e1b586a8fdf (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#20 0x00005e1b585c1fef clang::emitBackendOutput(clang::CompilerInstance&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/marcus/llvm20/bin/clang-20+0x97c1fef)
#21 0x00005e1b59a64ca2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/marcus/llvm20/bin/clang-20+0xac64ca2)
#22 0x00005e1b57d7a684 clang::ParseAST(clang::Sema&, bool, bool) (/home/marcus/llvm20/bin/clang-20+0x8f7a684)
#23 0x00005e1b5873281e clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0x993281e)
#24 0x00005e1b5869a9ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/marcus/llvm20/bin/clang-20+0x989a9ab)
#25 0x00005e1b5869ad9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0x989ad9c)
#26 0x00005e1b58698b4e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0x9898b4e)
#27 0x00005e1b5869a2f1 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x00005e1b58697d19 clang_main(int, char**, llvm::ToolContext const&) (/home/marcus/llvm20/bin/clang-20+0x9897d19)
#29 0x00005e1b52a9b752 (/home/marcus/llvm20/bin/clang-20+0x3c9b752)
#30 0x00007762de826187 (/usr/lib/libc.so.6+0x26187)
#31 0x00007762de826237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#32 0x00005e1b533bce71 _start (/home/marcus/llvm20/bin/clang-20+0x45bce71)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.c
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.sh
clang: note: diagnostic msg: 

********************
make[2]: *** [scripts/Makefile.host:114: scripts/asn1_compiler] Fehler 1
make[1]: *** [/tmp/makepkg/linux-cachyos/src/linux-6.13/Makefile:1255: scripts] Fehler 2
make: *** [Makefile:251: __sub-make] Fehler 2
```
[asn1_compiler-82f6f2.c.txt](https://github.com/user-attachments/files/18495726/asn1_compiler-82f6f2.c.txt)
[asn1_compiler-82f6f2.sh.txt](https://github.com/user-attachments/files/18495727/asn1_compiler-82f6f2.sh.txt)

## Comments

### Comment 1 - ms178

I could reproduce what looks to be the same issue with very modest flags on other packages, e.g. dbus-broker-git. It seems to get triggered by using `-O2` and above, `-O1` is fine.

```
export CC=clang
export CXX=clang++
export CC_LD=lld
export CXX_LD=lld
export AR=llvm-ar
export NM=llvm-nm
export STRIP=llvm-strip
export OBJCOPY=llvm-objcopy
export OBJDUMP=llvm-objdump
export READELF=llvm-readelf
export RANLIB=llvm-ranlib
export HOSTCC=clang
export HOSTCXX=clang++
export HOSTAR=llvm-ar
export CPPFLAGS="-D_FORTIFY_SOURCE=0"
export CFLAGS="-O1 -march=native -mtune=native -falign-functions=32 -fno-semantic-interposition -fcf-protection=none -mharden-sls=none -w"
export CXXFLAGS="${CFLAGS} -Wp,-U_GLIBCXX_ASSERTIONS"
export LDFLAGS="-Wl,-O3,-Bsymbolic-functions,--as-needed -fcf-protection=none -mharden-sls=none -Wl,--hash-style=gnu -Wl,--undefined-version"
export CCLDFLAGS="$LDFLAGS"
export CXXLDFLAGS="$LDFLAGS"
export ASFLAGS="-D__AVX__=1 -D__AVX2__=1 -D__FMA__=1"
```
Trace from dbus-broker-git (PGO optimizing stage):

```
[132/183] Compiling C object src/libbus-static.a.p/dbus_sasl.c.o
FAILED: src/libbus-static.a.p/dbus_sasl.c.o 
clang -Isrc/libbus-static.a.p -Isrc -I../dbus-broker-git/src -Isubprojects/libcdvar-1/src -I../dbus-broker-git/subprojects/libcdvar-1/src -Isubprojects/libcstdaux-1/src -I../dbus-broker-git/subprojects/libcstdaux-1/src -Isubprojects/libcutf8-1/src -I../dbus-broker-git/subprojects/libcutf8-1/src -Isubprojects/libclist-3/src -I../dbus-broker-git/subprojects/libclist-3/src -Isubprojects/libcrbtree-3/src -I../dbus-broker-git/subprojects/libcrbtree-3/src -flto -fdiagnostics-color=always -DNDEBUG -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -std=gnu18 -O3 -D_GNU_SOURCE -Wno-gnu-alignof-expression -Wno-maybe-uninitialized -Wno-unknown-warning-option -Wno-unused-parameter -Wno-error=type-limits -Wno-error=missing-field-initializers -Wdate-time -Wdeclaration-after-statement -Wlogical-op -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wredundant-decls -Wshadow -Wstrict-aliasing=3 -Wsuggest-attribute=noreturn -Wundef -Wwrite-strings '-DBINDIR="/usr/bin"' -DPACKAGE_VERSION=36 -DSYSTEMUIDMAX=999 '-DSYSTEM_CONSOLE_USERS="gdm","sddm","lightdm","lxdm",' -O2 -march=native -mtune=native -falign-functions=32 -fno-semantic-interposition -fcf-protection=none -mharden-sls=none -w -fprofile-use=/tmp/makepkg/dbus-broker-git/src/default.profdata -D_FORTIFY_SOURCE=0 -fPIC -fvisibility=hidden -fno-common -MD -MQ src/libbus-static.a.p/dbus_sasl.c.o -MF src/libbus-static.a.p/dbus_sasl.c.o.d -o src/libbus-static.a.p/dbus_sasl.c.o -c ../dbus-broker-git/src/dbus/sasl.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -Isrc/libbus-static.a.p -Isrc -I../dbus-broker-git/src -Isubprojects/libcdvar-1/src -I../dbus-broker-git/subprojects/libcdvar-1/src -Isubprojects/libcstdaux-1/src -I../dbus-broker-git/subprojects/libcstdaux-1/src -Isubprojects/libcutf8-1/src -I../dbus-broker-git/subprojects/libcutf8-1/src -Isubprojects/libclist-3/src -I../dbus-broker-git/subprojects/libclist-3/src -Isubprojects/libcrbtree-3/src -I../dbus-broker-git/subprojects/libcrbtree-3/src -flto -fdiagnostics-color=always -DNDEBUG -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -std=gnu18 -O3 -D_GNU_SOURCE -Wno-gnu-alignof-expression -Wno-maybe-uninitialized -Wno-unknown-warning-option -Wno-unused-parameter -Wno-error=type-limits -Wno-error=missing-field-initializers -Wdate-time -Wdeclaration-after-statement -Wlogical-op -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wredundant-decls -Wshadow -Wstrict-aliasing=3 -Wsuggest-attribute=noreturn -Wundef -Wwrite-strings -DBINDIR=\"/usr/bin\" -DPACKAGE_VERSION=36 -DSYSTEMUIDMAX=999 -DSYSTEM_CONSOLE_USERS=\"gdm\",\"sddm\",\"lightdm\",\"lxdm\", -O2 -march=native -mtune=native -falign-functions=32 -fno-semantic-interposition -fcf-protection=none -mharden-sls=none -w -fprofile-use=/tmp/makepkg/dbus-broker-git/src/default.profdata -D_FORTIFY_SOURCE=0 -fPIC -fvisibility=hidden -fno-common -MD -MQ src/libbus-static.a.p/dbus_sasl.c.o -MF src/libbus-static.a.p/dbus_sasl.c.o.d -o src/libbus-static.a.p/dbus_sasl.c.o -c ../dbus-broker-git/src/dbus/sasl.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "../dbus-broker-git/src/dbus/sasl.c"
4.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "sasl_server_dispatch"
 #0 0x00005c37642d88c2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) Signals.cpp:0:0
 #1 0x00005c37642d86b5 llvm::sys::CleanupOnSignal(unsigned long) (/home/marcus/llvm20/bin/clang-20+0xc0d86b5)
 #2 0x00005c376429e878 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x00005c376429e9fe CrashRecoverySignalHandler(int) (.llvm.7573658860347809712) CrashRecoveryContext.cpp:0:0
 #4 0x00007c5bb1a45f50 (/usr/lib/libc.so.6+0x45f50)
 #5 0x00005c37621d7acd llvm::X86TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) (.cold) X86TargetTransformInfo.cpp:0:0
 #6 0x00005c3765088c83 llvm::VPWidenCastRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&) const (/home/marcus/llvm20/bin/clang-20+0xce88c83)
 #7 0x00005c376508322b llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce8322b)
 #8 0x00005c37650761c3 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce761c3)
 #9 0x00005c376507637d llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/home/marcus/llvm20/bin/clang-20+0xce7637d)
#10 0x00005c3765023e9c llvm::LoopVectorizationPlanner::cost(llvm::VPlan&, llvm::ElementCount) const LoopVectorize.cpp:0:0
#11 0x00005c3765024265 llvm::LoopVectorizationPlanner::computeBestVF() LoopVectorize.cpp:0:0
#12 0x00005c3765052312 llvm::LoopVectorizePass::processLoop(llvm::Loop*) LoopVectorize.cpp:0:0
#13 0x00005c3762f252c1 llvm::LoopVectorizePass::runImpl(llvm::Function&) LoopVectorize.cpp:0:0
#14 0x00005c3762daed27 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (.cold) bolt-pseudo.o:0:0
#15 0x00005c3761ef07cd llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilder.cpp:0:0
#16 0x00005c37610fc769 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/marcus/llvm20/bin/clang-20+0x8efc769)
#17 0x00005c37610fc11b llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) AMDGPUTargetMachine.cpp:0:0
#18 0x00005c37613aea24 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/marcus/llvm20/bin/clang-20+0x91aea24)
#19 0x00005c37613ae6a9 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) AMDGPUTargetMachine.cpp:0:0
#20 0x00005c37613b7744 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/marcus/llvm20/bin/clang-20+0x91b7744)
#21 0x00005c3761aa8fdf (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#22 0x00005c37619c1fef clang::emitBackendOutput(clang::CompilerInstance&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/marcus/llvm20/bin/clang-20+0x97c1fef)
#23 0x00005c3762e64ca2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/marcus/llvm20/bin/clang-20+0xac64ca2)
#24 0x00005c376117a684 clang::ParseAST(clang::Sema&, bool, bool) (/home/marcus/llvm20/bin/clang-20+0x8f7a684)
#25 0x00005c3761b3281e clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0x993281e)
#26 0x00005c3761a9a9ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/marcus/llvm20/bin/clang-20+0x989a9ab)
#27 0x00005c3761a9ad9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0x989ad9c)
#28 0x00005c3761a98b4e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0x9898b4e)
#29 0x00005c3761a9a2f1 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x00005c3761a9a1a1 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#31 0x00005c3761a9a179 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/marcus/llvm20/bin/clang-20+0x989a179)
#32 0x00005c3761a9a04d clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/home/marcus/llvm20/bin/clang-20+0x989a04d)
#33 0x00005c3761a99ea4 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/marcus/llvm20/bin/clang-20+0x9899ea4)
#34 0x00005c3761a99c24 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/marcus/llvm20/bin/clang-20+0x9899c24)
#35 0x00005c3761a97a90 clang_main(int, char**, llvm::ToolContext const&) (/home/marcus/llvm20/bin/clang-20+0x9897a90)
#36 0x00005c375be9b752 (/home/marcus/llvm20/bin/clang-20+0x3c9b752)
#37 0x00007c5bb1a26187 (/usr/lib/libc.so.6+0x26187)
#38 0x00007c5bb1a26237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#39 0x00005c375c7bce71 _start (/home/marcus/llvm20/bin/clang-20+0x45bce71)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/sasl-839093.c
clang: note: diagnostic msg: /tmp/sasl-839093.sh
clang: note: diagnostic msg: 
```

[sasl-839093.c.txt](https://github.com/user-attachments/files/18495945/sasl-839093.c.txt)
[sasl-839093.sh.txt](https://github.com/user-attachments/files/18495944/sasl-839093.sh.txt)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (ms178)

<details>
**Description:**

I am encountering a segmentation fault while compiling the `asn1_compiler` component of the Linux kernel 6.13 using a recent development snapshot of LLVM. The crash occurs during the loop vectorization optimization pass and appears to be related to the cost model for cast instructions on x86.

**Environment:**

*   **LLVM Version:** 20.0.0git (commit: e4f03b158c97098e1835cc1f00d0175398974f98)
*   **Target:** x86_64-pc-linux-gnu
*   **Host System:**  CachyOS
*   **Target CPU:** Intel Raptor Lake
*   **Kernel:** Linux Kernel 6.13 (CachyOS patched)

**Steps to Reproduce:**

1.  Clone the CachyOS Linux kernel 6.13 source code. I am using some extra patches available [in my repo](https://github.com/ms178/archpkgbuilds/tree/main/packages/linux-cachyos).
2.  Configure the kernel build with LLVM as the compiler.
3.  Build the kernel.

**Expected Behavior:**

The `asn1_compiler` should compile successfully without any errors.

**Actual Behavior:**

The compilation crashes with a segmentation fault during the optimization of `scripts/asn1_compiler.c`.

Stack trace:

```
HOSTCC  scripts/asn1_compiler
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/marcus/llvm20/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir scripts/asn1_compiler- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name asn1_compiler.c -function-alignment 5 -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -ffp-contract=fast -fno-rounding-math -ffp-exception-behavior=ignore -mconstructor-aliases -funwind-tables=2 -target-cpu raptorlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +sahf -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +aes -target-feature +bmi2 -target-feature +pclmul -tune-cpu raptorlake -debugger-tuning=gdb -fdebug-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -ffunction-sections -fdata-sections -fsplit-machine-functions -fcoverage-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -resource-dir /home/marcus/llvm20/lib/clang/20 -dependency-file scripts/.asn1_compiler.d -MT scripts/asn1_compiler -I ./scripts/include -I ./include -internal-isystem /home/marcus/llvm20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -source-date-epoch 1737487879 -O3 -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-error -w -std=gnu11 -ferror-limit 19 -fcf-protection=none -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mprefer-vector-width=256 -mllvm -inline-threshold=1000 -mllvm -extra-vectorizer-passes -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement -mllvm -enable-gvn-hoist -mllvm -enable-dfa-jump-thread -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/asn1_compiler-456ed7.o -x c scripts/asn1_compiler.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(float2int,lower-constant-intrinsics,chr,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,extra-vector-passes,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,early-cse&lt;&gt;,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-unroll-and-jam),loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "scripts/asn1_compiler.c"
4.      Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "main"
 #<!-- -->0 0x00005e1b5aed88c2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) Signals.cpp:0:0
 #<!-- -->1 0x00005e1b5aed8ccf SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007762de845f50 (/usr/lib/libc.so.6+0x45f50)
 #<!-- -->3 0x00005e1b58dd7acd llvm::X86TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) (.cold) X86TargetTransformInfo.cpp:0:0
 #<!-- -->4 0x00005e1b5bc88c83 llvm::VPWidenCastRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&amp;) const (/home/marcus/llvm20/bin/clang-20+0xce88c83)
 #<!-- -->5 0x00005e1b5bc8322b llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce8322b)
 #<!-- -->6 0x00005e1b5bc761c3 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce761c3)
 #<!-- -->7 0x00005e1b5bc7637d llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce7637d)
 #<!-- -->8 0x00005e1b5bc23e9c llvm::LoopVectorizationPlanner::cost(llvm::VPlan&amp;, llvm::ElementCount) const LoopVectorize.cpp:0:0
 #<!-- -->9 0x00005e1b5bc24265 llvm::LoopVectorizationPlanner::computeBestVF() LoopVectorize.cpp:0:0
#<!-- -->10 0x00005e1b5bc52312 llvm::LoopVectorizePass::processLoop(llvm::Loop*) LoopVectorize.cpp:0:0
#<!-- -->11 0x00005e1b59b252c1 llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) LoopVectorize.cpp:0:0
#<!-- -->12 0x00005e1b599aed27 llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (.cold) bolt-pseudo.o:0:0
#<!-- -->13 0x00005e1b58af07cd llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) PassBuilder.cpp:0:0
#<!-- -->14 0x00005e1b57cfc769 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x8efc769)
#<!-- -->15 0x00005e1b57cfc11b llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) AMDGPUTargetMachine.cpp:0:0
#<!-- -->16 0x00005e1b57faea24 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x91aea24)
#<!-- -->17 0x00005e1b57fae6a9 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) AMDGPUTargetMachine.cpp:0:0
#<!-- -->18 0x00005e1b57fb7744 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x91b7744)
#<!-- -->19 0x00005e1b586a8fdf (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->20 0x00005e1b585c1fef clang::emitBackendOutput(clang::CompilerInstance&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/marcus/llvm20/bin/clang-20+0x97c1fef)
#<!-- -->21 0x00005e1b59a64ca2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xac64ca2)
#<!-- -->22 0x00005e1b57d7a684 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/marcus/llvm20/bin/clang-20+0x8f7a684)
#<!-- -->23 0x00005e1b5873281e clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0x993281e)
#<!-- -->24 0x00005e1b5869a9ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/marcus/llvm20/bin/clang-20+0x989a9ab)
#<!-- -->25 0x00005e1b5869ad9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0x989ad9c)
#<!-- -->26 0x00005e1b58698b4e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0x9898b4e)
#<!-- -->27 0x00005e1b5869a2f1 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->28 0x00005e1b58697d19 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/marcus/llvm20/bin/clang-20+0x9897d19)
#<!-- -->29 0x00005e1b52a9b752 (/home/marcus/llvm20/bin/clang-20+0x3c9b752)
#<!-- -->30 0x00007762de826187 (/usr/lib/libc.so.6+0x26187)
#<!-- -->31 0x00007762de826237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#<!-- -->32 0x00005e1b533bce71 _start (/home/marcus/llvm20/bin/clang-20+0x45bce71)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.c
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.sh
clang: note: diagnostic msg: 

********************
make[2]: *** [scripts/Makefile.host:114: scripts/asn1_compiler] Fehler 1
make[1]: *** [/tmp/makepkg/linux-cachyos/src/linux-6.13/Makefile:1255: scripts] Fehler 2
make: *** [Makefile:251: __sub-make] Fehler 2
```
[asn1_compiler-82f6f2.c.txt](https://github.com/user-attachments/files/18495726/asn1_compiler-82f6f2.c.txt)
[asn1_compiler-82f6f2.sh.txt](https://github.com/user-attachments/files/18495727/asn1_compiler-82f6f2.sh.txt)
</details>


---

### Comment 3 - ms178

Further investigation with an assertion build from the same LLVM revision suggest that the BOLT optimizations performed on that build might have caused or triggered the issue. 

With an assertion build from the same revision but minus the BOLT optimization stage that I usually perform on top, I cannot reproduce the reported issue. 

However, if I perform the BOLT optimization on the assertion build, I get issues, albeit with a different trace this time that also reproduce with O1.

With the Kernel:

```
Stack dump:
0.      Program arguments: /home/marcus/llvm20/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir scripts/basic/fixdep- -disable-free -clear-ast-before-backend -main-file-name fixdep.c -function-alignment 5 -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -ffp-contract=fast -fno-rounding-math -ffp-exception-behavior=ignore -mconstructor-aliases -funwind-tables=2 -target-cpu raptorlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +sahf -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +aes -target-feature +bmi2 -target-feature +pclmul -tune-cpu raptorlake -debugger-tuning=gdb -fdebug-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -ffunction-sections -fdata-sections -fsplit-machine-functions -fcoverage-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -resource-dir /home/marcus/llvm20/lib/clang/20 -dependency-file scripts/basic/.fixdep.d -MT scripts/basic/fixdep -I ./scripts/include -internal-isystem /home/marcus/llvm20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -source-date-epoch 1737500420 -O3 -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-error -w -std=gnu11 -ferror-limit 19 -fcf-protection=none -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mprefer-vector-width=256 -mllvm -inline-threshold=1000 -mllvm -extra-vectorizer-passes -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement -mllvm -enable-gvn-hoist -mllvm -enable-dfa-jump-thread -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/fixdep-243e73.o -x c scripts/basic/fixdep.c
1.      /usr/include/unistd.h:1011:67: current parser token ';'
 #0 0x00005ff2461d9a66 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) Signals.cpp:0:0
 #1 0x00005ff2461d9e69 SignalHandler(int) Signals.cpp:0:0
 #2 0x0000778eddc45f50 (/usr/lib/libc.so.6+0x45f50)
 #3 0x00005ff242600a98 clang::Decl::getTranslationUnitDecl() (/home/marcus/llvm20/bin/clang-20+0xb800a98)
 #4 0x00005ff2426c545e clang::Decl::addAttr(clang::Attr*) (/home/marcus/llvm20/bin/clang-20+0xb8c545e)
 #5 0x00005ff242a4936f (anonymous namespace)::ParsedAttrInfoConst::handleDeclAttribute(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) const (.0105da2943f03a831a0cebd4d20633ef) ParsedAttr.cpp:0:0
 #6 0x00005ff242a4adc5 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.llvm.14143684358988481704) SemaDeclAttr.cpp:0:0
 #7 0x00005ff24261bde8 clang::Sema::ProcessDeclAttributeList(clang::Scope*, clang::Decl*, clang::ParsedAttributesView const&, clang::Sema::ProcessDeclAttributeOptions const&) (/home/marcus/llvm20/bin/clang-20+0xb81bde8)
 #8 0x00005ff24261bd04 clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/home/marcus/llvm20/bin/clang-20+0xb81bd04)
 #9 0x00005ff2426e8180 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/home/marcus/llvm20/bin/clang-20+0xb8e8180)
#10 0x00005ff242679744 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/home/marcus/llvm20/bin/clang-20+0xb879744)
#11 0x00005ff242679258 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/home/marcus/llvm20/bin/clang-20+0xb879258)
#12 0x00005ff242670419 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) ParseDecl.cpp:0:0
#13 0x00005ff242675c0f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/marcus/llvm20/bin/clang-20+0xb875c0f)
#14 0x00005ff242676704 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) Parser.cpp:0:0
#15 0x00005ff242676482 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/marcus/llvm20/bin/clang-20+0xb876482)
#16 0x00005ff2426716ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/marcus/llvm20/bin/clang-20+0xb8716ca)
#17 0x00005ff2426b043f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/marcus/llvm20/bin/clang-20+0xb8b043f)
#18 0x00005ff2426b017a clang::ParseAST(clang::Sema&, bool, bool) (/home/marcus/llvm20/bin/clang-20+0xb8b017a)
#19 0x00005ff243107229 clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0xc307229)
#20 0x00005ff242ee04f3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/marcus/llvm20/bin/clang-20+0xc0e04f3)
#21 0x00005ff242ee0a18 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0xc0e0a18)
#22 0x00005ff242ede14d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0xc0de14d)
#23 0x00005ff242eda472 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x00005ff242edc7e8 clang_main(int, char**, llvm::ToolContext const&) (/home/marcus/llvm20/bin/clang-20+0xc0dc7e8)
#25 0x00005ff23ab045f2 (/home/marcus/llvm20/bin/clang-20+0x3d045f2)
#26 0x0000778eddc26187 (/usr/lib/libc.so.6+0x26187)
#27 0x0000778eddc26237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#28 0x00005ff23adee751 _start (/home/marcus/llvm20/bin/clang-20+0x3fee751)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************
```

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:

[fixdep-d6ca3c.c.txt](https://github.com/user-attachments/files/18497512/fixdep-d6ca3c.c.txt)
[fixdep-d6ca3c.sh.txt](https://github.com/user-attachments/files/18497511/fixdep-d6ca3c.sh.txt)

With dbus-broker-git:

```
Stack dump:
0.      Program arguments: clang -Isubprojects/libcdvar-1/src/libcdvar-1.so.0.p -Isubprojects/libcdvar-1/src -I../dbus-broker-git/subprojects/libcdvar-1/src -Isubprojects/libcstdaux-1/src -I../dbus-broker-git/subprojects/libcstdaux-1/src -Isubprojects/libcutf8-1/src -I../dbus-broker-git/subprojects/libcutf8-1/src -flto -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -std=c11 -O0 -g -D_GNU_SOURCE -Wno-gnu-alignof-expression -Wno-maybe-uninitialized -Wno-unknown-warning-option -Wno-unused-parameter -Wno-error=type-limits -Wno-error=missing-field-initializers -Wdate-time -Wdeclaration-after-statement -Wlogical-op -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wredundant-decls -Wshadow -Wstrict-aliasing=3 -Wsuggest-attribute=noreturn -Wundef -Wwrite-strings -O3 -march=native -mtune=native -falign-functions=32 -fno-semantic-interposition -fcf-protection=none -mharden-sls=none -w -fprofile-generate -g3 -fno-omit-frame-pointer -mllvm -vp-counters-per-site=10 -D_FORTIFY_SOURCE=0 -fPIC -fvisibility=hidden -fno-common -MD -MQ subprojects/libcdvar-1/src/libcdvar-1.so.0.p/c-dvar.c.o -MF subprojects/libcdvar-1/src/libcdvar-1.so.0.p/c-dvar.c.o.d -o subprojects/libcdvar-1/src/libcdvar-1.so.0.p/c-dvar.c.o -c ../dbus-broker-git/subprojects/libcdvar-1/src/c-dvar.c
1.      /usr/include/errno.h:37:64: current parser token ';'
 #0 0x00006404f59d9a66 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) Signals.cpp:0:0
 #1 0x00006404f59d984d llvm::sys::CleanupOnSignal(unsigned long) (/home/marcus/llvm20/bin/clang-20+0xf3d984d)
 #2 0x00006404f59737a0 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x00006404f59738fd CrashRecoverySignalHandler(int) (.llvm.7482630021576115543) CrashRecoveryContext.cpp:0:0
 #4 0x0000735835a45f50 (/usr/lib/libc.so.6+0x45f50)
 #5 0x00006404f1e00a98 clang::Decl::getTranslationUnitDecl() (/home/marcus/llvm20/bin/clang-20+0xb800a98)
 #6 0x00006404f1ec545e clang::Decl::addAttr(clang::Attr*) (/home/marcus/llvm20/bin/clang-20+0xb8c545e)
 #7 0x00006404f224936f (anonymous namespace)::ParsedAttrInfoConst::handleDeclAttribute(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) const (.0105da2943f03a831a0cebd4d20633ef) ParsedAttr.cpp:0:0
 #8 0x00006404f224adc5 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.llvm.14143684358988481704) SemaDeclAttr.cpp:0:0
 #9 0x00006404f1e1bde8 clang::Sema::ProcessDeclAttributeList(clang::Scope*, clang::Decl*, clang::ParsedAttributesView const&, clang::Sema::ProcessDeclAttributeOptions const&) (/home/marcus/llvm20/bin/clang-20+0xb81bde8)
#10 0x00006404f1e1bd04 clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/home/marcus/llvm20/bin/clang-20+0xb81bd04)
#11 0x00006404f1ee8180 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/home/marcus/llvm20/bin/clang-20+0xb8e8180)
#12 0x00006404f1e79744 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/home/marcus/llvm20/bin/clang-20+0xb879744)
#13 0x00006404f1e79258 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/home/marcus/llvm20/bin/clang-20+0xb879258)
#14 0x00006404f1e70419 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) ParseDecl.cpp:0:0
#15 0x00006404f1e75c0f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/marcus/llvm20/bin/clang-20+0xb875c0f)
#16 0x00006404f1e76704 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) Parser.cpp:0:0
#17 0x00006404f1e76482 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/marcus/llvm20/bin/clang-20+0xb876482)
#18 0x00006404f1e716ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/marcus/llvm20/bin/clang-20+0xb8716ca)
#19 0x00006404f1eb043f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/marcus/llvm20/bin/clang-20+0xb8b043f)
#20 0x00006404f1eb017a clang::ParseAST(clang::Sema&, bool, bool) (/home/marcus/llvm20/bin/clang-20+0xb8b017a)
#21 0x00006404f2907229 clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0xc307229)
#22 0x00006404f26e04f3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/marcus/llvm20/bin/clang-20+0xc0e04f3)
#23 0x00006404f26e0a18 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0xc0e0a18)
#24 0x00006404f26de14d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0xc0de14d)
#25 0x00006404f26da472 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00006404f26da319 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#27 0x00006404f26da2dd llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/marcus/llvm20/bin/clang-20+0xc0da2dd)
#28 0x00006404f26da1c0 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/home/marcus/llvm20/bin/clang-20+0xc0da1c0)
#29 0x00006404f26d9fb4 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/marcus/llvm20/bin/clang-20+0xc0d9fb4)
#30 0x00006404f26d9e70 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/marcus/llvm20/bin/clang-20+0xc0d9e70)
#31 0x00006404f26d9bdf clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/marcus/llvm20/bin/clang-20+0xc0d9bdf)
#32 0x00006404f26dc53f clang_main(int, char**, llvm::ToolContext const&) (/home/marcus/llvm20/bin/clang-20+0xc0dc53f)
#33 0x00006404ea3045f2 (/home/marcus/llvm20/bin/clang-20+0x3d045f2)
#34 0x0000735835a26187 (/usr/lib/libc.so.6+0x26187)
#35 0x0000735835a26237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#36 0x00006404ea5ee751 _start (/home/marcus/llvm20/bin/clang-20+0x3fee751)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************
```

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:

[c-dvar-80b4b0.c.txt](https://github.com/user-attachments/files/18497527/c-dvar-80b4b0.c.txt)
[c-dvar-80b4b0.sh.txt](https://github.com/user-attachments/files/18497526/c-dvar-80b4b0.sh.txt)


As [I've used some BOLT patches](https://github.com/ms178/archpkgbuilds/blob/main/toolchain-experimental/llvm-git/fixes.patch) that are still undergoing review might be a factor here, too. I'll try again with a more conservative BOLT configuration.

---

### Comment 4 - ms178

@aaupov This is the other issue I was diagnosing today. I am now confident that either BOLT or the extra BOLT patches under review that I am carrying around (https://github.com/ms178/archpkgbuilds/blob/main/toolchain-experimental/llvm-git/fixes.patch) have broken the Clang/LLD binaries as the LTO+PGOed binaries of the same LLVM revision work fine so far and cannot reproduce the issues reported here.  Experimenting with different BOLT configuration options didn't help so far. Are you aware of changes in BOLT during the past two weeks that might have broken the BOLTed Clang/LLD binaries that I am seeing these stack traces with?

For reference, this is the relevant part of [my BOLT script](https://github.com/ms178/archpkgbuilds/blob/main/toolchain-experimental/llvm-bolt-scripts-master/build_stage3-bolt-without-sampling.bash) that produced working binaries as of two weeks ago:

```
# Configure build
cmake -G Ninja ../llvm-project/llvm \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-pc-linux-gnu" \
    -DLLVM_TARGETS_TO_BUILD="X86" \
    -DLLVM_ENABLE_PROJECTS="polly;lld;clang;compiler-rt" \
    -D CMAKE_C_FLAGS="-O3 -march=native -mtune=native -mllvm -inline-threshold=500 -mllvm -polly -mllvm -polly-position=early -mllvm -polly-dependences-computeout=60000000 -mllvm -polly-detect-profitability-min-per-loop-insts=40 -mllvm -polly-tiling=true -mllvm -polly-prevect-width=256 -mllvm -polly-vectorizer=stripmine -mllvm -polly-invariant-load-hoisting -mllvm -polly-loopfusion-greedy -mllvm -polly-run-inliner -mllvm -polly-run-dce -mllvm -polly-enable-delicm=true -mllvm -polly -fmerge-all-constants -mllvm -extra-vectorizer-passes -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -fno-math-errno -fno-trapping-math -falign-functions=32 -fno-semantic-interposition -fomit-frame-pointer -fcf-protection=none -mharden-sls=none -flto=thin -fwhole-program-vtables -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement=1 -mllvm -enable-gvn-hoist=1 -mllvm -enable-dfa-jump-thread=1" \
    -D CMAKE_CXX_FLAGS="-O3 -march=native -mtune=native -mllvm -inline-threshold=500 -mllvm -polly -mllvm -polly-position=early -mllvm -polly-dependences-computeout=60000000 -mllvm -polly-detect-profitability-min-per-loop-insts=40 -mllvm -polly-tiling=true -mllvm -polly-prevect-width=256 -mllvm -polly-vectorizer=stripmine -mllvm -polly-invariant-load-hoisting -mllvm -polly-loopfusion-greedy -mllvm -polly-run-inliner -mllvm -polly-run-dce -mllvm -polly-enable-delicm=true -mllvm -polly -fmerge-all-constants -mllvm -extra-vectorizer-passes -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -fno-math-errno -fno-trapping-math -falign-functions=32 -fno-semantic-interposition -fomit-frame-pointer -fcf-protection=none -mharden-sls=none -flto=thin -fwhole-program-vtables -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement=1 -mllvm -enable-gvn-hoist=1 -mllvm -enable-dfa-jump-thread=1" \
    -D CMAKE_EXE_LINKER_FLAGS="-Wl,--lto-CGO3 -Wl,--gc-sections -Wl,--icf=all -Wl,--lto-O3,-O3,-Bsymbolic-functions,--as-needed -march=native -mtune=native -fuse-ld=lld -fcf-protection=none -mharden-sls=none -flto=thin -fwhole-program-vtables -Wl,--push-state -Wl,-whole-archive -lmimalloc -Wl,--pop-state -lpthread -lstdc++ -lm -ldl" \
    -D CMAKE_MODULE_LINKER_FLAGS="-Wl,--lto-CGO3 -Wl,--gc-sections -Wl,--icf=all -Wl,--lto-O3,-O3,-Bsymbolic-functions,--as-needed -march=native -mtune=native -fuse-ld=lld -fcf-protection=none -mharden-sls=none -flto=thin -fwhole-program-vtables -Wl,--push-state -Wl,-whole-archive -lmimalloc -Wl,--pop-state -lpthread -lstdc++ -lm -ldl" \
    -D CMAKE_SHARED_LINKER_FLAGS="-Wl,--lto-CGO3 -Wl,--gc-sections -Wl,--icf=all -Wl,--lto-O3,-O3,-Bsymbolic-functions,--as-needed -march=native -mtune=native -fuse-ld=lld -fcf-protection=none -mharden-sls=none -flto=thin -fwhole-program-vtables -Wl,--push-state -Wl,-whole-archive -lmimalloc -Wl,--pop-state -lpthread -lstdc++ -lm -ldl" \
    -DLLVM_VP_COUNTERS_PER_SITE=10 \
    -DCMAKE_AR="${CPATH}/llvm-ar" \
    -DCMAKE_C_COMPILER="${CPATH}/clang-20" \
    -DCMAKE_CXX_COMPILER="${CPATH}/clang++" \
    -DLLVM_USE_LINKER="${CPATH}/ld.lld" \
    -DCMAKE_RANLIB="${CPATH}/llvm-ranlib" \
    -DCMAKE_INSTALL_PREFIX="${STAGE3_DIR}/install"

# Build with timeout
timeout 120s ninja || true

echo "Merging generated profiles"
cd ${TOPLEV}/stage3-without-sampling/instrumentdata
LD_PRELOAD=/usr/lib/libmimalloc.so ${BOLTPATH}/merge-fdata *.fdata > combined.fdata

optimize_binary() {
    local binary="$1"
    local profile="combined.fdata"

    echo "Optimizing ${binary}"
    LD_PRELOAD=/usr/lib/libmimalloc.so "${BOLTPATH}/llvm-bolt" "${binary}.org" \
        --data "${profile}" \
        -o "${binary}" \
        --dyno-stats \
        --cu-processing-batch-size=64 \
        --eliminate-unreachable\
        --frame-opt=all \
        --icf=all \
        --jump-tables=aggressive \
        --min-branch-clusters \
        --stoke \
        --sctc-mode=always \
        --plt=all \
        --hot-data \
        --hugify \
        --frame-opt-rm-stores \
        --peepholes=all \
        --infer-stale-profile=1 \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reg-reassign \
        --use-aggr-reg-reassign \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit || return 1
}
```

---

### Comment 5 - aaupov

I would try to reduce BOLT aggressiveness first, e.g. leave only the following:
```
        --data "${profile}" \
        -o "${binary}" \
        --dyno-stats \
        # --cu-processing-batch-size=64 \ # no effect without update-debug-info
        # --eliminate-unreachable\ # enabled by default
        # --frame-opt=all \ # try =hot or remove altogether 
        # --icf=all \ # try =safe
        # --jump-tables=aggressive \ # BOLT makes a sane default
        # --min-branch-clusters \ # not sure about effectiveness
        # --stoke \ # likewise
        # --sctc-mode=always \ # BOLT makes a sane default
        --plt=all \
        # --hot-data \ # not sure about effectiveness
        --hugify \
        # --frame-opt-rm-stores \ # this optimization might be dangerous, please verify
        --peepholes=all \
        # --infer-stale-profile=1 \ # not needed if you're optimizing the same profiled binary
        --x86-strip-redundant-address-size \ 
        --indirect-call-promotion=all \ 
        # --reg-reassign \ # might be dangerous
        # --use-aggr-reg-reassign \ # likewise
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit || return 1
```

If you have interest in improving BOLT, it would be great if you could narrow down the issue to a specific BOLT pass and binary function, I can help with that.

---

### Comment 6 - ms178

@aaupov 

Reducing the BOLT options indeed seems to have worked to produce working binaries.

```
        --dyno-stats \
        --icf=safe \
        --plt=all \
        --hugify \
        --peepholes=all \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit || return 1
```
The stats also look very similar.

Old Stats:

```
Profile from 1741 files merged.
Optimizing /home/marcus/toolchain/llvm/stage2-prof-use-lto/install/bin/clang-20
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: e4f03b158c97098e1835cc1f00d0175398974f98
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0xb600000, offset 0xb600000
BOLT-INFO: enabling relocation mode
BOLT-WARNING: Failed to analyze 1301 relocations
BOLT-INFO: pre-processing profile using branch profile reader
BOLT-INFO: 14463 out of 76900 functions in the binary (18.8%) have non-empty execution profile
BOLT-INFO: 354 functions with profile could not be optimized
BOLT-INFO: profile for 1 objects was ignored
BOLT-INFO: among the hottest 1000 functions top 5% function CFG discontinuity is 0.00%
BOLT-INFO: validate-mem-refs updated 8 object references
BOLT-INFO: 2624414 instructions were shortened
BOLT-INFO: removed 834 empty blocks
BOLT-INFO: ICF folded 5 out of 77303 functions in 3 passes. 1 functions had jump tables.
BOLT-INFO: Removing all identical functions will save 1.67 KB of code space. Folded functions were called 773463 times based on profile.
BOLT-INFO: ICP Total indirect calls = 2273540058, 1367 callsites cover 99% of all indirect calls
BOLT-INFO: ICP total indirect callsites with profile = 2079
BOLT-INFO: ICP total jump table callsites = 294
BOLT-INFO: ICP total number of calls = 6644315164
BOLT-INFO: ICP percentage of calls that are indirect = 34.0%
BOLT-INFO: ICP percentage of indirect calls that can be optimized = 64.5%
BOLT-INFO: ICP percentage of indirect callsites that are optimized = 62.4%
BOLT-INFO: ICP number of method load elimination candidates = 0
BOLT-INFO: ICP percentage of method calls candidates that have loads eliminated = 0.0%
BOLT-INFO: ICP percentage of indirect branches that are optimized = 0.0%
BOLT-INFO: ICP percentage of jump table callsites that are optimized = 0.0%
BOLT-INFO: ICP number of jump table callsites that can use hot indices = 0
BOLT-INFO: ICP percentage of jump table callsites that use hot indices = 0.0%
BOLT-INFO: Reg Reassignment Pass Stats:
           7338 functions affected.
           16756 static bytes saved.
           82560382034 dynamic bytes saved.
BOLT-INFO: 595561 PLT calls in the binary were optimized.
BOLT-INFO: basic block reordering modified layout of 11723 functions (81.06% of profiled, 15.17% of total)
BOLT-INFO: UCE removed 4 blocks and 159 bytes of code
BOLT-INFO: splitting separates 12605327 hot bytes from 19054242 cold bytes (39.82% of split functions is hot).
BOLT-INFO: 56 Functions were reordered by LoopInversionPass
BOLT-INFO: splitting separates 5522890 hot bytes from 16079535 cold bytes (25.57% of split functions is hot).
BOLT-INFO: redirection of never-taken jumps saved 99704 bytes hot section code size and 1868832 bytes total code size
BOLT-INFO: program-wide dynostats after all optimizations before SCTC and FOP:

        225148690034 : executed forward branches
         68516387232 : taken forward branches
         64701052898 : executed backward branches
         28910051542 : taken backward branches
         26766942652 : executed unconditional branches
         33700347521 : all function calls
          5496764201 : indirect calls
          3183152077 : PLT calls
       1834349424989 : executed instructions
        409511408792 : executed load instructions
        215237501815 : executed store instructions
          3997495910 : taken jump table branches
                   0 : taken unknown indirect branches
        316616685584 : total branches
        124193381426 : taken branches
        192423304158 : non-taken conditional branches
         97426438774 : taken conditional branches
        289849742932 : all conditional branches

        249654219975 : executed forward branches (+10.9%)
         14786426010 : taken forward branches (-78.4%)
         41972830350 : executed backward branches (-35.1%)
         22372325387 : taken backward branches (-22.6%)
          8112953888 : executed unconditional branches (-69.7%)
         30517195444 : all function calls (-9.4%)
          4034884939 : indirect calls (-26.6%)
                   0 : PLT calls (-100.0%)
       1816111348845 : executed instructions (-1.0%)
        409819666319 : executed load instructions (+0.1%)
        215237501815 : executed store instructions (=)
          3997495910 : taken jump table branches (=)
                   0 : taken unknown indirect branches (=)
        299740004213 : total branches (-5.3%)
         45271705285 : taken branches (-63.5%)
        254468298928 : non-taken conditional branches (+32.2%)
         37158751397 : taken conditional branches (-61.9%)
        291627050325 : all conditional branches (+0.6%)

STOKE-INFO: begin of stoke pass
STOKE-INFO: output file is required
BOLT-INFO: SCTC: patched 425 tail calls (425 forward) tail calls (0 backward) from a total of 426 while removing 2608 double jumps and removing 544 basic blocks totalling 2384 bytes of code. CTCs total execution count is 70453647 and the number of times CTCs are taken is 64468541
BOLT-INFO: Peephole: 1906776 double jumps patched.
BOLT-INFO: Peephole: 6572 tail call traps inserted.
BOLT-INFO: Peephole: 0 useless conditional branches removed.
BOLT-INFO: FOP optimized 437 redundant load(s) and 17 unused store(s)
BOLT-INFO: Frequency of redundant loads is 28435026 and frequency of unused stores is 5690614
BOLT-INFO: Frequency of loads changed to use a register is 28435026 and frequency of loads changed to use an immediate is 0
BOLT-INFO: FOP deleted 154 load(s) (dyn count: 4478700) and 17 store(s)
BOLT-INFO: FRAME ANALYSIS: 1391 function(s) were not optimized.
BOLT-INFO: FRAME ANALYSIS: 36992 function(s) (66.8% dyn cov) could not have its frame indices restored.
BOLT-INFO: Shrink wrapping moved 1592 spills inserting load/stores and 17 spills inserting push/pops
BOLT-INFO: Shrink wrapping reduced 5750461848 store executions (0.3% total instructions executed, 2.7% store instructions)
BOLT-INFO: Shrink wrapping failed at reducing 0 store executions (0.0% total instructions executed, 0.0% store instructions)
BOLT-INFO: Allocation combiner: 1532 empty spaces coalesced (dyn count: 5406928588).
BOLT-INFO: setting __bolt_runtime_start to 0x13a4d360
BOLT-INFO: setting __hot_start to 0xb800000
BOLT-INFO: setting __hot_end to 0xc72f8ce
Optimizing /home/marcus/toolchain/llvm/stage2-prof-use-lto/install/bin/lld
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: e4f03b158c97098e1835cc1f00d0175398974f98
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x7e00000, offset 0x7e00000
BOLT-INFO: enabling relocation mode
BOLT-WARNING: Failed to analyze 1354 relocations
BOLT-INFO: pre-processing profile using branch profile reader
BOLT-INFO: 8442 out of 63269 functions in the binary (13.3%) have non-empty execution profile
BOLT-INFO: 119 functions with profile could not be optimized
BOLT-WARNING: 253 (3.0% of all profiled) functions have invalid (possibly stale) profile. Use -report-stale to see the list.
BOLT-WARNING: 13227300073 out of 201798121390 samples in the binary (6.6%) belong to functions with invalid (possibly stale) profile.
BOLT-INFO: 0 stale functions (0.0% of all stale) have matching block count.
BOLT-INFO: 0 stale blocks (-nan% of all stale) have matching icount.
BOLT-INFO: profile for 6260 objects was ignored
BOLT-INFO: among the hottest 1000 functions top 5% function CFG discontinuity is 0.00%
BOLT-INFO: validate-mem-refs updated 13 object references
BOLT-INFO: 1563134 instructions were shortened
BOLT-INFO: removed 480 empty blocks
BOLT-INFO: ICF folded 7 out of 63594 functions in 3 passes. 1 functions had jump tables.
BOLT-INFO: Removing all identical functions will save 4.35 KB of code space. Folded functions were called 773463 times based on profile.
BOLT-INFO: ICP Total indirect calls = 582440257, 520 callsites cover 99% of all indirect calls
BOLT-INFO: ICP total indirect callsites with profile = 960
BOLT-INFO: ICP total jump table callsites = 109
BOLT-INFO: ICP total number of calls = 1643493769
BOLT-INFO: ICP percentage of calls that are indirect = 35.2%
BOLT-INFO: ICP percentage of indirect calls that can be optimized = 71.4%
BOLT-INFO: ICP percentage of indirect callsites that are optimized = 50.1%
BOLT-INFO: ICP number of method load elimination candidates = 0
BOLT-INFO: ICP percentage of method calls candidates that have loads eliminated = 0.0%
BOLT-INFO: ICP percentage of indirect branches that are optimized = 0.0%
BOLT-INFO: ICP percentage of jump table callsites that are optimized = 0.0%
BOLT-INFO: ICP number of jump table callsites that can use hot indices = 0
BOLT-INFO: ICP percentage of jump table callsites that use hot indices = 0.0%
BOLT-INFO: Reg Reassignment Pass Stats:
           3943 functions affected.
           10647 static bytes saved.
           52758290230 dynamic bytes saved.
BOLT-INFO: 395797 PLT calls in the binary were optimized.
BOLT-INFO: basic block reordering modified layout of 6268 functions (74.25% of profiled, 9.86% of total)
BOLT-INFO: UCE removed 4 blocks and 159 bytes of code
BOLT-INFO: splitting separates 6612312 hot bytes from 7269227 cold bytes (47.63% of split functions is hot).
BOLT-INFO: 24 Functions were reordered by LoopInversionPass
BOLT-INFO: splitting separates 2395092 hot bytes from 5871191 cold bytes (28.97% of split functions is hot).
BOLT-INFO: redirection of never-taken jumps saved 76588 bytes hot section code size and 1028240 bytes total code size
BOLT-INFO: program-wide dynostats after all optimizations before SCTC and FOP:

        102152176941 : executed forward branches
         32076361804 : taken forward branches
         40539969036 : executed backward branches
         18394796784 : taken backward branches
         14497763713 : executed unconditional branches
         13706127809 : all function calls
          2519115434 : indirect calls
          1671443892 : PLT calls
        889388107169 : executed instructions
        188808789105 : executed load instructions
         94966820406 : executed store instructions
          1513018445 : taken jump table branches
                   0 : taken unknown indirect branches
        157189909690 : total branches
         64968922301 : taken branches
         92220987389 : non-taken conditional branches
         50471158588 : taken conditional branches
        142692145977 : all conditional branches

        117077723661 : executed forward branches (+14.6%)
          7356361855 : taken forward branches (-77.1%)
         26329868890 : executed backward branches (-35.1%)
         15055295003 : taken backward branches (-18.2%)
          3913938452 : executed unconditional branches (-73.0%)
         12034683917 : all function calls (-12.2%)
          1941782156 : indirect calls (-22.9%)
                   0 : PLT calls (-100.0%)
        879125427835 : executed instructions (-1.2%)
        188944924265 : executed load instructions (+0.1%)
         94966820406 : executed store instructions (=)
          1513018445 : taken jump table branches (=)
                   0 : taken unknown indirect branches (=)
        147321531003 : total branches (-6.3%)
         26325595310 : taken branches (-59.5%)
        120995935693 : non-taken conditional branches (+31.2%)
         22411656858 : taken conditional branches (-55.6%)
        143407592551 : all conditional branches (+0.5%)

STOKE-INFO: begin of stoke pass
STOKE-INFO: output file is required
BOLT-INFO: SCTC: patched 419 tail calls (419 forward) tail calls (0 backward) from a total of 420 while removing 52 double jumps and removing 446 basic blocks totalling 2164 bytes of code. CTCs total execution count is 8786526 and the number of times CTCs are taken is 8712664
BOLT-INFO: Peephole: 1058007 double jumps patched.
BOLT-INFO: Peephole: 6017 tail call traps inserted.
BOLT-INFO: Peephole: 0 useless conditional branches removed.
BOLT-INFO: FOP optimized 399 redundant load(s) and 14 unused store(s)
BOLT-INFO: Frequency of redundant loads is 27615219 and frequency of unused stores is 5690614
BOLT-INFO: Frequency of loads changed to use a register is 27615219 and frequency of loads changed to use an immediate is 0
BOLT-INFO: FOP deleted 146 load(s) (dyn count: 4291959) and 14 store(s)
BOLT-INFO: FRAME ANALYSIS: 923 function(s) were not optimized.
BOLT-INFO: FRAME ANALYSIS: 25942 function(s) (63.6% dyn cov) could not have its frame indices restored.
BOLT-INFO: Shrink wrapping moved 930 spills inserting load/stores and 16 spills inserting push/pops
BOLT-INFO: Shrink wrapping reduced 2792752822 store executions (0.3% total instructions executed, 2.9% store instructions)
BOLT-INFO: Shrink wrapping failed at reducing 0 store executions (0.0% total instructions executed, 0.0% store instructions)
BOLT-INFO: Allocation combiner: 908 empty spaces coalesced (dyn count: 2672819785).
BOLT-INFO: setting __bolt_runtime_start to 0xd209de0
BOLT-INFO: setting __hot_start to 0x8000000
BOLT-INFO: setting __hot_end to 0x88d7134
```

Stats with reduced config:

```
Profile from 1745 files merged.
Optimizing /home/marcus/toolchain/llvm/stage2-prof-use-lto/install/bin/clang-20
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: e4f03b158c97098e1835cc1f00d0175398974f98
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0xb600000, offset 0xb600000
BOLT-INFO: enabling relocation mode
BOLT-INFO: enabling lite mode
BOLT-WARNING: Failed to analyze 1301 relocations
BOLT-INFO: pre-processing profile using branch profile reader
BOLT-INFO: forcing -jump-tables=move as PIC jump table was detected in function _ZN5clang13TreeTransformIN12_GLOBAL__N_120TemplateInstantiatorEE13TransformTypeENS_8QualTypeE/1(*2)
BOLT-INFO: 14465 out of 76900 functions in the binary (18.8%) have non-empty execution profile
BOLT-INFO: 354 functions with profile could not be optimized
BOLT-INFO: profile for 1 objects was ignored
BOLT-INFO: among the hottest 1000 functions top 5% function CFG discontinuity is 0.00%
BOLT-INFO: 778892 instructions were shortened
BOLT-INFO: removed 813 empty blocks
BOLT-INFO: ICP Total indirect calls = 2274207689, 1367 callsites cover 99% of all indirect calls
BOLT-INFO: ICP total indirect callsites with profile = 2079
BOLT-INFO: ICP total jump table callsites = 294
BOLT-INFO: ICP total number of calls = 6644457440
BOLT-INFO: ICP percentage of calls that are indirect = 34.0%
BOLT-INFO: ICP percentage of indirect calls that can be optimized = 64.5%
BOLT-INFO: ICP percentage of indirect callsites that are optimized = 62.4%
BOLT-INFO: ICP number of method load elimination candidates = 0
BOLT-INFO: ICP percentage of method calls candidates that have loads eliminated = 0.0%
BOLT-INFO: ICP percentage of indirect branches that are optimized = 0.0%
BOLT-INFO: ICP percentage of jump table callsites that are optimized = 0.0%
BOLT-INFO: ICP number of jump table callsites that can use hot indices = 0
BOLT-INFO: ICP percentage of jump table callsites that use hot indices = 0.0%
BOLT-INFO: 162521 PLT calls in the binary were optimized.
BOLT-INFO: basic block reordering modified layout of 11723 functions (81.04% of profiled, 15.17% of total)
BOLT-INFO: splitting separates 12619975 hot bytes from 19048864 cold bytes (39.85% of split functions is hot).
BOLT-INFO: 55 Functions were reordered by LoopInversionPass
BOLT-INFO: splitting separates 5532904 hot bytes from 15993780 cold bytes (25.70% of split functions is hot).
BOLT-INFO: program-wide dynostats after all optimizations before SCTC and FOP:

        225079404134 : executed forward branches
         68475968819 : taken forward branches
         64608629626 : executed backward branches
         28855067944 : taken backward branches
         26756273046 : executed unconditional branches
         33689703855 : all function calls
          5496071130 : indirect calls
          3182050006 : PLT calls
       1833392896717 : executed instructions
        409358999012 : executed load instructions
        215177641722 : executed store instructions
          3997367103 : taken jump table branches
                   0 : taken unknown indirect branches
        316444306806 : total branches
        124087309809 : taken branches
        192356996997 : non-taken conditional branches
         97331036763 : taken conditional branches
        289688033760 : all conditional branches

        249384702758 : executed forward branches (+10.8%)
         14779112065 : taken forward branches (-78.4%)
         42081426356 : executed backward branches (-34.9%)
         22314238938 : taken backward branches (-22.7%)
          8117953397 : executed unconditional branches (-69.7%)
         30507653849 : all function calls (-9.4%)
          4034085978 : indirect calls (-26.6%)
                   0 : PLT calls (-100.0%)
       1815769822496 : executed instructions (-1.0%)
        409667327480 : executed load instructions (+0.1%)
        215177641722 : executed store instructions (=)
          3997367103 : taken jump table branches (=)
                   0 : taken unknown indirect branches (=)
        299584082511 : total branches (-5.3%)
         45211304400 : taken branches (-63.6%)
        254372778111 : non-taken conditional branches (+32.2%)
         37093351003 : taken conditional branches (-61.9%)
        291466129114 : all conditional branches (+0.6%)

BOLT-INFO: SCTC: patched 74 tail calls (74 forward) tail calls (0 backward) from a total of 74 while removing 2 double jumps and removing 76 basic blocks totalling 380 bytes of code. CTCs total execution count is 70456088 and the number of times CTCs are taken is 64470787
BOLT-INFO: Peephole: 120 double jumps patched.
BOLT-INFO: Peephole: 1521 tail call traps inserted.
BOLT-INFO: Peephole: 0 useless conditional branches removed.
BOLT-INFO: setting __bolt_runtime_start to 0xdfe7790
BOLT-INFO: setting __hot_start to 0xb800000
BOLT-INFO: setting __hot_end to 0xc743a0d
Optimizing /home/marcus/toolchain/llvm/stage2-prof-use-lto/install/bin/lld
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: e4f03b158c97098e1835cc1f00d0175398974f98
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x7e00000, offset 0x7e00000
BOLT-INFO: enabling relocation mode
BOLT-INFO: enabling lite mode
BOLT-WARNING: Failed to analyze 1354 relocations
BOLT-INFO: pre-processing profile using branch profile reader
BOLT-INFO: forcing -jump-tables=move as PIC jump table was detected in function _ZNK4llvm11Instruction12getSuccessorEj
BOLT-INFO: 8442 out of 63269 functions in the binary (13.3%) have non-empty execution profile
BOLT-INFO: 119 functions with profile could not be optimized
BOLT-WARNING: 253 (3.0% of all profiled) functions have invalid (possibly stale) profile. Use -report-stale to see the list.
BOLT-WARNING: 13222130277 out of 201602198256 samples in the binary (6.6%) belong to functions with invalid (possibly stale) profile.
BOLT-INFO: 0 stale functions (0.0% of all stale) have matching block count.
BOLT-INFO: 0 stale blocks (-nan% of all stale) have matching icount.
BOLT-INFO: profile for 6262 objects was ignored
BOLT-INFO: among the hottest 1000 functions top 5% function CFG discontinuity is 0.00%
BOLT-INFO: 358139 instructions were shortened
BOLT-INFO: removed 441 empty blocks
BOLT-INFO: ICP Total indirect calls = 582693492, 520 callsites cover 99% of all indirect calls
BOLT-INFO: ICP total indirect callsites with profile = 960
BOLT-INFO: ICP total jump table callsites = 109
BOLT-INFO: ICP total number of calls = 1643422752
BOLT-INFO: ICP percentage of calls that are indirect = 35.2%
BOLT-INFO: ICP percentage of indirect calls that can be optimized = 71.4%
BOLT-INFO: ICP percentage of indirect callsites that are optimized = 50.1%
BOLT-INFO: ICP number of method load elimination candidates = 0
BOLT-INFO: ICP percentage of method calls candidates that have loads eliminated = 0.0%
BOLT-INFO: ICP percentage of indirect branches that are optimized = 0.0%
BOLT-INFO: ICP percentage of jump table callsites that are optimized = 0.0%
BOLT-INFO: ICP number of jump table callsites that can use hot indices = 0
BOLT-INFO: ICP percentage of jump table callsites that use hot indices = 0.0%
BOLT-INFO: 85278 PLT calls in the binary were optimized.
BOLT-INFO: basic block reordering modified layout of 6268 functions (74.25% of profiled, 9.86% of total)
BOLT-INFO: splitting separates 6618673 hot bytes from 7267022 cold bytes (47.67% of split functions is hot).
BOLT-INFO: 23 Functions were reordered by LoopInversionPass
BOLT-INFO: splitting separates 2413886 hot bytes from 5864079 cold bytes (29.16% of split functions is hot).
BOLT-INFO: program-wide dynostats after all optimizations before SCTC and FOP:

        102081672049 : executed forward branches
         32037415556 : taken forward branches
         40446722560 : executed backward branches
         18340857274 : taken backward branches
         14486644513 : executed unconditional branches
         13695719052 : all function calls
          2518050237 : indirect calls
          1670180212 : PLT calls
        888415126321 : executed instructions
        188650781032 : executed load instructions
         94905804829 : executed store instructions
          1512754546 : taken jump table branches
                   0 : taken unknown indirect branches
        157015039122 : total branches
         64864917343 : taken branches
         92150121779 : non-taken conditional branches
         50378272830 : taken conditional branches
        142528394609 : all conditional branches

        117055675900 : executed forward branches (+14.7%)
          7343691555 : taken forward branches (-77.1%)
         26188506567 : executed backward branches (-35.3%)
         15014456310 : taken backward branches (-18.1%)
          3905927318 : executed unconditional branches (-73.0%)
         12025538840 : all function calls (-12.2%)
          1939763767 : indirect calls (-23.0%)
                   0 : PLT calls (-100.0%)
        878459105862 : executed instructions (-1.1%)
        188786162232 : executed load instructions (+0.1%)
         94905804829 : executed store instructions (=)
          1512754546 : taken jump table branches (=)
                   0 : taken unknown indirect branches (=)
        147150109785 : total branches (-6.3%)
         26264075183 : taken branches (-59.5%)
        120886034602 : non-taken conditional branches (+31.2%)
         22358147865 : taken conditional branches (-55.6%)
        143244182467 : all conditional branches (+0.5%)

BOLT-INFO: SCTC: patched 24 tail calls (24 forward) tail calls (0 backward) from a total of 24 while removing 2 double jumps and removing 26 basic blocks totalling 130 bytes of code. CTCs total execution count is 8786066 and the number of times CTCs are taken is 8712613
BOLT-INFO: Peephole: 71 double jumps patched.
BOLT-INFO: Peephole: 1066 tail call traps inserted.
BOLT-INFO: Peephole: 0 useless conditional branches removed.
BOLT-INFO: setting __bolt_runtime_start to 0x9419780
BOLT-INFO: setting __hot_start to 0x8000000
BOLT-INFO: setting __hot_end to 0x88df36e
```

---

### Comment 7 - ms178

@aaupov I've now tested several variants, and I could successfully narrow the problem down to the `--use-aggr-reg-reassign` option. As using that used to work with an earlier LLVM-20git build (3def49cb64ec1298290724081bd37dbdeb2ea5f8), a change between this revision and (e4f03b158c97098e1835cc1f00d0175398974f98) might have surfaced or triggered this problem.

I've also tested that this one produced working binaries:

```
        --dyno-stats \
        --lite=false \
        --cu-processing-batch-size=64 \
        --eliminate-unreachable\
        --frame-opt=all \
        --icf=all \
        --jump-tables=aggressive \
        --min-branch-clusters \
        --stoke \
        --sctc-mode=always \
        --plt=all \
        --hot-data \
        --hugify \
        --frame-opt-rm-stores \
        --peepholes=all \
        --infer-stale-profile=1 \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reg-reassign \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit \
        --redirect-never-taken-jumps || return 1
```

While this one doesn't:

```
        --dyno-stats \
        --icf=safe \
        --plt=all \
        --hugify \
        --peepholes=all \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit \
        --reg-reassign \
        --use-aggr-reg-reassign || return 1
```
Getting rid of `--use-aggr-reg-reassign`  while keeping `--reg-reassign` also lead to working clang/lld binaries.

---

### Comment 8 - Andarwinux

> Getting rid of `--use-aggr-reg-reassign` while keeping `--reg-reassign` also lead to working clang/lld binaries.

It looks like you found this set of options from my llvm script?  I've found this before, `--use-aggr-reg-reassign` causes BOLT-optimized clang to crash, but I had `-mllvm -enable-ipra` in my CFLAGS, and once that was removed there was no problem, so I didn't report this (btw ipra just reduces performance on x86).

---

### Comment 9 - ms178

> > Getting rid of `--use-aggr-reg-reassign` while keeping `--reg-reassign` also lead to working clang/lld binaries.
> 
> It looks like you found this set of options from my llvm script? I've found this before, `--use-aggr-reg-reassign` causes BOLT-optimized clang to crash, but I had `-mllvm -enable-ipra` in my CFLAGS, and once that was removed there was no problem, so I didn't report this (btw ipra just reduces performance on x86).

Heh, you've caught me. :) Indeed I took inspiration by your BOLT options a couple of weeks ago to optimize my build scripts further. By the way, I've BOLTed several other packages for Arch compatible distros during the past month (PKGBUILDS are available in my repo), I hope these will provide an easy to use way for other people to catch these BOLT bugs and improve it further for everyone.

---

### Comment 10 - fhahn

@ms178 IIUC the original crash cannot be reproduced any longer? And this seems to be BOLT mis-compile? In that case, I think we should update the title + labels

---

### Comment 11 - ms178

@fhahn It still can be reproduced, but only when my Clang + LLD binaries have been BOLTed with the mentioned BOLT configuration options.

I cannot say what the root cause is, if BOLT exposes a bug or if the defect is on the BOLT side causing a mis-compile.

If you think that a BOLT bug is more likely, I can change the title of course.

---

### Comment 12 - fhahn

If it is only happening with BOLT, this seems to indicate that it is a BOLT miscompile, causing the crash and the first step should be checking if the changes BOLT makes are correct.

---

### Comment 13 - ms178

I've updated the title and the top post with relevant information that was made in the meantime. As I lack access, I'd like to kindly ask someone to update the labels.

---

### Comment 14 - llvmbot


@llvm/issue-subscribers-bolt

Author: None (ms178)

<details>
**Description:**

I am encountering compiler errors while compiling various projects with BOLTed Clang/LLD binaries that were built with the `--use-aggr-reg-reassign` option, e.g. in the `asn1_compiler` component of the Linux kernel 6.13. There, the faulty binaries crash during the loop vectorization optimization pass. Without that option, a BOLTed Clang/LLD with the same revision worked fine for the same task.

**Environment:**

*   **LLVM Version:** 20.0.0git (commit: e4f03b158c97098e1835cc1f00d0175398974f98)
*   **Target:** x86_64-pc-linux-gnu
*   **Host System:**  CachyOS
*   **Target CPU:** Intel Raptor Lake
*   **Kernel:** Linux Kernel 6.13 (CachyOS patched)

**Steps to Reproduce:**

1.  Clone the CachyOS Linux kernel 6.13 source code. I am using some extra patches available [in my repo](https://github.com/ms178/archpkgbuilds/tree/main/packages/linux-cachyos).
2.  Configure the kernel build with LLVM as the compiler.
3.  Build the kernel using a BOLTed LLVM toolchain which was build with these BOLT options:

```
        --dyno-stats \
        --icf=safe \
        --plt=all \
        --hugify \
        --peepholes=all \
        --x86-strip-redundant-address-size \
        --indirect-call-promotion=all \
        --reorder-blocks=ext-tsp \
        --reorder-functions=cdsort \
        --split-all-cold \
        --split-eh \
        --split-functions \
        --split-strategy=cdsplit \
        --reg-reassign \
        --use-aggr-reg-reassign || return 1
```

**Expected Behavior:**

The `asn1_compiler` should compile successfully without any errors.

**Actual Behavior:**

The compilation crashes with a segmentation fault during the optimization of `scripts/asn1_compiler.c`.

Stack trace:

```
HOSTCC  scripts/asn1_compiler
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/marcus/llvm20/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir scripts/asn1_compiler- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name asn1_compiler.c -function-alignment 5 -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -ffp-contract=fast -fno-rounding-math -ffp-exception-behavior=ignore -mconstructor-aliases -funwind-tables=2 -target-cpu raptorlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +sahf -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +aes -target-feature +bmi2 -target-feature +pclmul -tune-cpu raptorlake -debugger-tuning=gdb -fdebug-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -ffunction-sections -fdata-sections -fsplit-machine-functions -fcoverage-compilation-dir=/tmp/makepkg/linux-cachyos/src/linux-6.13 -resource-dir /home/marcus/llvm20/lib/clang/20 -dependency-file scripts/.asn1_compiler.d -MT scripts/asn1_compiler -I ./scripts/include -I ./include -internal-isystem /home/marcus/llvm20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -source-date-epoch 1737487879 -O3 -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-error -w -std=gnu11 -ferror-limit 19 -fcf-protection=none -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mprefer-vector-width=256 -mllvm -inline-threshold=1000 -mllvm -extra-vectorizer-passes -mllvm -enable-cond-stores-vec -mllvm -slp-vectorize-hor-store -mllvm -enable-loopinterchange -mllvm -enable-loop-distribute -mllvm -enable-unroll-and-jam -mllvm -enable-loop-flatten -mllvm -unroll-runtime-multi-exit -mllvm -aggressive-ext-opt -mllvm -enable-interleaved-mem-accesses -mllvm -enable-masked-interleaved-mem-accesses -mllvm -adce-remove-loops -mllvm -enable-ext-tsp-block-placement -mllvm -enable-gvn-hoist -mllvm -enable-dfa-jump-thread -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/asn1_compiler-456ed7.o -x c scripts/asn1_compiler.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(float2int,lower-constant-intrinsics,chr,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,extra-vector-passes,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,early-cse&lt;&gt;,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-unroll-and-jam),loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "scripts/asn1_compiler.c"
4.      Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "main"
 #<!-- -->0 0x00005e1b5aed88c2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) Signals.cpp:0:0
 #<!-- -->1 0x00005e1b5aed8ccf SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007762de845f50 (/usr/lib/libc.so.6+0x45f50)
 #<!-- -->3 0x00005e1b58dd7acd llvm::X86TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) (.cold) X86TargetTransformInfo.cpp:0:0
 #<!-- -->4 0x00005e1b5bc88c83 llvm::VPWidenCastRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&amp;) const (/home/marcus/llvm20/bin/clang-20+0xce88c83)
 #<!-- -->5 0x00005e1b5bc8322b llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce8322b)
 #<!-- -->6 0x00005e1b5bc761c3 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce761c3)
 #<!-- -->7 0x00005e1b5bc7637d llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xce7637d)
 #<!-- -->8 0x00005e1b5bc23e9c llvm::LoopVectorizationPlanner::cost(llvm::VPlan&amp;, llvm::ElementCount) const LoopVectorize.cpp:0:0
 #<!-- -->9 0x00005e1b5bc24265 llvm::LoopVectorizationPlanner::computeBestVF() LoopVectorize.cpp:0:0
#<!-- -->10 0x00005e1b5bc52312 llvm::LoopVectorizePass::processLoop(llvm::Loop*) LoopVectorize.cpp:0:0
#<!-- -->11 0x00005e1b59b252c1 llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) LoopVectorize.cpp:0:0
#<!-- -->12 0x00005e1b599aed27 llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (.cold) bolt-pseudo.o:0:0
#<!-- -->13 0x00005e1b58af07cd llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) PassBuilder.cpp:0:0
#<!-- -->14 0x00005e1b57cfc769 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x8efc769)
#<!-- -->15 0x00005e1b57cfc11b llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) AMDGPUTargetMachine.cpp:0:0
#<!-- -->16 0x00005e1b57faea24 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x91aea24)
#<!-- -->17 0x00005e1b57fae6a9 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) AMDGPUTargetMachine.cpp:0:0
#<!-- -->18 0x00005e1b57fb7744 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/marcus/llvm20/bin/clang-20+0x91b7744)
#<!-- -->19 0x00005e1b586a8fdf (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->20 0x00005e1b585c1fef clang::emitBackendOutput(clang::CompilerInstance&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/marcus/llvm20/bin/clang-20+0x97c1fef)
#<!-- -->21 0x00005e1b59a64ca2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/marcus/llvm20/bin/clang-20+0xac64ca2)
#<!-- -->22 0x00005e1b57d7a684 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/marcus/llvm20/bin/clang-20+0x8f7a684)
#<!-- -->23 0x00005e1b5873281e clang::FrontendAction::Execute() (/home/marcus/llvm20/bin/clang-20+0x993281e)
#<!-- -->24 0x00005e1b5869a9ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/marcus/llvm20/bin/clang-20+0x989a9ab)
#<!-- -->25 0x00005e1b5869ad9c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/marcus/llvm20/bin/clang-20+0x989ad9c)
#<!-- -->26 0x00005e1b58698b4e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/marcus/llvm20/bin/clang-20+0x9898b4e)
#<!-- -->27 0x00005e1b5869a2f1 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->28 0x00005e1b58697d19 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/marcus/llvm20/bin/clang-20+0x9897d19)
#<!-- -->29 0x00005e1b52a9b752 (/home/marcus/llvm20/bin/clang-20+0x3c9b752)
#<!-- -->30 0x00007762de826187 (/usr/lib/libc.so.6+0x26187)
#<!-- -->31 0x00007762de826237 __libc_start_main (/usr/lib/libc.so.6+0x26237)
#<!-- -->32 0x00005e1b533bce71 _start (/home/marcus/llvm20/bin/clang-20+0x45bce71)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git e4f03b158c97098e1835cc1f00d0175398974f98)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/marcus/llvm20/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.c
clang: note: diagnostic msg: /tmp/asn1_compiler-82f6f2.sh
clang: note: diagnostic msg: 

********************
make[2]: *** [scripts/Makefile.host:114: scripts/asn1_compiler] Fehler 1
make[1]: *** [/tmp/makepkg/linux-cachyos/src/linux-6.13/Makefile:1255: scripts] Fehler 2
make: *** [Makefile:251: __sub-make] Fehler 2
```
[asn1_compiler-82f6f2.c.txt](https://github.com/user-attachments/files/18495726/asn1_compiler-82f6f2.c.txt)
[asn1_compiler-82f6f2.sh.txt](https://github.com/user-attachments/files/18495727/asn1_compiler-82f6f2.sh.txt)
</details>


---

### Comment 15 - ms178

Closing in protest due to (https://seylaw.blogspot.com/2025/05/when-compiler-engineers-act-as-judges.html)

---

