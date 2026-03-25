#!/bin/bash

# Usage:
# Pass the directory name of arm-toolchain directory
# ./atfe_assertions.sh /home/vrupan01/arm-toolchain
# ./atfe_assertions.sh /home/ubuntu/arm-toolchain

clear

PS4='+ $(date)\011 '
set -Eeuxo pipefail
set -v

trap 'rc=$?; echo "Build failed at $(date "+%Y-%m-%d %H:%M:%S") on line $LINENO (exit code: $rc)"' ERR
trap 'rc=$?; echo "Build ended at $(date "+%Y-%m-%d %H:%M:%S") with exit code $rc"' EXIT

# Disable CCACHE
export CCACHE_DISABLE=1

REPO_ROOT="${1:-/home/vrupan01/arm-toolchain}"

# Check if directory exists
if [[ ! -d "$REPO_ROOT/arm-software/embedded" ]]; then
  echo "Error: Directory does not exist: $REPO_ROOT/arm-software/embedded" >&2
  exit 1
fi

stage1()
{
    echo "==> Stage 1: Building clang without assertions"

    # Compile bootstrap compiler
    rm -Rf "${REPO_ROOT}"/build_llvm
    mkdir "${REPO_ROOT}"/build_llvm;
    cd "${REPO_ROOT}"/build_llvm;

    cmake ../llvm -G Ninja \
        -DLLVM_ENABLE_PROJECTS="clang;llvm;lld" \
        -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind;compiler-rt" \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_TARGETS_TO_BUILD="AArch64;X86" \
        -DCLANG_DEFAULT_LINKER=lld \
        -DLLVM_INCLUDE_TESTS=OFF \
        -DCLANG_INCLUDE_TESTS=OFF \
        -DLLD_INCLUDE_TESTS=OFF

        # Build toolchain + runtimes so libc++.so.1 is produced
        ninja clang lld cxx cxxabi unwind compiler-rt
}

stage2()
{
    echo "==> Stage 2: Building clang with assertions"

    rm -Rf "${REPO_ROOT}"/build_assertions
    mkdir "${REPO_ROOT}"/build_assertions
    cd "${REPO_ROOT}"/build_assertions

    # Path can be either x86 machine or aarch64 machine.
    export LD_LIBRARY_PATH="${REPO_ROOT}/build_llvm/lib/aarch64-unknown-linux-gnu:${REPO_ROOT}/build_llvm/lib/x86_64-unknown-linux-gnu:${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    cmake ../arm-software/embedded -GNinja \
        -DFETCHCONTENT_QUIET=OFF \
        -DLLVM_TOOLCHAIN_ENABLE_PICOLIBC=OFF \
        -DLLVM_TOOLCHAIN_ENABLE_LLVMLIBC=ON \
        -DCMAKE_C_COMPILER="${REPO_ROOT}/build_llvm/bin/clang" \
        -DCMAKE_CXX_COMPILER="${REPO_ROOT}/build_llvm/bin/clang++" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_ENABLE_EXPENSIVE_CHECKS=ON \
        -DLLVM_EXTERNAL_LIT="${REPO_ROOT}/build_llvm/bin/llvm-lit" \
        -DLLVM_ENABLE_LIBCXX=ON \
        -DENABLE_PARALLEL_LIB_CONFIG=OFF \
        -DLLVM_TOOLCHAIN_LIBRARY_VARIANTS="aarch64a_exn_rtti_unaligned;armv7m_hard_fpv5_d16_exn_rtti_unaligned_size;armv8.1m.main_soft_nofp_nomve_pacret_bti_unaligned_size"

    ninja -j4 -l4 package-llvm-toolchain

    echo "==> Stage 2: Completed building clang with assertions"
}

test_stage2()
{
    cd "${REPO_ROOT}"/build_assertions

    # Path can be either x86 machine or aarch64 machine.
    export LD_LIBRARY_PATH="${REPO_ROOT}/build_llvm/lib/aarch64-unknown-linux-gnu:${REPO_ROOT}/build_llvm/lib/x86_64-unknown-linux-gnu:${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    # Testing with assertions
    ninja -v check-all
    ninja -v check-compiler-rt-armv7m_hard_fpv5_d16_exn_rtti_unaligned_size
    # ninja -v check-picolibc-armv7m_hard_fpv5_d16_exn_rtti_unaligned_size
    ninja -v check-cxx-armv7m_hard_fpv5_d16_exn_rtti_unaligned_size
    ninja -v check-cxxabi-armv7m_hard_fpv5_d16_exn_rtti_unaligned_size
    ninja -v check-unwind-armv7m_hard_fpv5_d16_exn_rtti_unaligned_size

    echo "==> Testing is complete"
}

cd "$REPO_ROOT"
stage1
stage2
test_stage2

echo "==> Script is finished successfully"

