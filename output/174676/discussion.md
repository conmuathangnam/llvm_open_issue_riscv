# wasm-ld crash (rust build of crc-fast 1.6.0)

**Author:** pchickey
**URL:** https://github.com/llvm/llvm-project/issues/174676
**Status:** Closed
**Labels:** lld:wasm, crash
**Closed Date:** 2026-01-30T00:47:04Z

## Body

Here is a crash of wasm-ld, with reproduction via lld’s --reproduce option: 

[wasm-ld-crc-fast-crash.tar.gz](https://github.com/user-attachments/files/24462051/wasm-ld-crc-fast-crash.tar.gz)

Running the same through wasm-ld built with asan demonstrates there is a use-after-free, but I can't confirm that it is related.

The wasm-ld crash reproduces as a SIGSEGV for source trees within a small hamming distance of a the source checkout crc-fast 1.6.0. You can make certain changes to the source code or dependencies and it will still crash, but for instance I discovered that by updating the version in Cargo.toml to 1.7.0, it will no longer crash - that version number ends up in various symbols in the object file. I strongly suspect, based bisecting crc-fast source changes for a few hours and the investigation below, that the crash isn’t caused by anything malformed about crc-fast itself, but that something about those inputs lines things up just right to cause a crash or assertion failure.

I have reproduced this crash on both MacOS 15.7 (aarch64) and Ubuntu 24.02.3 (x86_64).

In the wild, the wasm-ld that ships installed in rustc 1.92 as rust-lld will crash with a SIGSEGV:
<details>
<pre>
[p.hickey@KVKD0WG7VF:src/crc-fast-rust]% git clone  https://github.com/awesomized/crc-fast-rust --branch 1.6.0
Cloning into 'crc-fast-rust'...
remote: Enumerating objects: 1829, done.
remote: Counting objects: 100% (639/639), done.
remote: Compressing objects: 100% (169/169), done.
remote: Total 1829 (delta 504), reused 558 (delta 470), pack-reused 1190 (from 1)
Receiving objects: 100% (1829/1829), 1.52 MiB | 3.48 MiB/s, done.
Resolving deltas: 100% (1305/1305), done.
Note: switching to '45394b13b4270a2722266c6be772aef02059815d'.

[p.hickey@KVKD0WG7VF:src/crc-fast-rust]% cd crc-fast-rust
[p.hickey@KVKD0WG7VF:crc-fast-rust/crc-fast-rust]% cargo --version
cargo 1.92.0 (344c4567c 2025-10-21)
[p.hickey@KVKD0WG7VF:crc-fast-rust/crc-fast-rust]% cargo build --target wasm32-wasip2
   Compiling wit-bindgen v0.46.0
   Compiling version_check v0.9.5
   Compiling typenum v1.19.0
   Compiling zerocopy v0.8.27
   Compiling getrandom v0.3.4
   Compiling cfg-if v1.0.4
   Compiling memchr v2.7.6
   Compiling regex-syntax v0.8.8
   Compiling rustversion v1.0.22
   Compiling crc-catalog v2.4.0
   Compiling aho-corasick v1.1.4
   Compiling generic-array v0.14.9
   Compiling crc v3.3.0
   Compiling regex-automata v0.4.13
   Compiling wasip2 v1.0.1+wasi-0.2.4
   Compiling rand_core v0.9.3
   Compiling block-buffer v0.10.4
   Compiling crypto-common v0.1.6
   Compiling digest v0.10.7
   Compiling regex v1.12.2
   Compiling ppv-lite86 v0.2.21
   Compiling rand_chacha v0.9.0
   Compiling rand v0.9.2
   Compiling crc-fast v1.6.0 (/Users/p.hickey/src/crc-fast-rust/crc-fast-rust)
error: linking with `wasm-component-ld` failed: exit status: 1
  |
  = note:  "wasm-component-ld" "-flavor" "wasm" "--export" "cabi_realloc" "-z" "stack-size=1048576" "--stack-first" "--allow-undefined" "--no-demangle" "<76 object files omitted>" "<sysroot>/lib/rustlib/wasm32-wasip2/lib/libpanic_abort-*.rlib" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/{libcrc-e9bf5a3fb54d8c43,libcrc_catalog-2b954ecada58b52b,libdigest-19e655bc742aa5b7,libblock_buffer-4879d7a255417a1d,libcrypto_common-dcc9be42abefd1f3,libgeneric_array-17a4a1431b2b229c,libtypenum-46d8c497f70ad3aa}.rlib" "<sysroot>/lib/rustlib/wasm32-wasip2/lib/{libstd-*,libwasi-*,libwasi-*,libwit_bindgen-*,libcfg_if-*,librustc_demangle-*,libstd_detect-*,libhashbrown-*,librustc_std_workspace_alloc-*,libminiz_oxide-*,libadler2-*,libunwind-*,liblibc-*}.rlib" "-l" "c" "<sysroot>/lib/rustlib/wasm32-wasip2/lib/{librustc_std_workspace_core-*,liballoc-*,libcore-*,libcompiler_builtins-*}.rlib" "-L" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/build/wit-bindgen-0c06af0fc17a621e/out" "-L" "<sysroot>/lib/rustlib/wasm32-wasip2/lib/self-contained" "-o" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.wasm" "--gc-sections" "--no-entry" "-O0"
  = note: some arguments are omitted. use `--verbose` to show all linker arguments
  = note: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
          Stack dump:
          0.    Program arguments: rust-lld -flavor wasm --export=cabi_realloc -z stack-size=1048576 --stack-first --allow-undefined --no-demangle /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.05je5b8g8gpt8m6haz22f2d15.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.077l77d8u4q7g9a9jalnqkg3i.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0dcxn8t8ve30zsvdpiehc7gup.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0f5z4huo1ydm52nvkkxs3ewy9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0f8e2p2ufcjh3au7q1z51nezt.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0w7m46bfvdtca6rbdhmfn8j5u.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0xvpei4kwn3auj467zcaue3hp.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.10cijm07ym6wkuneuj0yl6rxe.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.15l1dz2waw9piymkjkhl24ic9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.16c3zu7iawcdeandr4cqypan7.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1h1i3jt1iequf1gcxxx3m4mjv.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1kpg5uai4tvjumru3rftuv0q3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1xzzsmdn3qysibqd0rzm5487k.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.21bp6ci417cel5en21xefepm6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2g4z0n7e29btomqzz4267j72x.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2ir36uiorsy4dl06qr5po0ndn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2j4bbguoghi79t486y6m773u4.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2krjjlyflysl1hri6c3ox7hc3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.310jvgst7oslzej5e53rieon0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.34os6pwjb1p32xoljio95f9fs.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.34w8q2v7s4wdy6cr1m504r7ma.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.39xs38uo09g2zpl99strm3kny.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3jsconz79lsuiwcz4ykxx29l0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3v5lpnxo74ned2eq09n2bfzs4.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3zd77p17crqwmolnt8ajfeohb.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.46brk1o5ec63tvidsy7vbcfdn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.4fuhtb91m0rct9r1ektxjj2dp.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.4nawpm0wwya4srbti0ujhoggz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.52st5e42xgt5nb6a6tb07kif3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.595jyn2ks32if923rqfd1t1vt.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5a81iessx21ydeniyoll4ucx7.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5aamgi7ospr3e6wij4623l5jw.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5b5udeec366afczjfecqp50dm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5uc8g4ssjd1j24w1eqy9fecql.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5zzwfyuyhcuqunal425n745f0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.62ggluxgmnnm6e457nariuakc.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.68hj1iw9giuzzwhh640szrdfg.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6gsgrbx707dpmv6bcp0esyuen.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6pgnl2aimxsw4aksmtbhsy7v0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6qrnetburkv2ggetvshs6yhbn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.70uc21et5v5mwvbebcwiblzqk.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.762aieug1ab5gkwwu6d56c3yu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.77bsvx4abhisrj3uon2y5e4h6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.78wy67aae0zh8c2rmtcwlgaj1.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7c63ydfq3ga9h32f6bdp728tu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7ituz68qpfy5aaq44xjjordu5.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7wh8dggpis6oi8a5d29pczdhz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.80o9c7wdp0yhzrmqm3ngdu5oj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.87t7i7b2394hguen0gk4o1cu3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8b0kp57tlc9mwx400r9hjmh1u.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8lv1mes2omsepz0ezkykle3wu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8qtv3dwwf01tjq7pqc7drf2qq.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8rduq9bcy6npk8fhbhiz0qkaz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8tiyc1em4aocw2xoyt8eld2ou.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8w46fni2sax5bfotyvuidisjf.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.911iy7e3ub37b1q8lpzrel0dz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.916abqluvq7hioorqc17xgdrj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.9g5ot0s0wvm7xaqvq8m1hrjjg.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.9p2pvbt0zy5ukrbpbnb824pfm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.a4oj5fte20zn11bhz4uohtxog.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.a6mfkt39r60mpwtnzhtvqh615.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.actb0tc56m9vcnke88mos6ze6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.aii0ghkd9swx7cih3m0swvbuq.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.aq2kpvr9y5hq5l356qakaot42.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.as1m2kxn4ctsnd8rza7dkct3w.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.bgp46wnnimy3px94wol111ehr.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.big3n3phaxj162h9n0fe7eqcm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.bomqukqu8jvv1rium8pgm6lov.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.c0tbykef5esvw25x0ob9var8h.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.ccmoetc6tcve1fjwto0ey0nxa.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.cw5q1vxr7q5sef4e5a8m813r9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.czf7vvcil8gqhu850m4o1id1k.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.d87yrdri0d6rv757m5d3wokaj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.en1dzwmud3je4rces9bu7lnz5.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.f4z8aq1m6vq6tfc8xm5n2si0a.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.35d1nrx3vhep32bcyldo4brb2.0qql4pl.rcgu.o /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libpanic_abort-58e7ca17f279ee2c.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrc-e9bf5a3fb54d8c43.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrc_catalog-2b954ecada58b52b.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libdigest-19e655bc742aa5b7.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libblock_buffer-4879d7a255417a1d.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrypto_common-dcc9be42abefd1f3.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libgeneric_array-17a4a1431b2b229c.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libtypenum-46d8c497f70ad3aa.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libstd-ac1e0f366cac206d.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwasi-5ec33c9bb1237458.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwasi-3dc04dcc7e8548ce.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwit_bindgen-07ccf6eb5537f8e1.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcfg_if-2b8fc8464cc71432.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_demangle-d46baddf8b53a26e.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libstd_detect-5b51ac855217f0ee.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libhashbrown-8c4807d0bf47ded7.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_std_workspace_alloc-adeafb8cbc20fea0.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libminiz_oxide-657e314d8aae041c.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libadler2-98f62f238d3961ac.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libunwind-c937bedff093a1e5.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/liblibc-df85883f14163f18.rlib -l c /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_std_workspace_core-72476db19138a823.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/liballoc-1c40e0cd1ed66cbc.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcore-d9216f2ff3c21f88.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcompiler_builtins-fc15b45d6c71cd23.rlib -L /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/build/wit-bindgen-0c06af0fc17a621e/out -L /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/self-contained --gc-sections --no-entry -O 0 -o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/.tmpYeCSEw/crc_fast.wasm
           #0 0x0000000100fdc7d0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1001087d0)
           #1 0x0000000100fda6a4 llvm::sys::RunSignalHandlers() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1001066a4)
           #2 0x0000000100fdd274 SignalHandler(int, __siginfo*, void*) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100109274)
           #3 0x00000001893816a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
           #4 0x00000001013b7edc lld::wasm::ElemSection::writeBody() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004e3edc)
           #5 0x00000001013bb2f0 lld::wasm::SyntheticSection::finalizeContents() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004e72f0)
           #6 0x00000001013c8924 lld::wasm::(anonymous namespace)::Writer::run() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004f4924)
           #7 0x00000001013c2250 lld::wasm::writeResult() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004ee250)
           #8 0x00000001013930bc lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004bf0bc)
           #9 0x000000010138f168 lld::wasm::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004bb168)
          #10 0x000000010100be20 lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100137e20)
          #11 0x0000000100ed8408 lld_main(int, char**, llvm::ToolContext const&) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100004408)
          #12 0x0000000100ed8a54 main (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100004a54)
          #13 0x0000000188fa6b98
          error: failed to invoke LLD: signal: 11 (SIGSEGV)


error: could not compile `crc-fast` (lib) due to 1 previous error
</pre>
</details>

Building from llvm/llvm-project `main` (74fd7516),  wasm-ld will crash with an assertion failure, and a very similar context:
 ```
[p.hickey@KVKD0WG7VF:reproduced_lld_problem/wasm-ld-crc-fast-crash]% ~/src/rust/upstream-main-lld/bin/wasm-ld @response.txt
Assertion failed: (ctx.sym.indirectFunctionTable), function writeBody, file SyntheticSections.cpp, line 589.
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.	Program arguments: /Users/p.hickey/src/rust/upstream-main-lld/bin/wasm-ld @response.txt
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.dylib            0x0000000110fe3118 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 56
1  libLLVM.dylib            0x0000000110fe395c SignalHandler(int, __siginfo*, void*) + 372
2  libsystem_platform.dylib 0x00000001893816a4 _sigtramp + 56
3  libsystem_pthread.dylib  0x0000000189347848 pthread_kill + 296
4  libsystem_c.dylib        0x00000001892509e4 abort + 124
5  libsystem_c.dylib        0x000000018924fc18 err + 0
6  wasm-ld                  0x0000000102f32d18 lld::wasm::ElemSection::writeBody() + 88
7  wasm-ld                  0x0000000102f36bfc lld::wasm::SyntheticSection::finalizeContents() + 36
8  wasm-ld                  0x0000000102f5417c lld::wasm::(anonymous namespace)::Writer::finalizeSections() + 128
9  wasm-ld                  0x0000000102f4cb60 lld::wasm::(anonymous namespace)::Writer::run() + 2616
10 wasm-ld                  0x0000000102f4c0c4 lld::wasm::writeResult() + 64
11 wasm-ld                  0x0000000102ecf93c lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) + 4620
12 wasm-ld                  0x0000000102ece600 lld::wasm::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) + 268
13 wasm-ld                  0x00000001026b3c38 lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) + 180
14 wasm-ld                  0x00000001026a9e84 lld_main(int, char**, llvm::ToolContext const&) + 264
15 wasm-ld                  0x00000001026ad4f0 main + 108
16 dyld                     0x0000000188fa6b98 start + 6076
zsh: abort      ~/src/rust/upstream-main-lld/bin/wasm-ld @response.txt
```

I swapped in a wasm-ld built with asan, and got a use-after-free of something allocated in a bump arena. However, I spot-checked this on other known good projects (ones which do not crash wasm-ld) and got similar, but not identical, use-after-frees there, so I don't know if asan has revealed this crash's root cause or an unrelated issue. I'm not aware of this project's policy for san bugs in the linker, but I can prepare other reproductions if they are considered bugs.

<details>
<pre>
wasm-ld(15569,0x1f72da200) malloc: nano zone abandoned due to inability to reserve vm space.
=================================================================
==15569==ERROR: AddressSanitizer: use-after-poison on address 0x621000001569 at pc 0x0001040a9c7c bp 0x00016f42a530 sp 0x00016f429cd0
WRITE of size 8 at 0x621000001569 thread T0
    #0 0x0001040a9c78 in memcpy+0x29c (libclang_rt.asan_osx_dynamic.dylib:arm64e+0x85c78)
    #1 0x00010cd8b468 in llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&, llvm::SmallVectorImpl<char const*>&, bool)+0x168 (libLLVM.dylib:arm64+0x63468)
    #2 0x00010cd8c0b0 in llvm::cl::ExpansionContext::expandResponseFile(llvm::StringRef, llvm::SmallVectorImpl<char const*>&)+0x10c (libLLVM.dylib:arm64+0x640b0)
    #3 0x00010cd8ccf8 in llvm::cl::ExpansionContext::expandResponseFiles(llvm::SmallVectorImpl<char const*>&)+0x230 (libLLVM.dylib:arm64+0x64cf8)
    #4 0x00010cd8dc8c in llvm::cl::ExpandResponseFiles(llvm::StringSaver&, void (*)(llvm::StringRef, llvm::StringSaver&, llvm::SmallVectorImpl<char const*>&, bool), llvm::SmallVectorImpl<char const*>&)+0xdc (libLLVM.dylib:arm64+0x65c8c)
    #5 0x000101e257fc in lld::wasm::(anonymous namespace)::WasmOptTable::parse(llvm::ArrayRef<char const*>)+0x484 (wasm-ld:arm64+0x1014557fc)
    #6 0x000101e20254 in lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>)+0x8f4 (wasm-ld:arm64+0x101450254)
    #7 0x000101e1f5f4 in lld::wasm::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool)+0x368 (wasm-ld:arm64+0x10144f5f4)
    #8 0x0001009e94a8 in lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool)+0x214 (wasm-ld:arm64+0x1000194a8)
    #9 0x0001009d26bc in lld_main(int, char**, llvm::ToolContext const&)+0x2f8 (wasm-ld:arm64+0x1000026bc)
    #10 0x0001009d9908 in main+0x23c (wasm-ld:arm64+0x100009908)
    #11 0x000188fa6b94 in start+0x17b8 (dyld:arm64e+0xfffffffffff3ab94)

0x621000001569 is located 105 bytes inside of 4096-byte region [0x621000001500,0x621000002500)
allocated by thread T0 here:
    #0 0x0001040621a4 in __sanitizer_mz_memalign+0x80 (libclang_rt.asan_osx_dynamic.dylib:arm64e+0x3e1a4)
    #1 0x00018917978c in _malloc_zone_memalign+0x13c (libsystem_malloc.dylib:arm64e+0x3378c)
    #2 0x000189163b1c in _malloc_type_aligned_alloc_outlined+0x68 (libsystem_malloc.dylib:arm64e+0x1db1c)
    #3 0x0001893014d0 in operator new(unsigned long, std::align_val_t)+0x4c (libc++abi.dylib:arm64e+0x1a4d0)
    #4 0x0001892ff4f4 in operator new(unsigned long, std::align_val_t, std::nothrow_t const&)+0xc (libc++abi.dylib:arm64e+0x184f4)
    #5 0x00010cdc9ef8 in llvm::allocate_buffer(unsigned long, unsigned long)+0x10 (libLLVM.dylib:arm64+0xa1ef8)
    #6 0x000100a6ad64 in llvm::MallocAllocator::Allocate(unsigned long, unsigned long)+0x20 (wasm-ld:arm64+0x10009ad64)
    #7 0x000100a6b16c in llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::StartNewSlab()+0x4c (wasm-ld:arm64+0x10009b16c)
    #8 0x000100a6a8e0 in llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::AllocateSlow(unsigned long, unsigned long, llvm::Align)+0x2d0 (wasm-ld:arm64+0x10009a8e0)
    #9 0x000100a6a36c in llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::Allocate(unsigned long, llvm::Align)+0x2dc (wasm-ld:arm64+0x10009a36c)
    #10 0x000100a68e14 in llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::Allocate(unsigned long, unsigned long)+0x1ac (wasm-ld:arm64+0x100098e14)
    #11 0x000100a68940 in lld::SpecificAllocBase::getOrCreate(void*, unsigned long, unsigned long, lld::SpecificAllocBase* (&)(void*))+0x188 (wasm-ld:arm64+0x100098940)
    #12 0x000101e58690 in llvm::SpecificBumpPtrAllocator<lld::wasm::SymbolTable>& lld::getSpecificAllocSingleton<lld::wasm::SymbolTable>()+0x24 (wasm-ld:arm64+0x101488690)
    #13 0x000101e1f8d8 in lld::wasm::SymbolTable* lld::make<lld::wasm::SymbolTable>()+0xc (wasm-ld:arm64+0x10144f8d8)
    #14 0x000101e1f574 in lld::wasm::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool)+0x2e8 (wasm-ld:arm64+0x10144f574)
    #15 0x0001009e94a8 in lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool)+0x214 (wasm-ld:arm64+0x1000194a8)
    #16 0x0001009d26bc in lld_main(int, char**, llvm::ToolContext const&)+0x2f8 (wasm-ld:arm64+0x1000026bc)
    #17 0x0001009d9908 in main+0x23c (wasm-ld:arm64+0x100009908)
    #18 0x000188fa6b94 in start+0x17b8 (dyld:arm64e+0xfffffffffff3ab94)

SUMMARY: AddressSanitizer: use-after-poison (libLLVM.dylib:arm64+0x63468) in llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&, llvm::SmallVectorImpl<char const*>&, bool)+0x168
Shadow bytes around the buggy address:
  0x621000001280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001480: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=>0x621000001500: 00 00 00 00 00 00 00 00 00 00 00 00 00[f7]f7 f7
  0x621000001580: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001600: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001680: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001700: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001780: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==15569==ABORTING
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.    Program arguments: /Users/p.hickey/src/rust/custom-lld/bin/wasm-ld @response.txt
 #0 0x0000000110b27118 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3dff118)
 #1 0x0000000110b2795c SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3dff95c)
 #2 0x00000001893816a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #3 0x0000000189347848 (/usr/lib/system/libsystem_pthread.dylib+0x180473848)
 #4 0x00000001892509e4 (/usr/lib/system/libsystem_c.dylib+0x18037c9e4)
 #5 0x00000001040857f4 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x617f4)
 #6 0x0000000104084f14 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x60f14)
 #7 0x0000000104068dcc (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x44dcc)
 #8 0x0000000104068028 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x44028)
 #9 0x00000001040a9c88 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x85c88)
#10 0x000000010cd8b46c llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&, llvm::SmallVectorImpl<char const*>&, bool) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x6346c)
#11 0x000000010cd8c0b4 llvm::cl::ExpansionContext::expandResponseFile(llvm::StringRef, llvm::SmallVectorImpl<char const*>&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x640b4)
#12 0x000000010cd8ccfc llvm::cl::ExpansionContext::expandResponseFiles(llvm::SmallVectorImpl<char const*>&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x64cfc)
#13 0x000000010cd8dc90 llvm::cl::ExpandResponseFiles(llvm::StringSaver&, void (*)(llvm::StringRef, llvm::StringSaver&, llvm::SmallVectorImpl<char const*>&, bool), llvm::SmallVectorImpl<char const*>&) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x65c90)
#14 0x0000000101e25800 lld::wasm::(anonymous namespace)::WasmOptTable::parse(llvm::ArrayRef<char const*>) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x101455800)
#15 0x0000000101e20258 lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x101450258)
#16 0x0000000101e1f5f8 lld::wasm::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x10144f5f8)
#17 0x00000001009e94ac lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x1000194ac)
#18 0x00000001009d26c0 lld_main(int, char**, llvm::ToolContext const&) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x1000026c0)
#19 0x00000001009d990c main (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x10000990c)
#20 0x0000000188fa6b98
</pre>
</details>

If you want to reproduce running the entire build in your instrumented (or fixed) wasm-ld using rust:

RUSTFLAGS="-Clinker=/path/to/some/wasm-ld -Clink-arg=--reproduce=reproduce.tar" cargo build --target=wasm32-wasip2

It is safe to substitute wasm-ld for the usual linker used with wasip2 (wasm-component-ld) because wasm-component-ld first passes all of its arguments to wasm-ld, then post-processes the output from a core module to a component. The post-processing aspect isn't relevant to this bug.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-wasm

Author: Pat Hickey (pchickey)

<details>
Here is a crash of wasm-ld, with reproduction via lld’s --reproduce option: 

[wasm-ld-crc-fast-crash.tar.gz](https://github.com/user-attachments/files/24462051/wasm-ld-crc-fast-crash.tar.gz)

Running the same through wasm-ld built with asan demonstrates there is a use-after-free, but I can't confirm that it is related.

The wasm-ld crash reproduces as a SIGSEGV for source trees within a small hamming distance of a the source checkout crc-fast 1.6.0. You can make certain changes to the source code or dependencies and it will still crash, but for instance I discovered that by updating the version in Cargo.toml to 1.7.0, it will no longer crash - that version number ends up in various symbols in the object file. I strongly suspect, based bisecting crc-fast source changes for a few hours and the investigation below, that the crash isn’t caused by anything malformed about crc-fast itself, but that something about those inputs lines things up just right to cause a crash or assertion failure.

I have reproduced this crash on both MacOS 15.7 (aarch64) and Ubuntu 24.02.3 (x86_64).

In the wild, wasm-ld (installed in the rust project as rust-lld) will crash with a SIGSEGV:
&lt;details&gt;
&lt;pre&gt;
[p.hickey@<!-- -->KVKD0WG7VF:src/crc-fast-rust]% git clone  https://github.com/awesomized/crc-fast-rust --branch 1.6.0
Cloning into 'crc-fast-rust'...
remote: Enumerating objects: 1829, done.
remote: Counting objects: 100% (639/639), done.
remote: Compressing objects: 100% (169/169), done.
remote: Total 1829 (delta 504), reused 558 (delta 470), pack-reused 1190 (from 1)
Receiving objects: 100% (1829/1829), 1.52 MiB | 3.48 MiB/s, done.
Resolving deltas: 100% (1305/1305), done.
Note: switching to '45394b13b4270a2722266c6be772aef02059815d'.

[p.hickey@<!-- -->KVKD0WG7VF:src/crc-fast-rust]% cd crc-fast-rust
[p.hickey@<!-- -->KVKD0WG7VF:crc-fast-rust/crc-fast-rust]% cargo --version
cargo 1.92.0 (344c4567c 2025-10-21)
[p.hickey@<!-- -->KVKD0WG7VF:crc-fast-rust/crc-fast-rust]% cargo build --target wasm32-wasip2
   Compiling wit-bindgen v0.46.0
   Compiling version_check v0.9.5
   Compiling typenum v1.19.0
   Compiling zerocopy v0.8.27
   Compiling getrandom v0.3.4
   Compiling cfg-if v1.0.4
   Compiling memchr v2.7.6
   Compiling regex-syntax v0.8.8
   Compiling rustversion v1.0.22
   Compiling crc-catalog v2.4.0
   Compiling aho-corasick v1.1.4
   Compiling generic-array v0.14.9
   Compiling crc v3.3.0
   Compiling regex-automata v0.4.13
   Compiling wasip2 v1.0.1+wasi-0.2.4
   Compiling rand_core v0.9.3
   Compiling block-buffer v0.10.4
   Compiling crypto-common v0.1.6
   Compiling digest v0.10.7
   Compiling regex v1.12.2
   Compiling ppv-lite86 v0.2.21
   Compiling rand_chacha v0.9.0
   Compiling rand v0.9.2
   Compiling crc-fast v1.6.0 (/Users/p.hickey/src/crc-fast-rust/crc-fast-rust)
error: linking with `wasm-component-ld` failed: exit status: 1
  |
  = note:  "wasm-component-ld" "-flavor" "wasm" "--export" "cabi_realloc" "-z" "stack-size=1048576" "--stack-first" "--allow-undefined" "--no-demangle" "&lt;76 object files omitted&gt;" "&lt;sysroot&gt;/lib/rustlib/wasm32-wasip2/lib/libpanic_abort-*.rlib" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/{libcrc-e9bf5a3fb54d8c43,libcrc_catalog-2b954ecada58b52b,libdigest-19e655bc742aa5b7,libblock_buffer-4879d7a255417a1d,libcrypto_common-dcc9be42abefd1f3,libgeneric_array-17a4a1431b2b229c,libtypenum-46d8c497f70ad3aa}.rlib" "&lt;sysroot&gt;/lib/rustlib/wasm32-wasip2/lib/{libstd-*,libwasi-*,libwasi-*,libwit_bindgen-*,libcfg_if-*,librustc_demangle-*,libstd_detect-*,libhashbrown-*,librustc_std_workspace_alloc-*,libminiz_oxide-*,libadler2-*,libunwind-*,liblibc-*}.rlib" "-l" "c" "&lt;sysroot&gt;/lib/rustlib/wasm32-wasip2/lib/{librustc_std_workspace_core-*,liballoc-*,libcore-*,libcompiler_builtins-*}.rlib" "-L" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/build/wit-bindgen-0c06af0fc17a621e/out" "-L" "&lt;sysroot&gt;/lib/rustlib/wasm32-wasip2/lib/self-contained" "-o" "/Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.wasm" "--gc-sections" "--no-entry" "-O0"
  = note: some arguments are omitted. use `--verbose` to show all linker arguments
  = note: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
          Stack dump:
          0.    Program arguments: rust-lld -flavor wasm --export=cabi_realloc -z stack-size=1048576 --stack-first --allow-undefined --no-demangle /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.05je5b8g8gpt8m6haz22f2d15.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.077l77d8u4q7g9a9jalnqkg3i.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0dcxn8t8ve30zsvdpiehc7gup.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0f5z4huo1ydm52nvkkxs3ewy9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0f8e2p2ufcjh3au7q1z51nezt.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0w7m46bfvdtca6rbdhmfn8j5u.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.0xvpei4kwn3auj467zcaue3hp.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.10cijm07ym6wkuneuj0yl6rxe.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.15l1dz2waw9piymkjkhl24ic9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.16c3zu7iawcdeandr4cqypan7.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1h1i3jt1iequf1gcxxx3m4mjv.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1kpg5uai4tvjumru3rftuv0q3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.1xzzsmdn3qysibqd0rzm5487k.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.21bp6ci417cel5en21xefepm6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2g4z0n7e29btomqzz4267j72x.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2ir36uiorsy4dl06qr5po0ndn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2j4bbguoghi79t486y6m773u4.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.2krjjlyflysl1hri6c3ox7hc3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.310jvgst7oslzej5e53rieon0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.34os6pwjb1p32xoljio95f9fs.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.34w8q2v7s4wdy6cr1m504r7ma.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.39xs38uo09g2zpl99strm3kny.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3jsconz79lsuiwcz4ykxx29l0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3v5lpnxo74ned2eq09n2bfzs4.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.3zd77p17crqwmolnt8ajfeohb.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.46brk1o5ec63tvidsy7vbcfdn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.4fuhtb91m0rct9r1ektxjj2dp.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.4nawpm0wwya4srbti0ujhoggz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.52st5e42xgt5nb6a6tb07kif3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.595jyn2ks32if923rqfd1t1vt.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5a81iessx21ydeniyoll4ucx7.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5aamgi7ospr3e6wij4623l5jw.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5b5udeec366afczjfecqp50dm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5uc8g4ssjd1j24w1eqy9fecql.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.5zzwfyuyhcuqunal425n745f0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.62ggluxgmnnm6e457nariuakc.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.68hj1iw9giuzzwhh640szrdfg.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6gsgrbx707dpmv6bcp0esyuen.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6pgnl2aimxsw4aksmtbhsy7v0.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.6qrnetburkv2ggetvshs6yhbn.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.70uc21et5v5mwvbebcwiblzqk.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.762aieug1ab5gkwwu6d56c3yu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.77bsvx4abhisrj3uon2y5e4h6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.78wy67aae0zh8c2rmtcwlgaj1.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7c63ydfq3ga9h32f6bdp728tu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7ituz68qpfy5aaq44xjjordu5.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.7wh8dggpis6oi8a5d29pczdhz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.80o9c7wdp0yhzrmqm3ngdu5oj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.87t7i7b2394hguen0gk4o1cu3.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8b0kp57tlc9mwx400r9hjmh1u.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8lv1mes2omsepz0ezkykle3wu.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8qtv3dwwf01tjq7pqc7drf2qq.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8rduq9bcy6npk8fhbhiz0qkaz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8tiyc1em4aocw2xoyt8eld2ou.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.8w46fni2sax5bfotyvuidisjf.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.911iy7e3ub37b1q8lpzrel0dz.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.916abqluvq7hioorqc17xgdrj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.9g5ot0s0wvm7xaqvq8m1hrjjg.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.9p2pvbt0zy5ukrbpbnb824pfm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.a4oj5fte20zn11bhz4uohtxog.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.a6mfkt39r60mpwtnzhtvqh615.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.actb0tc56m9vcnke88mos6ze6.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.aii0ghkd9swx7cih3m0swvbuq.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.aq2kpvr9y5hq5l356qakaot42.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.as1m2kxn4ctsnd8rza7dkct3w.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.bgp46wnnimy3px94wol111ehr.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.big3n3phaxj162h9n0fe7eqcm.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.bomqukqu8jvv1rium8pgm6lov.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.c0tbykef5esvw25x0ob9var8h.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.ccmoetc6tcve1fjwto0ey0nxa.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.cw5q1vxr7q5sef4e5a8m813r9.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.czf7vvcil8gqhu850m4o1id1k.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.d87yrdri0d6rv757m5d3wokaj.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.en1dzwmud3je4rces9bu7lnz5.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.f4z8aq1m6vq6tfc8xm5n2si0a.0qql4pl.rcgu.o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/crc_fast.35d1nrx3vhep32bcyldo4brb2.0qql4pl.rcgu.o /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libpanic_abort-58e7ca17f279ee2c.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrc-e9bf5a3fb54d8c43.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrc_catalog-2b954ecada58b52b.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libdigest-19e655bc742aa5b7.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libblock_buffer-4879d7a255417a1d.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libcrypto_common-dcc9be42abefd1f3.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libgeneric_array-17a4a1431b2b229c.rlib /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/libtypenum-46d8c497f70ad3aa.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libstd-ac1e0f366cac206d.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwasi-5ec33c9bb1237458.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwasi-3dc04dcc7e8548ce.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libwit_bindgen-07ccf6eb5537f8e1.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcfg_if-2b8fc8464cc71432.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_demangle-d46baddf8b53a26e.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libstd_detect-5b51ac855217f0ee.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libhashbrown-8c4807d0bf47ded7.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_std_workspace_alloc-adeafb8cbc20fea0.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libminiz_oxide-657e314d8aae041c.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libadler2-98f62f238d3961ac.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libunwind-c937bedff093a1e5.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/liblibc-df85883f14163f18.rlib -l c /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/librustc_std_workspace_core-72476db19138a823.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/liballoc-1c40e0cd1ed66cbc.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcore-d9216f2ff3c21f88.rlib /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/libcompiler_builtins-fc15b45d6c71cd23.rlib -L /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/build/wit-bindgen-0c06af0fc17a621e/out -L /Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/wasm32-wasip2/lib/self-contained --gc-sections --no-entry -O 0 -o /Users/p.hickey/src/crc-fast-rust/crc-fast-rust/target/wasm32-wasip2/debug/deps/.tmpYeCSEw/crc_fast.wasm
           #<!-- -->0 0x0000000100fdc7d0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1001087d0)
           #<!-- -->1 0x0000000100fda6a4 llvm::sys::RunSignalHandlers() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1001066a4)
           #<!-- -->2 0x0000000100fdd274 SignalHandler(int, __siginfo*, void*) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100109274)
           #<!-- -->3 0x00000001893816a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
           #<!-- -->4 0x00000001013b7edc lld::wasm::ElemSection::writeBody() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004e3edc)
           #<!-- -->5 0x00000001013bb2f0 lld::wasm::SyntheticSection::finalizeContents() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004e72f0)
           #<!-- -->6 0x00000001013c8924 lld::wasm::(anonymous namespace)::Writer::run() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004f4924)
           #<!-- -->7 0x00000001013c2250 lld::wasm::writeResult() (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004ee250)
           #<!-- -->8 0x00000001013930bc lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004bf0bc)
           #<!-- -->9 0x000000010138f168 lld::wasm::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x1004bb168)
          #<!-- -->10 0x000000010100be20 lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100137e20)
          #<!-- -->11 0x0000000100ed8408 lld_main(int, char**, llvm::ToolContext const&amp;) (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100004408)
          #<!-- -->12 0x0000000100ed8a54 main (/Users/p.hickey/.rustup/toolchains/stable-aarch64-apple-darwin/lib/rustlib/aarch64-apple-darwin/bin/rust-lld+0x100004a54)
          #<!-- -->13 0x0000000188fa6b98
          error: failed to invoke LLD: signal: 11 (SIGSEGV)


error: could not compile `crc-fast` (lib) due to 1 previous error
&lt;/pre&gt;
&lt;/details&gt;

When compiled with assertions, wasm-ld will crash with an assertion failure, and a very similar context:
 ```
= note: Assertion failed: (ctx.sym.indirectFunctionTable), function writeBody, file SyntheticSections.cpp, line 589.
```

I swapped in a wasm-ld built with asan, and got a use-after-free of something allocated in a bump arena. However, I spot-checked this on other known good projects (ones which do not crash wasm-ld) and got similar, but not identical, use-after-frees there, so I don't know if asan has revealed this crash's root cause or an unrelated issue. I'm not aware of this project's policy for san bugs in the linker, but I can prepare other reproductions if they are considered bugs.

&lt;details&gt;
&lt;pre&gt;
wasm-ld(15569,0x1f72da200) malloc: nano zone abandoned due to inability to reserve vm space.
=================================================================
==15569==ERROR: AddressSanitizer: use-after-poison on address 0x621000001569 at pc 0x0001040a9c7c bp 0x00016f42a530 sp 0x00016f429cd0
WRITE of size 8 at 0x621000001569 thread T0
    #<!-- -->0 0x0001040a9c78 in memcpy+0x29c (libclang_rt.asan_osx_dynamic.dylib:arm64e+0x85c78)
    #<!-- -->1 0x00010cd8b468 in llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&amp;, llvm::SmallVectorImpl&lt;char const*&gt;&amp;, bool)+0x168 (libLLVM.dylib:arm64+0x63468)
    #<!-- -->2 0x00010cd8c0b0 in llvm::cl::ExpansionContext::expandResponseFile(llvm::StringRef, llvm::SmallVectorImpl&lt;char const*&gt;&amp;)+0x10c (libLLVM.dylib:arm64+0x640b0)
    #<!-- -->3 0x00010cd8ccf8 in llvm::cl::ExpansionContext::expandResponseFiles(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)+0x230 (libLLVM.dylib:arm64+0x64cf8)
    #<!-- -->4 0x00010cd8dc8c in llvm::cl::ExpandResponseFiles(llvm::StringSaver&amp;, void (*)(llvm::StringRef, llvm::StringSaver&amp;, llvm::SmallVectorImpl&lt;char const*&gt;&amp;, bool), llvm::SmallVectorImpl&lt;char const*&gt;&amp;)+0xdc (libLLVM.dylib:arm64+0x65c8c)
    #<!-- -->5 0x000101e257fc in lld::wasm::(anonymous namespace)::WasmOptTable::parse(llvm::ArrayRef&lt;char const*&gt;)+0x484 (wasm-ld:arm64+0x1014557fc)
    #<!-- -->6 0x000101e20254 in lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;)+0x8f4 (wasm-ld:arm64+0x101450254)
    #<!-- -->7 0x000101e1f5f4 in lld::wasm::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool)+0x368 (wasm-ld:arm64+0x10144f5f4)
    #<!-- -->8 0x0001009e94a8 in lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool)+0x214 (wasm-ld:arm64+0x1000194a8)
    #<!-- -->9 0x0001009d26bc in lld_main(int, char**, llvm::ToolContext const&amp;)+0x2f8 (wasm-ld:arm64+0x1000026bc)
    #<!-- -->10 0x0001009d9908 in main+0x23c (wasm-ld:arm64+0x100009908)
    #<!-- -->11 0x000188fa6b94 in start+0x17b8 (dyld:arm64e+0xfffffffffff3ab94)

0x621000001569 is located 105 bytes inside of 4096-byte region [0x621000001500,0x621000002500)
allocated by thread T0 here:
    #<!-- -->0 0x0001040621a4 in __sanitizer_mz_memalign+0x80 (libclang_rt.asan_osx_dynamic.dylib:arm64e+0x3e1a4)
    #<!-- -->1 0x00018917978c in _malloc_zone_memalign+0x13c (libsystem_malloc.dylib:arm64e+0x3378c)
    #<!-- -->2 0x000189163b1c in _malloc_type_aligned_alloc_outlined+0x68 (libsystem_malloc.dylib:arm64e+0x1db1c)
    #<!-- -->3 0x0001893014d0 in operator new(unsigned long, std::align_val_t)+0x4c (libc++abi.dylib:arm64e+0x1a4d0)
    #<!-- -->4 0x0001892ff4f4 in operator new(unsigned long, std::align_val_t, std::nothrow_t const&amp;)+0xc (libc++abi.dylib:arm64e+0x184f4)
    #<!-- -->5 0x00010cdc9ef8 in llvm::allocate_buffer(unsigned long, unsigned long)+0x10 (libLLVM.dylib:arm64+0xa1ef8)
    #<!-- -->6 0x000100a6ad64 in llvm::MallocAllocator::Allocate(unsigned long, unsigned long)+0x20 (wasm-ld:arm64+0x10009ad64)
    #<!-- -->7 0x000100a6b16c in llvm::BumpPtrAllocatorImpl&lt;llvm::MallocAllocator, 4096ul, 4096ul, 128ul&gt;::StartNewSlab()+0x4c (wasm-ld:arm64+0x10009b16c)
    #<!-- -->8 0x000100a6a8e0 in llvm::BumpPtrAllocatorImpl&lt;llvm::MallocAllocator, 4096ul, 4096ul, 128ul&gt;::AllocateSlow(unsigned long, unsigned long, llvm::Align)+0x2d0 (wasm-ld:arm64+0x10009a8e0)
    #<!-- -->9 0x000100a6a36c in llvm::BumpPtrAllocatorImpl&lt;llvm::MallocAllocator, 4096ul, 4096ul, 128ul&gt;::Allocate(unsigned long, llvm::Align)+0x2dc (wasm-ld:arm64+0x10009a36c)
    #<!-- -->10 0x000100a68e14 in llvm::BumpPtrAllocatorImpl&lt;llvm::MallocAllocator, 4096ul, 4096ul, 128ul&gt;::Allocate(unsigned long, unsigned long)+0x1ac (wasm-ld:arm64+0x100098e14)
    #<!-- -->11 0x000100a68940 in lld::SpecificAllocBase::getOrCreate(void*, unsigned long, unsigned long, lld::SpecificAllocBase* (&amp;)(void*))+0x188 (wasm-ld:arm64+0x100098940)
    #<!-- -->12 0x000101e58690 in llvm::SpecificBumpPtrAllocator&lt;lld::wasm::SymbolTable&gt;&amp; lld::getSpecificAllocSingleton&lt;lld::wasm::SymbolTable&gt;()+0x24 (wasm-ld:arm64+0x101488690)
    #<!-- -->13 0x000101e1f8d8 in lld::wasm::SymbolTable* lld::make&lt;lld::wasm::SymbolTable&gt;()+0xc (wasm-ld:arm64+0x10144f8d8)
    #<!-- -->14 0x000101e1f574 in lld::wasm::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool)+0x2e8 (wasm-ld:arm64+0x10144f574)
    #<!-- -->15 0x0001009e94a8 in lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool)+0x214 (wasm-ld:arm64+0x1000194a8)
    #<!-- -->16 0x0001009d26bc in lld_main(int, char**, llvm::ToolContext const&amp;)+0x2f8 (wasm-ld:arm64+0x1000026bc)
    #<!-- -->17 0x0001009d9908 in main+0x23c (wasm-ld:arm64+0x100009908)
    #<!-- -->18 0x000188fa6b94 in start+0x17b8 (dyld:arm64e+0xfffffffffff3ab94)

SUMMARY: AddressSanitizer: use-after-poison (libLLVM.dylib:arm64+0x63468) in llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&amp;, llvm::SmallVectorImpl&lt;char const*&gt;&amp;, bool)+0x168
Shadow bytes around the buggy address:
  0x621000001280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x621000001480: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=&gt;0x621000001500: 00 00 00 00 00 00 00 00 00 00 00 00 00[f7]f7 f7
  0x621000001580: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001600: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001680: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001700: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
  0x621000001780: f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7 f7
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==15569==ABORTING
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.    Program arguments: /Users/p.hickey/src/rust/custom-lld/bin/wasm-ld @<!-- -->response.txt
 #<!-- -->0 0x0000000110b27118 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3dff118)
 #<!-- -->1 0x0000000110b2795c SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x3dff95c)
 #<!-- -->2 0x00000001893816a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->3 0x0000000189347848 (/usr/lib/system/libsystem_pthread.dylib+0x180473848)
 #<!-- -->4 0x00000001892509e4 (/usr/lib/system/libsystem_c.dylib+0x18037c9e4)
 #<!-- -->5 0x00000001040857f4 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x617f4)
 #<!-- -->6 0x0000000104084f14 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x60f14)
 #<!-- -->7 0x0000000104068dcc (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x44dcc)
 #<!-- -->8 0x0000000104068028 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x44028)
 #<!-- -->9 0x00000001040a9c88 (/Library/Developer/CommandLineTools/usr/lib/clang/17/lib/darwin/libclang_rt.asan_osx_dynamic.dylib+0x85c88)
#<!-- -->10 0x000000010cd8b46c llvm::cl::TokenizeGNUCommandLine(llvm::StringRef, llvm::StringSaver&amp;, llvm::SmallVectorImpl&lt;char const*&gt;&amp;, bool) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x6346c)
#<!-- -->11 0x000000010cd8c0b4 llvm::cl::ExpansionContext::expandResponseFile(llvm::StringRef, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x640b4)
#<!-- -->12 0x000000010cd8ccfc llvm::cl::ExpansionContext::expandResponseFiles(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x64cfc)
#<!-- -->13 0x000000010cd8dc90 llvm::cl::ExpandResponseFiles(llvm::StringSaver&amp;, void (*)(llvm::StringRef, llvm::StringSaver&amp;, llvm::SmallVectorImpl&lt;char const*&gt;&amp;, bool), llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (/opt/homebrew/Cellar/llvm/21.1.8/lib/libLLVM.dylib+0x65c90)
#<!-- -->14 0x0000000101e25800 lld::wasm::(anonymous namespace)::WasmOptTable::parse(llvm::ArrayRef&lt;char const*&gt;) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x101455800)
#<!-- -->15 0x0000000101e20258 lld::wasm::(anonymous namespace)::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x101450258)
#<!-- -->16 0x0000000101e1f5f8 lld::wasm::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x10144f5f8)
#<!-- -->17 0x00000001009e94ac lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x1000194ac)
#<!-- -->18 0x00000001009d26c0 lld_main(int, char**, llvm::ToolContext const&amp;) (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x1000026c0)
#<!-- -->19 0x00000001009d990c main (/Users/p.hickey/src/rust/custom-lld/bin/wasm-ld+0x10000990c)
#<!-- -->20 0x0000000188fa6b98
&lt;/pre&gt;
&lt;/details&gt;

If you want to reproduce running the entire build in your instrumented (or fixed) wasm-ld using rust:

RUSTFLAGS="-Clinker=/path/to/some/wasm-ld -Clink-arg=--reproduce=reproduce.tar" cargo build --target=wasm32-wasip2"

By It is safe to substitute wasm-ld for the usual linker used with wasip2 (wasm-component-ld) because wasm-component-ld first passes all of its arguments to wasm-ld, then post-processes the output from a core module to a component. The post-processing aspect isn't relevant to this bug.
</details>


---

### Comment 2 - sbc100

This crash is a sadly familiar.  It seems that somehow the `__indirect_function_table` symbol is not be correctly defined.

The logic for dealing with this symbol is IIRC kind of painful because it used to beg implicitly defined in the past.  See 63393828078c382e8e69d9b8146372b70bbce20f and all the followups.

---

### Comment 3 - sbc100

That was quite an annoying bug to reproduce but finally figured it out!  Fix is in https://github.com/llvm/llvm-project/pull/178773

---

### Comment 4 - pchickey

Hooray! Thanks so much @sbc100 

---

