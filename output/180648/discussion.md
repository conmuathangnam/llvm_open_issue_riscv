# [clang] Clang codegen crash with x86-64 WinEH

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/180648
**Status:** Closed
**Labels:** clang:codegen, crash
**Closed Date:** 2026-03-12T02:53:33Z

## Body

`$ cat /tmp/a.cc`

```c++
class span {
  char data_;
  size_t size_;
};
long g(span);
void f(span output) {
  __try {
  } __except (g(output)) {
  }
}
```
```
$ clang -cc1 -triple x86_64-pc-windows-msvc19.34.0 -fms-compatibility -emit-llvm /tmp/a.cc -o /dev/null
...
 #4 0x00000000035207e7 llvm::Instruction::clone() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x35207e7)
 #5 0x00000000044bf7ef clang::CodeGen::CodeGenFunction::recoverAddrOfEscapedLocal(clang::CodeGen::CodeGenFunction&, clang::CodeGen::Address, llvm::Value*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44bf7ef)
 #6 0x00000000044c2315 clang::CodeGen::CodeGenFunction::EmitCapturedLocals(clang::CodeGen::CodeGenFunction&, clang::Stmt const*, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c2315)
 #7 0x00000000044c2fe0 clang::CodeGen::CodeGenFunction::startOutlinedSEHHelper(clang::CodeGen::CodeGenFunction&, bool, clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c2fe0)
 #8 0x00000000044c30f9 clang::CodeGen::CodeGenFunction::GenerateSEHFilterFunction(clang::CodeGen::CodeGenFunction&, clang::SEHExceptStmt const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c30f9)
 #9 0x00000000044c3747 clang::CodeGen::CodeGenFunction::EnterSEHTryStmt(clang::SEHTryStmt const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c3747)
#10 0x00000000044c38ca clang::CodeGen::CodeGenFunction::EmitSEHTryStmt(clang::SEHTryStmt const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c38ca)
#11 0x00000000040adb1c clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x40adb1c)
#12 0x00000000040b48f9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x40b48f9)
#13 0x00000000041166f5 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x41166f5)
#14 0x000000000412a183 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x412a183)
#15 0x0000000004184b2b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x4184b2b)
#16 0x00000000041805b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x41805b4)
#17 0x0000000004180eb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x4180eb3)
#18 0x000000000418aa2f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
...
```

according to godbolt this crashes in the frontend codegen very far back to old clangs

## Comments

### Comment 1 - GkvJwa

I will record this case

This is related to the parameters passed. The following code works fine.
```c++
//  llvm::Argument
class span {
  char data_;
  size_t size_; 
};
->
// llvm::AllocaInst
class span {
  char data_;
  int size_;
};

// size_t -> int
// Less than 8 bytes is AllocaInst 
```

But this only handles the AllocaInst case.
https://github.com/llvm/llvm-project/blob/0952ccc712ffc97943fbd0fc3c38a53e7aa875ac/clang/lib/CodeGen/CGException.cpp#L1835-L1849

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Arthur Eubanks (aeubanks)

<details>
`$ cat /tmp/a.cc`

```c++
class span {
  char data_;
  size_t size_;
};
long g(span);
void f(span output) {
  __try {
  } __except (g(output)) {
  }
}
```
```
$ clang -cc1 -triple x86_64-pc-windows-msvc19.34.0 -fms-compatibility -emit-llvm /tmp/a.cc -o /dev/null
...
 #<!-- -->4 0x00000000035207e7 llvm::Instruction::clone() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x35207e7)
 #<!-- -->5 0x00000000044bf7ef clang::CodeGen::CodeGenFunction::recoverAddrOfEscapedLocal(clang::CodeGen::CodeGenFunction&amp;, clang::CodeGen::Address, llvm::Value*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44bf7ef)
 #<!-- -->6 0x00000000044c2315 clang::CodeGen::CodeGenFunction::EmitCapturedLocals(clang::CodeGen::CodeGenFunction&amp;, clang::Stmt const*, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c2315)
 #<!-- -->7 0x00000000044c2fe0 clang::CodeGen::CodeGenFunction::startOutlinedSEHHelper(clang::CodeGen::CodeGenFunction&amp;, bool, clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c2fe0)
 #<!-- -->8 0x00000000044c30f9 clang::CodeGen::CodeGenFunction::GenerateSEHFilterFunction(clang::CodeGen::CodeGenFunction&amp;, clang::SEHExceptStmt const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c30f9)
 #<!-- -->9 0x00000000044c3747 clang::CodeGen::CodeGenFunction::EnterSEHTryStmt(clang::SEHTryStmt const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c3747)
#<!-- -->10 0x00000000044c38ca clang::CodeGen::CodeGenFunction::EmitSEHTryStmt(clang::SEHTryStmt const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x44c38ca)
#<!-- -->11 0x00000000040adb1c clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x40adb1c)
#<!-- -->12 0x00000000040b48f9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x40b48f9)
#<!-- -->13 0x00000000041166f5 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x41166f5)
#<!-- -->14 0x000000000412a183 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x412a183)
#<!-- -->15 0x0000000004184b2b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x4184b2b)
#<!-- -->16 0x00000000041805b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x41805b4)
#<!-- -->17 0x0000000004180eb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x4180eb3)
#<!-- -->18 0x000000000418aa2f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
...
```

according to godbolt this crashes in the frontend codegen very far back to old clangs
</details>


---

