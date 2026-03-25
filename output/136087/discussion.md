# [RISCV] Missing register overlap check for XTheadMemPair loads

**Author:** topperc
**URL:** https://github.com/llvm/llvm-project/issues/136087
**Status:** Closed
**Labels:** good first issue, backend:RISC-V
**Closed Date:** 2025-04-18T03:16:59Z

## Body

I think the XTheadMemPair extension requires rs1 != rd1 && rs1 != rd2 && rd1 != rd2.

We have a check in the assembler in `validateInstruction` but it isn't as strict. 

```
  if (Opcode == RISCV::TH_LDD || Opcode == RISCV::TH_LWUD ||                     
      Opcode == RISCV::TH_LWD) {                                                 
    MCRegister Rd1 = Inst.getOperand(0).getReg();                                
    MCRegister Rd2 = Inst.getOperand(1).getReg();                                
    MCRegister Rs1 = Inst.getOperand(2).getReg();                                
    // The encoding with rd1 == rd2 == rs1 is reserved for XTHead load pair.     
    if (Rs1 == Rd1 && Rs1 == Rd2) {                                              
      SMLoc Loc = Operands[1]->getStartLoc();                                    
      return Error(Loc, "rs1, rd1, and rd2 cannot all be the same");             
    }                                                                            
  } 
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Craig Topper (topperc)

<details>
The XTheadMemPair extension requires rs1 != rd1 &amp;&amp; rs1 != rd2 &amp;&amp; rd1 != rd2. We don't check for this in the assembler today, but binutils does.

We need to add a check to validateInstruction in RISCVAsmParser.cpp
</details>


---

### Comment 2 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 3 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Craig Topper (topperc)

<details>
The XTheadMemPair extension requires rs1 != rd1 &amp;&amp; rs1 != rd2 &amp;&amp; rd1 != rd2. We don't check for this in the assembler today, but binutils does.

We need to add a check to validateInstruction in RISCVAsmParser.cpp
</details>


---

