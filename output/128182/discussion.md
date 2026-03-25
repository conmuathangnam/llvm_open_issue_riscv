# Random bug report

**Author:** asbott
**URL:** https://github.com/llvm/llvm-project/issues/128182

## Body

Github does not allow me to attach the files you requested me to attach, so here are google drive links:

https://drive.google.com/file/d/1MWyKAqCx5EEDiFujOtLezvByxiTDDzFk/view?usp=sharing
https://drive.google.com/file/d/11wRltLF4_-O-kGUqHm2sEX3Cq7IGQyOp/view?usp=sharing


PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\Software\\LLVM\\bin\\clang.exe -cc1 -triple x86_64-pc-windows-msvc19.41.34123 -emit-obj -mincremental-linker-compatible -dumpdir test_result\\hello_world-release.exe- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name hello_world.c -mrelocation-model pic -pic-level 2 -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -target-feature +avx -target-feature +avx2 -tune-cpu generic -fdebug-compilation-dir=C:\\One-Std\\tests -fcoverage-compilation-dir=C:\\One-Std\\tests -resource-dir C:\\Software\\LLVM\\lib\\clang\\19 -D DEBUG -I ../vendors -D RUNNING_TESTS -D TESTING_DURATION=2 -internal-isystem C:\\Software\\LLVM\\lib\\clang\\19\\include -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.41.34120\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.41.34120\\atlmfc\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\cppwinrt" -O2 -Wall -Werror -Weverything -Wno-pre-c23-compat -pedantic -std=c23 -ferror-limit 19 -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.41.34123 -fskip-odr-check-in-gmf -fdelayed-template-parsing -vectorize-loops -vectorize-slp -faddrsig -o C:\\Users\\charl\\AppData\\Local\\Temp\\hello_world-33abd5.o -x c hello_world.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<no-nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "hello_world.c"
4.      Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<no-nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split))" on module "hello_world.c"
5.      Running pass "loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>)" on function "spv_emit_expr"
Exception Code: 0xC0000005
 #0 0x00007ff7c655d4f8 (C:\Software\LLVM\bin\clang.exe+0x1a1d4f8)
 #1 0x00007ff7c655a90a (C:\Software\LLVM\bin\clang.exe+0x1a1a90a)
 #2 0x00007ff7c6553a0a (C:\Software\LLVM\bin\clang.exe+0x1a13a0a)
 #3 0x00007ff7c4f7b5c5 (C:\Software\LLVM\bin\clang.exe+0x43b5c5)
 #4 0x00007ff7c4f7b3ab (C:\Software\LLVM\bin\clang.exe+0x43b3ab)
 #5 0x00007ff7c6482cc6 (C:\Software\LLVM\bin\clang.exe+0x1942cc6)
 #6 0x00007ff7c4efef0e (C:\Software\LLVM\bin\clang.exe+0x3bef0e)
 #7 0x00007ff7c4efeeab (C:\Software\LLVM\bin\clang.exe+0x3beeab)
 #8 0x00007ff7c61aecca (C:\Software\LLVM\bin\clang.exe+0x166ecca)
 #9 0x00007ff7c61ae791 (C:\Software\LLVM\bin\clang.exe+0x166e791)
#10 0x00007ff7c61b01e3 (C:\Software\LLVM\bin\clang.exe+0x16701e3)
#11 0x00007ff7c61afe61 (C:\Software\LLVM\bin\clang.exe+0x166fe61)
#12 0x00007ff7c5f77630 (C:\Software\LLVM\bin\clang.exe+0x1437630)
#13 0x00007ff7c5f7732b (C:\Software\LLVM\bin\clang.exe+0x143732b)
#14 0x00007ff7c63e43ab (C:\Software\LLVM\bin\clang.exe+0x18a43ab)
#15 0x00007ff7c63e408b (C:\Software\LLVM\bin\clang.exe+0x18a408b)
#16 0x00007ff7c6530d89 (C:\Software\LLVM\bin\clang.exe+0x19f0d89)
#17 0x00007ff7c6530b1b (C:\Software\LLVM\bin\clang.exe+0x19f0b1b)
#18 0x00007ff7c64e65d6 (C:\Software\LLVM\bin\clang.exe+0x19a65d6)
#19 0x00007ff7c4f30051 (C:\Software\LLVM\bin\clang.exe+0x3f0051)
#20 0x00007ff7c4e1bdb0 (C:\Software\LLVM\bin\clang.exe+0x2dbdb0)
#21 0x00007ff7c5338200 (C:\Software\LLVM\bin\clang.exe+0x7f8200)
#22 0x00007ff7c5338031 (C:\Software\LLVM\bin\clang.exe+0x7f8031)
#23 0x00007ff7c4e1bdb0 (C:\Software\LLVM\bin\clang.exe+0x2dbdb0)
#24 0x00007ff7c4e17468 (C:\Software\LLVM\bin\clang.exe+0x2d7468)
#25 0x00007ff7c4e0df62 (C:\Software\LLVM\bin\clang.exe+0x2cdf62)
#26 0x00007ff7c61b2baa (C:\Software\LLVM\bin\clang.exe+0x1672baa)
#27 0x00007ff7c66a2a88 (C:\Software\LLVM\bin\clang.exe+0x1b62a88)
#28 0x00007ff7c5103b81 (C:\Software\LLVM\bin\clang.exe+0x5c3b81)
#29 0x00007ff7c5103712 (C:\Software\LLVM\bin\clang.exe+0x5c3712)
#30 0x00007ff7c5100486 (C:\Software\LLVM\bin\clang.exe+0x5c0486)
#31 0x00007ff7c50fd445 (C:\Software\LLVM\bin\clang.exe+0x5bd445)
#32 0x00007ff7c50fbdde (C:\Software\LLVM\bin\clang.exe+0x5bbdde)
#33 0x00007ff7c4f8348a (C:\Software\LLVM\bin\clang.exe+0x44348a)
#34 0x00007ff7c4f7fee3 (C:\Software\LLVM\bin\clang.exe+0x43fee3)
#35 0x00007ff7c6742d58 (C:\Software\LLVM\bin\clang.exe+0x1c02d58)
#36 0x00007ffdd9b1259d (C:\windows\System32\KERNEL32.DLL+0x1259d)
#37 0x00007ffddb4aaf38 (C:\windows\SYSTEM32\ntdll.dll+0x5af38)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.6
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\Software\LLVM\bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: C:\Users\charl\AppData\Local\Temp\hello_world-34f3e2.c
clang: note: diagnostic msg: C:\Users\charl\AppData\Local\Temp\hello_world-34f3e2.sh
clang: note: diagnostic msg:

********************

## Comments

### Comment 1 - asbott

I should note that this only happened once and I am not able to reproduce the crash with the exact same code and exact same command line arguments.

---

