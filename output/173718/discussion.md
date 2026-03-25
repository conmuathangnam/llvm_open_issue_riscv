# [llvm][lli] lli crash when run variable arguments function as a interpret

**Author:** Fushj89
**URL:** https://github.com/llvm/llvm-project/issues/173718
**Status:** Open
**Labels:** crash, tools:lli

## Body

### Description
Run `lli` comand with the flag `-force-interpreter=true` to execute LLVM bitcode, if `lli` run `variable arguments` function in the bitcode, it will crash.

### Steps to reproduce

1、compile the c code below to bitcode
```
#include <stdarg.h>

int sum(int a, ...) {
  int total = a;
  va_list list;
  va_start(list, a);
  total += va_arg(list, int);
  total += va_arg(list, int);
  va_end(list);
  return total;
}

int main() {
    int total = sum(1, 2, 3);
    return total;
}
```

2、use the command below to execute bitcode which produced by `step 1`
`
lli -force-interpreter=true  xxx.bc
`

3 crashed

Crashing thread backtrace:
```
Stack dump:
0.	Program arguments: /Users/user/llvm-project/build/bin/lli -force-interpreter=true main.bc
 #0 0x0000000100d3edb0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/user/llvm-project/build/bin/lli+0x100b16db0)
 #1 0x0000000100d3ccd8 llvm::sys::RunSignalHandlers() (/Users/user/llvm-project/build/bin/lli+0x100b14cd8)
 #2 0x0000000100d3f804 SignalHandler(int, __siginfo*, void*) (/Users/user/llvm-project/build/bin/lli+0x100b17804)
 #3 0x000000018c1196a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #4 0x00000001007ce900 llvm::Interpreter::visitVAArgInst(llvm::VAArgInst&) (/Users/user/llvm-project/build/bin/lli+0x1005a6900)
 #5 0x00000001007cfa84 llvm::Interpreter::run() (/Users/user/llvm-project/build/bin/lli+0x1005a7a84)
 #6 0x00000001007d2dfc llvm::Interpreter::runFunction(llvm::Function*, llvm::ArrayRef<llvm::GenericValue>) (/Users/user/llvm-project/build/bin/lli+0x1005aadfc)
 #7 0x00000001007b84e4 llvm::ExecutionEngine::runFunctionAsMain(llvm::Function*, std::__1::vector<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>, std::__1::allocator<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>> const&, char const* const*) (/Users/user/llvm-project/build/bin/lli+0x1005904e4)
 #8 0x000000010022e0d8 main (/Users/user/llvm-project/build/bin/lli+0x1000060d8)
 #9 0x000000018bd3eb98
[1]    4895 segmentation fault  /Users/user/llvm-project/build/bin/lli -force-interpreter=true main.bc
```

### environment
Mac mini  Apple M2 



## Comments

### Comment 1 - Fushj89

find same issue #21571

---

