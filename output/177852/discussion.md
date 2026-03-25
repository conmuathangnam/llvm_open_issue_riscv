# crash when section has `.cfi_startproc` but no `.cfi_endproc`

**Author:** folkertdev
**URL:** https://github.com/llvm/llvm-project/issues/177852
**Status:** Open
**Labels:** llvm:mc, crash

## Body

LLVM crashes on this program

https://godbolt.org/z/9W51vc3bW

```llvm
module asm ".pushsection .text.foo,\22ax\22, @progbits"
module asm ".type foo, @function"
module asm "foo:"
module asm ".cfi_startproc"
module asm ".popsection"

module asm ".type bar, @function"
module asm "bar:"
module asm ".cfi_startproc"
module asm ".cfi_endproc"
```

```
llc: /root/llvm-project/llvm/lib/MC/MCExpr.cpp:223: llvm::MCSymbolRefExpr::MCSymbolRefExpr(const llvm::MCSymbol*, llvm::MCExpr::Spec, const llvm::MCAsmInfo*, llvm::SMLoc): Assertion `Symbol' failed.
```

This assembly is malformed (but in a way that might come up during development). When the `.popsection` is removed, we actually get a nice error:

```
<inline asm>:8:1: error: starting new .cfi frame before finishing the previous one
.cfi_endproc
^
Compiler returned: 1
```

I think that `.popsection` should emit an error when the section opened a CFI scope that wasn't yet closed.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Folkert de Vries (folkertdev)

<details>
LLVM crashes on this program

https://godbolt.org/z/9W51vc3bW

```llvm
module asm ".pushsection .text.foo,\22ax\22, @<!-- -->progbits"
module asm ".type foo, @<!-- -->function"
module asm "foo:"
module asm ".cfi_startproc"
module asm ".popsection"

module asm ".type bar, @<!-- -->function"
module asm "bar:"
module asm ".cfi_startproc"
module asm ".cfi_endproc"
```

```
llc: /root/llvm-project/llvm/lib/MC/MCExpr.cpp:223: llvm::MCSymbolRefExpr::MCSymbolRefExpr(const llvm::MCSymbol*, llvm::MCExpr::Spec, const llvm::MCAsmInfo*, llvm::SMLoc): Assertion `Symbol' failed.
```

This assembly is malformed (but in a way that might come up during development). When the `.popsection` is removed, we actually get a nice error:

```
&lt;inline asm&gt;:8:1: error: starting new .cfi frame before finishing the previous one
.cfi_endproc
^
Compiler returned: 1
```

I think that `.popsection` should emit an error when the section opened a CFI scope that wasn't yet closed.

</details>


---

