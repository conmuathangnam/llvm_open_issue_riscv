# Crash in TableGen parser

**Author:** jurahul
**URL:** https://github.com/llvm/llvm-project/issues/184879
**Status:** Open
**Labels:** tablegen, crash

## Body

For the following input file:

```
class X {
  int F = 0;
  int D = !add(F, 1);
}

class Y : X  {
  string F = "0";
}
```

tablegen hits an isa<> assert during parsing:

```
test.td:7:14: error: Field 'F' of type 'int' is incompatible with value '"0"' of type 'string'
  string F = "0";
             ^
llvm-tblgen: llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<llvm::UnsetInit, const llvm::Init *>::doit(const From *) [To = llvm::UnsetInit, From = const llvm::Init *]: Assertion `Val && "isa<> used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.


## Comments

### Comment 1 - jurahul

Looks like we need an early exit somewhere after this error is detected

---

