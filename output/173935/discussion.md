# [clang-format] using object-like macro as function-like macro leads to a crash

**Author:** tesselslate
**URL:** https://github.com/llvm/llvm-project/issues/173935
**Status:** Open
**Labels:** clang-format, crash

## Body

Defining an object-like macro with the `Macros` option in `.clang-format` but invoking it as a function-like macro causes clang-format to crash. (tested on 8e90208)

`.clang-format`:
```yaml
Macros:
  - X=c
```

`test.c`:
```c
void a(X(c) b) {}
```

<details>
<summary>Log with -debug:</summary>

```
Args: bin/clang-format /tmp/c/test2.c -debug 
Trying /tmp/c/.clang-format...
Using configuration file /tmp/c/.clang-format
File encoding: UTF8
Language: C
----
Next [0] Token: void / void, Macro: 0
Next [1] Token: identifier / a, Macro: 0
Next [2] Token: l_paren / (, Macro: 0
Next [3] Token: identifier / X, Macro: 0
Getting Position: 3
Next [4] Token: l_paren / (, Macro: 0
Getting Position: 4
Next [5] Token: identifier / c, Macro: 0
Next [6] Token: r_paren / ), Macro: 0
Next [7] Token: identifier / b, Macro: 0
Macro call: X((c )
Macro "X" not overloaded for arity 1or not function-like, using object-like overload.Setting Position: 3
Next [4] Token: l_paren / (, Macro: 0
Inserting:
  [12] Token: identifier / c, Macro: 1
  [13] Token: eof / , Macro: 1
  Jump from: 13 -> 4
At inserted token [12] Token: identifier / c, Macro: 1
Expanded: c  
Next [4] Token: l_paren / (, Macro: 0
Next [5] Token: identifier / c, Macro: 0
Next [6] Token: r_paren / ), Macro: 0
Next [7] Token: identifier / b, Macro: 0
Next [8] Token: r_paren / ), Macro: 0
Next [9] Token: l_brace / {, Macro: 0
Peeking [10] Token: r_brace / }, Macro: 0
Getting Position: 9
Next [10] Token: r_brace / }, Macro: 0
Next [11] Token: eof / , Macro: 0
Setting Position: 9
Peeking [10] Token: r_brace / }, Macro: 0
Next [10] Token: r_brace / }, Macro: 0
Adding unwrapped line:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=2, "c"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
MCR: new line...
MCR: Token: void, Parent: <null>, First: 1
ParentMap:
MCR: New parent: <null>
-> void
MCR: Token: a, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> a
MCR: Token: (, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> (
MCR: Token: b, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> b
MCR: Token: c, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> X
MCR: Token: (, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> (
MCR: Token: c, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> c
MCR: Token: ), Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> )
MCR: Token: b, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> b
MCR: Token: ), Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> )
MCR: Token: {, Parent: <null>, First: 0
ParentMap:
MCR: New parent: <null>
-> {
Finalizing reconstructed lines:

 void a ( b X ( c ) b ) { 

Adding unexpanded line:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=7, "X"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Next [11] Token: eof / , Macro: 0
Adding unwrapped line:
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Adding unwrapped line:
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Expanded lines:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=2, "c"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Unwrapped lines:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=7, "X"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Run 0...
clang-format: /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:309: bool clang::format::{anonymous}::AnnotatingParser::parseParens(bool): Assertion `OpeningParen.is(tok::l_paren)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: bin/clang-format /tmp/c/test2.c -debug
 #0 0x0000557e09a92a26 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:22
 #1 0x0000557e09a92e9d PrintStackTraceSignalHandler(void*) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x0000557e09a90115 llvm::sys::RunSignalHandlers() /home/dog/clones/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #3 0x0000557e09a92293 SignalHandler(int, siginfo_t*, void*) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #4 0x00007f3cee43e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #5 0x00007f3cee49890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #6 0x00007f3cee43e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007f3cee42557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #8 0x00007f3cee4254e3 __assert_perror_fail /usr/src/debug/glibc/glibc/assert/assert-perr.c:31:1
 #9 0x0000557e09bdb3dd clang::format::(anonymous namespace)::AnnotatingParser::parseParens(bool) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:310:69
#10 0x0000557e09be0a22 clang::format::(anonymous namespace)::AnnotatingParser::consumeToken() /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:1569:11
#11 0x0000557e09be29b1 clang::format::(anonymous namespace)::AnnotatingParser::parseLine() /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:2057:11
#12 0x0000557e09be8ba4 clang::format::TokenAnnotator::annotate(clang::format::AnnotatedLine&) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:3810:13
#13 0x0000557e09bd1dc2 clang::format::TokenAnalyzer::process(bool) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnalyzer.cpp:120:5
#14 0x0000557e09b6e90e clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)::operator()(clang::format::Environment const&) const /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4092:16
#15 0x0000557e09b7b6d2 std::pair<clang::tooling::Replacements, unsigned int> std::__invoke_impl<std::pair<clang::tooling::Replacements, unsigned int>, clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)&, clang::format::Environment const&>(std::__invoke_other, clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)&, clang::format::Environment const&) /usr/include/c++/15.2.1/bits/invoke.h:63:70
#16 0x0000557e09b79621 std::enable_if<is_invocable_r_v<std::pair<clang::tooling::Replacements, unsigned int>, clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)&, clang::format::Environment const&>, std::pair<clang::tooling::Replacements, unsigned int>>::type std::__invoke_r<std::pair<clang::tooling::Replacements, unsigned int>, clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)&, clang::format::Environment const&>(clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)&, clang::format::Environment const&) /usr/include/c++/15.2.1/bits/invoke.h:119:5
#17 0x0000557e09b77b48 std::_Function_handler<std::pair<clang::tooling::Replacements, unsigned int> (clang::format::Environment const&), clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&)>::_M_invoke(std::_Any_data const&, clang::format::Environment const&) /usr/include/c++/15.2.1/bits/std_function.h:294:7
#18 0x0000557e09b98a9b std::function<std::pair<clang::tooling::Replacements, unsigned int> (clang::format::Environment const&)>::operator()(clang::format::Environment const&) const /usr/include/c++/15.2.1/bits/std_function.h:594:7
#19 0x0000557e09b6f998 clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*) /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4140:9
#20 0x0000557e09b6ff73 clang::format::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, llvm::StringRef, clang::format::FormattingAttemptStatus*) /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4182:28
#21 0x0000557e09993f8e clang::format::format(llvm::StringRef, bool) /home/dog/clones/llvm-project/clang/tools/clang-format/ClangFormat.cpp:504:76
#22 0x0000557e09995821 main /home/dog/clones/llvm-project/clang/tools/clang-format/ClangFormat.cpp:731:11
#23 0x00007f3cee427635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#24 0x00007f3cee4276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#25 0x00007f3cee4276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#26 0x0000557e09991d85 _start (bin/clang-format+0x111d85)
```

</details>

I tried to debug this myself for a little while, but I'm not too familiar with C++ specifically or the internals of clang-format. I did notice in the log that the macro expansion causes an extra `b` to appear, but I'm not sure why exactly that is or if/how it relates to the crash.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-format

Author: None (tesselslate)

<details>
Defining an object-like macro with the `Macros` option in `.clang-format` but invoking it as a function-like macro causes clang-format to crash. (tested on 8e90208)

`.clang-format`:
```yaml
Macros:
  - X=c
```

`test.c`:
```c
void a(X(c) b) {}
```

&lt;details&gt;
&lt;summary&gt;Log with -debug:&lt;/summary&gt;

```
Args: bin/clang-format /tmp/c/test2.c -debug 
Trying /tmp/c/.clang-format...
Using configuration file /tmp/c/.clang-format
File encoding: UTF8
Language: C
----
Next [0] Token: void / void, Macro: 0
Next [1] Token: identifier / a, Macro: 0
Next [2] Token: l_paren / (, Macro: 0
Next [3] Token: identifier / X, Macro: 0
Getting Position: 3
Next [4] Token: l_paren / (, Macro: 0
Getting Position: 4
Next [5] Token: identifier / c, Macro: 0
Next [6] Token: r_paren / ), Macro: 0
Next [7] Token: identifier / b, Macro: 0
Macro call: X((c )
Macro "X" not overloaded for arity 1or not function-like, using object-like overload.Setting Position: 3
Next [4] Token: l_paren / (, Macro: 0
Inserting:
  [12] Token: identifier / c, Macro: 1
  [13] Token: eof / , Macro: 1
  Jump from: 13 -&gt; 4
At inserted token [12] Token: identifier / c, Macro: 1
Expanded: c  
Next [4] Token: l_paren / (, Macro: 0
Next [5] Token: identifier / c, Macro: 0
Next [6] Token: r_paren / ), Macro: 0
Next [7] Token: identifier / b, Macro: 0
Next [8] Token: r_paren / ), Macro: 0
Next [9] Token: l_brace / {, Macro: 0
Peeking [10] Token: r_brace / }, Macro: 0
Getting Position: 9
Next [10] Token: r_brace / }, Macro: 0
Next [11] Token: eof / , Macro: 0
Setting Position: 9
Peeking [10] Token: r_brace / }, Macro: 0
Next [10] Token: r_brace / }, Macro: 0
Adding unwrapped line:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=2, "c"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
MCR: new line...
MCR: Token: void, Parent: &lt;null&gt;, First: 1
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; void
MCR: Token: a, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; a
MCR: Token: (, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; (
MCR: Token: b, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; b
MCR: Token: c, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; X
MCR: Token: (, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; (
MCR: Token: c, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; c
MCR: Token: ), Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; )
MCR: Token: b, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; b
MCR: Token: ), Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; )
MCR: Token: {, Parent: &lt;null&gt;, First: 0
ParentMap:
MCR: New parent: &lt;null&gt;
-&gt; {
Finalizing reconstructed lines:

 void a ( b X ( c ) b ) { 

Adding unexpanded line:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=7, "X"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Next [11] Token: eof / , Macro: 0
Adding unwrapped line:
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Adding unwrapped line:
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Expanded lines:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=2, "c"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Unwrapped lines:
Line(0, FSC=0): void[T=168, OC=0, "void"] identifier[T=168, OC=5, "a"] l_paren[T=168, OC=6, "("] identifier[T=168, OC=12, "b"] identifier[T=168, OC=7, "X"] l_paren[T=168, OC=8, "("] identifier[T=168, OC=9, "c"] r_paren[T=168, OC=10, ")"] identifier[T=168, OC=12, "b"] r_paren[T=168, OC=13, ")"] l_brace[T=52, OC=15, "{"] 
Line(0, FSC=0): r_brace[T=168, OC=16, "}"] 
Line(0, FSC=0): eof[T=168, OC=0, ""] 
Run 0...
clang-format: /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:309: bool clang::format::{anonymous}::AnnotatingParser::parseParens(bool): Assertion `OpeningParen.is(tok::l_paren)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: bin/clang-format /tmp/c/test2.c -debug
 #<!-- -->0 0x0000557e09a92a26 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:22
 #<!-- -->1 0x0000557e09a92e9d PrintStackTraceSignalHandler(void*) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x0000557e09a90115 llvm::sys::RunSignalHandlers() /home/dog/clones/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->3 0x0000557e09a92293 SignalHandler(int, siginfo_t*, void*) /home/dog/clones/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #<!-- -->4 0x00007f3cee43e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->5 0x00007f3cee49890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f3cee43e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->7 0x00007f3cee42557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #<!-- -->8 0x00007f3cee4254e3 __assert_perror_fail /usr/src/debug/glibc/glibc/assert/assert-perr.c:31:1
 #<!-- -->9 0x0000557e09bdb3dd clang::format::(anonymous namespace)::AnnotatingParser::parseParens(bool) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:310:69
#<!-- -->10 0x0000557e09be0a22 clang::format::(anonymous namespace)::AnnotatingParser::consumeToken() /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:1569:11
#<!-- -->11 0x0000557e09be29b1 clang::format::(anonymous namespace)::AnnotatingParser::parseLine() /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:2057:11
#<!-- -->12 0x0000557e09be8ba4 clang::format::TokenAnnotator::annotate(clang::format::AnnotatedLine&amp;) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnnotator.cpp:3810:13
#<!-- -->13 0x0000557e09bd1dc2 clang::format::TokenAnalyzer::process(bool) /home/dog/clones/llvm-project/clang/lib/Format/TokenAnalyzer.cpp:120:5
#<!-- -->14 0x0000557e09b6e90e clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)::operator()(clang::format::Environment const&amp;) const /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4092:16
#<!-- -->15 0x0000557e09b7b6d2 std::pair&lt;clang::tooling::Replacements, unsigned int&gt; std::__invoke_impl&lt;std::pair&lt;clang::tooling::Replacements, unsigned int&gt;, clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&amp;, clang::format::Environment const&amp;&gt;(std::__invoke_other, clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&amp;, clang::format::Environment const&amp;) /usr/include/c++/15.2.1/bits/invoke.h:63:70
#<!-- -->16 0x0000557e09b79621 std::enable_if&lt;is_invocable_r_v&lt;std::pair&lt;clang::tooling::Replacements, unsigned int&gt;, clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&amp;, clang::format::Environment const&amp;&gt;, std::pair&lt;clang::tooling::Replacements, unsigned int&gt;&gt;::type std::__invoke_r&lt;std::pair&lt;clang::tooling::Replacements, unsigned int&gt;, clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&amp;, clang::format::Environment const&amp;&gt;(clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&amp;, clang::format::Environment const&amp;) /usr/include/c++/15.2.1/bits/invoke.h:119:5
#<!-- -->17 0x0000557e09b77b48 std::_Function_handler&lt;std::pair&lt;clang::tooling::Replacements, unsigned int&gt; (clang::format::Environment const&amp;), clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*)::'lambda6'(clang::format::Environment const&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, clang::format::Environment const&amp;) /usr/include/c++/15.2.1/bits/std_function.h:294:7
#<!-- -->18 0x0000557e09b98a9b std::function&lt;std::pair&lt;clang::tooling::Replacements, unsigned int&gt; (clang::format::Environment const&amp;)&gt;::operator()(clang::format::Environment const&amp;) const /usr/include/c++/15.2.1/bits/std_function.h:594:7
#<!-- -->19 0x0000557e09b6f998 clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*) /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4140:9
#<!-- -->20 0x0000557e09b6ff73 clang::format::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, llvm::StringRef, clang::format::FormattingAttemptStatus*) /home/dog/clones/llvm-project/clang/lib/Format/Format.cpp:4182:28
#<!-- -->21 0x0000557e09993f8e clang::format::format(llvm::StringRef, bool) /home/dog/clones/llvm-project/clang/tools/clang-format/ClangFormat.cpp:504:76
#<!-- -->22 0x0000557e09995821 main /home/dog/clones/llvm-project/clang/tools/clang-format/ClangFormat.cpp:731:11
#<!-- -->23 0x00007f3cee427635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->24 0x00007f3cee4276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->25 0x00007f3cee4276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->26 0x0000557e09991d85 _start (bin/clang-format+0x111d85)
```

&lt;/details&gt;

I tried to debug this myself for a little while, but I'm not too familiar with C++ specifically or the internals of clang-format. I did notice in the log that the macro expansion causes an extra `b` to appear, but I'm not sure why exactly that is or if/how it relates to the crash.
</details>


---

