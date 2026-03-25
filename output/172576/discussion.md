# linker script causes SIGABRT

**Author:** Raupinger
**URL:** https://github.com/llvm/llvm-project/issues/172576
**Status:** Open
**Labels:** lld:ELF, crash

## Body

I ran into this while trying to get some embedded rust code to work. I'm not sure if the script I got is just broken and this is expected behavior, tbh I barely know what I'm doing atp. But the crash handler asks for a report including the stacktrace, so here I am. I'm also appending the linker script that causes this when added.

the command:
```bash
[florian@florian-thinkpad synth]$ LANG=C /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/flori

LLD 21.1.5 (/checkout/src/llvm-project/llvm 16b5ac8b0a5456eb23c72e1bff3fc9bd6e824f84) (compatible with GNU linkers)
terminate called after throwing an instance of 'std::length_error'
  what():  basic_string::_M_create
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv_rt-5ef3c95216980afb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libusb_device-73c04382b9ac072d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libheapless-0a416b1b196233d6.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libhash32-0d88857e7ab5ee84.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbyteorder-04c59ea5b68bc5fa.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libportable_atomic-e96436ba5e86cd05.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_hal_common-bfeeec97ba1f9962.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_nb-3403940c6cd5bf2e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpio-b1d1110a50985ccb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnum_enum-2bf72152d4d5dde7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libarrayvec-8109e6ce11e13ad5.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_async-51b21e585dfd32b7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk-d3d92329595c1549.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk_core-34014a87cd606766.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_dma-9a3c8538a4f1804d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libstable_deref_trait-3176585db0be7d1e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_binary_info-4058ad7bf4603b53.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv-8308b5f9b500ed70.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-6661a1fb75f9f989.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-47badc6989a78dc1.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvoid-65764fabbdf783fc.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-df609e99f0ceb9e0.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-e51c7e785ab05b74.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_pac-403532b3724fda18.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libcritical_section-5d408afd88010354.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvcell-91f6d01c984f1fc8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfugit-1aeecb9a89f55262.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libgcd-a826ec0d34e4946a.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpanic_halt-497d8270afe69c24.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcore-520121e3c0637912.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcompiler_builtins-d8d4240441e6a5a8.rlib -L /tmp/rustc6BZPQE/raw-dylibs -Bdynamic -z noexecstack -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/defmt-5fcfb1e8f08b911c/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/riscv-rt-f9fd78418b7be426/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/rp235x-pac-1ccca2560a325449/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/build -o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd --gc-sections --nmagic -Trp235x_riscv.x -Tdefmt.x --wrap=malloc --wrap=calloc --wrap=realloc --wrap=free --wrap=printf --wrap=vprintf --wrap=puts --wrap=putchar --wrap=getchar --script=/home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/pico-sdk/src/rp2_common/pico_crt0/rp2350/memmap_default.ld --gc-sections -v
 #0 0x00007fe20a040df7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x5640df7)
 #1 0x00007fe20a0416ed SignalHandler(int, siginfo_t*, void*) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x56416ed)
 #2 0x00007fe20463e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #3 0x00007fe20469890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #4 0x00007fe20463e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #5 0x00007fe20462557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #6 0x00007fe209ee81d5 __gnu_cxx::__verbose_terminate_handler() (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e81d5)
 #7 0x00007fe209ee7936 __cxxabiv1::__terminate(void (*)()) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e7936)
 #8 0x00007fe209ee79a1 (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e79a1)
 #9 0x00007fe209ee7af5 (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e7af5)
#10 0x00007fe209f2fdf2 std::__throw_length_error(char const*) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x552fdf2)
#11 0x00007fe20d54ea9f (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x8b4ea9f)
#12 0x00007fe209f648dc std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct(unsigned long, char) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x55648dc)
#13 0x00005622d975a82d lld::elf::ScriptLexer::setError(llvm::Twine const&) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x4e282d)
#14 0x00005622d97407ee lld::elf::readLinkerScript(lld::elf::Ctx&, llvm::MemoryBufferRef) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x4c87ee)
#15 0x00005622d95d2dbf lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) Driver.cpp:0:0
#16 0x00005622d95d1514 lld::elf::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x359514)
#17 0x00005622d94d773a lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25f73a)
#18 0x00005622d94d6800 lld_main(int, char**, llvm::ToolContext const&) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25e800)
#19 0x00005622d94d6eb1 main (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25eeb1)
#20 0x00007fe204627635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#21 0x00007fe2046276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#22 0x00007fe2046276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#23 0x00005622d9359469 _start (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0xe1469)
Abgebrochen                (Speicherabzug geschrieben) LANG=C /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv_rt-5ef3c95216980afb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libusb_device-73c04382b9ac072d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libheapless-0a416b1b196233d6.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libhash32-0d88857e7ab5ee84.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbyteorder-04c59ea5b68bc5fa.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libportable_atomic-e96436ba5e86cd05.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_hal_common-bfeeec97ba1f9962.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_nb-3403940c6cd5bf2e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpio-b1d1110a50985ccb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnum_enum-2bf72152d4d5dde7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libarrayvec-8109e6ce11e13ad5.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_async-51b21e585dfd32b7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk-d3d92329595c1549.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk_core-34014a87cd606766.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_dma-9a3c8538a4f1804d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libstable_deref_trait-3176585db0be7d1e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_binary_info-4058ad7bf4603b53.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv-8308b5f9b500ed70.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-6661a1fb75f9f989.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-47badc6989a78dc1.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvoid-65764fabbdf783fc.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-df609e99f0ceb9e0.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-e51c7e785ab05b74.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_pac-403532b3724fda18.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libcritical_section-5d408afd88010354.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvcell-91f6d01c984f1fc8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfugit-1aeecb9a89f55262.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libgcd-a826ec0d34e4946a.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpanic_halt-497d8270afe69c24.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcore-520121e3c0637912.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcompiler_builtins-d8d4240441e6a5a8.rlib -L /tmp/rustc6BZPQE/raw-dylibs -Bdynamic -z noexecstack -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/defmt-5fcfb1e8f08b911c/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/riscv-rt-f9fd78418b7be426/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/rp235x-pac-1ccca2560a325449/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/build -o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd --gc-sections --nmagic -Trp235x_riscv.x -Tdefmt.x --wrap=malloc --wrap=calloc --wrap=realloc --wrap=free --wrap=printf --wrap=vprintf --wrap=puts --wrap=putchar --wrap=getchar --script=/home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/pico-sdk/src/rp2_common/pico_crt0/rp2350/memmap_default.ld --gc-sections -v
```

linker script
```
/* Based on GCC ARM embedded samples.
   Defines the following symbols for use by code:
    __exidx_start
    __exidx_end
    __etext
    __data_start__
    __preinit_array_start
    __preinit_array_end
    __init_array_start
    __init_array_end
    __fini_array_start
    __fini_array_end
    __data_end__
    __bss_start__
    __bss_end__
    __end__
    end
    __HeapLimit
    __StackLimit
    __StackTop
    __stack (== StackTop)
*/

MEMORY
{
    INCLUDE "pico_flash_region.ld"
    RAM(rwx) : ORIGIN =  0x20000000, LENGTH = 512k
    SCRATCH_X(rwx) : ORIGIN = 0x20080000, LENGTH = 4k
    SCRATCH_Y(rwx) : ORIGIN = 0x20081000, LENGTH = 4k
}

ENTRY(_entry_point)

SECTIONS
{
    .flash_begin : {
        __flash_binary_start = .;
    } > FLASH

    /* The bootrom will enter the image at the point indicated in your
       IMAGE_DEF, which is usually the reset handler of your vector table.

       The debugger will use the ELF entry point, which is the _entry_point
       symbol, and in our case is *different from the bootrom's entry point.*
       This is used to go back through the bootrom on debugger launches only,
       to perform the same initial flash setup that would be performed on a
       cold boot.
    */

    .text : {
        __logical_binary_start = .;
        KEEP (*(.vectors))
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.embedded_block))
        __embedded_block_end = .;
        KEEP (*(.reset))
        /* TODO revisit this now memset/memcpy/float in ROM */
        /* bit of a hack right now to exclude all floating point and time critical (e.g. memset, memcpy) code from
         * FLASH ... we will include any thing excluded here in .data below by default */
        *(.init)
        *libgcc.a:cmse_nonsecure_call.o
        *(EXCLUDE_FILE(*libgcc.a: *libc.a:*lib_a-mem*.o *libm.a:) .text*)
        *(.fini)
        /* Pull all c'tors into .text */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        /* Followed by destructors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(SORT(.preinit_array.*)))
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        *(SORT(.fini_array.*))
        *(.fini_array)
        PROVIDE_HIDDEN (__fini_array_end = .);

        *(.eh_frame*)
        . = ALIGN(4);
    } > FLASH

    /* Note the boot2 section is optional, and should be discarded if there is
       no reference to it *inside* the binary, as it is not called by the
       bootrom. (The bootrom performs a simple best-effort XIP setup and
       leaves it to the binary to do anything more sophisticated.) However
       there is still a size limit of 256 bytes, to ensure the boot2 can be
       stored in boot RAM.

       Really this is a "XIP setup function" -- the name boot2 is historic and
       refers to its dual-purpose on RP2040, where it also handled vectoring
       from the bootrom into the user image.
    */

    .boot2 : {
        __boot2_start__ = .;
        *(.boot2)
        __boot2_end__ = .;
    } > FLASH

    ASSERT(__boot2_end__ - __boot2_start__ <= 256,
        "ERROR: Pico second stage bootloader must be no more than 256 bytes in size")

    .rodata : {
        *(EXCLUDE_FILE(*libgcc.a: *libc.a:*lib_a-mem*.o *libm.a:) .rodata*)
        *(.srodata*)
        . = ALIGN(4);
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.flashdata*)))
        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } > FLASH
    __binary_info_end = .;
    . = ALIGN(4);

    .ram_vector_table (NOLOAD): {
        *(.ram_vector_table)
    } > RAM

    .uninitialized_data (NOLOAD): {
        . = ALIGN(4);
        *(.uninitialized_data*)
    } > RAM

    .data : {
        __data_start__ = .;
        *(vtable)

        *(.time_critical*)

        /* remaining .text and .rodata; i.e. stuff we exclude above because we want it in RAM */
        *(.text*)
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);

        *(.data*)
        *(.sdata*)

        . = ALIGN(4);
        *(.after_data.*)
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__mutex_array_start = .);
        KEEP(*(SORT(.mutex_array.*)))
        KEEP(*(.mutex_array))
        PROVIDE_HIDDEN (__mutex_array_end = .);

        *(.jcr)
        . = ALIGN(4);
    } > RAM AT> FLASH

    .tdata : {
        . = ALIGN(4);
		*(.tdata .tdata.* .gnu.linkonce.td.*)
        /* All data end */
        __tdata_end = .;
    } > RAM AT> FLASH
    PROVIDE(__data_end__ = .);

    /* __etext is (for backwards compatibility) the name of the .data init source pointer (...) */
    __etext = LOADADDR(.data);

    .tbss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start__ = .;
        __tls_base = .;
        *(.tbss .tbss.* .gnu.linkonce.tb.*)
        *(.tcommon)

        __tls_end = .;
    } > RAM

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __tbss_end = .;

        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
        *(COMMON)
        PROVIDE(__global_pointer$ = . + 2K);
        *(.sbss*)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        KEEP(*(.heap*))
    } > RAM
    /* historically on GCC sbrk was growing past __HeapLimit to __StackLimit, however
       to be more compatible, we now set __HeapLimit explicitly to where the end of the heap is */
    __HeapLimit = ORIGIN(RAM) + LENGTH(RAM);

    /* Start and end symbols must be word-aligned */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > FLASH
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > FLASH
    __scratch_y_source__ = LOADADDR(.scratch_y);

    /* .stack*_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later
     *
     * stack1 section may be empty/missing if platform_launch_core1 is not used */

    /* by default we put core 0 stack at the end of scratch Y, so that if core 1
     * stack is not used then all of SCRATCH_X is free.
     */
    .stack1_dummy (NOLOAD):
    {
        *(.stack1*)
    } > SCRATCH_X
    .stack_dummy (NOLOAD):
    {
        KEEP(*(.stack*))
    } > SCRATCH_Y

    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } > FLASH =0xaa

    /* stack limit is poorly named, but historically is maximum heap ptr */
    __StackLimit = ORIGIN(RAM) + LENGTH(RAM);
    __StackOneTop = ORIGIN(SCRATCH_X) + LENGTH(SCRATCH_X);
    __StackTop = ORIGIN(SCRATCH_Y) + LENGTH(SCRATCH_Y);
    __StackOneBottom = __StackOneTop - SIZEOF(.stack1_dummy);
    __StackBottom = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* picolibc and LLVM */
    PROVIDE (__heap_start = __end__);
    PROVIDE (__heap_end = __HeapLimit);
    PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
    PROVIDE( __tls_size_align = (__tls_size + __tls_align - 1) & ~(__tls_align - 1));
    PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );

    /* llvm-libc */
    PROVIDE (_end = __end__);
    PROVIDE (__llvm_libc_heap_limit = __HeapLimit);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed")

    ASSERT( __binary_info_header_end - __logical_binary_start <= 1024, "Binary info must be in first 1024 bytes of the binary")
    ASSERT( __embedded_block_end - __logical_binary_start <= 4096, "Embedded block must be in first 4096 bytes of the binary")

    /* todo assert on extra code */
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-elf

Author: Florian  (Raupinger)

<details>
I ran into this while trying to get some embedded rust code to work. I'm not sure if the script I got is just broken and this is expected behavior, tbh I barely know what I'm doing atp. But the crash handler asks for a report including the stacktrace, so here I am. I'm also appending the linker script that causes this when added.

the command:
```bash
[florian@<!-- -->florian-thinkpad synth]$ LANG=C /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/flori

LLD 21.1.5 (/checkout/src/llvm-project/llvm 16b5ac8b0a5456eb23c72e1bff3fc9bd6e824f84) (compatible with GNU linkers)
terminate called after throwing an instance of 'std::length_error'
  what():  basic_string::_M_create
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv_rt-5ef3c95216980afb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libusb_device-73c04382b9ac072d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libheapless-0a416b1b196233d6.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libhash32-0d88857e7ab5ee84.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbyteorder-04c59ea5b68bc5fa.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libportable_atomic-e96436ba5e86cd05.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_hal_common-bfeeec97ba1f9962.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_nb-3403940c6cd5bf2e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpio-b1d1110a50985ccb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnum_enum-2bf72152d4d5dde7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libarrayvec-8109e6ce11e13ad5.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_async-51b21e585dfd32b7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk-d3d92329595c1549.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk_core-34014a87cd606766.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_dma-9a3c8538a4f1804d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libstable_deref_trait-3176585db0be7d1e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_binary_info-4058ad7bf4603b53.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv-8308b5f9b500ed70.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-6661a1fb75f9f989.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-47badc6989a78dc1.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvoid-65764fabbdf783fc.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-df609e99f0ceb9e0.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-e51c7e785ab05b74.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_pac-403532b3724fda18.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libcritical_section-5d408afd88010354.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvcell-91f6d01c984f1fc8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfugit-1aeecb9a89f55262.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libgcd-a826ec0d34e4946a.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpanic_halt-497d8270afe69c24.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcore-520121e3c0637912.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcompiler_builtins-d8d4240441e6a5a8.rlib -L /tmp/rustc6BZPQE/raw-dylibs -Bdynamic -z noexecstack -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/defmt-5fcfb1e8f08b911c/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/riscv-rt-f9fd78418b7be426/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/rp235x-pac-1ccca2560a325449/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/build -o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd --gc-sections --nmagic -Trp235x_riscv.x -Tdefmt.x --wrap=malloc --wrap=calloc --wrap=realloc --wrap=free --wrap=printf --wrap=vprintf --wrap=puts --wrap=putchar --wrap=getchar --script=/home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/pico-sdk/src/rp2_common/pico_crt0/rp2350/memmap_default.ld --gc-sections -v
 #<!-- -->0 0x00007fe20a040df7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x5640df7)
 #<!-- -->1 0x00007fe20a0416ed SignalHandler(int, siginfo_t*, void*) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x56416ed)
 #<!-- -->2 0x00007fe20463e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->3 0x00007fe20469890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #<!-- -->4 0x00007fe20463e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->5 0x00007fe20462557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #<!-- -->6 0x00007fe209ee81d5 __gnu_cxx::__verbose_terminate_handler() (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e81d5)
 #<!-- -->7 0x00007fe209ee7936 __cxxabiv1::__terminate(void (*)()) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e7936)
 #<!-- -->8 0x00007fe209ee79a1 (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e79a1)
 #<!-- -->9 0x00007fe209ee7af5 (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x54e7af5)
#<!-- -->10 0x00007fe209f2fdf2 std::__throw_length_error(char const*) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x552fdf2)
#<!-- -->11 0x00007fe20d54ea9f (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x8b4ea9f)
#<!-- -->12 0x00007fe209f648dc std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_construct(unsigned long, char) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/../../../libLLVM.so.21.1-rust-1.94.0-nightly+0x55648dc)
#<!-- -->13 0x00005622d975a82d lld::elf::ScriptLexer::setError(llvm::Twine const&amp;) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x4e282d)
#<!-- -->14 0x00005622d97407ee lld::elf::readLinkerScript(lld::elf::Ctx&amp;, llvm::MemoryBufferRef) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x4c87ee)
#<!-- -->15 0x00005622d95d2dbf lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;) Driver.cpp:0:0
#<!-- -->16 0x00005622d95d1514 lld::elf::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x359514)
#<!-- -->17 0x00005622d94d773a lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25f73a)
#<!-- -->18 0x00005622d94d6800 lld_main(int, char**, llvm::ToolContext const&amp;) (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25e800)
#<!-- -->19 0x00005622d94d6eb1 main (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0x25eeb1)
#<!-- -->20 0x00007fe204627635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->21 0x00007fe2046276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->22 0x00007fe2046276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->23 0x00005622d9359469 _start (/home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld+0xe1469)
Abgebrochen                (Speicherabzug geschrieben) LANG=C /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/rust-lld -flavor gnu /tmp/rustc6BZPQE/symbols.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.01djon4t9yo935fvm29zqq62f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.22mlm162gmx9s3u54vyo8sjxn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.2x7qyzkmxfiz6glsbc5bihs4x.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.3xc52vlfu67hxfh0wl7twm2fy.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.51uzc27dz27ivv6blolzy4ygj.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.54ryd15cqqnjh5aiqr8x0ty4p.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.5a7h3qfa6185cmc01n3qtymhn.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.70r5enqbkhm93uxdwo2dm3c7s.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.7lva9hafnvrxu1gvvmro9gdkg.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.8rs5lexhqs9u8jxm7zm7rrhqv.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.92hnwl6mo7u3f1ztpv7tf2n85.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.b058v563n0hpg02x0bcxclpx7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bbpwe7ntia8raxpg8r2jcs7r9.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bya9uty5mk49y3fytkrme3j06.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.bzdc16s2g4k6jeqe0uwyn1glq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cspflczgr3o6f7jde69rqla5e.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cv105rwsas7e5ppzi2jymm7bq.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.cybgyrsfz5nou58v2q2q0ljg7.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.d9n9pohhaluznc18osqxbcm0f.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dgvrysahbav1l0erziuk4ttm0.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.dm1l67tprhfbl0gnx5q2qj8af.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.e4kxnymjk75o8gxb4a7tqkrn1.0fxkusb.rcgu.o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd.efmoks1ik17rb2d6xnpgydizr.0fxkusb.rcgu.o --as-needed -Bstatic -lfreertos -lpico-sdk /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_hal-1594427643c6478e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libitertools-f2791af7ea8409e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libeither-598adc92d9ea606b.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_io-91c01600ac8b6b75.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librand_core-0df05d4ec466d6f2.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libsha2_const_stable-b9d98fb9629c1543.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitfield-dfae5da5149d2684.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-d298265c07f772e8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libdefmt-65daec21fc661811.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbitflags-ab27c7270b3afd24.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv_rt-5ef3c95216980afb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libusb_device-73c04382b9ac072d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libheapless-0a416b1b196233d6.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libhash32-0d88857e7ab5ee84.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libbyteorder-04c59ea5b68bc5fa.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libportable_atomic-e96436ba5e86cd05.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_hal_common-bfeeec97ba1f9962.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_nb-3403940c6cd5bf2e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpio-b1d1110a50985ccb.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnum_enum-2bf72152d4d5dde7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libarrayvec-8109e6ce11e13ad5.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal_async-51b21e585dfd32b7.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk-d3d92329595c1549.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfrunk_core-34014a87cd606766.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_dma-9a3c8538a4f1804d.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libstable_deref_trait-3176585db0be7d1e.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp_binary_info-4058ad7bf4603b53.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libriscv-8308b5f9b500ed70.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-6661a1fb75f9f989.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libembedded_hal-47badc6989a78dc1.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvoid-65764fabbdf783fc.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-df609e99f0ceb9e0.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libnb-e51c7e785ab05b74.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/librp235x_pac-403532b3724fda18.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libcritical_section-5d408afd88010354.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libvcell-91f6d01c984f1fc8.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libfugit-1aeecb9a89f55262.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libgcd-a826ec0d34e4946a.rlib /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/libpanic_halt-497d8270afe69c24.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcore-520121e3c0637912.rlib /home/florian/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/riscv32imac-unknown-none-elf/lib/libcompiler_builtins-d8d4240441e6a5a8.rlib -L /tmp/rustc6BZPQE/raw-dylibs -Bdynamic -z noexecstack -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/synth-eaa623e5ac22ad1e/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/defmt-5fcfb1e8f08b911c/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/riscv-rt-f9fd78418b7be426/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/build/rp235x-pac-1ccca2560a325449/out -L /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/build -o /home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/target/riscv32imac-unknown-none-elf/debug/deps/synth-cbcfa7347bab1ffd --gc-sections --nmagic -Trp235x_riscv.x -Tdefmt.x --wrap=malloc --wrap=calloc --wrap=realloc --wrap=free --wrap=printf --wrap=vprintf --wrap=puts --wrap=putchar --wrap=getchar --script=/home/florian/Nextcloud/sync/unia/wise25/forschungsmodul/synth/pico-sdk/src/rp2_common/pico_crt0/rp2350/memmap_default.ld --gc-sections -v
```

linker script
```
/* Based on GCC ARM embedded samples.
   Defines the following symbols for use by code:
    __exidx_start
    __exidx_end
    __etext
    __data_start__
    __preinit_array_start
    __preinit_array_end
    __init_array_start
    __init_array_end
    __fini_array_start
    __fini_array_end
    __data_end__
    __bss_start__
    __bss_end__
    __end__
    end
    __HeapLimit
    __StackLimit
    __StackTop
    __stack (== StackTop)
*/

MEMORY
{
    INCLUDE "pico_flash_region.ld"
    RAM(rwx) : ORIGIN =  0x20000000, LENGTH = 512k
    SCRATCH_X(rwx) : ORIGIN = 0x20080000, LENGTH = 4k
    SCRATCH_Y(rwx) : ORIGIN = 0x20081000, LENGTH = 4k
}

ENTRY(_entry_point)

SECTIONS
{
    .flash_begin : {
        __flash_binary_start = .;
    } &gt; FLASH

    /* The bootrom will enter the image at the point indicated in your
       IMAGE_DEF, which is usually the reset handler of your vector table.

       The debugger will use the ELF entry point, which is the _entry_point
       symbol, and in our case is *different from the bootrom's entry point.*
       This is used to go back through the bootrom on debugger launches only,
       to perform the same initial flash setup that would be performed on a
       cold boot.
    */

    .text : {
        __logical_binary_start = .;
        KEEP (*(.vectors))
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.embedded_block))
        __embedded_block_end = .;
        KEEP (*(.reset))
        /* TODO revisit this now memset/memcpy/float in ROM */
        /* bit of a hack right now to exclude all floating point and time critical (e.g. memset, memcpy) code from
         * FLASH ... we will include any thing excluded here in .data below by default */
        *(.init)
        *libgcc.a:cmse_nonsecure_call.o
        *(EXCLUDE_FILE(*libgcc.a: *libc.a:*lib_a-mem*.o *libm.a:) .text*)
        *(.fini)
        /* Pull all c'tors into .text */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        /* Followed by destructors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(SORT(.preinit_array.*)))
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        *(SORT(.fini_array.*))
        *(.fini_array)
        PROVIDE_HIDDEN (__fini_array_end = .);

        *(.eh_frame*)
        . = ALIGN(4);
    } &gt; FLASH

    /* Note the boot2 section is optional, and should be discarded if there is
       no reference to it *inside* the binary, as it is not called by the
       bootrom. (The bootrom performs a simple best-effort XIP setup and
       leaves it to the binary to do anything more sophisticated.) However
       there is still a size limit of 256 bytes, to ensure the boot2 can be
       stored in boot RAM.

       Really this is a "XIP setup function" -- the name boot2 is historic and
       refers to its dual-purpose on RP2040, where it also handled vectoring
       from the bootrom into the user image.
    */

    .boot2 : {
        __boot2_start__ = .;
        *(.boot2)
        __boot2_end__ = .;
    } &gt; FLASH

    ASSERT(__boot2_end__ - __boot2_start__ &lt;= 256,
        "ERROR: Pico second stage bootloader must be no more than 256 bytes in size")

    .rodata : {
        *(EXCLUDE_FILE(*libgcc.a: *libc.a:*lib_a-mem*.o *libm.a:) .rodata*)
        *(.srodata*)
        . = ALIGN(4);
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.flashdata*)))
        . = ALIGN(4);
    } &gt; FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } &gt; FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } &gt; FLASH
    __exidx_end = .;

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } &gt; FLASH
    __binary_info_end = .;
    . = ALIGN(4);

    .ram_vector_table (NOLOAD): {
        *(.ram_vector_table)
    } &gt; RAM

    .uninitialized_data (NOLOAD): {
        . = ALIGN(4);
        *(.uninitialized_data*)
    } &gt; RAM

    .data : {
        __data_start__ = .;
        *(vtable)

        *(.time_critical*)

        /* remaining .text and .rodata; i.e. stuff we exclude above because we want it in RAM */
        *(.text*)
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);

        *(.data*)
        *(.sdata*)

        . = ALIGN(4);
        *(.after_data.*)
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__mutex_array_start = .);
        KEEP(*(SORT(.mutex_array.*)))
        KEEP(*(.mutex_array))
        PROVIDE_HIDDEN (__mutex_array_end = .);

        *(.jcr)
        . = ALIGN(4);
    } &gt; RAM AT&gt; FLASH

    .tdata : {
        . = ALIGN(4);
		*(.tdata .tdata.* .gnu.linkonce.td.*)
        /* All data end */
        __tdata_end = .;
    } &gt; RAM AT&gt; FLASH
    PROVIDE(__data_end__ = .);

    /* __etext is (for backwards compatibility) the name of the .data init source pointer (...) */
    __etext = LOADADDR(.data);

    .tbss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start__ = .;
        __tls_base = .;
        *(.tbss .tbss.* .gnu.linkonce.tb.*)
        *(.tcommon)

        __tls_end = .;
    } &gt; RAM

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __tbss_end = .;

        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
        *(COMMON)
        PROVIDE(__global_pointer$ = . + 2K);
        *(.sbss*)
        . = ALIGN(4);
        __bss_end__ = .;
    } &gt; RAM

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        KEEP(*(.heap*))
    } &gt; RAM
    /* historically on GCC sbrk was growing past __HeapLimit to __StackLimit, however
       to be more compatible, we now set __HeapLimit explicitly to where the end of the heap is */
    __HeapLimit = ORIGIN(RAM) + LENGTH(RAM);

    /* Start and end symbols must be word-aligned */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } &gt; SCRATCH_X AT &gt; FLASH
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } &gt; SCRATCH_Y AT &gt; FLASH
    __scratch_y_source__ = LOADADDR(.scratch_y);

    /* .stack*_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later
     *
     * stack1 section may be empty/missing if platform_launch_core1 is not used */

    /* by default we put core 0 stack at the end of scratch Y, so that if core 1
     * stack is not used then all of SCRATCH_X is free.
     */
    .stack1_dummy (NOLOAD):
    {
        *(.stack1*)
    } &gt; SCRATCH_X
    .stack_dummy (NOLOAD):
    {
        KEEP(*(.stack*))
    } &gt; SCRATCH_Y

    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } &gt; FLASH =0xaa

    /* stack limit is poorly named, but historically is maximum heap ptr */
    __StackLimit = ORIGIN(RAM) + LENGTH(RAM);
    __StackOneTop = ORIGIN(SCRATCH_X) + LENGTH(SCRATCH_X);
    __StackTop = ORIGIN(SCRATCH_Y) + LENGTH(SCRATCH_Y);
    __StackOneBottom = __StackOneTop - SIZEOF(.stack1_dummy);
    __StackBottom = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* picolibc and LLVM */
    PROVIDE (__heap_start = __end__);
    PROVIDE (__heap_end = __HeapLimit);
    PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
    PROVIDE( __tls_size_align = (__tls_size + __tls_align - 1) &amp; ~(__tls_align - 1));
    PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );

    /* llvm-libc */
    PROVIDE (_end = __end__);
    PROVIDE (__llvm_libc_heap_limit = __HeapLimit);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit &gt;= __HeapLimit, "region RAM overflowed")

    ASSERT( __binary_info_header_end - __logical_binary_start &lt;= 1024, "Binary info must be in first 1024 bytes of the binary")
    ASSERT( __embedded_block_end - __logical_binary_start &lt;= 4096, "Embedded block must be in first 4096 bytes of the binary")

    /* todo assert on extra code */
}
```
</details>


---

