# [RISC-V] Miscompile with `-flto`

**Author:** patrick-rivos
**URL:** https://github.com/llvm/llvm-project/issues/111458
**Status:** Closed
**Labels:** miscompilation, vectorizers, LTO
**Closed Date:** 2025-04-29T11:44:09Z

## Body

Testcase:
```c
int printf(const char *, ...);
long a;
long long b;
int c = -349518949;
unsigned long long d[1][20];
long long e[1][12], g[1][12];
int f[1][12];
short h[1][12][12];
unsigned long long i[1][12][12];
long long j[2][12][12][12][12][12];
unsigned short k[1][12][12][12][12][12];
unsigned char l[1][15];
unsigned short m[15][15][15];
unsigned char n[1][15][15];
unsigned long long o[1][15][15][15];
short p[22];
unsigned char q[22][22][22][22][22];
int w[15][15][50625];
short v[225];
void aa(int ab, unsigned long long ac[][20], long long ad[][12], int ae[][12],
        long long af[][12], short ag[][12][12], unsigned long long ah[][12][12],
        long long ai[][12][12][12][12][12],
        unsigned short aj[][12][12][12][12][12], unsigned char ak[][15],
        unsigned short al[][15][15], unsigned char am[][15][15],
        unsigned long long an[][15][15][15], short ao[],
        unsigned char ap[][22][22][22][22]) {
  for (unsigned r = 0; r < (unsigned)1085708641301355050; r = -40805543)
    for (short s = ao[r]; s; s++)
      for (int t = 0; t < ab + 349518971;
           t += (20091 ? ~2166664188148818947ULL : 0) - 2111069176)
        for (short u = 3; u < 18; u += (short)~-2147483629 + 21)
          a = ap[r][3][r][t][1] ?: ab;
}
void aq(long long *ab, int ac) { *ab ^= ac; }
void ar() {
  for (long x = 0; x < 2; ++x)
    for (long y = 0; y < 2; ++y)
      for (long z = 0; z < 2; ++z)
        for (long as = 0; as < 2; ++as)
          for (long at = 0; at < 12; ++at)
            for (long au = 0; au < 12; ++au)
              j[x][y][z][as][at][au] = 7;
  for (long av = 0; av < 12; ++av)
    for (long aw = 0; aw < 15; ++aw)
      for (long ax = 0; ax < 15; ++ax)
        m[av][aw][ax] = p[av] = 4;
  for (long ay = 0; ay < 2; ++ay)
    for (long az = 0; az < 2; ++az)
      for (long ba = 0; ba < 2; ++ba)
        for (long bb = 0; bb < 22; ++bb)
          for (long bc = 0; bc < 22; ++bc)
            q[ay][az][ba][bb][bc] = 0;
  for (long bd = 0; bd < 15; ++bd)
    for (long be = 0; be < 5; ++be)
      for (long bf = 0; bf < 5; ++bf)
        for (long bg = 0; bg < 15; ++bg)
          for (long bh = 0; bh < 5; ++bh)
            for (long bi = 0; bi < 15; ++bi)
              w[bd][be][bf + bg * 15 + bi] = 2;
  for (long bj = 0; bj < 15; ++bj)
    for (long bk = 0; bk < 15; ++bk)
      v[bj * 15 + bk] = 80;
}
int main() {
  ar();
  aa(c, d, e, f, g, h, i, j, k, l, m, n, o, p, q);
  aq(&b, a);
  printf("%llu\n", b);
}
```
Commands:
```bash
> /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -flto -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
> QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
18424225731631039435

> /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O0 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
> QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
18446744073360032667
```

Does not appear when using `-fuse-ld=lld`.

opt-bisect-limit points to `LoopVectorizePass`

Reduced LLVM IR:
```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@w = global [15 x [15 x [50625 x i32]]] zeroinitializer

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(write)
declare void @llvm.masked.scatter.nxv4i32.nxv4p0(<vscale x 4 x i32>, <vscale x 4 x ptr>, i32 immarg, <vscale x 4 x i1>) #0

define i32 @main(<vscale x 4 x i32> %0, <vscale x 4 x i1> %1, <vscale x 4 x i32> %2, <vscale x 4 x i1> %3) #1 {
entry:
  tail call void @llvm.masked.scatter.nxv4i32.nxv4p0(<vscale x 4 x i32> %0, <vscale x 4 x ptr> shufflevector (<vscale x 4 x ptr> insertelement (<vscale x 4 x ptr> poison, ptr @w, i64 0), <vscale x 4 x ptr> poison, <vscale x 4 x i32> zeroinitializer), i32 0, <vscale x 4 x i1> %1)
  br label %for.cond4.preheader.us.i

for.cond4.preheader.us.i:                         ; preds = %for.cond4.preheader.us.i, %entry
  br i1 true, label %aa.exit, label %for.cond4.preheader.us.i

aa.exit:                                          ; preds = %for.cond4.preheader.us.i
  %call = tail call i32 (...) @printf()
  ret i32 0
}

declare i32 @printf(...)

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(write) }
attributes #1 = { "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+v,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-smctr,-experimental-smmpm,-experimental-smnpm,-experimental-ssctr,-experimental-ssnpm,-experimental-sspm,-experimental-supm,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smepmp,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zaamo,-zabha,-zacas,-zalrsc,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"target-abi", !"lp64d"}

```

Reproducer:
```
/scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -flto -fuse-ld=lld -opt-bisect-limit=0 $1 -o user-config-bad.out -fsigned-char -fno-strict-aliasing -fwrapv -Wl,-mllvm,-opt-bisect-limit=0 2> /dev/null
/scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O0 $1 -o user-config-good.out -fsigned-char -fno-strict-aliasing -fwrapv
BAD="$(QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config-bad.out)"
GOOD="$(QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config-good.out)"

echo $GOOD
echo $BAD
```

Output:
```
> ./min.sh reduced.ll

*
```

Found via fuzzer.

This was a somewhat strange reduction so if there are issues reproducing please let me know and I'll re-reduce.

## Comments

### Comment 1 - topperc

`/scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -flto -fuse-ld=lld -opt-bisect-limit=0 $1 -o user-config-bad.out -fsigned-char -fno-strict-aliasing -fwrapv -Wl,-mllvm,-opt-bisect-limit=0 2> /dev/null`

That's passing -opt-bisect-limit=0 to clang without a -mllvm. I guess that gets parsed as `-o pt-bisect-limit=0` which is overridden by the later `-o`?

---

### Comment 2 - patrick-rivos

```
/scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -flto -fuse-ld=lld -mllvm -opt-bisect-limit=0 $1 -o user-config-bad.out -fsigned-char -fno-strict-aliasing -fwrapv -Wl,-mllvm,-opt-bisect-limit=0 2> /dev/null
```
Updating to use `-mllvm` gives the same result - presumably since it's implicitly `-O0`?

---

### Comment 3 - fhahn

Does still reproduce?

---

### Comment 4 - fhahn

@patrick-rivos there have been a number of fixes recently, could you check if the issue still reproduces?

---

### Comment 5 - lukel97

I just checked with a version of clang I had lying around and it's fixed as of at least bd0b903a91721570e0bc98f7ed8b0aaf2f2a628b

---

