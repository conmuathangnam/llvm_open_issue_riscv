# RVV assert in inline asm for tied constraints

**Author:** Rexicon226
**URL:** https://github.com/llvm/llvm-project/issues/151877

## Body

```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux5.10.0-musl"

define i16 @foo() {
Entry:
  %0 = call <2 x i8> asm "vadd.vv ${0}, ${1}, ${2}", "=&m,{x11},{x12}"(<2 x i8> zeroinitializer, <2 x i8> zeroinitializer)
  ret i16 0
}
```

```
Assertion failed: ((OpInfo.isMultipleAlternative || (OpInfo.Type == InlineAsm::isInput)) && "Can only indirectify direct input operands!"), function visitInlineAsm, file SelectionDAGBuilder.cpp, line 9901.
```

https://llc.godbolt.org/z/Y8qj3nTdx

Should probably result in an error when selecting the constraint instead of an Assert.

## Comments

### Comment 1 - topperc

Did this come from clang or another frontend?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: David Rubin (Rexicon226)

<details>
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux5.10.0-musl"

define i16 @<!-- -->foo() {
Entry:
  %0 = call &lt;2 x i8&gt; asm "vadd.vv ${0}, ${1}, ${2}", "=&amp;m,{x11},{x12}"(&lt;2 x i8&gt; zeroinitializer, &lt;2 x i8&gt; zeroinitializer)
  ret i16 0
}
```

```
Assertion failed: ((OpInfo.isMultipleAlternative || (OpInfo.Type == InlineAsm::isInput)) &amp;&amp; "Can only indirectify direct input operands!"), function visitInlineAsm, file SelectionDAGBuilder.cpp, line 9901.
```

https://llc.godbolt.org/z/Y8qj3nTdx

Should probably result in an error when selecting the constraint instead of an Assert.
</details>


---

### Comment 3 - Rexicon226

> Did this come from clang or another frontend?

Sorry for the delay - this came from Zig,
```zig
const V = @Vector(2, u8);

export fn foo(x: V, y: V) V {
    return asm (
        \\vadd.vv %[result], %[x], %[y]
        : [result] "=&m" (-> V),
        : [x] "{x11}" (x),
          [y] "{x12}" (y),
    );
}
```

(I know the asm is pretty nonsense, was just testing stuff)


---

### Comment 4 - XChy

The Zig frontend may be to blame. It seems that the Zig frontend doesn't produce the indirect constraint for the output operand.

---

