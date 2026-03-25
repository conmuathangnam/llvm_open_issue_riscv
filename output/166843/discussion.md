# [clangd] Assertion failure in clangd: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/166843
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-11-10T19:28:26Z

## Body

We're seeing an assertion fire in clangd
```
clangd: llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From *) [To = clang::UsingShadowDecl, From = const clang::NamedDecl]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
```

I'm working on getting code and logs I can share, since we're seeing this in internal code. This may take me a bit but I wanted to report this as early as possible. The code base triggering this had a pretty massive upgrade from 03b0f55d9c6319a851a60bb084faca0e32a38f2b to 62adc83c918b1b55f70e4ee33c727b040da2a9ed, but I'm still able to trigger the assertions as of this morning.

I'm aware of https://clangd.llvm.org/troubleshooting, so I'll try to get logs, a simplified command, and a credeuced/sanitized source file, but some of these steps can take a bit to complete.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Paul Kirth (ilovepi)

<details>
We're seeing an assertion fire in clangd
```
clangd: llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From *) [To = clang::UsingShadowDecl, From = const clang::NamedDecl]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
```

I'm working on getting code and logs I can share, since we're seeing this in internal code. This may take me a bit but I wanted to report this as early as possible. The code base triggering this had a pretty massive upgrade from 03b0f55d9c6319a851a60bb084faca0e32a38f2b to 62adc83c918b1b55f70e4ee33c727b040da2a9ed, but I'm still able to trigger the assertions as of this morning.

I'm aware of https://clangd.llvm.org/troubleshooting, so I'll try to get logs, a simplified command, and a credeuced/sanitized source file, but some of these steps can take a bit to complete.


</details>


---

### Comment 2 - ilovepi

We were able to repo this on completely open source code. I'm trying to get a reduced case but we do have a failure w/ a single file and straightforward instructions w/o a huge checkout.

Error log + backtrace on asserts build
```console
 ~/assert-clang/bin/clangd -j=1 --background-index=0 --clang-tidy=0 --compile-commands-dir=environment/pigweed-venv/gn --check=pw_rpc/pwpb/client_integration_test.cc --log=verbose
I[15:58:15.895] Fuchsia clangd version 22.0.0git (https://llvm.googlesource.com/llvm-project 5d9d89092e0fd34f3ea2ca15c9f30d4c01bb29f3)
I[15:58:15.895] Features: linux+debug
I[15:58:15.895] PID: 2986600
I[15:58:15.895] Working directory: /usr/local/google/home/paulkirth/pigweed
I[15:58:15.895] argv[0]: /usr/local/google/home/paulkirth/assert-clang/bin/clangd
I[15:58:15.895] argv[1]: -j=1
I[15:58:15.895] argv[2]: --background-index=0
I[15:58:15.895] argv[3]: --clang-tidy=0
I[15:58:15.895] argv[4]: --compile-commands-dir=environment/pigweed-venv/gn
I[15:58:15.895] argv[5]: --check=pw_rpc/pwpb/client_integration_test.cc
I[15:58:15.895] argv[6]: --log=verbose
V[15:58:15.895] User config file is /usr/local/google/home/paulkirth/.config/clangd/config.yaml
I[15:58:15.895] Entering check mode (no LSP server)
I[15:58:15.895] Testing on source file /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc
V[15:58:15.895] Config fragment: compiling <unknown>:0 -> 0x0000556E51654AD8 (trusted=false)
I[15:58:15.895] Loading compilation database...
I[15:58:15.947] Loaded compilation database from /usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/compile_commands.json
V[15:58:15.948] Broadcasting compilation database from /usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn
I[15:58:15.948] Compile command from CDB is: [/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn] ../../cipd/packages/pigweed/bin/clang++ --driver-mode=g++ -MMD -MF pw_strict_host_clang_size_optimized/obj/pw_rpc/pwpb/client_integration_test.client_integration_test.cc.o.d -fdiagnostics-color -g -fno-common -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -Wall -Wextra -Wimplicit-fallthrough -Wcast-qual -Wundef -Wpointer-arith -Werror -Wno-error=cpp -Wno-error=deprecated-declarations -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -ffile-prefix-map=../../../= -ffile-prefix-map==out/ -ffile-prefix-map=../../../= -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -ffile-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -ffixed-point -g3 --warning-suppression-mappings=../../../pw_toolchain/cc/args/warning_exceptions.txt --sysroot=../../cipd/packages/pigweed/clang_sysroot/ -Oz -Wshadow -Wredundant-decls -Wthread-safety -Wthread-safety-beta -Wswitch-enum -Wextra-semi -Wpedantic -Wno-c++20-designator -Wno-gnu-zero-variadic-macro-arguments -Wshadow-all -fno-rtti -Wnon-virtual-dtor -std=c++17 -Wno-register -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DPW_SPAN_ENABLE_ASSERTS=true -DPW_STATUS_CFG_CHECK_IF_USED=1 -DPW_STRING_ENABLE_DECIMAL_FLOAT_EXPANSION=1 -I../../../pw_rpc/pwpb/public -I../../../pw_sync_stl/public -I../../../pw_sync_stl/public_overrides -I../../../pw_sync/public -I../../../pw_chrono_stl/public -I../../../pw_chrono_stl/clock_public_overrides -I../../../pw_chrono/public -I../../../pw_preprocessor/public -I../../../pw_polyfill/public -I../../../pw_rpc/public -Iprotocol_buffer/gen/pw_rpc/protos.proto_library/pwpb -I../../../pw_containers/public -I../../../pw_toolchain/public -I../../../pw_assert/print_and_abort_assert_public_overrides -I../../../pw_assert/public -I../../../pw_assert/print_and_abort_check_public_overrides -I../../../pw_numeric/public -I../../../pw_string/public -I../../../pw_function/public -I../../../third_party/fuchsia/repo/sdk/lib/fit/include -I../../../third_party/fuchsia/repo/sdk/lib/stdcompat/include -I../../../pw_protobuf/public -I../../../pw_bytes/public -I../../../pw_stream/public -I../../../pw_span/public -I../../../pw_status/public -I../../../pw_result/public -I../../../pw_varint/public -I../../../pw_log_basic/public_overrides -I../../../pw_log_basic/public -I../../../pw_log/public -I../../../pw_hdlc/public -I../../../pw_checksum/public -I../../../pw_unit_test/public -I../../../pw_unit_test/light_public_overrides -I../../../pw_unit_test/public_overrides -Iprotocol_buffer/gen/pw_rpc/protos.proto_library/pwpb_rpc -I../../../pw_rpc/raw/public -c -o pw_strict_host_clang_size_optimized/obj/pw_rpc/pwpb/client_integration_test.client_integration_test.cc.o -resource-dir=/usr/local/google/home/paulkirth/assert-clang/lib/clang/22 -- /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc
I[15:58:15.948] Parsing command...
V[15:58:15.949] Ignored diagnostic. argument unused during compilation: '-c'
I[15:58:15.950] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -Oz -fsyntax-only -disable-free -clear-ast-before-backend -main-file-name client_integration_test.cc -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -gkey-instructions -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fdebug-prefix-map=../../../= -fdebug-prefix-map==out/ -fdebug-prefix-map=../../../= -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fdebug-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -ffunction-sections -fdata-sections -fcoverage-compilation-dir=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn -resource-dir /usr/local/google/home/paulkirth/assert-clang/lib/clang/22 -dependency-file pw_strict_host_clang_size_optimized/obj/pw_rpc/pwpb/client_integration_test.client_integration_test.cc.o.d -MT pw_strict_host_clang_size_optimized/obj/pw_rpc/pwpb/client_integration_test.client_integration_test.cc.o -D _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -D PW_SPAN_ENABLE_ASSERTS=true -D PW_STATUS_CFG_CHECK_IF_USED=1 -D PW_STRING_ENABLE_DECIMAL_FLOAT_EXPANSION=1 -I ../../../pw_rpc/pwpb/public -I ../../../pw_sync_stl/public -I ../../../pw_sync_stl/public_overrides -I ../../../pw_sync/public -I ../../../pw_chrono_stl/public -I ../../../pw_chrono_stl/clock_public_overrides -I ../../../pw_chrono/public -I ../../../pw_preprocessor/public -I ../../../pw_polyfill/public -I ../../../pw_rpc/public -I protocol_buffer/gen/pw_rpc/protos.proto_library/pwpb -I ../../../pw_containers/public -I ../../../pw_toolchain/public -I ../../../pw_assert/print_and_abort_assert_public_overrides -I ../../../pw_assert/public -I ../../../pw_assert/print_and_abort_check_public_overrides -I ../../../pw_numeric/public -I ../../../pw_string/public -I ../../../pw_function/public -I ../../../third_party/fuchsia/repo/sdk/lib/fit/include -I ../../../third_party/fuchsia/repo/sdk/lib/stdcompat/include -I ../../../pw_protobuf/public -I ../../../pw_bytes/public -I ../../../pw_stream/public -I ../../../pw_span/public -I ../../../pw_status/public -I ../../../pw_result/public -I ../../../pw_varint/public -I ../../../pw_log_basic/public_overrides -I ../../../pw_log_basic/public -I ../../../pw_log/public -I ../../../pw_hdlc/public -I ../../../pw_checksum/public -I ../../../pw_unit_test/public -I ../../../pw_unit_test/light_public_overrides -I ../../../pw_unit_test/public_overrides -I protocol_buffer/gen/pw_rpc/protos.proto_library/pwpb_rpc -I ../../../pw_rpc/raw/public -isysroot ../../cipd/packages/pigweed/clang_sysroot/ -internal-isystem ../../cipd/packages/pigweed/bin/../include/x86_64-unknown-linux-gnu/c++/v1 -internal-isystem ../../cipd/packages/pigweed/bin/../include/c++/v1 -internal-isystem /usr/local/google/home/paulkirth/assert-clang/lib/clang/22/include -internal-isystem ../../cipd/packages/pigweed/clang_sysroot/usr/local/include -internal-externc-isystem ../../cipd/packages/pigweed/clang_sysroot/usr/include/x86_64-linux-gnu -internal-externc-isystem ../../cipd/packages/pigweed/clang_sysroot/include -internal-externc-isystem ../../cipd/packages/pigweed/clang_sysroot/usr/include -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fmacro-prefix-map=../../../= -fmacro-prefix-map==out/ -fmacro-prefix-map=../../../= -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fmacro-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fcoverage-prefix-map=../../../= -fcoverage-prefix-map==out/ -fcoverage-prefix-map=../../../= -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/= -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn= -fcoverage-prefix-map=/usr/local/google/home/paulkirth/pigweed/environment/pigweed-venv/gn/=out/ -Wall -Wextra -Wimplicit-fallthrough -Wcast-qual -Wundef -Wpointer-arith -Werror -Wno-error=cpp -Wno-error=deprecated-declarations -Wshadow -Wredundant-decls -Wthread-safety -Wthread-safety-beta -Wswitch-enum -Wextra-semi -Wpedantic -Wno-c++20-designator -Wno-gnu-zero-variadic-macro-arguments -Wshadow-all -Wnon-virtual-dtor -Wno-register -ffixed-point -std=c++17 -fdeprecated-macro -ferror-limit 19 -fmessage-length=213 -fno-rtti -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf --warning-suppression-mappings=../../../pw_toolchain/cc/args/warning_exceptions.txt -vectorize-slp -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++ /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc
I[15:58:15.950] Building preamble...
I[15:58:16.779] Built preamble of size 10735760 for file /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc version null in 0.83 seconds
I[15:58:16.779] Indexing headers...
V[15:58:16.921] indexed preamble AST for /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc version null:
  symbol slab: 10529 symbols, 3032672 bytes
  ref slab: 0 symbols, 0 refs, 128 bytes
  relations slab: 337 relations, 8728 bytes
V[15:58:16.946] Build dynamic index for header symbols with estimated memory usage of 7972396 bytes
E[15:58:16.950] [pp_file_not_found] Line 16: 'pw_rpc/benchmark.rpc.pwpb.h' file not found
I[15:58:16.950] Building AST...
V[15:58:16.971] Dropped diagnostic: /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc: in template: static assertion expression is not an integral constant expression
V[15:58:16.972] Dropped diagnostic: /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc: in template: 'operator bool' is a protected member of 'fit::internal::generic_function_base<8>'
clangd: llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From *) [To = clang::UsingShadowDecl, From = const clang::NamedDecl]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /usr/local/google/home/paulkirth/assert-clang/bin/clangd -j=1 --background-index=0 --clang-tidy=0 --compile-commands-dir=environment/pigweed-venv/gn --check=pw_rpc/pwpb/client_integration_test.cc --log=verbose
1.      /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc:65:51: current parser token 'Message'
2.      /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc:21:1: parsing namespace 'pwpb_rpc_test'
3.      /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc:22:1: parsing namespace 'pwpb_rpc_test::(anonymous)'
4.      /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc:64:64: parsing function body 'pwpb_rpc_test::(anonymous namespace)::Payload'
5.      /usr/local/google/home/paulkirth/pigweed/pw_rpc/pwpb/client_integration_test.cc:64:64: in compound statement ('{}')
#0 0x0000556e3e9724d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/google/home/paulkirth/assert-clang/bin/clangd+0xf7a4d8)
[1]    2986600 IOT instruction (core dumped)  ~/assert-clang/bin/clangd -j=1 --background-index=0 --clang-tidy=0
```

Version info
```console
 ~/assert-clang/bin/clangd -j=1 --background-index=0 --clang-tidy=0 --compile-commands-dir=environment/pigweed-venv/gn --check=pw_rpc/pwpb/client_integration_test.cc --log=verbose --version
Fuchsia clangd version 22.0.0git (https://llvm.googlesource.com/llvm-project 5d9d89092e0fd34f3ea2ca15c9f30d4c01bb29f3)
Features: linux+debug
Platform: x86_64-unknown-linux-gnu
```

Repro instructions:
```console
# Using Pigweed commit hash c6e8f6650589075dfb83ad67914416cc983c994c
git clone https://github.com/pigweed-project/pigweed.git && cd pigweed
source bootstrap.sh
clangd --background-index --clang-tidy --compile-commands-dir=environment/pigweed-venv/gn --check=pw_rpc/pwpb/client_integration_test.cc
```

I know its not exactly a nice self contained reproducer, like we get from clang, but pigweed isn't so huge (e.g. its not android or chrome). I'm trying to get something nice and small, like a preprocessed file that will still trigger the behavior.

Please let me know if there are more steps I should take. or if there's any tricks to getting better reductions for clangd than the normal advice we give for things like creduce.


---

### Comment 3 - ilovepi

hmm, I got the main `.cpp` file down to just a few lines, but the issue seems to be in something in the headers ... cvise doesn't seem to play well with reducing the headers, since it copies things into a temp directory, and the include directives bypass that ... any advice on how to get this down to a reasonable test case?

---

### Comment 4 - ilovepi

```cpp
// RUN: clangd -j=1 --background-index=0 --clang-tidy=0 --check=%s --log=verbose
namespace a {        template < b > c() ((::c::
```
I think this is about as small as we can get. 

slightly more syntactically valid and still triggers the assertion:
```cpp
// RUN: clang++ %s -fsyntax-only
// RUN:  clangd -j=1 --background-index=0 --clang-tidy=0 --check=%s --log=verbose
namespace a {
template <class b>
void c() {
  ((::c::));
}
}  // namespace a
```

```console
clang++: /usr/local/google/home/paulkirth/llvm-fork/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From *) [To = clang::UsingShadowDecl, From = const clang::NamedDecl]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/google/home/paulkirth/llvm-fork/build/bin/clang++ bar.cpp -fsyntax-only
1.      bar.cpp:4:10: current parser token ')'
2.      bar.cpp:1:1: parsing namespace 'a'
3.      bar.cpp:3:10: parsing function body 'a::c'
4.      bar.cpp:3:10: in compound statement ('{}')
#0 0x000055c86c7f16c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/paulkirth/llvm-fork/llvm/lib/Support/Unix/Signals.inc:834:13
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Fuchsia clang version 22.0.0git (git@github.com:ilovepi/llvm-project 83930beb8d0162a40489bce95fef0362b385db91)
```



Note that this reduced version also triggers the behavior in clang. The original source files (e.g. not reduced) build fine with the same revision of the compiler clangd is built at, so there is a chance that the reduced cases are triggering the same assert, but not for the same reasons.

---

### Comment 5 - ilovepi

Looks like the dyn_cast at https://github.com/llvm/llvm-project/blob/a3b5b4bd79cf17e0ff1279574b66783b4b3d619d/clang/lib/Sema/SemaCXXScopeSpec.cpp#L407 is having an issue, since in GDB it looks like the NamedDecl in the caller at https://github.com/llvm/llvm-project/blob/a3b5b4bd79cf17e0ff1279574b66783b4b3d619d/clang/lib/Sema/SemaCXXScopeSpec.cpp#L783 is `nullptr`.

---

### Comment 6 - ilovepi

I think https://github.com/llvm/llvm-project/pull/166995 should address the issue.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Paul Kirth (ilovepi)

<details>
We're seeing an assertion fire in clangd
```
clangd: llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From *) [To = clang::UsingShadowDecl, From = const clang::NamedDecl]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
```

I'm working on getting code and logs I can share, since we're seeing this in internal code. This may take me a bit but I wanted to report this as early as possible. The code base triggering this had a pretty massive upgrade from 03b0f55d9c6319a851a60bb084faca0e32a38f2b to 62adc83c918b1b55f70e4ee33c727b040da2a9ed, but I'm still able to trigger the assertions as of this morning.

I'm aware of https://clangd.llvm.org/troubleshooting, so I'll try to get logs, a simplified command, and a credeuced/sanitized source file, but some of these steps can take a bit to complete.


</details>


---

