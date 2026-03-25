# vector miscompile from AArch64 backend

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/125989
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, release:backport, NEON
**Closed Date:** 2025-02-07T16:37:07Z

## Body

please bear with me here, this is the best I could get out of llvm-reduce:
```llvm
define i16 @f(<8 x i16> %0) {
  %2 = shufflevector <8 x i16> %0, <8 x i16> zeroinitializer, <8 x i32> <i32 4, i32 5, i32 6, i32 7, i32 9, i32 7, i32 5, i32 3>
  %3 = zext <8 x i16> %2 to <8 x i64>
  %new0 = add <8 x i64> %3, %3
  %last = trunc <8 x i64> %new0 to <8 x i16>
  %4 = extractelement <8 x i16> %last, i32 0
  ret i16 %4
}
```
Alive thinks (and I agree) that when this is called like this:
```
call i16 @f(<8 x i16> <i16 0, i16 3, i16 3, i16 3, i16 1, i16 3, i16 3, i16 3>)
```
the result should be 2.

but when I compile to AArch64 using current llc I get:
```
_f:
	movi.2d	v1, #0x00ffff0000ffff
	uzp1.4s	v0, v0, v0
	and.16b	v0, v0, v1
	add.4s	v0, v0, v0
	xtn.4h	v0, v0
	umov.h	w0, v0[0]
	ret
```
and this -- for that same input -- computes 0

cc @hatsunespica @nunoplopes 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
please bear with me here, this is the best I could get out of llvm-reduce:
```llvm
define i16 @<!-- -->f(&lt;8 x i16&gt; %0) {
  %2 = shufflevector &lt;8 x i16&gt; %0, &lt;8 x i16&gt; zeroinitializer, &lt;8 x i32&gt; &lt;i32 4, i32 5, i32 6, i32 7, i32 9, i32 7, i32 5, i32 3&gt;
  %3 = zext &lt;8 x i16&gt; %2 to &lt;8 x i64&gt;
  %new0 = add &lt;8 x i64&gt; %3, %3
  %last = trunc &lt;8 x i64&gt; %new0 to &lt;8 x i16&gt;
  %4 = extractelement &lt;8 x i16&gt; %last, i32 0
  ret i16 %4
}
```
Alive thinks (and I agree) that when this is called like this:
```
call i16 @<!-- -->f(&lt;8 x i16&gt; &lt;i16 0, i16 3, i16 3, i16 3, i16 1, i16 3, i16 3, i16 3&gt;)
```
the result should be 2.

but when I compile to AArch64 using current llc I get:
```
_f:
	movi.2d	v1, #<!-- -->0x00ffff0000ffff
	uzp1.4s	v0, v0, v0
	and.16b	v0, v0, v1
	add.4s	v0, v0, v0
	xtn.4h	v0, v0
	umov.h	w0, v0[0]
	ret
```
and this -- for that same input -- computes 0

cc @<!-- -->hatsunespica @<!-- -->nunoplopes 
</details>


---

### Comment 2 - davemgreen

Thanks for the report. I think performZExtDeinterleaveShuffleCombine might be missing an extra check for undef elements.



---

### Comment 3 - davemgreen

/cherry-pick 2c43479683651f0eb208c97bf12e49bacbea4e6f

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#126263

---

