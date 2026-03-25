# [clang] Crash with spaceship operator and floating-point values

**Author:** muhamad
**URL:** https://github.com/llvm/llvm-project/issues/170015
**Status:** Closed
**Labels:** good first issue, c++20, clang:frontend, crash-on-invalid, confirmed, spaceship
**Closed Date:** 2025-12-16T11:45:32Z

## Body

using the following code with Clang21.1.0 and the upcoming Clang22.0.0, with the flags `-std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++` causes the compiler to crash.

```cpp

namespace std {
    struct partial_ordering {
        signed char value;

        constexpr partial_ordering(signed char v) : value(v) {}

        // relational operators (minimal)
        constexpr bool operator==(partial_ordering other) const { return value == other.value; }
        constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
        constexpr bool operator<(partial_ordering other) const { return value < other.value; }
        constexpr bool operator>(partial_ordering other) const { return value > other.value; }
        constexpr bool operator<=(partial_ordering other) const { return value <= other.value; }
        constexpr bool operator>=(partial_ordering other) const { return value >= other.value; }

        // static constants for convenience
        static constexpr signed char less = -1;
        static constexpr signed char equivalent = 0;
        static constexpr signed char greater = 1;
        static constexpr signed char unordered = 2;
    };
}

struct compare_result {
    signed char v; // -1, 0, 1, 2 for unordered

    template <typename T>
    constexpr compare_result(T r) {
        if (r < 0) v = -1;
        else if (r > 0) v = 1;
        else if (r == 0) v = 0;
        else v = 2; // unordered, e.g., NaN
    }

    constexpr bool operator==(const compare_result& o) const { return v == o.v; }
    constexpr bool operator<(const compare_result& o) const { return v < o.v; }
};

int main() {
    float a = 3.0f, b = 3.5f;
    compare_result r = a <=> b;
    int c = r.v;
}
````

the code compiles correctly using the same flags under GCC 15.2. I've used Compiler explorer to test it. adding the `-v` flags produce the following results

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9bae84b01718e53495abf50958abc86ea45f16bb)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
 (in-process)
 "/cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -nostdsysteminc -nobuiltininc -resource-dir /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/lib/clang/22 -std=c++23 -fdeprecated-macro -ferror-limit 19 -fno-builtin -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /app/output.s -x c++ <source>
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
#include "..." search starts here:
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++ -v <source>
1.	<source>:41:31: current parser token ';'
2.	<source>:39:12: parsing function body 'main'
3.	<source>:39:12: in compound statement ('{}')
 #0 0x0000000003cd7358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd7358)
 #1 0x0000000003cd4d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd4d2c)
 #2 0x0000000003c1ac38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000786daa842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007294195 clang::RecordDecl::field_begin() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7294195)
 #5 0x0000000007272aa2 clang::ComparisonCategoryInfo::ValueInfo::hasValidIntValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7272aa2)
 #6 0x000000000665f2e6 clang::Sema::CheckComparisonCategoryType(clang::ComparisonCategoryType, clang::SourceLocation, clang::Sema::ComparisonCategoryUsage) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x665f2e6)
 #7 0x0000000006768cb7 clang::Sema::CheckCompareOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6768cb7)
 #8 0x000000000676a294 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676a294)
 #9 0x000000000676b760 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676b760)
#10 0x000000000676bb3c clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676bb3c)
#11 0x0000000006265bfc clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6265bfc)
#12 0x0000000006290a70 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6290a70)
#13 0x000000000621d855 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621d855)
#14 0x0000000006231ffc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6231ffc)
#15 0x0000000006234a0c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234a0c)
#16 0x0000000006234e5a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234e5a)
#17 0x00000000062f15df clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f15df)
#18 0x00000000062f1f26 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f1f26)
#19 0x00000000062fb419 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fb419)
#20 0x00000000062fbeba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbeba)
#21 0x00000000061e5b4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e5b4b)
#22 0x000000000623245f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623245f)
#23 0x00000000061def8f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61def8f)
#24 0x00000000061df6c7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61df6c7)
#25 0x00000000061e809f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e809f)
#26 0x00000000061e98a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e98a7)
#27 0x00000000061c277a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c277a)
#28 0x0000000004595f65 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4595f65)
#29 0x000000000489760a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489760a)
#30 0x00000000048164eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48164eb)
#31 0x000000000498c14b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498c14b)
#32 0x0000000000dce015 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdce015)
#33 0x0000000000dc5ecb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000dc5f6d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004603299 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003c1b053 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1b053)
#37 0x00000000046034b9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000045c5e42 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c5e42)
#39 0x00000000045c6d21 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c6d21)
#40 0x00000000045cf90c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cf90c)
#41 0x0000000000dca999 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca999)
#42 0x0000000000c74c84 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c84)
#43 0x0000786daa829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x0000786daa829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dc5965 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5965)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
````

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Muhammad (muhamad)

<details>
using the following code with Clang21.1.0 and the upcoming Clang22.0.0, with the flags `-std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++` causes the compiler to crash.

```cpp

namespace std {
    struct partial_ordering {
        signed char value;

        constexpr partial_ordering(signed char v) : value(v) {}

        // relational operators (minimal)
        constexpr bool operator==(partial_ordering other) const { return value == other.value; }
        constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
        constexpr bool operator&lt;(partial_ordering other) const { return value &lt; other.value; }
        constexpr bool operator&gt;(partial_ordering other) const { return value &gt; other.value; }
        constexpr bool operator&lt;=(partial_ordering other) const { return value &lt;= other.value; }
        constexpr bool operator&gt;=(partial_ordering other) const { return value &gt;= other.value; }

        // static constants for convenience
        static constexpr signed char less = -1;
        static constexpr signed char equivalent = 0;
        static constexpr signed char greater = 1;
        static constexpr signed char unordered = 2;
    };
}

struct compare_result {
    signed char v; // -1, 0, 1, 2 for unordered

    template &lt;typename T&gt;
    constexpr compare_result(T r) {
        if (r &lt; 0) v = -1;
        else if (r &gt; 0) v = 1;
        else if (r == 0) v = 0;
        else v = 2; // unordered, e.g., NaN
    }

    constexpr bool operator==(const compare_result&amp; o) const { return v == o.v; }
    constexpr bool operator&lt;(const compare_result&amp; o) const { return v &lt; o.v; }
};

int main() {
    float a = 3.0f, b = 3.5f;
    compare_result r = a &lt;=&gt; b;
    int c = r.v;
}
````

the code compiles correctly using the same flags under GCC 15.2. I've used Compiler explorer to test it. adding the `-v` flags produce the following results

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9bae84b01718e53495abf50958abc86ea45f16bb)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
 (in-process)
 "/cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -nostdsysteminc -nobuiltininc -resource-dir /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/lib/clang/22 -std=c++23 -fdeprecated-macro -ferror-limit 19 -fno-builtin -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /app/output.s -x c++ &lt;source&gt;
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
#include "..." search starts here:
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++ -v &lt;source&gt;
1.	&lt;source&gt;:41:31: current parser token ';'
2.	&lt;source&gt;:39:12: parsing function body 'main'
3.	&lt;source&gt;:39:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003cd7358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd7358)
 #<!-- -->1 0x0000000003cd4d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd4d2c)
 #<!-- -->2 0x0000000003c1ac38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000786daa842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007294195 clang::RecordDecl::field_begin() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7294195)
 #<!-- -->5 0x0000000007272aa2 clang::ComparisonCategoryInfo::ValueInfo::hasValidIntValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7272aa2)
 #<!-- -->6 0x000000000665f2e6 clang::Sema::CheckComparisonCategoryType(clang::ComparisonCategoryType, clang::SourceLocation, clang::Sema::ComparisonCategoryUsage) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x665f2e6)
 #<!-- -->7 0x0000000006768cb7 clang::Sema::CheckCompareOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6768cb7)
 #<!-- -->8 0x000000000676a294 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676a294)
 #<!-- -->9 0x000000000676b760 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676b760)
#<!-- -->10 0x000000000676bb3c clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676bb3c)
#<!-- -->11 0x0000000006265bfc clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6265bfc)
#<!-- -->12 0x0000000006290a70 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6290a70)
#<!-- -->13 0x000000000621d855 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621d855)
#<!-- -->14 0x0000000006231ffc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6231ffc)
#<!-- -->15 0x0000000006234a0c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234a0c)
#<!-- -->16 0x0000000006234e5a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234e5a)
#<!-- -->17 0x00000000062f15df clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f15df)
#<!-- -->18 0x00000000062f1f26 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f1f26)
#<!-- -->19 0x00000000062fb419 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fb419)
#<!-- -->20 0x00000000062fbeba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbeba)
#<!-- -->21 0x00000000061e5b4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e5b4b)
#<!-- -->22 0x000000000623245f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623245f)
#<!-- -->23 0x00000000061def8f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61def8f)
#<!-- -->24 0x00000000061df6c7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61df6c7)
#<!-- -->25 0x00000000061e809f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e809f)
#<!-- -->26 0x00000000061e98a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e98a7)
#<!-- -->27 0x00000000061c277a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c277a)
#<!-- -->28 0x0000000004595f65 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4595f65)
#<!-- -->29 0x000000000489760a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489760a)
#<!-- -->30 0x00000000048164eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48164eb)
#<!-- -->31 0x000000000498c14b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498c14b)
#<!-- -->32 0x0000000000dce015 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdce015)
#<!-- -->33 0x0000000000dc5ecb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000dc5f6d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004603299 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003c1b053 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1b053)
#<!-- -->37 0x00000000046034b9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000045c5e42 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c5e42)
#<!-- -->39 0x00000000045c6d21 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c6d21)
#<!-- -->40 0x00000000045cf90c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cf90c)
#<!-- -->41 0x0000000000dca999 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca999)
#<!-- -->42 0x0000000000c74c84 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c84)
#<!-- -->43 0x0000786daa829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000786daa829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000dc5965 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5965)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
````
</details>


---

### Comment 2 - tbaederr

This also crashes with just `clang -std=c++20` for me.

We have to check if the field in `ComparisonCategoryInfo::hasValidIntValue()` is even of record type so clang doesn't crash. That will reject the example though, since the constants should be of type `partial_ordering`. GCC accepts it.

---

### Comment 3 - frederick-vs-ja

Also, this is unrelated to freestanding mode.

Reduced to ([Godbolt link](https://godbolt.org/z/xob3frhxo)):
```C++
namespace std {
  struct partial_ordering {
    signed char value;

    constexpr partial_ordering(signed char v) : value(v) {}

    // relational operators (minimal)
    constexpr bool operator==(partial_ordering other) const { return value == other.value; }
    constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
    constexpr bool operator<(partial_ordering other) const { return value < other.value; }
    constexpr bool operator>(partial_ordering other) const { return value > other.value; }
    constexpr bool operator<=(partial_ordering other) const { return value <= other.value; }
    constexpr bool operator>=(partial_ordering other) const { return value >= other.value; }

    // static constants for convenience(?)
    static constexpr signed char less = -1;
    static constexpr signed char equivalent = 0;
    static constexpr signed char greater = 1;
    static constexpr signed char unordered = -128;
  };
}

void test() {
  (void) (3.0f <=> 3.5f);
}
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

Author: Muhammad (muhamad)

<details>
using the following code with Clang21.1.0 and the upcoming Clang22.0.0, with the flags `-std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++` causes the compiler to crash.

```cpp

namespace std {
    struct partial_ordering {
        signed char value;

        constexpr partial_ordering(signed char v) : value(v) {}

        // relational operators (minimal)
        constexpr bool operator==(partial_ordering other) const { return value == other.value; }
        constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
        constexpr bool operator&lt;(partial_ordering other) const { return value &lt; other.value; }
        constexpr bool operator&gt;(partial_ordering other) const { return value &gt; other.value; }
        constexpr bool operator&lt;=(partial_ordering other) const { return value &lt;= other.value; }
        constexpr bool operator&gt;=(partial_ordering other) const { return value &gt;= other.value; }

        // static constants for convenience
        static constexpr signed char less = -1;
        static constexpr signed char equivalent = 0;
        static constexpr signed char greater = 1;
        static constexpr signed char unordered = 2;
    };
}

struct compare_result {
    signed char v; // -1, 0, 1, 2 for unordered

    template &lt;typename T&gt;
    constexpr compare_result(T r) {
        if (r &lt; 0) v = -1;
        else if (r &gt; 0) v = 1;
        else if (r == 0) v = 0;
        else v = 2; // unordered, e.g., NaN
    }

    constexpr bool operator==(const compare_result&amp; o) const { return v == o.v; }
    constexpr bool operator&lt;(const compare_result&amp; o) const { return v &lt; o.v; }
};

int main() {
    float a = 3.0f, b = 3.5f;
    compare_result r = a &lt;=&gt; b;
    int c = r.v;
}
````

the code compiles correctly using the same flags under GCC 15.2. I've used Compiler explorer to test it. adding the `-v` flags produce the following results

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9bae84b01718e53495abf50958abc86ea45f16bb)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
 (in-process)
 "/cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -nostdsysteminc -nobuiltininc -resource-dir /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/lib/clang/22 -std=c++23 -fdeprecated-macro -ferror-limit 19 -fno-builtin -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /app/output.s -x c++ &lt;source&gt;
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
#include "..." search starts here:
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++ -v &lt;source&gt;
1.	&lt;source&gt;:41:31: current parser token ';'
2.	&lt;source&gt;:39:12: parsing function body 'main'
3.	&lt;source&gt;:39:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003cd7358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd7358)
 #<!-- -->1 0x0000000003cd4d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd4d2c)
 #<!-- -->2 0x0000000003c1ac38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000786daa842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007294195 clang::RecordDecl::field_begin() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7294195)
 #<!-- -->5 0x0000000007272aa2 clang::ComparisonCategoryInfo::ValueInfo::hasValidIntValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7272aa2)
 #<!-- -->6 0x000000000665f2e6 clang::Sema::CheckComparisonCategoryType(clang::ComparisonCategoryType, clang::SourceLocation, clang::Sema::ComparisonCategoryUsage) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x665f2e6)
 #<!-- -->7 0x0000000006768cb7 clang::Sema::CheckCompareOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6768cb7)
 #<!-- -->8 0x000000000676a294 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676a294)
 #<!-- -->9 0x000000000676b760 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676b760)
#<!-- -->10 0x000000000676bb3c clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676bb3c)
#<!-- -->11 0x0000000006265bfc clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6265bfc)
#<!-- -->12 0x0000000006290a70 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6290a70)
#<!-- -->13 0x000000000621d855 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621d855)
#<!-- -->14 0x0000000006231ffc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6231ffc)
#<!-- -->15 0x0000000006234a0c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234a0c)
#<!-- -->16 0x0000000006234e5a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234e5a)
#<!-- -->17 0x00000000062f15df clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f15df)
#<!-- -->18 0x00000000062f1f26 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f1f26)
#<!-- -->19 0x00000000062fb419 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fb419)
#<!-- -->20 0x00000000062fbeba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbeba)
#<!-- -->21 0x00000000061e5b4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e5b4b)
#<!-- -->22 0x000000000623245f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623245f)
#<!-- -->23 0x00000000061def8f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61def8f)
#<!-- -->24 0x00000000061df6c7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61df6c7)
#<!-- -->25 0x00000000061e809f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e809f)
#<!-- -->26 0x00000000061e98a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e98a7)
#<!-- -->27 0x00000000061c277a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c277a)
#<!-- -->28 0x0000000004595f65 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4595f65)
#<!-- -->29 0x000000000489760a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489760a)
#<!-- -->30 0x00000000048164eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48164eb)
#<!-- -->31 0x000000000498c14b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498c14b)
#<!-- -->32 0x0000000000dce015 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdce015)
#<!-- -->33 0x0000000000dc5ecb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000dc5f6d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004603299 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003c1b053 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1b053)
#<!-- -->37 0x00000000046034b9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000045c5e42 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c5e42)
#<!-- -->39 0x00000000045c6d21 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c6d21)
#<!-- -->40 0x00000000045cf90c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cf90c)
#<!-- -->41 0x0000000000dca999 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca999)
#<!-- -->42 0x0000000000c74c84 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c84)
#<!-- -->43 0x0000786daa829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000786daa829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000dc5965 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5965)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
````
</details>


---

### Comment 5 - cor3ntin

Right, the definition of `partial_ordering::less` and quo are fairly precise
https://eel.is/c++draft/cmp.partialord#lib:equivalent,partial_ordering

Clang makes the assumption that the standard library is conforming, but it would not be a bad idea to be a bit more defensive.

Here is a fix

```cpp
bool ComparisonCategoryInfo::ValueInfo::hasValidIntValue() const {
  assert(VD && "must have var decl");
  if (!VD->isUsableInConstantExpressions(VD->getASTContext()))
    return false;

  // Before we attempt to get the value of the first field, ensure that we
  // actually have one (and only one) field.
  const auto *Record = VD->getType()->getAsCXXRecordDecl();
  if (!Record || std::distance(Record->field_begin(), Record->field_end()) != 1 ||
      !Record->field_begin()->getType()->isIntegralOrEnumerationType())
    return false;

  return true;
}
```

---

### Comment 6 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 7 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Muhammad (muhamad)

<details>
using the following code with Clang21.1.0 and the upcoming Clang22.0.0, with the flags `-std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++` causes the compiler to crash.

```cpp

namespace std {
    struct partial_ordering {
        signed char value;

        constexpr partial_ordering(signed char v) : value(v) {}

        // relational operators (minimal)
        constexpr bool operator==(partial_ordering other) const { return value == other.value; }
        constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
        constexpr bool operator&lt;(partial_ordering other) const { return value &lt; other.value; }
        constexpr bool operator&gt;(partial_ordering other) const { return value &gt; other.value; }
        constexpr bool operator&lt;=(partial_ordering other) const { return value &lt;= other.value; }
        constexpr bool operator&gt;=(partial_ordering other) const { return value &gt;= other.value; }

        // static constants for convenience
        static constexpr signed char less = -1;
        static constexpr signed char equivalent = 0;
        static constexpr signed char greater = 1;
        static constexpr signed char unordered = 2;
    };
}

struct compare_result {
    signed char v; // -1, 0, 1, 2 for unordered

    template &lt;typename T&gt;
    constexpr compare_result(T r) {
        if (r &lt; 0) v = -1;
        else if (r &gt; 0) v = 1;
        else if (r == 0) v = 0;
        else v = 2; // unordered, e.g., NaN
    }

    constexpr bool operator==(const compare_result&amp; o) const { return v == o.v; }
    constexpr bool operator&lt;(const compare_result&amp; o) const { return v &lt; o.v; }
};

int main() {
    float a = 3.0f, b = 3.5f;
    compare_result r = a &lt;=&gt; b;
    int c = r.v;
}
````

the code compiles correctly using the same flags under GCC 15.2. I've used Compiler explorer to test it. adding the `-v` flags produce the following results

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9bae84b01718e53495abf50958abc86ea45f16bb)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin
Found candidate GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Selected GCC installation: /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
 (in-process)
 "/cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -v -fcoverage-compilation-dir=/app -nostdsysteminc -nobuiltininc -resource-dir /cefs/ee/eec5d4d1d46cbdcf81dd02c6_clang-trunk-20251129/lib/clang/22 -std=c++23 -fdeprecated-macro -ferror-limit 19 -fno-builtin -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /app/output.s -x c++ &lt;source&gt;
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
#include "..." search starts here:
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -ffreestanding -nostdlib -fno-builtin -nostdinc -nostdinc++ -v &lt;source&gt;
1.	&lt;source&gt;:41:31: current parser token ';'
2.	&lt;source&gt;:39:12: parsing function body 'main'
3.	&lt;source&gt;:39:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003cd7358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd7358)
 #<!-- -->1 0x0000000003cd4d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd4d2c)
 #<!-- -->2 0x0000000003c1ac38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000786daa842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007294195 clang::RecordDecl::field_begin() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7294195)
 #<!-- -->5 0x0000000007272aa2 clang::ComparisonCategoryInfo::ValueInfo::hasValidIntValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7272aa2)
 #<!-- -->6 0x000000000665f2e6 clang::Sema::CheckComparisonCategoryType(clang::ComparisonCategoryType, clang::SourceLocation, clang::Sema::ComparisonCategoryUsage) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x665f2e6)
 #<!-- -->7 0x0000000006768cb7 clang::Sema::CheckCompareOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6768cb7)
 #<!-- -->8 0x000000000676a294 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676a294)
 #<!-- -->9 0x000000000676b760 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676b760)
#<!-- -->10 0x000000000676bb3c clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676bb3c)
#<!-- -->11 0x0000000006265bfc clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6265bfc)
#<!-- -->12 0x0000000006290a70 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6290a70)
#<!-- -->13 0x000000000621d855 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621d855)
#<!-- -->14 0x0000000006231ffc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6231ffc)
#<!-- -->15 0x0000000006234a0c clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234a0c)
#<!-- -->16 0x0000000006234e5a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6234e5a)
#<!-- -->17 0x00000000062f15df clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f15df)
#<!-- -->18 0x00000000062f1f26 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f1f26)
#<!-- -->19 0x00000000062fb419 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fb419)
#<!-- -->20 0x00000000062fbeba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbeba)
#<!-- -->21 0x00000000061e5b4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e5b4b)
#<!-- -->22 0x000000000623245f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623245f)
#<!-- -->23 0x00000000061def8f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61def8f)
#<!-- -->24 0x00000000061df6c7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61df6c7)
#<!-- -->25 0x00000000061e809f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e809f)
#<!-- -->26 0x00000000061e98a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e98a7)
#<!-- -->27 0x00000000061c277a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c277a)
#<!-- -->28 0x0000000004595f65 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4595f65)
#<!-- -->29 0x000000000489760a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489760a)
#<!-- -->30 0x00000000048164eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48164eb)
#<!-- -->31 0x000000000498c14b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498c14b)
#<!-- -->32 0x0000000000dce015 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdce015)
#<!-- -->33 0x0000000000dc5ecb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000dc5f6d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004603299 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003c1b053 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1b053)
#<!-- -->37 0x00000000046034b9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000045c5e42 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c5e42)
#<!-- -->39 0x00000000045c6d21 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c6d21)
#<!-- -->40 0x00000000045cf90c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cf90c)
#<!-- -->41 0x0000000000dca999 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca999)
#<!-- -->42 0x0000000000c74c84 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c84)
#<!-- -->43 0x0000786daa829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000786daa829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000dc5965 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5965)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
````
</details>


---

### Comment 8 - Abhiramjampani

Hi @muhamad , I'd like to work on this issue. Could you please assign it to me?


---

### Comment 9 - cor3ntin

@Abhiramjampani no need to be assigned! Feel free to open a PR, I'd be happy to review it :)

---

