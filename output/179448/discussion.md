# Miscompile with _BitInt

**Author:** bjope
**URL:** https://github.com/llvm/llvm-project/issues/179448
**Status:** Open
**Labels:** clang:codegen, regression, miscompilation

## Body

The test case here show a regression on trunk for clang-23: https://godbolt.org/z/xT7zG9ed9

What we have is a test case like this on C level:
```c
#include <stdio.h>

signed _BitInt(121) idbi(signed _BitInt(121) b) {
  return b;
}

unsigned int test_zero_llrr(unsigned int tnr) {
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x << 35;
    tnr += 0x100U + (res1 == 0 ? 0 : 1) ;
    x = idbi(0);
    res2 = x << 35;
    tnr += 0x200U + (res2 == res1 ? 0 : 2) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x << 23;
    tnr += 0x400U + (res1 == 0 ? 0 : 4) ;
    x = idbi(0);
    res2 = x << 23;
    tnr += 0x800U + (res2 == res1 ? 0 : 8) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x >> 61;
    tnr += 0x1000U + (res1 == 0 ? 0 : 16) ;
    x = idbi(0);
    res2 = x >> 61;
    tnr += 0x2000U + (res2 == res1 ? 0 : 32) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x >> 23;
    tnr += 0x4000U + (res1 == 0 ? 0 : 64) ;
    x = idbi(0);
    res2 = x >> 23;
    tnr += 0x8000U + (res2 == res1 ? 0 : 128) ;
  }
  return tnr;
}

int main(void)
{
  unsigned int tnr = test_zero_llrr(0);
  printf("0x%x\n", tnr);

  return tnr % 0xff;
}
```

Expected printout is "0xff00", but after commit a52177421743 ([pull request #177578](https://github.com/llvm/llvm-project/pull/177578)) the test case started to fail like this (when using x86-64 as a target and enabling ubsan):
```
> clang -O2 -fsanitize=undefined -fno-sanitize-recover=undefined test.c -o a.out
> ./a.out
0xff0f
```

I'm not quite sure if #177578 is to blame for this, or if instruction selection just is a victim for earlier misbehavior in the front/middle end.

One thing I've noticed is that the "idbi" function result in IR such as:
```llvm
  %3 = alloca i121, align 8
  ...
  %12 = trunc i128 %11 to i121
  store i121 %12, ptr %3, align 8
  %13 = load { i64, i64 }, ptr %3, align 8
  ret { i64, i64 } %13
```
which looks a bit suspicious. It stores an i121 and then loads { i64, i64 }.

This example show that changing the store to an i128 store would avoid the miscompile: https://godbolt.org/z/vabnsc11r

Maybe the real problem is a bit more related to commit 9ad72df55cb74b291932 ([#91364](https://github.com/llvm/llvm-project/pull/91364)), which changed a lot around ABI lowering to no longer use the non-byte-sized types. Here we still use i121 for the temporary return value alloca.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Björn Pettersson (bjope)

<details>
The test case here show a regression on trunk for clang-23: https://godbolt.org/z/xT7zG9ed9

What we have is a test case like this on C level:
```
#include &lt;stdio.h&gt;

signed _BitInt(121) idbi(signed _BitInt(121) b) {
  return b;
}

unsigned int test_zero_llrr(unsigned int tnr) {
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x &lt;&lt; 35;
    tnr += 0x100U + (res1 == 0 ? 0 : 1) ;
    x = idbi(0);
    res2 = x &lt;&lt; 35;
    tnr += 0x200U + (res2 == res1 ? 0 : 2) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x &lt;&lt; 23;
    tnr += 0x400U + (res1 == 0 ? 0 : 4) ;
    x = idbi(0);
    res2 = x &lt;&lt; 23;
    tnr += 0x800U + (res2 == res1 ? 0 : 8) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x &gt;&gt; 61;
    tnr += 0x1000U + (res1 == 0 ? 0 : 16) ;
    x = idbi(0);
    res2 = x &gt;&gt; 61;
    tnr += 0x2000U + (res2 == res1 ? 0 : 32) ;
  }
  {
    signed _BitInt(121) x;
    signed _BitInt(121) res1, res2;
    x = idbi(0);
    res1 = x &gt;&gt; 23;
    tnr += 0x4000U + (res1 == 0 ? 0 : 64) ;
    x = idbi(0);
    res2 = x &gt;&gt; 23;
    tnr += 0x8000U + (res2 == res1 ? 0 : 128) ;
  }
  return tnr;
}

int main(void)
{
  unsigned int tnr = test_zero_llrr(0);
  printf("0x%x\n", tnr);

  return tnr % 0xff;
}
```

Expected printout is "0xff00", but after commit a52177421743 ([pull request #<!-- -->177578](https://github.com/llvm/llvm-project/pull/177578)) the test case started to fail like this (when using x86-64 as a target and enabling ubsan):
```
&gt; clang -O2 -fsanitize=undefined -fno-sanitize-recover=undefined test.c -o a.out
&gt; ./a.out
0xff0f
```

I'm not quite sure if #<!-- -->177578 is to blame for this, or if instruction selection just is a victim for earlier misbehavior in the front/middle end.

One thing I've noticed is that the "idbi" function result in IR such as:
```
  %3 = alloca i121, align 8
  ...
  %12 = trunc i128 %11 to i121
  store i121 %12, ptr %3, align 8
  %13 = load { i64, i64 }, ptr %3, align 8
  ret { i64, i64 } %13
```
which looks a bit suspicious. It stores an i121 and then loads { i64, i64 }.

This example show that changing the store to an i128 store would avoid the miscompile: https://godbolt.org/z/vabnsc11r

Maybe the real problem is a bit more related to commit 9ad72df55cb74b291932 ([#<!-- -->91364](https://github.com/llvm/llvm-project/pull/91364)), which changed a lot around ABI lowering to no longer use the non-byte-sized types. Here we still use i121 for the temporary return value alloca.
</details>


---

### Comment 2 - Fznamznon

I vaguely remember details since I'm (still) an amateur in clang CodeGen, but I think yeah, the idea behind https://github.com/llvm/llvm-project/pull/91364 was that clang should not generate stores/loads of i121 or any other non-byte-sized number of bits. Instead, when working with memory, a power of 8 number of bits should be used. cc @efriedma-quic @rjmccall 

---

### Comment 3 - efriedma-quic

We try not to use generate i121 because there are bad interactions with LLVM datalayout rules.  That shouldn't be relevant here, though; a 128-bit load from an `alloca i121` is perfectly valid.

---

### Comment 4 - rjmccall

Yeah. With that said, it worries me that something is generating an `alloca i121`. If something in CodeGen is creating an alloca of the scalar type and then using the from-memory / to-memory load/store operations on it, there may be some case where that could miscompile.

---

### Comment 5 - bjope

It is not really the type of the alloca that made me think the behavior in the frontend was bad.

Afaik the LangRef isn't saying where padding bits go when storing i121 to memory. And the semantic description of load (https://llvm.org/docs/LangRef.html#id214) says 

> When loading a value of a type like i20 with a size that is not an integral number of bytes, the result is undefined if the value was not originally written using a store of the same type.

So is it really well defined what should happen when storing `i121` and then loading `i128` (or loading an aggregate such as `{i64, i64}` as we do here)?

I however agree that it isn't obviously wrong either. It is basically just some sort of type punning by storing and then loading back from memory.

-----

I have tried to reduce the test a bit based on the assumption that the output from the frontend is correct.

First suspect after reducing/bisecting would then still be the patch related to poison at ISel: https://github.com/llvm/llvm-project/commit/a521774217437ef87576ff275c80a92fd6e25cc0

If also assuming that the fault is earlier in the pipeline (using a fixed llc version including that patch) and continuing bisecting one will end up at https://github.com/llvm/llvm-project/commit/58cffea94a31e52d6492ce7103e04c6b073dee16 . But that commit does not look very suspicious, but the canonicalization at instcombine enables SLP vectorization, which seems to be needed to show the problem (haven't seen the fault with `-vectorize-slp=0`, but haven't really seen anything pointing at SLP either).

So maybe the problem really is related to commit/a521774217437ef875. Unfortunately my attempts to reduce the IR (using llvm-reduce) mostly end up in IR that has lots of poison already in the before ISel. Maybe I need to find some better strategy for how to reduce it.

---

### Comment 6 - efriedma-quic

Using -opt-bisect-limit points to isel... so either it's just an isel issue, or SLP is somehow introducing poison.  isel seems like a more likely culprt, unfortunately.

---

### Comment 7 - rjmccall

Oh, I would agree that we shouldn't be storing as an `i121` and then loading as some other type, regardless of whether IR guarantees sufficient storage in the alloca. Is this doing a coercion for something like call argument lowering? That should really be doing a to-memory conversion before the store.

---

### Comment 8 - rjmccall

Also, even if this is a backend bug, sidestepping backend bugs is a more-or-less legitimate reason to avoid emitting "weird" IR. By all means, fix the backend bug at the same time, but I don't think we should feel guilty about making lowering decisions that put less pressure on LLVM.

---

### Comment 9 - bjope

So far I've found two backend bugs related to poison handling when investigating this a bit more:
1) DAGCombiner::visitINSERT_VECTOR_ELT can turn an insert into AND/OR operation, not taking into consideration that the source vector can have poison elements (and for example `AND poison, 0` is poison and not zero).
2) TargetLowering::SimplifyDemandedVectorElts is not demanding elements from Op0 if Op1 is known to be zero for those elements. That code is similarly not considering that a recursive call to SimplifyDemandedVectorElts on Op0 may turn those elements into poison, making the result more poisonous.

I'll prepare some patches for those backend bugs.

---

