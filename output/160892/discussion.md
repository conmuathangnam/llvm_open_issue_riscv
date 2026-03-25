# [SPIR-V] USM tests crashes when SYCL is running with the LLVM SPIR-V backend in DG2

**Author:** YixingZhang007
**URL:** https://github.com/llvm/llvm-project/issues/160892
**Status:** Closed
**Labels:** invalid, crash, backend:SPIR-V
**Closed Date:** 2026-03-16T08:03:46Z

## Body

2 SYCL E2E tests in USM crash when SYCL is running with the LLVM SPIR-V backend in DG2.  This issue is to cover triaging and introducing support for missing features and/or extensions.

The 2 tests that fail are:
sycl/test-e2e/USM/memops2d/copy2d_device_to_host.cpp
sycl/test-e2e/USM/memops2d/memcpy2d_device_to_host.cpp

## Comments

### Comment 1 - frederick-vs-ja

Could there be some reduced examples?

---

### Comment 2 - YixingZhang007

This issue is related to PR https://github.com/intel/llvm/pull/20418 . Additional details (logs and reproduction steps) for this issue will be added after the PR merges. 

---

### Comment 3 - jmmartinez

Hi @YixingZhang007 ,
It seems that https://github.com/intel/llvm/pull/20418 landed. Could you add a minimal reproducer to the issue?

---

