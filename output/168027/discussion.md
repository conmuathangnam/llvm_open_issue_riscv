# [BOLT][AARCH64] Bolt  crashed while instrumenting the aarch64 shared library with realocation

**Author:** wdxx
**URL:** https://github.com/llvm/llvm-project/issues/168027
**Status:** Closed
**Labels:** BOLT, crash
**Closed Date:** 2025-11-25T07:06:58Z

## Body

Hi,
When I was instrumenting [libUnreal.so](https://drive.google.com/file/d/1_YKEakw92BBOXe_2v5skIl_Jen_B358e/view?usp=drivesdk), the bolt tool crashed.It looks like get invalid symbol for some relocation in function `RewriteInstance::handleRelocation`.

```diff
diff --git a/bolt/lib/Rewrite/RewriteInstance.cpp b/bolt/lib/Rewrite/RewriteInstance.cpp
index 8d6731e7540a..4fce92b94e93 100644
--- a/bolt/lib/Rewrite/RewriteInstance.cpp
+++ b/bolt/lib/Rewrite/RewriteInstance.cpp
@@ -3027,6 +3027,12 @@ void RewriteInstance::handleRelocation(const SectionRef &RelocatedSection,
         ReferencedSymbol =
             BC->getOrCreateGlobalSymbol(SymbolAddress, "SYMBOLat");
       } else {
+       {
+          auto Itr = Rel.getSymbol();
+          if (Itr == InputFile->symbol_end()) {
+            BC->outs() << "Get Invalid Symbol?\n";
+          }
+        }
         SymbolRef Symbol = *Rel.getSymbol();
         const uint64_t SymbolSize =
             IsAArch64 ? 0 : ELFSymbolRef(Symbol).getSize();
```

### Bolt source code version
llvmorg-21.1.4
### command line
```bash
bin/llvm-bolt  -instrument libUnreal-rel.so  -o libUnreal-instr.so --instrumentation-file=/data/local/tmp/prof.fdata --instrumentation-sleep-time=3
```
### bolt log 
```
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: aarch64
BOLT-INFO: BOLT version: cdbf243f8669b241fca9682a0ebc3d62b4d27f7c
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x16400000, offset 0x16400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
BOLT-WARNING: ignoring data marker conflicting with function symbol _armv8_sha512_probe
Get Invalid Symbol?
Failure value returned from cantFail wrapped call
can't read an entry at 0x2628030: it goes past the end of the section (0x2628030)
UNREACHABLE executed at  /llvm/llvm-project/llvm/include/llvm/Support/Error.h:810!
 #0 0x000055cdd4f6c9cc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x000055cdd4f6ce49 PrintStackTraceSignalHandler(void*)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055cdd4f6a03d llvm::sys::RunSignalHandlers()  /llvm/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #3 0x000055cdd4f6c1f4 SignalHandler(int, siginfo_t*, void*)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x000014d25b24f520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000014d25b2a39fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000014d25b2a39fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000014d25b2a39fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000014d25b24f476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000014d25b2357f3 abort ./stdlib/abort.c:81:7
#10 0x000055cdd4e97b11 bindingsErrorHandler(void*, char const*, bool)  /llvm/llvm-project/llvm/lib/Support/ErrorHandling.cpp:253:55
#11 0x000055cdd43d59e2 unsigned int llvm::cantFail<unsigned int>(llvm::Expected<unsigned int>, char const*)  /llvm/llvm-project/llvm/include/llvm/Support/Error.h:812:1
#12 0x000055cdd50cc366 llvm::bolt::RewriteInstance::handleRelocation(llvm::object::SectionRef const&, llvm::object::RelocationRef const&)  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:3034:46
#13 0x000055cdd50c9df9 llvm::bolt::RewriteInstance::readRelocations(llvm::object::SectionRef const&)  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2677:55
#14 0x000055cdd50c8443 llvm::bolt::RewriteInstance::processRelocations()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2484:20
#15 0x000055cdd50c1a4f llvm::bolt::RewriteInstance::discoverFileObjects()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:1376:20
#16 0x000055cdd50bd6f2 llvm::bolt::RewriteInstance::run()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:740:32
#17 0x000055cdd3d22b8a main  /llvm/llvm-project/bolt/tools/driver/llvm-bolt.cpp:267:29
#18 0x000014d25b236d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#19 0x000014d25b236e40 call_init ./csu/../csu/libc-start.c:128:20
#20 0x000014d25b236e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#21 0x000055cdd3d215a5 _start (bin/llvm-bolt+0x2ac5a5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bolt

Author: None (wdxx)

<details>
Hi,
When I was instrumenting [libUnreal.so](https://drive.google.com/file/d/1_YKEakw92BBOXe_2v5skIl_Jen_B358e/view?usp=drivesdk), the bolt tool crashed.It looks like get invalid symbol for some relocation in function `RewriteInstance::handleRelocation`.

```diff
diff --git a/bolt/lib/Rewrite/RewriteInstance.cpp b/bolt/lib/Rewrite/RewriteInstance.cpp
index 8d6731e7540a..4fce92b94e93 100644
--- a/bolt/lib/Rewrite/RewriteInstance.cpp
+++ b/bolt/lib/Rewrite/RewriteInstance.cpp
@@ -3027,6 +3027,12 @@ void RewriteInstance::handleRelocation(const SectionRef &amp;RelocatedSection,
         ReferencedSymbol =
             BC-&gt;getOrCreateGlobalSymbol(SymbolAddress, "SYMBOLat");
       } else {
+       {
+          auto Itr = Rel.getSymbol();
+          if (Itr == InputFile-&gt;symbol_end()) {
+            BC-&gt;outs() &lt;&lt; "Get Invalid Symbol?\n";
+          }
+        }
         SymbolRef Symbol = *Rel.getSymbol();
         const uint64_t SymbolSize =
             IsAArch64 ? 0 : ELFSymbolRef(Symbol).getSize();
```

### Bolt source code version
llvmorg-21.1.4
### command line
```bash
bin/llvm-bolt  -instrument libUnreal-rel.so  -o libUnreal-instr.so --instrumentation-file=/data/local/tmp/prof.fdata --instrumentation-sleep-time=3
```
### bolt log 
```
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: aarch64
BOLT-INFO: BOLT version: cdbf243f8669b241fca9682a0ebc3d62b4d27f7c
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x16400000, offset 0x16400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
BOLT-WARNING: ignoring data marker conflicting with function symbol _armv8_sha512_probe
Get Invalid Symbol?
Failure value returned from cantFail wrapped call
can't read an entry at 0x2628030: it goes past the end of the section (0x2628030)
UNREACHABLE executed at  /llvm/llvm-project/llvm/include/llvm/Support/Error.h:810!
 #<!-- -->0 0x000055cdd4f6c9cc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x000055cdd4f6ce49 PrintStackTraceSignalHandler(void*)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055cdd4f6a03d llvm::sys::RunSignalHandlers()  /llvm/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->3 0x000055cdd4f6c1f4 SignalHandler(int, siginfo_t*, void*)  /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->4 0x000014d25b24f520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000014d25b2a39fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000014d25b2a39fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000014d25b2a39fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000014d25b24f476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000014d25b2357f3 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000055cdd4e97b11 bindingsErrorHandler(void*, char const*, bool)  /llvm/llvm-project/llvm/lib/Support/ErrorHandling.cpp:253:55
#<!-- -->11 0x000055cdd43d59e2 unsigned int llvm::cantFail&lt;unsigned int&gt;(llvm::Expected&lt;unsigned int&gt;, char const*)  /llvm/llvm-project/llvm/include/llvm/Support/Error.h:812:1
#<!-- -->12 0x000055cdd50cc366 llvm::bolt::RewriteInstance::handleRelocation(llvm::object::SectionRef const&amp;, llvm::object::RelocationRef const&amp;)  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:3034:46
#<!-- -->13 0x000055cdd50c9df9 llvm::bolt::RewriteInstance::readRelocations(llvm::object::SectionRef const&amp;)  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2677:55
#<!-- -->14 0x000055cdd50c8443 llvm::bolt::RewriteInstance::processRelocations()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:2484:20
#<!-- -->15 0x000055cdd50c1a4f llvm::bolt::RewriteInstance::discoverFileObjects()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:1376:20
#<!-- -->16 0x000055cdd50bd6f2 llvm::bolt::RewriteInstance::run()  /llvm/llvm-project/bolt/lib/Rewrite/RewriteInstance.cpp:740:32
#<!-- -->17 0x000055cdd3d22b8a main  /llvm/llvm-project/bolt/tools/driver/llvm-bolt.cpp:267:29
#<!-- -->18 0x000014d25b236d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->19 0x000014d25b236e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->20 0x000014d25b236e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->21 0x000055cdd3d215a5 _start (bin/llvm-bolt+0x2ac5a5)
```
</details>


---

### Comment 2 - paschalis-mpeis

Thanks for reporting this @wdxx. Can you print the disassembly (-print-disasm) of the function where the crash happens?
I wonder whether this earlier message is related:
```
BOLT-WARNING: ignoring data marker conflicting with function symbol _armv8_sha512_probe
```

---

### Comment 3 - yavtuk

@maksfb probably it's due to the recent fix related to entry point offset

---

### Comment 4 - yavtuk

I faced with _armv8_sha512_probe function for openssl, need to check instructions inside because there should be entry offset != 0

@wdxx do you use openssl lib? If yes which one version

---

### Comment 5 - wdxx

> Thanks for reporting this [@wdxx](https://github.com/wdxx). Can you print the disassembly (-print-disasm) of the function where the crash happens? I wonder whether this earlier message is related:
> 
> ```
> BOLT-WARNING: ignoring data marker conflicting with function symbol _armv8_sha512_probe
> ```

Thanks very much for your  reply @paschalis-mpeis. Below is some information about the function when the crash occurred.
#### relocation info:
```
BOLT-DEBUG: Relocation: offset = 0xa9fe14c; type = R_AARCH64_ADD_ABS_LO12_NC; value = 0xb98; symbol = RELSYMat0xa80 (.dynsym); symbol address = 0xa80; addend = 0x118; address = 0xb98; in = _ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv
```
#### bolt disassembly 
```
Binary Function "_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv" debug in handleRelocation {
  Number      : 191
  State       : empty
  Address     : 0xa9fe11c
  Size        : 0x120
  MaxSize     : 0x120
  Offset      : 0x0
  Section     : .text
  Orc Section : .local.text._ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv
  LSDA        : 0x0
  IsSimple    : 1
  IsMultiEntry: 0
  IsSplit     : 0
  BB Count    : 0
}
DWARF CFI Instructions:
    <empty>
End of Function "_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv"
```
#### llvm disassembly 
```assembly
000000000a9fe11c <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv>:
 a9fe11c: a9bf7bfd     	stp	x29, x30, [sp, #-0x10]!
 a9fe120: 910003fd     	mov	x29, sp
 a9fe124: 90059b28     	adrp	x8, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe128: f9406908     	ldr	x8, [x8, #0xd0]
 a9fe12c: 08dffd08     	ldarb	w8, [x8]
 a9fe130: 36000168     	tbz	w8, #0x0, 0xa9fe15c <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv+0x40>
 a9fe134: 90059b28     	adrp	x8, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe138: f9406d08     	ldr	x8, [x8, #0xd8]
 a9fe13c: 08dffd08     	ldarb	w8, [x8]
 a9fe140: 36000408     	tbz	w8, #0x0, 0xa9fe1c0 <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv+0xa4>
 a9fe144: 90059b21     	adrp	x1, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe148: f005a600     	adrp	x0, 0x15ec1000 <Curl_cft_h2_proxy>
 a9fe14c: 912e6000     	add	x0, x0, #0xb98
 a9fe150: f9407021     	ldr	x1, [x1, #0xe0]
 a9fe154: a8c17bfd     	ldp	x29, x30, [sp], #0x10
 a9fe158: 17ffec63     	b	0xa9f92e4 <_ZN2UE5Trace7Private10FEventNode10InitializeEPKNS1_10FEventInfoE>
 a9fe15c: 90059b20     	adrp	x0, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe160: f9406800     	ldr	x0, [x0, #0xd0]
 a9fe164: 95fef0f5     	bl	0x129ba538 <_ZN5Eigen8SparseQRINS_12SparseMatrixIdLi0EiEENS_14COLAMDOrderingIiEEE14analyzePatternERKS2_+0xd50>
 a9fe168: 34fffe60     	cbz	w0, 0xa9fe134 <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv+0x18>
 a9fe16c: 90059b28     	adrp	x8, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe170: f0ffa809     	adrp	x9, 0x9f01000 <.L.str.381+0x15017>
 a9fe174: 52a0010a     	mov	w10, #0x80000           // =524288
 a9fe178: 9124dd29     	add	x9, x9, #0x937
 a9fe17c: 528061ab     	mov	w11, #0x30d             // =781
 a9fe180: 90059b20     	adrp	x0, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe184: f9407508     	ldr	x8, [x8, #0xe8]
 a9fe188: b900090a     	str	w10, [x8, #0x8]
 a9fe18c: f0ffa60a     	adrp	x10, 0x9ec1000 <.L.str.161+0x5366>
 a9fe190: f9000109     	str	x9, [x8]
 a9fe194: 52800109     	mov	w9, #0x8                // =8
 a9fe198: 913fdd4a     	add	x10, x10, #0xff7
 a9fe19c: 7900190b     	strh	w11, [x8, #0xc]
 a9fe1a0: b900111f     	str	wzr, [x8, #0x10]
 a9fe1a4: b9002109     	str	w9, [x8, #0x20]
 a9fe1a8: f9000d0a     	str	x10, [x8, #0x18]
 a9fe1ac: 7900491f     	strh	wzr, [x8, #0x24]
 a9fe1b0: b900291f     	str	wzr, [x8, #0x28]
 a9fe1b4: f9406800     	ldr	x0, [x0, #0xd0]
 a9fe1b8: 95fef0e3     	bl	0x129ba544 <_ZN5Eigen8SparseQRINS_12SparseMatrixIdLi0EiEENS_14COLAMDOrderingIiEEE14analyzePatternERKS2_+0xd5c>
 a9fe1bc: 17ffffde     	b	0xa9fe134 <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv+0x18>
 a9fe1c0: 90059b20     	adrp	x0, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe1c4: f9406c00     	ldr	x0, [x0, #0xd8]
 a9fe1c8: 95fef0dc     	bl	0x129ba538 <_ZN5Eigen8SparseQRINS_12SparseMatrixIdLi0EiEENS_14COLAMDOrderingIiEEE14analyzePatternERKS2_+0xd50>
 a9fe1cc: 34fffbc0     	cbz	w0, 0xa9fe144 <_ZZN2UE5Trace7Private25F$TraceThreadTimingFields10InitializeEvENKUlvE_clEv+0x28>
 a9fe1d0: 90059b28     	adrp	x8, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe1d4: 528000ca     	mov	w10, #0x6               // =6
 a9fe1d8: f0ff9e09     	adrp	x9, 0x9dc1000 <.L.str.136+0x5805>
 a9fe1dc: d0ff93eb     	adrp	x11, 0x9c7c000 <.L.str.840+0xb2>
 a9fe1e0: 91151129     	add	x9, x9, #0x544
 a9fe1e4: 91296d6b     	add	x11, x11, #0xa5b
 a9fe1e8: f9407108     	ldr	x8, [x8, #0xe0]
 a9fe1ec: 90059b20     	adrp	x0, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe1f0: 3900210a     	strb	w10, [x8, #0x8]
 a9fe1f4: 90059b2a     	adrp	x10, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe1f8: f9000109     	str	x9, [x8]
 a9fe1fc: 52800189     	mov	w9, #0xc                // =12
 a9fe200: f940754a     	ldr	x10, [x10, #0xe8]
 a9fe204: f900090b     	str	x11, [x8, #0x10]
 a9fe208: 5280002b     	mov	w11, #0x1               // =1
 a9fe20c: 39006109     	strb	w9, [x8, #0x18]
 a9fe210: 72a0008b     	movk	w11, #0x4, lsl #16
 a9fe214: f900110a     	str	x10, [x8, #0x20]
 a9fe218: b900290b     	str	w11, [x8, #0x28]
 a9fe21c: f9406c00     	ldr	x0, [x0, #0xd8]
 a9fe220: 95fef0c9     	bl	0x129ba544 <_ZN5Eigen8SparseQRINS_12SparseMatrixIdLi0EiEENS_14COLAMDOrderingIiEEE14analyzePatternERKS2_+0xd5c>
 a9fe224: 90059b21     	adrp	x1, 0x15d62000 <_binary_classes_dex_size+0x15d5fd94>
 a9fe228: f005a600     	adrp	x0, 0x15ec1000 <Curl_cft_h2_proxy>
 a9fe22c: 912e6000     	add	x0, x0, #0xb98
 a9fe230: f9407021     	ldr	x1, [x1, #0xe0]
 a9fe234: a8c17bfd     	ldp	x29, x30, [sp], #0x10
 a9fe238: 17ffec2b     	b	0xa9f92e4 <_ZN2UE5Trace7Private10FEventNode10InitializeEPKNS1_10FEventInfoE>
```

---

### Comment 6 - wdxx

> I faced with _armv8_sha512_probe function for openssl, need to check instructions inside because there should be entry offset != 0
> 
> [@wdxx](https://github.com/wdxx) do you use openssl lib? If yes which one version

Hi @yavtuk, thanks for your reply. We used openssl version `OpenSSL 1.1.1t  7 Feb 2023`.

---

### Comment 7 - yavtuk

@wdxx hello, can you share some details about compiler, compiler options, linker (options, version),
the relocations for:
 a9fe148: f005a600     	adrp	x0, 0x15ec1000 <Curl_cft_h2_proxy>
 a9fe14c: 912e6000     	add	x0, x0, #0xb98

000000000a9fe148  0000000000000113 R_AARCH64_ADR_PREL_PG_HI21                    118
000000000a9fe14c  0000000000000115 R_AARCH64_ADD_ABS_LO12_NC                    118

this is PC-relative relocation to get an adress for symbol, 

relocation has 3 fields
u64 offset
u64 symIdx, relType
u64 value

index in symbol table is 0, it seems that it's incorrect value

if you have source code you can check which one symbol is used inside this function

if not you can try external linker to check the value

---

### Comment 8 - wdxx

> [@wdxx](https://github.com/wdxx) hello, can you share some details about compiler, compiler options, linker (options, version), the relocations for: a9fe148: f005a600 adrp x0, 0x15ec1000 <Curl_cft_h2_proxy> a9fe14c: 912e6000 add x0, x0, #0xb98
> 
> 000000000a9fe148 0000000000000113 R_AARCH64_ADR_PREL_PG_HI21 118 000000000a9fe14c 0000000000000115 R_AARCH64_ADD_ABS_LO12_NC 118
> 
> this is PC-relative relocation to get an adress for symbol,
> 
> relocation has 3 fields u64 offset u64 symIdx, relType u64 value
> 
> index in symbol table is 0, it seems that it's incorrect value
> 
> if you have source code you can check which one symbol is used inside this function
> 
> if not you can try external linker to check the value

Hi @yavtuk , thanks very much for your help. It does indeed appear to be a elf relocation information error. I will check the compiler and linker options.

---

