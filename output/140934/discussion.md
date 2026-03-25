# trunk fails to `#include <string>`

**Author:** scarf005
**URL:** https://github.com/llvm/llvm-project/issues/140934
**Status:** Closed
**Labels:** clang:frontend, regression, libstdc++, lambda
**Closed Date:** 2025-05-22T09:16:02Z

## Body

## Reproduction

```c++
#include <string> 
```
https://godbolt.org/z/Kbaj8Mv6j
with flag `-std=c++20` fails with:
```
In file included from <source>:1:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/string:57:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.tcc:633:12: error: variable has incomplete type 'void'
  633 |       auto __r = std::move(__op)(__p + 0, __n + 0);
      |            ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4831:7: note: in instantiation of function template specialization 'std::basic_string<wchar_t>::__resize_and_overwrite<(lambda at /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4832:9)>' requested here
 4831 |         __ws.__resize_and_overwrite(__s.size(),
      |              ^
1 error generated.
Compiler returned: 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: scarf (scarf005)

<details>
## Reproduction

[20.1.0 (works)](&lt;https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:13,endLineNumber:11,positionColumn:13,positionLineNumber:11,selectionStartColumn:13,selectionStartLineNumber:11,startColumn:13,startLineNumber:11),source:'%23include+%3Cvector%3E%0A%23include+%3Cranges%3E%0A%23include+%3Ciostream%3E%0A%0A///+@<!-- -->remark+poor+person!'s+flatmap%0Ainline+constexpr+auto+flat_map+%3D+%5B%5D(+auto+%26%26func+)%0A%7B%0A++++return+std::views::transform(+std::forward%3Cdecltype(+func+)%3E(+func+)+)+%7C+std::views::join%3B%0A%7D%3B%0A%0Aint+main()+%7B%0A++++auto+res+%3D+std::vector%7B+1,+2,+3+%7D%0A+++++++++++++++++++%7C+flat_map(+%5B%5D(+const+int+i+)+-%3E+auto+%7B+return+std::vector%7Bi,+i+*+2+%7D%3B+%7D+)%0A+++++++++++++++++++%7C+std::ranges::to%3Cstd::vector%3E()%3B%0A+++++++++++++++++++%0A++++for+(int+i+:+res)+%7B%0A++++++++std::cout+%3C%3C+i+%3C%3C+!'+!'%3B%0A++++%7D%0A++++return+0%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),header:(),k:51.06906906906907,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang2010,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'1',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+20.1.0+(Editor+%231)',t:'0'),(h:output,i:(compilerName:'x86-64+gcc+14.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+20.1.0+(Compiler+%231)',t:'0')),header:(),k:48.93093093093094,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4&gt;)
[trunk (fails)](&lt;https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:24,endLineNumber:16,positionColumn:24,positionLineNumber:16,selectionStartColumn:24,selectionStartLineNumber:16,startColumn:24,startLineNumber:16),source:'%23include+%3Cvector%3E%0A%23include+%3Cranges%3E%0A%23include+%3Ciostream%3E%0A%0A///+@<!-- -->remark+poor+person!'s+flatmap%0Ainline+constexpr+auto+flat_map+%3D+%5B%5D(+auto+%26%26func+)%0A%7B%0A++++return+std::views::transform(+std::forward%3Cdecltype(+func+)%3E(+func+)+)+%7C+std::views::join%3B%0A%7D%3B%0A%0Aint+main()+%7B%0A++++auto+res+%3D+std::vector%7B+1,+2,+3+%7D%0A+++++++++++++++++++%7C+flat_map(+%5B%5D(+const+int+i+)+-%3E+auto+%7B+return+std::vector%7Bi,+i+*+2+%7D%3B+%7D+)%0A+++++++++++++++++++%7C+std::ranges::to%3Cstd::vector%3E()%3B%0A+++++++++++++++++++%0A++++for+(int+i+:+res)+%7B%0A++++++++std::cout+%3C%3C+i+%3C%3C+!'+!'%3B%0A++++%7D%0A++++return+0%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),header:(),k:51.06906906906907,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang_trunk,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'1',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+(trunk)+(Editor+%231)',t:'0'),(h:output,i:(compilerName:'x86-64+gcc+14.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+(trunk)+(Compiler+%231)',t:'0')),header:(),k:48.93093093093094,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4&gt;)

also fails to build on ubuntu clang++ 20.1.6

## Error log

```
In file included from &lt;source&gt;:2:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/ranges:45:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/iterator:68:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/streambuf_iterator.h:37:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/streambuf:45:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/ios_base.h:43:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/locale_classes.h:42:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/string:57:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.tcc:633:12: error: variable has incomplete type 'void'
  633 |       auto __r = std::move(__op)(__p + 0, __n + 0);
      |            ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.tcc:607:7: note: in instantiation of function template specialization 'std::basic_string&lt;wchar_t&gt;::resize_and_overwrite&lt;(lambda at /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4832:9) &amp;&gt;' requested here
  607 |     { resize_and_overwrite&lt;_Operation&amp;&gt;(__n, __op); }
      |       ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4831:7: note: in instantiation of function template specialization 'std::basic_string&lt;wchar_t&gt;::__resize_and_overwrite&lt;(lambda at /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4832:9)&gt;' requested here
 4831 |         __ws.__resize_and_overwrite(__s.size(),
      |              ^
1 error generated.
ASM generation compiler returned: 1
In file included from &lt;source&gt;:2:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/ranges:45:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/iterator:68:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/streambuf_iterator.h:37:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/streambuf:45:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/ios_base.h:43:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/locale_classes.h:42:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/string:57:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.tcc:633:12: error: variable has incomplete type 'void'
  633 |       auto __r = std::move(__op)(__p + 0, __n + 0);
      |            ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.tcc:607:7: note: in instantiation of function template specialization 'std::basic_string&lt;wchar_t&gt;::resize_and_overwrite&lt;(lambda at /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4832:9) &amp;&gt;' requested here
  607 |     { resize_and_overwrite&lt;_Operation&amp;&gt;(__n, __op); }
      |       ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4831:7: note: in instantiation of function template specialization 'std::basic_string&lt;wchar_t&gt;::__resize_and_overwrite&lt;(lambda at /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/basic_string.h:4832:9)&gt;' requested here
 4831 |         __ws.__resize_and_overwrite(__s.size(),
      |              ^
1 error generated.
Execution build compiler returned: 1
Build failed
```
</details>


---

### Comment 2 - shafik

Note, this works on trunk w/ libc++: https://godbolt.org/z/oz8WEcczx




---

### Comment 3 - scarf005

hi, may i ask what does the `needs-reduction` label means?

---

### Comment 4 - EugeneZelenko

@scarf005: smaller test case that reproduces problem is needed.

---

### Comment 5 - shafik

> hi, may i ask what does the `needs-reduction` label means?

We usually want a [Minimal, Reproducible Example](https://stackoverflow.com/help/minimal-reproducible-example). In this case b/c you have includes which expand to very large amount of source in order to fully understand it we want to perform a reduction on it.

[c-reduce](https://pramodkumbhar.com/2024/01/c-reduce-systematically-tackling-not-only-compiler-bugs-in-hpc-too/) is the goto tool folks use and there is also [cvise](https://github.com/marxin/cvise) which is a parallel form.

Once we have reduced code it is usually a lot quicker to see what the actual problem is and then determine what the fix should be. Right now it is not clear if this is a libstdc++ issue or a clang issue or maybe some combination of the two. 

---

### Comment 6 - scarf005

turns out just including `<ranges>` alone causes the problem. updated issue body.

---

### Comment 7 - shafik

This is regression on trunk: https://godbolt.org/z/rYGrEY1rx



---

### Comment 8 - shafik

@EugeneZelenko that is not a reduction b/c it still include a standard header.

---

### Comment 9 - EugeneZelenko

> [@EugeneZelenko](https://github.com/EugeneZelenko) that is not a reduction b/c it still include a standard header.

Sorry for noise.

---

### Comment 10 - scarf005

still includes standard header but reduced further by reproducing with only `#include <string>`

---

### Comment 11 - shafik

I can reproduce this locally at all, I even tried using a preprocessed file and no luck. So someone else needs to reduce this.

---

### Comment 12 - shafik

@cor3ntin let me know offline that perhaps he fixed this yesterday and he will follow-up tomorrow.

---

### Comment 13 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/drTznacWf)):

```C++
template<class Op>
constexpr void take_operation(Op op) {
  auto m [[maybe_unused]] = op();
}

void bar() {
  if constexpr (false) {
    ::take_operation([]{ return 42; });
  }
}
```

It seems that in a discarded `if constexpr` branch, Clang sometimes doesn't deduce the return type of a lambda expression without trailing return type (or, alternatively, ignores the lambda body), and simply treat the return type as `void`. This approach is incorrect when the discarded branch needs to be instantiated.

It's curious that `constexpr` on `take_operation` (or libstdc++'s `__resize_and_overwrite`) is necessary to reproduce the error.

---

### Comment 14 - frederick-vs-ja

> [@cor3ntin](https://github.com/cor3ntin) let me know offline that perhaps he fixed this yesterday and he will follow-up tomorrow.

Yeah. The test case added in #140859 looks quite similar to the reduced example. Perhaps the PR has already fixed this issue.

---

### Comment 15 - cor3ntin

Fixed. https://godbolt.org/z/dq5ebGr9d
Sorry about that

---

