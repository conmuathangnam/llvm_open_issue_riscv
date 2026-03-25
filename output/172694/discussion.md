# [PowerPC] Wrong code at O1

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/172694
**Status:** Open
**Labels:** backend:PowerPC, miscompilation

## Body

Testcase: 
```c
void printf(char *, ...);
typedef long uint64_t;
int safe_lshift_func_int16_t_s_u_right, g_2322 = 2, g_3554;
static int g_939 = 2;
uint64_t g_1577;
short func_2___trans_tmp_4;
void func_2(signed char p_3, int p_4, uint64_t p_5)
{
    g_3554 = (p_3 && p_5) | p_4;
    for (;;)
    {
        uint64_t l_3564;
        __asm goto("" : : : : BS_LABEL_3);
        p_4 = p_3;
        if (p_5) break;
        func_2___trans_tmp_4 = p_3 < 0 ?: safe_lshift_func_int16_t_s_u_right;
    BS_LABEL_3:
    }
    g_1577 = p_5 && p_4;
}
int main()
{
    func_2(g_2322, 6, g_939);
    g_939 = 0;
    int crc = g_1577;
    printf("checksum = %X\n", crc);
}
```

Command:
```
> clang --target=powerpc64le-unknown-linux-musl --sysroot=/opt/ppc64/powerpc64le-unknown-linux-musl/sysroot --gcc-toolchain=/opt/ppc64 -O1 -static a.c
> qemu-ppc64le a.out
checksum = 0

> clang --target=powerpc64le-unknown-linux-musl --sysroot=/opt/ppc64/powerpc64le-unknown-linux-musl/sysroot --gcc-toolchain=/opt/ppc64 -O0 -static a.c
> qemu-ppc64le a.out
checksum = 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Hongyu Chen (XChy)

<details>
Testcase: 
```c
void printf(char *, ...);
typedef long uint64_t;
int safe_lshift_func_int16_t_s_u_right, g_2322 = 2, g_3554;
static int g_939 = 2;
uint64_t g_1577;
short func_2___trans_tmp_4;
void func_2(signed char p_3, int p_4, uint64_t p_5)
{
    g_3554 = (p_3 &amp;&amp; p_5) | p_4;
    for (;;)
    {
        uint64_t l_3564;
        __asm goto("" : : : : BS_LABEL_3);
        p_4 = p_3;
        if (p_5) break;
        func_2___trans_tmp_4 = p_3 &lt; 0 ?: safe_lshift_func_int16_t_s_u_right;
    BS_LABEL_3:
    }
    g_1577 = p_5 &amp;&amp; p_4;
}
int main()
{
    func_2(g_2322, 6, g_939);
    g_939 = 0;
    int crc = g_1577;
    printf("checksum = %X\n", crc);
}
```
</details>


---

