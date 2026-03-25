# [Inliner]  Assertion "Referenced value not in value map!" failed.

**Author:** JonPsson
**URL:** https://github.com/llvm/llvm-project/issues/48409
**Status:** Closed
**Labels:** backend:SystemZ, bugzilla, llvm:optimizations, crash
**Closed Date:** 2025-04-15T11:59:16Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [49065](https://llvm.org/bz49065) |
| Version | trunk |
| OS | Linux |
| Attachments | [reduced testcase](https://user-images.githubusercontent.com/55128014/143761748-a6f37f75-ac30-4c52-a012-db02c2ff1050.gz) |
| CC | @LebedevRI,@JonPsson |

## Extended Description 


First bad commit: 1742203 "[SimplifyCFG] FoldBranchToCommonDest(): re-lift restrictions on liveout uses of bonus instructions"

```
opt -mtriple=s390x-unknown-linux -mcpu=arch13 -o out.opt.ll -O3 -S -bonus-inst-threshold=2 -disable-basic-aa tc.ll

 #6 0x000003ffbbec08b2 __assert_fail_base /build/glibc-8Bkpa0/glibc-2.30/assert/assert.c:89:7
 #7 0x000003ffbbec0926 (/lib/s390x-linux-gnu/libc.so.6+0x40926)
 #8 0x000002aa3f3c25f0 (anonymous namespace)::Mapper::remapInstruction(llvm::Instruction*) (./bin/opt+0x1a425f0)
 #9 0x000002aa3f3c418e llvm::ValueMapper::remapInstruction(llvm::Instruction&) (./bin/opt+0x1a4418e)
#10 0x000002aa3f2549aa (anonymous namespace)::PruningFunctionCloner::CloneBlock(llvm::BasicBlock const*, llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void>, false, true>, std::vector<llvm::BasicBlock const*, std::allocator<llvm::BasicBlock const*> >&) (./bin/opt+0x18d49aa)
#11 0x000002aa3f255c00 llvm::CloneAndPruneIntoFromInst(llvm::Function*, llvm::Function const*, llvm::Instruction const*, llvm::ValueMap<llvm::Value const*, llvm::WeakTrackingVH, llvm::ValueMapConfig<llvm::Value const*, llvm::sys::SmartMutex<false> > >&, bool, llvm::SmallVectorImpl<llvm::ReturnInst*>&, char const*, llvm::ClonedCodeInfo*) (./bin/opt+0x18d5c00)
#12 0x000002aa3f2576e6 llvm::CloneAndPruneFunctionInto(llvm::Function*, llvm::Function const*, llvm::ValueMap<llvm::Value const*, llvm::WeakTrackingVH, llvm::ValueMapConfig<llvm::Value const*, llvm::sys::SmartMutex<false> > >&, bool, llvm::SmallVectorImpl<llvm::ReturnInst*>&, char const*, llvm::ClonedCodeInfo*, llvm::Instruction*) (./bin/opt+0x18d76e6)
#13 0x000002aa3f2a7400 llvm::InlineFunction(llvm::CallBase&, llvm::InlineFunctionInfo&, llvm::AAResults*, bool, llvm::Function*) (./bin/opt+0x1927400)
```

## Comments

### Comment 1 - Endilll

@JonPsson Are you still experiencing this on trunk?

---

### Comment 2 - dominik-steenken

```
$ bin/opt -mtriple=s390x-unknown-linux -mcpu=arch13 -o out.opt.ll -O3 -S -bonus-inst-threshold=2 -disable-basic-aa tc.ll ; echo $?
0

$ bin/opt --version
LLVM (http://llvm.org/):
  LLVM version 21.0.0git
  Optimized build with assertions.
  Default target: s390x-unknown-linux-gnu
  Host CPU: z16

$ git rev-parse HEAD
88d0b0835d030635c5d08c9a9754c21b5ac00be9
```

As far as i can tell, this is no longer observed on current `main`.

---

