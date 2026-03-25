# [clang][Sema] theoretical ABI break in LLVM 20.1.1

**Author:** isuckatcs
**URL:** https://github.com/llvm/llvm-project/issues/133955

## Body

In LLVM 20.1.0 we have the following `Sema` class.
```c++
class Sema final : public SemaBase {
  ...

private:
  /// Function or variable declarations to be checked for whether the deferred
  /// diagnostics should be emitted.
  llvm::SmallSetVector<Decl *, 4> DeclsToCheckForDeferredDiags;

  /// Map of current shadowing declarations to shadowed declarations. Warn if
  /// it looks like the user is trying to modify the shadowing declaration.
  llvm::DenseMap<const NamedDecl *, const NamedDecl *> ShadowingDecls;

  ...
};
```

In LLVM 20.1.1 this definition is changed the way below.

```c++
class Sema final : public SemaBase {
  ...

public:

  ...

  /// Function or variable declarations to be checked for whether the deferred
  /// diagnostics should be emitted.
  llvm::SmallSetVector<Decl *, 4> DeclsToCheckForDeferredDiags;

private:
  

  /// Map of current shadowing declarations to shadowed declarations. Warn if
  /// it looks like the user is trying to modify the shadowing declaration.
  llvm::DenseMap<const NamedDecl *, const NamedDecl *> ShadowingDecls;

  ...
};
```

In other words, `DeclsToCheckForDeferredDiags` is made `public` in 20.1.1, when it was `private` in 20.1.0.

Until C++23 the C++ standard said the following about this.
> [Note: Non-static data members of a (non-union) class with the same access control (11.9) and non-zero
> size (6.7.2) are allocated so that later members have higher addresses within a class object. **The order of
> allocation of non-static data members with different access control is unspecified**.
>
> N4849 § 11.4 19

With C++23 however this restriction was removed though.
> [Note 8 : Non-variant non-static data members of non-zero size (6.7.2) are allocated so that later members 
> have higher addresses within a class object (7.6.9).
>
> N4950 § 11.4.1 20

LLVM is still compiled targeting the C++17 standard, which means, the first quote of the standard applies and compilers have the right to change the field order of `Sema` IIUC.

Normally this ABI break is the part of `libclangSema.a`, however we also have `libclang-cpp.so`, which collects all of the static libraries of clang and makes it possible to link against them dynamically. If an application is linked against clang dynamically, then according to the standard `libclang-cpp.so.20.1.1` is not compatible with `libclang-cpp.so.20.1`.

I'm not sure if there are compilers taking advantage of this, but if they do, the resulting `libclang-cpp.so` might not be ABI compatible with it's previous release, as fields might have different offsets within the object.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (isuckatcs)

<details>
In LLVM 20.1.0 we have the following `Sema` class.
```c++
class Sema final : public SemaBase {
  ...

private:
  /// Function or variable declarations to be checked for whether the deferred
  /// diagnostics should be emitted.
  llvm::SmallSetVector&lt;Decl *, 4&gt; DeclsToCheckForDeferredDiags;

  /// Map of current shadowing declarations to shadowed declarations. Warn if
  /// it looks like the user is trying to modify the shadowing declaration.
  llvm::DenseMap&lt;const NamedDecl *, const NamedDecl *&gt; ShadowingDecls;

  ...
};
```

In LLVM 20.1.1 this definition is changed the way below.

```c++
class Sema final : public SemaBase {
  ...

public:

  ...

  /// Function or variable declarations to be checked for whether the deferred
  /// diagnostics should be emitted.
  llvm::SmallSetVector&lt;Decl *, 4&gt; DeclsToCheckForDeferredDiags;

private:
  

  /// Map of current shadowing declarations to shadowed declarations. Warn if
  /// it looks like the user is trying to modify the shadowing declaration.
  llvm::DenseMap&lt;const NamedDecl *, const NamedDecl *&gt; ShadowingDecls;

  ...
};
```

In other words, `DeclsToCheckForDeferredDiags` is made `public` in 20.1.1, when it was `private` in 20.1.0.

Until C++23 the C++ standard said the following about this.
&gt; [Note: Non-static data members of a (non-union) class with the same access control (11.9) and non-zero
&gt; size (6.7.2) are allocated so that later members have higher addresses within a class object. **The order of
&gt; allocation of non-static data members with different access control is unspecified**.
&gt;
&gt; N4849 § 11.4 19

With C++23 however this restriction was removed though.
&gt; [Note 8 : Non-variant non-static data members of non-zero size (6.7.2) are allocated so that later members 
&gt; have higher addresses within a class object (7.6.9).
&gt;
&gt; N4950 § 11.4.1 20

LLVM is still compiled targeting the C++17 standard, which means, the first quote of the standard applies and compilers have the right to change the field order of `Sema` IIUC.

Normally this ABI break is the part of `libclangSema.a`, however we also have `libclang-cpp.so`, which collects all of the static libraries of clang and makes it possible to link against them dynamically. If an application is linked against clang dynamically, then according to the standard `libclang-cpp.so.20.1.1` is not compatible with `libclang-cpp.so.20.1`.

I'm not sure if there are compilers taking advantage of this, but if they do, the resulting `libclang-cpp.so` might not be ABI compatible with it's previous release, as fields might have different offsets within the object.
</details>


---

### Comment 2 - isuckatcs

I was wrong in that this is not an issue in `libclang.so`, but an issue in `libclang-cpp.so`, so I updated the wording accordingly.

---

### Comment 3 - shafik

CC @tstellar is this a "real" ABI break? I am hearing from some folks we have an ABI validator tool, is this something we run on reach release?

---

### Comment 4 - tstellar

We do have an ABI validator that we run, but it doesn't seem to work any more.  It hasn't been catching obvious breaks and I don't think it ever worked for checking enum values.

---

### Comment 5 - Endilll

> I'm not sure if there are compilers taking advantage of this, but if they do, the resulting libclang-cpp.so might not be ABI compatible with it's previous release, as fields might have different offsets within the object.

According to [P1847R4](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1847r4.pdf), only EDG implemented that, but none of their customers actually used the ability to reorder data members that have different access specifiers. I agree that theoretically this is an ABI break, but I doubt that layout of `Sema` has actually changed in 20.1.1.

---

### Comment 6 - isuckatcs

Here is another interesting change, I found.

LLVM 20.1.0:

```c++
class MachineInstr {
public:
  bool isIndirectBranch(QueryType Type = AnyInBundle) const {
    return hasProperty(MCID::IndirectBranch, Type);
  }
};
```

LLVM 20.1.1:

```c++
class MachineInstr {
public:
  bool isIndirectBranch(QueryType Type = AnyInBundle,
                        bool IncludeJumpTable = true) const {
    return hasProperty(MCID::IndirectBranch, Type) &&
           (IncludeJumpTable || !llvm::any_of(operands(), [](const auto &Op) {
              return Op.isJTI();
            }));
  }
};
```
Notice how the signature of the function has changed. Since it is defined inside the class body, it should be inlined in the user code, and even if the compiler decides not to inline it, the definition should always be present in the translatation unit, where the method is called.

Some guides however still not recommend doing something like this. The [KDE Binary Compatibility Policy](https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B) states the following:

> You can ... change an inline function or make an inline function non-inline if it is safe that programs linked with the prior version of the library call the old implementation. This is tricky and might be dangerous. Think twice before doing it.

I'm not sure how the program can call the new implementation, as IIUC, every TU, where the symbol of the method is used should contain the definition.

The only concern that might arise is that having one definition in the consumer, and loading the new version of the library, that contains a completely different definition of the same symbol, will lead to ODR violation, as it is also pointed out by this other [C++ ABI stability Guidelines](https://github.com/react-native-community/discussions-and-proposals/issues/257#top), I found. 

> Changing the implementation of some methods in a somewhat incompatible manner with the previous implementation might cause logical errors in the code. This can happen because some of the previous implementations (possibly conflicting with the new one) of those functions can be inlined into the application. To make it safe, the old and new implementations of the same function must be able to work together. Besides that, even if it usually works fine, theoretically, having two different implementations of the same function inside one executable is a violation of the [one definition rule](https://en.cppreference.com/w/cpp/language/definition).

IIRC, an ODR violation is condiered UB.

---

### Comment 7 - philnik777

While both things presented here are hypothetically ABI break, they aren't in reality. The original case will not change the layout in any implementation, and the second case changes the mangling of the function, so they're fine to exist in a single application. I'm not even sure the seconds case is an ODR violation. I'm quite certain it wouldn't be an ODR violation if they were free functions.

---

### Comment 8 - isuckatcs

> The original case will not change the layout in any implementation

The `Sema` fields are declared in the following order:
```c++
class Sema {
public:
  [Group1]
private:
  [Group2]
public:
  [Group3]
private:
  [Group4]
public:
  [Group5] <-- DeclsToCheckForDeferredDiags in 20.1.1
private:
  [Group6] <-- DeclsToCheckForDeferredDiags in 20.1.0
public:
  [Group7]

... I stopped counting here
};
```
Quoting the `P1847R4` proposal:
> EDG has a configuration flag (“targ_field_alloc_sequence_equals_decl_sequence”) that can be **set to false, in
which case** we first lay out **public field, followed by protected fields, followed by private fields**. But not aware of an
actual customer using it and can't recall a customer report with that mode used.

If `targ_field_alloc_sequence_equals_decl_sequence` is turned off, the EDG front-end does change the layout of `Sema`.

```c++
class Sema {
public:
  [Group1]
  [Group3]
  [Group5] <-- DeclsToCheckForDeferredDiags in 20.1.1
  [Group7]

private:
  [Group2]
  [Group4]
  [Group6] <-- DeclsToCheckForDeferredDiags in 20.1.0
};
```
I agree however that in the most popular compilers, like `clang`, `gcc` and `msvc`, this is not a real issue.

---

### Comment 9 - isuckatcs

However, since I started looking at our minor releases recently, I want to share a pretty obvious ABI break, that is easily reproducable with LLVM 15.0.0 and 15.0.1.

```c++
// LLVM 15.0.0
class MarkupFilter {
public:
  MarkupFilter(raw_ostream &OS, Optional<bool> ColorsEnabled = llvm::None);
  ...

private:
  ...

  raw_ostream &OS;
  const bool ColorsEnabled;

  ...
};
```

```c++
// LLVM 15.0.1
class MarkupFilter {
public:
  MarkupFilter(raw_ostream &OS, LLVMSymbolizer &Symbolizer,
               Optional<bool> ColorsEnabled = llvm::None);
  ...

private:
  ...

  raw_ostream &OS;
  LLVMSymbolizer &Symbolizer;
  const bool ColorsEnabled;

  ...
};
```
In 15.0.1, both the layout is changed, and a symbol is removed. Also, this actually brakes the API too. See [`8f820dd`](https://github.com/llvm/llvm-project/commit/8f820dd89e9b27523db809ad8e205d4b0ddbf284) for the full change.

To see it in action, compile the following snippet and link it against `libllvm.so.15.0.0`, then just install `libllvm.so.15.0.1` and run the executable once again to see the loader error out.
```c++
#include <llvm/DebugInfo/Symbolize/MarkupFilter.h>

int main() {
  llvm::symbolize::MarkupFilter MF(llvm::errs());

  return 0;
}
```

+1: [`ea96040`](https://github.com/llvm/llvm-project/commit/ea960400213ad6a619d40536ae8965fca70eac89#diff-1995b973a05d8f751094d612c29537f582b4d563bbf46423487b710a34e2de97L99-L106) is somewhat easy to reproduce too, as it changes the vtable layout of a class, clients can derive from, in LLVM 19.1.5.

---

### Comment 10 - Endilll

> If targ_field_alloc_sequence_equals_decl_sequence is turned off, the EDG front-end does change the layout of Sema.

As said both in paper and in my message, EDG is not aware of customers that use that, so it's still in the realm of theoretical ABI breaks.

---

