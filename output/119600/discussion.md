# Very large template parameter packs break sizeof...() and may crash clang

**Author:** OgnianM
**URL:** https://github.com/llvm/llvm-project/issues/119600

## Body

Incorrect results from sizeof...(): https://godbolt.org/z/1ox1o44Eh

Compiler crash: https://godbolt.org/z/Wb8rWMs7n

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ognyan Mirev (OgnianM)

<details>
Incorrect results from sizeof...(): https://godbolt.org/z/1ox1o44Eh

Compiler crash: https://godbolt.org/z/Wb8rWMs7n
</details>


---

### Comment 2 - zyn0217

I think this is due to a limitation in `SubstNonTypeTemplateParmExpr` where `PackIndex` is only 16 bits long. A clue is that `100000 % 65536 == 34464`, which exactly matches the number in the bogus diagnostic.



---

### Comment 3 - zyn0217

Related: https://github.com/llvm/llvm-project/issues/39203

---

### Comment 4 - frederick-vs-ja

> Incorrect results from sizeof...(): https://godbolt.org/z/1ox1o44Eh

This used to be correctly handed in Clang 15 ([Godbolt link](https://godbolt.org/z/vsvnKovnz)). Some limitation added in Clang 16 caused miscompilation.

The latter case didn't seem supported before. It seems intended that Clang doesn't support function with more than 65535 parameters, although we need to reject such use with proper error message instead of crash.

---

