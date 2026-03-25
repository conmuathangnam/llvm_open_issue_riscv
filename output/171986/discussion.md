# Clang-format crash on header template file

**Author:** BoelGerhardt
**URL:** https://github.com/llvm/llvm-project/issues/171986
**Status:** Closed
**Labels:** duplicate, clang-format, crash
**Closed Date:** 2025-12-13T03:25:42Z

## Body

Segmentation fault when running clang format (v 19) on a template header file. See below for stacktrace running on file.h containing only the problematic line. 

```
$ clang-format --version
clang-format version 19.1.7 ( 19.1.7-2.module+el8.10.0+23045+e1f8e80e)

$ cat /etc/redhat-release 
Red Hat Enterprise Linux release 8.10 (Ootpa)
```

```
$ clang-format file.h 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-format file.h
 #0 0x00007fbdbb9904fc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.19.1+0xd984fc)
 #1 0x00007fbdbb98db24 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.19.1+0xd95b24)
 #2 0x00007fbdbb98ea55 (/lib64/libLLVM.so.19.1+0xd96a55)
 #3 0x00007fbdc6f7f990 __restore_rt (/lib64/libpthread.so.0+0x12990)
 #4 0x00007fbdc5dffa6c clang::format::FormatToken::isBlockIndentedInitRBrace(clang::format::FormatStyle const&) const (/lib64/libclang-cpp.so.19.1+0x3b12a6c)
 #5 0x00007fbdc5dd9b1b clang::format::ContinuationIndenter::mustBreak(clang::format::LineState const&) (/lib64/libclang-cpp.so.19.1+0x3aecb1b)
 #6 0x00007fbdc5e531ac (/lib64/libclang-cpp.so.19.1+0x3b661ac)
 #7 0x00007fbdc5e4f7fe clang::format::UnwrappedLineFormatter::format(llvm::SmallVectorImpl<clang::format::AnnotatedLine*> const&, bool, int, bool, unsigned int, unsigned int, unsigned int) (/lib64/libclang-cpp.so.19.1+0x3b627fe)
 #8 0x00007fbdc5deea6b (/lib64/libclang-cpp.so.19.1+0x3b01a6b)
 #9 0x00007fbdc5e32045 clang::format::TokenAnalyzer::process(bool) (/lib64/libclang-cpp.so.19.1+0x3b45045)
#10 0x00007fbdc5dead43 (/lib64/libclang-cpp.so.19.1+0x3afdd43)
#11 0x00007fbdc5deadbb (/lib64/libclang-cpp.so.19.1+0x3afddbb)
#12 0x00007fbdc5df0495 clang::format::internal::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*) (/lib64/libclang-cpp.so.19.1+0x3b03495)
#13 0x00007fbdc5df1fda clang::format::reformat(clang::format::FormatStyle const&, llvm::StringRef, llvm::ArrayRef<clang::tooling::Range>, llvm::StringRef, clang::format::FormattingAttemptStatus*) (/lib64/libclang-cpp.so.19.1+0x3b04fda)
#14 0x000055fe06240425 (/usr/bin/clang-format+0x13425)
#15 0x000055fe06238958 (/usr/bin/clang-format+0xb958)
#16 0x00007fbdb9f2c7e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#17 0x000055fe0623998e (/usr/bin/clang-format+0xc98e)
Segmentation fault (core dumped)
```

```
$ cat file.h
#define {{parameter_a}}_C_{{parameter_b|upper}} {{some_parameter_values + some_index}}
```

Another example file trying to isolate the issue further - the seg fault seems to trigger when the line exceeds a certain length:

```
$ cat another-crash-example.h 
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxxxx_over_by_1}}
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxx_this_is_ok}}

```
This file will cause the same seg-fault, but if commenting the first line it works just fine:

```
$ cat no-crash-example.h 
// #define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxxxx_over_by_1}}
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxx_this_is_ok}}

```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-format

Author: Boel Gerhardt (BoelGerhardt)

<details>
Segmentation fault when running clang format (v 19) on a template header file. See below for stacktrace running on file.h containing only the problematic line. 

```
$ clang-format --version
clang-format version 19.1.7 ( 19.1.7-2.module+el8.10.0+23045+e1f8e80e)

$ cat /etc/redhat-release 
Red Hat Enterprise Linux release 8.10 (Ootpa)
```

```
$ clang-format file.h 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: clang-format file.h
 #<!-- -->0 0x00007fbdbb9904fc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.19.1+0xd984fc)
 #<!-- -->1 0x00007fbdbb98db24 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.19.1+0xd95b24)
 #<!-- -->2 0x00007fbdbb98ea55 (/lib64/libLLVM.so.19.1+0xd96a55)
 #<!-- -->3 0x00007fbdc6f7f990 __restore_rt (/lib64/libpthread.so.0+0x12990)
 #<!-- -->4 0x00007fbdc5dffa6c clang::format::FormatToken::isBlockIndentedInitRBrace(clang::format::FormatStyle const&amp;) const (/lib64/libclang-cpp.so.19.1+0x3b12a6c)
 #<!-- -->5 0x00007fbdc5dd9b1b clang::format::ContinuationIndenter::mustBreak(clang::format::LineState const&amp;) (/lib64/libclang-cpp.so.19.1+0x3aecb1b)
 #<!-- -->6 0x00007fbdc5e531ac (/lib64/libclang-cpp.so.19.1+0x3b661ac)
 #<!-- -->7 0x00007fbdc5e4f7fe clang::format::UnwrappedLineFormatter::format(llvm::SmallVectorImpl&lt;clang::format::AnnotatedLine*&gt; const&amp;, bool, int, bool, unsigned int, unsigned int, unsigned int) (/lib64/libclang-cpp.so.19.1+0x3b627fe)
 #<!-- -->8 0x00007fbdc5deea6b (/lib64/libclang-cpp.so.19.1+0x3b01a6b)
 #<!-- -->9 0x00007fbdc5e32045 clang::format::TokenAnalyzer::process(bool) (/lib64/libclang-cpp.so.19.1+0x3b45045)
#<!-- -->10 0x00007fbdc5dead43 (/lib64/libclang-cpp.so.19.1+0x3afdd43)
#<!-- -->11 0x00007fbdc5deadbb (/lib64/libclang-cpp.so.19.1+0x3afddbb)
#<!-- -->12 0x00007fbdc5df0495 clang::format::internal::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, unsigned int, unsigned int, unsigned int, llvm::StringRef, clang::format::FormattingAttemptStatus*) (/lib64/libclang-cpp.so.19.1+0x3b03495)
#<!-- -->13 0x00007fbdc5df1fda clang::format::reformat(clang::format::FormatStyle const&amp;, llvm::StringRef, llvm::ArrayRef&lt;clang::tooling::Range&gt;, llvm::StringRef, clang::format::FormattingAttemptStatus*) (/lib64/libclang-cpp.so.19.1+0x3b04fda)
#<!-- -->14 0x000055fe06240425 (/usr/bin/clang-format+0x13425)
#<!-- -->15 0x000055fe06238958 (/usr/bin/clang-format+0xb958)
#<!-- -->16 0x00007fbdb9f2c7e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#<!-- -->17 0x000055fe0623998e (/usr/bin/clang-format+0xc98e)
Segmentation fault (core dumped)
```

```
$ cat file.h
#define {{parameter_a}}_C_{{parameter_b|upper}} {{some_parameter_values + some_index}}
```

Another example file trying to isolate the issue further - the seg fault seems to trigger when the line exceeds a certain length:

```
$ cat another-crash-example.h 
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxxxx_over_by_1}}
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxx_this_is_ok}}

```
This file will cause the same seg-fault, but if commenting the first line it works just fine:

```
$ cat no-crash-example.h 
// #define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxxxx_over_by_1}}
#define {{some_very_long_long_long_parameter_name_xxxxxxxxxxxxxxxxxxxxxxxxxx_this_is_ok}}

```

</details>


---

### Comment 2 - frederick-vs-ja

Could you try LLVM 21 or trunk?

---

### Comment 3 - EugeneZelenko

Please note that only most recent release is maintained.

---

### Comment 4 - BoelGerhardt

> Could you try LLVM 21 or trunk?

I could not install any other version on the same RHEL8 machine (controlled environment at work).

At home I tried reproducing on Ubuntu 24.04, but could not reproduce with neither clang-format v 18, 19 nor 20.

---

### Comment 5 - EugeneZelenko

> > Could you try LLVM 21 or trunk?
> 
> I could not install any other version on the same RHEL8 machine (controlled environment at work).
> 
> At home I tried reproducing on Ubuntu 24.04, but could not reproduce with neither clang-format v 18, 19 nor 20.

Do you have access to recent versions of Visual Studio Code and its C/C++ extensions on work? If so, recent versions of Clang-Format and Clang-Tidy should be available.

---

