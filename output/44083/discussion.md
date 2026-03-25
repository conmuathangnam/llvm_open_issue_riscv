# Assertion `(EltTy->isDependentType() || EltTy->isIncompleteType() || EltTy->isConstantSizeType()) && "Constant array of VLAs is illegal!"' failed.

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/44083

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [44738](https://llvm.org/bz44738) |
| Version | trunk |
| OS | All |
| Reporter | LLVM Bugzilla Contributor |
| CC | @zygoloid |

## Extended Description 
POC:

```
# 1 "<built-in>"
# 1 "test1.c"
b() {
  int a[][c()] = {}
```

Run script:

```
# Crash reproducer for clang version 11.0.0 (https://github.com/llvm/llvm-project.git 56b7f595d2b402ff177ca42214325c8fdea10fb0)
# Driver args: "-x" "c" "test1.c" "-o" "test"
# Original command:  "/home/rxz226/llvm-project/bld_instru/bin/clang-11" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-main-file-name" "test1.c" "-mrelocation-model" "static" "-mthread-model" "posix" "-mframe-pointer=all" "-fmath-errno" "-fno-rounding-math" "-masm-verbose" "-mconstructor-aliases" "-munwind-tables" "-target-cpu" "x86-64" "-dwarf-column-info" "-fno-split-dwarf-inlining" "-debugger-tuning=gdb" "-resource-dir" "/home/rxz226/llvm-project/bld_instru/lib/clang/11.0.0" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/home/rxz226/llvm-project/bld_instru/lib/clang/11.0.0/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdebug-compilation-dir" "/home/rxz226/fuzz_clang1/hangs/test" "-ferror-limit" "19" "-fmessage-length" "0" "-fgnuc-version=4.2.1" "-fobjc-runtime=gcc" "-fdiagnostics-show-option" "-fcolor-diagnostics" "-faddrsig" "-o" "/tmp/test1-b92dad.o" "-x" "c" "test1.c"
 "/home/rxz226/llvm-project/bld_instru/bin/clang-11" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-main-file-name" "test1.c" "-mrelocation-model" "static" "-mthread-model" "posix" "-mframe-pointer=all" "-fmath-errno" "-fno-rounding-math" "-masm-verbose" "-mconstructor-aliases" "-munwind-tables" "-target-cpu" "x86-64" "-dwarf-column-info" "-fno-split-dwarf-inlining" "-debugger-tuning=gdb" "-ferror-limit" "19" "-fmessage-length" "0" "-fgnuc-version=4.2.1" "-fobjc-runtime=gcc" "-fdiagnostics-show-option" "-fcolor-diagnostics" "-faddrsig" "-x" "c" "test1-af8ec3.c"
```

Stack dump:

```
 #&#8203;0 0x000000000681faf6 llvm::sys::PrintStackTrace(llvm::raw_ostream&) /home/rxz226/llvm-project/llvm/lib/Support/Unix/Signals.inc:560:7
 #&#8203;1 0x000000000681ac5e llvm::sys::RunSignalHandlers() /home/rxz226/llvm-project/llvm/lib/Support/Signals.cpp:69:18
 #&#8203;2 0x000000000681dd30 llvm::sys::CleanupOnSignal(unsigned long) /home/rxz226/llvm-project/llvm/lib/Support/Unix/Signals.inc:361:3
 #&#8203;3 0x000000000662a5aa (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/rxz226/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:80:5
 #&#8203;4 0x000000000662a9e6 /home/rxz226/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:361:51
 #&#8203;5 0x00007fbe9041a390 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x11390)
 #&#8203;6 0x00007fbe8ef84428 raise /build/glibc-LK5gWL/glibc-2.23/signal/../sysdeps/unix/sysv/linux/raise.c:54:0
 #&#8203;7 0x00007fbe8ef8602a abort /build/glibc-LK5gWL/glibc-2.23/stdlib/abort.c:91:0
 #&#8203;8 0x00007fbe8ef7cbd7 __assert_fail_base /build/glibc-LK5gWL/glibc-2.23/assert/assert.c:92:0
 #&#8203;9 0x00007fbe8ef7cc82 (/lib/x86_64-linux-gnu/libc.so.6+0x2dc82)
#&#8203;10 0x000000000ddbdf59 (/home/rxz226/llvm-project/bld_instru/bin/clang+0xddbdf59)
#&#8203;11 0x000000000cb4ceb6 (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) /home
/rxz226/llvm-project/clang/lib/Sema/SemaInit.cpp:1938:14
#&#8203;12 0x000000000cb45a54 llvm::APInt::~APInt() /home/rxz226/llvm-project/llvm/include/llvm/ADT/APInt.h:0:0
#&#8203;13 0x000000000cb45a54 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) /home/
rxz226/llvm-project/clang/lib/Sema/SemaInit.cpp:1280:0
#&#8203;14 0x000000000cb0293b (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) /home/rxz226/llvm-project/clang/lib/Sema/S
emaInit.cpp:1177:7
#&#8203;15 0x000000000cb0293b (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool) /home/rxz226/llvm-project/clang/lib/Sema/Sem
aInit.cpp:953:0
#&#8203;16 0x000000000cb22dce clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) /home/rxz226/llvm-project/clan
g/lib/Sema/SemaInit.cpp:8203:23
#&#8203;17 0x000000000c174389 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /home/rxz226/llvm-project/clang/lib/Sema/SemaDecl.cpp:11858:33
#&#8203;18 0x000000000bc15c12 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /home/rxz226/llvm-project/clang/lib/Parse/ParseDecl.c
pp:0:17
#&#8203;19 0x000000000bc10989 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/rxz226/llvm-project/clang/lib/Parse/ParseDecl.cpp:2128:9
...
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-c

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - AaronBallman

The assertion reproduces as of Clang 16.0.0 but requires an asserts build to reproduce: https://gcc.godbolt.org/z/TGn48hen4

---

### Comment 4 - AaronBallman

A more realistic minimal reproducer is:
```
int c();
void b() {
  int a[][c()] = {};
}
```

---

