# [GlobalISel] Indirectify memory inputs for inline assembly

**Author:** Pierre-vh
**URL:** https://github.com/llvm/llvm-project/issues/139078
**Status:** Open
**Labels:** miscompilation, llvm:globalisel

## Body

We want memory input operands to be indirect in inline assembly, and if that's not the case then lowering fails.
https://github.com/llvm/llvm-project/pull/139077 was twice reverted because of it.

It looks like all we need to do is store the value on the stack then return the address of the stack slot to "indirectify" the input. Constants can be handled through the constant pool

see `getAddressForMemoryInput` in `SelectionDAGBuilder.cpp`

