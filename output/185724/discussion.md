# [libc++] 608012ace43b breaks ABI for cin, cout and friends

**Author:** DimitryAndric
**URL:** https://github.com/llvm/llvm-project/issues/185724
**Status:** Closed
**Labels:** libc++, ABI, regression:21
**Closed Date:** 2026-03-12T09:18:27Z

## Body

The simplification of `iostream.cpp` in `llvmorg-21-init-111-g`608012ace43b (#124103, by @philnik777, cc @ldionne) changes the sizes of `cin`, `cout`, and friends, which breaks the ABI. Because of copy relocations, those sizes can be emitted into executables (typically when not compiled in PIE mode). Those executables, compiled against libc++ 20, will no longer run properly against libc++ 21, usually throwing an exception, but other strange behavior is also possible.

Case in point is `helloworld.cpp`:

```c++
#include <iostream>

int main(void)
{
  std::cout << "Hello World!\n";
  return 0;
}
```

When compiled against libc++ 20 (or libc++ just before `llvmorg-21-init-111-g`608012ace43b) on a recent FreeBSD system, where PIE mode is _not_ the default, `cout` and `ctype<char>::id` will get copy relocations in the executable:

```text
Relocation section '.rela.dyn' at offset 0x9b8 contains 2 entries:
    Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
0000000000205230  0000001400000005 R_X86_64_COPY          0000000000205230 std::__1::cout + 0
00000000002052e0  0000001500000005 R_X86_64_COPY          00000000002052e0 std::__1::ctype<char>::id + 0
```

The symbols themselves end up in the `.dynsym` section:

```text
Symbol table '.dynsym' contains 23 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    20: 0000000000205230   160 OBJECT  GLOBAL DEFAULT   25 std::__1::cout
    21: 00000000002052e0    16 OBJECT  GLOBAL DEFAULT   25 std::__1::ctype<char>::id
```

Note that `cout` is 160 bytes, while `ctype<char>::id` is 16 bytes.

Compiling the same program against libc++ 21 (or libc++ just after `llvmorg-21-init-111-g`608012ace43b) will show similar copy relocations in the executable, but for the symbols themselves:

```text
Symbol table '.dynsym' contains 23 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    20: 0000000000205020   384 OBJECT  GLOBAL DEFAULT   25 std::__1::cout
    21: 00000000002051a0    16 OBJECT  GLOBAL DEFAULT   25 std::__1::ctype<char>::id
```

Note how `cout` has grown to 384 bytes.

When you run the `helloworld-against-libc++20` binary against libc++ 21 (after 608012ace43b), the latter's `iostream.cpp` will scribble its 384 bytes of data over the 160 bytes allocated for `cout`, typically zeroing `ctype<char>::id`'s state.

When that happens, the next time a locale attempts to `__get` the `id` for the `ctype<char>` fact, it will get a new id, typically 30, which is over the limit of the locale's `facets_` vector. Whenever this facet is used with `use_facet`, a `bad_cast` exception will be thrown:

```text
#0  __cxa_throw (thrown_exception=0x800a08080, tinfo=0x8003e4280 <typeinfo for std::bad_cast>, dest=0x8003e24a0 <std::exception::~exception()>) at /usr/src/contrib/libcxxrt/exception.cc:827
#1  0x00000008003502b5 in std::__1::__throw_bad_cast[abi:se210108]() () at /share/dim/src/freebsd/llvm-21-update/contrib/llvm-project/libcxx/include/typeinfo:382
#2  0x000000080037893a in std::__1::locale::__imp::use_facet (this=0x8003c7a68 <std::__1::locale::__imp::classic_locale_imp_>, id=30) at /share/dim/src/freebsd/llvm-21-update/contrib/llvm-project/libcxx/src/locale.cpp:481
#3  0x0000000800379580 in std::__1::locale::use_facet (this=0x7fffffffe228, x=...) at /share/dim/src/freebsd/llvm-21-update/contrib/llvm-project/libcxx/src/locale.cpp:575
#4  0x0000000000202dbf in std::__1::use_facet[abi:se190107]<std::__1::ctype<char> >(std::__1::locale const&) (__l=...) at /usr/include/c++/v1/__locale:168
#5  0x0000000000202cfc in std::__1::basic_ios<char, std::__1::char_traits<char> >::widen[abi:se190107](char) const (this=0x205238 <std::__1::cout+8>, __c=32 ' ') at /usr/include/c++/v1/ios:693
#6  0x00000000002028f2 in std::__1::basic_ios<char, std::__1::char_traits<char> >::fill[abi:se190107]() const (this=0x205238 <std::__1::cout+8>) at /usr/include/c++/v1/ios:701
#7  0x000000000020253e in std::__1::__put_character_sequence[abi:se190107]<char, std::__1::char_traits<char> >(std::__1::basic_ostream<char, std::__1::char_traits<char> >&, char const*, unsigned long) (__os=..., __str=0x200b69 "Hello World!\n", __len=13) at /usr/include/c++/v1/__ostream/basic_ostream.h:512
#8  0x0000000000202479 in std::__1::operator<<[abi:se190107]<std::__1::char_traits<char> >(std::__1::basic_ostream<char, std::__1::char_traits<char> >&, char const*) (__os=..., __str=0x200b69 "Hello World!\n") at /usr/include/c++/v1/__ostream/basic_ostream.h:619
#9  0x0000000000202438 in main () at helloworld.cpp:5
```

@emaste originally noticed this `bad_cast` exception when running the `cmake` binary from a FreeBSD package which was built against libc++ 19, on a system which had libc++ 21. But any program that uses the standard streams may crash in such a manner.

The problem is also not unique to FreeBSD, as it can be reproduced on Linux. However, most modern distributions use PIE mode by default, either by patching their compiler(s), or by submitting patches upstream. When compiled in PIE mode on x86_64, the relocations become GLOB_DAT relocations instead:

```text
Relocation section '.rela.dyn' at offset 0x9c8 contains 11 entries:
    Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
...
0000000000003fd0  0000000b00000006 R_X86_64_GLOB_DAT      0000000000000000 std::__1::ctype<char>::id + 0
0000000000003fd8  0000001500000006 R_X86_64_GLOB_DAT      0000000000000000 std::__1::cout + 0
```

and the `cout` and `ctype<char>::id` become `UND` instead, so the object is resolved a runtime instead:

```text
Symbol table '.dynsym' contains 25 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    11: 0000000000000000     0 OBJECT  GLOBAL DEFAULT  UND std::__1::ctype<char>::id
...
    21: 0000000000000000     0 OBJECT  GLOBAL DEFAULT  UND std::__1::cout
```

But if you compile the program with `-fno-PIE -no-pie`, even on Linux will you get the copy relocations, and the sizes of the objects in the symbol table. I.e. with libc++ 20:

```text
Relocation section '.rela.dyn' at offset 0x940 contains 4 entries:
    Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
...
00000000004040a0  0000001400000005 R_X86_64_COPY          00000000004040a0 std::__1::ctype<char>::id + 0
00000000004040b0  0000001300000005 R_X86_64_COPY          00000000004040b0 std::__1::cout + 0

...
Symbol table '.dynsym' contains 22 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    19: 00000000004040b0   160 OBJECT  GLOBAL DEFAULT   25 std::__1::cout
    20: 00000000004040a0    16 OBJECT  GLOBAL DEFAULT   25 std::__1::ctype<char>::id
```

and with libc++ 21:

```text
Relocation section '.rela.dyn' at offset 0x940 contains 4 entries:
    Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
...
00000000004040a0  0000001400000005 R_X86_64_COPY          00000000004040a0 std::__1::ctype<char>::id + 0
00000000004040b0  0000001300000005 R_X86_64_COPY          00000000004040b0 std::__1::cout + 0

...
Symbol table '.dynsym' contains 22 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    19: 00000000004040b0   264 OBJECT  GLOBAL DEFAULT   25 std::__1::cout
    20: 00000000004040a0    16 OBJECT  GLOBAL DEFAULT   25 std::__1::ctype<char>::id
```

Here, `cout` grows from 160 to 264 bytes, and it doesn't cause the program to crash immediately, but you get a warning from glib's dynamic linker instead:

```text
$ LD_LIBRARY_PATH=~/llvm/llvmorg-21-init-111-g608012ace43b-linux6-x86_64-ninja-gcc-rel-1/lib/x86_64-unknown-linux-gnu
./llvmorg-21-init-110-g07a0e2be86f3/helloworld
./llvmorg-21-init-110-g07a0e2be86f3/helloworld: Symbol `_ZNSt3__14coutE' has different size in shared object, consider re-linking
Hello World!
```

Ergo, on Linux there is also ABI breakage. I didn't inspect other platforms, but I suspect most of them might have a similar problem.


## Comments

### Comment 1 - Desel72

Hi @DimitryAndric 
I'd love to work on this. Can I pick this up?
Who can assign this to me?

---

### Comment 2 - ldionne

CC @llvm/libcxx-vendors 

We have a fix in #185839 thanks for @philnik777 and the fix should make it into LLVM 22.1.2 if everything goes well. We believe this issue only affects people building in non-PIE mode.

---

### Comment 3 - DimitryAndric

Thanks for the quick fix!


---

