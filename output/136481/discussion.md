# Windows clang-cl.exe 19.1.1 crash

**Author:** MiguelBarro
**URL:** https://github.com/llvm/llvm-project/issues/136481

## Body

## Environment
- [Github workflow](https://raw.githubusercontent.com/MiguelBarro/CppCoroutines/refs/heads/main/.github/workflows/ci.yml)
- Github hosted windows-latest runner (matches windows-2025 on this report date)
- using `CMake` with visual studio generator using `ClangCl` toolset.

## Reproduction steps
The crash was generated in [this repo](https://github.com/MiguelBarro/CppCoroutines.git) *github actions*.
The repo workflow uses windows, ubuntu and Macos github hosted runners. It runs `CMake` to build C++ sources.
The issue is in the `windows-clang` job. Note that `clang` works perfectly on Ubuntu (see `ubuntu-clang` and `macos-clang` jobs).
The issue appears when the C++ sources were modified to use `unique_ptr<char[]>`.

## Crash report
It is available [here](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#logs).
basically:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Tools\\Llvm\\x64\\bin\\clang-cl.exe" @C:\\Users\\runneradmin\\AppData\\Local\\Temp\\MSBuildTemp\\tmp53ddccde22e94fa5985920d130732494.rsp
  1.	<eof> parser at end of file
  2.	Optimizer
  3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split))" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  5.	Running pass "jump-threading" on function "?session@@YA?AV?$future@U?$pair@_K_K@std@@@std@@AEAVio_service@asio@@AEAV?$basic_resolver_iterator@Vtcp@ip@asio@@@ip@4@_KAEAU?$atomic@_N@2@@Z.resume"
  Exception Code: 0xC0000005
   #0 0x00007ff790d194ce (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xec94ce)
   #1 0x00007ff7911330ea (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x12e30ea)
   #2 0x00007ff79164d3c4 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17fd3c4)
   #3 0x00007ff791645076 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17f5076)
   #4 0x00007ff79173db96 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18edb96)
   #5 0x00007ff7917390af (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e90af)
   #6 0x00007ff7917355f0 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e55f0)
   #7 0x00007ff791177c24 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327c24)
   #8 0x00007ff791177a71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327a71)
   #9 0x00007ff7914b9643 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1669643)
  #10 0x00007ff7914b92c1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x16692c1)
  #11 0x00007ff791280680 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1430680)
  #12 0x00007ff79128037b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x143037b)
  #13 0x00007ff7916f5fab (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5fab)
  #14 0x00007ff7916f5c8b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5c8b)
  #15 0x00007ff791835b59 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e5b59)
  #16 0x00007ff7918358eb (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e58eb)
  #17 0x00007ff7917eb4d6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x199b4d6)
  #18 0x00007ff790235b71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x3e5b71)
  #19 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #20 0x00007ff790642c50 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2c50)
  #21 0x00007ff790642a81 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2a81)
  #22 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #23 0x00007ff790125918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2d5918)
  #24 0x00007ff79011c432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2cc432)
  #25 0x00007ff7914bc015 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x166c015)
  #26 0x00007ff7919ac588 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1b5c588)
  #27 0x00007ff79040d8a1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd8a1)
  #28 0x00007ff79040d432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd432)
  #29 0x00007ff79040a1a6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5ba1a6)
  #30 0x00007ff790407165 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b7165)
  #31 0x00007ff790405afe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b5afe)
  #32 0x00007ff790b1f23d (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xccf23d)
  #33 0x00007ff79058db25 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73db25)
  #34 0x00007ff79058d918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73d918)
  #35 0x00007ff79028b8[78](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:79) (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43b878)
  #36 0x00007ff[79](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:80)028abfe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43abfe)
  #37 0x00007ff79028a777 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43a777)
  #38 0x00007ff7902[81](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:82)f06 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x431f06)
  #39 0x00007ff79027f683 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x42f683)
  #40 0x00007ff791a4c[82](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:83)8 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1bfc828)
  #41 0x00007ffe114f4cb0 (C:\Windows\System32\KERNEL32.DLL+0x14cb0)
  #42 0x00007ffe11d3edcb (C:\Windows\SYSTEM32\ntdll.dll+0x7edcb)
clang-cl : error : clang frontend command failed due to signal (use -v to see invocation) [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  clang version 19.1.1
  Target: amd64-pc-windows-msvc
  Thread model: posix
  InstalledDir: C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
  PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
  Preprocessed source(s) and associated run script(s) are located at:
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.cpp [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.sh [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
```

## Comments

### Comment 1 - Endilll

Your report says
```
  PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
  Preprocessed source(s) and associated run script(s) are located at:
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.cpp [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.sh [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
```
but I don't see those files attached anywhere

---

### Comment 2 - MiguelBarro

After some iteration with github actions I managed to upload the missing files [here](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14686553231/artifacts/3015714050).

---

### Comment 3 - Endilll

Attaching the reproducer here to make sure it doesn't go away: [WindowsClangErrorReports.zip](https://github.com/user-attachments/files/19926000/WindowsClangErrorReports.zip)

---

### Comment 4 - Endilll

I'm able to reproduce this locally using assertions build of Clang 19:
```
Instruction does not dominate all uses!
  %221 = phi ptr [ %217, %214 ], [ %53, %50 ], !dbg !34927
  %268 = phi ptr [ %221, %264 ]
Instruction does not dominate all uses!
  %220 = phi ptr [ %218, %214 ], [ %54, %50 ], !dbg !34923
  %269 = phi ptr [ %220, %264 ]
Instruction does not dominate all uses!
  %221 = phi ptr [ %217, %214 ], [ %53, %50 ], !dbg !34927
  %271 = phi ptr [ %221, %264 ]
Instruction does not dominate all uses!
  %220 = phi ptr [ %218, %214 ], [ %54, %50 ], !dbg !34923
  %272 = phi ptr [ %220, %264 ]
Instruction does not dominate all uses!
  %221 = phi ptr [ %217, %214 ], [ %53, %50 ], !dbg !34927
  %274 = phi ptr [ %221, %264 ]
Instruction does not dominate all uses!
  %220 = phi ptr [ %218, %214 ], [ %54, %50 ], !dbg !34923
  %275 = phi ptr [ %220, %264 ]
Instruction does not dominate all uses!
  %221 = phi ptr [ %217, %214 ], [ %53, %50 ], !dbg !34927
  %277 = phi ptr [ %221, %264 ]
Instruction does not dominate all uses!
  %220 = phi ptr [ %218, %214 ], [ %54, %50 ], !dbg !34923
  %278 = phi ptr [ %220, %264 ]
Instruction does not dominate all uses!
  %196 = phi ptr [ %174, %165 ], [ %126, %117 ], [ %162, %156 ], [ %191, %184 ], [ %137, %130 ], [ %181, %178 ], [ %145, %140 ], [ %153, %148 ], !dbg !34927
  %280 = phi ptr [ %196, %264 ]
Instruction does not dominate all uses!
  %195 = phi ptr [ %176, %165 ], [ %128, %117 ], [ %164, %156 ], [ %193, %184 ], [ %139, %130 ], [ %182, %178 ], [ %147, %140 ], [ %155, %148 ], !dbg !34923
  %281 = phi ptr [ %195, %264 ]
Instruction does not dominate all uses!
  %196 = phi ptr [ %174, %165 ], [ %126, %117 ], [ %162, %156 ], [ %191, %184 ], [ %137, %130 ], [ %181, %178 ], [ %145, %140 ], [ %153, %148 ], !dbg !34927
  %283 = phi ptr [ %196, %264 ]
Instruction does not dominate all uses!
  %195 = phi ptr [ %176, %165 ], [ %128, %117 ], [ %164, %156 ], [ %193, %184 ], [ %139, %130 ], [ %182, %178 ], [ %147, %140 ], [ %155, %148 ], !dbg !34923
  %284 = phi ptr [ %195, %264 ]
Instruction does not dominate all uses!
  %196 = phi ptr [ %174, %165 ], [ %126, %117 ], [ %162, %156 ], [ %191, %184 ], [ %137, %130 ], [ %181, %178 ], [ %145, %140 ], [ %153, %148 ], !dbg !34927
  %286 = phi ptr [ %196, %264 ]
Instruction does not dominate all uses!
  %195 = phi ptr [ %176, %165 ], [ %128, %117 ], [ %164, %156 ], [ %193, %184 ], [ %139, %130 ], [ %182, %178 ], [ %147, %140 ], [ %155, %148 ], !dbg !34923
  %287 = phi ptr [ %195, %264 ]
Instruction does not dominate all uses!
  %196 = phi ptr [ %174, %165 ], [ %126, %117 ], [ %162, %156 ], [ %191, %184 ], [ %137, %130 ], [ %181, %178 ], [ %145, %140 ], [ %153, %148 ], !dbg !34927
  %289 = phi ptr [ %196, %264 ]
Instruction does not dominate all uses!
  %195 = phi ptr [ %176, %165 ], [ %128, %117 ], [ %164, %156 ], [ %193, %184 ], [ %139, %130 ], [ %182, %178 ], [ %147, %140 ], [ %155, %148 ], !dbg !34923
  %290 = phi ptr [ %195, %264 ]
Instruction does not dominate all uses!
  %196 = phi ptr [ %174, %165 ], [ %126, %117 ], [ %162, %156 ], [ %191, %184 ], [ %137, %130 ], [ %181, %178 ], [ %145, %140 ], [ %153, %148 ], !dbg !34927
  %292 = phi ptr [ %196, %264 ]
Instruction does not dominate all uses!
  %195 = phi ptr [ %176, %165 ], [ %128, %117 ], [ %164, %156 ], [ %193, %184 ], [ %139, %130 ], [ %182, %178 ], [ %147, %140 ], [ %155, %148 ], !dbg !34923
  %293 = phi ptr [ %195, %264 ]
fatal error: error in backend: Broken function
```

---

### Comment 5 - Endilll

Confirmed on Clang 21.0: https://godbolt.org/z/1fqzeW1fj
Reduced by me and C-Reduce:
```cpp
namespace std {

template <class>
struct coroutine_traits;

template <class>
struct coroutine_handle {
  static coroutine_handle from_address(void *);
  operator coroutine_handle<void>();
};

} // namespace std

template <class T>
T move(T);

struct unique_ptr {
  int *ptr;
  
  ~unique_ptr() {
    delete ptr;
  }
};

struct suspend_never {
  int await_ready() noexcept;
  void await_suspend(std::coroutine_handle<void>) noexcept;
  void await_resume() noexcept;
};

struct future {};

template <>
struct std::coroutine_traits<future> {
  struct promise_type {
    future get_return_object();
    suspend_never initial_suspend();
    suspend_never final_suspend() noexcept;
    void unhandled_exception();
  };
};

void close();

future session() {
  unique_ptr read_data{nullptr};
  unique_ptr write_data{nullptr};
  try {
    co_await suspend_never{};
    read_data = move(write_data);
  } catch (int) {
  }
  close();
}
```
Clang 21.0 crash:
```
Instruction does not dominate all uses!
  %read_data.sroa.0.0 = phi ptr [ %.try.cont82, %"try.cont.from.??1unique_ptr@@QEAA@XZ.exit" ], [ %.try.cont, %try.cont.from.catch ], !dbg !157
  %read_data.sroa.0.0.ehcleanup18 = phi ptr [ %read_data.sroa.0.0, %ehcleanup18.corodispatch ]
fatal error: error in backend: Broken function

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -target amd64-pc-windows-msvc19.43.34808 -O1 <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "<source>"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "<source>"
5.	While splitting coroutine @"?session@@YA?AUfuture@@XZ"
 #0 0x0000000003f185e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f185e8)
 #1 0x0000000003f16274 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f16274)
 #2 0x0000000003e5b306 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5b306)
 #3 0x0000000003f0d99e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f0d99e)
 #4 0x0000000000d9fda0 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003e65fe3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e65fe3)
 #6 0x0000000003e66148 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e66148)
 #7 0x000000000573b5cf llvm::coro::SwitchABI::splitCoroutine(llvm::Function&, llvm::coro::Shape&, llvm::SmallVectorImpl<llvm::Function*>&, llvm::TargetTransformInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x573b5cf)
 #8 0x000000000573c163 llvm::CoroSplitPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x573c163)
 #9 0x00000000055f833e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::CoroSplitPass, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x55f833e)
#10 0x0000000002d5fed2 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2d5fed2)
#11 0x00000000055f722e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x55f722e)
#12 0x0000000002d655a6 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2d655a6)
#13 0x00000000055f724e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::DevirtSCCRepeatedPass, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x55f724e)
#14 0x0000000002d63476 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2d63476)
#15 0x00000000055f720e llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x55f720e)
#16 0x00000000038b9df0 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38b9df0)
#17 0x00000000057f42d3 llvm::ModuleInlinerWrapperPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57f42d3)
#18 0x000000000549800e llvm::detail::PassModel<llvm::Module, llvm::ModuleInlinerWrapperPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x549800e)
#19 0x00000000038b9df0 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38b9df0)
#20 0x00000000041cbe32 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#21 0x00000000041cf57b clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41cf57b)
#22 0x00000000048b7af0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b7af0)
#23 0x0000000006579a1c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6579a1c)
#24 0x00000000048b7ed8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b7ed8)
#25 0x0000000004bab3c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bab3c5)
#26 0x0000000004b2a0ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2a0ee)
#27 0x0000000004c9a9fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a9fe)
#28 0x0000000000da245f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda245f)
#29 0x0000000000d9946a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x000000000491e759 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003e5b244 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5b244)
#32 0x000000000491ed6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000048e11cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e11cd)
#34 0x00000000048e225e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e225e)
#35 0x00000000048ea0b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ea0b5)
#36 0x0000000000d9f1a8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f1a8)
#37 0x0000000000c24bd4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc24bd4)
#38 0x00007a3b16a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007a3b16a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000d98f15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd98f15)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
```

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-coroutines

Author: MiguelBarro (MiguelBarro)

<details>
## Environment
- [Github workflow](https://raw.githubusercontent.com/MiguelBarro/CppCoroutines/refs/heads/main/.github/workflows/ci.yml)
- Github hosted windows-latest runner (matches windows-2025 on this report date)
- using `CMake` with visual studio generator using `ClangCl` toolset.

## Reproduction steps
The crash was generated in [this repo](https://github.com/MiguelBarro/CppCoroutines.git) *github actions*.
The repo workflow uses windows, ubuntu and Macos github hosted runners. It runs `CMake` to build C++ sources.
The issue is in the `windows-clang` job. Note that `clang` works perfectly on Ubuntu (see `ubuntu-clang` and `macos-clang` jobs).
The issue appears when the C++ sources were modified to use `unique_ptr&lt;char[]&gt;`.

## Crash report
It is available [here](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#logs).
basically:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Tools\\Llvm\\x64\\bin\\clang-cl.exe" @<!-- -->C:\\Users\\runneradmin\\AppData\\Local\\Temp\\MSBuildTemp\\tmp53ddccde22e94fa5985920d130732494.rsp
  1.	&lt;eof&gt; parser at end of file
  2.	Optimizer
  3.	Running pass "require&lt;globals-aa&gt;,function(invalidate&lt;aa&gt;),require&lt;profile-summary&gt;,cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split)),function(invalidate&lt;should-not-run-function-passes&gt;),cgscc(devirt&lt;4&gt;())" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  4.	Running pass "cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split))" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  5.	Running pass "jump-threading" on function "?session@@<!-- -->YA?AV?$future@<!-- -->U?$pair@<!-- -->_K_K@<!-- -->std@@@<!-- -->std@@<!-- -->AEAVio_service@<!-- -->asio@@<!-- -->AEAV?$basic_resolver_iterator@<!-- -->Vtcp@<!-- -->ip@<!-- -->asio@@@<!-- -->ip@<!-- -->4@<!-- -->_KAEAU?$atomic@<!-- -->_N@<!-- -->2@@<!-- -->Z.resume"
  Exception Code: 0xC0000005
   #<!-- -->0 0x00007ff790d194ce (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xec94ce)
   #<!-- -->1 0x00007ff7911330ea (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x12e30ea)
   #<!-- -->2 0x00007ff79164d3c4 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17fd3c4)
   #<!-- -->3 0x00007ff791645076 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17f5076)
   #<!-- -->4 0x00007ff79173db96 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18edb96)
   #<!-- -->5 0x00007ff7917390af (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e90af)
   #<!-- -->6 0x00007ff7917355f0 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e55f0)
   #<!-- -->7 0x00007ff791177c24 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327c24)
   #<!-- -->8 0x00007ff791177a71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327a71)
   #<!-- -->9 0x00007ff7914b9643 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1669643)
  #<!-- -->10 0x00007ff7914b92c1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x16692c1)
  #<!-- -->11 0x00007ff791280680 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1430680)
  #<!-- -->12 0x00007ff79128037b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x143037b)
  #<!-- -->13 0x00007ff7916f5fab (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5fab)
  #<!-- -->14 0x00007ff7916f5c8b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5c8b)
  #<!-- -->15 0x00007ff791835b59 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e5b59)
  #<!-- -->16 0x00007ff7918358eb (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e58eb)
  #<!-- -->17 0x00007ff7917eb4d6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x199b4d6)
  #<!-- -->18 0x00007ff790235b71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x3e5b71)
  #<!-- -->19 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #<!-- -->20 0x00007ff790642c50 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2c50)
  #<!-- -->21 0x00007ff790642a81 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2a81)
  #<!-- -->22 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #<!-- -->23 0x00007ff790125918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2d5918)
  #<!-- -->24 0x00007ff79011c432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2cc432)
  #<!-- -->25 0x00007ff7914bc015 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x166c015)
  #<!-- -->26 0x00007ff7919ac588 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1b5c588)
  #<!-- -->27 0x00007ff79040d8a1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd8a1)
  #<!-- -->28 0x00007ff79040d432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd432)
  #<!-- -->29 0x00007ff79040a1a6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5ba1a6)
  #<!-- -->30 0x00007ff790407165 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b7165)
  #<!-- -->31 0x00007ff790405afe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b5afe)
  #<!-- -->32 0x00007ff790b1f23d (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xccf23d)
  #<!-- -->33 0x00007ff79058db25 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73db25)
  #<!-- -->34 0x00007ff79058d918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73d918)
  #<!-- -->35 0x00007ff79028b8[78](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:79) (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43b878)
  #<!-- -->36 0x00007ff[79](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:80)028abfe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43abfe)
  #<!-- -->37 0x00007ff79028a777 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43a777)
  #<!-- -->38 0x00007ff7902[81](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:82)f06 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x431f06)
  #<!-- -->39 0x00007ff79027f683 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x42f683)
  #<!-- -->40 0x00007ff791a4c[82](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:83)8 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1bfc828)
  #<!-- -->41 0x00007ffe114f4cb0 (C:\Windows\System32\KERNEL32.DLL+0x14cb0)
  #<!-- -->42 0x00007ffe11d3edcb (C:\Windows\SYSTEM32\ntdll.dll+0x7edcb)
clang-cl : error : clang frontend command failed due to signal (use -v to see invocation) [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  clang version 19.1.1
  Target: amd64-pc-windows-msvc
  Thread model: posix
  InstalledDir: C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
  PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
  Preprocessed source(s) and associated run script(s) are located at:
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.cpp [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.sh [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
```
</details>


---

### Comment 7 - llvmbot


@llvm/issue-subscribers-c-20

Author: MiguelBarro (MiguelBarro)

<details>
## Environment
- [Github workflow](https://raw.githubusercontent.com/MiguelBarro/CppCoroutines/refs/heads/main/.github/workflows/ci.yml)
- Github hosted windows-latest runner (matches windows-2025 on this report date)
- using `CMake` with visual studio generator using `ClangCl` toolset.

## Reproduction steps
The crash was generated in [this repo](https://github.com/MiguelBarro/CppCoroutines.git) *github actions*.
The repo workflow uses windows, ubuntu and Macos github hosted runners. It runs `CMake` to build C++ sources.
The issue is in the `windows-clang` job. Note that `clang` works perfectly on Ubuntu (see `ubuntu-clang` and `macos-clang` jobs).
The issue appears when the C++ sources were modified to use `unique_ptr&lt;char[]&gt;`.

## Crash report
It is available [here](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#logs).
basically:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Tools\\Llvm\\x64\\bin\\clang-cl.exe" @<!-- -->C:\\Users\\runneradmin\\AppData\\Local\\Temp\\MSBuildTemp\\tmp53ddccde22e94fa5985920d130732494.rsp
  1.	&lt;eof&gt; parser at end of file
  2.	Optimizer
  3.	Running pass "require&lt;globals-aa&gt;,function(invalidate&lt;aa&gt;),require&lt;profile-summary&gt;,cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split)),function(invalidate&lt;should-not-run-function-passes&gt;),cgscc(devirt&lt;4&gt;())" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  4.	Running pass "cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split))" on module "D:\a\CppCoroutines\CppCoroutines\examples\Gor\client.cpp"
  5.	Running pass "jump-threading" on function "?session@@<!-- -->YA?AV?$future@<!-- -->U?$pair@<!-- -->_K_K@<!-- -->std@@@<!-- -->std@@<!-- -->AEAVio_service@<!-- -->asio@@<!-- -->AEAV?$basic_resolver_iterator@<!-- -->Vtcp@<!-- -->ip@<!-- -->asio@@@<!-- -->ip@<!-- -->4@<!-- -->_KAEAU?$atomic@<!-- -->_N@<!-- -->2@@<!-- -->Z.resume"
  Exception Code: 0xC0000005
   #<!-- -->0 0x00007ff790d194ce (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xec94ce)
   #<!-- -->1 0x00007ff7911330ea (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x12e30ea)
   #<!-- -->2 0x00007ff79164d3c4 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17fd3c4)
   #<!-- -->3 0x00007ff791645076 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x17f5076)
   #<!-- -->4 0x00007ff79173db96 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18edb96)
   #<!-- -->5 0x00007ff7917390af (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e90af)
   #<!-- -->6 0x00007ff7917355f0 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18e55f0)
   #<!-- -->7 0x00007ff791177c24 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327c24)
   #<!-- -->8 0x00007ff791177a71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1327a71)
   #<!-- -->9 0x00007ff7914b9643 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1669643)
  #<!-- -->10 0x00007ff7914b92c1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x16692c1)
  #<!-- -->11 0x00007ff791280680 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1430680)
  #<!-- -->12 0x00007ff79128037b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x143037b)
  #<!-- -->13 0x00007ff7916f5fab (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5fab)
  #<!-- -->14 0x00007ff7916f5c8b (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x18a5c8b)
  #<!-- -->15 0x00007ff791835b59 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e5b59)
  #<!-- -->16 0x00007ff7918358eb (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x19e58eb)
  #<!-- -->17 0x00007ff7917eb4d6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x199b4d6)
  #<!-- -->18 0x00007ff790235b71 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x3e5b71)
  #<!-- -->19 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #<!-- -->20 0x00007ff790642c50 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2c50)
  #<!-- -->21 0x00007ff790642a81 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x7f2a81)
  #<!-- -->22 0x00007ff79012a260 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2da260)
  #<!-- -->23 0x00007ff790125918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2d5918)
  #<!-- -->24 0x00007ff79011c432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x2cc432)
  #<!-- -->25 0x00007ff7914bc015 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x166c015)
  #<!-- -->26 0x00007ff7919ac588 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1b5c588)
  #<!-- -->27 0x00007ff79040d8a1 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd8a1)
  #<!-- -->28 0x00007ff79040d432 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5bd432)
  #<!-- -->29 0x00007ff79040a1a6 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5ba1a6)
  #<!-- -->30 0x00007ff790407165 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b7165)
  #<!-- -->31 0x00007ff790405afe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x5b5afe)
  #<!-- -->32 0x00007ff790b1f23d (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0xccf23d)
  #<!-- -->33 0x00007ff79058db25 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73db25)
  #<!-- -->34 0x00007ff79058d918 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x73d918)
  #<!-- -->35 0x00007ff79028b8[78](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:79) (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43b878)
  #<!-- -->36 0x00007ff[79](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:80)028abfe (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43abfe)
  #<!-- -->37 0x00007ff79028a777 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x43a777)
  #<!-- -->38 0x00007ff7902[81](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:82)f06 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x431f06)
  #<!-- -->39 0x00007ff79027f683 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x42f683)
  #<!-- -->40 0x00007ff791a4c[82](https://github.com/MiguelBarro/CppCoroutines/actions/runs/14553506438/job/40827454188#step:3:83)8 (C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-cl.exe+0x1bfc828)
  #<!-- -->41 0x00007ffe114f4cb0 (C:\Windows\System32\KERNEL32.DLL+0x14cb0)
  #<!-- -->42 0x00007ffe11d3edcb (C:\Windows\SYSTEM32\ntdll.dll+0x7edcb)
clang-cl : error : clang frontend command failed due to signal (use -v to see invocation) [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  clang version 19.1.1
  Target: amd64-pc-windows-msvc
  Thread model: posix
  InstalledDir: C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
  PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
  Preprocessed source(s) and associated run script(s) are located at:
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.cpp [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: C:\Users\RUNNER~1\AppData\Local\Temp\client-85519e.sh [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
clang-cl : message : diagnostic msg: [D:\a\CppCoroutines\CppCoroutines\examples\build\Gor\client.vcxproj]
  ********************
```
</details>


---

### Comment 8 - zmodem

I tried building older versions, and this seems to repro at least back to `llvmorg-15-init` (didn't try older ones), so not a recent regression.

---

