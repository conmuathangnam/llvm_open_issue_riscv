# [Polly] Potential Miscompilation / Wrong code generation involving assume constraints in loop bounds

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/187922
**Status:** Open
**Labels:** miscompilation, loopoptim, polly

## Body

I have probably encountered a miscompilation problem in LLVM's Polly optimization passes.
When compiled with standard -O3, the program outputs the correct checksum. However, when compiled with -O3 -mllvm -polly, it results in SIGSEGV. 

To prove that the compiler assumptions (__builtin_unreachable) are strictly valid and not causing undefined behavior, the code includes a VALIDATION macro that swaps them for standard assert(). Running with -DVALIDATION passes all assertions successfully.

Reproducer: https://godbolt.org/z/xjvxMsh3K

```c
#include <stdio.h>
#include <assert.h>

#ifndef VALIDATION
    #define ASSUME(cond) do{if (!(cond)) __builtin_unreachable();}while(0)
#else
    #define ASSUME(cond) do{assert(cond); }while(0)
#endif

#define max(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a>_b?_a:_b; })
#define min(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a<_b?_a:_b; })

#define E0 (((int)(signed char)(var_15 ? (unsigned int)min(1, max(0, (int)var_17)) : 2147483648U)) - 1)
#define EB ((signed char)var_0 - 70)
#define E1 (((int)(unsigned short)(max((long long)var_7, 1LL - var_0) ? var_3 : var_4)) - 211)
#define E2 (((int)(_Bool)(var_3 ? (int)arr_4[i_1 - 1][i_1 - 1][i_0] : (int)var_7)) - 1)

long long var_0 = 1675129639484229466LL;
unsigned int var_3 = 898901400U;
unsigned char var_4 = 213;
_Bool var_7 = 0;
int var_15 = -230299294;
unsigned short var_17 = 18302;

short arr_4[20][20][20];
signed char arr_8[20];
int arr_11[20][20][20];
unsigned long long seed = 0;

void hash(unsigned long long v) { seed ^= v + 0x9e3779b9 + (seed<<6) + (seed>>2); }

int main() {
    for(int i=0; i<20; i++) for(int j=0; j<20; j++) for(int k=0; k<20; k++) arr_4[i][j][k] = -14263;

    ASSUME((signed char)E0 == 0);
    ASSUME((signed char)EB == 20);
    ASSUME((EB - E0 + 2ULL) / 3ULL == 7ULL);

    for (signed char i_0 = E0; i_0 < EB; i_0 += 3)
        for (unsigned short i_1 = E1; i_1 < 18; i_1 += 4) {
            ASSUME(1 - E2 == 1ULL);
            for (_Bool i_2 = E2; i_2 < 1; i_2 += 1) {
                arr_8[i_2] = (signed char)arr_4[i_1][i_1][i_0];
                arr_11[i_0][i_0][i_2] = 0;
            }
        }

    for(int i=0; i<20; i++) hash(arr_8[i]);
    for(int i=0; i<20; i++) for(int j=0; j<20; j++) for(int k=0; k<20; k++) hash(arr_11[i][j][k]);

    printf("%llu\n", seed);
    return 0;
}
```

LLVM version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 64c4e529a9560b77a049fe80f635d9a29a7b515e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
```

Steps to reproduce: 

```sh 
$ clang -O0 -DVALIDATION repro.c -o repro_val
$ ./repro_val
7234749625435485745

$ clang -O3 repro.c -o repro_correct
$ ./repro_correct
7234749625435485745

$ clang -O3 -mllvm -polly repro.c -o repro_polly
$ ./repro_polly
Segmentation Fault (Core dumped)

```

Attached files: 

- [repro.c](https://github.com/user-attachments/files/26164602/repro.c)
- Preprocessed C file:  [preprocessed.c](https://github.com/user-attachments/files/26164609/preprocessed.c)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
I have probably encountered a miscompilation problem in LLVM's Polly optimization passes.
When compiled with standard -O3, the program outputs the correct checksum. However, when compiled with -O3 -mllvm -polly, it results in SIGSEGV. 

To prove that the compiler assumptions (__builtin_unreachable) are strictly valid and not causing undefined behavior, the code includes a VALIDATION macro that swaps them for standard assert(). Running with -DVALIDATION passes all assertions successfully.

Reproducer: https://godbolt.org/z/xjvxMsh3K

```c
#include &lt;stdio.h&gt;
#include &lt;assert.h&gt;

#ifndef VALIDATION
    #define ASSUME(cond) do{if (!(cond)) __builtin_unreachable();}while(0)
#else
    #define ASSUME(cond) do{assert(cond); }while(0)
#endif

#define max(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a&gt;_b?_a:_b; })
#define min(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a&lt;_b?_a:_b; })

#define E0 (((int)(signed char)(var_15 ? (unsigned int)min(1, max(0, (int)var_17)) : 2147483648U)) - 1)
#define EB ((signed char)var_0 - 70)
#define E1 (((int)(unsigned short)(max((long long)var_7, 1LL - var_0) ? var_3 : var_4)) - 211)
#define E2 (((int)(_Bool)(var_3 ? (int)arr_4[i_1 - 1][i_1 - 1][i_0] : (int)var_7)) - 1)

long long var_0 = 1675129639484229466LL;
unsigned int var_3 = 898901400U;
unsigned char var_4 = 213;
_Bool var_7 = 0;
int var_15 = -230299294;
unsigned short var_17 = 18302;

short arr_4[20][20][20];
signed char arr_8[20];
int arr_11[20][20][20];
unsigned long long seed = 0;

void hash(unsigned long long v) { seed ^= v + 0x9e3779b9 + (seed&lt;&lt;6) + (seed&gt;&gt;2); }

int main() {
    for(int i=0; i&lt;20; i++) for(int j=0; j&lt;20; j++) for(int k=0; k&lt;20; k++) arr_4[i][j][k] = -14263;

    ASSUME((signed char)E0 == 0);
    ASSUME((signed char)EB == 20);
    ASSUME((EB - E0 + 2ULL) / 3ULL == 7ULL);

    for (signed char i_0 = E0; i_0 &lt; EB; i_0 += 3)
        for (unsigned short i_1 = E1; i_1 &lt; 18; i_1 += 4) {
            ASSUME(1 - E2 == 1ULL);
            for (_Bool i_2 = E2; i_2 &lt; 1; i_2 += 1) {
                arr_8[i_2] = (signed char)arr_4[i_1][i_1][i_0];
                arr_11[i_0][i_0][i_2] = 0;
            }
        }

    for(int i=0; i&lt;20; i++) hash(arr_8[i]);
    for(int i=0; i&lt;20; i++) for(int j=0; j&lt;20; j++) for(int k=0; k&lt;20; k++) hash(arr_11[i][j][k]);

    printf("%llu\n", seed);
    return 0;
}
```

LLVM version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 64c4e529a9560b77a049fe80f635d9a29a7b515e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
```

Steps to reproduce: 

```sh 
$ clang -O0 -DVALIDATION repro.c -o repro_val
$ ./repro_val
7234749625435485745

$ clang -O3 repro.c -o repro_correct
$ ./repro_correct
7234749625435485745

$ clang -O3 -mllvm -polly repro.c -o repro_polly
$ ./repro_polly
Segmentation Fault (Core dumped)

```

Attached files: 

- [repro.c](https://github.com/user-attachments/files/26164602/repro.c)
- Preprocessed C file:  [preprocessed.c](https://github.com/user-attachments/files/26164609/preprocessed.c)
</details>


---

### Comment 2 - nalin1o1

Hi, I tried running the repro on my machine with an LLVM build that has Polly enabled, and I wasn’t able to reproduce the issue.

Could you verify whether Polly is built into your LLVM toolchain? Many Godbolt builds do not include Polly support.

If Polly is not present, flags like `-polly` or `-mllvm -polly` may not be recognized or may be ignored, which can lead to unexpected behavior. This could explain the issue you're seeing.


---

### Comment 3 - ms178

I could reproduce the issue with a Polly-enabled clang version 23.0.0git (fee5075b16a7e5b20d8fb724bf3479675d052e76) and the given reproducer.


---

