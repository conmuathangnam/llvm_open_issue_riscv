# [libc++]  ABI break: std::deque changes its size in LLVM 21

**Author:** zibi2
**URL:** https://github.com/llvm/llvm-project/issues/154146
**Status:** Closed
**Labels:** libc++, ABI, regression:20, regression:21
**Closed Date:** 2025-08-23T05:58:39Z

## Body

The following code shows how the size of std::deque has changed in LLVM version 21.

// t.h
```
#include <deque>
#include <memory>
#include <cassert>


struct malloc_allocator_base {
  static std::size_t outstanding_bytes;
  static std::size_t alloc_count;
  static std::size_t dealloc_count;
  static bool disable_default_constructor;

  static std::size_t outstanding_alloc() {
    assert(alloc_count >= dealloc_count);
    return (alloc_count - dealloc_count);
  }

  static void reset() {
    assert(outstanding_alloc() == 0);
    disable_default_constructor = false;
    outstanding_bytes           = 0;
    alloc_count                 = 0;
    dealloc_count               = 0;
  }
};

size_t malloc_allocator_base::outstanding_bytes         = 0;
size_t malloc_allocator_base::alloc_count               = 0;
size_t malloc_allocator_base::dealloc_count             = 0;
bool malloc_allocator_base::disable_default_constructor = false;

template <class T>
class malloc_allocator : public malloc_allocator_base {
public:
  typedef T value_type;

  malloc_allocator() noexcept { assert(!disable_default_constructor); }

  template <class U>
  malloc_allocator(malloc_allocator<U>) noexcept {}

  T* allocate(std::size_t n) {
    const std::size_t nbytes = n * sizeof(T);
    ++alloc_count;
    outstanding_bytes += nbytes;
    return static_cast<T*>(std::malloc(nbytes));
  }

  void deallocate(T* p, std::size_t n) {
    const std::size_t nbytes = n * sizeof(T);
    ++dealloc_count;
    outstanding_bytes -= nbytes;
    std::free(static_cast<void*>(p));
  }

  friend bool operator==(malloc_allocator, malloc_allocator) { return true; }
  friend bool operator!=(malloc_allocator x, malloc_allocator y) { return !(x == y); }
};


int sizeOfCreatedDeque();
std::deque<int, malloc_allocator<int> > dq;
```
 
t.C
```
#include "t.h"

int sizeOfCreatedDeque() {
  dq.push_front(1);
  dq.push_front(2);
  dq.push_front(3);
  return sizeof(dq);
}
```

// t2.C
```
#include "t.h"
#include <iostream>

int main() {
using namespace std;
  int previousSize = sizeOfCreatedDeque();
  int currentSize = sizeof(dq);
  if (currentSize != previousSize)
    cerr << "ABI break std::deque`s size changed from " << previousSize << " to " << currentSize << endl;
  return 0;
}
```

Compile `t2.C` with `LLVM 21/20` clang and `t.C` with previous version, for example LLVM 19 . Then, run executable:
I'm getting the following:
**ABI break std::deque`s size changed from 48 to 56**
I compiled `t.C` with clang version 18.1.8 but any version prior to PR 76756 would do as well.

My investigation point to https://github.com/llvm/llvm-project/pull/76756 as culprit. 

## Comments

### Comment 1 - frederick-vs-ja

The old `__compressed_pair` achieved EBO more thoroughly, while the new approach always puts the second member after the first one, which sometimes fails to reuse padding bytes of empty classes due to ABI restriction. CC @philnik777.

---

### Comment 2 - perry-ca

Sounds like this is something that needs to be fixed in LLVM 21.  Agree?

---

### Comment 3 - ldionne

Yes, we need to fix this. We will also need to determine the extent of the breakage (we did similar changes in many many types) and get it addressed everywhere.

I will sync up with @philnik777 tomorrow.

---

### Comment 4 - perry-ca

Thanks Louis.  Zibi did testing with the lit tests and the dump record layout to compare layouts between llvm 18 and llvm 21.  We can share the results if you like.

---

### Comment 5 - ldionne

> Thanks Louis. Zibi did testing with the lit tests and the dump record layout to compare layouts between llvm 18 and llvm 21. We can share the results if you like.

Yes, I think anything you have to share would be helpful.

Edit: @perry-ca Actually, I think we managed to get a decent understanding of the issue, no need to post additional data unless you have other reproducers that we might not be aware of. I'd be curious to know how you folks came across this ABI break though?

---

### Comment 6 - ldionne

Alright folks, I just met with @philnik777 and we went over this issue. This is the reproducer we came up with based on the OP: https://godbolt.org/z/o17Tb8f36

It appears that an empty member of type `T` marked with `[[no_unique_address]]` might end up at the end of the enclosing class (say `Enclosing`) if there is already a member of type `T` in `Enclosing` or in a subobject of `Enclosing`. We understand that the compiler needs to select a different address from the first `T` member, however we don't understand why the compiler decides to move the empty object to the end of the class, which ends up increasing the size of the overall type.

Sadly, this has a pretty large blast radius. It means that even code like this changes ABI whenever `malloc_allocator<int>` is empty:

```
struct UserContainer {
    std::vector<int, malloc_allocator<int> > vec_;
    [[no_unique_address]] malloc_allocator<int> alloc_;
};

// sizeof(UserContainer) changed between LLVM 19 and LLVM 20
```

This also applies to several other types where we made the `_LIBCPP_COMPRESSED_PAIR` change, including `std::basic_string`.

`std::deque` is a bit special (but perhaps not unique) in that `std::deque` holds a member of type `__split_buffer`, and that `__split_buffer` itself holds the allocator as a member. That is why the issue reproduces by checking `sizeof(deque)` directly in the OP, as opposed to witnessing the ABI break through another user-defined type.

We believe we have a fix for this issue but it will require a bit more work. The workaround is basically to use an anonymous struct around the members of the `_LIBCPP_COMPRESSED_PAIR`: https://godbolt.org/z/5K7ra7dE4. That allows restoring the old (pre-`_LIBCPP_COMPRESSED_PAIR`) ABI in the cases that we looked at.

Sadly, the timing for finding out about this issue is not ideal. The issue was introduced in LLVM 20, which is now considered "done" as we are working towards the LLVM 21 release (planned on August 26th according to https://llvm.org).

We would be able to fix this in LLVM 21, however I will also sync up with the release managers to discuss whether fixing LLVM 20 might still be doable. I suspect that many vendors for whom ABI stability is important (and are sometimes lagging a bit behind) might not have been impacted yet by the issue.

Summary
----------
@tru @tstellar

We shipped an ABI break in LLVM 20. We believe we can fix it in time for LLVM 21.1.0 without further impacting the release process, but we would like to discuss the course of action for LLVM 20.

@philnik777 is working on getting us test coverage for this regression and preparing a fix. I will handle vendor communication once we've agreed on a decisive course of action.

---

### Comment 7 - h-vetinari

> We shipped an ABI break in LLVM 20. We believe we can fix it in time for LLVM 21.1.0 without further impacting the release process, but we would like to discuss the course of action for LLVM 20.

I discovered this issue by accident. I think this topic would deserve more visibility, i.e. a thread on discourse

---

### Comment 8 - ldionne

> 
> I discovered this issue by accident. I think this topic would deserve more visibility, i.e. a thread on discourse

Yes, I will give it proper visibility but I wanted to wait to have a plan agreed upon with the release managers before I sound the alarm, to avoid noise and speculation.

---

### Comment 9 - h-vetinari

That makes sense. On the other hand, the degree of impact this has (e.g. based on people's responses) would presumably play a role in the decision whether to backport, especially whether this is critical enough to push another release for v20.

---

### Comment 10 - tru

I have no issue with fixing it in 21.1.0 if we can land it this week (or absolutely latest Monday)

As for 20.1.x I am unsure. Maybe we could do a 20.2.x with the fixed Abi. I think we have done something like that before - ping @tstellar 

---

### Comment 11 - h-vetinari

> I think we have done something like that before - ping [@tstellar](https://github.com/tstellar)

Last time that I can remember was 11.0 -> 11.1. Not sure how severe the ABI-break here is in practice. What I can say is that we haven't observed any breakage due to this in conda-forge (or at least I haven't heard of it, across a couple 100k [downloads](https://anaconda.org/conda-forge/libcxx/files)), despite crucially depending on ABI stability: on macOS we compile with clang 18/19 and matching libcxx headers, but the actual runtime library is always newest libcxx, i.e. we've been using v20.1 since https://github.com/conda-forge/libcxx-feedstock/pull/216 or about 5 months.

Not saying it's not serious of course, just that it's perhaps hard to hit (as another example I can remember, #60270 didn't cause a new release at the time).

---

### Comment 12 - ldionne

Brain dumping some notes from this morning's session about `[[no_unique_address]]` and its quirks, for posterity. Basically, what seems to be happening is that the Itanium C++ ABI layout algorithm treats `[[no_unique_address]]` the same way it treated the empty base optimization. That is, it moves the empty object at the beginning of the enclosing struct. In particular, it does *not* move the empty object to the previous non-empty member. Those are equivalent in the case of a 2-element struct (like `__compressed_pair` and most EBO optimizations), but not equivalent in the case of a multi-element struct.

So the issue we introduced by moving to `_LIBCPP_COMPRESSED_PAIR` is that we allowed the empty member to move to the beginning of the enclosing struct, meaning that any member that followed the enclosing struct and expected to be compressed out couldn't be compressed anymore due to C++ aliasing rules.

Using a `struct { ... }` around the compressed pair members creates a boundary beyond which the compiler can't move the empty object, which achieves the same effect as our old `__compressed_pair` struct.

There is also a lesson to learn for implementing containers going forward. If we want to optimize an empty member and we know we have a non-empty member before it, we don't want to mark the empty with `[[no_unique_address]]` and call it a day. That would allow the empty member to move to the very beginning of the struct and potentially prevent optimizations in a user-defined type that encloses that container. Consider:

```c++
template <class T, class Allocator>
struct SizeBasedVectorBad {
    T* begin_;
    std::size_t size_;
    std::size_t capacity_;
    [[no_unique_address]] Allocator alloc_;
};

struct User {
    SizeBasedVectorBad<int, allocator<int> > myvec_;
    char const* name_;
    // this won't be compressed because it tries to move to the beginning
    // of the whole struct, where there's already an object of the same type
    [[no_unique_address]] allocator<int> myalloc_;
};
```

Instead, we want to force the allocator to share the address of the previous non-empty member in our container, like so:

```c++
template <class T, class Allocator>
struct SizeBasedVectorGood {
    T* begin_;
    std::size_t size_;
    struct { // boundary for "EBO application"
        std::size_t capacity_;
        [[no_unique_address]] Allocator alloc_;
    };
};

struct User {
    SizeBasedVectorGood<int, allocator<int> > myvec_;
    char const* name_;
    // now this can move to the beginning of the struct because there's
    // no other object of the same type at that address
    [[no_unique_address]] allocator<int> myalloc_;
};
```

Godbolt: https://godbolt.org/z/MEEv48T9a

IMO, `[[no_unique_address]]` is mis-designed in that regard, because it means that we're probably missing some compression opportunities. Realistically, people won't be using anonymous structs to control the EBO boundary and they will end up with non-optimal data layouts without realizing it. But in libc++ at least, we can prevent our own types from contributing to these inefficiencies by jumping through a few hoops.

---

### Comment 13 - pinskia

I am not sure why you can't use a named struct here rather an unnamed struct field extension here? Is it because you need to know the arguments to `Allocator`'s constructor? Can't you just use a template constructor to do that?
So basically why not just do:
```
    struct alloct { // boundary for "EBO application"
        std::size_t capacity_;
       template<typename T...> alloct(T... &&args) : alloc_(std::forward(args)...) {} 
       [[no_unique_address]] Allocator alloc_;
    };
  alloct alloc_;
```

And then replace capacity_ usage with m_a_.capacity_ and alloc_ with alloc_.alloc_ ?

---

### Comment 14 - ldionne

> I am not sure why you can't use a named struct here rather an unnamed struct field extension here? Is it because you need to know the arguments to `Allocator`'s constructor? Can't you just use a template constructor to do that? So basically why not just do:
> 
> ```
>     struct alloct { // boundary for "EBO application"
>         std::size_t capacity_;
>        template<typename T...> alloct(T... &&args) : alloc_(std::forward(args)...) {} 
>        [[no_unique_address]] Allocator alloc_;
>     };
>   alloct alloc_;
> ```
> 
> And then replace capacity_ usage with m_a_.capacity_ and alloc_ with alloc_.alloc_ ?

We are generating these structs from a macro, so we can't easily select a name. We could pass the name into the macro, but then we have to change all of the code to use the outer member name to access the inner fields, and that also means that we can't *not* introduce an EBO boundary like we currently do when `_LIBCPP_ABI_NO_COMPRESSED_PAIR_PADDING` is defined. That may be a reasonable tradeoff, but TBH I think it would be a lot better if anonymous structs just worked consistently in Clang and GCC.

---

