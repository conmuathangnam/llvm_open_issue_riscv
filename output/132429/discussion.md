# Clang crash when using stdc++ features in OpenMP offloaded regions

**Author:** KaruroChori
**URL:** https://github.com/llvm/llvm-project/issues/132429
**Status:** Open
**Labels:** openmp, llvm:codegen, llvm:crash, libstdc++, backend:NVPTX

## Body

To be clear, this code is not expected to compile, but the appropriate error message would be for the linker to complain of missing symbols, as it rightfully does when stripping a bit the content of `gen_cpp`.

```
fatal error: error in backend: Copy one register into another with a different width
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project 1b455df780ed1d578b63f969c636fe78b2eb6014)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /archive/shared/apps/cross-clang/install3/usr/local/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-sm_86-e545ff.cpp
clang++: note: diagnostic msg: /tmp/main-016287.cpp
clang++: note: diagnostic msg: /tmp/main-sm_86-e545ff.sh
clang++: note: diagnostic msg: 
```

The code responsible is
```
    #pragma omp target 
    {
        SDF_Sphere.gen_cpp(std::cout);
    }
```
Which internally expects to operate on an ostream. 
The error message is not stable, in the sense that small variations in code, location and context led to a different one yesterday. I no longer have records of that, but it was something about the compiler being unable to relax a type, or something of that sort.

[2025-03-20-bug-report.zip](https://github.com/user-attachments/files/19394305/2025-03-20-bug-report.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-openmp

Author: None (KaruroChori)

<details>
To be clear, this code is not expected to compile, but the appropriate error message would be for the linker to complain of missing symbols, as it rightfully does when stripping a bit the content of `gen_cpp`.

```
fatal error: error in backend: Copy one register into another with a different width
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project 1b455df780ed1d578b63f969c636fe78b2eb6014)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /archive/shared/apps/cross-clang/install3/usr/local/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-sm_86-e545ff.cpp
clang++: note: diagnostic msg: /tmp/main-016287.cpp
clang++: note: diagnostic msg: /tmp/main-sm_86-e545ff.sh
clang++: note: diagnostic msg: 
```

The code responsible is
```
    #pragma omp target 
    {
        SDF_Sphere.gen_cpp(std::cout);
    }
```
Which internally expects to operate on an ostream. 
The error message is not stable, in the sense that small variations in code, location and context led to a different one yesterday. I no longer have records of that, but it was something about the compiler being unable to relax a type, or something of that sort.

[2025-03-20-bug-report.zip](https://github.com/user-attachments/files/19394305/2025-03-20-bug-report.zip)
</details>


---

### Comment 2 - jhuber6

Fun fact, this works if you build all the C++ libraries for the GPU. It's a pretty terrible idea to actually use though.
```c++
#include <iostream>

#pragma omp declare target to(std::cerr)

int main(int argc, char **argv) {

#pragma omp target
  std::cerr << "Hello world!" << std::endl;
}

```
```
$ clang++ cout.cpp -fopenmp --offload-arch=gfx1030 -O1 -stdlib=libc++ -Xoffload-linker -lc++ -Xoffload-linker -lc++abi
$ ./a.out
Hello World!
```

 On NVPTX this is completely broken because PTX doesn't accept self-referential initializers and there's not much we can do about that. For this case, it's probably some backend bug due to the function. If you had the IR we could potentially reduce it, but that'd take awhile. You can get the IR with `-save-temps` and try to reproduce the issue with `llc`.

---

### Comment 3 - KaruroChori

That must be the most cursed thing I have seen over this week. Now I have to take my 5700xt out of storage just to run things which were never meant to be on there. Thanks for ruining my weekend :).

> You can get the IR with -save-temps and try to reproduce the issue with llc.

I will try to do that.

---

### Comment 4 - jhuber6

> That must be the most cursed thing I have seen over this week. Now I have to take my 5700xt out of storage just to run things which were never meant to be on there.

My specialty, see https://www.phoronix.com/news/DOOM-ROCm-LLVM-Port and https://www.youtube.com/watch?v=4TxGWis1mws for some shameless self-promotion. 



---

### Comment 5 - KaruroChori

Thanks for your shameless self-promotion then, I just checked the talk, that was impressively cool.
I mean, I know well that general computing is possible on modern GPUs, but knowing and seeing it done in such a blatant way is just not the same :D.

---

