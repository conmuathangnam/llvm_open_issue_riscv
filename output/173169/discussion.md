# Clang hangs on valid code when compiling with profiled data using PGO at -O3

**Author:** helloqirun
**URL:** https://github.com/llvm/llvm-project/issues/173169

## Body

Reproducible at the latest commit 611a271e8df12d67f567dc9b71.
```
$ clang-trunk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 611a271e8df12d67f567dc9b71bd64d57b0629bb)
Target: x86_64-unknown-linux-gnu
```

The (attached) testcases are large, and I have done a bisection which pointed to the commit 5af27f8c208b4ba13f339801c9188cfc19cebdc0

[aa-105567.c](https://github.com/user-attachments/files/24274923/aa-105567.c)
[prof.txt](https://github.com/user-attachments/files/24274924/prof.txt)


The profiling data is produced by
`$ llvm-profdata merge prof.txt -o prof.profdata`


It compiles at -Os
`clang-trunk -Os  -fprofile-instr-use=prof.profdata aa-105567.c`

However, it hangs at -O3.
`$ clang-trunk -O3  -fprofile-instr-use=prof.profdata aa-105567.c`

