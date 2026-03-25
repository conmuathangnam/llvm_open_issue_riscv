# Clang is crashing

**Author:** MAKMED1337
**URL:** https://github.com/llvm/llvm-project/issues/152571
**Status:** Closed
**Labels:** clang:frontend, crash, needs-reduction
**Closed Date:** 2025-08-08T19:53:56Z

## Body

Command: `/usr/bin/clang++ --driver-mode=g++ -DLOCAL -I/home/makmed1337/CF/. -g -std=gnu++20 -fsanitize=address,undefined,float-divide-by-zero,local-bounds,nullability,implicit-conversion -g -o CMakeFiles/CF.dir/main.cpp.o -c -std=gnu++23 -DLOCAL -resource-dir=/home/makmed1337/.local/share/nvim/mason/packages/clangd/clangd_20.1.8/lib/clang/20 -- /home/makmed1337/CF/main.cpp`
Output:  [output.txt](https://github.com/user-attachments/files/21671295/output.txt)

[files.zip](https://github.com/user-attachments/files/21671258/result.zip)
main.cpp - my code, 2 other files that were required, and config.yaml for clangd (this crash was from it), and compile commands from cmake, I am not sure what else should I add here.

Clang is from arch packages, version: `20.1.8-1`


## Comments

### Comment 1 - frederick-vs-ja

This is possibly fixed by #143423. Could you use Clang 21 to check whether the bug is fixed?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eduard Tykhoniuk (MAKMED1337)

<details>
Command: `/usr/bin/clang++ --driver-mode=g++ -DLOCAL -I/home/makmed1337/CF/. -g -std=gnu++20 -fsanitize=address,undefined,float-divide-by-zero,local-bounds,nullability,implicit-conversion -g -o CMakeFiles/CF.dir/main.cpp.o -c -std=gnu++23 -DLOCAL -resource-dir=/home/makmed1337/.local/share/nvim/mason/packages/clangd/clangd_20.1.8/lib/clang/20 -- /home/makmed1337/CF/main.cpp`
Output:  [output.txt](https://github.com/user-attachments/files/21671295/output.txt)

[files.zip](https://github.com/user-attachments/files/21671258/result.zip)
main.cpp - my code, 2 other files that were required, and config.yaml for clangd (this crash was from it), and compile commands from cmake, I am not sure what else should I add here.

Clang is from arch packages, version: `20.1.8-1`

</details>


---

### Comment 3 - MAKMED1337

> This is possibly fixed by [#143423](https://github.com/llvm/llvm-project/pull/143423). Could you use Clang 21 to check whether the bug is fixed?

This branch explicitly (`gh pr checkout 143423`) does not compile for me
```
/home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:272:16: error: static assertion failed: The '-' operator is only defined for random access iterators.
  272 |         BaseT::IsRandomAccess,
      |                ^~~~~~~~~~~~~~
/home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:272:16: note: ‘llvm::iterator_facade_base<llvm::mapped_iterator<llvm::StringMapIterator<llvm::jitlink::Symbol*>, llvm::jitlink::LinkGraph::GetExternalSymbolMapEntryValue>, std::forward_iterator_tag, llvm::jitlink::Symbol*, long int, llvm::jitlink::Symbol**, llvm::jitlink::Symbol*>::IsRandomAccess’ evaluates to false
/home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:274:14: error: no match for ‘operator-’ (operand types are ‘const llvm::StringMapIterator<llvm::jitlink::Symbol*>’ and ‘const llvm::StringMapIterator<llvm::jitlink::Symbol*>’)
  274 |     return I - RHS.I;
      |            ~~^~~~~~~
```

---

### Comment 4 - frederick-vs-ja

> > This is possibly fixed by [#143423](https://github.com/llvm/llvm-project/pull/143423). Could you use Clang 21 to check whether the bug is fixed?
> 
> This branch explicitly (`gh pr checkout 143423`) does not compile for me
> 
> ```
> /home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:272:16: error: static assertion failed: The '-' operator is only defined for random access iterators.
>   272 |         BaseT::IsRandomAccess,
>       |                ^~~~~~~~~~~~~~
> /home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:272:16: note: ‘llvm::iterator_facade_base<llvm::mapped_iterator<llvm::StringMapIterator<llvm::jitlink::Symbol*>, llvm::jitlink::LinkGraph::GetExternalSymbolMapEntryValue>, std::forward_iterator_tag, llvm::jitlink::Symbol*, long int, llvm::jitlink::Symbol**, llvm::jitlink::Symbol*>::IsRandomAccess’ evaluates to false
> /home/makmed1337/llvm-project/llvm/include/llvm/ADT/iterator.h:274:14: error: no match for ‘operator-’ (operand types are ‘const llvm::StringMapIterator<llvm::jitlink::Symbol*>’ and ‘const llvm::StringMapIterator<llvm::jitlink::Symbol*>’)
>   274 |     return I - RHS.I;
>       |            ~~^~~~~~~
> ```

This issue looks like #139072.

---

### Comment 5 - MAKMED1337

> This is possibly fixed by [#143423](https://github.com/llvm/llvm-project/pull/143423).

This fixes the crash

---

