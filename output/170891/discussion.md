# lldb: Crash in `RunCommandInterpreter` When Using `force_io_methods=True` in Python API (Regression in LLDB 21)

**Author:** patryk4815
**URL:** https://github.com/llvm/llvm-project/issues/170891
**Status:** Closed
**Labels:** lldb, crash
**Closed Date:** 2025-12-11T17:24:05Z

## Body

Starting with **LLDB 21**, calling `RunCommandInterpreter` crashes when
the debugger's output and error streams are initialized using
`lldb.SBFile.Create(..., force_io_methods=True)` in Python.

The same code works correctly on **LLDB 20**.

Removing `force_io_methods=True` (or explicitly setting it to `False`)
avoids the crash, indicating a regression in LLDB's handling of `SBFile`
IO redirection.

Probably regression is introduced by:\
https://github.com/llvm/llvm-project/commit/58279d1ee1b567e8ca793d6d1eb6e0f1d5e7279e

------------------------------------------------------------------------

## Affected Versions

-   **LLDB 20** -- OK
-   **LLDB 21** -- **CRASH** when invoking `RunCommandInterpreter`

Tested on: 
- **Fedora 42** (lldb-20) --- works 
- **Fedora 43** (lldb-21) --- crashes

Install using:
```bash   
dnf install lldb -y
```

------------------------------------------------------------------------

## Minimal Reproduction

``` python
import lldb
import sys

lldb.SBDebugger.Initialize()
debugger = lldb.SBDebugger.Create()

debugger.SetOutputFile(lldb.SBFile.Create(sys.stdout, borrow=True, force_io_methods=True))
debugger.SetErrorFile(lldb.SBFile.Create(sys.stderr, borrow=True, force_io_methods=True))

# CRASHES on LLDB 21
debugger.RunCommandInterpreter(
    True, False,
    lldb.SBCommandInterpreterRunOptions(),
    0, False, False
)
```

### Notes

-   If `force_io_methods=True` is removed or set to `False`, **no crash occurs**.
-   The crash happens immediately upon entering `RunCommandInterpreter`.

------------------------------------------------------------------------

## Expected Behavior

`RunCommandInterpreter` should run normally, as it does in LLDB 20.

------------------------------------------------------------------------

## Additional context

<img width="940" height="890" alt="Image" src="https://github.com/user-attachments/assets/798b28bb-158d-4cc4-a1a6-0d55434de0a3" />

`stdout` and `stderr` is null, crash SIGSEGV


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: None (patryk4815)

<details>
Starting with **LLDB 21**, calling `RunCommandInterpreter` crashes when
the debugger's output and error streams are initialized using
`lldb.SBFile.Create(..., force_io_methods=True)` in Python.

The same code works correctly on **LLDB 20**.

Removing `force_io_methods=True` (or explicitly setting it to `False`)
avoids the crash, indicating a regression in LLDB's handling of `SBFile`
IO redirection.

Probably regression is introduced by:\
https://github.com/llvm/llvm-project/commit/58279d1ee1b567e8ca793d6d1eb6e0f1d5e7279e

------------------------------------------------------------------------

## Affected Versions

-   **LLDB 20** -- OK
-   **LLDB 21** -- **CRASH** when invoking `RunCommandInterpreter`

Tested on: 
- **Fedora 42** (lldb-20) --- works 
- **Fedora 43** (lldb-21) --- crashes

Install using:
```bash   
dnf install lldb -y
```

------------------------------------------------------------------------

## Minimal Reproduction

``` python
import lldb
import sys

lldb.SBDebugger.Initialize()
debugger = lldb.SBDebugger.Create()

debugger.SetOutputFile(lldb.SBFile.Create(sys.stdout, borrow=True, force_io_methods=True))
debugger.SetErrorFile(lldb.SBFile.Create(sys.stderr, borrow=True, force_io_methods=True))

# CRASHES on LLDB 21
debugger.RunCommandInterpreter(
    True, False,
    lldb.SBCommandInterpreterRunOptions(),
    0, False, False
)
```

### Notes

-   If `force_io_methods=True` is removed or set to `False`, **no crash occurs**.
-   The crash happens immediately upon entering `RunCommandInterpreter`.

------------------------------------------------------------------------

## Expected Behavior

`RunCommandInterpreter` should run normally, as it does in LLDB 20.

------------------------------------------------------------------------

## Additional context

&lt;img width="940" height="890" alt="Image" src="https://github.com/user-attachments/assets/798b28bb-158d-4cc4-a1a6-0d55434de0a3" /&gt;

`stdout` and `stderr` is null, crash SIGSEGV

</details>


---

### Comment 2 - DavidSpickett

> Probably regression is introduced by:
https://github.com/llvm/llvm-project/commit/58279d1ee1b567e8ca793d6d1eb6e0f1d5e7279e

It was, I was able to confirm on AArch64 Ubuntu Linux.

```
$ export PYTHONPATH=$(./bin/lldb --python-path)
$ python3 /tmp/testlldb.py 
Segmentation fault (core dumped)
```

```
(gdb) bt
#0  __GI___fileno (fp=0x0) at ./libio/fileno.c:35
#1  0x0000ffffeb426138 in el_init () from target:/lib/aarch64-linux-gnu/libedit.so.2
#2  0x0000ffffecb3b188 in lldb_private::Editline::ConfigureEditor(bool) () from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#3  0x0000ffffecb3cd48 in lldb_private::Editline::GetLine(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, bool&) ()
   from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#4  0x0000ffffecab19e4 in lldb_private::IOHandlerEditline::Run() () from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#5  0x0000ffffeca8d75c in lldb_private::Debugger::RunIOHandlers() () from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#6  0x0000ffffecb6cd80 in lldb_private::CommandInterpreter::RunCommandInterpreter(lldb_private::CommandInterpreterRunOptions&) ()
   from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#7  0x0000ffffec878898 in lldb::SBDebugger::RunCommandInterpreter(bool, bool, lldb::SBCommandInterpreterRunOptions&, int&, bool&, bool&) ()
   from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#8  0x0000ffffec9a7f6c in _wrap_SBDebugger_RunCommandInterpreter(_object*, _object*) ()
   from target:/home/david.spickett/build-llvm-aarch64/local/lib/python3.10/dist-packages/lldb/_lldb.cpython-310-aarch64-linux-gnu.so
#9  0x0000aaaaaaba3edc in ?? ()
#10 0x0000aaaaaab9a480 in _PyObject_MakeTpCall ()
#11 0x0000aaaaaab91718 in _PyEval_EvalFrameDefault ()
#12 0x0000aaaaaaba4b48 in _PyFunction_Vectorcall ()
#13 0x0000aaaaaab8cc70 in _PyEval_EvalFrameDefault ()
#14 0x0000aaaaaac87b70 in ?? ()
#15 0x0000aaaaaac879f4 in PyEval_EvalCode ()
#16 0x0000aaaaaacbad2c in ?? ()
#17 0x0000aaaaaacb3448 in ?? ()
#18 0x0000aaaaaacba9dc in ?? ()
#19 0x0000aaaaaacb9b44 in _PyRun_SimpleFileObject ()
#20 0x0000aaaaaacb9710 in _PyRun_AnyFileObject ()
#21 0x0000aaaaaacaa090 in Py_RunMain ()
#22 0x0000aaaaaac78b48 in Py_BytesMain ()
#23 0x0000fffff7d273fc in __libc_start_call_main (main=main@entry=0xaaaaaac78b20, argc=argc@entry=2, argv=argv@entry=0xfffffffff288) at ../sysdeps/nptl/libc_start_call_main.h:58
#24 0x0000fffff7d274cc in __libc_start_main_impl (main=0xaaaaaac78b20, argc=2, argv=0xfffffffff288, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=<optimized out>)
    at ../csu/libc-start.c:392
#25 0x0000aaaaaac78a30 in _start ()
```

---

### Comment 3 - DavidSpickett

@JDevlieghere as the author of that change.

---

### Comment 4 - JDevlieghere

I'm able to reproduce this with ToT on Darwin too:

```
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x68)
  * frame #0: 0x00000001921df2dc libsystem_c.dylib`flockfile + 28
    frame #1: 0x00000001921e3e1c libsystem_c.dylib`fileno + 24
    frame #2: 0x00000001eb153744 libedit.3.dylib`el_init + 112
    frame #3: 0x0000000120a46c84 liblldb.22.0.0git.dylib`lldb_private::Editline::ConfigureEditor(this=0x0000000b6fc97400, multiline=<unavailable>) at Editline.cpp:1270:16 [opt]
    frame #4: 0x0000000120a480e4 liblldb.22.0.0git.dylib`lldb_private::Editline::GetLine(this=0x0000000b6fc97400, line="", interrupted=0x000000016d7015af) at Editline.cpp:1610:3 [opt]
    frame #5: 0x000000012098f6a8 liblldb.22.0.0git.dylib`lldb_private::IOHandlerEditline::Run(this=0x0000000b6fd07498) at IOHandler.cpp:600:11 [opt]
    frame #6: 0x0000000120968738 liblldb.22.0.0git.dylib`lldb_private::Debugger::RunIOHandlers(this=0x0000000b6ecb4000) at Debugger.cpp:1277:16 [opt]
```

According to `SBFileExtensions.i`, if `force_scripting_io` is set then the python read/write methods will be called even if a file descriptor is available. It's not yet clear to me why that's a problem or how my commit would have changed that.

---

### Comment 5 - JDevlieghere

Confirmed that this regressed with 58279d1ee1b567e8ca793d6d1eb6e0f1d5e7279e. Parent works: [557628d](https://github.com/llvm/llvm-project/commit/557628dbe6a935b1ad5e1bcfd51ac3a65e35d874).

---

### Comment 6 - JDevlieghere

Alright, this took me a while to figure out because I couldn't wrap my head around how this was working before my patch. Turns out it wasn't, and the regression is that I unintentionally changed the check for using Editline:

The original code was checking the stream: `GetOutputFILE` and `GetErrorFILE`. 

```
use_editline = GetInputFILE() && GetOutputFILE() && GetErrorFILE() &&
                 m_input_sp && m_input_sp->GetIsRealTerminal();
```

The new code was checking the file: `m_output_sp` and `m_error_sp`

```
use_editline = m_input_sp && m_output_sp && m_error_sp &&
                            m_input_sp->GetIsRealTerminal();
```

The correct check is:

```
  const bool use_editline =
      m_input_sp && m_input_sp->GetIsRealTerminal() &&             // Input
      m_output_sp && m_output_sp->GetUnlockedFile().GetStream() && // Output
      m_error_sp && m_error_sp->GetUnlockedFile().GetStream();     // Error
```

We don't need to update the check for the input, because we're handling the missing stream there correctly in the call to the constructor:

```
    m_editline_up = std::make_unique<Editline>(
        editline_name, m_input_sp ? m_input_sp->GetStream() : nullptr,
        m_output_sp, m_error_sp, m_color);
```

We can't do the same for the output and error because we need to pass the file, not the stream (to do proper locking). 

---

