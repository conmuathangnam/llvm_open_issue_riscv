# [LTO] ld assertion failed with -O3 -flto: Assertion `BB->getParent() == F && "Block not part of specified function"' failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/175516
**Status:** Open
**Labels:** crash, LTO, inline-asm

## Body

Clang crashed when compiled with `-O3 -flto`
Compiler Explorer: https://godbolt.org/z/vrYW5cK1r

```
$ cat small.c
void a(int b) {
  __asm__ goto("" ::"r"(&&c) : : c);
c:
  if (b > 0)
    __asm__("");
  __asm__ goto("" : : : : c);
}
void main() { a(1); }
```

```
$ clang -O3 -flto small.c -o a.out
ld: /root/llvm-project/llvm/lib/IR/Constants.cpp:1933: static llvm::BlockAddress* llvm::BlockAddress::get(llvm::Function*, llvm::BasicBlock*): Assertion `BB->getParent() == F && "Block not part of specified function"' failed.
clang: error: unable to execute command: Aborted (core dumped)
clang: error: linker command failed due to signal (use -v to see invocation)
Compiler returned: 254
```

