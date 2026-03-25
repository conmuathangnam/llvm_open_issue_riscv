# MLIR: `mlir-runner` consistently crashes with SIGSEGV (-11)

**Author:** kashyapc
**URL:** https://github.com/llvm/llvm-project/issues/188073
**Status:** Open
**Labels:** mlir, crash

## Body


## Description

MLIR Execution Engine consistently crashes during JIT execution with SIGSEGV (-11)

I'm not quite sure if it is the same issue that I reported here:  https://github.com/llvm/llvm-project/issues/162655

But this can be worked around with this commit: https://github.com/kashyapc/llvm-project/commit/86e20a7c0f04 and also just filtering out the tests wit this

```
LIT_OPTS="-vv --filter-out=mlir-runner" ninja -j1 -C build check-mlir
```
I had to pass `-j1` to `ninja`, or else the build crashes with a fatal 'glibc' error.

Or:

```
./build/bin/llvm-lit -vv --filter-out=mlir-runner build/tools/mlir/test
```



## Environment

Hardware: 

- UR-DP1000  (this bug report is based on this)
- SiFive HiFive Premier P550

## Reproducer

Running from a recent upstream LLVM commit:
```
$ git describe; rpm -q glibc; uname -r
llvmorg-23-init-7561-g603f1e91c1e9
glibc-2.41-9.fc42.riscv64
6.17.2+

```
My `cmake` invocation:

```
$ cmake -G Ninja -S llvm -B build \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="RISCV" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_USE_LINKER=lld \
  -DLLVM_PARALLEL_LINK_JOBS=1 \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++
```

Run the `mlir-runner` test suite:
```
$ ./build/bin/llvm-lit -vv -j1 --filter=mlir-runner build/tools/mlir/test/mlir-runner
-- Testing: 23 tests, 1 workers --
FAIL: MLIR :: mlir-runner/async-error.mlir (1 of 23)
******************** TEST 'MLIR :: mlir-runner/async-error.mlir' FAILED ********************
Exit Code: 2

Command Output (stdout):
--
# RUN: at line 1
/home/fedora/src/upstream/llvm-project/build/bin/mlir-opt /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir -pass-pipeline="builtin.module(async-to-async-runtime,func.func(async-runtime-ref-counting,async-runtime-ref-counting-opt),convert-async-to-llvm,func.func(convert-linalg-to-loops,convert-scf-to-cf),convert-vector-to-llvm,func.func(convert-arith-to-llvm),convert-func-to-llvm,convert-cf-to-llvm,reconcile-unrealized-casts)"  | /home/fedora/src/upstream/llvm-project/build/bin/mlir-runner                                                           -e main -entry-point-result=void -O0                                    -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_c_runner_utils.so       -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_runner_utils.so         -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_async_runtime.so    | /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/mlir-opt /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir '-pass-pipeline=builtin.module(async-to-async-runtime,func.func(async-runtime-ref-counting,async-runtime-ref-counting-opt),convert-async-to-llvm,func.func(convert-linalg-to-loops,convert-scf-to-cf),convert-vector-to-llvm,func.func(convert-arith-to-llvm),convert-func-to-llvm,convert-cf-to-llvm,reconcile-unrealized-casts)'
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/mlir-runner -e main -entry-point-result=void -O0 -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_c_runner_utils.so -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_runner_utils.so -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_async_runtime.so
# .---command stderr------------
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# `-----------------------------
# error: command failed with exit status: -11
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# .---command stderr------------
# | FileCheck error: '<stdin>' is empty.
# | FileCheck command line:  /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# `-----------------------------
# error: command failed with exit status: 2
[...]
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Kashyap Chamarthy (kashyapc)

<details>

## Description

MLIR Execution Engine consistently crashes during JIT execution with SIGSEGV (-11)

I'm not quite sure if it is the same issue that I reported here:  https://github.com/llvm/llvm-project/issues/162655

But this can be worked around with this commit: https://github.com/kashyapc/llvm-project/commit/86e20a7c0f04 and also just filtering out the tests wit this

```
LIT_OPTS="-vv --filter-out=mlir-runner" ninja -j1 -C build check-mlir
```
I had to pass `-j1` to `ninja`, or else the build crashes with a fatal 'glibc' error.

Or:

```
./build/bin/llvm-lit -vv --filter-out=mlir-runner build/tools/mlir/test
```



## Environment

Hardware: 

- UR-DP1000  (this bug report is based on this)
- SiFive HiFive Premier P550

## Reproducer

Running from a recent upstream LLVM commit:
```
$ git describe; rpm -q glibc; uname -r
llvmorg-23-init-7561-g603f1e91c1e9
glibc-2.41-9.fc42.riscv64
6.17.2+

```
My `cmake` invocation:

```
$ cmake -G Ninja -S llvm -B build \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="RISCV" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_USE_LINKER=lld \
  -DLLVM_PARALLEL_LINK_JOBS=1 \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++
```

Run the `mlir-runner` test suite:
```
$ ./build/bin/llvm-lit -vv -j1 --filter=mlir-runner build/tools/mlir/test/mlir-runner
-- Testing: 23 tests, 1 workers --
FAIL: MLIR :: mlir-runner/async-error.mlir (1 of 23)
******************** TEST 'MLIR :: mlir-runner/async-error.mlir' FAILED ********************
Exit Code: 2

Command Output (stdout):
--
# RUN: at line 1
/home/fedora/src/upstream/llvm-project/build/bin/mlir-opt /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir -pass-pipeline="builtin.module(async-to-async-runtime,func.func(async-runtime-ref-counting,async-runtime-ref-counting-opt),convert-async-to-llvm,func.func(convert-linalg-to-loops,convert-scf-to-cf),convert-vector-to-llvm,func.func(convert-arith-to-llvm),convert-func-to-llvm,convert-cf-to-llvm,reconcile-unrealized-casts)"  | /home/fedora/src/upstream/llvm-project/build/bin/mlir-runner                                                           -e main -entry-point-result=void -O0                                    -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_c_runner_utils.so       -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_runner_utils.so         -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_async_runtime.so    | /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/mlir-opt /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir '-pass-pipeline=builtin.module(async-to-async-runtime,func.func(async-runtime-ref-counting,async-runtime-ref-counting-opt),convert-async-to-llvm,func.func(convert-linalg-to-loops,convert-scf-to-cf),convert-vector-to-llvm,func.func(convert-arith-to-llvm),convert-func-to-llvm,convert-cf-to-llvm,reconcile-unrealized-casts)'
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/mlir-runner -e main -entry-point-result=void -O0 -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_c_runner_utils.so -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_runner_utils.so -shared-libs=/home/fedora/src/upstream/llvm-project/build/lib/libmlir_async_runtime.so
# .---command stderr------------
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# `-----------------------------
# error: command failed with exit status: -11
# executed command: /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# .---command stderr------------
# | FileCheck error: '&lt;stdin&gt;' is empty.
# | FileCheck command line:  /home/fedora/src/upstream/llvm-project/build/bin/FileCheck /home/fedora/src/upstream/llvm-project/mlir/test/mlir-runner/async-error.mlir --dump-input=always
# `-----------------------------
# error: command failed with exit status: 2
[...]
```
</details>


---

### Comment 2 - kashyapc

Based on the `mlir-runner` test invocation, I ran GDB and got the attached backtraces. Attached here; I'm not sure how useful this is as I did _not_ build LLVM with debugging for this run (I could do it if need be).

[backtrace-mlir-runner.txt](https://github.com/user-attachments/files/26189886/backtrace-mlir-runner.txt)
[mlir-runner-SIGSEGV.txt](https://github.com/user-attachments/files/26189887/mlir-runner-SIGSEGV.txt)

---

