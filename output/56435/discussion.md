# miscompilation involving musttail and DSE

**Author:** bostick
**URL:** https://github.com/llvm/llvm-project/issues/56435

## Body

I believe I am seeing a miscompilation involving `musttail` and DSE.

I have included a semi-boiled-down demonstration of the problem.

The relevant part is at the bottom of the function:
```llvm
  ...
  %add.ptr = getelementptr inbounds i8, ptr %15, i64 %vbase.offset
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(40) %agg.tmp17, ptr noundef nonnull align 8 dereferenceable(40) %token, i64 40, i1 false)
  musttail call void %call15(ptr noundef nonnull %add.ptr, ptr noundef nonnull %agg.tmp17)
  ret void
}
```

I run `opt -S --dse` and I get:
```llvm
  ...
  %add.ptr = getelementptr inbounds i8, ptr %15, i64 %vbase.offset
  musttail call void %call15(ptr noundef nonnull %add.ptr, ptr noundef nonnull %agg.tmp17)
  ret void
}
```

`%agg.tmp17` is passed to the function, so it is obviously not dead.

But the `@llvm.memcpy` is removed even though it is copying to `%agg.tmp17`.

It seems that `%agg.tmp17` is being treated as dead at the end of the function.


This depends on the `musttail` being present. If `musttail` is removed, then the `@llvm.memcpy` is correctly kept.


This comes from trying to use the `[[clang::musttail]]` annotation for a parser that I am writing. I see garbage passed to the function being called.


Let me know if there should more info or more work done in simplifying the example.


Here is the semi-boiled-down demo:
```llvm
; ModuleID = '../Parser.cpp'
source_filename = "../Parser.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx12.0.0"


%"class.std::__1::unique_ptr.66" = type { %"class.std::__1::__compressed_pair.67" }
%"class.std::__1::__compressed_pair.67" = type { %"struct.std::__1::__compressed_pair_elem.68" }
%"struct.std::__1::__compressed_pair_elem.68" = type { ptr }
%"struct.std::__1::array" = type { [440 x ptr] }


%struct.Context = type <{ ptr, ptr, i64, i8, [7 x i8] }>

%class.Parser = type { %"class.std::__1::vector", %"class.std::__1::vector.7", %"class.std::__1::vector", %"class.std::__1::vector.14", %class.TriviaSeq, %class.TriviaSeq }

%"class.std::__1::vector.7" = type { ptr, ptr, %"class.std::__1::__compressed_pair.8" }
%"class.std::__1::__compressed_pair.8" = type { %"struct.std::__1::__compressed_pair_elem.9" }
%"struct.std::__1::__compressed_pair_elem.9" = type { ptr }
%"class.std::__1::vector" = type { ptr, ptr, %"class.std::__1::__compressed_pair.4" }
%"class.std::__1::__compressed_pair.4" = type { %"struct.std::__1::__compressed_pair_elem.5" }
%"struct.std::__1::__compressed_pair_elem.5" = type { ptr }
%"class.std::__1::vector.14" = type { ptr, ptr, %"class.std::__1::__compressed_pair.15" }
%"class.std::__1::__compressed_pair.15" = type { %"struct.std::__1::__compressed_pair_elem.16" }
%"struct.std::__1::__compressed_pair_elem.16" = type { ptr }
%class.TriviaSeq = type { %"class.std::__1::vector.21" }
%"class.std::__1::vector.21" = type { ptr, ptr, %"class.std::__1::__compressed_pair.22" }
%"class.std::__1::__compressed_pair.22" = type { %"struct.std::__1::__compressed_pair_elem.23" }
%"struct.std::__1::__compressed_pair_elem.23" = type { ptr }

%struct.BufferAndLength = type { ptr, ptr }
%struct.Token = type <{ %struct.BufferAndLength, %struct.Source, %struct.TokenEnum, [6 x i8] }>
%struct.Source = type { %struct.SourceLocation, %struct.SourceLocation }
%struct.SourceLocation = type { i32, i32 }
%struct.TokenEnum = type { i16 }

%"class.std::__1::unique_ptr.47" = type { %"class.std::__1::__compressed_pair.48" }

%"class.std::__1::__compressed_pair.48" = type { %"struct.std::__1::__compressed_pair_elem.49" }

%"struct.std::__1::__compressed_pair_elem.49" = type { ptr }

@TheTokenizer = external global %"class.std::__1::unique_ptr.66", align 8

@infixParselets = external global %"struct.std::__1::array", align 8

@__func__._Z18Parser_tryContinueP8Parselet5Token = private unnamed_addr constant [19 x i8] c"Parser_tryContinue\00", align 1

@.str = private unnamed_addr constant [11 x i8] c"Parser.cpp\00", align 1

@.str.1 = private unnamed_addr constant [2 x i8] c"F\00", align 1

@TheParser = global { { ptr } } zeroinitializer, align 8

@_ZTISt18bad_variant_access = external constant ptr

@_ZTVSt18bad_variant_access = external unnamed_addr constant { [5 x ptr] }, align 8

@_ZTVSt9exception = available_externally unnamed_addr constant { [5 x ptr] } { [5 x ptr] [ptr null, ptr @_ZTISt9exception, ptr 

@_ZNSt9exceptionD1Ev, ptr @_ZNSt9exceptionD0Ev, ptr @_ZNKSt9exception4whatEv] }, align 8

@_ZTISt9exception = external constant ptr

@_ZTISt12length_error = external constant ptr

@_ZTVSt12length_error = available_externally unnamed_addr constant { [5 x ptr] } { [5 x ptr] [ptr null, ptr @_ZTISt12length_error, ptr 

@_ZNSt12length_errorD1Ev, ptr @_ZNSt12length_errorD0Ev, ptr @_ZNKSt11logic_error4whatEv] }, align 8



declare void @_ZN6Parser19pushContextAndShiftE10Precedence(ptr noundef nonnull align 8 dereferenceable(144) %this, i8 noundef zeroext %Prec) local_unnamed_addr #2 align 2

declare void @_ZN9Tokenizer12currentTokenEh(ptr sret(%struct.Token) align 8, ptr noundef nonnull align 8 dereferenceable(48), i8 noundef zeroext) local_unnamed_addr #3

declare void @_ZN9Tokenizer9nextTokenE5Token(ptr noundef nonnull align 8 dereferenceable(48), ptr noundef) local_unnamed_addr #3

declare void @_ZN9TriviaSeq5resetEv(ptr noundef nonnull align 8 dereferenceable(24)) local_unnamed_addr #3

declare void @_ZN9TriviaSeq6appendE5Token(ptr noundef nonnull align 8 dereferenceable(24), ptr noundef) local_unnamed_addr #3

declare noundef ptr @_ZNKSt11logic_error4whatEv(ptr noundef nonnull align 8 dereferenceable(16)) unnamed_addr #24

declare noundef ptr @_ZNKSt9exception4whatEv(ptr noundef nonnull align 8 dereferenceable(8)) unnamed_addr #24

declare void @_ZNSt12length_errorD0Ev(ptr noundef nonnull align 8 dereferenceable(16)) unnamed_addr #24

declare noundef ptr @_ZNSt12length_errorD1Ev(ptr noundef nonnull returned align 8 dereferenceable(16)) unnamed_addr #24

declare i64 @_ZNSt3__16__moveIP5TokenNS_20back_insert_iteratorINS_6vectorINS_7variantIJNS_10unique_ptrI4NodeNS_14default_deleteIS7_EEEES1_EEENS_9allocatorISB_EEEEEEEET0_T_SH_SG_(ptr noundef %__first, ptr noundef %__last, i64 %__result.coerce) local_unnamed_addr #14

declare void @_ZNSt9exceptionD0Ev(ptr noundef nonnull align 8 dereferenceable(8)) unnamed_addr #24

declare noundef ptr @_ZNSt9exceptionD1Ev(ptr noundef nonnull returned align 8 dereferenceable(8)) unnamed_addr #24

declare void @__assert_rtn(ptr noundef, ptr noundef, i32 noundef, ptr noundef) local_unnamed_addr #13

declare i32 @__gxx_personality_v0(...)

declare void @llvm.assume(i1 noundef) #21

declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #7

declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #7

declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #8




define void @_Z17Parser_parseClimbP8Parselet5Token(ptr nocapture noundef readnone %Ignored, ptr nocapture noundef readonly %Ignored2) local_unnamed_addr #2 {
entry:
  %agg.tmp.i29 = alloca %struct.Token, align 8
  %agg.tmp.i.i = alloca %struct.Token, align 8
  %agg.tmp.i = alloca %struct.Token, align 8
  %ref.tmp.i = alloca %struct.Token, align 8
  %token = alloca %struct.Token, align 8
  %agg.tmp17 = alloca %struct.Token, align 8
  %0 = load ptr, ptr @TheParser, align 8
  %trivia1.i = getelementptr inbounds %class.Parser, ptr %0, i64 0, i32 4
  call void @llvm.lifetime.start.p0(i64 40, ptr nonnull %token) #29
  %GroupStack.i = getelementptr inbounds %class.Parser, ptr %0, i64 0, i32 3
  %1 = load ptr, ptr %GroupStack.i, align 8
  %__end_.i.i = getelementptr inbounds %class.Parser, ptr %0, i64 0, i32 3, i32 1
  %2 = load ptr, ptr %__end_.i.i, align 8
  %cmp.i.i = icmp eq ptr %1, %2
  %3 = load ptr, ptr @TheTokenizer, align 8
  %neg.i = select i1 %cmp.i.i, i8 6, i8 2
  call void @_ZN9Tokenizer12currentTokenEh(ptr nonnull sret(%struct.Token) align 8 %token, ptr noundef nonnull align 8 dereferenceable(48) %3, i8 noundef zeroext %neg.i)
  %4 = load ptr, ptr @TheParser, align 8
  call void @llvm.lifetime.start.p0(i64 40, ptr nonnull %agg.tmp.i)
  %Tok.i = getelementptr inbounds %struct.Token, ptr %token, i64 0, i32 2
  %5 = load i16, ptr %Tok.i, align 8
  %6 = and i16 %5, 508
  %cmp.i6.i = icmp eq i16 %6, 8
  br i1 %cmp.i6.i, label %while.body.lr.ph.i, label %_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit

while.body.lr.ph.i:                               ; preds = %entry
  %GroupStack.i.i = getelementptr inbounds %class.Parser, ptr %4, i64 0, i32 3
  %__end_.i.i.i = getelementptr inbounds %class.Parser, ptr %4, i64 0, i32 3, i32 1
  br label %while.body.i

while.body.i:                                     ; preds = %while.body.i, %while.body.lr.ph.i
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(40) %agg.tmp.i, ptr noundef nonnull align 8 dereferenceable(40) %token, i64 40, i1 false)
  call void @_ZN9TriviaSeq6appendE5Token(ptr noundef nonnull align 8 dereferenceable(24) %trivia1.i, ptr noundef nonnull %agg.tmp.i)
  call void @llvm.lifetime.start.p0(i64 40, ptr nonnull %agg.tmp.i.i)
  %7 = load ptr, ptr @TheTokenizer, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(40) %agg.tmp.i.i, ptr noundef nonnull align 8 dereferenceable(40) %token, i64 40, i1 false)
  call void @_ZN9Tokenizer9nextTokenE5Token(ptr noundef nonnull align 8 dereferenceable(48) %7, ptr noundef nonnull %agg.tmp.i.i)
  call void @llvm.lifetime.end.p0(i64 40, ptr nonnull %agg.tmp.i.i)
  call void @llvm.lifetime.start.p0(i64 40, ptr nonnull %ref.tmp.i) #29
  %8 = load ptr, ptr %GroupStack.i.i, align 8
  %9 = load ptr, ptr %__end_.i.i.i, align 8
  %cmp.i.i.i = icmp eq ptr %8, %9
  %10 = load ptr, ptr @TheTokenizer, align 8
  %neg.i.i = select i1 %cmp.i.i.i, i8 6, i8 2
  call void @_ZN9Tokenizer12currentTokenEh(ptr nonnull sret(%struct.Token) align 8 %ref.tmp.i, ptr noundef nonnull align 8 dereferenceable(48) %10, i8 noundef zeroext %neg.i.i)
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(34) %token, ptr noundef nonnull align 8 dereferenceable(34) %ref.tmp.i, i64 34, i1 false)
  call void @llvm.lifetime.end.p0(i64 40, ptr nonnull %ref.tmp.i) #29
  %11 = load i16, ptr %Tok.i, align 8
  %12 = and i16 %11, 508
  %cmp.i.i26 = icmp eq i16 %12, 8
  br i1 %cmp.i.i26, label %while.body.i, label %_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit

_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit: ; preds = %while.body.i, %entry
  %13 = phi i16 [ %5, %entry ], [ %11, %while.body.i ]
  call void @llvm.lifetime.end.p0(i64 40, ptr nonnull %agg.tmp.i)
  %14 = and i16 %13, 511
  %conv = zext i16 %14 to i64
  %cmp.i = icmp ult i16 %14, 440
  call void @llvm.assume(i1 %cmp.i) #29
  %arrayidx.i = getelementptr inbounds [440 x ptr], ptr @infixParselets, i64 0, i64 %conv
  %15 = load ptr, ptr %arrayidx.i, align 8
  %vtable = load ptr, ptr %15, align 8
  %vfn = getelementptr inbounds ptr, ptr %vtable, i64 3
  %16 = load ptr, ptr %vfn, align 8
  %call6 = call noundef zeroext i8 %16(ptr noundef nonnull align 8 dereferenceable(8) %15)
  %17 = load ptr, ptr @TheParser, align 8
  %ContextStack.i = getelementptr inbounds %class.Parser, ptr %17, i64 0, i32 1
  %18 = load ptr, ptr %ContextStack.i, align 8
  %__end_.i.i27 = getelementptr inbounds %class.Parser, ptr %17, i64 0, i32 1, i32 1
  %19 = load ptr, ptr %__end_.i.i27, align 8
  %cmp.i.i28 = icmp eq ptr %18, %19
  br i1 %cmp.i.i28, label %_ZN6Parser13topPrecedenceEv.exit, label %if.end.i

if.end.i:                                         ; preds = %_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit
  %Prec.i = getelementptr %struct.Context, ptr %19, i64 -1, i32 3
  %20 = load i8, ptr %Prec.i, align 8
  %phi.bo = or i8 %20, 1
  br label %_ZN6Parser13topPrecedenceEv.exit

_ZN6Parser13topPrecedenceEv.exit:                 ; preds = %_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit, %if.end.i
  %retval.0.i = phi i8 [ %phi.bo, %if.end.i ], [ 1, %_ZN6Parser31eatTriviaButNotToplevelNewlinesER5TokenhR9TriviaSeq.exit ]
  %cmp = icmp ugt i8 %retval.0.i, %call6
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %_ZN6Parser13topPrecedenceEv.exit
  call void @_ZN9TriviaSeq5resetEv(ptr noundef nonnull align 8 dereferenceable(24) %trivia1.i)
  call void @llvm.lifetime.start.p0(i64 40, ptr nonnull %agg.tmp.i29)
  %21 = load ptr, ptr @TheParser, align 8
  %ContextStack.i.i = getelementptr inbounds %class.Parser, ptr %21, i64 0, i32 1
  %22 = load ptr, ptr %ContextStack.i.i, align 8
  %__end_.i.i.i30 = getelementptr inbounds %class.Parser, ptr %21, i64 0, i32 1, i32 1
  %23 = load ptr, ptr %__end_.i.i.i30, align 8
  %cmp.i.i.i31 = icmp eq ptr %22, %23
  br i1 %cmp.i.i.i31, label %_Z18Parser_tryContinueP8Parselet5Token.exit, label %_ZN6Parser10topContextEv.exit.i

_ZN6Parser10topContextEv.exit.i:                  ; preds = %if.then
  %add.ptr.i.i.i = getelementptr inbounds %struct.Context, ptr %23, i64 -1
  %24 = load ptr, ptr %add.ptr.i.i.i, align 8
  %tobool.not.i = icmp eq ptr %24, null
  br i1 %tobool.not.i, label %cond.true.i, label %cond.end.i

cond.true.i:                                      ; preds = %_ZN6Parser10topContextEv.exit.i
  tail call void @__assert_rtn(ptr noundef nonnull @__func__._Z18Parser_tryContinueP8Parselet5Token, ptr noundef nonnull @.str, i32 noundef 300, ptr noundef nonnull @.str.1) #32
  unreachable

cond.end.i:                                       ; preds = %_ZN6Parser10topContextEv.exit.i
  %P5.i = getelementptr %struct.Context, ptr %23, i64 -1, i32 1
  %25 = load ptr, ptr %P5.i, align 8
  musttail call void %24(ptr noundef %25, ptr noundef nonnull %agg.tmp.i29)
  ret void

_Z18Parser_tryContinueP8Parselet5Token.exit:      ; preds = %if.then
  call void @llvm.lifetime.end.p0(i64 40, ptr nonnull %agg.tmp.i29)
  ret void

if.end:                                           ; preds = %_ZN6Parser13topPrecedenceEv.exit
  call void @_ZN6Parser19pushContextAndShiftE10Precedence(ptr noundef nonnull align 8 dereferenceable(144) %17, i8 noundef zeroext %call6)
  %26 = load ptr, ptr @TheParser, align 8
  %27 = load ptr, ptr %trivia1.i, align 8
  %__end_.i.i32 = getelementptr inbounds %class.Parser, ptr %0, i64 0, i32 4, i32 0, i32 1
  %28 = load ptr, ptr %__end_.i.i32, align 8
  %coerce.val.pi.i23.i = ptrtoint ptr %26 to i64
  %call21.i.i = call i64 @_ZNSt3__16__moveIP5TokenNS_20back_insert_iteratorINS_6vectorINS_7variantIJNS_10unique_ptrI4NodeNS_14default_deleteIS7_EEEES1_EEENS_9allocatorISB_EEEEEEEET0_T_SH_SG_(ptr noundef %27, ptr noundef %28, i64 %coerce.val.pi.i23.i)
  %29 = load ptr, ptr %trivia1.i, align 8
  store ptr %29, ptr %__end_.i.i32, align 8
  %vtable13 = load ptr, ptr %15, align 8
  %vfn14 = getelementptr inbounds ptr, ptr %vtable13, i64 2
  %30 = load ptr, ptr %vfn14, align 8
  %call15 = call noundef ptr %30(ptr noundef nonnull align 8 dereferenceable(8) %15)
  %vtable16 = load ptr, ptr %15, align 8
  %vbase.offset.ptr = getelementptr i8, ptr %vtable16, i64 -32
  %vbase.offset = load i64, ptr %vbase.offset.ptr, align 8
  %add.ptr = getelementptr inbounds i8, ptr %15, i64 %vbase.offset
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(40) %agg.tmp17, ptr noundef nonnull align 8 dereferenceable(40) %token, i64 40, i1 false)
  musttail call void %call15(ptr noundef nonnull %add.ptr, ptr noundef nonnull %agg.tmp17)
  ret void
}
```

## Comments

### Comment 1 - nikic

`%agg.tmp17` is an alloca, and tail calls are forbidden from reading or writing allocas. As such, the transform is correct from a DSE perspective.

Can't tell you who is originally at fault here -- possibly it's clang for allowing you to place the attribute in a case where a tail call cannot be generated. The input C++ code might be helpful.

---

### Comment 2 - bostick

Thank you for that info. With that, here is a smaller example that shows how easy it is to accidentally have a tail call that passes an alloca.

With this source:
```cpp
#include <cstdint>

struct Token {
    uint8_t p0;
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t p4;
    uint8_t p5;
    uint8_t p6;
    uint8_t p7;
    uint8_t p8;
    uint8_t p9;
    uint8_t p10;
    uint8_t p11;
    uint8_t p12;
    uint8_t p13;
    uint8_t p14;
    uint8_t p15;
    uint8_t T;
};

Token foo(Token Tok);
Token bar(Token Tok);

Token foo(Token a) {
	int r = 37;
	a.T = a.T + r;
	[[clang::musttail]]
	return bar(a);
}

Token bar(Token a) {
	int r = 42;
	Token b;
	b.T = a.T + r;
	return b;
}
```

then we see the problem that I originally gave:
```llvm
...
%agg.tmp = alloca %struct.Token, align 1
...
musttail call void @_Z3bar5Token(ptr sret(%struct.Token) align 1 %agg.result, ptr noundef %agg.tmp)
ret void
...
```

But!

If we remove `uint8_t p15;` from the struct, then we get:
```llvm
%2 = load [2 x i64], ptr %agg.tmp, align 1
%call = musttail call [2 x i64] @_Z3bar5Token([2 x i64] %2)
ret [2 x i64] %call
```

So when the struct goes from 16 bytes to 17 bytes, then there is a silent miscompilation.

---

### Comment 3 - nikic

@bostick That example looks fine to me, because the argument is `byval` in that case (that is the one exception where an alloca *can* be passed to a musttail function).

---

### Comment 4 - bostick

@nikic, sorry I'm being a bit dense, but I do not see `byval` anywhere in the example. Could you say more?

---

### Comment 5 - nikic

@bostick I'm looking at this example: https://c.godbolt.org/z/Gh4sEMjvM The call looks like this:
```llvm
musttail call void @_Z3bar5Token(ptr sret(%struct.Token) align 1 %agg.result, ptr noundef byval(%struct.Token) align 8 %agg.tmp)
```
The argument referencing the alloca has a byval attribute there, so that would be fine. Do I understand correctly that the byval attribute is not present with your version of clang?

---

### Comment 6 - bostick

Correct, I am not seeing `byval`. I am on an M1 Mac.

This is similar to what I see:
https://c.godbolt.org/z/jP8f5q1oj

```llvm
...
%4 = alloca %struct.Token, align 1
...
musttail call void @_Z3bar5Token(ptr sret(%struct.Token) align 1 %0, ptr noundef %4)
```


---

### Comment 7 - nikic

I see, so this is due to an ABI difference between x86 and aarch64. Possibly clang needs to reject the musttail attribute in this case (by-value passing where the passing happens by-pointer but without byval), though this would be pretty fragile, as the ABI is target-dependent.

Not sure who knows about this stuff, maybe @haberman @efriedma-quic @fhahn.

---

### Comment 8 - bostick

Whatever the ultimate resolution is, are there platform-agnostic guidelines for using `musttail`?

---

### Comment 9 - haberman

> Whatever the ultimate resolution is, are there platform-agnostic guidelines for using musttail?

Clang performs several checks to make sure caller and callee match (same calling convention, same number of arguments, etc), and these are intended to make musttail reasonably platform-agnostic. At a minimum, they are intended to make sure Clang generates valid LLVM IR that follows LLVM's rules for `musttail`.

There has been some pushback in the other direction, suggesting that `musttail` should be more permissive, and that we should allow it even in cases where we know this would make the code more platform-dependent: https://github.com/llvm/llvm-project/issues/54964  I disagree with this, but wanted to mention that this tension exists between "make it as platform-independent as possible" and "allow it wherever it would work."

It looks like the Clang diagnostics for `musttail` are failing to flag code that passes a pointer to stack memory, whether explicitly (in cases like https://godbolt.org/z/MzjMKeM73), or implicitly (as in this bug, where platform ABI conventions turned pass-by-value into pass-by-pointer).

Currently we are relying on [JumpDiagnostics.cpp](https://clang.llvm.org/doxygen/JumpDiagnostics_8cpp_source.html) to check the currently open scopes for any variables that require cleanup. However trivially destructible objects won't trigger this check. It seems like we need a separate check for whether any pointers to the stack are escaping when we perform our `musttail` call.

---

### Comment 10 - efriedma-quic

There isn't any fundamental reason we can't emit a musttail call here on aarch64.  We have memory we can use to store the value that was passed indirectly: the corresponding argument in the caller.  (In the testcase from https://github.com/llvm/llvm-project/issues/56435#issuecomment-1178972007, we have the pointer to "a".)  clang just needs to explicitly memcpy the argument to that address, and pass it to the call.  (So this is a clang bug, not an LLVM optimization bug.)

It gets more complicated if you're trying to pass something with a non-trivial copy constructor, since we can't just emit a memcpy, but that's a separate issue.

---

### Comment 11 - efriedma-quic

> It looks like the Clang diagnostics for musttail are failing to flag code that passes a pointer to stack memory, whether explicitly (in cases like https://godbolt.org/z/MzjMKeM73),

The explicit case is impossible to diagnose in general, like any other use-after-free.  But we should probably enhance `-Wreturn-stack-address` to detect obvious cases.

---

### Comment 12 - foonathan

I've found a related miscompilation: https://godbolt.org/z/efeG7qsjE

A warning there would be really helpful, or at the very least an update to the docs - they don't make it obvious that [[clang::musttail]] alters the semantics in situations like these.

---

### Comment 13 - efriedma-quic

Opened https://github.com/llvm/llvm-project/issues/56676 for the related cases that aren't miscompiles.

---

### Comment 14 - apinski-quic

I think I found a similar miscompiling for aarch64:
```
typedef struct Foo {
   int o[16];
}Foo;
int moo(int, Foo);
int goo(int, Foo x)
{
  [[clang::musttail]]
  return moo(1, x);
}
```

This should just reuse the x argument space/location as the goo does not need to own x any more.
I Noticed this while improving GCC tail call to use the argument provided space (https://gcc.gnu.org/PR42909).
After my patch to GCC, GCC gets the above correct (and not rejects the musttail for not working).

---

