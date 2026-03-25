# [clang] [x86-64] lrint()/lrintf() using instruction writing to 32-bit register if assigned to 32-bit int even though long is 64-bit

**Author:** joelkp
**URL:** https://github.com/llvm/llvm-project/issues/125324
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2025-02-06T02:58:38Z

## Body

This concerns a difference in the new optimizer behavior of clang 19, compared to clang 18 and older, and gcc as well. When building with `-O -ffast-math` or above, clang 19 makes programs behave quite differently compared to clang 18, and I think it's a bug though I'm not sure whether clang and llvm devs will agree.

According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.

The crucial thing is the way that the instructions used behave differently depending on the size of int register written to. If writing to a 32-bit register like `eax`, then the float-to-int conversion _saturates_ the value at the 32-bit signed int range. If writing to a 64-bit int register, then larger float values can be converted, and this is necessary for example if you want to convert a value between the signed and unsigned 32-bit int max from float to int. And if you expect a larger value to wrap around in converting to a 64-bit int, such as a float between 32-bit and 64-bit int max, then suddenly this no longer happens when assigning the result to a 32-bit int, even an unsigned one, despite using a 64-bit C API function.

[Here's](https://godbolt.org/z/EzYT45fYr) a test function `use_rounding()` showing the difference in what clang 18 vs. clang 19 generates. (Modern gcc also behaves like clang 18.)
```
#include <stdint.h>
#include <math.h>

uint32_t use_rounding(float x) {
    return lrintf(x);
}
```

clang 18 output, `-O -ffast-math`:
```
use_rounding:
        cvtss2si        rax, xmm0
        ret
```

clang 19 output, `-O -ffast-math`:
```
use_rounding:
        cvtss2si        eax, xmm0
        ret
```

If this wrapper around `lrintf()` had returned `uint64_t`, then the outputs would have been the same. Likewise, if `llrintf()` instead of `lrintf()` is used, the output is also the same.

This works the same for `lrint()`/`llrint()` as for `lrintf()`/`llrintf()`, it's a slightly different SSE instruction but with the same choice of register size. It results in the same problem too.


As I mentioned at the start, I think this is a bug. I also had to change my software to fix its behavior with clang 19 (so that my audio code makes the right noises again, not wrong ones). But at least there is a change possible, using the ll-versions rather than l-versions of these functions. But depending on whether or not clang and llvm devs consider this a bug, that may or may not be possible in later versions.

After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked. If you do change those in the same way as well, then software like mine, and any other software depending on llrint functions handling larger values gracefully, will break, and the only recourse will be to disable optimizations when building with clang.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Joel K. Pettersson (joelkp)

<details>
This concerns a difference in the new optimizer behavior of clang 19, compared to clang 18 and older, and gcc as well. When building with `-O -ffast-math` or above, clang 19 makes programs behave quite differently compared to clang 18, and I think it's a bug though I'm not sure whether clang and llvm devs will agree.

According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.

The crucial thing is the way that the instructions used behave differently depending on the size of int register written to. If writing to a 32-bit register like `eax`, then the float-to-int conversion _saturates_ the value at the 32-bit signed int range. If writing to a 64-bit int register, then larger float values can be converted, and this is necessary for example if you want to convert a value between the signed and unsigned 32-bit int max from float to int. And if you expect a larger value to wrap around in converting to a 64-bit int, such as a float between 32-bit and 64-bit int max, then suddenly this no longer happens when assigning the result to a 32-bit int, even an unsigned one, despite using a 64-bit C API function.

[Here's](https://godbolt.org/z/EzYT45fYr) a test function `use_rounding()` showing the difference in what clang 18 vs. clang 19 generates. (Modern gcc also behaves like clang 18.)
```
#include &lt;stdint.h&gt;
#include &lt;math.h&gt;

uint32_t use_rounding(float x) {
    return lrintf(x);
}
```

clang 18 output, `-O -ffast-math`:
```
use_rounding:
        cvtss2si        rax, xmm0
        ret
```

clang 19 output, `-O -ffast-math`:
```
use_rounding:
        cvtss2si        eax, xmm0
        ret
```

If this wrapper around `lrintf()` had returned `uint64_t`, then the outputs would have been the same. Likewise, if `llrintf()` instead of `lrintf()` is used, the output is also the same.

This works the same for `lrint()`/`llrint()` as for `lrintf()`/`llrintf()`, it's a slightly different SSE instruction but with the same choice of register size. It results in the same problem too.


As I mentioned at the start, I think this is a bug. I also had to change my software to fix its behavior with clang 19 (so that my audio code makes the right noises again, not wrong ones). But at least there is a change possible, using the ll-versions rather than l-versions of these functions. But depending on whether or not clang and llvm devs consider this a bug, that may or may not be possible in later versions.

After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked. If you do change those in the same way as well, then software like mine, and any other software depending on llrint functions handling larger values gracefully, will break, and the only recourse will be to disable optimizations when building with clang.
</details>


---

### Comment 2 - topperc

@phoebewang @fhahn  is this the same issue raised here https://github.com/llvm/llvm-project/pull/90065#issuecomment-2391211056

---

### Comment 3 - phoebewang

> [@phoebewang](https://github.com/phoebewang) [@fhahn](https://github.com/fhahn) is this the same issue raised here [#90065 (comment)](https://github.com/llvm/llvm-project/pull/90065#issuecomment-2391211056)

Yes. We have discussed it before.

> According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.

@joelkp We should to notice the `fast-math` optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept `lrint()` returns different value by implementation, it's also acceptable for the difference by optimizations.


> After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked.

This is due to the same reason. `llrint` has no implementation difference, neither does optimization. User should use the ll-versions if they want the consistent result.

---

### Comment 4 - topperc

> @joelkp We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.

@phoebewang The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.

Our previous discussion suggested you were going to write a documentation or release note about this behavior change did that ever happen?

---

### Comment 5 - topperc

Adding a few other people involved in FP who may have opinions. @andykaylor @arsenm @kpneal @jcranmer-intel 

---

### Comment 6 - phoebewang

> > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> 
> [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.

That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY
Clang doesn't distinguish them so far.

> Our previous discussion suggested you were going to write a documentation or release note about this behavior change did that ever happen?

No, I think it's too late to relase note, but maybe I can write it in User manual. I'll do it later.

---

### Comment 7 - topperc

> > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > 
> > 
> > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> 
> That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.

clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular, unlike -ffast-math, -fno-math-errno by itself does not set any fast math flags on instructions.

---

### Comment 8 - phoebewang

> > > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > 
> > > 
> > > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> > 
> > 
> > That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.
> 
> clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular it does not set any fast math flags on instructions.

Oh, you are right. Anyway, that matches with GCC 32-bit behavior. The point here is we can either consider -fno-math-errno has a broader effect to lrint, or it's a bug to both GCC and LLVM, and we should require one more  fast math flags here.

---

### Comment 9 - topperc

> > > > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > > 
> > > > 
> > > > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> > > 
> > > 
> > > That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.
> > 
> > 
> > clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular it does not set any fast math flags on instructions.
> 
> Oh, you are right. Anyway, that matches with GCC 32-bit behavior. The point here is we can either consider -fno-math-errno has a broader effect to lrint, or it's a bug to both GCC and LLVM, and we should require one more fast math flags here.

What is the gcc bug? With -fno-math-errno, gcc uses rax in 64-bit and eax in 32-bit. https://godbolt.org/z/5n936r9h3  This is what clang used to do.

---

### Comment 10 - phoebewang

> > > > > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > > > 
> > > > > 
> > > > > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> > > > 
> > > > 
> > > > That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.
> > > 
> > > 
> > > clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular it does not set any fast math flags on instructions.
> > 
> > 
> > Oh, you are right. Anyway, that matches with GCC 32-bit behavior. The point here is we can either consider -fno-math-errno has a broader effect to lrint, or it's a bug to both GCC and LLVM, and we should require one more fast math flags here.
> 
> What is the gcc bug? With -fno-math-errno, gcc uses rax in 64-bit and eax in 32-bit which is what clang used to do.

The point of "-fno-math-errno should not change result". This option indeed changes the result in 32-bit in GCC. And that the basis of the behavior change in 64-bit.

---

### Comment 11 - phoebewang

> > > > > > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > > > > 
> > > > > > 
> > > > > > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> > > > > 
> > > > > 
> > > > > That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.
> > > > 
> > > > 
> > > > clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular it does not set any fast math flags on instructions.
> > > 
> > > 
> > > Oh, you are right. Anyway, that matches with GCC 32-bit behavior. The point here is we can either consider -fno-math-errno has a broader effect to lrint, or it's a bug to both GCC and LLVM, and we should require one more fast math flags here.
> > 
> > 
> > What is the gcc bug? With -fno-math-errno, gcc uses rax in 64-bit and eax in 32-bit which is what clang used to do.
> 
> The point of "-fno-math-errno should not change result". This option indeed changes the result in 32-bit in GCC. And that the basis of the behavior change in 64-bit.

Think it again. I think I got what's the divergence of our understanding. You assumed the library implementation in 32-bit has the same result of using eax with -fno-math-errno, so it's not a result change. But my understanding is they are independent, the use of eax in 32-bit and rax in 64-bit makes the inconsistent of -fno-math-errno.

I'd consider it a special case of -fno-math-errno, rather than inventing a new flag for it (I don't think any current fast math flags conver this scenario).

---

### Comment 12 - topperc

> > > > > > > > [@joelkp](https://github.com/joelkp) We should to notice the fast-math optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept lrint() returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > > > > > 
> > > > > > > 
> > > > > > > [@phoebewang](https://github.com/phoebewang) The change in behavior here is part of -fno-math-errno which is a subset of -ffast-math. -fno-math-errno should not change result.
> > > > > > 
> > > > > > 
> > > > > > That's the observation of GCC's behavior: https://godbolt.org/z/YvahesfYY Clang doesn't distinguish them so far.
> > > > > 
> > > > > 
> > > > > clang behavior is changed with just -fno-math-errno. https://godbolt.org/z/j7EessPnn -fno-math-errno is a subset of -ffast-math in clang. In particular it does not set any fast math flags on instructions.
> > > > 
> > > > 
> > > > Oh, you are right. Anyway, that matches with GCC 32-bit behavior. The point here is we can either consider -fno-math-errno has a broader effect to lrint, or it's a bug to both GCC and LLVM, and we should require one more fast math flags here.
> > > 
> > > 
> > > What is the gcc bug? With -fno-math-errno, gcc uses rax in 64-bit and eax in 32-bit which is what clang used to do.
> > 
> > The point of "-fno-math-errno should not change result". This option indeed changes the result in 32-bit in GCC. And that the basis of the behavior change in 64-bit.
> 
> Think it again. I think I got what's the divergence of our understanding. You assumed the library implementation in 32-bit has the same result of using eax with -fno-math-errno, so it's not a result change. But my understanding is they are independent, the use of eax in 32-bit and rax in 64-bit makes the inconsistent of -fno-math-errno.
>

The library function should always obey the size of long which is 32 bits on 32-bit and 64 bits on 64-bit except for Windows. -fno-math-errno should not change the result. It should only change whether errno can be affected.

---

### Comment 13 - joelkp

Okay, so it sounds like this optimization will probably remain, though you don't all agree on the exact flags to enable/disable it. (I'd argue it would be nicest for users if they can disable it without enabling math errno.)

The thing that surprised me the most about it was, I'd written code which checks at compile time the size of long, comparing LONG_MAX and INT64_MAX, like this: `(LONG_MAX >= INT64_MAX ? lrintf(x) : llrintf(x))` -- and this kind of code I had believed to be portable for choosing the "least long" version of something which is "long enough", but it no longer was with clang 19 and this optimization.

> > [@phoebewang](https://github.com/phoebewang) [@fhahn](https://github.com/fhahn) is this the same issue raised here [#90065 (comment)](https://github.com/llvm/llvm-project/pull/90065#issuecomment-2391211056)
> 
> Yes. We have discussed it before.
> 
> > According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.
> 
> [@joelkp](https://github.com/joelkp) We should to notice the `fast-math` optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept `lrint()` returns different value by implementation, it's also acceptable for the difference by optimizations.
> 
> > After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked.
> 
> This is due to the same reason. `llrint` has no implementation difference, neither does optimization. User should use the ll-versions if they want the consistent result.

It's good to hear that the ll-versions of these functions will work as before in the future too.

The rest of what you wrote is a little confusing. As Topper also pointed to, the `lrint` function and behavior normally is tied to what `long` is defined as. There's no difference between `llrint` and `lrint` in this regard other than that `long` happens to vary between x86-64 OSes, while `long long` and thus `llrint` doesn't.

Other compilers stick to making the fast intrinsic version match the size of `long` too, but maybe you find it worth breaking compatibility.

---

### Comment 14 - andykaylor

I don't think this should be happening with any set of fast-math flags. The IR produced is this:

```
define dso_local noundef i32 @use_rounding(float noundef %x) local_unnamed_addr {
entry:
  %0 = tail call i64 @llvm.lrint.i64.f32(float %x)
  %conv = trunc i64 %0 to i32
  ret i32 %conv
}
```

The semantics of llvm.lrint.i64.f32 are independent of the `trunc` instruction, and the `trunc` instruction is not subject to fast-math flags.

As Craig pointed out, this is happening as a result of the intrinsic rather than function call being used. I don't see any reason to believe that it was an intentional decision based on fast-math flags.

---

### Comment 15 - jcranmer-intel

From a user-advocacy perspective, I think `-fno-math-errno` should be consistently interpreted as "allow the use of hardware instructions that would return the same results as the C library function".

From a general LLVM IR semantics perspective, `call i64 @llvm.lrint.i64.f32(float %x)` should do the semantics of a `rint` from binary32 -> i64, and only do otherwise if there is a particular fast-math flag changing its semantics. Trying to fuse the `trunc` and `lrint` should only be possible at best with fast-math flags.

Should there be a fast-math flag? There's a benefit to having a binary32 -> i32 conversion for vectorized code, which suggests yes. My first instinct for a fast-math flag would be `contract`, except `trunc` (being an integer instruction) doesn't have FMF. Of the existing flags, `afn` may be appropriate here. I've also been mulling a `no-overflow`-esque FMF that may apply here, but there is a big gulf in infrastructure needing to get that enabled, given the exhaustion of bits for FMF.

---

### Comment 16 - phoebewang

> Okay, so it sounds like this optimization will probably remain, though you don't all agree on the exact flags to enable/disable it. (I'd argue it would be nicest for users if they can disable it without enabling math errno.)
> 
> The thing that surprised me the most about it was, I'd written code which checks at compile time the size of long, comparing LONG_MAX and INT64_MAX, like this: `(LONG_MAX >= INT64_MAX ? lrintf(x) : llrintf(x))` -- and this kind of code I had believed to be portable for choosing the "least long" version of something which is "long enough", but it no longer was with clang 19 and this optimization.

No, it's not just about flags, otherwise, there's no reason why ll-versions not be affected.

I think the code you mentioned demonstrates the philosophy behand the optimization, i.e., user who cares about consistency is already aware of the use of `llrint`.

The code gives me an contradictory feeling. The users knows much about the size of long on different OS, but hopes `lrint` can save size for them. I think it's more clear to always use `llrintf(x)` in the code.

> > > [@phoebewang](https://github.com/phoebewang) [@fhahn](https://github.com/fhahn) is this the same issue raised here [#90065 (comment)](https://github.com/llvm/llvm-project/pull/90065#issuecomment-2391211056)
> > 
> > 
> > Yes. We have discussed it before.
> > > According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.
> > 
> > 
> > [@joelkp](https://github.com/joelkp) We should to notice the `fast-math` optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept `lrint()` returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked.
> > 
> > 
> > This is due to the same reason. `llrint` has no implementation difference, neither does optimization. User should use the ll-versions if they want the consistent result.
> 
> It's good to hear that the ll-versions of these functions will work as before in the future too.
> 
> The rest of what you wrote is a little confusing. As Topper also pointed to, the `lrint` function and behavior normally is tied to what `long` is defined as. There's no difference between `llrint` and `lrint` in this regard other than that `long` happens to vary between x86-64 OSes, while `long long` and thus `llrint` doesn't.
> 
> Other compilers stick to making the fast intrinsic version match the size of `long` too, but maybe you find it worth breaking compatibility.

You example code above is a good demonstration of what I was arguing for, ie., in a broader view, users who use `lrint` either don't care about the overflow, or they will use `llrint` instead. Given `lrint` is already inconsistency without `fast-math` (though current only occur between different OSes), it's reasonable to extend it a bit more with `fast-math`.

The argument about using different flags doesn't look correct to me. I think it's more clear if we split it into 2-steps:

- `-fno-math-errno` turns `lrint` into intrinsic to allow more optimization. In this step, only errno was considered, so there's no problem here;
- Further optimization optimizes it leveraging the nature of the function itself, e.g., users should not are about the overflow. It's an extension, but it looks reasonable to me. It is not a general optimization that user controls from flags, it's implied (or more clearly, encoded) in the intrinsic itself;

The root reason of the difference `llrint` and `lrint` are just details. In most case, users who are aware of it will take care of using `llrint`. As I commented above, the use in your scenario has its contradiction, it should not be a common problem.

---

### Comment 17 - joelkp

> > The thing that surprised me the most about it was, I'd written code which checks at compile time the size of long, comparing LONG_MAX and INT64_MAX, like this: `(LONG_MAX >= INT64_MAX ? lrintf(x) : llrintf(x))` -- and this kind of code I had believed to be portable for choosing the "least long" version of something which is "long enough", but it no longer was with clang 19 and this optimization.
> 
> I think the code you mentioned demonstrates the philosophy behand the optimization, i.e., user who cares about consistency is already aware of the use of `llrint`.
> 
> The code gives me an contradictory feeling. The users knows much about the size of long on different OS, but hopes `lrint` can save size for them. I think it's more clear to always use `llrintf(x)` in the code.

The point is that the intention behind such code is to be at least a bit more general than simply targeting x86-64. If it were purely x86-64 targeted, then yes, the simple way to make it right on all OSes is to pick the ll-versions of the functions. That said, I would by no means claim the example above is general enough to suit most people who want to use more platforms.

More generally, in C all of `int`, `long`, and `long long` etc. are inconsistent between platforms, and so is every single function which varies with how these are defined in the standard, which is _a great many functions_. But that doesn't mean that valid optimizations can just go, 'Oh, it's inconsistent anyway, so it can be made to be anything.' When looking at consistency, what's the scope considered? For optimizations, it's usually a platform which is _both_ an architecture and an OS combined, and at that level, there is no inconsistency at all in the size of `long` and the related `lrintf()` question.

As for the bigger scope of all platforms, with its great inconsistencies, that's why the standard grew to include things like types `int32_t` and `int64_t` in place of `int` and `long`, usually defined in terms of the latter sort of basic types per platform. Unfortunately that kind of thing is limited to mainly type names and some named constants. The standard library lacks a set of functions named in an analogous way whenever integers are concerned -- so if you want to extend that philosophy of having the exact size you want by name in whichever thing, then you need to roll your own macros and inline functions.

That's the kind of philosophy, to the extent attempted in a codebase, which leads to comparing sizes and such at compile time to decide what to use on that basis. But if you try to do that, the code will break if the compiler implements optimizations like the one discussed in this issue. Generally, I think it's philosophically unsound for that reason to have such optimizer behavior.

> 
> > > > [@phoebewang](https://github.com/phoebewang) [@fhahn](https://github.com/fhahn) is this the same issue raised here [#90065 (comment)](https://github.com/llvm/llvm-project/pull/90065#issuecomment-2391211056)
> > > 
> > > 
> > > Yes. We have discussed it before.
> > > > According to the C standard, functions like `lrint()` and `llrint()` are to round their argument to the nearest integer, but the implementation can do whatever it likes with the return value if the argument is too large to fit into a `long` or `long long` respectively. On 32-bit systems where `long` is 32-bit, this means that lrint() and llrint() behave differently, and that's fine. But when `long` is 64-bit, `lrint()` is supposed to accommodate values large enough to fit into a `long`. This no longer happens with the clang 19 optimization, depending on how `lrint()` is used.
> > > 
> > > 
> > > [@joelkp](https://github.com/joelkp) We should to notice the `fast-math` optimization is an extension of C standard. The principle under this optimization is if it's appropriate to accept `lrint()` returns different value by implementation, it's also acceptable for the difference by optimizations.
> > > > After all, if you consider the new optimization to actually be valid, then there does not exist any reason for it to not be done for the ll-versions of these functions as well, other than it probably having been overlooked.
> > > 
> > > 
> > > This is due to the same reason. `llrint` has no implementation difference, neither does optimization. User should use the ll-versions if they want the consistent result.
> > 
> > 
> > It's good to hear that the ll-versions of these functions will work as before in the future too.
> > The rest of what you wrote is a little confusing. As Topper also pointed to, the `lrint` function and behavior normally is tied to what `long` is defined as. There's no difference between `llrint` and `lrint` in this regard other than that `long` happens to vary between x86-64 OSes, while `long long` and thus `llrint` doesn't.
> > Other compilers stick to making the fast intrinsic version match the size of `long` too, but maybe you find it worth breaking compatibility.
> 
> You example code above is a good demonstration of what I was arguing for, ie., in a broader view, users who use `lrint` either don't care about the overflow, or they will use `llrint` instead. Given `lrint` is already inconsistency without `fast-math` (though current only occur between different OSes), it's reasonable to extend it a bit more with `fast-math`.

---

### Comment 18 - phoebewang

> > > The thing that surprised me the most about it was, I'd written code which checks at compile time the size of long, comparing LONG_MAX and INT64_MAX, like this: `(LONG_MAX >= INT64_MAX ? lrintf(x) : llrintf(x))` -- and this kind of code I had believed to be portable for choosing the "least long" version of something which is "long enough", but it no longer was with clang 19 and this optimization.
> > 
> > 
> > I think the code you mentioned demonstrates the philosophy behand the optimization, i.e., user who cares about consistency is already aware of the use of `llrint`.
> > The code gives me an contradictory feeling. The users knows much about the size of long on different OS, but hopes `lrint` can save size for them. I think it's more clear to always use `llrintf(x)` in the code.
> 
> The point is that the intention behind such code is to be at least a bit more general than simply targeting x86-64. If it were purely x86-64 targeted, then yes, the simple way to make it right on all OSes is to pick the ll-versions of the functions. That said, I would by no means claim the example above is general enough to suit most people who want to use more platforms.

I don't have cross platforms experience, so I probably didn't conside enough about it. Can you elaborate in which platform that doesn't support `llrint` or it has difference with `lrint` when LONG_MAX is INT64_MAX?

> More generally, in C all of `int`, `long`, and `long long` etc. are inconsistent between platforms, and so is every single function which varies with how these are defined in the standard, which is _a great many functions_. But that doesn't mean that valid optimizations can just go, 'Oh, it's inconsistent anyway, so it can be made to be anything.' When looking at consistency, what's the scope considered? For optimizations, it's usually a platform which is _both_ an architecture and an OS combined, and at that level, there is no inconsistency at all in the size of `long` and the related `lrintf()` question.
> 
> As for the bigger scope of all platforms, with its great inconsistencies, that's why the standard grew to include things like types `int32_t` and `int64_t` in place of `int` and `long`, usually defined in terms of the latter sort of basic types per platform. Unfortunately that kind of thing is limited to mainly type names and some named constants. The standard library lacks a set of functions named in an analogous way whenever integers are concerned -- so if you want to extend that philosophy of having the exact size you want by name in whichever thing, then you need to roll your own macros and inline functions.
> 
> That's the kind of philosophy, to the extent attempted in a codebase, which leads to comparing sizes and such at compile time to decide what to use on that basis. But if you try to do that, the code will break if the compiler implements optimizations like the one discussed in this issue. Generally, I think it's philosophically unsound for that reason to have such optimizer behavior.

I'm opposed to extend this case into a general `long` and `long long` inconsistent problem. I believe in some cases there are UB to compiler which probably be already used by optimizations or can be a good candidate for the future. We should analyze them case by case.

---

### Comment 19 - topperc

@phoebewang how does this optimization benefit users? For scalar code it maybe says 1 byte of encoding but does not improve performance. Is there a vector advantage?

---

### Comment 20 - phoebewang

> [@phoebewang](https://github.com/phoebewang) how does this optimization benefit users? For scalar code it maybe says 1 byte of encoding but does not improve performance. Is there a vector advantage?

Yes, vectorized code will be benefit much with this optimization. We don't have a vector PS->QQ instruction before AVX512DQ. And even AVX512DQ and later will get 2x throughput improvement with it.

---

### Comment 21 - topperc

I'm not sure what to do here. It appears that @jcranmer-intel @andykaylor and myself do not agree that this is a valid optimization. It is not a valid optimization with the current IR documentation, so in order to keep that must be updated. But I don't know if you're going to find anyone approve such an update.

---

### Comment 22 - joelkp

> > [@phoebewang](https://github.com/phoebewang) how does this optimization benefit users? For scalar code it maybe says 1 byte of encoding but does not improve performance. Is there a vector advantage?
> 
> Yes, vectorized code will be benefit much with this optimization. We don't have a vector PS->QQ instruction before AVX512DQ. And even AVX512DQ and later will get 2x throughput improvement with it.

There is an alternative feature you could consider offering instead, for the same performance. While the C standard does not include the `irint()` family of functions (same as `lrint()` except return `int` instead of `long`), it exists in some less widely used math libraries. Offering `__builtin_irint()` and the same for irintf and irintl would allow people to use that in code.

I just checked, and gcc actually offers these `__builtin_irint()` intrinsics, though they are undocumented. They give the expected result, i.e. set result to `eax` on x86-64. clang could do the same.

---

### Comment 23 - phoebewang

> I'm not sure what to do here. It appears that [@jcranmer-intel](https://github.com/jcranmer-intel) [@andykaylor](https://github.com/andykaylor) and myself do not agree that this is a valid optimization. It is not a valid optimization with the current IR documentation, so in order to keep that must be updated. But I don't know if you're going to find anyone approve such an update.

Ok, I respect the consensus here. I create #125848 to revert the combination, PTAL.

Thanks all for the great inputs!

---

### Comment 24 - phoebewang

> > > [@phoebewang](https://github.com/phoebewang) how does this optimization benefit users? For scalar code it maybe says 1 byte of encoding but does not improve performance. Is there a vector advantage?
> > 
> > 
> > Yes, vectorized code will be benefit much with this optimization. We don't have a vector PS->QQ instruction before AVX512DQ. And even AVX512DQ and later will get 2x throughput improvement with it.
> 
> There is an alternative feature you could consider offering instead, for the same performance. While the C standard does not include the `irint()` family of functions (same as `lrint()` except return `int` instead of `long`), it exists in some less widely used math libraries. Offering `__builtin_irint()` and the same for irintf and irintl would allow people to use that in code.
> 
> I just checked, and gcc actually offers these `__builtin_irint()` intrinsics, though they are undocumented. They give the expected result, i.e. set result to `eax` on x86-64. clang could do the same.

It sounds a good idea, I'll explore along this way. Thanks for the information!

---

