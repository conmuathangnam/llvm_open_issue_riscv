# LLDB crashes trying to execute an expression when connected to web-assembly-micro-runtime

**Author:** DavidSpickett
**URL:** https://github.com/llvm/llvm-project/issues/179915
**Status:** Open
**Labels:** lldb, backend:WebAssembly, crash

## Body

Found while following along with https://jonasdevlieghere.com/post/wasm-debugging-with-lldb-and-wamr/.

My example program:
```
#include <stddef.h>

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j);
}
```
Compile with:
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--no-stack-first -O0 -g -o simple.wasm simple.c
```
Note the no-stack-first. Without this, iwasm will fail to allocate the exec stack required for the debug interpreter.

Run it in iwasm with debugging enabled:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:23:53:350 - FD569AF9F020]: warning: allocate 544768 bytes memory failed
[02:23:53:350 - FD569AF9F020]: WASM Debug Engine warning: failed to allocate linear memory for execution. 
Will not be able to evaluate expressions during debugging
[02:23:53:350 - FD569AD0F0E0]: control thread of debug object 0xc405dac829d0 start

[02:23:53:350 - FD569AD0F0E0]: Debug server listening on 127.0.0.1:1234
```
Note that iwasm failed to allocate space for expression execution. This is expected in this case because the example's address map has no space and there's no fallback way for iwasm to get it.

Now connect lldb to iwasm:
```
$ lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
<...>
Process 1 stopped
* thread #1, name = 'nobody', stop reason = trace
    frame #0: 0x40000000000001d1 main at simple.c:7
   4   	  return a + b;
   5   	}
   6   	
-> 7   	int main() {
```
Try to execute an expression:
```
(lldb) p add(1, 2)
lldb             <  14> send packet: $_M1000,rwx#fa
lldb             <   7> read packet: $E03#a8
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-589ed7
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #0 0x0000acef75f9452c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #1 0x0000acef75f9237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #2 0x0000acef75f952f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #4 0x0000f89ab9085b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #5 0x0000f89abfb29a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #6 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #7 0x0000f89ab9022008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #8 0x0000f89ab8fda83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000f89ab8fc7134 abort ./stdlib/abort.c:81:7
#10 0x0000f89abfac68ac llvm::report_fatal_error(llvm::Twine const&, bool) ErrorHandling.cpp:0:0
#11 0x0000f89abfac6690 llvm::report_fatal_error(llvm::Twine const&, bool) ErrorHandling.cpp:0:0
#12 0x0000f89ac2aa0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#13 0x0000f89ac27c556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#14 0x0000f89ac27c6e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#15 0x0000f89abf7df174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&, unsigned long&, unsigned long&) IRExecutionUnit.cpp:0:0
#16 0x0000f89ac16c8338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&, unsigned long&, std::shared_ptr<lldb_private::IRExecutionUnit>&, lldb_private::ExecutionContext&, bool&, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#17 0x0000f89abf7ddb50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&, unsigned long&, std::shared_ptr<lldb_private::IRExecutionUnit>&, lldb_private::ExecutionContext&, bool&, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#18 0x0000f89ac17009f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&, lldb_private::ExecutionContext&, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#19 0x0000f89ac1701028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&, lldb_private::ExecutionContext&, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#20 0x0000f89abf3c4908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&, lldb_private::EvaluateExpressionOptions const&, llvm::StringRef, llvm::StringRef, std::shared_ptr<lldb_private::Valu
eObject>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#21 0x0000f89abf554118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr<lldb_private::ValueObject>&, lldb_private::EvaluateExpressionOptions const&,
 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, lldb_private::ValueObject*) Target.cpp:0:0
#22 0x0000f89ac28027d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&) CommandObjectDWIMPrint.cpp:0:0
#23 0x0000f89abf41e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&) CommandObject.cpp:0:0
#24 0x0000f89abf40cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&, bool) CommandInterpreter.cpp:0:0
#25 0x0000f89abf411e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) CommandInterpreter.cpp:0
:0
#26 0x0000f89abf32d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#27 0x0000f89abf2fe2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#28 0x0000f89abf413a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&) CommandInterpreter.cpp:0:0
#29 0x0000f89abf0d0a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#30 0x0000acef75f631f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#31 0x0000acef75f63de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#32 0x0000f89ab8fc7400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x0000f89ab8fc74d8 call_init ./csu/../csu/libc-start.c:128:20
#34 0x0000f89ab8fc74d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#35 0x0000acef75f603f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
The allocate memory packet (https://lldb.llvm.org/resources/lldbgdbremote.html#m-size-permissions) fails. This is expected. What's not is that we seem to still try to JIT but using the WASM target for the host? (host is AArch64 in my case).

Usually we would fall back to the interpreter if we can't allocate target memory.

So, let's see what happens when iwasm can allocate memory. Change the example and recompile:
```
#include <stddef.h>

void *malloc(size_t size) {
  #define MEMSIZE 0x85000
  static char some_memory[MEMSIZE];
  static char* bump_ptr = some_memory;
  if ((bump_ptr + size) <= (some_memory+MEMSIZE)) {
    char* ret = bump_ptr;
    bump_ptr += size;
    return ret;
  }

  __builtin_trap();
}

void free(void *ptr) {}

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j); 
}
```
The amount of memory in the allocator comes from a magic number in WAMR:
```
#if WASM_ENABLE_DEBUG_INTERP != 0
#ifndef DEBUG_EXECUTION_MEMORY_SIZE
/* 0x85000 is the size required by lldb, if this is changed to a smaller value,
 * then the debugger will not be able to evaluate user expressions, other
 * functionality such as breakpoint and stepping are not influenced by this */
#define DEBUG_EXECUTION_MEMORY_SIZE 0x85000
#endif
#endif /* end of WASM_ENABLE_DEBUG_INTERP != 0 */
```
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--stack-first -Wl,--export=malloc -Wl,--export=free -O0 -g -o simple.wasm simple.c
```
This time since we are exporting a malloc/free, iwasm will call that to allocate, and we can use the stack-first layout.

Now we see that iwasm does not complain:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:28:18:295 - E64D994EF0E0]: control thread of debug object 0xb90ec941fd80 start

[02:28:18:295 - E64D994EF0E0]: Debug server listening on 127.0.0.1:1234
```
Back to lldb:
```
(lldb) p add(1, 2)
lldb             <  14> send packet: $_M1000,rwx#fa
lldb             <   9> read packet: $10010#f2
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-94c0e1
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #0 0x0000b784af1b452c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #1 0x0000b784af1b237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #2 0x0000b784af1b52f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #4 0x0000f00f25ec5b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #5 0x0000f00f2c969a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #6 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #7 0x0000f00f25e62008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #8 0x0000f00f25e1a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000f00f25e07134 abort ./stdlib/abort.c:81:7
#10 0x0000f00f2c9068ac llvm::report_fatal_error(llvm::Twine const&, bool) ErrorHandling.cpp:0:0
#11 0x0000f00f2c906690 llvm::report_fatal_error(llvm::Twine const&, bool) ErrorHandling.cpp:0:0
#12 0x0000f00f2f8e0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#13 0x0000f00f2f60556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#14 0x0000f00f2f606e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#15 0x0000f00f2c61f174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&, unsigned long&, unsigned long&) IRExecutionUnit.cpp:0:0
#16 0x0000f00f2e508338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&, unsigned long&, std::shared_ptr<lldb_private::IRExecutionUnit>&, lldb_private::ExecutionContext&, bool&, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#17 0x0000f00f2c61db50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&, unsigned long&, std::shared_ptr<lldb_private::IRExecutionUnit>&, lldb_private::ExecutionContext&, bool&, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#18 0x0000f00f2e5409f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&, lldb_private::ExecutionContext&, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#19 0x0000f00f2e541028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&, lldb_private::ExecutionContext&, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#20 0x0000f00f2c204908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&, lldb_private::EvaluateExpressionOptions const&, llvm::StringRef, llvm::StringRef, std::shared_ptr<lldb_private::Valu
eObject>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#21 0x0000f00f2c394118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr<lldb_private::ValueObject>&, lldb_private::EvaluateExpressionOptions const&,
 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, lldb_private::ValueObject*) Target.cpp:0:0
#22 0x0000f00f2f6427d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&) CommandObjectDWIMPrint.cpp:0:0
#23 0x0000f00f2c25e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&) CommandObject.cpp:0:0
#24 0x0000f00f2c24cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&, bool) CommandInterpreter.cpp:0:0
#25 0x0000f00f2c251e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) CommandInterpreter.cpp:0
:0
#26 0x0000f00f2c16d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#27 0x0000f00f2c13e2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#28 0x0000f00f2c253a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&) CommandInterpreter.cpp:0:0
#29 0x0000f00f2bf10a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#30 0x0000b784af1831f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#31 0x0000b784af183de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#32 0x0000f00f25e07400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x0000f00f25e074d8 call_init ./csu/../csu/libc-start.c:128:20
#34 0x0000f00f25e074d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#35 0x0000b784af1803f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
In this case we did allocate target memory but still crashed due to the JIT mismatch.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: David Spickett (DavidSpickett)

<details>
Found while following along with https://jonasdevlieghere.com/post/wasm-debugging-with-lldb-and-wamr/.

My example program:
```
#include &lt;stddef.h&gt;

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j);
}
```
Compile with:
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--no-stack-first -O0 -g -o simple.wasm simple.c
```
Note the no-stack-first. Without this, iwasm will fail to allocate the exec stack required for the debug interpreter.

Run it in iwasm with debugging enabled:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:23:53:350 - FD569AF9F020]: warning: allocate 544768 bytes memory failed
[02:23:53:350 - FD569AF9F020]: WASM Debug Engine warning: failed to allocate linear memory for execution. 
Will not be able to evaluate expressions during debugging
[02:23:53:350 - FD569AD0F0E0]: control thread of debug object 0xc405dac829d0 start

[02:23:53:350 - FD569AD0F0E0]: Debug server listening on 127.0.0.1:1234
```
Note that iwasm failed to allocate space for expression execution. This is expected in this case because the example's address map has no space and there's no fallback way for iwasm to get it.

Now connect lldb to iwasm:
```
$ lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
&lt;...&gt;
Process 1 stopped
* thread #<!-- -->1, name = 'nobody', stop reason = trace
    frame #<!-- -->0: 0x40000000000001d1 main at simple.c:7
   4   	  return a + b;
   5   	}
   6   	
-&gt; 7   	int main() {
```
Try to execute an expression:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   7&gt; read packet: $E03#a8
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-589ed7
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000acef75f9452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000acef75f9237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000acef75f952f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f89ab9085b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f89abfb29a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f89ab9022008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f89ab8fda83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f89ab8fc7134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f89abfac68ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f89abfac6690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f89ac2aa0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f89ac27c556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f89ac27c6e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f89abf7df174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f89ac16c8338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f89abf7ddb50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f89ac17009f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f89ac1701028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f89abf3c4908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f89abf554118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f89ac28027d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f89abf41e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f89abf40cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f89abf411e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f89abf32d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f89abf2fe2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f89abf413a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f89abf0d0a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000acef75f631f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000acef75f63de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f89ab8fc7400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f89ab8fc74d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f89ab8fc74d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000acef75f603f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
The allocate memory packet (https://lldb.llvm.org/resources/lldbgdbremote.html#m-size-permissions) fails. This is expected. What's not is that we seem to still try to JIT but using the WASM target for the host? (host is AArch64 in my case).

Usually we would fall back to the interpreter if we can't allocate target memory.

So, let's see what happens when iwasm can allocate memory. Change the example and recompile:
```
#include &lt;stddef.h&gt;

void *malloc(size_t size) {
  #define MEMSIZE 0x85000
  static char some_memory[MEMSIZE];
  static char* bump_ptr = some_memory;
  if ((bump_ptr + size) &lt;= (some_memory+MEMSIZE)) {
    char* ret = bump_ptr;
    bump_ptr += size;
    return ret;
  }

  __builtin_trap();
}

void free(void *ptr) {}

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j); 
}
```
The amount of memory in the allocator comes from a magic number in WAMR:
```
#if WASM_ENABLE_DEBUG_INTERP != 0
#ifndef DEBUG_EXECUTION_MEMORY_SIZE
/* 0x85000 is the size required by lldb, if this is changed to a smaller value,
 * then the debugger will not be able to evaluate user expressions, other
 * functionality such as breakpoint and stepping are not influenced by this */
#define DEBUG_EXECUTION_MEMORY_SIZE 0x85000
#endif
#endif /* end of WASM_ENABLE_DEBUG_INTERP != 0 */
```
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--stack-first -Wl,--export=malloc -Wl,--export=free -O0 -g -o simple.wasm simple.c
```
This time since we are exporting a malloc/free, iwasm will call that to allocate, and we can use the stack-first layout.

Now we see that iwasm does not complain:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:28:18:295 - E64D994EF0E0]: control thread of debug object 0xb90ec941fd80 start

[02:28:18:295 - E64D994EF0E0]: Debug server listening on 127.0.0.1:1234
```
Back to lldb:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   9&gt; read packet: $10010#f2
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-94c0e1
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000b784af1b452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000b784af1b237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000b784af1b52f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f00f25ec5b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f00f2c969a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f00f25e62008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f00f25e1a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f00f25e07134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f00f2c9068ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f00f2c906690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f00f2f8e0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f00f2f60556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f00f2f606e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f00f2c61f174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f00f2e508338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f00f2c61db50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f00f2e5409f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f00f2e541028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f00f2c204908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f00f2c394118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f00f2f6427d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f00f2c25e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f00f2c24cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f00f2c251e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f00f2c16d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f00f2c13e2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f00f2c253a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f00f2bf10a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000b784af1831f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000b784af183de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f00f25e07400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f00f25e074d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f00f25e074d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000b784af1803f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
In this case we did allocate target memory but still crashed due to the JIT mismatch.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: David Spickett (DavidSpickett)

<details>
Found while following along with https://jonasdevlieghere.com/post/wasm-debugging-with-lldb-and-wamr/.

My example program:
```
#include &lt;stddef.h&gt;

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j);
}
```
Compile with:
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--no-stack-first -O0 -g -o simple.wasm simple.c
```
Note the no-stack-first. Without this, iwasm will fail to allocate the exec stack required for the debug interpreter.

Run it in iwasm with debugging enabled:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:23:53:350 - FD569AF9F020]: warning: allocate 544768 bytes memory failed
[02:23:53:350 - FD569AF9F020]: WASM Debug Engine warning: failed to allocate linear memory for execution. 
Will not be able to evaluate expressions during debugging
[02:23:53:350 - FD569AD0F0E0]: control thread of debug object 0xc405dac829d0 start

[02:23:53:350 - FD569AD0F0E0]: Debug server listening on 127.0.0.1:1234
```
Note that iwasm failed to allocate space for expression execution. This is expected in this case because the example's address map has no space and there's no fallback way for iwasm to get it.

Now connect lldb to iwasm:
```
$ lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
&lt;...&gt;
Process 1 stopped
* thread #<!-- -->1, name = 'nobody', stop reason = trace
    frame #<!-- -->0: 0x40000000000001d1 main at simple.c:7
   4   	  return a + b;
   5   	}
   6   	
-&gt; 7   	int main() {
```
Try to execute an expression:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   7&gt; read packet: $E03#a8
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-589ed7
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000acef75f9452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000acef75f9237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000acef75f952f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f89ab9085b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f89abfb29a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f89ab9022008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f89ab8fda83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f89ab8fc7134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f89abfac68ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f89abfac6690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f89ac2aa0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f89ac27c556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f89ac27c6e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f89abf7df174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f89ac16c8338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f89abf7ddb50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f89ac17009f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f89ac1701028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f89abf3c4908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f89abf554118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f89ac28027d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f89abf41e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f89abf40cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f89abf411e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f89abf32d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f89abf2fe2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f89abf413a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f89abf0d0a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000acef75f631f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000acef75f63de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f89ab8fc7400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f89ab8fc74d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f89ab8fc74d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000acef75f603f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
The allocate memory packet (https://lldb.llvm.org/resources/lldbgdbremote.html#m-size-permissions) fails. This is expected. What's not is that we seem to still try to JIT but using the WASM target for the host? (host is AArch64 in my case).

Usually we would fall back to the interpreter if we can't allocate target memory.

So, let's see what happens when iwasm can allocate memory. Change the example and recompile:
```
#include &lt;stddef.h&gt;

void *malloc(size_t size) {
  #define MEMSIZE 0x85000
  static char some_memory[MEMSIZE];
  static char* bump_ptr = some_memory;
  if ((bump_ptr + size) &lt;= (some_memory+MEMSIZE)) {
    char* ret = bump_ptr;
    bump_ptr += size;
    return ret;
  }

  __builtin_trap();
}

void free(void *ptr) {}

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j); 
}
```
The amount of memory in the allocator comes from a magic number in WAMR:
```
#if WASM_ENABLE_DEBUG_INTERP != 0
#ifndef DEBUG_EXECUTION_MEMORY_SIZE
/* 0x85000 is the size required by lldb, if this is changed to a smaller value,
 * then the debugger will not be able to evaluate user expressions, other
 * functionality such as breakpoint and stepping are not influenced by this */
#define DEBUG_EXECUTION_MEMORY_SIZE 0x85000
#endif
#endif /* end of WASM_ENABLE_DEBUG_INTERP != 0 */
```
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--stack-first -Wl,--export=malloc -Wl,--export=free -O0 -g -o simple.wasm simple.c
```
This time since we are exporting a malloc/free, iwasm will call that to allocate, and we can use the stack-first layout.

Now we see that iwasm does not complain:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:28:18:295 - E64D994EF0E0]: control thread of debug object 0xb90ec941fd80 start

[02:28:18:295 - E64D994EF0E0]: Debug server listening on 127.0.0.1:1234
```
Back to lldb:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   9&gt; read packet: $10010#f2
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-94c0e1
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000b784af1b452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000b784af1b237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000b784af1b52f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f00f25ec5b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f00f2c969a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f00f25e62008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f00f25e1a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f00f25e07134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f00f2c9068ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f00f2c906690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f00f2f8e0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f00f2f60556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f00f2f606e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f00f2c61f174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f00f2e508338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f00f2c61db50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f00f2e5409f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f00f2e541028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f00f2c204908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f00f2c394118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f00f2f6427d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f00f2c25e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f00f2c24cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f00f2c251e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f00f2c16d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f00f2c13e2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f00f2c253a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f00f2bf10a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000b784af1831f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000b784af183de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f00f25e07400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f00f25e074d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f00f25e074d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000b784af1803f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
In this case we did allocate target memory but still crashed due to the JIT mismatch.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-bug

Author: David Spickett (DavidSpickett)

<details>
Found while following along with https://jonasdevlieghere.com/post/wasm-debugging-with-lldb-and-wamr/.

My example program:
```
#include &lt;stddef.h&gt;

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j);
}
```
Compile with:
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--no-stack-first -O0 -g -o simple.wasm simple.c
```
Note the no-stack-first. Without this, iwasm will fail to allocate the exec stack required for the debug interpreter.

Run it in iwasm with debugging enabled:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:23:53:350 - FD569AF9F020]: warning: allocate 544768 bytes memory failed
[02:23:53:350 - FD569AF9F020]: WASM Debug Engine warning: failed to allocate linear memory for execution. 
Will not be able to evaluate expressions during debugging
[02:23:53:350 - FD569AD0F0E0]: control thread of debug object 0xc405dac829d0 start

[02:23:53:350 - FD569AD0F0E0]: Debug server listening on 127.0.0.1:1234
```
Note that iwasm failed to allocate space for expression execution. This is expected in this case because the example's address map has no space and there's no fallback way for iwasm to get it.

Now connect lldb to iwasm:
```
$ lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
&lt;...&gt;
Process 1 stopped
* thread #<!-- -->1, name = 'nobody', stop reason = trace
    frame #<!-- -->0: 0x40000000000001d1 main at simple.c:7
   4   	  return a + b;
   5   	}
   6   	
-&gt; 7   	int main() {
```
Try to execute an expression:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   7&gt; read packet: $E03#a8
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-589ed7
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000acef75f9452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000acef75f9237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000acef75f952f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f89ab9085b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f89abfb29a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f89ac51d98f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f89ab9022008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f89ab8fda83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f89ab8fc7134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f89abfac68ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f89abfac6690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f89ac2aa0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f89ac27c556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f89ac27c6e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f89abf7df174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f89ac16c8338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f89abf7ddb50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f89ac17009f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f89ac1701028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f89abf3c4908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f89abf554118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f89ac28027d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f89abf41e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f89abf40cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f89abf411e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f89abf32d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f89abf2fe2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f89abf413a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f89abf0d0a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000acef75f631f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000acef75f63de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f89ab8fc7400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f89ab8fc74d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f89ab8fc74d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000acef75f603f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
The allocate memory packet (https://lldb.llvm.org/resources/lldbgdbremote.html#m-size-permissions) fails. This is expected. What's not is that we seem to still try to JIT but using the WASM target for the host? (host is AArch64 in my case).

Usually we would fall back to the interpreter if we can't allocate target memory.

So, let's see what happens when iwasm can allocate memory. Change the example and recompile:
```
#include &lt;stddef.h&gt;

void *malloc(size_t size) {
  #define MEMSIZE 0x85000
  static char some_memory[MEMSIZE];
  static char* bump_ptr = some_memory;
  if ((bump_ptr + size) &lt;= (some_memory+MEMSIZE)) {
    char* ret = bump_ptr;
    bump_ptr += size;
    return ret;
  }

  __builtin_trap();
}

void free(void *ptr) {}

int add(int a, int b) {
  return a + b;
}

int main() {
  int i = 1;
  int j = 2;
  return add(i, j); 
}
```
The amount of memory in the allocator comes from a magic number in WAMR:
```
#if WASM_ENABLE_DEBUG_INTERP != 0
#ifndef DEBUG_EXECUTION_MEMORY_SIZE
/* 0x85000 is the size required by lldb, if this is changed to a smaller value,
 * then the debugger will not be able to evaluate user expressions, other
 * functionality such as breakpoint and stepping are not influenced by this */
#define DEBUG_EXECUTION_MEMORY_SIZE 0x85000
#endif
#endif /* end of WASM_ENABLE_DEBUG_INTERP != 0 */
```
```
$ clang -target wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--stack-first -Wl,--export=malloc -Wl,--export=free -O0 -g -o simple.wasm simple.c
```
This time since we are exporting a malloc/free, iwasm will call that to allocate, and we can use the stack-first layout.

Now we see that iwasm does not complain:
```
$ ./build/iwasm --heap-size=1024 -g=127.0.0.1:1234 simple.wasm 
[02:28:18:295 - E64D994EF0E0]: control thread of debug object 0xb90ec941fd80 start

[02:28:18:295 - E64D994EF0E0]: Debug server listening on 127.0.0.1:1234
```
Back to lldb:
```
(lldb) p add(1, 2)
lldb             &lt;  14&gt; send packet: $_M1000,rwx#fa
lldb             &lt;   9&gt; read packet: $10010#f2
WARNING: This target JIT is not designed for the host you are running.  If bad things happen, please choose a different -march switch.
LLVM ERROR: Incompatible object format!
LLDB diagnostics will be written to /tmp/diagnostics-94c0e1
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /home/davspi01/build-llvm-aarch64/bin/lldb -o "log enable gdb-remote packets" -o "process connect --plugin wasm connect://127.0.0.1:1234"
 #<!-- -->0 0x0000b784af1b452c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7452c)
 #<!-- -->1 0x0000b784af1b237c llvm::sys::RunSignalHandlers() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x7237c)
 #<!-- -->2 0x0000b784af1b52f0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f00f25ec5b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f00f2c969a48 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f00f320118f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f00f25e62008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f00f25e1a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f00f25e07134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f00f2c9068ac llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->11 0x0000f00f2c906690 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ErrorHandling.cpp:0:0
#<!-- -->12 0x0000f00f2f8e0280 llvm::RuntimeDyld::getSymbol(llvm::StringRef) const RuntimeDyld.cpp:0:0
#<!-- -->13 0x0000f00f2f60556c llvm::MCJIT::generateCodeForModule(llvm::Module*) MCJIT.cpp:0:0
#<!-- -->14 0x0000f00f2f606e18 llvm::MCJIT::getPointerToFunction(llvm::Function*) MCJIT.cpp:0:0
#<!-- -->15 0x0000f00f2c61f174 lldb_private::IRExecutionUnit::GetRunnableInfo(lldb_private::Status&amp;, unsigned long&amp;, unsigned long&amp;) IRExecutionUnit.cpp:0:0
#<!-- -->16 0x0000f00f2e508338 lldb_private::ClangExpressionParser::DoPrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_pri
vate::ExecutionPolicy) ClangExpressionParser.cpp:0:0
#<!-- -->17 0x0000f00f2c61db50 lldb_private::ExpressionParser::PrepareForExecution(unsigned long&amp;, unsigned long&amp;, std::shared_ptr&lt;lldb_private::IRExecutionUnit&gt;&amp;, lldb_private::ExecutionContext&amp;, bool&amp;, lldb_private::E
xecutionPolicy) ExpressionParser.cpp:0:0
#<!-- -->18 0x0000f00f2e5409f4 lldb_private::ClangUserExpression::TryParse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->19 0x0000f00f2e541028 lldb_private::ClangUserExpression::Parse(lldb_private::DiagnosticManager&amp;, lldb_private::ExecutionContext&amp;, lldb_private::ExecutionPolicy, bool, bool) ClangUserExpression.cpp:0:0
#<!-- -->20 0x0000f00f2c204908 lldb_private::UserExpression::Evaluate(lldb_private::ExecutionContext&amp;, lldb_private::EvaluateExpressionOptions const&amp;, llvm::StringRef, llvm::StringRef, std::shared_ptr&lt;lldb_private::Valu
eObject&gt;&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) UserExpression.cpp:0:0
#<!-- -->21 0x0000f00f2c394118 lldb_private::Target::EvaluateExpression(llvm::StringRef, lldb_private::ExecutionContextScope*, std::shared_ptr&lt;lldb_private::ValueObject&gt;&amp;, lldb_private::EvaluateExpressionOptions const&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, lldb_private::ValueObject*) Target.cpp:0:0
#<!-- -->22 0x0000f00f2f6427d4 lldb_private::CommandObjectDWIMPrint::DoExecute(llvm::StringRef, lldb_private::CommandReturnObject&amp;) CommandObjectDWIMPrint.cpp:0:0
#<!-- -->23 0x0000f00f2c25e93c lldb_private::CommandObjectRaw::Execute(char const*, lldb_private::CommandReturnObject&amp;) CommandObject.cpp:0:0
#<!-- -->24 0x0000f00f2c24cfb4 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->25 0x0000f00f2c251e10 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:0
:0
#<!-- -->26 0x0000f00f2c16d04c lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->27 0x0000f00f2c13e2a8 lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->28 0x0000f00f2c253a8c lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->29 0x0000f00f2bf10a08 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x59c0a08)
#<!-- -->30 0x0000b784af1831f0 Driver::MainLoop() (/home/davspi01/build-llvm-aarch64/bin/lldb+0x431f0)
#<!-- -->31 0x0000b784af183de8 main (/home/davspi01/build-llvm-aarch64/bin/lldb+0x43de8)
#<!-- -->32 0x0000f00f25e07400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x0000f00f25e074d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x0000f00f25e074d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->35 0x0000b784af1803f0 _start (/home/davspi01/build-llvm-aarch64/bin/lldb+0x403f0)
Aborted (core dumped)
```
In this case we did allocate target memory but still crashed due to the JIT mismatch.
</details>


---

### Comment 4 - DavidSpickett

> Usually we would fall back to the interpreter if we can't allocate target memory.

I guess that we in fact are trying to do this but for some reason we keep trying to use the WASM JIT rather than the host one.

---

### Comment 5 - JDevlieghere

When I last tested this, we correctly reported being unable to allocate memory. Maybe the use of "no-stack-first" explain the difference. Regardless, we shouldn't crash.

I expect expression evaluation to require more work. I haven't looked into that yet. For example, while exporting malloc and free is clever, that's allocating in linear memory, while code lives in a separate address space and (so far) I've done nothing to keep those two separe in the Wasm plugin. I'll file a top level issue and link it in https://github.com/llvm/llvm-project/issues/150449.

---

