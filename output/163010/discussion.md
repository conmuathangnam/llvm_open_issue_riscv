# Issue while building O2Physics on macOS (when compiling ROOT@v6-32-06-alice10 )

**Author:** ahmadtabikh
**URL:** https://github.com/llvm/llvm-project/issues/163010

## Body

Hello,

I followed the step-by-step instructions for installing O2Physics on my Mac as described in the official O2 documentation. However, the build consistently crashes during the compilation of ROOT@v6-32-06-alice10 when running aliBuild build O2Physics.

For context, I don’t have any previous ROOT installation on my machine.
I’ve attached the log file for reference.

Thank you for your help,
Ahmad

[log.txt](https://github.com/user-attachments/files/22865734/log.txt)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch, LLVM, not Apple?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ahmadtabikh)

<details>
Hello,

I followed the step-by-step instructions for installing O2Physics on my Mac as described in the official O2 documentation. However, the build consistently crashes during the compilation of ROOT@<!-- -->v6-32-06-alice10 when running aliBuild build O2Physics.

For context, I don’t have any previous ROOT installation on my machine.
I’ve attached the log file for reference.

Thank you for your help,
Ahmad

[log.txt](https://github.com/user-attachments/files/22865734/log.txt)
</details>


---

### Comment 3 - ahmadtabikh

> Could you please try 21 or `main` branch, LLVM, not Apple?

Excuse me, I didn't understand. Could you please explain me in more detail? Thank you very much 

---

### Comment 4 - EugeneZelenko

From log file:

```
-- The C compiler identification is AppleClang 17.0.0.17000319
-- The CXX compiler identification is AppleClang 17.0.0.17000319
```

LLVM and Apple code bases could be different. You need to reproduce problem with LLVM code, or report it to Apple.

---

