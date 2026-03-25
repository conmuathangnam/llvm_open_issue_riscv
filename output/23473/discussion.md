# `std::future` regression

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/23473
**Status:** Closed
**Labels:** clang, c++11, bugzilla, regression, miscompilation, obsolete, libstdc++
**Closed Date:** 2025-04-26T08:00:38Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [23099](https://llvm.org/bz23099) |
| Version | 3.6 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@DimitryAndric |

## Extended Description 
Since using clang 3.6.0, the following code crashes with
```
Illegal instruction (core dumped)
```
Whereas clang 3.5.0  returns the correct output value `1`
```cpp
//file future.cpp
#include <thread>
#include <future>
#include <iostream>

int bla()
{
  return 1;
}

int main()
{
  std::future<int> f = std::async(std::launch::async, bla);
  f.wait();
  std::cout<<f.get()<<std::endl;
  return 0;
}
//end file
```
Build it with
```
clang++ --std=c++11 -lpthread future.cpp
```

## Comments

### Comment 1 - llvmbot

Works fine for me with newer version of libstdc++ :)

Thanks for your help!
Dirk

---

### Comment 2 - llvmbot

I will try to rebuild clang with a newer libstdc++ to get rid of the problem :)

---

### Comment 3 - llvmbot

Consequently, the greatest glibc version symbol is
`GLIBCXX_3.4.17`

---

### Comment 4 - llvmbot

My fault, its gcc 4.7.3.

---

### Comment 5 - llvmbot

Hi

I'am using Ubuntu 12.04 with my own gcc 4.8.3 compiler.
The libstdc++.so.6 used by clang and the repro code is build with this gcc compiler.
The greatest version symbol i can find in there is
`GLIBCXX_3.4.19`.

gdb backtraces reads as follows:
```
Program received signal SIGILL, Illegal instruction.
bool std::__future_base::_State_base::_S_check<std::__future_base::_State_base>(std::shared_ptr<std::__future_base::_State_base> const&) ()
    at /sfw/gcc/4.7.3-static-gmp-mpfr-mpc-cloog/lib64/gcc/x86_64-unknown-linux-gnu/4.7.3/../../../../include/c++/4.7.3/future:462
462	          if (!static_cast<bool>(__p))
(gdb) bt
#0  bool std::__future_base::_State_base::_S_check<std::__future_base::_State_base>(std::shared_ptr<std::__future_base::_State_base> const&) ()
    at /sfw/gcc/4.7.3-static-gmp-mpfr-mpc-cloog/lib64/gcc/x86_64-unknown-linux-gnu/4.7.3/../../../../include/c++/4.7.3/future:462
#1  0x0000000000402b1d in std::__basic_future<int>::__basic_future(std::shared_ptr<std::__future_base::_State_base> const&) ()
    at /sfw/gcc/4.7.3-static-gmp-mpfr-mpc-cloog/lib64/gcc/x86_64-unknown-linux-gnu/4.7.3/../../../../include/c++/4.7.3/future:613
#2  0x0000000000402ae8 in std::future<int>::future(std::shared_ptr<std::__future_base::_State_base> const&) ()
    at /sfw/gcc/4.7.3-static-gmp-mpfr-mpc-cloog/lib64/gcc/x86_64-unknown-linux-gnu/4.7.3/../../../../include/c++/4.7.3/future:654
#3  0x0000000000401f48 in _ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFT_DpT0_EE4typeEESt6launchOS4_DpOS5_ ()
    at /sfw/gcc/4.7.3-static-gmp-mpfr-mpc-cloog/lib64/gcc/x86_64-unknown-linux-gnu/4.7.3/../../../../include/c++/4.7.3/future:1499
#4  0x0000000000401b1b in main () at future.cpp:12
```
line 12 is the line with `std::async`.

If i can give you any further information, i'm pleased to do so.

Thanks in advance
Dirk

---

### Comment 6 - DimitryAndric

This works fine on FreeBSD 11 (with clang 3.6.0), but we're using a relatively recent libc++ there.

Which C++ library are you using in case of the crash?  Is that libstdc++ on your particular Linux instance?  And which version?

Also, it would be interesting to see a debug backtrace.  Is it crashing on a `ud2` instruction, by any chance?

---

### Comment 7 - frederick-vs-ja

This doesn't seem reproducible any more. I think we should close this issue.

---

