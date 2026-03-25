# Crash while parsing function body in compound statement ('{}')

**Author:** jamacias
**URL:** https://github.com/llvm/llvm-project/issues/178301
**Status:** Open
**Labels:** clangd, obsolete, crash

## Body

Hi, I am currently working on a project that depends on [Corrade](https://github.com/mosra/corrade) and while using the [clangd extension](https://open-vsx.org/extension/llvm-vs-code-extensions/vscode-clangd) in [VSCodium](https://vscodium.com) the server crashes always in (at least) [one file](https://github.com/mosra/corrade/blob/master/src/Corrade/PluginManager/PluginMetadata.cpp#L38).

I believe this may be a bug and as suggested by the output, I am posting it here. Unfortunately I do not have the knowledge to understand what is going on. 

Could anyone please confirm whether this is really a bug and/or if there is any way to work around or solve the issue?

Version: 21.1.8.

### (Verbose) stacktrace

```
Stack dump:
0.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:39:69: current parser token ';'
1.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:32:1: parsing namespace 'Corrade'
2.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:32:21: parsing namespace 'Corrade::PluginManager'
3.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:38:60: parsing function body 'Corrade::PluginManager::PluginMetadata::depends'
4.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:38:60: in compound statement ('{}')
V[22:28:16.560] indexed preamble AST for /home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp version 71:
  symbol slab: 4563 symbols, 1488095 bytes
  ref slab: 0 symbols, 0 refs, 128 bytes
  relations slab: 209 relations, 4376 bytes
V[22:28:16.580] Build dynamic index for header symbols with estimated memory usage of 3620163 bytes
[Error - 10:28:18 PM] Server process exited with signal SIGSEGV.
[Error - 10:28:18 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
```

### Related findings

Removing a few of the offending returns (i.e., those that look like `return static_cast<const Implementation::Plugin*>(this)->depends;`), the server does not crash anymore.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (jamacias)

<details>
Hi, I am currently working on a project that depends on [Corrade](https://github.com/mosra/corrade) and while using the [clangd extension](https://open-vsx.org/extension/llvm-vs-code-extensions/vscode-clangd) in [VSCodium](https://vscodium.com) the server crashes always in (at least) [one file](https://github.com/mosra/corrade/blob/master/src/Corrade/PluginManager/PluginMetadata.cpp#L38).

I believe this may be a bug and as suggested by the output, I am posting it here. Unfortunately I do not have the knowledge to understand what is going on. 

Could anyone please confirm whether this is really a bug and/or if there is any way to work around or solve the issue?

Version: 21.1.8.

### (Verbose) stacktrace

```
Stack dump:
0.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:39:69: current parser token ';'
1.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:32:1: parsing namespace 'Corrade'
2.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:32:21: parsing namespace 'Corrade::PluginManager'
3.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:38:60: parsing function body 'Corrade::PluginManager::PluginMetadata::depends'
4.	/home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp:38:60: in compound statement ('{}')
V[22:28:16.560] indexed preamble AST for /home/javi/workspace/magnum/corrade/src/Corrade/PluginManager/PluginMetadata.cpp version 71:
  symbol slab: 4563 symbols, 1488095 bytes
  ref slab: 0 symbols, 0 refs, 128 bytes
  relations slab: 209 relations, 4376 bytes
V[22:28:16.580] Build dynamic index for header symbols with estimated memory usage of 3620163 bytes
[Error - 10:28:18 PM] Server process exited with signal SIGSEGV.
[Error - 10:28:18 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
```

### Related findings

Removing a few of the offending returns (i.e., those that look like `return static_cast&lt;const Implementation::Plugin*&gt;(this)-&gt;depends;`), the server does not crash anymore.


</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.

---

### Comment 3 - jamacias

@EugeneZelenko it worked with [LLVM 22.1.0-rc1](https://github.com/llvm/llvm-project/releases/tag/llvmorg-22.1.0-rc1) and the crash is gone! Thank you very much.

---

