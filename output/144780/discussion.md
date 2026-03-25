# [llvm] Miscompilation of freeze instruction with llc -O1

**Author:** 8051Enthusiast
**URL:** https://github.com/llvm/llvm-project/issues/144780
**Status:** Open
**Labels:** llvm:codegen, miscompilation

## Body

If my understanding of the freeze instruction is correct, given `n`, the following code should always return a number that is of the form `n + 2 * k` where `k` can be anything.
```llvm
define i32 @preserve_parity(i32 %n) {
    %2 = freeze i32 poison
    %3 = add i32 %n, %2
    %4 = add i32 %3, %2
    ret i32 %4
}
```
However, for example on x86_64 `llc` (with optimization level at least 1) outputs the following code:
```asm
preserve_parity:                        # @preserve_parity
        mov     eax, edi
        add     eax, eax
        add     eax, eax
        ret
```
Given that this always outputs an even number, but `n + 2 * k` is always odd for odd `n`, this function seems to behave incorrectly for odd n.
This does not only happen on x86_64, but a bunch of other backends too: https://godbolt.org/z/nzM4Mo98v.

## Comments

### Comment 1 - topperc

I didn't look at this test, but if I recall correctly, freeze is lower to a copy in MIR. If the input to the copy is undef, various copy propagation optimization can propagate the undef and break the freeze.

---

### Comment 2 - wareya

Per docs:

> The ‘freeze’ instruction is used to stop propagation of [undef](https://llvm.org/docs/LangRef.html#undefvalues) and [poison](https://llvm.org/docs/LangRef.html#poisonvalues) values.

> If the argument is undef or poison, ‘freeze’ returns an arbitrary, but fixed, value of type ‘ty’. Otherwise, this instruction is a no-op and returns the input argument. All uses of a value returned by the same ‘freeze’ instruction are guaranteed to always observe the same value, while different ‘freeze’ instructions may yield different values.

So a given execution of a given freeze instruction should produce one particular (possibly unknown) bit state.

---

### Comment 3 - topperc

https://github.com/llvm/llvm-project/issues/58321 is another report for the same issue.

---

