# Crash in Lexer when doing code completion

**Author:** ckandeler
**URL:** https://github.com/llvm/llvm-project/issues/47195
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash
**Closed Date:** 2025-02-10T01:52:47Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [47851](https://llvm.org/bz47851) |
| Version | trunk |
| OS | Linux |
| Attachments | [example source file](https://user-images.githubusercontent.com/93435768/143761309-672480a2-385a-43b0-a338-7de228aa14da.gz) |
| CC | @ZequanWu |

## Extended Description 


Consider this simple piece of source code:

#define MY_MACRO() 0
#if MY_MACRO(

If you try to complete at the very end, you get a lexer crash:

$ clang++ -cc1 -xc -fsyntax-only -code-completion-at crash.cpp:2:14 crash.cpp
Stack dump:
0.	Program arguments: /usr/bin/clang++ -cc1 -xc -fsyntax-only -code-completion-at crash.cpp:2:14 crash.cpp -fcolor-diagnostics 
1.	crash.cpp:2:2: current parser token 'if'
 #&#8203;0 0x00007fdff8de6eeb llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/usr/bin/../lib/libLLVM-10.so+0x9e9eeb)
 #&#8203;1 0x00007fdff8de4a44 llvm::sys::RunSignalHandlers() (/usr/bin/../lib/libLLVM-10.so+0x9e7a44)
 #&#8203;2 0x00007fdff8de4b99 (/usr/bin/../lib/libLLVM-10.so+0x9e7b99)
 #&#8203;3 0x00007fdff80946a0 __restore_rt (/usr/bin/../lib/libc.so.6+0x3d6a0)
 #&#8203;4 0x00007fdffdfd8143 clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x7c0143)
 #&#8203;5 0x00007fdffdfdfe74 clang::Preprocessor::HandleDirective(clang::Token&) (/usr/bin/../lib/libclang-cpp.so.10+0x7c7e74)
 #&#8203;6 0x00007fdffdfa5af2 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x78daf2)
 #&#8203;7 0x00007fdffe014cc0 clang::Preprocessor::Lex(clang::Token&) (/usr/bin/../lib/libclang-cpp.so.10+0x7fccc0)
 #&#8203;8 0x00007fdffe01b85f clang::ParseAST(clang::Sema&, bool, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x80385f)
 #&#8203;9 0x00007fdfff8a90a9 clang::FrontendAction::Execute() (/usr/bin/../lib/libclang-cpp.so.10+0x20910a9)
#&#8203;10 0x00007fdfff85da94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/bin/../lib/libclang-cpp.so.10+0x2045a94)
#&#8203;11 0x00007fdfff92d832 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/../lib/libclang-cpp.so.10+0x2115832)
#&#8203;12 0x000055a8ec3f97ed cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0x127ed)
#&#8203;13 0x000055a8ec3f6ffc (/usr/bin/clang+++0xfffc)
#&#8203;14 0x000055a8ec3f3525 main (/usr/bin/clang+++0xc525)
#&#8203;15 0x00007fdff807f152 __libc_start_main (/usr/bin/../lib/libc.so.6+0x28152)
#&#8203;16 0x000055a8ec3f696e _start (/usr/bin/clang+++0xf96e)


A short investigation shows that in Preprocessor::EnterMacro(), a null CurLexer gets pushed on the stack via PushIncludeMacroStack(). After the next call to PopIncludeMacroStack(), this null Lexer is being used in Preprocessor::HandleIfDirective(), causing a segfault.

## Comments

### Comment 1 - zyn0217

https://godbolt.org/z/9MboEMTsh

This has been fixed since 13.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Christian Kandeler (ckandeler)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [47851](https://llvm.org/bz47851) |
| Version | trunk |
| OS | Linux |
| Attachments | [example source file](https://user-images.githubusercontent.com/93435768/143761309-672480a2-385a-43b0-a338-7de228aa14da.gz) |
| CC | @<!-- -->ZequanWu |

## Extended Description 


Consider this simple piece of source code:

#define MY_MACRO() 0
#if MY_MACRO(

If you try to complete at the very end, you get a lexer crash:

$ clang++ -cc1 -xc -fsyntax-only -code-completion-at crash.cpp:2:14 crash.cpp
Stack dump:
0.	Program arguments: /usr/bin/clang++ -cc1 -xc -fsyntax-only -code-completion-at crash.cpp:2:14 crash.cpp -fcolor-diagnostics 
1.	crash.cpp:2:2: current parser token 'if'
 #&amp;#<!-- -->8203;0 0x00007fdff8de6eeb llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) (/usr/bin/../lib/libLLVM-10.so+0x9e9eeb)
 #&amp;#<!-- -->8203;1 0x00007fdff8de4a44 llvm::sys::RunSignalHandlers() (/usr/bin/../lib/libLLVM-10.so+0x9e7a44)
 #&amp;#<!-- -->8203;2 0x00007fdff8de4b99 (/usr/bin/../lib/libLLVM-10.so+0x9e7b99)
 #&amp;#<!-- -->8203;3 0x00007fdff80946a0 __restore_rt (/usr/bin/../lib/libc.so.6+0x3d6a0)
 #&amp;#<!-- -->8203;4 0x00007fdffdfd8143 clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x7c0143)
 #&amp;#<!-- -->8203;5 0x00007fdffdfdfe74 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/usr/bin/../lib/libclang-cpp.so.10+0x7c7e74)
 #&amp;#<!-- -->8203;6 0x00007fdffdfa5af2 clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x78daf2)
 #&amp;#<!-- -->8203;7 0x00007fdffe014cc0 clang::Preprocessor::Lex(clang::Token&amp;) (/usr/bin/../lib/libclang-cpp.so.10+0x7fccc0)
 #&amp;#<!-- -->8203;8 0x00007fdffe01b85f clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/bin/../lib/libclang-cpp.so.10+0x80385f)
 #&amp;#<!-- -->8203;9 0x00007fdfff8a90a9 clang::FrontendAction::Execute() (/usr/bin/../lib/libclang-cpp.so.10+0x20910a9)
#&amp;#<!-- -->8203;10 0x00007fdfff85da94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/bin/../lib/libclang-cpp.so.10+0x2045a94)
#&amp;#<!-- -->8203;11 0x00007fdfff92d832 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/../lib/libclang-cpp.so.10+0x2115832)
#&amp;#<!-- -->8203;12 0x000055a8ec3f97ed cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x127ed)
#&amp;#<!-- -->8203;13 0x000055a8ec3f6ffc (/usr/bin/clang+++0xfffc)
#&amp;#<!-- -->8203;14 0x000055a8ec3f3525 main (/usr/bin/clang+++0xc525)
#&amp;#<!-- -->8203;15 0x00007fdff807f152 __libc_start_main (/usr/bin/../lib/libc.so.6+0x28152)
#&amp;#<!-- -->8203;16 0x000055a8ec3f696e _start (/usr/bin/clang+++0xf96e)


A short investigation shows that in Preprocessor::EnterMacro(), a null CurLexer gets pushed on the stack via PushIncludeMacroStack(). After the next call to PopIncludeMacroStack(), this null Lexer is being used in Preprocessor::HandleIfDirective(), causing a segfault.
</details>


---

