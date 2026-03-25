# [InferAddressSpaces] Verifier crash after inserting `addrspacecast` before phi-node

**Author:** Kerang-BR
**URL:** https://github.com/llvm/llvm-project/issues/162982
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-11-17T03:00:17Z

## Body

The `infer-address-spaces` pass attempts to insert `addrspacecast` instructions before phi-node in some corner cases.  This triggers the verifier crash when compiling.

See details in  https://godbolt.org/z/1nW61Pc75

## Comments

### Comment 1 - Kerang-BR

Fixed

---

