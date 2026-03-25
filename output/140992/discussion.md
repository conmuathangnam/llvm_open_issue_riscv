# [InstCombine] Do not rely on the nsz flag on fcmp

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/140992
**Status:** Closed
**Labels:** metaissue, miscompilation, llvm:instcombine, floating-point
**Closed Date:** 2025-06-02T15:16:56Z

## Body

As discussed in https://discourse.llvm.org/t/rfc-clarify-the-behavior-of-fp-operations-on-bit-strings-with-nsz-flag/85981, forbidding nsz flags in bitwise FP operations (select/phi/copysign/fabs/fneg) is infeasible as it blocks some common optimizations (e.g., `(X < 0.0) ? -X : X -> fabs(X)`). I am working on a better formal definition for the nsz flag (and other rewrite-based flags).

However, the nsz flag on fcmp is meaningless since fcmp treats both positive and negative zero as equal. It allows us to freely add nsz: https://alive2.llvm.org/ce/z/x2iNNo
Unfortunately, I notice that some existing optimizations in InstCombine rely on the nsz flag on fcmp. It is a strange behavior and causes some miscompilations (e.g., https://alive2.llvm.org/ce/z/J9ZBXX).

I have created a [tool](https://github.com/dtcxzyw/llvm-tools/blob/main/fcmp-nsz.py) for metamorphic testing, to ensure that the nsz flag on fcmp doesn't affect the optimized IR.


