# [clang] Crash in release mode and assertion fail in build with asserts when compiling for Windows with -std=c++17

**Author:** Fznamznon
**URL:** https://github.com/llvm/llvm-project/issues/144081
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash-on-valid
**Closed Date:** 2025-06-26T20:30:44Z

## Body

Code

```
struct a { int j; };
template<int a::* i> void g_() { }
void bar() {
  g_< (int a::*)0 >();
}

Additional options required: -target x86_64-windows -std=c++17
```
godbolt demo https://godbolt.org/z/GcMGcW4PE

Seems like clang 21 regression.

Triage notes:

1. The assertion message is:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/MicrosoftCXXABI.cpp:234: clang::MSInheritanceModel clang::CXXRecordDecl::getMSInheritanceModel() const: Assertion `IA && "Expected MSInheritanceAttr on the CXXRecordDecl!"' failed.
```

Seems `MSInheritanceAttr`  is missing.

2. The attribute is added in `assignInheritanceModel` function defined in SemaType.cpp if the sample is compiled in C++14 mode. We never enter it when compiling in C++17 mode. The codepath to `assignInheritanceModel` leads through `Sema::CheckTemplateArgument` and then `CheckTemplateArgumentPointerToMember`. In C++17 mode we exit from `Sema::CheckTemplateArgument` earlier and never call `assignInheritanceModel`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mariya Podchishchaeva (Fznamznon)

<details>
Code

```
struct a { int j; };
template&lt;int a::* i&gt; void g_() { }
void bar() {
  g_&lt; (int a::*)0 &gt;();
}

Additional options required: -target x86_64-windows -std=c++17
```
godbolt demo https://godbolt.org/z/GcMGcW4PE

Seems like clang 21 regression.

Triage notes:

1. The assertion message is:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/MicrosoftCXXABI.cpp:234: clang::MSInheritanceModel clang::CXXRecordDecl::getMSInheritanceModel() const: Assertion `IA &amp;&amp; "Expected MSInheritanceAttr on the CXXRecordDecl!"' failed.
```

Seems `MSInheritanceAttr`  is missing.

2. The attribute is added in `assignInheritanceModel` function defined in SemaType.cpp if the sample is compiled in C++14 mode. We never enter it when compiling in C++17 mode. The codepath to `assignInheritanceModel` leads though `Sema::CheckTemplateArgument` and then `CheckTemplateArgumentPointerToMember`. In C++17 mode we exit from `Sema::CheckTemplateArgument` earlier and never call `assignInheritanceModel`
</details>


---

### Comment 2 - shafik

@mizvekov this bisect down to 6003c3055a4630be31cc3d459cdbb88248a007b9

---

### Comment 3 - mizvekov

Should be fixed by https://github.com/llvm/llvm-project/pull/145958

---

