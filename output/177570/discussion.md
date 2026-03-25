# "help breakpoint set" crashes lldb if the terminal is exactly 105 columns wide

**Author:** DavidSpickett
**URL:** https://github.com/llvm/llvm-project/issues/177570
**Status:** Closed
**Labels:** lldb, crash
**Closed Date:** 2026-02-26T11:00:54Z

## Body

LLDB built from main 4e51f9044f17a00f5c6c0f5b2abf1fdb9b8d9eeb. Using Ubuntu Ubuntu 22.04.5 LTS's default terminal.

When the width is exactly 105 columns (which happens to be exactly half my monitor), this command crashes:
```
$ tput cols
105
$ ./bin/lldb -o "help breakpoint set"
(lldb) help breakpoint set
lldb: /home/davspi01/llvm-project/lldb/source/Interpreter/Options.cpp:320: void lldb_private::Options::Ou
tputFormattedUsageText(lldb_private::Stream &, const lldb_private::OptionDefinition &, uint32_t, bool): A
ssertion `start < final_end' failed.
LLDB diagnostics will be written to /tmp/diagnostics-afa476
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrac
e.
Stack dump:
0.	Program arguments: ./bin/lldb -o "help breakpoint set"
 #0 0x0000bc5c233e43ac llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/lldb+0x743ac)
 #1 0x0000bc5c233e21fc llvm::sys::RunSignalHandlers() (./bin/lldb+0x721fc)
 #2 0x0000bc5c233e5170 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000f47b7d49f8f8 (linux-vdso.so.1+0x8f8)
 #4 0x0000f47b71405b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #5 0x0000f47b77e373ec SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #6 0x0000f47b7d49f8f8 (linux-vdso.so.1+0x8f8)
 #7 0x0000f47b713a2008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #8 0x0000f47b7135a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000f47b71347134 abort ./stdlib/abort.c:81:7
#10 0x0000f47b71354114 __assert_fail_base ./assert/assert.c:91:7
#11 0x0000f47b7135418c (/lib/aarch64-linux-gnu/libc.so.6+0x3418c)
#12 0x0000f47b777450e8 lldb_private::Options::OutputFormattedUsageText(lldb_private::Stream&, lldb_privat
e::OptionDefinition const&, unsigned int, bool) Options.cpp:0:0
#13 0x0000f47b77745a24 lldb_private::Options::GenerateOptionUsage(lldb_private::Stream&, lldb_private::Co
mmandObject&, unsigned int, bool) Options.cpp:0:0
#14 0x0000f47b7772bb5c lldb_private::CommandObject::GenerateHelpText(lldb_private::Stream&) CommandObject
.cpp:0:0
#15 0x0000f47b7772b8ec lldb_private::CommandObject::GenerateHelpText(lldb_private::CommandReturnObject&) 
CommandObject.cpp:0:0
#16 0x0000f47b7ab11e0c lldb_private::CommandObjectHelp::DoExecute(lldb_private::Args&, lldb_private::Comm
andReturnObject&) CommandObjectHelp.cpp:0:0
#17 0x0000f47b7772c468 lldb_private::CommandObjectParsed::Execute(char const*, lldb_private::CommandRetur
nObject&) CommandObject.cpp:0:0
#18 0x0000f47b7771af58 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBoo
l, lldb_private::CommandReturnObject&, bool) CommandInterpreter.cpp:0:0
#19 0x0000f47b7771fdb4 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&,
 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) CommandInterpreter.cpp:
0:0
#20 0x0000f47b7763a608 lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#21 0x0000f47b7760c42c lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#22 0x0000f47b77721a30 lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInter
preterRunOptions&) CommandInterpreter.cpp:0:0
#23 0x0000f47b773ddfac lldb::SBDebugger::RunCommandInterpreter(lldb::SBCommandInterpreterRunOptions const
&) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x595dfac)
#24 0x0000bc5c233b2efc Driver::MainLoop() (./bin/lldb+0x42efc)
#25 0x0000bc5c233b3d98 main (./bin/lldb+0x43d98)
#26 0x0000f47b71347400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x0000f47b713474d8 call_init ./csu/../csu/libc-start.c:128:20
#28 0x0000f47b713474d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#29 0x0000bc5c233b03b0 _start (./bin/lldb+0x403b0)
Aborted (core dumped)
```
When the width is not 105, it works.
```
$ tput cols
106
davspi01@AWS-ARMVM:~/build-llvm-aarch64$ ./bin/lldb -o "help breakpoint set"
Sets a breakpoint or set of breakpoints in the executable.

Syntax: breakpoint set <cmd-options>
<...>
```
The height of the console does not seem to matter. Which makes sense as the failing assert is in code that tries to break the output into multiple lines.

I wonder if the addition of ANSI codes to underline the shortcut characters has thrown off the width calculations.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: David Spickett (DavidSpickett)

<details>
LLDB built from main 4e51f9044f17a00f5c6c0f5b2abf1fdb9b8d9eeb. Using Ubuntu Ubuntu 22.04.5 LTS's default terminal.

When the width is exactly 105 columns, this crashes:
```
$ ./bin/lldb -o "help breakpoint set"
(lldb) help breakpoint set
lldb: /home/davspi01/llvm-project/lldb/source/Interpreter/Options.cpp:320: void lldb_private::Options::Ou
tputFormattedUsageText(lldb_private::Stream &amp;, const lldb_private::OptionDefinition &amp;, uint32_t, bool): A
ssertion `start &lt; final_end' failed.
LLDB diagnostics will be written to /tmp/diagnostics-afa476
Please include the directory content when filing a bug report
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrac
e.
Stack dump:
0.	Program arguments: ./bin/lldb -o "help breakpoint set"
 #<!-- -->0 0x0000bc5c233e43ac llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./bin/lldb+0x743ac)
 #<!-- -->1 0x0000bc5c233e21fc llvm::sys::RunSignalHandlers() (./bin/lldb+0x721fc)
 #<!-- -->2 0x0000bc5c233e5170 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000f47b7d49f8f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->4 0x0000f47b71405b68 syscall ./misc/../sysdeps/unix/sysv/linux/aarch64/syscall.S:39:0
 #<!-- -->5 0x0000f47b77e373ec SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->6 0x0000f47b7d49f8f8 (linux-vdso.so.1+0x8f8)
 #<!-- -->7 0x0000f47b713a2008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x0000f47b7135a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000f47b71347134 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000f47b71354114 __assert_fail_base ./assert/assert.c:91:7
#<!-- -->11 0x0000f47b7135418c (/lib/aarch64-linux-gnu/libc.so.6+0x3418c)
#<!-- -->12 0x0000f47b777450e8 lldb_private::Options::OutputFormattedUsageText(lldb_private::Stream&amp;, lldb_privat
e::OptionDefinition const&amp;, unsigned int, bool) Options.cpp:0:0
#<!-- -->13 0x0000f47b77745a24 lldb_private::Options::GenerateOptionUsage(lldb_private::Stream&amp;, lldb_private::Co
mmandObject&amp;, unsigned int, bool) Options.cpp:0:0
#<!-- -->14 0x0000f47b7772bb5c lldb_private::CommandObject::GenerateHelpText(lldb_private::Stream&amp;) CommandObject
.cpp:0:0
#<!-- -->15 0x0000f47b7772b8ec lldb_private::CommandObject::GenerateHelpText(lldb_private::CommandReturnObject&amp;) 
CommandObject.cpp:0:0
#<!-- -->16 0x0000f47b7ab11e0c lldb_private::CommandObjectHelp::DoExecute(lldb_private::Args&amp;, lldb_private::Comm
andReturnObject&amp;) CommandObjectHelp.cpp:0:0
#<!-- -->17 0x0000f47b7772c468 lldb_private::CommandObjectParsed::Execute(char const*, lldb_private::CommandRetur
nObject&amp;) CommandObject.cpp:0:0
#<!-- -->18 0x0000f47b7771af58 lldb_private::CommandInterpreter::HandleCommand(char const*, lldb_private::LazyBoo
l, lldb_private::CommandReturnObject&amp;, bool) CommandInterpreter.cpp:0:0
#<!-- -->19 0x0000f47b7771fdb4 lldb_private::CommandInterpreter::IOHandlerInputComplete(lldb_private::IOHandler&amp;,
 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) CommandInterpreter.cpp:
0:0
#<!-- -->20 0x0000f47b7763a608 lldb_private::IOHandlerEditline::Run() IOHandler.cpp:0:0
#<!-- -->21 0x0000f47b7760c42c lldb_private::Debugger::RunIOHandlers() Debugger.cpp:0:0
#<!-- -->22 0x0000f47b77721a30 lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInter
preterRunOptions&amp;) CommandInterpreter.cpp:0:0
#<!-- -->23 0x0000f47b773ddfac lldb::SBDebugger::RunCommandInterpreter(lldb::SBCommandInterpreterRunOptions const
&amp;) (/home/davspi01/build-llvm-aarch64/bin/../lib/liblldb.so.23.0git+0x595dfac)
#<!-- -->24 0x0000bc5c233b2efc Driver::MainLoop() (./bin/lldb+0x42efc)
#<!-- -->25 0x0000bc5c233b3d98 main (./bin/lldb+0x43d98)
#<!-- -->26 0x0000f47b71347400 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x0000f47b713474d8 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x0000f47b713474d8 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->29 0x0000bc5c233b03b0 _start (./bin/lldb+0x403b0)
Aborted (core dumped)
```
When the width is not 105, it works.
```
$ tput cols
106
davspi01@<!-- -->AWS-ARMVM:~/build-llvm-aarch64$ ./bin/lldb -o "help breakpoint set"
Sets a breakpoint or set of breakpoints in the executable.

Syntax: breakpoint set &lt;cmd-options&gt;
&lt;...&gt;
```
The height of the console does not seem to matter. Which makes sense as the failing assert is in code that tries to break the output into multiple lines.

I wonder if the addition of ANSI codes to underline the shortcut characters has thrown off the width calculations.
</details>


---

### Comment 2 - DavidSpickett

I think the problem is that we're doing one thing by "visible_length" after ANSI codes have been stripped, then using those indexes into the "actual_text" which still includes the ANSI codes.

```
>>>>>>>>>>
actual_text length: 101
visible_length: 93
Actual text:
>>>>>Move breakpoints to nearest code. If not set the target.move-to-nearest-code setting is used.<<<<<
```
The `>>> <<<<` are there to show that there are no spaces on either end of the string. The M of Move has an underline set on it. So we have more data than characters visible in the final string.
```
Actual text raw:
[00]: 7)
[01]: [ (91)
[02]: 4 (52)
[03]: m (109)
[04]: M (77)
[05]: 7)
[06]: [ (91)
[07]: 0 (48)
[08]: m (109)
[09]: o (111)
[10]: v (118)
[11]: e (101)
[12]:   (32)
[13]: b (98)
[14]: r (114)
[15]: e (101)
[16]: a (97)
[17]: k (107)
[18]: p (112)
[19]: o (111)
[20]: i (105)
[21]: n (110)
[22]: t (116)
[23]: s (115)
[24]:   (32)
[25]: t (116)
[26]: o (111)
[27]:   (32)
[28]: n (110)
[29]: e (101)
[30]: a (97)
[31]: r (114)
[32]: e (101)
[33]: s (115)
[34]: t (116)
[35]:   (32)
[36]: c (99)
[37]: o (111)
[38]: d (100)
[39]: e (101)
[40]: . (46)
[41]:   (32)
[42]: I (73)
[43]: f (102)
[44]:   (32)
[45]: n (110)
[46]: o (111)
[47]: t (116)
[48]:   (32)
[49]: s (115)
[50]: e (101)
[51]: t (116)
[52]:   (32)
[53]: t (116)
[54]: h (104)
[55]: e (101)
[56]:   (32)
[57]: t (116)
[58]: a (97)
[59]: r (114)
[60]: g (103)
[61]: e (101)
[62]: t (116)
[63]: . (46)
[64]: m (109)
[65]: o (111)
[66]: v (118)
[67]: e (101)
[68]: - (45)
[69]: t (116)
[70]: o (111)
[71]: - (45)
[72]: n (110)
[73]: e (101)
[74]: a (97)
[75]: r (114)
[76]: e (101)
[77]: s (115)
[78]: t (116)
[79]: - (45)
[80]: c (99)
[81]: o (111)
[82]: d (100)
[83]: e (101)
[84]:   (32)
[85]: s (115)
[86]: e (101)
[87]: t (116)
[88]: t (116)
[89]: i (105)
[90]: n (110)
[91]: g (103)
[92]:   (32)
[93]: i (105)
[94]: s (115)
[95]:   (32)
[96]: u (117)
[97]: s (115)
[98]: e (101)
[99]: d (100)
[100]: . (46)
```
The code tries to split this line to fit:
```
output_max_columns: 105
GetIndentLevel(): 12
text_width: 92
```
The goal is to print text to the terminal's with, minus one. So 105-12 = 93, minus one is 92. Each line can only be this long once rendered in the terminal.

Recall that the visible length of the current string is 93. So our first guess is to print the first 92 characters on this first line.
```
end: 92
```
Then we would prefer to end a line on a whitespace character, so we would walk backwards to find one. You'd expect this to go from the final full stop, back to "is ".

However, because we use the visible length index into the actual text containing ANSI codes, there's actually a space at that index:
```
was not at end of text
actual_text[end]: ' ' (32)
```
So we leave end at 92 and are going to attempt to print a 92 character sub-string.
```
sub_len: 92
start: 0, final_end: 93
```
That works, now start is set to end+1 and end to start plus text_width, for printing the next line. So start is 93 and end is 185.
```
end: 185
was at end of text
sub_len: 0
start: 93, final_end: 93
```
This is going to print nothing because sub_len is 0, then there is an assert that start < end, which fails.
```
lldb: /home/davspi01/llvm-project/lldb/source/Interpreter/Options.cpp:339: void lldb_private::Options::Ou
tputFormattedUsageText(lldb_private::Stream &, const lldb_private::OptionDefinition &, uint32_t, bool): A
ssertion `start < final_end' failed.
```

So the reason it only crashes with width 105 is that that makes `actual_text[end]` point to a space. Somehow that check needs to translate the index into the visible string, into an index in the string containing the ANSI codes.

We can make a stripped copy of the string to solve part of that, but we still need to print the text with the ANSI codes included.

---

### Comment 3 - DavidSpickett

If I replace that particular space with an underscore:
```
diff --git a/lldb/source/Commands/Options.td b/lldb/source/Commands/Options.td
index d96354a39b8b..2c0f8ac102bb 100644
--- a/lldb/source/Commands/Options.td
+++ b/lldb/source/Commands/Options.td
@@ -348,7 +348,7 @@ let Command = "breakpoint set" in {
         Groups<[1, 9, 12]>,
         Arg<"Boolean">,
         Desc<"${M}ove breakpoints to nearest code. If not set the "
-             "target.move-to-nearest-code setting is used.">;
+             "target.move-to-nearest-code setting_is used.">;
   def breakpoint_set_file_colon_line
       : Option<"joint-specifier", "y">,
         Group<12>,
```
We avoid the crash but the string is truncated.
```
       -m <boolean> ( --move-to-nearest-code <boolean> )
            Move breakpoints to nearest code. If not set the target.move-to-nearest-code
            setting_
```
I think this is for the same reason. Using the visible length to reason about the raw text we need to print, which is larger.

---

### Comment 4 - DavidSpickett

Last changed by @JDevlieghere in 2498f0bcf5ddb08da7290d7bf211120236627b81.

Istr you have fixed this sort of confusion in the past. It's equivalent to assuming that the 2nd unicode character would always be at byte 2, I think you had to handle that for the status bar.

Any suggestions here? A conversion table between "visible character index" and "original data index" is my first thought but maybe there's a simpler way.

---

### Comment 5 - DavidSpickett

I see that `StripAnsiTerminalCodes` is able to find sequences using `FindNextAnsiSequence`. So I could use that to maintain an index into the original data as the loop runs.

---

### Comment 6 - DavidSpickett

> We avoid the crash but the string is truncated.

Without any changes I can see some options being cut off at other terminal widths too. So it's not just the space check that's going wrong:
```
       -s <shlib-name> ( --shlib <shlib-name> )
            Set the breakpoint only in this shared library.  Can repeat this option
            multiple times to specify multiple shared li

       -t <thread-id> ( --thread-id <thread-id> )
            The breakpoint stops only for the thread whose TID matches this argument. 
            The token 'current' resolves to the current thre
```

---

### Comment 7 - DavidSpickett

I'll work on a patch to fix the problems.

---

### Comment 8 - JDevlieghere

Thanks for the investigation David. I think your hunch is correct that we're confusing visible length and string length. Ideally, everywhere we manipulate strings for printing, we should go through a helper function that are ANSI and Unicode aware. We already have some helpers for that in `AnsiTerminal.h` but I keep finding places where those are not used, which I bet is the case here too.

---

