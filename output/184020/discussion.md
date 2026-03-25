# [clangd] NULL pointer dereference in `VisitCXXThrowExpr` when using `textDocument/typeDefinition` on a bare `throw;` expression

**Author:** hongtaihu
**URL:** https://github.com/llvm/llvm-project/issues/184020
**Status:** Closed
**Labels:** duplicate, clangd, crash
**Closed Date:** 2026-03-01T23:05:46Z

## Body


# Summary

clangd crashes with a NULL pointer dereference when a `textDocument/typeDefinition` (or `textDocument/hover`) LSP request is issued with the cursor on a bare `throw;` rethrow expression (i.e., a `throw` statement with no operand).

The crash is in `XRefs.cpp:VisitCXXThrowExpr`, which calls `S->getSubExpr()->getType()` without checking for `nullptr`. For a rethrow expression (`throw;`), `CXXThrowExpr::getSubExpr()` is documented to return `nullptr` (it uses `cast_or_null`, and the class itself checks `if (!getSubExpr())` in `getEndLoc()`).

Discovered via fuzzing with a fuzzer 

[clangd_bug_repro.gz](https://github.com/user-attachments/files/25664404/clangd_bug_repro.gz)

on clangd trunk `aff5afc48df63615053b2432da198a4932435c3f`.

---

# Affected Version

- **clangd**: 23.0.0git (trunk commit `aff5afc48df63615053b2432da198a4932435c3f`)
- **Likely affected**: all versions containing `VisitCXXThrowExpr` in `XRefs.cpp`

---

# Steps to Reproduce

## Option A — Minimal self-contained Python reproducer

Requires an ASAN-instrumented clangd build. Run:

```bash
python3 repro.py
# or explicitly: python3 repro.py /path/to/clangd_asan
```

The script uses the inline 6-line source from Option B below and sends a `textDocument/typeDefinition` request at `position: {line: 4, character: 8}` (0-indexed), which is the bare `throw;` rethrow.

## Option B — Manual LSP interaction

**Source file** (`minimal.cpp`):

```cpp
void f() {
    try {
        throw 42;
    } catch (int) {
        throw;   // <-- send typeDefinition/hover here
    }
}
```

**LSP messages** (in order):

1. `initialize`
2. `initialized`
3. `textDocument/didOpen` — open `minimal.cpp`
4. *(wait ~8 s for AST build)*
5. `textDocument/typeDefinition` with `position: {line: 4, character: 8}` — cursor on `throw;`
   *(0-indexed; "line 4" in the snippet above, which is the bare `throw;`)*

---

# ASAN Stack Trace

```
==<PID>==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008
==<PID>==The signal is caused by a READ memory access.
==<PID>==Hint: address points to the zero page.
    #0  syscall                             sysdeps/unix/sysv/linux/x86_64/syscall.S:38
    #1  SignalHandler                       llvm/lib/Support/Unix/Signals.inc:429
    #2  <libc signal frame>
    #3  clang::Expr::getType() const        clang/include/clang/AST/Expr.h:144
    #4  VisitCXXThrowExpr                   clangd/XRefs.cpp:2090           <-- CRASH
    #5  Visit                               clang/AST/StmtNodes.inc:670
    #6  typeForNode                         clangd/XRefs.cpp:2133
    #7  operator()  (findType lambda)       clangd/XRefs.cpp:2209
    #8  operator()  (findType lambda)       clangd/XRefs.cpp:2217
    #9  callback_fn                         llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #10 SelectionTree::createEach           clangd/Selection.cpp:1082
    #11 findType                            clangd/XRefs.cpp:2215
    #12 operator()  (ClangdServer lambda)   clangd/ClangdServer.cpp:990
    ...
    [AST worker thread]
```

**ASAN classification**: `SourceAv` / `NOT_EXPLOITABLE` — read access violation at address `0x8` (null pointer + field offset).

---



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Macro Terra (hongtaihu)

<details>

# Summary

clangd crashes with a NULL pointer dereference when a `textDocument/typeDefinition` (or `textDocument/hover`) LSP request is issued with the cursor on a bare `throw;` rethrow expression (i.e., a `throw` statement with no operand).

The crash is in `XRefs.cpp:VisitCXXThrowExpr`, which calls `S-&gt;getSubExpr()-&gt;getType()` without checking for `nullptr`. For a rethrow expression (`throw;`), `CXXThrowExpr::getSubExpr()` is documented to return `nullptr` (it uses `cast_or_null`, and the class itself checks `if (!getSubExpr())` in `getEndLoc()`).

Discovered via fuzzing with a fuzzer 

[clangd_bug_repro.gz](https://github.com/user-attachments/files/25664404/clangd_bug_repro.gz)

on clangd trunk `aff5afc48df63615053b2432da198a4932435c3f`.

---

# Affected Version

- **clangd**: 23.0.0git (trunk commit `aff5afc48df63615053b2432da198a4932435c3f`)
- **Likely affected**: all versions containing `VisitCXXThrowExpr` in `XRefs.cpp`

---

# Steps to Reproduce

## Option A — Minimal self-contained Python reproducer

Requires an ASAN-instrumented clangd build. Run:

```bash
python3 repro.py
# or explicitly: python3 repro.py /path/to/clangd_asan
```

The script uses the inline 6-line source from Option B below and sends a `textDocument/typeDefinition` request at `position: {line: 4, character: 8}` (0-indexed), which is the bare `throw;` rethrow.

## Option B — Manual LSP interaction

**Source file** (`minimal.cpp`):

```cpp
void f() {
    try {
        throw 42;
    } catch (int) {
        throw;   // &lt;-- send typeDefinition/hover here
    }
}
```

**LSP messages** (in order):

1. `initialize`
2. `initialized`
3. `textDocument/didOpen` — open `minimal.cpp`
4. *(wait ~8 s for AST build)*
5. `textDocument/typeDefinition` with `position: {line: 4, character: 8}` — cursor on `throw;`
   *(0-indexed; "line 4" in the snippet above, which is the bare `throw;`)*

---

# ASAN Stack Trace

```
==&lt;PID&gt;==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008
==&lt;PID&gt;==The signal is caused by a READ memory access.
==&lt;PID&gt;==Hint: address points to the zero page.
    #<!-- -->0  syscall                             sysdeps/unix/sysv/linux/x86_64/syscall.S:38
    #<!-- -->1  SignalHandler                       llvm/lib/Support/Unix/Signals.inc:429
    #<!-- -->2  &lt;libc signal frame&gt;
    #<!-- -->3  clang::Expr::getType() const        clang/include/clang/AST/Expr.h:144
    #<!-- -->4  VisitCXXThrowExpr                   clangd/XRefs.cpp:2090           &lt;-- CRASH
    #<!-- -->5  Visit                               clang/AST/StmtNodes.inc:670
    #<!-- -->6  typeForNode                         clangd/XRefs.cpp:2133
    #<!-- -->7  operator()  (findType lambda)       clangd/XRefs.cpp:2209
    #<!-- -->8  operator()  (findType lambda)       clangd/XRefs.cpp:2217
    #<!-- -->9  callback_fn                         llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #<!-- -->10 SelectionTree::createEach           clangd/Selection.cpp:1082
    #<!-- -->11 findType                            clangd/XRefs.cpp:2215
    #<!-- -->12 operator()  (ClangdServer lambda)   clangd/ClangdServer.cpp:990
    ...
    [AST worker thread]
```

**ASAN classification**: `SourceAv` / `NOT_EXPLOITABLE` — read access violation at address `0x8` (null pointer + field offset).

---


</details>


---

