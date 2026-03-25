# [clang] TemplateArgument printing falls for infinite recursion

**Author:** tomrittervg
**URL:** https://github.com/llvm/llvm-project/issues/122455
**Status:** Closed
**Labels:** invalid, clang:frontend, crash-on-invalid
**Closed Date:** 2025-01-30T19:48:09Z

## Body

cc @llvm/issue-subscribers-clang-static-analyzer 

The following program (creduce-d from real Firefox source code, originally 133,189 lines long) and command produces a segfault in clang static analyzer.  It seems like an infinite loop and stack exhaustion, the stack is 9333 frames long for me.

```
template < typename >
struct a  
template < typename b>
struct __is_invocable
    : __is_invocable_impl< a< b>>::c 
;
struct d
    template < typename  >
using e =
    d  ;
template < typename b >
struct f
    :  __is_invocable<  
              e< b> >
```

```
clang-20 --analyze VTuneWrapper.cpp
```

I'm using clang from version https://github.com/llvm/llvm-project/commit/9d5299eb61a64cd4df5fefa0299b0cf8d917978f which is ~ 17 hours old from time of writing.

I'm currently getting a segfault on about 3971 out of 6219 Firefox source files - I don't know if this is the source of all of those segfaults, but I'm imagining it's a significant portion of them.

The stack looks like this:

```
#0  0x00007ffff5abeee5 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#1  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#2  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#3  0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#4  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#5  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#6  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#7  0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#8  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#10 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#11 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#12 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#13 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#14 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#15 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#16 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#17 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#18 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#19 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#20 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#21 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#22 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#23 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#24 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#25 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#26 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#27 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#28 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#29 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#30 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#31 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#32 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#33 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#34 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#35 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#36 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#37 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#38 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#39 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#40 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#41 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#42 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#43 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
--Type <RET> for more, q to quit, c to continue without paging--c
#44 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#45 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#46 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#47 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#48 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#49 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#50 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#51 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#52 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#53 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#54 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#55 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#56 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#57 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#58 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#59 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#60 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#61 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#62 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#63 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#64 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#65 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#66 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#67 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#68 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#69 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#70 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#71 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#72 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#73 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#74 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#75 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#76 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#77 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#78 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#79 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#80 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#81 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#82 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#83 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#84 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#85 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#86 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#87 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#88 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#89 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#90 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#91 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#92 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#93 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#94 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#95 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#96 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#97 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#98 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#99 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#100 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#101 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#102 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#103 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#104 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#105 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#106 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#107 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#108 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#109 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#110 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#111 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#112 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#113 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#114 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#115 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#116 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#117 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#118 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#119 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#120 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#121 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#122 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#123 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#124 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#125 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#126 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#127 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#128 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#129 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#130 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#131 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#132 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#133 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#134 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#135 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#136 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#137 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#138 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#139 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#140 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#141 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#142 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#143 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#144 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#145 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#146 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#147 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#148 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#149 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#150 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#151 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#152 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#153 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#154 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#155 0x00007ffff5ac2c6c in void printTo<clang::TemplateArgument>(llvm::raw_ostream&, llvm::ArrayRef<clang::TemplateArgument>, clang::PrintingPolicy const&, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#156 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#157 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&, clang::PrintingPolicy const&, llvm::Twine const&, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#158 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&, llvm::raw_ostream&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: Tom Ritter (tomrittervg)

<details>
cc @<!-- -->llvm/issue-subscribers-clang-static-analyzer 

The following program (creduce-d from real Firefox source code, originally 133,189 lines long) and command produces a segfault in clang static analyzer.  It seems like an infinite loop and stack exhaustion, the stack is 9333 frames long for me.

```
template &lt; typename &gt;
struct a  
template &lt; typename b&gt;
struct __is_invocable
    : __is_invocable_impl&lt; a&lt; b&gt;&gt;::c 
;
struct d
    template &lt; typename  &gt;
using e =
    d  ;
template &lt; typename b &gt;
struct f
    :  __is_invocable&lt;  
              e&lt; b&gt; &gt;
```

```
clang-20 --analyze VTuneWrapper.cpp
```

I'm using clang from version https://github.com/llvm/llvm-project/commit/9d5299eb61a64cd4df5fefa0299b0cf8d917978f which is ~ 17 hours old from time of writing.

I'm currently getting a segfault on about 3971 out of 6219 Firefox source files - I don't know if this is the source of all of those segfaults, but I'm imagining it's a significant portion of them.

The stack looks like this:

```
#<!-- -->0  0x00007ffff5abeee5 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->1  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->2  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->3  0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->4  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->5  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->6  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->7  0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->8  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->9  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->10 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->11 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->12 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->13 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->14 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->15 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->16 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->17 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->18 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->19 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->20 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->21 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->22 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->23 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->24 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->25 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->26 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->27 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->28 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->29 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->30 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->31 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->32 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->33 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->34 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->35 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->36 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->37 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->38 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->39 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->40 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->41 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->42 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->43 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
--Type &lt;RET&gt; for more, q to quit, c to continue without paging--c
#<!-- -->44 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->45 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->46 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->47 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->48 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->49 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->50 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->51 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->52 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->53 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->54 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->55 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->56 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->57 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->58 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->59 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->60 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->61 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->62 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->63 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->64 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->65 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->66 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->67 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->68 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->69 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->70 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->71 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->72 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->73 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->74 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->75 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->76 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->77 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->78 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->79 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->80 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->81 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->82 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->83 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->84 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->85 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->86 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->87 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->88 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->89 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->90 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->91 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->92 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->93 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->94 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->95 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->96 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->97 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->98 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->99 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->100 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->101 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->102 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->103 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->104 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->105 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->106 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->107 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->108 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->109 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->110 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->111 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->112 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->113 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->114 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->115 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->116 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->117 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->118 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->119 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->120 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->121 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->122 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->123 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->124 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->125 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->126 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->127 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->128 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->129 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->130 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->131 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->132 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->133 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->134 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->135 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->136 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->137 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->138 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->139 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->140 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->141 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->142 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->143 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->144 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->145 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->146 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->147 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->148 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->149 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->150 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->151 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->152 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->153 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->154 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->155 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->156 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->157 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->158 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git

```
</details>


---

### Comment 2 - steakhal

I've built the desired clang, pasted the code into a file named `VTuneWrapper.cpp`, then ran `build/bin/clang-20 --analyze VTuneWrapper.cpp` and it didn't crash.
```
[many more lines]
VTuneWrapper.cpp:5:7: note: (skipping 1015 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
VTuneWrapper.cpp:5:7: note: in instantiation of template class '__is_invocable<a<a<a<a<d>>>>>' requested here
VTuneWrapper.cpp:5:7: note: in instantiation of template class '__is_invocable<a<a<a<d>>>>' requested here
VTuneWrapper.cpp:5:7: note: in instantiation of template class '__is_invocable<a<a<d>>>' requested here
VTuneWrapper.cpp:5:7: note: in instantiation of template class '__is_invocable<a<d>>' requested here
VTuneWrapper.cpp:13:8: note: in instantiation of template class '__is_invocable<d>' requested here
   13 |     :  __is_invocable<  
      |        ^
VTuneWrapper.cpp:5:7: note: use -ftemplate-depth=N to increase recursive template instantiation depth
    5 |     : __is_invocable_impl< a< b>>::c 
      |       ^
5 errors generated
```

I have assertions enabled, release with debug info.
Could you double check your config? @tomrittervg

---

### Comment 3 - steakhal

FYI the trace also looks suspicious, as in production I don't think we should really print types, and it seems like the inf recursion happens inside type printing. Maybe you could share the topmost 50 stack frames, to see how the Static Analyzer gets involved here to get a better understanding.

---

### Comment 4 - tomrittervg

HEre's the top of the stack

```
#9238 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9239 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9240 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9241 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9242 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9243 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9244 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9245 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9246 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9247 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9248 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9249 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9250 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9251 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9252 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9253 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9254 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9255 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9256 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9257 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9258 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9259 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9260 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9261 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9262 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9263 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9264 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9265 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9266 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9267 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9268 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9269 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9270 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9271 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9272 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9273 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9274 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9275 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9276 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9277 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9278 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9279 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9280 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9281 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9282 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9283 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9284 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9285 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9286 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9287 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9288 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9289 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9290 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9291 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9292 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9293 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9294 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9295 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9296 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9297 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9298 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9299 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9300 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9301 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9302 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9303 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9304 0x00007ffff56002d2 in clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9305 0x00007ffff56dc815 in clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9306 0x00007ffff56d8266 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9307 0x00007ffff5b68d3f in clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9308 0x00007ffff5aff6b4 in clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9309 0x00007ffff5afe396 in clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9310 0x00007ffff55f043e in clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9311 0x00007ffff5dd4bbd in clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9312 0x00007ffff5dd7802 in clang::Parser::ParseBaseSpecifier(clang::Decl*) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9313 0x00007ffff5dd6bf6 in clang::Parser::ParseBaseClause(clang::Decl*) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9314 0x00007ffff62805b8 in clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9315 0x00007ffff627db42 in clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9316 0x00007ffff583cae0 in clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9317 0x00007ffff5750a5a in clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9318 0x00007ffff574ebc1 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9319 0x00007ffff5de4ff4 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9320 0x00007ffff5de5ee7 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9321 0x00007ffff60373b7 in clang::ParseAST(clang::Sema&, bool, bool) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9322 0x00007ffff605bf8c in clang::FrontendAction::Execute() () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9323 0x00007ffff605bbdc in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9324 0x00007ffff76936d3 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9325 0x0000555555565161 in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) ()
#9326 0x000055555555edbe in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) ()
#9327 0x00007ffff723aa99 in void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::$_0>(long) () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9328 0x00007ffff22d54b3 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libLLVM.so.20.0git
#9329 0x00007ffff604419e in clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9330 0x00007ffff5f33afa in clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9331 0x00007ffff5f334bd in clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#9332 0x000055555556104c in clang_main(int, char**, llvm::ToolContext const&) ()
#9333 0x000055555555f7f4 in main ()
```

If it doesn't reproduce on vanilla clang it's possible it's being caused by one of our [local patches](https://searchfox.org/mozilla-central/source/build/build-clang/clang-trunk.json) but I'd be surprised.  I will build from source and try also.

---

### Comment 5 - steakhal

The stack trace suggests that the crash happens during parsing, so it doesn't even reach the Static Analyzer consumer. Consequently, the crash couldn't happen during CSA analysis. So, I'll drop the static analyzer label, and leave a vanilla clang one.

---

### Comment 6 - tomrittervg

So after some experimenting, I've figured out this seems to come down to `-ftemplate-depth` - my local build of clang (with Mozilla patches) didn't crash originally, but doing `clang -ftemplate-depth=9999 --analyze VTuneWrapper.cpp` will produce a segfault with a similar stack.  Similarly, by limiting Mozilla's clang build I can avoid the crash.  

It reports "VTuneWrapper.cpp:5:7: fatal error: recursive template instantiation exceeded maximum depth of 9999" but presumably it shouldn't crash, so it seems like a bug, although not a very interesting one.

I think I still have a bug in the analyzer though, I just unhelpfully stumbled across this separate crash while minimizing.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tom Ritter (tomrittervg)

<details>
cc @<!-- -->llvm/issue-subscribers-clang-static-analyzer 

The following program (creduce-d from real Firefox source code, originally 133,189 lines long) and command produces a segfault in clang static analyzer.  It seems like an infinite loop and stack exhaustion, the stack is 9333 frames long for me.

```
template &lt; typename &gt;
struct a  
template &lt; typename b&gt;
struct __is_invocable
    : __is_invocable_impl&lt; a&lt; b&gt;&gt;::c 
;
struct d
    template &lt; typename  &gt;
using e =
    d  ;
template &lt; typename b &gt;
struct f
    :  __is_invocable&lt;  
              e&lt; b&gt; &gt;
```

```
clang-20 --analyze VTuneWrapper.cpp
```

I'm using clang from version https://github.com/llvm/llvm-project/commit/9d5299eb61a64cd4df5fefa0299b0cf8d917978f which is ~ 17 hours old from time of writing.

I'm currently getting a segfault on about 3971 out of 6219 Firefox source files - I don't know if this is the source of all of those segfaults, but I'm imagining it's a significant portion of them.

The stack looks like this:

```
#<!-- -->0  0x00007ffff5abeee5 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->1  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->2  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->3  0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->4  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->5  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->6  0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->7  0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->8  0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->9  0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->10 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->11 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->12 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->13 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->14 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->15 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->16 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->17 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->18 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->19 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->20 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->21 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->22 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->23 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->24 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->25 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->26 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->27 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->28 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->29 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->30 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->31 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->32 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->33 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->34 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->35 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->36 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->37 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->38 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->39 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->40 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->41 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->42 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->43 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
--Type &lt;RET&gt; for more, q to quit, c to continue without paging--c
#<!-- -->44 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->45 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->46 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->47 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->48 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->49 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->50 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->51 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->52 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->53 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->54 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->55 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->56 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->57 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->58 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->59 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->60 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->61 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->62 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->63 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->64 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->65 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->66 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->67 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->68 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->69 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->70 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->71 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->72 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->73 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->74 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->75 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->76 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->77 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->78 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->79 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->80 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->81 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->82 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->83 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->84 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->85 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->86 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->87 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->88 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->89 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->90 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->91 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->92 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->93 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->94 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->95 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->96 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->97 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->98 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->99 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->100 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->101 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->102 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->103 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->104 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->105 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->106 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->107 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->108 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->109 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->110 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->111 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->112 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->113 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->114 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->115 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->116 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->117 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->118 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->119 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->120 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->121 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->122 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->123 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->124 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->125 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->126 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->127 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->128 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->129 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->130 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->131 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->132 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->133 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->134 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->135 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->136 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->137 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->138 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->139 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->140 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->141 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->142 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->143 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->144 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->145 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->146 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->147 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->148 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->149 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->150 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->151 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->152 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->153 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->154 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->155 0x00007ffff5ac2c6c in void printTo&lt;clang::TemplateArgument&gt;(llvm::raw_ostream&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::PrintingPolicy const&amp;, clang::TemplateParameterList const*, bool, unsigned int) [clone .llvm.8085457571282805237] () from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->156 0x00007ffff5abf690 in (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) [clone .llvm.8085457571282805237] ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->157 0x00007ffff5abedf8 in clang::QualType::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, llvm::Twine const&amp;, unsigned int) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git
#<!-- -->158 0x00007ffff561cfc2 in clang::TemplateArgument::print(clang::PrintingPolicy const&amp;, llvm::raw_ostream&amp;, bool) const ()
   from /home/tom/Documents/moz/static-analysis/clang-20-2025-01-09/bin/../lib/libclang-cpp.so.20.0git

```
</details>


---

### Comment 8 - steakhal

@tomrittervg If you encountered this inf. recursion (thus stack overflow) on some real code, let us know.
Otherwise, if you reported this because you accidentally reduced some other crash into this crash, then I don't think it's too interesting.

Let me elaborate on my position.

I think it's okay to have a compiler crash on code that only remotely resembles valid C++ code.
The error recovery - by nature - can and will do unholy things to recover from parsing errors.
After such tricks, all bets are off, and it tries to continue running on a best effort basis.
This time it means it goes into inf. recursion, and after some time halts with a crash.

So I'm tempted to close this issue as invalid.

---

### Comment 9 - tomrittervg

As far as I can tell, no this wasn't real code, it just accidentally reduced some other crash into this crash as you said.

---

### Comment 10 - steakhal

> As far as I can tell, no this wasn't real code, it just accidentally reduced some other crash into this crash as you said.

In this case, I'll close this.
Remember, we can always reopen it if it ever becomes relevant.

Thabk you.

---

