# llvm.wasm.throw is inconsistently considered non-unwinding or invocable

**Author:** purplesyringa
**URL:** https://github.com/llvm/llvm-project/issues/124710
**Status:** Closed
**Labels:** backend:WebAssembly, clang:codegen, miscompilation
**Closed Date:** 2025-02-25T22:12:36Z

## Body

1. Miscompiled:

```cpp
void loud();

void throw_and_catch(void *ex) {
    try {
        __builtin_wasm_throw(0, ex);
    } catch (...) {
        loud();
    }
}
```

The generated IR is

```llvm
define hidden void @throw_and_catch(void*)(ptr noundef %ex) {
entry:
  %ex.addr = alloca ptr, align 4
  store ptr %ex, ptr %ex.addr, align 4
  %0 = load ptr, ptr %ex.addr, align 4
  call void @llvm.wasm.throw(i32 0, ptr %0)
  ret void
}
```

without a catchpad.

https://godbolt.org/z/Po668Yxaz

2. Broken IR:

```cpp
void loud();

void my_throw(void *ex) {
    __builtin_wasm_throw(0, ex);
}

void throw_and_catch(void *ex) {
    try {
        my_throw(ex);
    } catch (...) {
        loud();
    }
}
```

InlinerPass replaces

```llvm
  invoke void @my_throw(void*)(ptr noundef %ex)
          to label %try.cont unwind label %catch.dispatch
```

(correct, working) with

```llvm
  invoke void @llvm.wasm.throw(i32 0, ptr %ex)
          to label %.noexc unwind label %catch.dispatch
```

(broken, `llvm.wasm.throw` cannot be invoked), which halts the backend.

https://godbolt.org/z/rMEd1x79z

As far as I'm aware, `llvm.wasm.throw` is the only function that can unwind but cannot be invoked, making this troublesome for external frontends like rustc, not just for optimization passes. Ideally it would become invocable.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Alisa Sireneva (purplesyringa)

<details>
1. Miscompiled:

```cpp
void loud();

void throw_and_catch(void *ex) {
    try {
        __builtin_wasm_throw(0, ex);
    } catch (...) {
        loud();
    }
}
```

The generated IR is

```llvm
define hidden void @<!-- -->throw_and_catch(void*)(ptr noundef %ex) {
entry:
  %ex.addr = alloca ptr, align 4
  store ptr %ex, ptr %ex.addr, align 4
  %0 = load ptr, ptr %ex.addr, align 4
  call void @<!-- -->llvm.wasm.throw(i32 0, ptr %0)
  ret void
}
```

without a catchpad.

https://godbolt.org/z/Po668Yxaz

2. Broken IR:

```cpp
void loud();

void my_throw(void *ex) {
    __builtin_wasm_throw(0, ex);
}

void throw_and_catch(void *ex) {
    try {
        my_throw(ex);
    } catch (...) {
        loud();
    }
}
```

InlinerPass replaces

```llvm
  invoke void @<!-- -->my_throw(void*)(ptr noundef %ex)
          to label %try.cont unwind label %catch.dispatch
```

(correct, working) with

```llvm
  invoke void @<!-- -->llvm.wasm.throw(i32 0, ptr %ex)
          to label %.noexc unwind label %catch.dispatch
```

(broken, `llvm.wasm.throw` cannot be invoked), which halts the backend.

https://godbolt.org/z/rMEd1x79z

As far as I'm aware, `llvm.wasm.throw` is the only function that can unwind but cannot be invoked, making this troublesome for external frontends like rustc, not just for optimization passes. Ideally it would become invocable.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Alisa Sireneva (purplesyringa)

<details>
1. Miscompiled:

```cpp
void loud();

void throw_and_catch(void *ex) {
    try {
        __builtin_wasm_throw(0, ex);
    } catch (...) {
        loud();
    }
}
```

The generated IR is

```llvm
define hidden void @<!-- -->throw_and_catch(void*)(ptr noundef %ex) {
entry:
  %ex.addr = alloca ptr, align 4
  store ptr %ex, ptr %ex.addr, align 4
  %0 = load ptr, ptr %ex.addr, align 4
  call void @<!-- -->llvm.wasm.throw(i32 0, ptr %0)
  ret void
}
```

without a catchpad.

https://godbolt.org/z/Po668Yxaz

2. Broken IR:

```cpp
void loud();

void my_throw(void *ex) {
    __builtin_wasm_throw(0, ex);
}

void throw_and_catch(void *ex) {
    try {
        my_throw(ex);
    } catch (...) {
        loud();
    }
}
```

InlinerPass replaces

```llvm
  invoke void @<!-- -->my_throw(void*)(ptr noundef %ex)
          to label %try.cont unwind label %catch.dispatch
```

(correct, working) with

```llvm
  invoke void @<!-- -->llvm.wasm.throw(i32 0, ptr %ex)
          to label %.noexc unwind label %catch.dispatch
```

(broken, `llvm.wasm.throw` cannot be invoked), which halts the backend.

https://godbolt.org/z/rMEd1x79z

As far as I'm aware, `llvm.wasm.throw` is the only function that can unwind but cannot be invoked, making this troublesome for external frontends like rustc, not just for optimization passes. Ideally it would become invocable.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Alisa Sireneva (purplesyringa)

<details>
1. Miscompiled:

```cpp
void loud();

void throw_and_catch(void *ex) {
    try {
        __builtin_wasm_throw(0, ex);
    } catch (...) {
        loud();
    }
}
```

The generated IR is

```llvm
define hidden void @<!-- -->throw_and_catch(void*)(ptr noundef %ex) {
entry:
  %ex.addr = alloca ptr, align 4
  store ptr %ex, ptr %ex.addr, align 4
  %0 = load ptr, ptr %ex.addr, align 4
  call void @<!-- -->llvm.wasm.throw(i32 0, ptr %0)
  ret void
}
```

without a catchpad.

https://godbolt.org/z/Po668Yxaz

2. Broken IR:

```cpp
void loud();

void my_throw(void *ex) {
    __builtin_wasm_throw(0, ex);
}

void throw_and_catch(void *ex) {
    try {
        my_throw(ex);
    } catch (...) {
        loud();
    }
}
```

InlinerPass replaces

```llvm
  invoke void @<!-- -->my_throw(void*)(ptr noundef %ex)
          to label %try.cont unwind label %catch.dispatch
```

(correct, working) with

```llvm
  invoke void @<!-- -->llvm.wasm.throw(i32 0, ptr %ex)
          to label %.noexc unwind label %catch.dispatch
```

(broken, `llvm.wasm.throw` cannot be invoked), which halts the backend.

https://godbolt.org/z/rMEd1x79z

As far as I'm aware, `llvm.wasm.throw` is the only function that can unwind but cannot be invoked, making this troublesome for external frontends like rustc, not just for optimization passes. Ideally it would become invocable.
</details>


---

### Comment 4 - aheejin

(The fix needed both #128104 and #128105, and #128104 was a wasm backend problem)

---

### Comment 5 - purplesyringa

Thanks a lot! Really appreciate it.

---

