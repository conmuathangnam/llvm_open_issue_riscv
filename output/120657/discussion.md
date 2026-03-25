# In LTO builds, PostOrderFunctionAttrsPass can infer that `__cxa_throw` is `nounwind`

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/120657
**Status:** Open
**Labels:** libunwind, miscompilation, LTO

## Body

When trying to enable LTO/FatLTO in the runtimes build, I hit a single test failure in RTSan, ThrowingAnExceptionDiesWhenRealtime(.

After some investigation, I've found that the test, which has a trivial try/catch w/ an explicit `throw` is miscompiled.

Before GlobalOpt the IR has a an invoke to `__cxa_throw` and some code to catch the exception in one of its successors
```llvm
; Function Attrs: cold mustprogress noinline uwtable
define dso_local void @func() #0 personality ptr @__gxx_personality_v0 {
  %1 = tail call ptr @__cxa_allocate_exception(i64 8) #6
  store ptr getelementptr inbounds nuw inrange(-16, 24) (i8, ptr @_ZTVSt9exception, i64 16), ptr %1, align 8, !tbaa !4478
  invoke void @__cxa_throw(ptr nonnull %1, ptr nonnull @_ZTISt9exception, ptr nonnull @_ZNSt9exceptionD1Ev) #7
          to label %11 unwind label %2

2:                                                ; preds = %0
  %3 = landingpad { ptr, i32 }
          catch ptr @_ZTISt9exception
  %4 = extractvalue { ptr, i32 } %3, 1
  %5 = tail call i32 @llvm.eh.typeid.for.p0(ptr nonnull @_ZTISt9exception) #6
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %7, label %10

7:                                                ; preds = %2
  %8 = extractvalue { ptr, i32 } %3, 0
  %9 = tail call ptr @__cxa_begin_catch(ptr %8) #6
  tail call void @__cxa_end_catch()
  ret void

10:                                               ; preds = %2
  resume { ptr, i32 } %3

11:                                               ; preds = %0
  unreachable
}

After GlobalOpt runs the catch handling is removed: 

```llvm
; Function Attrs: cold mustprogress noinline uwtable
define dso_local void @func() local_unnamed_addr #0 personality ptr @__gxx_personality_v0 {
  %1 = tail call ptr @__cxa_allocate_exception(i64 8) #4
  store ptr getelementptr inbounds nuw inrange(-16, 24) (i8, ptr @_ZTVSt9exception, i64 16), ptr %1, align 8, !tbaa !4478
  call void @__cxa_throw(ptr nonnull %1, ptr nonnull @_ZTISt9exception, ptr nonnull @_ZNSt9exceptionD1Ev) #5
  unreachable
}
```

This happens because w/in GlobalOpt the analysis checks if the ivoke instruction's callee is marked `nounwind`, and in this case it is. I've run the change of attributes for `__cxa_throw` back to the attribute inference in FunctionAttrs.cpp. Specifically, it seems that the logic in `IstrBreaksNonThrowing` is incomplete, since its determining that `__cxa_throw` is `nounwind`

```llvm
; *** IR Dump After PGOIndirectCallPromotion on [module] ***
; Function Attrs: mustprogress noreturn uwtable
define dso_local void @__cxa_throw(ptr noundef initializes((-120, -80), (-32, -16)) %0, ptr noundef %1, ptr noundef %2) #24 !dbg !51606 {
    #dbg_value(ptr %0, !51610, !DIExpression(), !51615)
    #dbg_value(ptr %1, !51611, !DIExpression(), !51615)
    #dbg_value(ptr %2, !51612, !DIExpression(), !51615)
  %4 = tail call ptr @__cxa_get_globals(), !dbg !51616
    #dbg_value(ptr %4, !51613, !DIExpression(), !51615)
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 8, !dbg !51617
  %6 = load i32, ptr %5, align 8, !dbg !51618, !tbaa !51457
  %7 = add i32 %6, 1, !dbg !51618
  store i32 %7, ptr %5, align 8, !dbg !51618, !tbaa !51457
  %8 = tail call ptr @__cxa_init_primary_exception(ptr noundef %0, ptr noundef %1, ptr noundef %2) #59, !dbg !51619
    #dbg_value(ptr %8, !51614, !DIExpression(), !51615)
  %9 = getelementptr inbounds nuw i8, ptr %8, i64 8, !dbg !51620
  store i64 1, ptr %9, align 8, !dbg !51621, !tbaa !51495
  %10 = getelementptr inbounds nuw i8, ptr %8, i64 96, !dbg !51622
  %11 = tail call i32 @_Unwind_RaiseException(ptr noundef nonnull %10), !dbg !51623
  tail call fastcc void @_ZN10__cxxabiv1L12failed_throwEPNS_15__cxa_exceptionE(ptr noundef nonnull %8) #60, !dbg !51624
  unreachable, !dbg !51624
}
; *** IR Dump After PostOrderFunctionAttrsPass on (__cxa_throw) ***
; Function Attrs: cold mustprogress noreturn nounwind uwtable
define dso_local void @__cxa_throw(ptr noundef initializes((-120, -80), (-32, -16)) %0, ptr noundef %1, ptr noundef %2) #36 !dbg !8793 {
    #dbg_value(ptr %0, !8798, !DIExpression(), !8803)
    #dbg_value(ptr %1, !8799, !DIExpression(), !8803)
    #dbg_value(ptr %2, !8800, !DIExpression(), !8803)
  %4 = tail call ptr @__cxa_get_globals(), !dbg !8804
    #dbg_value(ptr %4, !8801, !DIExpression(), !8803)
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 8, !dbg !8805
  %6 = load i32, ptr %5, align 8, !dbg !8806, !tbaa !8807
  %7 = add i32 %6, 1, !dbg !8806
  store i32 %7, ptr %5, align 8, !dbg !8806, !tbaa !8807
  %8 = tail call ptr @__cxa_init_primary_exception(ptr noundef %0, ptr noundef %1, ptr noundef %2) #60, !dbg !8814
    #dbg_value(ptr %8, !8802, !DIExpression(), !8803)
  %9 = getelementptr inbounds nuw i8, ptr %8, i64 8, !dbg !8815
  store i64 1, ptr %9, align 8, !dbg !8816, !tbaa !8817
  %10 = getelementptr inbounds nuw i8, ptr %8, i64 96, !dbg !8824
  %11 = tail call i32 @_Unwind_RaiseException(ptr noundef nonnull %10), !dbg !8825
  tail call fastcc void @_ZN10__cxxabiv1L12failed_throwEPNS_15__cxa_exceptionE(ptr noundef nonnull %8) #61, !dbg !8826
  unreachable, !dbg !8826
}
```

I would have assumed that the call to` _Unwind_RaiseException` would prevent this from happening.





## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Paul Kirth (ilovepi)

<details>
When trying to enable LTO/FatLTO in the runtimes build, I hit a single test failure in RTSan, ThrowingAnExceptionDiesWhenRealtime(.

After some investigation, I've found that the test, which has a trivial try/catch w/ an explicit `throw` is miscompiled.

Before GlobalOpt the IR has a an invoke to `__cxa_throw` and some code to catch the exception in one of its successors
```llvm
; Function Attrs: cold mustprogress noinline uwtable
define dso_local void @<!-- -->func() #<!-- -->0 personality ptr @<!-- -->__gxx_personality_v0 {
  %1 = tail call ptr @<!-- -->__cxa_allocate_exception(i64 8) #<!-- -->6
  store ptr getelementptr inbounds nuw inrange(-16, 24) (i8, ptr @<!-- -->_ZTVSt9exception, i64 16), ptr %1, align 8, !tbaa !4478
  invoke void @<!-- -->__cxa_throw(ptr nonnull %1, ptr nonnull @<!-- -->_ZTISt9exception, ptr nonnull @<!-- -->_ZNSt9exceptionD1Ev) #<!-- -->7
          to label %11 unwind label %2

2:                                                ; preds = %0
  %3 = landingpad { ptr, i32 }
          catch ptr @<!-- -->_ZTISt9exception
  %4 = extractvalue { ptr, i32 } %3, 1
  %5 = tail call i32 @<!-- -->llvm.eh.typeid.for.p0(ptr nonnull @<!-- -->_ZTISt9exception) #<!-- -->6
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %7, label %10

7:                                                ; preds = %2
  %8 = extractvalue { ptr, i32 } %3, 0
  %9 = tail call ptr @<!-- -->__cxa_begin_catch(ptr %8) #<!-- -->6
  tail call void @<!-- -->__cxa_end_catch()
  ret void

10:                                               ; preds = %2
  resume { ptr, i32 } %3

11:                                               ; preds = %0
  unreachable
}

After GlobalOpt runs the catch handling is removed: 

```llvm
; Function Attrs: cold mustprogress noinline uwtable
define dso_local void @<!-- -->func() local_unnamed_addr #<!-- -->0 personality ptr @<!-- -->__gxx_personality_v0 {
  %1 = tail call ptr @<!-- -->__cxa_allocate_exception(i64 8) #<!-- -->4
  store ptr getelementptr inbounds nuw inrange(-16, 24) (i8, ptr @<!-- -->_ZTVSt9exception, i64 16), ptr %1, align 8, !tbaa !4478
  call void @<!-- -->__cxa_throw(ptr nonnull %1, ptr nonnull @<!-- -->_ZTISt9exception, ptr nonnull @<!-- -->_ZNSt9exceptionD1Ev) #<!-- -->5
  unreachable
}
```

This happens because w/in GlobalOpt the analysis checks if the ivoke instruction's callee is marked `nounwind`, and in this case it is. I've run the change of attributes for `__cxa_throw` back to the attribute inference in FunctionAttrs.cpp. Specifically, it seems that the logic in `IstrBreaksNonThrowing` is incomplete, since its determining that `__cxa_throw` is `nounwind`

```llvm
; *** IR Dump After PGOIndirectCallPromotion on [module] ***
; Function Attrs: mustprogress noreturn uwtable
define dso_local void @<!-- -->__cxa_throw(ptr noundef initializes((-120, -80), (-32, -16)) %0, ptr noundef %1, ptr noundef %2) #<!-- -->24 !dbg !51606 {
    #dbg_value(ptr %0, !51610, !DIExpression(), !51615)
    #dbg_value(ptr %1, !51611, !DIExpression(), !51615)
    #dbg_value(ptr %2, !51612, !DIExpression(), !51615)
  %4 = tail call ptr @<!-- -->__cxa_get_globals(), !dbg !51616
    #dbg_value(ptr %4, !51613, !DIExpression(), !51615)
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 8, !dbg !51617
  %6 = load i32, ptr %5, align 8, !dbg !51618, !tbaa !51457
  %7 = add i32 %6, 1, !dbg !51618
  store i32 %7, ptr %5, align 8, !dbg !51618, !tbaa !51457
  %8 = tail call ptr @<!-- -->__cxa_init_primary_exception(ptr noundef %0, ptr noundef %1, ptr noundef %2) #<!-- -->59, !dbg !51619
    #dbg_value(ptr %8, !51614, !DIExpression(), !51615)
  %9 = getelementptr inbounds nuw i8, ptr %8, i64 8, !dbg !51620
  store i64 1, ptr %9, align 8, !dbg !51621, !tbaa !51495
  %10 = getelementptr inbounds nuw i8, ptr %8, i64 96, !dbg !51622
  %11 = tail call i32 @<!-- -->_Unwind_RaiseException(ptr noundef nonnull %10), !dbg !51623
  tail call fastcc void @<!-- -->_ZN10__cxxabiv1L12failed_throwEPNS_15__cxa_exceptionE(ptr noundef nonnull %8) #<!-- -->60, !dbg !51624
  unreachable, !dbg !51624
}
; *** IR Dump After PostOrderFunctionAttrsPass on (__cxa_throw) ***
; Function Attrs: cold mustprogress noreturn nounwind uwtable
define dso_local void @<!-- -->__cxa_throw(ptr noundef initializes((-120, -80), (-32, -16)) %0, ptr noundef %1, ptr noundef %2) #<!-- -->36 !dbg !8793 {
    #dbg_value(ptr %0, !8798, !DIExpression(), !8803)
    #dbg_value(ptr %1, !8799, !DIExpression(), !8803)
    #dbg_value(ptr %2, !8800, !DIExpression(), !8803)
  %4 = tail call ptr @<!-- -->__cxa_get_globals(), !dbg !8804
    #dbg_value(ptr %4, !8801, !DIExpression(), !8803)
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 8, !dbg !8805
  %6 = load i32, ptr %5, align 8, !dbg !8806, !tbaa !8807
  %7 = add i32 %6, 1, !dbg !8806
  store i32 %7, ptr %5, align 8, !dbg !8806, !tbaa !8807
  %8 = tail call ptr @<!-- -->__cxa_init_primary_exception(ptr noundef %0, ptr noundef %1, ptr noundef %2) #<!-- -->60, !dbg !8814
    #dbg_value(ptr %8, !8802, !DIExpression(), !8803)
  %9 = getelementptr inbounds nuw i8, ptr %8, i64 8, !dbg !8815
  store i64 1, ptr %9, align 8, !dbg !8816, !tbaa !8817
  %10 = getelementptr inbounds nuw i8, ptr %8, i64 96, !dbg !8824
  %11 = tail call i32 @<!-- -->_Unwind_RaiseException(ptr noundef nonnull %10), !dbg !8825
  tail call fastcc void @<!-- -->_ZN10__cxxabiv1L12failed_throwEPNS_15__cxa_exceptionE(ptr noundef nonnull %8) #<!-- -->61, !dbg !8826
  unreachable, !dbg !8826
}
```

I would have assumed that the call to` _Unwind_RaiseException` would prevent this from happening.




</details>


---

### Comment 2 - ilovepi

@nikic It seems like you looked at something similar in the pat w.r.t. attribute inference. Do you have a good idea of how to triage this? The right fix for the heuristic isn't super obvious to me.

---

### Comment 3 - nikic

What attributes does _Unwind_RaiseException have in your IR? I suspect that it is marked `nounwind` and that's why `__cxa_throw` also ends up being nounwind.

---

### Comment 4 - nikic

Is libunwind also part of your LTO build here? It looks like libunwind is built with `-fno-exceptions`, so `_Unwind_RaiseException` will end up as `nounwind`.

I think this needs to be fixed in libunwind by making sure this function at least is compiled with `-fexceptions` (or otherwise not marked as nounwind).

---

### Comment 5 - ilovepi

> Is libunwind also part of your LTO build here? It looks like libunwind is built with `-fno-exceptions`, so `_Unwind_RaiseException` will end up as `nounwind`.
> 
> I think this needs to be fixed in libunwind by making sure this function at least is compiled with `-fexceptions` (or otherwise not marked as nounwind).

Thanks for pointing that out. This does seem like the root cause. I'll take a look at libunwind after the holiday. In the short term I can probably either not LTO libunwind in our toolchain or otherwise work around that in our build.

---

