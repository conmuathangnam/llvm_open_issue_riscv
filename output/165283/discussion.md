# Error: "Assertion `!m_exe_ctx.GetTargetPtr()' failed." after I attempted to disassemble a C function in lldb

**Author:** RedHotRojo
**URL:** https://github.com/llvm/llvm-project/issues/165283
**Status:** Open
**Labels:** lldb, crash

## Body

I got this error when I entered "di" in lldb to disassemble a function. I've disassembled it successfully before, but I encountered an error this time. It told me to report, so here I am.
```
lldb: /tmp/cssw/Clang/15.0.5/GCC-11.4.0/llvm-project-15.0.5.src/lldb/source/Interpreter/CommandObject.cpp:153: bool lldb_private::CommandObject::CheckRequirements(lldb_private::CommandReturnObject &): Assertion `!m_exe_ctx.GetTargetPtr()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: lldb box
 #0 0x000056286b7cf5e3 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x2b5e3)
 #1 0x000056286b7cd6c0 llvm::sys::RunSignalHandlers() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x296c0)
 #2 0x000056286b7cfc2a SignalHandler(int) Signals.cpp:0:0
 #3 0x00007f3a7e3f0520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f3a7e4449fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f3a7e4449fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f3a7e4449fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f3a7e3f0476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f3a7e3d67f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f3a7e3d671b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f3a7e3e7e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x00007f3a807b2ead lldb_private::CommandObject::CheckRequirements(lldb_private::CommandReturnObject&) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3dead)
#12 0x00007f3a807b4979 lldb_private::CommandObjectParsed::Execute(char const*, lldb_private::CommandReturnObject&) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3f979)
#13 0x00007f3a807a83ad lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb333ad)
#14 0x00007f3a807ac5ed lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb375ed)
#15 0x00007f3a806f0e82 lldb_private::IOHandlerEditline::Run() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xa7be82)
#16 0x00007f3a806ce3ae lldb_private::Debugger::RunIOHandlers() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xa593ae)
#17 0x00007f3a807ae84d lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3984d)
#18 0x00007f3a8059c3a6 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0x9273a6)
#19 0x000056286b7b1dfe Driver::MainLoop() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0xddfe)
#20 0x000056286b7b30fc main (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0xf0fc)
#21 0x00007f3a7e3d7d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#22 0x00007f3a7e3d7e40 call_init ./csu/../csu/libc-start.c:128:20
#23 0x00007f3a7e3d7e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#24 0x000056286b7ade55 _start (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x9e55)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Rohit Gayali (RedHotRojo)

<details>
I got this error when I entered "di" in lldb to disassemble a function. I've disassembled it successfully before, but I encountered an error this time. It told me to report, so here I am.
```
lldb: /tmp/cssw/Clang/15.0.5/GCC-11.4.0/llvm-project-15.0.5.src/lldb/source/Interpreter/CommandObject.cpp:153: bool lldb_private::CommandObject::CheckRequirements(lldb_private::CommandReturnObject &amp;): Assertion `!m_exe_ctx.GetTargetPtr()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: lldb box
 #<!-- -->0 0x000056286b7cf5e3 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x2b5e3)
 #<!-- -->1 0x000056286b7cd6c0 llvm::sys::RunSignalHandlers() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x296c0)
 #<!-- -->2 0x000056286b7cfc2a SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x00007f3a7e3f0520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f3a7e4449fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f3a7e4449fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f3a7e4449fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f3a7e3f0476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f3a7e3d67f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f3a7e3d671b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f3a7e3e7e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x00007f3a807b2ead lldb_private::CommandObject::CheckRequirements(lldb_private::CommandReturnObject&amp;) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3dead)
#<!-- -->12 0x00007f3a807b4979 lldb_private::CommandObjectParsed::Execute(char const*, lldb_private::CommandReturnObject&amp;) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3f979)
#<!-- -->13 0x00007f3a807a83ad lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBool, lldb_private::CommandReturnObject&amp;) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb333ad)
#<!-- -->14 0x00007f3a807ac5ed lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb375ed)
#<!-- -->15 0x00007f3a806f0e82 lldb_private::IOHandlerEditline::Run() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xa7be82)
#<!-- -->16 0x00007f3a806ce3ae lldb_private::Debugger::RunIOHandlers() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xa593ae)
#<!-- -->17 0x00007f3a807ae84d lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&amp;) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0xb3984d)
#<!-- -->18 0x00007f3a8059c3a6 lldb::SBDebugger::RunCommandInterpreter(bool, bool) (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/lib/liblldb.so.15+0x9273a6)
#<!-- -->19 0x000056286b7b1dfe Driver::MainLoop() (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0xddfe)
#<!-- -->20 0x000056286b7b30fc main (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0xf0fc)
#<!-- -->21 0x00007f3a7e3d7d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->22 0x00007f3a7e3d7e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->23 0x00007f3a7e3d7e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->24 0x000056286b7ade55 _start (/sw/ubuntu2204/ebu082025/software/common/compiler/gcc/11.4.0/clang/15.0.5/bin/lldb+0x9e55)
Aborted
```
</details>


---

### Comment 2 - jimingham

This is asserting because you are running a command with no selected target.  We'd need to see the debug session leading up to this assert to see how that happened.

---

