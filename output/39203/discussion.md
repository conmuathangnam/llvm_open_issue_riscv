# limit length of pack that `__make_integer_seq` will produce

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/39203

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [39856](https://llvm.org/bz39856) |
| Version | trunk |
| OS | Linux |
| Attachments | [Code](https://user-images.githubusercontent.com/60944935/143758407-9ef70cbd-be30-4524-9bbd-e7a293055c09.gz), [Parameters](https://user-images.githubusercontent.com/60944935/143758408-78085312-7618-439e-9159-c7170b6725cc.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @AaronBallman,@DougGregor,@DimitryAndric,@erichkeane,@zygoloid,@RippeR37 |

## Extended Description 


Code:
```cpp
#include <array>
#include <thread>
#include <tuple>
#include <utility>

template <class... Functions, class... Args, std::size_t... Function_indexes>
void run_asynchronous_helper(std::index_sequence<Function_indexes...>, std::tuple<Functions...> &&functions, Args &... args) {
        std::array<std::thread, sizeof...(Functions) - 1> threads = {std::thread{std::get<Function_indexes + 1>(functions), args...}...};
        std::get<0>(functions)(args...);
        for (auto &thread : threads) {
                thread.join();
        }
}

template <class... Functions, class... Args>
void run_asynchronous(std::tuple<Functions...> &&functions, Args &... args) {
        run_asynchronous_helper(std::make_index_sequence<sizeof...(Functions) - 1>{}, std::move(functions), args...);
}

int main() {
        //run_asynchronous(std::forward_as_tuple());
}
```
Output:
```
LLVM ERROR: out of memory
Stack dump:
0.      Program arguments: /home/toeger/Projects/llvm/build/bin/clang-7 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name main.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -resource-dir /home/toeger/Projects/llvm/build/lib/clang/7.0.1 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/x86_64-linux-gnu/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/x86_64-linux-gnu/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8/backward -internal-isystem /usr/local/include -internal-isystem /home/toeger/Projects/llvm/build/lib/clang/7.0.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++17 -fdeprecated-macro -fdebug-compilation-dir /home/toeger/Projects/llvm/build/bin -ferror-limit 19 -fmessage-length 190 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -o /tmp/main-dbeaff.o -x c++ /home/toeger/qt-projects/test/main.cpp -faddrsig 
1.      <eof> parser at end of file
2.      /home/toeger/qt-projects/test/main.cpp:16:6: instantiating function definition 'run_asynchronous<>'
#0 0x0000558099f48324 llvm::sys::PrintStackTrace(llvm::raw_ostream&) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:490:22
#1 0x0000558099f483b7 PrintStackTraceSignalHandler(void*) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:554:1
#2 0x0000558099f463df llvm::sys::RunSignalHandlers() /tmp/llvm/build/../lib/Support/Signals.cpp:67:20
#3 0x0000558099f47dea SignalHandler(int) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:353:1
#4 0x00007fbb784a68e0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x128e0)
#5 0x00007fbb77d7ef3b gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x35f3b)
#6 0x00007fbb77d802f1 abort (/lib/x86_64-linux-gnu/libc.so.6+0x372f1)
#7 0x0000558099ea6237 out_of_memory_new_handler() /tmp/llvm/build/../lib/Support/ErrorHandling.cpp:186:41
#8 0x0000558097cd1b13 llvm::safe_realloc(void*, unsigned long) /tmp/llvm/build/../include/llvm/Support/MemAlloc.h:45:10
#9 0x0000558099eba567 llvm::SmallVectorBase::grow_pod(void*, unsigned long, unsigned long) /tmp/llvm/build/../lib/Support/SmallVector.cpp:61:27
#10 0x000055809c28cbb6 llvm::SmallVectorTemplateCommon<clang::TemplateArgumentLoc, void>::grow_pod(unsigned long, unsigned long) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:100:3
#11 0x000055809c27ae02 llvm::SmallVectorTemplateBase<clang::TemplateArgumentLoc, true>::grow(unsigned long) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:307:71
#12 0x000055809c269a96 llvm::SmallVectorTemplateBase<clang::TemplateArgumentLoc, true>::push_back(clang::TemplateArgumentLoc const&) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:313:11
#13 0x000055809c263ff5 clang::TemplateArgumentListInfo::addArgument(clang::TemplateArgumentLoc const&) /tmp/llvm/build/../tools/clang/include/clang/AST/TemplateBase.h:594:3
#14 0x000055809cca01e0 checkBuiltinTemplateIdType(clang::Sema&, clang::BuiltinTemplateDecl*, llvm::SmallVectorImpl<clang::TemplateArgument> const&, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:2950:25
#15 0x000055809cca1a62 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3277:56
#16 0x000055809ce026d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#17 0x000055809cdff02b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#18 0x000055809cdeddf9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#19 0x000055809cdd775b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#20 0x000055809cdd6f94 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#21 0x000055809cdd6727 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4135:19
#22 0x000055809cdcec26 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1622:38
#23 0x000055809cca11c0 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3152:55
#24 0x000055809ce026d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#25 0x000055809cdff02b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#26 0x000055809cdeddf9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#27 0x000055809cdd775b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#28 0x000055809cdd6f94 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#29 0x000055809cdd6727 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4135:19
#30 0x000055809cdcec26 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1622:38
#31 0x000055809cca11c0 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3152:55
#32 0x000055809ce026d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#33 0x000055809cdff02b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#34 0x000055809cdeddf9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#35 0x000055809cdd775b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#36 0x000055809cded42f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6012:12
#37 0x000055809cdd7661 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:95:1
#38 0x000055809cdd6f94 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#39 0x000055809cdd6bc7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypeWithDeducedTST(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4195:28
#40 0x000055809cde0fe4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:11184:19
#41 0x000055809cdd5934 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:317:1
#42 0x000055809cdda3d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3365:43
#43 0x000055809cddaf68 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3512:14
#44 0x000055809cde14ed clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:9277:3
#45 0x000055809cdd598a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:329:1
#46 0x000055809cdda17e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3287:18
#47 0x000055809cdfd43a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6489:16
#48 0x000055809cdf1cef clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6477:1
#49 0x000055809cdd94bd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:79:1
#50 0x000055809cdd2b3c clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2781:38
#51 0x000055809ce238af clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:3965:45
#52 0x000055809ce27d26 clang::Sema::PerformPendingInstantiations(bool) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5206:33
#53 0x000055809c4c43b6 clang::Sema::ActOnEndOfTranslationUnit() /tmp/llvm/build/../tools/clang/lib/Sema/Sema.cpp:910:5
#54 0x000055809c13620c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) /tmp/llvm/build/../tools/clang/lib/Parse/Parser.cpp:601:12
#55 0x000055809c132052 clang::ParseAST(clang::Sema&, bool, bool) /tmp/llvm/build/../tools/clang/lib/Parse/ParseAST.cpp:158:35
#56 0x000055809a9180e5 clang::ASTFrontendAction::ExecuteAction() /tmp/llvm/build/../tools/clang/lib/Frontend/FrontendAction.cpp:1011:11
#57 0x000055809ae06002 clang::CodeGenAction::ExecuteAction() /tmp/llvm/build/../tools/clang/lib/CodeGen/CodeGenAction.cpp:1045:1
#58 0x000055809a917b0f clang::FrontendAction::Execute() /tmp/llvm/build/../tools/clang/lib/Frontend/FrontendAction.cpp:914:38
#59 0x000055809a8ab4db clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /tmp/llvm/build/../tools/clang/lib/Frontend/CompilerInstance.cpp:962:24
#60 0x000055809aa66e9a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /tmp/llvm/build/../tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:257:38
#61 0x0000558097c6d059 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /tmp/llvm/build/../tools/clang/tools/driver/cc1_main.cpp:218:38
#62 0x0000558097c62b59 ExecuteCC1Tool(llvm::ArrayRef<char const*>, llvm::StringRef) /tmp/llvm/build/../tools/clang/tools/driver/driver.cpp:310:64
#63 0x0000558097c637b5 main /tmp/llvm/build/../tools/clang/tools/driver/driver.cpp:382:26
#64 0x00007fbb77d6bb17 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x22b17)
#65 0x0000558097c6071a _start (/home/toeger/Projects/llvm/build/bin/clang-7+0x1eb471a)
clang-7: error: unable to execute command: Aborted
clang-7: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 7.0.1 (https://github.com/llvm-mirror/clang e44b4e4240e03c24f5479f81235519493823c27b) (https://github.com/llvm-mirror/llvm 08f7943cf05b989e05297fea48e2464f5af11bf1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/toeger/Projects/llvm/build/bin/.
clang-7: note: diagnostic msg: PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
clang-7: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-7: note: diagnostic msg: /tmp/main-841c9d.cpp
clang-7: note: diagnostic msg: /tmp/main-841c9d.sh
clang-7: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - AaronBallman

mentioned in issue llvm/llvm-bugzilla-archive#48246

---

### Comment 2 - erichkeane

I had a patch to do this: https://reviews.llvm.org/D112659

However, the response was a request to instead implement a more general error (a limit on pack-size or template arg list size).  I wasn't able to come up with a good place to put that with a bit of looking, so I'll have to come back to this later.

---

### Comment 3 - AaronBallman

*** Bug llvm/llvm-bugzilla-archive#48246 has been marked as a duplicate of this bug. ***

---

### Comment 4 - llvmbot

Minimal repro:

#include <utility>
using boom = std::make_index_sequence<-1ull>;

---

### Comment 5 - DimitryAndric

Tried this on a relatively beefy machine, and it quickly grew to >60 GiB memory, before I killed it.  There's definitely something exponential going on here. :)

---

### Comment 6 - llvmbot

Preprocessed code: https://gist.github.com/Toeger/9b1cfdbd66bb18824608509c973c2485

---

### Comment 7 - zyn0217

@erichkeane I think we should cap the limit of the NTTP pack size to `2**16`, rather than `2**24` or some other number that the user could tweak through a flag. As noted in #119600, we might hit the integer overflow when instantiating a template that takes such builtin templates as a parameter, because the 16-bit `PackIndex` in `SubstNonTypeTemplateParmExpr` is the shortcoming.

---

### Comment 8 - erichkeane

That is effectively what Richard suggests in my patch, though he wants a more general solution.  At the time I didn't find a good place to put this diagnostic into place (max size of a pack), but if you come up with one, I'm willing to review it.

---

### Comment 9 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [39856](https://llvm.org/bz39856) |
| Version | trunk |
| OS | Linux |
| Attachments | [Code](https://user-images.githubusercontent.com/60944935/143758407-9ef70cbd-be30-4524-9bbd-e7a293055c09.gz), [Parameters](https://user-images.githubusercontent.com/60944935/143758408-78085312-7618-439e-9159-c7170b6725cc.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->AaronBallman,@<!-- -->DougGregor,@<!-- -->DimitryAndric,@<!-- -->erichkeane,@<!-- -->zygoloid,@<!-- -->RippeR37 |

## Extended Description 


Code:
```cpp
#include &lt;array&gt;
#include &lt;thread&gt;
#include &lt;tuple&gt;
#include &lt;utility&gt;

template &lt;class... Functions, class... Args, std::size_t... Function_indexes&gt;
void run_asynchronous_helper(std::index_sequence&lt;Function_indexes...&gt;, std::tuple&lt;Functions...&gt; &amp;&amp;functions, Args &amp;... args) {
        std::array&lt;std::thread, sizeof...(Functions) - 1&gt; threads = {std::thread{std::get&lt;Function_indexes + 1&gt;(functions), args...}...};
        std::get&lt;0&gt;(functions)(args...);
        for (auto &amp;thread : threads) {
                thread.join();
        }
}

template &lt;class... Functions, class... Args&gt;
void run_asynchronous(std::tuple&lt;Functions...&gt; &amp;&amp;functions, Args &amp;... args) {
        run_asynchronous_helper(std::make_index_sequence&lt;sizeof...(Functions) - 1&gt;{}, std::move(functions), args...);
}

int main() {
        //run_asynchronous(std::forward_as_tuple());
}
```
Output:
```
LLVM ERROR: out of memory
Stack dump:
0.      Program arguments: /home/toeger/Projects/llvm/build/bin/clang-7 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name main.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -resource-dir /home/toeger/Projects/llvm/build/lib/clang/7.0.1 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/x86_64-linux-gnu/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/x86_64-linux-gnu/c++/8 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8/backward -internal-isystem /usr/local/include -internal-isystem /home/toeger/Projects/llvm/build/lib/clang/7.0.1/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++17 -fdeprecated-macro -fdebug-compilation-dir /home/toeger/Projects/llvm/build/bin -ferror-limit 19 -fmessage-length 190 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -o /tmp/main-dbeaff.o -x c++ /home/toeger/qt-projects/test/main.cpp -faddrsig 
1.      &lt;eof&gt; parser at end of file
2.      /home/toeger/qt-projects/test/main.cpp:16:6: instantiating function definition 'run_asynchronous&lt;&gt;'
#<!-- -->0 0x0000558099f48324 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:490:22
#<!-- -->1 0x0000558099f483b7 PrintStackTraceSignalHandler(void*) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:554:1
#<!-- -->2 0x0000558099f463df llvm::sys::RunSignalHandlers() /tmp/llvm/build/../lib/Support/Signals.cpp:67:20
#<!-- -->3 0x0000558099f47dea SignalHandler(int) /tmp/llvm/build/../lib/Support/Unix/Signals.inc:353:1
#<!-- -->4 0x00007fbb784a68e0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x128e0)
#<!-- -->5 0x00007fbb77d7ef3b gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x35f3b)
#<!-- -->6 0x00007fbb77d802f1 abort (/lib/x86_64-linux-gnu/libc.so.6+0x372f1)
#<!-- -->7 0x0000558099ea6237 out_of_memory_new_handler() /tmp/llvm/build/../lib/Support/ErrorHandling.cpp:186:41
#<!-- -->8 0x0000558097cd1b13 llvm::safe_realloc(void*, unsigned long) /tmp/llvm/build/../include/llvm/Support/MemAlloc.h:45:10
#<!-- -->9 0x0000558099eba567 llvm::SmallVectorBase::grow_pod(void*, unsigned long, unsigned long) /tmp/llvm/build/../lib/Support/SmallVector.cpp:61:27
#<!-- -->10 0x000055809c28cbb6 llvm::SmallVectorTemplateCommon&lt;clang::TemplateArgumentLoc, void&gt;::grow_pod(unsigned long, unsigned long) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:100:3
#<!-- -->11 0x000055809c27ae02 llvm::SmallVectorTemplateBase&lt;clang::TemplateArgumentLoc, true&gt;::grow(unsigned long) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:307:71
#<!-- -->12 0x000055809c269a96 llvm::SmallVectorTemplateBase&lt;clang::TemplateArgumentLoc, true&gt;::push_back(clang::TemplateArgumentLoc const&amp;) /tmp/llvm/build/../include/llvm/ADT/SmallVector.h:313:11
#<!-- -->13 0x000055809c263ff5 clang::TemplateArgumentListInfo::addArgument(clang::TemplateArgumentLoc const&amp;) /tmp/llvm/build/../tools/clang/include/clang/AST/TemplateBase.h:594:3
#<!-- -->14 0x000055809cca01e0 checkBuiltinTemplateIdType(clang::Sema&amp;, clang::BuiltinTemplateDecl*, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt; const&amp;, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:2950:25
#<!-- -->15 0x000055809cca1a62 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3277:56
#<!-- -->16 0x000055809ce026d9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#<!-- -->17 0x000055809cdff02b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#<!-- -->18 0x000055809cdeddf9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#<!-- -->19 0x000055809cdd775b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#<!-- -->20 0x000055809cdd6f94 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#<!-- -->21 0x000055809cdd6727 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4135:19
#<!-- -->22 0x000055809cdcec26 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1622:38
#<!-- -->23 0x000055809cca11c0 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3152:55
#<!-- -->24 0x000055809ce026d9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#<!-- -->25 0x000055809cdff02b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#<!-- -->26 0x000055809cdeddf9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#<!-- -->27 0x000055809cdd775b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#<!-- -->28 0x000055809cdd6f94 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#<!-- -->29 0x000055809cdd6727 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4135:19
#<!-- -->30 0x000055809cdcec26 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1622:38
#<!-- -->31 0x000055809cca11c0 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplate.cpp:3152:55
#<!-- -->32 0x000055809ce026d9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:12556:1
#<!-- -->33 0x000055809cdff02b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5901:12
#<!-- -->34 0x000055809cdeddf9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:5771:76
#<!-- -->35 0x000055809cdd775b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:100:1
#<!-- -->36 0x000055809cded42f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6012:12
#<!-- -->37 0x000055809cdd7661 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /tmp/llvm/build/../tools/clang/include/clang/AST/TypeNodes.def:95:1
#<!-- -->38 0x000055809cdd6f94 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4156:12
#<!-- -->39 0x000055809cdd6bc7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTypeWithDeducedTST(clang::TypeSourceInfo*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:4195:28
#<!-- -->40 0x000055809cde0fe4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:11184:19
#<!-- -->41 0x000055809cdd5934 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:317:1
#<!-- -->42 0x000055809cdda3d6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3365:43
#<!-- -->43 0x000055809cddaf68 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3512:14
#<!-- -->44 0x000055809cde14ed clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:9277:3
#<!-- -->45 0x000055809cdd598a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:329:1
#<!-- -->46 0x000055809cdda17e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:3287:18
#<!-- -->47 0x000055809cdfd43a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6489:16
#<!-- -->48 0x000055809cdf1cef clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*) /tmp/llvm/build/../tools/clang/lib/Sema/TreeTransform.h:6477:1
#<!-- -->49 0x000055809cdd94bd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*) /tmp/llvm/build/tools/clang/include/clang/AST/StmtNodes.inc:79:1
#<!-- -->50 0x000055809cdd2b3c clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2781:38
#<!-- -->51 0x000055809ce238af clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:3965:45
#<!-- -->52 0x000055809ce27d26 clang::Sema::PerformPendingInstantiations(bool) /tmp/llvm/build/../tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5206:33
#<!-- -->53 0x000055809c4c43b6 clang::Sema::ActOnEndOfTranslationUnit() /tmp/llvm/build/../tools/clang/lib/Sema/Sema.cpp:910:5
#<!-- -->54 0x000055809c13620c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;) /tmp/llvm/build/../tools/clang/lib/Parse/Parser.cpp:601:12
#<!-- -->55 0x000055809c132052 clang::ParseAST(clang::Sema&amp;, bool, bool) /tmp/llvm/build/../tools/clang/lib/Parse/ParseAST.cpp:158:35
#<!-- -->56 0x000055809a9180e5 clang::ASTFrontendAction::ExecuteAction() /tmp/llvm/build/../tools/clang/lib/Frontend/FrontendAction.cpp:1011:11
#<!-- -->57 0x000055809ae06002 clang::CodeGenAction::ExecuteAction() /tmp/llvm/build/../tools/clang/lib/CodeGen/CodeGenAction.cpp:1045:1
#<!-- -->58 0x000055809a917b0f clang::FrontendAction::Execute() /tmp/llvm/build/../tools/clang/lib/Frontend/FrontendAction.cpp:914:38
#<!-- -->59 0x000055809a8ab4db clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /tmp/llvm/build/../tools/clang/lib/Frontend/CompilerInstance.cpp:962:24
#<!-- -->60 0x000055809aa66e9a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /tmp/llvm/build/../tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:257:38
#<!-- -->61 0x0000558097c6d059 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /tmp/llvm/build/../tools/clang/tools/driver/cc1_main.cpp:218:38
#<!-- -->62 0x0000558097c62b59 ExecuteCC1Tool(llvm::ArrayRef&lt;char const*&gt;, llvm::StringRef) /tmp/llvm/build/../tools/clang/tools/driver/driver.cpp:310:64
#<!-- -->63 0x0000558097c637b5 main /tmp/llvm/build/../tools/clang/tools/driver/driver.cpp:382:26
#<!-- -->64 0x00007fbb77d6bb17 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x22b17)
#<!-- -->65 0x0000558097c6071a _start (/home/toeger/Projects/llvm/build/bin/clang-7+0x1eb471a)
clang-7: error: unable to execute command: Aborted
clang-7: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 7.0.1 (https://github.com/llvm-mirror/clang e44b4e4240e03c24f5479f81235519493823c27b) (https://github.com/llvm-mirror/llvm 08f7943cf05b989e05297fea48e2464f5af11bf1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/toeger/Projects/llvm/build/bin/.
clang-7: note: diagnostic msg: PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
clang-7: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-7: note: diagnostic msg: /tmp/main-841c9d.cpp
clang-7: note: diagnostic msg: /tmp/main-841c9d.sh
clang-7: note: diagnostic msg: 

********************
```
</details>


---

### Comment 10 - shafik

>Looks like we don't crash now: https://godbolt.org/z/37KKvoP9G

>CC @erichkeane @zyn0217 

Even worse, if you comment in the commented out line, we now sigkill :) https://godbolt.org/z/E6az5a89T

---

