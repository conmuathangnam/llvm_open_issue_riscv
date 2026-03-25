# [clangd] Crash with std::optional assertion failure in IncludeCleaner during AST build

**Author:** Light7734
**URL:** https://github.com/llvm/llvm-project/issues/161700
**Status:** Open
**Labels:** clangd, crash, clang-include-cleaner

## Body

### Summary
Clangd (20.1.8) crashes with an std::optional assertion when dereferencing an unengaged clang::clangd::IncludeStructure::HeaderID in IncludeCleaner during AST builds. Happens in a CMake project with PCH, C++23 and libc++.

### Steps to Reproduce

CMake project with public/private modules and PCH.
Flags: `-std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer.`
Edit/save a source file in LSP client (Neovim).
Crashes on AST rebuild for file and inferred headers.

Not sure if this related, but I use some symlink magic to make my project's directory structure not require the verbose `"lib_name/include/lib_name"` directory structure. Instead those are handled by CMake like so:

```cmake
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/public_includes")
file(MAKE_DIRECTORY "${PUBLIC_INCLUDE_DIR}")
file(CREATE_LINK
  "${CMAKE_CURRENT_SOURCE_DIR}/public/"
  "${PUBLIC_INCLUDE_DIR}/${libname}"
  SYMBOLIC
)
...
target_include_directories(${libname} 
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${PRIVATE_INCLUDE_DIR}
)
```

The actual project can be found at:
https://git.light7734.com/light7734/light.git

Or one these mirrors:
https://github.com/light7734/light
https://gitlab.com/light7734/light
https://codeberg.org/light7734/light

### Expected Behavior
AST builds and semantic tokens without crashes.

### Actual Behavior
Assertion: 
`/usr/include/c++/15.1.1/optional:1186: constexpr _Tp& std::optional<_Tp>::operator*() & [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this->_M_is_engaged()'` failed.

Backtrace:
```cc
 #0 0x00007f283d8ea01d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int)
 #1 0x00007f283d8e7647 llvm::sys::RunSignalHandlers()
 #2 0x00007f283d8e7647 SignalHandler
 #3 0x00007f283c83e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x00007f283c89894c __pthread_kill_implementation
 #5 0x00007f283c83e410 raise
 #6 0x00007f283c82557a abort
 #7 0x00007f283cc9a41f (/usr/lib/libstdc++.so.6+0x9a41f)
 #8 0x00005601033e2c28 llvm::DenseMapBase<...>::moveFromOldBuckets(...)
 #9 0x00005601033e2c28 llvm::DenseMap<...>::grow(unsigned int)
#10 0x00005601033e2c28 llvm::DenseMapBase<...>::grow(unsigned int)
#11 0x00005601033e2c28 llvm::DenseMapBase<...>::InsertIntoBucketImpl<...>(...)
#12 0x00005601033e2c28 llvm::DenseMapBase<...>::InsertIntoBucket<...>(...)
#13 0x00005601033e2c28 llvm::DenseMapBase<...>::try_emplace<...>(...)
#14 0x00005601033e2c28 llvm::detail::DenseSetImpl<...>::insert(...)
#15 0x00005601033e2c28 operator() /usr/src/debug/clang/.../IncludeCleaner.cpp:396:24
#16 0x0000560103b709f9 llvm::SmallVectorTemplateCommon<...>::isSmall() const
#17 0x0000560103b709f9 llvm::SmallVectorImpl<...>::~SmallVectorImpl()
#18 0x0000560103b709f9 llvm::SmallVector<...>::~SmallVector()
#19 0x0000560103b709f9 operator() /usr/src/debug/clang/.../Analysis.cpp:67:41
#20 0x0000560103b709f9 callback_fn<...>
#21 0x0000560103b9a040 TraverseTypeLoc
#22 0x0000560103b9ae19 TraverseNestedNameSpecifierLoc
#23 0x0000560103b9dc17 TraverseFunctionHelper
#24 0x0000560103b9f039 TraverseCXXDestructorDecl
#25 0x0000560103b99afa TraverseDeclContextHelper
#26 0x0000560103b9eb5d TraverseNamespaceDecl
#27 0x0000560103b99afa TraverseDeclContextHelper
#28 0x0000560103b9eb5d TraverseNamespaceDecl
#29 0x0000560103b7e0f0 clang::include_cleaner::walkUsed(...)
#30 0x00005601036749f7 __gnu_cxx::__normal_iterator<...>::__normal_iterator(...)
#31 0x00005601036749f7 std::vector<...>::end()
#32 0x00005601036749f7 decltype(fp.end()) std::end<...>(...)
#33 0x00005601036749f7 decltype(end(forward<...>(fp))) llvm::adl_detail::end_impl<...>(...)
#34 0x00005601036749f7 decltype(end_impl(forward<...>(fp))) llvm::adl_end<...>(...)
#35 0x00005601036749f7 stable_sort<...>
#36 0x00005601036749f7 clang::clangd::computeIncludeCleanerFindings(...)
#37 0x00005601036cbaa5 getIncludeCleanerDiags
#38 0x00005601036cbaa5 clang::clangd::ParsedAST::build(...)
#39 0x0000560103747e2c generateDiagnostics
#40 0x000056010374eece operator()
#41 0x000056010374334d runTask
#42 0x00005601037452be currentContext
#43 0x00005601037452be clang::clangd::Context::swapCurrent(...)
#44 0x00005601037452be clang::clangd::WithContext::~WithContext()
#45 0x00005601037452be run
#46 0x00005601038f56c2 llvm::detail::UniqueFunctionBase<...>::operator=(...)
#47 0x00005601038f56c2 llvm::unique_function<...>::operator=(...)
#48 0x00005601038f56c2 operator()
#49 0x00005601038f56c2 operator()<...>
#50 0x00005601038f56c2 __invoke_impl<...>
#51 0x00005601038f56c2 __invoke<...>
#52 0x00005601038f56c2 __apply_impl<...>
#53 0x00005601038f56c2 apply<...>
#54 0x00005601038f56c2 GenericThreadProxy<...>
#55 0x00005601038f56c2 ThreadProxy<...>
#56 0x00007f283c8969cb start_thread
#57 0x00007f283c91aa0c __GI___clone3
Signalled during AST worker action: Update
Filename: /home/light/dev/light/modules/renderer/private/system.cpp
Directory: /home/light/dev/light/build
Command Line: /usr/bin/clang++ --driver-mode=g++ -DENABLE_UNIT_TESTS=1 -DLIGHT_PLATFORM_LINUX -I/home/light/dev/light/build/modules/renderer/public_includes -I/home/light/dev/light/build/modules/renderer/private_includes -I/home/light/dev/light/build/modules/base/public_includes -I/home/light/dev/light/build/modules/app/public_includes -I/home/light/dev/light/build/modules/ecs/public_includes -I/home/light/dev/light/build/modules/logger/public_includes -I/home/light/dev/light/build/modules/debug/public_includes -I/home/light/dev/light/build/modules/memory/public_includes -I/home/light/dev/light/build/modules/assets/public_includes -I/home/light/dev/light/build/modules/time/public_includes -I/home/light/dev/light/build/modules/surface/public_includes -I/home/light/dev/light/build/modules/math/public_includes -std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer -g -Winvalid-pch -Xclang -include-pch -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx -o modules/renderer/CMakeFiles/renderer.dir/private/system.cpp.o -c -resource-dir=/usr/lib/clang/20 -- /home/light/dev/light/modules/renderer/private/system.cpp
Version: 141
```

Relavant LSP logs:
```
I[14:51:21.657] <-- textDocument/didChange
I[14:51:21.783] ASTWorker building file .../system.cpp version 141 with command ...
I[14:51:21.807] <-- textDocument/didChange
I[14:51:21.815] <-- textDocument/semanticTokens/full/delta(125)
"/usr/include/c++/15.1.1/optional:1186: constexpr _Tp& std::optional<_Tp>::operator*() & [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this->_M_is_engaged()' failed."
```

### Environment
```
OS: Arch Linux x86_64
Kernel: 6.16.8-arch3-1
CPU: Intel i9-10900 (20) @ 5.200GHz
Memory: 32GB
```

`clang --version`:
```
clang --version
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

`nvim -V1 -v`:
```
NVIM v0.11.4
Build type: RelWithDebInfo
LuaJIT 2.1.1753364724
Compilation: /usr/bin/cc -march=x86-64 -mtune=generic -O2 -pipe -fno-plt -fexceptions     -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security     -fstack-clash-protection -fcf-protection     -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -g -ffile-prefix-map=/build/neovim/src=/usr/src/debug/neovim -flto=auto -O2 -g -flto=auto -fno-fat-lto-objects -Wall -Wextra -pedantic -Wno-unused-parameter -Wstrict-prototypes -std=gnu99 -Wshadow -Wconversion -Wvla -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute -Wmissing-prototypes -fsigned-char -fstack-protector-strong -Wno-conversion -fno-common -Wno-unused-result -Wimplicit-fallthrough -fdiagnostics-color=always  -DUNIT_TESTING -D_GNU_SOURCE -DINCLUDE_GENERATED_DECLARATIONS -DUTF8PROC_STATIC -I/usr/include/luajit-2.1 -I/usr/include -I/build/neovim/src/neovim/build/src/nvim/auto -I/build/neovim/src/neovim/build/include -I/build/neovim/src/neovim/build/cmake.config -I/build/neovim/src/neovim/src

system vimrc file: "$VIM/sysinit.vim"
fall-back for $VIM: "/usr/share/nvim"
```



:checkhealth of vim.lsp for clangd:
```

- LSP log level : WARN
- Log path: /home/light/.local/state/nvim/lsp.log
- ⚠️ WARNING Log size: 239926 KB

vim.lsp: Active Clients ~
- No active clients

vim.lsp: Enabled Configurations ~
- clangd:
  - capabilities: {
      offsetEncoding = { "utf-8", "utf-16" },
      textDocument = {
        completion = {
          completionItem = {
            commitCharactersSupport = true,
            deprecatedSupport = true,
            insertReplaceSupport = true,
            insertTextModeSupport = {
              valueSet = { 1, 2 }
            },
            labelDetailsSupport = true,
            preselectSupport = true,
            resolveSupport = {
              properties = { "documentation", "additionalTextEdits", "insertTextFormat", "insertTextMode", "command" }
            },
            snippetSupport = true,
            tagSupport = {
              valueSet = { 1 }
            }
          },
          completionList = {
            itemDefaults = { "commitCharacters", "editRange", "insertTextFormat", "insertTextMode", "data" }
          },
          contextSupport = true,
          dynamicRegistration = false,
          editsNearCursor = true,
          insertTextMode = 1
        }
      }
    }
  - cmd: { "clangd" }
  - filetypes: c, cpp, objc, objcpp, cuda
  - on_attach: <function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:91>
  - on_init: <function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:86>
  - root_markers: { ".clangd", ".clang-tidy", ".clang-format", "compile_commands.json", "compile_flags.txt", "configure.ac", ".git" }
```

Please let me know if I can help with anything ^^

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Light (Light7734)

<details>
### Summary
Clangd (20.1.8) crashes with an std::optional assertion when dereferencing an unengaged clang::clangd::IncludeStructure::HeaderID in IncludeCleaner during AST builds. Happens in a CMake project with PCH, C++23 and libc++.

### Steps to Reproduce

CMake project with public/private modules and PCH.
Flags: `-std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer.`
Edit/save a source file in LSP client (Neovim).
Crashes on AST rebuild for file and inferred headers.

Not sure if this related, but I use some symlink magic to make my project's directory structure not require the verbose `"lib_name/include/lib_name"` directory structure. Instead those are handled by CMake like so:

```cmake
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/public_includes")
file(MAKE_DIRECTORY "${PUBLIC_INCLUDE_DIR}")
file(CREATE_LINK
  "${CMAKE_CURRENT_SOURCE_DIR}/public/"
  "${PUBLIC_INCLUDE_DIR}/${libname}"
  SYMBOLIC
)
...
target_include_directories(${libname} 
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${PRIVATE_INCLUDE_DIR}
)
```

The actual project can be found at:
https://git.light7734.com/light7734/light.git

Or one these mirrors:
https://github.com/light7734/light
https://gitlab.com/light7734/light
https://codeberg.org/light7734/light

### Expected Behavior
AST builds and semantic tokens without crashes.

### Actual Behavior
Assertion: 
`/usr/include/c++/15.1.1/optional:1186: constexpr _Tp&amp; std::optional&lt;_Tp&gt;::operator*() &amp; [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this-&gt;_M_is_engaged()'` failed.

Backtrace:
```cc
 #<!-- -->0 0x00007f283d8ea01d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int)
 #<!-- -->1 0x00007f283d8e7647 llvm::sys::RunSignalHandlers()
 #<!-- -->2 0x00007f283d8e7647 SignalHandler
 #<!-- -->3 0x00007f283c83e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x00007f283c89894c __pthread_kill_implementation
 #<!-- -->5 0x00007f283c83e410 raise
 #<!-- -->6 0x00007f283c82557a abort
 #<!-- -->7 0x00007f283cc9a41f (/usr/lib/libstdc++.so.6+0x9a41f)
 #<!-- -->8 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::moveFromOldBuckets(...)
 #<!-- -->9 0x00005601033e2c28 llvm::DenseMap&lt;...&gt;::grow(unsigned int)
#<!-- -->10 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::grow(unsigned int)
#<!-- -->11 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::InsertIntoBucketImpl&lt;...&gt;(...)
#<!-- -->12 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::InsertIntoBucket&lt;...&gt;(...)
#<!-- -->13 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::try_emplace&lt;...&gt;(...)
#<!-- -->14 0x00005601033e2c28 llvm::detail::DenseSetImpl&lt;...&gt;::insert(...)
#<!-- -->15 0x00005601033e2c28 operator() /usr/src/debug/clang/.../IncludeCleaner.cpp:396:24
#<!-- -->16 0x0000560103b709f9 llvm::SmallVectorTemplateCommon&lt;...&gt;::isSmall() const
#<!-- -->17 0x0000560103b709f9 llvm::SmallVectorImpl&lt;...&gt;::~SmallVectorImpl()
#<!-- -->18 0x0000560103b709f9 llvm::SmallVector&lt;...&gt;::~SmallVector()
#<!-- -->19 0x0000560103b709f9 operator() /usr/src/debug/clang/.../Analysis.cpp:67:41
#<!-- -->20 0x0000560103b709f9 callback_fn&lt;...&gt;
#<!-- -->21 0x0000560103b9a040 TraverseTypeLoc
#<!-- -->22 0x0000560103b9ae19 TraverseNestedNameSpecifierLoc
#<!-- -->23 0x0000560103b9dc17 TraverseFunctionHelper
#<!-- -->24 0x0000560103b9f039 TraverseCXXDestructorDecl
#<!-- -->25 0x0000560103b99afa TraverseDeclContextHelper
#<!-- -->26 0x0000560103b9eb5d TraverseNamespaceDecl
#<!-- -->27 0x0000560103b99afa TraverseDeclContextHelper
#<!-- -->28 0x0000560103b9eb5d TraverseNamespaceDecl
#<!-- -->29 0x0000560103b7e0f0 clang::include_cleaner::walkUsed(...)
#<!-- -->30 0x00005601036749f7 __gnu_cxx::__normal_iterator&lt;...&gt;::__normal_iterator(...)
#<!-- -->31 0x00005601036749f7 std::vector&lt;...&gt;::end()
#<!-- -->32 0x00005601036749f7 decltype(fp.end()) std::end&lt;...&gt;(...)
#<!-- -->33 0x00005601036749f7 decltype(end(forward&lt;...&gt;(fp))) llvm::adl_detail::end_impl&lt;...&gt;(...)
#<!-- -->34 0x00005601036749f7 decltype(end_impl(forward&lt;...&gt;(fp))) llvm::adl_end&lt;...&gt;(...)
#<!-- -->35 0x00005601036749f7 stable_sort&lt;...&gt;
#<!-- -->36 0x00005601036749f7 clang::clangd::computeIncludeCleanerFindings(...)
#<!-- -->37 0x00005601036cbaa5 getIncludeCleanerDiags
#<!-- -->38 0x00005601036cbaa5 clang::clangd::ParsedAST::build(...)
#<!-- -->39 0x0000560103747e2c generateDiagnostics
#<!-- -->40 0x000056010374eece operator()
#<!-- -->41 0x000056010374334d runTask
#<!-- -->42 0x00005601037452be currentContext
#<!-- -->43 0x00005601037452be clang::clangd::Context::swapCurrent(...)
#<!-- -->44 0x00005601037452be clang::clangd::WithContext::~WithContext()
#<!-- -->45 0x00005601037452be run
#<!-- -->46 0x00005601038f56c2 llvm::detail::UniqueFunctionBase&lt;...&gt;::operator=(...)
#<!-- -->47 0x00005601038f56c2 llvm::unique_function&lt;...&gt;::operator=(...)
#<!-- -->48 0x00005601038f56c2 operator()
#<!-- -->49 0x00005601038f56c2 operator()&lt;...&gt;
#<!-- -->50 0x00005601038f56c2 __invoke_impl&lt;...&gt;
#<!-- -->51 0x00005601038f56c2 __invoke&lt;...&gt;
#<!-- -->52 0x00005601038f56c2 __apply_impl&lt;...&gt;
#<!-- -->53 0x00005601038f56c2 apply&lt;...&gt;
#<!-- -->54 0x00005601038f56c2 GenericThreadProxy&lt;...&gt;
#<!-- -->55 0x00005601038f56c2 ThreadProxy&lt;...&gt;
#<!-- -->56 0x00007f283c8969cb start_thread
#<!-- -->57 0x00007f283c91aa0c __GI___clone3
Signalled during AST worker action: Update
Filename: /home/light/dev/light/modules/renderer/private/system.cpp
Directory: /home/light/dev/light/build
Command Line: /usr/bin/clang++ --driver-mode=g++ -DENABLE_UNIT_TESTS=1 -DLIGHT_PLATFORM_LINUX -I/home/light/dev/light/build/modules/renderer/public_includes -I/home/light/dev/light/build/modules/renderer/private_includes -I/home/light/dev/light/build/modules/base/public_includes -I/home/light/dev/light/build/modules/app/public_includes -I/home/light/dev/light/build/modules/ecs/public_includes -I/home/light/dev/light/build/modules/logger/public_includes -I/home/light/dev/light/build/modules/debug/public_includes -I/home/light/dev/light/build/modules/memory/public_includes -I/home/light/dev/light/build/modules/assets/public_includes -I/home/light/dev/light/build/modules/time/public_includes -I/home/light/dev/light/build/modules/surface/public_includes -I/home/light/dev/light/build/modules/math/public_includes -std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer -g -Winvalid-pch -Xclang -include-pch -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx -o modules/renderer/CMakeFiles/renderer.dir/private/system.cpp.o -c -resource-dir=/usr/lib/clang/20 -- /home/light/dev/light/modules/renderer/private/system.cpp
Version: 141
```

Relavant LSP logs:
```
I[14:51:21.657] &lt;-- textDocument/didChange
I[14:51:21.783] ASTWorker building file .../system.cpp version 141 with command ...
I[14:51:21.807] &lt;-- textDocument/didChange
I[14:51:21.815] &lt;-- textDocument/semanticTokens/full/delta(125)
"/usr/include/c++/15.1.1/optional:1186: constexpr _Tp&amp; std::optional&lt;_Tp&gt;::operator*() &amp; [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this-&gt;_M_is_engaged()' failed."
```

### Environment
```
OS: Arch Linux x86_64
Kernel: 6.16.8-arch3-1
CPU: Intel i9-10900 (20) @ 5.200GHz
Memory: 32GB
```

`clang --version`:
```
clang --version
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

`nvim -V1 -v`:
```
NVIM v0.11.4
Build type: RelWithDebInfo
LuaJIT 2.1.1753364724
Compilation: /usr/bin/cc -march=x86-64 -mtune=generic -O2 -pipe -fno-plt -fexceptions     -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security     -fstack-clash-protection -fcf-protection     -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -g -ffile-prefix-map=/build/neovim/src=/usr/src/debug/neovim -flto=auto -O2 -g -flto=auto -fno-fat-lto-objects -Wall -Wextra -pedantic -Wno-unused-parameter -Wstrict-prototypes -std=gnu99 -Wshadow -Wconversion -Wvla -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute -Wmissing-prototypes -fsigned-char -fstack-protector-strong -Wno-conversion -fno-common -Wno-unused-result -Wimplicit-fallthrough -fdiagnostics-color=always  -DUNIT_TESTING -D_GNU_SOURCE -DINCLUDE_GENERATED_DECLARATIONS -DUTF8PROC_STATIC -I/usr/include/luajit-2.1 -I/usr/include -I/build/neovim/src/neovim/build/src/nvim/auto -I/build/neovim/src/neovim/build/include -I/build/neovim/src/neovim/build/cmake.config -I/build/neovim/src/neovim/src

system vimrc file: "$VIM/sysinit.vim"
fall-back for $VIM: "/usr/share/nvim"
```



:checkhealth of vim.lsp for clangd:
```

- LSP log level : WARN
- Log path: /home/light/.local/state/nvim/lsp.log
- ⚠️ WARNING Log size: 239926 KB

vim.lsp: Active Clients ~
- No active clients

vim.lsp: Enabled Configurations ~
- clangd:
  - capabilities: {
      offsetEncoding = { "utf-8", "utf-16" },
      textDocument = {
        completion = {
          completionItem = {
            commitCharactersSupport = true,
            deprecatedSupport = true,
            insertReplaceSupport = true,
            insertTextModeSupport = {
              valueSet = { 1, 2 }
            },
            labelDetailsSupport = true,
            preselectSupport = true,
            resolveSupport = {
              properties = { "documentation", "additionalTextEdits", "insertTextFormat", "insertTextMode", "command" }
            },
            snippetSupport = true,
            tagSupport = {
              valueSet = { 1 }
            }
          },
          completionList = {
            itemDefaults = { "commitCharacters", "editRange", "insertTextFormat", "insertTextMode", "data" }
          },
          contextSupport = true,
          dynamicRegistration = false,
          editsNearCursor = true,
          insertTextMode = 1
        }
      }
    }
  - cmd: { "clangd" }
  - filetypes: c, cpp, objc, objcpp, cuda
  - on_attach: &lt;function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:91&gt;
  - on_init: &lt;function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:86&gt;
  - root_markers: { ".clangd", ".clang-tidy", ".clang-format", "compile_commands.json", "compile_flags.txt", "configure.ac", ".git" }
```

Please let me know if I can help with anything ^^
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-include-cleaner

Author: Light (Light7734)

<details>
### Summary
Clangd (20.1.8) crashes with an std::optional assertion when dereferencing an unengaged clang::clangd::IncludeStructure::HeaderID in IncludeCleaner during AST builds. Happens in a CMake project with PCH, C++23 and libc++.

### Steps to Reproduce

CMake project with public/private modules and PCH.
Flags: `-std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer.`
Edit/save a source file in LSP client (Neovim).
Crashes on AST rebuild for file and inferred headers.

Not sure if this related, but I use some symlink magic to make my project's directory structure not require the verbose `"lib_name/include/lib_name"` directory structure. Instead those are handled by CMake like so:

```cmake
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/public_includes")
file(MAKE_DIRECTORY "${PUBLIC_INCLUDE_DIR}")
file(CREATE_LINK
  "${CMAKE_CURRENT_SOURCE_DIR}/public/"
  "${PUBLIC_INCLUDE_DIR}/${libname}"
  SYMBOLIC
)
...
target_include_directories(${libname} 
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${PRIVATE_INCLUDE_DIR}
)
```

The actual project can be found at:
https://git.light7734.com/light7734/light.git

Or one these mirrors:
https://github.com/light7734/light
https://gitlab.com/light7734/light
https://codeberg.org/light7734/light

### Expected Behavior
AST builds and semantic tokens without crashes.

### Actual Behavior
Assertion: 
`/usr/include/c++/15.1.1/optional:1186: constexpr _Tp&amp; std::optional&lt;_Tp&gt;::operator*() &amp; [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this-&gt;_M_is_engaged()'` failed.

Backtrace:
```cc
 #<!-- -->0 0x00007f283d8ea01d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int)
 #<!-- -->1 0x00007f283d8e7647 llvm::sys::RunSignalHandlers()
 #<!-- -->2 0x00007f283d8e7647 SignalHandler
 #<!-- -->3 0x00007f283c83e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x00007f283c89894c __pthread_kill_implementation
 #<!-- -->5 0x00007f283c83e410 raise
 #<!-- -->6 0x00007f283c82557a abort
 #<!-- -->7 0x00007f283cc9a41f (/usr/lib/libstdc++.so.6+0x9a41f)
 #<!-- -->8 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::moveFromOldBuckets(...)
 #<!-- -->9 0x00005601033e2c28 llvm::DenseMap&lt;...&gt;::grow(unsigned int)
#<!-- -->10 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::grow(unsigned int)
#<!-- -->11 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::InsertIntoBucketImpl&lt;...&gt;(...)
#<!-- -->12 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::InsertIntoBucket&lt;...&gt;(...)
#<!-- -->13 0x00005601033e2c28 llvm::DenseMapBase&lt;...&gt;::try_emplace&lt;...&gt;(...)
#<!-- -->14 0x00005601033e2c28 llvm::detail::DenseSetImpl&lt;...&gt;::insert(...)
#<!-- -->15 0x00005601033e2c28 operator() /usr/src/debug/clang/.../IncludeCleaner.cpp:396:24
#<!-- -->16 0x0000560103b709f9 llvm::SmallVectorTemplateCommon&lt;...&gt;::isSmall() const
#<!-- -->17 0x0000560103b709f9 llvm::SmallVectorImpl&lt;...&gt;::~SmallVectorImpl()
#<!-- -->18 0x0000560103b709f9 llvm::SmallVector&lt;...&gt;::~SmallVector()
#<!-- -->19 0x0000560103b709f9 operator() /usr/src/debug/clang/.../Analysis.cpp:67:41
#<!-- -->20 0x0000560103b709f9 callback_fn&lt;...&gt;
#<!-- -->21 0x0000560103b9a040 TraverseTypeLoc
#<!-- -->22 0x0000560103b9ae19 TraverseNestedNameSpecifierLoc
#<!-- -->23 0x0000560103b9dc17 TraverseFunctionHelper
#<!-- -->24 0x0000560103b9f039 TraverseCXXDestructorDecl
#<!-- -->25 0x0000560103b99afa TraverseDeclContextHelper
#<!-- -->26 0x0000560103b9eb5d TraverseNamespaceDecl
#<!-- -->27 0x0000560103b99afa TraverseDeclContextHelper
#<!-- -->28 0x0000560103b9eb5d TraverseNamespaceDecl
#<!-- -->29 0x0000560103b7e0f0 clang::include_cleaner::walkUsed(...)
#<!-- -->30 0x00005601036749f7 __gnu_cxx::__normal_iterator&lt;...&gt;::__normal_iterator(...)
#<!-- -->31 0x00005601036749f7 std::vector&lt;...&gt;::end()
#<!-- -->32 0x00005601036749f7 decltype(fp.end()) std::end&lt;...&gt;(...)
#<!-- -->33 0x00005601036749f7 decltype(end(forward&lt;...&gt;(fp))) llvm::adl_detail::end_impl&lt;...&gt;(...)
#<!-- -->34 0x00005601036749f7 decltype(end_impl(forward&lt;...&gt;(fp))) llvm::adl_end&lt;...&gt;(...)
#<!-- -->35 0x00005601036749f7 stable_sort&lt;...&gt;
#<!-- -->36 0x00005601036749f7 clang::clangd::computeIncludeCleanerFindings(...)
#<!-- -->37 0x00005601036cbaa5 getIncludeCleanerDiags
#<!-- -->38 0x00005601036cbaa5 clang::clangd::ParsedAST::build(...)
#<!-- -->39 0x0000560103747e2c generateDiagnostics
#<!-- -->40 0x000056010374eece operator()
#<!-- -->41 0x000056010374334d runTask
#<!-- -->42 0x00005601037452be currentContext
#<!-- -->43 0x00005601037452be clang::clangd::Context::swapCurrent(...)
#<!-- -->44 0x00005601037452be clang::clangd::WithContext::~WithContext()
#<!-- -->45 0x00005601037452be run
#<!-- -->46 0x00005601038f56c2 llvm::detail::UniqueFunctionBase&lt;...&gt;::operator=(...)
#<!-- -->47 0x00005601038f56c2 llvm::unique_function&lt;...&gt;::operator=(...)
#<!-- -->48 0x00005601038f56c2 operator()
#<!-- -->49 0x00005601038f56c2 operator()&lt;...&gt;
#<!-- -->50 0x00005601038f56c2 __invoke_impl&lt;...&gt;
#<!-- -->51 0x00005601038f56c2 __invoke&lt;...&gt;
#<!-- -->52 0x00005601038f56c2 __apply_impl&lt;...&gt;
#<!-- -->53 0x00005601038f56c2 apply&lt;...&gt;
#<!-- -->54 0x00005601038f56c2 GenericThreadProxy&lt;...&gt;
#<!-- -->55 0x00005601038f56c2 ThreadProxy&lt;...&gt;
#<!-- -->56 0x00007f283c8969cb start_thread
#<!-- -->57 0x00007f283c91aa0c __GI___clone3
Signalled during AST worker action: Update
Filename: /home/light/dev/light/modules/renderer/private/system.cpp
Directory: /home/light/dev/light/build
Command Line: /usr/bin/clang++ --driver-mode=g++ -DENABLE_UNIT_TESTS=1 -DLIGHT_PLATFORM_LINUX -I/home/light/dev/light/build/modules/renderer/public_includes -I/home/light/dev/light/build/modules/renderer/private_includes -I/home/light/dev/light/build/modules/base/public_includes -I/home/light/dev/light/build/modules/app/public_includes -I/home/light/dev/light/build/modules/ecs/public_includes -I/home/light/dev/light/build/modules/logger/public_includes -I/home/light/dev/light/build/modules/debug/public_includes -I/home/light/dev/light/build/modules/memory/public_includes -I/home/light/dev/light/build/modules/assets/public_includes -I/home/light/dev/light/build/modules/time/public_includes -I/home/light/dev/light/build/modules/surface/public_includes -I/home/light/dev/light/build/modules/math/public_includes -std=c++23 -stdlib=libc++ -g -fno-omit-frame-pointer -g -Winvalid-pch -Xclang -include-pch -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/light/dev/light/build/modules/renderer/CMakeFiles/renderer.dir/cmake_pch.hxx -o modules/renderer/CMakeFiles/renderer.dir/private/system.cpp.o -c -resource-dir=/usr/lib/clang/20 -- /home/light/dev/light/modules/renderer/private/system.cpp
Version: 141
```

Relavant LSP logs:
```
I[14:51:21.657] &lt;-- textDocument/didChange
I[14:51:21.783] ASTWorker building file .../system.cpp version 141 with command ...
I[14:51:21.807] &lt;-- textDocument/didChange
I[14:51:21.815] &lt;-- textDocument/semanticTokens/full/delta(125)
"/usr/include/c++/15.1.1/optional:1186: constexpr _Tp&amp; std::optional&lt;_Tp&gt;::operator*() &amp; [with _Tp = clang::clangd::IncludeStructure::HeaderID]: Assertion 'this-&gt;_M_is_engaged()' failed."
```

### Environment
```
OS: Arch Linux x86_64
Kernel: 6.16.8-arch3-1
CPU: Intel i9-10900 (20) @ 5.200GHz
Memory: 32GB
```

`clang --version`:
```
clang --version
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

`nvim -V1 -v`:
```
NVIM v0.11.4
Build type: RelWithDebInfo
LuaJIT 2.1.1753364724
Compilation: /usr/bin/cc -march=x86-64 -mtune=generic -O2 -pipe -fno-plt -fexceptions     -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security     -fstack-clash-protection -fcf-protection     -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -g -ffile-prefix-map=/build/neovim/src=/usr/src/debug/neovim -flto=auto -O2 -g -flto=auto -fno-fat-lto-objects -Wall -Wextra -pedantic -Wno-unused-parameter -Wstrict-prototypes -std=gnu99 -Wshadow -Wconversion -Wvla -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute -Wmissing-prototypes -fsigned-char -fstack-protector-strong -Wno-conversion -fno-common -Wno-unused-result -Wimplicit-fallthrough -fdiagnostics-color=always  -DUNIT_TESTING -D_GNU_SOURCE -DINCLUDE_GENERATED_DECLARATIONS -DUTF8PROC_STATIC -I/usr/include/luajit-2.1 -I/usr/include -I/build/neovim/src/neovim/build/src/nvim/auto -I/build/neovim/src/neovim/build/include -I/build/neovim/src/neovim/build/cmake.config -I/build/neovim/src/neovim/src

system vimrc file: "$VIM/sysinit.vim"
fall-back for $VIM: "/usr/share/nvim"
```



:checkhealth of vim.lsp for clangd:
```

- LSP log level : WARN
- Log path: /home/light/.local/state/nvim/lsp.log
- ⚠️ WARNING Log size: 239926 KB

vim.lsp: Active Clients ~
- No active clients

vim.lsp: Enabled Configurations ~
- clangd:
  - capabilities: {
      offsetEncoding = { "utf-8", "utf-16" },
      textDocument = {
        completion = {
          completionItem = {
            commitCharactersSupport = true,
            deprecatedSupport = true,
            insertReplaceSupport = true,
            insertTextModeSupport = {
              valueSet = { 1, 2 }
            },
            labelDetailsSupport = true,
            preselectSupport = true,
            resolveSupport = {
              properties = { "documentation", "additionalTextEdits", "insertTextFormat", "insertTextMode", "command" }
            },
            snippetSupport = true,
            tagSupport = {
              valueSet = { 1 }
            }
          },
          completionList = {
            itemDefaults = { "commitCharacters", "editRange", "insertTextFormat", "insertTextMode", "data" }
          },
          contextSupport = true,
          dynamicRegistration = false,
          editsNearCursor = true,
          insertTextMode = 1
        }
      }
    }
  - cmd: { "clangd" }
  - filetypes: c, cpp, objc, objcpp, cuda
  - on_attach: &lt;function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:91&gt;
  - on_init: &lt;function @/home/light/.local/share/nvim/lazy/nvim-lspconfig/lsp/clangd.lua:86&gt;
  - root_markers: { ".clangd", ".clang-tidy", ".clang-format", "compile_commands.json", "compile_flags.txt", "configure.ac", ".git" }
```

Please let me know if I can help with anything ^^
</details>


---

### Comment 3 - HighCommander4

> Please let me know if I can help with anything

It's worth testing with the latest weekly snapshot from https://github.com/clangd/clangd/releases to see if it's still affected.

Assuming it is: if you're able to reduce the input the triggers the crash from your entire project to something smaller (ideally, a minimal testcase with no dependencies), that is likely to lead to the crash being investigated sooner.

---

### Comment 4 - Light7734

I believe it was crashing even on a simple project (no symlinks or dependencies), not sure if my own editor/lsp configurations is messed up or not.

I'll try the snapshot when I get some free time to see if it's fixed.

For now if anyone has this issue, you can fix it by adding a `.clangd` file on the root of your project with the following content:
```yaml
Diagnostics:
  UnusedIncludes: None
  MissingIncludes: None
```

---

