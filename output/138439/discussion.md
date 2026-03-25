# [clangd] Crash when `char[]` is initialized with both `#embed` and literal values

**Author:** 398utubzyt
**URL:** https://github.com/llvm/llvm-project/issues/138439

## Body

```c
char message[] = {
#embed "message.txt"
, 0
};
```

The code above will crash when hovering over `message`. It functions as normal without the `, 0` bit.

```
LLVM ERROR: Not implemented
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled during AST worker action: Hover
  Filename: main.cpp
  Directory: d:\source\cpp\hover_crash\src\main
  Command Line: D:\Tools\Clang\20.1.0\bin\clang -resource-dir=D:\Tools\Clang\20.1.0\lib\clang\20 -- d:\source\cpp\hover_crash\src\main\main.cpp
  Version: 84
```
The logs don't seem to give a backtrace.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: 398utubzyt (398utubzyt)

<details>
```c
char message[] = {
#embed "message.txt"
, 0
};
```

The code above will crash when hovering over `message`. It functions as normal without the `, 0` bit.

```
LLVM ERROR: Not implemented
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled during AST worker action: Hover
  Filename: main.cpp
  Directory: d:\source\cpp\hover_crash\src\main
  Command Line: D:\Tools\Clang\20.1.0\bin\clang -resource-dir=D:\Tools\Clang\20.1.0\lib\clang\20 -- d:\source\cpp\hover_crash\src\main\main.cpp
  Version: 84
```
The logs don't seem to give a backtrace.
</details>


---

### Comment 2 - cor3ntin

@398utubzyt hovering?  Is that a clangd bug?

---

### Comment 3 - tbaederr

This is probably fixed by https://github.com/llvm/llvm-project/pull/135957

---

### Comment 4 - 398utubzyt

> [@398utubzyt](https://github.com/398utubzyt) hovering? Is that a clangd bug?

Indeed it's a clangd bug. It's in the title.

> This is probably fixed by [#135957](https://github.com/llvm/llvm-project/pull/135957)

The crash still occurs on 20.1.4 (latest official release as of writing), and it seems like [#135957](https://github.com/llvm/llvm-project/pull/135957) was merged prior to its release? I can confirm that `#embed` on its own behaves correctly, but the crash is caused by the inclusion of the following extra data: `, 0`.

---

### Comment 5 - tbaederr

@Fznamznon Did you ever backport that fix to the 20 branch?

---

### Comment 6 - Fznamznon

> [@Fznamznon](https://github.com/Fznamznon) Did you ever backport that fix to the 20 branch?

No, I did not. Should I?

---

### Comment 7 - tbaederr

It affects people so I think backporting it makes sense

---

### Comment 8 - Fznamznon

Sorry for the delay, one of the tests is crashing when the patch is applied to clang 20.x for a reason that I don't know. Is that still worth pursuing?

---

### Comment 9 - cor3ntin

@Fznamznon Probably not https://discourse.llvm.org/t/llvm-20-x-release-schedule-update/86688 @AaronBallman 

---

