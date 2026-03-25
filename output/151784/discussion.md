# Template argument limit of 0x7fff in clang-21 onward

**Author:** jameswplayer
**URL:** https://github.com/llvm/llvm-project/issues/151784

## Body

I noticed that clang TOT fails to compile variadic templates with argument count exceeding 0x7fff.  I plugged a simple reproducer into [godbolt](https://godbolt.org/z/nq4q4djz7) and noticed that this behavior starts at clang-21 (i.e. clang-20 compiles successfully).

The simplest repro I came up with was to simply instantiate a `std::index_sequence` at various sizes, where the first `static_assert()` succeeds and the second one fails:
```
namespace std {
typedef int a;
template <typename b, b... c> struct d {
  constexpr a size() { return sizeof...(c); }
};
template <typename b, b e> using f = __make_integer_seq<d, b, e>;
template <a e> using make_index_sequence = f<a, e>;
} // namespace std
int main() {
  constexpr unsigned SeqLength = 0x7fff;
  std::make_index_sequence<SeqLength> Seq;
  static_assert(Seq.size() == SeqLength);
  {
    constexpr unsigned SeqLength = 0x8000;
    std::make_index_sequence<SeqLength> Seq;
    static_assert(Seq.size() == SeqLength);
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: James Player (jameswplayer)

<details>
I noticed that clang TOT fails to compile variadic templates with argument count exceeding 0x7fff.  I plugged a simple reproducer into [godbolt](https://godbolt.org/z/nq4q4djz7) and noticed that this behavior starts at clang-21 (i.e. clang-20 compiles successfully).

The simplest repro I came up with was to simply instantiate a `std::index_sequence` at various sizes, where the first `static_assert()` succeeds and the second one fails:
```
namespace std {
typedef int a;
template &lt;typename b, b... c&gt; struct d {
  constexpr a size() { return sizeof...(c); }
};
template &lt;typename b, b e&gt; using f = __make_integer_seq&lt;d, b, e&gt;;
template &lt;a e&gt; using make_index_sequence = f&lt;a, e&gt;;
} // namespace std
int main() {
  constexpr unsigned SeqLength = 0x7fff;
  std::make_index_sequence&lt;SeqLength&gt; Seq;
  static_assert(Seq.size() == SeqLength);
  {
    constexpr unsigned SeqLength = 0x8000;
    std::make_index_sequence&lt;SeqLength&gt; Seq;
    static_assert(Seq.size() == SeqLength);
  }
}
```
</details>


---

### Comment 2 - frederick-vs-ja

I believe at least we should reject the `N` beyond the limit, rather than silently miscompile the template specialization.

---

### Comment 3 - efriedma-quic

I think the truncation to 15 bits instead of 16 bits is f302f35526553abcb46dab278c4494c3d01deb45 .  CC @mizvekov .

Limiting the maximum number of template arguments is probably fine as long as there's some explicit limit.

---

### Comment 4 - mizvekov

Yeah, a lot of our impl limits are basically however many free bits we have somewhere. If we need another bit for something else, it's simplest to decrease that limit, as long as it still looks reasonably large.

One solution here is to simply tail allocate the extra bits for the number of template args when needed, if we want to support that many arguments.

---

### Comment 5 - shafik

Like the previous comment, it feels like it would be more user-friendly to generate a diagnostic if we pass the implementation limit here. 

---

### Comment 6 - mizvekov

I am not sure a diagnostic would be worth here, it would maybe be easier to implement support for a really huge amount of template arguments.

And we have these sorts of issues with almost all implementation limits by the way, it is just that most of them are a really big number which people are unlikely to hit. It would be a huge effort to make all of these hit a diagnostic.

---

### Comment 7 - efriedma-quic

We have checks for most limits, as far as I know.  For example, we have diag::err_function_parameter_limit_exceeded.  It's not that hard to implement.

It's probably better to have a limit smaller than 2^32 so people don't accidentally OOM writing `__make_integer_seq<d, unsigned, -1u>` or something.

---

### Comment 8 - mizvekov

err_function_parameter_limit_exceeded is quite a recent exception, but if you look at the same file where this limit is implemented in `include/clang/AST/Type.h`, and scroll through the various TypeBitfields, you can spot many bitfields storing an open ended amount.

Some examples:
ObjCObjectTypeBitfields {NumTypeArgs, NumProtocols}
VectorTypeBitfields {NumElements}
AutoTypeBitfields {NumArgs}
TemplateTypeParmTypeBitfields {Depth, Index}
SubstTemplateTypeParmTypeBitfields {Index, PackIndex}
SubstTemplateTypeParmPackTypeBitfields {Index, NumArgs}
TemplateSpecializationTypeBitfields {NumArgs}
DependentTemplateSpecializationTypeBitfields {NumArgs}
PackExpansionTypeBitfields {NumExpansions}

Some examples from `DeclBase.h` bitfields:
EnumDeclBitfields {NumPositiveBits, NumNegativeBits}
CXXConstructorDeclBitfields {NumCtorInitializers}

Pruning that least of repeats, that's one diagnostic for each of:

* NumTypeArgs and NumProtocols for ObjCObjectType
* NumElements for VectorType
* Number of template arguments for various nodes, including Indexes and PackIndexes.
* Template depth
* EnumDecl amount of positive and negative enumerators.
* Number of constructor initializers.

> It's probably better to have a limit smaller than 2^32 so people don't accidentally OOM writing __make_integer_seq<d, unsigned, -1u> or something.

Sure, that limit is somewhat easy to hit accidentally, but it would be frustrating to go through the trouble of increasing the limit beyond what fits in the bitfield, but then pick an arbitrary hardcoded limit.
We could make these limits configurable with a flag instead, for people who really need and can use those large numbers.

---

### Comment 9 - shafik

So IIUC since we have so many we can do, and doing them all would be a lot of work then we should do none of them. 

I believe there is value in doing these incrementally, especially since we have a bug report in which someone really did run into this issue and were completely stumped. Which is not a great user experience.

Ideally, doing one shows how to do the rest and maybe they are good first issues for folks to tackle. We see that good first issues tend to get snapped up, at least for the frontend stuff.

---

### Comment 10 - mizvekov

I am not saying to not fix the issue, that is not the argument, just that increasing the limit beyond what we can reasonably expect users to hit is also a viable option.

---

### Comment 11 - ZXShady

Hi I myself hit this issue using in my library which uses large int sequences

---

