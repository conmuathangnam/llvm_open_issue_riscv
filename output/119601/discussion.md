# Lambda converted to function pointer converted to boolean in the requires clause of a lambda causes a segfault

**Author:** Bu11etmagnet
**URL:** https://github.com/llvm/llvm-project/issues/119601

## Body

<https://godbolt.org/z/jadvT8oec>
```
int main()
{
    return
    []
    (auto&&...)
    requires (not not []{}) // segfault
    //requires true // this is fine
    { return 42; }
    ();
}
```
segfaults with clang 19.1.0 and 18.1.0, but not 17.1.0

## Comments

### Comment 1 - Bu11etmagnet

[crash-minim.txt](https://github.com/user-attachments/files/18100310/crash-minim.txt)
Stack trace from GDB

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ráduly, Csaba (Bu11etmagnet)

<details>
&lt;https://godbolt.org/z/jadvT8oec&gt;
```
int main()
{
    return
    []
    (auto&amp;&amp;...)
    requires (not not []{}) // segfault
    //requires true // this is fine
    { return 42; }
    ();
}
```
segfaults with clang 19.1.0 and 18.1.0, but not 17.1.0
</details>


---

### Comment 3 - shafik

@zygoloid git bisect points to 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 as the responsible commit

---

