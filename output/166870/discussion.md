# Firefox crashes when built with LLVM 21, due to llvm/llvm-project#129889

**Author:** steven-michaud
**URL:** https://github.com/llvm/llvm-project/issues/166870
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2025-11-11T15:44:56Z

## Body

I've already commented several times at https://github.com/llvm/llvm-project/pull/129889, so I'll keep my comments here brief.

As promised, I'm submitting copies (lightly edited) of two `lldb` sessions. One was made running a local trunk build of Firefox made with LLVM 21.1.6 (current code on the `release/21.x` branch). It shows the crash, and how it was caused by this commit. The second was made running a local trunk build of Firefox made with a patched LLVM 21.1.6 that disables copy propagation, using the first of the three patches [here](https://bugzilla.mozilla.org/show_bug.cgi?id=1995582#c20). Everything was done on an M1 Mac Mini running macOS 26.1.

[lldb-session-bad.txt](https://github.com/user-attachments/files/23404321/lldb-session-bad.txt)
[lldb-session-good.txt](https://github.com/user-attachments/files/23404322/lldb-session-good.txt)

The "bad" session crashes. The "good" session doesn't. The only relevant difference between the two builds is that in the "good" one, the `stp    x8, x9, [sp, #0x28]` instruction is preceded by the following two instructions, while in the bad one it isn't. In the "bad" build these two instructions were removed by the copy propagation commit.

```
mov    w9, w9
mov    w8, w8
``` 

The `x8` register ends up being used as `x` in the following code (located [here](https://searchfox.org/firefox-main/source/image/encoders/png/nsPNGEncoder.cpp#729) on the Firefox trunk). The `nsPNGEncoder::ConvertHostARGBRow()` method is inlined inside `nsPNGEncoder::AddImageFrame()`.

```
for (uint32_t x = 0; x < aPixelWidth; x++) {
  const uint32_t& pixelIn = ((const uint32_t*)(aSrc))[x];
  uint8_t* pixelOut = &aDest[x * pixelStride];

  uint8_t alpha = (pixelIn & 0xff000000) >> 24;
  pixelOut[pixelStride - 1] = alpha;  // overwritten below if pixelStride == 3

  ...

}
```

In the bad build `x8` has junk in its top 32 bits (`0x00000030`). In the good build it doesn't. The junk makes `x8` much larger than it should be, and causes a buffer overflow.


## Comments

### Comment 1 - steven-michaud

The Github UI doesn't allow you to view added files directly, so I'll paste them in here.

The "bad" session:

```
Process 97073 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x0000000115cded08 XUL`nsPNGEncoder::AddImageFrame(this=0x0000000140d101c0, aData="", aLength=<unavailable>, aWidth=48, aHeight=48, aStride=192, aInputFormat=2, aFrameOptions=0x000000016fdf99e0) at nsPNGEncoder.cpp:0:5 [opt]
   29  	      mIsAnimation(false),
   30  	      mFinished(false),
   31  	      mImageBuffer(nullptr),
-> 32  	      mImageBufferSize(0),
   33  	      mImageBufferUsed(0),
   34  	      mImageBufferHash(0),
   35  	      mImageBufferReadPoint(0),
Note: this address is compiler-generated code in function nsPNGEncoder::AddImageFrame(unsigned char const*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, nsTSubstring<char16_t> const&) that has no source code associated with it.
Target 0: (firefox) stopped.
warning: XUL was compiled with optimization - stepping may behave oddly; variables may not be available.
(lldb) disassemble -m -pc
XUL`nsPNGEncoder::AddImageFrame:
->  0x115cded08 <+680>: stp    x8, x9, [sp, #0x28]
    0x115cded0c <+684>: mov    w28, #0x0 ; =0 
    0x115cded10 <+688>: mov    x27, x23
    0x115cded14 <+692>: b      0x115cded38    ; <+728> at nsPNGEncoder.cpp:300:35
(lldb) register read x8 x9
      x8 = 0x0000000000000030
      x9 = 0x0000003000000030
(lldb) c
Process 97073 resuming
Process 97073 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 2.1
    frame #0: 0x0000000115cded44 XUL`nsPNGEncoder::AddImageFrame(this=0x0000000140d101c0, aData="", aLength=<unavailable>, aWidth=48, aHeight=48, aStride=192, aInputFormat=2, aFrameOptions=0x000000016fdf99e0) at nsPNGEncoder.cpp:0:35 [opt]
   29  	      mIsAnimation(false),
   30  	      mFinished(false),
   31  	      mImageBuffer(nullptr),
-> 32  	      mImageBufferSize(0),
   33  	      mImageBufferUsed(0),
   34  	      mImageBufferHash(0),
   35  	      mImageBufferReadPoint(0),
Note: this address is compiler-generated code in function nsPNGEncoder::AddImageFrame(unsigned char const*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, nsTSubstring<char16_t> const&) that has no source code associated with it.
Target 0: (firefox) stopped.
(lldb) disassemble -m -s 0x115cded3c
XUL`nsPNGEncoder::AddImageFrame:
    0x115cded3c <+732>: mov    w9, #0x0 ; =0 
    0x115cded40 <+736>: ldr    x8, [sp, #0x30]
->  0x115cded44 <+740>: b      0x115cded6c    ; <+780> [inlined] nsPNGEncoder::ConvertHostARGBRow(unsigned char const*, unsigned char*, unsigned int, bool) + 36 at nsPNGEncoder.cpp:731:26
    0x115cded48 <+744>: mov    x13, x12

   734 	    pixelOut[pixelStride - 1] = alpha;  // overwritten below if pixelStride == 3
   735 	    if (alpha == 255) {
** 736 	      pixelOut[0] = (pixelIn & 0xff0000) >> 16;

(lldb) register read x8
      x8 = 0x0000003000000030
(lldb) c
Process 97073 resuming
Process 97073 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 3.1
    frame #0: 0x0000000115cded68 XUL`nsPNGEncoder::ConvertHostARGBRow(this=0x0000000140d101c0, aSrc="", aDest="", aPixelWidth=<unavailable>, aUseTransparency=<unavailable>) at nsPNGEncoder.cpp:729:3 [opt] [inlined]
   726 	                                      uint32_t aPixelWidth,
   727 	                                      bool aUseTransparency) {
   728 	  uint32_t pixelStride = aUseTransparency ? 4 : 3;
-> 729 	  for (uint32_t x = 0; x < aPixelWidth; x++) {
   730 	    const uint32_t& pixelIn = ((const uint32_t*)(aSrc))[x];
   731 	    uint8_t* pixelOut = &aDest[x * pixelStride];
   732 	
Target 0: (firefox) stopped.
(lldb) disassemble -m -s 0x115cded60
XUL`nsPNGEncoder::ConvertHostARGBRow:
    0x115cded60 <+768>: add    w9, w9, w27
    0x115cded64 <+772>: subs   x8, x8, #0x1

-> 727 	                                      bool aUseTransparency) {
-> 728 	  uint32_t pixelStride = aUseTransparency ? 4 : 3;
-> 729 	  for (uint32_t x = 0; x < aPixelWidth; x++) {
-> 730 	    const uint32_t& pixelIn = ((const uint32_t*)(aSrc))[x];

->  0x115cded68 <+776>: b.eq   0x115cded18    ; <+696> at nsPNGEncoder.cpp:302:21

(lldb) register read x8
      x8 = 0x000000300000002f
(lldb) c
Process 97073 resuming
Process 97073 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=2, address=0x1402fc003)
    frame #0: 0x0000000115cded7c XUL`nsPNGEncoder::ConvertHostARGBRow(this=0x0000000140d101c0, aSrc="", aDest="", aPixelWidth=<unavailable>, aUseTransparency=<unavailable>) at nsPNGEncoder.cpp:734:31 [opt]
   731 	    uint8_t* pixelOut = &aDest[x * pixelStride];
   732 	
   733 	    uint8_t alpha = (pixelIn & 0xff000000) >> 24;
-> 734 	    pixelOut[pixelStride - 1] = alpha;  // overwritten below if pixelStride == 3
   735 	    if (alpha == 255) {
   736 	      pixelOut[0] = (pixelIn & 0xff0000) >> 16;
   737 	      pixelOut[1] = (pixelIn & 0x00ff00) >> 8;
Target 0: (firefox) stopped.
(lldb) register read x8
      x8 = 0x0000002fffffeb50
(lldb) kill
```

The "good" session:

```
Process 97182 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x000000015e6aed48 XUL`nsPNGEncoder::AddImageFrame(this=0x00000003091d7ac0, aData="", aLength=<unavailable>, aWidth=48, aHeight=48, aStride=192, aInputFormat=2, aFrameOptions=0x000000016fdf99b0) at nsPNGEncoder.cpp:0:5 [opt]
   29  	      mIsAnimation(false),
   30  	      mFinished(false),
   31  	      mImageBuffer(nullptr),
-> 32  	      mImageBufferSize(0),
   33  	      mImageBufferUsed(0),
   34  	      mImageBufferHash(0),
   35  	      mImageBufferReadPoint(0),
Note: this address is compiler-generated code in function nsPNGEncoder::AddImageFrame(unsigned char const*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, nsTSubstring<char16_t> const&) that has no source code associated with it.
Target 0: (firefox) stopped.
warning: XUL was compiled with optimization - stepping may behave oddly; variables may not be available.
(lldb) disassemble -m -pc
XUL`nsPNGEncoder::AddImageFrame:
->  0x15e6aed48 <+680>: mov    w9, w9

   297 	      return NS_ERROR_OUT_OF_MEMORY;
   298 	    }
** 299 	    for (uint32_t y = 0; y < aHeight; y++) {
   300 	      ConvertHostARGBRow(&aData[y * aStride], row.get(), aWidth,
   301 	                         useTransparency);

    0x15e6aed4c <+684>: mov    w8, w8
    0x15e6aed50 <+688>: stp    x8, x9, [sp, #0x28]
    0x15e6aed54 <+692>: mov    w28, #0x0 ; =0 
(lldb) register read x8 x9
      x8 = 0x0000000000000030
      x9 = 0x0000003000000030
(lldb) c
Process 97182 resuming
Process 97182 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 2.1
    frame #0: 0x000000015e6aed50 XUL`nsPNGEncoder::AddImageFrame(this=0x00000003091d7ac0, aData="", aLength=<unavailable>, aWidth=48, aHeight=48, aStride=192, aInputFormat=2, aFrameOptions=0x000000016fdf99b0) at nsPNGEncoder.cpp:299:28 [opt]
   296 	    if (NS_WARN_IF(!row)) {
   297 	      return NS_ERROR_OUT_OF_MEMORY;
   298 	    }
-> 299 	    for (uint32_t y = 0; y < aHeight; y++) {
   300 	      ConvertHostARGBRow(&aData[y * aStride], row.get(), aWidth,
   301 	                         useTransparency);
   302 	      png_write_row(mPNG, row.get());
Target 0: (firefox) stopped.
(lldb) disassemble -m -pc
XUL`nsPNGEncoder::AddImageFrame:
->  0x15e6aed50 <+688>: stp    x8, x9, [sp, #0x28]
    0x15e6aed54 <+692>: mov    w28, #0x0 ; =0 
    0x15e6aed58 <+696>: mov    x27, x23
    0x15e6aed5c <+700>: b      0x15e6aed80    ; <+736> at nsPNGEncoder.cpp:300:35
(lldb) register read x8 x9
      x8 = 0x0000000000000030
      x9 = 0x0000000000000030
(lldb) c
Process 97182 resuming
Process 97182 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 3.1
    frame #0: 0x000000015e6aed8c XUL`nsPNGEncoder::AddImageFrame(this=0x00000003091d7ac0, aData="", aLength=<unavailable>, aWidth=48, aHeight=48, aStride=192, aInputFormat=2, aFrameOptions=0x000000016fdf99b0) at nsPNGEncoder.cpp:0:35 [opt]
   29  	      mIsAnimation(false),
   30  	      mFinished(false),
   31  	      mImageBuffer(nullptr),
-> 32  	      mImageBufferSize(0),
   33  	      mImageBufferUsed(0),
   34  	      mImageBufferHash(0),
   35  	      mImageBufferReadPoint(0),
Note: this address is compiler-generated code in function nsPNGEncoder::AddImageFrame(unsigned char const*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, nsTSubstring<char16_t> const&) that has no source code associated with it.
Target 0: (firefox) stopped.
(lldb) disassemble -m -s 0x15e6aed84
XUL`nsPNGEncoder::AddImageFrame:
    0x15e6aed84 <+740>: mov    w9, #0x0 ; =0 
    0x15e6aed88 <+744>: ldr    x8, [sp, #0x30]
->  0x15e6aed8c <+748>: b      0x15e6aedb4    ; <+788> [inlined] nsPNGEncoder::ConvertHostARGBRow(unsigned char const*, unsigned char*, unsigned int, bool) + 36 at nsPNGEncoder.cpp:731:26
    0x15e6aed90 <+752>: mov    x13, x12

   734 	    pixelOut[pixelStride - 1] = alpha;  // overwritten below if pixelStride == 3
   735 	    if (alpha == 255) {
** 736 	      pixelOut[0] = (pixelIn & 0xff0000) >> 16;

(lldb) register read x8
      x8 = 0x0000000000000030
(lldb) c
Process 97182 resuming
Process 97182 stopped
* thread #1, name = 'MainThread', queue = 'com.apple.main-thread', stop reason = breakpoint 4.1
    frame #0: 0x000000015e6aedb0 XUL`nsPNGEncoder::ConvertHostARGBRow(this=0x00000003091d7ac0, aSrc="", aDest="", aPixelWidth=<unavailable>, aUseTransparency=<unavailable>) at nsPNGEncoder.cpp:729:3 [opt] [inlined]
   726 	                                      uint32_t aPixelWidth,
   727 	                                      bool aUseTransparency) {
   728 	  uint32_t pixelStride = aUseTransparency ? 4 : 3;
-> 729 	  for (uint32_t x = 0; x < aPixelWidth; x++) {
   730 	    const uint32_t& pixelIn = ((const uint32_t*)(aSrc))[x];
   731 	    uint8_t* pixelOut = &aDest[x * pixelStride];
   732 	
Target 0: (firefox) stopped.
(lldb) disassemble -m -s 0x15e6aeda8
XUL`nsPNGEncoder::ConvertHostARGBRow:
    0x15e6aeda8 <+776>: add    w9, w9, w27
    0x15e6aedac <+780>: subs   x8, x8, #0x1

-> 727 	                                      bool aUseTransparency) {
-> 728 	  uint32_t pixelStride = aUseTransparency ? 4 : 3;
-> 729 	  for (uint32_t x = 0; x < aPixelWidth; x++) {
-> 730 	    const uint32_t& pixelIn = ((const uint32_t*)(aSrc))[x];

->  0x15e6aedb0 <+784>: b.eq   0x15e6aed60    ; <+704> at nsPNGEncoder.cpp:302:21

(lldb) register read x8
      x8 = 0x000000000000002f
(lldb) br dis
```

---

### Comment 2 - steven-michaud

Here's the MIR code[?] corresponding to the two instructions removed from the "bad" build. It's part of the output from my debug logging patch (third of three patches [here](https://bugzilla.mozilla.org/show_bug.cgi?id=1995582#c20)).

```
renamable $w9 = ORRWrs $wzr, killed renamable $w9, 0
renamable $x9 = KILL killed renamable $w9
renamable $w8 = ORRWrs $wzr, killed renamable $w8, 0, debug-location !11642; image/encoders/png/nsPNGEncoder.cpp:299:28
renamable $x8 = KILL killed renamable $w8, debug-location !11642; image/encoders/png/nsPNGEncoder.cpp:299:28
STPXi killed renamable $x8, killed renamable $x9, $sp, 5 :: (store (s64) into %stack.20), (store (s64) into %stack.21)
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steven Michaud (steven-michaud)

<details>
I've already commented several times at https://github.com/llvm/llvm-project/pull/129889, so I'll keep my comments here brief.

As promised, I'm submitting copies (lightly edited) of two `lldb` sessions. One was made running a local trunk build of Firefox made with LLVM 21.6 (current code on the `release/21.x` branch). It shows the crash, and how it was caused by this commit. The second was made running a local trunk build of Firefox made with a patched LLVM 21.6 that disables copy propagation, using the first of the three patches [here](https://bugzilla.mozilla.org/show_bug.cgi?id=1995582#c20). Everything was done on an M1 Mac Mini running macOS 26.1.

[lldb-session-bad.txt](https://github.com/user-attachments/files/23404321/lldb-session-bad.txt)
[lldb-session-good.txt](https://github.com/user-attachments/files/23404322/lldb-session-good.txt)

The "bad" session crashes. The "good" session doesn't. The only relevant difference between the two builds is that in the "good" one, the `stp    x8, x9, [sp, #<!-- -->0x28]` instruction is preceded by the following two instructions, while in the bad one it isn't. In the "bad" build these two instructions were removed by the copy propagation commit.

```
mov    w9, w9
mov    w8, w8
``` 

The `x8` register ends up being used as `x` in the following code (located [here](https://searchfox.org/firefox-main/source/image/encoders/png/nsPNGEncoder.cpp#<!-- -->729) on the Firefox trunk). The `nsPNGEncoder::ConvertHostARGBRow()` method is inlined inside `nsPNGEncoder::AddImageFrame()`.

```
for (uint32_t x = 0; x &lt; aPixelWidth; x++) {
  const uint32_t&amp; pixelIn = ((const uint32_t*)(aSrc))[x];
  uint8_t* pixelOut = &amp;aDest[x * pixelStride];

  uint8_t alpha = (pixelIn &amp; 0xff000000) &gt;&gt; 24;
  pixelOut[pixelStride - 1] = alpha;  // overwritten below if pixelStride == 3

  ...

}
```

In the bad build `x8` has junk in its top 32 bits (`0x00000030`). In the good build it doesn't. The junk makes `x8` much larger than it should be, and causes a buffer overflow.

</details>


---

### Comment 4 - mstorsjo

So, @davemgreen did try to fix this in #137348, and that did fix the issues I had - but apparently that fix wasn't enough. Are you able to provide a standalone source file (preprocessed source, or IR with `-mllvm -disable-llvm-optzns` i presume) that can reproduce the object files where the essential `mov` instructions are removed?

---

### Comment 5 - davemgreen

Yeah many things could be going wrong. Are you able to provide the IR going into the backend? Either for this function or the whole module. -emit-llvm is usually enough if you are compiling with clang. If it is using LTO then it might need to use -save-temps, possibly as a LTO linker option.

---

### Comment 6 - davemgreen

Something where we can run `llc <file>` and see the problem would be ideal. godbolt.org can be a good way to show the issue. As in https://godbolt.org/z/f1so95E3c (which actually has the opposite problem - a few mov's that could be removed but we don't).

---

### Comment 7 - steven-michaud

Remember that I don't have a reduced testcase, and probably never will. So I don't have something I can run `llc` on or put in godbolt.

> Are you able to provide the IR going into the backend?

Can I add `-emit-llvm` to CFLAGS and/or CPPFLAGS for a Firefox build and know that it will "do the right thing"? Will it just produce the output you want without making any other changes?

A more promising approach may be to alter my debug logging patch (third patch of three [here](https://bugzilla.mozilla.org/show_bug.cgi?id=1995582#c20)). In fact it may already be doing what you want. Let me know.

Also, it'd help to know what "going into the backend" means.

---

### Comment 8 - topperc

> Remember that I don't have a reduced testcase, and probably never will. So I don't have something I can run `llc` on or put in godbolt.
> 
> > Are you able to provide the IR going into the backend?
> 
> Can I add `-emit-llvm` to CFLAGS and/or CPPFLAGS for a Firefox build and know that it will "do the right thing"? Will it just produce the output you want without making any other changes?
> 
> A more promising approach may be to alter my debug logging patch (third patch of three [here](https://bugzilla.mozilla.org/show_bug.cgi?id=1995582#c20)). In fact it may already be doing what you want. Let me know.
> 
> Also, it'd help to know what "going into the backend" means.

-emit-llvm will cause the `-o` output file to be written with LLVM bitcode instead of an ELF binary. Doing that in CFLAGS will probably cause the linker to fail when it gets unexpected input.

Can you find the command line for the affected file and manually run it with `-emit-llvm`? Probably want to change `-c` to `-S` so it will dump textual IR instead of bitcode.

---

### Comment 9 - topperc

I think I understand now. I think this comes down to the hacky way we model upper bits being zero with SUBREG_TO_REG.

I think there was a SUBREG_TO_REG that was replaced by KILL in ExpandPostRAPseudos. The ORRWrs instruction has no idea its upper bits are live. Only the SUBREG_TO_REG/KILL instruction knows that. It's unsafe to remove the ORRWs without knowing whether it is used by a KILL.

---

### Comment 10 - steven-michaud

I've revised my debug logging patch so that it logs the entire `nsPNGEncoder::AddImageFrame()` function both in AArch64-specific intermediate language and target-agnostic intermediate language. Here's the output. I *think* that's what you guys want. If it isn't, give me a very specific description of what it is you do want, and suggestions about how to revise my debug logging patch yet again to get it.

[nsPNGEncoder_AddImageFrame.txt](https://github.com/user-attachments/files/23427699/nsPNGEncoder_AddImageFrame.txt)

---

### Comment 11 - topperc

> I've revised my debug logging patch so that it logs the entire `nsPNGEncoder::AddImageFrame()` function both in AArch64-specific intermediate language and target-agnostic intermediate language. Here's the output. I _think_ that's what you guys want. If it isn't, give me a very specific description of what it is you do want, and suggestions about how to revise my debug logging patch yet again to get it.
> 
> [nsPNGEncoder_AddImageFrame.txt](https://github.com/user-attachments/files/23427699/nsPNGEncoder_AddImageFrame.txt)

We need a .ll file that we can be parsed by llc and that we can run our reducing tools on. Adding -emit-llvm -S to the clang command line will generate that.

---

### Comment 12 - steven-michaud

> > I've revised my debug logging patch so that it logs the entire `nsPNGEncoder::AddImageFrame()` function both in AArch64-specific intermediate language and target-agnostic intermediate language. Here's the output. I _think_ that's what you guys want. If it isn't, give me a very specific description of what it is you do want, and suggestions about how to revise my debug logging patch yet again to get it.
> > [nsPNGEncoder_AddImageFrame.txt](https://github.com/user-attachments/files/23427699/nsPNGEncoder_AddImageFrame.txt)
> 
> We need a .ll file that we can be parsed by llc and that we can run our reducing tools on. Adding -emit-llvm -S to the clang command line will generate that.

Once again, I don't have anything I can run `llc` on. If you want me to help you, you need to give me something I can do.

Do look through the entire file I just uploaded.

---

### Comment 13 - topperc

> > > I've revised my debug logging patch so that it logs the entire `nsPNGEncoder::AddImageFrame()` function both in AArch64-specific intermediate language and target-agnostic intermediate language. Here's the output. I _think_ that's what you guys want. If it isn't, give me a very specific description of what it is you do want, and suggestions about how to revise my debug logging patch yet again to get it.
> > > [nsPNGEncoder_AddImageFrame.txt](https://github.com/user-attachments/files/23427699/nsPNGEncoder_AddImageFrame.txt)
> > 
> > 
> > We need a .ll file that we can be parsed by llc and that we can run our reducing tools on. Adding -emit-llvm -S to the clang command line will generate that.
> 
> Once again, I don't have anything I can run `llc` on. If you want me to help you, you need to give me something I can do.
> 
> Do look through the entire file I just uploaded.

Can you isolate the clang command line for the individual C++ file and run it manually? Perhaps there's a verbose flag for the firefox build that will print all of the build commands?

If you can amend that command line with `-emit-llvm -S -o IR.ll` that will give us what we need. 

Or if you can run the file through the preprocessor and send us the preprocessed source file and the clang command line so we can run clang without needing the firefox and MacOS build environment that would also work.

---

### Comment 14 - steven-michaud

Look through the file I just uploaded. I think it contains (in part) what would normally go into an `.ll` file.

---

### Comment 15 - topperc

> Look through the file I just uploaded. I think it contains (in part) what would normally go into an `.ll` file.

I see that, but if it's not a complete IR file, I can't run the compiler on it to test fixes. But I will definitely try.

---

### Comment 16 - steven-michaud

All right then, give me some idea how to get the extra information in my debug logging patch. I'm about to post my revision.


---

### Comment 17 - topperc

> All right then, give me some idea how to get the extra information in my debug logging patch.

You can print the entire Module object that owns the Function.

---

### Comment 18 - steven-michaud

Here's my revised debug logging patch:

```
diff --git a/llvm/lib/CodeGen/MachineCopyPropagation.cpp b/llvm/lib/CodeGen/MachineCopyPropagation.cpp
index 742de1101faa..d741469fc0ef 100644
--- a/llvm/lib/CodeGen/MachineCopyPropagation.cpp
+++ b/llvm/lib/CodeGen/MachineCopyPropagation.cpp
@@ -450,10 +450,18 @@ public:
   }
 };
 
+// This debug logging uses https://github.com/steven-michaud/PySerialPortLogger.
+// Install it, then run Terminal and open three tabs. Then run serialportlogger
+// in its third tab.
+#define BUGZILLA_1995582_DEBUG_LOG 1
+
 class MachineCopyPropagation {
   const TargetRegisterInfo *TRI = nullptr;
   const TargetInstrInfo *TII = nullptr;
   const MachineRegisterInfo *MRI = nullptr;
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  const MachineFunction *MF_ = nullptr;
+#endif
 
   // Return true if this is a copy instruction and false otherwise.
   bool UseCopyInstr;
@@ -874,10 +882,39 @@ void MachineCopyPropagation::forwardUses(MachineInstr &MI) {
   }
 }
 
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+#include <stdarg.h>
+#include <stdio.h>
+#include <fcntl.h>
+#include <termios.h>
+
+#define VIRTUAL_SERIAL_PORT "/dev/ttys003"
+bool g_virtual_serial_checked = false;
+int g_virtual_serial = -1;
+std::unique_ptr<raw_fd_ostream> g_virtual_serial_stream;
+
+static void maybe_initialize_tty()
+{
+  if (!g_virtual_serial_checked) {
+    g_virtual_serial_checked = true;
+    g_virtual_serial =
+      open(VIRTUAL_SERIAL_PORT, O_WRONLY | O_NONBLOCK | O_NOCTTY);
+    if (g_virtual_serial >= 0) {
+      g_virtual_serial_stream =
+        std::make_unique<raw_fd_ostream>(g_virtual_serial, false, true);
+    }
+  }
+}
+#endif
+
 void MachineCopyPropagation::ForwardCopyPropagateBlock(MachineBasicBlock &MBB) {
   LLVM_DEBUG(dbgs() << "MCP: ForwardCopyPropagateBlock " << MBB.getName()
                     << "\n");
 
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  bool ErasedInstruction = false;
+#endif
+
   for (MachineInstr &MI : llvm::make_early_inc_range(MBB)) {
     // Analyze copies (which don't overlap themselves).
     std::optional<DestSourcePair> CopyOperands =
@@ -942,6 +979,14 @@ void MachineCopyPropagation::ForwardCopyPropagateBlock(MachineBasicBlock &MBB) {
       // are the same and are not referring to a reserved register). If so,
       // delete it.
       if (RegSrc == RegDef && !MRI->isReserved(RegSrc)) {
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+        ErasedInstruction = true;
+        maybe_initialize_tty();
+        *g_virtual_serial_stream << "******\n";
+        *g_virtual_serial_stream << "MachineInstr::eraseFromParent(UseCopyInstr " << UseCopyInstr << "):\n\n";
+        MI.print(*g_virtual_serial_stream);
+        *g_virtual_serial_stream << "\n";
+#endif
         MI.eraseFromParent();
         NumDeletes++;
         Changed = true;
@@ -1084,6 +1129,18 @@ void MachineCopyPropagation::ForwardCopyPropagateBlock(MachineBasicBlock &MBB) {
   MaybeDeadCopies.clear();
   CopyDbgUsers.clear();
   Tracker.clear();
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  if (ErasedInstruction) {
+    *g_virtual_serial_stream << "******\n";
+    tcdrain(g_virtual_serial);
+    MF_->print(*g_virtual_serial_stream);
+    tcdrain(g_virtual_serial);
+    *g_virtual_serial_stream << "******\n";
+    MF_->getFunction().print(*g_virtual_serial_stream);
+    tcdrain(g_virtual_serial);
+    *g_virtual_serial_stream << "******\n";
+  }
+#endif
 }
 
 static bool isBackwardPropagatableCopy(const DestSourcePair &CopyOperands,
@@ -1616,6 +1673,9 @@ bool MachineCopyPropagation::run(MachineFunction &MF) {
   TRI = MF.getSubtarget().getRegisterInfo();
   TII = MF.getSubtarget().getInstrInfo();
   MRI = &MF.getRegInfo();
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  MF_ = &MF;
+#endif
 
   for (MachineBasicBlock &MBB : MF) {
     if (isSpillageCopyElimEnabled)
```

I'll try also printing the module object.


---

### Comment 19 - steven-michaud

I'm having trouble printing the module object. It overwhelms my logging infrastructure. I'll try hacking my debug logging patch to only print the `nsPNGEncoder.cpp` module.


---

### Comment 20 - steven-michaud

Here's the `nsPNGEncoder.cpp` module. I hope it's what you guys need. If not, please tell me what more output you need from my debug logger, and I'll be working on it tomorrow.

[nsPNGEncoder_cpp_module.txt](https://github.com/user-attachments/files/23430045/nsPNGEncoder_cpp_module.txt)

---

### Comment 21 - steven-michaud

The previous `nsPNGEncoder.cpp` module I posted was printed using `Module::print(raw_ostream &OS, unsigned Indent, bool Dump)` with `Dump` set to `false`. I just tried it again with `Dump` set to `true`. Here's the result:

[nsPNGEncoder_cpp_module_full.txt](https://github.com/user-attachments/files/23433258/nsPNGEncoder_cpp_module_full.txt)

---

### Comment 22 - steven-michaud

Here's the latest version of my debug logging patch:

```
diff --git a/llvm/lib/CodeGen/MachineCopyPropagation.cpp b/llvm/lib/CodeGen/MachineCopyPropagation.cpp
index 742de1101faa..5dbb165ac0c6 100644
--- a/llvm/lib/CodeGen/MachineCopyPropagation.cpp
+++ b/llvm/lib/CodeGen/MachineCopyPropagation.cpp
@@ -450,17 +450,33 @@ public:
   }
 };
 
+// This debug logging uses https://github.com/steven-michaud/PySerialPortLogger.
+// Install it, then run Terminal and open three tabs. Then run serialportlogger
+// in its third tab. If LOG_WHOLE_MODULE is defined, also open a fourth tab and
+// run serialportlogger there.
+
+#define BUGZILLA_1995582_DEBUG_LOG 1
+#define LOG_WHOLE_MODULE "nsPNGEncoder.cpp"
+
 class MachineCopyPropagation {
   const TargetRegisterInfo *TRI = nullptr;
   const TargetInstrInfo *TII = nullptr;
   const MachineRegisterInfo *MRI = nullptr;
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  const MachineFunction *MF_ = nullptr;
+  bool ErasedInstruction;
+#endif
 
   // Return true if this is a copy instruction and false otherwise.
   bool UseCopyInstr;
 
 public:
   MachineCopyPropagation(bool CopyInstr = false)
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+      : ErasedInstruction(false), UseCopyInstr(CopyInstr || MCPUseCopyInstr) {}
+#else
       : UseCopyInstr(CopyInstr || MCPUseCopyInstr) {}
+#endif
 
   bool run(MachineFunction &MF);
 
@@ -874,6 +890,47 @@ void MachineCopyPropagation::forwardUses(MachineInstr &MI) {
   }
 }
 
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+#include <stdarg.h>
+#include <stdio.h>
+#include <fcntl.h>
+#include <termios.h>
+
+#include "llvm/IR/Module.h"
+
+#define VIRTUAL_SERIAL_PORT "/dev/ttys003"
+bool g_virtual_serial_checked = false;
+int g_virtual_serial = -1;
+std::unique_ptr<raw_fd_ostream> g_virtual_serial_stream;
+
+#ifdef LOG_WHOLE_MODULE
+#define VIRTUAL_SERIAL_PORT_ALT "/dev/ttys005"
+int g_virtual_serial_alt = -1;
+std::unique_ptr<raw_fd_ostream> g_virtual_serial_stream_alt;
+#endif
+
+static void maybe_initialize_tty()
+{
+  if (!g_virtual_serial_checked) {
+    g_virtual_serial_checked = true;
+    g_virtual_serial =
+      open(VIRTUAL_SERIAL_PORT, O_WRONLY | O_NONBLOCK | O_NOCTTY);
+    if (g_virtual_serial >= 0) {
+      g_virtual_serial_stream =
+        std::make_unique<raw_fd_ostream>(g_virtual_serial, false, true);
+    }
+#ifdef LOG_WHOLE_MODULE
+    g_virtual_serial_alt =
+      open(VIRTUAL_SERIAL_PORT_ALT, O_WRONLY | O_NONBLOCK | O_NOCTTY);
+    if (g_virtual_serial_alt >= 0) {
+      g_virtual_serial_stream_alt =
+        std::make_unique<raw_fd_ostream>(g_virtual_serial_alt, false, true);
+    }
+#endif
+  }
+}
+#endif
+
 void MachineCopyPropagation::ForwardCopyPropagateBlock(MachineBasicBlock &MBB) {
   LLVM_DEBUG(dbgs() << "MCP: ForwardCopyPropagateBlock " << MBB.getName()
                     << "\n");
@@ -942,6 +999,14 @@ void MachineCopyPropagation::ForwardCopyPropagateBlock(MachineBasicBlock &MBB) {
       // are the same and are not referring to a reserved register). If so,
       // delete it.
       if (RegSrc == RegDef && !MRI->isReserved(RegSrc)) {
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+        ErasedInstruction = true;
+        maybe_initialize_tty();
+        *g_virtual_serial_stream << "******\n";
+        *g_virtual_serial_stream << "MachineInstr::eraseFromParent(UseCopyInstr " << UseCopyInstr << "):\n\n";
+        MI.print(*g_virtual_serial_stream);
+        *g_virtual_serial_stream << "\n";
+#endif
         MI.eraseFromParent();
         NumDeletes++;
         Changed = true;
@@ -1616,14 +1681,51 @@ bool MachineCopyPropagation::run(MachineFunction &MF) {
   TRI = MF.getSubtarget().getRegisterInfo();
   TII = MF.getSubtarget().getInstrInfo();
   MRI = &MF.getRegInfo();
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  MF_ = &MF;
+  const BasicBlock *BB = nullptr;
+  ErasedInstruction = false;
+#endif
 
   for (MachineBasicBlock &MBB : MF) {
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+    if (!BB) {
+      BB = MBB.getBasicBlock();
+    }
+#endif
     if (isSpillageCopyElimEnabled)
       EliminateSpillageCopies(MBB);
     BackwardCopyPropagateBlock(MBB);
     ForwardCopyPropagateBlock(MBB);
   }
 
+#ifdef BUGZILLA_1995582_DEBUG_LOG
+  if (ErasedInstruction) {
+    *g_virtual_serial_stream << "******\n";
+    tcdrain(g_virtual_serial);
+    MF_->print(*g_virtual_serial_stream);
+    tcdrain(g_virtual_serial);
+    *g_virtual_serial_stream << "******\n";
+    MF_->getFunction().print(*g_virtual_serial_stream);
+    tcdrain(g_virtual_serial);
+    *g_virtual_serial_stream << "******\n";
+    const Module *mod = BB ? BB->getModule() : nullptr;
+    if (mod) {
+      *g_virtual_serial_stream << "Module::getName() "
+                               << mod->getName() << "\n";
+      *g_virtual_serial_stream << "******\n";
+#ifdef LOG_WHOLE_MODULE
+      StringRef module_name(LOG_WHOLE_MODULE);
+      if (mod->getName().contains(module_name)) {
+        tcdrain(g_virtual_serial_alt);
+        mod->print(*g_virtual_serial_stream_alt, 0, /* false */ true);
+        tcdrain(g_virtual_serial_alt);
+      }
+#endif
+    }
+  }
+#endif
+
   return Changed;
 }
 
```


---

### Comment 23 - topperc

> The previous `nsPNGEncoder.cpp` module I posted was printed using `Module::print(raw_ostream &OS, unsigned Indent, bool Dump)` with `Dump` set to `false`. I just tried it again with `Dump` set to `true`. Here's the result:
> 
> [nsPNGEncoder_cpp_module_full.txt](https://github.com/user-attachments/files/23433258/nsPNGEncoder_cpp_module_full.txt)

I'm a little confused. Isn't this the signature for Module::print found in Module.h?

```
  void print(raw_ostream &OS, AssemblyAnnotationWriter *AAW,                     
             bool ShouldPreserveUseListOrder = false,                            
             bool IsForDebug = false) const;
```

---

### Comment 24 - topperc

Am I able to parse the Module dump with llc. I'm working on reducing it now.

---

### Comment 25 - steven-michaud

> > The previous `nsPNGEncoder.cpp` module I posted was printed using `Module::print(raw_ostream &OS, unsigned Indent, bool Dump)` with `Dump` set to `false`. I just tried it again with `Dump` set to `true`. Here's the result:
> > [nsPNGEncoder_cpp_module_full.txt](https://github.com/user-attachments/files/23433258/nsPNGEncoder_cpp_module_full.txt)
> 
> I'm a little confused. Isn't this the signature for Module::print found in Module.h?
> 
> ```
>   void print(raw_ostream &OS, AssemblyAnnotationWriter *AAW,                     
>              bool ShouldPreserveUseListOrder = false,                            
>              bool IsForDebug = false) const;
> ```

Nope. Not at least in current code on the `release/21.x` branch. In fact it's not in any header file at all, though its definition *is* present in [`AsmWriter.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/llvm/lib/IR/AsmWriter.cpp#L5069). The one I used is present in both [`Module.h`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/include/clang/Basic/Module.h#L856) and [`Module.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/lib/Basic/Module.cpp#L463).

The documentation [here](https://llvm.org/doxygen/classllvm_1_1Module.html) seems to be a bit behind.

Edit: Oops, now I've found it [here](https://github.com/llvm/llvm-project/blob/release/21.x/llvm/include/llvm/IR/Module.h#L867C8-L867C14). But the one I used works just fine.

> I am able to parse the Module dump with llc. I'm working on reducing it now.

Glad to hear it!


---

### Comment 26 - asb

Many thanks for that module dump, this is a reduced test case made using it:

```
; ModuleID = '<stdin>'
source_filename = "<stdin>"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx10.15.0"

define i32 @ham(i32 %arg, i1 %arg1, i1 %arg2, ptr %arg3) {
bb:
  br i1 %arg1, label %bb6, label %bb4

bb4:                                              ; preds = %bb
  %load = load ptr, ptr null, align 8
  br i1 %arg2, label %bb6, label %bb5

bb5:                                              ; preds = %bb4
  %call = call i32 @zot() #0
  %zext = zext i32 %arg to i64
  br i1 %arg1, label %bb6, label %bb7

bb6:                                              ; preds = %bb5, %bb4, %bb
  ret i32 0

bb7:                                              ; preds = %bb8, %bb7, %bb5
  store i8 0, ptr %arg3, align 1
  %icmp = icmp eq i64 %zext, 0
  br i1 %icmp, label %bb8, label %bb7

bb8:                                              ; preds = %bb7
  call void @quux()
  br label %bb7
}

declare i32 @zot()

declare void @quux()

attributes #0 = { returns_twice }
```

---

### Comment 27 - steven-michaud

I've also been seeing crashes in the `sqlite3.c` module, in Firefox builds made using LLVM 21.1.6 (current `release/21.x` branch) without copy propagation disabled. They're not easily reproducible, so I won't be able to post `lldb` sessions. And I'm not entirely sure they're due to copy propagation. Two functions have been altered, but each has only one instruction removed, so I hope it won't be too difficult to find the problem[s], if they exist.

Here's the output of my debug logging patch for both `sqlite2.c` functions. Note that the AArch64-specific intermediate code still contains the lines that were removed.

[sqlite3_c_funtions.txt](https://github.com/user-attachments/files/23442120/sqlite3_c_funtions.txt)

Oddly, my patch logged *two* `sqlite3.c` modules, each slightly different. I'm pretty sure there's only one `sqlite3.c` file. Could the different modules represent different passes?

The Github UI just failed to upload the files. They're large -- about 50MB each. Here's a WeTransfer link instead. It will expire in seven days.

https://we.tl/t-aTCGcKGFC9


---

### Comment 28 - topperc

> Nope. Not at least in current code on the `release/21.x` branch. In fact it's not in any header file at all, though its definition _is_ present in [`AsmWriter.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/llvm/lib/IR/AsmWriter.cpp#L5069). The one I used is present in both [`Module.h`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/include/clang/Basic/Module.h#L856) and [`Module.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/lib/Basic/Module.cpp#L463).

That's the clang Module.cpp and Module.h, not the llvm files found in llvm/include/llvm/IR/Module.h and llvm/lib/IR/Module.cpp. Your code includes the correct file from LLVM. I think your code happened to work because 0 for the second argument is implicitly convertible to a `AssemblyAnnotationWriter*`.

---

### Comment 29 - topperc

> Many thanks for that module dump, this is a reduced test case made using it:
> 
> ```
> ; ModuleID = '<stdin>'
> source_filename = "<stdin>"
> target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
> target triple = "arm64-apple-macosx10.15.0"
> 
> define i32 @ham(i32 %arg, i1 %arg1, i1 %arg2, ptr %arg3) {
> bb:
>   br i1 %arg1, label %bb6, label %bb4
> 
> bb4:                                              ; preds = %bb
>   %load = load ptr, ptr null, align 8
>   br i1 %arg2, label %bb6, label %bb5
> 
> bb5:                                              ; preds = %bb4
>   %call = call i32 @zot() #0
>   %zext = zext i32 %arg to i64
>   br i1 %arg1, label %bb6, label %bb7
> 
> bb6:                                              ; preds = %bb5, %bb4, %bb
>   ret i32 0
> 
> bb7:                                              ; preds = %bb8, %bb7, %bb5
>   store i8 0, ptr %arg3, align 1
>   %icmp = icmp eq i64 %zext, 0
>   br i1 %icmp, label %bb8, label %bb7
> 
> bb8:                                              ; preds = %bb7
>   call void @quux()
>   br label %bb7
> }
> 
> declare i32 @zot()
> 
> declare void @quux()
> 
> attributes #0 = { returns_twice }
> ```

From what I've seen in the debug logs, there is not enough information present on the ORRXrs to know if the upper bits are used. We would need to trace the users which is hard to do from post RA. I think we need to revert the no-op removal on the llvm 21 branch and trunk while we look for a way to do this safely.

---

### Comment 30 - steven-michaud

> > Nope. Not at least in current code on the `release/21.x` branch. In fact it's not in any header file at all, though its definition _is_ present in [`AsmWriter.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/llvm/lib/IR/AsmWriter.cpp#L5069). The one I used is present in both [`Module.h`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/include/clang/Basic/Module.h#L856) and [`Module.cpp`](https://github.com/llvm/llvm-project/blob/release/21.x/clang/lib/Basic/Module.cpp#L463).
> 
> That's the clang Module.cpp and Module.h, not the llvm files found in llvm/include/llvm/IR/Module.h and llvm/lib/IR/Module.cpp. Your code includes the correct file from LLVM. I think your code happened to work because 0 for the second argument is implicitly convertible to a `AssemblyAnnotationWriter*`.

You're right. When I changed the second parameter from `0` to `1` I got the compiler error `cannot initialize a parameter of type 'AssemblyAnnotationWriter *' with an rvalue of type 'int'`.

---

