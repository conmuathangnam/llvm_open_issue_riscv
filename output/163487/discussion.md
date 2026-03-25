# clang-tidy 21.1.2 crashes on JUCE-based GhostLatheProcessor.cpp

**Author:** DavidDanielSauce
**URL:** https://github.com/llvm/llvm-project/issues/163487
**Status:** Closed
**Labels:** clang-tidy, crash, regression:21
**Closed Date:** 2025-11-22T05:04:01Z

## Body



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (DavidDanielSauce)

<details>
### Summary

`clang-tidy` 21.1.2 from Homebrew (`llvm@<!-- -->21`, arm64) segfaults when analyzing JUCE-heavy translation units in the GhostLathe audio plugin project. The crash is deterministic on macOS 14.5 when running against `GhostLatheProcessor.cpp` or the associated unit tests.

### Version

21.1.2 (Homebrew `llvm@<!-- -->21` build, arm64)

### OS

macOS 14.5 (arm64)

### Steps to Reproduce

1. Install llvm@<!-- -->21 via Homebrew: `brew install llvm@<!-- -->21`.
2. Configure GhostLathe with compile commands:
   ```
   cmake -S . -B build -G Ninja \
     -DCMAKE_BUILD_TYPE=Debug \
     -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
   ```
3. Ensure `/opt/homebrew/opt/llvm@<!-- -->21/bin` is first on `PATH`.
4. Run clang-tidy against the problematic TU:
   ```
   run-clang-tidy -p build \
     -clang-tidy-binary clang-tidy \
     src/plugin/GhostLatheProcessor.cpp
   ```

### Expected Behavior

clang-tidy completes analysis and emits diagnostics for the translation unit.

### Actual Behavior

clang-tidy terminates with a segmentation fault while traversing JUCE-generated constructs in `GhostLatheProcessor.cpp`. The crash reproduces consistently on related unit tests (`tests/plugin/GhostLatheProcessorTests.cpp`) as well. A sanitized backtrace and logs are included in the attachments.

### Additional Context

- Downgrading to llvm@<!-- -->20 (clang-tidy 20.1.x) avoids the crash; we currently pin CI to that toolchain.
- JUCE version: 8.0.10. Project builds with CMake 3.30.2 and Ninja 1.12.1; SDKROOT points to the Xcode 16 macOS 14.4/26.0 SDK.
- Sanitized reproduction bundle (sources subset, compile commands, logs) plus stack trace are published here: &lt;https://gist.github.com/DavidDanielSauce/e5cea023e78b31b9b0de0572ed7fef1f&gt;.
  - The gist contains `clang_tidy_crash_report.md`, `clang_tidy_crash_20250930.log`, and a base64-encoded `clang_tidy_crash_bundled.tgz` (decode with `base64 -d clang_tidy_crash_bundled.tgz.base64 &gt; clang_tidy_crash_bundled.tgz`).
- Happy to provide additional sanitized artefacts (e.g., full compile database) if needed.

Thanks for taking a look!

</details>


---

### Comment 2 - chrchr-github

The stack trace looks similar to the one in https://github.com/llvm/llvm-project/issues/155906

---

### Comment 3 - localspook

Could you provide the TU that's crashing? (Ideally a self-contained one. It's okay if it's not fully reduced.) I took an initial look at the crash and the check's code and didn't get anywhere. I also tried [reducing just the JUCE code on godbolt](https://godbolt.org/z/z91GxnY9h) and couldn't reproduce the crash. It would help to see how your code is actually using JUCE.

---

### Comment 4 - EugeneZelenko

@DavidDanielSauce; could you please elaborate?

---

