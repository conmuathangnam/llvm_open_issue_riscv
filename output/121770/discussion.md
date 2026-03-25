# [JumpThreading] jump threading pass hangs

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121770

## Body

Reproducer: https://godbolt.org/z/x6TPjPszv
```
; bin/opt -passes=jump-threading reduced.ll -S
define i32 @func_134(i1 %cmp274) {
entry:
  br label %for.cond227

for.cond227:                                      ; preds = %for.cond271.for.cond227_crit_edge, %for.cond227, %entry
  br i1 false, label %for.cond227.for.cond271_crit_edge, label %for.cond227

for.cond227.for.cond271_crit_edge:                ; preds = %for.cond227
  br label %for.cond271

for.cond271:                                      ; preds = %func_166.exit, %for.cond227.for.cond271_crit_edge
  %call241.pn = phi { i64, i64 } [ %.fca.1.insert.i, %func_166.exit ], [ zeroinitializer, %for.cond227.for.cond271_crit_edge ]
  br i1 %cmp274, label %for.body276, label %for.cond271.for.cond227_crit_edge

for.cond271.for.cond227_crit_edge:                ; preds = %for.cond271
  br label %for.cond227

for.body276:                                      ; preds = %for.cond271
  %l_2045.sroa.0.0 = extractvalue { i64, i64 } %call241.pn, 0
  %cmp.i.i = icmp eq i64 %l_2045.sroa.0.0, 0
  br i1 %cmp.i.i, label %func_166.exit, label %cond.false.i.i

cond.false.i.i:                                   ; preds = %for.body276
  br label %func_166.exit

func_166.exit:                                    ; preds = %cond.false.i.i, %for.body276
  %.fca.1.insert.i = insertvalue { i64, i64 } %call241.pn, i64 0, 1
  br label %for.cond271
}
```


