# clang incorrectly expands trigraphs when pasting tokens

**Author:** DutChen18
**URL:** https://github.com/llvm/llvm-project/issues/158360

## Body

This program prints `|`, but it should print `??!`.

```c
#include <stdio.h>

#define PASTE(X, Y) X ## Y

int main(void) {
	printf("%ls\n", PASTE(L, "??\
!"));
}
```

After pasting the tokens `L` and `"??!"`, clang incorrectly expands the trigraph inside of the second token.

https://godbolt.org/z/s7esnTdjz

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (DutChen18)

<details>
This program prints `|`, but it should print `??!`.

```c
#include &lt;stdio.h&gt;

#define PASTE(X, Y) X ## Y

int main(void) {
	printf("%ls\n", PASTE(L, "??\
!"));
}
```

After pasting the tokens `L` and `"??!"`, clang incorrectly expands the trigraph inside of the second token.

https://godbolt.org/z/s7esnTdjz
</details>


---

### Comment 2 - zygoloid

Confimed, phase 1 should not be rerun after concatenation. A couple of options; I think either should work:
1) Instead of concatenating the (cleaned) token spellings when forming the scratch buffer, concatenate the original spellings. (Put `L"??\<newline>!"` in the scratch buffer in this case.) We'd still rerun phase 1 on the result, but the outcome should be the same, since a splice can never form a new `\<newline>` (a token can't start with a newline) or `??<trigraph char>` (a token can't end `??` or start `?<trigraph char>`) sequence.
2) (Somehow) track that the scratch buffer has already had phase 1 applied, and make `getSpelling` directly copy out the spelling instead of reattempting cleaning.

---

### Comment 3 - shafik

CC @cor3ntin @tahonermann 

---

### Comment 4 - zygoloid

Option 1 is probably the most straightforward, and adds the least complexity outside of the token pasting logic -- everything else can treat queries of the "original source text" the same and will always see pre-phase-1 text. But it's a bit subtle as to why it's correct.

For option 2, I suppose we could check `SourceManager::isWrittenInScratchSpace()` at the right moment(s), and assume all scratch-space text has already had translation phase 1 applied. (Though those `SourceManager` functions that compare the buffer name feel really sketchy to me...)

I'm definitely leaning towards option 1 being the way to go here.

---

### Comment 5 - zygoloid

A variant of option 1 that might be better: continue to concatenate the cleaned (post-phase-1) token spellings, but (if trigraphs are enabled) scan the result for trigraphs and escape each one by inserting an escaped newline within the trigraph sequence.

---

