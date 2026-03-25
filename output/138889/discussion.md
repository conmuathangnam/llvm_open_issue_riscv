# [clang][concepts] Clang doesn't properly diagnose concept return types with insufficient template arg count

**Author:** erichkeane
**URL:** https://github.com/llvm/llvm-project/issues/138889

## Body

Given:

```
template<typename T> concept C = true;
```
We properly diagnose:
```
void foo(C<int> auto);
```
as:
`<source>:7:10: error: too many template arguments for concept 'C'`

This happens as a part of the call to `InventTemplateParameter`.

However, we don't do the same for return types or trailing return types:
```
C<int> auto Foo();
auto Foo2() -> C<int> auto;
```
Should both diagnose, in Block, File, or Record scope.

However, calls to the `InventTemplateParameter` all need the `InventedParameterInfos` to have something in it, which it obviously doesn't, since there are no parameters to be filled in.  Likely we need to do something similar with return types, though it isn't clear where that could come from.

@zygoloid did something similar in `https://github.com/llvm/llvm-project/commit/9cf98d26e7b1204478cc13ae3df44a6843965c11` for trailing return types, but only applied it to inside a lambda or prototype context, and not File/Block/Record context, so it is incomplete.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Erich Keane (erichkeane)

<details>
Given:

```
template&lt;typename T&gt; concept C = true;
```
We properly diagnose:
```
void foo(C&lt;int&gt; auto);
```
as:
`&lt;source&gt;:7:10: error: too many template arguments for concept 'C'`

This happens as a part of the call to `InventTemplateParameter`.

However, we don't do the same for return types or trailing return types:
```
C&lt;int&gt; auto Foo();
auto Foo2() -&gt; C&lt;int&gt; auto;
```
Should both diagnose, in Block, File, or Record scope.

However, calls to the `InventTemplateParameter` all need the `InventedParameterInfos` to have something in it, which it obviously doesn't, since there are no parameters to be filled in.  Likely we need to do something similar with return types, though it isn't clear where that could come from.

@<!-- -->zygoloid did something similar in `https://github.com/llvm/llvm-project/commit/9cf98d26e7b1204478cc13ae3df44a6843965c11` for trailing return types, but only applied it to inside a lambda or prototype context, and not File/Block/Record context, so it is incomplete.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Erich Keane (erichkeane)

<details>
Given:

```
template&lt;typename T&gt; concept C = true;
```
We properly diagnose:
```
void foo(C&lt;int&gt; auto);
```
as:
`&lt;source&gt;:7:10: error: too many template arguments for concept 'C'`

This happens as a part of the call to `InventTemplateParameter`.

However, we don't do the same for return types or trailing return types:
```
C&lt;int&gt; auto Foo();
auto Foo2() -&gt; C&lt;int&gt; auto;
```
Should both diagnose, in Block, File, or Record scope.

However, calls to the `InventTemplateParameter` all need the `InventedParameterInfos` to have something in it, which it obviously doesn't, since there are no parameters to be filled in.  Likely we need to do something similar with return types, though it isn't clear where that could come from.

@<!-- -->zygoloid did something similar in `https://github.com/llvm/llvm-project/commit/9cf98d26e7b1204478cc13ae3df44a6843965c11` for trailing return types, but only applied it to inside a lambda or prototype context, and not File/Block/Record context, so it is incomplete.
</details>


---

### Comment 3 - shafik

Interesting MSVC catches none of them: https://godbolt.org/z/s3PfxWz43

---

