# llvm-bolt --instrument crash happens

**Author:** zhouyu0502
**URL:** https://github.com/llvm/llvm-project/issues/158792
**Status:** Open
**Labels:** BOLT, crash

## Body

1. i want to instrument a shared library which is arm64 arch in x86_64 server (i successfully cross-compiler bolt-rt in x86_64 for arm64 and verify bolt instrument  feature sucessfully in simple arm64 so), but i come across this crash in a complicated so because of assert **ContainingBF && "cannot find function for address in code"**

<img width="3662" height="1348" alt="Image" src="https://github.com/user-attachments/assets/c1e8dbcb-b17b-4e54-a522-f068a04b0799" />

./llvmtest/build_debug/bin/llvm-bolt libduml_fastrtps.so --instrument -o out.so
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: aarch64
BOLT-INFO: BOLT version: 6abb6264ea5577071dc5ac9e289dd1660553ad39
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x800000, offset 0x800000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
llvm-bolt: /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2762:  void llvm::bolt::RewriteInstance::handleRelocation(const llvm::object::SectionRef&, const llvm::object::RelocationRef&): **Assertion ContainingBF && "cannot find function for address in code"' failed.**
 #0 0x0000556fbf94bf3a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
#1 0x0000556fbf94c3bf PrintStackTraceSignalHandler(void*) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
#2 0x0000556fbf949650 llvm::sys::RunSignalHandlers() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Signals.cpp:104:20
#3 0x0000556fbf94b785 SignalHandler(int, siginfo_t*, void*) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
#4 0x00007ff55a820420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
#5 0x00007ff55a2ed00b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
#6 0x00007ff55a2cc859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
#7 0x00007ff55a2cc729 get_sysdep_segment_value /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:509:8
#8 0x00007ff55a2cc729 _nl_load_domain /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:970:34
#9 0x00007ff55a2ddfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
#10 0x0000556fbfa9deb2 llvm::bolt::RewriteInstance::handleRelocation(llvm::object::SectionRef const&, llvm::object::RelocationRef const&) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2763:9
#11 0x0000556fbfa9d320 llvm::bolt::RewriteInstance::readRelocations(llvm::object::SectionRef const&) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2645:55
#12 0x0000556fbfa9b979 llvm::bolt::RewriteInstance::processRelocations() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2453:20
#13 0x0000556fbfa95344 llvm::bolt::RewriteInstance::discoverFileObjects() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:1352:20
#14 0x0000556fbfa911ca llvm::bolt::RewriteInstance::run() /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:709:32
#15 0x0000556fbe79a8fc main /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/tools/driver/llvm-bolt.cpp:267:29
#16 0x00007ff55a2ce083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#17 0x0000556fbe79942e _start (./llvmtest/build_debug/bin/llvm-bolt+0x29c42e)

2.i add some logs and find out this happens when **exe handleRelocation() in readRelocations() for section rela.init**
it cannot find **offset rela.init (0x13f904)** in **BinaryFunctions**
Rel.getOffset() = **0x13f904**
BinaryFunctions.size() = 12912 
BinaryFunctions.firstkey = **0x13f920**
BinaryFunctions.lastkey  = 0x50da40
so **getBinaryFunctionContainingAddress(**Rel.getOffset()) return nullptr and cause crash

<img width="2697" height="656" alt="Image" src="https://github.com/user-attachments/assets/6e0adf2a-8a84-4e4a-9731-26d0160e8574" />


P.S.  **readelf -r rela.init** section shows Offset is  **00000013f904** 

Relocation section '.rela.init' at offset 0x7179b0 contains 1 entry:
   Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
**00000013f904**  00010000011b R_AARCH64_CALL26  000000000014eb10 .text + 85d6c



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bolt

Author: None (zhouyu0502)

<details>
1. i want to instrument a shared library which is arm64 arch in x86_64 server (i successfully cross-compiler bolt-rt in x86_64 for arm64),but i come across this crash because of assert **ContainingBF &amp;&amp; "cannot find function for address in code"**

&lt;img width="3662" height="1348" alt="Image" src="https://github.com/user-attachments/assets/c1e8dbcb-b17b-4e54-a522-f068a04b0799" /&gt;


2.i add some logs and find out 
Rel.getOffset() = 0x13f904
BinaryFunctions.size() = 12912 
BinaryFunctions.firstkey = 0x13f920
BinaryFunctions.lastkey  = 0x50da40
so getBinaryFunctionContainingAddress(Rel.getOffset()) return nullptr
&lt;img width="2697" height="656" alt="Image" src="https://github.com/user-attachments/assets/6e0adf2a-8a84-4e4a-9731-26d0160e8574" /&gt;

3.how can i debug this bug?
</details>


---

### Comment 2 - zhouyu0502

1. i add return when ContainingBF is nullptr to avoid the first crash(which is not right but just for debug ).But another crash happens because of assert **Start && "Entry point function not found"**

<img width="2918" height="704" alt="Image" src="https://github.com/user-attachments/assets/a9ec91b1-5bc8-4b2f-ad4e-86298924b9a5" />

 ./llvmtest/build_debug/bin/llvm-bolt sobintest/duml/v2_nopac/libduml_fastrtps.so -instrument -o out.s
o
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: aarch64
BOLT-INFO: BOLT version: 6abb6264ea5577071dc5ac9e289dd1660553ad39
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x800000, offset 0x800000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
BOLT-INFO: number of removed linker-inserted veneers: 0
BOLT-INFO: 0 out of 9043 functions in the binary (0.0%) have non-empty execution profile
llvm-bolt: /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Passes/Instrumentation.cpp:748: void llvm::bolt::Instrumentation::createAuxiliaryFunctions(llvm::bolt::BinaryContext&): **Assertion `Start && "Entry point function not found"'** failed.
#0 0x00005590e7f60f3a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x00005590e7f613bf PrintStackTraceSignalHandler(void*) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x00005590e7f5e650 llvm::sys::RunSignalHandlers() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Signals.cpp:104:20
#3 0x00005590e7f60785 SignalHandler(int, siginfo_t*, void*) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x00007f6903a7a420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #5 0x00007f690354700b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #6 0x00007f6903526859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
 #7 0x00007f6903526729 get_sysdep_segment_value /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:509:8
 #8 0x00007f6903526729 _nl_load_domain /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:970:34
 #9 0x00007f6903537fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
#10 0x00005590e8a9eca8 llvm::bolt::Instrumentation::createAuxiliaryFunctions(llvm::bolt::BinaryContext&) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Passes/Instrumentation.cpp:749:50
#11 0x00005590e8a9deaa llvm::bolt::Instrumentation::runOnFunctions(llvm::bolt::BinaryContext&) (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Passes/Instrumentation.cpp:622:3
#12 0x00005590e8163260 llvm::bolt::BinaryFunctionPassManager::runPasses()::'lambda'()::operator()() const /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/BinaryPassManager.cpp:308:25
#13 0x00005590e81652a3 void llvm::bolt::callWithDynoStats<llvm::bolt::BinaryFunctionPassManager::runPasses()::'lambda'(), std::map<unsigned long, llvm::bolt::BinaryFunction, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::bolt::BinaryFunction> > > >(llvm::raw_ostream&, llvm::bolt::BinaryFunctionPassManager::runPasses()::'lambda'()&&, std::map<unsigned long, llvm::bolt::BinaryFunction, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::bolt::BinaryFunction> > >&, llvm::StringRef, bool, bool) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/include/bolt/Core/DynoStats.h:171:3
#14 0x00005590e81636c8 llvm::bolt::BinaryFunctionPassManager::runPasses() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/BinaryPassManager.cpp:306:22
#15 0x00005590e8164fad llvm::bolt::BinaryFunctionPassManager::runAllPasses(llvm::bolt::BinaryContext&) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/BinaryPassManager.cpp:351:39
#16 0x00005590e80b89c3 llvm::bolt::RewriteInstance::runOptimizationPasses() (.localalias) /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:3624:34
#17 0x00005590e80a63c2 llvm::bolt::RewriteInstance::run() /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:732:26
#18 0x00005590e6daf8fc main /home/yulia.zhou/code/llvm/llvmtest/llvm-project/bolt/tools/driver/llvm-bolt.cpp:267:29
#19 0x00007f6903528083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#20 0x00005590e6dae42e _start (./llvmtest/build_debug/bin/llvm-bolt+0x29c42e)

2.i add some logs and find the reason is  can not **getBinaryFunctionAtAddress** in BinaryFunctions  when StartFunctionAddress = **0x13f8f8**


3. readelf -d | grep INIT  show how to get the StartFunctionAddress **0x13f8f8**

Dynamic section at offset 0x5f71c0 contains 33 entries:
  Tag        Type                         Name/Value
......
 0x000000000000000c (INIT)               **0x13f8f8**

<img width="1917" height="711" alt="Image" src="https://github.com/user-attachments/assets/94b428ba-ea36-45ed-bae8-983228808eac" />

4.the following 2 crashes seems that BinaryFunctions  is not complete when deal with some address, can you tell me how can i fix these 2 problems?

---

### Comment 3 - zhouyu0502

i have some debug today and i find that  in function **discoverFileObjects()**
i add a log here 
<img width="3010" height="857" alt="Image" src="https://github.com/user-attachments/assets/9b3aaf4b-b598-4a0b-8326-e6e8d5650c62" />
result seems the two address(**0x13f904** and  **0x13f8f8** ) don't in **InputFile->symbols()** . i cant find how to parser symbols in the code.
can you tell me how to debug?


---

### Comment 4 - zhouyu0502

in summary, when parsing the input binary the addresses for the **.rela.init** and **dynamic section INIT**  are not being added to the **origin Symbols map** when exe **discoverFileObjects()**. I'd like to know how to debug this to identify the root cause.
thanks very much and hope any update from you, thanks very much!
hope to hear from you!

---

### Comment 5 - EugeneZelenko

@zhouyu0502: Please use text snippets and links to code instead of screenshots. Former are searchable.

---

### Comment 6 - zhouyu0502

> [@zhouyu0502](https://github.com/zhouyu0502): Please use text snippets and links to code instead of screenshots. Former are searchable.

i have edit again and use text snippets and links to code instead of screenshots. hope to hear from you.

---

### Comment 7 - zhouyu0502

tree -a
.
├── .debug
│   └── libduml_fastrtps.so
└── libduml_fastrtps.so

file .debug/libduml_fastrtps.so 
**ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked, with debug_info, not stripped**
readelf -S .debug/libduml_fastrtps.so | grep gnu_debuglink 

file libduml_fastrtps.so 
**ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked, not stripped**
readelf -S libduml_fastrtps.so | grep gnu_debuglink 
**[33] .gnu_debuglink    PROGBITS         0000000000000000  00602200**

i guess this may cause the crash , how do you think and how to fix this situation?

---

### Comment 8 - zhouyu0502

any update?


---

### Comment 9 - zhouyu0502

i have fixed this temporarily by add debug section,but i still want to know for input so,is debuginfo necessary or which part is necessary about debuginfo?

---

