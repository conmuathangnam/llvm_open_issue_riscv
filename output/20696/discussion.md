# invalid symbol type in a shared library with `-fvisibility=hidden` when using a custom allocator in `std::basic_string`

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/20696
**Status:** Open
**Labels:** c++, clang:codegen, bugzilla, miscompilation

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [20322](https://llvm.org/bz20322) |
| Version | 3.4 |
| OS | Linux |
| Attachments | [The source files and the build script to reproduce the issue.](https://user-images.githubusercontent.com/60944935/143749748-c72db997-c851-4a76-ac5b-7f297e366612.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor |

## Extended Description 


I am trying to assign a value to std::basic_string with a custom `Allocator` across a shared library boundaries and the code generated causes a crash when compiling with `-fvisibility=hidden`.

Here are files used:
```console
% cat SecureAllocator.h
```
```cpp
#pragma once

#ifndef SECURE_ALLOCATOR_H_
#define SECURE_ALLOCATOR_H_

#include <memory>
#include <algorithm>

namespace Runtime {


template <class T>
class SecureAllocator : public std::allocator<T> {
   typedef std::allocator<T> base_type;

public:
    template<class U> struct rebind { typedef SecureAllocator<U> other; };

    SecureAllocator() throw() {}
    SecureAllocator(const SecureAllocator&) throw() : base_type() { }
    template <class U>
    SecureAllocator(const SecureAllocator<U>&) throw() : base_type() { }

    void deallocate(typename base_type::pointer p,
                    typename base_type::size_type n) {
        std::fill_n((volatile char*)p, n * sizeof(T), 0);
        base_type::deallocate(p, n);
    }
};


} // namespace Runtime

#endif // SECURE_ALLOCATOR_H_
```

```console
% cat Value.h
```
```cpp
#pragma once

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include "SecureAllocator.h"


namespace Runtime {

typedef std::basic_string<char,
                          std::char_traits<char>,
                          SecureAllocator<char> > SecretString;

class Value {
public:
   template<typename T>
   static void Get(T& out);

};

} // namespace Runtime

#endif // VALUE_H_
```
```console
% cat Value.cpp
```
```cpp
#include "Value.h"

namespace Runtime {


template<>
__attribute__ ((visibility ("default"))) void
Value::Get<SecretString>(SecretString& out)
{
   out = SecretString("something");
}


} // namespace Runtime
```
```console
% cat main.cpp
```
```cpp
#include "Value.h"

using namespace Runtime;

int main()
{
   SecretString tmp;
   Value::Get(tmp);
}
```
The file `Value.cpp` is build into the `libvalue.so` and then `main.cpp` is linked into an executable against `libvalue.so`
```console
% cat build-lite.sh
```
```sh
#!/bin/sh

SH=/bin/sh
BLD_DIR=build
CXX=${CXX:=clang++}

CXXFLAGS="-fPIC -fvisibility=hidden -pipe"
LDFLAGS=

echo "CXX = $CXX"
echo "CXXFLAGS = $CXXFLAGS"
echo "LDFLAGS = $LDFLAGS"

mkdir $BLD_DIR

# compile the files
$CXX -c -v -o $BLD_DIR/Value.o $CXXFLAGS Value.cpp
$CXX -c -v -o $BLD_DIR/main.o $CXXFLAGS main.cpp

# link the shared lib
$CXX -v -shared -o $BLD_DIR/libvalue.so $LDFLAGS $BLD_DIR/Value.o

# link the executable
$CXX -o $BLD_DIR/a.out $LDFLAGS $BLD_DIR/main.o $BLD_DIR/libvalue.so
```

Here is the output from the above script:
```console
% ./build-lite.sh
CXX = clang++
CXXFLAGS = -fPIC -fvisibility=hidden -pipe
LDFLAGS =

clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/dbc/dbc101/georgid/sysroot/bin/clang" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name Value.cpp -mrelocation-model pic -pic-level 2 -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.20.51.0.2 -v -coverage-file /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/Value.o -resource-dir /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7 -internal-isystem /usr/local/include -internal-isystem /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime -ferror-limit 19 -fmessage-length 229 -fvisibility hidden -mstackrealign -fobjc-runtime=gcc -fobjc-default-synthesize-properties -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -backend-option -vectorize-loops -o build/Value.o -x c++ Value.cpp
clang -cc1 version 3.4 based upon LLVM 3.4svn default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward
 /usr/local/include
 /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include
 /usr/include
End of search list.
clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/dbc/dbc101/georgid/sysroot/bin/clang" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.20.51.0.2 -v -coverage-file /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/main.o -resource-dir /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7 -internal-isystem /usr/local/include -internal-isystem /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime -ferror-limit 19 -fmessage-length 229 -fvisibility hidden -mstackrealign -fobjc-runtime=gcc -fobjc-default-synthesize-properties -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -backend-option -vectorize-loops -o build/main.o -x c++ main.cpp
clang -cc1 version 3.4 based upon LLVM 3.4svn default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward
 /usr/local/include
 /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include
 /usr/include
End of search list.
clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/usr/bin/ld" --eh-frame-hdr -m elf_x86_64 -shared -o build/libvalue.so /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64/crti.o /usr/lib/gcc/x86_64-redhat-linux/4.4.7/crtbeginS.o -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../.. -L/lib -L/usr/lib build/Value.o -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-redhat-linux/4.4.7/crtendS.o /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64/crtn.o
```


When I try to run the above binary I get:
```console
% ./build/a.out
*** glibc detected *** ./build/a.out: free(): invalid pointer: 0x0000000000601c40 ***
======= Backtrace: =========
/lib64/libc.so.6[0x38a1276126]
./build/a.out(_ZN9__gnu_cxx13new_allocatorIcE10deallocateEPcm+0x1d)[0x40104d]
build/libvalue.so(+0x194b)[0x7feb11d1d94b]
build/libvalue.so(+0x1884)[0x7feb11d1d884]
build/libvalue.so(+0x17da)[0x7feb11d1d7da]
build/libvalue.so(+0x1615)[0x7feb11d1d615]
build/libvalue.so(+0x149d)[0x7feb11d1d49d]
build/libvalue.so(_ZN7Runtime5Value3GetISbIcSt11char_traitsIcENS_15SecureAllocatorIcEEEEEvRT_+0x46)[0x7feb11d1d2b6]
./build/a.out[0x400b11]
/lib64/libc.so.6(__libc_start_main+0xfd)[0x38a121ecdd]
./build/a.out[0x400a29]
======= Memory map: ========
00400000-00402000 r-xp 00000000 fd:00 322571941                          /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/a.out
00601000-00602000 rw-p 00001000 fd:00 322571941                          /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/a.out
00e1e000-00e3f000 rw-p 00000000 00:00 0                                  [heap]
312a800000-312a8e8000 r-xp 00000000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312a8e8000-312aae8000 ---p 000e8000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aae8000-312aaef000 r--p 000e8000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aaef000-312aaf1000 rw-p 000ef000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aaf1000-312ab06000 rw-p 00000000 00:00 0
3728800000-3728816000 r-xp 00000000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
3728816000-3728a15000 ---p 00016000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
3728a15000-3728a16000 rw-p 00015000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
38a0a00000-38a0a20000 r-xp 00000000 08:03 524290                         /lib64/ld-2.12.so
38a0c1f000-38a0c20000 r--p 0001f000 08:03 524290                         /lib64/ld-2.12.so
38a0c20000-38a0c21000 rw-p 00020000 08:03 524290                         /lib64/ld-2.12.so
38a0c21000-38a0c22000 rw-p 00000000 00:00 0
38a1200000-38a138a000 r-xp 00000000 08:03 524295                         /lib64/libc-2.12.so
38a138a000-38a1589000 ---p 0018a000 08:03 524295                         /lib64/libc-2.12.so
38a1589000-38a158d000 r--p 00189000 08:03 524295                         /lib64/libc-2.12.so
38a158d000-38a158e000 rw-p 0018d000 08:03 524295                         /lib64/libc-2.12.so
38a158e000-38a1593000 rw-p 00000000 00:00 0
38a1e00000-38a1e83000 r-xp 00000000 08:03 524301                         /lib64/libm-2.12.so
38a1e83000-38a2082000 ---p 00083000 08:03 524301                         /lib64/libm-2.12.so
38a2082000-38a2083000 r--p 00082000 08:03 524301                         /lib64/libm-2.12.so
38a2083000-38a2084000 rw-p 00083000 08:03 524301                         /lib64/libm-2.12.so
7feb11d07000-7feb11d0d000 rw-p 00000000 00:00 0
7feb11d1b000-7feb11d1c000 rw-p 00000000 00:00 0
7feb11d1c000-7feb11d20000 r-xp 00000000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11d20000-7feb11f1f000 ---p 00004000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11f1f000-7feb11f20000 rw-p 00003000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11f20000-7feb11f21000 rw-p 00000000 00:00 0
7fff0964b000-7fff09661000 rw-p 00000000 00:00 0                          [stack]
7fff097ff000-7fff09800000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
[3]    64804 abort (core dumped)  ./build/a.out
```



In GDB the backtrace looks like this:
```
(gdb) bt
#0  0x00000038a12328e5 in raise (sig=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:64
#1  0x00000038a12340c5 in abort () at abort.c:92
#2  0x00000038a12707f7 in __libc_message (do_abort=2, fmt=0x38a1357fc0 "*** glibc detected *** %s: %s: 0x%s ***\n") at ../sysdeps/unix/sysv/linux/libc_fatal.c:198
#3  0x00000038a1276126 in malloc_printerr (action=3, str=0x38a13560af "free(): invalid pointer", ptr=<value optimized out>) at malloc.c:6311
#4  0x000000000040104d in __gnu_cxx::new_allocator<char>::deallocate(char*, unsigned long) ()
#5  0x00007ffff7dfa94b in Runtime::SecureAllocator<char>::deallocate(char*, unsigned long) () from build/libvalue.so
#6  0x00007ffff7dfa884 in std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_M_destroy(Runtime::SecureAllocator<char> const&) () from build/libvalue.so
#7  0x00007ffff7dfa7da in std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_M_dispose(Runtime::SecureAllocator<char> const&) () from build/libvalue.so
#8  0x00007ffff7dfa615 in std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::assign(std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> > const&) () from build/libvalue.so
#9  0x00007ffff7dfa49d in std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::operator=(std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> > const&) () from build/libvalue.so
#10 0x00007ffff7dfa2b6 in void Runtime::Value::Get<std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> > >(std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >&) () from build/libvalue.so
#11 0x0000000000400b11 in main ()
```

The issue is that the code attempts to delete the buffer used for empty string which should not happen.


The exact same code works fine which clang 3.2 and gcc 4.4.3 and above (I have not tested with older versions)

It is also working with clang 3.4 when `-fvisibility=hidden` is not set.


As far as I can see the problem is with the 

`std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage` symbol which is used for the "empty string" case.

The code generated by gcc is: (Working)
```console
% nm -a -C build_gcc/a.out |grep empty
0000000000401310 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000401426 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000602040 u std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage

% nm -a -C build_gcc/libvalue.so |grep empty
0000000000002fa6 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000002b71 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000205040 u std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage
```

The code generated by Clang 3.2 is:  (Working)
```console
% nm -a -C build_clang32/a.out |grep empty
0000000000401820 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000401560 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000603020 V std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage

% nm -a -C build_clang32/libvalue.so |grep empty
00000000000037e0 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000002c10 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000205020 V std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage
```

The code generated by Clang 3.4 is: (Crash)
```console
% nm -a -C build_clang34/a.out |grep empty
00000000004011c0 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000400f00 W std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000602020 V std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage

% nm -a -C build_clang34/libvalue.so |grep empty
0000000000002430 t std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_S_empty_rep()
0000000000001840 t std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep()
0000000000204020 b std::basic_string<char, std::char_traits<char>, Runtime::SecureAllocator<char> >::_Rep::_S_empty_rep_storage
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [20322](https://llvm.org/bz20322) |
| Version | 3.4 |
| OS | Linux |
| Attachments | [The source files and the build script to reproduce the issue.](https://user-images.githubusercontent.com/60944935/143749748-c72db997-c851-4a76-ac5b-7f297e366612.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor |

## Extended Description 


I am trying to assign a value to std::basic_string with a custom `Allocator` across a shared library boundaries and the code generated causes a crash when compiling with `-fvisibility=hidden`.

Here are files used:
```console
% cat SecureAllocator.h
```
```cpp
#pragma once

#ifndef SECURE_ALLOCATOR_H_
#define SECURE_ALLOCATOR_H_

#include &lt;memory&gt;
#include &lt;algorithm&gt;

namespace Runtime {


template &lt;class T&gt;
class SecureAllocator : public std::allocator&lt;T&gt; {
   typedef std::allocator&lt;T&gt; base_type;

public:
    template&lt;class U&gt; struct rebind { typedef SecureAllocator&lt;U&gt; other; };

    SecureAllocator() throw() {}
    SecureAllocator(const SecureAllocator&amp;) throw() : base_type() { }
    template &lt;class U&gt;
    SecureAllocator(const SecureAllocator&lt;U&gt;&amp;) throw() : base_type() { }

    void deallocate(typename base_type::pointer p,
                    typename base_type::size_type n) {
        std::fill_n((volatile char*)p, n * sizeof(T), 0);
        base_type::deallocate(p, n);
    }
};


} // namespace Runtime

#endif // SECURE_ALLOCATOR_H_
```

```console
% cat Value.h
```
```cpp
#pragma once

#ifndef VALUE_H_
#define VALUE_H_

#include &lt;string&gt;
#include "SecureAllocator.h"


namespace Runtime {

typedef std::basic_string&lt;char,
                          std::char_traits&lt;char&gt;,
                          SecureAllocator&lt;char&gt; &gt; SecretString;

class Value {
public:
   template&lt;typename T&gt;
   static void Get(T&amp; out);

};

} // namespace Runtime

#endif // VALUE_H_
```
```console
% cat Value.cpp
```
```cpp
#include "Value.h"

namespace Runtime {


template&lt;&gt;
__attribute__ ((visibility ("default"))) void
Value::Get&lt;SecretString&gt;(SecretString&amp; out)
{
   out = SecretString("something");
}


} // namespace Runtime
```
```console
% cat main.cpp
```
```cpp
#include "Value.h"

using namespace Runtime;

int main()
{
   SecretString tmp;
   Value::Get(tmp);
}
```
The file `Value.cpp` is build into the `libvalue.so` and then `main.cpp` is linked into an executable against `libvalue.so`
```console
% cat build-lite.sh
```
```sh
#!/bin/sh

SH=/bin/sh
BLD_DIR=build
CXX=${CXX:=clang++}

CXXFLAGS="-fPIC -fvisibility=hidden -pipe"
LDFLAGS=

echo "CXX = $CXX"
echo "CXXFLAGS = $CXXFLAGS"
echo "LDFLAGS = $LDFLAGS"

mkdir $BLD_DIR

# compile the files
$CXX -c -v -o $BLD_DIR/Value.o $CXXFLAGS Value.cpp
$CXX -c -v -o $BLD_DIR/main.o $CXXFLAGS main.cpp

# link the shared lib
$CXX -v -shared -o $BLD_DIR/libvalue.so $LDFLAGS $BLD_DIR/Value.o

# link the executable
$CXX -o $BLD_DIR/a.out $LDFLAGS $BLD_DIR/main.o $BLD_DIR/libvalue.so
```

Here is the output from the above script:
```console
% ./build-lite.sh
CXX = clang++
CXXFLAGS = -fPIC -fvisibility=hidden -pipe
LDFLAGS =

clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/dbc/dbc101/georgid/sysroot/bin/clang" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name Value.cpp -mrelocation-model pic -pic-level 2 -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.20.51.0.2 -v -coverage-file /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/Value.o -resource-dir /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7 -internal-isystem /usr/local/include -internal-isystem /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime -ferror-limit 19 -fmessage-length 229 -fvisibility hidden -mstackrealign -fobjc-runtime=gcc -fobjc-default-synthesize-properties -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -backend-option -vectorize-loops -o build/Value.o -x c++ Value.cpp
clang -cc1 version 3.4 based upon LLVM 3.4svn default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward
 /usr/local/include
 /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include
 /usr/include
End of search list.
clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/dbc/dbc101/georgid/sysroot/bin/clang" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.20.51.0.2 -v -coverage-file /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/main.o -resource-dir /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7 -internal-isystem /usr/local/include -internal-isystem /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime -ferror-limit 19 -fmessage-length 229 -fvisibility hidden -mstackrealign -fobjc-runtime=gcc -fobjc-default-synthesize-properties -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -backend-option -vectorize-loops -o build/main.o -x c++ main.cpp
clang -cc1 version 3.4 based upon LLVM 3.4svn default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/x86_64-redhat-linux/c++/4.4.7"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/x86_64-redhat-linux
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../include/c++/4.4.7/backward
 /usr/local/include
 /dbc/dbc101/georgid/sysroot/bin/../lib/clang/3.4/include
 /usr/include
End of search list.
clang version 3.4 (http://llvm.org/git/clang 48eff6c3512fd6c768072b05ab4c287c7719072b) (http://llvm.org/git/llvm 8240ef04108620fef51219e9495a6e71e95ccd75)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/usr/bin/ld" --eh-frame-hdr -m elf_x86_64 -shared -o build/libvalue.so /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64/crti.o /usr/lib/gcc/x86_64-redhat-linux/4.4.7/crtbeginS.o -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../.. -L/lib -L/usr/lib build/Value.o -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-redhat-linux/4.4.7/crtendS.o /usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../lib64/crtn.o
```


When I try to run the above binary I get:
```console
% ./build/a.out
*** glibc detected *** ./build/a.out: free(): invalid pointer: 0x0000000000601c40 ***
======= Backtrace: =========
/lib64/libc.so.6[0x38a1276126]
./build/a.out(_ZN9__gnu_cxx13new_allocatorIcE10deallocateEPcm+0x1d)[0x40104d]
build/libvalue.so(+0x194b)[0x7feb11d1d94b]
build/libvalue.so(+0x1884)[0x7feb11d1d884]
build/libvalue.so(+0x17da)[0x7feb11d1d7da]
build/libvalue.so(+0x1615)[0x7feb11d1d615]
build/libvalue.so(+0x149d)[0x7feb11d1d49d]
build/libvalue.so(_ZN7Runtime5Value3GetISbIcSt11char_traitsIcENS_15SecureAllocatorIcEEEEEvRT_+0x46)[0x7feb11d1d2b6]
./build/a.out[0x400b11]
/lib64/libc.so.6(__libc_start_main+0xfd)[0x38a121ecdd]
./build/a.out[0x400a29]
======= Memory map: ========
00400000-00402000 r-xp 00000000 fd:00 322571941                          /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/a.out
00601000-00602000 rw-p 00001000 fd:00 322571941                          /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/a.out
00e1e000-00e3f000 rw-p 00000000 00:00 0                                  [heap]
312a800000-312a8e8000 r-xp 00000000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312a8e8000-312aae8000 ---p 000e8000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aae8000-312aaef000 r--p 000e8000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aaef000-312aaf1000 rw-p 000ef000 08:03 26478803                       /usr/lib64/libstdc++.so.6.0.13
312aaf1000-312ab06000 rw-p 00000000 00:00 0
3728800000-3728816000 r-xp 00000000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
3728816000-3728a15000 ---p 00016000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
3728a15000-3728a16000 rw-p 00015000 08:03 524495                         /lib64/libgcc_s-4.4.7-20120601.so.1
38a0a00000-38a0a20000 r-xp 00000000 08:03 524290                         /lib64/ld-2.12.so
38a0c1f000-38a0c20000 r--p 0001f000 08:03 524290                         /lib64/ld-2.12.so
38a0c20000-38a0c21000 rw-p 00020000 08:03 524290                         /lib64/ld-2.12.so
38a0c21000-38a0c22000 rw-p 00000000 00:00 0
38a1200000-38a138a000 r-xp 00000000 08:03 524295                         /lib64/libc-2.12.so
38a138a000-38a1589000 ---p 0018a000 08:03 524295                         /lib64/libc-2.12.so
38a1589000-38a158d000 r--p 00189000 08:03 524295                         /lib64/libc-2.12.so
38a158d000-38a158e000 rw-p 0018d000 08:03 524295                         /lib64/libc-2.12.so
38a158e000-38a1593000 rw-p 00000000 00:00 0
38a1e00000-38a1e83000 r-xp 00000000 08:03 524301                         /lib64/libm-2.12.so
38a1e83000-38a2082000 ---p 00083000 08:03 524301                         /lib64/libm-2.12.so
38a2082000-38a2083000 r--p 00082000 08:03 524301                         /lib64/libm-2.12.so
38a2083000-38a2084000 rw-p 00083000 08:03 524301                         /lib64/libm-2.12.so
7feb11d07000-7feb11d0d000 rw-p 00000000 00:00 0
7feb11d1b000-7feb11d1c000 rw-p 00000000 00:00 0
7feb11d1c000-7feb11d20000 r-xp 00000000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11d20000-7feb11f1f000 ---p 00004000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11f1f000-7feb11f20000 rw-p 00003000 fd:00 322571940                  /dbc/dbc101/georgid/tmp/clang_bugs/clang-secure-alloc/runtime/build/libvalue.so
7feb11f20000-7feb11f21000 rw-p 00000000 00:00 0
7fff0964b000-7fff09661000 rw-p 00000000 00:00 0                          [stack]
7fff097ff000-7fff09800000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
[3]    64804 abort (core dumped)  ./build/a.out
```



In GDB the backtrace looks like this:
```
(gdb) bt
#<!-- -->0  0x00000038a12328e5 in raise (sig=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:64
#<!-- -->1  0x00000038a12340c5 in abort () at abort.c:92
#<!-- -->2  0x00000038a12707f7 in __libc_message (do_abort=2, fmt=0x38a1357fc0 "*** glibc detected *** %s: %s: 0x%s ***\n") at ../sysdeps/unix/sysv/linux/libc_fatal.c:198
#<!-- -->3  0x00000038a1276126 in malloc_printerr (action=3, str=0x38a13560af "free(): invalid pointer", ptr=&lt;value optimized out&gt;) at malloc.c:6311
#<!-- -->4  0x000000000040104d in __gnu_cxx::new_allocator&lt;char&gt;::deallocate(char*, unsigned long) ()
#<!-- -->5  0x00007ffff7dfa94b in Runtime::SecureAllocator&lt;char&gt;::deallocate(char*, unsigned long) () from build/libvalue.so
#<!-- -->6  0x00007ffff7dfa884 in std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_M_destroy(Runtime::SecureAllocator&lt;char&gt; const&amp;) () from build/libvalue.so
#<!-- -->7  0x00007ffff7dfa7da in std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_M_dispose(Runtime::SecureAllocator&lt;char&gt; const&amp;) () from build/libvalue.so
#<!-- -->8  0x00007ffff7dfa615 in std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::assign(std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt; const&amp;) () from build/libvalue.so
#<!-- -->9  0x00007ffff7dfa49d in std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::operator=(std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt; const&amp;) () from build/libvalue.so
#<!-- -->10 0x00007ffff7dfa2b6 in void Runtime::Value::Get&lt;std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt; &gt;(std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;&amp;) () from build/libvalue.so
#<!-- -->11 0x0000000000400b11 in main ()
```

The issue is that the code attempts to delete the buffer used for empty string which should not happen.


The exact same code works fine which clang 3.2 and gcc 4.4.3 and above (I have not tested with older versions)

It is also working with clang 3.4 when `-fvisibility=hidden` is not set.


As far as I can see the problem is with the 

`std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage` symbol which is used for the "empty string" case.

The code generated by gcc is: (Working)
```console
% nm -a -C build_gcc/a.out |grep empty
0000000000401310 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000401426 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000602040 u std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage

% nm -a -C build_gcc/libvalue.so |grep empty
0000000000002fa6 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000002b71 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000205040 u std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage
```

The code generated by Clang 3.2 is:  (Working)
```console
% nm -a -C build_clang32/a.out |grep empty
0000000000401820 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000401560 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000603020 V std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage

% nm -a -C build_clang32/libvalue.so |grep empty
00000000000037e0 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000002c10 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000205020 V std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage
```

The code generated by Clang 3.4 is: (Crash)
```console
% nm -a -C build_clang34/a.out |grep empty
00000000004011c0 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000400f00 W std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000602020 V std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage

% nm -a -C build_clang34/libvalue.so |grep empty
0000000000002430 t std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_S_empty_rep()
0000000000001840 t std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep()
0000000000204020 b std::basic_string&lt;char, std::char_traits&lt;char&gt;, Runtime::SecureAllocator&lt;char&gt; &gt;::_Rep::_S_empty_rep_storage
```
</details>


---

