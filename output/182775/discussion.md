# Crash during loop optimization

**Author:** sesse
**URL:** https://github.com/llvm/llvm-project/issues/182775
**Status:** Closed
**Labels:** duplicate, crash, llvm:transforms
**Closed Date:** 2026-03-10T09:03:59Z

## Body

This is minimized from a (huge, vector-intrinsic-heavy) program:

```c++
long *a;
void o() {
  for (unsigned b;; ++b)
    for (unsigned c = b; c < 4; ++c)
      for (unsigned d = c; d; ++d)
        for (unsigned e = d; e; ++e)
          for (unsigned f = e; f; ++f)
            for (unsigned g = f; g; ++g)
              for (unsigned l = g; l; ++l)
                for (; a[c];)
                  for (unsigned h = l; h < 4; ++h)
                    for (unsigned i = h; i < 4; ++i)
                      for (unsigned j = i; j < 4; ++j)
                        for (long k[j]; a;)
                          for (unsigned m = j; m < 4; ++m)
                            for (long n = a[m]; n; n = 1)
                              ;
}
```

Compiling with `clang++-23 -O -std=c++20 -c foo.cc` on `Debian clang version 23.0.0 (++20260204101751+dd02b404b020-1~exp1)` yields:

```
foo.cc:14:37: warning: variable length arrays in C++ are a Clang extension [-Wvla-cxx-extension]
   14 |                         for (long k[j]; a;)
      |                                     ^
foo.cc:14:37: note: read of non-const variable 'j' is not allowed in a constant expression
foo.cc:13:37: note: declared here
   13 |                       for (unsigned j = i; j < 4; ++j)
      |                                     ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++-23 -O -std=c++20 -c foo.cc
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "foo.cc"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "foo.cc"
5.	Running pass "loop(loop-idiom,indvars,loop-deletion,loop-unroll-full)" on function "_Z1ov"
#0 0x00007f79cd54431a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d4431a)
#1 0x00007f79cd541b47 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d41b47)
#2 0x00007f79cd46fd34 (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4c6fd34)
#3 0x00007f79c824aa70 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40a70)
#4 0x00007f79c95bdfc0 (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0xdbdfc0)
clang++-23: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 23.0.0 (++20260204101751+dd02b404b020-1~exp1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-23/bin
clang++-23: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-23: note: diagnostic msg: /tmp/foo-5bbb4a.cpp
clang++-23: note: diagnostic msg: /tmp/foo-5bbb4a.sh
clang++-23: note: diagnostic msg: 

********************
```

This worked with Clang 19, but fails in the same way on Clang 20, 21 and 22.

## Comments

### Comment 1 - sesse

Correction: This specific reduction seemingly works on Clang 21 and 22 (at least without asserts; just testing with the Debian package). I can re-reduce from the original to get one that fails in 22.

---

### Comment 2 - sesse

Reduced for Clang 22 (also crashes in Clang 23, works in 19):

```c++
long* b;  
void p();  
void q() { 
 for (unsigned f;; ++f)
  for (unsigned g = f; g < 4; ++g)
   for (; b[g];)                    
    for (unsigned h = g; h; ++h)
     for (unsigned i = h; i; ++i) 
      for (unsigned a = i; a; ++a) 
       for (unsigned j = a; j; ++j) 
        for (unsigned k = j; k; ++k) 
         for (unsigned c = k; c < 4; ++c)
          for (unsigned n = c; n < 4; ++n) 
           for (unsigned l = n; l < 4; ++l) {
            p();                               
            for (unsigned m = l; m < 4; ++m)                     
             for (long o = (&b)[0][m]; o; o &= 1)
              ;                                    
           }     
}             
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-22 -O -std=c++20 -fcolor-diagnostics -c -o foo.o foo.cc
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "foo.cc"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,libcalls-shrinkwrap,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<no-nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,loop-deletion,loop-unroll-full),sroa<modify-cfg>,memcpyopt,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,coro-elide,adce,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "foo.cc"
5.	Running pass "loop(loop-idiom,indvars,loop-deletion,loop-unroll-full)" on function "_Z1qv"
#0 0x00007f15a0e2d35a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x462d35a)
#1 0x00007f15a0e2ab57 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x462ab57)
#2 0x00007f15a0d63a24 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x4563a24)
#3 0x00007f159c24aa70 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40a70)
#4 0x00007f159d2b91e0 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xab91e0)
clang++-22: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 22.0.0 (++20250731080150+be449d6b6587-1~exp1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-22: note: diagnostic msg: 

clang++-22: note: diagnostic msg: /tmp/foo-e7e2c1.sh
clang++-22: note: diagnostic msg: 

********************
```

[foo-e7e2c1.sh](https://github.com/user-attachments/files/25477725/foo-e7e2c1.sh)
[foo-e7e2c1.cpp](https://github.com/user-attachments/files/25477726/foo-e7e2c1.cpp)

---

### Comment 3 - cabbaken

Maybe duplicate of #153090, I cannot reproduce this crash with this patch #172234.

---

### Comment 4 - sesse

Wow, I had forgotten that I had reported #153090, they come from the same project. They are duplicates, yes.

---

