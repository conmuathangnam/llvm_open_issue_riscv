# [DAGCombiner, SystemZ]  WRONG code

**Author:** JonPsson1
**URL:** https://github.com/llvm/llvm-project/issues/121306
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2024-12-30T16:26:09Z

## Body

A bisect shows 31b7d4333a6c10aa8b7e1a7ca5aa0e281f124ec2 `"Extend extract_element(bitcast(scalar_to_vector(X))) -> trunc(srl(X,C))"` to be the first bad commit, and the problem also goes away on main if this one commit is reverted.

This simple program is supposed to print 0, as both 'g' and 'b' are 0 throughout the program, so the expression assigning 'cPtr' is always false.

```
int printf(const char *, ...);
long a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;
static char Bytes[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
void foo(char) {}

int main() {
  if (a)
    for (;; d++)
      Bytes[0] = 0;

  long *ePtr = &e;
  long *cPtr = &c;
  for (int IV = 2; IV <= 7; IV++) {
    char Arg0 = Bytes[IV + 1] ^ f;
    *cPtr = ((g || (*ePtr = b)) >= Bytes[IV + 1]);
    foo(Arg0);
  }

  printf("%d\n", c);
}
```
The LoopVectorizer creates a vectorized loop which later optimizations turn into a block of code containing vector (<2 x i8>) operations. The comparison of Bytes[8] is made by first loading it like this:

```
  %scevgep = getelementptr i8, ptr @Bytes, i64 %lsr.iv.next
  %scevgep26 = getelementptr i8, ptr %scevgep, i64 7
  %wide.load.le = load <2 x i8>, ptr %scevgep26, align 1, !tbaa !12
  %8 = extractelement <2 x i8> %wide.load.le, i64 1
  %conv8.le = zext i8 %8 to i32
```

  Before DAGCombiner (Type-legalized selection DAG) the nodes look like this:

```
          t77: i32,ch = load<(invariant load (s16) from %ir.scevgep1, align 1, !tbaa !12), anyext from i16> t0, t64, undef:i64
        t71: v8i16 = scalar_to_vector t77
      t72: v16i8 = bitcast t71
    t76: i32 = extract_vector_elt t72, Constant:i32<1>

```
  The guilty commit performs this dag combine:

```
    Combining: t76: i32 = extract_vector_elt t72, Constant:i32<1>
    ... into: t99: i32 = srl t77, Constant:i32<16>

```
This seems broken to me: 2 bytes are loaded, containing the final <2 x i8> vector, but then those bytes are immediately shifted out to the right. This would have worked if in fact an i32 was loaded, but this is an extended i16 load.

If those two bytes are loaded as an integer, the order of the elements are reversed with BigEndian so the i32 becomes (extended i16 load) [0089]. So the correct shift amount should be 0, to keep the last byte in its place.

If the two bytes are loaded as a vector, the order in the vector becomes <..., 9, 8>, where the 8 is element 0. Therefore element 1 contains the last byte in this case.

The result is that instead of the final '9', a '0' from the upper half of the i16->i32 extended register is used, and the cPtr is assigned a 1, which is incorrect.
  
  clang -march=z16 -O2  wrong0.i -o a.out -w -mllvm -unroll-count=1 -mllvm -unroll-full-max-count=1
  llc -mtriple=s390x-linux-gnu -mcpu=z15 -O3 ./wrong0.ll
[tc_dagcombiner_extract_el.tar.gz](https://github.com/user-attachments/files/18271304/tc_dagcombiner_extract_el.tar.gz)

@RKSimon @davemgreen @uweigand 

## Comments

### Comment 1 - RKSimon

What seems to be broken is handling for ISD::SCALAR_TO_VECTOR implicit truncation - #117900 just made it more likely to be exposed

---

### Comment 2 - RKSimon

Thanks @JonPsson1 - I have an incoming fix that will just disable the fold for implicit truncation cases for now.

---

### Comment 3 - RKSimon

@JonPsson1 Please can you confirm this fixes the original issue you were seeing?

---

### Comment 4 - JonPsson1

> [@JonPsson1](https://github.com/JonPsson1) Please can you confirm this fixes the original issue you were seeing?

Yes - the right checksum is printed now also with -O2 with your patch applied - both with the reduced and original csmith input.



---

