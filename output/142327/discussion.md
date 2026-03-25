# Behaviour of quotation marks in C++-style when running clang cpp with `-traditional-cpp`

**Author:** myzhang1029
**URL:** https://github.com/llvm/llvm-project/issues/142327

## Body

Using the `-traditional-cpp` flag (or invoking as `cpp` directly), clang seems to be mis-parsing quotation marks inside C++-style comments.

Test code:
```c
// Test "double
// Try 'single
```
Command:
```
$CC -traditional-cpp -E test.c | cat
```
(piping through `cat` to separate `stderr` from `stdout`)

Output from `clang` (Homebrew clang version 20.1.6)
(omitting line number directives)
```
test.c:1:9: warning: missing terminating '"' character [-Winvalid-pp-token]
    1 | // Test "double
      |         ^
test.c:2:8: warning: missing terminating ' character [-Winvalid-pp-token]
    2 | // Try 'single
      |        ^
2 warnings generated.

// Test "double
// Try 'single
```

The `invalid-pp-token` warning seems to be bogus.

Output from GCC (clean output)
```
// Test "double
// Try 'single
```

Honestly, since this seems to be a small issue, I am personally fine with this becoming a wont fix or a `FIXME` inside `test/Preprocessor/traditional-cpp.c`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Zhang Maiyun (myzhang1029)

<details>
Using the `-traditional-cpp` flag (or invoking as `cpp` directly), clang seems to be mis-parsing quotation marks inside C++-style comments.

Test code:
```c
// Test "double
// Try 'single
```
Command:
```
$CC -traditional-cpp -E test.c | cat
```
(piping through `cat` to separate `stderr` from `stdout`)

Output from `clang` (Homebrew clang version 20.1.6)
(omitting line number directives)
```
test.c:1:9: warning: missing terminating '"' character [-Winvalid-pp-token]
    1 | // Test "double
      |         ^
test.c:2:8: warning: missing terminating ' character [-Winvalid-pp-token]
    2 | // Try 'single
      |        ^
2 warnings generated.

// Test "double
// Try 'single
```

The `invalid-pp-token` warning seems to be bogus.

Output from GCC (clean output)
```
// Test "double
// Try 'single
```

Honestly, since this seems to be a small issue, I am personally fine with this becoming a wont fix or a `FIXME` inside `test/Preprocessor/traditional-cpp.c`.
</details>


---

### Comment 2 - AaronBallman

From https://gcc.gnu.org/onlinedocs/cpp/Traditional-lexical-analysis.html:
> Generally speaking, in traditional mode an opening quote need not have a matching closing quote. In particular, a macro may be defined with replacement text that contains an unmatched quote. Of course, if you attempt to compile preprocessed output containing an unmatched quote you will get a syntax error. 


---

### Comment 3 - myzhang1029

If I understood correctly then, this is not really a bug, because C++-style comments are invisible to the lexer of the traditional cpp, so to the cpp, the test code I gave actually has unbalanced quotes.

---

### Comment 4 - AaronBallman

I read the GCC documentation as saying that quotes do not need to be balanced, regardless of whether in a comment or otherwise. However, GCC still diagnoses unmatched quotes in macro expansion lists, same as Clang, and in this case, Clang isn't diagnosing the use in the comment: https://godbolt.org/z/xxdxz6xjh

---

### Comment 5 - AaronBallman

Actually, I can only reproduce the original issue in C mode (pulling the confirmed label for the moment): https://godbolt.org/z/vYnz1Kv7T

---

### Comment 6 - myzhang1029

Same behaviour locally. No warning is emitted if I call `clang++`.

---

