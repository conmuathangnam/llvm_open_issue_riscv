# [clang]: emits SIGSEGV when calling a generic lambda whos requires clauses contains another lambda expression.

**Author:** RungeCC
**URL:** https://github.com/llvm/llvm-project/issues/70064
**Status:** Closed
**Labels:** c++20, clang:frontend, concepts, crash-on-valid
**Closed Date:** 2026-02-19T12:22:33Z

## Body

[**Example**](https://godbolt.org/z/drarr49Pd):

```c++
// flag:
//    -std=c++23
// or -std=c++20
auto X = []<class = void>() 
    requires requires { []() {}; }
{ return 1; }();
```

**Version**:

Trunk, (clang 17 accepts it.):

```
clang version 18.0.0 (https://github.com/llvm/llvm-project.git cc455033d4d4f5731461a6d5fa72e0f9bf871a70)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk-20231024/bin
```

**Expected behavior**:

accepts this, as `gcc` and `msvc` are shown above.

**Actual behavior**:

crashed. Even, there is no stack-trace log and no assertion triggered.

**Note**:

It's astonishing that clang crashes without outputting any valid information.

## Comments

### Comment 1 - dtcxzyw

It seems like Clang ran into a stack overflow when mangling function names.

---

### Comment 2 - DimitryAndric

This seems to have regressed with 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 ("Implement mangling rules for C++20 concepts and requires-expressions") by @zygoloid @erichkeane .

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-20

Author: Runge (RungeCC)

<details>
[**Example**](https://godbolt.org/z/drarr49Pd):

```c++
// flag:
//    -std=c++23
// or -std=c++20
auto X = []&lt;class = void&gt;() 
    requires requires { []() {}; }
{ return 1; }();
```

**Version**:

Trunk, (clang 17 accepts it.):

```
clang version 18.0.0 (https://github.com/llvm/llvm-project.git cc455033d4d4f5731461a6d5fa72e0f9bf871a70)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk-20231024/bin
```

**Expected behavior**:

accepts this, as `gcc` and `msvc` are shown above.

**Actual behavior**:

crashed. Even, there is no stack-trace log and no assertion triggered.

**Note**:

It's astonishing that clang crashes without outputting any valid information.
</details>


---

### Comment 4 - erichkeane

Reproduced on debug it at least gives a stack trace:

```
ekeane@dev-epyc4:/local/home/ekeane/llvm-project/build/bin$ ./clang -cc1 temp2.cpp -std=c++20 -emit-llvm -o -
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./clang -cc1 temp2.cpp -std=c++20 -emit-llvm -o -
1.      temp2.cpp:6:20: current parser token ')'
2.      temp2.cpp:4:11: instantiating function definition '(anonymous class)::operator()<void>'
3.      temp2.cpp:4:11: LLVM IR generation of declaration '(anonymous class)::operator()'
4.      temp2.cpp:4:11: Mangling declaration '(anonymous class)::operator()'
  #0 0x000000000c22e62d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /local/home/ekeane/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
  #1 0x000000000c22eb6b PrintStackTraceSignalHandler(void*) /local/home/ekeane/llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
  #2 0x000000000c22ca8b llvm::sys::RunSignalHandlers() /local/home/ekeane/llvm-project/llvm/lib/Support/Signals.cpp:104:5
  #3 0x000000000c22f391 SignalHandler(int) /local/home/ekeane/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
  #4 0x00007f68fdb253c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x153c0)
  #5 0x000000000c562448 llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>>, 1, clang::ExtQuals const*>::PointerUnionMembers() /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75:0
  #6 0x000000000c562438 llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>>, 0, clang::Type const*, clang::ExtQuals const*>::PointerUnionMembers() /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:75:35
  #7 0x000000000c5623e8 llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>::PointerUnion() /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:135:26
  #8 0x000000000c5623a5 llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>::getFromOpaqueValue(void*) /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:196:49
  #9 0x000000000c562375 llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>::getFromVoidPointer(void*) /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:271:12
 #10 0x000000000c56234e llvm::PointerIntPairInfo<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, 3u, llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>>::getPointer(long) /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:191:12
 #11 0x000000000c5622fd llvm::PointerIntPair<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, 3u, unsigned int, llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>, llvm::PointerIntPairInfo<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, 3u, llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>>>::getPointer() const /local/home/ekeane/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:41
 #12 0x000000000c562675 clang::QualType::isNull() const /local/home/ekeane/llvm-project/clang/include/clang/AST/Type.h:804:18
 #13 0x000000000c5625ec clang::QualType::getCommonPtr() const /local/home/ekeane/llvm-project/clang/include/clang/AST/Type.h:752:5
 #14 0x000000000c5625c5 clang::QualType::getTypePtr() const /local/home/ekeane/llvm-project/clang/include/clang/AST/Type.h:6734:26
 #15 0x000000000c568565 llvm::simplify_type<clang::QualType>::getSimplifiedValue(clang::QualType) /local/home/ekeane/llvm-project/clang/include/clang/AST/Type.h:1430:5
 #16 0x000000000c956d60 bool llvm::detail::isPresent<clang::QualType>(clang::QualType const&) /local/home/ekeane/llvm-project/llvm/include/llvm/Support/Casting.h:631:7
 #17 0x000000000c956cd5 decltype(auto) llvm::dyn_cast<clang::BuiltinType, clang::QualType>(clang::QualType const&) /local/home/ekeane/llvm-project/llvm/include/llvm/Support/Casting.h:650:3
 #18 0x000000000c956cae clang::BuiltinType const* clang::Type::getAs<clang::BuiltinType>() const /local/home/ekeane/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:87:1
 #19 0x000000000c956c48 clang::Type::isSpecificBuiltinType(unsigned int) const /local/home/ekeane/llvm-project/clang/include/clang/AST/Type.h:7273:26
 #20 0x0000000012e0db70 isTypeSubstitutable(clang::Qualifiers, clang::Type const*, clang::ASTContext&) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2888:7
 #21 0x0000000012e0d3e9 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2969:8
 #22 0x0000000012e0c87d (anonymous namespace)::CXXNameMangler::makeFunctionReturnTypeTags(clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6884:23
 #23 0x0000000012e0c090 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:843:7
 #24 0x0000000012e2afda (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #25 0x0000000012e0ce68 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1074:5
 #26 0x0000000012e0c758 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1042:1
 #27 0x0000000012e29ae0 (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3644:1
 #28 0x0000000012e19d60 (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3641:1
 #29 0x0000000012e0d9ea (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /local/home/ekeane/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:71:1
 #30 0x0000000012e1e98b (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5533:5
 #31 0x0000000012e20a97 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4556:9
 #32 0x0000000012e1e15d (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5391:45
 #33 0x0000000012e224e3 (anonymous namespace)::CXXNameMangler::mangleRequiresClause(clang::Expr const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2083:1
 #34 0x0000000012e27f4e (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
 #35 0x0000000012e0cc22 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:957:1
 #36 0x0000000012e0c12b (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:5
 #37 0x0000000012e2afda (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #38 0x0000000012e0ce68 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1074:5
 #39 0x0000000012e0c758 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1042:1
 #40 0x0000000012e29ae0 (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3644:1
 #41 0x0000000012e19d60 (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3641:1
 #42 0x0000000012e0d9ea (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /local/home/ekeane/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:71:1
 #43 0x0000000012e1e98b (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5533:5
 #44 0x0000000012e20a97 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4556:9
 #45 0x0000000012e1e15d (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5391:45
 #46 0x0000000012e224e3 (anonymous namespace)::CXXNameMangler::mangleRequiresClause(clang::Expr const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2083:1
 #47 0x0000000012e27f4e (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
 #48 0x0000000012e0cc22 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:957:1
 #49 0x0000000012e0c12b (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:5
 #50 0x0000000012e2afda (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #51 0x0000000012e0ce68 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1074:5
 #52 0x0000000012e0c758 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1042:1
 #53 0x0000000012e29ae0 (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3644:1
 #54 0x0000000012e19d60 (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3641:1
 #55 0x0000000012e0d9ea (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /local/home/ekeane/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:71:1
 #56 0x0000000012e1e98b (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5533:5
 #57 0x0000000012e20a97 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4556:9
 #58 0x0000000012e1e15d (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5391:45
 #59 0x0000000012e224e3 (anonymous namespace)::CXXNameMangler::mangleRequiresClause(clang::Expr const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2083:1
 #60 0x0000000012e27f4e (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5
 #61 0x0000000012e0cc22 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:957:1
 #62 0x0000000012e0c12b (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:855:5
 #63 0x0000000012e2afda (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #64 0x0000000012e0ce68 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1074:5
 #65 0x0000000012e0c758 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1042:1
 #66 0x0000000012e29ae0 (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3644:1
 #67 0x0000000012e19d60 (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3641:1
 #68 0x0000000012e0d9ea (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /local/home/ekeane/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:71:1
 #69 0x0000000012e1e98b (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5533:5
 #70 0x0000000012e20a97 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:4556:9
 #71 0x0000000012e1e15d (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5391:45
 #72 0x0000000012e224e3 (anonymous namespace)::CXXNameMangler::mangleRequiresClause(clang::Expr const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:2083:1
 #73 0x0000000012e27f4e (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /local/home/ekeane/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:5

```

@dtcxzyw  is correct, there is some sort of infinite recursion that is happening mangling types/names.  

---

### Comment 5 - DimitryAndric

Valgrind also agrees:

```text
==39771== Memcheck, a memory error detector
==39771== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==39771== Using Valgrind-3.22.0.GIT and LibVEX; rerun with -h for copyright info
==39771== Command: ./llvmorg-18-init-06337-g933ad5c897e/bin/clang++ -std=c++20 -c pr70064.cpp
==39771==
==39771== Stack overflow in thread #1: can't grow stack to 0x1ffa001000
==39771== Can't extend stack to 0x1ffa000da8 during signal delivery for thread 1:
==39771==   too small or bad protection modes
==39771==
==39771== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==39771==  Access not within mapped region at address 0x1FFA000DA8
==39771==    at 0x6EF12B0: (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) (in /share/dim/src/llvm/bugs/pr70064/llvmorg-18-init-06337-g933ad5c897e/bin/clang-18)
==39771==  If you believe this happened as a result of a stack
==39771==  overflow in your program's main thread (unlikely but
==39771==  possible), you can try to increase the size of the
==39771==  main thread stack using the --main-stacksize= flag.
==39771==  The main thread stack size used in this run was 33554432.
==39771==
==39771== HEAP SUMMARY:
==39771==     in use at exit: 1,490,813 bytes in 3,157 blocks
==39771==   total heap usage: 35,063 allocs, 31,906 frees, 33,749,136 bytes allocated
==39771==
==39771== LEAK SUMMARY:
==39771==    definitely lost: 0 bytes in 0 blocks
==39771==    indirectly lost: 0 bytes in 0 blocks
==39771==      possibly lost: 0 bytes in 0 blocks
==39771==    still reachable: 1,490,813 bytes in 3,157 blocks
==39771==         suppressed: 0 bytes in 0 blocks
==39771== Rerun with --leak-check=full to see details of leaked memory
==39771==
==39771== For lists of detected and suppressed errors, rerun with: -s
==39771== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
Segmentation fault
```

---

### Comment 6 - erichkeane

Ooof... I don't know about how mangling should work here to fix it, but I see the problem.  

```
auto X = []<class = void>() 
    requires requires { []() {}; }
{ return 1; }();
```

Given the above, we wish to mangle the lambda on Line#1 (Named 'X').  In order to do that, we ALSO need to mangle its requires clause, since that is a differentiator between two (SHOULD it be for lambdas?).  To mangle the requires clause, we need to mangle each of the individual `Requirements`, one of which is the lambda on line #2.

HOWEVER, since that is a `Local` Class, its name depends on its containing construct.  So in order to mangle IT, we need to mangle `X` (since it is essentially named `<name of X><local class>0`).

There are a few things that could 'fix' this I guess, but we'd need guidance from someone who is more knowledgeable about the Itanium ABI (@zygoloid @rjmccall , perhaps @efriedma-quic or @asl ) as to what the correct answer here is (OR, to go to the Itanium ABI group and figure get it solved there!).

We could possibly change how we mangle lambdas in a requirements clause to always mangle as a 'non-local' class, but I'm unsure of the implications of that.


---

### Comment 7 - efriedma-quic

For the given example, it looks like gcc simply doesn't mangle the requires clause at all.

---

### Comment 8 - RungeCC

@erichkeane. I found another related issue (I'm not sure should I open another issue, as I do not know is this just exactly the same as the previous issue.), if reference a captured entity insider the requires requires, clang will run into an infinity recursion(https://godbolt.org/z/d6abc3oa5):

```c++
auto X = [x=1]<class = void>() 
    requires requires { x; }
{ return 1; }();
```

Note, `gcc` and `msvc` failed in compiling it because seems they haven't support lookup captured names inside the tailing requires clauses yet.


---

### Comment 9 - Taw3e8

This one also seems to be the same problem: https://godbolt.org/z/4PKsKGEP7

---

### Comment 10 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Runge (RungeCC)

<details>
[**Example**](https://godbolt.org/z/drarr49Pd):

```c++
// flag:
//    -std=c++23
// or -std=c++20
auto X = []&lt;class = void&gt;() 
    requires requires { []() {}; }
{ return 1; }();
```

**Version**:

Trunk, (clang 17 accepts it.):

```
clang version 18.0.0 (https://github.com/llvm/llvm-project.git cc455033d4d4f5731461a6d5fa72e0f9bf871a70)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/compiler-explorer/clang-trunk-20231024/bin
```

**Expected behavior**:

accepts this, as `gcc` and `msvc` are shown above.

**Actual behavior**:

crashed. Even, there is no stack-trace log and no assertion triggered.

**Note**:

It's astonishing that clang crashes without outputting any valid information.
</details>


---

### Comment 11 - oToToT

I cannot reproduce this error on the latest trunk. After a `git bisect`, 6d00c4297f6714c03ab10f57c10063ebd79264a1 seems fixed the issue. 

I think we can close this now?

---

### Comment 12 - DimitryAndric

Let's close it indeed, @RungeCC please feel free to reopen if it still occurs for you with recent main.

---

