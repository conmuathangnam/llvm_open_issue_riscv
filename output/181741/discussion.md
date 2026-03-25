# Clang frontend crash when validating malformed printf format attribute

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/181741

## Body

Clang crashes when attempting to compile malformed input with mix of ASCII and non-ASCII characters. The crash occurs in at least two clang versions (Clang 21.1.8 and Clang 20.1.2) on Ubuntu25 and is reproducible. The bug did not occur on another computer running Ubuntu22 with Clang 14.0.0. The bug was found using AFL++ and minimized by hand.

[crash.c](https://github.com/user-attachments/files/25349270/crash.c)

### Code:
```
00000000: 696e 7420 2f2f 2052 2563 6c61 6e67 5f67  int // R%clang_g
00000010: 6331 202d 6673 796e 7461 782d 6f6e 6c79  c1 -fsyntax-only
00000020: 2025 7320 323e 2631 207c 2046 696c 6543   %s 2>&1 | FileC
00000030: 6865 636b 202d 7374 7269 6374 2d77 6869  heck -strict-whi
00000040: 7465 7370 6163 6520 2573 0a0a 2f2f 2f20  tespace %s../// 
00000050: 7920 6c69 6e65 7320 696e 206d 756c 7469  y lines in multi
00000060: 2d6c 69b1 2000 a74c 9520 696e 6e6e 6520  -li. ..L. innne 
00000070: 6469 6167 6e6f 2d73 7472 6963 742d 7768  diagno-strict-wh
00000080: 6974 6573 7061 6365 2025 730a 0a2f 2f2f  itespace %s..///
00000090: 202e 0a73 7461 7469 635f 6173 7365 7274   ..static_assert
000000a0: 286d 6120 636c 616e 6720 6c6f 6f70 2076  (ma clang loop v
000000b0: 6563 6661 6c65 6520 2626 200a 2020 2020  ecfalee && .    
000000c0: 2020 2020 2020 2020 2016 7472 7565 2c20           .true, 
000000d0: 2222 293b 0a2f 2f20 4348 4543 4b2d 4e45  "");.// CHECK-NE
000000e0: 5854 3a20 7b95 5e7d 7d20 2020 2034 207c  XT: {.^}}    4 |
000000f0: 2073 7461 7469 635f 6173 7365 7274 2866   static_assert(f
00000100: 616c 7365 2026 267b 7b24 7d7d 0a2f 3838  alse &&{{$}}./88
00000110: 3233 3532 3934 2f3d 2e32 3335 3239 4c24  235294/=.23529L$
00000120: 2424 2424 2424 cdcd 642f 2043 485e 7d7d  $$$$$$..d/ CH^}}
00000130: 2020 2020 2020 7c20 2020 2020 2020 2020        |         
00000140: 2020 2020 2020 5e7e 7e7e 7e7e 7e7e 7b7b        ^~~~~~~~{{
00000150: 647d 7d0a 2f2f 2043 4845 434b 2d4e 4558  d}}.// CHECK-NEX
00000160: 543a 207b 7b5e 7d7d 2020 2020 2020 83ee  T: {{^}}      ..
00000170: 8083 3b00 0002 00ff 2020 2020 2020 7e7e  ..;.....      ~~
00000180: 7e7e 7b7b 247d 7d0a 0a0a 2f2f 2f20 236c  ~~{{$}}.../// #l
00000190: 696e 6520 7072 6167 6d61 7320 6172 6520  ine pragmas are 
000001a0: 7264 7370 6563 7465 640a 766f 6964 2070  rdspected.void p
000001b0: 7269 6e74 6628 636f 6e73 7420 6368 6172  rintf(const char
000001c0: 202a 612c 202a 2a2a 7fff 2a5c 5c2e 2e2e   *a, ***..*\\...
000001d0: 2920 5f5f 6174 7472 6962 7574 655f 5f28  ) __attribute__(
000001e0: 285f 5f66 6f72 6d61 745f 5f28 5f5f 7072  (__format__(__pr
000001f0: 696e 7466 5f5f 2c20 312c 3532 2929 293b  intf__, 1,52)));
00000200: 0a23 6c69 6e65 2031 300a 766f 6964 2066  .#line 10.void f
00000210: 2868 6e74 2078 2920 7b0a 2020 7072 696e  (hnt x) {.  prin
00000220: 7466 2822 25ea 222c 0a20 2020 2020 2020  tf("%.",.       
00000230: 2020 7829 3b0a 7d0a 2f2f 2043 4845 434b    x);.}.// CHECK
00000240: 3a20 3132 3a31 303a 2077 6172 6e69 6e67  : 12:10: warning
00000250: 3a20 666f 724b 2d4e 4558 543a 207b 7b5e  : forK-NEXT: {{^
00000260: 7d7d 2037 2031 3120 7c0a 2f2f 2043 4845  }} 7 11 |.// CHE
00000270: 434b 2d4e 4558 543a 207b 7b5e 7d7d 2020  CK-NEXT: {{^}}  
00000280: 2020 2020 7c74 2061 2c20 6e74 2062 2cff      |t a, nt b,.
00000290: 5854 3a20 7b7b 5e7d 7d20 2020 200f 207c  XT: {{^}}    . |
000002a0: 0a2f 2f20 4348 4543 4b2d 4e45 5854 3a20  .// CHECK-NEXT: 
000002b0: 7b7b 5e7d 7d20 2020 3132 207c 0a0a 236c  {{^}}   12 |..#l
000002c0: 696e 6520 3130 0a                        ine 10.                             
```
### Command:
```
clang crash.c
```
### Expected Output
Syntax Error and graceful termination

### Actual Output
Clang crashes with multiple parsing errors, UTF-8 errors, and eventually a stack smashing/segfault.
```
./crash.c:6:15: error: unknown type name 'ma'
    6 | static_assert(ma clang loop vecfalee && 
      |               ^
./crash.c:6:24: error: expected ')'
    6 | static_assert(ma clang loop vecfalee && 
      |                        ^
./crash.c:6:14: note: to match this '('
    6 | static_assert(ma clang loop vecfalee && 
      |              ^
./crash.c:9:1: error: expected identifier or '('
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      | ^
./crash.c:9:26: error: source file is not valid UTF-8
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      |                          ^
./crash.c:9:27: error: source file is not valid UTF-8
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      |                              ^
./crash.c:9:34: error: extraneous closing brace ('}')
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                        ^
./crash.c:9:35: error: extraneous closing brace ('}')
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                         ^
./crash.c:9:42: error: expected identifier or '('
    9 | /88235294/=.23529L$$$$$$$<CD><CD>d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                                ^
./crash.c:14:31: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
   14 | void printf(const char *a, ***<U+007F><FF>*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                               ^
      |                            int
./crash.c:14:31: error: expected ')'
./crash.c:14:12: note: to match this '('
   14 | void printf(const char *a, ***<U+007F><FF>*\\...) __attribute__((__format__(__printf__, 1,52)));
      |            ^
./crash.c:14:32: error: source file is not valid UTF-8
   14 | void printf(const char *a, ***<U+007F><FF>*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                                       ^
./crash.c:14:6: warning: GCC requires a function with the '__format__' attribute to be variadic [-Wgcc-compat]
   14 | void printf(const char *a, ***<U+007F><FF>*\\...) __attribute__((__format__(__printf__, 1,52)));
      |      ^
./crash.c:14:6: warning: incompatible redeclaration of library function 'printf' [-Wincompatible-library-redeclaration]
./crash.c:14:6: note: 'printf' is a builtin with type 'int (const char *, ...)'
./crash.c:10:8: error: unknown type name 'hnt'; did you mean 'int'?
   10 | void f(hnt x) {
      |        ^~~
      |        int
./crash.c:11:12: warning: illegal character encoding in string literal [-Winvalid-source-encoding]
   11 |   printf("%<EA>",
      |            ^~~~
./crash.c:12:10: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'int ***' [-Wint-conversion]
   12 |          x);
      |          ^
./crash.c:14:31: note: passing argument to parameter here
   14 | void printf(const char *a, ***<U+007F><FF>*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                               ^
./crash.c:11:12: warning: invalid conversion specifier '\xea' [-Wformat-invalid-specifier]
   11 |   printf("%<EA>",
      |           ~^~~~
*** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name crash.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Data/Clang_Orch_155/min_crashes_to_submit -fcoverage-compilation-dir=/home/user/test -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/crash-114959.o -x c ./crash.c
1.	./crash.c:12:11: current parser token ')'
2.	./crash.c:10:15: parsing function body 'f'
3.	./crash.c:10:15: in compound statement ('{}')
fatal error: error in backend: IO failure on output stream: No space left on device
```
### CASR Output:
```{
  "ProcFiles": [],
  "NetworkConnections": [],
  "CrashSeverity": {
    "Type": "EXPLOITABLE",
    "ShortDescription": "SegFaultOnPc",
    "Description": "Segmentation fault on program counter",
    "Explanation": "The target tried to access data at an address that matches the program counter. This likely indicates that the program counter contents are tainted and can be controlled by an attacker."
  }
}
```
### Environment
- OS: Ubuntu 25.04
- RAM: 64 GB
- CPU Architecture: x86_64

## Comments

### Comment 1 - shafik

So if you are going to submit bug reports based on fuzzing, we need you to do a few things to make this more effective:

- godbolt links w/ reproducer really make a huge difference
- How far back does this go, we really want only recent regression (past five releases) 
- You should look for duplicates and point them out in the report
- You need to use an assertions build so we can see any assertions, they are more useful because usually they fail earlier and point to the source of the bug better.
- We need you to include the assertion and the backtrace as it shows up in godbolt, this helps us to discover duplicates later on 
- Please point out this was generated via fuzzing

Finally we can't really process a large amount of fuzzing based reports, so you should prioritize recent regressions if you must.

Can you tell us more about what your trying to accomplish, are you doing this work on your own, as part of university or part of your job.

---

### Comment 2 - apbenson00

Thanks for letting me know, I was not familiar with how fuzzing bugs are normally presented.

Below is the  link to compiler explorer for the x86_64 clang trunk assertions build: 
https://godbolt.org/z/6oe5fq7x6

- This error did not appear when running the other non-assertion builds of clang on CompilerExplorer
- I did not see duplicate reports mentioning SemaChecking with a similar bug
- These fuzzing bugs were found during work on a university research project. Bugs were deduplicated, clustered, and classified using CASR (https://github.com/ispras/casr)
- I went through by hand and reported the unique "exploitable" bugs from CASR (however unlikely that may be)

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
Clang crashes when attempting to compile malformed input with mix of ASCII and non-ASCII characters. The crash occurs in at least two clang versions (Clang 21.1.8 and Clang 20.1.2) on Ubuntu25 and is reproducible. The bug did not occur on another computer running Ubuntu22 with Clang 14.0.0. The bug was found using AFL++ and minimized by hand.

[crash.c](https://github.com/user-attachments/files/25349270/crash.c)

### Code:
```
00000000: 696e 7420 2f2f 2052 2563 6c61 6e67 5f67  int // R%clang_g
00000010: 6331 202d 6673 796e 7461 782d 6f6e 6c79  c1 -fsyntax-only
00000020: 2025 7320 323e 2631 207c 2046 696c 6543   %s 2&gt;&amp;1 | FileC
00000030: 6865 636b 202d 7374 7269 6374 2d77 6869  heck -strict-whi
00000040: 7465 7370 6163 6520 2573 0a0a 2f2f 2f20  tespace %s../// 
00000050: 7920 6c69 6e65 7320 696e 206d 756c 7469  y lines in multi
00000060: 2d6c 69b1 2000 a74c 9520 696e 6e6e 6520  -li. ..L. innne 
00000070: 6469 6167 6e6f 2d73 7472 6963 742d 7768  diagno-strict-wh
00000080: 6974 6573 7061 6365 2025 730a 0a2f 2f2f  itespace %s..///
00000090: 202e 0a73 7461 7469 635f 6173 7365 7274   ..static_assert
000000a0: 286d 6120 636c 616e 6720 6c6f 6f70 2076  (ma clang loop v
000000b0: 6563 6661 6c65 6520 2626 200a 2020 2020  ecfalee &amp;&amp; .    
000000c0: 2020 2020 2020 2020 2016 7472 7565 2c20           .true, 
000000d0: 2222 293b 0a2f 2f20 4348 4543 4b2d 4e45  "");.// CHECK-NE
000000e0: 5854 3a20 7b95 5e7d 7d20 2020 2034 207c  XT: {.^}}    4 |
000000f0: 2073 7461 7469 635f 6173 7365 7274 2866   static_assert(f
00000100: 616c 7365 2026 267b 7b24 7d7d 0a2f 3838  alse &amp;&amp;{{$}}./88
00000110: 3233 3532 3934 2f3d 2e32 3335 3239 4c24  235294/=.23529L$
00000120: 2424 2424 2424 cdcd 642f 2043 485e 7d7d  $$$$$$..d/ CH^}}
00000130: 2020 2020 2020 7c20 2020 2020 2020 2020        |         
00000140: 2020 2020 2020 5e7e 7e7e 7e7e 7e7e 7b7b        ^~~~~~~~{{
00000150: 647d 7d0a 2f2f 2043 4845 434b 2d4e 4558  d}}.// CHECK-NEX
00000160: 543a 207b 7b5e 7d7d 2020 2020 2020 83ee  T: {{^}}      ..
00000170: 8083 3b00 0002 00ff 2020 2020 2020 7e7e  ..;.....      ~~
00000180: 7e7e 7b7b 247d 7d0a 0a0a 2f2f 2f20 236c  ~~{{$}}.../// #l
00000190: 696e 6520 7072 6167 6d61 7320 6172 6520  ine pragmas are 
000001a0: 7264 7370 6563 7465 640a 766f 6964 2070  rdspected.void p
000001b0: 7269 6e74 6628 636f 6e73 7420 6368 6172  rintf(const char
000001c0: 202a 612c 202a 2a2a 7fff 2a5c 5c2e 2e2e   *a, ***..*\\...
000001d0: 2920 5f5f 6174 7472 6962 7574 655f 5f28  ) __attribute__(
000001e0: 285f 5f66 6f72 6d61 745f 5f28 5f5f 7072  (__format__(__pr
000001f0: 696e 7466 5f5f 2c20 312c 3532 2929 293b  intf__, 1,52)));
00000200: 0a23 6c69 6e65 2031 300a 766f 6964 2066  .#line 10.void f
00000210: 2868 6e74 2078 2920 7b0a 2020 7072 696e  (hnt x) {.  prin
00000220: 7466 2822 25ea 222c 0a20 2020 2020 2020  tf("%.",.       
00000230: 2020 7829 3b0a 7d0a 2f2f 2043 4845 434b    x);.}.// CHECK
00000240: 3a20 3132 3a31 303a 2077 6172 6e69 6e67  : 12:10: warning
00000250: 3a20 666f 724b 2d4e 4558 543a 207b 7b5e  : forK-NEXT: {{^
00000260: 7d7d 2037 2031 3120 7c0a 2f2f 2043 4845  }} 7 11 |.// CHE
00000270: 434b 2d4e 4558 543a 207b 7b5e 7d7d 2020  CK-NEXT: {{^}}  
00000280: 2020 2020 7c74 2061 2c20 6e74 2062 2cff      |t a, nt b,.
00000290: 5854 3a20 7b7b 5e7d 7d20 2020 200f 207c  XT: {{^}}    . |
000002a0: 0a2f 2f20 4348 4543 4b2d 4e45 5854 3a20  .// CHECK-NEXT: 
000002b0: 7b7b 5e7d 7d20 2020 3132 207c 0a0a 236c  {{^}}   12 |..#l
000002c0: 696e 6520 3130 0a                        ine 10.                             
```
### Command:
```
clang crash.c
```
### Expected Output
Syntax Error and graceful termination

### Actual Output
Clang crashes with multiple parsing errors, UTF-8 errors, and eventually a stack smashing/segfault.
```
./crash.c:6:15: error: unknown type name 'ma'
    6 | static_assert(ma clang loop vecfalee &amp;&amp; 
      |               ^
./crash.c:6:24: error: expected ')'
    6 | static_assert(ma clang loop vecfalee &amp;&amp; 
      |                        ^
./crash.c:6:14: note: to match this '('
    6 | static_assert(ma clang loop vecfalee &amp;&amp; 
      |              ^
./crash.c:9:1: error: expected identifier or '('
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      | ^
./crash.c:9:26: error: source file is not valid UTF-8
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      |                          ^
./crash.c:9:27: error: source file is not valid UTF-8
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      |                              ^
./crash.c:9:34: error: extraneous closing brace ('}')
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                        ^
./crash.c:9:35: error: extraneous closing brace ('}')
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                         ^
./crash.c:9:42: error: expected identifier or '('
    9 | /88235294/=.23529L$$$$$$$&lt;CD&gt;&lt;CD&gt;d/ CH^}}      |               ^~~~~~~~{{d}}
      |                                                ^
./crash.c:14:31: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
   14 | void printf(const char *a, ***&lt;U+007F&gt;&lt;FF&gt;*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                               ^
      |                            int
./crash.c:14:31: error: expected ')'
./crash.c:14:12: note: to match this '('
   14 | void printf(const char *a, ***&lt;U+007F&gt;&lt;FF&gt;*\\...) __attribute__((__format__(__printf__, 1,52)));
      |            ^
./crash.c:14:32: error: source file is not valid UTF-8
   14 | void printf(const char *a, ***&lt;U+007F&gt;&lt;FF&gt;*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                                       ^
./crash.c:14:6: warning: GCC requires a function with the '__format__' attribute to be variadic [-Wgcc-compat]
   14 | void printf(const char *a, ***&lt;U+007F&gt;&lt;FF&gt;*\\...) __attribute__((__format__(__printf__, 1,52)));
      |      ^
./crash.c:14:6: warning: incompatible redeclaration of library function 'printf' [-Wincompatible-library-redeclaration]
./crash.c:14:6: note: 'printf' is a builtin with type 'int (const char *, ...)'
./crash.c:10:8: error: unknown type name 'hnt'; did you mean 'int'?
   10 | void f(hnt x) {
      |        ^~~
      |        int
./crash.c:11:12: warning: illegal character encoding in string literal [-Winvalid-source-encoding]
   11 |   printf("%&lt;EA&gt;",
      |            ^~~~
./crash.c:12:10: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'int ***' [-Wint-conversion]
   12 |          x);
      |          ^
./crash.c:14:31: note: passing argument to parameter here
   14 | void printf(const char *a, ***&lt;U+007F&gt;&lt;FF&gt;*\\...) __attribute__((__format__(__printf__, 1,52)));
      |                               ^
./crash.c:11:12: warning: invalid conversion specifier '\xea' [-Wformat-invalid-specifier]
   11 |   printf("%&lt;EA&gt;",
      |           ~^~~~
*** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name crash.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/user/Data/Clang_Orch_155/min_crashes_to_submit -fcoverage-compilation-dir=/home/user/test -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/crash-114959.o -x c ./crash.c
1.	./crash.c:12:11: current parser token ')'
2.	./crash.c:10:15: parsing function body 'f'
3.	./crash.c:10:15: in compound statement ('{}')
fatal error: error in backend: IO failure on output stream: No space left on device
```
### CASR Output:
```{
  "ProcFiles": [],
  "NetworkConnections": [],
  "CrashSeverity": {
    "Type": "EXPLOITABLE",
    "ShortDescription": "SegFaultOnPc",
    "Description": "Segmentation fault on program counter",
    "Explanation": "The target tried to access data at an address that matches the program counter. This likely indicates that the program counter contents are tainted and can be controlled by an attacker."
  }
}
```
### Environment
- OS: Ubuntu 25.04
- RAM: 64 GB
- CPU Architecture: x86_64
</details>


---

### Comment 4 - AaronBallman

This has nothing to do with non-ASCII characters. Smaller reproducer:
```c
void printf(const char *a, int x) __attribute__((__format__(__printf__, 1,4)));
void f(int x) {
  printf("%m", x);
}
```
https://godbolt.org/z/ssTr8978e

I think the issue is that when we need an invalid format specifier handler, we need the printf format specifier attribute information to be correct. I think we're missing a diagnostic about the incorrect attribute.

---

