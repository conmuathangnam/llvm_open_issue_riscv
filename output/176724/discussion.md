# [Clang] SourceRange on QualifiedTypeLoc doesn't include cv qualifiers

**Author:** upsj
**URL:** https://github.com/llvm/llvm-project/issues/176724

## Body

Opening the following file
```cpp
const double* a;
```
in clang-query 21.1.8 and matching the typeLoc of the declaration
```
match varDecl(hasTypeLoc(typeLoc().bind("type")))
```
returns a source range that doesn't include the `const`, i.e. it makes the typeLoc unsuitable to use for clang-tidy Transformer checks:
```
Match #1:

/Users/tobi/test.cpp:1:1: note: "root" binds here
    1 | const double* a;
      | ^~~~~~~~~~~~~~~
/Users/tobi/test.cpp:1:7: note: "type" binds here
    1 | const double* a;
      |       ^~~~~~~
1 match.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tobias Ribizel (upsj)

<details>
Opening the following file
```cpp
const double* a;
```
in clang-query 21.1.8 and matching the typeLoc of the declaration
```
match varDecl(hasTypeLoc(typeLoc().bind("type")))
```
returns a source range that doesn't include the `const`, i.e. it makes the typeLoc unsuitable to use for clang-tidy Transformer checks:
```
Match #<!-- -->1:

/Users/tobi/test.cpp:1:1: note: "root" binds here
    1 | const double* a;
      | ^~~~~~~~~~~~~~~
/Users/tobi/test.cpp:1:7: note: "type" binds here
    1 | const double* a;
      |       ^~~~~~~
1 match.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Tobias Ribizel (upsj)

<details>
Opening the following file
```cpp
const double* a;
```
in clang-query 21.1.8 and matching the typeLoc of the declaration
```
match varDecl(hasTypeLoc(typeLoc().bind("type")))
```
returns a source range that doesn't include the `const`, i.e. it makes the typeLoc unsuitable to use for clang-tidy Transformer checks:
```
Match #<!-- -->1:

/Users/tobi/test.cpp:1:1: note: "root" binds here
    1 | const double* a;
      | ^~~~~~~~~~~~~~~
/Users/tobi/test.cpp:1:7: note: "type" binds here
    1 | const double* a;
      |       ^~~~~~~
1 match.
```
</details>


---

### Comment 3 - upsj

Looks like this might be related to this FIXME: The const token is missing from the typeLoc even for non-pointer parameters, independent of whether it is west-const or east-const.
https://github.com/llvm/llvm-project/blob/c16340e49e201d0f413bc2949fe8a1bf2e405f99/clang/lib/AST/TypeLoc.cpp#L210-L213

---

### Comment 4 - upsj

Digging deeper, this seems based on an intentional decision 16 years ago, more specifically https://github.com/llvm/llvm-project/commit/33815690af25a41f6739ff9a5cc71bde389f2363
https://github.com/llvm/llvm-project/blob/17370e0478d5b0c34a031b81e427f4f6436fb2a1/clang/include/clang/AST/TypeLoc.h#L295-L299
@rjmccall if I can bother you with a question here: Do you remember if there were any concrete reservations against adding source location information for type qualifiers, or was this mainly about implementation complexity?

---

### Comment 5 - AaronBallman

I'm not @rjmccall but one potential reason to not track that information is memory overhead and complexity -- each source location is 32-bits to track (currently, potentially will be changed to 64-bits someday) and we'd have to be able to track locations (and eventually merge into coherent ranges) for things like `const int volatile long _Atomic long unsigned`. That's a lot of extra location information to track, so the question is whether the benefits outweigh the overhead costs.

---

### Comment 6 - upsj

Thanks for the detailed explanation! If this is something that is desirable to fix somehow, a smaller step could be to only provide the source range for the full QualifiedTypeLoc instead of individual qualifiers - together with narrowing matchers on the type qualifiers, this would still support replacing types without having to rebuild the entire declaration from scratch, which is what I currently have to do if I want to replace a qualified type with a Transformer clang-tidy check.

---

### Comment 7 - rjmccall

Right, our concerns at the time were primarily about memory use and the complexity of implementation that would be required to minimize memory use. (We cannot, for example, afford to have a possibly-null SourceLocation slot for every qualifier that *might* be there; we would need to only store locations for the qualifiers that are actually present.)

I did also notice during the implementation that we have a lot of implicit qualifier adjustments that would complicate the implementation, in ways that we don't for other written type components.

---

