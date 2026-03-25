# Crash during lexing process

**Author:** solonovamax
**URL:** https://github.com/llvm/llvm-project/issues/182939

## Body

This issue may be related to #126356, however I'm not to sure. Usually I'd spend more time looking for similar issues to make sure I'm not opening a duplicate, however there are way too many issues on this repo and this is quite out of my depth.

here's what clang crashes with:
```
Stack dump:
0.	Program arguments: /nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang++ @/tmp/cc-params.txr1Tr
1.	<unknown> parser at unknown location
2.	/home/solonovamax/Programming/C++/ros2-robot-body-filter/robot_body_filter/src/robot_body_filter.cpp:21:1: parsing namespace 'robot_body_filter'
 #0 0x00007f63f057e91c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x117e91c)
 #1 0x00007f63f057c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x117c6a4)
 #2 0x00007f63f0469568 CrashRecoverySignalHandler(int) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x1069568)
 #3 0x00007f63eec40f30 __restore_rt (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x40f30)
 #4 0x00007f63fa78213c clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xd8213c)
 #5 0x00007f63fa7879ee clang::Lexer::LexTokenInternal(clang::Token&, bool) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xd879ee)
 #6 0x00007f63fa82597c clang::Preprocessor::Lex(clang::Token&) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe2597c)
 #7 0x00007f63fa840beb clang::Parser::ConsumeBrace() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe40beb)
 #8 0x00007f63fa84b5c2 clang::BalancedDelimiterTracker::consumeClose() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe4b5c2)
 #9 0x00007f63fa887049 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe87049)
#10 0x00007f63fa869f72 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe69f72)
#11 0x00007f63fa95d2a2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xf5d2a2)
#12 0x00007f63fa95e085 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xf5e085)
#13 0x00007f63fa82ff0a clang::ParseAST(clang::Sema&, bool, bool) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe2ff0a)
#14 0x00007f63fce49a38 clang::FrontendAction::Execute() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x3449a38)
#15 0x00007f63fcdbd4bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x33bd4bb)
#16 0x00007f63fcee9f56 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x34e9f56)
#17 0x000000000041669a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x41669a)
#18 0x000000000040e903 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e903)
#19 0x00007f63fc95ec99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f5ec99)
#20 0x00007f63f0469993 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x1069993)
#21 0x00007f63fc95f745 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f5f745)
#22 0x00007f63fc91eaec clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f1eaec)
#23 0x00007f63fc91fab2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f1fab2)
#24 0x00007f63fc93505c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f3505c)
#25 0x0000000000413124 clang_main(int, char**, llvm::ToolContext const&) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x413124)
#26 0x000000000040e2c4 main (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e2c4)
#27 0x00007f63eec2a1fe __libc_start_call_main (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x2a1fe)
#28 0x00007f63eec2a2b9 __libc_start_main@GLIBC_2.2.5 (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x2a2b9)
#29 0x000000000040e325 _start (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e325)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.2
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin
clang++: note: diagnostic msg:
********************
```

Here are the files that it requested to have attached:
[robot_body_filter-f711bb.cpp](https://github.com/user-attachments/files/25500150/robot_body_filter-f711bb.cpp)
[robot_body_filter-f711bb.sh](https://github.com/user-attachments/files/25500154/robot_body_filter-f711bb.sh)

also, if you're using nix and wish to reproduce it with the exact environment, I've attached a tarball of the project which should allow you to do that. However, I'm unable to reproduce this after the first time I encountered it. (also included is the full log from the compilation which failed):

[llvm-lexing-crash.tar.gz](https://github.com/user-attachments/files/25500351/llvm-lexing-crash.tar.gz)

I am running the build with the following command:
```bash
colcon build --mixin compile-commands mold clang ninja rel-with-deb-info --symlink-install
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: solo (solonovamax)

<details>
This issue may be related to #<!-- -->126356, however I'm not to sure. Usually I'd spend more time looking for similar issues to make sure I'm not opening a duplicate, however there are way too many issues on this repo and this is quite out of my depth.

here's what clang crashes with:
```
Stack dump:
0.	Program arguments: /nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang++ @/tmp/cc-params.txr1Tr
1.	&lt;unknown&gt; parser at unknown location
2.	/home/solonovamax/Programming/C++/ros2-robot-body-filter/robot_body_filter/src/robot_body_filter.cpp:21:1: parsing namespace 'robot_body_filter'
 #<!-- -->0 0x00007f63f057e91c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x117e91c)
 #<!-- -->1 0x00007f63f057c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x117c6a4)
 #<!-- -->2 0x00007f63f0469568 CrashRecoverySignalHandler(int) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x1069568)
 #<!-- -->3 0x00007f63eec40f30 __restore_rt (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x40f30)
 #<!-- -->4 0x00007f63fa78213c clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xd8213c)
 #<!-- -->5 0x00007f63fa7879ee clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xd879ee)
 #<!-- -->6 0x00007f63fa82597c clang::Preprocessor::Lex(clang::Token&amp;) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe2597c)
 #<!-- -->7 0x00007f63fa840beb clang::Parser::ConsumeBrace() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe40beb)
 #<!-- -->8 0x00007f63fa84b5c2 clang::BalancedDelimiterTracker::consumeClose() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe4b5c2)
 #<!-- -->9 0x00007f63fa887049 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe87049)
#<!-- -->10 0x00007f63fa869f72 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe69f72)
#<!-- -->11 0x00007f63fa95d2a2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xf5d2a2)
#<!-- -->12 0x00007f63fa95e085 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xf5e085)
#<!-- -->13 0x00007f63fa82ff0a clang::ParseAST(clang::Sema&amp;, bool, bool) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0xe2ff0a)
#<!-- -->14 0x00007f63fce49a38 clang::FrontendAction::Execute() (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x3449a38)
#<!-- -->15 0x00007f63fcdbd4bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x33bd4bb)
#<!-- -->16 0x00007f63fcee9f56 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x34e9f56)
#<!-- -->17 0x000000000041669a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x41669a)
#<!-- -->18 0x000000000040e903 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e903)
#<!-- -->19 0x00007f63fc95ec99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f5ec99)
#<!-- -->20 0x00007f63f0469993 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/nix/store/7h4y27qa03f7inykx1ky4466l7qzvcwk-llvm-20.1.2-lib/lib/libLLVM.so.20.1+0x1069993)
#<!-- -->21 0x00007f63fc95f745 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f5f745)
#<!-- -->22 0x00007f63fc91eaec clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f1eaec)
#<!-- -->23 0x00007f63fc91fab2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f1fab2)
#<!-- -->24 0x00007f63fc93505c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/nix/store/v3xqsinpbf93fk7lkxdxfrhd9m6slfm9-clang-20.1.2-lib/lib/libclang-cpp.so.20.1+0x2f3505c)
#<!-- -->25 0x0000000000413124 clang_main(int, char**, llvm::ToolContext const&amp;) (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x413124)
#<!-- -->26 0x000000000040e2c4 main (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e2c4)
#<!-- -->27 0x00007f63eec2a1fe __libc_start_call_main (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x2a1fe)
#<!-- -->28 0x00007f63eec2a2b9 __libc_start_main@<!-- -->GLIBC_2.2.5 (/nix/store/rmy663w9p7xb202rcln4jjzmvivznmz8-glibc-2.40-66/lib/libc.so.6+0x2a2b9)
#<!-- -->29 0x000000000040e325 _start (/nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin/clang+++0x40e325)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.2
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /nix/store/ksaxqiqma06cslfy29h5199sja3kxfdk-clang-20.1.2/bin
clang++: note: diagnostic msg:
********************
```

Here are the files that it requested to have attached:
[robot_body_filter-f711bb.cpp](https://github.com/user-attachments/files/25500150/robot_body_filter-f711bb.cpp)
[robot_body_filter-f711bb.sh](https://github.com/user-attachments/files/25500154/robot_body_filter-f711bb.sh)

also, if you're using nix and wish to reproduce it with the exact environment, I've attached a tarball of the project which should allow you to do that. However, I'm unable to reproduce this after the first time I encountered it. (also included is the full log from the compilation which failed):

[llvm-lexing-crash.tar.gz](https://github.com/user-attachments/files/25500351/llvm-lexing-crash.tar.gz)

I am running the build with the following command:
```bash
colcon build --mixin compile-commands mold clang ninja rel-with-deb-info --symlink-install
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.

---

### Comment 3 - solonovamax

> Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.

sorry, I'm working on a project that uses ros2 humble, which is based off of ubuntu 22.04. so because of that, I'm stuck on a much older version of clang than I otherwise would like to use (and afaik the version of clang I'm using in my dev environment is already much newer than the one shipped with ubuntu 22.04, due to me using nix-ros-overlay)

but if you think it's been fixed in the most recent release, I can close the issue

---

