# [clang] Crash in `SemaConcept.cpp` / `Sema::getNormalizedAssociatedConstraints` (invalidated iterator?)

**Author:** jiixyj
**URL:** https://github.com/llvm/llvm-project/issues/165238
**Status:** Closed
**Labels:** clang:frontend, regression, concepts, crash
**Closed Date:** 2025-10-28T07:15:36Z

## Body

On 1322e71f2baac9d7cfa77cfa5345bfffbff74cf7 I'm getting a crash in `SemaConcept.cpp` when compiling some C++ code (this is a `RelWithDebInfo` build with assertions enabled):

```
 #7 0x00007ff333c3eefc abort ./stdlib/abort.c:81:3                                                                                                                                              
 #8 0x00007ff333c3ee6a __assert_perror_fail ./assert/assert-perr.c:31:1                                                                                                                         
 #9 0x000056107c5555ee llvm::DenseMapIterator<llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, clang::NormalizedConstraint*, llvm::DenseMapInfo<llvm::Poi
nterUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, void>, llvm::detail::DenseMapPair<llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement 
const*>, clang::NormalizedConstraint*>, false>::operator*() const /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1217:5                                              
#10 0x000056107c5555ee llvm::DenseMapIterator<llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, clang::NormalizedConstraint*, llvm::DenseMapInfo<llvm::Poi
nterUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, void>, llvm::detail::DenseMapPair<llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement 
const*>, clang::NormalizedConstraint*>, false>::operator->() const /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1220:54                                            
#11 0x000056107c5555ee clang::Sema::getNormalizedAssociatedConstraints(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedC
onstraint>) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaConcept.cpp:2419:10                                                                                                     
#12 0x000056107c551812 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::Sour
ceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaConcept.cpp:1155:8                          
#13 0x000056107c551582 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstrai
nt>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) /freebsd/data/linux/git-net/llvm-project/c
lang/lib/Sema/SemaConcept.cpp:1227:7                                                                              
```

This comes from the line `return CacheEntry->second;`. It looks like this iterator gets invalidated, maybe by the earlier call `SubstituteParameterMappings(*this).substitute(*Normalized)`.

This works around the issue:

```patch
diff --git a/clang/lib/Sema/SemaConcept.cpp b/clang/lib/Sema/SemaConcept.cpp
index f04cc454cdb7..8135e1bc239f 100644
--- a/clang/lib/Sema/SemaConcept.cpp
+++ b/clang/lib/Sema/SemaConcept.cpp
@@ -2408,12 +2408,16 @@ const NormalizedConstraint *Sema::getNormalizedAssociatedConstraints(
   if (CacheEntry == NormalizationCache.end()) {
     auto *Normalized = NormalizedConstraint::fromAssociatedConstraints(
         *this, ND, AssociatedConstraints);
-    CacheEntry =
-        NormalizationCache.try_emplace(ConstrainedDeclOrNestedReq, Normalized)
-            .first;
+    auto [it, inserted] = NormalizationCache.try_emplace(ConstrainedDeclOrNestedReq, Normalized);
+    assert(inserted);
+    assert(it != NormalizationCache.end());
+
     if (!Normalized ||
         SubstituteParameterMappings(*this).substitute(*Normalized))
       return nullptr;
+
+    CacheEntry = NormalizationCache.find(ConstrainedDeclOrNestedReq);
+    assert(CacheEntry != NormalizationCache.end());
   }
   return CacheEntry->second;
 }
```

Would a minimal reproducer help? It's a bit difficult, because I'm using C++20 modules...

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jan Kokemüller (jiixyj)

<details>
On 1322e71f2baac9d7cfa77cfa5345bfffbff74cf7 I'm getting a crash in `SemaConcept.cpp` when compiling some C++ code (this is a `RelWithDebInfo` build with assertions enabled):

```
 #<!-- -->7 0x00007ff333c3eefc abort ./stdlib/abort.c:81:3                                                                                                                                              
 #<!-- -->8 0x00007ff333c3ee6a __assert_perror_fail ./assert/assert-perr.c:31:1                                                                                                                         
 #<!-- -->9 0x000056107c5555ee llvm::DenseMapIterator&lt;llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, clang::NormalizedConstraint*, llvm::DenseMapInfo&lt;llvm::Poi
nterUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, void&gt;, llvm::detail::DenseMapPair&lt;llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement 
const*&gt;, clang::NormalizedConstraint*&gt;, false&gt;::operator*() const /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1217:5                                              
#<!-- -->10 0x000056107c5555ee llvm::DenseMapIterator&lt;llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, clang::NormalizedConstraint*, llvm::DenseMapInfo&lt;llvm::Poi
nterUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, void&gt;, llvm::detail::DenseMapPair&lt;llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement 
const*&gt;, clang::NormalizedConstraint*&gt;, false&gt;::operator-&gt;() const /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1220:54                                            
#<!-- -->11 0x000056107c5555ee clang::Sema::getNormalizedAssociatedConstraints(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedC
onstraint&gt;) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaConcept.cpp:2419:10                                                                                                     
#<!-- -->12 0x000056107c551812 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::Sour
ceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaConcept.cpp:1155:8                          
#<!-- -->13 0x000056107c551582 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstrai
nt&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) /freebsd/data/linux/git-net/llvm-project/c
lang/lib/Sema/SemaConcept.cpp:1227:7                                                                              
```

This comes from the line `return CacheEntry-&gt;second;`. It looks like this iterator gets invalidated, maybe by the earlier call `SubstituteParameterMappings(*this).substitute(*Normalized)`.

This works around the issue:

```patch
diff --git a/clang/lib/Sema/SemaConcept.cpp b/clang/lib/Sema/SemaConcept.cpp
index f04cc454cdb7..8135e1bc239f 100644
--- a/clang/lib/Sema/SemaConcept.cpp
+++ b/clang/lib/Sema/SemaConcept.cpp
@@ -2408,12 +2408,16 @@ const NormalizedConstraint *Sema::getNormalizedAssociatedConstraints(
   if (CacheEntry == NormalizationCache.end()) {
     auto *Normalized = NormalizedConstraint::fromAssociatedConstraints(
         *this, ND, AssociatedConstraints);
-    CacheEntry =
-        NormalizationCache.try_emplace(ConstrainedDeclOrNestedReq, Normalized)
-            .first;
+    auto [it, inserted] = NormalizationCache.try_emplace(ConstrainedDeclOrNestedReq, Normalized);
+    assert(inserted);
+    assert(it != NormalizationCache.end());
+
     if (!Normalized ||
         SubstituteParameterMappings(*this).substitute(*Normalized))
       return nullptr;
+
+    CacheEntry = NormalizationCache.find(ConstrainedDeclOrNestedReq);
+    assert(CacheEntry != NormalizationCache.end());
   }
   return CacheEntry-&gt;second;
 }
```

Would a minimal reproducer help? It's a bit difficult, because I'm using C++20 modules...
</details>


---

### Comment 2 - EugeneZelenko

Sure, reproducer will be helpful. Preferably on https://godbolt.org.

---

### Comment 3 - shafik

A reproducer is preferred to understand the full extent of the issue and what the right fix is. It is also important so that we can write regression tests to ensure that we don't introduce the bug or something similar in the future.

CC @ChuanqiXu9 @mpark 

---

### Comment 4 - jiixyj

OK, I'll try to create a reproducer with cvise! I forgot to mention that I could bisect the problem to e9972debc98ce5d00db47409248bbcf06fafaf73 .

---

### Comment 5 - jiixyj

Here is a reproducer without C++20 modules:

```c++
#include <format>

namespace bug_165238 {

struct call_t {
    static void operator()(const char*);
    static void operator()(std::formattable<char> auto);
};

inline constexpr bug_165238::call_t call{};

}  // namespace bug_165238

int main() { bug_165238::call("hello"); }
```

https://godbolt.org/z/e7f7P4bYo


I'll try to whittle it down further.

---

### Comment 6 - shafik

CC @cor3ntin 

---

### Comment 7 - jiixyj

cvise reduced it to this:

```c++
namespace std {
template <typename, typename _Tp>
concept output_iterator = requires(_Tp __t) { __t; };
template <typename _Out> struct basic_format_context {
  static_assert(output_iterator<_Out, int>);
  using char_type = _Out;
};
template <typename> class basic_format_parse_context;
template <typename, typename _Context, typename _Formatter,
          typename = basic_format_parse_context<typename _Context::char_type>>
concept __parsable_with = requires(_Formatter __f) { __f; };
template <typename _Tp, typename _CharT,
          typename _Context = basic_format_context<_CharT>>
concept __formattable_impl = __parsable_with<_Tp, _Context, _Context>;
template <typename _Tp, typename _CharT>
concept formattable = __formattable_impl<_Tp, _CharT>;
} // namespace std
struct {
  void operator()(std::formattable<char> auto);
} call;
int main() { call(""); }
```

https://godbolt.org/z/ajoz5T9Mb

---

### Comment 8 - jiixyj

I can confirm my earlier suspicion that `SubstituteParameterMappings(*this).substitute(*Normalized))` calls `Sema::getNormalizedAssociatedConstraints` recursively, by adding some logging:

```
entering Sema::getNormalizedAssociatedConstraints                                                                                                                                               
before if (!Normalized || SubstituteParameterMappings(*this).substitute(*Normalized))                                                                                                           
entering Sema::getNormalizedAssociatedConstraints                                                                                                                                               
before if (!Normalized || SubstituteParameterMappings(*this).substitute(*Normalized))                                                                                                           
after if (!Normalized || SubstituteParameterMappings(*this).substitute(*Normalized))                                                                                                            
after if (!Normalized || SubstituteParameterMappings(*this).substitute(*Normalized))                                                                                                            

clang-22: /freebsd/data/linux/git-net/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1216: reference llvm::DenseMapIterator<llvm::PointerUnion<const clang::NamedDecl *, const clang::concepts::N
estedRequirement *>, clang::NormalizedConstraint *>::operator*() const [KeyT = llvm::PointerUnion<const clang::NamedDecl *, const clang::concepts::NestedRequirement *>, ValueT = clang::Normali
zedConstraint *, KeyInfoT = llvm::DenseMapInfo<llvm::PointerUnion<const clang::NamedDecl *, const clang::concepts::NestedRequirement *>>, Bucket = llvm::detail::DenseMapPair<llvm::PointerUnion
<const clang::NamedDecl *, const clang::concepts::NestedRequirement *>, clang::NormalizedConstraint *>, IsConst = false]: Assertion `isHandleInSync() && "invalid iterator access!"' failed.    
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.                                     
```

---

