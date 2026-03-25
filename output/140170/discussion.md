# Clangd language server crashing when using C++ #embed macro.

**Author:** Ankizle
**URL:** https://github.com/llvm/llvm-project/issues/140170
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, embed
**Closed Date:** 2025-05-16T02:18:45Z

## Body

I'm using the `#embed` macro in clangd to include some shader source into a header file. I have a setup like so:

shader/source.h:
```cpp
#pragma once

namespace frontend::shader {
const char source[] = {
// type declarations
#embed "type/camera.wgsl"
    , '\n',

// globals
#embed "bindings.wgsl"
    , '\n',

// logic

// entry points
#embed "visual.wgsl"
    , '\n', 0,
};
} // namespace frontend::shader
```

init_shader.cpp:
```cpp
#include "shader/source.h"
#include "frontend/engine.h"
#include "frontend/util/str.h"
#include "webgpu/webgpu.h"
#include <vector>

namespace frontend {
void engine::init_shader() {
    auto shader_src_desc = WGPUShaderSourceWGSL{
        .chain =
            WGPUChainedStruct{
                .next = nullptr,
                .sType = WGPUSType_ShaderSourceWGSL,
            },
        .code = util::string_view(shader::source), /** Problematic line. Removing this line stops the crashing. */
    };
    auto shader_mod_desc = WGPUShaderModuleDescriptor{
        .nextInChain = &shader_src_desc.chain,
        .label = util::string_view("shader module"),
    };
}
} // namespace frontend
```

But Clangd repeatedly crashes with the error:

```
/usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -c -fPIC -MMD -x c++-header -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/shader/source.h
I[21:05:50.999] --> windowDoneProgress/create(0)
I[21:05:50.999] Enqueueing 11 commands for indexing
I[21:05:50.999] <-- reply(0)
I[21:05:50.999] --> $/progress
I[21:05:50.999] --> $/progress
I[21:05:51.000] --> textDocument/clangd.fileStatus
I[21:05:51.000] --> textDocument/clangd.fileStatus
I[21:05:51.002] <-- textDocument/documentLink(3)
I[21:05:51.003] <-- textDocument/inlayHint(4)
I[21:05:51.004] Built preamble of size 239148 for file /home/user/Project5/src/shader/source.h version 15 in 0.00 seconds
I[21:05:51.004] Indexing c++17 standard library in the context of /home/user/Project5/src/shader/source.h
I[21:05:51.005] --> workspace/semanticTokens/refresh(1)
I[21:05:51.005] --> textDocument/clangd.fileStatus
I[21:05:51.005] <-- reply(1)
I[21:05:51.008] --> textDocument/publishDiagnostics
I[21:05:51.009] --> textDocument/inactiveRegions
I[21:05:51.009] --> textDocument/clangd.fileStatus
I[21:05:51.011] <-- textDocument/semanticTokens/full(5)
I[21:05:51.087] --> $/progress
I[21:05:51.087] --> $/progress
I[21:05:51.087] --> $/progress
I[21:05:51.200] <-- textDocument/foldingRange(6)
I[21:05:51.200] --> reply:textDocument/foldingRange(6) 0 ms
I[21:05:51.299] <-- textDocument/documentSymbol(7)
I[21:05:51.324] Built preamble of size 6906136 for file /home/user/Project5/src/init_shader.cpp version 1 in 0.32 seconds
I[21:05:51.324] --> workspace/semanticTokens/refresh(2)
I[21:05:51.324] <-- reply(2)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/user/Project5/src/init_shader.cpp:15:43: current parser token 'source'
1.	/home/user/Project5/src/init_shader.cpp:7:1: parsing namespace 'frontend'
2.	/home/user/Project5/src/init_shader.cpp:8:28: parsing function body 'frontend::engine::init_shader'
3.	/home/user/Project5/src/init_shader.cpp:8:28: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clangd    0x000000000055715b
1  clangd    0x0000000000554fbe
2  clangd    0x0000000000557af6
3  libc.so.6 0x00007f9cb9932050
4  libc.so.6 0x00007f9ca0038fa0
Signalled during AST worker action: Build AST
  Filename: frontend/src/init_shader.cpp
  Directory: /home/user/Project5
  Command Line: /usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -o build/init/shader.o -c -fPIC -MMD -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/init_shader.cpp
  Version: 1
[Error - 9:05:52 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/codeAction failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/inlayHint failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/semanticTokens/full failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
```

I believe this is due to using `#embed`, because removing them stops the crashing. Strangely, I am allowed to use one or two `#embed` macros, but using three causes a crash.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (Ankizle)

<details>
I'm using the `#embed` macro in clangd to include some shader source into a header file. I have a setup like so:

shader/source.h:
```cpp
#pragma once

namespace frontend::shader {
const char source[] = {
// type declarations
#embed "type/camera.wgsl"
    , '\n',

// globals
#embed "bindings.wgsl"
    , '\n',

// logic

// entry points
#embed "visual.wgsl"
    , '\n', 0,
};
} // namespace frontend::shader
```

init_shader.cpp:
```cpp
#include "shader/source.h"
#include "frontend/engine.h"
#include "frontend/util/str.h"
#include "webgpu/webgpu.h"
#include &lt;vector&gt;

namespace frontend {
void engine::init_shader() {
    auto shader_src_desc = WGPUShaderSourceWGSL{
        .chain =
            WGPUChainedStruct{
                .next = nullptr,
                .sType = WGPUSType_ShaderSourceWGSL,
            },
        .code = util::string_view(shader::source), /** Problematic line. Removing this line stops the crashing. */
    };
    auto shader_mod_desc = WGPUShaderModuleDescriptor{
        .nextInChain = &amp;shader_src_desc.chain,
        .label = util::string_view("shader module"),
    };
}
} // namespace frontend
```

But Clangd repeatedly crashes with the error:

```
/usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -c -fPIC -MMD -x c++-header -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/shader/source.h
I[21:05:50.999] --&gt; windowDoneProgress/create(0)
I[21:05:50.999] Enqueueing 11 commands for indexing
I[21:05:50.999] &lt;-- reply(0)
I[21:05:50.999] --&gt; $/progress
I[21:05:50.999] --&gt; $/progress
I[21:05:51.000] --&gt; textDocument/clangd.fileStatus
I[21:05:51.000] --&gt; textDocument/clangd.fileStatus
I[21:05:51.002] &lt;-- textDocument/documentLink(3)
I[21:05:51.003] &lt;-- textDocument/inlayHint(4)
I[21:05:51.004] Built preamble of size 239148 for file /home/user/Project5/src/shader/source.h version 15 in 0.00 seconds
I[21:05:51.004] Indexing c++17 standard library in the context of /home/user/Project5/src/shader/source.h
I[21:05:51.005] --&gt; workspace/semanticTokens/refresh(1)
I[21:05:51.005] --&gt; textDocument/clangd.fileStatus
I[21:05:51.005] &lt;-- reply(1)
I[21:05:51.008] --&gt; textDocument/publishDiagnostics
I[21:05:51.009] --&gt; textDocument/inactiveRegions
I[21:05:51.009] --&gt; textDocument/clangd.fileStatus
I[21:05:51.011] &lt;-- textDocument/semanticTokens/full(5)
I[21:05:51.087] --&gt; $/progress
I[21:05:51.087] --&gt; $/progress
I[21:05:51.087] --&gt; $/progress
I[21:05:51.200] &lt;-- textDocument/foldingRange(6)
I[21:05:51.200] --&gt; reply:textDocument/foldingRange(6) 0 ms
I[21:05:51.299] &lt;-- textDocument/documentSymbol(7)
I[21:05:51.324] Built preamble of size 6906136 for file /home/user/Project5/src/init_shader.cpp version 1 in 0.32 seconds
I[21:05:51.324] --&gt; workspace/semanticTokens/refresh(2)
I[21:05:51.324] &lt;-- reply(2)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/user/Project5/src/init_shader.cpp:15:43: current parser token 'source'
1.	/home/user/Project5/src/init_shader.cpp:7:1: parsing namespace 'frontend'
2.	/home/user/Project5/src/init_shader.cpp:8:28: parsing function body 'frontend::engine::init_shader'
3.	/home/user/Project5/src/init_shader.cpp:8:28: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clangd    0x000000000055715b
1  clangd    0x0000000000554fbe
2  clangd    0x0000000000557af6
3  libc.so.6 0x00007f9cb9932050
4  libc.so.6 0x00007f9ca0038fa0
Signalled during AST worker action: Build AST
  Filename: frontend/src/init_shader.cpp
  Directory: /home/user/Project5
  Command Line: /usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -o build/init/shader.o -c -fPIC -MMD -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/init_shader.cpp
  Version: 1
[Error - 9:05:52 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/codeAction failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/inlayHint failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/semanticTokens/full failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
```

I believe this is due to using `#embed`, because removing them stops the crashing. Strangely, I am allowed to use one or two `#embed` macros, but using three causes a crash.
</details>


---

### Comment 2 - HighCommander4

Likely duplicate of https://github.com/llvm/llvm-project/issues/107869, which is fixed on trunk.

Could you try a snapshot build from https://github.com/clangd/clangd/releases to confirm?

---

### Comment 3 - Ankizle

Yep, using the snapshot build solves the issue. Thanks.

---

### Comment 4 - HighCommander4

Thanks for confirming!

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Ankizle)

<details>
I'm using the `#embed` macro in clangd to include some shader source into a header file. I have a setup like so:

shader/source.h:
```cpp
#pragma once

namespace frontend::shader {
const char source[] = {
// type declarations
#embed "type/camera.wgsl"
    , '\n',

// globals
#embed "bindings.wgsl"
    , '\n',

// logic

// entry points
#embed "visual.wgsl"
    , '\n', 0,
};
} // namespace frontend::shader
```

init_shader.cpp:
```cpp
#include "shader/source.h"
#include "frontend/engine.h"
#include "frontend/util/str.h"
#include "webgpu/webgpu.h"
#include &lt;vector&gt;

namespace frontend {
void engine::init_shader() {
    auto shader_src_desc = WGPUShaderSourceWGSL{
        .chain =
            WGPUChainedStruct{
                .next = nullptr,
                .sType = WGPUSType_ShaderSourceWGSL,
            },
        .code = util::string_view(shader::source), /** Problematic line. Removing this line stops the crashing. */
    };
    auto shader_mod_desc = WGPUShaderModuleDescriptor{
        .nextInChain = &amp;shader_src_desc.chain,
        .label = util::string_view("shader module"),
    };
}
} // namespace frontend
```

But Clangd repeatedly crashes with the error:

```
/usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -c -fPIC -MMD -x c++-header -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/shader/source.h
I[21:05:50.999] --&gt; windowDoneProgress/create(0)
I[21:05:50.999] Enqueueing 11 commands for indexing
I[21:05:50.999] &lt;-- reply(0)
I[21:05:50.999] --&gt; $/progress
I[21:05:50.999] --&gt; $/progress
I[21:05:51.000] --&gt; textDocument/clangd.fileStatus
I[21:05:51.000] --&gt; textDocument/clangd.fileStatus
I[21:05:51.002] &lt;-- textDocument/documentLink(3)
I[21:05:51.003] &lt;-- textDocument/inlayHint(4)
I[21:05:51.004] Built preamble of size 239148 for file /home/user/Project5/src/shader/source.h version 15 in 0.00 seconds
I[21:05:51.004] Indexing c++17 standard library in the context of /home/user/Project5/src/shader/source.h
I[21:05:51.005] --&gt; workspace/semanticTokens/refresh(1)
I[21:05:51.005] --&gt; textDocument/clangd.fileStatus
I[21:05:51.005] &lt;-- reply(1)
I[21:05:51.008] --&gt; textDocument/publishDiagnostics
I[21:05:51.009] --&gt; textDocument/inactiveRegions
I[21:05:51.009] --&gt; textDocument/clangd.fileStatus
I[21:05:51.011] &lt;-- textDocument/semanticTokens/full(5)
I[21:05:51.087] --&gt; $/progress
I[21:05:51.087] --&gt; $/progress
I[21:05:51.087] --&gt; $/progress
I[21:05:51.200] &lt;-- textDocument/foldingRange(6)
I[21:05:51.200] --&gt; reply:textDocument/foldingRange(6) 0 ms
I[21:05:51.299] &lt;-- textDocument/documentSymbol(7)
I[21:05:51.324] Built preamble of size 6906136 for file /home/user/Project5/src/init_shader.cpp version 1 in 0.32 seconds
I[21:05:51.324] --&gt; workspace/semanticTokens/refresh(2)
I[21:05:51.324] &lt;-- reply(2)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/user/Project5/src/init_shader.cpp:15:43: current parser token 'source'
1.	/home/user/Project5/src/init_shader.cpp:7:1: parsing namespace 'frontend'
2.	/home/user/Project5/src/init_shader.cpp:8:28: parsing function body 'frontend::engine::init_shader'
3.	/home/user/Project5/src/init_shader.cpp:8:28: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clangd    0x000000000055715b
1  clangd    0x0000000000554fbe
2  clangd    0x0000000000557af6
3  libc.so.6 0x00007f9cb9932050
4  libc.so.6 0x00007f9ca0038fa0
Signalled during AST worker action: Build AST
  Filename: frontend/src/init_shader.cpp
  Directory: /home/user/Project5
  Command Line: /usr/bin/clang++ --driver-mode=g++ -fdiagnostics-color=always -g -Iinclude -Iexternal/glfw/include -Iexternal/imgui -Iexternal/wgpu-native/dist/include -o build/init/shader.o -c -fPIC -MMD -resource-dir=/home/user/.config/VSCodium/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/user/Project5/src/init_shader.cpp
  Version: 1
[Error - 9:05:52 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/codeAction failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/inlayHint failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/semanticTokens/full failed.
[object Object]
[Error - 9:05:52 PM] Request textDocument/documentSymbol failed.
[object Object]
```

I believe this is due to using `#embed`, because removing them stops the crashing. Strangely, I am allowed to use one or two `#embed` macros, but using three causes a crash.
</details>


---

