# clang++ 21 crash in clang::CXXRecordDecl::defaultedDestructorIsConstexpr

**Author:** apache-hb
**URL:** https://github.com/llvm/llvm-project/issues/135506
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-04-16T12:09:00Z

## Body

```
elliothb@ELLIOT-SERVER:~/github/bezos/build/packages/kernel$ /home/elliothb/github/bezos/install/llvm/bin/clang++ bug.cpp 
In file included from /home/elliothb/github/bezos/sources/kernel/test/system/device.cpp:1:
In file included from /home/elliothb/github/bezos/sources/kernel/test/system/system_test.hpp:4:
In file included from ../../../sources/kernel/include/system/system.hpp:6:
In file included from ../../../sources/kernel/include/std/vector.hpp:5:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/memory:943:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocator.h:17:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocate_at_least.h:14:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocator_traits.h:17:
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:3: error: unknown type name 'requires'
  260 |   requires(__has_pointer<_Tp>::value)
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:32: error: no member named 'value' in '__has_pointer<type-parameter-0-0, void>'
  260 |   requires(__has_pointer<_Tp>::value)
      |            ~~~~~~~~~~~~~~~~~~~~^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:38: error: expected ';' after top level declarator
  260 |   requires(__has_pointer<_Tp>::value)
      |                                      ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:261:21: error: use of undeclared identifier '_Tp'
  261 | struct __pointer_of<_Tp> {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:266:3: error: a type specifier is required for all declarations
  266 |   requires(!__has_pointer<_Tp>::value && __has_element_type<_Tp>::value)
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:266:73: error: expected ';' after top level declarator
  266 |   requires(!__has_pointer<_Tp>::value && __has_element_type<_Tp>::value)
      |                                                                         ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:267:21: error: use of undeclared identifier '_Tp'
  267 | struct __pointer_of<_Tp> {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:272:3: error: a type specifier is required for all declarations
  272 |   requires(!__has_pointer<_Tp>::value && !__has_element_type<_Tp>::value &&
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:273:59: error: expected ';' after top level declarator
  273 |            __has_element_type<pointer_traits<_Tp>>::value)
      |                                                           ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:274:21: error: use of undeclared identifier '_Tp'
  274 | struct __pointer_of<_Tp> {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:3: error: unknown type name 'requires'
  287 |   requires requires { typename __pointer_of_t<_Tp>; }
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:32: error: expected a qualified name after 'typename'
  287 |   requires requires { typename __pointer_of_t<_Tp>; }
      |                                ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:51: error: expected '(' for function-style cast or type construction
  287 |   requires requires { typename __pointer_of_t<_Tp>; }
      |                                ~~~~~~~~~~~~~~~~~~~^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:54: error: expected ';' after top level declarator
  287 |   requires requires { typename __pointer_of_t<_Tp>; }
      |                                                      ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:288:24: error: use of undeclared identifier '_Tp'
  288 | struct __pointer_of_or<_Tp, _Up> {
      |                        ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:296:1: error: unknown type name 'concept'
  296 | concept __resettable_smart_pointer = requires(_Smart __s) { __s.reset(); };
      | ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:296:47: error: '_Smart' does not refer to a value
  296 | concept __resettable_smart_pointer = requires(_Smart __s) { __s.reset(); };
      |                                               ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:295:17: note: declared here
  295 | template <class _Smart>
      |                 ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:299:1: error: unknown type name 'concept'
  299 | concept __resettable_smart_pointer_with_args = requires(_Smart __s, _Pointer __p, _Args... __args) {
      | ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:299:57: error: '_Smart' does not refer to a value
  299 | concept __resettable_smart_pointer_with_args = requires(_Smart __s, _Pointer __p, _Args... __args) {
      |                                                         ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:298:17: note: declared here
  298 | template <class _Smart, class _Pointer, class... _Args>
      |                 ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/elliothb/github/bezos/install/llvm/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/elliothb/github/bezos/build/packages/kernel -fcoverage-compilation-dir=/home/elliothb/github/bezos/build/packages/kernel -resource-dir /home/elliothb/github/bezos/install/llvm/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /home/elliothb/github/bezos/install/llvm/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-36fd83.o -x c++ bug.cpp
1.      <eof> parser at end of file
2.      ../../../sources/kernel/include/std/rcuptr.hpp:7:1: parsing namespace 'sm'
3.      ../../../sources/kernel/include/std/rcuptr.hpp:148:5: parsing struct/union/class body 'sm::RcuSharedPtr'
4.      ../../../sources/kernel/include/std/rcuptr.hpp:198:35: parsing function body 'sm::RcuSharedPtr::~RcuSharedPtr<T>'
 #0 0x000055ca00edb804 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x16c0804)
 #1 0x000055ca00edbd08 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fa57681c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000055ca02267594 clang::CXXRecordDecl::defaultedDestructorIsConstexpr() const (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a4c594)
 #4 0x000055ca0229015f CheckConstexprDestructorSubobjects(clang::Sema&, clang::CXXDestructorDecl const*, clang::Sema::CheckConstexprKind)::$_0::operator()(clang::SourceLocation, clang::QualType, clang::FieldDecl const*) const SemaDeclCXX.cpp:0:0
 #5 0x000055ca02266f7a clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a4bf7a)
 #6 0x000055ca0222b73f clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a1073f)
 #7 0x000055ca020ff92d clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e492d)
 #8 0x000055ca0208625a clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x286b25a)
 #9 0x000055ca020855d0 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x286a5d0)
#10 0x000055ca020abbda clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2890bda)
#11 0x000055ca020aa067 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x288f067)
#12 0x000055ca02093c68 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2878c68)
#13 0x000055ca021045c9 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e95c9)
#14 0x000055ca02103dc1 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e8dc1)
#15 0x000055ca0210387f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e887f)
#16 0x000055ca0208f9a6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28749a6)
#17 0x000055ca02079985 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285e985)
#18 0x000055ca020a4a5e clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2889a5e)
#19 0x000055ca020a45e0 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28895e0)
#20 0x000055ca0208fa4e clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2874a4e)
#21 0x000055ca02079985 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285e985)
#22 0x000055ca02078a8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285da8d)
#23 0x000055ca02075dca clang::ParseAST(clang::Sema&, bool, bool) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285adca)
#24 0x000055ca014f063d clang::FrontendAction::Execute() (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1cd563d)
#25 0x000055ca0149bfc7 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1c80fc7)
#26 0x000055ca0156f895 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1d54895)
#27 0x000055ca0048b447 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc70447)
#28 0x000055ca00488fd9 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x000055ca0048885d clang_main(int, char**, llvm::ToolContext const&) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc6d85d)
#30 0x000055ca00492388 main (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc77388)
#31 0x00007fa576803d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007fa576803e40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007fa576803e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000055ca00487995 _start (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc6c995)
```
[bug-4930ea.cpp.txt](https://github.com/user-attachments/files/19722189/bug-4930ea.cpp.txt)
[bug-4930ea.sh.txt](https://github.com/user-attachments/files/19722188/bug-4930ea.sh.txt)

## Comments

### Comment 1 - apache-hb

some extra context: i think mp_units is the root cause of this, i've had alot of crashes from mp_units when include files are missing.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Elliot (apache-hb)

<details>
```
elliothb@<!-- -->ELLIOT-SERVER:~/github/bezos/build/packages/kernel$ /home/elliothb/github/bezos/install/llvm/bin/clang++ bug.cpp 
In file included from /home/elliothb/github/bezos/sources/kernel/test/system/device.cpp:1:
In file included from /home/elliothb/github/bezos/sources/kernel/test/system/system_test.hpp:4:
In file included from ../../../sources/kernel/include/system/system.hpp:6:
In file included from ../../../sources/kernel/include/std/vector.hpp:5:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/memory:943:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocator.h:17:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocate_at_least.h:14:
In file included from /home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/allocator_traits.h:17:
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:3: error: unknown type name 'requires'
  260 |   requires(__has_pointer&lt;_Tp&gt;::value)
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:32: error: no member named 'value' in '__has_pointer&lt;type-parameter-0-0, void&gt;'
  260 |   requires(__has_pointer&lt;_Tp&gt;::value)
      |            ~~~~~~~~~~~~~~~~~~~~^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:260:38: error: expected ';' after top level declarator
  260 |   requires(__has_pointer&lt;_Tp&gt;::value)
      |                                      ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:261:21: error: use of undeclared identifier '_Tp'
  261 | struct __pointer_of&lt;_Tp&gt; {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:266:3: error: a type specifier is required for all declarations
  266 |   requires(!__has_pointer&lt;_Tp&gt;::value &amp;&amp; __has_element_type&lt;_Tp&gt;::value)
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:266:73: error: expected ';' after top level declarator
  266 |   requires(!__has_pointer&lt;_Tp&gt;::value &amp;&amp; __has_element_type&lt;_Tp&gt;::value)
      |                                                                         ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:267:21: error: use of undeclared identifier '_Tp'
  267 | struct __pointer_of&lt;_Tp&gt; {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:272:3: error: a type specifier is required for all declarations
  272 |   requires(!__has_pointer&lt;_Tp&gt;::value &amp;&amp; !__has_element_type&lt;_Tp&gt;::value &amp;&amp;
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:273:59: error: expected ';' after top level declarator
  273 |            __has_element_type&lt;pointer_traits&lt;_Tp&gt;&gt;::value)
      |                                                           ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:274:21: error: use of undeclared identifier '_Tp'
  274 | struct __pointer_of&lt;_Tp&gt; {
      |                     ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:3: error: unknown type name 'requires'
  287 |   requires requires { typename __pointer_of_t&lt;_Tp&gt;; }
      |   ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:32: error: expected a qualified name after 'typename'
  287 |   requires requires { typename __pointer_of_t&lt;_Tp&gt;; }
      |                                ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:51: error: expected '(' for function-style cast or type construction
  287 |   requires requires { typename __pointer_of_t&lt;_Tp&gt;; }
      |                                ~~~~~~~~~~~~~~~~~~~^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:287:54: error: expected ';' after top level declarator
  287 |   requires requires { typename __pointer_of_t&lt;_Tp&gt;; }
      |                                                      ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:288:24: error: use of undeclared identifier '_Tp'
  288 | struct __pointer_of_or&lt;_Tp, _Up&gt; {
      |                        ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:296:1: error: unknown type name 'concept'
  296 | concept __resettable_smart_pointer = requires(_Smart __s) { __s.reset(); };
      | ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:296:47: error: '_Smart' does not refer to a value
  296 | concept __resettable_smart_pointer = requires(_Smart __s) { __s.reset(); };
      |                                               ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:295:17: note: declared here
  295 | template &lt;class _Smart&gt;
      |                 ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:299:1: error: unknown type name 'concept'
  299 | concept __resettable_smart_pointer_with_args = requires(_Smart __s, _Pointer __p, _Args... __args) {
      | ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:299:57: error: '_Smart' does not refer to a value
  299 | concept __resettable_smart_pointer_with_args = requires(_Smart __s, _Pointer __p, _Args... __args) {
      |                                                         ^
/home/elliothb/github/bezos/install/llvm-libcxx/include/c++/v1/__memory/pointer_traits.h:298:17: note: declared here
  298 | template &lt;class _Smart, class _Pointer, class... _Args&gt;
      |                 ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/elliothb/github/bezos/install/llvm/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/elliothb/github/bezos/build/packages/kernel -fcoverage-compilation-dir=/home/elliothb/github/bezos/build/packages/kernel -resource-dir /home/elliothb/github/bezos/install/llvm/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /home/elliothb/github/bezos/install/llvm/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-36fd83.o -x c++ bug.cpp
1.      &lt;eof&gt; parser at end of file
2.      ../../../sources/kernel/include/std/rcuptr.hpp:7:1: parsing namespace 'sm'
3.      ../../../sources/kernel/include/std/rcuptr.hpp:148:5: parsing struct/union/class body 'sm::RcuSharedPtr'
4.      ../../../sources/kernel/include/std/rcuptr.hpp:198:35: parsing function body 'sm::RcuSharedPtr::~RcuSharedPtr&lt;T&gt;'
 #<!-- -->0 0x000055ca00edb804 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x16c0804)
 #<!-- -->1 0x000055ca00edbd08 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fa57681c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000055ca02267594 clang::CXXRecordDecl::defaultedDestructorIsConstexpr() const (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a4c594)
 #<!-- -->4 0x000055ca0229015f CheckConstexprDestructorSubobjects(clang::Sema&amp;, clang::CXXDestructorDecl const*, clang::Sema::CheckConstexprKind)::$_0::operator()(clang::SourceLocation, clang::QualType, clang::FieldDecl const*) const SemaDeclCXX.cpp:0:0
 #<!-- -->5 0x000055ca02266f7a clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a4bf7a)
 #<!-- -->6 0x000055ca0222b73f clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2a1073f)
 #<!-- -->7 0x000055ca020ff92d clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e492d)
 #<!-- -->8 0x000055ca0208625a clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x286b25a)
 #<!-- -->9 0x000055ca020855d0 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x286a5d0)
#<!-- -->10 0x000055ca020abbda clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2890bda)
#<!-- -->11 0x000055ca020aa067 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x288f067)
#<!-- -->12 0x000055ca02093c68 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2878c68)
#<!-- -->13 0x000055ca021045c9 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e95c9)
#<!-- -->14 0x000055ca02103dc1 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e8dc1)
#<!-- -->15 0x000055ca0210387f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28e887f)
#<!-- -->16 0x000055ca0208f9a6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28749a6)
#<!-- -->17 0x000055ca02079985 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285e985)
#<!-- -->18 0x000055ca020a4a5e clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2889a5e)
#<!-- -->19 0x000055ca020a45e0 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x28895e0)
#<!-- -->20 0x000055ca0208fa4e clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x2874a4e)
#<!-- -->21 0x000055ca02079985 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285e985)
#<!-- -->22 0x000055ca02078a8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285da8d)
#<!-- -->23 0x000055ca02075dca clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x285adca)
#<!-- -->24 0x000055ca014f063d clang::FrontendAction::Execute() (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1cd563d)
#<!-- -->25 0x000055ca0149bfc7 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1c80fc7)
#<!-- -->26 0x000055ca0156f895 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0x1d54895)
#<!-- -->27 0x000055ca0048b447 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc70447)
#<!-- -->28 0x000055ca00488fd9 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x000055ca0048885d clang_main(int, char**, llvm::ToolContext const&amp;) (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc6d85d)
#<!-- -->30 0x000055ca00492388 main (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc77388)
#<!-- -->31 0x00007fa576803d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->32 0x00007fa576803e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007fa576803e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->34 0x000055ca00487995 _start (/home/elliothb/github/bezos/install/llvm/bin/clang-21+0xc6c995)
```
[bug-4930ea.cpp.txt](https://github.com/user-attachments/files/19722189/bug-4930ea.cpp.txt)
[bug-4930ea.sh.txt](https://github.com/user-attachments/files/19722188/bug-4930ea.sh.txt)
</details>


---

### Comment 3 - tbaederr

We're passing this function to `CheckConstexprFunctionDefinition`:
```
FunctionDecl 0x7bff7cc4a8b8 parent 0x7bff7cc4a220 <../../../sources/kernel/subprojects/mp-units-2.4.0/src/core/include/mp-units/framework/unit.h:159:17, col:60> col:44 consteval friend_undeclared operator* 'Unit auto (M, U)' delete implicit-inline
|-delete message: StringLiteral 0x7bff7cc4b5c0 <line:174:14> 'NULL TYPE' lvalue "To scale a unit use `mag * unit` syntax"
|-ParmVarDecl 0x7bff7cc4a630 <line:159:54> col:55 'M'
|-ParmVarDecl 0x7bff7cc4a6b8 <col:57, col:59> col:59 referenced u 'U'
`-WarnUnusedResultAttr 0x7bff7cc4aa18 <col:5> nodiscard ""
```
which doesn't have a body, so the assertion in `CheckConstexprFunctionDefinition` fails. In `ActOnFinishFunctionBody`, we check that `Body` isn't null, but not that `FD->getBody()` isn't null. That also has this comment:

```c++
     // If this is called by Parser::ParseFunctionDefinition() after marking
      // the declaration as deleted, and if the deleted-function-body contains
      // a message (C++26), then a DefaultedOrDeletedInfo will have already been
      // added to store that message; do not overwrite it in that case.
      //
      // Since this would always set the body to 'nullptr' in that case anyway,
      // which is already done when the function decl is initially created,
      // always skipping this irrespective of whether there is a delete message
      // should not be a problem.
      if (!FD->isDeletedAsWritten())
        FD->setBody(Body);
```
so we don't set the body on `FD` in this case. CC @Sirraide who wrote that comment.


---

### Comment 4 - Sirraide

Reduced:
```c++
struct unit_interface {
  friend consteval int f() {}
  friend consteval int f() = delete;
};
```


---

### Comment 5 - Sirraide

In this case it makes sense that the function both has a body and is deleted... because it’s a redeclaration. I’ll take a look at fixing this.

---

### Comment 6 - Sirraide

Ok, so this code
```c++
struct S {
    friend int f() { return 3; }
    friend int f() = delete;
};
```
has been crashing since I added support for `= delete("message")`, but the actual problem here is that even before that we... weren’t diagnosing this, at all. I think it’s the latter that needs to be fixed here to resolve this.

---

