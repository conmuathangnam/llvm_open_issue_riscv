# slp-vectorizer miscompile

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/161140
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-09-29T18:14:27Z

## Body

llvm commit: ab5bba580c5
Reproduce with:
```
opt -passes=slp-vectorizer bbi-110973_2.ll -S -o - -slp-threshold=-100
```
which results in
```
define i1 @foo(i1 %v) {
entry:
  %0 = insertelement <2 x i1> poison, i1 %v, i32 0
  %1 = shufflevector <2 x i1> %0, <2 x i1> poison, <2 x i32> zeroinitializer
  %2 = mul <2 x i1> <i1 false, i1 true>, %1
  %3 = extractelement <2 x i1> %2, i32 0
  %4 = extractelement <2 x i1> %2, i32 1
  %sub = sub i1 %3, %4
  ret i1 %sub
}
```
If you e.g. calculate the result for input value %v == 1 you see that the results before/after slp-vectorizer are different.

See also
[alive2](https://alive2.llvm.org/ce/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:llvm,selection:(endColumn:14,endLineNumber:7,positionColumn:14,positionLineNumber:7,selectionStartColumn:14,selectionStartLineNumber:7,startColumn:14,startLineNumber:7),source:'define+i1+@src(i1+%25v)+%7B%0Aentry:%0A++%25not+%3D+xor+i1+%25v,+1%0A++%25not1+%3D+xor+i1+%25not,+1%0A++%25mul+%3D+mul+i1+%25v,+1%0A++%25sub+%3D+sub+i1+%25not1,+%25mul%0A++ret+i1+%25sub%0A%7D%0A%0Adefine+i1+@tgt(i1+%25v)+%7B%0Aentry:%0A++%250+%3D+insertelement+%3C2+x+i1%3E+poison,+i1+%25v,+i32+0%0A++%251+%3D+shufflevector+%3C2+x+i1%3E+%250,+%3C2+x+i1%3E+poison,+%3C2+x+i32%3E+zeroinitializer%0A++%252+%3D+mul+%3C2+x+i1%3E+%3Ci1+false,+i1+true%3E,+%251%0A++%253+%3D+extractelement+%3C2+x+i1%3E+%252,+i32+0%0A++%254+%3D+extractelement+%3C2+x+i1%3E+%252,+i32+1%0A++%25sub+%3D+sub+i1+%253,+%254%0A++ret+i1+%25sub%0A%7D%0A'),l:'5',n:'0',o:'LLVM+IR+source+%231',t:'0')),k:43.15667262669051,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:alive,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'1',trim:'1'),fontScale:14,j:1,lang:llvm,libs:!(),options:'--disable-undef-input',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'alive-tv+(Editor+%231,+Compiler+%231)+LLVM+IR',t:'0')),k:56.8433273733095,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)

[bbi-110973_2.ll.gz](https://github.com/user-attachments/files/22591794/bbi-110973_2.ll.gz)

## Comments

### Comment 1 - mikaelholmen

This starts happening with d41e517748e2
```
[SLP] Make getSameOpcode support interchangeable instructions. (#135797)

We use the term "interchangeable instructions" to refer to different
operators that have the same meaning (e.g., `add x, 0` is equivalent to
`mul x, 1`).
Non-constant values are not supported, as they may incur high costs with
little benefit.

---------

Co-authored-by: Alexey Bataev <a.bataev@gmx.com>
```
and it still happens at latest trunk (ab5bba580c).

Ping @HanKuanChen and @alexey-bataev 

---

### Comment 2 - mikaelholmen

I've verified that [1f82553](https://github.com/llvm/llvm-project/commit/1f82553e385f449efee92da3dca43facb4a1ee66) solves the miscompile we saw.
Thanks!

---

