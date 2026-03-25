# [hexagon] Assertion `WD >= WS' failed in BitTracker.cpp [endgame]

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/181362
**Status:** Closed
**Labels:** backend:Hexagon, release:backport, release:cherry-pick-failed, crash
**Closed Date:** 2026-03-10T12:39:02Z

## Body

While building the endgame test case from llvm-test-suite using clang built from c780d890daf3474c73c4ffa736e155c619c3d0be, we see the assertion below.

```
clang: /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/Target/Hexagon/BitTracker.cpp:744: virtual bool llvm::BitTracker::MachineEvaluator::evaluate(const llvm::MachineInstr &, const llvm::BitTracker::CellMapType &, llvm::BitTracker::CellMapType &) const: Assertion `WD >= WS' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name endgame.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -w -ferror-limit 19 -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -vectorize-loops -vectorize-slp -faddrsig -x c /local/mnt/workspace/bcain-reduce-20260212_190946/case-endgame-becf7b/endgame-becf7b.c -o /dev/null
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-endgame-becf7b/endgame-becf7b.c'.
4.      Running pass 'Hexagon Loop Rescheduling' on function '@endgame'
...
```

## Comments

### Comment 1 - androm3da

Fixed by #181360

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
While building the endgame test case from llvm-test-suite using clang built from c780d890daf3474c73c4ffa736e155c619c3d0be, we see the assertion below.

```
clang: /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/Target/Hexagon/BitTracker.cpp:744: virtual bool llvm::BitTracker::MachineEvaluator::evaluate(const llvm::MachineInstr &amp;, const llvm::BitTracker::CellMapType &amp;, llvm::BitTracker::CellMapType &amp;) const: Assertion `WD &gt;= WS' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name endgame.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -w -ferror-limit 19 -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -vectorize-loops -vectorize-slp -faddrsig -x c /local/mnt/workspace/bcain-reduce-20260212_190946/case-endgame-becf7b/endgame-becf7b.c -o /dev/null
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-endgame-becf7b/endgame-becf7b.c'.
4.      Running pass 'Hexagon Loop Rescheduling' on function '@<!-- -->endgame'
...
```
</details>


---

### Comment 3 - androm3da

/cherry-pick 689ecf880373bb4e0f01ed5e004f19a466e869dc

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#182641

---

### Comment 5 - androm3da

Revert #182861 of previous attempt at fixing.

Given my track record lately, I think I should probably let @aankit-ca handle this one

---

### Comment 6 - kparzysz

Could you attach the preprocessed source?

Edit: I found endgame.c and it's only using system headers.  I'm going to try and see what I get with it.

Edit: I was able to reproduce the issue.

---

### Comment 7 - kparzysz

The problem is at this line: https://github.com/llvm/llvm-project/pull/164416/changes#diff-874988b656cd117ecddd76e0f265fa38c002eca553e55dfd71089ee79b59507cR1066

What gets stored in `Words` should always be 32-bit values.

---

### Comment 8 - androm3da

/cherry-pick f0289f00b5b85fda63cde96cda3eb38693adc534

---

### Comment 9 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: f0289f00b5b85fda63cde96cda3eb38693adc534

https://github.com/llvm/llvm-project/actions/runs/22366392263

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 10 - androm3da

If this failed to cherry-pick then it should be reopened.

---

### Comment 11 - androm3da

Opened a new PR #184735

---

