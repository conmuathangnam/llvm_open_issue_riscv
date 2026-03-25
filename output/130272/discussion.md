# [clang] Assertion hit in `CallExpr::getBeginLoc()`: `assert(HasFirstArg)` fails

**Author:** jiixyj
**URL:** https://github.com/llvm/llvm-project/issues/130272
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-03-12T05:01:53Z

## Body

I'm getting an assertion hit in `CallExpr::getBeginLoc()`:

```
clang-21: /freebsd/data/linux/git-net/llvm-project/clang/lib/AST/Expr.cpp:1660: SourceLocation clang::CallExpr::getBeginLoc() const: Assertion `HasFirstArg' failed.
```

A snippet of the stacktrace (I can send you the full one on request):

```
1.	/home/jan/git/ldgr/src/ldgr/args.cppm:290:20: current parser token ')'
2.	/home/jan/git/ldgr/src/ldgr/args.cppm:86:1: parsing namespace 'ldgr'
3.	/home/jan/git/ldgr/src/ldgr/args.cppm:204:1: parsing struct/union/class body 'ldgr::argument_option_impl'
4.	/home/jan/git/ldgr/src/ldgr/args.cppm:288:5: parsing function body 'ldgr::argument_option_impl::next_arg'
5.	/home/jan/git/ldgr/src/ldgr/args.cppm:288:5: in compound statement ('{}')
 #0 0x000055603d52b588 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:13
 #1 0x000055603d529690 llvm::sys::RunSignalHandlers() /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #2 0x000055603d52bc01 SignalHandler(int, siginfo_t*, void*) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #3 0x00007fb403e558c0 __restore_rt libc_sigaction.c:0:0
 #4 0x00007fb403ea462c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007fb403e55822 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x00007fb403e3e4af abort ./stdlib/abort.c:81:7
 #7 0x00007fb403e3e3d7 _nl_load_domain.cold ./intl/loadmsgcat.c:1177:9
 #8 0x00007fb403e4e342 (/usr/lib/libc.so.6+0x38342)
 #9 0x000055603ffb696c clang::CallExpr::getBeginLoc() const /freebsd/data/linux/git-net/llvm-project/clang/lib/AST/Expr.cpp:1666:26
#10 0x000055603f7bd395 TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:1795:10
#11 0x000055603f7ccb87 TryCopyInitialization(clang::Sema&, clang::Expr*, clang::QualType, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:5759:10
#12 0x000055603f7d09c8 clang::Sema::AddConversionCandidate(clang::CXXConversionDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::Expr*, clang::QualType, clang::OverloadCandidateSet&, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:8160:7
#13 0x000055603f7c358e IsUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&, clang::OverloadCandidateSet&, clang::Sema::AllowedExplicit, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:3978:13
#14 0x000055603f7f0296 TryUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:1665:3
#15 0x000055603f7bd5b3 TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:0:10
```

The problematic code in `args.cppm`:

```c++
286                                                                                                                                                                                             
287     bool next_arg(std::string_view& current_arg, argument_input_iterator& in, ldgr::util::any_sentinel s)                                                                                   
288     {                                                                                                                                                                                       
289         ++in;                                                                                                                                                                               
290         if (in == s) {         // <----- it crashes here
291             throw args_parse_error{std::format("option '--{}' needs an argument", long_name_)};                                                                                             
292         }                                                                                                                                                                                   
293         return this->set_result(current_arg, in, *in);                                                                                                                                      
294     }                   
```

This leads to the following function:

```c++
    template <std::sentinel_for<It> S>
    bool operator==(this input_iterator_ref self, const S& s)
    {
        return *self.it_ == s;
    }
```

i.e. a comparison operator with an explicit object parameter.

@HighCommander4 : I've seen that you have done some work in this area -- maybe this is another special case that must be added?

I can try to create a minimal example. At the moment I can workaround the crash by adding `&& getNumArgs() > 0` to the `if` statement.

## Comments

### Comment 1 - jiixyj

https://github.com/llvm/llvm-project/pull/117841 is the PR that introduced the problem.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jan Kokemüller (jiixyj)

<details>
I'm getting an assertion hit in `CallExpr::getBeginLoc()`:

```
clang-21: /freebsd/data/linux/git-net/llvm-project/clang/lib/AST/Expr.cpp:1660: SourceLocation clang::CallExpr::getBeginLoc() const: Assertion `HasFirstArg' failed.
```

A snippet of the stacktrace (I can send you the full one on request):

```
1.	/home/jan/git/ldgr/src/ldgr/args.cppm:290:20: current parser token ')'
2.	/home/jan/git/ldgr/src/ldgr/args.cppm:86:1: parsing namespace 'ldgr'
3.	/home/jan/git/ldgr/src/ldgr/args.cppm:204:1: parsing struct/union/class body 'ldgr::argument_option_impl'
4.	/home/jan/git/ldgr/src/ldgr/args.cppm:288:5: parsing function body 'ldgr::argument_option_impl::next_arg'
5.	/home/jan/git/ldgr/src/ldgr/args.cppm:288:5: in compound statement ('{}')
 #<!-- -->0 0x000055603d52b588 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:13
 #<!-- -->1 0x000055603d529690 llvm::sys::RunSignalHandlers() /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x000055603d52bc01 SignalHandler(int, siginfo_t*, void*) /freebsd/data/linux/git-net/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #<!-- -->3 0x00007fb403e558c0 __restore_rt libc_sigaction.c:0:0
 #<!-- -->4 0x00007fb403ea462c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007fb403e55822 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x00007fb403e3e4af abort ./stdlib/abort.c:81:7
 #<!-- -->7 0x00007fb403e3e3d7 _nl_load_domain.cold ./intl/loadmsgcat.c:1177:9
 #<!-- -->8 0x00007fb403e4e342 (/usr/lib/libc.so.6+0x38342)
 #<!-- -->9 0x000055603ffb696c clang::CallExpr::getBeginLoc() const /freebsd/data/linux/git-net/llvm-project/clang/lib/AST/Expr.cpp:1666:26
#<!-- -->10 0x000055603f7bd395 TryImplicitConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:1795:10
#<!-- -->11 0x000055603f7ccb87 TryCopyInitialization(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:5759:10
#<!-- -->12 0x000055603f7d09c8 clang::Sema::AddConversionCandidate(clang::CXXConversionDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::Expr*, clang::QualType, clang::OverloadCandidateSet&amp;, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:8160:7
#<!-- -->13 0x000055603f7c358e IsUserDefinedConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&amp;, clang::OverloadCandidateSet&amp;, clang::Sema::AllowedExplicit, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:3978:13
#<!-- -->14 0x000055603f7f0296 TryUserDefinedConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:1665:3
#<!-- -->15 0x000055603f7bd5b3 TryImplicitConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) /freebsd/data/linux/git-net/llvm-project/clang/lib/Sema/SemaOverload.cpp:0:10
```

The problematic code in `args.cppm`:

```c++
286                                                                                                                                                                                             
287     bool next_arg(std::string_view&amp; current_arg, argument_input_iterator&amp; in, ldgr::util::any_sentinel s)                                                                                   
288     {                                                                                                                                                                                       
289         ++in;                                                                                                                                                                               
290         if (in == s) {         // &lt;----- it crashes here
291             throw args_parse_error{std::format("option '--{}' needs an argument", long_name_)};                                                                                             
292         }                                                                                                                                                                                   
293         return this-&gt;set_result(current_arg, in, *in);                                                                                                                                      
294     }                   
```

This leads to the following function:

```c++
    template &lt;std::sentinel_for&lt;It&gt; S&gt;
    bool operator==(this input_iterator_ref self, const S&amp; s)
    {
        return *self.it_ == s;
    }
```

i.e. a comparison operator with an explicit object parameter.

@<!-- -->HighCommander4 : I've seen that you have done some work in this area -- maybe this is another special case that must be added?

I can try to create a minimal example. At the moment I can workaround the crash by adding `&amp;&amp; getNumArgs() &gt; 0` to the `if` statement.
</details>


---

### Comment 3 - shafik

Can you provide a minimal reproducer, preferably w/ a godbolt link demonstrating the crash?

---

### Comment 4 - HighCommander4

> [#117841](https://github.com/llvm/llvm-project/pull/117841) is the PR that introduced the problem.

Note that a more recent PR, https://github.com/llvm/llvm-project/pull/126868, made `CallExpr::getBeginLoc()` more robust, such that in an assertions-disabled build it recovers successfully from this scenario, and at worst returns a suboptimal begin location.

I left the assertion in to try to flush out remaining edge cases and produce a better begin location in those cases as well. It sounds like you've stumbled onto such an edge case. I'm happy to investigate it given a reproducer.

---

### Comment 5 - jiixyj

Thank you! I've managed to reduce it down to this example that uses a conversion operator with an explicit this parameter:

```c++
struct A {};
struct B {
  operator A(this B);
} _ = A(B{});
```

https://godbolt.org/z/Ye9PvExhf

---

### Comment 6 - jiixyj

...and a less minimal one, but one that should actually compile correctly:

```c++
struct C {};
template <typename D> struct A {
  operator C(this D);
};
struct B : A<B> {};
auto _ = C(B{});
```

https://godbolt.org/z/Pe868YfnT

---

### Comment 7 - HighCommander4

Thanks for the reproducer!

So the `CallExpr` object representing the conversion that ends up in the final AST passes the assertion fine.

The `CallExpr` object that fails the assertion is a temporary one constructed during overload resolution (?) by [`CallExpr::CreateTemporary()`](https://searchfox.org/llvm/rev/f8cf007b4cdc975e44c94c2a3276023b7816cd74/clang/include/clang/AST/Expr.h#3016), which is described as (emphasis mine):

> /// Create a temporary call expression **with no arguments** in the memory
> /// pointed to by Mem. Mem must points to at least sizeof(CallExpr)
> /// + sizeof(Stmt *) bytes of storage, aligned to alignof(CallExpr):

Such `CallExpr` objects will fail the assertion because they're constructed without any arguments even if the callee requires arguments.

I don't think we care about the `getBeginLoc()` of such temporary call expressions that arise during overload resolution (and if we do, the location of the callee is the only thing we can return anyways since we don't have arguments), so it would be appropriate to skip the assertion for such expressions.

However, it's not immediately clear how I can tell that a given `CallExpr` is this temporary kind rather than a regular one.

---

### Comment 8 - zyn0217

> However, it's not immediately clear how I can tell that a given CallExpr is this temporary kind rather than a regular one.

I don't think we need to strictly distinguish these peculiar expressions, perhaps we could simply validate the number of arguments when computing the source location and skip the refining logic if no arguments are otherwise available. I.e. remove the assertion anyway.

---

### Comment 9 - HighCommander4

> > However, it's not immediately clear how I can tell that a given CallExpr is this temporary kind rather than a regular one.
> 
> I don't think we need to strictly distinguish these peculiar expressions, perhaps we could simply validate the number of arguments when computing the source location and skip the refining logic if no arguments are otherwise available. I.e. remove the assertion anyway.

Makes sense, thanks! Sent out https://github.com/llvm/llvm-project/pull/130725.

---

