# [clang][crash] Clang crash for wasm target when processing a file using modules on macOS

**Author:** Algorithm0
**URL:** https://github.com/llvm/llvm-project/issues/164113

## Body

I am using EMSDK. On versions 4.0.16 and 4.0.17, I see clang crashing on my MacBock on M4 Pro (macOS Tahoe 26.0.1).
I would like to draw your attention to the fact that the same code base with the same configuration compiles perfectly on Linux x86_64 (Fedora Workstation/Ubuntu).\
I have included the crash log in the [output.log](https://github.com/user-attachments/files/22986560/output.log) file. Below are the files that I was also asked to send in the event of a crash (I simply couldn't find any files with the mask `clang-22_<YYYY-MM-DD-HHMMSS>_<hostname>.crash` in the location `/Users/aurum/Library/Logs/DiagnosticReports`).

[/var/folders/6g/b6tv394d3hx3zccjy5cs1mhc0000gn/T/app_mo_separator_unifier-09b611.cpp](https://github.com/user-attachments/files/22986558/app_mo_separator_unifier-09b611.cpp)
[/var/folders/6g/b6tv394d3hx3zccjy5cs1mhc0000gn/T/app_mo_separator_unifier-09b611.sh](https://github.com/user-attachments/files/22986559/app_mo_separator_unifier-09b611.sh)



