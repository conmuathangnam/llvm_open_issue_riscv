# -ast-dump triggers assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"'

**Author:** stbergmann
**URL:** https://github.com/llvm/llvm-project/issues/43179
**Status:** Closed
**Labels:** c++, clang:frontend, bugzilla, confirmed, crash-on-valid
**Closed Date:** 2025-08-09T08:06:55Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [43834](https://llvm.org/bz43834) |
| Resolution | WORKSFORME |
| Resolved on | Oct 29, 2021 07:01 |
| Version | trunk |
| OS | Linux |
| CC | @AaronBallman,@grandinj,@zygoloid |

## Extended Description 
At least with recent trunk ("clang version 10.0.0 (git@github.com:llvm/llvm-project 93e137f898296d142d056c892b10d40ca3c26f00)"):

> $ cat test.cc
> namespace N { template<typename> struct S1; }
> template<typename T> struct S2 {
>     using U = N::S1<typename N::S1<T>::t>;
>     S2(typename U::t);
> };
> template<typename T> S2(T) -> S2<void>;

> $ clang++ -Xclang -ast-dump -std=c++17 -fsyntax-only test.cc >/dev/null
> clang-10: /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328: void clang::NestedNameSpecifier::print(llvm::raw_ostream&, const clang::PrintingPolicy&, bool) const: Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"' failed.
> Stack dump:
> 0.	Program arguments: /data/sbergman/llvm/debug/inst/bin/clang-10 -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -main-file-name test.cc -mrelocation-model static -mthread-model posix -mframe-pointer=all -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -resource-dir /data/sbergman/llvm/debug/inst/lib/clang/10.0.0 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/backward -internal-isystem /usr/local/include -internal-isystem /data/sbergman/llvm/debug/inst/lib/clang/10.0.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++17 -fdeprecated-macro -fdebug-compilation-dir /data/sbergman/lo-clang2/core -ferror-limit 19 -fmessage-length 0 -fgnuc-version=4.2.1 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -ast-dump -faddrsig -x c++ test.cc 
> 1.	<eof> parser at end of file
>  #&#8203;0 0x00007f7b17f046fe llvm::sys::PrintStackTrace(llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:544:22
>  #&#8203;1 0x00007f7b17f04791 PrintStackTraceSignalHandler(void*) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:605:1
>  #&#8203;2 0x00007f7b17f028e8 llvm::sys::RunSignalHandlers() /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Signals.cpp:68:20
>  #&#8203;3 0x00007f7b17f0414e SignalHandler(int) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:391:1
>  #&#8203;4 0x00007f7b170e3c60 __restore_rt (/lib64/libpthread.so.0+0x12c60)
>  #&#8203;5 0x00007f7b1462ce35 raise /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
>  #&#8203;6 0x00007f7b14617895 abort /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/stdlib/abort.c:81:7
>  #&#8203;7 0x00007f7b14617769 _nl_load_domain.cold /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/intl/loadmsgcat.c:1177:9
>  #&#8203;8 0x00007f7b14625566 (/lib64/libc.so.6+0x30566)
>  #&#8203;9 0x00007f7b12492f69 clang::NestedNameSpecifier::print(llvm::raw_ostream&, clang::PrintingPolicy const&, bool) const (.localalias) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328:5
> #&#8203;10 0x00007f7b1258ee46 (anonymous namespace)::TypePrinter::printDependentNameBefore(clang::DependentNameType const*, llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1367:37
> #&#8203;11 0x00007f7b1258a09b (anonymous namespace)::TypePrinter::printBefore(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:35:1
> #&#8203;12 0x00007f7b12589ad6 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:186:9
> #&#8203;13 0x00007f7b12589a56 (anonymous namespace)::TypePrinter::print(clang::QualType, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:174:1
> #&#8203;14 0x00007f7b1258ccb1 (anonymous namespace)::TypePrinter::printFunctionProtoAfter(clang::FunctionProtoType const*, llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:791:5
> #&#8203;15 0x00007f7b1258aa61 (anonymous namespace)::TypePrinter::printAfter(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:44:1
> #&#8203;16 0x00007f7b12589b04 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:183:70
> #&#8203;17 0x00007f7b12590838 clang::QualType::getAsStringInternal(clang::Type const*, clang::Qualifiers, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1906:3
> #&#8203;18 0x00007f7b125905fc clang::QualType::getAsString[abi:cxx11](clang::Type const*, clang::Qualifiers, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1877:10
> #&#8203;19 0x00007f7b12462f17 clang::QualType::getAsString[abi:cxx11](clang::SplitQualType, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/Type.h:982:3
> #&#8203;20 0x00007f7b1253957d clang::TextNodeDumper::dumpBareType(clang::QualType, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:390:58
> #&#8203;21 0x00007f7b125396d4 clang::TextNodeDumper::dumpType(clang::QualType) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:403:1
> #&#8203;22 0x00007f7b1253d831 clang::TextNodeDumper::VisitFunctionDecl(clang::FunctionDecl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:1274:39
> #&#8203;23 0x00007f7b1255944f clang::declvisitor::Base<llvm::make_const_ptr, clang::TextNodeDumper, void>::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
> #&#8203;24 0x00007f7b12553e80 clang::declvisitor::Base<llvm::make_const_ptr, clang::TextNodeDumper, void>::Visit(clang::Decl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
> #&#8203;25 0x00007f7b12538c30 clang::TextNodeDumper::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:277:1
> #&#8203;26 0x00007f7b11fe5869 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:78:32
> #&#8203;27 0x00007f7b11fef9ac void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:103:14
> #&#8203;28 0x00007f7b1200a153 std::_Function_handler<void (bool), void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)>::_M_invoke(std::_Any_data const&, bool&&) /usr/include/c++/9/bits/std_function.h:302:7
> #&#8203;29 0x00007f7b11fefdae std::function<void (bool)>::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
> #&#8203;30 0x00007f7b11fef9ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:72:27
> #&#8203;31 0x00007f7b1200a153 std::_Function_handler<void (bool), void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)>::_M_invoke(std::_Any_data const&, bool&&) /usr/include/c++/9/bits/std_function.h:302:7
> #&#8203;32 0x00007f7b11fefdae std::function<void (bool)>::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
> #&#8203;33 0x00007f7b11fefcae void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:116:33
> #&#8203;34 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
> #&#8203;35 0x00007f7b11fe5a0c clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
> #&#8203;36 0x00007f7b11fe8489 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::dumpDeclContext(clang::DeclContext const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:230:5
> #&#8203;37 0x00007f7b11fe59c0 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:95:11
> #&#8203;38 0x00007f7b11fefb24 void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:59:28
> #&#8203;39 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
> #&#8203;40 0x00007f7b11fe5a0c clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
> #&#8203;41 0x00007f7b11fdf118 clang::Decl::dump(llvm::raw_ostream&, bool, clang::ASTDumpOutputFormat) const /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/ASTDumper.cpp:193:77
> #&#8203;42 0x00007f7b155105cd (anonymous namespace)::ASTPrinter::print(clang::Decl*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:96:5
> #&#8203;43 0x00007f7b155101c9 (anonymous namespace)::ASTPrinter::HandleTranslationUnit(clang::ASTContext&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:48:23
> #&#8203;44 0x00007f7b106ed7b0 clang::ParseAST(clang::Sema&, bool, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:178:14
> #&#8203;45 0x00007f7b156ce653 clang::ASTFrontendAction::ExecuteAction() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1042:11
> #&#8203;46 0x00007f7b156cdfbf clang::FrontendAction::Execute() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:939:38
> #&#8203;47 0x00007f7b15654689 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:964:42
> #&#8203;48 0x00007f7b1518e719 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:290:38
> #&#8203;49 0x0000000000269729 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/cc1_main.cpp:250:40
> #&#8203;50 0x000000000025f64d ExecuteCC1Tool(llvm::ArrayRef<char const*>, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:309:64
> #&#8203;51 0x000000000025fcad main /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:382:26
> #&#8203;52 0x00007f7b14618f43 __libc_start_main /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/csu/../csu/libc-start.c:342:3
> #&#8203;53 0x000000000025e02e _start (/data/sbergman/llvm/debug/inst/bin/clang-10+0x25e02e)

## Comments

### Comment 1 - stbergmann

indeed, appears to have been fixed in the meantime

---

### Comment 2 - AaronBallman

I cannot reproduce this issue. If you're still seeing the problem with Clang trunk, please reopen this bug with additional details on how to reproduce.

---

### Comment 3 - nickdesaulniers

```c++
namespace {
template <bool, typename, typename> struct conditional;
template <bool _Cond, typename _Iftrue, typename _Iffalse>
using conditional_t = conditional<_Cond, _Iftrue, _Iffalse>;
const int Dynamic{};
template <typename, int> class Matrix;
template <int = Dynamic> class InnerStride;
template <int = Dynamic> class OuterStride;
template <typename PlainObjectType, int,
          typename = conditional_t<PlainObjectType::IsVectorAtCompileTime,
                                   InnerStride<>, OuterStride<>>>
class Ref;
namespace {
template <typename VectorType> void stable_norm_impl_inner_step() {
  typedef typename VectorType::Scalar Scalar;
  typedef typename VectorType::type VectorTypeCopyClean;
  enum { CanAlign };
  conditional_t<CanAlign,
                Ref<const Matrix<Scalar, 1>, VectorTypeCopyClean ::Alignment>,
                typename VectorTypeCopyClean::ConstSegmentReturnType>
      SegmentWrapper;
}
} // namespace
} // namespace
int main() {}
```
```
$ clang++ foo.i -Xclang -ast-dump
...
| |         `-VarDecl 0x55c1e248eed0 <line:18:3, line:21:7> col:7 SegmentWrapper 'conditional_t<CanAlign, Ref<const Matrix<Scalar, 1>, VectorTypeCopyClean::Alignment>, typename VectorTypeCopyClean::ConstSegmentReturnType>':'clang-18: /android0/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:309: void clang::NestedNameSpecifier::print(raw_ostream &, const PrintingPolicy &, bool) const: Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
...
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-frontend

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [43834](https://llvm.org/bz43834) |
| Resolution | WORKSFORME |
| Resolved on | Oct 29, 2021 07:01 |
| Version | trunk |
| OS | Linux |
| CC | @AaronBallman,@grandinj,@zygoloid |

## Extended Description 
At least with recent trunk ("clang version 10.0.0 (git@github.com:llvm/llvm-project 93e137f898296d142d056c892b10d40ca3c26f00)"):

> $ cat test.cc
> namespace N { template<typename> struct S1; }
> template<typename T> struct S2 {
>     using U = N::S1<typename N::S1<T>::t>;
>     S2(typename U::t);
> };
> template<typename T> S2(T) -> S2<void>;

> $ clang++ -Xclang -ast-dump -std=c++17 -fsyntax-only test.cc >/dev/null
> clang-10: /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328: void clang::NestedNameSpecifier::print(llvm::raw_ostream&, const clang::PrintingPolicy&, bool) const: Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"' failed.
> Stack dump:
> 0.	Program arguments: /data/sbergman/llvm/debug/inst/bin/clang-10 -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -main-file-name test.cc -mrelocation-model static -mthread-model posix -mframe-pointer=all -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -resource-dir /data/sbergman/llvm/debug/inst/lib/clang/10.0.0 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/backward -internal-isystem /usr/local/include -internal-isystem /data/sbergman/llvm/debug/inst/lib/clang/10.0.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++17 -fdeprecated-macro -fdebug-compilation-dir /data/sbergman/lo-clang2/core -ferror-limit 19 -fmessage-length 0 -fgnuc-version=4.2.1 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -ast-dump -faddrsig -x c++ test.cc 
> 1.	<eof> parser at end of file
>  #&#8203;0 0x00007f7b17f046fe llvm::sys::PrintStackTrace(llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:544:22
>  #&#8203;1 0x00007f7b17f04791 PrintStackTraceSignalHandler(void*) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:605:1
>  #&#8203;2 0x00007f7b17f028e8 llvm::sys::RunSignalHandlers() /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Signals.cpp:68:20
>  #&#8203;3 0x00007f7b17f0414e SignalHandler(int) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:391:1
>  #&#8203;4 0x00007f7b170e3c60 __restore_rt (/lib64/libpthread.so.0+0x12c60)
>  #&#8203;5 0x00007f7b1462ce35 raise /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
>  #&#8203;6 0x00007f7b14617895 abort /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/stdlib/abort.c:81:7
>  #&#8203;7 0x00007f7b14617769 _nl_load_domain.cold /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/intl/loadmsgcat.c:1177:9
>  #&#8203;8 0x00007f7b14625566 (/lib64/libc.so.6+0x30566)
>  #&#8203;9 0x00007f7b12492f69 clang::NestedNameSpecifier::print(llvm::raw_ostream&, clang::PrintingPolicy const&, bool) const (.localalias) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328:5
> #&#8203;10 0x00007f7b1258ee46 (anonymous namespace)::TypePrinter::printDependentNameBefore(clang::DependentNameType const*, llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1367:37
> #&#8203;11 0x00007f7b1258a09b (anonymous namespace)::TypePrinter::printBefore(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:35:1
> #&#8203;12 0x00007f7b12589ad6 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:186:9
> #&#8203;13 0x00007f7b12589a56 (anonymous namespace)::TypePrinter::print(clang::QualType, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:174:1
> #&#8203;14 0x00007f7b1258ccb1 (anonymous namespace)::TypePrinter::printFunctionProtoAfter(clang::FunctionProtoType const*, llvm::raw_ostream&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:791:5
> #&#8203;15 0x00007f7b1258aa61 (anonymous namespace)::TypePrinter::printAfter(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:44:1
> #&#8203;16 0x00007f7b12589b04 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:183:70
> #&#8203;17 0x00007f7b12590838 clang::QualType::getAsStringInternal(clang::Type const*, clang::Qualifiers, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1906:3
> #&#8203;18 0x00007f7b125905fc clang::QualType::getAsString[abi:cxx11](clang::Type const*, clang::Qualifiers, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1877:10
> #&#8203;19 0x00007f7b12462f17 clang::QualType::getAsString[abi:cxx11](clang::SplitQualType, clang::PrintingPolicy const&) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/Type.h:982:3
> #&#8203;20 0x00007f7b1253957d clang::TextNodeDumper::dumpBareType(clang::QualType, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:390:58
> #&#8203;21 0x00007f7b125396d4 clang::TextNodeDumper::dumpType(clang::QualType) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:403:1
> #&#8203;22 0x00007f7b1253d831 clang::TextNodeDumper::VisitFunctionDecl(clang::FunctionDecl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:1274:39
> #&#8203;23 0x00007f7b1255944f clang::declvisitor::Base<llvm::make_const_ptr, clang::TextNodeDumper, void>::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
> #&#8203;24 0x00007f7b12553e80 clang::declvisitor::Base<llvm::make_const_ptr, clang::TextNodeDumper, void>::Visit(clang::Decl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
> #&#8203;25 0x00007f7b12538c30 clang::TextNodeDumper::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:277:1
> #&#8203;26 0x00007f7b11fe5869 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:78:32
> #&#8203;27 0x00007f7b11fef9ac void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:103:14
> #&#8203;28 0x00007f7b1200a153 std::_Function_handler<void (bool), void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)>::_M_invoke(std::_Any_data const&, bool&&) /usr/include/c++/9/bits/std_function.h:302:7
> #&#8203;29 0x00007f7b11fefdae std::function<void (bool)>::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
> #&#8203;30 0x00007f7b11fef9ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:72:27
> #&#8203;31 0x00007f7b1200a153 std::_Function_handler<void (bool), void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)>::_M_invoke(std::_Any_data const&, bool&&) /usr/include/c++/9/bits/std_function.h:302:7
> #&#8203;32 0x00007f7b11fefdae std::function<void (bool)>::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
> #&#8203;33 0x00007f7b11fefcae void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:116:33
> #&#8203;34 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
> #&#8203;35 0x00007f7b11fe5a0c clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
> #&#8203;36 0x00007f7b11fe8489 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::dumpDeclContext(clang::DeclContext const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:230:5
> #&#8203;37 0x00007f7b11fe59c0 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:95:11
> #&#8203;38 0x00007f7b11fefb24 void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:59:28
> #&#8203;39 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*)::'lambda'()>(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
> #&#8203;40 0x00007f7b11fe5a0c clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
> #&#8203;41 0x00007f7b11fdf118 clang::Decl::dump(llvm::raw_ostream&, bool, clang::ASTDumpOutputFormat) const /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/ASTDumper.cpp:193:77
> #&#8203;42 0x00007f7b155105cd (anonymous namespace)::ASTPrinter::print(clang::Decl*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:96:5
> #&#8203;43 0x00007f7b155101c9 (anonymous namespace)::ASTPrinter::HandleTranslationUnit(clang::ASTContext&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:48:23
> #&#8203;44 0x00007f7b106ed7b0 clang::ParseAST(clang::Sema&, bool, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:178:14
> #&#8203;45 0x00007f7b156ce653 clang::ASTFrontendAction::ExecuteAction() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1042:11
> #&#8203;46 0x00007f7b156cdfbf clang::FrontendAction::Execute() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:939:38
> #&#8203;47 0x00007f7b15654689 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:964:42
> #&#8203;48 0x00007f7b1518e719 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:290:38
> #&#8203;49 0x0000000000269729 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/cc1_main.cpp:250:40
> #&#8203;50 0x000000000025f64d ExecuteCC1Tool(llvm::ArrayRef<char const*>, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:309:64
> #&#8203;51 0x000000000025fcad main /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:382:26
> #&#8203;52 0x00007f7b14618f43 __libc_start_main /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/csu/../csu/libc-start.c:342:3
> #&#8203;53 0x000000000025e02e _start (/data/sbergman/llvm/debug/inst/bin/clang-10+0x25e02e)
</details>

---

### Comment 5 - AaronBallman

I reduced the example a little bit more down to:
```
template <bool, typename> struct conditional;
template <bool _Cond, typename _Iftrue>
using conditional_t = conditional<_Cond, _Iftrue>;
template <typename> class Matrix;
template <typename PlainObjectType, 
          typename = conditional_t<PlainObjectType::IsVectorAtCompileTime,
                                   int>>
class Ref;

template <typename VectorType> void stable_norm_impl_inner_step() {
  conditional_t<true, Ref<const Matrix<VectorType>>> SegmentWrapper;
}
```
Interesting things I noticed while doing this:

1) `const Matrix` causes the crash, but `Matrix` without the `const` does not
2) Cannot get rid of `PlainObjectType::IsVectorAtCompileTime` and replace it with a constant, that is the nested name specifier that we're failing to print.
3) Instantiating `Ref` isn't sufficient to cause the crash; it needs to be instantiated within an argument to `conditional_t` (adding a different class template and using that instead of `conditional_t` also makes the crash go away.

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-c-1

Author: None (stbergmann)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [43834](https://llvm.org/bz43834) |
| Resolution | WORKSFORME |
| Resolved on | Oct 29, 2021 07:01 |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->AaronBallman,@<!-- -->grandinj,@<!-- -->zygoloid |

## Extended Description 
At least with recent trunk ("clang version 10.0.0 (git@<!-- -->github.com:llvm/llvm-project 93e137f898296d142d056c892b10d40ca3c26f00)"):

&gt; $ cat test.cc
&gt; namespace N { template&lt;typename&gt; struct S1; }
&gt; template&lt;typename T&gt; struct S2 {
&gt;     using U = N::S1&lt;typename N::S1&lt;T&gt;::t&gt;;
&gt;     S2(typename U::t);
&gt; };
&gt; template&lt;typename T&gt; S2(T) -&gt; S2&lt;void&gt;;

&gt; $ clang++ -Xclang -ast-dump -std=c++17 -fsyntax-only test.cc &gt;/dev/null
&gt; clang-10: /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328: void clang::NestedNameSpecifier::print(llvm::raw_ostream&amp;, const clang::PrintingPolicy&amp;, bool) const: Assertion `!isa&lt;ElaboratedType&gt;(T) &amp;&amp; "Elaborated type in nested-name-specifier"' failed.
&gt; Stack dump:
&gt; 0.	Program arguments: /data/sbergman/llvm/debug/inst/bin/clang-10 -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -main-file-name test.cc -mrelocation-model static -mthread-model posix -mframe-pointer=all -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -resource-dir /data/sbergman/llvm/debug/inst/lib/clang/10.0.0 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/9/../../../../include/c++/9/backward -internal-isystem /usr/local/include -internal-isystem /data/sbergman/llvm/debug/inst/lib/clang/10.0.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++17 -fdeprecated-macro -fdebug-compilation-dir /data/sbergman/lo-clang2/core -ferror-limit 19 -fmessage-length 0 -fgnuc-version=4.2.1 -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -ast-dump -faddrsig -x c++ test.cc 
&gt; 1.	&lt;eof&gt; parser at end of file
&gt;  #&amp;#<!-- -->8203;0 0x00007f7b17f046fe llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:544:22
&gt;  #&amp;#<!-- -->8203;1 0x00007f7b17f04791 PrintStackTraceSignalHandler(void*) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:605:1
&gt;  #&amp;#<!-- -->8203;2 0x00007f7b17f028e8 llvm::sys::RunSignalHandlers() /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Signals.cpp:68:20
&gt;  #&amp;#<!-- -->8203;3 0x00007f7b17f0414e SignalHandler(int) /data/sbergman/github.com/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:391:1
&gt;  #&amp;#<!-- -->8203;4 0x00007f7b170e3c60 __restore_rt (/lib64/libpthread.so.0+0x12c60)
&gt;  #&amp;#<!-- -->8203;5 0x00007f7b1462ce35 raise /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
&gt;  #&amp;#<!-- -->8203;6 0x00007f7b14617895 abort /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/stdlib/abort.c:81:7
&gt;  #&amp;#<!-- -->8203;7 0x00007f7b14617769 _nl_load_domain.cold /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/intl/loadmsgcat.c:1177:9
&gt;  #&amp;#<!-- -->8203;8 0x00007f7b14625566 (/lib64/libc.so.6+0x30566)
&gt;  #&amp;#<!-- -->8203;9 0x00007f7b12492f69 clang::NestedNameSpecifier::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, bool) const (.localalias) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:328:5
&gt; #&amp;#<!-- -->8203;10 0x00007f7b1258ee46 (anonymous namespace)::TypePrinter::printDependentNameBefore(clang::DependentNameType const*, llvm::raw_ostream&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1367:37
&gt; #&amp;#<!-- -->8203;11 0x00007f7b1258a09b (anonymous namespace)::TypePrinter::printBefore(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:35:1
&gt; #&amp;#<!-- -->8203;12 0x00007f7b12589ad6 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:186:9
&gt; #&amp;#<!-- -->8203;13 0x00007f7b12589a56 (anonymous namespace)::TypePrinter::print(clang::QualType, llvm::raw_ostream&amp;, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:174:1
&gt; #&amp;#<!-- -->8203;14 0x00007f7b1258ccb1 (anonymous namespace)::TypePrinter::printFunctionProtoAfter(clang::FunctionProtoType const*, llvm::raw_ostream&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:791:5
&gt; #&amp;#<!-- -->8203;15 0x00007f7b1258aa61 (anonymous namespace)::TypePrinter::printAfter(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/TypeNodes.inc:44:1
&gt; #&amp;#<!-- -->8203;16 0x00007f7b12589b04 (anonymous namespace)::TypePrinter::print(clang::Type const*, clang::Qualifiers, llvm::raw_ostream&amp;, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:183:70
&gt; #&amp;#<!-- -->8203;17 0x00007f7b12590838 clang::QualType::getAsStringInternal(clang::Type const*, clang::Qualifiers, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;&amp;, clang::PrintingPolicy const&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1906:3
&gt; #&amp;#<!-- -->8203;18 0x00007f7b125905fc clang::QualType::getAsString[abi:cxx11](clang::Type const*, clang::Qualifiers, clang::PrintingPolicy const&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TypePrinter.cpp:1877:10
&gt; #&amp;#<!-- -->8203;19 0x00007f7b12462f17 clang::QualType::getAsString[abi:cxx11](clang::SplitQualType, clang::PrintingPolicy const&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/Type.h:982:3
&gt; #&amp;#<!-- -->8203;20 0x00007f7b1253957d clang::TextNodeDumper::dumpBareType(clang::QualType, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:390:58
&gt; #&amp;#<!-- -->8203;21 0x00007f7b125396d4 clang::TextNodeDumper::dumpType(clang::QualType) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:403:1
&gt; #&amp;#<!-- -->8203;22 0x00007f7b1253d831 clang::TextNodeDumper::VisitFunctionDecl(clang::FunctionDecl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:1274:39
&gt; #&amp;#<!-- -->8203;23 0x00007f7b1255944f clang::declvisitor::Base&lt;llvm::make_const_ptr, clang::TextNodeDumper, void&gt;::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
&gt; #&amp;#<!-- -->8203;24 0x00007f7b12553e80 clang::declvisitor::Base&lt;llvm::make_const_ptr, clang::TextNodeDumper, void&gt;::Visit(clang::Decl const*) /data/sbergman/llvm/debug/build/tools/clang/include/clang/AST/DeclNodes.inc:399:1
&gt; #&amp;#<!-- -->8203;25 0x00007f7b12538c30 clang::TextNodeDumper::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/TextNodeDumper.cpp:277:1
&gt; #&amp;#<!-- -->8203;26 0x00007f7b11fe5869 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:78:32
&gt; #&amp;#<!-- -->8203;27 0x00007f7b11fef9ac void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:103:14
&gt; #&amp;#<!-- -->8203;28 0x00007f7b1200a153 std::_Function_handler&lt;void (bool), void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)&gt;::_M_invoke(std::_Any_data const&amp;, bool&amp;&amp;) /usr/include/c++/9/bits/std_function.h:302:7
&gt; #&amp;#<!-- -->8203;29 0x00007f7b11fefdae std::function&lt;void (bool)&gt;::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
&gt; #&amp;#<!-- -->8203;30 0x00007f7b11fef9ec void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)::operator()(bool) const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:72:27
&gt; #&amp;#<!-- -->8203;31 0x00007f7b1200a153 std::_Function_handler&lt;void (bool), void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper)::'lambda'(bool)&gt;::_M_invoke(std::_Any_data const&amp;, bool&amp;&amp;) /usr/include/c++/9/bits/std_function.h:302:7
&gt; #&amp;#<!-- -->8203;32 0x00007f7b11fefdae std::function&lt;void (bool)&gt;::operator()(bool) const /usr/include/c++/9/bits/std_function.h:690:71
&gt; #&amp;#<!-- -->8203;33 0x00007f7b11fefcae void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:116:33
&gt; #&amp;#<!-- -->8203;34 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
&gt; #&amp;#<!-- -->8203;35 0x00007f7b11fe5a0c clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
&gt; #&amp;#<!-- -->8203;36 0x00007f7b11fe8489 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::dumpDeclContext(clang::DeclContext const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:230:5
&gt; #&amp;#<!-- -->8203;37 0x00007f7b11fe59c0 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()::operator()() const /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:95:11
&gt; #&amp;#<!-- -->8203;38 0x00007f7b11fefb24 void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(llvm::StringRef, clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:59:28
&gt; #&amp;#<!-- -->8203;39 0x00007f7b11fe84ec void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*)::'lambda'()&gt;(clang::ASTDumper) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/TextNodeDumper.h:49:3
&gt; #&amp;#<!-- -->8203;40 0x00007f7b11fe5a0c clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*) /data/sbergman/github.com/llvm/llvm-project/clang/include/clang/AST/ASTNodeTraverser.h:98:3
&gt; #&amp;#<!-- -->8203;41 0x00007f7b11fdf118 clang::Decl::dump(llvm::raw_ostream&amp;, bool, clang::ASTDumpOutputFormat) const /data/sbergman/github.com/llvm/llvm-project/clang/lib/AST/ASTDumper.cpp:193:77
&gt; #&amp;#<!-- -->8203;42 0x00007f7b155105cd (anonymous namespace)::ASTPrinter::print(clang::Decl*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:96:5
&gt; #&amp;#<!-- -->8203;43 0x00007f7b155101c9 (anonymous namespace)::ASTPrinter::HandleTranslationUnit(clang::ASTContext&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/ASTConsumers.cpp:48:23
&gt; #&amp;#<!-- -->8203;44 0x00007f7b106ed7b0 clang::ParseAST(clang::Sema&amp;, bool, bool) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:178:14
&gt; #&amp;#<!-- -->8203;45 0x00007f7b156ce653 clang::ASTFrontendAction::ExecuteAction() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1042:11
&gt; #&amp;#<!-- -->8203;46 0x00007f7b156cdfbf clang::FrontendAction::Execute() /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:939:38
&gt; #&amp;#<!-- -->8203;47 0x00007f7b15654689 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /data/sbergman/github.com/llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:964:42
&gt; #&amp;#<!-- -->8203;48 0x00007f7b1518e719 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/sbergman/github.com/llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:290:38
&gt; #&amp;#<!-- -->8203;49 0x0000000000269729 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/cc1_main.cpp:250:40
&gt; #&amp;#<!-- -->8203;50 0x000000000025f64d ExecuteCC1Tool(llvm::ArrayRef&lt;char const*&gt;, llvm::StringRef) /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:309:64
&gt; #&amp;#<!-- -->8203;51 0x000000000025fcad main /data/sbergman/github.com/llvm/llvm-project/clang/tools/driver/driver.cpp:382:26
&gt; #&amp;#<!-- -->8203;52 0x00007f7b14618f43 __libc_start_main /usr/src/debug/glibc-2.29-32-g6d8eaf4a25/csu/../csu/libc-start.c:342:3
&gt; #&amp;#<!-- -->8203;53 0x000000000025e02e _start (/data/sbergman/llvm/debug/inst/bin/clang-10+0x25e02e)
</details>


---

### Comment 7 - danix800

Might be related to https://github.com/llvm/llvm-project/issues/68670

---

### Comment 8 - mizvekov

While the crash had stopped happening because the particular assertion this was tripping has been removed in  https://github.com/llvm/llvm-project/pull/133610, https://github.com/llvm/llvm-project/pull/147835 will fix this for the mere fact that ElaboratedType nodes will cease to exist.

---

