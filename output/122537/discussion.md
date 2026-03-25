# [TBAA][GVN] Miscompile due to incorrectly replacing pointer with undef

**Author:** mshockwave
**URL:** https://github.com/llvm/llvm-project/issues/122537
**Status:** Closed
**Labels:** miscompilation, release:backport, TBAA, llvm:GVN
**Closed Date:** 2025-01-31T11:45:14Z

## Body

This is a relatively intertwinding bug and I think the best way to describe it is through chronological order:

At this moment RISC-V LLVM miscompiles 482.sphinx3 when we enable VLS vectorization + LTO:
```
clang -O3 -flto -target riscv64-linux-gnu -mcpu=sifive-p670 -mrvv-vector-bits=zvl ...
```

If we remove the `-mrvv-vector-bits=zvl` flag (i.e. not using VLS vectorization), the issue goes away. Here are what happened:
  1. With `-mrvv-vector-bits=zvl`, since we use a fixed VLEN all the `llvm.vscale` intrinsic calls will go away. All kinds of code simplifications then kick in and make some of the functions smaller
  2. Some of the these functions started to get inlined, among them is a function (let's name it `allocate2D`) that allocates and returns a 2D array. This is effectively how it got called and used:
```c
void **allocate2D();

int bar (float *p);

int foo() {
  float **buffer = (float **)allocate2D();
  return bar(buffer[0]);
}
```
  3. This is what it looks like after `allocate2D` got inlined
``` llvm
declare i32 @bar(ptr nocapture noundef)

declare dso_local noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #0

define i32 @foo(ptr %src, i64 %num, i64 %N) {
b3:
  %p0 = call noalias ptr @malloc(i64 noundef %num)
  br label %b2

b2:
  %offset = phi i64 [0, %b3], [%iv, %b2]
  %p2 = getelementptr inbounds nuw ptr, ptr %p0, i64 %offset
  store ptr %src, ptr %p2, align 8, !tbaa !4
  %iv = add nuw nsw i64 %offset, 1
  %c = icmp eq i64 %iv, %N
  br i1 %c, label %b1, label %b2

b1:
  %p = load ptr, ptr %p0, align 8, !tbaa !6
  br label %b0

b0:
  %r = call i32 @bar(ptr noundef %p)
  ret i32 %r
}

attributes #0 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" }

!0 = !{!"any pointer", !1, i64 0}
!1 = !{!"omnipotent char", !2, i64 0}
!2 = !{!"Simple C/C++ TBAA"}
!3 = !{!"p1 omnipotent char", !0, i64 0}
!4 = !{!3, !3, i64 0}
!5 = !{!"p1 float", !0, i64 0}
!6 = !{!5, !5, i64 0}
```
b2, and b3 both originally belong to `allocate2D`: b3 mallocs the memory for row pointers in this 2D array and b2 initializes them (the code showed here is NOT what it actually did but merely demonstrating that it's using a loop to initialize the row pointers memory). b1 and b0, on the other hand, grab the first row from this 2D array and pass it to `bar`.

  4. The problem is, if you run this snippet through GVN, `%r = call i32 @bar(ptr noundef %p)` will be turned into `%r = call i32 @bar(ptr noundef undef)`. Which is of course incorrect.
  5. The reason GVN thought it's a good idea to replace %p with undef is twofold: first it uses MemoryDependenceAnalysis to retrieve all the dependencies of `%p = load ptr, ptr %p0, align 8, !tbaa !6`. And for some reasons it only returns `%p0 = call noalias ptr @malloc(i64 noundef %num)` as the only Def dependency and (incorrectly) ignores `store ptr %src, ptr %p2, align 8, !tbaa !4` which might clobber `%p0`. And since `malloc` has the `allockind("uninitialized")` attribute, loading unintialized content is, to my best understandingss, undefined behavior so the optimizer can replace it with whatever it wants, hence the undef value.
  6. The store instruction was ignored because the AA pipeline thought `store ptr %src, ptr %p2` and `%p = load ptr, ptr %p0` have NoAlias, which is rare considered 90% of the time we expected MayAlias and also wrong -- it should return MayAlias for the potential clobber.
  7. Now the problem can be boiled down to the incorrect AA result between the said load and store instructions. This AA query was first processed by BasicAA, which (correctly) returns MayAlias
  8. But when it passes to the subsequent TBAA, it thought they are NoAlias because of their type descriptor graph. Specifically, this is store instruction's tree:
```
<0x555555676658> = !{<0x5555555b33d8>, <0x5555555b33d8>, i64 0}
  <0x5555555b33d8> = !{!"p1 omnipotent char", <0x55555583a118>, i64 0}
    !"p1 omnipotent char"
    <0x55555583a118> = !{!"any pointer", <0x55555583a3c8>, i64 0}
      !"any pointer"
      <0x55555583a3c8> = !{!"omnipotent char", <0x55555583a0d8>, i64 0}
        !"omnipotent char"
        <0x55555583a0d8> = !{!"Simple C/C++ TBAA"}
          !"Simple C/C++ TBAA"
```
and this is load instruction's tree:
```
<0x5555557bdbe8> = !{<0x5555557c9108>, <0x5555557c9108>, i64 0}
  <0x5555557c9108> = !{!"p1 float", <0x55555583a118>, i64 0}
    !"p1 sink"
    <0x55555583a118> = !{!"any pointer", <0x55555583a3c8>, i64 0}
      !"any pointer"
      <0x55555583a3c8> = !{!"omnipotent char", <0x55555583a0d8>, i64 0}
        !"omnipotent char"
        <0x55555583a0d8> = !{!"Simple C/C++ TBAA"}
          !"Simple C/C++ TBAA"
```
And based on TBAA's rule, these two type descriptor graphs are indeed not alias.

Personally I think the TBAA tags are wrong. ~~: the tag for store instructions -- which represents storing `void *` to `void **` -- should probably be _any pointer_.~~

## Comments

### Comment 1 - nikic

Does https://github.com/llvm/llvm-project/pull/122116 fix this?

---

### Comment 2 - mshockwave

> Does [#122116](https://github.com/llvm/llvm-project/pull/122116) fix this?

I'll give it a try (Update: no it doesn't fix this). That said, I looked into the original C code for `allocate2D`:
```c
void **allocate2D (int32 X, int32 Y, int32 elemsize, ...)
{
    char **ref, *mem;
    int32 i, offset;

    mem = (char *) calloc(...);
    ref = (char **) malloc(...);

    for (i = 0, offset = 0; i < X; i++, offset += Y*elemsize)
        ref[i] = mem + offset;

    return ((void **) ref);
}
```
Seems like the TBAA tag for store is for `char *` rather than `void *`. 

---

### Comment 3 - fhahn

> > Does [#122116](https://github.com/llvm/llvm-project/pull/122116) fix this?
> 
> I'll give it a try (Update: no it doesn't fix this). That said, I looked into the original C code for `allocate2D`:
> 
> void **allocate2D (int32 X, int32 Y, int32 elemsize, ...)
> {
>     char **ref, *mem;
>     int32 i, offset;
> 
>     mem = (char *) calloc(...);
>     ref = (char **) malloc(...);
> 
>     for (i = 0, offset = 0; i < X; i++, offset += Y*elemsize)
>         ref[i] = mem + offset;
> 
>     return ((void **) ref);
> }
> Seems like the TBAA tag for store is for `char *` rather than `void *`.



Which tag? IIUC the code is storing `char *` through `ref[I]` and then returning `ref` as `void **`. How is the return value then used to access the data? If it is loading `void *`, then the issue should probably be fixed by #122116. If it's a different type other than `char *`, it is a strict alias violation you probably either have to fix the source code or pass `-fno-strict-aliasing` (or `-fno-pointer-tbaa` .

You can also use `-fsanitize=type` to check for strict alias violations, although the sanitizer is still experimental.

---

### Comment 4 - mshockwave

> > > Does [#122116](https://github.com/llvm/llvm-project/pull/122116) fix this?
> > 
> > 
> > I'll give it a try (Update: no it doesn't fix this). That said, I looked into the original C code for `allocate2D`:
> > void **allocate2D (int32 X, int32 Y, int32 elemsize, ...)
> > {
> > char **ref, *mem;
> > int32 i, offset;
> > ```
> > mem = (char *) calloc(...);
> > ref = (char **) malloc(...);
> > 
> > for (i = 0, offset = 0; i < X; i++, offset += Y*elemsize)
> >     ref[i] = mem + offset;
> > 
> > return ((void **) ref);
> > ```
> > 
> > 
> >     
> >       
> >     
> > 
> >       
> >     
> > 
> >     
> >   
> > }
> > Seems like the TBAA tag for store is for `char *` rather than `void *`.
> 
> Which tag? IIUC the code is storing `char *` through `ref[I]` and then returning `ref` as `void **`. How is the return value then used to access the data? If it is loading `void *`, then the issue should probably be fixed by [#122116](https://github.com/llvm/llvm-project/pull/122116). If it's a different type other than `char *`, it is a strict alias violation you probably either have to fix the source code or pass `-fno-strict-aliasing` (or `-fno-pointer-tbaa` .
> 
> You can also use `-fsanitize=type` to check for strict alias violations, although the sanitizer is still experimental.

While type sanitizer is not supported on RISC-V yet, I confirm that either `-fno-pointer-tbaa` or `-fno-strict-aliasing` fixes this issue.

---

### Comment 5 - fhahn

/cherry-pick 77d3f8a92564b533a3c60a8c8e0657c38fd88ba1

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#125206

---

