# ext_vector_type crashes on bool

**Author:** 4vtomat
**URL:** https://github.com/llvm/llvm-project/issues/152954

## Body

The following code crashed:

$ cat fail.c

```cpp
#include <stdbool.h>

typedef bool bx16 __attribute__((ext_vector_type(16)));
bx16 test(bx16 a) {
  bx16 ret;
  asm volatile ("vmand.mm %0, %1, %2" : "=vm"(ret) : "vm"(a), "vm"(a));
  return ret;
}
```
with command
```
bin/clang -target riscv64 -march=rv64gcv fail.c -S
```

Note: Only inline assembly with output failed, if removing `ret` and it won't crash.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Brandon Wu (4vtomat)

<details>
The following code crashed:
```
$ cat fail.c

#include &lt;stdbool.h&gt;

typedef bool bx16 __attribute__((ext_vector_type(16)));
bx16 test(bx16 a) {
  bx16 ret;
  asm volatile ("vmand.mm %0, %1, %2" : "=vm"(ret) : "vm"(a), "vm"(a));
  return ret;
}
```
with command
```
bin/clang -target riscv64 -march=rv64gcv fail.c -S
```

Note: Only inline assembly with output failed, if removing `ret` and it won't crash.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Brandon Wu (4vtomat)

<details>
The following code crashed:
```
$ cat fail.c

#include &lt;stdbool.h&gt;

typedef bool bx16 __attribute__((ext_vector_type(16)));
bx16 test(bx16 a) {
  bx16 ret;
  asm volatile ("vmand.mm %0, %1, %2" : "=vm"(ret) : "vm"(a), "vm"(a));
  return ret;
}
```
with command
```
bin/clang -target riscv64 -march=rv64gcv fail.c -S
```

Note: Only inline assembly with output failed, if removing `ret` and it won't crash.
</details>


---

### Comment 3 - wangpc-pp

It is weird that this is a `LLVM ERROR: out of memory`: https://godbolt.org/z/jEoYGbh73


---

### Comment 4 - unoyx

i've some investigate with this case, a brief crash stack is like this:
```
__GI___assert_fail
llvm::cast<llvm::FixedVectorType, llvm::Type>
clang::CodeGen::CodeGenFunction::emitBoolVecConversion
clang::CodeGen::CodeGenFunction::EmitToMemory
```

in `EmitToMemory` and `emitBoolVecConversion` is processing the node: `%3 = call i16 asm sideeffect "vmand.mm $0, $1, $2", "=^vm,^vm,^vm"(<16 x i1> %1, <16 x i1> %2) #1, !srcloc !0`
and the cast is trying to convert `bx16` (`i16`) to `llvm::FixedVectorType`,  a assert is triggered



---

### Comment 5 - MouseSplinter

This seems to be a common bug，at least the code will crash also on Aarch64 when use ExtVectorBoolType.

As @unoyx mentioned above, the `llvm::Type` of `%3` is not as expected as `llvm::FixedVectorType`. The reason is that
```
  if (Ty->isExtVectorBoolType()) {

    llvm::Type *StoreTy = convertTypeForLoadStore(Ty, Value->getType());
    if (StoreTy->isVectorTy() && StoreTy->getScalarSizeInBits() >
                                     Value->getType()->getScalarSizeInBits())
      return Builder.CreateZExt(Value, StoreTy);

    // Expand to the memory bit width.
    unsigned MemNumElems = StoreTy->getPrimitiveSizeInBits();
    // <N x i1> --> <P x i1>.
    Value = emitBoolVecConversion(Value, MemNumElems, "insertvec");
    // <P x i1> --> iP.
    Value = Builder.CreateBitCast(Value, StoreTy);
  }
```
(quoted from `CodeGenFunction::EmitToMemory(llvm::Value *Value, QualType Ty)`), the `Value` Type is `i16` while the AST Node type `Ty` is <16 x i1>. It seems that there is no need to do `<N x i1> --> <P x i1> --> iP` if `!isa<llvm::FixedVectorType>(Value->getType())`. If add the constraint, the ir will generate as
```
define dso_local i64 @test(i64 %a.coerce) #0 {
entry:
  %retval = alloca <16 x i1>, align 2
  %a = alloca i16, align 2
  %a.addr = alloca i16, align 2
  %ret = alloca i16, align 2
  %retval.coerce = alloca i64, align 8
  %coerce.val.ii = trunc i64 %a.coerce to i16
  store i16 %coerce.val.ii, ptr %a, align 2
  %load_bits = load i16, ptr %a, align 2
  %a1 = bitcast i16 %load_bits to <16 x i1>
  %0 = bitcast <16 x i1> %a1 to i16
  store i16 %0, ptr %a.addr, align 2, !tbaa !13
  call void @llvm.lifetime.start.p0(ptr %ret) #3
  %load_bits2 = load i16, ptr %a.addr, align 2
  %1 = bitcast i16 %load_bits2 to <16 x i1>
  %load_bits3 = load i16, ptr %a.addr, align 2
  %2 = bitcast i16 %load_bits3 to <16 x i1>
  %3 = call i16 asm sideeffect "vmand.mm $0, $1, $2", "=^vm,^vm,^vm"(<16 x i1> %1, <16 x i1> %2) #3, !srcloc !14
  store i16 %3, ptr %ret, align 2, !tbaa !13
  %load_bits4 = load i16, ptr %ret, align 2
  %4 = bitcast i16 %load_bits4 to <16 x i1>
  store <16 x i1> %4, ptr %retval, align 2
  call void @llvm.lifetime.end.p0(ptr %ret) #3
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %retval.coerce, ptr align 2 %retval, i64 2, i1 false)
  %5 = load i64, ptr %retval.coerce, align 8
  ret i64 %5
}
```
And will report error
```
./test.c:11:17: error: couldn't allocate output register for constraint 'vm'
   11 |   asm volatile ("vmand.mm %0, %1, %2" : "=vm"(ret) : "vm"(a), "vm"(a));
      |                 ^
```
 at `SelectionDAGBuilder::visitInlineAsm`. The IR before riscv-isel is
```
*** IR Dump After Module Verifier (verify) ***
; Function Attrs: nounwind uwtable vscale_range(2,1024)
define dso_local range(i64 0, 65536) i64 @test(i64 %a.coerce) local_unnamed_addr #0 {
entry:
  %coerce.val.ii = trunc i64 %a.coerce to i16
  %a1 = bitcast i16 %coerce.val.ii to <16 x i1>
  %0 = tail call i16 asm sideeffect "vmand.mm $0, $1, $2", "=^vm,^vm,^vm"(<16 x i1> %a1, <16 x i1> %a1) #1, !srcloc !13
  %retval.coerce.0.insert.ext = zext i16 %0 to i64
  ret i64 %retval.coerce.0.insert.ext
}
```
It seems the backend cannot allocate register for `%0`.

---

