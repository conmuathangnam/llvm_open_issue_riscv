# lld mingw crash

**Author:** jlsantiago0
**URL:** https://github.com/llvm/llvm-project/issues/187318
**Status:** Open
**Labels:** lld, confirmed, crash

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/ld.lld -m i386pep --shared -Bdynamic -e DllMainCRTStartup --enable-auto-image-base -o libavutil/avutil-60.dll /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/x86_64-w64-mingw32/lib/dllcrt2.o /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/x86_64-w64-mingw32/lib/crtbegin.o -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavutil -Llibswscale -Llibswresample -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/dav1d/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/zlib/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/bzip2/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/lzma/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/openssl/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/sdl2/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/dav1d/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/zlib/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/bzip2/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/lzma/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/openssl/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/sdl2/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/x86_64-w64-mingw32/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/x86_64-w64-mingw32/mingw/lib -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/clang/22/lib/windows --out-implib libavutil/libavutil.dll.a --disable-auto-image-base libavutil/avutil-60.def --image-base 0x180000000 --nxcompat --dynamicbase --high-entropy-va --as-needed --export-all-symbols --enable-runtime-pseudo-reloc -no-undefined --as-needed libavutil/adler32.o libavutil/aes.o libavutil/aes_ctr.o libavutil/ambient_viewing_environment.o libavutil/audio_fifo.o libavutil/avsscanf.o libavutil/avstring.o libavutil/base64.o libavutil/blowfish.o libavutil/bprint.o libavutil/buffer.o libavutil/camellia.o libavutil/cast5.o libavutil/channel_layout.o libavutil/container_fifo.o libavutil/cpu.o libavutil/crc.o libavutil/csp.o libavutil/des.o libavutil/detection_bbox.o libavutil/dict.o libavutil/display.o libavutil/dovi_meta.o libavutil/downmix_info.o libavutil/encryption_info.o libavutil/error.o libavutil/eval.o libavutil/executor.o libavutil/fifo.o libavutil/file.o libavutil/file_open.o libavutil/film_grain_params.o libavutil/fixed_dsp.o libavutil/float_dsp.o libavutil/float_scalarproduct.o libavutil/frame.o libavutil/hash.o libavutil/hdr_dynamic_metadata.o libavutil/hdr_dynamic_vivid_metadata.o libavutil/hmac.o libavutil/hwcontext.o libavutil/hwcontext_amf.o libavutil/hwcontext_cuda.o libavutil/hwcontext_d3d11va.o libavutil/hwcontext_d3d12va.o libavutil/hwcontext_dxva2.o libavutil/hwcontext_stub.o libavutil/iamf.o libavutil/imgutils.o libavutil/integer.o libavutil/intmath.o libavutil/lfg.o libavutil/lls.o libavutil/log.o libavutil/log2_tab.o libavutil/lzo.o libavutil/mastering_display_metadata.o libavutil/mathematics.o libavutil/md5.o libavutil/mem.o libavutil/murmur3.o libavutil/opt.o libavutil/parseutils.o libavutil/pixdesc.o libavutil/pixelutils.o libavutil/random_seed.o libavutil/rational.o libavutil/rc4.o libavutil/refstruct.o libavutil/reverse.o libavutil/ripemd.o libavutil/samplefmt.o libavutil/sha.o libavutil/sha512.o libavutil/side_data.o libavutil/slicethread.o libavutil/spherical.o libavutil/stereo3d.o libavutil/tdrdi.o libavutil/tea.o libavutil/threadmessage.o libavutil/time.o libavutil/timecode.o libavutil/timecode_internal.o libavutil/timestamp.o libavutil/tree.o libavutil/twofish.o libavutil/tx.o libavutil/tx_double.o libavutil/tx_float.o libavutil/tx_int32.o libavutil/utils.o libavutil/uuid.o libavutil/version.o libavutil/vfavutil.o libavutil/vfavutil_atomic.o libavutil/vfavutil_mem.o libavutil/video_enc_params.o libavutil/video_hint.o libavutil/x86/aes.o libavutil/x86/aes_init.o libavutil/x86/cpu.o libavutil/x86/cpuid.o libavutil/x86/crc.o libavutil/x86/fixed_dsp.o libavutil/x86/fixed_dsp_init.o libavutil/x86/float_dsp.o libavutil/x86/float_dsp_init.o libavutil/x86/imgutils.o libavutil/x86/imgutils_init.o libavutil/x86/lls.o libavutil/x86/lls_init.o libavutil/x86/pixelutils.o libavutil/x86/tx_float.o libavutil/x86/tx_float_init.o libavutil/xga_font_data.o libavutil/xtea.o libavutil/avutilres.o -lm -lz -lssl -lcrypto -luser32 -lbcrypt -ldav1d -lmingw32 /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/clang/22/lib/windows/libclang_rt.builtins-x86_64.a -lunwind -lmoldname -lmingwex -lmsvcrt -ladvapi32 -lshell32 -luser32 -lkernel32 -lmingw32 /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/clang/22/lib/windows/libclang_rt.builtins-x86_64.a -lunwind -lmoldname -lmingwex -lmsvcrt -lkernel32 /mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/x86_64-w64-mingw32/lib/crtend.o --reproduce=repro25.tar
 #0 0x000000010c5d9508 (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/libLLVM.dylib+0x154b508)
 #1 0x000000010c5d7893 (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/libLLVM.dylib+0x1549893)
 #2 0x000000010c5d9a9e (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/lib/libLLVM.dylib+0x154ba9e)
 #3 0x00007ff815c9031d (/usr/lib/system/libsystem_platform.dylib+0x7ff801f0c31d)
 #4 0x000010000000000c
 #5 0x00000001020ca920 (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x10002c920)
 #6 0x00000001020becc7 (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x100020cc7)
 #7 0x00000001024dd18c (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x10043f18c)
 #8 0x00000001020a2792 (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x100004792)
 #9 0x00000001020a0a5a (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x100002a5a)
#10 0x00000001020a08ec (/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/lld+0x1000028ec)
#11 0x00007ff8158b2530
```

[repro25.tar.gz](https://github.com/user-attachments/files/26092460/repro25.tar.gz)

Using toolchain from here:

https://github.com/mstorsjo/llvm-mingw/releases/download/20260311/llvm-mingw-20260311-ucrt-macos-universal.tar.xz

NOTE: I tested also the 21.1.x toolchains as well and they all have the same issue.

Original Command that caused the linker crash:

```
/System/Volumes/Data/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/cross/mingw-w64-win64/bin/x86_64-w64-mingw32-clang \
   -shared \
   -Wl,--out-implib,libavutil/libavutil.dll.a \
   -Wl,--disable-auto-image-base \
   libavutil/avutil-60.def \
   -Wl,--image-base,0x180000000 \
   -Llibavcodec \
   -Llibavdevice \
   -Llibavfilter \
   -Llibavformat \
   -Llibavutil \
   -Llibswscale \
   -Llibswresample \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/dav1d/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/zlib/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/bzip2/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/lzma/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/openssl/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/sdl2/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/dav1d/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/zlib/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/bzip2/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/lzma/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/openssl/lib \
   -L/mnt/jlsws5/dev/orthrus/master/build/osx15-win64/stage/sdl2/lib \
   -march=x86-64 \
   -Wl,--nxcompat,--dynamicbase \
   -Wl,--high-entropy-va \
   -Wl,--as-needed \
   -Qunused-arguments \
   -Wl,--export-all-symbols \
   -Wl,--enable-runtime-pseudo-reloc \
   -Wl,-no-undefined \
   -Wl,--as-needed \
   -o \
   libavutil/avutil-60.dll \
   @libavutil/avutil-60.dll.objs \
   -lm \
   -lz \
   -lssl \
   -lcrypto \
   -luser32 \
   -lbcrypt \
   -ldav1d \
   -Wl,--reproduce=repro25.tar \
   2>&1 | tee crash.log
```


## Comments

### Comment 1 - mstorsjo

Thanks for the report; I can reproduce this.

I reproduced the crash both with the version you mentioned, and with older versions. In particular, I could repro it both with 20250709 (LLVM 20.1.8) and 20250613 (LLVM 20.1.7); this is relevant, because since the 20.1.8 build, llvm-mingw releases are built with PGO (and ThinLTO). If crashes in the toolchain happen only when built with PGO, it is one magnitude more painful to debug it, but luckily that doesn't seem to be the case.

However I don't see crashes in a straight up local build of LLD (from 22.1.1). But it should be possible to reproduce somehow as it happens with the release binaries.

EDIT: This reproduces very nicely and easily in a build with `-DLLVM_ENABLE_ASSERTIONS=ON`.

---

