# [clang] fails to compile print("{}", 5s) when using libstdc++

**Author:** Taw3e8
**URL:** https://github.com/llvm/llvm-project/issues/139067
**Status:** Closed
**Labels:** clang:frontend, regression, libstdc++, diverges-from:gcc, diverges-from:msvc, diverges-from:edg
**Closed Date:** 2025-05-14T08:25:03Z

## Body

This [fails to compile](https://godbolt.org/z/EWrMKdd3o) without adding libc++:

```cpp
#include <chrono>
#include <print>

int main() {
	using namespace std::chrono_literals;
	std::print("{}", 5s);
}
```

## Comments

### Comment 1 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/aYhd851hE)):
```C++
template <class R>
constexpr auto kind =
  primary_template_not_defined(
    kind<R> // How can this hack be valid!
  );

template <class T, decltype(sizeof(0)) N>
constexpr int kind<T[N]> = 0;
```

This was accepted by old versions of Clang, but not the trunk...

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jan Piotrowicz (Taw3e8)

<details>
This [fails to compile](https://godbolt.org/z/EWrMKdd3o) without adding libc++:

```cpp
#include &lt;chrono&gt;
#include &lt;print&gt;

int main() {
	using namespace std::chrono_literals;
	std::print("{}", 5s);
}
```
</details>


---

### Comment 3 - erichkeane

> Reduced example ([Godbolt link](https://godbolt.org/z/aYhd851hE)):
> ```c++
> template <class R>
> constexpr auto kind =
>   primary_template_not_defined(
>     kind<R> // How can this hack be valid!
>   );
> 
> template <class T, decltype(sizeof(0)) N>
> constexpr int kind<T[N]> = 0;
> ```
> This was accepted by old versions of Clang, but not the trunk...

Looks like this example fails in GCC as well if you instantiate `kind` for the same reason.  So assuming this is accurate, this is a case of Clang diagnosing a 'never valid' template immediately, which I believe is correct behavior.



---

### Comment 4 - frederick-vs-ja

> Looks like this example fails in GCC as well if you instantiate `kind` for the same reason. So assuming this is accurate, this is a case of Clang diagnosing a 'never valid' template immediately, which I believe is correct behavior.

Hmm, the primary template is assumed to be never valid, which is required for `std::format_kind` ([[format.range.fmtkind]/1](https://eel.is/c++draft/format.range.fmtkind#1)).

Although libstdc++'s technique looks too tricky for me...

---

### Comment 5 - frederick-vs-ja

This seems to be caused by #134522. CC @zwuis.

---

### Comment 6 - cor3ntin

@jwakely fyi

---

### Comment 7 - jwakely

> [@jwakely](https://github.com/jwakely) fyi

Yes, this is https://gcc.gnu.org/bugzilla/show_bug.cgi?id=120190

---

### Comment 8 - cor3ntin

Should we revert https://github.com/llvm/llvm-project/pull/134522 until libstdc++ is fixed? This seems rather disruptive at the moment @zwuis @AaronBallman @erichkeane 

---

### Comment 9 - zwuis

> Should we revert https://github.com/llvm/llvm-project/pull/134522 until libstdc++ is fixed? This seems rather disruptive at the moment @zwuis @AaronBallman @erichkeane 

I have no idea. It depends on you.

---

### Comment 10 - AaronBallman

> Should we revert [#134522](https://github.com/llvm/llvm-project/pull/134522) until libstdc++ is fixed? This seems rather disruptive at the moment [@zwuis](https://github.com/zwuis) [@AaronBallman](https://github.com/AaronBallman) [@erichkeane](https://github.com/erichkeane)

I think that's a sufficiently new change to be worth reverting until we can find a way to work around the issue (even if it's a compatibility hack that only works in system headers).

---

### Comment 11 - erichkeane

> > Should we revert [#134522](https://github.com/llvm/llvm-project/pull/134522) until libstdc++ is fixed? This seems rather disruptive at the moment [@zwuis](https://github.com/zwuis) [@AaronBallman](https://github.com/AaronBallman) [@erichkeane](https://github.com/erichkeane)
> 
> I think that's a sufficiently new change to be worth reverting until we can find a way to work around the issue (even if it's a compatibility hack that only works in system headers).

Its a little more than a month old and fixes some pretty annoying bugs.  I'd like for us to not revert this if possible.  Is @zwuis around?  I would really like it if we could just put some sort of compatibility hack in place instead.

---

### Comment 12 - frederick-vs-ja

> Should we revert [#134522](https://github.com/llvm/llvm-project/pull/134522) until libstdc++ is fixed? This seems rather disruptive at the moment [@zwuis](https://github.com/zwuis) [@AaronBallman](https://github.com/AaronBallman) [@erichkeane](https://github.com/erichkeane)

IIUC it's sufficient to just give up early diagnostics for primary templates at this moment. I want a partial revert like that in #139490.

---

