# [HLSL] Matrix constructors is using the initalizer index when it needs to convert the index to a column major index

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/165663
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-11-05T03:22:26Z

## Body

Today Matrix initalization is just taking in the index from the initalizer list:
https://github.com/llvm/llvm-project/blob/15bbdd143cb196be8d60ea4dc813bf7bc5e4650c/clang/lib/Sema/SemaInit.cpp#L1907-L1913

This means we aren't reordering the index to account for layout DXC is expecting.

See https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-per-component-math#matrix-ordering


The fix seems to be
```diff
@@ -1906,10 +1906,12 @@ void InitListChecker::CheckMatrixType(const InitializedEntity &Entity,
 
   while (NumEltsInit < MaxElts && Index < IList->getNumInits()) {
     // Not a sublist: just consume directly.
-    ElemEnt.setElementIndex(Index);
-    CheckSubElementType(ElemEnt, IList, ElemTy, Index, StructuredList,
+    unsigned ColMajorIndex = (Index % MT->getNumRows()) * MT->getNumColumns() + (Index / MT->getNumRows());
+    ElemEnt.setElementIndex(ColMajorIndex);
+    CheckSubElementType(ElemEnt, IList, ElemTy, ColMajorIndex, StructuredList,
                         StructuredIndex);
     ++NumEltsInit;
+    ++Index;
   }
```

## Comments

### Comment 1 - Icohedron

I don't think this is actually an issue. 
Clang and DXC currently generate functionally-equivalent IR for matrix construction/initialization and subscripting/indexing.
DXC just transforms and loads elements from matrices in a way that makes things confusing.

Edit: I realize now the IR is actually not functionally equivalent. I notice Clang isn't consistent with using row- versus column-major ordering between matrix initialization and computing the flat index. In the below example, the matrix is initialized in row-major order, but the flat index being computed is row + col * 2 which is a flat index intended for a column-major order matrix. 

https://hlsl.godbolt.org/z/57MnrTaYY

HLSL:
```hlsl
// compile args: -T lib_6_8
export int Foo(uint row, uint col) {
  int2x3 M = int2x3(0, 1, 2, 3, 4, 5);
  return M[row][col];
}
```

Clang:
```llvm
define i32 @Foo(unsigned int, unsigned int)(i32 %0, i32 %1) local_unnamed_addr #0 !dbg !22 {
; Construct the matrix M (%3) from the elements given to the constructor in row-major order.
  %3 = alloca [6 x i32], align 4, !dbg !32
  %4 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 0, !dbg !32
  store i32 0, ptr %4, align 4, !dbg !32
  %5 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 1, !dbg !32
  store i32 1, ptr %5, align 4, !dbg !32
  %6 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 2, !dbg !32
  store i32 2, ptr %6, align 4, !dbg !32
  %7 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 3, !dbg !32
  store i32 3, ptr %7, align 4, !dbg !32
  %8 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 4, !dbg !32
  store i32 4, ptr %8, align 4, !dbg !32
  %9 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 5, !dbg !32
  store i32 5, ptr %9, align 4, !dbg !32
; Compute the index for the matrix: col * 2 + row == %1 * 2 + %0
  %10 = shl i32 %1, 1, !dbg !32
  %11 = add i32 %10, %0, !dbg !32
; Bounds check
  %12 = icmp ult i32 %11, 6, !dbg !32
  tail call void @llvm.assume(i1 %12), !dbg !32
; No-op address calculation from no-op GEP transformation.
  %13 = mul i32 %11, 1, !dbg !32
  %14 = add i32 0, %13, !dbg !32
; Load from the matrix at element col * 2 + row.
  %15 = getelementptr inbounds [6 x i32], ptr %3, i32 0, i32 %14, !dbg !32
  %16 = load i32, ptr %15, align 4, !dbg !32
  ret i32 %16, !dbg !33
}
```

DXC:
```llvm
; Define a global constant holding the result of a row-major to column-major order transformation of the compile-time constant matrix M.
@.hca = internal unnamed_addr constant [6 x i32] [i32 0, i32 3, i32 1, i32 4, i32 2, i32 5]

define i32 @"\01?Foo@@YAHII@Z"(i32 %row, i32 %col) #0 {
; Construct an array of indices [ row, row + 2, row + 4 ]
  %1 = alloca [3 x i32], align 4
  %2 = add i32 %row, 2, !dbg !47 ; line:3 col:10
  %3 = add i32 %row, 4, !dbg !47 ; line:3 col:10
  %4 = getelementptr [3 x i32], [3 x i32]* %1, i32 0, i32 0, !dbg !47 ; line:3 col:10
  store i32 %row, i32* %4, align 4, !dbg !47 ; line:3 col:10
  %5 = getelementptr [3 x i32], [3 x i32]* %1, i32 0, i32 1, !dbg !47 ; line:3 col:10
  store i32 %2, i32* %5, align 4, !dbg !47 ; line:3 col:10
  %6 = getelementptr [3 x i32], [3 x i32]* %1, i32 0, i32 2, !dbg !47 ; line:3 col:10
  store i32 %3, i32* %6, align 4, !dbg !47 ; line:3 col:10
; Load load the col'th element from the array [ row, row + 2, row + 4] to get the index into the column-major version of the matrix M.
  %7 = getelementptr [3 x i32], [3 x i32]* %1, i32 0, i32 %col, !dbg !47 ; line:3 col:10
  %8 = load i32, i32* %7, align 4, !dbg !47 ; line:3 col:10
; Load the corresponding element from the column-major version of the matrix M using the index that was just loaded.
  %9 = getelementptr [6 x i32], [6 x i32]* @.hca, i32 0, i32 %8, !dbg !47 ; line:3 col:10
  %10 = load i32, i32* %9, align 4, !dbg !47 ; line:3 col:10
  ret i32 %10, !dbg !48 ; line:3 col:3
}
```

While in this example it may appear that DXC stores matrices in column-major order, it is not the case. 
You can see in this example: https://godbolt.org/z/bzKeY91Ts
Where the matrix is from a function argument, DXC will create a copy of the matrix but with a row-major order to column-major order transformation, and then perform the same steps to index the column-order copy instead of the original row-major matrix from the function argument.

---

### Comment 2 - farzonl

Its not the store thats the issue its the indexing. I dont think you are interpreting what the array alloca of size 3 is doing correctly. The array is encoding base indices that should be the equivalent of row *3 + col

---

### Comment 3 - Icohedron

Indeed DXC is calculating row * 3 + col, which is the calculation for indexing into a row-major order 2x3 matrix. 
It uses that to index into the row-major copy of the matrix M (not the original column-major order M).

Edit: I realize now both DXC and Clang are actually computing row + col * 2. The actual difference here is Clang is initializing the matrix M in row-major order, while DXC is initializing M in column-major order. Therefore it is actually Clang's initialization of the matrix M that is incorrect if we want DXC to be the spec.

---

### Comment 4 - farzonl

> I indeed DXC is calculating row * 3 + col, which is the calculation for indexing into a row-major order matrix. It uses that to index into the row-major copy of the matrix M (not the original column-major order M).

But thats not what clang is doing today hence the bug. Below see the indexing differences:

row | col |  DXC today and Clang after #165666 row * 3 + col | clang today col * 2 + row
-- | -- | -- | --
0 | 0 | 0 * 3 + 0 = 0 | 0 * 2 + 0 = 0
0 | 1 | 0 * 3 + 1 = 1 | 1 * 2 + 0 = 2
0 | 2 | 0 * 3 + 2 = 2 | 2 * 2 + 0 = 4
1 | 0 | 1 * 3 + 0 = 3 | 0 * 2 + 1 = 1
1 | 1 | 1 * 3 + 1 = 4 | 1 * 2 + 1 = 3
1 | 2 | 1 * 3 + 2 = 5 | 2 * 2 + 1 = 5



---

### Comment 5 - Icohedron

Yes, but Clang is indexing into M itself (a column-major order matrix).
Meanwhile DXC is indexing into a row-major order copy of M.
That's why you see the indices differ.

DXC is introducing an intermediate row-major order copy of M for reasons I do not know.

Edit: Clang is actually initializing M in row-major order but indexing it with an index intended for a column-major order matrix. 
DXC initialized M in column-major order and computes the index intended for a column-major order matrix but uses an extra array [row, row + 2, row + 4] instead of multiplying col by 2.

---

### Comment 6 - farzonl

> Yes, but Clang is indexing into M itself. Meanwhile DXC is indexing into a row-major order copy of M. That's why you see the indices differ.
> 
> DXC is introducing an intermediate row-major order copy of M for reasons I do not know.

I'm not proposing we copy DXCs implementation. But DXCs behavior is by default the spec. We need an indexing scheme that matches it. I'll follow up with @llvm-beanz  and @tex3d on the peculiarities.

---

### Comment 7 - tex3d

> DXC is introducing an intermediate row-major order copy of M for reasons I do not know.

HLSL has only one logical ordering for matrices: **row-major**.

Initialization is _always_ row-major.  Indexing is _always_ row-major.  The type is always written as `<scalar-type><rows>x<columns>`, or `matrix<scalar-type, rows, columns>`.

The global orientation setting and `row_major`/`column_major` modifiers only impact the storage layout. They do not impact the logical ordering of the matrix from the HLSL code perspective.

This isn't a comment on exactly how this must be implemented in IR, this is just how HLSL has always defined matrices.

When it comes to local storage layout which is not visible to the application, the compiler could make whatever decisions it deems most efficient for a given usage pattern.

I've heard the comment that "matrices in HLSL are column-major". This is a confusion. They _always_ appear as row-major _inside_ HLSL code. In storage, the matrix may be stored in row-major or column-major order.  It defaults to column-major storage for historical reasons - column-major fits a 4x3 matrix into 3 float4 constant registers instead of 4 registers leaving the last component as padding.

> You can see in this example: https://godbolt.org/z/bzKeY91Ts

I would also caution against using library functions with matrix parameter or return types in DXC as a reference, which IIRC are buggy. One issue is that matrices in library functions keep their high-level type, though function signatures were intended to be lowered at some point. The ugly code seen in the example is a result of intermediate operations and transformations that never quite get properly lowered and optimized.

That's not to say non-library matrix code is bug-free in DXC either. I'd expect cases involving dynamic indexing and storage orientation changes are most likely sources of bugs, such as where you dynamically index the row of a column-major matrix - which all examples here do.  In fact, I just noticed an alloca size bug (https://github.com/microsoft/DirectXShaderCompiler/issues/7865) when dynamically indexing a matrix row from a column-major stored matrix in a cbuffer (which resolves to dynamically indexing a column vector loaded from the cbuffer).

---

### Comment 8 - Icohedron

Yes, I knew that from HLSL code the matrices are always logically row-major. They're initialized in row-major from the constructor and/or initializer list (which is the most natural way to define matrices in text/list form) and indexed with [row][col] as opposed to [col][row]. The modifiers `row_major` and `column_major` only impact the storage layout which isn't visible from HLSL, but will impact the IR / DXIL.

Though I still have a question regarding your comment:
> When it comes to local storage layout which is not visible to the application, the compiler could make whatever decisions it deems most efficient for a given usage pattern.

First take a look at this example: https://hlsl.godbolt.org/z/ec6Y9Eodd

```hlsl
RWStructuredBuffer<int> In;
RWStructuredBuffer<int> Out;
cbuffer Constants {
    uint row;
    uint col;
};
[numthreads(1, 1, 1)]
void CSMain(uint GI : SV_GroupIndex) {
    int2x3 M = int2x3(In[0], In[1], In[2], 
                      In[3], In[4], In[5]);
    Out[0] = M[row][col];
}
```

Inspecting the IR, I can see the following procedures done by Clang and DXC:
```c
/* Clang */
// Constructs M as a row-major order matrix
M = [ In[0], In[1], In[2], In[3], In[4], In[5] ]
// Computes a column-major flat index from row and col
Out[0] = M[row + col * 2] // this should be M[row * 3 + col] to be correct

/* DXC */
// Constructs M as a column-major order matrix
M = [ In[0], In[3], In[1], In[4], In[2], In[5] ]
// Computes a column-major flat index
I = [ row, row + 2, row + 4 ]
Out[0] = M[I[col]] // equivalent to M[row + col * 2]
```

Notably, Clang currently appears to:
1. Construct `M` as a row-major order matrix, whereas DXC constructs a column-major order matrix
2. Compute the flat index for the matrix `M` incorrectly (which is fixed by #165666)

My question is: Do we want Clang to also default to constructing matrices in column-major order as DXC? Or allow it to be arbitrary unless explicitly specified with the `row_major` and `column_major` modifiers? I believe this matrix declaration falls under "local storage layout which is not visible to the application".

We should also determine if we want Clang to compute the index in a similar fashion to DXC using an extra array to hold indices for each possible value of `col`. Why does DXC compute it this way in the first place?

---

### Comment 9 - farzonl

> Out[0] = M[row * 2 + col] // this should be M[row * 3 + col]
> ...
> Notably, Clang currently appears to:
> 
> 1. Construct `M` as a row-major order matrix, whereas DXC constructs a column-major order matrix
> 2. Compute the flat index for the matrix `M` incorrectly

Clang today does `col * 2 + row` not `row * 2 + col`. You are correct we need it to do `row * 3 + col`.

Point one is incorrect Clang does not construct the matrix any differently than DXC.
https://clang.llvm.org/docs/MatrixTypes.html#decisions-for-the-implementation-in-clang
See:
`The elements of a value of a matrix type are laid out in column-major order without padding.`

The only bug is in the indexing.

> My question is: Do we want Clang to also default to constructing matrices in column-major order as DXC? Or allow it to be 
> arbitrary unless explicitly specified with the row_major and column_major modifiers? I believe this matrix declaration falls under "local storage layout which is not visible to the application".

As mentioned earlier Clang already defaults to column major  order so the answer to that question is yes.

To your second question no but is more nuanced than that. The ` row_major` and `column_major` modifiers are not relevant for this discussion. They only come in to play for matrix types defined in a global memory and are not modifiers used in the function scope that this bug  was found. 

See an example of usage here.
https://godbolt.org/z/M8aYWTKzz

For your third point  yes this does falll under local storage layout.

> We should also determine if we want Clang to compute the index in a similar fashion to DXC using an extra array to hold indices for each possible value of col. Why does DXC compute it this way in the first place?

No we are not going to do this. We don't need to carry over implementation details. Its only important to carry over behavior.





---

### Comment 10 - Icohedron

> Point one is incorrect Clang does not construct the matrix any differently than DXC.
https://clang.llvm.org/docs/MatrixTypes.html#decisions-for-the-implementation-in-clang
See:
The elements of a value of a matrix type are laid out in column-major order without padding.
The only bug is in the indexing.

I see. That does appear to be relevant documentation for matrices in Clang. 
However, I still believe there is something incorrect in the implementation for the matrix initialization in Clang for HLSL.
Please take a look at the IR generated by Clang in https://hlsl.godbolt.org/z/ec6Y9Eodd (same example as in my last comment)

Again, this is the HLSL shader:
```hlsl
RWStructuredBuffer<int> In;
RWStructuredBuffer<int> Out;
cbuffer Constants {
    uint row;
    uint col;
};
[numthreads(1, 1, 1)]
void CSMain(uint GI : SV_GroupIndex) {
    int2x3 M = int2x3(In[0], In[1], In[2], 
                      In[3], In[4], In[5]);
    Out[0] = M[row][col];
}
```

Here are the instructions Clang emits:
```llvm
  ; Alloca to hold the matrix M.
  %matrixext.i.alloca = alloca [6 x i32], align 4, !dbg !116
  ; ...
  ; Reading elements from the UAV (In)
  %6 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 0, i32 0, i8 1, i32 4), !dbg !143
  %7 = extractvalue %dx.types.ResRet.i32 %6, 0, !dbg !143
  %8 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 1, i32 0, i8 1, i32 4), !dbg !144
  %9 = extractvalue %dx.types.ResRet.i32 %8, 0, !dbg !144
  %10 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 2, i32 0, i8 1, i32 4), !dbg !145
  %11 = extractvalue %dx.types.ResRet.i32 %10, 0, !dbg !145
  %12 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 3, i32 0, i8 1, i32 4), !dbg !146
  %13 = extractvalue %dx.types.ResRet.i32 %12, 0, !dbg !146
  %14 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 4, i32 0, i8 1, i32 4), !dbg !147
  %15 = extractvalue %dx.types.ResRet.i32 %14, 0, !dbg !147
  %16 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 5, i32 0, i8 1, i32 4), !dbg !148
  %17 = extractvalue %dx.types.ResRet.i32 %16, 0, !dbg !148
  ; Hence: %7 = In[0], %9 = In[1], %11 = In[2], %13 = In[3], %15 = In[4], %17 = In[5]
  ; ...
  ; Populate the alloca for the matrix M
  %matrixext.i.index15 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 0, !dbg !149
  store i32 %7, ptr %matrixext.i.index15, align 4, !dbg !149
  %matrixext.i.index516 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 1, !dbg !149
  store i32 %9, ptr %matrixext.i.index516, align 4, !dbg !149
  %matrixext.i.index717 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 2, !dbg !149
  store i32 %11, ptr %matrixext.i.index717, align 4, !dbg !149
  %matrixext.i.index918 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 3, !dbg !149
  store i32 %13, ptr %matrixext.i.index918, align 4, !dbg !149
  %matrixext.i.index1119 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 4, !dbg !149
  store i32 %15, ptr %matrixext.i.index1119, align 4, !dbg !149
  %matrixext.i.index1320 = getelementptr inbounds [6 x i32], ptr %matrixext.i.alloca, i32 0, i32 5, !dbg !149
  store i32 %17, ptr %matrixext.i.index1320, align 4, !dbg !149
  ; Hence, %matrixext.i.alloca now holds the array: [ %7, %9, %11, %13, %15, %17 ] == [ In[0], In[1], In[2], In[3], In[4], In[5] ]
```

Do you see? The alloca `%matrixext.i.alloca` (corresponding to the int2x3 matrix M) is an array of `[ In[0], In[1], In[2], In[3], In[4], In[5] ]`.

This leads to my second point/observation. 

You said the following:
> As mentioned earlier Clang already defaults to column major order so the answer to that question is yes.

Now combine this with the statement: "Row-major and column-major packing order has no influence on the packing order of constructors (which always follows row-major ordering)." from the same HLSL documentation linked in the issue description (https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-per-component-math#matrix-ordering)

We have:
1. Clang defaults to column-major ordering for matrices, and
2. the packing order of matrix constructors always follows row-major ordering

From these two points, that implies Clang's procedure for populating the alloca `%matrixext.i.alloca` is incorrect!

The matrix constructor is as follows from the HLSL: `int2x3 M = int2x3(In[0], In[1], In[2], In[3], In[4], In[5]);`
The elements of the matrix in this constructor are -- as said in the documentation -- in row-major order.

Clang takes these elements, and **copies the elements in exactly the same order into the alloca** `%matrixext.i.alloca`. Therefore, the matrix in `%matrixext.i.alloca` is in row-major order!

The correct sequence of elements for the alloca `%matrixext.i.alloca` should be `[ In[0], In[3], In[1], In[4], In[2], In[5] ]`. This is the memory layout expected for a column-major order matrix.

```
0 1 2
3 4 5

Row-major order: [ 0 1 2 3 4 5 ]
Row-major flat index calculation: row * 3 + col

Column-major order: [ 0 3 1 4 2 5 ]
Column-major flat index calculation: row + col * 2
```

Again I, in my last comment, have already given a high-level explanation of what I see the IR is doing from Clang and DXC for this example.
```c
/* Clang */
// Constructs M as a row-major order matrix
M = [ In[0], In[1], In[2], In[3], In[4], In[5] ]
// Computes a column-major flat index from row and col
Out[0] = M[row + col * 2] // this should be M[row * 3 + col] to be correct

/* DXC */
// Constructs M as a column-major order matrix
M = [ In[0], In[3], In[1], In[4], In[2], In[5] ]
// Computes a column-major flat index
I = [ row, row + 2, row + 4 ]
Out[0] = M[I[col]] // equivalent to M[row + col * 2]
```

Please closely read the IR to confirm that what I am seeing is correct.
Right now, Clang and DXC compute the same flat index, but differ in the matrix initialization.

Hence, if matrices in Clang default to column-major ordering, then it is not the index calculation that is incorrect in Clang, but rather the matrix initialization / construction for HLSL.

---

### Comment 11 - tex3d

> Though I still have a question regarding your comment:

> > When it comes to local storage layout which is not visible to the application, the compiler could make whatever decisions it deems most efficient for a given usage pattern.

> ...

> My question is: Do we want Clang to also default to constructing matrices in column-major order as DXC? Or allow it to be arbitrary unless explicitly specified with the row_major and column_major modifiers? I believe this matrix declaration falls under "local storage layout which is not visible to the application".

> We should also determine if we want Clang to compute the index in a similar fashion to DXC using an extra array to hold indices for each possible value of col. Why does DXC compute it this way in the first place?

DXC background:

DXC uses the storage layout of the variable, _even when local_, though it didn't have to be that way. This is why you see DXC storing the matrix in column-major, because that's the default storage layout when you don't explicitly specify one. If you changed the default to row major with `-Zpr`, the pragma, or `row_major` on the decl, you'd see it store the matrix in row major order instead. It also uses storage layout for function parameters - which impacts the matrix layout in a shader signature, which is visible to the host application in the way it might fit into limited shader attributes.

When you use the subscript operator, DXC generates a high-level subscript call that includes computed indexes for each output component of the operator's vector return type: `[ row, row + 2, row + 4 ]`. It's a little odd to generate the code that way initially, but that's how it was implemented to account for the matrix swizzles as well. In `HLMatrixLowerPass.cpp`, you'll see `lowerHLMatSubscript` and `lowerHLMatElementSubscript` with a first line/comment:
```cpp
  (void)RowMajor; // It doesn't look like we actually need this?
```
Which is because the index list for each returned vector element was passed as arguments during Clang CodeGen, already accounting for the matrix layout. For the case you've written `mat[r][c]` and `c` is not constant, `HLMatrixSubscriptUseReplacer::tryGetScalarIndex` generates an index array from the arguments to the subscript operator and dynamically indexes that. This is what generates the `I = [ row, row + 2, row + 4 ]` you see from DXC.

The alternative might be to generate the full vector from the row subscript, then let standard dynamic vector indexing translation copy that to a local array and dynamically index into it. The index array is considered better because it allows for a single load from a single computed index, rather than loading all components into a local array for indexing.

But this is also not ideal because the mathematical relationship between the index into the column and the index into the flattened vector location in the matrix is obfuscated during clang CodeGen by calculating each individual component index early. Without that, simple mul+add ops could have been used instead of the intermediate array for the dynamic column indexing case.

Clang:

Using storage layout for all stored matrices might simplify making sure the desired layout is used where necessary (like global/groupshared memory and shader signatures). It could complicate generated operations a bit, but operations should be encoded with the information necessary to access the matrix correctly from the stored layout. If we preserved layout information, we could potentially perform optimizations that reduce unnecessary transpositions by remapping local layouts that are invisible to the host application.

Using a fixed internal layout could simplify some assumptions for intermediate operations, but choosing a fixed orientation of column-major which doesn't match the fixed logical orientation in HLSL (row-major), might also lead to confusion/inefficiencies.

Regarding https://clang.llvm.org/docs/MatrixTypes.html#decisions-for-the-implementation-in-clang:

> The elements of a value of a matrix type are laid out in column-major order without padding.

The next line is:

> We propose to provide a Clang option to override this behavior and allow contraction of those operations (e.g. -ffp-contract=matrix).

I'm not sure what this would mean for individual values, like if this could only ever be a global setting rather than per-value, we could drive it from the global option (`-Zpr`, `-Zpc`), but it wouldn't mean that the layout in IR matches the orientation requested for any given HLSL variable.

---

### Comment 12 - farzonl

@Icohedron  @tex3d  We don't need a design meeting for this. I think we need to refocus the problem we are trying to solve which right now is just the default matrix layout behavior. I have closed out the fix in the `MatrixSubscriptExpr`. While that would have also fixed the execution test failures I was seeing the issue we have today is a layout order which can be fixed with two lines.

 As I was trying to explain in refinement this issue can be fixed easily in our iteration of the initalizer list. The bug was that we were using the initalizer lists index to populate the matrix. This meant that [0..n] would coorelate to [0..n] indicies of the flattened matrix. Hence why we were seeing the Row-major order: [ 0 1 2 3 4 5 ].  To fix this we can simply  converted these indicies to the Column-major order: [ 0 3 1 4 2 5 ].

The rest of the discussion on this ticket is beyond the scope of this bug

```diff
@@ -1906,10 +1906,12 @@ void InitListChecker::CheckMatrixType(const InitializedEntity &Entity,
 
   while (NumEltsInit < MaxElts && Index < IList->getNumInits()) {
     // Not a sublist: just consume directly.
-    ElemEnt.setElementIndex(Index);
-    CheckSubElementType(ElemEnt, IList, ElemTy, Index, StructuredList,
+    unsigned ColMajorIndex = (Index % MT->getNumRows()) * MT->getNumColumns() + (Index / MT->getNumRows());
+    ElemEnt.setElementIndex(ColMajorIndex);
+    CheckSubElementType(ElemEnt, IList, ElemTy, ColMajorIndex, StructuredList,
                         StructuredIndex);
     ++NumEltsInit;
+    ++Index;
   }
```

TLDR we were fetching the wrong elements off of the initalizer list because we were reusing the initalizer lists index.

---

### Comment 13 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Farzon Lotfi (farzonl)

<details>
Today Matrix initalization is just taking in the index from the initalizer list:
https://github.com/llvm/llvm-project/blob/15bbdd143cb196be8d60ea4dc813bf7bc5e4650c/clang/lib/Sema/SemaInit.cpp#L1907-L1913

This means we aren't reordering the index to account for layout DXC is expecting.

See https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-per-component-math#matrix-ordering


The fix seems to be
```diff
@@ -1906,10 +1906,12 @@ void InitListChecker::CheckMatrixType(const InitializedEntity &amp;Entity,
 
   while (NumEltsInit &lt; MaxElts &amp;&amp; Index &lt; IList-&gt;getNumInits()) {
     // Not a sublist: just consume directly.
-    ElemEnt.setElementIndex(Index);
-    CheckSubElementType(ElemEnt, IList, ElemTy, Index, StructuredList,
+    unsigned ColMajorIndex = (Index % MT-&gt;getNumRows()) * MT-&gt;getNumColumns() + (Index / MT-&gt;getNumRows());
+    ElemEnt.setElementIndex(ColMajorIndex);
+    CheckSubElementType(ElemEnt, IList, ElemTy, ColMajorIndex, StructuredList,
                         StructuredIndex);
     ++NumEltsInit;
+    ++Index;
   }
```
</details>


---

