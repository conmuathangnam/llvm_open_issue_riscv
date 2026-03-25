# lldb-dap crash on windows when python script formatter are used

**Author:** frleheri-havok
**URL:** https://github.com/llvm/llvm-project/issues/177373
**Status:** Open
**Labels:** platform:windows, crash, lldb-dap

## Body

I am running lldb-dap in vscode and getting crashes when using python scripts to format variables.

I have made a minimal repro here: [lldb-dap_crash.zip](https://github.com/user-attachments/files/24797448/lldb-dap_crash.zip). The archive contains a script that run lldb-dap a perform two commands that get it to crash:
- initialize lldb
- launch the executable with a init commands arg.

Note that commenting out line 161 in repro.ps1 (sending the init command) stop lldb.dap from crashing. 

The script produce a crashdump but I don't have any of the symbols available to decode the stacktrace.

The logs from lldb-dap are:
>1769093070.899796247 (stdio) --> {"command":"initialize","arguments":{"columnsStartAt1":true,"linesStartAt1":true,"clientID":"vscode","adapterID":"lldb-dap","pathFormat":"path"},"type":"request","seq":1}
>1769093070.916259527 (stdio) <-- {"body":{"$__lldb_version":"lldb version 21.1.8","completionTriggerCharacters":["."," ","\t"],"exceptionBreakpointFilters":[{"description":"C++ Catch","filter":"cpp_catch","label":"C++ Catch","supportsCondition":true},{"description":"C++ Throw","filter":"cpp_throw","label":"C++ Throw","supportsCondition":true},{"description":"Objective-C Catch","filter":"objc_catch","label":"Objective-C Catch","supportsCondition":true},{"description":"Objective-C Throw","filter":"objc_throw","label":"Objective-C Throw","supportsCondition":true}],"supportTerminateDebuggee":true,"supportsBreakpointLocationsRequest":true,"supportsCancelRequest":true,"supportsCompletionsRequest":true,"supportsConditionalBreakpoints":true,"supportsConfigurationDoneRequest":true,"supportsDataBreakpoints":true,"supportsDelayedStackTraceLoading":true,"supportsDisassembleRequest":true,"supportsEvaluateForHovers":true,"supportsExceptionFilterOptions":true,"supportsExceptionInfoRequest":true,"supportsFunctionBreakpoints":true,"supportsHitConditionalBreakpoints":true,"supportsInstructionBreakpoints":true,"supportsLogPoints":true,"supportsModulesRequest":true,"supportsReadMemoryRequest":true,"supportsSetVariable":true,"supportsSteppingGranularity":true,"supportsValueFormattingOptions":true,"supportsWriteMemoryRequest":true},"command":"initialize","request_seq":1,"seq":0,"success":true,"type":"response"}
>1769093070.957221508 (stdio) --> {"command":"launch","arguments":{"initCommands":["command script import script.py"],"program":"D:/git/A/out/build/navigation-vdb-ClangCL-x64-debug-lldb/Public/Demo/Demos/Demos.exe"},"type":"request","seq":2}

And then it just crash without outputting any errors

Note that running lldb directly work:
> PS D:\WORK_DIR\lldb-dap_crash> lldb notepad.exe
>(lldb) target create "notepad.exe"
>Current executable set to 'C:\WINDOWS\SYSTEM32\notepad.exe' (x86_64).
>(lldb) command script import script.py
>hello world

I am on Windows 11 with LLVM installed with the windows installer from https://github.com/llvm/llvm-project/releases/tag/llvmorg-21.1.8

lldb-dap version:
>PS D:\WORK_DIR\lldb-dap_crash> lldb-dap.exe --version
>lldb-dap: LLVM (http://llvm.org/):
>  LLVM version 21.1.8
>  Optimized build.
>liblldb: lldb version 21.1.8

Python version (that the only version I have installed)
> PS D:\WORK_DIR\lldb-dap_crash> python --version
> Python 3.10.11
>PS D:\WORK_DIR\lldb-dap_crash> py -0
> -V:3.10 *        Python 3.10 (64-bit)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb-dap

Author: François L (frleheri-havok)

<details>
I am running lldb-dap in vscode and getting crashes when using python scripts to format variables.

I have made a minimal repro here: [lldb-dap_crash.zip](https://github.com/user-attachments/files/24797448/lldb-dap_crash.zip). The archive contains a script that run lldb-dap a perform two commands that get it to crash:
- initialize lldb
- launch the executable with a init commands arg.

Note that commenting out line 161 in repro.ps1 (sending the init command) stop lldb.dap from crashing. 

The script produce a crashdump but I don't have any of the symbols available to decode the stacktrace.

The logs from lldb-dap are:
&gt;1769093070.899796247 (stdio) --&gt; {"command":"initialize","arguments":{"columnsStartAt1":true,"linesStartAt1":true,"clientID":"vscode","adapterID":"lldb-dap","pathFormat":"path"},"type":"request","seq":1}
&gt;1769093070.916259527 (stdio) &lt;-- {"body":{"$__lldb_version":"lldb version 21.1.8","completionTriggerCharacters":["."," ","\t"],"exceptionBreakpointFilters":[{"description":"C++ Catch","filter":"cpp_catch","label":"C++ Catch","supportsCondition":true},{"description":"C++ Throw","filter":"cpp_throw","label":"C++ Throw","supportsCondition":true},{"description":"Objective-C Catch","filter":"objc_catch","label":"Objective-C Catch","supportsCondition":true},{"description":"Objective-C Throw","filter":"objc_throw","label":"Objective-C Throw","supportsCondition":true}],"supportTerminateDebuggee":true,"supportsBreakpointLocationsRequest":true,"supportsCancelRequest":true,"supportsCompletionsRequest":true,"supportsConditionalBreakpoints":true,"supportsConfigurationDoneRequest":true,"supportsDataBreakpoints":true,"supportsDelayedStackTraceLoading":true,"supportsDisassembleRequest":true,"supportsEvaluateForHovers":true,"supportsExceptionFilterOptions":true,"supportsExceptionInfoRequest":true,"supportsFunctionBreakpoints":true,"supportsHitConditionalBreakpoints":true,"supportsInstructionBreakpoints":true,"supportsLogPoints":true,"supportsModulesRequest":true,"supportsReadMemoryRequest":true,"supportsSetVariable":true,"supportsSteppingGranularity":true,"supportsValueFormattingOptions":true,"supportsWriteMemoryRequest":true},"command":"initialize","request_seq":1,"seq":0,"success":true,"type":"response"}
&gt;1769093070.957221508 (stdio) --&gt; {"command":"launch","arguments":{"initCommands":["command script import script.py"],"program":"D:/git/A/out/build/navigation-vdb-ClangCL-x64-debug-lldb/Public/Demo/Demos/Demos.exe"},"type":"request","seq":2}

And then it just crash without outputting any errors

Note that running lldb directly work:
&gt; PS D:\WORK_DIR\lldb-dap_crash&gt; lldb notepad.exe
&gt;(lldb) target create "notepad.exe"
&gt;Current executable set to 'C:\WINDOWS\SYSTEM32\notepad.exe' (x86_64).
&gt;(lldb) command script import script.py
&gt;hello world

I am on Windows 11 with LLVM installed with the windows installer from https://github.com/llvm/llvm-project/releases/tag/llvmorg-21.1.8

lldb-dap version:
&gt;PS D:\WORK_DIR\lldb-dap_crash&gt; lldb-dap.exe --version
&gt;lldb-dap: LLVM (http://llvm.org/):
&gt;  LLVM version 21.1.8
&gt;  Optimized build.
&gt;liblldb: lldb version 21.1.8

Python version (that the only version I have installed)
&gt; PS D:\WORK_DIR\lldb-dap_crash&gt; python --version
&gt; Python 3.10.11
&gt;PS D:\WORK_DIR\lldb-dap_crash&gt; py -0
&gt; -V:3.10 *        Python 3.10 (64-bit)
</details>


---

