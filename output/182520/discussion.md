# [hexagon] "Unhandled HVX operation" in portable_simd

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/182520
**Status:** Closed
**Labels:** backend:Hexagon, release:backport, crash
**Closed Date:** 2026-02-25T20:10:48Z

## Body

LLVM crashes with "Unhandled HVX operation" when truncating a sub-HVX-width integer vector to a boolean vector in HVX 128-byte mode (e.g., `<64 x i8>` to `<64 x i1>`).

This issue was discovered when compiling Rust code using SIMD vector operations targeting Hexagon with HVX. The Rust code performed SIMD comparisons that produce boolean masks, which are then converted to bitmasks.

A minimal Rust reproducer:

```rust
#![feature(repr_simd, platform_intrinsics)]
#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_: &PanicInfo) -> ! { loop {} }

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct u8x64([u8; 64]);

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct i8x64([i8; 64]);

extern "platform-intrinsic" {
    fn simd_lt<T, U>(a: T, b: T) -> U;
    fn simd_bitmask<T, U>(a: T) -> U;
}

// This function triggers the crash
#[no_mangle]
pub fn compare_and_bitmask(a: u8x64, b: u8x64) -> u64 {
    unsafe {
        // Compare a < b, producing i8x64 of -1 or 0
        let mask: i8x64 = simd_lt(a, b);
        // Convert to bitmask - triggers trunc <64 x i8> to <64 x i1>
        simd_bitmask(mask)
    }
}
```

Build with cargo (requires nightly for `-Zbuild-std`):
```bash
# Setup
mkdir hvx_repro && cd hvx_repro
cargo init --lib
cp hvx_trunc_rust_repro.rs src/lib.rs
echo '[lib]
crate-type = ["staticlib"]' >> Cargo.toml

# Create .cargo/config.toml
mkdir -p .cargo
echo '[target.hexagon-unknown-linux-musl]
rustflags = ["-C", "target-feature=+hvx,+hvx-length128b"]' > .cargo/config.toml

# Build - this triggers the crash
cargo +nightly build --target=hexagon-unknown-linux-musl \
      -Zbuild-std=core --release
```

When compiled for Hexagon with HVX enabled, `simd_bitmask` generates LLVM IR that truncates a `<64 x i8>` vector to `<64 x i1>`, which triggers the crash.

## Symptom in Rust

```
error: could not compile `myproject` (lib)

Caused by:
  process didn't exit successfully: `rustc ...` (signal: 6, SIGABRT: process abort signal)

LLVM ERROR: Unhandled HVX operation
UNREACHABLE executed at HexagonISelLoweringHVX.cpp
```

## Minimal LLVM IR Reproducer

```llvm
; trunc-v64i8-to-v64i1.ll
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i64 @crash(<64 x i8> %v) #0 {
  %1 = trunc <64 x i8> %v to <64 x i1>
  %2 = bitcast <64 x i1> %1 to i64
  ret i64 %2
}

attributes #0 = { "target-features"="-small-data,+hvx,+hvx-length128b" }
```

## How to Reproduce

```bash
llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv73 \
    -mattr=+hvxv73,+hvx-length128b \
    trunc-v64i8-to-v64i1.ll -o /dev/null
```

## Crash Output

```
t5: v64i1 = truncate t4
  t4: v64i8 = extract_subvector t2, Constant:i32<0>
    t2: v128i8,ch = CopyFromReg t0, Register:v128i8 %0
      t0: ch,glue = EntryToken
Unhandled HVX operation
UNREACHABLE executed at llvm/lib/Target/Hexagon/HexagonISelLoweringHVX.cpp:3554!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv73 -mattr=+hvxv73,+hvx-length128b trunc-v64i8-to-v64i1.ll -o /dev/null
1.      Running pass 'Function Pass Manager' on module 'trunc-v64i8-to-v64i1.ll'.
2.      Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@crash'
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
LLVM crashes with "Unhandled HVX operation" when truncating a sub-HVX-width integer vector to a boolean vector in HVX 128-byte mode (e.g., `&lt;64 x i8&gt;` to `&lt;64 x i1&gt;`).

This issue was discovered when compiling Rust code using SIMD vector operations targeting Hexagon with HVX. The Rust code performed SIMD comparisons that produce boolean masks, which are then converted to bitmasks.

A minimal Rust reproducer:

```rust
#![feature(repr_simd, platform_intrinsics)]
#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_: &amp;PanicInfo) -&gt; ! { loop {} }

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct u8x64([u8; 64]);

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct i8x64([i8; 64]);

extern "platform-intrinsic" {
    fn simd_lt&lt;T, U&gt;(a: T, b: T) -&gt; U;
    fn simd_bitmask&lt;T, U&gt;(a: T) -&gt; U;
}

// This function triggers the crash
#[no_mangle]
pub fn compare_and_bitmask(a: u8x64, b: u8x64) -&gt; u64 {
    unsafe {
        // Compare a &lt; b, producing i8x64 of -1 or 0
        let mask: i8x64 = simd_lt(a, b);
        // Convert to bitmask - triggers trunc &lt;64 x i8&gt; to &lt;64 x i1&gt;
        simd_bitmask(mask)
    }
}
```

Build with cargo (requires nightly for `-Zbuild-std`):
```bash
# Setup
mkdir hvx_repro &amp;&amp; cd hvx_repro
cargo init --lib
cp hvx_trunc_rust_repro.rs src/lib.rs
echo '[lib]
crate-type = ["staticlib"]' &gt;&gt; Cargo.toml

# Create .cargo/config.toml
mkdir -p .cargo
echo '[target.hexagon-unknown-linux-musl]
rustflags = ["-C", "target-feature=+hvx,+hvx-length128b"]' &gt; .cargo/config.toml

# Build - this triggers the crash
cargo +nightly build --target=hexagon-unknown-linux-musl \
      -Zbuild-std=core --release
```

When compiled for Hexagon with HVX enabled, `simd_bitmask` generates LLVM IR that truncates a `&lt;64 x i8&gt;` vector to `&lt;64 x i1&gt;`, which triggers the crash.

## Symptom in Rust

```
error: could not compile `myproject` (lib)

Caused by:
  process didn't exit successfully: `rustc ...` (signal: 6, SIGABRT: process abort signal)

LLVM ERROR: Unhandled HVX operation
UNREACHABLE executed at HexagonISelLoweringHVX.cpp
```

## Minimal LLVM IR Reproducer

```llvm
; trunc-v64i8-to-v64i1.ll
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i64 @<!-- -->crash(&lt;64 x i8&gt; %v) #<!-- -->0 {
  %1 = trunc &lt;64 x i8&gt; %v to &lt;64 x i1&gt;
  %2 = bitcast &lt;64 x i1&gt; %1 to i64
  ret i64 %2
}

attributes #<!-- -->0 = { "target-features"="-small-data,+hvx,+hvx-length128b" }
```

## How to Reproduce

```bash
llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv73 \
    -mattr=+hvxv73,+hvx-length128b \
    trunc-v64i8-to-v64i1.ll -o /dev/null
```

## Crash Output

```
t5: v64i1 = truncate t4
  t4: v64i8 = extract_subvector t2, Constant:i32&lt;0&gt;
    t2: v128i8,ch = CopyFromReg t0, Register:v128i8 %0
      t0: ch,glue = EntryToken
Unhandled HVX operation
UNREACHABLE executed at llvm/lib/Target/Hexagon/HexagonISelLoweringHVX.cpp:3554!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv73 -mattr=+hvxv73,+hvx-length128b trunc-v64i8-to-v64i1.ll -o /dev/null
1.      Running pass 'Function Pass Manager' on module 'trunc-v64i8-to-v64i1.ll'.
2.      Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->crash'
```
</details>


---

### Comment 2 - androm3da

Fixed by #182528

---

### Comment 3 - aankit-ca

/cherry-pick 0c53a89b228a2c1b4d2d02cb80d674b2b20a9c7c

---

### Comment 4 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: [0c53a89](https://github.com/llvm/llvm-project/commit/0c53a89b228a2c1b4d2d02cb80d674b2b20a9c7c)

https://github.com/llvm/llvm-project/actions/runs/22413712178

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 5 - llvmbot

/pull-request llvm/llvm-project#183376

---

### Comment 6 - androm3da

Opened #185565 instead

---

