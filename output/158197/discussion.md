# [VectorCombine] scalarizeExtExtract uses LE lane mapping on BE targets

**Author:** uyoyo0
**URL:** https://github.com/llvm/llvm-project/issues/158197
**Status:** Closed
**Labels:** miscompilation, release:backport, llvm::vectorcombine
**Closed Date:** 2025-09-17T08:51:35Z

## Body

The `scalarizeExtExtract` combine packs <N x i8> into i(N*8) and then extracts lanes via lshr/and. The shift amount assumes little-endian ordering which is incorrect for big-endian targets.

This causes miscompiles on big-endian targets such as AIX/PowerPC under -O3 -flto. 

## Comments

### Comment 1 - RKSimon

Reopening - will backport once its had time to stew for a few days in trunk

---

### Comment 2 - RKSimon

/cherry-pick 994a6a39e13dcc335247a127a5da05905d1ac541

---

### Comment 3 - llvmbot

/pull-request llvm/llvm-project#159286

---

