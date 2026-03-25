# MemorySSA incorrectly handles loop-varying clobbers of locations in calls

**Author:** ormris
**URL:** https://github.com/llvm/llvm-project/issues/54682
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2026-03-19T09:18:40Z

## Body

The following test case produces no output at O2 when "B" is expected. We've bisected this issue to commit f5446b769a7929806f72256fccd4826d66502e59:

```
  commit f5446b769a7929806f72256fccd4826d66502e59
  Author: Olle Fredriksson [fredriksson.olle@gmail.com](mailto:fredriksson.olle@gmail.com)
  Date:   Wed Apr 21 22:48:28 2021 +0200

      [MemCpyOpt] Allow variable lengths in memcpy optimizer

      This makes the memcpy-memcpy and memcpy-memset optimizations work for
      variable sizes as long as they are equal, relaxing the old restriction
      that they are constant integers. If they're not equal, the old
      requirement that they are constant integers with certain size
      restrictions is used.

      The implementation works by pushing the length tests further down in the
      code, which reveals some places where it's enough that the lengths are
      equal (but not necessarily constant).

      Differential Revision: https://reviews.llvm.org/D100870
```

Steps to reproduce:

```
$ cat test.cpp
#include <string.h>
#include <stdio.h>

void testcase(const char *path, char *name)
{
  if (name)
  {
    *name = 0;
  }
  char tmp[260];
  int pos = (int)strlen(path) - 1;
  int pos2 = sizeof(tmp) - 1;
  bool loop = true;
  while (pos >= 0 && pos2 >= 0 && loop)
  {
    switch (path[pos])
    {
      case '\\':
      case '/':
        memcpy(name, tmp + pos2 + 1, sizeof(tmp) - pos2 - 1);
        name[sizeof(tmp) - pos2 - 1] = 0;
        loop = false;
        break;
      default:
        tmp[pos2--] = path[pos--];
        break;
    }
  }
}
int main(void)
{
  char name[260];
  testcase("A/B", name);
  printf("%s\n", name);

  return 1;
}
$ clang -O2 test.cpp
$ ./a.out # "B" is the expected output

$ clang -O1 test.cpp
$ ./a.out
B
$
```

A couple of things I noticed:

  1. Moving tmp to global scope seems to workaround this issue.

  2. A zero length memcpy is possible, if the input begins with a "/".

Thanks in advance for any help on this.

## Comments

### Comment 1 - nikic

Relevant IR before memcpyopt: https://gist.github.com/nikic/f9f4c34a9868096ac4b9231f0e5d6488 MemCpyOpt is perform a "memcpy from undef" optimization here.

---

### Comment 2 - nikic

Reduced test case:
```llvm
define void @test(i1 %c, ptr nocapture noundef readonly %path, ptr noundef writeonly %name) {
entry:
  %tmp = alloca [260 x i8], align 16
  br label %while.body

while.body:
  %iv = phi i64 [ %iv.next, %while.body ], [ 259, %entry ]
  %iv.next = add nsw i64 %iv, -1
  %tmp.iv = getelementptr inbounds [260 x i8], ptr %tmp, i64 0, i64 %iv
  store i8 42, i8* %tmp.iv, align 1
  br i1 %c, label %while.body, label %exit

exit:
  %tmp.iv.1 = getelementptr inbounds i8, ptr %tmp.iv, i64 1
  %sub7 = sub nsw i64 259, %iv
  call void @llvm.memcpy.p0i8.p0i8.i64(ptr align 1 %name, ptr nonnull align 1 %tmp.iv.1, i64 %sub7, i1 false)
  ret void
}
```

With `-passes='print<memoryssa-walker>'` this gives:
```llvm
define void @test(i1 %c, ptr noundef readonly captures(none) %path, ptr noundef writeonly %name) {
entry:
  %tmp = alloca [260 x i8], align 16
  br label %while.body

while.body:                                       ; preds = %while.body, %entry
; 3 = MemoryPhi({entry,liveOnEntry},{while.body,1})
  %iv = phi i64 [ %iv.next, %while.body ], [ 259, %entry ]
  %iv.next = add nsw i64 %iv, -1
  %tmp.iv = getelementptr inbounds [260 x i8], ptr %tmp, i64 0, i64 %iv
; 1 = MemoryDef(3)->3 - clobbered by 3 = MemoryPhi({entry,liveOnEntry},{while.body,1})
  store i8 42, ptr %tmp.iv, align 1
  br i1 %c, label %while.body, label %exit

exit:                                             ; preds = %while.body
  %tmp.iv.1 = getelementptr inbounds i8, ptr %tmp.iv, i64 1
  %sub7 = sub nsw i64 259, %iv
; 2 = MemoryDef(1)->liveOnEntry - clobbered by liveOnEntry
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %name, ptr nonnull align 1 %tmp.iv.1, i64 %sub7, i1 false)
  ret void
}
```

It says the memcpy is clobbered by liveOnEntry, but it should be clobbered by the MemoryPhi node, as the store in the loop clobbers the memory on the previous iteration.

cc @alinas @fhahn 

---

### Comment 3 - nikic

I think this happens because the upward_defs_iterator only does something meaningful (i.e. phi translation and loop invariance check) when working on a MemoryLocation. But if the clobber query is against a call, we don't do anything. But we still end up using non-translated/non-invariant MemoryLocations for individual function arguments in getModRefInfo queries.

---

### Comment 4 - nikic

Work around for MemCpyOpt: https://reviews.llvm.org/D122911

---

### Comment 5 - alinas

I looked into this in detail. The MemorySSA result does look incorrect, unfortunately all is working as intended.
The clobbering information in MemorySSA is based on the results from alias analysis, which does not have access to loop info; while MSSA does (some)phi translation, it does not do loop invariance check. So safety checks that rely on iteration based accesses need to be done by the passes themselves. 
This was fairly clear in loop passes, but in this instance it affected a function pass which also doesn't have access to loop info (an access outside a loop is not seeing the aliasing relation with an access in a loop in another iteration of said loop).
So, given the status quo, the work around in MemCpyOpt is the proper solution for fixing the issue.


---

### Comment 6 - nikic

@alinas MemorySSA *does* perform loop invariance checks in https://github.com/llvm/llvm-project/blob/6b0bed7ea563624622c3c1fb1a3c90cd32c78db6/llvm/include/llvm/Analysis/MemorySSA.h#L1255-L1294 though -- it's a crude check, but it's conservative, i.e. MemorySSA will assume loop variance outside of obviously invariant cases. (But this is only done for MemoryLocations, not for calls.)

---

### Comment 7 - fhahn

> bool IsGuaranteedLoopInvariant(Value *Ptr) const;

I think this was needed to fix another correctness issue. So maybe it's just a case of using it elsewhere. `DSE` has its own implementation of something similar, might be good to unify those.

---

### Comment 8 - alinas

Circling back to this, I'm looking if this can be added to MemorySSA.

---

### Comment 9 - fhahn

Sounds great, thanks!

---

### Comment 10 - nikic

BasicAA has since gained the "cross-iteration mode". I've created this fix sketch based on that: https://github.com/nikic/llvm-project/commit/944b225ddb4f227d46466f1409c62608651460e2

Interestingly, making use of that not just for calls but also for locations both improves results on existing tests and fixes a miscompile: https://github.com/nikic/llvm-project/commit/52174f0f7bed5a1d0d4a3f4d06cae7f1c2d78157

---

