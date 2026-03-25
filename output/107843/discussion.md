# Clang crashes on incorrect code with build compiler returned: 254

**Author:** Strke
**URL:** https://github.com/llvm/llvm-project/issues/107843

## Body

Reproducer: https://gcc.godbolt.org/z/dj3765YPn (assertions trunk）
```
#include <iostream>
class Base;
class Derived;
class DerivedClass2;
class Base
{
public:
    Base()  = default;
    Base(const Base&) {}
    virtual ~Base()  = default;
    virtual bool operator==(const Derived&) const noexcept {}
    virtual bool operator<(const Base&) const noexcept {}

protected:
    DerivedClass2 obj;
};
class Derived : public Base
{
public:
    bool operator==(const Derived&) const = default;
    bool operator<(const Base&) const = default;
};
class DerivedClass2 : public Derived
{
public:
    bool operator==(const Derived&) const = default;
    bool operator<(const Base&) const = default;
};
class DerivedClass : public Derived
{
public:
    bool operator==(const Derived&) const = default;
    bool operator<(const Base&) const = default;
protected:
    Base obj(const Derived&);
};
DerivedClass2 obj{};
int main()
{
    std::cout << "ok";
}
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20240909/lib/clang/20 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20240909/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include --std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-338b0c.o -x c++ <source>
1.	<source>:34:2: current parser token ';'
2.	<source>:29:1: parsing struct/union/class body 'DerivedClass2'
  #0 0x0000000003b258d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x3b258d8)
  #1 0x0000000003b2301c SignalHandler(int) Signals.cpp:0:0
  #2 0x0000725aa1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x0000000001abfe06 _ZN4llvm7hashing6detail23hash_combine_range_implIKjEENSt9enable_ifIXsrNS1_16is_hashable_dataIT_EE5valueENS_9hash_codeEE4typeEPS6_SB_ (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x1abfe06)
  #4 0x0000000003a78db1 llvm::FoldingSetBase::FindNodeOrInsertPos(llvm::FoldingSetNodeID const&, void*&, llvm::FoldingSetBase::FoldingSetInfo const&) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x3a78db1)
  #5 0x000000000793da12 clang::DeclarationNameTable::getCXXDestructorName(clang::CanQual<clang::Type>) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x793da12)
  #6 0x0000000007954f58 clang::CXXRecordDecl::getDestructor() const (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x7954f58)
  #7 0x0000000007957659 clang::CXXRecordDecl::hasConstexprDestructor() const (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x7957659)
  #8 0x0000000006a02d28 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
  #9 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
 #10 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
............
#234 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#235 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#236 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#237 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#238 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#239 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#240 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#241 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#242 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#243 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#244 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#245 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#246 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#247 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#248 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#249 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#250 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#251 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#252 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#253 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#254 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
#255 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&&)::operator()<'lambda'(clang::QualType, auto&&)&>(clang::QualType, auto&&) const (.isra.0) SemaDeclCXX.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Linrz (Strke)

<details>
Reproducer: https://gcc.godbolt.org/z/dj3765YPn (assertions trunk）
```
#include &lt;iostream&gt;
class Base;
class Derived;
class DerivedClass2;
class Base
{
public:
    Base()  = default;
    Base(const Base&amp;) {}
    virtual ~Base()  = default;
    virtual bool operator==(const Derived&amp;) const noexcept {}
    virtual bool operator&lt;(const Base&amp;) const noexcept {}

protected:
    DerivedClass2 obj;
};
class Derived : public Base
{
public:
    bool operator==(const Derived&amp;) const = default;
    bool operator&lt;(const Base&amp;) const = default;
};
class DerivedClass2 : public Derived
{
public:
    bool operator==(const Derived&amp;) const = default;
    bool operator&lt;(const Base&amp;) const = default;
};
class DerivedClass : public Derived
{
public:
    bool operator==(const Derived&amp;) const = default;
    bool operator&lt;(const Base&amp;) const = default;
protected:
    Base obj(const Derived&amp;);
};
DerivedClass2 obj{};
int main()
{
    std::cout &lt;&lt; "ok";
}
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20240909/lib/clang/20 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20240909/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include --std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-338b0c.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:34:2: current parser token ';'
2.	&lt;source&gt;:29:1: parsing struct/union/class body 'DerivedClass2'
  #<!-- -->0 0x0000000003b258d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x3b258d8)
  #<!-- -->1 0x0000000003b2301c SignalHandler(int) Signals.cpp:0:0
  #<!-- -->2 0x0000725aa1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->3 0x0000000001abfe06 _ZN4llvm7hashing6detail23hash_combine_range_implIKjEENSt9enable_ifIXsrNS1_16is_hashable_dataIT_EE5valueENS_9hash_codeEE4typeEPS6_SB_ (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x1abfe06)
  #<!-- -->4 0x0000000003a78db1 llvm::FoldingSetBase::FindNodeOrInsertPos(llvm::FoldingSetNodeID const&amp;, void*&amp;, llvm::FoldingSetBase::FoldingSetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x3a78db1)
  #<!-- -->5 0x000000000793da12 clang::DeclarationNameTable::getCXXDestructorName(clang::CanQual&lt;clang::Type&gt;) (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x793da12)
  #<!-- -->6 0x0000000007954f58 clang::CXXRecordDecl::getDestructor() const (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x7954f58)
  #<!-- -->7 0x0000000007957659 clang::CXXRecordDecl::hasConstexprDestructor() const (/opt/compiler-explorer/clang-assertions-trunk-20240909/bin/clang-20+0x7957659)
  #<!-- -->8 0x0000000006a02d28 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
  #<!-- -->9 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
 #<!-- -->10 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
............
#<!-- -->234 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->235 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->236 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->237 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->238 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->239 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->240 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->241 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->242 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->243 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->244 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->245 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->246 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->247 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->248 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->249 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->250 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->251 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->252 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->253 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->254 0x0000000006a02df9 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
#<!-- -->255 0x0000000006a02e64 bool clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const::'lambda'(clang::QualType, auto&amp;&amp;)::operator()&lt;'lambda'(clang::QualType, auto&amp;&amp;)&amp;&gt;(clang::QualType, auto&amp;&amp;) const (.isra.0) SemaDeclCXX.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
</details>


---

### Comment 2 - shafik

This looks like a regression since clang-18 does not `SIGSEGV` on this: https://gcc.godbolt.org/z/Mjs1bnoe6

Regressions are usually good catches. 

---

### Comment 3 - mariete1223

Programs that gives same error:

```
#include <iostream>
#include <vector>
using namespace std;
template<typename T> struct B { T x; };
template<typename T1, typename T2> struct D
{
        B<D<T1, T2>> y;
        T1 x;
};
int main()
{
        f(*(D<vector<B<vector<int>>>, B<vector<B<vector<int>>>>>*)nullptr);
return 0;
}
```

---

### Comment 4 - shafik

This does not crash on trunk anymore.

---

