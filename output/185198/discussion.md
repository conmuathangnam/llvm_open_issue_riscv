# Clang 22 crash when diagnosing invalid pointer-to-member in in-class initializer with templated constructor

**Author:** cicuvc
**URL:** https://github.com/llvm/llvm-project/issues/185198
**Status:** Closed
**Labels:** clang:frontend, crash, regression:21
**Closed Date:** 2026-03-10T00:52:13Z

## Body

### Description

Clang-22 crashes while compiling a small ill-formed program that attempts to pass a pointer to a member function to a templated constructor during an in-class member initializer. The compiler first emits a diagnostic about an invalid lookup (&::func), then crashes during template argument deduction.

This appears to be a frontend crash and should likely produce only a diagnostic instead of terminating. It also crashes clang-21 and clang-20 does not crash.

### Reproducer

```cpp
struct A{
    template<typename T>
    A(void (T::*f)()){}
};

struct C{
    A op{&::func};
    void func(){

    }
};
```

### Command

```
clang-22 -c test.cpp
```


### Actual Behavior

Clang reports the following diagnostic:

```
test.cpp:7:11: error: no member named 'func' in the global namespace; did you mean '::C::func'?
    A op{&::func};
          ^~~~~~
          ::C::func
test.cpp:8:10: note: '::C::func' declared here
```

After emitting the diagnostic, the compiler crashes with a segmentation fault during template argument deduction.

### Expected Behavior

Clang should emit the diagnostic and terminate normally without crashing.

### Crash Backtrace

```
Stack dump:
0.      Program arguments: /opt/LLVM-22.1.0-Linux-X64/bin/clang-22 -c test.cpp
1.      <eof> parser at end of file
2.      test.cpp:6:1: parsing struct/union/class body 'C'
 #0 0x0000557113c05017 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0x8a05017)
 #1 0x0000557113c04c35 llvm::sys::CleanupOnSignal(unsigned long) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0x8a04c35)
 #2 0x0000557113bb592e (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x0000557113bb5afe CrashRecoverySignalHandler(int) (.llvm.15320821377120032643) CrashRecoveryContext.cpp:0:0
 #4 0x00007f926a85a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #5 0x0000557116f5519d clang::MemberPointerType::isSugared() const (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xbd5519d)
 #6 0x00005571169c70ea DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) (.llvm.17434987631688419319) SemaTemplateDeduction.cpp:0:0
 #7 0x00005571169b82b2 DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) (.llvm.17434987631688419319) SemaTemplateDeduction.cpp:0:0
 #8 0x0000557116f70466 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xbd70466)
 #9 0x0000557116f6e4ee AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.llvm.1530312495864442754) SemaOverload.cpp:0:0
#10 0x0000557116fcffb0 IsUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&, clang::OverloadCandidateSet&, clang::Sema::AllowedExplicit, bool) (.llvm.1530312495864442754) SemaOverload.cpp:0:0
#11 0x0000557116fc87cc TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#12 0x00005571166c2899 clang::Sema::AddOverloadCandidate(clang::FunctionDecl*, clang::DeclAccessPair, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, bool, clang::CallExpr::ADLCallKind, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool, bool) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4c2899)
#13 0x00005571166bf1f0 TryConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::QualType, clang::InitializationSequence&, bool, bool) SemaInit.cpp:0:0
#14 0x00005571166b3230 TryListInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, clang::InitListExpr*, clang::InitializationSequence&, bool) SemaInit.cpp:0:0
#15 0x00005571166bbd6c clang::InitializationSequence::InitializationSequence(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4bbd6c)
#16 0x00005571166d12bd clang::Sema::ConvertMemberDefaultInitExpression(clang::FieldDecl*, clang::Expr*, clang::SourceLocation) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4d12bd)
#17 0x000055711660ddc4 clang::Sema::ActOnFinishCXXInClassMemberInitializer(clang::Decl*, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb40ddc4)
#18 0x00005571164f436e clang::Parser::ParseLexedMemberInitializer(clang::Parser::LateParsedMemberInitializer&) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb2f436e)
...
```

(Full output and stack trace attached in the original crash output [crash.txt](https://github.com/user-attachments/files/25815331/crash.txt))

### Environment

```
clang version 22.1.0 (https://github.com/llvm/llvm-project 4434dabb69916856b824f68a64b029c67175e532)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/LLVM-22.1.0-Linux-X64/bin
```

Preprocessed source: [test-629db4.cpp.txt](https://github.com/user-attachments/files/25815338/test-629db4.cpp.txt)
Script: [test-629db4.sh](https://github.com/user-attachments/files/25815343/test-629db4.sh)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (cicuvc)

<details>
### Description

Clang-22 crashes while compiling a small ill-formed program that attempts to pass a pointer to a member function to a templated constructor during an in-class member initializer. The compiler first emits a diagnostic about an invalid lookup (&amp;::func), then crashes during template argument deduction.

This appears to be a frontend crash and should likely produce only a diagnostic instead of terminating. It also crashes clang-21 and clang-20 does not crash.

### Reproducer

```cpp
struct A{
    template&lt;typename T&gt;
    A(void (T::*f)()){}
};

struct C{
    A op{&amp;::func};
    void func(){

    }
};
```

### Command

```
clang-22 -c test.cpp
```


### Actual Behavior

Clang reports the following diagnostic:

```
test.cpp:7:11: error: no member named 'func' in the global namespace; did you mean '::C::func'?
    A op{&amp;::func};
          ^~~~~~
          ::C::func
test.cpp:8:10: note: '::C::func' declared here
```

After emitting the diagnostic, the compiler crashes with a segmentation fault during template argument deduction.

### Expected Behavior

Clang should emit the diagnostic and terminate normally without crashing.

### Crash Backtrace

```
Stack dump:
0.      Program arguments: /opt/LLVM-22.1.0-Linux-X64/bin/clang-22 -c test.cpp
1.      &lt;eof&gt; parser at end of file
2.      test.cpp:6:1: parsing struct/union/class body 'C'
 #<!-- -->0 0x0000557113c05017 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0x8a05017)
 #<!-- -->1 0x0000557113c04c35 llvm::sys::CleanupOnSignal(unsigned long) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0x8a04c35)
 #<!-- -->2 0x0000557113bb592e (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000557113bb5afe CrashRecoverySignalHandler(int) (.llvm.15320821377120032643) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007f926a85a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->5 0x0000557116f5519d clang::MemberPointerType::isSugared() const (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xbd5519d)
 #<!-- -->6 0x00005571169c70ea DeduceTemplateArgumentsByTypeMatch(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, PartialOrderingKind, bool, bool*) (.llvm.17434987631688419319) SemaTemplateDeduction.cpp:0:0
 #<!-- -->7 0x00005571169b82b2 DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) (.llvm.17434987631688419319) SemaTemplateDeduction.cpp:0:0
 #<!-- -->8 0x0000557116f70466 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xbd70466)
 #<!-- -->9 0x0000557116f6e4ee AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.llvm.1530312495864442754) SemaOverload.cpp:0:0
#<!-- -->10 0x0000557116fcffb0 IsUserDefinedConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&amp;, clang::OverloadCandidateSet&amp;, clang::Sema::AllowedExplicit, bool) (.llvm.1530312495864442754) SemaOverload.cpp:0:0
#<!-- -->11 0x0000557116fc87cc TryImplicitConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#<!-- -->12 0x00005571166c2899 clang::Sema::AddOverloadCandidate(clang::FunctionDecl*, clang::DeclAccessPair, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, bool, clang::CallExpr::ADLCallKind, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool, bool) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4c2899)
#<!-- -->13 0x00005571166bf1f0 TryConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType, clang::QualType, clang::InitializationSequence&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->14 0x00005571166b3230 TryListInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, clang::InitListExpr*, clang::InitializationSequence&amp;, bool) SemaInit.cpp:0:0
#<!-- -->15 0x00005571166bbd6c clang::InitializationSequence::InitializationSequence(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4bbd6c)
#<!-- -->16 0x00005571166d12bd clang::Sema::ConvertMemberDefaultInitExpression(clang::FieldDecl*, clang::Expr*, clang::SourceLocation) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb4d12bd)
#<!-- -->17 0x000055711660ddc4 clang::Sema::ActOnFinishCXXInClassMemberInitializer(clang::Decl*, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb40ddc4)
#<!-- -->18 0x00005571164f436e clang::Parser::ParseLexedMemberInitializer(clang::Parser::LateParsedMemberInitializer&amp;) (/opt/LLVM-22.1.0-Linux-X64/bin/clang-22+0xb2f436e)
...
```

(Full output and stack trace attached in the original crash output [crash.txt](https://github.com/user-attachments/files/25815331/crash.txt))

### Environment

```
clang version 22.1.0 (https://github.com/llvm/llvm-project 4434dabb69916856b824f68a64b029c67175e532)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/LLVM-22.1.0-Linux-X64/bin
```

Preprocessed source: [test-629db4.cpp.txt](https://github.com/user-attachments/files/25815338/test-629db4.cpp.txt)
Script: [test-629db4.sh](https://github.com/user-attachments/files/25815343/test-629db4.sh)

</details>


---

### Comment 2 - shafik

godbolt links are always helpful, this looks fixed on trunk: https://godbolt.org/z/bWYxqovWh

---

