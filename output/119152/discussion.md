# [[clang::musttail]] unnecessarily errors when tail-calling non-member function from member function

**Author:** michael-kenzel
**URL:** https://github.com/llvm/llvm-project/issues/119152

## Body

The following example fails to compile:
```cpp
struct A
{
    void fun(int x);
};

void blub(A*, int);

void A::fun(int x)
{
    [[clang::musttail]] return blub(this, x);  // error: non-static member function cannot perform a tail call to non-member function 'blub'
}
```
This is, however, unnecessary. On many targets, a tail call would be perfectly possible here. In fact, clang will happily generate a tail call if the attribute is removed: https://godbolt.org/z/xKbcf71EM. gcc accepts this example: https://godbolt.org/z/d3f6b1b5c.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Michael Kenzel (michael-kenzel)

<details>
The following example fails to compile:
```cpp
struct A
{
    void fun(int x);
};

void blub(A*, int);

void A::fun(int x)
{
    [[clang::musttail]] return blub(this, x);  // error: non-static member function cannot perform a tail call to non-member function 'blub'
}
```
This is, however, unnecessary. On many targets, a tail call would be perfectly possible here. In fact, clang will happily generate a tail call if the attribute is removed: https://godbolt.org/z/xKbcf71EM. gcc accepts this example: https://godbolt.org/z/d3f6b1b5c.

</details>


---

### Comment 2 - pinskia

Note the way GCC handles musttail is NOT in the front-ends but rather in the middle-end so it will accept more cases that clang will reject.

For an example even this:
```
void f(int a, int b);
void g(int a, int b, int c)
{
  [[clang::musttail]] return f(a, b);
}
```

---

### Comment 3 - shafik

Adding to the previous comment based on the commit: 8344675908424ee532d4ae30e5043c5a5834e02c it seems like we need to be more conservative b/c we can't know on the frontend.

CC @erichkeane 

---

### Comment 4 - erichkeane

@haberman : should probably help us here as the implementer :) 

I don't understand the necessity of the diagnostic there, why we think a non-member function can't be tail-called into a member function.

I'm hoping this is just a case of us being overly conservative and a rule we can loosen in some situations.  Perhaps the review https://reviews.llvm.org/D99517 mentions it in discussion?  Its a little long for me to review right now, but is probably worth looking into to figure out the reasoning for this limit.

---

