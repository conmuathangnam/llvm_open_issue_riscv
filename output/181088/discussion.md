# Clang crash with non-type template parameter using pointer-to-member of class

**Author:** ccotter
**URL:** https://github.com/llvm/llvm-project/issues/181088

## Body

```c++
template <auto _Ptr>
class __intrusive_mpsc_queue;

template <class T>
struct AT {
    T t_;
};

template <class _Node, AT<_Node*> _Node::* _Next>
class __intrusive_mpsc_queue<_Next> {};

struct cc {
    AT<void*> next_{nullptr};
};

enum something {};

void foo() {
    __intrusive_mpsc_queue<&something::next_> command_queue_;
}
```

The above code is not valid and should not compile; it crashes clang-21 and clang trunk: https://godbolt.org/z/znMEPn6a8 .. clang-20 does not crash.

Note that `&something::next_` is passed, even though `something` is an enum, 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Chris Cotter (ccotter)

<details>
```c++
template &lt;auto _Ptr&gt;
class __intrusive_mpsc_queue;

template &lt;class T&gt;
struct AT {
    T t_;
};

template &lt;class _Node, AT&lt;_Node*&gt; _Node::* _Next&gt;
class __intrusive_mpsc_queue&lt;_Next&gt; {};

struct cc {
    AT&lt;void*&gt; next_{nullptr};
};

enum something {};

void foo() {
    __intrusive_mpsc_queue&lt;&amp;something::next_&gt; command_queue_;
}
```

The above code is not valid and should not compile; it crashes clang-21 and clang trunk: https://godbolt.org/z/znMEPn6a8 .. clang-20 does not crash.

Note that `&amp;something::next_` is passed, even though `something` is an enum, 
</details>


---

