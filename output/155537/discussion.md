# [clang] namespace duplicated in getQualifiedNameAsString output

**Author:** rupprecht
**URL:** https://github.com/llvm/llvm-project/issues/155537
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-08-27T21:04:56Z

## Body

Calling `getQualifiedNameAsString()` for a function decl in a namespace will end up duplicating the namespace.

Test case (using the pattern from [LookupTest.cpp](https://github.com/llvm/llvm-project/blob/main/clang/unittests/Tooling/LookupTest.cpp)):

```cc
namespace {
struct CheckFunctionDeclVisitor : TestVisitor {
  bool VisitFunctionDecl(FunctionDecl *FD) override {
    EXPECT_EQ("x::y::Foo::Foo<T>", FD->getQualifiedNameAsString());
    return true;
  }
};
}

TEST(LookupTest, namespaceInQualifiedName) {
  CheckFunctionDeclVisitor Visitor;
  Visitor.runOver(
      "namespace x { namespace y {\n"
      "template <typename T> class Foo { Foo() {} };\n"
      "}}");
}
```

This passes prior to #147835. After that, and still at trunk, it fails:
```cc
Expected equality of these values:
  "x::y::Foo::Foo<T>"
  FD->getQualifiedNameAsString()
    Which is: "x::y::Foo::x::y::Foo<T>"
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jordan Rupprecht (rupprecht)

<details>
Calling `getQualifiedNameAsString()` for a function decl in a namespace will end up duplicating the namespace.

Test case (using the pattern from [LookupTest.cpp](https://github.com/llvm/llvm-project/blob/main/clang/unittests/Tooling/LookupTest.cpp)):

```cc
namespace {
struct CheckFunctionDeclVisitor : TestVisitor {
  bool VisitFunctionDecl(FunctionDecl *FD) override {
    EXPECT_EQ("x::y::Foo::Foo&lt;T&gt;", FD-&gt;getQualifiedNameAsString());
    return true;
  }
};
}

TEST(LookupTest, namespaceInQualifiedName) {
  CheckFunctionDeclVisitor Visitor;
  Visitor.runOver(
      "namespace x { namespace y {\n"
      "template &lt;typename T&gt; class Foo { Foo() {} };\n"
      "}}");
}
```

This passes prior to #<!-- -->147835. After that, and still at trunk, it fails:
```cc
Expected equality of these values:
  "x::y::Foo::Foo&lt;T&gt;"
  FD-&gt;getQualifiedNameAsString()
    Which is: "x::y::Foo::x::y::Foo&lt;T&gt;"
```
</details>


---

