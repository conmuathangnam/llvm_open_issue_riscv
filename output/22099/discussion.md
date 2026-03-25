# wrong results with union and strict-aliasing

**Author:** brunodefraine
**URL:** https://github.com/llvm/llvm-project/issues/22099
**Status:** Open
**Labels:** clang:codegen, bugzilla, miscompilation, TBAA

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [21725](https://llvm.org/bz21725) |
| Version | 3.4 |
| OS | Linux |
| CC | @aelovikov-intel,@compnerd,@hfinkel,@kosarev,@dobbelaj-snps,@sunfishcode,@RalfJung,@regehr,@zygoloid,@sanjoy,@seanm |

## Extended Description 


The following testcase produces wrong results with '-O1' and higher (tested with llvm-3.4 and llvm-3.5).
```cpp
#include <stdio.h>

union U {
	short s;
	int i;
} u;

int v = 987;
union U* up = &u;

int main()
{
	for (int i = 0; i < 2; ++i) {
		u.s = v;         // 987
		up->i = 123;     // 123
	}
	printf("%d\n", up->i);   // 123     clang: 123
	printf("%d\n", u.i);     // 123     clang: 987 ******
	printf("%d\n", up->i);   // 123     clang: 987 ******
	printf("%d\n", u.i);     // 123     clang: 987 ******
	return 0;
}
```

Disabling 'strict-aliasing' makes the problem go away.
We do believe that the behavior should be the same with and without strict-aliasing.

It seems that initially, the write to 'u.s' is moved out of the loop.
(more exactly, after the loop)
Later on the loop is removed, with only the write to 'up->i' remaining.
As the store to 'u.s' is still following, the end result is not what we expect to see.

## Comments

### Comment 1 - DimitryAndric

mentioned in issue llvm/llvm-bugzilla-archive#31928

---

### Comment 2 - llvmbot


> So gcc assumes that even differently-typed accesses may overlap, but it
> seems to have little impact on optimizations as in most cases when accesses
> are not meant to overlap, it can prove it. It also means gcc is able to
> compile a substantially larger amount of existing code in strict aliasing
> mode. Does this sound correct?

Yes, I think so but I'm not familiar with gcc implementation, I've only tested it from outside.

> If so, then what gcc does is very clever and supporting this model looks
> like a huge potential improvement to LLVM. And from this perspective all the
> mentioned cases should, indeed, be considered bugs.

I think it could also simplify treatment of unions and placement news. unions are mostly just a more limited case of the same, plus type-punning. The situation with placement news is probably even more interesting. I don't know what plans clang community has in regard to them but it's possible that optimization around placement news will require the same kind of analysis as described above. (I've posted an example with placement news to bug 26603 which I believe is valid according to stricter C++ rules and which illustrates that placement new should serve as a quite strong optimization barrier if the discussed analysis is not employed.)

---

### Comment 3 - kosarev

So gcc assumes that even differently-typed accesses may overlap, but it seems to have little impact on optimizations as in most cases when accesses are not meant to overlap, it can prove it. It also means gcc is able to compile a substantially larger amount of existing code in strict aliasing mode. Does this sound correct?

If so, then what gcc does is very clever and supporting this model looks like a huge potential improvement to LLVM. And from this perspective all the mentioned cases should, indeed, be considered bugs.

Thanks a lot for the pointers.

---

### Comment 4 - llvmbot


> > What I see is that gcc requires differently-types access to not overlap in
> > strict aliasing mode.
> > 
> > $ cat x.c
> > void test(short *ps, int *pi) {
> >    *pi = 5;
> >    *ps = 7;
> >    *pi = *pi + 1;
> > }
> > 
> > $ gcc -O1 -fstrict-aliasing -S x.c
> > $ cat x.s
> > ...
> > test:
> > .LFB0:
> >          .cfi_startproc
> >          movw    $7, (%rdi)
> >          movl    $6, (%rsi)
> >          ret
> 
> Yeah, this is a good example of TBAA in action even in those, "harder"
> conditions -- the read of int via *pi in the last line immediately follows
> the read of short via *ps which is only allowed if they overlap and gcc
Oh, as it's written it's total garbage. Other parts seem fine but in this line I've managed to mix words two times. Please read the last line as following:

the _write_ of short via *ps which is only allowed if they _don't_ overlap and gcc

> rightly optimizes based on that. An example of case when accesses are
> allowed to overlap and this interacts badly with optimization you can find
> in the first gcc bug referenced above and it looks less natural. Hopefully
> this means that such situations are quite rare and supporting them will not
> hamper optimizations too much.
> 
> To reiterate, writes and reads are not equal, only writes may change
> effective type, reads have to have a matching type.

---

### Comment 5 - llvmbot

A bit more simplified:

```
#include <stdlib.h>
#include <stdio.h>

static void test(short *ps, int *pi)
{
  for (int i = 0; i < 1; ++i) {
    *ps = 0; // moved out of loop by "Loop Invariant Code Motion"
    pi[i] = 1;
  }
}

int main()
{
  int *p = malloc(10);

  test((void *)p, p);

  printf("%d\n", *p);
}
```

Results:
```
$ clang -std=c11 -Weverything test.c && ./a.out
1
$ clang -std=c11 -Weverything -O3 test.c && ./a.out
0
```

clang version: clang version 6.0.0 (https://llvm.org/git/clang.git 5f17d5ca46b48156b96f81e7c32d381a0ca65b41) (https://llvm.org/git/llvm.git 4ff96e5fbb1d79afb90f25698c4bf1696e2d63b9)


The same bug in gcc -- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81028 and https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57359 .

---

### Comment 6 - llvmbot


>> Implementing TBAA in such conditions is harder but it's possible as
>> evidenced by gcc.
> 
> Can you please elaborate on this?

For example, I've reported a couple of bugs in gcc in this area:

https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69776
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=70484

and they were promptly fixed. BTW the former one affects clang too -- bug 26603.

Another one is a bug I've reported just a couple of days ago (not found by me; I've just reported it):

https://gcc.gnu.org/bugzilla/show_bug.cgi?id=82697

I'm not idealizing gcc though, there are some old bugs that are not yet fixed (possibly for unrelated reasons).

> What I see is that gcc requires differently-types access to not overlap in
> strict aliasing mode.
> 
> $ cat x.c
> void test(short *ps, int *pi) {
>    *pi = 5;
>    *ps = 7;
>    *pi = *pi + 1;
> }
> 
> $ gcc -O1 -fstrict-aliasing -S x.c
> $ cat x.s
> ...
> test:
> .LFB0:
>          .cfi_startproc
>          movw    $7, (%rdi)
>          movl    $6, (%rsi)
>          ret

Yeah, this is a good example of TBAA in action even in those, "harder" conditions -- the read of int via *pi in the last line immediately follows the read of short via *ps which is only allowed if they overlap and gcc rightly optimizes based on that. An example of case when accesses are allowed to overlap and this interacts badly with optimization you can find in the first gcc bug referenced above and it looks less natural. Hopefully this means that such situations are quite rare and supporting them will not hamper optimizations too much.

To reiterate, writes and reads are not equal, only writes may change effective type, reads have to have a matching type.

---

### Comment 7 - kosarev


> The problem is it's allowed by the C standard.

Yes, I'm aware of that. In real life, however, there are cases when the compiler cannot deduce effective types and active union members, meaning we are either forced to consider differently-typed accesses to be potentially overlapping (relaxed aliasing) or forced to require them to be non-overlapping (strict aliasing). The latter comes with the exception for unions that we can support provided all accesses to union members are written in a way that makes it possible for the compiler to recognize them as such.

> Implementing TBAA in such conditions is harder but it's possible as
> evidenced by gcc.

Can you please elaborate on this?

What I see is that gcc requires differently-types access to not overlap in strict aliasing mode.

$ cat x.c
void test(short *ps, int *pi) {
  *pi = 5;
  *ps = 7;
  *pi = *pi + 1;
}

$ gcc -O1 -fstrict-aliasing -S x.c
$ cat x.s
...
test:
.LFB0:
        .cfi_startproc
        movw    $7, (%rdi)
        movl    $6, (%rsi)
        ret

---

### Comment 8 - llvmbot


> 
> > Unions are a bit more tricky -- a read could be done with a "wrong" type if
> > it's done explicitly through the union (type punning). (That's why I prefer
> > examples with allocated memory instead of unions.)
> 
> Yes, accesses to direct and indirect union members may overlap regardless of
> the final access types. That's why in strict aliasing mode all such accesses
> are required to be performed through an explicit lvalue of the most
> enclosing union type. This is how the compiler knows these accesses may
> overlap with same- or differently-typed accesses to direct or indirect
> members of that union type.

We are in agreement about reads of inactive members of unions but writes differ. AIUI C++ requires writes to inactive members of unions to go through unions (this undoubtly has some benefits, e.g. it allows to disciminate between several members of the same type). OTOH C doesn't have such restriction or at least it's so terse/moot about this issue that you can read it any way you. But it's not that important given the freedom allowed with allocated memory.

> In your example, there are no clues the compiler could use to deduce that
> the short and int pointers the test() function takes may point to
> overlapping objects. With TBAA enabled, dereferences of these pointers are
> considered to be non-overlapping accesses as they have different types. If
> you want the compiler to consider them as potentially overlapping, you have
> to disable TBAA.

The problem is it's allowed by the C standard. C11, 6.5p6, reads: "If a value is stored into an object having no declared type through an lvalue having a type that is not a character type, then the type of the lvalue becomes the effective type of the object for that access and for subsequent accesses that do not modify the stored value."

I'm not aware of serious attempts to abandon the possibility to reuse allocated memory in C. But maybe I've missed them?

Implementing TBAA in such conditions is harder but it's possible as evidenced by gcc.

---

### Comment 9 - kosarev


> Unions are a bit more tricky -- a read could be done with a "wrong" type if
> it's done explicitly through the union (type punning). (That's why I prefer
> examples with allocated memory instead of unions.)

Yes, accesses to direct and indirect union members may overlap regardless of the final access types. That's why in strict aliasing mode all such accesses are required to be performed through an explicit lvalue of the most enclosing union type. This is how the compiler knows these accesses may overlap with same- or differently-typed accesses to direct or indirect members of that union type.

In your example, there are no clues the compiler could use to deduce that the short and int pointers the test() function takes may point to overlapping objects. With TBAA enabled, dereferences of these pointers are considered to be non-overlapping accesses as they have different types. If you want the compiler to consider them as potentially overlapping, you have to disable TBAA.

---

### Comment 10 - llvmbot

I mean that if you see two consecutive reads of different types than you can deduce that they don't overlap and you can reorder all accesses (writes and reads) done through these lvalues. The same if you see consecutive write and read of different types.

Unions are a bit more tricky -- a read could be done with a "wrong" type if it's done explicitly through the union (type punning). (That's why I prefer examples with allocated memory instead of unions.)

---

### Comment 11 - kosarev


Well, if there are only non-volatile read accesses, then I guess we can reorder them regardless of whether they may overlap. We don't need TBAA to respond in a specific way to do this.

In presence of write accesses, however, overlapping accesses do matter. Since the purpose of TBAA is to tell us whether a couple of accesses are allowed to overlap by the rules of the input language, permitting accesses of different types to overlap, like in your test case, effectively means disabling TBAA.

---

### Comment 12 - llvmbot


> As your modified example, it seems there is no practical way for TBAA to
> support such cases. If we permit accesses of different types to alias, then
> there is no work to do for TBAA.

It depends on type of accesses. Based on types information only, you cannot reorder writes as they can change effective type of memory but you can reorder reads etc.

---

### Comment 13 - kosarev

It's not really fixed, just "pessimized" accesses to union members. See

https://reviews.llvm.org/D33328

for more details. I would suggest that we do not close any bug reports of this kind until we have a proper solution.

As your modified example, it seems there is no practical way for TBAA to support such cases. If we permit accesses of different types to alias, then there is no work to do for TBAA.

---

### Comment 14 - llvmbot

The original testcase seems to be fixed now but replacing the union by allocated memory makes the problem come back.

Source code:

```cpp
#include <stdlib.h>
#include <stdio.h>

int v = 987;

void test(short *ps, int *pi, int *pi2)
{
  for (int i = 0; i < 2; ++i) {
    *ps = v;
    *pi = 123;
  }
  printf("%d\n", *pi);
  printf("%d\n", *pi2);
  printf("%d\n", *pi);
  printf("%d\n", *pi2);
}

int main()
{
  void *p = malloc(10);

  test(p, p, p);
}
```

Results:
```
$ clang -std=c11 -Wall -O3 test.c && ./a.out
123
987
987
987
```

clang version: clang version 6.0.0 (https://llvm.org/git/clang.git f027325999d75572cbdb4dda2e475bd27bcc74da) (https://llvm.org/git/llvm.git f7bb38ca4e1c9eb290f7c4c9dbfcb9593f1b140f)

---

### Comment 15 - hfinkel


> Sounds good. Jeroen, can you make a patch with a test case (or just add it
> to  test/CodeGen/tbaa-struct.cpp) and send it to the cfe-commits list for
> review?

This was posted to https://reviews.llvm.org/D8056 but then abandoned because of other bugs uncovered. Please see this thread for more information: http://lists.llvm.org/pipermail/cfe-dev/2015-March/042015.html

---

### Comment 16 - hfinkel


> Sounds good. Jeroen, can you make a patch with a test case (or just add it
> to  test/CodeGen/tbaa-struct.cpp) and send it to the cfe-commits list for
> review?

This was posted to https://reviews.llvm.org/D8056 but then abandoned because of other bugs uncovered. Please see this thread for more information: http://lists.llvm.org/pipermail/cfe-dev/2015-March/042015.html

---

### Comment 17 - hfinkel

Sounds good. Jeroen, can you make a patch with a test case (or just add it to  test/CodeGen/tbaa-struct.cpp) and send it to the cfe-commits list for review?

---

### Comment 18 - llvmbot

So the after metadata looks correct after, so i'm on board with this.

I think it's the right path.

I don't know if there are other bugs lurking that caused this disablement in the first place though.

(I also think Chris is correct in the bug that was duped to this one. There is nothing you can do in the general case, because you can always play games with where the union occurs. But this will at least get us "as right as other compilers generally are" :P)

---

### Comment 19 - dobbelaj-snps

[Output for 'clang -g -O1 --emit-llvm'. _With_ the change](https://user-images.githubusercontent.com/71122038/143750461-57018e0a-7717-429d-b9d6-6688938fc0eb.gz)
The main difference is that we now (with the patch) have a member access (to `!39`):
```llvm
!37 = !{!"int", !32, i64 0}
!39 = !{#40, !41, i64 0}
!40 = !{!"U", !41, i64 0, !37, i64 0}
!41 = !{!"short", !32, i64 0}


in stead of basic type access (to !41) that we had before (without the patch):
!41 = !{#42, !42, i64 0}
!42 = !{!"short", !32, i64 0}

---

### Comment 20 - dobbelaj-snps

[Output for 'clang -g -O1 --emit-llvm'. Without the change](https://user-images.githubusercontent.com/71122038/143750462-3df12b55-6b83-432f-ada9-84a1f91b150b.gz)

---

### Comment 21 - llvmbot


> Created attachment 13970 [details]
> Proposed fix
> 
> This patch enables 'tbaa path' for union member access at the clang side. It
> resolves the described issue, without triggering testcase regressions.
> 
> Does this look good ? Or is there a path in the core llvm where these
> changes will have unexpected effects ?
> If this is the way forward, I can create a unittest to track this behavior.
> (similar to clang/test/CodeGen/tbaa-struct.cpp)



Can you paste what metadata looks like with this patch?

---

### Comment 22 - dobbelaj-snps

[Proposed fix](https://user-images.githubusercontent.com/71122038/143750463-851005cd-4441-4fcc-b185-ec5f28cba3a8.gz)
This patch enables 'tbaa path' for union member access at the clang side. It resolves the described issue, without triggering testcase regressions.

Does this look good ? Or is there a path in the core llvm where these changes will have unexpected effects ?
If this is the way forward, I can create a unittest to track this behavior.
(similar to clang/test/CodeGen/tbaa-struct.cpp)

---

### Comment 23 - dobbelaj-snps

*** Bug llvm/llvm-project#12338 has been marked as a duplicate of this bug. ***

---

### Comment 24 - llvmbot

As a side note,

Looking at the output, the TBAA tree from clang and how it gets used is very different than what gcc does.  I'm pretty sure what clang produces is wrong.

GCC produces a TBAA tree for the union type, and makes int and short descendents.


IE
```
 union
  /   \
 /     \
int    short
```

clang produces a tree that has an anonymous type for the union.
But it never has int and short as descendents

This is true even if you try to force it by making a union copy in main.
( by doing
```
  union U f;
     f = u;
```
)



Instead, the tree clang gives is:

```
        char
 /       |      \
union   int      short
```


This is, IMHO, clearly wrong for the union.

int and short should be descendants of the union

That way, any access to int is considered a possible read/write to the union (which is right), and any access to the union is considered a possible read/write to the int.

I do not know if this is causing *this* bug, but it seems very wrong to me.

(maybe someone smarter can explain what i'm missing here :P)

---

### Comment 25 - llvmbot

So, 
1. I agree this is completely valid and should print 123 4 times
2. (I can debug it myself, but figure i'd ask if someone has looked)

Is this failing due to the struct path queries, or just the regular low level TBAA queries.

I can see how the former would happen, i have trouble seeing how the latter happens.

---

### Comment 26 - hfinkel

It seems as though the way of handling this, to adhere to the standard, is only to use TBAA for (read <-> write) aliasing queries, not (write <-> write) queries, because for (write <-> write) queries, the writes could be changing the effective type of the underlying storage.

This would not apply to objects with fixed effective type. Perhaps for such objects, we could tag the alloca or global with the TBAA metadata directly? Then if we had a (write <-> write) query, and we could see that the underlying objects had fixed effective type, we could continue to use TBAA to answer the AA query.

---

### Comment 27 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Surprisingly, this is a question about union aliasing on which the C standard is actually clear. C's effective type rules say that this program is valid and is required to print 123 four times (and more generally you can't reorder a store past a store with a different type unless you can prove that one of the stores is to an object with a fixed effective type[1] or that the stores refer to disjoint storage).

There's a separate question of whether we should follow what C says here (or come up with some saner rules), but I think whatever we do, the attached testcase should behave as intended.


 [1] You get a fixed effective type if the object was created by a declaration rather than by (say) malloc.

---

### Comment 28 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Bruno De Fraine (brunodefraine)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [21725](https://llvm.org/bz21725) |
| Version | 3.4 |
| OS | Linux |
| CC | @<!-- -->aelovikov-intel,@<!-- -->compnerd,@<!-- -->hfinkel,@<!-- -->kosarev,@<!-- -->dobbelaj-snps,@<!-- -->sunfishcode,@<!-- -->RalfJung,@<!-- -->regehr,@<!-- -->zygoloid,@<!-- -->sanjoy,@<!-- -->seanm |

## Extended Description 


The following testcase produces wrong results with '-O1' and higher (tested with llvm-3.4 and llvm-3.5).
```cpp
#include &lt;stdio.h&gt;

union U {
	short s;
	int i;
} u;

int v = 987;
union U* up = &amp;u;

int main()
{
	for (int i = 0; i &lt; 2; ++i) {
		u.s = v;         // 987
		up-&gt;i = 123;     // 123
	}
	printf("%d\n", up-&gt;i);   // 123     clang: 123
	printf("%d\n", u.i);     // 123     clang: 987 ******
	printf("%d\n", up-&gt;i);   // 123     clang: 987 ******
	printf("%d\n", u.i);     // 123     clang: 987 ******
	return 0;
}
```

Disabling 'strict-aliasing' makes the problem go away.
We do believe that the behavior should be the same with and without strict-aliasing.

It seems that initially, the write to 'u.s' is moved out of the loop.
(more exactly, after the loop)
Later on the loop is removed, with only the write to 'up-&gt;i' remaining.
As the store to 'u.s' is still following, the end result is not what we expect to see.
</details>


---

