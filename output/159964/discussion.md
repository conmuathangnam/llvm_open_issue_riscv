# [libomp] BUS error during omp_init_lock when used in ImageMagick 7.1.2-4 convert utility

**Author:** wammr
**URL:** https://github.com/llvm/llvm-project/issues/159964
**Status:** Open
**Labels:** crash, openmp:libomp

## Body

## [libomp] BUS error during omp_init_lock when used in ImageMagick 7.1.2-4 convert utility
Product: LLVM
Component: OpenMP (libomp)
Version: LLVM/Clang 16.0.6, libomp 16.0.6
Severity: crash
OS: Ubuntu 22.04.2 LTS, Linux kernel 6.8.0-49-generic, x86_64
Architecture: x86_64

### **Description:**
When running ImageMagick 7.1.2-4 `magick` utility (convert) on a specially crafted input file, the program crashes during global mutex initialization in the OpenMP runtime (libomp). The crash is reproducible reliably when AddressSanitizer is enabled.
The crash occurs in `omp_init_lock`, called from `InitializeMagickMutex` in `MagickCoreGenesis`. ASan reports a BUS error caused by a high-value memory access (likely an invalid pointer dereference).
This issue may impact server-side batch processing and automated workflows using ImageMagick with OpenMP.


**Steps to Reproduce:**

1. Compile ImageMagick 7.1.2-4 with AddressSanitizer enabled:
```
CFLAGS="-fsanitize=address -g" LDFLAGS="-fsanitize=address" ./configure
make -j$(nproc)
```

2. Obtain the attached crash input file: _crashsample_ (which can be obtained from attachments below)

3. Run:
```
ASAN_OPTIONS=abort_on_error=1:symbolize=1 /usr/local/imagemagick-fuzz/bin/magick /path/to/crash_sample null
```

4. Observe crash and ASan output.


### **ASan Stack Trace:**

```
AddressSanitizer:DEADLYSIGNAL
=================================================================
==2193077==ERROR: AddressSanitizer: BUS on unknown address (pc 0x7ffff5ea6ee1 bp 0x7fffffffde00 sp 0x7fffffffd5b8 T0)
#0 0x7ffff5ea6ee1 (/lib/x86_64-linux-gnu/libc.so.6+0x1b1ee1)
#1 0x5555555f7ccb in __interceptor_strcpy (/usr/local/imagemagick-fuzz/bin/magick+0xa3ccb)
#2 0x7ffff62ad6f4 (/usr/lib/llvm-16/lib/libomp.so.5+0x466f4)
#3 0x7ffff629f0fb (/usr/lib/llvm-16/lib/libomp.so.5+0x380fb)
#4 0x7ffff629f044 (/usr/lib/llvm-16/lib/libomp.so.5+0x38044)
#5 0x7ffff633563c in omp_init_lock (/usr/lib/llvm-16/lib/libomp.so.5+0xce63c)
#6 0x7ffff7456c00 in InitializeMagickMutex /src/ImageMagick/./MagickCore/mutex.h:60:5
#7 0x7ffff7456c00 in MagickCoreGenesis /src/ImageMagick/MagickCore/magick.c:1497:3
#8 0x55555564b2f7 in MagickMain /src/ImageMagick/utilities/magick.c:133:3
#9 0x55555564b2f7 in main /src/ImageMagick/utilities/magick.c:193:10
#10 0x7ffff5d1ed8f (/lib/x86_64-linux-gnu/libc.so.6+0x29d8f)
#11 0x7ffff5d1ee3f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e3f)
#12 0x5555555744a4 in _start (/usr/local/imagemagick-fuzz/bin/magick+0x204a4)

SUMMARY: AddressSanitizer: BUS (/lib/x86_64-linux-gnu/libc.so.6+0x1b1ee1) in __interceptor_strcpy
==2193077==ABORTING

```

### **Impact:**

- Type: Memory access violation (BUS error / SIGBUS) during OpenMP lock initialization
- Affected: ImageMagick users with OpenMP enabled
- May trigger denial-of-service in server applications or batch processing environments


### **Additional Notes:**

- Occurs during global OpenMP mutex initialization (`translation_unit_mutex`)
- Appears independent of number of threads
- ImageMagick maintainers suggested reporting to OpenMP/LLVM team
- No hardening in ImageMagick prevents this crash (atomic checks, memset, etc.)


### **Environment:**
- LLVM/Clang: 16.0.6
- libomp: 16.0.6
- ImageMagick: 7.1.2-4 (Q16-HDRI, OpenMP 5.0)
- OS: Ubuntu 22.04.2 LTS, Linux kernel 6.8.0-49-generic, x86_64
- Compiler options: -fsanitize=address -g

### **Attachments:**

1. Crash input file
[crashsample.zip](https://github.com/user-attachments/files/22449330/crashsample.zip)

2. ASan log (.txt)
[asanlog.txt](https://github.com/user-attachments/files/22449329/asanlog.txt)

