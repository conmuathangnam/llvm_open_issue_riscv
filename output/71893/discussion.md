# opt -O2 results in incorrect code on RPI (arm 32)

**Author:** sjpschutte
**URL:** https://github.com/llvm/llvm-project/issues/71893

## Body

Hello all

I am running into a problem where the code seems to be incorrect after running it through the optimizer.
**It only happens on 32bit arm (RPI, etc)
When I follow the same steps on x86_64(linux) or arm64(MacOS), everything works as expected.**
The code also works correctly if the optimizer is not used.
I basically get the same when compiling the code with llc, but showing the problem is easier with lli.

The steps to show the problem is as follows:

1) This works correctly and produce the correct output.
pi@rpi400:~/stcompile $ $LLVM/bin/lli --load=runtime.so obj/json.bc
warning: ignoring debug info with an invalid version (0) in obj/json.bc
zone:internal:iface = eth1
filter is type JSONtype:ARRAY
filter:size=7
filter:0:service = icmp
filter:1:service = http
filter:2:service = https
filter:3:service = hastat
filter:4:service = frmon
filter:5:service = ifmon
filter:6:service = pssh
parse:size=7
description
log
zone
policy
filter
service
snat

2) Then I run it through the optimizer just using -O2. (The problem arise with any level above -O0)
pi@rpi400:~/stcompile $ $LLVM/bin/opt -O2 obj/json.bc -o obj/json.bc2
warning: ignoring debug info with an invalid version (0) in obj/json.bc

3) I now try to run the optimized version, but it seems broken.
pi@rpi400:~/stcompile` $ $LLVM/bin/lli --load=runtime.so obj/json.bc2

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.

Stack dump:
0.      Program arguments: /home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli --load=runtime.so obj/json.bc2
#0 0x00fe76fc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli+0xfe76fc)
#1 0x00fe55e4 llvm::sys::RunSignalHandlers() (/home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli+0xfe55e4)
#2 0x00fe57c4 SignalHandler(int) Signals.cpp:0:0
#3 0xffffffffb6b4c900 __default_sa_restorer ./signal/../sysdeps/unix/sysv/linux/arm/sigrestorer.S:67:0
Segmentation fault`

These are the versions of the tools I used for this report:

pi@rpi400:~/stcompile $ $LLVM/bin/lli -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.

pi@rpi400:~/stcompile $ $LLVM/bin/opt -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.
  Default target: armv7l-unknown-linux-gnueabihf
  Host CPU: cortex-a72


pi@rpi400:~/stcompile $ $LLVM/bin/llc -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.
  Default target: armv7l-unknown-linux-gnueabihf
  Host CPU: cortex-a72

  Registered Targets:
    aarch64     - AArch64 (little endian)
    aarch64_32  - AArch64 (little endian ILP32)
    aarch64_be  - AArch64 (big endian)
    amdgcn      - AMD GCN GPUs
    arm         - ARM
    arm64       - ARM64 (little endian)
    arm64_32    - ARM64 (little endian ILP32)
    armeb       - ARM (big endian)
    avr         - Atmel AVR Microcontroller
    bpf         - BPF (host endian)
    bpfeb       - BPF (big endian)
    bpfel       - BPF (little endian)
    hexagon     - Hexagon
    lanai       - Lanai
    loongarch32 - 32-bit LoongArch
    loongarch64 - 64-bit LoongArch
    mips        - MIPS (32-bit big endian)
    mips64      - MIPS (64-bit big endian)
    mips64el    - MIPS (64-bit little endian)
    mipsel      - MIPS (32-bit little endian)
    msp430      - MSP430 [experimental]
    nvptx       - NVIDIA PTX 32-bit
    nvptx64     - NVIDIA PTX 64-bit
    ppc32       - PowerPC 32
    ppc32le     - PowerPC 32 LE
    ppc64       - PowerPC 64
    ppc64le     - PowerPC 64 LE
    r600        - AMD GPUs HD2XXX-HD6XXX
    riscv32     - 32-bit RISC-V
    riscv64     - 64-bit RISC-V
    sparc       - Sparc
    sparcel     - Sparc LE
    sparcv9     - Sparc V9
    systemz     - SystemZ
    thumb       - Thumb
    thumbeb     - Thumb (big endian)
    ve          - VE
    wasm32      - WebAssembly 32-bit
    wasm64      - WebAssembly 64-bit
    x86         - 32-bit X86: Pentium-Pro and above
    x86-64      - 64-bit X86: EM64T and AMD64
    xcore       - XCore

The project was compiled using sources from github with these options:
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_INCLUDE_BENCHMARKS=OFF ..

But I tried different versions (13.0.0, 14.0.1, 15.0.0, 17.0.1) of llvm and the result is always the same, so it is not tied a specific version.

[json.bc2.gz](https://github.com/llvm/llvm-project/files/13315512/json.bc2.gz)
[json.bc.gz](https://github.com/llvm/llvm-project/files/13315515/json.bc.gz)
[runtime.so.gz](https://github.com/llvm/llvm-project/files/13315516/runtime.so.gz)


## Comments

### Comment 1 - sjpschutte

The issue goes away when you have less variables on the stack.
There seems to be a problem when you have a lot of stack variables and use optimization.
I can see the stack pointer is not advancing under these circumstances.
It only happens on 32bit arm. I have no problems with any other architecture. 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: None (sjpschutte)

<details>
Hello all

I am running into a problem where the code seems to be incorrect after running it through the optimizer.
**It only happens on 32bit arm (RPI, etc)
When I follow the same steps on x86_64(linux) or arm64(MacOS), everything works as expected.**
The code also works correctly if the optimizer is not used.
I basically get the same when compiling the code with llc, but showing the problem is easier with lli.

The steps to show the problem is as follows:

1) This works correctly and produce the correct output.
pi@<!-- -->rpi400:~/stcompile $ $LLVM/bin/lli --load=runtime.so obj/json.bc
warning: ignoring debug info with an invalid version (0) in obj/json.bc
zone:internal:iface = eth1
filter is type JSONtype:ARRAY
filter:size=7
filter:0:service = icmp
filter:1:service = http
filter:2:service = https
filter:3:service = hastat
filter:4:service = frmon
filter:5:service = ifmon
filter:6:service = pssh
parse:size=7
description
log
zone
policy
filter
service
snat

2) Then I run it through the optimizer just using -O2. (The problem arise with any level above -O0)
pi@<!-- -->rpi400:~/stcompile $ $LLVM/bin/opt -O2 obj/json.bc -o obj/json.bc2
warning: ignoring debug info with an invalid version (0) in obj/json.bc

3) I now try to run the optimized version, but it seems broken.
pi@<!-- -->rpi400:~/stcompile` $ $LLVM/bin/lli --load=runtime.so obj/json.bc2

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.

Stack dump:
0.      Program arguments: /home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli --load=runtime.so obj/json.bc2
#<!-- -->0 0x00fe76fc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli+0xfe76fc)
#<!-- -->1 0x00fe55e4 llvm::sys::RunSignalHandlers() (/home/pi/devel/llvm-project-17.0.1.src/llvm/build/bin/lli+0xfe55e4)
#<!-- -->2 0x00fe57c4 SignalHandler(int) Signals.cpp:0:0
#<!-- -->3 0xffffffffb6b4c900 __default_sa_restorer ./signal/../sysdeps/unix/sysv/linux/arm/sigrestorer.S:67:0
Segmentation fault`

These are the versions of the tools I used for this report:

pi@<!-- -->rpi400:~/stcompile $ $LLVM/bin/lli -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.

pi@<!-- -->rpi400:~/stcompile $ $LLVM/bin/opt -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.
  Default target: armv7l-unknown-linux-gnueabihf
  Host CPU: cortex-a72


pi@<!-- -->rpi400:~/stcompile $ $LLVM/bin/llc -version
LLVM (http://llvm.org/):
  LLVM version 17.0.1
  Optimized build.
  Default target: armv7l-unknown-linux-gnueabihf
  Host CPU: cortex-a72

  Registered Targets:
    aarch64     - AArch64 (little endian)
    aarch64_32  - AArch64 (little endian ILP32)
    aarch64_be  - AArch64 (big endian)
    amdgcn      - AMD GCN GPUs
    arm         - ARM
    arm64       - ARM64 (little endian)
    arm64_32    - ARM64 (little endian ILP32)
    armeb       - ARM (big endian)
    avr         - Atmel AVR Microcontroller
    bpf         - BPF (host endian)
    bpfeb       - BPF (big endian)
    bpfel       - BPF (little endian)
    hexagon     - Hexagon
    lanai       - Lanai
    loongarch32 - 32-bit LoongArch
    loongarch64 - 64-bit LoongArch
    mips        - MIPS (32-bit big endian)
    mips64      - MIPS (64-bit big endian)
    mips64el    - MIPS (64-bit little endian)
    mipsel      - MIPS (32-bit little endian)
    msp430      - MSP430 [experimental]
    nvptx       - NVIDIA PTX 32-bit
    nvptx64     - NVIDIA PTX 64-bit
    ppc32       - PowerPC 32
    ppc32le     - PowerPC 32 LE
    ppc64       - PowerPC 64
    ppc64le     - PowerPC 64 LE
    r600        - AMD GPUs HD2XXX-HD6XXX
    riscv32     - 32-bit RISC-V
    riscv64     - 64-bit RISC-V
    sparc       - Sparc
    sparcel     - Sparc LE
    sparcv9     - Sparc V9
    systemz     - SystemZ
    thumb       - Thumb
    thumbeb     - Thumb (big endian)
    ve          - VE
    wasm32      - WebAssembly 32-bit
    wasm64      - WebAssembly 64-bit
    x86         - 32-bit X86: Pentium-Pro and above
    x86-64      - 64-bit X86: EM64T and AMD64
    xcore       - XCore

The project was compiled using sources from github with these options:
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_INCLUDE_BENCHMARKS=OFF ..

But I tried different versions (13.0.0, 14.0.1, 15.0.0, 17.0.1) of llvm and the result is always the same, so it is not tied a specific version.

[json.bc2.gz](https://github.com/llvm/llvm-project/files/13315512/json.bc2.gz)
[json.bc.gz](https://github.com/llvm/llvm-project/files/13315515/json.bc.gz)
[runtime.so.gz](https://github.com/llvm/llvm-project/files/13315516/runtime.so.gz)

</details>


---

