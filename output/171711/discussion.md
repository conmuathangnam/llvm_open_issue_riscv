# [RISC-V] Crash when assembling `la.tlsdesc`

**Author:** arichardson
**URL:** https://github.com/llvm/llvm-project/issues/171711

## Body

I am not sure if this instruction is supposed to be supported in the assembler, but I was just trying to write an unrelated test and found that this crashes since it's not handled in RISCVAsmParser.cpp. Should this have `isCodeGenOnly = 1` or is it intended for the assembler? Wasn't quite clear to me from looking at https://github.com/llvm/llvm-project/commit/03a61d34ebf4f8eeaa6861bec3ab39c75bb41778.

Reproducer (https://godbolt.org/z/zzE4cjG53): `la.tlsdesc a0, sym`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alexander Richardson (arichardson)

<details>
I am not sure if this instruction is supposed to be supported in the assembler, but I was just trying to write an unrelated test and found that this crashes since it's not handled in RISCVAsmParser.cpp. Should this have `isCodeGenOnly = 1` or is it intended for the assembler? Wasn't quite clear to me from looking at https://github.com/llvm/llvm-project/commit/03a61d34ebf4f8eeaa6861bec3ab39c75bb41778.

Reproducer (https://godbolt.org/z/zzE4cjG53): `la.tlsdesc a0, sym`
</details>


---

### Comment 2 - topperc

I don't see it in binutils assembler mnemonics so probably just need `isCodeGenOnly = 1`.

---

### Comment 3 - lenary

This does seem like an oversight, but I think we probably should support it rather than not, given we support `la.tls.gd` and `la.tls.ie` pseudos (though I don't know how well they are supported)

One thing is that there's no docs for the TLS-related assembly instructions in https://github.com/riscv-non-isa/riscv-asm-manual/ - we should probably correct that too.


---

### Comment 4 - jrtc27

You can't support it as-is in the assembler, since it needs two scratch registers that get created in RISCVPreRAExpandPseudo. Though I suppose you could make it use a0 and t0 as the scratch registers, they're being clobbered anyway so there's not much benefit to using any other registers. That or you make them explicit operands, like how the PC-relative store pseudos take a scratch register operand after the symbol.

---

### Comment 5 - jrtc27

la.tls.{gd,ie} are well-supported in both toolchains, and mentioned in the psABI spec. LLD's own tests use them too.

---

### Comment 6 - arichardson

> You can't support it as-is in the assembler, since it needs two scratch registers that get created in RISCVPreRAExpandPseudo. Though I suppose you could make it use a0 and t0 as the scratch registers, they're being clobbered anyway so there's not much benefit to using any other registers. That or you make them explicit operands, like how the PC-relative store pseudos take a scratch register operand after the symbol.

Thanks, that (and lack of binutils support) sounds like a good reason to set `isCodeGenOnly = 1`.

---

### Comment 7 - lenary

> la.tls.{gd,ie} are well-supported in both toolchains, and mentioned in the psABI spec. LLD's own tests use them too.

I recalled they were documented somewhere, I looked in the wrong place.

I'm happy to set `isCodegenOnly = true`.

---

