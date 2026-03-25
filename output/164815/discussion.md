# [flang][OpenMP] Crash with multiple Fujitsu OpenMP tests

**Author:** eugeneepshteyn
**URL:** https://github.com/llvm/llvm-project/issues/164815
**Status:** Closed
**Labels:** flang:frontend, crash, flang:openmp
**Closed Date:** 2025-10-27T18:00:08Z

## Body

Here's one particular file that reproduces the crash: https://github.com/fujitsu/compiler-test-suite/blob/main/Fortran/0141/0141_0017.f90

List of Fujitsu tests that seem to crash in similar fashion: `0141_0017 0156_0011 0391_0023 0401_0005 0401_0010 0619_0107 0629_0203 0629_0406 0683_0099 0683_0100 0683_0101 0684_0032 0684_0037 0686_0002 0686_0003 0715_0001 0715_0002 0762_0000 1009_0034 1010_0001 1012_0002 1012_0013 1012_0015 1012_0016 1012_0017 1012_0018 1012_0020 1013_0048 1013_0052 1013_0054 1013_0059`

Partial backtrace:
```
$ flang -c -fopenmp 0141/0141_0017.f90 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fopenmp -resource-dir /lib/clang/22 -mframe-pointer=all -o 0141_0017.o -x f95 0141/0141_0017.f90
 #0 0x0000587be8ffcc91 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000587be8ffd20b PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000587be8ffaf26 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000587be8ffd98d SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x0000709245845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000587bea30854c Fortran::common::Interval<char const*>::empty() const vm-project/flang/include/flang/Common/interval.h:57:47
 #6 0x0000587bea304345 Fortran::parser::CharBlock::empty() const llvm-project/flang/include/flang/Parser/char-block.h:40:34
 #7 0x0000587beb6484b2 Fortran::semantics::OmpStructureChecker::Enter(Fortran::parser::BlockData const&) llvm-project/flang/lib/Semantics/check-omp-structure.cpp:101:13
 #8 0x0000587beb1bbfb4 bool Fortran::semantics::SemanticsVisitor<Fortran::semantics::OmpStructureChecker>::Pre<Fortran::parser::BlockData>(Fortran::parser::BlockData const&) llvm-project/flang/lib/Semantics/semantics.cpp:89:5
 #9 0x0000587beb1bbf5d std::enable_if<TupleTrait<Fortran::parser::BlockData>, void>::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk<Fortran::parser::BlockData, Fortran::semantics::SemanticsVisitor<Fortran::semantics::OmpStructureChecker>>(Fortran::parser::BlockData const&, Fortran::semantics::SemanticsVisitor<Fortran::semantics::OmpStructureChecker>&) llvm-project/flang/include/flang/Parser/parse-tree-visitor.h:182:9
```

Commit ID of my compiler: 911e5aa5cceda96dfcf578baae41e5452ff2ef11

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: Eugene Epshteyn (eugeneepshteyn)

<details>
Here's one particular file that reproduces the crash: https://github.com/fujitsu/compiler-test-suite/blob/main/Fortran/0141/0141_0017.f90

List of Fujitsu tests that seem to crash in similar fashion: `0141_0017 0156_0011 0391_0023 0401_0005 0401_0010 0619_0107 0629_0203 0629_0406 0683_0099 0683_0100 0683_0101 0684_0032 0684_0037 0686_0002 0686_0003 0715_0001 0715_0002 0762_0000 1009_0034 1010_0001 1012_0002 1012_0013 1012_0015 1012_0016 1012_0017 1012_0018 1012_0020 1013_0048 1013_0052 1013_0054 1013_0059`

Partial backtrace:
```
$ flang -c -fopenmp 0141/0141_0017.f90 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fopenmp -resource-dir /lib/clang/22 -mframe-pointer=all -o 0141_0017.o -x f95 0141/0141_0017.f90
 #<!-- -->0 0x0000587be8ffcc91 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000587be8ffd20b PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000587be8ffaf26 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000587be8ffd98d SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x0000709245845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000587bea30854c Fortran::common::Interval&lt;char const*&gt;::empty() const vm-project/flang/include/flang/Common/interval.h:57:47
 #<!-- -->6 0x0000587bea304345 Fortran::parser::CharBlock::empty() const llvm-project/flang/include/flang/Parser/char-block.h:40:34
 #<!-- -->7 0x0000587beb6484b2 Fortran::semantics::OmpStructureChecker::Enter(Fortran::parser::BlockData const&amp;) llvm-project/flang/lib/Semantics/check-omp-structure.cpp:101:13
 #<!-- -->8 0x0000587beb1bbfb4 bool Fortran::semantics::SemanticsVisitor&lt;Fortran::semantics::OmpStructureChecker&gt;::Pre&lt;Fortran::parser::BlockData&gt;(Fortran::parser::BlockData const&amp;) llvm-project/flang/lib/Semantics/semantics.cpp:89:5
 #<!-- -->9 0x0000587beb1bbf5d std::enable_if&lt;TupleTrait&lt;Fortran::parser::BlockData&gt;, void&gt;::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::BlockData, Fortran::semantics::SemanticsVisitor&lt;Fortran::semantics::OmpStructureChecker&gt;&gt;(Fortran::parser::BlockData const&amp;, Fortran::semantics::SemanticsVisitor&lt;Fortran::semantics::OmpStructureChecker&gt;&amp;) llvm-project/flang/include/flang/Parser/parse-tree-visitor.h:182:9
```

Commit ID of my compiler: 911e5aa5cceda96dfcf578baae41e5452ff2ef11
</details>


---

### Comment 2 - eugeneepshteyn

No crash with this commit ID: bc7e3ff5c55ccdf4b9981cfa255cca858190e3fb (last known good)

---

### Comment 3 - eugeneepshteyn

Fixed by https://github.com/llvm/llvm-project/pull/165250

---

