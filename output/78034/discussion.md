# c23 aggregate initialization

**Author:** uecker
**URL:** https://github.com/llvm/llvm-project/issues/78034
**Status:** Closed
**Labels:** c23, clang:codegen, regression, miscompilation, confirmed
**Closed Date:** 2025-04-07T20:00:52Z

## Body

It seems clang recently started to miscompile aggregate initialization.

```c
union foo {
    char x;
    int  y;
    long z;
} x = { 0 };
```

would only initialize x, which is OK (although a questionable decision). But my understanding is that
for C23  x = { } zero-filling is required, and this also does not happen anymore.

https://godbolt.org/z/68q9bY5cT

## Comments

### Comment 1 - shafik

This looks like a potential regression from clang-17: https://godbolt.org/z/nPYo1ejWr

CC @AaronBallman 

---

### Comment 2 - AaronBallman

Something odd is going on here as it seems to relate to use of `[[gnu::noinline]]` of all things: https://godbolt.org/z/ze5YMP7bG and the issue does not reproduce at -O0.

---

### Comment 3 - AaronBallman

FWIW, the changes in Clang 17 related to aggregate initialization for C2x are in https://reviews.llvm.org/D147349. But those changes were in Clang 17 and so I don't think relate to the issue (Clang 17 doesn't demonstrate the bug).

---

### Comment 4 - uecker

Am Dienstag, dem 16.01.2024 um 15:22 +0000 schrieb Aaron Ballman:
> Something odd is going on here as it seems to relate to use of [[gnu::noinline]] of all things: https://godbolt.org/z/ze5YMP7bG and the issue does not reproduce at -O0.

The noinline is only required so that the test with memcpy is not
optimized and correctly also compares the padding.

Martin

> —
> Reply to this email directly, view it on GitHub, or unsubscribe.
> You are receiving this because you authored the thread.Message ID: ***@***.***>



---

### Comment 5 - AaronBallman

We seem to be initializing the union based on the first element of the union rather than the largest element of the union. Switching element order so that the largest is first causes the issue to go away.

https://godbolt.org/z/cvojEdaMc

`store i8 0, ptr %a, align 8` is the problematic bit. If you change the order of the union elements, then it becomes `store i64 0, ptr %a, align 8` as it was in Clang 17.

CC @rjmccall @efriedma-quic 

---

### Comment 6 - AaronBallman

I think this is an LLVM optimization pass issue, the IR looks correct at O0 with no LLVM passes being run:
```
define dso_local i32 @main() {
entry:
  %retval = alloca i32, align 4
  %a = alloca %union.foo, align 8
  store i32 0, ptr %retval, align 4
  call void @llvm.memset.p0.i64(ptr align 8 %a, i8 0, i64 8, i1 false)
  %call = call i32 @c(ptr noundef %a)
  ret i32 %call
}
```
That call to memset should be sufficient to zero initialize the local, but for some reason LLVM seems to think it doesn't need to zero initialize the inactive members (maybe because it thinks they're padding and thus unspecified?).

---

### Comment 7 - efriedma-quic

Looks like the codegen changed... but it's semantically the same: we initialize the first element, the rest is undef.  https://godbolt.org/z/Wxb5esWhY .  I think clang 17 just got lucky with LLVM optimizations for this particular example.

As far as I can tell, the issue is just that our original extension for "empty initialization" didn't initialize padding.  And the corresponding C++ rule doesn't require initializing padding.  And nobody realized the C23 has different semantics from the extension we originally implemented.  (Probably a consequence of the fact that the rules are sort of weird.)

Some examples:

```
union X { char a; long b; } x = {};
struct Y {char a; __attribute((packed)) long b; int c;} y = {};
void g(void*);
void f() {
  union X x = {}; g(&x);
  struct Y y = {}; g(&y);
  struct Z {long a; union X x; } z = { (long)&f, {}}; g(&z);
}
```

On a side-note, it's probably worth amending LLVM LangRef to explicitly specify what happens to padding in global variables of struct type; I don't think it's noted anywhere.

---

### Comment 8 - efriedma-quic

Actually, regarding the C++ rule, I'm not sure it allows initializing the padding even if we wanted to.  (For simple variables, probably it's impossible for user code to tell, but list initialization shows up in other places.)

---

### Comment 9 - rjmccall

I agree with Eli; it looks like we need to:
1. teach IRGen to zero-initialize explicit padding fields (at least in C2x mode) instead of `undef`-initializing them when emitting constant struct initializers for globals and
2. figure out what, if anything, LLVM guarantees for the initialization of implicit padding in global variables.  If LLVM doesn't give strong enough guarantees here, we will need to emit constant struct initializers with a type that makes all padding explicit.

---

### Comment 10 - uecker

Am Dienstag, dem 16.01.2024 um 21:19 +0000 schrieb Eli Friedman:
> Looks like the codegen changed... but it's semantically the same: we initialize the first element, the rest is undef. https://godbolt.org/z/Wxb5esWhY . I think clang 17 just got lucky with
> LLVM optimizations for this particular example.
> As far as I can tell, the issue is just that our original extension for "empty initialization" didn't initialize padding. And the corresponding C++ rule doesn't require initializing
> padding. And nobody realized the C23 has different semantics from the extension we originally implemented. (Probably a consequence of the fact that the rules are sort of weird.)

I am not terribly convinced that what LLVM does with
padding conforms to the C standard anyway.  Even when
uninitialized, I don't think undef is correct.

Martin



> Some examples:
> union X { char a; long b; } x = {};
> struct Y {char a; __attribute((packed)) long b; int c;} y = {};
> void f() {
>   union X x = {}; g(&x);
>   struct Y y = {}; g(&y);
> }
> On a side-note, it's probably worth amending LLVM LangRef to explicitly specify what happens to padding in global variables of struct type; I don't think it's noted anywhere.
> —
> Reply to this email directly, view it on GitHub, or unsubscribe.
> You are receiving this because you authored the thread.Message ID: ***@***.***>



---

### Comment 11 - AaronBallman

> I agree with Eli; it looks like we need to:

Thank you both for taking a look!

I think this is a relatively serious regression (even though unions are not the most common thing used in C, they are used quite frequently, as is the zero-init idiom), do you think this is something one of you could plausibly tackle for Clang 18?

---

### Comment 12 - efriedma-quic

I spent a bit more time digging into this; it's unfortunately more complicated than I'd hoped.

Checklist of issues that need to be fixed:

- Either constant evaluation (ExprConstant) needs to model padding, or we have to zero-initialize all padding in constant-evaluated structs.  Currently there's no way to express the difference between `{}` and `{0}` in APValue.
- CGExprConstant lowering of InitListExprs needs to zero out padding for empty initializer lists.
- AggExprEmitter::VisitInitListExpr lowering of InitListExprs needs to zero out padding for empty initializer lists.
- CodeGenModule::EmitNullConstant needs to zero-initialize padding... but it looks like it already does that by default anyway, for the sake of implementation convenience, so I guess there's nothing to do here.  Probably worth adding an explicit comment here.
- Figure out what to do about LLVM constants for struct types with implicit padding.  I would guess most things sort of just work: globals in LLVM effectively have zeroed padding (not documented, but that's how it currently works in practice as far as I can tell), and automatic variable initializations translates zeroing to memset.  But the LLVM handling of global variables isn't documented, and automatic variable initialization doesn't work right for structs that contain pointers to address spaces where null isn't an all-zero pattern.  And making assumptions here is fragile (for example, the current version of #79502 assumes padding is irrelevant).

CC @serge-sans-paille

---

### Comment 13 - efriedma-quic

d0b87c2ea362eaf1397c13d566c34663c483aac3 is the easy bits.  Should handle the obvious cases, but there are a lot of edge cases it won't handle correctly.

---

### Comment 14 - DimitryAndric

Initially I ran into trouble reproducing this problem, as the padding on the stack in main isn't initialized:

```asm
main:                                   # @main
        .cfi_startproc
# %bb.0:                                # %entry
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset %rbp, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register %rbp
        subq    $16, %rsp
        movb    $0, -8(%rbp)
        leaq    -8(%rbp), %rdi
        callq   c
```

So depending on which OS or possibly even which machine you run it on, the return value of `c` is unpredictable. :)

In any case, I did some bisecting to find out when the `movq $0, -8(%rbp)` changed into `movb $0, -8(%rbp)`, and this turns out to be `llvmorg-18-init-12497-g`0d2860b7958 ["clang] Avoid memcopy for small structure with padding under -ftrivial-auto-var-init (#71677)"] by @serge-sans-paille : 

```diff
--- pr78034-0-llvmorg-18-init-12496-g9ab133bd9f9.s	2024-03-20 16:57:06.857920000 +0000
+++ pr78034-0-llvmorg-18-init-12497-g0d2860b7958.s	2024-03-20 16:57:39.139362000 +0000
@@ -38,7 +38,7 @@
 	movq	%rsp, %rbp
 	.cfi_def_cfa_register %rbp
 	subq	$16, %rsp
-	movq	$0, -8(%rbp)
+	movb	$0, -8(%rbp)
 	leaq	-8(%rbp), %rdi
 	callq	c
 	addq	$16, %rsp
@@ -52,7 +52,7 @@
 	.type	z,@object                       # @z
 	.local	z
 	.comm	z,8,8
-	.ident	"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 9ab133bd9f9cedb880b453dd830b58857fab41ec)"
+	.ident	"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 0d2860b795879f4dd152963b52f969b53b136899)"
 	.section	".note.GNU-stack","",@progbits
 	.addrsig
 	.addrsig_sym z
```

I will probably just revert that commit in FreeBSD, unless there is a better solution now?



---

### Comment 15 - AaronBallman

> I will probably just revert that commit in FreeBSD, unless there is a better solution now?

FWIW, we have a review out to revert this partially: https://github.com/llvm/llvm-project/pull/84230



---

### Comment 16 - yabinc

Hi, I run into a similar bug of using {} to initialize a static union variable in linux kernel (https://lore.kernel.org/llvm/20240620181736.1270455-1-yabinc@google.com/).

```c
typedef union {
	__be32		a4;
	__be32		a6[4];
	struct in6_addr	in6;
} xfrm_address_t;

struct xfrm_state* xfrm_state_find() {
  static xfrm_address_t saddr_wildcard = {};
}
```

In llvm IR, saddr_wildcard only has its first field zero initialized, left bits are marked as undef.
```llvm
@saddr_wildcard = internal global { i32, [12 x i8] } { i32 0, [12 x i8] undef }, align 4, !dbg !9
```

However, the code assumes all fields are initialized as zero, and use them.
This works with some optimization flags. Because sdaddr_wildcard is still allocated in bss section, which is zero initialized.
But with some optimization flags (like -O2 with always_inline attribute), clang may decide to replace saddr_wildcard with undef or poison values (GlobalOptPass), remove instructions for using undef and poison values (InstCombinePass), and give a wrong result.

I think this problem always exists in clang. Because clang always marks bits not belonging to the first field as undef.
A reproduce case is in https://godbolt.org/z/f6E14KeMG. In it, the correct return value of av() is 3, while both clang trunk and clang 16 decides to return 2.

I checked https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2900.htm. It says:
  
4.5. OPTIONAL CHANGE 0: Largest-Then-First Initialization - Modify §6.7.9 paragraph 10, last bullet point
…

— if it is a union and the initializer is the empty initializer, the largest member is initialized (recursively) according to these rules, then the first named member is initialized (recursively) according to these rules, and any padding is initialized to zero bits;
— if it is a union and the initializer is not an empty initializer , the first named member is initialized (recursively) according to these rules, and any padding is initialized to zero bits;

6.2. Compiler Extensions + Union Aliasing

Ultimately, the Committee was not in favor of the largest-member-initialized behavior for an empty initializer { }.


It seems the standard doesn't plan to guarantee the largest-member-initialized behavior. But given the current situation:
1) gcc seems to support the largest-member-initialized behavior, at least not optimizing the code wrongly for it.
2) The behavior of clang of supporting this is tricky: works in some optimization flags, but not in others.
3) Using -ftrivial-auto-var-init=zero can protect automatic variables, but not static or global variables. And clang doesn't give any warning for it.

Shall we support largest-member-initialized behavior for an empty initializer { }, or at least make it a warning when the first member isn't the largest one?


---

### Comment 17 - rjmccall

Since your case is a static local variable, zero-initializing the padding seems to be required, and that should be good enough to get acceptable behavior.

---

### Comment 18 - nickdesaulniers

Thread with @ThePhD : https://fosstodon.org/@thephd@pony.social/112651845876189104

I hope the C standards committee can revisit the _the first named member is initialized_ wording.

`= {};` is frequently used to zero initialize aggregates (structs, arrays).  That it doesn't do that for unions with static storage is _surprising_ and _error prone_.

---

### Comment 19 - ThePhD

It is standards conforming to zero the entire union, and I am unsure why Clang doesn't do it as an implementation extension and a compiler that has safety and predictability in mind for their code. As I understand it `= {}` had this behavior before the C23 wording, so I'm wondering why they don't just keep doing what they were doing before C23 came out?

Nevertheless, as I explained in that thread, my fight for largest-union-member initialization did, in fact, fail. And my fight to get

```cpp
union u {
int x;
double y;
};

static union u val;

int main () {
  union u {
       double y;
       int x;
  }; // considered compatible to previous "u" decl
   val = (union u){}; // empty-inits `double y`, the largest value, and then assigns from this compatible union
}
```

to stay after the C23 Improved Rules for Tag Compatbility changes. But this was shot down by accepted National Body comment processing, where it was noticed that unions with different member orders were considered compatible; we thus imposed a same-order rule on the unions in particular to clarify this.

If you would like this to change, you need to petition the Committee. The pushback to the suggestion came primarily from implementers, both Static Analysis folks and "regular" Binary-producing Production Compilers. The weight of your case would go much farther if you speak directly somewhere that can be referenced, rather than having your concern filtered through me or anyone else, though I will absolutely put my weight behind you because I think it's extremely dangerous -- for the many reasons in this thread -- to not init the first member and zero **all** outside-first-member bits, and not JUST the padding bits.

---

### Comment 20 - rjmccall

When we zero-initialize padding in a union, we should be initializing all bits outside the member that's initialized.  That is a consistent rule that will lead to all bits being initialized one way or another, regardless of the exact form of the initialization.

---

### Comment 21 - uecker

Yes, the first member should be default initialized and everything else should just be set to zero.  I think this was intended and if this isn't clear I think we will revise the wording to make it clear.  But my understanding is that the bytes corresponding to a larger non-active union member fall under the definition of padding bytes, because they pad the active member to the required size. And I think LLVM's undef is wrong for any padding bytes in C anyway (and dangerous).

---

### Comment 22 - yabinc

As suggested in the comments of Linus Torvalds in https://lore.kernel.org/lkml/CAHk-=whWXCP9Jn=y=MXot3T6sECEyK5nTmuvT=WDQM9h_NtJqA@mail.gmail.com/T/:
  The linux kernel depends on zero initializing unspecified fields in a union (or struct) type, when an empty initializer or a partial initializer appears.

This applies to both global variables and local auto variables.

Although it isn't required by the C standard, it's also almost impossible to fix all places in the linux kernel. So we kind of need to support this in clang.

---

### Comment 23 - AaronBallman

FWIW, I think we *should* default initialize the first member and all other bits in the union should be set to zero even if it's not required by the standard. Barring any shocking performance issues with that approach, I think it's the behavior that fits most folks' mental model of how default init of unions works. In the worst case, we initialize some bits that we didn't need to initialize because they're not being read, but that seems like a somewhat unusual use case for unions and worth it for the security tradeoffs of more understandable initialization behavior.

---

### Comment 24 - AaronBallman

I believe this issue is resolved by 7a086e1b2dc05f54afae3591614feede727601fa and can now be closed. Can anyone else confirm?

---

### Comment 25 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Martin Uecker (uecker)

<details>
It seems clang recently started to miscompile aggregate initialization.

```c
union foo {
    char x;
    int  y;
    long z;
} x = { 0 };
```

would only initialize x, which is OK (although a questionable decision). But my understanding is that
for C23  x = { } zero-filling is required, and this also does not happen anymore.

https://godbolt.org/z/68q9bY5cT
</details>


---

### Comment 26 - efriedma-quic

Yes, fixed.  That said, we didn't touch the behavior for C++.  Which is compliant with the current C++ spec, but maybe the committee should consider changing that.

---

