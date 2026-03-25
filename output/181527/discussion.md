# [WebAssembly] Crash in emitGlobalVariable with addrspace(1) globals due to missing Subtarget during doFinalization

**Author:** ParkHanbum
**URL:** https://github.com/llvm/llvm-project/issues/181527
**Status:** Open
**Labels:** backend:WebAssembly, crash

## Body

Summary

When compiling a WebAssembly module containing a global variable in addrspace(1) (Wasm Global), llc crashes with LLVM ERROR: Aggregate globals not yet implemented.

The error message is misleading. The crash occurs because emitGlobalVariable is called during doFinalization, where the Subtarget member is nullptr. This causes computeLegalValueVTs to be skipped, leaving the VTs vector empty, which triggers the assertion failure in wasmSymbolSetType.

Reproduction

1. Case where it fails (Bug):
Using addrspace(1) triggers the Wasm-specific global handling path.
```wasm

; crash.ll
target triple = "wasm32-unknown-unknown"

; A scalar global in Wasm global space (addrspace 1)
@wasm_val = addrspace(1) global i32 123
```

```Bash
llc -mtriple=wasm32-unknown-unknown crash.ll -o -
```
Output: LLVM ERROR: Aggregate globals not yet implemented

2. Case where it works (Normal):
Standard globals (addrspace 0) work fine because they are handled by the base AsmPrinter class and return early before reaching the buggy code.
```wasm

; works.ll
target triple = "wasm32-unknown-unknown"
@std_val = global i32 123
```
Root Cause Analysis 

The crash happens in WebAssemblyAsmPrinter::emitGlobalVariable. This function is called during doFinalization. At this stage, the AsmPrinter::Subtarget member is nullptr because there is no active MachineFunction.

The code checks if (Subtarget) to compute value types:
```cpp
    if (Subtarget) {
       // ... calls computeLegalValueVTs ...
    }
```

Since Subtarget is null, computeLegalValueVTs is skipped, and VTs remains empty (size 0).

WebAssembly::wasmSymbolSetType is called with an empty VTs.

Inside wasmSymbolSetType, it checks if (VTs.size() == 1). Since size is 0, it falls through to the else block and calls report_fatal_error("Aggregate globals not yet implemented").

Proposed Solution

If Subtarget is null (which is expected during doFinalization), we should retrieve the default subtarget from the TargetMachine (TM).



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: hanbeom (ParkHanbum)

<details>
Summary

When compiling a WebAssembly module containing a global variable in addrspace(1) (Wasm Global), llc crashes with LLVM ERROR: Aggregate globals not yet implemented.

The error message is misleading. The crash occurs because emitGlobalVariable is called during doFinalization, where the Subtarget member is nullptr. This causes computeLegalValueVTs to be skipped, leaving the VTs vector empty, which triggers the assertion failure in wasmSymbolSetType.

Reproduction

1. Case where it fails (Bug):
Using addrspace(1) triggers the Wasm-specific global handling path.
```wasm

; crash.ll
target triple = "wasm32-unknown-unknown"

; A scalar global in Wasm global space (addrspace 1)
@<!-- -->wasm_val = addrspace(1) global i32 123
```

```Bash
llc -mtriple=wasm32-unknown-unknown crash.ll -o -
```
Output: LLVM ERROR: Aggregate globals not yet implemented

2. Case where it works (Normal):
Standard globals (addrspace 0) work fine because they are handled by the base AsmPrinter class and return early before reaching the buggy code.
```wasm

; works.ll
target triple = "wasm32-unknown-unknown"
@<!-- -->std_val = global i32 123
```
Root Cause Analysis 

The crash happens in WebAssemblyAsmPrinter::emitGlobalVariable. This function is called during doFinalization. At this stage, the AsmPrinter::Subtarget member is nullptr because there is no active MachineFunction.

The code checks if (Subtarget) to compute value types:
```cpp
    if (Subtarget) {
       // ... calls computeLegalValueVTs ...
    }
```

Since Subtarget is null, computeLegalValueVTs is skipped, and VTs remains empty (size 0).

WebAssembly::wasmSymbolSetType is called with an empty VTs.

Inside wasmSymbolSetType, it checks if (VTs.size() == 1). Since size is 0, it falls through to the else block and calls report_fatal_error("Aggregate globals not yet implemented").

Proposed Solution

If Subtarget is null (which is expected during doFinalization), we should retrieve the default subtarget from the TargetMachine (TM).


</details>


---

