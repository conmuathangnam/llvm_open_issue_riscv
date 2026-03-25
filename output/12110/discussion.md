# clang segfaults when file changes

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/12110

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [11738](https://llvm.org/bz11738) |
| Version | trunk |
| OS | All |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor |

## Extended Description 
The following bug is probably very stupid, and I don't mind if it gets closed, but I thought I would report it anyway.

Take any reasonable sized C++ file, call it `X.cc` (I preprocessed a boost file to get a ~2M file)

Execute the following command:
```
for i in `seq 10000`; do cat X.cc > t.cc; done &
```
While that is spinning in the background, type:
```
clang++ t.cc -c
```
A few times, and you are likely to see a crash like the one before. I am fairly sure this is not from clang++ just seeing a truncated file, as I have tried to truncate the file I was using in many places, and have not seen clang crash (I actually found this bug when my fuzzer started misbehaving and changing the file while clang was compiling. I get a segfault like the one below.

Note the files mentioned at the bottom, writen to `/tmp`, compile fine (well, often with errors, but no segfaults).
```
0  clang           0x0000000001b5f98f
1  clang           0x0000000001b5fe19
2  libpthread.so.0 0x00007f93be567060
3  clang           0x0000000000fbd1ac
4  clang           0x0000000000fc1af2 clang::SourceManager::getLineNumber(clang::FileID, unsigned int, bool*) const + 482
5  clang           0x0000000000fc2862 clang::SourceManager::getPresumedLoc(clang::SourceLocation) const + 450
6  clang           0x0000000000f78283 clang::Preprocessor::HandleDigitDirective(clang::Token&) + 2739
7  clang           0x0000000000f7dd33 clang::Preprocessor::HandleDirective(clang::Token&) + 1411
8  clang           0x0000000000f60128 clang::Lexer::LexTokenInternal(clang::Token&) + 2104
9  clang           0x0000000000f6181f clang::Lexer::LexTokenInternal(clang::Token&) + 7983
10 clang           0x0000000000882a38
11 clang           0x000000000087c73a clang::Parser::ExpectAndConsume(clang::tok::TokenKind, unsigned int, char const*, clang::tok::TokenKind) + 762
12 clang           0x00000000008905ee clang::Parser::ParseDeclGroup(clang::Parser::ParsingDeclSpec&, unsigned int, bool, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 1886
13 clang           0x000000000089695c clang::Parser::ParseSimpleDeclaration(clang::ASTOwningVector<clang::Stmt*, 32u>&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*) + 652
14 clang           0x0000000000896b02 clang::Parser::ParseDeclaration(clang::ASTOwningVector<clang::Stmt*, 32u>&, unsigned int, clang::SourceLocation&, clang::Parser::ParsedAttributesWithRange&) + 178
15 clang           0x000000000087eb4d clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::Parser::ParsingDeclSpec*) + 893
16 clang           0x000000000087f628 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) + 168
17 clang           0x00000000008543cd clang::ParseAST(clang::Sema&, bool) + 269
18 clang           0x000000000060101d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 317
19 clang           0x00000000005e7a1a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 1114
20 clang           0x00000000005dc4b2 cc1_main(char const**, char const**, char const*, void*) + 866
21 clang           0x00000000005e62be main + 702
22 libc.so.6       0x00007f93bd83430d __libc_start_main + 237
23 clang           0x00000000005dbff9
Stack dump:
0.	Program arguments: /scratch/dist/clang/bin/clang -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name t.cc -mrelocation-model static -mdisable-fp-elim -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.21.53.20110810 -momit-leaf-frame-pointer -coverage-file t.o -resource-dir /scratch/dist/clang/bin/../lib/clang/3.1 -fmodule-cache-path /var/tmp/clang-module-cache -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6/x86_64-linux-gnu -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6/backward -internal-isystem /usr/local/include -internal-isystem /scratch/dist/clang/bin/../lib/clang/3.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/caj/temp -ferror-limit 19 -fmessage-length 136 -mstackrealign -fgnu-runtime -fobjc-runtime-has-arc -fobjc-runtime-has-weak -fobjc-fragile-abi -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -o t.o -x c++ t.cc 
1.	clang: error: unable to execute command: Bus error
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang: note: diagnostic msg: Please submit a bug report to http://llvm.org/bugs/ and include command line arguments and all diagnostic information.
clang: note: diagnostic msg: Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/t-VkNgCT.ii
clang: note: diagnostic msg: /tmp/t-VkNgCT.sh
```

## Comments

### Comment 1 - llvmbot

This still fails in clang 3.6

---

### Comment 2 - DougGregor

Clang is supposed to be able to recover from such problems, so we certainly shouldn't close this bug.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [11738](https://llvm.org/bz11738) |
| Version | trunk |
| OS | All |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor |

## Extended Description 
The following bug is probably very stupid, and I don't mind if it gets closed, but I thought I would report it anyway.

Take any reasonable sized C++ file, call it `X.cc` (I preprocessed a boost file to get a ~2M file)

Execute the following command:
```
for i in `seq 10000`; do cat X.cc &gt; t.cc; done &amp;
```
While that is spinning in the background, type:
```
clang++ t.cc -c
```
A few times, and you are likely to see a crash like the one before. I am fairly sure this is not from clang++ just seeing a truncated file, as I have tried to truncate the file I was using in many places, and have not seen clang crash (I actually found this bug when my fuzzer started misbehaving and changing the file while clang was compiling. I get a segfault like the one below.

Note the files mentioned at the bottom, writen to `/tmp`, compile fine (well, often with errors, but no segfaults).
```
0  clang           0x0000000001b5f98f
1  clang           0x0000000001b5fe19
2  libpthread.so.0 0x00007f93be567060
3  clang           0x0000000000fbd1ac
4  clang           0x0000000000fc1af2 clang::SourceManager::getLineNumber(clang::FileID, unsigned int, bool*) const + 482
5  clang           0x0000000000fc2862 clang::SourceManager::getPresumedLoc(clang::SourceLocation) const + 450
6  clang           0x0000000000f78283 clang::Preprocessor::HandleDigitDirective(clang::Token&amp;) + 2739
7  clang           0x0000000000f7dd33 clang::Preprocessor::HandleDirective(clang::Token&amp;) + 1411
8  clang           0x0000000000f60128 clang::Lexer::LexTokenInternal(clang::Token&amp;) + 2104
9  clang           0x0000000000f6181f clang::Lexer::LexTokenInternal(clang::Token&amp;) + 7983
10 clang           0x0000000000882a38
11 clang           0x000000000087c73a clang::Parser::ExpectAndConsume(clang::tok::TokenKind, unsigned int, char const*, clang::tok::TokenKind) + 762
12 clang           0x00000000008905ee clang::Parser::ParseDeclGroup(clang::Parser::ParsingDeclSpec&amp;, unsigned int, bool, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 1886
13 clang           0x000000000089695c clang::Parser::ParseSimpleDeclaration(clang::ASTOwningVector&lt;clang::Stmt*, 32u&gt;&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*) + 652
14 clang           0x0000000000896b02 clang::Parser::ParseDeclaration(clang::ASTOwningVector&lt;clang::Stmt*, 32u&gt;&amp;, unsigned int, clang::SourceLocation&amp;, clang::Parser::ParsedAttributesWithRange&amp;) + 178
15 clang           0x000000000087eb4d clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&amp;, clang::Parser::ParsingDeclSpec*) + 893
16 clang           0x000000000087f628 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;) + 168
17 clang           0x00000000008543cd clang::ParseAST(clang::Sema&amp;, bool) + 269
18 clang           0x000000000060101d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 317
19 clang           0x00000000005e7a1a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 1114
20 clang           0x00000000005dc4b2 cc1_main(char const**, char const**, char const*, void*) + 866
21 clang           0x00000000005e62be main + 702
22 libc.so.6       0x00007f93bd83430d __libc_start_main + 237
23 clang           0x00000000005dbff9
Stack dump:
0.	Program arguments: /scratch/dist/clang/bin/clang -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name t.cc -mrelocation-model static -mdisable-fp-elim -masm-verbose -mconstructor-aliases -munwind-tables -target-cpu x86-64 -target-linker-version 2.21.53.20110810 -momit-leaf-frame-pointer -coverage-file t.o -resource-dir /scratch/dist/clang/bin/../lib/clang/3.1 -fmodule-cache-path /var/tmp/clang-module-cache -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6/x86_64-linux-gnu -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../include/c++/4.6/backward -internal-isystem /usr/local/include -internal-isystem /scratch/dist/clang/bin/../lib/clang/3.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/caj/temp -ferror-limit 19 -fmessage-length 136 -mstackrealign -fgnu-runtime -fobjc-runtime-has-arc -fobjc-runtime-has-weak -fobjc-fragile-abi -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -o t.o -x c++ t.cc 
1.	clang: error: unable to execute command: Bus error
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang: note: diagnostic msg: Please submit a bug report to http://llvm.org/bugs/ and include command line arguments and all diagnostic information.
clang: note: diagnostic msg: Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/t-VkNgCT.ii
clang: note: diagnostic msg: /tmp/t-VkNgCT.sh
```
</details>


---

### Comment 4 - Endilll

I ran into this myself several times recently.

---

