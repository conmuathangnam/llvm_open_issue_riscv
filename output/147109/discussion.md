# [HLSL] [DirectX] Clang emits struct alloca instructions for structs with one or more array-typed fields, or any struct when compiling with `-O0`

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/147109
**Status:** Open
**Labels:** miscompilation, backend:DirectX, HLSL

## Body

After resolving #146974, validation errors of the form `Pointer type bitcast must be have same size` and `Bitcast on struct types is not allowed` appear.
There are 16 occurrences of both errors in total, and they all originate from four DML shaders: `OneHot_256_uint16_native_int64_emulated_{4/8}` and `OneHot_256_uint16_native_int32_native_{4/8}`

The issue is that Clang is emitting an alloca for **structs that contain one or more array-typed fields.**

For example, take this shader: https://godbolt.org/z/o5he6GnG9
```hlsl
// compile args: -E CSMain -T cs_6_2 -enable-16bit-types -Xclang -emit-llvm
RWStructuredBuffer<uint> output;
struct MyStruct {
  uint arr[2];
};
[numthreads(1, 1, 1)]
void CSMain(uint3 Tid: SV_DispatchThreadID) {
  MyStruct s = {Tid.x, 0};
  uint d = s.arr[Tid.y];
  output[0] = d ;
}
```

A struct definition and an alloca of the struct appear in the IR that is to be fed to the DirectX backend.
```llvm
%struct.MyStruct = type { [2 x i32] }
define void @CSMain() local_unnamed_addr #0 {
  %1 = alloca %struct.MyStruct, align 4, !DIAssignID !56
...
```

This is causing issues because the DirectX backend does not handle allocas for structs and thus causes a cascade of validation errors.
```
Function: CSMain: error: Pointer type bitcast must be have same size.
note: at '%3 = bitcast %struct.MyStruct* %s.i to i32*' in block 'entry' of function 'CSMain'.
Function: CSMain: error: Bitcast on struct types is not allowed.
note: at '%4 = bitcast %struct.MyStruct* %s.i to [1 x %struct.MyStruct]*' in block 'entry' of function 'CSMain'.
Function: CSMain: error: Pointer type bitcast must be have same size.
note: at '%5 = bitcast %struct.MyStruct* %arrayinit.element.i1 to i32*' in block 'entry' of function 'CSMain'.
Function: CSMain: error: Bitcast on struct types is not allowed.
note: at '%6 = bitcast %struct.MyStruct* %s.i to [2 x i32]*' in block 'entry' of function 'CSMain'.
Validation failed.
```

DXC will instead eliminate the struct
```hlsl
define void @CSMain() {
  %s.0 = alloca [2 x i32], align 4
```

Additionally, **if you compile with `-O0`, Clang will emit structs and struct allocas regardless of whether or not the struct contains an array-typed field.**

The issue is that the SROA pass is responsible for eliminating the structs, but fails to do so when the struct contains an array-typed field. Furthermore, the SROA pass does not run when the flag `-O0` is specified.

## Unknown load/store offsets prevent struct decomposition

The reason SROA will not decompose the struct into separate fields in this instance is due to the dynamic index into the array field.
As an example, take this struct
```c++
struct S { int arr[2]; int x; };
S s;
s.arr[i] = 1; // i is unknown, s.arr[i] may alias s.x
s.x = 2;
```
The SROA pass will not decompose the struct into
```c++
int arr[2]; 
int x;
arr[i] = 1; // i is unknown, but arr[i] may not alias x. (x may even get removed)
x = 2;
```
because there exists a value of `i` that could be used to access `s.x` from `s.arr` since they are laid out contiguously in memory due to being in the same struct. This property is not guaranteed to the be case when `x` and `arr` are independent variables.

Perhaps we can implement a way to decompose the struct if the access is known to be inbounds?

## SROA will not run on functions with OptimizeNone attribute (`-O0`)

https://github.com/llvm/llvm-project/blob/c93d166c58f0f4ac69c58ec54fb08668b462de03/llvm/lib/Transforms/Scalar/SROA.cpp#L5824-L5826

The SROA pass explicitly refuses to run on functions with the OptimizeNone attribute.

This particular issue is tracked by #151564 and #151567 


## Comments

### Comment 1 - Icohedron

As of current, there are no LLVM IR semantics capable of guaranteeing the safe transformation of dynamically-indexed accesses to struct allocas. The closest thing we have is the `inbounds` property of GEPs which only assume that the resulting pointer remains within the bounds of the original struct allocation and does not guarantee that the pointer will point to the same struct member.

Therefore, this issue is blocked on the following proposal: https://discourse.llvm.org/t/rfc-adding-instructions-to-to-carry-gep-type-traversal-information/88141
See https://github.com/llvm/llvm-project/pull/161601#pullrequestreview-3294935062 for more details.


---

### Comment 2 - damyanp

See #176145 

---

### Comment 3 - Icohedron

Waiting for the SROA to be updated with support for structured GEP before proceeding with fixing this issue.

---

