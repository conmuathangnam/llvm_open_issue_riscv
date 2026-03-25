# SLP Vectorizer issue with memcmp?

**Author:** lenary
**URL:** https://github.com/llvm/llvm-project/issues/60391
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2024-12-05T17:23:18Z

## Body

The following (reduced) C file gives shows some very odd behaviour, which we believe is caused by an issue in the SLP Vectorizer:

```c
extern __attribute__((__nothrow__)) int printf(const char *__restrict fmt, ...)
    __attribute__((__nonnull__(1)));
extern __attribute__((__nothrow__)) int memcmp(const void *s1, const void *s2,
                                               unsigned long n)
    __attribute__((__nonnull__(1, 2)));

static void initialize_arg(void *obj, unsigned long len) {
  unsigned char *start = obj;
  for (unsigned long i = 0; i < len; ++i) {
    start[i] = i;
  }
}

static void print_arg(const char *prefix, void *arg, unsigned long len) {
  char *start = arg;
  unsigned long i;

  printf("%12s: ", prefix);

  for (i = 0; i < len; ++i)
    printf("0x%02x ", start[i]);

  printf("\n");
}

typedef union {
  __attribute__((aligned(8))) char ret_1[12];
} ret_union_t;

typedef struct {
  __attribute__((aligned(16))) char arg_1[17];
} arg_struct_t;

ret_union_t global_ret;
ret_union_t a_ret;
ret_union_t b_ret;

__attribute__((noinline)) static ret_union_t test(arg_struct_t arg) {
  return global_ret;
}

int main(void) {
  arg_struct_t arg;

  initialize_arg(&arg, sizeof(arg));
  initialize_arg(&global_ret, sizeof(global_ret));

  b_ret = test(arg);
  a_ret = test(arg);

  if (memcmp(&a_ret.ret_1[0], &a_ret.ret_1[0], sizeof(a_ret.ret_1[0])) ||
      memcmp(&a_ret.ret_1[1], &b_ret.ret_1[1], sizeof(a_ret.ret_1[1])) ||
      memcmp(&a_ret.ret_1[2], &b_ret.ret_1[2], sizeof(a_ret.ret_1[2])) ||
      memcmp(&a_ret.ret_1[3], &b_ret.ret_1[3], sizeof(a_ret.ret_1[3])) ||
      memcmp(&a_ret.ret_1[4], &b_ret.ret_1[4], sizeof(a_ret.ret_1[4])) ||
      memcmp(&a_ret.ret_1[5], &b_ret.ret_1[5], sizeof(a_ret.ret_1[5])) ||
      memcmp(&a_ret.ret_1[6], &b_ret.ret_1[6], sizeof(a_ret.ret_1[6])) ||
      memcmp(&a_ret.ret_1[7], &b_ret.ret_1[7], sizeof(a_ret.ret_1[7])) ||
      memcmp(&a_ret.ret_1[8], &b_ret.ret_1[8], sizeof(a_ret.ret_1[8])) ||
      memcmp(&a_ret.ret_1[9], &b_ret.ret_1[9], sizeof(a_ret.ret_1[9])) ||
      memcmp(&a_ret.ret_1[10], &b_ret.ret_1[10], sizeof(a_ret.ret_1[10])) ||
      memcmp(&a_ret.ret_1[11], &b_ret.ret_1[11], sizeof(a_ret.ret_1[11]))) {
    printf("Mismatch between a_ret and b_ret\n");
    print_arg("global_ret", &global_ret, sizeof(global_ret));
    print_arg("a_ret", &a_ret, sizeof(a_ret));
    print_arg("b_ret", &b_ret, sizeof(b_ret));
    printf("Test Failed\n");
    return 1;
  }

  printf("Test Passed\n");
  return 0;
}

```

When compiled with
`clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-obj" "-nobuiltininc" "-O2" "-vectorize-slp" "test_c.c" "-o" "test_c.o"` it prints

```
Mismatch between a_ret and b_ret
  global_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
       a_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
       b_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
Test Failed
```

Which, upon close inspection, is very confusing as the memcmps should only be non-zero if the inspected memory is different, but the memory, once printed, is exactly the same.

This has been reduced from a randomized internal testcase which uses both C and assembly, but the code above reproduces the same problem with pure C. So far, we know it requires:
- Big-endian AArch64
- `-vectorize-slp` must be in the `-cc1` arguments. Without it, this prints "Test Passed".
- `-O2` must be in the `-cc1` arguments. At all other optimisation levels this prints "Test Passed".
- `test` must be noinline, as marked.
- `arg_struct_t` must be more than 16 bytes, so it is passed indirectly on the stack. It apparently also needs to be aligned to 16 bytes as well. It must also be present, even though the test does not look at it for side effects.
- `ret_union_t` must be 8 byte aligned. We cannot reduce its size, because if there are fewer memcmp calls, we get "Test Passed".
- `initialize_arg` must not set everything to zero, and also must not be marked `noinline` (this last requirement is perplexing).

The file does get pass remarks from the SLP Vectorizer with `-Rpass=slp-vectorizer`:
```
test_c.c:53:72: remark: Vectorized horizontal reduction with cost -3 and with tree size 6 [-Rpass=slp-vectorizer]
      memcmp(&a_ret.ret_1[2], &b_ret.ret_1[2], sizeof(a_ret.ret_1[2])) ||
                                                                       ^
test_c.c:10:14: remark: Stores SLP vectorized with cost -15 and with tree size 2 [-Rpass=slp-vectorizer]
    start[i] = i;
```

I have also attached a LLVM IR reproducer [test_c.ll.txt](https://github.com/llvm/llvm-project/files/10538869/test_c.ll.txt), which is the same C file, compiled to LLVM IR using `-emit-llvm -disable-llvm-optzns`, which shows the same issues. It can be compiled (after renaming to `test_c.ll`) with `clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-obj" "-nobuiltininc" "-O2" "test_c.ll" "-o" "test_c.o" "-vectorize-slp"` to get the equivalent object file.

We haven't reported this nearly as quickly as we would have liked to, because it has been very difficult to reduce into something useful for a bug report, but it shows on `main` just before the clang-16 branch point (that's when the IR file is from), it still shows on `main` this evening, and it is showing in our downstream release branch from September 2022.

We first saw the bug in our downstream compiler, but it reproduces with upstream directly, and we don't have modifications to the SLP vectorizer in our downstream codebase. 

## Comments

### Comment 1 - lenary

Two suspicious things that were remarked upon:
- `start[i] = i` in `initialize_arg` is assigning an `unsigned long` into a `unsigned char` array. If I change the body to `start[i] = (unsigned char)(i & 0x0f);`, the problem still shows, and the SLP vectoriser remarks are unchanged.
- Unions on big-endian are potentially an issue. If I change the `ret_union_t` (which has one field) into a `struct` (which should therefore have identical layout), then the problem still reproduces.

---

### Comment 2 - alexey-bataev

Caused by the GVN pass, SLP just reveals the problem.
```
# clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-llvm" "-nobuiltininc" "-O2" "repro.ll" -o res_wrong.ll "-vectorize-slp" -mllvm -opt-bisect-limit=231 && clang res_wrong.ll && ./a.out
Mismatch between a_ret and b_ret
  global_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f
       a_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f
       b_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f
Test Failed
```
```
# clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-llvm" "-nobuiltininc" "-O2" "repro.ll" -o res_correct.ll "-vectorize-slp" -mllvm -opt-bisect-limit=230 && clang res_correct.ll && ./a.out
Test Passed
```

BISECT: running pass (228) SROAPass on main
BISECT: running pass (229) VectorCombinePass on main
BISECT: running pass (230) MergedLoadStoreMotionPass on main
BISECT: NOT running pass (231) GVNPass on main
BISECT: NOT running pass (232) SCCPPass on main
BISECT: NOT running pass (233) BDCEPass on main
BISECT: NOT running pass (234) InstCombinePass on main
BISECT: NOT running pass (235) JumpThreadingPass on main
BISECT: NOT running pass (236) CorrelatedValuePropagationPass on main


---

### Comment 3 - tmatheson-arm

Here is a slightly reduced reproducer, without the union:
```cpp
extern __attribute__((__nothrow__)) int printf(const char *__restrict fmt, ...)
    __attribute__((__nonnull__(1)));
extern __attribute__((__nothrow__)) int memcmp(const void *s1, const void *s2,
                                               unsigned long n)
    __attribute__((__nonnull__(1, 2)));

static void initialize_arg(void *obj, unsigned long len) {
  unsigned char *start = obj;
  for (unsigned long i = 0; i < len; ++i) {
    start[i] = (unsigned char)(i & 0x0f);
  }
}

static void print_arg(const char *prefix, void *arg, unsigned long len) {
  char *start = arg;
  unsigned long i;

  printf("%12s: ", prefix);

  for (i = 0; i < len; ++i)
    printf("0x%02x ", start[i]);

  printf("\n");
}

typedef struct {
  __attribute__((aligned(8))) char ret_1[12];
} ret_struct_t;

typedef struct {
  __attribute__((aligned(16))) char arg_1[17];
} arg_struct_t;

ret_struct_t global_ret;
ret_struct_t a_ret;
ret_struct_t b_ret;

__attribute__((noinline)) static ret_struct_t test(arg_struct_t arg) {
  return global_ret;
}

int main(void) {
  arg_struct_t arg;

  initialize_arg(&arg, sizeof(arg));
  initialize_arg(&global_ret, sizeof(global_ret));

  b_ret = test(arg);
  a_ret = test(arg);

  if (memcmp(&a_ret.ret_1[0], &a_ret.ret_1[0], sizeof(a_ret.ret_1[0])) ||
      memcmp(&a_ret.ret_1[1], &b_ret.ret_1[1], sizeof(a_ret.ret_1[1])) ||
      memcmp(&a_ret.ret_1[2], &b_ret.ret_1[2], sizeof(a_ret.ret_1[2])) ||
      memcmp(&a_ret.ret_1[3], &b_ret.ret_1[3], sizeof(a_ret.ret_1[3])) ||
      memcmp(&a_ret.ret_1[4], &b_ret.ret_1[4], sizeof(a_ret.ret_1[4])) ||
      memcmp(&a_ret.ret_1[5], &b_ret.ret_1[5], sizeof(a_ret.ret_1[5])) ||
      memcmp(&a_ret.ret_1[6], &b_ret.ret_1[6], sizeof(a_ret.ret_1[6])) ||
      memcmp(&a_ret.ret_1[7], &b_ret.ret_1[7], sizeof(a_ret.ret_1[7])) ||
      memcmp(&a_ret.ret_1[8], &b_ret.ret_1[8], sizeof(a_ret.ret_1[8])) ||
      memcmp(&a_ret.ret_1[9], &b_ret.ret_1[9], sizeof(a_ret.ret_1[9])) ||
      memcmp(&a_ret.ret_1[10], &b_ret.ret_1[10], sizeof(a_ret.ret_1[10])) ||
      memcmp(&a_ret.ret_1[11], &b_ret.ret_1[11], sizeof(a_ret.ret_1[11]))) {
    printf("Mismatch between a_ret and b_ret\n");
    print_arg("global_ret", &global_ret, sizeof(global_ret));
    print_arg("a_ret", &a_ret, sizeof(a_ret));
    print_arg("b_ret", &b_ret, sizeof(b_ret));
    printf("Test Failed\n");
    return 1;
  }

  printf("Test Passed\n");
  return 0;
}
```

---

### Comment 4 - ostannard

This was fixed by #68673, confirmed by a bisect.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sam Elliott (lenary)

<details>
The following (reduced) C file gives shows some very odd behaviour, which we believe is caused by an issue in the SLP Vectorizer:

```c
extern __attribute__((__nothrow__)) int printf(const char *__restrict fmt, ...)
    __attribute__((__nonnull__(1)));
extern __attribute__((__nothrow__)) int memcmp(const void *s1, const void *s2,
                                               unsigned long n)
    __attribute__((__nonnull__(1, 2)));

static void initialize_arg(void *obj, unsigned long len) {
  unsigned char *start = obj;
  for (unsigned long i = 0; i &lt; len; ++i) {
    start[i] = i;
  }
}

static void print_arg(const char *prefix, void *arg, unsigned long len) {
  char *start = arg;
  unsigned long i;

  printf("%12s: ", prefix);

  for (i = 0; i &lt; len; ++i)
    printf("0x%02x ", start[i]);

  printf("\n");
}

typedef union {
  __attribute__((aligned(8))) char ret_1[12];
} ret_union_t;

typedef struct {
  __attribute__((aligned(16))) char arg_1[17];
} arg_struct_t;

ret_union_t global_ret;
ret_union_t a_ret;
ret_union_t b_ret;

__attribute__((noinline)) static ret_union_t test(arg_struct_t arg) {
  return global_ret;
}

int main(void) {
  arg_struct_t arg;

  initialize_arg(&amp;arg, sizeof(arg));
  initialize_arg(&amp;global_ret, sizeof(global_ret));

  b_ret = test(arg);
  a_ret = test(arg);

  if (memcmp(&amp;a_ret.ret_1[0], &amp;a_ret.ret_1[0], sizeof(a_ret.ret_1[0])) ||
      memcmp(&amp;a_ret.ret_1[1], &amp;b_ret.ret_1[1], sizeof(a_ret.ret_1[1])) ||
      memcmp(&amp;a_ret.ret_1[2], &amp;b_ret.ret_1[2], sizeof(a_ret.ret_1[2])) ||
      memcmp(&amp;a_ret.ret_1[3], &amp;b_ret.ret_1[3], sizeof(a_ret.ret_1[3])) ||
      memcmp(&amp;a_ret.ret_1[4], &amp;b_ret.ret_1[4], sizeof(a_ret.ret_1[4])) ||
      memcmp(&amp;a_ret.ret_1[5], &amp;b_ret.ret_1[5], sizeof(a_ret.ret_1[5])) ||
      memcmp(&amp;a_ret.ret_1[6], &amp;b_ret.ret_1[6], sizeof(a_ret.ret_1[6])) ||
      memcmp(&amp;a_ret.ret_1[7], &amp;b_ret.ret_1[7], sizeof(a_ret.ret_1[7])) ||
      memcmp(&amp;a_ret.ret_1[8], &amp;b_ret.ret_1[8], sizeof(a_ret.ret_1[8])) ||
      memcmp(&amp;a_ret.ret_1[9], &amp;b_ret.ret_1[9], sizeof(a_ret.ret_1[9])) ||
      memcmp(&amp;a_ret.ret_1[10], &amp;b_ret.ret_1[10], sizeof(a_ret.ret_1[10])) ||
      memcmp(&amp;a_ret.ret_1[11], &amp;b_ret.ret_1[11], sizeof(a_ret.ret_1[11]))) {
    printf("Mismatch between a_ret and b_ret\n");
    print_arg("global_ret", &amp;global_ret, sizeof(global_ret));
    print_arg("a_ret", &amp;a_ret, sizeof(a_ret));
    print_arg("b_ret", &amp;b_ret, sizeof(b_ret));
    printf("Test Failed\n");
    return 1;
  }

  printf("Test Passed\n");
  return 0;
}

```

When compiled with
`clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-obj" "-nobuiltininc" "-O2" "-vectorize-slp" "test_c.c" "-o" "test_c.o"` it prints

```
Mismatch between a_ret and b_ret
  global_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
       a_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
       b_ret: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 
Test Failed
```

Which, upon close inspection, is very confusing as the memcmps should only be non-zero if the inspected memory is different, but the memory, once printed, is exactly the same.

This has been reduced from a randomized internal testcase which uses both C and assembly, but the code above reproduces the same problem with pure C. So far, we know it requires:
- Big-endian AArch64
- `-vectorize-slp` must be in the `-cc1` arguments. Without it, this prints "Test Passed".
- `-O2` must be in the `-cc1` arguments. At all other optimisation levels this prints "Test Passed".
- `test` must be noinline, as marked.
- `arg_struct_t` must be more than 16 bytes, so it is passed indirectly on the stack. It apparently also needs to be aligned to 16 bytes as well. It must also be present, even though the test does not look at it for side effects.
- `ret_union_t` must be 8 byte aligned. We cannot reduce its size, because if there are fewer memcmp calls, we get "Test Passed".
- `initialize_arg` must not set everything to zero, and also must not be marked `noinline` (this last requirement is perplexing).

The file does get pass remarks from the SLP Vectorizer with `-Rpass=slp-vectorizer`:
```
test_c.c:53:72: remark: Vectorized horizontal reduction with cost -3 and with tree size 6 [-Rpass=slp-vectorizer]
      memcmp(&amp;a_ret.ret_1[2], &amp;b_ret.ret_1[2], sizeof(a_ret.ret_1[2])) ||
                                                                       ^
test_c.c:10:14: remark: Stores SLP vectorized with cost -15 and with tree size 2 [-Rpass=slp-vectorizer]
    start[i] = i;
```

I have also attached a LLVM IR reproducer [test_c.ll.txt](https://github.com/llvm/llvm-project/files/10538869/test_c.ll.txt), which is the same C file, compiled to LLVM IR using `-emit-llvm -disable-llvm-optzns`, which shows the same issues. It can be compiled (after renaming to `test_c.ll`) with `clang "-cc1" "-triple" "aarch64_be-arm-none-eabi" "-emit-obj" "-nobuiltininc" "-O2" "test_c.ll" "-o" "test_c.o" "-vectorize-slp"` to get the equivalent object file.

We haven't reported this nearly as quickly as we would have liked to, because it has been very difficult to reduce into something useful for a bug report, but it shows on `main` just before the clang-16 branch point (that's when the IR file is from), it still shows on `main` this evening, and it is showing in our downstream release branch from September 2022.

We first saw the bug in our downstream compiler, but it reproduces with upstream directly, and we don't have modifications to the SLP vectorizer in our downstream codebase. 
</details>


---

