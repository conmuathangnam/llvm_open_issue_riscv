# [clang] Crash in `clang::Sema::CorrectTypo`

**Author:** RedNicStone
**URL:** https://github.com/llvm/llvm-project/issues/141533

## Body

I've encountered a crash in clang. I'm only reporting it here, I did not investigate it any further.
This error occurred after adding `-fmodules-cache-path` to the build command as either clang or cmake was failing to recompile modified module dependencies (deleting the module cache may temporarily fix that).

```
/usr/bin/ccache /usr/lib64/ccache/clang++  -I/home/nic/CLionProjects/ember-build/test/lib/doctest -I/home/nic/CLionProjects/ember-build/test/lib/nanobench/src/include -Wno-module-file-config-mismatch -march=native -mtune=native -fmodules -fmodules-cache-path=/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules -g -std=gnu++23 -fcolor-diagnostics -MD -MT test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -MF test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.d @test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.modmap -o test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -c /home/nic/CLionProjects/ember-build/test/core/common/format.cpp
/home/nic/CLionProjects/ember-build/test/core/common/format.cpp:8:1: fatal error: module '_Builtin_stddef' is defined in both '/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules/1ZCX7GDHMZICM/_Builtin_stddef-3A2VCGLXOAVXM.pcm' and '/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules/26PDHLN8CXSV6/_Builtin_stddef-3A2VCGLXOAVXM.pcm'
    8 | import ember.core.common.format;
      | ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -I/home/nic/CLionProjects/ember-build/test/lib/doctest -I/home/nic/CLionProjects/ember-build/test/lib/nanobench/src/include -Wno-module-file-config-mismatch -march=native -mtune=native -fmodules -fmodules-cache-path=/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules -g -std=gnu++23 -fcolor-diagnostics -MD -MT test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -MF test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.d @test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.modmap -o test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -c /home/nic/CLionProjects/ember-build/test/core/common/format.cpp
1.	/home/nic/CLionProjects/ember-build/test/core/common/format.cpp:12:1: current parser token 'void'
 #0 0x00007ff180227eea llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x227eea)
 #1 0x00007ff180224e74 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x224e74)
 #2 0x00007ff18013ec74 (/lib64/libLLVM.so.20.1+0x13ec74)
 #3 0x00007ff17fa26df0 __restore_rt (/lib64/libc.so.6+0x19df0)
 #4 0x00007ff18ae12fb7 clang::ASTIdentifierIterator::Next() (/lib64/libclang-cpp.so.20.1+0x2612fb7)
 #5 0x00007ff189b15c10 clang::Sema::makeTypoCorrectionConsumer(clang::DeclarationNameInfo const&, clang::Sema::LookupNameKind, clang::Scope*, clang::CXXScopeSpec*, clang::CorrectionCandidateCallback&, clang::DeclContext*, bool, clang::ObjCObjectPointerType const*, bool) (/lib64/libclang-cpp.so.20.1+0x1315c10)
 #6 0x00007ff189b171fe clang::Sema::CorrectTypo(clang::DeclarationNameInfo const&, clang::Sema::LookupNameKind, clang::Scope*, clang::CXXScopeSpec*, clang::CorrectionCandidateCallback&, clang::Sema::CorrectTypoKind, clang::DeclContext*, bool, clang::ObjCObjectPointerType const*, bool) (/lib64/libclang-cpp.so.20.1+0x13171fe)
 #7 0x00007ff18988d771 (/lib64/libclang-cpp.so.20.1+0x108d771)
 #8 0x00007ff18988d122 clang::Sema::ActOnUsingDirective(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::SourceLocation, clang::IdentifierInfo*, clang::ParsedAttributesView const&) (/lib64/libclang-cpp.so.20.1+0x108d122)
 #9 0x00007ff188a51882 clang::Parser::ParseUsingDirective(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&) (/lib64/libclang-cpp.so.20.1+0x251882)
#10 0x00007ff188a514fa clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&, clang::ParsedAttributes&) (/lib64/libclang-cpp.so.20.1+0x2514fa)
#11 0x00007ff188a2ab1b clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22ab1b)
#12 0x00007ff188b0ae11 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x30ae11)
#13 0x00007ff188b09796 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/lib64/libclang-cpp.so.20.1+0x309796)
#14 0x00007ff188a12bde clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212bde)
#15 0x00007ff18b11514f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291514f)
#16 0x00007ff18b078b84 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.20.1+0x2878b84)
#17 0x00007ff18b1a8cae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.20.1+0x29a8cae)
#18 0x0000563876db3687 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0xa687)
#19 0x0000563876daf727 (/usr/bin/clang+++0x6727)
#20 0x00007ff18ac192dd (/lib64/libclang-cpp.so.20.1+0x24192dd)
#21 0x00007ff18013e980 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib64/libLLVM.so.20.1+0x13e980)
#22 0x00007ff18ac18b95 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/lib64/libclang-cpp.so.20.1+0x2418b95)
#23 0x00007ff18abd4e59 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/lib64/libclang-cpp.so.20.1+0x23d4e59)
#24 0x00007ff18abd5117 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/lib64/libclang-cpp.so.20.1+0x23d5117)
#25 0x00007ff18abf7c69 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/lib64/libclang-cpp.so.20.1+0x23f7c69)
#26 0x0000563876daeb54 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang+++0x5b54)
#27 0x0000563876dbe978 main (/usr/bin/clang+++0x15978)
#28 0x00007ff17fa105f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#29 0x00007ff17fa106a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#30 0x0000563876dace05 _start (/usr/bin/clang+++0x3e05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.2 (Fedora 20.1.2-3.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/format-46a1c0.cpp
clang++: note: diagnostic msg: /tmp/format-46a1c0.cache
clang++: note: diagnostic msg: /tmp/

clang++: note: diagnostic msg: 

********************
```

Files requested as per note:
[format-46a1c0.tar.gz](https://github.com/user-attachments/files/20446167/format-46a1c0.tar.gz)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: RedNicStone (RedNicStone)

<details>
I've encountered a crash in clang. I'm only reporting it here, I did not investigate it any further.
This error occurred after adding `-fmodules-cache-path` to the build command as either clang or cmake was failing to recompile modified module dependencies (deleting the module cache may temporarily fix that).

```
/usr/bin/ccache /usr/lib64/ccache/clang++  -I/home/nic/CLionProjects/ember-build/test/lib/doctest -I/home/nic/CLionProjects/ember-build/test/lib/nanobench/src/include -Wno-module-file-config-mismatch -march=native -mtune=native -fmodules -fmodules-cache-path=/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules -g -std=gnu++23 -fcolor-diagnostics -MD -MT test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -MF test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.d @<!-- -->test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.modmap -o test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -c /home/nic/CLionProjects/ember-build/test/core/common/format.cpp
/home/nic/CLionProjects/ember-build/test/core/common/format.cpp:8:1: fatal error: module '_Builtin_stddef' is defined in both '/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules/1ZCX7GDHMZICM/_Builtin_stddef-3A2VCGLXOAVXM.pcm' and '/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules/26PDHLN8CXSV6/_Builtin_stddef-3A2VCGLXOAVXM.pcm'
    8 | import ember.core.common.format;
      | ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -I/home/nic/CLionProjects/ember-build/test/lib/doctest -I/home/nic/CLionProjects/ember-build/test/lib/nanobench/src/include -Wno-module-file-config-mismatch -march=native -mtune=native -fmodules -fmodules-cache-path=/home/nic/CLionProjects/ember-build/cmake-build-debug-clang-modules -g -std=gnu++23 -fcolor-diagnostics -MD -MT test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -MF test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.d @<!-- -->test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o.modmap -o test/CMakeFiles/test-common-format.dir/core/common/format.cpp.o -c /home/nic/CLionProjects/ember-build/test/core/common/format.cpp
1.	/home/nic/CLionProjects/ember-build/test/core/common/format.cpp:12:1: current parser token 'void'
 #<!-- -->0 0x00007ff180227eea llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x227eea)
 #<!-- -->1 0x00007ff180224e74 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x224e74)
 #<!-- -->2 0x00007ff18013ec74 (/lib64/libLLVM.so.20.1+0x13ec74)
 #<!-- -->3 0x00007ff17fa26df0 __restore_rt (/lib64/libc.so.6+0x19df0)
 #<!-- -->4 0x00007ff18ae12fb7 clang::ASTIdentifierIterator::Next() (/lib64/libclang-cpp.so.20.1+0x2612fb7)
 #<!-- -->5 0x00007ff189b15c10 clang::Sema::makeTypoCorrectionConsumer(clang::DeclarationNameInfo const&amp;, clang::Sema::LookupNameKind, clang::Scope*, clang::CXXScopeSpec*, clang::CorrectionCandidateCallback&amp;, clang::DeclContext*, bool, clang::ObjCObjectPointerType const*, bool) (/lib64/libclang-cpp.so.20.1+0x1315c10)
 #<!-- -->6 0x00007ff189b171fe clang::Sema::CorrectTypo(clang::DeclarationNameInfo const&amp;, clang::Sema::LookupNameKind, clang::Scope*, clang::CXXScopeSpec*, clang::CorrectionCandidateCallback&amp;, clang::Sema::CorrectTypoKind, clang::DeclContext*, bool, clang::ObjCObjectPointerType const*, bool) (/lib64/libclang-cpp.so.20.1+0x13171fe)
 #<!-- -->7 0x00007ff18988d771 (/lib64/libclang-cpp.so.20.1+0x108d771)
 #<!-- -->8 0x00007ff18988d122 clang::Sema::ActOnUsingDirective(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::IdentifierInfo*, clang::ParsedAttributesView const&amp;) (/lib64/libclang-cpp.so.20.1+0x108d122)
 #<!-- -->9 0x00007ff188a51882 clang::Parser::ParseUsingDirective(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x251882)
#<!-- -->10 0x00007ff188a514fa clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x2514fa)
#<!-- -->11 0x00007ff188a2ab1b clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22ab1b)
#<!-- -->12 0x00007ff188b0ae11 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x30ae11)
#<!-- -->13 0x00007ff188b09796 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib64/libclang-cpp.so.20.1+0x309796)
#<!-- -->14 0x00007ff188a12bde clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212bde)
#<!-- -->15 0x00007ff18b11514f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291514f)
#<!-- -->16 0x00007ff18b078b84 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.20.1+0x2878b84)
#<!-- -->17 0x00007ff18b1a8cae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.20.1+0x29a8cae)
#<!-- -->18 0x0000563876db3687 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0xa687)
#<!-- -->19 0x0000563876daf727 (/usr/bin/clang+++0x6727)
#<!-- -->20 0x00007ff18ac192dd (/lib64/libclang-cpp.so.20.1+0x24192dd)
#<!-- -->21 0x00007ff18013e980 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/lib64/libLLVM.so.20.1+0x13e980)
#<!-- -->22 0x00007ff18ac18b95 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/lib64/libclang-cpp.so.20.1+0x2418b95)
#<!-- -->23 0x00007ff18abd4e59 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/lib64/libclang-cpp.so.20.1+0x23d4e59)
#<!-- -->24 0x00007ff18abd5117 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/lib64/libclang-cpp.so.20.1+0x23d5117)
#<!-- -->25 0x00007ff18abf7c69 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/lib64/libclang-cpp.so.20.1+0x23f7c69)
#<!-- -->26 0x0000563876daeb54 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0x5b54)
#<!-- -->27 0x0000563876dbe978 main (/usr/bin/clang+++0x15978)
#<!-- -->28 0x00007ff17fa105f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#<!-- -->29 0x00007ff17fa106a8 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#<!-- -->30 0x0000563876dace05 _start (/usr/bin/clang+++0x3e05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.2 (Fedora 20.1.2-3.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/format-46a1c0.cpp
clang++: note: diagnostic msg: /tmp/format-46a1c0.cache
clang++: note: diagnostic msg: /tmp/

clang++: note: diagnostic msg: 

********************
```

Files requested as per note:
[format-46a1c0.tar.gz](https://github.com/user-attachments/files/20446167/format-46a1c0.tar.gz)
</details>


---

### Comment 2 - ssorensen

I hit something similar on main@936bf29dda64ee33977c4ff687f21f158dc11740. Reduced repo sample:
```
void f() {
    using enum foo::bar;
}
```
Triggers this assertion:
```
clang++: /home/ssorensen/sources/llvm-project/clang/lib/Sema/SemaDeclCXX.cpp:12627: clang::Decl* clang::Sema::ActOnUsingEnumDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, clang::SourceLocation, clang::SourceRange, const clang::IdentifierInfo&, clang::ParsedType, clang::CXXScopeSpec*): Asser
tion `SS && !SS->isInvalid() && "ScopeSpec is invalid"' failed.
```

---

