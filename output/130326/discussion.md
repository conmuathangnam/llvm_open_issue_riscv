# Coroutine miscompile: parameter destroyed twice(?) at -O1

**Author:** zmodem
**URL:** https://github.com/llvm/llvm-project/issues/130326
**Status:** Closed
**Labels:** miscompilation, coroutines
**Closed Date:** 2025-03-27T09:23:31Z

## Body

I'm not actually sure that the parameter is destroyed twice, but that's how the bug first appeared.

Consider:

```
#include <coroutine>
#include <utility>
#include <stdio.h>

// Boilerplate based on https://theshoemaker.de/posts/yet-another-cpp-coroutine-tutorial
class Task { 
public:
    struct FinalAwaiter {
        bool await_ready() const noexcept { return false; }
        template <typename P> auto await_suspend(std::coroutine_handle<P> handle) noexcept {
            return handle.promise().continuation;
        }
        void await_resume() const noexcept { }
    };

    struct Promise {
        std::coroutine_handle<> continuation;
        Task get_return_object() {
            return Task { std::coroutine_handle<Promise>::from_promise(*this) };
        }
        void unhandled_exception() noexcept { }
        void return_void() noexcept { }
        std::suspend_always initial_suspend() noexcept { return {}; }
        FinalAwaiter final_suspend() noexcept { return {}; }
    };
    using promise_type = Promise;

    Task() = default;
    ~Task() { if (handle_) { handle_.destroy(); } }

    struct Awaiter {
        std::coroutine_handle<Promise> handle;
        bool await_ready() const noexcept { return !handle || handle.done(); }
        auto await_suspend(std::coroutine_handle<> calling) noexcept {
            handle.promise().continuation = calling;
            return handle;
        }
        void await_resume() const noexcept { }
    };

    auto operator co_await() noexcept { return Awaiter{handle_}; }
    void run() {
      handle_.promise().continuation = std::noop_coroutine();
      handle_.resume();
    }

private:
    explicit Task(std::coroutine_handle<Promise> handle) : handle_(handle) { }
    std::coroutine_handle<Promise> handle_;
};

// The interesting part starts here.
namespace {
struct Cleanup {
  Cleanup() : is_engaged(true) {}
  Cleanup(Cleanup&& other) { is_engaged = true; other.is_engaged = false; }
  ~Cleanup() { if (is_engaged) { printf("CLEANUP\n"); } }
  bool is_engaged;
};
}

Task hello(Cleanup c) {
  printf("HELLO\n");
  co_return;
}

Task foo(Cleanup c) { co_await hello(std::move(c)); }

int main() {
  Task t = foo(Cleanup());
  t.run();
}

$ clang++ /tmp/a.cc -std=c++20 -O0 && ./a.out
HELLO
CLEANUP

$ clang++ /tmp/a.cc -std=c++20 -O1 && ./a.out
HELLO
CLEANUP
CLEANUP
```

The expectation is that there is only one "engaged" Cleanup object, and so we should only see that printed once.

The anonymous namespace seems to be an important factor.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-coroutines

Author: Hans Wennborg (zmodem)

<details>
I'm not actually sure that the parameter is destroyed twice, but that's how the bug first appeared.

Consider:

```
#include &lt;coroutine&gt;
#include &lt;utility&gt;
#include &lt;stdio.h&gt;

// Boilerplate based on https://theshoemaker.de/posts/yet-another-cpp-coroutine-tutorial
class Task { 
public:
    struct FinalAwaiter {
        bool await_ready() const noexcept { return false; }
        template &lt;typename P&gt; auto await_suspend(std::coroutine_handle&lt;P&gt; handle) noexcept {
            return handle.promise().continuation;
        }
        void await_resume() const noexcept { }
    };

    struct Promise {
        std::coroutine_handle&lt;&gt; continuation;
        Task get_return_object() {
            return Task { std::coroutine_handle&lt;Promise&gt;::from_promise(*this) };
        }
        void unhandled_exception() noexcept { }
        void return_void() noexcept { }
        std::suspend_always initial_suspend() noexcept { return {}; }
        FinalAwaiter final_suspend() noexcept { return {}; }
    };
    using promise_type = Promise;

    Task() = default;
    ~Task() { if (handle_) { handle_.destroy(); } }

    struct Awaiter {
        std::coroutine_handle&lt;Promise&gt; handle;
        bool await_ready() const noexcept { return !handle || handle.done(); }
        auto await_suspend(std::coroutine_handle&lt;&gt; calling) noexcept {
            handle.promise().continuation = calling;
            return handle;
        }
        void await_resume() const noexcept { }
    };

    auto operator co_await() noexcept { return Awaiter{handle_}; }
    void run() {
      handle_.promise().continuation = std::noop_coroutine();
      handle_.resume();
    }

private:
    explicit Task(std::coroutine_handle&lt;Promise&gt; handle) : handle_(handle) { }
    std::coroutine_handle&lt;Promise&gt; handle_;
};

// The interesting part starts here.
namespace {
struct Cleanup {
  Cleanup() : is_engaged(true) {}
  Cleanup(Cleanup&amp;&amp; other) { is_engaged = true; other.is_engaged = false; }
  ~Cleanup() { if (is_engaged) { printf("CLEANUP\n"); } }
  bool is_engaged;
};
}

Task hello(Cleanup c) {
  printf("HELLO\n");
  co_return;
}

Task foo(Cleanup c) { co_await hello(std::move(c)); }

int main() {
  Task t = foo(Cleanup());
  t.run();
}

$ clang++ /tmp/a.cc -std=c++20 -O0 &amp;&amp; ./a.out
HELLO
CLEANUP

$ clang++ /tmp/a.cc -std=c++20 -O1 &amp;&amp; ./a.out
HELLO
CLEANUP
CLEANUP
```

The expectation is that there is only one "engaged" Cleanup object, and so we should only see that printed once.

The anonymous namespace seems to be an important factor.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Hans Wennborg (zmodem)

<details>
I'm not actually sure that the parameter is destroyed twice, but that's how the bug first appeared.

Consider:

```
#include &lt;coroutine&gt;
#include &lt;utility&gt;
#include &lt;stdio.h&gt;

// Boilerplate based on https://theshoemaker.de/posts/yet-another-cpp-coroutine-tutorial
class Task { 
public:
    struct FinalAwaiter {
        bool await_ready() const noexcept { return false; }
        template &lt;typename P&gt; auto await_suspend(std::coroutine_handle&lt;P&gt; handle) noexcept {
            return handle.promise().continuation;
        }
        void await_resume() const noexcept { }
    };

    struct Promise {
        std::coroutine_handle&lt;&gt; continuation;
        Task get_return_object() {
            return Task { std::coroutine_handle&lt;Promise&gt;::from_promise(*this) };
        }
        void unhandled_exception() noexcept { }
        void return_void() noexcept { }
        std::suspend_always initial_suspend() noexcept { return {}; }
        FinalAwaiter final_suspend() noexcept { return {}; }
    };
    using promise_type = Promise;

    Task() = default;
    ~Task() { if (handle_) { handle_.destroy(); } }

    struct Awaiter {
        std::coroutine_handle&lt;Promise&gt; handle;
        bool await_ready() const noexcept { return !handle || handle.done(); }
        auto await_suspend(std::coroutine_handle&lt;&gt; calling) noexcept {
            handle.promise().continuation = calling;
            return handle;
        }
        void await_resume() const noexcept { }
    };

    auto operator co_await() noexcept { return Awaiter{handle_}; }
    void run() {
      handle_.promise().continuation = std::noop_coroutine();
      handle_.resume();
    }

private:
    explicit Task(std::coroutine_handle&lt;Promise&gt; handle) : handle_(handle) { }
    std::coroutine_handle&lt;Promise&gt; handle_;
};

// The interesting part starts here.
namespace {
struct Cleanup {
  Cleanup() : is_engaged(true) {}
  Cleanup(Cleanup&amp;&amp; other) { is_engaged = true; other.is_engaged = false; }
  ~Cleanup() { if (is_engaged) { printf("CLEANUP\n"); } }
  bool is_engaged;
};
}

Task hello(Cleanup c) {
  printf("HELLO\n");
  co_return;
}

Task foo(Cleanup c) { co_await hello(std::move(c)); }

int main() {
  Task t = foo(Cleanup());
  t.run();
}

$ clang++ /tmp/a.cc -std=c++20 -O0 &amp;&amp; ./a.out
HELLO
CLEANUP

$ clang++ /tmp/a.cc -std=c++20 -O1 &amp;&amp; ./a.out
HELLO
CLEANUP
CLEANUP
```

The expectation is that there is only one "engaged" Cleanup object, and so we should only see that printed once.

The anonymous namespace seems to be an important factor.
</details>


---

### Comment 3 - zmodem

(Mostly dumping notes to myself..)

> The anonymous namespace seems to be an important factor.

It was at some point during the test case reduction, but not anymore it seems.

---

Attached is the test case from above with a bunch of noinline and optnone attributes on the uninteresting parts:  [a.cc.txt](https://github.com/user-attachments/files/19448086/a.cc.txt)
 Marking `Cleanup::is_engaged` volatile "fixes" the test, and is hopefully a small enough change to diff the IR on.

Good:

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -DVOLATILE && ./a.out
HELLO
CLEANUP
```

Bad:

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions && ./a.out
HELLO
CLEANUP
CLEANUP
```

---

Where exactly are the print calls coming from?

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -g && gdb ./a.out
(gdb) b puts
(gdb) r
Breakpoint 1, __GI__IO_puts (str=0x55555555600b "CLEANUP") at ./libio/ioputs.c:35
warning: 35     ./libio/ioputs.c: No such file or directory
(gdb) bt
#0  __GI__IO_puts (str=0x55555555600b "CLEANUP") at ./libio/ioputs.c:35
#1  0x0000555555555541 in hello (c=...) at /tmp/a.cc:65
#2  0x00005555555556c8 in foo (c=...) at /tmp/a.cc:70
#3  0x0000555555555371 in main () at /tmp/a.cc:74

(gdb) c
Breakpoint 1, __GI__IO_puts (str=0x55555555600b "CLEANUP") at ./libio/ioputs.c:35
warning: 35     ./libio/ioputs.c: No such file or directory
(gdb) bt
#0  __GI__IO_puts (str=0x55555555600b "CLEANUP") at ./libio/ioputs.c:35
#1  0x0000555555555787 in Cleanup::~Cleanup (this=<optimized out>) at /tmp/a.cc:57
#2  foo (c=...) at /tmp/a.cc:70
#3  0x000055555555537b in main () at /tmp/a.cc:75
```

The first call is as expected: the hello coroutine is destructing its copy of the Cleanup argument, which is "engaged". The second is not: there should only be one "engaged" Cleanup object. Looking closer at that call:

```
(gdb) up
(gdb) disas
Dump of assembler code for function _Z3foo7Cleanup:
   0x0000555555555750 <+0>:     push   %rbx
   0x0000555555555751 <+1>:     mov    %rdi,%rbx
   0x0000555555555754 <+4>:     cmpq   $0x0,(%rdi)
   0x0000555555555758 <+8>:     je     0x55555555577b <_Z3foo7Cleanup+43>
   0x000055555555575a <+10>:    cmpb   $0x0,0x28(%rbx)
   0x000055555555575e <+14>:    je     0x55555555577b <_Z3foo7Cleanup+43>
   0x0000555555555760 <+16>:    lea    0x20(%rbx),%rdi
   0x0000555555555764 <+20>:    call   0x5555555558d0 <_ZN4TaskD2Ev>
   0x0000555555555769 <+25>:    cmpb   $0x1,0x29(%rbx)
   0x000055555555576d <+29>:    jne    0x55555555577b <_Z3foo7Cleanup+43>
   0x000055555555576f <+31>:    lea    0x895(%rip),%rdi        # 0x55555555600b
   0x0000555555555776 <+38>:    call   0x555555555060 <puts@plt>
   0x000055555555577b <+43>:    lea    0x889(%rip),%rdi        # 0x55555555600b
   0x0000555555555782 <+50>:    call   0x555555555060 <puts@plt>
=> 0x0000555555555787 <+55>:    mov    $0x30,%esi
   0x000055555555578c <+60>:    mov    %rbx,%rdi
   0x000055555555578f <+63>:    pop    %rbx
   0x0000555555555790 <+64>:    jmp    0x555555555050 <_ZdlPvm@plt>
```

GDB's demangling is lying: that function is actually `_Z3foo7Cleanup.destroy`.

Let's step through to see which path it takes:

```
(gdb) b _Z3foo7Cleanup.destroy
(gdb) r
Breakpoint 1, Cleanup::~Cleanup (this=0x0) at /tmp/a.cc:57
57        ~Cleanup() { if (is_engaged) { printf("CLEANUP\n"); } }
(gdb) disas
Dump of assembler code for function _Z3foo7Cleanup:
   0x0000555555555750 <+0>:     push   %rbx
   0x0000555555555751 <+1>:     mov    %rdi,%rbx
   0x0000555555555754 <+4>:     cmpq   $0x0,(%rdi)
=> 0x0000555555555758 <+8>:     je     0x55555555577b <_Z3foo7Cleanup+43>
   0x000055555555575a <+10>:    cmpb   $0x0,0x28(%rbx)
   0x000055555555575e <+14>:    je     0x55555555577b <_Z3foo7Cleanup+43>
   0x0000555555555760 <+16>:    lea    0x20(%rbx),%rdi
   0x0000555555555764 <+20>:    call   0x5555555558d0 <_ZN4TaskD2Ev>
   0x0000555555555769 <+25>:    cmpb   $0x1,0x29(%rbx)
   0x000055555555576d <+29>:    jne    0x55555555577b <_Z3foo7Cleanup+43>
   0x000055555555576f <+31>:    lea    0x895(%rip),%rdi        # 0x55555555600b
   0x0000555555555776 <+38>:    call   0x555555555060 <puts@plt>
   0x000055555555577b <+43>:    lea    0x889(%rip),%rdi        # 0x55555555600b
   0x0000555555555782 <+50>:    call   0x555555555060 <puts@plt>
   0x0000555555555787 <+55>:    mov    $0x30,%esi
   0x000055555555578c <+60>:    mov    %rbx,%rdi
   0x000055555555578f <+63>:    pop    %rbx
   0x0000555555555790 <+64>:    jmp    0x555555555050 <_ZdlPvm@plt>
(gdb) display/i $pc
(gdb) stepi
=> 0x55555555577b <_Z3foo7Cleanup+43>:  lea    0x889(%rip),%rdi        # 0x55555555600b
(gdb) stepi
=> 0x555555555782 <_Z3foo7Cleanup+50>:  call   0x555555555060 <puts@plt>
```

Okay, so `(%rdi)` was zero and it jumped straight to the second puts call.

---

In any case, `_Z3foo7Cleanup.destroy` looks wrong since it will always call puts one or two times. It should be possible to call it zero times.

Each puts call corresponds to a Cleanup object. There are three at play in the `foo` coroutine:

- the argument, `%c`, who's destructor will be run by `main`
- the "coroutine-local copy" of `%c`, `%c1`. This will be destructed by the coroutine, and may call puts depending on its state. After SROA, that state is maintained implicitly: it has no alloca, rather its value is decided by phi nodes.
- the argument passed to hello, `%agg.tmp`

The .destroy function is responsible for destroying the last two. But it needs to check or deduce their "engaged" state to decide if it should call puts. The IR looks like:

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -fno-discard-value-names -S -emit-llvm -o - | less

define internal fastcc void @_Z3foo7Cleanup.destroy(ptr noundef nonnull align 8 dereferenceable(48) %0) #5 {
entry.destroy:
  %index.addr = getelementptr inbounds nuw i8, ptr %0, i64 40
  %index = load i2, ptr %index.addr, align 8
  %1 = load ptr, ptr %0, align 8
  %2 = icmp eq ptr %1, null
  %switch = icmp eq i2 %index, 0
  %or.cond = select i1 %2, i1 true, i1 %switch
  br i1 %or.cond, label %if.then.i33, label %cleanup8

cleanup8:                                         ; preds = %entry.destroy
  %agg.tmp.reload.addr = getelementptr inbounds nuw i8, ptr %0, i64 41
  %ref.tmp4.reload.addr = getelementptr inbounds nuw i8, ptr %0, i64 32
  tail call void @_ZN4TaskD2Ev(ptr noundef nonnull align 8 dereferenceable(8) %ref.tmp4.reload.addr) #11
  %3 = load i8, ptr %agg.tmp.reload.addr, align 1, !tbaa !10, !range !16, !noundef !17
  %loadedv.i28 = trunc nuw i8 %3 to i1
  br i1 %loadedv.i28, label %if.then.i29, label %if.then.i33

if.then.i29:                                      ; preds = %cleanup8
  %puts.i30 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  br label %if.then.i33

if.then.i33:                                      ; preds = %if.then.i29, %cleanup8, %entry.destroy
  %puts.i34 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @_ZdlPvm(ptr noundef nonnull %0, i64 noundef 48) #11
  ret void
}
```

We can see the check of  `%agg.tmp` in `%cleanup8`, which I think corresponds to if the coroutine suspended after calling `hello` and didn't resume after that (the Task returned by foo is also destroyed).

But there is no check for `%c1`. So where did that get lost?

---

Following the .destory function, I think that check has been folded away already from the start:

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -fno-discard-value-names -mllvm -print-before-all -mllvm -filter-print-funcs=_Z3foo7Cleanup.destroy 2>&1 | less

; *** IR Dump Before InlinerPass on (_Z3foo7Cleanup.destroy) ***
; Function Attrs: mustprogress noinline nounwind uwtable
define internal fastcc void @_Z3foo7Cleanup.destroy(ptr noundef nonnull align 8 dereferenceable(48) %0) #8 {
entry.destroy:
  %ref.tmp3.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 3
  %ref.tmp4.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 4
  %ref.tmp.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 3
  %agg.tmp.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 6
  %ref.tmp14.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 3
  %__promise.reload.addr = getelementptr inbounds %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 2
  br label %resume.entry
[..]
resume.entry:                                     ; preds = %entry.destroy
  %index.addr = getelementptr inbounds nuw %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 5
  %index = load i2, ptr %index.addr, align 1
  %ResumeFn.addr1 = getelementptr inbounds nuw %_Z3foo7Cleanup.Frame, ptr %0, i32 0, i32 0
  %7 = load ptr, ptr %ResumeFn.addr1, align 8   
  %8 = icmp eq ptr %7, null
  br i1 %8, label %resume.2, label %Switch
[..]
(%7 is null in our case, so we proceed to)
esume.2:                                         ; preds = %resume.entry
  br label %resume.2.landing
resume.2.landing:                                 ; preds = %resume.2
  br label %AfterCoroSuspend43
[..]
AfterCoroSuspend43:                               ; preds = %resume.2.landing
  switch i8 1, label %coro.ret [
    i8 0, label %final.ready
    i8 1, label %cleanup17
  ]
[..]
cleanup17:                                        ; preds = %final.ready, %AfterCoroSuspend43
  br label %cleanup20

cleanup20:                                        ; preds = %cleanup17, %_ZN7CleanupD2Ev.exit31, %cleanup
  %loadedv.i32 = trunc nuw i8 1 to i1
  br i1 %loadedv.i32, label %if.then.i33, label %_ZN7CleanupD2Ev.exit35

if.then.i33:                                      ; preds = %cleanup20
  %puts.i34 = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  br label %_ZN7CleanupD2Ev.exit35
```

I think that constant 1 in `%loadedv.i32 = trunc nuw i8 1 to i1` was not a constant in the pre-split coroutine..

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -fno-discard-value-names -mllvm -print-before=coro-split -mllvm -filter-print-funcs=_Z3foo7Cleanup 2>&1 | less

[...]
define dso_local void @_Z3foo7Cleanup(ptr dead_on_unwind writable sret(%class.Task) align 8 %agg.result, ptr noundef %c) #15 {
entry:
  %__promise = alloca %"struct.Task::Promise", align 8
  %ref.tmp = alloca %"struct.std::__n4861::suspend_always", align 1
  %ref.tmp3 = alloca %"struct.Task::Awaiter", align 8
  %ref.tmp4 = alloca %class.Task, align 8
  %agg.tmp = alloca %struct.Cleanup, align 1
  %ref.tmp14 = alloca %"struct.Task::FinalAwaiter", align 1
  %0 = call token @llvm.coro.id(i32 16, ptr nonnull %__promise, ptr nonnull @_Z3foo7Cleanup, ptr null)
  %1 = call i1 @llvm.coro.alloc(token %0)
  br i1 %1, label %coro.alloc, label %coro.init

coro.alloc:                                       ; preds = %entry
  %2 = call i64 @llvm.coro.size.i64()
  %call = call noalias noundef nonnull ptr @_Znwm(i64 noundef %2) #20
  br label %coro.init

coro.init:                                        ; preds = %coro.alloc, %entry
  %3 = phi ptr [ null, %entry ], [ %call, %coro.alloc ]
  %4 = call ptr @llvm.coro.begin(token %0, ptr %3) #21
  store i8 0, ptr %c, align 1, !tbaa !5
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %__promise) #2
  store ptr null, ptr %__promise, align 8, !tbaa !10
  call void @_ZN4Task7Promise17get_return_objectEv(ptr dead_on_unwind writable sret(%class.Task) align 8 %agg.result, ptr noundef nonnull align 8 dereferenceable(8) %__promise)
  call void @llvm.lifetime.start.p0(i64 1, ptr nonnull %ref.tmp) #2
  call void @_ZN4Task7Promise15initial_suspendEv(ptr noundef nonnull align 8 dereferenceable(8) %__promise) #2
  %5 = call token @llvm.coro.save(ptr null)
  call void @llvm.coro.await.suspend.void(ptr nonnull %ref.tmp, ptr %4, ptr nonnull @_Z3foo7Cleanup.__await_suspend_wrapper__init) #2
  %6 = call i8 @llvm.coro.suspend(token %5, i1 false)
  switch i8 %6, label %coro.ret [
    i8 0, label %init.ready
    i8 1, label %cleanup
  ]
[...]

cleanup17:                                        ; preds = %final.suspend, %final.ready
  call void @llvm.lifetime.end.p0(i64 1, ptr nonnull %ref.tmp14) #2
  br label %cleanup20

cleanup20:                                        ; preds = %_ZN7CleanupD2Ev.exit31, %cleanup, %cleanup17
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %__promise) #2
  %loadedv.i32 = trunc nuw i8 %6 to i1
  br i1 %loadedv.i32, label %if.then.i33, label %_ZN7CleanupD2Ev.exit35
```

Hmm, that doesn't seem right. I think `%loadedv.i32` has been oversimplified there already?

---

```
$ build/bin/clang++ /tmp/a.cc -std=c++20 -O1 -fno-exceptions -fno-discard-value-names -mllvm -print-after-all -mllvm -filter-print-funcs=_Z3foo7Cleanup 2>&1 | less
[..]
; *** IR Dump After InlinerPass on (_Z3foo7Cleanup) ***
[..]
cleanup20:                                        ; preds = %_ZN7CleanupD2Ev.exit31, %cleanup, %cleanup17
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %__promise) #2
  %13 = load i8, ptr %c1, align 1, !tbaa !5, !range !13, !noundef !14
  %loadedv.i32 = trunc nuw i8 %13 to i1
  br i1 %loadedv.i32, label %if.then.i33, label %_ZN7CleanupD2Ev.exit35
[..]

; *** IR Dump After SROAPass on _Z3foo7Cleanup ***
[..]
cleanup20:                                        ; preds = %_ZN7CleanupD2Ev.exit31, %cleanup, %cleanup17
  %c1.sroa.0.0 = phi i8 [ 0, %cleanup17 ], [ 0, %_ZN7CleanupD2Ev.exit31 ], [ 1, %cleanup ]
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %__promise) #2
  %loadedv.i32 = trunc nuw i8 %c1.sroa.0.0 to i1
  br i1 %loadedv.i32, label %if.then.i33, label %_ZN7CleanupD2Ev.exit35
```

Okay, so `%loadedv.i32` is 1 if we came from `%cleanup`, which we do if `%6 = call i8 @llvm.coro.suspend` was 1, so the simplification to `trunc nuw i8 %6 to i1` seems reasonable. It happens here:

```
[...]
; *** IR Dump After InstCombinePass on _Z3foo7Cleanup ***
[...]
cleanup20:                                        ; preds = %_ZN7CleanupD2Ev.exit31, %cleanup, %cleanup17
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %__promise) #2
  %loadedv.i32 = trunc nuw i8 %6 to i1
  br i1 %loadedv.i32, label %if.then.i33, label %_ZN7CleanupD2Ev.exit35
```

My head hurts a little, but I think we've arrived at the heart of the problem. I'm not sure that transform is sound when combined with coro splitting:

In our test program, it is true that the first `@llvm.coro.suspend` returned one, but when we eventually arrive at `%cleanup20` it will be after resuming, and we do not arrive there via the `%cleanup` block.

---

### Comment 4 - zmodem

I believe the instcombine in question is `simplifyUsingControlFlow`

---

### Comment 5 - zmodem

> I believe the instcombine in question is `simplifyUsingControlFlow`

Confirmed that the diff below fixes the test case above. More importantly, it also fixes the original test which sparked this bug:

```
$ git diff
diff --git a/llvm/lib/Transforms/InstCombine/InstCombinePHI.cpp b/llvm/lib/Transforms/InstCombine/InstCombinePHI.cpp
index 80308bf92dbb..9ff26316c6ec 100644
--- a/llvm/lib/Transforms/InstCombine/InstCombinePHI.cpp
+++ b/llvm/lib/Transforms/InstCombine/InstCombinePHI.cpp
@@ -1290,6 +1290,10 @@ static Value *simplifyUsingControlFlow(InstCombiner &Self, PHINode &PN,
   //         ...      ...
   //          \       /
   //       phi [v1] [v2]
+
+  // XXX: Avoid presplit coroutines.
+  if (PN.getParent()->getParent()->isPresplitCoroutine()) return nullptr;
+
   // Make sure all inputs are constants.
   if (!all_of(PN.operands(), [](Value *V) { return isa<ConstantInt>(V); }))
     return nullptr;
```



---

### Comment 6 - zmodem

The problem in our case is the interaction between simplifyUsingControlFlow and llvm.coro.suspend(), but the issue is more general. Here's an example:

```
  %s = call @llvm.coro.suspend(...)
             |
       switch (%s)
case v1: /       \ case v2:
        ...      ...
         |       suspend point
         |       ...
         \       /
 %x = phi [v1] [v2]
             |
            ...
             |
         use of %x

```

The instcombine transformation will realize that the phi can be replaced with `%s` directly.

However, `%s` does not behave like a regular value. Corosplit will substitute different values for the return value of coro.suspend() when splitting the function. So even though `%s` had a particular value when control actually passed through the switch, it may have a *different* value when we reach the `use of %x` part, which instcombine has turned into `use of %s`.

That's a problem for *any* use of the return value of this intrinsic that crosses suspend points, not just this instcombine.

Preserving values across suspend points is what the coroutine frame is for. So why isn't `%s` spilled and reloaded from the frame? Because it's explicitly excluded here: https://github.com/llvm/llvm-project/blob/fed4727187e73c1b61bc65c63a66de58318379fc/llvm/lib/Transforms/Coroutines/SpillUtils.cpp#L27-L32

I believe that is the root cause of this bug.

The suspend intrinsic was added to that list as part of a263a60ad5c6b299c23c86c440c69a3ff98bbcd5. It's not really clear if that was intentional, or just kind of happened as part of refactoring the code by extracting `isCoroutineStructureIntrinsic()` to a separate function. The added test still passes if I remove the suspend intrinsic from there.

In fact, *all* tests pass after that change[^1]. I believe that will be the fix; it just needs a nice test case :)

[^1]: Except `coro-materialize.ll` but that's because of a pre-existing bug in the test which was now exposed.

---

### Comment 7 - rnk

I think the key finding here is that optimizations are allowed to introduce new uses of any SSA value (should be obvious, but still surprising). These cleanup booleans are essentially extra flags carrying the same information as the coro.suspend return value, which the optimizer realizes, and it cleans up the extra phis and control flow to reuse the coro.suspend switch input.

The presplit coroutine IR from the frontend doesn't use these values across suspend points so one might thing they don't have to be saved, but to be sound, it seems like they must be saved.

Nice work! =D

---

