# clang-19 BPF backend crashes compiling progs/pyperf180.c (Branch target out of insn range)

**Author:** Derelicte
**URL:** https://github.com/llvm/llvm-project/issues/178523
**Status:** Open
**Labels:** llvm:mc, crash

## Body

```
  <codeclang>CLANG-bpf  samples/bpf/sockex1_kern.o
  CLANG-bpf  samples/bpf/sockex2_kern.o
  CLANG-bpf  samples/bpf/sockex3_kern.o
  CLANG-bpf  samples/bpf/tracex1_kern.o
  CLANG-bpf  samples/bpf/tracex2_kern.o
  CLANG-bpf  samples/bpf/tracex3_kern.o
  CLANG-bpf  samples/bpf/tracex4_kern.o
  CLANG-bpf  samples/bpf/tracex6_kern.o
  CLANG-bpf  samples/bpf/tracex7_kern.o
  CLANG-bpf  samples/bpf/sock_flags_kern.o
  CLANG-bpf  samples/bpf/test_probe_write_user_kern.o
  CLANG-bpf  samples/bpf/trace_output_kern.o
  CLANG-bpf  samples/bpf/tcbpf1_kern.o
  CLANG-bpf  samples/bpf/tc_l2_redirect_kern.o
  CLANG-bpf  samples/bpf/lathist_kern.o
  CLANG-bpf  samples/bpf/offwaketime_kern.o
  CLANG-bpf  samples/bpf/spintest_kern.o
  CLANG-bpf  samples/bpf/map_perf_test_kern.o
  CLANG-bpf  samples/bpf/test_overhead_tp_kern.o
  CLANG-bpf  samples/bpf/test_overhead_raw_tp_kern.o
  CLANG-bpf  samples/bpf/test_overhead_kprobe_kern.o
  CLANG-bpf  samples/bpf/parse_varlen.o
  CLANG-bpf  samples/bpf/parse_simple.o
  CLANG-bpf  samples/bpf/parse_ldabs.o
  CLANG-bpf  samples/bpf/test_cgrp2_tc_kern.o
  CLANG-bpf  samples/bpf/xdp1_kern.o
  CLANG-bpf  samples/bpf/xdp2_kern.o
  CLANG-bpf  samples/bpf/test_current_task_under_cgroup_kern.o
  CLANG-bpf  samples/bpf/trace_event_kern.o
  CLANG-bpf  samples/bpf/sampleip_kern.o
  CLANG-bpf  samples/bpf/lwt_len_hist_kern.o
  CLANG-bpf  samples/bpf/xdp_tx_iptunnel_kern.o
  CLANG-bpf  samples/bpf/test_map_in_map_kern.o
  CLANG-bpf  samples/bpf/tcp_synrto_kern.o
  CLANG-bpf  samples/bpf/tcp_rwnd_kern.o
  CLANG-bpf  samples/bpf/tcp_bufs_kern.o
  CLANG-bpf  samples/bpf/tcp_cong_kern.o
  CLANG-bpf  samples/bpf/tcp_iw_kern.o
  CLANG-bpf  samples/bpf/tcp_clamp_kern.o
  CLANG-bpf  samples/bpf/tcp_basertt_kern.o
  CLANG-bpf  samples/bpf/tcp_tos_reflect_kern.o
  CLANG-bpf  samples/bpf/xdp_rxq_info_kern.o
  CLANG-bpf  samples/bpf/tcp_dumpstats_kern.o
  CLANG-bpf  samples/bpf/xdp2skb_meta_kern.o
  CLANG-bpf  samples/bpf/syscall_tp_kern.o
  CLANG-bpf  samples/bpf/cpustat_kern.o
  CLANG-bpf  samples/bpf/xdp_adjust_tail_kern.o
  CLANG-bpf  samples/bpf/xdp_fwd_kern.o
  CLANG-bpf  samples/bpf/task_fd_query_kern.o
  CLANG-bpf  samples/bpf/xdp_sample_pkts_kern.o
  CLANG-bpf  samples/bpf/ibumad_kern.o
  CLANG-bpf  samples/bpf/hbm_out_kern.o
  CLANG-bpf  samples/bpf/hbm_edt_kern.o
  CLANG-bpf  samples/bpf/tracex5_kern.o
  CLANG-BPF  samples/bpf/xdp_sample.bpf.o
  CLANG-BPF  samples/bpf/xdp_monitor.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_cpu.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_map.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_map_multi.bpf.o
  CLANG-BPF  samples/bpf/xdp_router_ipv4.bpf.o
  BPF GEN-OBJ  samples/bpf/xdp_monitor
  BPF GEN-OBJ  samples/bpf/xdp_redirect_cpu
  BPF GEN-OBJ  samples/bpf/xdp_redirect_map_multi
  BPF GEN-OBJ  samples/bpf/xdp_redirect_map
  BPF GEN-OBJ  samples/bpf/xdp_redirect
  BPF GEN-OBJ  samples/bpf/xdp_router_ipv4
  BPF GEN-SKEL samples/bpf/xdp_monitor
  BPF GEN-SKEL samples/bpf/xdp_redirect_cpu
  BPF GEN-SKEL samples/bpf/xdp_redirect_map_multi
  BPF GEN-SKEL samples/bpf/xdp_redirect_map
  BPF GEN-SKEL samples/bpf/xdp_redirect
  BPF GEN-SKEL samples/bpf/xdp_router_ipv4
++ pwd
+ export BPFTOOL=/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/bpf/bpftool/bpftool
+ BPFTOOL=/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/bpf/bpftool/bpftool
+ pushd tools/testing/selftests
~/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests ~/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64
+ /usr/bin/make -s 'HOSTCFLAGS=-O2  -fexceptions -g -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1 -fstack-protector-strong -specs=/usr/lib/rpm/redhat/redhat-annobin-cc1  -m64 -march=x86-64-v2 -mtune=generic -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection' 'HOSTLDFLAGS=-Wl,-z,relro -Wl,--as-needed  -Wl,-z,now -specs=/usr/lib/rpm/redhat/redhat-hardened-ld -specs=/usr/lib/rpm/redhat/redhat-annobin-cc1 ' -j30 ARCH=x86_64 V=1 'TARGETS=bpf vm livepatch net net/forwarding net/mptcp netfilter tc-testing memfd' SKIP_TARGETS= FORCE_TARGETS=1 INSTALL_PATH=/builddir/build/BUILDROOT/kernel-rt-5.14.0-284.30.1.rt14.315.el9.x86_64/usr/libexec/kselftests VMLINUX_H= install
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:114: error: "SCHED_NORMAL" redefined [-Werror]
  114 | #define SCHED_NORMAL            0
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:32: note: this is the location of the previous definition
   32 | # define SCHED_NORMAL           SCHED_OTHER
      |
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:136: error: "SCHED_FLAG_KEEP_ALL" redefined [-Werror]
  136 | #define SCHED_FLAG_KEEP_ALL     (SCHED_FLAG_KEEP_POLICY | \
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:51: note: this is the location of the previous definition
   51 | #define SCHED_FLAG_KEEP_ALL             0x18
      |
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:139: error: "SCHED_FLAG_UTIL_CLAMP" redefined [-Werror]
  139 | #define SCHED_FLAG_UTIL_CLAMP   (SCHED_FLAG_UTIL_CLAMP_MIN | \
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:52: note: this is the location of the previous definition
   52 | #define SCHED_FLAG_UTIL_CLAMP           0x60
      |
cc1: all warnings being treated as errors
make[1]: *** [Makefile:527: /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/cgroup_helpers.o] Error 1
make[1]: *** Waiting for unfinished jobs....
fatal error: error in backend: Branch target out of insn range
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -g -Werror -D__TARGET_ARCH_x86 -mlittle-endian -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/tools/include -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/usr/include -idirafter /usr/lib64/llvm19/bin/../../../lib/clang/19/include -idirafter /usr/local/include -idirafter /usr/include -Wno-compare-distinct-pointer-types -DENABLE_ATOMICS_TESTS -O2 -target bpf -c progs/pyperf180.c -mcpu=v3 -o /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/pyperf180.o
1.      <eof> parser at end of file
2.      Code generation
 #0 0x00007fe33022a6ba llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x95b6ba)
 #1 0x00007fe330227af4 llvm::sys::RunSignalHandlers() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x958af4)
 #2 0x00007fe330147692 (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x878692)
 #3 0x00007fe33014763f llvm::CrashRecoveryContext::HandleExit(int) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x87863f)
 #4 0x00007fe33022388d (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x95488d)
 #5 0x00000000004133be (/usr/lib64/llvm19/bin/clang-19+0x4133be)
 #6 0x00007fe33015ba50 llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x88ca50)
 #7 0x00007fe33015b8eb llvm::report_fatal_error(char const*, bool) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x88c8eb)
 #8 0x00007fe3338ad621 (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x3fde621)
 #9 0x00007fe331e6a028 llvm::MCAssembler::layout() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x259b028)
#10 0x00007fe331ea5b40 llvm::MCObjectStreamer::finishImpl() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x25d6b40)
#11 0x00007fe331e91b1e llvm::MCELFStreamer::finishImpl() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x25c2b1e)
#12 0x00007fe330d28f60 llvm::AsmPrinter::doFinalization(llvm::Module&) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x1459f60)
#13 0x00007fe3303d3371 llvm::FPPassManager::doFinalization(llvm::Module&) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0xb04371)
#14 0x00007fe3303cb592 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0xafc592)
#15 0x00007fe3388fdc9f clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2157c9f)
#16 0x00007fe338d5f86e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x25b986e)
#17 0x00007fe337287489 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0xae1489)
#18 0x00007fe339ad0c5b clang::FrontendAction::Execute() (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x332ac5b)
#19 0x00007fe339a239d4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x327d9d4)
#20 0x00007fe339b5fed0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x33b9ed0)
#21 0x00000000004128de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib64/llvm19/bin/clang-19+0x4128de)
#22 0x000000000040efb6 (/usr/lib64/llvm19/bin/clang-19+0x40efb6)
#23 0x00007fe3395eb13d (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e4513d)
#24 0x00007fe330147614 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x878614)
#25 0x00007fe3395eab16 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e44b16)
#26 0x00007fe3395a6fae clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e00fae)
#27 0x00007fe3395a72e7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e012e7)
#28 0x00007fe3395c8b0e clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e22b0e)
#29 0x000000000040e28e clang_main(int, char**, llvm::ToolContext const&) (/usr/lib64/llvm19/bin/clang-19+0x40e28e)
#30 0x000000000041e528 main (/usr/lib64/llvm19/bin/clang-19+0x41e528)
#31 0x00007fe32f4a8610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#32 0x00007fe32f4a86c0 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#33 0x000000000040c575 _start (/usr/lib64/llvm19/bin/clang-19+0x40c575)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 19.1.0 (Red Hat, Inc. 19.1.0-1.el9)
Target: bpf
Thread model: posix
InstalledDir: /usr/lib64/llvm19/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/pyperf180-6b2736.c
clang: note: diagnostic msg: /tmp/pyperf180-6b2736.sh
clang: note: diagnostic msg:

********************
make[1]: *** [Makefile:526: /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/pyperf180.o] Error 1
make: *** [Makefile:163: all] Error 2
error: Bad exit status from /var/tmp/rpm-tmp.ko6JYW (%build)


RPM build errors:
    Bad exit status from /var/tmp/rpm-tmp.ko6JYW (%build)<code>
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: AKA BDawg31 on XDA (Derelicte)

<details>
  &lt;codeclang&gt;CLANG-bpf  samples/bpf/sockex1_kern.o
  CLANG-bpf  samples/bpf/sockex2_kern.o
  CLANG-bpf  samples/bpf/sockex3_kern.o
  CLANG-bpf  samples/bpf/tracex1_kern.o
  CLANG-bpf  samples/bpf/tracex2_kern.o
  CLANG-bpf  samples/bpf/tracex3_kern.o
  CLANG-bpf  samples/bpf/tracex4_kern.o
  CLANG-bpf  samples/bpf/tracex6_kern.o
  CLANG-bpf  samples/bpf/tracex7_kern.o
  CLANG-bpf  samples/bpf/sock_flags_kern.o
  CLANG-bpf  samples/bpf/test_probe_write_user_kern.o
  CLANG-bpf  samples/bpf/trace_output_kern.o
  CLANG-bpf  samples/bpf/tcbpf1_kern.o
  CLANG-bpf  samples/bpf/tc_l2_redirect_kern.o
  CLANG-bpf  samples/bpf/lathist_kern.o
  CLANG-bpf  samples/bpf/offwaketime_kern.o
  CLANG-bpf  samples/bpf/spintest_kern.o
  CLANG-bpf  samples/bpf/map_perf_test_kern.o
  CLANG-bpf  samples/bpf/test_overhead_tp_kern.o
  CLANG-bpf  samples/bpf/test_overhead_raw_tp_kern.o
  CLANG-bpf  samples/bpf/test_overhead_kprobe_kern.o
  CLANG-bpf  samples/bpf/parse_varlen.o
  CLANG-bpf  samples/bpf/parse_simple.o
  CLANG-bpf  samples/bpf/parse_ldabs.o
  CLANG-bpf  samples/bpf/test_cgrp2_tc_kern.o
  CLANG-bpf  samples/bpf/xdp1_kern.o
  CLANG-bpf  samples/bpf/xdp2_kern.o
  CLANG-bpf  samples/bpf/test_current_task_under_cgroup_kern.o
  CLANG-bpf  samples/bpf/trace_event_kern.o
  CLANG-bpf  samples/bpf/sampleip_kern.o
  CLANG-bpf  samples/bpf/lwt_len_hist_kern.o
  CLANG-bpf  samples/bpf/xdp_tx_iptunnel_kern.o
  CLANG-bpf  samples/bpf/test_map_in_map_kern.o
  CLANG-bpf  samples/bpf/tcp_synrto_kern.o
  CLANG-bpf  samples/bpf/tcp_rwnd_kern.o
  CLANG-bpf  samples/bpf/tcp_bufs_kern.o
  CLANG-bpf  samples/bpf/tcp_cong_kern.o
  CLANG-bpf  samples/bpf/tcp_iw_kern.o
  CLANG-bpf  samples/bpf/tcp_clamp_kern.o
  CLANG-bpf  samples/bpf/tcp_basertt_kern.o
  CLANG-bpf  samples/bpf/tcp_tos_reflect_kern.o
  CLANG-bpf  samples/bpf/xdp_rxq_info_kern.o
  CLANG-bpf  samples/bpf/tcp_dumpstats_kern.o
  CLANG-bpf  samples/bpf/xdp2skb_meta_kern.o
  CLANG-bpf  samples/bpf/syscall_tp_kern.o
  CLANG-bpf  samples/bpf/cpustat_kern.o
  CLANG-bpf  samples/bpf/xdp_adjust_tail_kern.o
  CLANG-bpf  samples/bpf/xdp_fwd_kern.o
  CLANG-bpf  samples/bpf/task_fd_query_kern.o
  CLANG-bpf  samples/bpf/xdp_sample_pkts_kern.o
  CLANG-bpf  samples/bpf/ibumad_kern.o
  CLANG-bpf  samples/bpf/hbm_out_kern.o
  CLANG-bpf  samples/bpf/hbm_edt_kern.o
  CLANG-bpf  samples/bpf/tracex5_kern.o
  CLANG-BPF  samples/bpf/xdp_sample.bpf.o
  CLANG-BPF  samples/bpf/xdp_monitor.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_cpu.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_map.bpf.o
  CLANG-BPF  samples/bpf/xdp_redirect_map_multi.bpf.o
  CLANG-BPF  samples/bpf/xdp_router_ipv4.bpf.o
  BPF GEN-OBJ  samples/bpf/xdp_monitor
  BPF GEN-OBJ  samples/bpf/xdp_redirect_cpu
  BPF GEN-OBJ  samples/bpf/xdp_redirect_map_multi
  BPF GEN-OBJ  samples/bpf/xdp_redirect_map
  BPF GEN-OBJ  samples/bpf/xdp_redirect
  BPF GEN-OBJ  samples/bpf/xdp_router_ipv4
  BPF GEN-SKEL samples/bpf/xdp_monitor
  BPF GEN-SKEL samples/bpf/xdp_redirect_cpu
  BPF GEN-SKEL samples/bpf/xdp_redirect_map_multi
  BPF GEN-SKEL samples/bpf/xdp_redirect_map
  BPF GEN-SKEL samples/bpf/xdp_redirect
  BPF GEN-SKEL samples/bpf/xdp_router_ipv4
++ pwd
+ export BPFTOOL=/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/bpf/bpftool/bpftool
+ BPFTOOL=/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/bpf/bpftool/bpftool
+ pushd tools/testing/selftests
~/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests ~/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64
+ /usr/bin/make -s 'HOSTCFLAGS=-O2  -fexceptions -g -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1 -fstack-protector-strong -specs=/usr/lib/rpm/redhat/redhat-annobin-cc1  -m64 -march=x86-64-v2 -mtune=generic -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection' 'HOSTLDFLAGS=-Wl,-z,relro -Wl,--as-needed  -Wl,-z,now -specs=/usr/lib/rpm/redhat/redhat-hardened-ld -specs=/usr/lib/rpm/redhat/redhat-annobin-cc1 ' -j30 ARCH=x86_64 V=1 'TARGETS=bpf vm livepatch net net/forwarding net/mptcp netfilter tc-testing memfd' SKIP_TARGETS= FORCE_TARGETS=1 INSTALL_PATH=/builddir/build/BUILDROOT/kernel-rt-5.14.0-284.30.1.rt14.315.el9.x86_64/usr/libexec/kselftests VMLINUX_H= install
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:114: error: "SCHED_NORMAL" redefined [-Werror]
  114 | #define SCHED_NORMAL            0
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:32: note: this is the location of the previous definition
   32 | # define SCHED_NORMAL           SCHED_OTHER
      |
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:136: error: "SCHED_FLAG_KEEP_ALL" redefined [-Werror]
  136 | #define SCHED_FLAG_KEEP_ALL     (SCHED_FLAG_KEEP_POLICY | \
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:51: note: this is the location of the previous definition
   51 | #define SCHED_FLAG_KEEP_ALL             0x18
      |
In file included from cgroup_helpers.c:10:
/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi/linux/sched.h:139: error: "SCHED_FLAG_UTIL_CLAMP" redefined [-Werror]
  139 | #define SCHED_FLAG_UTIL_CLAMP   (SCHED_FLAG_UTIL_CLAMP_MIN | \
      |
In file included from /usr/include/sched.h:43,
                 from cgroup_helpers.c:3:
/usr/include/bits/sched.h:52: note: this is the location of the previous definition
   52 | #define SCHED_FLAG_UTIL_CLAMP           0x60
      |
cc1: all warnings being treated as errors
make[1]: *** [Makefile:527: /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/cgroup_helpers.o] Error 1
make[1]: *** Waiting for unfinished jobs....
fatal error: error in backend: Branch target out of insn range
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -g -Werror -D__TARGET_ARCH_x86 -mlittle-endian -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/tools/include -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/include/uapi -I/builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/usr/include -idirafter /usr/lib64/llvm19/bin/../../../lib/clang/19/include -idirafter /usr/local/include -idirafter /usr/include -Wno-compare-distinct-pointer-types -DENABLE_ATOMICS_TESTS -O2 -target bpf -c progs/pyperf180.c -mcpu=v3 -o /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/pyperf180.o
1.      &lt;eof&gt; parser at end of file
2.      Code generation
 #<!-- -->0 0x00007fe33022a6ba llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x95b6ba)
 #<!-- -->1 0x00007fe330227af4 llvm::sys::RunSignalHandlers() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x958af4)
 #<!-- -->2 0x00007fe330147692 (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x878692)
 #<!-- -->3 0x00007fe33014763f llvm::CrashRecoveryContext::HandleExit(int) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x87863f)
 #<!-- -->4 0x00007fe33022388d (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x95488d)
 #<!-- -->5 0x00000000004133be (/usr/lib64/llvm19/bin/clang-19+0x4133be)
 #<!-- -->6 0x00007fe33015ba50 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x88ca50)
 #<!-- -->7 0x00007fe33015b8eb llvm::report_fatal_error(char const*, bool) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x88c8eb)
 #<!-- -->8 0x00007fe3338ad621 (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x3fde621)
 #<!-- -->9 0x00007fe331e6a028 llvm::MCAssembler::layout() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x259b028)
#<!-- -->10 0x00007fe331ea5b40 llvm::MCObjectStreamer::finishImpl() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x25d6b40)
#<!-- -->11 0x00007fe331e91b1e llvm::MCELFStreamer::finishImpl() (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x25c2b1e)
#<!-- -->12 0x00007fe330d28f60 llvm::AsmPrinter::doFinalization(llvm::Module&amp;) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x1459f60)
#<!-- -->13 0x00007fe3303d3371 llvm::FPPassManager::doFinalization(llvm::Module&amp;) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0xb04371)
#<!-- -->14 0x00007fe3303cb592 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0xafc592)
#<!-- -->15 0x00007fe3388fdc9f clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2157c9f)
#<!-- -->16 0x00007fe338d5f86e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x25b986e)
#<!-- -->17 0x00007fe337287489 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0xae1489)
#<!-- -->18 0x00007fe339ad0c5b clang::FrontendAction::Execute() (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x332ac5b)
#<!-- -->19 0x00007fe339a239d4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x327d9d4)
#<!-- -->20 0x00007fe339b5fed0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x33b9ed0)
#<!-- -->21 0x00000000004128de cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib64/llvm19/bin/clang-19+0x4128de)
#<!-- -->22 0x000000000040efb6 (/usr/lib64/llvm19/bin/clang-19+0x40efb6)
#<!-- -->23 0x00007fe3395eb13d (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e4513d)
#<!-- -->24 0x00007fe330147614 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib64/llvm19/lib/libLLVM.so.19.1+0x878614)
#<!-- -->25 0x00007fe3395eab16 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e44b16)
#<!-- -->26 0x00007fe3395a6fae clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e00fae)
#<!-- -->27 0x00007fe3395a72e7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e012e7)
#<!-- -->28 0x00007fe3395c8b0e clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib64/llvm19/lib/libclang-cpp.so.19.1+0x2e22b0e)
#<!-- -->29 0x000000000040e28e clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib64/llvm19/bin/clang-19+0x40e28e)
#<!-- -->30 0x000000000041e528 main (/usr/lib64/llvm19/bin/clang-19+0x41e528)
#<!-- -->31 0x00007fe32f4a8610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#<!-- -->32 0x00007fe32f4a86c0 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#<!-- -->33 0x000000000040c575 _start (/usr/lib64/llvm19/bin/clang-19+0x40c575)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 19.1.0 (Red Hat, Inc. 19.1.0-1.el9)
Target: bpf
Thread model: posix
InstalledDir: /usr/lib64/llvm19/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/pyperf180-6b2736.c
clang: note: diagnostic msg: /tmp/pyperf180-6b2736.sh
clang: note: diagnostic msg:

********************
make[1]: *** [Makefile:526: /builddir/build/BUILD/kernel-5.14.0-284.30.1.rt14.315.el9_2/linux-5.14.0-284.30.1.rt14.315.el9.x86_64/tools/testing/selftests/bpf/pyperf180.o] Error 1
make: *** [Makefile:163: all] Error 2
error: Bad exit status from /var/tmp/rpm-tmp.ko6JYW (%build)


RPM build errors:
    Bad exit status from /var/tmp/rpm-tmp.ko6JYW (%build)&lt;code&gt;
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 3 - yonghong-song

The error message from the compiler:
```
fatal error: error in backend: Branch target out of insn range
```

please use cpu v4.

---

