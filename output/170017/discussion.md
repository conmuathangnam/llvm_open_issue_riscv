# [armv7l] llc crash in llvm::ARMAsmPrinter::emitXXStructor

**Author:** Majiir
**URL:** https://github.com/llvm/llvm-project/issues/170017
**Status:** Closed
**Labels:** duplicate, backend:ARM, crash
**Closed Date:** 2025-11-30T06:53:05Z

## Body

I found this crash while cross-compiling `ghc` for `armv7l-linux` on an `x86_64-linux` host. The nixpkgs issue is https://github.com/NixOS/nixpkgs/issues/466116.

Reproduction steps:

1. Install Nix
2. Check out [nixpkgs](https://github.com/NixOS/nixpkgs) at latest `master`
3. `nix build .#pkgsCross.armv7l-hf-multiplatform.buildPackages.ghc`

Alternatively, to get a shell in the build environment:

1. Install Nix
2. Check out [nixpkgs](https://github.com/NixOS/nixpkgs) at latest `master`
3. `mkdir build && cd build`
4. `nix develop .#pkgsCross.armv7l-hf-multiplatform.buildPackages.ghc`
5. `genericBuild`

The build will keep going after the failure, so scroll up to find the crash. You can then re-run the failing command with different arguments.

Here is the build log output:

```
checking sys/event.h usability... Command line: _build/stage0/bin/armv7l-unknown-linux-gnueabihf-ghc -Wall -Wcompat -hisuf p_hi -osuf p_o -hcsuf p_hc -static -prof -hide-all-packages -no-user-package-db '-package-env -' '-package-db _build/stage1/inplace/package.conf.d' '-this-unit-id ghc-bignum-1.3-945c' '-this-package-name ghc-bignum' '-package-id ghc-prim-0.12.0-e538' -i -i/build/ghc-9.10.3-source/_build/stage1/libraries/ghc-bignum/build -i/build/ghc-9.10.3-source/_build/stage1/libraries/ghc-bignum/build/autogen -i/build/ghc-9.10.3-source/libraries/ghc-bignum/src/ -Irts/include -I_build/stage1/libraries/ghc-bignum/build -I_build/stage1/libraries/ghc-bignum/build/include/ -Ilibraries/ghc-bignum/include/ -I/nix/store/lv8cb0c13h34s54d1fszgh15x1y8rp0y-numactl-armv7l-unknown-linux-gnueabihf-2.0.18-dev/include -I/nix/store/80vp2xfhmqvb3lns61q2min5x3z9i83c-libffi-armv7l-unknown-linux-gnueabihf-3.5.2-dev/include -I/build/ghc-9.10.3-source/rts/include -I/build/ghc-9.10.3-source/_build/stage1/rts/build/include -optP-include -optP_build/stage1/libraries/ghc-bignum/build/autogen/cabal_macros.h -optc-marm -optP-DBIGNUM_GMP -outputdir _build/stage1/libraries/ghc-bignum/build -fdiagnostics-color=always -Wall -this-unit-id ghc-bignum -fno-catch-nonexhaustive-cases -XHaskell2010 -no-global-package-db -package-db=/build/ghc-9.10.3-source/_build/stage1/inplace/package.conf.d -ghcversion-file=rts/include/ghcversion.h -ghcversion-file=rts/include/ghcversion.h -Wnoncanonical-monad-instances -optc-Wno-error=inline -c libraries/ghc-bignum/src/GHC/Num/Backend/Selected.hs -o _build/stage1/libraries/ghc-bignum/build/GHC/Num/Backend/Selected.p_o -fsplit-sections -O -H64m -O2 -haddock -fllvm -fPIC -fexternal-dynamic-refs -Wno-deprecated-flags
===> Command failed with error code: 1
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc -O2 -relocation-model=pic -mcpu=generic /build/ghc55393_0/ghc_7.bc -o /build/ghc55393_0/ghc_8.lm_s
 #0 0x00007fffeec3d53b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x119253b)
 #1 0x00007fffeec3aa7a SignalHandler(int) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x118fa7a)
 #2 0x00007fffed55b9c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #3 0x00007ffff265ffc4 llvm::ARMAsmPrinter::emitXXStructor(llvm::DataLayout const&, llvm::Constant const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x4bb4fc4)
 #4 0x00007fffefa1f39a llvm::AsmPrinter::emitXXStructorList(llvm::DataLayout const&, llvm::Constant const*, bool) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f7439a)
 #5 0x00007fffefa1328e llvm::AsmPrinter::emitSpecialLLVMGlobal(llvm::GlobalVariable const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f6828e)
 #6 0x00007fffefa1e723 llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f73723)
 #7 0x00007fffefa163ca llvm::AsmPrinter::doFinalization(llvm::Module&) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f6b3ca)
 #8 0x00007fffeee16f4d llvm::FPPassManager::doFinalization(llvm::Module&) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x136bf4d)
 #9 0x00007fffeee21ee9 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1376ee9)
#10 0x00005555555747c4 compileModule(char**, llvm::LLVMContext&) (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x207c4)
#11 0x000055555556a389 main (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x16389)
#12 0x00007fffed5444d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#13 0x00007fffed54459b __libc_start_main@GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#14 0x000055555556b155 _start (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x17155)
`llc' failed in phase `LLVM Compiler'. (Exit code: -11)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Majiir Paktu (Majiir)

<details>
I found this crash while cross-compiling `ghc` for `armv7l-linux` on an `x86_64-linux` host. The nixpkgs issue is https://github.com/NixOS/nixpkgs/issues/466116.

Reproduction steps:

1. Install Nix
2. Check out [nixpkgs](https://github.com/NixOS/nixpkgs) at latest `master`
3. `nix build .#pkgsCross.armv7l-hf-multiplatform.buildPackages.ghc`

Alternatively, to get a shell in the build environment:

1. Install Nix
2. Check out [nixpkgs](https://github.com/NixOS/nixpkgs) at latest `master`
3. `mkdir build &amp;&amp; cd build`
4. `nix develop .#pkgsCross.armv7l-hf-multiplatform.buildPackages.ghc`
5. `genericBuild`

The build will keep going after the failure, so scroll up to find the crash. You can then re-run the failing command with different arguments.

Here is the build log output:

```
checking sys/event.h usability... Command line: _build/stage0/bin/armv7l-unknown-linux-gnueabihf-ghc -Wall -Wcompat -hisuf p_hi -osuf p_o -hcsuf p_hc -static -prof -hide-all-packages -no-user-package-db '-package-env -' '-package-db _build/stage1/inplace/package.conf.d' '-this-unit-id ghc-bignum-1.3-945c' '-this-package-name ghc-bignum' '-package-id ghc-prim-0.12.0-e538' -i -i/build/ghc-9.10.3-source/_build/stage1/libraries/ghc-bignum/build -i/build/ghc-9.10.3-source/_build/stage1/libraries/ghc-bignum/build/autogen -i/build/ghc-9.10.3-source/libraries/ghc-bignum/src/ -Irts/include -I_build/stage1/libraries/ghc-bignum/build -I_build/stage1/libraries/ghc-bignum/build/include/ -Ilibraries/ghc-bignum/include/ -I/nix/store/lv8cb0c13h34s54d1fszgh15x1y8rp0y-numactl-armv7l-unknown-linux-gnueabihf-2.0.18-dev/include -I/nix/store/80vp2xfhmqvb3lns61q2min5x3z9i83c-libffi-armv7l-unknown-linux-gnueabihf-3.5.2-dev/include -I/build/ghc-9.10.3-source/rts/include -I/build/ghc-9.10.3-source/_build/stage1/rts/build/include -optP-include -optP_build/stage1/libraries/ghc-bignum/build/autogen/cabal_macros.h -optc-marm -optP-DBIGNUM_GMP -outputdir _build/stage1/libraries/ghc-bignum/build -fdiagnostics-color=always -Wall -this-unit-id ghc-bignum -fno-catch-nonexhaustive-cases -XHaskell2010 -no-global-package-db -package-db=/build/ghc-9.10.3-source/_build/stage1/inplace/package.conf.d -ghcversion-file=rts/include/ghcversion.h -ghcversion-file=rts/include/ghcversion.h -Wnoncanonical-monad-instances -optc-Wno-error=inline -c libraries/ghc-bignum/src/GHC/Num/Backend/Selected.hs -o _build/stage1/libraries/ghc-bignum/build/GHC/Num/Backend/Selected.p_o -fsplit-sections -O -H64m -O2 -haddock -fllvm -fPIC -fexternal-dynamic-refs -Wno-deprecated-flags
===&gt; Command failed with error code: 1
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc -O2 -relocation-model=pic -mcpu=generic /build/ghc55393_0/ghc_7.bc -o /build/ghc55393_0/ghc_8.lm_s
 #<!-- -->0 0x00007fffeec3d53b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x119253b)
 #<!-- -->1 0x00007fffeec3aa7a SignalHandler(int) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x118fa7a)
 #<!-- -->2 0x00007fffed55b9c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #<!-- -->3 0x00007ffff265ffc4 llvm::ARMAsmPrinter::emitXXStructor(llvm::DataLayout const&amp;, llvm::Constant const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x4bb4fc4)
 #<!-- -->4 0x00007fffefa1f39a llvm::AsmPrinter::emitXXStructorList(llvm::DataLayout const&amp;, llvm::Constant const*, bool) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f7439a)
 #<!-- -->5 0x00007fffefa1328e llvm::AsmPrinter::emitSpecialLLVMGlobal(llvm::GlobalVariable const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f6828e)
 #<!-- -->6 0x00007fffefa1e723 llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f73723)
 #<!-- -->7 0x00007fffefa163ca llvm::AsmPrinter::doFinalization(llvm::Module&amp;) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1f6b3ca)
 #<!-- -->8 0x00007fffeee16f4d llvm::FPPassManager::doFinalization(llvm::Module&amp;) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x136bf4d)
 #<!-- -->9 0x00007fffeee21ee9 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/nix/store/2xxj9pxid3bd739b113mi5nm1ixnymlb-llvm-20.1.8-lib/lib/libLLVM.so.20.1+0x1376ee9)
#<!-- -->10 0x00005555555747c4 compileModule(char**, llvm::LLVMContext&amp;) (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x207c4)
#<!-- -->11 0x000055555556a389 main (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x16389)
#<!-- -->12 0x00007fffed5444d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#<!-- -->13 0x00007fffed54459b __libc_start_main@<!-- -->GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#<!-- -->14 0x000055555556b155 _start (/nix/store/3g01x759rryj4314i4ch6s0swqdngbq4-llvm-20.1.8/bin/llc+0x17155)
`llc' failed in phase `LLVM Compiler'. (Exit code: -11)
```
</details>


---

### Comment 2 - davemgreen

Can you try with the latest llvm? I believe this should be fixed after #165422.

If you want a workaround, then the module needs at least one (dummy) function in it. Let us know if it is not fixed.

---

