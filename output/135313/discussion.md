# [CodeGen] Miscompile with llvm.maximumnum.f64 and llvm.maximum.f64

**Author:** patrick-rivos
**URL:** https://github.com/llvm/llvm-project/issues/135313
**Status:** Closed
**Labels:** backend:X86, miscompilation, floating-point
**Closed Date:** 2025-07-07T01:57:03Z

## Body

Testcase:
```llvm ir
source_filename = "test.ll"
target triple = "x86_64-unknown-linux-gnu"

define i64 @main() #0 {
  %zext.1 = zext i32 -2147483648 to i64
  %zext.2 = zext i32 2147483647 to i64
  %shl.1 = shl i64 %zext.2, 32
  %or.1 = or i64 %zext.1, %shl.1
  %call.1 = call i64 @float_fn(i64 44, i64 %or.1, i64 4, ptr null)
  %add.1 = add i64 %call.1, 0
  %bitcast.1 = bitcast i64 15 to double
  %bitcast.2 = bitcast i64 %add.1 to double
  %call.2 = call double @llvm.maximumnum.f64(double %bitcast.1, double %bitcast.2)
  %bitcast.3 = bitcast double %call.2 to i64
  %add.2 = add i64 %bitcast.3, 0
  %bitcast.4 = bitcast i64 43 to double
  %bitcast.5 = bitcast i64 %add.1 to double
  %call.3 = call double @llvm.maximum.f64(double %bitcast.4, double %bitcast.5)
  %bitcast.6 = bitcast double %call.3 to i64
  %add.3 = add i64 %bitcast.6, 0
  %icmp.1 = icmp ne i64 %add.2, 15
  %res = zext i1 %icmp.1 to i64
  ret i64 %res
}

define internal i64 @float_fn(i64 %arg0, i64 %arg1, i64 %arg2, ptr %arg3) #0 {
  ret i64 u0x7ff8000000000000
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.maximumnum.f64(double, double) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.maximum.f64(double, double) #1

attributes #0 = { "target-cpu"="skylake" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Commands:
```
$ ./llvm/bin/opt good.ll -passes="early-cse" -o bad.bc
$ ./llvm/bin/clang bad.bc -O0 -o bad && ./bad
$ echo $?
130
$ ./llvm/bin/clang good.ll -O0 -o good && ./good
$ echo $?
0
```
Godbolt: https://godbolt.org/z/jExWdcGqx
Alive2: https://alive2.llvm.org/ce/z/WZDPeF (I added the willreturn attribute to get the value mismatch error msg)

## Comments

### Comment 1 - patrick-rivos

I'm a bit skeptical that I'm not missing an attribute or something - this appears going back to `opt 16.0` (and before then fails due to a syntax issue)

---

### Comment 2 - dtcxzyw

EarlyCSE is correct: https://alive2.llvm.org/ce/z/HXRkNE
Alive2 doesn't support function calls (just treated as uninterpreted functions).


---

### Comment 3 - patrick-rivos

Ah thank you. The difference is there when I execute the `src`/`tgt` programs:
https://godbolt.org/z/vqh1zxeEe

The optimized variant maps down to: `vmaxsd`
From the [2b ISA reference](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) for `vmaxsd`:
```
MAX(SRC1, SRC2)
{
  IF ((SRC1 = 0.0) and (SRC2 = 0.0)) THEN DEST := SRC2;
  ELSE IF (SRC1 = NaN) THEN DEST := SRC2; FI;
  ELSE IF (SRC2 = NaN) THEN DEST := SRC2; FI;
  ELSE IF (SRC1 > SRC2) THEN DEST := SRC1;
  ELSE DEST := SRC2;
  FI;
}
```
Following the logic if SRC1 is not a NaN and SRC2 is a NaN we'll hit on `ELSE IF (SRC2 = NaN) THEN DEST := SRC2; FI;`
The text from the same doc supports this:
> If only one value is a NaN (SNaN or QNaN) for this instruction, the second source operand, either a NaN or a valid
floating-point value, is written to the result.

This does not have the same semantics as `llvm.maximumnum.f64`:
From the [LLVM reference](https://llvm.org/docs/LangRef.html#id680):
> If one operand is NaN (including sNaN) and another operand is a number, return the number. 

The unoptimized variant seems to do the right thing, mapping down to a `vcmpordsd`, `vblendvpd`, `vmovq` sequence.

This looks like a x86 backend bug, not an optimization issue.

Here's a simplified testcase: https://godbolt.org/z/6f8qe3ezP
Expected result: true (1) Observed result: false (0)

---

### Comment 4 - dtcxzyw

cc @wzssyqa @RKSimon 

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Patrick O'Neill (patrick-rivos)

<details>
Testcase:
```llvm ir
source_filename = "test.ll"
target triple = "x86_64-unknown-linux-gnu"

define i64 @<!-- -->main() #<!-- -->0 {
  %zext.1 = zext i32 -2147483648 to i64
  %zext.2 = zext i32 2147483647 to i64
  %shl.1 = shl i64 %zext.2, 32
  %or.1 = or i64 %zext.1, %shl.1
  %call.1 = call i64 @<!-- -->float_fn(i64 44, i64 %or.1, i64 4, ptr null)
  %add.1 = add i64 %call.1, 0
  %bitcast.1 = bitcast i64 15 to double
  %bitcast.2 = bitcast i64 %add.1 to double
  %call.2 = call double @<!-- -->llvm.maximumnum.f64(double %bitcast.1, double %bitcast.2)
  %bitcast.3 = bitcast double %call.2 to i64
  %add.2 = add i64 %bitcast.3, 0
  %bitcast.4 = bitcast i64 43 to double
  %bitcast.5 = bitcast i64 %add.1 to double
  %call.3 = call double @<!-- -->llvm.maximum.f64(double %bitcast.4, double %bitcast.5)
  %bitcast.6 = bitcast double %call.3 to i64
  %add.3 = add i64 %bitcast.6, 0
  %icmp.1 = icmp ne i64 %add.2, 15
  %res = zext i1 %icmp.1 to i64
  ret i64 %res
}

define internal i64 @<!-- -->float_fn(i64 %arg0, i64 %arg1, i64 %arg2, ptr %arg3) #<!-- -->0 {
  ret i64 u0x7ff8000000000000
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @<!-- -->llvm.maximumnum.f64(double, double) #<!-- -->1

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @<!-- -->llvm.maximum.f64(double, double) #<!-- -->1

attributes #<!-- -->0 = { "target-cpu"="skylake" }
attributes #<!-- -->1 = { mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Commands:
```
$ ./llvm/bin/opt good.ll -passes="early-cse" -o bad.bc
$ ./llvm/bin/clang bad.bc -O0 -o bad &amp;&amp; ./bad
$ echo $?
130
$ ./llvm/bin/clang good.ll -O0 -o good &amp;&amp; ./good
$ echo $?
0
```
Godbolt: https://godbolt.org/z/jExWdcGqx
Alive2: https://alive2.llvm.org/ce/z/WZDPeF (I added the willreturn attribute to get the value mismatch error msg)
</details>


---

### Comment 6 - RKSimon

Sorry - I haven't had time to look at this - adding it back to the pile

---

### Comment 7 - patrick-rivos

cc @phoebewang 

---

