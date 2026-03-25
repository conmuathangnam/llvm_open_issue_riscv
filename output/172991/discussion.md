# [Clang 21] Crash in MicrosoftNumberingContext::getManglingNumber with virtual inheritance on armv7a-none-windows-eabi

**Author:** vossjannik
**URL:** https://github.com/llvm/llvm-project/issues/172991

## Body

Clang 21 crashes when compiling code with virtual inheritance targeting `armv7a-none-windows-eabi`. This is a regression from Clang 20.

### Minimal Reproducer

```cpp
class A {};
class B : virtual A {};
B b;
```

[View in Compiler Explorer](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:5,endLineNumber:3,positionColumn:5,positionLineNumber:3,selectionStartColumn:5,selectionStartLineNumber:3,startColumn:5,startLineNumber:3),source:'class+A+%7B%7D%3B%0Aclass+B+:+virtual+A+%7B%7D%3B%0AB+b%3B'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang2110,filters:(b:'0',binary:'1',binaryObject:'0',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--target%3Darmv7a-none-windows-eabi',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+21.1.0+(Editor+%231)',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86+msvc+v19.44+VS17.14',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+21.1.0+(Compiler+%231)',t:'0')),k:33.33333333333333,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)

### Command
```bash
clang++ --target=armv7a-none-windows-eabi -c test.cpp
```

### Expected Behavior
Code compiles successfully (as it does with Clang 20 and with other ARM targets like `armv7a-none-eabi`).

### Actual Behavior
Compiler crashes with SIGSEGV (exit code 139).

### Stack Trace
```
#0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int)
#1 llvm::sys::CleanupOnSignal(unsigned long)
#2 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 (/lib/x86_64-linux-gnu/libc.so. 6+0x42520)
#4 (anonymous namespace)::MicrosoftNumberingContext:: getManglingNumber(clang:: BlockDecl const*) (.cold) MicrosoftCXXABI.cpp:0:0
```

### Version Information
- **Crashes:** Clang 21.1.0
- **Works:** Clang 20.x
- **Works with targets:** `armv7a-none-eabi`, other non-Windows ARM targets
- **Crashes with targets:** `armv7a-none-windows-eabi`

### Analysis
The crash occurs in `MicrosoftCXXABI. cpp` in `MicrosoftNumberingContext::getManglingNumber(const BlockDecl*)`. This suggests an issue with the Microsoft ABI name mangling code when handling classes with virtual inheritance on Windows ARM targets. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jannik Voss (vossjannik)

<details>
Clang 21 crashes when compiling code with virtual inheritance targeting `armv7a-none-windows-eabi`. This is a regression from Clang 20.

### Minimal Reproducer

```cpp
class A {};
class B : virtual A {};
B b;
```

[View in Compiler Explorer](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:5,endLineNumber:3,positionColumn:5,positionLineNumber:3,selectionStartColumn:5,selectionStartLineNumber:3,startColumn:5,startLineNumber:3),source:'class+A+%7B%7D%3B%0Aclass+B+:+virtual+A+%7B%7D%3B%0AB+b%3B'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang2110,filters:(b:'0',binary:'1',binaryObject:'0',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--target%3Darmv7a-none-windows-eabi',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+21.1.0+(Editor+%231)',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86+msvc+v19.44+VS17.14',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+21.1.0+(Compiler+%231)',t:'0')),k:33.33333333333333,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)

### Command
```bash
clang++ --target=armv7a-none-windows-eabi -c test.cpp
```

### Expected Behavior
Code compiles successfully (as it does with Clang 20 and with other ARM targets like `armv7a-none-eabi`).

### Actual Behavior
Compiler crashes with SIGSEGV (exit code 139).

### Stack Trace
```
#<!-- -->0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int)
#<!-- -->1 llvm::sys::CleanupOnSignal(unsigned long)
#<!-- -->2 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#<!-- -->3 (/lib/x86_64-linux-gnu/libc.so. 6+0x42520)
#<!-- -->4 (anonymous namespace)::MicrosoftNumberingContext:: getManglingNumber(clang:: BlockDecl const*) (.cold) MicrosoftCXXABI.cpp:0:0
```

### Version Information
- **Crashes:** Clang 21.1.0
- **Works:** Clang 20.x
- **Works with targets:** `armv7a-none-eabi`, other non-Windows ARM targets
- **Crashes with targets:** `armv7a-none-windows-eabi`

### Analysis
The crash occurs in `MicrosoftCXXABI. cpp` in `MicrosoftNumberingContext::getManglingNumber(const BlockDecl*)`. This suggests an issue with the Microsoft ABI name mangling code when handling classes with virtual inheritance on Windows ARM targets. 
</details>


---

