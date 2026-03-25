# clang-3.1 breaks a piece of code that clang-3.0 compiles correctly

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/14980
**Status:** Closed
**Labels:** bugzilla, regression, miscompilation, llvm
**Closed Date:** 2026-02-11T12:12:04Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [14608](https://llvm.org/bz14608) |
| Version | 3.1 |
| OS | MacOS X |
| Reporter | LLVM Bugzilla Contributor |

## Extended Description 
Compiling GAP (www.gap-system.org) version 4.5.6. with clang-3.1 at optimisation level 1 or higher breaks. In GAP the symptom is:
```
 ┌───────┐   GAP, Version 4.dev of today (free software, GPL)
 │  GAP  │   http://www.gap-system.org
 └───────┘   Architecture: x86_64-apple-darwin12.2.1-clang-mp-3.1-default32
 Libs used:  gmp
 Loading the library and packages ...
Components: trans 1.0, prim 2.1, small* 1.0, id* 1.0
 Packages:   AClib 1.2, Alnuth 3.0.0, AutPGrp 1.5, CRISP 1.3.5, Cryst 4.1.10, 
             CrystCat 1.1.6, CTblLib 1.2.1, FactInt 1.5.3, FGA 1.1.1, GAPDoc 1.5.1, 
             IRREDSOL 1.2.1, LAGUNA 3.6.1, Polenta 1.3.1, Polycyclic 2.10.1, 
             RadiRoot 2.6, ResClasses 3.2.0, Sophus 1.23, TomLib 1.2.2
 Try '?help' for help. See also  '?copyright' and  '?authors'
gap> v := [Z(3)];
[ Z(3) ]
gap> ConvertToVectorRep(v);
3
gap> v;
[ 0*Z(3) ] 
```

The last line should be `[ Z(3) ]` as it is in clang-3.0 (or any known version of gcc). 

I have traced the bug, I think to the compilation of lines 478-482 of `src/vec8bit.c`. I can't understand the compiled code, but in execution, that section corrupts memory at adresses a few dozen bytes away from the ones it should be writing to. The source lines are:
```cpp
  for (i = 0; i < q; i++)
    for (j = 0; j < e; j++)
      for (k = 0; k < 256; k++)
  SETELT_FIELDINFO_8BIT(info)[(i*e + j)*256 + k] = (UInt1)
    ((k/pows[j+1])*pows[j+1] + i*pows[j] + (k % pows[j]));
```

`i`,`j`, `k` and `e` are 32 but unsigned integers, pows an array of the same. SETELT... is a macro (which should be side-effect free) which returns a pointer to an `unsigned char`.

## Comments

### Comment 1 - llvmbot

[Corresponding assembly file](https://user-images.githubusercontent.com/60944935/143746537-fc7aaf22-5dcd-45a7-8f75-36d3ddb29d22.gz)

---

### Comment 2 - llvmbot

[C code to be translated into an assembly file](https://user-images.githubusercontent.com/60944935/143746538-d3877f31-3c04-49a6-8ef2-d7f6b404edfb.gz)
Here is the smallest piece of C code exhibiting the bug with which I could come up.

The problem appears in the following line:
```cpp
((unsigned char*)(*info) + 5)[i*e + j] = 0;
```
The assembly file generated from the code actually contains instructions to do
```cpp
((unsigned char*)(*info))[i*(e+5) + j] = 0;
```
for some reason.

As I am not too familiar with the C standard, I'm not sure whether this is an actual bug in the optimisation or whether the code violates the standard in some way I'm missing. I would be grateful for any comment regarding this matter.

The bug persists in the current SVN version (clang version 3.3 (trunk 176181) (llvm/trunk 176177)).

---

### Comment 3 - llvmbot

[Preprocessed testcase](https://user-images.githubusercontent.com/60944935/143746539-8b65a820-532d-4296-b1b2-a3a6e4be8ae8.gz)
The attachment is the unedited output of
```console
$ clang -E src/vec8bit.c
```
in the release version of GAP 4.5.6 (http://www.gap-system.org/).

The bug is probably in the function `MakeFieldInfo8Bit`.

---

### Comment 4 - llvmbot

Same bug occurs in 3.2 with `-O1`

---

### Comment 5 - llvmbot

Does it work correctly with 3.2 (release candidates for this are available).

---

### Comment 6 - nikic

gap 4.5.6 does not build with current clang anymore and the issue doesn't reproduce with the latest version. I can only assume that this has been fixed in the meantime.

---

