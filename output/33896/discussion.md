# InstCombine cannot blindly assume that inttoptr(ptrtoint x) -> x

**Author:** nunoplopes
**URL:** https://github.com/llvm/llvm-project/issues/33896
**Status:** Open
**Labels:** bugzilla, miscompilation, llvm:instcombine

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [34548](https://llvm.org/bz34548) |
| Version | trunk |
| OS | All |
| Blocks | llvm/llvm-project#34577 llvm/llvm-project#39193  |
| CC | @comex,@majnemer,@vns-mn,@dwblaikie,@efriedma-quic,@fhahn,@hfinkel,@jensmaurer,@dobbelaj-snps,@aqjune,@RKSimon,@Meinersbur,@sunfishcode,@MattPD,@uecker,@RalfJung,@regehr,@rnk,@sanjoy,@rotateright,@yuanfang-chen |

## Extended Description 
Example of an end-to-end miscompilation by clang of the following code involving ptrtoint:

$ cat c.c
#include <stdio.h>

void f(int*, int*);

int main()
{
  int a=0, y[1], x = 0;
  uintptr_t pi = (uintptr_t) &x;
  uintptr_t yi = (uintptr_t) (y+1);
  uintptr_t n = pi != yi;

  if (n) {
    a = 100;
    pi = yi;
  }

  if (n) {
    a = 100;
    pi = (uintptr_t) y;
  }

  *(int *)pi = 15;

  printf("a=%d x=%d\n", a, x);

  f(&x,y);

  return 0;
}


$ cat b.c
void f(int*x, int*y) {}


$ clang -O2 c.c b.c -o foo

$ ./foo
a=0 x=0

This result is wrong.  The two possible outcomes are: a=0 x=15, and a=100 x=0.

The bug is in Instcombine that treats inttoptr(ptrtoint(x)) == x, which is incorrect.  This transformation can only be done if x is dereferenceable for the accesses through inttoptr.
Compare the following:
clang -O0 -S -emit-llvm -o - c.c | opt -S -sroa
clang -O0 -S -emit-llvm -o - c.c | opt -S -sroa -instcombine

Integer compares are replaces with pointer compares (wrong) and load/stores are changed from inttoptr to pointers directly (also wrong).

Test case by Gil Hur.

## Comments

### Comment 1 - dwblaikie

mentioned in issue llvm/llvm-bugzilla-archive#52570

---

### Comment 2 - RalfJung

mentioned in issue llvm/llvm-bugzilla-archive#46380

---

### Comment 3 - aqjune

mentioned in issue llvm/llvm-project#39193

---

### Comment 4 - aqjune

mentioned in issue llvm/llvm-bugzilla-archive#37469

---

### Comment 5 - RalfJung

mentioned in issue llvm/llvm-project#34577

---

### Comment 6 - 4584a04b-325b-46e1-813c-87051a6e36f1

For those who have not seen this. WG14 now has a draft TS which addresses this topic: 

http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2676.pdf

---

### Comment 7 - RalfJung

> If some transform isn't consistent with this, please file a bug.

llvm/llvm-bugzilla-archive#46380

---

### Comment 8 - efriedma-quic


> > icmp is defined to just take the raw pointer bits as an integer.  This is described in LangRef.
> 
> Doesn't LLVM also assume that a fresh pointer returned by malloc does not
> equal any older pointer, and optimize icmp accordingly?

There's a class of optimizations where LLVM implicitly replaces malloc/new with an alloca, and we can optimize comparisons against that.  In cases where that optimization isn't legal, LLVM doesn't make any assumptions, as far as I know.

---

### Comment 9 - RalfJung

> icmp is defined to just take the raw pointer bits as an integer.  This is described in LangRef.

Doesn't LLVM also assume that a fresh pointer returned by malloc does not equal any older pointer, and optimize icmp accordingly? They could physically be equal if addresses are reused (and that easily happens e.g. with jemalloc). C has a clause that pointers become undef when you deallocate the block they point to, but I have not seen such a clause in the LVLM semantics (but I might have just missed it).

---

### Comment 10 - efriedma-quic


> Not sure whether LLVM's AliasAnalysis interface is only defined for valid
> pointer representations (ie including one-past-the-last) of just
> dereferenceable pointers. The latter would be hard for analysis since one
> had to proof whether a pointer is dereferenceable before an AA query.

The alias() API is defined for any pair of pointers which are both defined at some position in the program.  The question is essentially, given two pointer values, for any location in the program where both pointer values are defined, is it possible that a store to one pointer would conflict with a load or store to the other pointer?  The possible answers are essentially yes, no, or don't know.

If it in't legal to dereference a pointer, it trivially doesn't alias anything: there are no memory accesses through the pointer, so there isn't any possible conflict.  Similarly, if a pointer points to a constant, it doesn't alias anything: there are no stores through it.


> The LLVM reference (except getelementptr inbounds) seem to miss concepts of
> valid pointer representation, pointer provenance or partial ordering of
> pointers. The ability of BasicAA to deduce pointers as non-aliasing based on
> provenance seem to be inferred from C/C++ semantics.

icmp is defined to just take the raw pointer bits as an integer.  This is described in LangRef.  If the text of LangRef describing icmp isn't sufficiently clear, suggestions welcome.  If some transform isn't consistent with this, please file a bug.

For provenance, the rules we have are described at http://llvm.org/docs/LangRef.html#pointeraliasing .  The problem described here is that the rules don't actually work.

See also http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2311.pdf .

---

### Comment 11 - Meinersbur

The LLVM reference (except getelementptr inbounds) seem to miss concepts of valid pointer representation, pointer provenance or partial ordering of pointers. The ability of BasicAA to deduce pointers as non-aliasing based on provenance seem to be inferred from C/C++ semantics.

From the semantics of icmp:
> If the operands are pointer typed, the pointer values are compared as if they were integers.

Sanjoy Das seem to have worked on non-integral pointer types which cannot be created "from thin air" by disallowing integer conversion instructions.
http://llvm.org/docs/LangRef.html#nointptrtype

---

### Comment 12 - RalfJung

> Note that to just allocate another byte past the last element of arrays (comment #&#8203;30) for stack variables and globals (and malloc implementations, in particular BumpPtrAllocator) would probably be easiest: It makes the int2ptr function injective. Similar the reason why the size of an empty struct is not zero and malloc(0) returns an unique allocation.

`getelementptr` without `inbounds` is allowed to create pointers that are more than "one past the end", and it is likewise allowed to cast those to integers and back. So, in LLVM, I don't think there is any way to make int2ptr injective.

Also see https://people.mpi-sws.org/~jung/twinsem/twinsem.pdf for a detailed discussion of LLVM pointer semantics.

---

### Comment 13 - Meinersbur

Just mentioning it would be easiest and there is precedence. For stack variables, I initially expected it to only necessary whenever there is pointer arithmetic involving non-constants which is only common for e.g. buffers. However, you're right it might be necessary for any captured variable; especially in C where there a pointer of a scalar variable is an array of size one and pass-by-reference is done by passing the pointer.

There is an aspect of AliasAnalysis that I did not consider: it might only care about dereferenceable pointers. Since the one-past-the-last pointer is not dereferenceable, int2ptr is injective over the set of derefenceable pointers.
However, in the original test case, AliasAnalysis is queried about &x and one-past &y. &x is dereferenceable, but one-past &y is not which BasicAA does not consider.

Not sure whether LLVM's AliasAnalysis interface is only defined for valid pointer representations (ie including one-past-the-last) of just dereferenceable pointers. The latter would be hard for analysis since one had to proof whether a pointer is dereferenceable before an AA query.

---

### Comment 14 - regehr

Michael, my guess is that adding a byte (which of course becomes more than a byte when there are alignment requirements) is a showstopper for memory-constrained embedded use cases. Also note that (I think!) this would have to be done even for stack variables and globals that are not arrays.

---

### Comment 15 - Meinersbur

Note that to just allocate another byte past the last element of arrays (comment #&#8203;30) for stack variables and globals (and malloc implementations, in particular BumpPtrAllocator) would probably be easiest: It makes the int2ptr function injective. Similar the reason why the size of an empty struct is not zero and malloc(0) returns an unique allocation.

---

### Comment 16 - Meinersbur

I came here from https://blog.regehr.org/archives/1737 and can maybe add another perspective.

It seems that the bug is the result of mixing two concepts of a pointer. First, as a symbolic reference to another value, second as the bit-pattern of a memory address. The former could also be represented as a tuple (baseptr,offset), the baseptr being the pointer's provenance. Pointers with different provenance cannot alias.

The problem being that there exist identical address bit-patterns that represent different logical pointers, namely the address of the pointer one past the baseptr's memory allocation can be another allocation.[*]

In the model of pointer as a logical reference, ptr2int is an implementation-defined non-injective (non-surjective) functions. That is, p!=q does not imply ptr2int(p)!=ptr2int(q) because of the validity of one-past-the-end pointers. Therefore, int2ptr is ambiguous: we cannot infer an unique (baseptr,offset) from the integer value. The conversion int2ptr(ptr2int(p)) therefore is 'lossy'.

AliasAnalysis is linked to pointer comparison in that if we can deduce `p+n < q || q+n < p` to be unconditionally false, then they do not not alias.

I hope this formalizes a bit what is also called 'magic', 'opaque' and 'non-deterministic' in this thread. Probably it's not just me who got confused.

Clearly, we want the transformation `(a != b ? b : a) == b` (comment #&#8203;17) to eventually take place on the address bit-pattern. I don't see why the final program needs to run slower on pointers than on integers. Note that, on the IR level, this mixes-up provenance regardless whether a or be are the result of int2ptr (comment #&#8203;43), so a solution that only handles int2ptr will be insufficient, at least for front-end languages that don't make pointer comparison with different provenance undefined.

I can think of two solutions:


1. We introduce two phases into the pass pipeline. In the first phase, pointers are logical references with provenance. The identity of pointers must be kept distinct unless it can be shown that they have the same provenance, especially within GVN. Operations such as `q - p` are undefined (poison?) if q and p have different provenance.

In the second phase, pointers are assumed to be address bit-patterns, i.e. ptr2int is assumed on any pointers. This is a kind of lowering. AliasAnalysis may not assume provenance anymore.


2. We track provenance explicitly (comment #&#8203;54), e.g. using metadata inserted by e.g. the front-end:

    %x = alloca                                                              !provenance !&#8203;0
    %add.ptr = getelementptr inbounds [1 x i32], [1 x i32]* %y, i64 0, i64 1 !provenance !&#8203;1
    %cmp = icmp ne i32* %x, %add.ptr
    %add.ptr.x = select i1 %cmp, i32* %add.ptr, i32* %x

Since provenance is language concept, it makes sense to not just assume the same thing in the IR, especially since there seem to be differences in C++ and C (comment #&#8203;77). If my interpretation is correct, the icmp above is required to be an integer comparison in C11, but can be assumed to be false in C++. In any case, the provenance of %add.ptr.x as to be cleared ("assumed unknown") since it merges pointers of different provenance to a single pointer. Alternatively, provenance could be a set.
Stripping all the provenance matadata would be equivalent to entering phase two of the previous solution.
Unfortunately, this solution increases the overhead into the direction of ScopedAA.



Note that there is the special case of std::less: To make std::set/std::map work, it must be defined on pointers of different provenance. https://en.cppreference.com/w/cpp/utility/functional/less says:

> If the function call operator of the specialization std::less<void> calls a built-in operator comparing pointers, it yields a strict total order even if the built-in operator< does not.

In libc++, I don't see any special mechanism to make on all pointers (https://github.com/llvm/llvm-project/blob/master/libcxx/include/__functional_base#L54). This case might be interesting:

https://godbolt.org/z/mU5nZp

     int x[1], y[1];
     std::set<int*> MySet;
     MySet.insert(&x[1]);
     MySet.insert(&y[0]);
     printf("count = %d\n", (int)MySet.size());



[*] We ignore that within a segmented memory model, or virtual page tables, multiple virtual memory addresses can be mapped to the same physical memory.

---

### Comment 17 - llvmbot

There are many thoughtful and thought-provoking comments here, thanks for this! But one comment has far-reaching consequences and especially resonated with my older thoughts:


> To put it another way, if I have a comparison that gives one answer to some
> observers and other answers to other observers, there may be nothing that
> prevents that logical contradiction from infecting the remainder of the
> execution.

This is a quite powerful principle.

1. An illustration for its application to comparisons of the form `&x == &y + 1` in gcc should be here: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61502#c42. If clang is ever modified to make such comparisons unstable again I guess it would be applicable to clang too. (But it's straightforward to demonstrate the same with bug 44342 for now if anyone wants.)

2. Is it possible to have unfrozen `undef` at all? See bug 44512.

---

### Comment 18 - aqjune


> 
> > Any small example test case about how the magic pointer can break things?
> 
> There is now a bunch of examples in bug 44313 and bug 44374. Mostly they
> provide full simple examples for aspects already touched in this bug in some
> way or another. But the example with `restrict` adds a surprising twist to
> the whole mess.
> 

There was also another bug that involves restrict and int-ptr casting that were reported by us: llvm/llvm-project#39193 
Conclusion was that we need an intrinsic something like @&#8203;llvm.ptr.also_derived_from(base, a, b, c, ...) that can be used for safely replacing a pointer based on pointer equality.
llvm/llvm-project#39193 #c8
That was on my to-do list, but I was busy with all other things including development of alive2 with Nuno (https://github.com/aliveToolkit/alive2 ), so its priority was low.
Actually, what we've recently found with alive2 was that it is safe to replace a pointer with another in certain cases, e.g.

define i8* @&#8203;f(i8* nocapture %p, i8* %q) {
  if (%p == %q) ret %p // this can be optimized to ret %q
  ret %q
}

(alive2 is still in progress; it does not support inttoptr and noalias yet, so wouldn't check your bug and this inttoptr bug, but I'll be happy if this tool can help determine when it's safe to replace pointer in the future).


> 
> > I tried to catch LLVM in the act of making pointer comparison
> > non-deterministic, but actually it seems to try hard to not do so.
> 
> It did in the past but that was fixed in 2014 -- see bug 13507 and bug 21327.
> 
This is very interesting. So LLVM (at least in 2014) regards such behavior as buggy.

---

### Comment 19 - llvmbot


> Any small example test case about how the magic pointer can break things?

There is now a bunch of examples in bug 44313 and bug 44374. Mostly they provide full simple examples for aspects already touched in this bug in some way or another. But the example with `restrict` adds a surprising twist to the whole mess.


> I tried to catch LLVM in the act of making pointer comparison
> non-deterministic, but actually it seems to try hard to not do so.

It did in the past but that was fixed in 2014 -- see bug 13507 and bug 21327.


> as I talked with Reid and Richard, it seems that the devirtualization has
> the same problem right now:
> 
> https://godbolt.org/g/LS9jc4

It seems there was an effort[1][2] to fix the problem as it applies to devirtualization independently from the general case, and the particular testcase was fixed between clang 6 and 7. I've just filed bug 44472 with new testcases.

[1] https://docs.google.com/document/d/16GVtCpzK8sIHNc2qZz6RN8amICNBtvjWUod2SujZVEo
[2] https://www.youtube.com/watch?v=Dt4UehzzcsE

---

### Comment 20 - nunoplopes

For reference, these slides give an illustrated explanation on this bug: http://llvm.org/devmtg/2018-04/slides/Lopes-Sbirlea-Pointers,%20Alias%20and%20ModRef%20Analyses.pdf

---

### Comment 21 - llvmbot


> 
> > 
> > > 
> > > > > It probably happens in practice (why
> > > > > wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> > > > > we cannot fold the comparisons to false.
> > > 
> > > Going back a step: why do we care about folding "p+n==q" to false?  Does this
> > > pattern come up often enough that constant folding it is necessary?
> > 
> > I don't think we do. I think the question is: if I have pointers p and q,
> > and p is based on a and q is based on b, and I have p == q, can I fold that
> > to false? If I need to prove first that p != a + n and q != b + m, then it
> > becomes harder.
> 
> (I feel like I may be rehashing things that have already been settled,
> please let me know if that's the case.)
> 
> That's what I was asking though -- are there cases where folding "p == q" to
> "false" (assuming we can prove based_on(p) != based_on(q)) is important? 

Are you assuming you can never determine q is a constant?
IE these are always variable pointers?

Or are you also considering the case where q is constant?

Because the obvious answer when q is constant nullptr is "null pointer checks".

Past that, this is generally how value profiling would work to remove aliasing, etc, so we should be careful there is some way to make that work (IE comparison of p or q with constant pointers or ranges, usually)

Past all that, when p and q is "guaranteed" to be non-constant, yeah, don't care in practice.

---

### Comment 22 - RalfJung

> By defining the comparison of p+n==q to return a non-deterministic value, it
> allows the compiler to fold the comparison to false, and the hardware to return
> true or false.

> Going back a step: why do we care about folding "p+n==q" to false?  Does this
> pattern come up often enough that constant folding it is necessary?

I tried to catch LLVM in the act of making pointer comparison non-deterministic, but actually it seems to try hard to not do so.  In particular, while it will fold "p+n == q" to false if that's the only use of p and qo, it no longer does so if p or q are escaped: <https://godbolt.org/g/RD91og>.

So, is this "p+n == q --> false" actually handled similar to foldAllocaCmp? (I've always been wondering why that one should only handle alloca, anyway.)

---

### Comment 23 - sanjoy


> 
> > 
> > > > It probably happens in practice (why
> > > > wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> > > > we cannot fold the comparisons to false.
> > 
> > Going back a step: why do we care about folding "p+n==q" to false?  Does this
> > pattern come up often enough that constant folding it is necessary?
> 
> I don't think we do. I think the question is: if I have pointers p and q,
> and p is based on a and q is based on b, and I have p == q, can I fold that
> to false? If I need to prove first that p != a + n and q != b + m, then it
> becomes harder.

(I feel like I may be rehashing things that have already been settled, please let me know if that's the case.)

That's what I was asking though -- are there cases where folding "p == q" to "false" (assuming we can prove based_on(p) != based_on(q)) is important?  Why can't we say that since we can't do provenance based comparisons at runtime, pointer comparisons are equivalent to comparing their bitwise representations at runtime?  This prevents propagateEquality on pointer types though, but it looks like we're fine with that?

---

### Comment 24 - hfinkel


> 
> > > It probably happens in practice (why
> > > wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> > > we cannot fold the comparisons to false.
> 
> Going back a step: why do we care about folding "p+n==q" to false?  Does this
> pattern come up often enough that constant folding it is necessary?

I don't think we do. I think the question is: if I have pointers p and q, and p is based on a and q is based on b, and I have p == q, can I fold that to false? If I need to prove first that p != a + n and q != b + m, then it becomes harder.

---

### Comment 25 - sanjoy


> > It probably happens in practice (why
> > wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> > we cannot fold the comparisons to false.

Going back a step: why do we care about folding "p+n==q" to false?  Does this
pattern come up often enough that constant folding it is necessary?

---

### Comment 26 - hfinkel


> 
> > 
> > > The comparision between pointers from different array is Unspecified
> > > Behavior, not UB. I spoked about it with Richard a few days ago and he said
> > > that they changed wording in a new standard, but the intention was always to
> > > have it as Unspecified Behavior. It also just makes sense, compiler should
> > > not be legal to insert unreachable when it finds out that pointers from
> > > different arrays are compared.
> > 
> > You're right. I am not sure that unspecified behavior is strong enough to
> > let us fold these comparisons away.
> 
> Reading the C++17 standard again, I think it is:
> http://eel.is/c++draft/defns.unspecified
> unspecified behavior: behavior, for a well-formed program construct and
> correct data, that depends on the implementation.
> 
> (my translation: you can return any value; but you cannot trigger UB
> otherwise it wouldn't be well-formed)
> 
> 
> http://eel.is/c++draft/expr.eq#2.1
> (p == q+n) is unspecified
> 
> Since it's up to the implementation, I can define the semantics to be
> "return true/false arbitrarily". We cannot return undef, since it can take
> different values on each use, but we can return a non-deterministic value
> (i.e., freeze(poison) in our model).
> The caveat is that theoretically we need to be careful when duplicating
> icmps. If the C++ program has a single comparison and then we duplicate it
> in the IR for some reason, then both of the comparisons would have to return
> the same value. So if we fold one of the icmps, we would need to guarantee
> that we will fold all the duplicates.

This is exactly the thing that I don't know how to guarantee, at least not without making them strictly no-duplicate (which would be far worse than not folding in this case). I could have the icmp in a function that is called in a loop. We could then unroll the loop and only inline some of the call sites. Maybe for the inlined call sites we fold the comparison, and for the others we don't. There seem to be lots of inlining/unrolling situations in which this could happen.

Maybe we should do this anyway, but I think some thought will be required to define what this means semantically.

> It probably happens in practice (why
> wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> we cannot fold the comparisons to false.

---

### Comment 27 - llvmbot


> 
> > 
> > > The comparision between pointers from different array is Unspecified
> > > Behavior, not UB. I spoked about it with Richard a few days ago and he said
> > > that they changed wording in a new standard, but the intention was always to
> > > have it as Unspecified Behavior. It also just makes sense, compiler should
> > > not be legal to insert unreachable when it finds out that pointers from
> > > different arrays are compared.
> > 
> > You're right. I am not sure that unspecified behavior is strong enough to
> > let us fold these comparisons away.
> 
> Reading the C++17 standard again, I think it is:
> http://eel.is/c++draft/defns.unspecified
> unspecified behavior: behavior, for a well-formed program construct and
> correct data, that depends on the implementation.
> 
> (my translation: you can return any value; but you cannot trigger UB
> otherwise it wouldn't be well-formed)
> 
> 
> http://eel.is/c++draft/expr.eq#2.1
> (p == q+n) is unspecified
> 
> Since it's up to the implementation, I can define the semantics to be
> "return true/false arbitrarily". We cannot return undef, since it can take
> different values on each use, but we can return a non-deterministic value
> (i.e., freeze(poison) in our model).
> The caveat is that theoretically we need to be careful when duplicating
> icmps. If the C++ program has a single comparison and then we duplicate it
> in the IR for some reason, then both of the comparisons would have to return
> the same value. So if we fold one of the icmps, we would need to guarantee
> that we will fold all the duplicates. It probably happens in practice (why
> wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim
> we cannot fold the comparisons to false.

Yes sorry, you are right, I was thinking of deleting the compare instead of folding. If the optimizer figures out the value of address, then I don't see why it would not be able to fold the compare to constant.

---

### Comment 28 - nunoplopes


> 
> > The comparision between pointers from different array is Unspecified
> > Behavior, not UB. I spoked about it with Richard a few days ago and he said
> > that they changed wording in a new standard, but the intention was always to
> > have it as Unspecified Behavior. It also just makes sense, compiler should
> > not be legal to insert unreachable when it finds out that pointers from
> > different arrays are compared.
> 
> You're right. I am not sure that unspecified behavior is strong enough to
> let us fold these comparisons away.

Reading the C++17 standard again, I think it is:
http://eel.is/c++draft/defns.unspecified
unspecified behavior: behavior, for a well-formed program construct and correct data, that depends on the implementation.

(my translation: you can return any value; but you cannot trigger UB otherwise it wouldn't be well-formed)


http://eel.is/c++draft/expr.eq#2.1
(p == q+n) is unspecified

Since it's up to the implementation, I can define the semantics to be "return true/false arbitrarily". We cannot return undef, since it can take different values on each use, but we can return a non-deterministic value (i.e., freeze(poison) in our model).
The caveat is that theoretically we need to be careful when duplicating icmps. If the C++ program has a single comparison and then we duplicate it in the IR for some reason, then both of the comparisons would have to return the same value. So if we fold one of the icmps, we would need to guarantee that we will fold all the duplicates. It probably happens in practice (why wouldn't it?).  I agree it's a potentially risky thing, but I wouldn't claim we cannot fold the comparisons to false.

---

### Comment 29 - nunoplopes

> > Otherwise we would need to
> > instrument the code to detect this case and return false (since otherwise
> > the comparison in hardware will return true because we'll do a simple
> > integer comparison).
> > By defining the comparison of p+n==q to return a non-deterministic value, it
> > allows the compiler to fold the comparison to false, and the hardware to
> > return true or false.
> 
> Exactly. This is what I'd prefer. This may be a nice thing we can't have,
> however. My concern is that there's some reason we can't do that and still
> end up with a model that makes sense. To put it another way, if I have a
> comparison that gives one answer to some observers and other answers to
> other observers, there may be nothing that prevents that logical
> contradiction from infecting the remainder of the execution. Moreover, there
> may be no reasonably way to program the remainder of the program such that
> actual UB is avoided in the presence of arbitrary logical contractions.

(...)
This is pretty cool info; thanks for the pointers. So far I was focusing only on C++ standard, hoping that the C standard would be equal, but apparently not. I agree with your points regarding not being possible to fold comparisons to false if you read the C++ standard to the letter.
It's funny that both GCC and LLVM ignore the standard, but MSVC and ICC do not: https://godbolt.org/g/Xbybrz

IMHO, there should be a difference between comparing pointers and pointers casted to integer. And there is in fact. If we want a memory model based on objects and so on, it makes sense to yield false if we compare 2 different objects.


> > 
> > > As a technical matter, I'm strongly opposed to any transformations
> > > introducing new ptr2int/int2ptr instructions. Unless, that is, we teach AA
> > > to deal with them and also deal with the associated integer arithmetic as
> > > pointer arithmetic.
> > 
> > I agree with that: we should avoid introducing ptr2int, since it's a very
> > strong construct. We are working on a patch that includes improvements to AA
> > to support ptr2int. We'll share the code & benchmarks results in a few weeks
> > hopefully.
> 
> Different topic, but my immediate question is going to be: can we
> canonicalize away from the ptr2int/int2ptr instead of teaching AA about them?

I only have 2 alternatives at the moment: ptr2int or introduce this new union instruction. We will benchmark both and then report back. There's no way of escaping some sort of int2ptr instruction, because it exists to widen the provenance of pointers: normal pointers can only point to one object, while pointers returned by int2ptr may alias 1, 2, or more. Whatever way we "fix" this, we will always want/need to teach AA about this multiple provenance of some pointers, but that still don't alias with everybody.

---

### Comment 30 - llvmbot


> 
> > The comparision between pointers from different array is Unspecified
> > Behavior, not UB. I spoked about it with Richard a few days ago and he said
> > that they changed wording in a new standard, but the intention was always to
> > have it as Unspecified Behavior. It also just makes sense, compiler should
> > not be legal to insert unreachable when it finds out that pointers from
> > different arrays are compared.
> 
> You're right. I am not sure that unspecified behavior is strong enough to
> let us fold these comparisons away.

It does not. By this standard says "language does not give you guarantee that things will be in right order in memory so that the comparision will always produce the same value". It could not be Implementation Defined, because most of the impmentation does not give this guarantee either.

---

