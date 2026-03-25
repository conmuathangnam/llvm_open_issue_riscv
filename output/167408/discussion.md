# [lld][MachO] lld segfaults when encountering N_SECT symbol with n_sect == 0 (malformed object?)

**Author:** Prabhuk
**URL:** https://github.com/llvm/llvm-project/issues/167408
**Status:** Closed
**Labels:** llvm:mc, crash
**Closed Date:** 2025-11-11T22:27:27Z

## Body

Expected behavior:
`lld` should diagnose the malformed object with a clear error and exit gracefully.

Actual behavior:
`lld` can segfault while processing the malformed object.

Likely cause / code path:
In `lld/MachO/InputFiles.cpp`, `ObjFile::parseSymbols` assumes `n_sect >= 1` for N_SECT symbols and indexes sections[sym.n_sect - 1] without guarding n_sect == 0. A malformed object with n_sect == 0 triggers an out-of-bounds access and crash. 

See PR #167025 which adds a fatal error for n_sect == 0.

Reason for malformed object creation:
https://github.com/llvm/llvm-project/blob/main/llvm/lib/MC/MachObjectWriter.cpp#L649

The object writer asserts for the size of the sections but does not explicitly break compilation when the size is greater than 255. In our toolchain distribution we ship binaries without assertions enabled which allowed creation of an object file that had sections larger than 255. One of the section symbols referring to one such section has n_sect set to zero which resulted in segfault during link. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-macho

Author: Prabhu Rajasekaran (Prabhuk)

<details>
Expected behavior:
`lld` should diagnose the malformed object with a clear error and exit gracefully.

Actual behavior:
`lld` can segfault while processing the malformed object.

Likely cause / code path:
In `lld/MachO/InputFiles.cpp`, `ObjFile::parseSymbols` assumes `n_sect &gt;= 1` for N_SECT symbols and indexes sections[sym.n_sect - 1] without guarding n_sect == 0. A malformed object with n_sect == 0 triggers an out-of-bounds access and crash. 

See PR #<!-- -->167025 which adds a fatal error for n_sect == 0.

Reason for malformed object creation:
https://github.com/llvm/llvm-project/blob/main/llvm/lib/MC/MachObjectWriter.cpp#L649

The object writer asserts for the size of the sections but does not explicitly break compilation when the size is greater than 255. In our toolchain distribution we ship binaries without assertions enabled which allowed creation of an object file that had sections larger than 255. One of the section symbols referring to one such section has n_sect set to zero which resulted in segfault during link. 
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Prabhu Rajasekaran (Prabhuk)

<details>
Expected behavior:
`lld` should diagnose the malformed object with a clear error and exit gracefully.

Actual behavior:
`lld` can segfault while processing the malformed object.

Likely cause / code path:
In `lld/MachO/InputFiles.cpp`, `ObjFile::parseSymbols` assumes `n_sect &gt;= 1` for N_SECT symbols and indexes sections[sym.n_sect - 1] without guarding n_sect == 0. A malformed object with n_sect == 0 triggers an out-of-bounds access and crash. 

See PR #<!-- -->167025 which adds a fatal error for n_sect == 0.

Reason for malformed object creation:
https://github.com/llvm/llvm-project/blob/main/llvm/lib/MC/MachObjectWriter.cpp#L649

The object writer asserts for the size of the sections but does not explicitly break compilation when the size is greater than 255. In our toolchain distribution we ship binaries without assertions enabled which allowed creation of an object file that had sections larger than 255. One of the section symbols referring to one such section has n_sect set to zero which resulted in segfault during link. 
</details>


---

