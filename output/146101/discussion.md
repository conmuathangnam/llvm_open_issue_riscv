# [clang-query] Crash when dumping `catch(...)` stmts when using IgnoreUnlessSpelledInSource traversal

**Author:** JonathanMarriott
**URL:** https://github.com/llvm/llvm-project/issues/146101
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-06-30T14:48:27Z

## Body

A minimal cpp file to reproduce the issue is
```cpp
// example.cpp
int main()
{
    try
    {
        return 0;
    }
    catch (...)
    {
        return 1;
    }
}
```
Then passing clang-query the commands
```
# cmd.txt
set output dump
set traversal IgnoreUnlessSpelledInSource

match stmt()
```
Invoking `clang-query example.cpp -p <build_dir> -f cmd.txt` will seg fault with stack trace 
```
Match #1:

Binding for "root":
CompoundStmt 0x12014a488 </Users/jmarriott13/code/oss/llvm-project/bug/example.cpp:2:1, line:11:1>
`-CXXTryStmt 0x12014a468 <line:3:5, line:10:5>
  |-CompoundStmt 0x12012b7d8 <line:4:5, line:6:5>
  | `-ReturnStmt 0x12012b7c8 <line:5:9, col:16>
  |   `-IntegerLiteral 0x12012b7a8 <col:16> 'int' 0
  `-CXXCatchStmt 0x12014a448 <line:7:5, line:10:5> #0 0x000000010482ff18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100213f18)
 #1 0x0000000104830510 PrintStackTraceSignalHandler(void*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100214510)
 #2 0x000000010482e13c llvm::sys::RunSignalHandlers() (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10021213c)
 #3 0x0000000104831990 SignalHandler(int, __siginfo*, void*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100215990)
 #4 0x0000000196a18624 (/usr/lib/system/libsystem_platform.dylib+0x1804ac624)
 #5 0x0000000104acbb48 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004afb48)
 #6 0x0000000104b5b32c clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::VisitCXXCatchStmt(clang::CXXCatchStmt const*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10053f32c)
 #7 0x0000000104b56d48 clang::StmtVisitorBase<llvm::make_const_ptr, clang::ASTDumper, void>::Visit(clang::Stmt const*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10053ad48)
 #8 0x0000000104b55770 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()::operator()() const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100539770)
 #9 0x0000000104b63d9c void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547d9c)
#10 0x0000000104b63c60 decltype(std::declval<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>()(std::declval<bool>())) std::__1::__invoke[abi:nn190102]<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool>(clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&&, bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c60)
#11 0x0000000104b63c08 void std::__1::__invoke_void_return_wrapper<void, true>::__call[abi:nn190102]<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool>(void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c08)
#12 0x0000000104b63bdc std::__1::__function::__alloc_func<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)>, void (bool)>::operator()[abi:nn190102](bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547bdc)
#13 0x0000000104b62b3c std::__1::__function::__func<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)>, void (bool)>::operator()(bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100546b3c)
#14 0x0000000104acdf20 std::__1::__function::__value_func<void (bool)>::operator()[abi:nn190102](bool&&) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1f20)
#15 0x0000000104acdbb0 std::__1::function<void (bool)>::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1bb0)
#16 0x0000000104b63ddc void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547ddc)
#17 0x0000000104b63c60 decltype(std::declval<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>()(std::declval<bool>())) std::__1::__invoke[abi:nn190102]<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool>(clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&&, bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c60)
#18 0x0000000104b63c08 void std::__1::__invoke_void_return_wrapper<void, true>::__call[abi:nn190102]<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool>(void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&, bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c08)
#19 0x0000000104b63bdc std::__1::__function::__alloc_func<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)>, void (bool)>::operator()[abi:nn190102](bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547bdc)
#20 0x0000000104b62b3c std::__1::__function::__func<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator<void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)>, void (bool)>::operator()(bool&&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100546b3c)
#21 0x0000000104acdf20 std::__1::__function::__value_func<void (bool)>::operator()[abi:nn190102](bool&&) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1f20)
#22 0x0000000104acdbb0 std::__1::function<void (bool)>::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1bb0)
#23 0x0000000104b55598 void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100539598)
#24 0x0000000104acc020 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Stmt const*, llvm::StringRef) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b0020)
#25 0x00000001063e8730 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::DynTypedNode const&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dcc730)
#26 0x00000001063e8174 clang::query::MatchQuery::run(llvm::raw_ostream&, clang::query::QuerySession&) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dcc174)
#27 0x00000001063e8db8 clang::query::FileQuery::run(llvm::raw_ostream&, clang::query::QuerySession&) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dccdb8)
#28 0x000000010461e530 runCommandsInFile(char const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, clang::query::QuerySession&) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100002530)
#29 0x000000010461eae8 main (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100002ae8)
#30 0x000000019663eb98 
zsh: segmentation fault  ../build/bin/clang-query example.cpp -p build -f cmd.txt
```

Notably using AsIs traversal is not affected by this issue
```
# cmd_as_is.txt
set output dump
set traversal AsIs

match stmt()
```
``clang-query example.cpp -p <build_dir> -f cmd_as_is.txt`` does not not seg fault

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jonathan Marriott (JonathanMarriott)

<details>
A minimal cpp file to reproduce the issue is
```cpp
// example.cpp
int main()
{
    try
    {
        return 0;
    }
    catch (...)
    {
        return 1;
    }
}
```
Then passing clang-query the commands
```
# cmd.txt
set output dump
set traversal IgnoreUnlessSpelledInSource

match stmt()
```
Invoking `clang-query example.cpp -p &lt;build_dir&gt; -f cmd.txt` will seg fault with stack trace 
```
Match #<!-- -->1:

Binding for "root":
CompoundStmt 0x12014a488 &lt;/Users/jmarriott13/code/oss/llvm-project/bug/example.cpp:2:1, line:11:1&gt;
`-CXXTryStmt 0x12014a468 &lt;line:3:5, line:10:5&gt;
  |-CompoundStmt 0x12012b7d8 &lt;line:4:5, line:6:5&gt;
  | `-ReturnStmt 0x12012b7c8 &lt;line:5:9, col:16&gt;
  |   `-IntegerLiteral 0x12012b7a8 &lt;col:16&gt; 'int' 0
  `-CXXCatchStmt 0x12014a448 &lt;line:7:5, line:10:5&gt; #<!-- -->0 0x000000010482ff18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100213f18)
 #<!-- -->1 0x0000000104830510 PrintStackTraceSignalHandler(void*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100214510)
 #<!-- -->2 0x000000010482e13c llvm::sys::RunSignalHandlers() (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10021213c)
 #<!-- -->3 0x0000000104831990 SignalHandler(int, __siginfo*, void*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100215990)
 #<!-- -->4 0x0000000196a18624 (/usr/lib/system/libsystem_platform.dylib+0x1804ac624)
 #<!-- -->5 0x0000000104acbb48 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004afb48)
 #<!-- -->6 0x0000000104b5b32c clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::VisitCXXCatchStmt(clang::CXXCatchStmt const*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10053f32c)
 #<!-- -->7 0x0000000104b56d48 clang::StmtVisitorBase&lt;llvm::make_const_ptr, clang::ASTDumper, void&gt;::Visit(clang::Stmt const*) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x10053ad48)
 #<!-- -->8 0x0000000104b55770 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()::operator()() const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100539770)
 #<!-- -->9 0x0000000104b63d9c void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547d9c)
#<!-- -->10 0x0000000104b63c60 decltype(std::declval&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;()(std::declval&lt;bool&gt;())) std::__1::__invoke[abi:nn190102]&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&gt;(clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&amp;&amp;, bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c60)
#<!-- -->11 0x0000000104b63c08 void std::__1::__invoke_void_return_wrapper&lt;void, true&gt;::__call[abi:nn190102]&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&gt;(void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c08)
#<!-- -->12 0x0000000104b63bdc std::__1::__function::__alloc_func&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&gt;, void (bool)&gt;::operator()[abi:nn190102](bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547bdc)
#<!-- -->13 0x0000000104b62b3c std::__1::__function::__func&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&gt;, void (bool)&gt;::operator()(bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100546b3c)
#<!-- -->14 0x0000000104acdf20 std::__1::__function::__value_func&lt;void (bool)&gt;::operator()[abi:nn190102](bool&amp;&amp;) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1f20)
#<!-- -->15 0x0000000104acdbb0 std::__1::function&lt;void (bool)&gt;::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1bb0)
#<!-- -->16 0x0000000104b63ddc void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547ddc)
#<!-- -->17 0x0000000104b63c60 decltype(std::declval&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;()(std::declval&lt;bool&gt;())) std::__1::__invoke[abi:nn190102]&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&gt;(clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&amp;&amp;, bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c60)
#<!-- -->18 0x0000000104b63c08 void std::__1::__invoke_void_return_wrapper&lt;void, true&gt;::__call[abi:nn190102]&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&gt;(void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&amp;, bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547c08)
#<!-- -->19 0x0000000104b63bdc std::__1::__function::__alloc_func&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&gt;, void (bool)&gt;::operator()[abi:nn190102](bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100547bdc)
#<!-- -->20 0x0000000104b62b3c std::__1::__function::__func&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool), std::__1::allocator&lt;void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'())::'lambda'(bool)&gt;, void (bool)&gt;::operator()(bool&amp;&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100546b3c)
#<!-- -->21 0x0000000104acdf20 std::__1::__function::__value_func&lt;void (bool)&gt;::operator()[abi:nn190102](bool&amp;&amp;) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1f20)
#<!-- -->22 0x0000000104acdbb0 std::__1::function&lt;void (bool)&gt;::operator()(bool) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b1bb0)
#<!-- -->23 0x0000000104b55598 void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef)::'lambda'()) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100539598)
#<!-- -->24 0x0000000104acc020 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Stmt const*, llvm::StringRef) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x1004b0020)
#<!-- -->25 0x00000001063e8730 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::DynTypedNode const&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dcc730)
#<!-- -->26 0x00000001063e8174 clang::query::MatchQuery::run(llvm::raw_ostream&amp;, clang::query::QuerySession&amp;) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dcc174)
#<!-- -->27 0x00000001063e8db8 clang::query::FileQuery::run(llvm::raw_ostream&amp;, clang::query::QuerySession&amp;) const (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x101dccdb8)
#<!-- -->28 0x000000010461e530 runCommandsInFile(char const*, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt; const&amp;, clang::query::QuerySession&amp;) (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100002530)
#<!-- -->29 0x000000010461eae8 main (/Users/jmarriott13/code/oss/llvm-project/build/bin/clang-query+0x100002ae8)
#<!-- -->30 0x000000019663eb98 
zsh: segmentation fault  ../build/bin/clang-query example.cpp -p build -f cmd.txt
```

Notably using AsIs traversal is not affected by this issue
```
# cmd_as_is.txt
set output dump
set traversal AsIs

match stmt()
```
``clang-query example.cpp -p &lt;build_dir&gt; -f cmd_as_is.txt`` does not not seg fault
</details>


---

