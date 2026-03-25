# placement new

**Author:** ordinary-github-user
**URL:** https://github.com/llvm/llvm-project/issues/136518

## Body

https://godbolt.org/z/qo1nKq4qb
`-std=c++26 -O3 -Wall -mavx512bw -mbmi -fno-exceptions`
```c++
#include <memory>
#include <cstdint>
#include <utility>

template<class T, std::size_t N>
struct StaticVector
{
	union { T m_data[N]; };
	std::uint8_t m_size;

	constexpr StaticVector(auto&&...args)noexcept :m_size{ sizeof...(args) }
	{
		std::construct_at(&m_data);
	}
	constexpr void push_back(const T x)noexcept
	{
		new(m_data+m_size)T{x};//remove this line, then it compiles
	}
};

static_assert([]
{
	StaticVector<std::uint8_t, 2> vec{1};
	vec.push_back(1);
	return true;
}());
```

```
free(): invalid next size (fast)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O3 -Wall -mavx512bw -mbmi -fno-exceptions <source>
1.	<eof> parser at end of file
 #0 0x0000000003be2778 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be2778)
 #1 0x0000000003be08bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be08bc)
 #2 0x0000000003b311c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000796790242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007967902969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000796790242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007967902287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000796790289676 (/lib/x86_64-linux-gnu/libc.so.6+0x89676)
 #8 0x00007967902a0cfc (/lib/x86_64-linux-gnu/libc.so.6+0xa0cfc)
 #9 0x00007967902a2a9d (/lib/x86_64-linux-gnu/libc.so.6+0xa2a9d)
#10 0x00007967902a5453 __libc_free (/lib/x86_64-linux-gnu/libc.so.6+0xa5453)
#11 0x0000000006d90ecb clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d90ecb)
#12 0x00000000062ee589 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#13 0x00000000062eed1d clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62eed1d)
#14 0x0000000006153cb3 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6153cb3)
#15 0x000000000614207a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614207a)
#16 0x000000000451cebd clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451cebd)
#17 0x000000000482b4fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x482b4fa)
#18 0x00000000047a7c4b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a7c4b)
#19 0x000000000491b313 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491b313)
#20 0x0000000000db1451 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb1451)
#21 0x0000000000da9bad ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#22 0x000000000458e8f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#23 0x0000000003b315e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b315e3)
#24 0x000000000458eb19 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#25 0x00000000045520cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45520cd)
#26 0x0000000004553091 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4553091)
#27 0x000000000455d3dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455d3dc)
#28 0x0000000000dae181 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdae181)
#29 0x0000000000c2fec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc2fec4)
#30 0x0000796790229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#31 0x0000796790229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#32 0x0000000000da9645 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda9645)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ordinary-github-user)

<details>
https://godbolt.org/z/qo1nKq4qb
`-std=c++26 -O3 -Wall -mavx512bw -mbmi -fno-exceptions`
```c++
#include &lt;memory&gt;
#include &lt;cstdint&gt;
#include &lt;utility&gt;

template&lt;class T, std::size_t N&gt;
struct StaticVector
{
	union { T m_data[N]; };
	std::uint8_t m_size;

	constexpr StaticVector(auto&amp;&amp;...args)noexcept :m_size{ sizeof...(args) }
	{
		std::construct_at(&amp;m_data);
	}
	constexpr void push_back(const T x)noexcept
	{
		new(m_data+m_size)T{x};//remove this line, then it compiles
	}
};

static_assert([]
{
	StaticVector&lt;std::uint8_t, 2&gt; vec{1};
	vec.push_back(1);
	return true;
}());
```

```
free(): invalid next size (fast)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O3 -Wall -mavx512bw -mbmi -fno-exceptions &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003be2778 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be2778)
 #<!-- -->1 0x0000000003be08bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be08bc)
 #<!-- -->2 0x0000000003b311c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000796790242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007967902969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000796790242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007967902287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000796790289676 (/lib/x86_64-linux-gnu/libc.so.6+0x89676)
 #<!-- -->8 0x00007967902a0cfc (/lib/x86_64-linux-gnu/libc.so.6+0xa0cfc)
 #<!-- -->9 0x00007967902a2a9d (/lib/x86_64-linux-gnu/libc.so.6+0xa2a9d)
#<!-- -->10 0x00007967902a5453 __libc_free (/lib/x86_64-linux-gnu/libc.so.6+0xa5453)
#<!-- -->11 0x0000000006d90ecb clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d90ecb)
#<!-- -->12 0x00000000062ee589 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->13 0x00000000062eed1d clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62eed1d)
#<!-- -->14 0x0000000006153cb3 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6153cb3)
#<!-- -->15 0x000000000614207a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614207a)
#<!-- -->16 0x000000000451cebd clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451cebd)
#<!-- -->17 0x000000000482b4fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x482b4fa)
#<!-- -->18 0x00000000047a7c4b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a7c4b)
#<!-- -->19 0x000000000491b313 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491b313)
#<!-- -->20 0x0000000000db1451 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb1451)
#<!-- -->21 0x0000000000da9bad ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->22 0x000000000458e8f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x0000000003b315e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b315e3)
#<!-- -->24 0x000000000458eb19 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x00000000045520cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45520cd)
#<!-- -->26 0x0000000004553091 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4553091)
#<!-- -->27 0x000000000455d3dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455d3dc)
#<!-- -->28 0x0000000000dae181 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdae181)
#<!-- -->29 0x0000000000c2fec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc2fec4)
#<!-- -->30 0x0000796790229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x0000796790229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000000000da9645 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda9645)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


---

### Comment 2 - shafik

Simplified slightly: https://godbolt.org/z/jova9r9b1

```cpp
#include <memory>

template<class T, std::size_t N>
struct StaticVector {
	T m_data[N];

	constexpr StaticVector(auto&&...args)noexcept  {
		std::construct_at(&m_data);
	}

	constexpr void push_back(const T x)noexcept {
		new(m_data)T{x};//remove this line, then it compiles
	}
};

static_assert([]
{
	StaticVector<std::uint8_t, 2> vec{1};
	
	return true;
}());
```

---

### Comment 3 - shafik

I can reproduce this on godbolt but not locally on a current main. So this looks like it may have been fixed.

---

### Comment 4 - frederick-vs-ja

This is only triggered by libstdc++'s `std::construct_at` now. [LWG3436](https://cplusplus.github.io/LWG/issue3436) seems related.

Reduced example ([Godbolt link](https://godbolt.org/z/4cnEa6odq)):
```C++
using my_size_t = decltype(sizeof(0));
using my_u8_t = unsigned char;

[[__nodiscard__]] inline constexpr void*
operator new(my_size_t, void* p) noexcept {
  return p;
}
[[__nodiscard__]] inline constexpr void*
operator new[](my_size_t, void* p) noexcept {
  return p;
}
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}

template<class T, my_size_t N>
struct StaticVector
{
	union { T m_data[N]; };

	constexpr StaticVector(auto&&...args) noexcept
	{
        using Arr = T[N];
        ::new (&m_data) Arr[1]();
	}
	constexpr void push_back(const T x)noexcept
	{
		::new (m_data + 1) T{x};
	}
};

static_assert([]
{
    StaticVector<my_u8_t, 2> vec{1};
	vec.push_back(1);
	return true;
}());
```

---

