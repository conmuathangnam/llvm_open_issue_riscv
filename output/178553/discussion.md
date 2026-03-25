# [clangd] Crash when opening a C++ file with trailing backslash in filename

**Author:** dfl8406
**URL:** https://github.com/llvm/llvm-project/issues/178553
**Status:** Open
**Labels:** clangd, crash

## Body

# Bug Report: clangd 21.1.0 crashes on Windows when opening a file with trailing backslash in filename

## Summary
clangd crashes immediately when opening a C++ file whose name ends with a trailing backslash (e.g., `app.cpp\`) in VS Code. The trailing backslash is **not visible** in the VS Code file explorer, making the issue difficult to diagnose. This crash is triggered during AST construction and results in a `STATUS_ACCESS_VIOLATION` (error code `3221225477`).

## Reproduction Steps
1. On Windows, open your target folder in VS Code.
2. In VS Code's file explorer: right-click → select "New File" → type the filename with a trailing backslash (e.g., `app.cpp\`) → press Enter.
3. VS Code will create the file **without any error prompts**, and the trailing backslash is **invisible** in VS Code's file list (the file shows as `app.cpp`).
4. Click to open the file in VS Code → clangd crashes immediately with the `write EPIPE` error and exits.

## Expected Behavior
- clangd should either:
  1.  Handle the invalid filename gracefully and report a clear error, or
  2.  Reject the file instead of crashing.

## Actual Behavior
- clangd crashes abruptly with a memory access violation (`STATUS_ACCESS_VIOLATION`).
- The server exits with code `3221225477`.
- VS Code shows errors like:
  ```
  Connection to server got closed. Server will not be restarted.
  Client Clang Language Server: connection to server is erroring. write EPIPE
  ```

## Environment
- **clangd version**: 21.1.0
- **VS Code version**: [1.108.2]
- **OS**: Windows [Windows 10 IoT LTSC 21H2]
- **Compiler**: `clang-cl.exe --driver-mode=cl`

## Crash Log Snippet
```
I[08:11:57.252] <-- textDocument/documentSymbol(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled during AST worker action: Build AST
  Filename: w:\myGit\myProject\Frame\demo\d01_window\application.cpp\
  Directory: W:/myGit/myProject/Frame/build
  Command Line: W:\sdk\Clang_LLVM\bin\clang-cl.exe --driver-mode=cl /nologo -DEvent_EXPORTS [...]
  Version: 1
[Error - 08:11:57] Client Clang Language Server: connection to server is erroring.
write EPIPE
Shutting down server.
[Error - 08:11:57] Server process exited with code 3221225477.
```

## Additional Context
- The trailing backslash in the filename is **invisible in VS Code's UI**, which makes the root cause hard to identify initially.
- Deleting the problematic file and restarting VS Code restores clangd to normal operation.
- This issue occurs consistently on multiple Windows machines with the same clangd version.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (dfl8406)

<details>
# Bug Report: clangd 21.1.0 crashes on Windows when opening a file with trailing backslash in filename

## Summary
clangd crashes immediately when opening a C++ file whose name ends with a trailing backslash (e.g., `app.cpp\`) in VS Code. The trailing backslash is **not visible** in the VS Code file explorer, making the issue difficult to diagnose. This crash is triggered during AST construction and results in a `STATUS_ACCESS_VIOLATION` (error code `3221225477`).

## Reproduction Steps
1.  On Windows, create a file with a trailing backslash in its name via Command Prompt:
    ```cmd
    type NUL &gt; "app.cpp\"
    ```
2.  Open the folder containing this file in VS Code.
3.  Open the file `app.cpp` (note: the trailing backslash is not displayed in VS Code's UI).
4.  Observe that clangd immediately crashes with a `write EPIPE` error and the server stops responding.

## Expected Behavior
- clangd should either:
  1.  Handle the invalid filename gracefully and report a clear error, or
  2.  Reject the file instead of crashing.

## Actual Behavior
- clangd crashes abruptly with a memory access violation (`STATUS_ACCESS_VIOLATION`).
- The server exits with code `3221225477`.
- VS Code shows errors like:
  ```
  Connection to server got closed. Server will not be restarted.
  Client Clang Language Server: connection to server is erroring. write EPIPE
  ```

## Environment
- **clangd version**: 21.1.0
- **VS Code version**: [1.108.2]
- **OS**: Windows [Windows 10 IoT LTSC 21H2]
- **Compiler**: `clang-cl.exe --driver-mode=cl`

## Crash Log Snippet
```
I[08:11:57.252] &lt;-- textDocument/documentSymbol(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled during AST worker action: Build AST
  Filename: w:\myGit\myProject\Frame\demo\d01_window\application.cpp\
  Directory: W:/myGit/myProject/Frame/build
  Command Line: W:\sdk\Clang_LLVM\bin\clang-cl.exe --driver-mode=cl /nologo -DEvent_EXPORTS [...]
  Version: 1
[Error - 08:11:57] Client Clang Language Server: connection to server is erroring.
write EPIPE
Shutting down server.
[Error - 08:11:57] Server process exited with code 3221225477.
```

## Additional Context
- The trailing backslash in the filename is **invisible in VS Code's UI**, which makes the root cause hard to identify initially.
- Deleting the problematic file and restarting VS Code restores clangd to normal operation.
- This issue occurs consistently on multiple Windows machines with the same clangd version.

</details>


---

### Comment 2 - efriedma-quic

I can't execute the first step on your list; trying to create the file gives "The directory name is invalid."

---

### Comment 3 - dfl8406

Hi @efriedma-quic,Thanks for your reply! I didn’t create the file via Command Prompt — I made it directly in VS Code’s file explorer: when I right-clicked to create a new file and typed "app.cpp" (with the trailing backslash) in the filename input box, VS Code didn’t report any errors and created the file successfully. The trailing backslash wasn’t visible in VS Code’s file list, but the actual filename did include it — and opening this file then caused clangd to crash.

---

### Comment 4 - Witty-31-06

This issue can be recreated in Linux too, although it's a bit different, in Linux, VSC shows trailing \ in the file explorer and opening the file doesn't invoke clangd, but rename an already existing cpp file to .cpp\ results in SIGSEGV

---

### Comment 5 - dfl8406

Hi @Witty-31-06,Thanks a lot for sharing the Linux reproduction scenario! It’s super helpful to know this issue isn’t just Windows-specific — this cross-platform detail will definitely help the developers track down the root cause of the filename handling bug.

---

