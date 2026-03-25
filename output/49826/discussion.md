# optimizer malloc

**Author:** 68b1592b-506f-4f32-b7cc-f7acec066692
**URL:** https://github.com/llvm/llvm-project/issues/49826
**Status:** Open
**Labels:** c, bugzilla, miscompilation, clang:to-be-triaged

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [50482](https://llvm.org/bz50482) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor,@fhahn,@ojeda,@Kojoley,@RalfJung,@zygoloid,@wjristow |

## Extended Description 
```cpp
#include <stdlib.h>
int is_any_memory_available(int n){
    char *x = malloc(n);
    int y = (x != NULL);
    free(x);
    return y;
}
```
Works with `-O0`
With `-O3` is optmized to 
```asm
is_any_memory_available:                # @is_any_memory_available
        mov     eax, 1
        ret
```

https://godbolt.org/z/4fr7Wz7n7

## Comments

### Comment 1 - RalfJung

Related example:
```cpp
#include <stdlib.h>
int test() {
  char *x = malloc(-1);
  char *y = malloc(2);
  int ret = (x != NULL) && (y != NULL);
  free(x); free(y);
  return ret;
}
```
Arguably, this function *must* return "0" since it is impossible that both of these 'malloc's succeed. However, clang optimizes this function to return "1". (gcc will not do that.)

---

### Comment 2 - 68b1592b-506f-4f32-b7cc-f7acec066692

I saw the C++ bug report but (a) it was withdrawn, and (b) the response cited a C++ rule for why it was allowed. This is a C bug report.

---

### Comment 3 - 68b1592b-506f-4f32-b7cc-f7acec066692

So are you saying that C++ rules apply in this case? The cited provision is not in C11.

---

### Comment 4 - Kojoley



*** This bug has been marked as a duplicate of bug llvm/llvm-project#28790  ***

---

