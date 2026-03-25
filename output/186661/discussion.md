# [clang-tidy] segfault

**Author:** ilobilo
**URL:** https://github.com/llvm/llvm-project/issues/186661
**Status:** Closed
**Labels:** duplicate, clang:static analyzer, incomplete, crash
**Closed Date:** 2026-03-16T18:16:27Z

## Body

```
/usr/bin/cmake -E __run_co_compile --tidy="/usr/bin/clang-tidy;--extra-arg-before=--driver-mode=g++" --source=/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++  -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH='"x86_64"' -DILOBILIX_SYSNAME='""' -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -D'ILOBILIX_VERSION="0.0.1"' -D'LIMINE_API_REVISION=2' -D'UACPI_OVERRIDE_LIBC' -D'UACPI_OVERRIDE_ARCH_HELPERS' -D'MAGIC_ENUM_NO_STREAMS=1' -D'MAGIC_ENUM_ENABLE_HASH=1' -D'FMT_THROW(x)=abort()' -D'FMT_USE_CONSTEVAL=1' -D'FMT_USE_CONSTEXPR_STRING=1' -D'FMT_OPTIMIZE_SIZE=2' -D'FMT_BUILTIN_TYPES=0' -D'FMT_FULLY_FREESTANDING=1' -D'__user=__attribute__((address_space(1)))' -D'__force=__attribute__((force))' -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/clang-tidy --extra-arg-before=--driver-mode=g++ /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++ -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH=\"x86_64\" -DILOBILIX_SYSNAME=\"\" -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -DILOBILIX_VERSION=\"0.0.1\" -DLIMINE_API_REVISION=2 -DUACPI_OVERRIDE_LIBC -DUACPI_OVERRIDE_ARCH_HELPERS -DMAGIC_ENUM_NO_STREAMS=1 -DMAGIC_ENUM_ENABLE_HASH=1 -DFMT_THROW(x)=abort() -DFMT_USE_CONSTEVAL=1 -DFMT_USE_CONSTEXPR_STRING=1 -DFMT_OPTIMIZE_SIZE=2 -DFMT_BUILTIN_TYPES=0 -DFMT_FULLY_FREESTANDING=1 -D__user=__attribute__((address_space(1))) -D__force=__attribute__((force)) -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
1.	<eof> parser at end of file
 #0 0x00007f8673acc569 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x50cc569)
 #1 0x00007f8673accfad (/lib64/libLLVM.so.21.1+0x50ccfad)
 #2 0x00007f866e427290 __restore_rt (/lib64/libc.so.6+0x1a290)
 #3 0x00007f86797a3937 clang::DynamicRecursiveASTVisitorBase<false>::TraverseTemplateInstantiations(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3937)
 #4 0x00007f86797a9e1a clang::DynamicRecursiveASTVisitorBase<false>::TraverseFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a9e1a)
 #5 0x00007f8678f60e07 clang::DynamicRecursiveASTVisitorBase<false>::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60e07)
 #6 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
 #7 0x00007f86797a59eb clang::DynamicRecursiveASTVisitorBase<false>::TraverseNamespaceDecl(clang::NamespaceDecl*) (/lib64/libclang-cpp.so.21.1+0x23a59eb)
 #8 0x00007f8678f610d1 clang::DynamicRecursiveASTVisitorBase<false>::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b610d1)
 #9 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#10 0x00007f86797a3ea8 clang::DynamicRecursiveASTVisitorBase<false>::TraverseLinkageSpecDecl(clang::LinkageSpecDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3ea8)
#11 0x00007f8678f60fe3 clang::DynamicRecursiveASTVisitorBase<false>::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fe3)
#12 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#13 0x00007f86797a3afe clang::DynamicRecursiveASTVisitorBase<false>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3afe)
#14 0x00007f8678f60fab clang::DynamicRecursiveASTVisitorBase<false>::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fab)
#15 0x00007f867adfa9ef (/lib64/libclang-cpp.so.21.1+0x39fa9ef)
#16 0x00007f867ac09615 clang::ento::CheckerManager::runCheckersOnASTDecl(clang::Decl const*, clang::ento::AnalysisManager&, clang::ento::BugReporter&) (/lib64/libclang-cpp.so.21.1+0x3809615)
#17 0x00007f867af0783f (/lib64/libclang-cpp.so.21.1+0x3b0783f)
#18 0x00007f867a9fb295 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x35fb295)
#19 0x00007f867890cbb2 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150cbb2)
#20 0x00007f86791c85bf clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc85bf)
#21 0x00007f86791c80d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x1dc80d0)
#22 0x00007f867ab68417 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x3768417)
#23 0x0000558c8e0401fc (/usr/bin/clang-tidy+0x21c1fc)
#24 0x00007f867ab682b7 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/lib64/libclang-cpp.so.21.1+0x37682b7)
#25 0x00007f867ab67260 clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x3767260)
#26 0x00007f867ab69a7c clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x3769a7c)
#27 0x0000558c8e03aad2 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x216ad2)
#28 0x0000558c8e022f38 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x1fef38)
#29 0x00007f866e4105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#30 0x00007f866e410668 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#31 0x0000558c8e01f055 _start (/usr/bin/clang-tidy+0x1fb055)
Segmentation fault
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: ilobilo (ilobilo)

<details>
```
/usr/bin/cmake -E __run_co_compile --tidy="/usr/bin/clang-tidy;--extra-arg-before=--driver-mode=g++" --source=/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++  -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH='"x86_64"' -DILOBILIX_SYSNAME='""' -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -D'ILOBILIX_VERSION="0.0.1"' -D'LIMINE_API_REVISION=2' -D'UACPI_OVERRIDE_LIBC' -D'UACPI_OVERRIDE_ARCH_HELPERS' -D'MAGIC_ENUM_NO_STREAMS=1' -D'MAGIC_ENUM_ENABLE_HASH=1' -D'FMT_THROW(x)=abort()' -D'FMT_USE_CONSTEVAL=1' -D'FMT_USE_CONSTEXPR_STRING=1' -D'FMT_OPTIMIZE_SIZE=2' -D'FMT_BUILTIN_TYPES=0' -D'FMT_FULLY_FREESTANDING=1' -D'__user=__attribute__((address_space(1)))' -D'__force=__attribute__((force))' -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/clang-tidy --extra-arg-before=--driver-mode=g++ /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++ -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH=\"x86_64\" -DILOBILIX_SYSNAME=\"\" -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -DILOBILIX_VERSION=\"0.0.1\" -DLIMINE_API_REVISION=2 -DUACPI_OVERRIDE_LIBC -DUACPI_OVERRIDE_ARCH_HELPERS -DMAGIC_ENUM_NO_STREAMS=1 -DMAGIC_ENUM_ENABLE_HASH=1 -DFMT_THROW(x)=abort() -DFMT_USE_CONSTEVAL=1 -DFMT_USE_CONSTEXPR_STRING=1 -DFMT_OPTIMIZE_SIZE=2 -DFMT_BUILTIN_TYPES=0 -DFMT_FULLY_FREESTANDING=1 -D__user=__attribute__((address_space(1))) -D__force=__attribute__((force)) -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00007f8673acc569 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x50cc569)
 #<!-- -->1 0x00007f8673accfad (/lib64/libLLVM.so.21.1+0x50ccfad)
 #<!-- -->2 0x00007f866e427290 __restore_rt (/lib64/libc.so.6+0x1a290)
 #<!-- -->3 0x00007f86797a3937 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3937)
 #<!-- -->4 0x00007f86797a9e1a clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a9e1a)
 #<!-- -->5 0x00007f8678f60e07 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60e07)
 #<!-- -->6 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
 #<!-- -->7 0x00007f86797a59eb clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseNamespaceDecl(clang::NamespaceDecl*) (/lib64/libclang-cpp.so.21.1+0x23a59eb)
 #<!-- -->8 0x00007f8678f610d1 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b610d1)
 #<!-- -->9 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#<!-- -->10 0x00007f86797a3ea8 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseLinkageSpecDecl(clang::LinkageSpecDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3ea8)
#<!-- -->11 0x00007f8678f60fe3 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fe3)
#<!-- -->12 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#<!-- -->13 0x00007f86797a3afe clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3afe)
#<!-- -->14 0x00007f8678f60fab clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fab)
#<!-- -->15 0x00007f867adfa9ef (/lib64/libclang-cpp.so.21.1+0x39fa9ef)
#<!-- -->16 0x00007f867ac09615 clang::ento::CheckerManager::runCheckersOnASTDecl(clang::Decl const*, clang::ento::AnalysisManager&amp;, clang::ento::BugReporter&amp;) (/lib64/libclang-cpp.so.21.1+0x3809615)
#<!-- -->17 0x00007f867af0783f (/lib64/libclang-cpp.so.21.1+0x3b0783f)
#<!-- -->18 0x00007f867a9fb295 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x35fb295)
#<!-- -->19 0x00007f867890cbb2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150cbb2)
#<!-- -->20 0x00007f86791c85bf clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc85bf)
#<!-- -->21 0x00007f86791c80d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x1dc80d0)
#<!-- -->22 0x00007f867ab68417 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x3768417)
#<!-- -->23 0x0000558c8e0401fc (/usr/bin/clang-tidy+0x21c1fc)
#<!-- -->24 0x00007f867ab682b7 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/lib64/libclang-cpp.so.21.1+0x37682b7)
#<!-- -->25 0x00007f867ab67260 clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x3767260)
#<!-- -->26 0x00007f867ab69a7c clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x3769a7c)
#<!-- -->27 0x0000558c8e03aad2 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x216ad2)
#<!-- -->28 0x0000558c8e022f38 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x1fef38)
#<!-- -->29 0x00007f866e4105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#<!-- -->30 0x00007f866e410668 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#<!-- -->31 0x0000558c8e01f055 _start (/usr/bin/clang-tidy+0x1fb055)
Segmentation fault
```
</details>


---

### Comment 2 - zeyi2

By looking at `clang::ento::`, this may be a CSA related issue?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: ilobilo (ilobilo)

<details>
```
/usr/bin/cmake -E __run_co_compile --tidy="/usr/bin/clang-tidy;--extra-arg-before=--driver-mode=g++" --source=/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++  -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH='"x86_64"' -DILOBILIX_SYSNAME='""' -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -D'ILOBILIX_VERSION="0.0.1"' -D'LIMINE_API_REVISION=2' -D'UACPI_OVERRIDE_LIBC' -D'UACPI_OVERRIDE_ARCH_HELPERS' -D'MAGIC_ENUM_NO_STREAMS=1' -D'MAGIC_ENUM_ENABLE_HASH=1' -D'FMT_THROW(x)=abort()' -D'FMT_USE_CONSTEVAL=1' -D'FMT_USE_CONSTEXPR_STRING=1' -D'FMT_OPTIMIZE_SIZE=2' -D'FMT_BUILTIN_TYPES=0' -D'FMT_FULLY_FREESTANDING=1' -D'__user=__attribute__((address_space(1)))' -D'__force=__attribute__((force))' -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/clang-tidy --extra-arg-before=--driver-mode=g++ /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm -- /usr/bin/clang++ -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/std/stubs -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/libc -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel -I/home/ilobilo/Documents/projects/ilobilix/kernel/kernel/include/kernel/uacpi -I/home/ilobilo/Documents/projects/ilobilix/build-x86_64/kernel/generated -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/compiler-rt-builtins/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/cwalk/cwalk/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/demangler/demangler/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/flanterm/flanterm/src -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/fmt/fmt/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/freestnd-c-hdrs/freestnd-c-hdrs/x86_64/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/frigg/frigg/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/libstdcxx-headers/libstdcxx-headers/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/limine-protocol/limine-protocol/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/magic_enum/magic_enum/include/magic_enum -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/parallel-hashmap/parallel-hashmap -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/string/string/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/uacpi/uACPI/include -I/home/ilobilo/Documents/projects/ilobilix/kernel/dependencies/veque/veque/include --target=x86_64-elf -fcolor-diagnostics -fmacro-prefix-map=/home/ilobilo/Documents/projects/ilobilix/kernel/= -ffreestanding -nostdinc -fno-omit-frame-pointer -fno-stack-protector -fstrict-vtable-pointers -funsigned-char -mgeneral-regs-only -Wall -Wextra -Wno-c23-extensions -Wno-c99-designator -Wno-unknown-attributes -Wno-unused-command-line-argument -Wno-deprecated-declarations -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel -mstack-alignment=8 -O3 -DILOBILIX_DEBUG=1 -g -DILOBILIX_ARCH=\"x86_64\" -DILOBILIX_SYSNAME=\"\" -DILOBILIX_SYSCALL_LOG=0 -DILOBILIX_LIMINE_MP=1 -DILOBILIX_UBSAN=0 -DILOBILIX_VERSION=\"0.0.1\" -DLIMINE_API_REVISION=2 -DUACPI_OVERRIDE_LIBC -DUACPI_OVERRIDE_ARCH_HELPERS -DMAGIC_ENUM_NO_STREAMS=1 -DMAGIC_ENUM_ENABLE_HASH=1 -DFMT_THROW(x)=abort() -DFMT_USE_CONSTEVAL=1 -DFMT_USE_CONSTEXPR_STRING=1 -DFMT_OPTIMIZE_SIZE=2 -DFMT_BUILTIN_TYPES=0 -DFMT_FULLY_FREESTANDING=1 -D__user=__attribute__((address_space(1))) -D__force=__attribute__((force)) -fno-rtti -fno-exceptions -fsized-deallocation -fcheck-new -D__cpp_lib_ranges_to_container=202202L -D__glibcxx_ranges_to_container=202202L -D__cpp_lib_constexpr_string=201907L -DKSYM_NAME_LEN=1024 -masm=intel -std=c++23 -fPIC -fcolor-diagnostics -MD -MT kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -MF kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.d @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap -o kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o -c /home/ilobilo/Documents/projects/ilobilix/kernel/kernel/interfaces/lib/ranged.cppm @<!-- -->kernel/interfaces/CMakeFiles/cxx_modules.dir/lib/ranged.cppm.o.modmap
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00007f8673acc569 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x50cc569)
 #<!-- -->1 0x00007f8673accfad (/lib64/libLLVM.so.21.1+0x50ccfad)
 #<!-- -->2 0x00007f866e427290 __restore_rt (/lib64/libc.so.6+0x1a290)
 #<!-- -->3 0x00007f86797a3937 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3937)
 #<!-- -->4 0x00007f86797a9e1a clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/lib64/libclang-cpp.so.21.1+0x23a9e1a)
 #<!-- -->5 0x00007f8678f60e07 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60e07)
 #<!-- -->6 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
 #<!-- -->7 0x00007f86797a59eb clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseNamespaceDecl(clang::NamespaceDecl*) (/lib64/libclang-cpp.so.21.1+0x23a59eb)
 #<!-- -->8 0x00007f8678f610d1 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b610d1)
 #<!-- -->9 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#<!-- -->10 0x00007f86797a3ea8 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseLinkageSpecDecl(clang::LinkageSpecDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3ea8)
#<!-- -->11 0x00007f8678f60fe3 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fe3)
#<!-- -->12 0x00007f86797f810c (/lib64/libclang-cpp.so.21.1+0x23f810c)
#<!-- -->13 0x00007f86797a3afe clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/lib64/libclang-cpp.so.21.1+0x23a3afe)
#<!-- -->14 0x00007f8678f60fab clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseDecl(clang::Decl*) (/lib64/libclang-cpp.so.21.1+0x1b60fab)
#<!-- -->15 0x00007f867adfa9ef (/lib64/libclang-cpp.so.21.1+0x39fa9ef)
#<!-- -->16 0x00007f867ac09615 clang::ento::CheckerManager::runCheckersOnASTDecl(clang::Decl const*, clang::ento::AnalysisManager&amp;, clang::ento::BugReporter&amp;) (/lib64/libclang-cpp.so.21.1+0x3809615)
#<!-- -->17 0x00007f867af0783f (/lib64/libclang-cpp.so.21.1+0x3b0783f)
#<!-- -->18 0x00007f867a9fb295 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x35fb295)
#<!-- -->19 0x00007f867890cbb2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150cbb2)
#<!-- -->20 0x00007f86791c85bf clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc85bf)
#<!-- -->21 0x00007f86791c80d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x1dc80d0)
#<!-- -->22 0x00007f867ab68417 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x3768417)
#<!-- -->23 0x0000558c8e0401fc (/usr/bin/clang-tidy+0x21c1fc)
#<!-- -->24 0x00007f867ab682b7 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/lib64/libclang-cpp.so.21.1+0x37682b7)
#<!-- -->25 0x00007f867ab67260 clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x3767260)
#<!-- -->26 0x00007f867ab69a7c clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x3769a7c)
#<!-- -->27 0x0000558c8e03aad2 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x216ad2)
#<!-- -->28 0x0000558c8e022f38 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x1fef38)
#<!-- -->29 0x00007f866e4105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#<!-- -->30 0x00007f866e410668 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#<!-- -->31 0x0000558c8e01f055 _start (/usr/bin/clang-tidy+0x1fb055)
Segmentation fault
```
</details>


---

### Comment 4 - vbvictor

> By looking at `clang::ento::`, this may be a CSA related issue?

Yes, must be

---

### Comment 5 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.

Could you please provide reproducer, preferably on https://godbolt.org? 

---

### Comment 6 - steakhal

I'm pretty sure from the stack trace that it's a duplicated of #176629
Even that is outside of the clang static analyzer, so it won't be fixed by us.
https://github.com/llvm/llvm-project/issues/176629#issuecomment-3765193993

---

