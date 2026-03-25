# Assertion `TL.getFullDataSize() == CurrTL.getFullDataSize()' failed.

**Author:** thevinster
**URL:** https://github.com/llvm/llvm-project/issues/150688

## Body

I'm running into crashes on the following assertion: 
```
clang::TypeSourceInfo* GetTypeSourceInfoForDeclarator({anonymous}::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*): Assertion `TL.getFullDataSize() == CurrTL.getFullDataSize()' failed.`
```

This was seen when using clang-19, but it repros on upstream main as well. This can be reproduced using an assertions enabled clang on [godbolt](https://godbolt.org/z/bacEnhzbY). The reproducer is a malformed program, but it was reduced from valid code in a large internal project. Using a noasserts compiler doesn't solve the issue either. Rather, it just crashes on something else downstream, so it could very well be a source code issue. However, whether it's an issue with the compiler or with the program, clang shouldn't be crashing, and should throw a valid error message in such situations. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vincent Lee (thevinster)

<details>
I'm running into crashes on the following assertion: 
```
clang::TypeSourceInfo* GetTypeSourceInfoForDeclarator({anonymous}::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*): Assertion `TL.getFullDataSize() == CurrTL.getFullDataSize()' failed.`
```

This was seen when using clang-19, but it repros on upstream main as well. This can be reproduced using an assertions enabled clang on [godbolt](https://godbolt.org/z/bacEnhzbY). The reproducer is a malformed program, but it was reduced from valid code in a large internal project. Using a noasserts compiler doesn't solve the issue either. Rather, it just crashes on something else downstream, so it could very well be a source code issue. However, whether it's an issue with the compiler or with the program, clang shouldn't be crashing, and should throw a valid error message in such situations. 
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Vincent Lee (thevinster)

<details>
I'm running into crashes on the following assertion: 
```
clang::TypeSourceInfo* GetTypeSourceInfoForDeclarator({anonymous}::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*): Assertion `TL.getFullDataSize() == CurrTL.getFullDataSize()' failed.`
```

This was seen when using clang-19, but it repros on upstream main as well. This can be reproduced using an assertions enabled clang on [godbolt](https://godbolt.org/z/bacEnhzbY). The reproducer is a malformed program, but it was reduced from valid code in a large internal project. Using a noasserts compiler doesn't solve the issue either. Rather, it just crashes on something else downstream, so it could very well be a source code issue. However, whether it's an issue with the compiler or with the program, clang shouldn't be crashing, and should throw a valid error message in such situations. 
</details>


---

### Comment 3 - shafik

The crash in the reproducer goes back to clang-3.7: https://godbolt.org/z/8hYG3WxTx but given the original code was valid maybe it is not representative.

---

### Comment 4 - thevinster

I'm trying to get a valid working program with the same crash, but not succeeding so far. If I run into some other instance of this, I'll try to reduce that one and see if I get different results. 

---

### Comment 5 - shafik

I was just chatting w/ someone folks and you might have been luck with a reduction by adding compiles successfully w/ gcc or another compiler as a condition. You could also check for errors and warnings and reject those as valid too. 

CC @Endilll 

---

