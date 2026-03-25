# [SPIR-V]  bindless_images tests crashes when SYCL is running with the LLVM SPIR-V backend in DG2

**Author:** YixingZhang007
**URL:** https://github.com/llvm/llvm-project/issues/160893
**Status:** Closed
**Labels:** invalid, crash, backend:SPIR-V
**Closed Date:** 2026-03-16T08:03:42Z

## Body

2 SYCL E2E tests in bindless_images crash when SYCL is running with the LLVM SPIR-V backend. This issue is to cover triaging and introducing support for missing features and/or extensions.

The 2 tests that fail are:
sycl/test-e2e/bindless_images/array/read_write_unsampled_array.cpp
sycl/test-e2e/bindless_images/read_write_unsampled.cpp

Previous PR for adding SPV_INTEL_bindless_images extension: https://github.com/llvm/llvm-project/pull/127737

## Comments

### Comment 1 - YixingZhang007

This issue is related to PR https://github.com/intel/llvm/pull/20418 . Additional details (logs and reproduction steps) for this issue will be added after the PR merges.

---

### Comment 2 - jmmartinez

Hi @YixingZhang007 ,
It seems that https://github.com/intel/llvm/pull/20418 landed. Could you add a minimal reproducer to the issue?

---

