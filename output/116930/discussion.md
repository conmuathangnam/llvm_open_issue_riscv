# return in __finally in loop hits breakpoint

**Author:** jaykrell
**URL:** https://github.com/llvm/llvm-project/issues/116930
**Status:** Open
**Labels:** clang:codegen, miscompilation, extension:microsoft

## Body

__except appears to have the same problem.

```
int main()
{
    while (1)
    {
        __try { }
        __finally { return 40; }
    }

    return 0;
}

/*
With Visual C++ this returns 40.

With clang-cl version 16.0.0.0 it hits a breakpoint in the generated code.
0:000> uf 1!main
1!main:
00007ff7`2d1d7100 4883ec28        sub     rsp,28h
00007ff7`2d1d7104 c744242400000000 mov     dword ptr [rsp+24h],0

1!main+0xc:
00007ff7`2d1d710c 4889e2          mov     rdx,rsp
00007ff7`2d1d710f 31c9            xor     ecx,ecx
00007ff7`2d1d7111 e80a000000      call    1!main+0x20 (00007ff7`2d1d7120)
00007ff7`2d1d7116 e9f1ffffff      jmp     1!main+0xc (00007ff7`2d1d710c)
0:000> u (00007ff7`2d1d7120)
1!main+0x20:
00007ff7`2d1d7120 4883ec10        sub     rsp,10h
00007ff7`2d1d7124 4889542408      mov     qword ptr [rsp+8],rdx
00007ff7`2d1d7129 884c2407        mov     byte ptr [rsp+7],cl
00007ff7`2d1d712d cc              int     3
*/
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Jay Krell (jaykrell)

<details>
__except appears to have the same problem.

```
int main()
{
    while (1)
    {
        __try { }
        __finally { return 40; }
    }

    return 0;
}

/*
With Visual C++ this returns 40.

With clang-cl version 16.0.0.0 it hits a breakpoint in the generated code.
0:000&gt; uf 1!main
1!main:
00007ff7`2d1d7100 4883ec28        sub     rsp,28h
00007ff7`2d1d7104 c744242400000000 mov     dword ptr [rsp+24h],0

1!main+0xc:
00007ff7`2d1d710c 4889e2          mov     rdx,rsp
00007ff7`2d1d710f 31c9            xor     ecx,ecx
00007ff7`2d1d7111 e80a000000      call    1!main+0x20 (00007ff7`2d1d7120)
00007ff7`2d1d7116 e9f1ffffff      jmp     1!main+0xc (00007ff7`2d1d710c)
0:000&gt; u (00007ff7`2d1d7120)
1!main+0x20:
00007ff7`2d1d7120 4883ec10        sub     rsp,10h
00007ff7`2d1d7124 4889542408      mov     qword ptr [rsp+8],rdx
00007ff7`2d1d7129 884c2407        mov     byte ptr [rsp+7],cl
00007ff7`2d1d712d cc              int     3
*/
```
</details>


---

