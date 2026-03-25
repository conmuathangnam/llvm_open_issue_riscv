# clang hangs when compiling a code sample

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/41493
**Status:** Closed
**Labels:** bugzilla, llvm:codegen, confirmed, hang
**Closed Date:** 2025-11-28T14:56:31Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [42148](https://llvm.org/bz42148) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@zygoloid |

## Extended Description 
My command line is clang++ -O3, its version is 9.0.0

The code line is as follows:

typedef unsigned __int128 u128_t;
typedef __float128 f128_t;

typedef union
{
 f128_t f;
 u128_t i;
} union128_t;

int test(volatile f128_t *start_X_ptr, volatile f128_t *start_Y_ptr)
{
 for (unsigned iter_count = 1; iter_count <= 2; iter_count++)
 {
 union128_t read_valX = { .f = *start_X_ptr };
 union128_t read_valY = { .f = *start_Y_ptr };

 u128_t expected_val = ((iter_count - 1) * read_valY.i);
 if (read_valX.i != expected_val)
 {
 return -1;
 }

 union128_t write_valX = { .i = (iter_count * (read_valX.i + 1)) };
 union128_t write_valY = { .i = (read_valX.i + 1) };

 *start_X_ptr = write_valX.f;
 *start_Y_ptr = write_valY.f;
 }
 return 0;
}

clang hangs when it compiles the code. It consumes a lot of CPU time.

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Build is getting stuck in llvm::DAGTypeLegalizer::run().

---

### Comment 2 - llvmbot

I tried -c and -o1. It does not hang, given the two parameters.

---

### Comment 3 - JOE1994

With clang 21.1.0 , compiling the given program with `-O3` compiles very fast:
* https://godbolt.org/z/7q51hK4cW

Closing ticket as resolved.

---

