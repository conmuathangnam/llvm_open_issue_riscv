# clangd: SIGSEGV after request for inlay hints

**Author:** Shebeno4ka
**URL:** https://github.com/llvm/llvm-project/issues/165609
**Status:** Closed
**Labels:** clangd, obsolete, crash
**Closed Date:** 2025-10-29T23:51:42Z

## Body

Global:
When using via vscode clangd extension to analyze C source code of postgres.
clangd vscode extension always tryes 5 times, then closes 

Machine:
`Linux sshheebbee-hp255g9 6.14.0-34-generic #34-Ubuntu SMP PREEMPT_DYNAMIC Wed Sep 17 09:21:29 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
`

```
sshheebbee@sshheebbee-hp255g9:~$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 25.04
Release:	25.04
Codename:	plucky
```

crash backtrace (copy from):

I[21:17:50.486] Ubuntu clangd version 20.1.2 (0ubuntu1)
I[21:17:50.486] Features: linux+grpc
I[21:17:50.486] PID: 234369
I[21:17:50.486] Working directory: /home/sshheebbee/opensource/postgres
I[21:17:50.486] argv[0]: /usr/bin/clangd
I[21:17:50.486] Starting LSP over stdin/stdout
I[21:17:50.486] <-- initialize(0)
I[21:17:50.487] --> reply:initialize(0) 0 ms
I[21:17:50.494] <-- initialized
I[21:17:50.500] <-- textDocument/didOpen
I[21:17:50.501] <-- textDocument/didOpen
I[21:17:50.503] <-- textDocument/didOpen
I[21:17:50.515] Loaded compilation database from /home/sshheebbee/opensource/postgres/build/compile_commands.json
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHashjoin.c version 12 with command 
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o -MF src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o.d -o src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o -c -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHashjoin.c
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHash.c version 3 with command 
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_nodeHash.c.o -MF src/backend/postgres_lib.a.p/executor_nodeHash.c.o.d -o src/backend/postgres_lib.a.p/executor_nodeHash.c.o -c -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHash.c
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/include/executor/executor.h version 1 with command inferred from ../src/backend/executor/execAmi.c
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_execAmi.c.o -MF src/backend/postgres_lib.a.p/executor_execAmi.c.o.d -c -x c-header -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
I[21:17:50.517] --> textDocument/clangd.fileStatus
I[21:17:50.517] --> textDocument/clangd.fileStatus
I[21:17:50.518] --> textDocument/clangd.fileStatus
I[21:17:50.580] Loaded compilation database from /home/sshheebbee/opensource/postgres/build/compile_commands.json
I[21:17:50.585] <-- textDocument/documentSymbol(1)
I[21:17:50.585] <-- textDocument/codeAction(2)
I[21:17:50.585] <-- textDocument/documentLink(3)
I[21:17:50.585] <-- textDocument/inlayHint(4)
I[21:17:50.586] --> window/workDoneProgress/create(0)
I[21:17:50.586] Enqueueing 1327 commands for indexing
I[21:17:50.609] <-- reply(0)
I[21:17:50.609] --> $/progress
I[21:17:50.609] --> $/progress
I[21:17:50.753] Built preamble of size 1926372 for file /home/sshheebbee/opensource/postgres/src/include/executor/executor.h version 1 in 0.23 seconds
I[21:17:50.754] --> workspace/semanticTokens/refresh(1)
I[21:17:50.754] Indexing c17 standard library in the context of /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
I[21:17:50.756] <-- reply(1)
I[21:17:50.757] <-- textDocument/semanticTokens/full(5)
I[21:17:50.801] --> textDocument/publishDiagnostics
I[21:17:50.802] --> textDocument/inactiveRegions
I[21:17:50.829] --> reply:textDocument/documentSymbol(1) 244 ms
I[21:17:50.834] --> reply:textDocument/codeAction(2) 249 ms
I[21:17:50.834] --> reply:textDocument/documentLink(3) 249 ms
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.20.1 0x00007d60b5e1dd1f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.20.1 0x00007d60b5e1ba29 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.20.1 0x00007d60b5e1e430
3  libc.so.6       0x00007d60b3c458d0
4  clangd          0x000059f26b203f50
5  clangd          0x000059f26b1f5d48
6  clangd          0x000059f26b1f5a2c
7  clangd          0x000059f26b1e9d5a
8  clangd          0x000059f26b208985
9  clangd          0x000059f26b1e3e51
10 clangd          0x000059f26b1e1701
11 clangd          0x000059f26b1e1ab8
12 clangd          0x000059f26b1e14de
13 clangd          0x000059f26b1e1203
14 clangd          0x000059f26b105304
15 clangd          0x000059f26b2d6f00
16 clangd          0x000059f26b2cefee
17 clangd          0x000059f26b2ceb8b
18 clangd          0x000059f26b439951
19 libc.so.6       0x00007d60b3ca27f1
20 libc.so.6       0x00007d60b3d33b5c
Signalled during AST worker action: InlayHints
  Filename: /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
  Directory: /home/sshheebbee/opensource/postgres/build
  Command Line: /usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_execAmi.c.o -MF src/backend/postgres_lib.a.p/executor_execAmi.c.o.d -c -x c-header -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
  Version: 1
[Info  - 21:17:52] Connection to server got closed. Server will restart.
true
[Error - 21:17:52] Request textDocument/inlayHint failed.
[object Object]
[Error - 21:17:52] Request textDocument/semanticTokens/full failed.
[object Object]
[Error - 21:17:52] Request textDocument/codeAction failed.
[object Object]
[Error - 21:17:52] Request textDocument/inlayHint failed.
[object Object]
[Error - 21:17:52] Server process exited with signal SIGSEGV.

## Comments

### Comment 1 - Shebeno4ka

sshheebbee@sshheebbee-hp255g9:~$ clangd --version
Ubuntu clangd version 20.1.2 (0ubuntu1)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu


---

### Comment 2 - Shebeno4ka

The same behaviour with

sshheebbee@sshheebbee-hp255g9:~$ clangd-19 --version
Ubuntu clangd version 19.1.7 (3ubuntu1)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (Shebeno4ka)

<details>
Global:
When using via vscode clangd extension to analyze C source code of postgres.
clangd vscode extension always tryes 5 times, then closes 

Machine:
`Linux sshheebbee-hp255g9 6.14.0-34-generic #<!-- -->34-Ubuntu SMP PREEMPT_DYNAMIC Wed Sep 17 09:21:29 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
`

```
sshheebbee@<!-- -->sshheebbee-hp255g9:~$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 25.04
Release:	25.04
Codename:	plucky
```

crash backtrace (copy from):

I[21:17:50.486] Ubuntu clangd version 20.1.2 (0ubuntu1)
I[21:17:50.486] Features: linux+grpc
I[21:17:50.486] PID: 234369
I[21:17:50.486] Working directory: /home/sshheebbee/opensource/postgres
I[21:17:50.486] argv[0]: /usr/bin/clangd
I[21:17:50.486] Starting LSP over stdin/stdout
I[21:17:50.486] &lt;-- initialize(0)
I[21:17:50.487] --&gt; reply:initialize(0) 0 ms
I[21:17:50.494] &lt;-- initialized
I[21:17:50.500] &lt;-- textDocument/didOpen
I[21:17:50.501] &lt;-- textDocument/didOpen
I[21:17:50.503] &lt;-- textDocument/didOpen
I[21:17:50.515] Loaded compilation database from /home/sshheebbee/opensource/postgres/build/compile_commands.json
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHashjoin.c version 12 with command 
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o -MF src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o.d -o src/backend/postgres_lib.a.p/executor_nodeHashjoin.c.o -c -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHashjoin.c
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHash.c version 3 with command 
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_nodeHash.c.o -MF src/backend/postgres_lib.a.p/executor_nodeHash.c.o.d -o src/backend/postgres_lib.a.p/executor_nodeHash.c.o -c -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/backend/executor/nodeHash.c
I[21:17:50.515] ASTWorker building file /home/sshheebbee/opensource/postgres/src/include/executor/executor.h version 1 with command inferred from ../src/backend/executor/execAmi.c
[/home/sshheebbee/opensource/postgres/build]
/usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_execAmi.c.o -MF src/backend/postgres_lib.a.p/executor_execAmi.c.o.d -c -x c-header -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
I[21:17:50.517] --&gt; textDocument/clangd.fileStatus
I[21:17:50.517] --&gt; textDocument/clangd.fileStatus
I[21:17:50.518] --&gt; textDocument/clangd.fileStatus
I[21:17:50.580] Loaded compilation database from /home/sshheebbee/opensource/postgres/build/compile_commands.json
I[21:17:50.585] &lt;-- textDocument/documentSymbol(1)
I[21:17:50.585] &lt;-- textDocument/codeAction(2)
I[21:17:50.585] &lt;-- textDocument/documentLink(3)
I[21:17:50.585] &lt;-- textDocument/inlayHint(4)
I[21:17:50.586] --&gt; window/workDoneProgress/create(0)
I[21:17:50.586] Enqueueing 1327 commands for indexing
I[21:17:50.609] &lt;-- reply(0)
I[21:17:50.609] --&gt; $/progress
I[21:17:50.609] --&gt; $/progress
I[21:17:50.753] Built preamble of size 1926372 for file /home/sshheebbee/opensource/postgres/src/include/executor/executor.h version 1 in 0.23 seconds
I[21:17:50.754] --&gt; workspace/semanticTokens/refresh(1)
I[21:17:50.754] Indexing c17 standard library in the context of /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
I[21:17:50.756] &lt;-- reply(1)
I[21:17:50.757] &lt;-- textDocument/semanticTokens/full(5)
I[21:17:50.801] --&gt; textDocument/publishDiagnostics
I[21:17:50.802] --&gt; textDocument/inactiveRegions
I[21:17:50.829] --&gt; reply:textDocument/documentSymbol(1) 244 ms
I[21:17:50.834] --&gt; reply:textDocument/codeAction(2) 249 ms
I[21:17:50.834] --&gt; reply:textDocument/documentLink(3) 249 ms
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.20.1 0x00007d60b5e1dd1f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM.so.20.1 0x00007d60b5e1ba29 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.20.1 0x00007d60b5e1e430
3  libc.so.6       0x00007d60b3c458d0
4  clangd          0x000059f26b203f50
5  clangd          0x000059f26b1f5d48
6  clangd          0x000059f26b1f5a2c
7  clangd          0x000059f26b1e9d5a
8  clangd          0x000059f26b208985
9  clangd          0x000059f26b1e3e51
10 clangd          0x000059f26b1e1701
11 clangd          0x000059f26b1e1ab8
12 clangd          0x000059f26b1e14de
13 clangd          0x000059f26b1e1203
14 clangd          0x000059f26b105304
15 clangd          0x000059f26b2d6f00
16 clangd          0x000059f26b2cefee
17 clangd          0x000059f26b2ceb8b
18 clangd          0x000059f26b439951
19 libc.so.6       0x00007d60b3ca27f1
20 libc.so.6       0x00007d60b3d33b5c
Signalled during AST worker action: InlayHints
  Filename: /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
  Directory: /home/sshheebbee/opensource/postgres/build
  Command Line: /usr/lib/llvm-20/bin/cc -Isrc/backend/postgres_lib.a.p -Isrc/include -I../src/include -I/usr/include/libxml2 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O2 -g -fno-strict-aliasing -fwrapv -fexcess-precision=standard -D_GNU_SOURCE -Wmissing-prototypes -Wpointer-arith -Werror=vla -Wendif-labels -Wmissing-format-attribute -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -Wdeclaration-after-statement -Wmissing-variable-declarations -Wno-format-truncation -Wno-stringop-truncation -fPIC -pthread -DBUILDING_DLL -MD -MQ src/backend/postgres_lib.a.p/executor_execAmi.c.o -MF src/backend/postgres_lib.a.p/executor_execAmi.c.o.d -c -x c-header -resource-dir=/usr/lib/llvm-20/lib/clang/20 -- /home/sshheebbee/opensource/postgres/src/include/executor/executor.h
  Version: 1
[Info  - 21:17:52] Connection to server got closed. Server will restart.
true
[Error - 21:17:52] Request textDocument/inlayHint failed.
[object Object]
[Error - 21:17:52] Request textDocument/semanticTokens/full failed.
[object Object]
[Error - 21:17:52] Request textDocument/codeAction failed.
[object Object]
[Error - 21:17:52] Request textDocument/inlayHint failed.
[object Object]
[Error - 21:17:52] Server process exited with signal SIGSEGV.
</details>


---

### Comment 4 - HighCommander4

Please see https://clangd.llvm.org/troubleshooting#getting-stacktraces-from-crashes for instructions for getting a symbolicated stack trace.

---

### Comment 5 - Shebeno4ka

clangd-17 works fine with the same environment and code (postgres)

---

### Comment 6 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.

---

### Comment 7 - Shebeno4ka

21 works perfectly, thank you!!!!!!!!!!!

> Could you please try 21 or `main` branch? Only most recent release is maintained.



---

