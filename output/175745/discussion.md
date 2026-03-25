# [AMDGPU] Wrong code at -Os

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/175745
**Status:** Open
**Labels:** backend:AMDGPU, miscompilation

## Body

### Testcase
A reduced OpenCL testcase:
```opencl
// test.cl
struct S1
{
    char f0;
    int f1;
    int f2;
    int f4;
    uint f5;
    ulong f6;
    int f8
};
struct S4
{
    char g_11[5][9];
    int g_17;
    long f2;
    uint g_24;
    ushort g_53;
    struct S1 g_57[8][7];
    short g_66;
    volatile struct S1 **g_118;
    struct S1 g_127
};
void func_29(int *, struct S4 *);
struct S1 func_58() {}
void func_14(struct S4 *p_238)
{
BS_LABEL_4:
    int volatile BS_COND_1 = 76;
    switch (BS_COND_1)
    {
        case 5: goto BS_LABEL_4;
        case -7: goto BS_LABEL_2;
        case 6: goto BS_LABEL_3;
    }
    int l_22[9];
    uint *l_23 = &p_238->g_24;
BS_LABEL_3:
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) l_22[j] = 0;
    *l_23 ^= 0 <= l_22[2];
    func_29(&p_238->g_17, p_238);
BS_LABEL_2:
}
void func_29(int *p_32, struct S4 *p_238)
{
BS_LABEL_3:
    for (; p_238->g_53; p_238 += 1)
    {
        int f1;
        for (; p_238->f2;)
        {
            struct S1 l_142;
            **p_238->g_118 = func_58();
            if (p_238->g_17) continue;
            p_238->g_127.f4 -= 1;
            for (; p_238->g_127.f4;)
                for (; p_238->g_127.f8; p_238 -= 1)
                {
                    int *l_149 = &p_238->g_57[7][0].f2;
                    for (; p_238->g_66; p_238 += 1)
                        if (p_238->g_11[p_238->g_53][p_238->g_53])
                            *l_149 |= 0 == p_32;
                    int volatile BS_COND_5;
                    if (BS_COND_5) goto BS_LABEL_3;
                }
        }
        for (; 8;) return;
    }
}
__kernel void entry(__global ulong *result, __global ulong *bs_result)
{
    struct S4 c_239 = {};
    func_14(&c_239);
    bs_result[0] = result[0] = c_239.g_24;
}
```

### Reproduce
GPU: AMD Radeon RX 9070
Reproduce method: Run the kernel `entry` on a single thread, and print `result[0]` and `bs_result[0]` at the host.
Host launcher `cl_launcher.c`: [cl_launcher.c](https://github.com/user-attachments/files/24588166/cl_launcher.c)

Result at Os:
```
0,
0,
```

Result at other optimization levels:
```
0x1,
0x1,
```

Full reproducible command:
```bash
# Compile the device code
clang -O1 -x cl -target amdgcn-amd-amdhsa -mcpu=gfx1201 test.cl -c -w -o ./unlinked.o
$ROCM_HOME/llvm/bin/ld.lld --no-undefined -shared -plugin-opt=mcpu=gfx1201 --enable-new-dtags ./unlinked.o -o correct.out
clang -Os -x cl -target amdgcn-amd-amdhsa -mcpu=gfx1201 test.cl -c -w -o ./unlinked.o
$ROCM_HOME/llvm/bin/ld.lld --no-undefined -shared -plugin-opt=mcpu=gfx1201 --enable-new-dtags ./unlinked.o -o wrong.out

# Compile the host launcher
gcc -lOpenCL cl_launcher.c -o cl_launcher

# Run the kernel entry on a single thread
./cl_launcher -f correct.out -p <platform id> -d <device id> -l 1 -g 1  ---backsmith ---binary
./cl_launcher -f wrong.out -p <platform id> -d <device id> -l 1 -g 1  ---backsmith ---binary
```

### UB-free check
We treat the device code as C code and run it on X86-64 with sanitizer: https://godbolt.org/z/d9341nxME
It passed the UBSan, MemorySan, and ASan.

## Comments

### Comment 1 - arsenm

BS_COND_5 is used uninitialized?

---

### Comment 2 - XChy

> BS_COND_5 is used uninitialized?

The use of BS_COND_5 is in the dead code: `p_238->f2`is zero, and therefore the loop `for (; p_238->f2;)` in `func_29` is dead. The UB inside the loop is never reached.

And if I don't miss something, `func_29` is just a dead loop.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Hongyu Chen (XChy)

<details>
### Testcase
A reduced OpenCL testcase:
```opencl
// test.cl
struct S1
{
    char f0;
    int f1;
    int f2;
    int f4;
    uint f5;
    ulong f6;
    int f8
};
struct S4
{
    char g_11[5][9];
    int g_17;
    long f2;
    uint g_24;
    ushort g_53;
    struct S1 g_57[8][7];
    short g_66;
    volatile struct S1 **g_118;
    struct S1 g_127
};
void func_29(int *, struct S4 *);
struct S1 func_58() {}
void func_14(struct S4 *p_238)
{
BS_LABEL_4:
    int volatile BS_COND_1 = 76;
    switch (BS_COND_1)
    {
        case 5: goto BS_LABEL_4;
        case -7: goto BS_LABEL_2;
        case 6: goto BS_LABEL_3;
    }
    int l_22[9];
    uint *l_23 = &amp;p_238-&gt;g_24;
BS_LABEL_3:
    for (int i = 0; i &lt; 3; i++)
        for (int j = 0; j &lt; 3; j++) l_22[j] = 0;
    *l_23 ^= 0 &lt;= l_22[2];
    func_29(&amp;p_238-&gt;g_17, p_238);
BS_LABEL_2:
}
void func_29(int *p_32, struct S4 *p_238)
{
BS_LABEL_3:
    for (; p_238-&gt;g_53; p_238 += 1)
    {
        int f1;
        for (; p_238-&gt;f2;)
        {
            struct S1 l_142;
            **p_238-&gt;g_118 = func_58();
            if (p_238-&gt;g_17) continue;
            p_238-&gt;g_127.f4 -= 1;
            for (; p_238-&gt;g_127.f4;)
                for (; p_238-&gt;g_127.f8; p_238 -= 1)
                {
                    int *l_149 = &amp;p_238-&gt;g_57[7][0].f2;
                    for (; p_238-&gt;g_66; p_238 += 1)
                        if (p_238-&gt;g_11[p_238-&gt;g_53][p_238-&gt;g_53])
                            *l_149 |= 0 == p_32;
                    int volatile BS_COND_5;
                    if (BS_COND_5) goto BS_LABEL_3;
                }
        }
        for (; 8;) return;
    }
}
__kernel void entry(__global ulong *result, __global ulong *bs_result)
{
    struct S4 c_239 = {};
    func_14(&amp;c_239);
    bs_result[0] = result[0] = c_239.g_24;
}
```

### Reproduce
GPU: AMD Radeon RX 9070
Reproduce method: Run the kernel `entry` on a single thread, and print `result[0]` and `bs_result[0]` at the host.
Host launcher `cl_launcher.c`: [cl_launcher.c](https://github.com/user-attachments/files/24588166/cl_launcher.c)

Result at Os:
```
0,
0,
```

Result at other optimization levels:
```
0x1,
0x1,
```

Full reproducible command:
```bash
# Compile the device code
clang -O1 -x cl -target amdgcn-amd-amdhsa -mcpu=gfx1201 test.cl -c -w -o ./unlinked.o
$ROCM_HOME/llvm/bin/ld.lld --no-undefined -shared -plugin-opt=mcpu=gfx1201 --enable-new-dtags ./unlinked.o -o correct.out
clang -Os -x cl -target amdgcn-amd-amdhsa -mcpu=gfx1201 test.cl -c -w -o ./unlinked.o
$ROCM_HOME/llvm/bin/ld.lld --no-undefined -shared -plugin-opt=mcpu=gfx1201 --enable-new-dtags ./unlinked.o -o wrong.out

# Compile the host launcher
gcc -lOpenCL cl_launcher.c -o cl_launcher

# Run the kernel entry on a single thread
./cl_launcher -f correct.out -p &lt;platform id&gt; -d &lt;device id&gt; -l 1 -g 1  ---backsmith ---binary
./cl_launcher -f wrong.out -p &lt;platform id&gt; -d &lt;device id&gt; -l 1 -g 1  ---backsmith ---binary
```

### UB-free check
We treat the device code as C code and run it on X86-64 with sanitizer: https://godbolt.org/z/d9341nxME
It pass the UBSan, MemorySan and ASan.
</details>


---

