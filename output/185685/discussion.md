# [lldb] Assertion "Base subobject externally placed at overlapping offset" with multiple empty bases from aliased template template params

**Author:** bmilanich
**URL:** https://github.com/llvm/llvm-project/issues/185685
**Status:** Open
**Labels:** lldb, crash

## Body

[repro_final.cpp](https://github.com/user-attachments/files/25876618/repro_final.cpp)

LLDB's expression evaluator crashes with:
  ```
  Assertion failed: (Allowed && "Base subobject externally placed at overlapping offset"),
  function LayoutBase, file RecordLayoutBuilder.cpp, line 1283
  ```

  ## Reproducer

LLDB must be built with assertions enabled (`-DLLVM_ENABLE_ASSERTIONS=ON`).

  ```cpp
  template <typename T, template <typename> class TT>
  struct Empty {};

  template <typename T, template <typename> class TT>
  using Alias = Empty<T, TT>;

  struct A {
    template <typename U>
    struct Trait : Alias<U, Trait> {};
  };

  struct B {
    template <typename U>
    struct Trait : Alias<U, Trait> {};
  };

  class C;

  class C : public A::Trait<C>, public B::Trait<C> {
  public:
    int x = 0;
    int getX() const;
  };

  int C::getX() const { return x; }

  __attribute__((noinline, optnone))
  void f(C obj) { __builtin_debugtrap(); }

  int main() {
    C obj;
    obj.x = 1;
    f(obj);
  }
  ```

  ```
  clang++ -std=c++20 -g -O0 -o repro repro.cpp
  lldb --batch -o "b f" -o run -o "p obj.getX()" -- ./repro
  ```

  ## Analysis

  The template alias `Alias` produces two `DW_TAG_typedef` entries with identical
  `DW_AT_name` (`"Alias<C, Trait>"`) because the nested `Trait` is unqualified in
  both `A::Trait` and `B::Trait`. The underlying `Empty<>` types are correctly
  distinct in DWARF (different `DW_TAG_GNU_template_template_param` values), but
  LLDB's `EmptySubobjectMap` sees two "identical" empty bases at offset 0 and
  asserts.

  Removing the alias (inheriting `Empty<U, Trait>` directly) avoids the assertion
  — LLDB can then distinguish the types.

  This pattern is common in MLIR (`StorageUserBase` + `Interface::Trait` +
  `AttributeTrait::TraitBase`).

  ## Environment

  - Binary compiled with Apple clang 17.0.0 (clang-1700.0.13.3), arm64-apple-darwin
  - Tested on lldb trunk (e3e4f161d, version 23.0.0git) — assertion fires
  - Also reproduces with clang 20.1.8 compiled binaries

## Related issues

  - #97443 — touches the same ExternalLayout code path in RecordLayoutBuilder
  - #72913 — same assertion category (LLDB layout reconstruction from DWARF)
  - #57059 — similar theme of DWARF lacking info for correct layout reconstruction

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Basil Milanich (bmilanich)

<details>
[repro_final.cpp](https://github.com/user-attachments/files/25876618/repro_final.cpp)

LLDB's expression evaluator crashes with:
  ```
  Assertion failed: (Allowed &amp;&amp; "Base subobject externally placed at overlapping offset"),
  function LayoutBase, file RecordLayoutBuilder.cpp, line 1283
  ```

  ## Reproducer

LLDB must be built with assertions enabled (`-DLLVM_ENABLE_ASSERTIONS=ON`).

  ```cpp
  template &lt;typename T, template &lt;typename&gt; class TT&gt;
  struct Empty {};

  template &lt;typename T, template &lt;typename&gt; class TT&gt;
  using Alias = Empty&lt;T, TT&gt;;

  struct A {
    template &lt;typename U&gt;
    struct Trait : Alias&lt;U, Trait&gt; {};
  };

  struct B {
    template &lt;typename U&gt;
    struct Trait : Alias&lt;U, Trait&gt; {};
  };

  class C;

  class C : public A::Trait&lt;C&gt;, public B::Trait&lt;C&gt; {
  public:
    int x = 0;
    int getX() const;
  };

  int C::getX() const { return x; }

  __attribute__((noinline, optnone))
  void f(C obj) { __builtin_debugtrap(); }

  int main() {
    C obj;
    obj.x = 1;
    f(obj);
  }
  ```

  ```
  clang++ -std=c++20 -g -O0 -o repro repro.cpp
  lldb --batch -o "b f" -o run -o "p obj.getX()" -- ./repro
  ```

  ## Analysis

  The template alias `Alias` produces two `DW_TAG_typedef` entries with identical
  `DW_AT_name` (`"Alias&lt;C, Trait&gt;"`) because the nested `Trait` is unqualified in
  both `A::Trait` and `B::Trait`. The underlying `Empty&lt;&gt;` types are correctly
  distinct in DWARF (different `DW_TAG_GNU_template_template_param` values), but
  LLDB's `EmptySubobjectMap` sees two "identical" empty bases at offset 0 and
  asserts.

  Removing the alias (inheriting `Empty&lt;U, Trait&gt;` directly) avoids the assertion
  — LLDB can then distinguish the types.

  This pattern is common in MLIR (`StorageUserBase` + `Interface::Trait` +
  `AttributeTrait::TraitBase`).

  ## Environment

  - Binary compiled with Apple clang 17.0.0 (clang-1700.0.13.3), arm64-apple-darwin
  - Tested on lldb trunk (e3e4f161d, version 23.0.0git) — assertion fires
  - Also reproduces with clang 20.1.8 compiled binaries

## Related issues

  - #<!-- -->97443 — touches the same ExternalLayout code path in RecordLayoutBuilder
  - #<!-- -->72913 — same assertion category (LLDB layout reconstruction from DWARF)
  - #<!-- -->57059 — similar theme of DWARF lacking info for correct layout reconstruction
</details>


---

### Comment 2 - Michael137

Could you attach a DWARF dump (`dwarfdump repro.dSYM`)?
My gut reaction was an interaction between `-gtemplate-alias` and `-gsimple-template-names`. But would be good to see the actual DWARF output

EDIT: nvm, of course simple template names are not involved since the name includes the template parameters as you point out. And this is also using the plain `DW_TAG_typedef`. So unlikely that anything changed in debug-info recently that caused this.

---

### Comment 3 - Michael137

Really `Trait` should just be qualified in the DW_TAG_typedef

---

### Comment 4 - Michael137

Hmm looks like that's already the case when compiling with top-of-tree:
```
0x00000078:   DW_TAG_typedef
                DW_AT_type      (0x0000000000000080 "Empty<C, A::Trait>")
                DW_AT_name      ("Alias<C, A::Trait>")
                DW_AT_decl_file ("/tmp/alias.cpp")
                DW_AT_decl_line (5)

0x000000aa:   DW_TAG_typedef
                DW_AT_type      (0x00000000000000b2 "Empty<C, B::Trait>")
                DW_AT_name      ("Alias<C, B::Trait>")
                DW_AT_decl_file ("/tmp/alias.cpp")
                DW_AT_decl_line (5)
```

Can't repro crash with top-of-tree clang+lldb (assertions enabled)

---

### Comment 5 - bmilanich

Yeah, it looks like the latest clang produces better type info. On the current xcode's clang (17) and the clang we use in our code base (20) it has this:
```
bmilanich@Basils-MacBook-Pro ~ % grep -A 5 DW_TAG_typedef < lldb-repro-xcode.txt
0x00000077:   DW_TAG_typedef
                DW_AT_type	(0x000000000000007f "Empty<C, A::Trait>")
                DW_AT_name	("Alias<C, Trait>")
                DW_AT_decl_file	("/tmp/llvm-repro/repro_final.cpp")
                DW_AT_decl_line	(16)

--
0x000000a9:   DW_TAG_typedef
                DW_AT_type	(0x00000000000000b1 "Empty<C, B::Trait>")
                DW_AT_name	("Alias<C, Trait>")
                DW_AT_decl_file	("/tmp/llvm-repro/repro_final.cpp")
                DW_AT_decl_line	(16)

bmilanich@Basils-MacBook-Pro ~ % grep -A 5 DW_TAG_typedef < lldb-repro-clang20.txt
0x0000007e:   DW_TAG_typedef
                DW_AT_type	(0x00000086 "Empty<C, A::Trait>")
                DW_AT_name	("Alias<C, Trait>")
                DW_AT_decl_file	("/tmp/llvm-repro/repro_final.cpp")
                DW_AT_decl_line	(16)

--
0x000000b0:   DW_TAG_typedef
                DW_AT_type	(0x000000b8 "Empty<C, B::Trait>")
                DW_AT_name	("Alias<C, Trait>")
                DW_AT_decl_file	("/tmp/llvm-repro/repro_final.cpp")
                DW_AT_decl_line	(16)

```
Both typedefs with the same DW_AT_name.

I figure that it's not worth fixing in the LLDB as it's unlikely somebody is using the latest LLDB with an old clang. We have a workaround for this so this issue could be closed.

[lldb-repro-clang20.txt](https://github.com/user-attachments/files/25904354/lldb-repro-clang20.txt)
[lldb-repro-xcode.txt](https://github.com/user-attachments/files/25904355/lldb-repro-xcode.txt)

---

