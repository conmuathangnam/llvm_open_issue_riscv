# [clangd] "Expected valid TemplateArgument" for `std::min`-like template

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/139019
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, crash-on-valid, regression:20
**Closed Date:** 2025-05-29T05:08:13Z

## Body

For these two files:
```c++
// test.cpp
#include "test.h"

void run(int num_total_tasks) {
  int cnt{};
  int x;

  for (int i = 0; i < min(x, num_total_tas - cnt); ++i) {}
}
```

```c++
// test.h
template<typename T>
T min(T A, T B) { return A < B ? A : B; }
```
`bin/clangd --check=test.cpp --check-completion` crashes:
```console
Expected valid TemplateArgument
UNREACHABLE executed at /home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp:68!
Process 3542402 stopped
* thread #1, name = 'clangd', stop reason = signal SIGABRT
    frame #0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
libc.so.6`__pthread_kill_implementation:
->  0x7fff8287ff54 <+276>: movl   %eax, %ebx
    0x7fff8287ff56 <+278>: negl   %ebx
    0x7fff8287ff58 <+280>: cmpl   $0xfffff000, %eax ; imm = 0xFFFFF000
    0x7fff8287ff5d <+285>: movl   $0x0, %eax
(lldb) bt
* thread #1, name = 'clangd', stop reason = signal SIGABRT
  * frame #0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
    frame #1: 0x00007fff82826f9e libc.so.6`raise + 30
    frame #2: 0x00007fff8280e942 libc.so.6`abort + 223
    frame #3: 0x00007fffa25151d5 libLLVM.so.21.0git`llvm::llvm_unreachable_internal(msg="Expected valid TemplateArgument", file="/home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp", line=68) at ErrorHandling.cpp:233:3
    frame #4: 0x00007fffe8b473c9 libclang-cpp.so.21.0git`(anonymous namespace)::TemplateArgumentHasher::AddTemplateArgument(this=0x00007bff808b3e40, TA=TemplateArgument @ 0x00007bff8071a720) at TemplateArgumentHasher.cpp:68:5
    frame #5: 0x00007fffe8b46f3c libclang-cpp.so.21.0git`clang::serialization::StableHashForTemplateArguments(Args=ArrayRef<clang::TemplateArgument> @ 0x00007bff808b3e20) at TemplateArgumentHasher.cpp:409:12
    frame #6: 0x00007fffe83b71f0 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializationsImpl(this=0x00007e8f8218d200, SpecLookups=0x00007e8f8218e188, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef<clang::TemplateArgument> @ 0x00007ffffffe0d30) at ASTReader.cpp:8388:20
    frame #7: 0x00007fffe83b77b3 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializations(this=0x00007e8f8218d200, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef<clang::TemplateArgument> @ 0x00007ffffffe0e00) at ASTReader.cpp:8412:7
    frame #8: 0x00007fffdd9eae82 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::loadLazySpecializationsImpl(this=0x00007e0f841bb1e8, Args=ArrayRef<clang::TemplateArgument> @ 0x00007ffffffe0ea8, TPL=0x0000000000000000) const at DeclTemplate.cpp:377:26
    frame #9: 0x00007fffdda040e8 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::SpecEntryTraits<clang::FunctionTemplateSpecializationInfo>::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl<clang::FunctionTemplateSpecializationInfo, llvm::ArrayRef<clang::TemplateArgument>&>(this=0x00007e0f841bb1e8, Specs=0x00007e0f841bb250, InsertPos=0x00007bff80bd9040, ProfileArgs=0x00007bff805c8f20) at DeclTemplate.cpp:405:8
    frame #10: 0x00007fffdd9eba3d libclang-cpp.so.21.0git`clang::FunctionTemplateDecl::findSpecialization(this=0x00007e0f841bb1e8, Args=ArrayRef<clang::TemplateArgument> @ 0x00007bff805c8f20, InsertPos=0x00007bff80bd9040) at DeclTemplate.cpp:487:10
    frame #11: 0x00007fffe405c00c libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8, TemplateParams=0x0000000000000000, FunctionRewriteKind=None) at SemaTemplateInstantiateDecl.cpp:2623:27
    frame #12: 0x00007fffe407cf13 libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at SemaTemplateInstantiateDecl.cpp:4343:10
    frame #13: 0x00007fffe3fe94b0 libclang-cpp.so.21.0git`clang::declvisitor::Base<std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at DeclNodes.inc:256:1
    frame #14: 0x00007fffe4223810 libclang-cpp.so.21.0git`clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0::operator()(this=0x00007bff808b39a0) const at SemaTemplateInstantiateDecl.cpp:4616:27
    frame #15: 0x00007fffe4223715 libclang-cpp.so.21.0git`void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(callable=136337303484832) at STLFunctionalExtras.h:46:12
    frame #16: 0x00007fffdb93ae2c libclang-cpp.so.21.0git`llvm::function_ref<void ()>::operator()(this=0x00007bff805c8de0) const at STLFunctionalExtras.h:69:12
    frame #17: 0x00007fffdb93b6a5 libclang-cpp.so.21.0git`clang::runWithSufficientStackSpace(Diag=function_ref<void ()> @ 0x00007ffffffe21b8, Fn=function_ref<void ()> @ 0x00007bff805c8de0) at Stack.h:49:7
    frame #18: 0x00007fffdb93b00c libclang-cpp.so.21.0git`clang::StackExhaustionHandler::runWithSufficientStackSpace(this=0x00007e8f82192460, Loc=(ID = 2147483606), Fn=function_ref<void ()> @ 0x00007ffffffe22b8) at StackExhaustionHandler.cpp:20:3
    frame #19: 0x00007fffe09b87a6 libclang-cpp.so.21.0git`clang::Sema::runWithSufficientStackSpace(this=0x00007e8f82192200, Loc=(ID = 2147483606), Fn=function_ref<void ()> @ 0x00007ffffffe2338) at Sema.cpp:606:16
    frame #20: 0x00007fffe4081b2e libclang-cpp.so.21.0git`clang::Sema::SubstDecl(this=0x00007e8f82192200, D=0x00007e0f841bb3c8, Owner=0x00007e0f841b6d30, TemplateArgs=0x00007bff80a8bb40) at SemaTemplateInstantiateDecl.cpp:4615:3
    frame #21: 0x00007fffe3a68a1d libclang-cpp.so.21.0git`clang::Sema::FinishTemplateArgumentDeduction(this=0x00007e8f82192200, FunctionTemplate=0x00007e0f841bb1e8, Deduced=0x00007bff80bd8220, NumExplicitlySpecified=0, Specialization=0x00007bff80a8b2e0, Info=0x00007bff80a8b0a0, OriginalCallArgs=0x00007bff80bd8400, PartialOverloading=true, PartialOrdering=false, CheckNonDependent=function_ref<bool ()> @ 0x00007bff80a8b820) at SemaTemplateDeduction.cpp:3947:7
```

The template argument is:
```
(clang::TemplateArgument) {
   = {
    DeclArg = {
      Kind = Null
      IsDefaulted = false
      QT = 0x0000000000000000
      D = 0x00007fffd22b2140
    }
    Integer = {
      Kind = Null
      IsDefaulted = false
      BitWidth = 32766
      IsUnsigned = false
       = {
        VAL = 0
        pVal = 0x0000000000000000
      }
      Type = 0x00007fffd22b2140
    }
    Value = {
      Kind = Null
      IsDefaulted = false
      Value = nullptr
      Type = 0x00007fffd22b2140
    }
    Args = {
      Kind = Null
      IsDefaulted = false
      NumArgs = 32766
      Args = nullptr
    }
    TemplateArg = {
      Kind = Null
      IsDefaulted = false
      NumExpansions = (Rep = 32766)
      Name = 0x0000000000000000
    }
    TypeOrValue = (Kind = Null, IsDefaulted = false, IsCanonicalExpr = false, V = 0)
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Timm Baeder (tbaederr)

<details>
For these two files:
```c++
// test.cpp
#include "test.h"

void run(int num_total_tasks) {
  int cnt{};
  int x;

  for (int i = 0; i &lt; min(x, num_total_tas - cnt); ++i) {}
}
```

```c++
// test.h
template&lt;typename T&gt;
T min(T A, T B) { return A &lt; B ? A : B; }
```
`bin/clangd --check=test.cpp --check-completion` crashes:
```console
Expected valid TemplateArgument
UNREACHABLE executed at /home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp:68!
Process 3542402 stopped
* thread #<!-- -->1, name = 'clangd', stop reason = signal SIGABRT
    frame #<!-- -->0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
libc.so.6`__pthread_kill_implementation:
-&gt;  0x7fff8287ff54 &lt;+276&gt;: movl   %eax, %ebx
    0x7fff8287ff56 &lt;+278&gt;: negl   %ebx
    0x7fff8287ff58 &lt;+280&gt;: cmpl   $0xfffff000, %eax ; imm = 0xFFFFF000
    0x7fff8287ff5d &lt;+285&gt;: movl   $0x0, %eax
(lldb) bt
* thread #<!-- -->1, name = 'clangd', stop reason = signal SIGABRT
  * frame #<!-- -->0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
    frame #<!-- -->1: 0x00007fff82826f9e libc.so.6`raise + 30
    frame #<!-- -->2: 0x00007fff8280e942 libc.so.6`abort + 223
    frame #<!-- -->3: 0x00007fffa25151d5 libLLVM.so.21.0git`llvm::llvm_unreachable_internal(msg="Expected valid TemplateArgument", file="/home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp", line=68) at ErrorHandling.cpp:233:3
    frame #<!-- -->4: 0x00007fffe8b473c9 libclang-cpp.so.21.0git`(anonymous namespace)::TemplateArgumentHasher::AddTemplateArgument(this=0x00007bff808b3e40, TA=TemplateArgument @ 0x00007bff8071a720) at TemplateArgumentHasher.cpp:68:5
    frame #<!-- -->5: 0x00007fffe8b46f3c libclang-cpp.so.21.0git`clang::serialization::StableHashForTemplateArguments(Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007bff808b3e20) at TemplateArgumentHasher.cpp:409:12
    frame #<!-- -->6: 0x00007fffe83b71f0 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializationsImpl(this=0x00007e8f8218d200, SpecLookups=0x00007e8f8218e188, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0d30) at ASTReader.cpp:8388:20
    frame #<!-- -->7: 0x00007fffe83b77b3 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializations(this=0x00007e8f8218d200, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0e00) at ASTReader.cpp:8412:7
    frame #<!-- -->8: 0x00007fffdd9eae82 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::loadLazySpecializationsImpl(this=0x00007e0f841bb1e8, Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0ea8, TPL=0x0000000000000000) const at DeclTemplate.cpp:377:26
    frame #<!-- -->9: 0x00007fffdda040e8 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::SpecEntryTraits&lt;clang::FunctionTemplateSpecializationInfo&gt;::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl&lt;clang::FunctionTemplateSpecializationInfo, llvm::ArrayRef&lt;clang::TemplateArgument&gt;&amp;&gt;(this=0x00007e0f841bb1e8, Specs=0x00007e0f841bb250, InsertPos=0x00007bff80bd9040, ProfileArgs=0x00007bff805c8f20) at DeclTemplate.cpp:405:8
    frame #<!-- -->10: 0x00007fffdd9eba3d libclang-cpp.so.21.0git`clang::FunctionTemplateDecl::findSpecialization(this=0x00007e0f841bb1e8, Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007bff805c8f20, InsertPos=0x00007bff80bd9040) at DeclTemplate.cpp:487:10
    frame #<!-- -->11: 0x00007fffe405c00c libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8, TemplateParams=0x0000000000000000, FunctionRewriteKind=None) at SemaTemplateInstantiateDecl.cpp:2623:27
    frame #<!-- -->12: 0x00007fffe407cf13 libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at SemaTemplateInstantiateDecl.cpp:4343:10
    frame #<!-- -->13: 0x00007fffe3fe94b0 libclang-cpp.so.21.0git`clang::declvisitor::Base&lt;std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*&gt;::Visit(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at DeclNodes.inc:256:1
    frame #<!-- -->14: 0x00007fffe4223810 libclang-cpp.so.21.0git`clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0::operator()(this=0x00007bff808b39a0) const at SemaTemplateInstantiateDecl.cpp:4616:27
    frame #<!-- -->15: 0x00007fffe4223715 libclang-cpp.so.21.0git`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0&gt;(callable=136337303484832) at STLFunctionalExtras.h:46:12
    frame #<!-- -->16: 0x00007fffdb93ae2c libclang-cpp.so.21.0git`llvm::function_ref&lt;void ()&gt;::operator()(this=0x00007bff805c8de0) const at STLFunctionalExtras.h:69:12
    frame #<!-- -->17: 0x00007fffdb93b6a5 libclang-cpp.so.21.0git`clang::runWithSufficientStackSpace(Diag=function_ref&lt;void ()&gt; @ 0x00007ffffffe21b8, Fn=function_ref&lt;void ()&gt; @ 0x00007bff805c8de0) at Stack.h:49:7
    frame #<!-- -->18: 0x00007fffdb93b00c libclang-cpp.so.21.0git`clang::StackExhaustionHandler::runWithSufficientStackSpace(this=0x00007e8f82192460, Loc=(ID = 2147483606), Fn=function_ref&lt;void ()&gt; @ 0x00007ffffffe22b8) at StackExhaustionHandler.cpp:20:3
    frame #<!-- -->19: 0x00007fffe09b87a6 libclang-cpp.so.21.0git`clang::Sema::runWithSufficientStackSpace(this=0x00007e8f82192200, Loc=(ID = 2147483606), Fn=function_ref&lt;void ()&gt; @ 0x00007ffffffe2338) at Sema.cpp:606:16
    frame #<!-- -->20: 0x00007fffe4081b2e libclang-cpp.so.21.0git`clang::Sema::SubstDecl(this=0x00007e8f82192200, D=0x00007e0f841bb3c8, Owner=0x00007e0f841b6d30, TemplateArgs=0x00007bff80a8bb40) at SemaTemplateInstantiateDecl.cpp:4615:3
    frame #<!-- -->21: 0x00007fffe3a68a1d libclang-cpp.so.21.0git`clang::Sema::FinishTemplateArgumentDeduction(this=0x00007e8f82192200, FunctionTemplate=0x00007e0f841bb1e8, Deduced=0x00007bff80bd8220, NumExplicitlySpecified=0, Specialization=0x00007bff80a8b2e0, Info=0x00007bff80a8b0a0, OriginalCallArgs=0x00007bff80bd8400, PartialOverloading=true, PartialOrdering=false, CheckNonDependent=function_ref&lt;bool ()&gt; @ 0x00007bff80a8b820) at SemaTemplateDeduction.cpp:3947:7
```

The template argument is:
```
(clang::TemplateArgument) {
   = {
    DeclArg = {
      Kind = Null
      IsDefaulted = false
      QT = 0x0000000000000000
      D = 0x00007fffd22b2140
    }
    Integer = {
      Kind = Null
      IsDefaulted = false
      BitWidth = 32766
      IsUnsigned = false
       = {
        VAL = 0
        pVal = 0x0000000000000000
      }
      Type = 0x00007fffd22b2140
    }
    Value = {
      Kind = Null
      IsDefaulted = false
      Value = nullptr
      Type = 0x00007fffd22b2140
    }
    Args = {
      Kind = Null
      IsDefaulted = false
      NumArgs = 32766
      Args = nullptr
    }
    TemplateArg = {
      Kind = Null
      IsDefaulted = false
      NumExpansions = (Rep = 32766)
      Name = 0x0000000000000000
    }
    TypeOrValue = (Kind = Null, IsDefaulted = false, IsCanonicalExpr = false, V = 0)
  }
}
```
</details>


---

### Comment 2 - tbaederr

I bisected this to 20e904950967c125abc1e91f57e5a373987ff016

CC @ChuanqiXu9 

---

### Comment 3 - ChuanqiXu9

> I bisected this to [20e9049](https://github.com/llvm/llvm-project/commit/20e904950967c125abc1e91f57e5a373987ff016)
> 
> CC [@ChuanqiXu9](https://github.com/ChuanqiXu9)

I suspect it relates to `spec_begin` and `spec_end`. The story is, if more specializations are loaded, the iterators may be invalided.

Would you like to verify if it is the case? If yes, may be we can fix the problem by storing all iterators before traversing.

(I am busy with other things and might not be able to do detailed debugging recently, sorry)

---

### Comment 4 - tbaederr

I know little to nothing about this part of clang, but if you can give me some details, I can verify that, yes.

---

### Comment 5 - ChuanqiXu9

> I know little to nothing about this part of clang, but if you can give me some details, I can verify that, yes.

The pattern I suspect is:

```
for (auto &Iter : template_decl.specializations()) // RedeclarableTemplateDecl::specializations() is the point
{
     ...
}
```

or 

```
auto Iter = template_decl.spec_begin();
auto E = template_decl.spec_end();
for (; Iter != E; Iter++) {
     ...
}
```

the pattern is, the core happens at `Iter++`. In another word, invalided iterator.

---

### Comment 6 - ChuanqiXu9

See the comment from https://github.com/clangd/clangd/issues/2387#issuecomment-2870427641



---

### Comment 7 - HighCommander4

(Note the duplicate report has a testcase which is valid code, so adding `crash-on-valid`.)

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
For these two files:
```c++
// test.cpp
#include "test.h"

void run(int num_total_tasks) {
  int cnt{};
  int x;

  for (int i = 0; i &lt; min(x, num_total_tas - cnt); ++i) {}
}
```

```c++
// test.h
template&lt;typename T&gt;
T min(T A, T B) { return A &lt; B ? A : B; }
```
`bin/clangd --check=test.cpp --check-completion` crashes:
```console
Expected valid TemplateArgument
UNREACHABLE executed at /home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp:68!
Process 3542402 stopped
* thread #<!-- -->1, name = 'clangd', stop reason = signal SIGABRT
    frame #<!-- -->0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
libc.so.6`__pthread_kill_implementation:
-&gt;  0x7fff8287ff54 &lt;+276&gt;: movl   %eax, %ebx
    0x7fff8287ff56 &lt;+278&gt;: negl   %ebx
    0x7fff8287ff58 &lt;+280&gt;: cmpl   $0xfffff000, %eax ; imm = 0xFFFFF000
    0x7fff8287ff5d &lt;+285&gt;: movl   $0x0, %eax
(lldb) bt
* thread #<!-- -->1, name = 'clangd', stop reason = signal SIGABRT
  * frame #<!-- -->0: 0x00007fff8287ff54 libc.so.6`__pthread_kill_implementation + 276
    frame #<!-- -->1: 0x00007fff82826f9e libc.so.6`raise + 30
    frame #<!-- -->2: 0x00007fff8280e942 libc.so.6`abort + 223
    frame #<!-- -->3: 0x00007fffa25151d5 libLLVM.so.21.0git`llvm::llvm_unreachable_internal(msg="Expected valid TemplateArgument", file="/home/tbaeder/code/llvm-project/clang/lib/Serialization/TemplateArgumentHasher.cpp", line=68) at ErrorHandling.cpp:233:3
    frame #<!-- -->4: 0x00007fffe8b473c9 libclang-cpp.so.21.0git`(anonymous namespace)::TemplateArgumentHasher::AddTemplateArgument(this=0x00007bff808b3e40, TA=TemplateArgument @ 0x00007bff8071a720) at TemplateArgumentHasher.cpp:68:5
    frame #<!-- -->5: 0x00007fffe8b46f3c libclang-cpp.so.21.0git`clang::serialization::StableHashForTemplateArguments(Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007bff808b3e20) at TemplateArgumentHasher.cpp:409:12
    frame #<!-- -->6: 0x00007fffe83b71f0 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializationsImpl(this=0x00007e8f8218d200, SpecLookups=0x00007e8f8218e188, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0d30) at ASTReader.cpp:8388:20
    frame #<!-- -->7: 0x00007fffe83b77b3 libclang-cpp.so.21.0git`clang::ASTReader::LoadExternalSpecializations(this=0x00007e8f8218d200, D=0x00007e0f841bb1e8, TemplateArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0e00) at ASTReader.cpp:8412:7
    frame #<!-- -->8: 0x00007fffdd9eae82 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::loadLazySpecializationsImpl(this=0x00007e0f841bb1e8, Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007ffffffe0ea8, TPL=0x0000000000000000) const at DeclTemplate.cpp:377:26
    frame #<!-- -->9: 0x00007fffdda040e8 libclang-cpp.so.21.0git`clang::RedeclarableTemplateDecl::SpecEntryTraits&lt;clang::FunctionTemplateSpecializationInfo&gt;::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl&lt;clang::FunctionTemplateSpecializationInfo, llvm::ArrayRef&lt;clang::TemplateArgument&gt;&amp;&gt;(this=0x00007e0f841bb1e8, Specs=0x00007e0f841bb250, InsertPos=0x00007bff80bd9040, ProfileArgs=0x00007bff805c8f20) at DeclTemplate.cpp:405:8
    frame #<!-- -->10: 0x00007fffdd9eba3d libclang-cpp.so.21.0git`clang::FunctionTemplateDecl::findSpecialization(this=0x00007e0f841bb1e8, Args=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007bff805c8f20, InsertPos=0x00007bff80bd9040) at DeclTemplate.cpp:487:10
    frame #<!-- -->11: 0x00007fffe405c00c libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8, TemplateParams=0x0000000000000000, FunctionRewriteKind=None) at SemaTemplateInstantiateDecl.cpp:2623:27
    frame #<!-- -->12: 0x00007fffe407cf13 libclang-cpp.so.21.0git`clang::TemplateDeclInstantiator::VisitFunctionDecl(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at SemaTemplateInstantiateDecl.cpp:4343:10
    frame #<!-- -->13: 0x00007fffe3fe94b0 libclang-cpp.so.21.0git`clang::declvisitor::Base&lt;std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*&gt;::Visit(this=0x00007bff808b3840, D=0x00007e0f841bb3c8) at DeclNodes.inc:256:1
    frame #<!-- -->14: 0x00007fffe4223810 libclang-cpp.so.21.0git`clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0::operator()(this=0x00007bff808b39a0) const at SemaTemplateInstantiateDecl.cpp:4616:27
    frame #<!-- -->15: 0x00007fffe4223715 libclang-cpp.so.21.0git`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0&gt;(callable=136337303484832) at STLFunctionalExtras.h:46:12
    frame #<!-- -->16: 0x00007fffdb93ae2c libclang-cpp.so.21.0git`llvm::function_ref&lt;void ()&gt;::operator()(this=0x00007bff805c8de0) const at STLFunctionalExtras.h:69:12
    frame #<!-- -->17: 0x00007fffdb93b6a5 libclang-cpp.so.21.0git`clang::runWithSufficientStackSpace(Diag=function_ref&lt;void ()&gt; @ 0x00007ffffffe21b8, Fn=function_ref&lt;void ()&gt; @ 0x00007bff805c8de0) at Stack.h:49:7
    frame #<!-- -->18: 0x00007fffdb93b00c libclang-cpp.so.21.0git`clang::StackExhaustionHandler::runWithSufficientStackSpace(this=0x00007e8f82192460, Loc=(ID = 2147483606), Fn=function_ref&lt;void ()&gt; @ 0x00007ffffffe22b8) at StackExhaustionHandler.cpp:20:3
    frame #<!-- -->19: 0x00007fffe09b87a6 libclang-cpp.so.21.0git`clang::Sema::runWithSufficientStackSpace(this=0x00007e8f82192200, Loc=(ID = 2147483606), Fn=function_ref&lt;void ()&gt; @ 0x00007ffffffe2338) at Sema.cpp:606:16
    frame #<!-- -->20: 0x00007fffe4081b2e libclang-cpp.so.21.0git`clang::Sema::SubstDecl(this=0x00007e8f82192200, D=0x00007e0f841bb3c8, Owner=0x00007e0f841b6d30, TemplateArgs=0x00007bff80a8bb40) at SemaTemplateInstantiateDecl.cpp:4615:3
    frame #<!-- -->21: 0x00007fffe3a68a1d libclang-cpp.so.21.0git`clang::Sema::FinishTemplateArgumentDeduction(this=0x00007e8f82192200, FunctionTemplate=0x00007e0f841bb1e8, Deduced=0x00007bff80bd8220, NumExplicitlySpecified=0, Specialization=0x00007bff80a8b2e0, Info=0x00007bff80a8b0a0, OriginalCallArgs=0x00007bff80bd8400, PartialOverloading=true, PartialOrdering=false, CheckNonDependent=function_ref&lt;bool ()&gt; @ 0x00007bff80a8b820) at SemaTemplateDeduction.cpp:3947:7
```

The template argument is:
```
(clang::TemplateArgument) {
   = {
    DeclArg = {
      Kind = Null
      IsDefaulted = false
      QT = 0x0000000000000000
      D = 0x00007fffd22b2140
    }
    Integer = {
      Kind = Null
      IsDefaulted = false
      BitWidth = 32766
      IsUnsigned = false
       = {
        VAL = 0
        pVal = 0x0000000000000000
      }
      Type = 0x00007fffd22b2140
    }
    Value = {
      Kind = Null
      IsDefaulted = false
      Value = nullptr
      Type = 0x00007fffd22b2140
    }
    Args = {
      Kind = Null
      IsDefaulted = false
      NumArgs = 32766
      Args = nullptr
    }
    TemplateArg = {
      Kind = Null
      IsDefaulted = false
      NumExpansions = (Rep = 32766)
      Name = 0x0000000000000000
    }
    TypeOrValue = (Kind = Null, IsDefaulted = false, IsCanonicalExpr = false, V = 0)
  }
}
```
</details>


---

### Comment 9 - HighCommander4

I sent out https://github.com/llvm/llvm-project/pull/139489 to clean up the "arguments `std::forwarded` multiple times" situation discussed in the comment linked above, but indeed it does not resolve the crash.

---

### Comment 10 - tbaederr

I fixed it!
```diff
diff --git i/clang/lib/Sema/SemaTemplateDeduction.cpp w/clang/lib/Sema/SemaTemplateDeduction.cpp
index 5dc06ebc2a23..032c5b314fd1 100644
--- i/clang/lib/Sema/SemaTemplateDeduction.cpp
+++ w/clang/lib/Sema/SemaTemplateDeduction.cpp
@@ -3045,6 +3045,7 @@ static TemplateDeductionResult ConvertDeducedTemplateArguments(
     // consider template parameters that were deduced, disregarding any default
     // arguments.
     if (IsIncomplete) {
+      Template->dump();
       *IsIncomplete = true;
       CTAI.SugaredConverted.push_back({});
       CTAI.CanonicalConverted.push_back({});
```
(wtf?)

---

### Comment 11 - tbaederr

Removing that entire if block also fixes it. CC @mizvekov for some more pointers.

---

### Comment 12 - ChuanqiXu9

> I fixed it!
> 
> diff --git i/clang/lib/Sema/SemaTemplateDeduction.cpp w/clang/lib/Sema/SemaTemplateDeduction.cpp
> index 5dc06ebc2a23..032c5b314fd1 100644
> --- i/clang/lib/Sema/SemaTemplateDeduction.cpp
> +++ w/clang/lib/Sema/SemaTemplateDeduction.cpp
> @@ -3045,6 +3045,7 @@ static TemplateDeductionResult ConvertDeducedTemplateArguments(
>      // consider template parameters that were deduced, disregarding any default
>      // arguments.
>      if (IsIncomplete) {
> +      Template->dump();
>        *IsIncomplete = true;
>        CTAI.SugaredConverted.push_back({});
>        CTAI.CanonicalConverted.push_back({});
> (wtf?)

`dump` will trigger the compiler to load a lot of things. I guess this is the reason.

---

### Comment 13 - tbaederr

In `Sema::DeduceTemplateArguments()`, `Deduced` will ultimately contain one default-initialized `DeducedTemplateArgument` (i.e. Kind = null) and we won't deduce any call arguments because here the call has zero non-dependent arguments.

---

### Comment 14 - HighCommander4

This is collecting a fair amount of duplicates. Should we consider a revert of the regressing patch?

---

### Comment 15 - zyn0217

> This is collecting a fair amount of duplicates. Should we consider a revert of the regressing patch?

That is not an option because afaik aa7497a66c4 has been there for a couple of months and is already heavily depended upon. Reverting it would cause us a lot of pain.

Ping @mizvekov 

---

### Comment 16 - mizvekov

Sorry, I think I missed the first notification on this bug. I will take a look.

---

### Comment 17 - mizvekov

Should be fixed by https://github.com/llvm/llvm-project/pull/141890

---

