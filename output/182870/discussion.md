# Crash in clang::TextDiagnostic::emitSnippetAndCaret for invalid expression

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/182870

## Body

- crash on invalid, found from fuzzing
- crashes current assertion build
- error does not occur when using the -w flag
- the last character is non-ASCII

Minimized Code
```
Hex                                      ASCII
5b38 3c3c 382b 3228 2e00                 [8<<8+2(..
```
Minimzed C File:
[minimized_crash_file.c](https://github.com/user-attachments/files/25493414/minimized_crash_file.c)

Full C File:
[full_crash_input.c](https://github.com/user-attachments/files/25493157/full_crash_input.c)

CompilerExplorer:
https://godbolt.org/z/j7cddEM4o


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
- crash on invalid, found from fuzzing
- crashes current assertion build
- error does not occur when using the -w flag
- the last character is non-ASCII

Minimized Code
```
Hex                                      ASCII
5b38 3c3c 382b 3228 2e00                 [8&lt;&lt;8+2(..
```
Minimzed C File:
[minimized_crash_file.c](https://github.com/user-attachments/files/25493414/minimized_crash_file.c)

Full C File:
[full_crash_input.c](https://github.com/user-attachments/files/25493157/full_crash_input.c)

CompilerExplorer:
https://godbolt.org/z/j7cddEM4o

</details>


---

