# `lldb` crashes on break via Vulkan Validation

**Author:** FrostKiwi
**URL:** https://github.com/llvm/llvm-project/issues/186948
**Status:** Open
**Labels:** lldb, platform:windows, crash

## Body

One of the awesome Vulkan features for devs are the Validation layers. When the Validation detects Vulkan being addressed incorrectly, there is an option to not only report it, but break the code when the violation happens, so you know which line is responsible.
`lldb.exe` crashes when this "Vulkan Induced" break occurs with the message below. (`diagnostics.log` remains empty)

```
❯ lldb.exe .\volumeVulkan.exe
(lldb) target create ".\\volumeVulkan.exe"
Current executable set to 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64).
(lldb) run
Process 23280 launched: 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64)
(lldb) PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
LLDB diagnostics will be written to C:\Users\artsimow\AppData\Local\Temp\diagnostics-152eb7
Please include the directory content when filing a bug report
Exception Code: 0xC0000005
 #0 0x00007ffcd59946ac llvm::codeview::LazyRandomTypeCollection::getType(llvm::codeview::TypeIndex) (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\libLLVM-22.dll+0x27446ac)
 #1 0x00007ffcd2d5a323 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4a323)
 #2 0x00007ffcd2d67c3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa57c3e)
 #3 0x00007ffcd2d58f3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48f3e)
 #4 0x00007ffcd2d58540 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48540)
 #5 0x00007ffcd2d4e617 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa3e617)
 #6 0x00007ffcd2d68bd2 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa58bd2)
 #7 0x00007ffcd2d58f3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48f3e)
 #8 0x00007ffcd2d5b0cb PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4b0cb)
 #9 0x00007ffcd26b1ddd PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3a1ddd)
#10 0x00007ffcd2d5649e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4649e)
#11 0x00007ffcd261b368 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x30b368)
#12 0x00007ffcd2623a14 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x313a14)
#13 0x00007ffcd2b98d95 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x888d95)
#14 0x00007ffcd2b9726d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x88726d)
#15 0x00007ffcd2b5b82f PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84b82f)
#16 0x00007ffcd2b5b2e8 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84b2e8)
#17 0x00007ffcd2b5afc2 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84afc2)
#18 0x00007ffcd2b5bf7d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84bf7d)
#19 0x00007ffcd26e6a27 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d6a27)
#20 0x00007ffcd26e8c23 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d8c23)
#21 0x00007ffcd26e8fc4 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d8fc4)
#22 0x00007ffcd26e9b17 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d9b17)
#23 0x00007ffcd26c5a1f PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3b5a1f)
#24 0x00007ffcd28dc079 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x5cc079)
#25 0x00007ffcd273bf1d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x42bf1d)
#26 0x00007ffcd274e57d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x43e57d)
#27 0x00007ffcd2db4690 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xaa4690)
#28 0x00007ffdb48137b0 (C:\WINDOWS\System32\ucrtbase.dll+0x37b0)
#29 0x00007ffdb581e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#30 0x00007ffdb6c4c48c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c48c)
```

Windows 11, LLVM Toolchain and debuggers installed via https://scoop.sh/#/apps?q=mingw-mstorsjo-llvm-ucrt&p=1&id=4e35bf42629409cf7722249d1b574c84d2d19675
```shell
❯ lldb --version
lldb version 22.1.1 (https://github.com/llvm/llvm-project.git revision fef02d48c08db859ef83f84232ed78bd9d1c323a)
  clang revision fef02d48c08db859ef83f84232ed78bd9d1c323a
  llvm revision fef02d48c08db859ef83f84232ed78bd9d1c323a
```

## Image of break on Validation Error in the Vulkan Configurator
<img width="1026" height="752" alt="Image" src="https://github.com/user-attachments/assets/10637207-a20f-499c-acd9-596d7a7d0d8e" />

## Freeze of Windows Terminal with lldb-dap
Possibly related is that using Neovim and debugging via `lldb-dap` causes a freeze instead of a crash. Bizarrely, it's also the Windows Terminal freezing, where Neovim is that started the `lldb-dap`. The whole thing, all tabs get a full freeze:

<img width="1832" height="1054" alt="Image" src="https://github.com/user-attachments/assets/0d609b36-3b24-413d-aef9-da843372081a" />

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Wladislav Artsimovich (FrostKiwi)

<details>
One of the awesome Vulkan features for devs are the Validation layers. When the Validation detects Vulkan being addressed incorrectly, there is an option to not only report it, but break the code when the violation happens, so you know which line is responsible.
`lldb.exe` crashes when this "Vulkan Induced" break occurs with the message below. (`diagnostics.log` remains empty)

```
❯ lldb.exe .\volumeVulkan.exe
(lldb) target create ".\\volumeVulkan.exe"
Current executable set to 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64).
(lldb) run
Process 23280 launched: 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64)
(lldb) PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
LLDB diagnostics will be written to C:\Users\artsimow\AppData\Local\Temp\diagnostics-152eb7
Please include the directory content when filing a bug report
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ffcd59946ac llvm::codeview::LazyRandomTypeCollection::getType(llvm::codeview::TypeIndex) (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\libLLVM-22.dll+0x27446ac)
 #<!-- -->1 0x00007ffcd2d5a323 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4a323)
 #<!-- -->2 0x00007ffcd2d67c3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa57c3e)
 #<!-- -->3 0x00007ffcd2d58f3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48f3e)
 #<!-- -->4 0x00007ffcd2d58540 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48540)
 #<!-- -->5 0x00007ffcd2d4e617 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa3e617)
 #<!-- -->6 0x00007ffcd2d68bd2 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa58bd2)
 #<!-- -->7 0x00007ffcd2d58f3e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa48f3e)
 #<!-- -->8 0x00007ffcd2d5b0cb PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4b0cb)
 #<!-- -->9 0x00007ffcd26b1ddd PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3a1ddd)
#<!-- -->10 0x00007ffcd2d5649e PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xa4649e)
#<!-- -->11 0x00007ffcd261b368 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x30b368)
#<!-- -->12 0x00007ffcd2623a14 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x313a14)
#<!-- -->13 0x00007ffcd2b98d95 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x888d95)
#<!-- -->14 0x00007ffcd2b9726d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x88726d)
#<!-- -->15 0x00007ffcd2b5b82f PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84b82f)
#<!-- -->16 0x00007ffcd2b5b2e8 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84b2e8)
#<!-- -->17 0x00007ffcd2b5afc2 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84afc2)
#<!-- -->18 0x00007ffcd2b5bf7d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x84bf7d)
#<!-- -->19 0x00007ffcd26e6a27 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d6a27)
#<!-- -->20 0x00007ffcd26e8c23 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d8c23)
#<!-- -->21 0x00007ffcd26e8fc4 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d8fc4)
#<!-- -->22 0x00007ffcd26e9b17 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3d9b17)
#<!-- -->23 0x00007ffcd26c5a1f PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x3b5a1f)
#<!-- -->24 0x00007ffcd28dc079 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x5cc079)
#<!-- -->25 0x00007ffcd273bf1d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x42bf1d)
#<!-- -->26 0x00007ffcd274e57d PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0x43e57d)
#<!-- -->27 0x00007ffcd2db4690 PyInit__lldb (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\liblldb.dll+0xaa4690)
#<!-- -->28 0x00007ffdb48137b0 (C:\WINDOWS\System32\ucrtbase.dll+0x37b0)
#<!-- -->29 0x00007ffdb581e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->30 0x00007ffdb6c4c48c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c48c)
```

Windows 11, LLVM Toolchain and debuggers installed via https://scoop.sh/#/apps?q=mingw-mstorsjo-llvm-ucrt&amp;p=1&amp;id=4e35bf42629409cf7722249d1b574c84d2d19675
```shell
❯ lldb --version
lldb version 22.1.1 (https://github.com/llvm/llvm-project.git revision fef02d48c08db859ef83f84232ed78bd9d1c323a)
  clang revision fef02d48c08db859ef83f84232ed78bd9d1c323a
  llvm revision fef02d48c08db859ef83f84232ed78bd9d1c323a
```

## Image of break on Validation Error in the Vulkan Configurator
&lt;img width="1026" height="752" alt="Image" src="https://github.com/user-attachments/assets/10637207-a20f-499c-acd9-596d7a7d0d8e" /&gt;

## Freeze of Windows Terminal with lldb-dap
Possibly related is that using Neovim and debugging via `lldb-dap` causes a freeze instead of a crash. Bizarrely, it's also the Windows Terminal freezing, where Neovim is that started the `lldb-dap`. The whole thing, all tabs get a full freeze:

&lt;img width="1832" height="1054" alt="Image" src="https://github.com/user-attachments/assets/0d609b36-3b24-413d-aef9-da843372081a" /&gt;
</details>


---

### Comment 2 - FrostKiwi

I tried out without mingw in the picture, same result.
Installed toolchain via https://scoop.sh/#/apps?q=llvm&id=4826099b45a26b97e237fdd6233f58c48358dff2 which uses MSVC Libs to do the builds.
```
lldb --version
lldb version 22.1.0 (https://github.com/llvm/llvm-project revision 4434dabb69916856b824f68a64b029c67175e532)
  clang revision 4434dabb69916856b824f68a64b029c67175e532
  llvm revision 4434dabb69916856b824f68a64b029c67175e532
```

Same crashing.
```
lldb .\volumeVulkan.exe
(lldb) target create ".\\volumeVulkan.exe"
Current executable set to 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64).
(lldb) run
Process 16012 launched: 'C:\Users\artsimow\volumeVulkan\build\volumeVulkan.exe' (x86_64)
(lldb) LLDB diagnostics will be written to C:\Users\artsimow\AppData\Local\Temp\diagnostics-40e82d
Please include the directory content when filing a bug report
Exception Code: 0xC0000005
 #0 0x00007fff69854b50 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1b64b50)
 #1 0x00007fff69028312 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1338312)
 #2 0x00007fff6903534c PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x134534c)
 #3 0x00007fff69027590 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1337590)
 #4 0x00007fff69026d55 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1336d55)
 #5 0x00007fff6883ef7c PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xb4ef7c)
 #6 0x00007fff6901d4a3 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x132d4a3)
 #7 0x00007fff69036cb2 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1346cb2)
 #8 0x00007fff69027590 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1337590)
 #9 0x00007fff690293c0 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x13393c0)
#10 0x00007fff6884f39d PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xb5f39d)
#11 0x00007fff69025722 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1335722)
#12 0x00007fff687dc54d PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xaec54d)
#13 0x00007fff6877ae5f PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xa8ae5f)
#14 0x00007fff6877ad6f PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xa8ad6f)
#15 0x00007fff68e136cf PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x11236cf)
#16 0x00007fff68e12500 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x1122500)
#17 0x00007fff68ddc066 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x10ec066)
#18 0x00007fff68ddbab8 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x10ebab8)
#19 0x00007fff68ddbe3d PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x10ebe3d)
#20 0x00007fff68ddb8fc PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x10eb8fc)
#21 0x00007fff68ddc75d PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x10ec75d)
#22 0x00007fff688b4bcf PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbc4bcf)
#23 0x00007fff688b7284 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbc7284)
#24 0x00007fff688b811f PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbc811f)
#25 0x00007fff688dbb13 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbebb13)
#26 0x00007fff68a6131a PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xd7131a)
#27 0x00007fff688a4f17 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbb4f17)
#28 0x00007fff6889ff30 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbaff30)
#29 0x00007fff688a61aa PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbb61aa)
#30 0x00007fff688b2c73 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0xbc2c73)
#31 0x00007fff694ace66 PyInit__lldb (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x17bce66)
#32 0x00007fff6a4fb0af xmlLinkGetData (C:\Users\artsimow\scoop\apps\llvm\current\bin\liblldb.dll+0x280b0af)
#33 0x00007ff84dc5e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#34 0x00007ff84f14c48c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c48c)
```

---

### Comment 3 - DavidSpickett

I assume this is done by embedding a break instruction into the binary, which we should be able to handle. We sometimes have problems stepping off of them, but stopping and inspecting should work.

Can you tell us how to build the Vulkan side of this? Does it require a supported GPU? (I've no clue about how graphics stacks work)

If you happen to have access to Linux, please try that too.

>  #0 0x00007ffcd59946ac llvm::codeview::LazyRandomTypeCollection::getType(llvm::codeview::TypeIndex) (C:\Users\artsimow\scoop\apps\mingw-mstorsjo-llvm-ucrt\current\bin\libLLVM-22.dll+0x27446ac)

Suggests that we're trying to show some variable in the scope where we stop. Making this a Windows/PDB specific issue.

---

### Comment 4 - FrostKiwi

> If you happen to have access to Linux, please try that too.

Tried on latest Ubuntu with the same lldb 22 and it was indeed fine, no crash.
Forgot to mention: Tested on different Windows 11 PCs, Laptops, CPUs, GPUs. Doesn't matter, always crashes.

> Can you tell us how to build the Vulkan side of this? Does it require a supported GPU? (I've no clue about how graphics stacks work)

As far as I can tell, it should crash on any device with Windows. A Vulkan supported device is needed, but that should be anything really. I'm pretty sure you can get a validation error on improperly initializing vulkan in the first place, so this should be reproducible even on vulkan unsupported devices.

You can build a crashing example from my repo here: https://github.com/FrostKiwi/CNCvulkan/tree/7a5b2bd6919f871fe50919d79f62eb277f5da2b4
(Specifically that commit, with missing `deviceQueueCreateInfo` on purpose). It happens with any validation break, but created an early one here.
I'll keep the repo public for a month.

As per Readme, it has to be built specifically with CMake `4.2.3`, due to CMake locking new features behind UUIDs that change every version and [it can't be `4.3.0`](https://gitlab.kitware.com/cmake/cmake/-/work_items/27706). You don't need anything Vulkan specific to build it, it pulls it automatically.

Then you need to activate the validation layers with `vkconfig-gui`, as shown in the Screenshot of the OP. (Choosing `Any Running Vulkan executable` in the dropdown, clicking `Validation` in the list, Enabling `Break` in the right list, then starting the build output `./volumeVulkan`) This will cause `Validation Error: [ VUID-vkGetDeviceQueue-queueFamilyIndex-00384 ]` on execution, which together with the `break` option should make the IDE stop where the programmer made a mistake, but with LLDB it will crash.

To get `vkconfig-gui` itself you can either [`scoop install vulkan`](https://scoop.sh/#/apps?q=vulkan&p=1&id=323e8b8af9e48fd7a812289e08e83ccc23a04dfc) or install the full Vulkan SDK manually from https://vulkan.lunarg.com/sdk/home#windows .

---

### Comment 5 - DavidSpickett

> Tried on latest Ubuntu with the same lldb 22 and it was indeed fine, no crash.
Forgot to mention: Tested on different Windows 11 PCs, Laptops, CPUs, GPUs. Doesn't matter, always crashes.

Thanks! This is really pointing to it being the Windows specific codeview/pdb parsing having problems.

I don't have easy access to a device, or much experience with PDB, but @Nerixyz does. Can you have a go at reproducing this? Perhaps we can get it down to an example with just the problematic types.

---

### Comment 6 - Nerixyz

Looks like this comes from `VkLayer_khronos_validation.pdb` which contains local variables with unusually high type indices (0x80000169, 0x80000036, ...). Will see how we can resolve this.

---

