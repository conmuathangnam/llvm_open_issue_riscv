# MIPS: a ICE for -msingle-float

**Author:** wzssyqa
**URL:** https://github.com/llvm/llvm-project/issues/161033
**Status:** Closed
**Labels:** backend:MIPS, crash
**Closed Date:** 2025-10-10T00:31:48Z

## Body

```
void f(void) {
	asm volatile ("nop" : : : "$f20", "$f21", "$f22", "$f23", "$f24", "$f25", "$f26", "$f27", "$f28", "$f29", "$f30", "$f31");
}
```

```
clang-22: /home/syq/llvm-project/llvm/include/llvm/CodeGen/TargetLowering.h:1072: virtual const TargetRegisterClass *llvm::TargetLoweringBase::getRegClassFor(MVT, bool) const: Assertion `RC && "This value type is not natively supported!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./bin/clang-22 --target=mipsel-linux-gnu -S -mabi=32 -msingle-float yy.c -O3
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'yy.c'.
4.      Running pass 'MIPS DAG->DAG Pattern Instruction Selection' on function '@f'
```

@Tazdevil971 do you have interesting on this problem?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: YunQiang Su (wzssyqa)

<details>
```
void f(void) {
	asm volatile ("nop" : : : "$f20", "$f21", "$f22", "$f23", "$f24", "$f25", "$f26", "$f27", "$f28", "$f29", "$f30", "$f31");
}
```

```
clang-22: /home/syq/llvm-project/llvm/include/llvm/CodeGen/TargetLowering.h:1072: virtual const TargetRegisterClass *llvm::TargetLoweringBase::getRegClassFor(MVT, bool) const: Assertion `RC &amp;&amp; "This value type is not natively supported!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./bin/clang-22 --target=mipsel-linux-gnu -S -mabi=32 -msingle-float yy.c -O3
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'yy.c'.
4.      Running pass 'MIPS DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->f'
```

@<!-- -->Tazdevil971 do you have interesting on this problem?
</details>


---

### Comment 2 - Tazdevil971

Looks like something I can tackle. Today I'm not home, so I'll give this a look in the following days.

---

### Comment 3 - Tazdevil971

Looked into it, I think #140575 already fixed this as I cannot recreate the bug on main. If you confirm this I think we can close the issue.

My guess is that [this](https://github.com/llvm/llvm-project/blob/d29798767cc10a0609b5f1f9f75f032b6760cf50/llvm/lib/Target/Mips/MipsISelLowering.cpp#L4267) line is what fixed the problem.

---

### Comment 4 - brad0

@wzssyqa Can you verify if the ICE is gone for you with main?

---

### Comment 5 - wzssyqa

Yes. This problem is resolved. 

---

