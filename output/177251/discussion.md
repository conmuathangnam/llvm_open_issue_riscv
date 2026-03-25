# [clang] Incomplete AST traversal of QualifiedTypeLoc by traversal matchers

**Author:** upsj
**URL:** https://github.com/llvm/llvm-project/issues/177251

## Body

Starting with #147835, the traversal via the `hasDescendant()`/`forEachDescendant()` matcher of a QualifiedTypeLoc node seems to skip over its inner node.
With the input
```cpp
template <typename T>
struct S{};

const S<int> s = {};
```
before we matched
```
clang-query> match qualifiedTypeLoc(forEachDescendant(typeLoc().bind("loc")))

Match #1:

Binding for "loc":
TemplateSpecializationTypeLoc </home/tribizel/llvm-project/example.cpp:7:7, col:12> 'S<int>'
|-name: 'S' qualified
| `-ClassTemplateDecl 0x48a44e0 <line:1:1, line:2:10> col:8 S
`-TemplateArgument <line:7:9> type 'int'
  `-BuiltinType 0x4855a00 'int'

Binding for "root":
QualifiedTypeLoc </home/tribizel/llvm-project/example.cpp:7:7, col:12> 'const S<int>'
`-ElaboratedTypeLoc <col:7, col:12> 'S<int>'
  `-TemplateSpecializationTypeLoc <col:7, col:12> 'S<int>'
    |-name: 'S' qualified
    | `-ClassTemplateDecl 0x48a44e0 <line:1:1, line:2:10> col:8 S
    `-TemplateArgument <line:7:9> type 'int'
      `-BuiltinType 0x4855a00 'int'


Match #2:

Binding for "loc":
BuiltinTypeLoc </home/tribizel/llvm-project/example.cpp:7:9> 'int'

Binding for "root":
QualifiedTypeLoc </home/tribizel/llvm-project/example.cpp:7:7, col:12> 'const S<int>'
`-ElaboratedTypeLoc <col:7, col:12> 'S<int>'
  `-TemplateSpecializationTypeLoc <col:7, col:12> 'S<int>'
    |-name: 'S' qualified
    | `-ClassTemplateDecl 0x48a44e0 <line:1:1, line:2:10> col:8 S
    `-TemplateArgument <line:7:9> type 'int'
      `-BuiltinType 0x4855a00 'int'
```
and afterwards we only match
```
clang-query> match qualifiedTypeLoc(forEachDescendant(typeLoc().bind("loc")))

Match #1:

Binding for "loc":
BuiltinTypeLoc </home/tribizel/llvm-project/example.cpp:7:9> 'int'

Binding for "root":
QualifiedTypeLoc </home/tribizel/llvm-project/example.cpp:7:7, col:12> 'const S<int>'
|-name: 'S' qualified
| `-ClassTemplateDecl 0x1cd77a0 <line:1:1, line:2:10> col:8 S
`-TemplateSpecializationTypeLoc <line:7:7, col:12> 'S<int>'
  |-name: 'S' qualified
  | `-ClassTemplateDecl 0x1cd77a0 <line:1:1, line:2:10> col:8 S
  `-TemplateArgument <line:7:9> type 'int'
    `-BuiltinType 0x1c89600 'int'
```
This seems like a regression in the AST traversal matcher implementation. Unfortunately I'm not familiar enough with the visitor implementation yet to be able to fix this myself directly.

cc @mizvekov in case you know what might have caused this, or whether this is intended behavior


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tobias Ribizel (upsj)

<details>
Starting with #<!-- -->147835, the traversal via the `hasDescendant()`/`forEachDescendant()` matcher of a QualifiedTypeLoc node seems to skip over its inner node.
With the input
```cpp
template &lt;typename T&gt;
struct S{};

const S&lt;int&gt; s = {};
```
before we matched
```
clang-query&gt; match qualifiedTypeLoc(forEachDescendant(typeLoc().bind("loc")))

Match #<!-- -->1:

Binding for "loc":
TemplateSpecializationTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:7, col:12&gt; 'S&lt;int&gt;'
|-name: 'S' qualified
| `-ClassTemplateDecl 0x48a44e0 &lt;line:1:1, line:2:10&gt; col:8 S
`-TemplateArgument &lt;line:7:9&gt; type 'int'
  `-BuiltinType 0x4855a00 'int'

Binding for "root":
QualifiedTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:7, col:12&gt; 'const S&lt;int&gt;'
`-ElaboratedTypeLoc &lt;col:7, col:12&gt; 'S&lt;int&gt;'
  `-TemplateSpecializationTypeLoc &lt;col:7, col:12&gt; 'S&lt;int&gt;'
    |-name: 'S' qualified
    | `-ClassTemplateDecl 0x48a44e0 &lt;line:1:1, line:2:10&gt; col:8 S
    `-TemplateArgument &lt;line:7:9&gt; type 'int'
      `-BuiltinType 0x4855a00 'int'


Match #<!-- -->2:

Binding for "loc":
BuiltinTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:9&gt; 'int'

Binding for "root":
QualifiedTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:7, col:12&gt; 'const S&lt;int&gt;'
`-ElaboratedTypeLoc &lt;col:7, col:12&gt; 'S&lt;int&gt;'
  `-TemplateSpecializationTypeLoc &lt;col:7, col:12&gt; 'S&lt;int&gt;'
    |-name: 'S' qualified
    | `-ClassTemplateDecl 0x48a44e0 &lt;line:1:1, line:2:10&gt; col:8 S
    `-TemplateArgument &lt;line:7:9&gt; type 'int'
      `-BuiltinType 0x4855a00 'int'
```
and afterwards we only match
```
clang-query&gt; match qualifiedTypeLoc(forEachDescendant(typeLoc().bind("loc")))

Match #<!-- -->1:

Binding for "loc":
BuiltinTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:9&gt; 'int'

Binding for "root":
QualifiedTypeLoc &lt;/home/tribizel/llvm-project/example.cpp:7:7, col:12&gt; 'const S&lt;int&gt;'
|-name: 'S' qualified
| `-ClassTemplateDecl 0x1cd77a0 &lt;line:1:1, line:2:10&gt; col:8 S
`-TemplateSpecializationTypeLoc &lt;line:7:7, col:12&gt; 'S&lt;int&gt;'
  |-name: 'S' qualified
  | `-ClassTemplateDecl 0x1cd77a0 &lt;line:1:1, line:2:10&gt; col:8 S
  `-TemplateArgument &lt;line:7:9&gt; type 'int'
    `-BuiltinType 0x1c89600 'int'
```
This seems like a regression in the AST traversal matcher implementation. Unfortunately I'm not familiar enough with the visitor implementation yet to be able to fix this myself directly.

cc @<!-- -->mizvekov in case you know what might have caused this, or whether this is intended behavior

</details>


---

