# Clang crash on global-scope statement with ellipsis ('...')

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/182154

## Body

- crash on invalid input 
- bug found from fuzzing

## Minimized Code
```c
g(h(x)...)
```
## Full Example Code
```c
int g(int);
int h(int);
int x = 0;
g(h(x)...);  // crash
```
Note:
- Crash only found in clang, not present in clang++
- If the same statement is moved inside a function (ex. main()), or assigned to a variable, the crash does not occur.

## CompilerExplorer
https://godbolt.org/z/j7q9Pcfh1


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
- crash on invalid input 
- bug found from fuzzing

## Minimized Code
```c
g(h(x)...)
```
## Full Example Code
```c
int g(int);
int h(int);
int x = 0;
g(h(x)...);  // crash
```
Note:
- Crash only found in clang, not present in clang++
- If the same statement is moved inside a function (ex. main()), or assigned to a variable, the crash does not occur.

## CompilerExplorer
https://godbolt.org/z/j7q9Pcfh1

</details>


---

### Comment 2 - csxplorer

May I pick up this issue?

---

### Comment 3 - zwuis

> May I pick up this issue?

We assign issues after there is a PR and we check that it is headed in the right direction.

---

### Comment 4 - csxplorer

> > May I pick up this issue?
> 
> We assign issues after there is a PR and we check that it is headed in the right direction.

Okay, thanks. Since @Ayush3941 's fix seems to solve this issue, I would seek other problems for contribution.

---

