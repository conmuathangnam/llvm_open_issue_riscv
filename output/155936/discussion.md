# [clang][ast] Inner struct with same name missing from AST layout

**Author:** rupprecht
**URL:** https://github.com/llvm/llvm-project/issues/155936
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-08-29T03:33:57Z

## Body

We have some tests failing after #147835 (plus fixes) that use this as the test snippet for their matcher:

```cc
struct Foo {
    struct A {
        struct Foo {};
    };
};
```

Before: https://compiler-explorer.com/z/7e6836e6G

```
TranslationUnitDecl
`-CXXRecordDecl <line:1:1, line:5:1> line:1:8 struct Foo definition
  |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
  | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl <col:1, col:8> col:8 implicit struct Foo
  `-CXXRecordDecl <line:2:5, line:4:5> line:2:12 struct A definition
    |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
    | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
    | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveConstructor exists simple trivial needs_implicit
    | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveAssignment exists simple trivial needs_implicit
    | `-Destructor simple irrelevant trivial needs_implicit
    |-CXXRecordDecl <col:5, col:12> col:12 implicit struct A
    `-CXXRecordDecl <line:3:9, col:21> col:16 struct Foo definition
      |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
      | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
      | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
      | |-MoveConstructor exists simple trivial needs_implicit
      | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
      | |-MoveAssignment exists simple trivial needs_implicit
      | `-Destructor simple irrelevant trivial needs_implicit
      `-CXXRecordDecl <col:9, col:16> col:16 implicit struct Foo
```

After: https://compiler-explorer.com/z/oeK8jaKh8

```
TranslationUnitDecl
`-CXXRecordDecl <line:1:1, line:5:1> line:1:8 struct Foo definition
  |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
  | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl <col:1, col:8> col:8 implicit struct Foo
  `-CXXRecordDecl <line:2:5, line:4:5> line:2:12 struct A definition
    |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
    | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
    | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveConstructor exists simple trivial needs_implicit
    | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveAssignment exists simple trivial needs_implicit
    | `-Destructor simple irrelevant trivial needs_implicit
    `-CXXRecordDecl <col:5, col:12> col:12 implicit struct A
```

The `Foo::A::Foo` type is missing from the AST layout, and the AST matcher does not reach it. (The test tries to find a match for `Foo::A::Foo` and fails).

This seems to only affect the AST. Actual codegen/behavior seems unchanged. For example:

```cc
struct Foo {
    int xx;
    struct A {
        struct Foo {
            int yy;  // "yy" does not appear in AST
        };
    };
};

void f() {
    Foo t;
    t.xx = 1;  // OK
    t.yy = 1;  // error: no member named 'yy' in 'Foo'

    Foo::A::Foo z;
    z.xx = 1;  // error: no member named 'xx' in 'Foo::A::Foo'
    z.yy = 1;  // OK
}
```

(Note: I tested this w/ #147835 plus fixes, and also at trunk. I did _not_ test yet with #147835 alone. The regression may be due to one of the fixes instead.)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jordan Rupprecht (rupprecht)

<details>
We have some tests failing after #<!-- -->147835 (plus fixes) that use this as the test snippet for their matcher:

```cc
struct Foo {
    struct A {
        struct Foo {};
    };
};
```

Before: https://compiler-explorer.com/z/7e6836e6G

```
TranslationUnitDecl
`-CXXRecordDecl &lt;line:1:1, line:5:1&gt; line:1:8 struct Foo definition
  |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
  | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl &lt;col:1, col:8&gt; col:8 implicit struct Foo
  `-CXXRecordDecl &lt;line:2:5, line:4:5&gt; line:2:12 struct A definition
    |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
    | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
    | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveConstructor exists simple trivial needs_implicit
    | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveAssignment exists simple trivial needs_implicit
    | `-Destructor simple irrelevant trivial needs_implicit
    |-CXXRecordDecl &lt;col:5, col:12&gt; col:12 implicit struct A
    `-CXXRecordDecl &lt;line:3:9, col:21&gt; col:16 struct Foo definition
      |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
      | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
      | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
      | |-MoveConstructor exists simple trivial needs_implicit
      | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
      | |-MoveAssignment exists simple trivial needs_implicit
      | `-Destructor simple irrelevant trivial needs_implicit
      `-CXXRecordDecl &lt;col:9, col:16&gt; col:16 implicit struct Foo
```

After: https://compiler-explorer.com/z/oeK8jaKh8

```
TranslationUnitDecl
`-CXXRecordDecl &lt;line:1:1, line:5:1&gt; line:1:8 struct Foo definition
  |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
  | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl &lt;col:1, col:8&gt; col:8 implicit struct Foo
  `-CXXRecordDecl &lt;line:2:5, line:4:5&gt; line:2:12 struct A definition
    |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
    | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
    | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveConstructor exists simple trivial needs_implicit
    | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
    | |-MoveAssignment exists simple trivial needs_implicit
    | `-Destructor simple irrelevant trivial needs_implicit
    `-CXXRecordDecl &lt;col:5, col:12&gt; col:12 implicit struct A
```

The `Foo::A::Foo` type is missing from the AST layout, and the AST matcher does not reach it. (The test tries to find a match for `Foo::A::Foo` and fails).

This seems to only affect the AST. Actual codegen/behavior seems unchanged. For example:

```cc
struct Foo {
    int xx;
    struct A {
        struct Foo {
            int yy;  // "yy" does not appear in AST
        };
    };
};

void f() {
    Foo t;
    t.xx = 1;  // OK
    t.yy = 1;  // error: no member named 'yy' in 'Foo'

    Foo::A::Foo z;
    z.xx = 1;  // error: no member named 'xx' in 'Foo::A::Foo'
    z.yy = 1;  // OK
}
```

(Note: I tested this w/ #<!-- -->147835 plus fixes, and also at trunk. I did _not_ test yet with #<!-- -->147835 alone. The regression may be due to one of the fixes instead.)
</details>


---

