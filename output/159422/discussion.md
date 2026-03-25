# [RISCV] Crash in RISCVVLOptimizer

**Author:** Sharjeel-Khan
**URL:** https://github.com/llvm/llvm-project/issues/159422
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-09-19T04:22:45Z

## Body

```
PLEASE submit a bug report to https://github.com/android-ndk/ndk/issues and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld -z now -z relro --use-android-relr-tags --pack-dyn-relocs=relr --hash-style=gnu --eh-frame-hdr -m elf64lriscv -X -shared -o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_shared_cfi_apex31/unstripped/libcodec2_soft_av1enc.so -L/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/lib/clang/22/lib/linux/riscv64 -L/lib/../lib64 -L/usr/lib64 -L/lib -L/usr/lib -plugin-opt=mcpu=generic-rv64 -plugin-opt=-enable-tlsdesc --no-demangle --export-dynamic-symbol=__cfi_check --reproduce=repro.tar out/soong/.intermediates/bionic/libc/crtbegin_so/android_riscv64_apex31/crtbegin_so.o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_static_cfi_apex31/obj/frameworks/av/media/codec2/components/aom/C2SoftAomEnc.o out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_selection_static/android_riscv64_static_cfi_apex31/libcodec2_hal_selection_static.a out/soong/.intermediates/frameworks/av/media/aconfig/aconfig_mediacodec_flags_c_lib/android_riscv64_static_cfi_apex31/aconfig_mediacodec_flags_c_lib.a out/soong/.intermediates/frameworks/av/media/aconfig/android.media.codec-aconfig-cc/android_riscv64_static_cfi_apex31/android.media.codec-aconfig-cc.a out/soong/.intermediates/external/libaom/libaom/android_riscv64_static_cfi_apex31/libaom.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.ubsan_minimal-riscv64-android.a prebuilts/clang/host/linux-x86/clang-dev/android_libc++/platform/riscv64/lib/libc++demangle.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.builtins-riscv64-android.a out/soong/.intermediates/system/server_configurable_flags/libflags/server_configurable_flags/android_riscv64_shared_apex31/server_configurable_flags.so out/soong/.intermediates/build/make/tools/aconfig/aconfig_storage_read_api/libaconfig_storage_read_api_cc/android_riscv64_shared_apex31/libaconfig_storage_read_api_cc.so out/soong/.intermediates/system/libbase/libbase/android_riscv64_shared_apex31/libbase.so out/soong/.intermediates/frameworks/av/media/codec2/core/libcodec2/android_riscv64_shared_apex31/libcodec2.so out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_common/android_riscv64_shared_cfi_apex31/libcodec2_hal_common.so out/soong/.intermediates/frameworks/av/media/codec2/vndk/libcodec2_vndk/android_riscv64_shared_cfi_apex31/libcodec2_vndk.so out/soong/.intermediates/system/core/libutils/libutils/android_riscv64_shared_apex31/libutils.so out/soong/.intermediates/frameworks/av/media/codec2/components/base/libcodec2_soft_common/android_riscv64_shared_cfi_apex31/libcodec2_soft_common.so out/soong/.intermediates/system/core/libcutils/libcutils/android_riscv64_shared_apex31/libcutils.so out/soong/.intermediates/system/logging/liblog/liblog/android_riscv64_shared_current/liblog.so out/soong/.intermediates/frameworks/av/media/codec2/sfplugin/utils/libsfplugin_ccodec_utils/android_riscv64_shared_cfi_apex31/libsfplugin_ccodec_utils.so out/soong/.intermediates/frameworks/av/media/module/foundation/libstagefright_foundation/android_riscv64_shared_cfi_apex31/libstagefright_foundation.so out/soong/.intermediates/prebuilts/clang/host/linux-x86/libc++/android_riscv64_shared_apex31/libc++.so out/soong/.intermediates/bionic/libc/libc/android_riscv64_shared_current/libc.so out/soong/.intermediates/bionic/libm/libm/android_riscv64_shared_current/libm.so out/soong/.intermediates/bionic/libdl/libdl/android_riscv64_shared_current/libdl.so out/soong/.intermediates/bionic/libc/crtend_so/android_riscv64_apex31/crtend_so.o out/soong/.intermediates/bionic/libc/crt_pad_segment/android_riscv64_apex31/crt_pad_segment.o -z noexecstack -z relro -z now --build-id=md5 --fatal-warnings --no-undefined-version --exclude-libs libgcc.a --exclude-libs libgcc_stripped.a --exclude-libs libunwind_llvm.a --exclude-libs libunwind.a --icf=safe --compress-debug-sections=zstd --no-undefined -mllvm -jump-is-expensive=false -z max-page-size=4096 --pack-dyn-relocs=android+relr --gc-sections -soname libcodec2_soft_av1enc.so -Bsymbolic --version-script frameworks/av/media/codec2/components/base/exports.lds --version-script build/soong/cc/config/cfi_exports.map -plugin-opt O1 --exclude-libs=libclang_rt.builtins-riscv64-android.a --exclude-libs=libclang_rt.ubsan_minimal-riscv64-android.a
1.      Running pass 'Function Pass Manager' on module 'ld-temp.o'.
2.      Running pass 'RISC-V VL Optimizer' on function '@cdef_filter_8_0_rvv'
 #0 0x0000563ebe3061d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9e1d8)
 -shared -o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_shared_cfi_apex31/unstripped/libcodec2_soft_av1enc.so -L/usr/local/google/home/sharjeelkhan/Development/llvm-18:41:25 [21/29779]
ll/linux-x86/clang-dev/lib/clang/22/lib/linux/riscv64 -L/lib/../lib64 -L/usr/lib64 -L/lib -L/usr/lib -plugin-opt=mcpu=generic-rv64 -plugin-opt=-enable-tlsdesc --no-demangle --export-dynamic-symbol=__cfi_check --reproduce=repro.tar out/soong/.intermediates/bionic/libc/crtbegin_so/android_riscv64_apex31/crtbegin_so.o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_static_cfi_apex31/obj/frameworks/av/media/codec2/components/aom/C2So
ftAomEnc.o out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_selection_static/android_riscv64_static_cfi_apex31/libcodec2_hal_selection_static.a out/soong/.intermediates/frameworks/av/media/aconfig/aconfig_mediacodec_flags_c_lib/android_riscv64_static_cfi_apex31/aconfig_mediacodec_flags_c_lib.a out/soong/.intermediates/frameworks/av/media/aconfig/android.media.codec-aconfig-cc/android_riscv64_static_cfi_apex31/android.media.codec-aconfig-cc.a out/soong/.inter
mediates/external/libaom/libaom/android_riscv64_static_cfi_apex31/libaom.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.ubsan_minimal-riscv64-android.a prebuilts/clang/host/linux-x86/clang-dev/android_libc++/platform/riscv64/lib/libc++demangle.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.builtins-riscv64-android.a out/soong/.intermediates/system/server_configurable_flags/libflags/server_configurable_flags/android_riscv64_s
hared_apex31/server_configurable_flags.so out/soong/.intermediates/build/make/tools/aconfig/aconfig_storage_read_api/libaconfig_storage_read_api_cc/android_riscv64_shared_apex31/libaconfig_storage_read_api_cc.so out/soong/.intermediates/system/l
ibbase/libbase/android_riscv64_shared_apex31/libbase.so out/soong/.intermediates/frameworks/av/media/codec2/core/libcodec2/android_riscv64_shared_apex31/libcodec2.so out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_co
mmon/android_riscv64_shared_cfi_apex31/libcodec2_hal_common.so out/soong/.intermediates/frameworks/av/media/codec2/vndk/libcodec2_vndk/android_riscv64_shared_cfi_apex31/libcodec2_vndk.so out/soong/.intermediates/system/core/libutils/libutils/android_riscv64_shared_apex31/libutils.so out/soong/.intermediates/frameworks/av/media/codec2/components/base/libcodec2_soft_common/android_riscv64_shared_cfi_apex31/libcodec2_soft_common.so out/soong/.intermediates/system/core/libcutils/libcutils/
android_riscv64_shared_apex31/libcutils.so out/soong/.intermediates/system/logging/liblog/liblog/android_riscv64_shared_current/liblog.so out/soong/.intermediates/frameworks/av/media/codec2/sfplugin/utils/libsfplugin_ccodec_utils/android_riscv64
_shared_cfi_apex31/libsfplugin_ccodec_utils.so out/soong/.intermediates/frameworks/av/media/module/foundation/libstagefright_foundation/android_riscv64_shared_cfi_apex31/libstagefright_foundation.so out/soong/.intermediates/prebuilts/clang/host/
linux-x86/libc++/android_riscv64_shared_apex31/libc++.so out/soong/.intermediates/bionic/libc/libc/android_riscv64_shared_current/libc.so out/soong/.intermediates/bionic/libm/libm/android_riscv64_shared_current/libm.so out/soong/.intermediates/bionic/libdl/libdl/android_riscv64_shared_current/libdl.so out/soong/.intermediates/bionic/libc/crtend_so/android_riscv64_apex31/crtend_so.o out/soong/.intermediates/bionic/libc/crt_pad_segment/android_riscv64_apex31/crt_pad_segment.o -z noexecst
ack -z relro -z now --build-id=md5 --fatal-warnings --no-undefined-version --exclude-libs libgcc.a --exclude-libs libgcc_stripped.a --exclude-libs libunwind_llvm.a --exclude-libs libunwind.a --icf=safe --compress-debug-sections=zstd --no-undefin
ed -mllvm -jump-is-expensive=false -z max-page-size=4096 --pack-dyn-relocs=android+relr --gc-sections -soname libcodec2_soft_av1enc.so -Bsymbolic --version-script frameworks/av/media/codec2/components/base/exports.lds --version-script build/soon
g/cc/config/cfi_exports.map -plugin-opt O1 --exclude-libs=libclang_rt.builtins-riscv64-android.a --exclude-libs=libclang_rt.ubsan_minimal-riscv64-android.a
1.      Running pass 'Function Pass Manager' on module 'ld-temp.o'.
2.      Running pass 'RISC-V VL Optimizer' on function '@cdef_filter_8_0_rvv'
 #0 0x0000563ebe3061d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9e1d8)
 #1 0x0000563ebe303bb5 llvm::sys::RunSignalHandlers() (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9bbb5)
 #2 0x0000563ebe307036 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f9999a7cdf0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #4 0x0000563ebed0691b isSupportedInstr(llvm::MachineInstr const&) RISCVVLOptimizer.cpp:0:0
 #5 0x0000563ebed04a44 (anonymous namespace)::RISCVVLOptimizer::runOnMachineFunction(llvm::MachineFunction&) RISCVVLOptimizer.cpp:0:0
 #6 0x0000563ebf9e173b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x337973b)
 #7 0x0000563ec0e609aa llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47f89aa)
 #8 0x0000563ec0e6705c llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47ff05c)
 #9 0x0000563ec0e616cf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47f96cf)
#10 0x0000563ebf1b0208 codegen(llvm::lto::Config const&, llvm::TargetMachine*, std::__1::function<llvm::Expected<std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>,
 unsigned int, llvm::Module&, llvm::ModuleSummaryIndex const&) LTOBackend.cpp:0:0
#11 0x0000563ebf1af2f3 llvm::lto::backend(llvm::lto::Config const&, std::__1::function<llvm::Expected<std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, unsigned i
nt, llvm::Module&, llvm::ModuleSummaryIndex&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b472f3)
#12 0x0000563ebf19681e llvm::lto::LTO::runRegularLTO(std::__1::function<llvm::Expected<std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>) (/usr/local/google/home/s
harjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b2e81e)
#13 0x0000563ebf195ce9 llvm::lto::LTO::run(std::__1::function<llvm::Expected<std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::FileCache) (/usr/local/google
/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b2dce9)
#14 0x0000563ebe4e07cc lld::elf::BitcodeCompiler::compile() (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1e787cc)
#15 0x0000563ebe439b8c void lld::elf::LinkerDriver::compileBitcodeFiles<llvm::object::ELFType<(llvm::endianness)1, true>>(bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1dd1b8c)
#16 0x0000563ebe422f60 void lld::elf::LinkerDriver::link<llvm::object::ELFType<(llvm::endianness)1, true>>(llvm::opt::InputArgList&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1db
af60)
#17 0x0000563ebe4074dd lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1d9f4dd)
#18 0x0000563ebe4039ed lld::elf::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1d9b9ed)
#19 0x0000563ebe3091ef lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/
bin/ld.lld+0x1ca11ef)
#20 0x0000563ebe268445 lld_main(int, char**, llvm::ToolContext const&) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c00445)
#21 0x0000563ebe2689a7 main (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c009a7)
#22 0x00007f9999a66ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007f9999a66d65 call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007f9999a66d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x0000563ebe268069 _start (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c00069)
```

I have attached the [repro](https://drive.google.com/file/d/11KSg9X3ZYiUBlIoQjXrDNyhH8VqVHvvq/view?usp=sharing)

## Comments

### Comment 1 - Sharjeel-Khan

I believe some change in https://github.com/llvm/llvm-project/pull/151285 caused it because it started after it

---

### Comment 2 - Sharjeel-Khan

CCing @lukel97 

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Sharjeel Khan (Sharjeel-Khan)

<details>
```
PLEASE submit a bug report to https://github.com/android-ndk/ndk/issues and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld -z now -z relro --use-android-relr-tags --pack-dyn-relocs=relr --hash-style=gnu --eh-frame-hdr -m elf64lriscv -X -shared -o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_shared_cfi_apex31/unstripped/libcodec2_soft_av1enc.so -L/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/lib/clang/22/lib/linux/riscv64 -L/lib/../lib64 -L/usr/lib64 -L/lib -L/usr/lib -plugin-opt=mcpu=generic-rv64 -plugin-opt=-enable-tlsdesc --no-demangle --export-dynamic-symbol=__cfi_check --reproduce=repro.tar out/soong/.intermediates/bionic/libc/crtbegin_so/android_riscv64_apex31/crtbegin_so.o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_static_cfi_apex31/obj/frameworks/av/media/codec2/components/aom/C2SoftAomEnc.o out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_selection_static/android_riscv64_static_cfi_apex31/libcodec2_hal_selection_static.a out/soong/.intermediates/frameworks/av/media/aconfig/aconfig_mediacodec_flags_c_lib/android_riscv64_static_cfi_apex31/aconfig_mediacodec_flags_c_lib.a out/soong/.intermediates/frameworks/av/media/aconfig/android.media.codec-aconfig-cc/android_riscv64_static_cfi_apex31/android.media.codec-aconfig-cc.a out/soong/.intermediates/external/libaom/libaom/android_riscv64_static_cfi_apex31/libaom.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.ubsan_minimal-riscv64-android.a prebuilts/clang/host/linux-x86/clang-dev/android_libc++/platform/riscv64/lib/libc++demangle.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.builtins-riscv64-android.a out/soong/.intermediates/system/server_configurable_flags/libflags/server_configurable_flags/android_riscv64_shared_apex31/server_configurable_flags.so out/soong/.intermediates/build/make/tools/aconfig/aconfig_storage_read_api/libaconfig_storage_read_api_cc/android_riscv64_shared_apex31/libaconfig_storage_read_api_cc.so out/soong/.intermediates/system/libbase/libbase/android_riscv64_shared_apex31/libbase.so out/soong/.intermediates/frameworks/av/media/codec2/core/libcodec2/android_riscv64_shared_apex31/libcodec2.so out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_common/android_riscv64_shared_cfi_apex31/libcodec2_hal_common.so out/soong/.intermediates/frameworks/av/media/codec2/vndk/libcodec2_vndk/android_riscv64_shared_cfi_apex31/libcodec2_vndk.so out/soong/.intermediates/system/core/libutils/libutils/android_riscv64_shared_apex31/libutils.so out/soong/.intermediates/frameworks/av/media/codec2/components/base/libcodec2_soft_common/android_riscv64_shared_cfi_apex31/libcodec2_soft_common.so out/soong/.intermediates/system/core/libcutils/libcutils/android_riscv64_shared_apex31/libcutils.so out/soong/.intermediates/system/logging/liblog/liblog/android_riscv64_shared_current/liblog.so out/soong/.intermediates/frameworks/av/media/codec2/sfplugin/utils/libsfplugin_ccodec_utils/android_riscv64_shared_cfi_apex31/libsfplugin_ccodec_utils.so out/soong/.intermediates/frameworks/av/media/module/foundation/libstagefright_foundation/android_riscv64_shared_cfi_apex31/libstagefright_foundation.so out/soong/.intermediates/prebuilts/clang/host/linux-x86/libc++/android_riscv64_shared_apex31/libc++.so out/soong/.intermediates/bionic/libc/libc/android_riscv64_shared_current/libc.so out/soong/.intermediates/bionic/libm/libm/android_riscv64_shared_current/libm.so out/soong/.intermediates/bionic/libdl/libdl/android_riscv64_shared_current/libdl.so out/soong/.intermediates/bionic/libc/crtend_so/android_riscv64_apex31/crtend_so.o out/soong/.intermediates/bionic/libc/crt_pad_segment/android_riscv64_apex31/crt_pad_segment.o -z noexecstack -z relro -z now --build-id=md5 --fatal-warnings --no-undefined-version --exclude-libs libgcc.a --exclude-libs libgcc_stripped.a --exclude-libs libunwind_llvm.a --exclude-libs libunwind.a --icf=safe --compress-debug-sections=zstd --no-undefined -mllvm -jump-is-expensive=false -z max-page-size=4096 --pack-dyn-relocs=android+relr --gc-sections -soname libcodec2_soft_av1enc.so -Bsymbolic --version-script frameworks/av/media/codec2/components/base/exports.lds --version-script build/soong/cc/config/cfi_exports.map -plugin-opt O1 --exclude-libs=libclang_rt.builtins-riscv64-android.a --exclude-libs=libclang_rt.ubsan_minimal-riscv64-android.a
1.      Running pass 'Function Pass Manager' on module 'ld-temp.o'.
2.      Running pass 'RISC-V VL Optimizer' on function '@<!-- -->cdef_filter_8_0_rvv'
 #<!-- -->0 0x0000563ebe3061d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9e1d8)
 -shared -o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_shared_cfi_apex31/unstripped/libcodec2_soft_av1enc.so -L/usr/local/google/home/sharjeelkhan/Development/llvm-18:41:25 [21/29779]
ll/linux-x86/clang-dev/lib/clang/22/lib/linux/riscv64 -L/lib/../lib64 -L/usr/lib64 -L/lib -L/usr/lib -plugin-opt=mcpu=generic-rv64 -plugin-opt=-enable-tlsdesc --no-demangle --export-dynamic-symbol=__cfi_check --reproduce=repro.tar out/soong/.intermediates/bionic/libc/crtbegin_so/android_riscv64_apex31/crtbegin_so.o out/soong/.intermediates/frameworks/av/media/codec2/components/aom/libcodec2_soft_av1enc/android_riscv64_static_cfi_apex31/obj/frameworks/av/media/codec2/components/aom/C2So
ftAomEnc.o out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_selection_static/android_riscv64_static_cfi_apex31/libcodec2_hal_selection_static.a out/soong/.intermediates/frameworks/av/media/aconfig/aconfig_mediacodec_flags_c_lib/android_riscv64_static_cfi_apex31/aconfig_mediacodec_flags_c_lib.a out/soong/.intermediates/frameworks/av/media/aconfig/android.media.codec-aconfig-cc/android_riscv64_static_cfi_apex31/android.media.codec-aconfig-cc.a out/soong/.inter
mediates/external/libaom/libaom/android_riscv64_static_cfi_apex31/libaom.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.ubsan_minimal-riscv64-android.a prebuilts/clang/host/linux-x86/clang-dev/android_libc++/platform/riscv64/lib/libc++demangle.a prebuilts/clang/host/linux-x86/clang-dev/lib/clang/22/lib/linux/libclang_rt.builtins-riscv64-android.a out/soong/.intermediates/system/server_configurable_flags/libflags/server_configurable_flags/android_riscv64_s
hared_apex31/server_configurable_flags.so out/soong/.intermediates/build/make/tools/aconfig/aconfig_storage_read_api/libaconfig_storage_read_api_cc/android_riscv64_shared_apex31/libaconfig_storage_read_api_cc.so out/soong/.intermediates/system/l
ibbase/libbase/android_riscv64_shared_apex31/libbase.so out/soong/.intermediates/frameworks/av/media/codec2/core/libcodec2/android_riscv64_shared_apex31/libcodec2.so out/soong/.intermediates/frameworks/av/media/codec2/hal/common/libcodec2_hal_co
mmon/android_riscv64_shared_cfi_apex31/libcodec2_hal_common.so out/soong/.intermediates/frameworks/av/media/codec2/vndk/libcodec2_vndk/android_riscv64_shared_cfi_apex31/libcodec2_vndk.so out/soong/.intermediates/system/core/libutils/libutils/android_riscv64_shared_apex31/libutils.so out/soong/.intermediates/frameworks/av/media/codec2/components/base/libcodec2_soft_common/android_riscv64_shared_cfi_apex31/libcodec2_soft_common.so out/soong/.intermediates/system/core/libcutils/libcutils/
android_riscv64_shared_apex31/libcutils.so out/soong/.intermediates/system/logging/liblog/liblog/android_riscv64_shared_current/liblog.so out/soong/.intermediates/frameworks/av/media/codec2/sfplugin/utils/libsfplugin_ccodec_utils/android_riscv64
_shared_cfi_apex31/libsfplugin_ccodec_utils.so out/soong/.intermediates/frameworks/av/media/module/foundation/libstagefright_foundation/android_riscv64_shared_cfi_apex31/libstagefright_foundation.so out/soong/.intermediates/prebuilts/clang/host/
linux-x86/libc++/android_riscv64_shared_apex31/libc++.so out/soong/.intermediates/bionic/libc/libc/android_riscv64_shared_current/libc.so out/soong/.intermediates/bionic/libm/libm/android_riscv64_shared_current/libm.so out/soong/.intermediates/bionic/libdl/libdl/android_riscv64_shared_current/libdl.so out/soong/.intermediates/bionic/libc/crtend_so/android_riscv64_apex31/crtend_so.o out/soong/.intermediates/bionic/libc/crt_pad_segment/android_riscv64_apex31/crt_pad_segment.o -z noexecst
ack -z relro -z now --build-id=md5 --fatal-warnings --no-undefined-version --exclude-libs libgcc.a --exclude-libs libgcc_stripped.a --exclude-libs libunwind_llvm.a --exclude-libs libunwind.a --icf=safe --compress-debug-sections=zstd --no-undefin
ed -mllvm -jump-is-expensive=false -z max-page-size=4096 --pack-dyn-relocs=android+relr --gc-sections -soname libcodec2_soft_av1enc.so -Bsymbolic --version-script frameworks/av/media/codec2/components/base/exports.lds --version-script build/soon
g/cc/config/cfi_exports.map -plugin-opt O1 --exclude-libs=libclang_rt.builtins-riscv64-android.a --exclude-libs=libclang_rt.ubsan_minimal-riscv64-android.a
1.      Running pass 'Function Pass Manager' on module 'ld-temp.o'.
2.      Running pass 'RISC-V VL Optimizer' on function '@<!-- -->cdef_filter_8_0_rvv'
 #<!-- -->0 0x0000563ebe3061d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9e1d8)
 #<!-- -->1 0x0000563ebe303bb5 llvm::sys::RunSignalHandlers() (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c9bbb5)
 #<!-- -->2 0x0000563ebe307036 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f9999a7cdf0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->4 0x0000563ebed0691b isSupportedInstr(llvm::MachineInstr const&amp;) RISCVVLOptimizer.cpp:0:0
 #<!-- -->5 0x0000563ebed04a44 (anonymous namespace)::RISCVVLOptimizer::runOnMachineFunction(llvm::MachineFunction&amp;) RISCVVLOptimizer.cpp:0:0
 #<!-- -->6 0x0000563ebf9e173b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x337973b)
 #<!-- -->7 0x0000563ec0e609aa llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47f89aa)
 #<!-- -->8 0x0000563ec0e6705c llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47ff05c)
 #<!-- -->9 0x0000563ec0e616cf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x47f96cf)
#<!-- -->10 0x0000563ebf1b0208 codegen(llvm::lto::Config const&amp;, llvm::TargetMachine*, std::__1::function&lt;llvm::Expected&lt;std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;,
 unsigned int, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;) LTOBackend.cpp:0:0
#<!-- -->11 0x0000563ebf1af2f3 llvm::lto::backend(llvm::lto::Config const&amp;, std::__1::function&lt;llvm::Expected&lt;std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, unsigned i
nt, llvm::Module&amp;, llvm::ModuleSummaryIndex&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b472f3)
#<!-- -->12 0x0000563ebf19681e llvm::lto::LTO::runRegularLTO(std::__1::function&lt;llvm::Expected&lt;std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;) (/usr/local/google/home/s
harjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b2e81e)
#<!-- -->13 0x0000563ebf195ce9 llvm::lto::LTO::run(std::__1::function&lt;llvm::Expected&lt;std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::FileCache) (/usr/local/google
/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x2b2dce9)
#<!-- -->14 0x0000563ebe4e07cc lld::elf::BitcodeCompiler::compile() (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1e787cc)
#<!-- -->15 0x0000563ebe439b8c void lld::elf::LinkerDriver::compileBitcodeFiles&lt;llvm::object::ELFType&lt;(llvm::endianness)1, true&gt;&gt;(bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1dd1b8c)
#<!-- -->16 0x0000563ebe422f60 void lld::elf::LinkerDriver::link&lt;llvm::object::ELFType&lt;(llvm::endianness)1, true&gt;&gt;(llvm::opt::InputArgList&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1db
af60)
#<!-- -->17 0x0000563ebe4074dd lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1d9f4dd)
#<!-- -->18 0x0000563ebe4039ed lld::elf::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1d9b9ed)
#<!-- -->19 0x0000563ebe3091ef lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/
bin/ld.lld+0x1ca11ef)
#<!-- -->20 0x0000563ebe268445 lld_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c00445)
#<!-- -->21 0x0000563ebe2689a7 main (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c009a7)
#<!-- -->22 0x00007f9999a66ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007f9999a66d65 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007f9999a66d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x0000563ebe268069 _start (/usr/local/google/home/sharjeelkhan/Development/llvm-toolchain/ToT/install/linux-x86/clang-dev/bin/ld.lld+0x1c00069)
```

I have attached the [repro](https://drive.google.com/file/d/11KSg9X3ZYiUBlIoQjXrDNyhH8VqVHvvq/view?usp=sharing)
</details>


---

### Comment 4 - lukel97

I can't seem to find any files in repro.tar. Do you have a .ll file or .c file which can reproduce it?

---

### Comment 5 - hvdijk

The files in `repro.tar` are there, ignore the "No items" text and use the download button in the top right corner. Inside the `repro` directory, run `/path/to/ld.lld @response.txt`.

The problem is in `RISCVVLOptimizer::transfer`, it assumes that `MRI->getVRegDef(MO.getReg())` returns an instruction, but the MIR contains a `DBG_VALUE` for a vreg whose definition has been optimised away.

---

