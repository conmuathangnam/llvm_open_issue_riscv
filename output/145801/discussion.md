# [Clang] Mixed use of VLAs and alloca leads to miscompilation (early pop of alloca memory)

**Author:** kkofler
**URL:** https://github.com/llvm/llvm-project/issues/145801
**Status:** Open
**Labels:** clang, miscompilation

## Body

In this (minimized, not linkable, use `clang -O2 -S allocatest.c` and look at the output to test) testcase:
```c
#include <alloca.h>

struct nlopt;

extern int n;
struct nlopt *create(void);
void f(struct nlopt *opt, double *x, void *userdata);
void solve(struct nlopt *opt);

int main(void)
{
  struct nlopt *opt = create();
  int i;
  {
    double x[n];
    for (i = 0; i < n; i++) {
      void *p = alloca(8);
      f(opt, x, p);
    }
  }
  solve(opt);
  return 0;
}
```
the `alloca` needs to live until the end of the function, but (as evidenced by the assembly output), Clang actually emits code to restore `rsp` to its value before the VLA creation at the point where the VLA goes out of scope, and that also ends up destroying the arrays from `alloca`.

In the real (much longer) code, the `solve` function (actually called `nlopt_optimize`) ends up using the pointers that were recorded in the `opt` structure by the function `f` (actually called `nlopt_add_inequality_constraint`), but due to the early pop, they will have been overwritten, leading to a crash.

I can reproduce this with all 3 of clang version 20.1.7, clang version 18.1.8, and with the ancient clang version 13.0.1, so this is a longstanding bug.

GCC gets this right: `gcc -O2 -S allocatest.c` shows that `rsp` is **not** restored until after `solve` is called (so the lifetime of the VLA also has to be extended, but there is not really a way around that in this case), and in fact, the real program also does not crash when compiled with GCC as it does when compiled with Clang.

## Comments

### Comment 1 - kkofler

Hmmmph, why does releases.llvm.org show me version 18.1.8 as the latest when the actual latest version here is 20.1.7?

---

### Comment 2 - kkofler

Bug still present with `clang version 20.1.7 (https://github.com/llvm/llvm-project 6146a88f60492b520a36f8f8f3231e15f3cc6082)`.

---

### Comment 3 - kkofler

GCC actually had a similar bug in the past, but it was fixed in 2015: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=14236

---

### Comment 4 - AaronBallman

CC @efriedma-quic @rjmccall 

My codegen-fu is not as strong as yours: is this happening because we call `llvm.stackrestore` in the `for.end` block? Is that basically restoring the saved stack for the VLA and the explicit alloca at the same time?

---

### Comment 5 - kkofler

The question is probably not directed to me, but I would guess so, yes. Though I am not familiar with the internals of LLVM. (I do have some experience with compiler internals though, having worked on a GCC port, see http://tigcc.ticalc.org/ .) So let us wait for the LLVM codegen experts to confirm.

---

### Comment 6 - nikic

Hm, is there anything we can do about this short of just never freeing VLAs if alloca() is used in the same function?

---

### Comment 7 - AaronBallman

> Hm, is there anything we can do about this short of just never freeing VLAs if alloca() is used in the same function?

I think we may run into similar problems when we get to the `defer` proposal because that will have similar stack allocation behavior to VLAs (block-scoped lifetime, not function-scoped like `alloca`). So maybe we need different LLVM IR for `alloca` vs block scoped stack allocations?

---

### Comment 8 - nikic

I don't think that different IR is going to help. We can't free allocations on the stack out of order, so as soon as an alloca occurs, we lose the ability to free anything that was allocated below it.

I kind of feel like using alloca in a function with VLAs should be considered ill-formed, as both of their requirements cannot be satisfied at the same time.

---

### Comment 9 - AaronBallman

> I don't think that different IR is going to help. We can't free allocations on the stack out of order, so as soon as an alloca occurs, we lose the ability to free anything that was allocated below it.

I was thinking we could play games with the stack to make that possible (e.g., allocas and VLAs grow different parts of the stack so there's not as strong of a relationship between the two). But I suppose a better question is: what did GCC do to address this?

> I kind of feel like using alloca in a function with VLAs should be considered ill-formed, as both of their requirements cannot be satisfied at the same time.

That would be tricky for the FE to diagnose reliably. Consider code where the `alloca` is conditionally controlled; if the `alloca` isn't called on a particular path, we'd be wrong to reject that code.

---

### Comment 10 - kkofler

What GCC does appears to be "just never freeing VLAs if alloca() is used in the same function", at least in any parts that can be reached from the alloca. That is sufficient to comply to the specs of alloca, and I am not convinced that it actually violates the constraints of VLAs. (Is it said anywhere that those **must** be popped at any specific point?)

Stack partitioning is a possibility, but is it worth it, when other compilers do not do it (so program(mer)s cannot rely on it being done anyway)?

---

### Comment 11 - AaronBallman

> What GCC does appears to be "just never freeing VLAs if alloca() is used in the same function", at least in any parts that can be reached from the alloca. That is sufficient to comply to the specs of alloca, and I am not convinced that it actually violates the constraints of VLAs. (Is it said anywhere that those **must** be popped at any specific point?)
> 
> Stack partitioning is a possibility, but is it worth it, when other compilers do not do it (so program(mer)s cannot rely on it being done anyway)?

The C standard has this to say about the lifetime of a VLA:

> For such an object that does have a variable length array type, its lifetime extends from the declaration of the object until execution of the program leaves the scope of the declaration. If the scope is entered recursively, a new instance of the object is created each time. The initial representation of the object is indeterminate.

So I think we're fine to extend the lifetime of the VLA to the function scope; it's UB to access it once the block containing the VLA is exited, but extending the lifetime in the presence of `alloca` doesn't change that. CC @uecker just in case I'm missing something subtle about VLAs.

---

### Comment 12 - efriedma-quic

The C standard technically doesn't require VLAs to be freed at any particular time... but if we don't use llvm.stacksave/llvm.stackrestore, we leak resources.

> That would be tricky for the FE to diagnose reliably. Consider code where the alloca is conditionally controlled; if the alloca isn't called on a particular path, we'd be wrong to reject that code.

I think if you have VLAs and alloca in the same function, you've basically already messed up, even if we can't prove the alloca is dynamically executed.

I don't mind copying gcc behavior here, but we should warn whenever we erase llvm.stackrestores.

---

### Comment 13 - uecker

 I think it should be ok to extend the lifetime of the VLA.

---

### Comment 14 - uecker

But destructors (cleanup) should probably not be delayed.

---

### Comment 15 - AaronBallman

> I think if you have VLAs and alloca in the same function, you've basically already messed up, even if we can't prove the alloca is dynamically executed.
> 
> I don't mind copying gcc behavior here, but we should warn whenever we erase llvm.stackrestores.

I think we could support a warning any time we see a VLA and a call to `alloca` or `__builtin_alloca`.

> I think it should be ok to extend the lifetime of the VLA.

Thanks, Martin!

> But destructors (cleanup) should probably not be delayed.

Agreed, we need to be careful with the VLA extension in C++ (and `__attribute__((cleanup))`).

---

### Comment 16 - rjmccall

> I was thinking we could play games with the stack to make that possible (e.g., allocas and VLAs grow different parts of the stack so there's not as strong of a relationship between the two). 

There aren't different parts of the stack we can grow. The stack having exactly one growth point is what makes it a stack, which is also ultimately what gives stack allocation the performance properties that drive people to want to use it.

I think we have three options:
- We can stop deallocating VLAs when `alloca` is called within their scope, as GCC does.
- We can use a different allocation scheme for `alloca`s that occur within the scope of a VLA, e.g. by preallocating a slab on the stack (in the static frame) but falling back on heap allocations if the slab runs out.
- We can diagnose this in some way.

The nice thing is that this situation is trivially statically detectable. You cannot abstract a call to `alloca`, and we can easily track whether there's a VLA currently in scope, so we can know with perfect reliability whether a specific call to `alloca` might have lifetime problems. A simple scope check might be a little too aggressive because it would sweep in some cases where the lifetime difference can't normally be observed, like when a VLA is created in the outermost scope in a function body. On the other hand, there are in fact ways to run code after the end of the formal lifetime of a VLA object in the outermost scope, like with `__attribute__((cleanup))` or `defer`. Those cases are still trivially static detectable, however (again with some risk of false positives). 

> I think we may run into similar problems when we get to the defer proposal because that will have similar stack allocation behavior to VLAs (block-scoped lifetime, not function-scoped like alloca).

`defer` doesn't have different allocation behavior. `defer` being block-scoped is not a problem for `alloca` for the same reason that a local `int` being block-scoped is not a problem for `alloca`: any allocations required are fixed-size and can be made out of the static frame, which can be allocated on the stack prior to any VLAs or `alloca`s, removing any potential for conflict. (`defer` does not in fact require an upfront allocation, but if it did, it would be fixed-size.)

> GCC actually had a similar bug in the past, but it was fixed in 2015: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=14236

This is a messy history. GCC had a default implementation of `-fstack-check` that used a VLA-like allocation; IIUC this has always been problematic, in part because VLAs are protected by the C standard in ways that fixed-size arrays are not (e.g. you cannot `switch` into the scope of a VLA). That broke `alloca` on those targets (bug 65958), which I think everyone would agree is a serious problem. A GCC developer fixed the ARM targets to not use that default implementation anymore, but then also changed the VLA/alloca interaction out of an abundance of caution for other targets. That latter choice doesn't seem to have gotten any discussion, which is weird for such a significant policy decision, and you can see in the narrowly-focused bug (14236) that other GCC developers were not originally keen on the idea.

`alloca` is a non-standard function, but most manual pages for it seem to include that it has complex interactions with VLAs. (This seems to be missing from BSD-based manuals, even those (like NetBSD) that have been substantially improved in other ways. But the Darwin manual straight-up specifies that the lifetime just shrinks when used within a VLA scope.) So I don't think we actually have to treat this as a bug, and I think GCC originally had the right idea in not trying to "fix" it. I agree with Eli that a warning is a good idea in any case, though.

---

### Comment 17 - rjmccall

> any allocations required are fixed-size and can be made out of the static frame, which can be allocated on the stack prior to any VLAs or allocas, removing any potential for conflict

FWIW, the manuals also generally include warnings about the compiler-dependent nature of `alloca`. In part this is because the compiler needs to know about it — the manuals want to make it clear that you really, really should not try to call `alloca` indirectly — but it's also because older compilers did not always build static frames the same way modern compilers generally do. C absolutely permits every new scope to push and pop its own memory, although this is generally incompatible with `alloca` and significantly complicates the implementation of `goto` and `switch`.

---

### Comment 18 - uecker

You really shouldn't use `alloca` at all. I think a warning if you mix it with VLAs is more than appropriate.

---

### Comment 19 - AaronBallman

I'm totally fine with a warning when the two are mixed, my only push back was on rejecting code outright with an error.

---

