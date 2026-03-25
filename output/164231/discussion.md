# Missing deduction guide for aggregate causes crash in some contexts

**Author:** mwk6mh5fd8-blip
**URL:** https://github.com/llvm/llvm-project/issues/164231

## Body

The context needed to cause the crash is quite unstable, so I was not able to reduce the code as I wanted. The namespaces were necessary for example, but one the code was reduced, it could work. Even if you were not able to reproduce the crash, the key point is nonetheless the absence of deduction guide for the class _`integral_iterator`_, as the log crash suggested (thanks for the relevant names). I hope the log will suffice to you.

```
0   libsystem_kernel.dylib     0x7ff813182846 __pthread_kill + 10
1   libsystem_pthread.dylib   0x7ff8131bdb12 pthread_kill + 259
2   libsystem_c.dylib              0x7ff8130a10fa raise + 24
3   clang                         	       0x104d40c32 SignalHandler(int, __siginfo*, void*) + 130
4   libsystem_platform.dylib 0x7ff8131f631d _sigtramp + 29
5   clang                         	       0x103ec0001 clang::Stmt::getSourceRange() const + 1969
6   clang                         	       0x1038e439f clang::Sema::**DeclareAggregateDeductionGuideFromInitList**(clang::TemplateDecl*, llvm::MutableArrayRef<clang::QualType>, clang::SourceLocation) + 431
7   clang                         	       0x1036d5abc clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind 
```

Notice that the fifth line (the one over **`DeclareAggregateDeductionGuideFromInitList`** ) may change, with even a strange mention of Microsoft!

```
5   clang                         	       0x112060001 (anonymous namespace)::MicrosoftRecordLayoutBuilder::getAdjustedElementInfo(clang::ASTRecordLayout const&) + 97

5   clang                         	       0x106870001 (anonymous namespace)::EmptySubobjectMap::CanPlaceBaseAtOffset((anonymous namespace)::BaseSubobjectInfo const*, clang::CharUnits) + 81

6   clang                         	       0x11257e81b (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::ConvertConstructorToDeductionGuideTransform(clang::Sema&, clang::ClassTemplateDecl*) + 91

5   clang                         	       0x10cb40001 clang::analyze_scanf::ScanfSpecifier::fixType(clang::QualType, clang::QualType, clang::LangOptions const&, clang::ASTContext&) + 753

5   ???                           	    0x7ff7b22fe468 ???

5   clang                         	       0x10a300001 clang::ASTContext::setNonKeyFunction(clang::CXXMethodDecl const*) + 145

5   clang                         	       0x1084e0001 void std::__1::__insertion_sort_move[abi:nn200100]<std::__1::_ClassicAlgPolicy, DumpRecordLayout(llvm::raw_ostream&, clang::RecordDecl const*, clang::ASTContext const&, clang::CharUnits, unsigned int, char const*, bool, bool)::$_0&, clang::CXXRecordDecl const**>(clang::CXXRecordDecl const**, clang::CXXRecordDecl const**, std::__1::iterator_traits<clang::CXXRecordDecl const**>::value_type*, DumpRecordLayout(llvm::raw_ostream&, clang::RecordDecl const*, clang::ASTContext const&, clang::CharUnits, unsigned int, char const*, bool, bool)::$_0&) + 753

5   clang                         	       0x109800001 RequiresVtordisp(llvm::SmallPtrSetImpl<clang::CXXRecordDecl const*> const&, clang::CXXRecordDecl const*) + 1

...
```


Here is the code.

My guess:
In the following code the compiler is led to check for the concept `input_range` on the class _`integral_range`_, thus checking for begin and end returning an _`integral_iterator`_ that could be constructed with a missing deduction guide. If I add the guide, the issue disappears. The instabilities to get a crash and the logs seem to indicate a memory corruption.

```c++
template <class I> struct integral_iterator
{
    I index;
    integral_iterator  operator++(int);
    integral_iterator& operator++();
    bool operator==(const integral_iterator&) const;
};
template <class I> struct integral_range
{
    I _min;
    I _max;
};
#if 1 // crash
// here are a use case for DeclareAggregateDeductionGuideFromInitList
template <class I> auto begin (const integral_range<I>& r) { return integral_iterator{r._min};}
template <class I> auto   end (const integral_range<I>& r) { return integral_iterator{r._max+1};}
#else
template <class I> integral_iterator<I> begin (const integral_range<I>& r);
template <class I> integral_iterator<I>   end (const integral_range<I>& r);
#endif

template <class T> struct A
{
    template <std::ranges::input_range AA> A(const AA&);
};

template <class T> using B = integral_range<T>;

template <class T> void f(A<T>); // overloaded function

template <class T> void f (const B<T>&);

template <class T> void foo(const B<T>& r) { return f(r);}
```

In fact, the code was in a more complex context of namespaces and structures, like this one:

```c++
namespace ZZ
{
    template <class T> struct Case0
    {
        static void f(A<T>);

        static void f (const B<T>&);
    };
    struct Case1 // ok
    {
        template <class T> static void f(A<T>);

        template <class T> static void f (const B<T>&);
    };
    template <class I> void f(const B<I>& r)
    {
//        return Case1::f(r);//ok (context!)
        return Case0<I>::f(r);
    }
//    template void f (const B<int>&); //ok with this. (context!)
}
namespace AA
{
    template <class T> void foo(const B<T>& r) { return ZZ::f(r);}

    template void f (const B<int>&);
}
```

## Comments

### Comment 1 - frederick-vs-ja

Could you reproduce this on [Compiler Explorer](https://godbolt.org/)?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mwk6mh5fd8-blip)

<details>
The context needed to cause the crash is quite unstable, so I was not able to reduce the code as I wanted. The namespaces were necessary for example, but one the code was reduced, it could work. Even if you were not able to reproduce the crash, the key point is nonetheless the absence of deduction guide for the class _`integral_iterator`_, as the log crash suggested (thanks for the relevant names). I hope the log will suffice to you.

```
0   libsystem_kernel.dylib     0x7ff813182846 __pthread_kill + 10
1   libsystem_pthread.dylib   0x7ff8131bdb12 pthread_kill + 259
2   libsystem_c.dylib              0x7ff8130a10fa raise + 24
3   clang                         	       0x104d40c32 SignalHandler(int, __siginfo*, void*) + 130
4   libsystem_platform.dylib 0x7ff8131f631d _sigtramp + 29
5   clang                         	       0x103ec0001 clang::Stmt::getSourceRange() const + 1969
6   clang                         	       0x1038e439f clang::Sema::**DeclareAggregateDeductionGuideFromInitList**(clang::TemplateDecl*, llvm::MutableArrayRef&lt;clang::QualType&gt;, clang::SourceLocation) + 431
7   clang                         	       0x1036d5abc clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind 
```

Notice that the fifth line (the one over **`DeclareAggregateDeductionGuideFromInitList`** ) may change, with even a strange mention of Microsoft!

```
5   clang                         	       0x112060001 (anonymous namespace)::MicrosoftRecordLayoutBuilder::getAdjustedElementInfo(clang::ASTRecordLayout const&amp;) + 97

5   clang                         	       0x106870001 (anonymous namespace)::EmptySubobjectMap::CanPlaceBaseAtOffset((anonymous namespace)::BaseSubobjectInfo const*, clang::CharUnits) + 81

6   clang                         	       0x11257e81b (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::ConvertConstructorToDeductionGuideTransform(clang::Sema&amp;, clang::ClassTemplateDecl*) + 91

5   clang                         	       0x10cb40001 clang::analyze_scanf::ScanfSpecifier::fixType(clang::QualType, clang::QualType, clang::LangOptions const&amp;, clang::ASTContext&amp;) + 753

5   ???                           	    0x7ff7b22fe468 ???

5   clang                         	       0x10a300001 clang::ASTContext::setNonKeyFunction(clang::CXXMethodDecl const*) + 145

5   clang                         	       0x1084e0001 void std::__1::__insertion_sort_move[abi:nn200100]&lt;std::__1::_ClassicAlgPolicy, DumpRecordLayout(llvm::raw_ostream&amp;, clang::RecordDecl const*, clang::ASTContext const&amp;, clang::CharUnits, unsigned int, char const*, bool, bool)::$_0&amp;, clang::CXXRecordDecl const**&gt;(clang::CXXRecordDecl const**, clang::CXXRecordDecl const**, std::__1::iterator_traits&lt;clang::CXXRecordDecl const**&gt;::value_type*, DumpRecordLayout(llvm::raw_ostream&amp;, clang::RecordDecl const*, clang::ASTContext const&amp;, clang::CharUnits, unsigned int, char const*, bool, bool)::$_0&amp;) + 753

5   clang                         	       0x109800001 RequiresVtordisp(llvm::SmallPtrSetImpl&lt;clang::CXXRecordDecl const*&gt; const&amp;, clang::CXXRecordDecl const*) + 1

...
```


Here is the code.

My guess:
In the following code the compiler is led to check for the concept `input_range` on the class _`integral_range`_, thus checking for begin and end returning an _`integral_iterator`_ that could be constructed with a missing deduction guide. If I add the guide, the issue disappears. The instabilities to get a crash and the logs seem to indicate a memory corruption.

```c++
template &lt;class I&gt; struct integral_iterator
{
    I index;
    integral_iterator  operator++(int);
    integral_iterator&amp; operator++();
    bool operator==(const integral_iterator&amp;) const;
};
template &lt;class I&gt; struct integral_range
{
    I _min;
    I _max;
};
#if 1 // crash
// here are a use case for DeclareAggregateDeductionGuideFromInitList
template &lt;class I&gt; auto begin (const integral_range&lt;I&gt;&amp; r) { return integral_iterator{r._min};}
template &lt;class I&gt; auto   end (const integral_range&lt;I&gt;&amp; r) { return integral_iterator{r._max+1};}
#else
template &lt;class I&gt; integral_iterator&lt;I&gt; begin (const integral_range&lt;I&gt;&amp; r);
template &lt;class I&gt; integral_iterator&lt;I&gt;   end (const integral_range&lt;I&gt;&amp; r);
#endif

template &lt;class T&gt; struct A
{
    template &lt;std::ranges::input_range AA&gt; A(const AA&amp;);
};

template &lt;class T&gt; using B = integral_range&lt;T&gt;;

template &lt;class T&gt; void f(A&lt;T&gt;); // overloaded function

template &lt;class T&gt; void f (const B&lt;T&gt;&amp;);

template &lt;class T&gt; void foo(const B&lt;T&gt;&amp; r) { return f(r);}
```

In fact, the code was in a more complex context of namespaces and structures, like this one:

```c++
namespace ZZ
{
    template &lt;class T&gt; struct Case0
    {
        static void f(A&lt;T&gt;);

        static void f (const B&lt;T&gt;&amp;);
    };
    struct Case1 // ok
    {
        template &lt;class T&gt; static void f(A&lt;T&gt;);

        template &lt;class T&gt; static void f (const B&lt;T&gt;&amp;);
    };
    template &lt;class I&gt; void f(const B&lt;I&gt;&amp; r)
    {
//        return Case1::f(r);//ok (context!)
        return Case0&lt;I&gt;::f(r);
    }
//    template void f (const B&lt;int&gt;&amp;); //ok with this. (context!)
}
namespace AA
{
    template &lt;class T&gt; void foo(const B&lt;T&gt;&amp; r) { return ZZ::f(r);}

    template void f (const B&lt;int&gt;&amp;);
}
```
</details>


---

