# [LLVM][RISCV][ICE] clang-23:  .. riscv-gnu-toolchain/llvm/llvm/lib/IR/Value.cpp:524: void llvm::Value::doRAUW(llvm::Value*, ReplaceMetadataUses): Assertion failure `!contains(New, this) && "this->replaceAllUsesWith(expr(this)) is NOT valid!"'.

**Author:** whileone-sk
**URL:** https://github.com/llvm/llvm-project/issues/178168

## Body

riscv-gnu-toolchain/llvm/llvm/lib/IR/Value.cpp:524: void llvm::Value::doRAUW(llvm::Value*, ReplaceMetadataUses): Assertion `!contains(New, this) && "this->replaceAllUsesWith(expr(this)) is NOT valid!"' failed.

It's a failed assertion within the Clang compiler's source code, specifically in the Value.cpp.
RUN script red-3b3f96.sh to reproduce the issue.

source [red-3b3f96.c](https://github.com/user-attachments/files/24882306/red-3b3f96.c)
script [red-3b3f96.sh](https://github.com/user-attachments/files/24882312/red-3b3f96.sh)

Attached backtrace.
[verbose-log.txt](https://github.com/user-attachments/files/24882349/verbose-log.txt)
[red.c](https://github.com/user-attachments/files/24882353/red.c)

This issue is Found via fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Sujay (whileone-sk)

<details>
riscv-gnu-toolchain/llvm/llvm/lib/IR/Value.cpp:524: void llvm::Value::doRAUW(llvm::Value*, ReplaceMetadataUses): Assertion `!contains(New, this) &amp;&amp; "this-&gt;replaceAllUsesWith(expr(this)) is NOT valid!"' failed.

It's a failed assertion within the Clang compiler's source code, specifically in the Value.cpp.
RUN script red-3b3f96.sh to reproduce the issue.

source [red-3b3f96.c](https://github.com/user-attachments/files/24882306/red-3b3f96.c)
script [red-3b3f96.sh](https://github.com/user-attachments/files/24882312/red-3b3f96.sh)

Attached backtrace.
[verbose-log.txt](https://github.com/user-attachments/files/24882349/verbose-log.txt)
[red.c](https://github.com/user-attachments/files/24882353/red.c)

This issue is Found via fuzzer.
</details>


---

### Comment 2 - frederick-vs-ja

Possibly related to #41958.

---

