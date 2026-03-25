# clang++ front end crash on combination of explicit object parameter, static_assert(): isInitialized() && "querying uninitialized conversion"

**Author:** m2
**URL:** https://github.com/llvm/llvm-project/issues/147121
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, needs-reduction, c++26
**Closed Date:** 2025-07-10T17:28:49Z

## Body

I encountered this with clang 20.1.2, 20.1.7 as installed from the Ubuntu PPA. Trying on godbolt.org, the "assertions trunk" version encounters this but x86-64 20.1.0 does not, so an "assertions" build seems relevant.

With the following code as bug.cpp:
```
#include <iterator>

struct X
{};

struct S1
{
  using difference_type = int;

  template<typename Self>
  Self operator++(this Self&& self, int) {
    return self;
  }

  template<typename Self>
  bool operator==(this Self&& self, const X&) {
    return true;
  }
};

struct S2 : public S1
{
  using value_type = char;

  char operator*() const {
    return {};
  }

  S2& operator++() {
    return *this;
  }

  using S1::operator++;
};

int main()
{
  static_assert(std::weakly_incrementable<S2>); // crash                        

  return 0;
}
```
`clang++ -std=c++2c -stdlib=libc++ bug.cpp
`
yields:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/tmp -fcoverage-compilation-dir=/tmp -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/llvm-20/bin/../include/c++/v1 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++2c -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-a73dec.o -x c++ bug.cpp
1.	bug.cpp:39:46: current parser token ')'
2.	bug.cpp:38:1: parsing function body 'main'
3.	bug.cpp:38:1: in compound statement ('{}')
4.	/usr/lib/llvm-20/bin/../include/c++/v1/__iterator/incrementable_traits.h:64:8: instantiating class definition 'std::iterator_traits<S2>'
#0 0x000079eace21f816 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
#1 0x000079eace21d410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
#2 0x000079eace21fecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
#3 0x000079eacca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x000079ead9262301 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x3662301)
clang++: error: unable to execute command: Segmentation fault
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 20.1.7 (++20250613062716+6146a88f6049-1~exp1~20250613062748.129)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-07fa73.cpp
clang++: note: diagnostic msg: /tmp/bug-07fa73.sh
clang++: note: diagnostic msg: 

********************
```

I intentionally didn't include the "PLEASE ATTACH" files yet because the requested .cpp is 760K and I'm assuming this can easily be reproduced with the source included above.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: mat (m2)

<details>
I encountered this with clang 20.1.2, 20.1.7 as installed from the Ubuntu PPA. Trying on godbolt.org, the "assertions trunk" version encounters this but x86-64 20.1.0 does not, so an "assertions" build seems relevant.

With the following code as bug.cpp:
```
#include &lt;iterator&gt;

struct X
{};

struct S1
{
  using difference_type = int;

  template&lt;typename Self&gt;
  Self operator++(this Self&amp;&amp; self, int) {
    return self;
  }

  template&lt;typename Self&gt;
  bool operator==(this Self&amp;&amp; self, const X&amp;) {
    return true;
  }
};

struct S2 : public S1
{
  using value_type = char;

  char operator*() const {
    return {};
  }

  S2&amp; operator++() {
    return *this;
  }

  using S1::operator++;
};

int main()
{
  static_assert(std::weakly_incrementable&lt;S2&gt;); // crash                        

  return 0;
}
```
`clang++ -std=c++2c -stdlib=libc++ bug.cpp
`
yields:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/tmp -fcoverage-compilation-dir=/tmp -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/llvm-20/bin/../include/c++/v1 -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++2c -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-a73dec.o -x c++ bug.cpp
1.	bug.cpp:39:46: current parser token ')'
2.	bug.cpp:38:1: parsing function body 'main'
3.	bug.cpp:38:1: in compound statement ('{}')
4.	/usr/lib/llvm-20/bin/../include/c++/v1/__iterator/incrementable_traits.h:64:8: instantiating class definition 'std::iterator_traits&lt;S2&gt;'
#<!-- -->0 0x000079eace21f816 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
#<!-- -->1 0x000079eace21d410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
#<!-- -->2 0x000079eace21fecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
#<!-- -->3 0x000079eacca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#<!-- -->4 0x000079ead9262301 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x3662301)
clang++: error: unable to execute command: Segmentation fault
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 20.1.7 (++20250613062716+6146a88f6049-1~exp1~20250613062748.129)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-07fa73.cpp
clang++: note: diagnostic msg: /tmp/bug-07fa73.sh
clang++: note: diagnostic msg: 

********************
```

I intentionally didn't include the "PLEASE ATTACH" files yet because the requested .cpp is 760K and I'm assuming this can easily be reproduced with the source included above.

</details>


---

### Comment 2 - EugeneZelenko

Crashes in `main` too: https://godbolt.org/z/PYxYMvheP

---

### Comment 3 - m2

I should've mentioned, this also crashes with -std=c++2a (along with a warning that explicit object parameters not supported) and -std=c++2b, so not c++26 specific.

---

### Comment 4 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/3Wd49G68M)):
```C++
struct X {};

struct S1 {
  void operator++(this auto&&, int) {}

  bool operator==(this auto&&, const X&) {
    return true;
  }
};

struct S2 : public S1 {
  void operator++() {}

  using S1::operator++;
};

template <class I>
concept has_inc = requires(I& i) {
  ++i;
  i++;
};

template <class I>
concept has_inc_eq = has_inc<I> && requires(const I& i) { i == i; };

template <class>
struct my_traits_base {};
template <has_inc I>
struct my_traits_base<I> {};
template <has_inc_eq I>
struct my_traits_base<I> {};

int main() {
  (void)my_traits_base<S2>{};
}
```

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Sema/Overload.h:716: 
clang::ImplicitConversionSequence::Kind clang::ImplicitConversionSequence::getKind() const: 
Assertion `isInitialized() && "querying uninitialized conversion"' failed.
```

Backtrace:

```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -pedantic-errors <source>
1.	<source>:34:29: current parser token ';'
2.	<source>:33:12: parsing function body 'main'
3.	<source>:33:12: in compound statement ('{}')
 #0 0x0000000003f7a2c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7a2c8)
 #1 0x0000000003f776f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f776f4)
 #2 0x0000000003ebc018 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ca1a2e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ca1a2e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ca1a2e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ca1a2e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ca1a2e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ca1a2e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006ec6b5f (anonymous namespace)::CompareOverloadCandidatesForDisplay::CompareConversions(clang::OverloadCandidate const&, clang::OverloadCandidate const&) (.isra.0) SemaOverload.cpp:0:0
#10 0x0000000006ec6e90 (anonymous namespace)::CompareOverloadCandidatesForDisplay::operator()(clang::OverloadCandidate const*, clang::OverloadCandidate const*) SemaOverload.cpp:0:0
#11 0x0000000006ee6258 void std::__merge_adaptive<clang::OverloadCandidate**, long, clang::OverloadCandidate**, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>>(clang::OverloadCandidate**, clang::OverloadCandidate**, clang::OverloadCandidate**, long, long, clang::OverloadCandidate**, long, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>) (.constprop.0) SemaOverload.cpp:0:0
#12 0x0000000006ee64e3 void std::__stable_sort_adaptive<clang::OverloadCandidate**, clang::OverloadCandidate**, long, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>>(clang::OverloadCandidate**, clang::OverloadCandidate**, clang::OverloadCandidate**, long, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>) (.constprop.0) SemaOverload.cpp:0:0
#13 0x0000000006f05a2e clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f05a2e)
#14 0x0000000006f1071e clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f1071e)
#15 0x0000000006b2da42 BuildOverloadedBinOp(clang::Sema&, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#16 0x0000000006b83726 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b83726)
#17 0x00000000071819ab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007157dba clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000717d62b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000717fb06 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000071583ae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000007161ab2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7161ab2)
#23 0x00000000069232ee calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#24 0x000000000692300e calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#25 0x00000000069252e9 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#26 0x0000000006925915 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925915)
#27 0x0000000007001fa7 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7001fa7)
#28 0x0000000007171810 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#29 0x00000000071580d1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#30 0x0000000007161ab2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7161ab2)
#31 0x00000000069232ee calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#32 0x00000000069252e9 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#33 0x0000000006925915 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925915)
#34 0x00000000070875de CheckDeducedArgumentConstraints(clang::Sema&, clang::NamedDecl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&) SemaTemplateDeduction.cpp:0:0
#35 0x000000000709a7cd FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
#36 0x000000000709ab42 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#37 0x0000000007eb71b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb71b1)
#38 0x00000000070a4cda _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#39 0x00000000071546ea getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#40 0x00000000071a5d99 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a5d99)
#41 0x0000000007224c4e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#42 0x0000000007eb71b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb71b1)
#43 0x000000000722d7ba clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722d7ba)
#44 0x000000000722ddb5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722ddb5)
#45 0x0000000006c2fa63 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2fa63)
#46 0x0000000006c302e9 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c302e9)
#47 0x00000000066bf8a6 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bf8a6)
#48 0x00000000066a3bc0 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a3bc0)
#49 0x00000000066a27de clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a27de)
#50 0x00000000066a4707 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4707)
#51 0x00000000066ad366 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ad366)
#52 0x00000000066a342c clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a342c)
#53 0x00000000066a4707 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4707)
#54 0x00000000066a4799 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4799)
#55 0x00000000066a8e49 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a8e49)
#56 0x000000000673d4f9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d4f9)
#57 0x0000000006734e08 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6734e08)
#58 0x0000000006735c2d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6735c2d)
#59 0x000000000673dbeb clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673dbeb)
#60 0x000000000673e43a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673e43a)
#61 0x0000000006635f73 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6635f73)
#62 0x000000000666bedd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666bedd)
#63 0x0000000006628cae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6628cae)
#64 0x0000000006629459 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6629459)
#65 0x00000000066310da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66310da)
#66 0x000000000663208d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663208d)
#67 0x0000000006623e2a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6623e2a)
#68 0x000000000492dcd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492dcd8)
#69 0x0000000004c27125 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c27125)
#70 0x0000000004ba408e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba408e)
#71 0x0000000004d18961 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d18961)
#72 0x0000000000d90b1f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd90b1f)
#73 0x0000000000d876fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#74 0x0000000004994889 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#75 0x0000000003ebc4b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebc4b4)
#76 0x0000000004994e9f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#77 0x0000000004956e6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4956e6d)
#78 0x0000000004957efe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957efe)
#79 0x0000000004960a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4960a95)
#80 0x0000000000d8cfcf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8cfcf)
#81 0x0000000000c42484 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc42484)
#82 0x00007ca1a2e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#83 0x00007ca1a2e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#84 0x0000000000d871a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd871a5)
```

---

