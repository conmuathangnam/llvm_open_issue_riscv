# Clang 20 compiler C++ frontend command failed due to signal

**Author:** n0F4x
**URL:** https://github.com/llvm/llvm-project/issues/131159

## Body

- I cannot decipher what is going wrong here.
- This happened while migrating [my project](https://github.com/n0F4x/data-driven-game-engine ) from Clang 19 to Clang 20.

Attached preprocessed source, associated run script, and compilation output: [generated.zip](https://github.com/user-attachments/files/19231861/generated.zip)

## Comments

### Comment 1 - n0F4x

I had the following main.cpp
```cpp
struct First {
    int value{ 42 };
};

struct Second {
    std::reference_wrapper<const int> ref;
};

auto main() -> int
{
    constexpr int result =
        core::app::Builder{}
            .extend_with<extensions::ResourceManager>()
            .use_resource(First{})
            .inject_resource([](const First& first) -> Second {
                return Second{ .ref = first.value };
            })
            .extend_with<extensions::Runnable>()
            .run([](auto app) {
                return app.resource_manager.template get<Second>().ref.get();
            });

    return result;
}
```
If I put First and Second in an unnamed namespace, it works.
```cpp
namespace {

struct First {
    int value{ 42 };
};

struct Second {
    std::reference_wrapper<const int> ref;
};

}   // namespace
```

---

### Comment 2 - Endilll

Unfortunately, preprocessed source that Clang generated doesn't include `core.app` and `extensions` modules. Can you reproduce this using `#include` directive? You can also try to provide sources of those modules, but frankly, a self-contained reproducer is much easier for us to work with. Assuming that this is not a bug in modules implementation, of course.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (n0F4x)

<details>
- I cannot decipher what is going wrong here.
- This happened while migrating [my project](https://github.com/n0F4x/data-driven-game-engine ) from Clang 19 to Clang 20.

Attached preprocessed source, associated run script, and compilation output: [generated.zip](https://github.com/user-attachments/files/19231861/generated.zip)
</details>


---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-1

Author: None (n0F4x)

<details>
- I cannot decipher what is going wrong here.
- This happened while migrating [my project](https://github.com/n0F4x/data-driven-game-engine ) from Clang 19 to Clang 20.

Attached preprocessed source, associated run script, and compilation output: [generated.zip](https://github.com/user-attachments/files/19231861/generated.zip)
</details>


---

### Comment 5 - Endilll

Stack trace from the provided archive:
```
FAILED: examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj 
C:\dev\MSYS2_temp\clang64\bin\clang++.exe -DVK_NO_PROTOTYPES -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_STRUCT_CONSTRUCTORS -DVULKAN_HPP_NO_TO_STRING -isystem C:/Users/Gabor/.conan2/p/vulkaaf4d40321f065/p/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vulkanmemoryallocator-src/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vk-bootstrap-src/src -isystem C:/Users/Gabor/.conan2/p/glmc7261f667dea4/p/include -isystem C:/Users/Gabor/.conan2/p/enttf8056d762f825/p/include -m64 -stdlib=libc++ -g -std=c++26 -fansi-escape-codes -fcolor-diagnostics -Wall -Wextra -pedantic-errors -Wconversion -Werror -Wno-missing-field-initializers -v -MD -MT examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -MF examples\src\demos\constexpr\CMakeFiles\constexpr.dir\main.cpp.obj.d @examples\src\demos\constexpr\CMakeFiles\constexpr.dir\main.cpp.obj.modmap -o examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -c C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp
clang version 20.1.0
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/dev/MSYS2_temp/clang64/bin
 (in-process)
 "C:/dev/MSYS2_temp/clang64/bin/clang++.exe" -cc1 -triple x86_64-w64-windows-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mms-bitfields -funwind-tables=2 -fno-sized-deallocation -fno-use-init-array -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug -v -fcoverage-compilation-dir=C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug -resource-dir C:/dev/MSYS2_temp/clang64/lib/clang/20 -dependency-file "examples\\src\\demos\\constexpr\\CMakeFiles\\constexpr.dir\\main.cpp.obj.d" -MT examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -sys-header-deps -isystem C:/Users/Gabor/.conan2/p/vulkaaf4d40321f065/p/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vulkanmemoryallocator-src/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vk-bootstrap-src/src -isystem C:/Users/Gabor/.conan2/p/glmc7261f667dea4/p/include -isystem C:/Users/Gabor/.conan2/p/enttf8056d762f825/p/include -D VK_NO_PROTOTYPES -D VULKAN_HPP_NO_SETTERS -D VULKAN_HPP_NO_SPACESHIP_OPERATOR -D VULKAN_HPP_NO_STRUCT_CONSTRUCTORS -D VULKAN_HPP_NO_TO_STRING -internal-isystem C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/include/c++/v1 -internal-isystem C:/dev/MSYS2_temp/clang64/include/c++/v1 -internal-isystem C:/dev/MSYS2_temp/clang64/lib/clang/20/include -internal-isystem C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/include -internal-isystem C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/usr/include -internal-isystem C:/dev/MSYS2_temp/clang64/include -Wall -Wextra -Wconversion -Werror -Wno-missing-field-initializers -pedantic-errors -std=c++26 -fdeprecated-macro -ferror-limit 19 -fno-use-cxa-atexit -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=core.app=lib/CMakeFiles/engine.dir/core.app.pcm -fmodule-file=extensions=lib/CMakeFiles/engine.dir/extensions.pcm -fmodule-file=addons.ResourceManager=lib/CMakeFiles/engine.dir/addons.ResourceManager.pcm -fmodule-file=core.app.App=lib/CMakeFiles/engine.dir/core.app.App.pcm -fmodule-file=core.app.App:addon_c=lib/CMakeFiles/engine.dir/core.app.App-addon_c.pcm -fmodule-file=core.app.App:details=lib/CMakeFiles/engine.dir/core.app.App-details.pcm -fmodule-file=core.app.App:fwd=lib/CMakeFiles/engine.dir/core.app.App-fwd.pcm -fmodule-file=core.app.Builder=lib/CMakeFiles/engine.dir/core.app.Builder.pcm -fmodule-file=core.app.Builder:details=lib/CMakeFiles/engine.dir/core.app.Builder-details.pcm -fmodule-file=core.app.Builder:extension_c=lib/CMakeFiles/engine.dir/core.app.Builder-extension_c.pcm -fmodule-file=core.app.Builder:fwd=lib/CMakeFiles/engine.dir/core.app.Builder-fwd.pcm -fmodule-file=core.resource.ResourceManager=lib/CMakeFiles/engine.dir/core.resource.ResourceManager.pcm -fmodule-file=core.store.Store=lib/CMakeFiles/engine.dir/core.store.Store.pcm -fmodule-file=core.store.storable_c=lib/CMakeFiles/engine.dir/core.store.storable_c.pcm -fmodule-file=extensions.AddonManager=lib/CMakeFiles/engine.dir/extensions.AddonManager.pcm -fmodule-file=extensions.Functional=lib/CMakeFiles/engine.dir/extensions.Functional.pcm -fmodule-file=extensions.ResourceManager=lib/CMakeFiles/engine.dir/extensions.ResourceManager.pcm -fmodule-file=extensions.Runnable=lib/CMakeFiles/engine.dir/extensions.Runnable.pcm -fmodule-file=utility.TypeList=lib/CMakeFiles/engine.dir/utility.TypeList.pcm -fmodule-file=utility.containers.StackedTuple=lib/CMakeFiles/engine.dir/utility.containers.StackedTuple.pcm -fmodule-file=utility.meta.concepts.decayed=lib/CMakeFiles/engine.dir/utility.meta.concepts.decayed.pcm -fmodule-file=utility.meta.concepts.functional.function=lib/CMakeFiles/engine.dir/utility.meta.concepts.functional.function.pcm -fmodule-file=utility.meta.concepts.functional.functor=lib/CMakeFiles/engine.dir/utility.meta.concepts.functional.functor.pcm -fmodule-file=utility.meta.concepts.functional.member_function_pointer=lib/CMakeFiles/engine.dir/utility.meta.concepts.functional.member_function_pointer.pcm -fmodule-file=utility.meta.concepts.integer_sequence.integer_sequence=lib/CMakeFiles/engine.dir/utility.meta.concepts.integer_sequence.integer_sequence.pcm -fmodule-file=utility.meta.concepts.type_list=lib/CMakeFiles/engine.dir/utility.meta.concepts.type_list.pcm -fmodule-file=utility.meta.concepts.type_list.all_of_type_list=lib/CMakeFiles/engine.dir/utility.meta.concepts.type_list.all_of_type_list.pcm -fmodule-file=utility.meta.concepts.type_list.type_list=lib/CMakeFiles/engine.dir/utility.meta.concepts.type_list.type_list.pcm -fmodule-file=utility.meta.type_traits.all_different=lib/CMakeFiles/engine.dir/utility.meta.type_traits.all_different.pcm -fmodule-file=utility.meta.type_traits.back=lib/CMakeFiles/engine.dir/utility.meta.type_traits.back.pcm -fmodule-file=utility.meta.type_traits.forward_like=lib/CMakeFiles/engine.dir/utility.meta.type_traits.forward_like.pcm -fmodule-file=utility.meta.type_traits.functional.arguments_of=lib/CMakeFiles/engine.dir/utility.meta.type_traits.functional.arguments_of.pcm -fmodule-file=utility.meta.type_traits.functional.invoke_result_of=lib/CMakeFiles/engine.dir/utility.meta.type_traits.functional.invoke_result_of.pcm -fmodule-file=utility.meta.type_traits.functional.signature=lib/CMakeFiles/engine.dir/utility.meta.type_traits.functional.signature.pcm -fmodule-file=utility.meta.type_traits.integer_sequence.integer_sequence_concat=lib/CMakeFiles/engine.dir/utility.meta.type_traits.integer_sequence.integer_sequence_concat.pcm -fmodule-file=utility.meta.type_traits.integer_sequence.integer_sequence_offset=lib/CMakeFiles/engine.dir/utility.meta.type_traits.integer_sequence.integer_sequence_offset.pcm -fmodule-file=utility.meta.type_traits.integer_sequence.integer_sequence_size=lib/CMakeFiles/engine.dir/utility.meta.type_traits.integer_sequence.integer_sequence_size.pcm -fmodule-file=utility.meta.type_traits.integer_sequence.is_integer_sequence=lib/CMakeFiles/engine.dir/utility.meta.type_traits.integer_sequence.is_integer_sequence.pcm -fmodule-file=utility.meta.type_traits.type_list.is_type_list=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.is_type_list.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_contains=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_contains.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_drop_back=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_drop_back.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_drop_front=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_drop_front.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_push_back=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_push_back.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_push_front=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_push_front.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_replace=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_replace.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_size=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_size.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_take=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_take.pcm -fmodule-file=utility.meta.type_traits.type_list.type_list_unique=lib/CMakeFiles/engine.dir/utility.meta.type_traits.type_list.type_list_unique.pcm -fmodule-file=utility.tuple=lib/CMakeFiles/engine.dir/utility.tuple.pcm -fmodule-file=utility.tuple.generate_tuple_from=lib/CMakeFiles/engine.dir/utility.tuple.generate_tuple_from.pcm -fmodule-file=utility.tuple.tuple_drop_back=lib/CMakeFiles/engine.dir/utility.tuple.tuple_drop_back.pcm -fmodule-file=utility.tuple.tuple_drop_front=lib/CMakeFiles/engine.dir/utility.tuple.tuple_drop_front.pcm -fmodule-file=utility.tuple.tuple_drop_nth=lib/CMakeFiles/engine.dir/utility.tuple.tuple_drop_nth.pcm -fmodule-file=utility.tuple.tuple_select=lib/CMakeFiles/engine.dir/utility.tuple.tuple_select.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -exception-model=seh -fcolor-diagnostics -fansi-escape-codes -faddrsig -o examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -x c++ C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp
clang -cc1 version 20.1.0 based upon LLVM 20.1.0 default target x86_64-w64-windows-gnu
ignoring nonexistent directory "C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/include/c++/v1"
ignoring nonexistent directory "C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/include"
ignoring nonexistent directory "C:/dev/MSYS2_temp/clang64/x86_64-w64-mingw32/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 C:/Users/Gabor/.conan2/p/vulkaaf4d40321f065/p/include
 C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vulkanmemoryallocator-src/include
 C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vk-bootstrap-src/src
 C:/Users/Gabor/.conan2/p/glmc7261f667dea4/p/include
 C:/Users/Gabor/.conan2/p/enttf8056d762f825/p/include
 C:/dev/MSYS2_temp/clang64/include/c++/v1
 C:/dev/MSYS2_temp/clang64/lib/clang/20/include
 C:/dev/MSYS2_temp/clang64/include
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: C:\\dev\\MSYS2_temp\\clang64\\bin\\clang++.exe -DVK_NO_PROTOTYPES -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_STRUCT_CONSTRUCTORS -DVULKAN_HPP_NO_TO_STRING -isystem C:/Users/Gabor/.conan2/p/vulkaaf4d40321f065/p/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vulkanmemoryallocator-src/include -isystem C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/build/Debug/_deps/vk-bootstrap-src/src -isystem C:/Users/Gabor/.conan2/p/glmc7261f667dea4/p/include -isystem C:/Users/Gabor/.conan2/p/enttf8056d762f825/p/include -m64 -stdlib=libc++ -g -std=c++26 -fansi-escape-codes -fcolor-diagnostics -Wall -Wextra -pedantic-errors -Wconversion -Werror -Wno-missing-field-initializers -v -MD -MT examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -MF examples\\src\\demos\\constexpr\\CMakeFiles\\constexpr.dir\\main.cpp.obj.d @examples\\src\\demos\\constexpr\\CMakeFiles\\constexpr.dir\\main.cpp.obj.modmap -o examples/src/demos/constexpr/CMakeFiles/constexpr.dir/main.cpp.obj -c C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp
1.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp:24:16: current parser token ')'
2.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp:15:1: parsing function body 'main'
3.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/examples/src/demos/constexpr/main.cpp:15:1: in compound statement ('{}')
4.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/Builder-details.ixx:211:20: instantiating function definition 'BasicBuilder<extensions::ResourceManagerTag, extensions::BasicResourceManager<Injection, Second (*)(const First &)>>::build<BasicBuilder<extensions::ResourceManagerTag, extensions::BasicResourceManager<Injection, Second (*)(const First &)>>>'
5.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/Builder-details.ixx:86:20: instantiating function definition 'BasicBuilderBase<extensions::ResourceManagerTag, extensions::BasicResourceManager<Injection, Second (*)(const First &)>, RootExtension>::build<BasicBuilder<extensions::ResourceManagerTag, extensions::BasicResourceManager<Injection, Second (*)(const First &)>>, core::app::App<>>'
6.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/Builder-details.ixx:86:20: instantiating function definition 'BasicBuilderBase<extensions::BasicResourceManager<Injection, Second (*)(const First &)>, RootExtension>::build<BasicBuilder<extensions::ResourceManagerTag, extensions::BasicResourceManager<Injection, Second (*)(const First &)>>, core::app::App<>>'
7.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/extensions/BasicResourceManager.ixx:189:67: instantiating function definition 'extensions::BasicResourceManager<Injection, Second (*)(const First &)>::operator()<core::app::App<DummyAddon>>'
8.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/extensions/BasicResourceManager.ixx:198:12: instantiating function definition 'extensions::BasicResourceManager<Injection, Second (*)(const First &)>::operator()(core::app::App<DummyAddon> &&)::(anonymous class)::operator()<0ULL, 1ULL>'
9.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/App.ixx:54:45: instantiating function definition 'core::app::App<DummyAddon>::add_on<addons::ResourceManager<First, Second>, core::app::App<DummyAddon>, const std::in_place_t &, Injection, Second (*)(const First &)>'
10.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/App.ixx:42:40: instantiating function definition 'core::app::App<addons::ResourceManager<First, Second>, DummyAddon>::App<core::app::App<DummyAddon>, const std::in_place_t &, Injection, Second (*)(const First &)>'
11.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/app/App-details.ixx:20:24: instantiating function definition 'AppBase<addons::ResourceManager<First, Second>, DummyAddon, RootAddon>::AppBase<core::app::App<DummyAddon>, const std::in_place_t &, Injection, Second (*)(const First &)>'
12.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/addons/ResourceManager.ixx:24:44: instantiating function definition 'addons::ResourceManager<First, Second>::ResourceManager<Injection, Second (*)(const First &)>'
13.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/core/resource/ResourceManager.ixx:56:60: instantiating function definition 'core::resource::ResourceManager<First, Second>::ResourceManager<Injection, Second (*)(const First &)>'
14.	C:/dev/MSYS2_temp/clang64/include/c++/v1/__memory/unique_ptr.h:766:76: instantiating function definition 'std::make_unique<util::StackedTuple<First, Second>, Injection, Second (*)(const First &), 0>'
15.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/utility/containers/StackedTuple.ixx:128:38: instantiating function definition 'util::StackedTuple<First, Second>::StackedTuple<Injection, Second (*)(const First &)>'
16.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/utility/containers/StackedTuple.ixx:113:27: instantiating function definition 'Impl<First, Second>::Impl<Injection, Second (*)(const First &)>'
17.	C:/Users/Gabor/Documents/Projects/Fun/data-driven-game-engine/lib/src/utility/containers/StackedTuple.ixx:104:20: instantiating function definition 'Impl<Second>::Impl<First, Second (*)(const First &)>'
Exception Code: 0xC0000005
  #0 0x00007fff792b0d23 clang::Decl::castFromDeclContext(clang::DeclContext const*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x100d23)
  #1 0x00007fff794762b4 clang::SwitchStmt::getConditionVariable() (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x2c62b4)
  #2 0x00007fff794768cb clang::SwitchStmt::getConditionVariable() (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x2c68cb)
  #3 0x00007fff793233fa clang::CFG::buildCFG(clang::Decl const*, clang::Stmt*, clang::ASTContext*, clang::CFG::BuildOptions const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1733fa)
  #4 0x00007fff7931a3fd clang::AnalysisDeclContext::getCFG() (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x16a3fd)
  #5 0x00007fff7931317c clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x16317c)
  #6 0x00007fff79244520 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x94520)
  #7 0x00007fff79241fca clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x91fca)
  #8 0x00007fff796f6452 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546452)
  #9 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #10 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #11 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #12 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #13 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #14 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #15 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #16 0x00007fff796ff400 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54f400)
 #17 0x00007fff796fdb77 clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54db77)
 #18 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #19 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #20 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #21 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #22 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #23 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #24 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #25 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #26 0x00007fff797d846a clang::Sema::BuildMemberInitializer(clang::ValueDecl*, clang::Expr*, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x62846a)
 #27 0x00007fff796fda62 clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54da62)
 #28 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #29 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #30 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #31 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #32 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #33 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #34 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #35 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #36 0x00007fff798f4be9 clang::Sema::BuildCXXNew(clang::SourceRange, bool, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::SourceRange, clang::QualType, clang::TypeSourceInfo*, std::__1::optional<clang::Expr*>, clang::SourceRange, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x744be9)
 #37 0x00007fff7992537a clang::CastExpr::getTrailingFPFeatures() (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x77537a)
 #38 0x00007fff794001f4 clang::Sema::BuildMemberReferenceExpr(clang::Expr*, clang::QualType, clang::SourceLocation, bool, clang::CXXScopeSpec const&, clang::SourceLocation, clang::NamedDecl*, clang::LookupResult&, clang::TemplateArgumentListInfo const*, clang::Scope const*, bool, clang::Sema::ActOnMemberAccessExtraArgs*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x2501f4)
 #39 0x00007fff793eaca1 clang::Sema::isQualifiedMemberAccess(clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x23aca1)
 #40 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
 #41 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
 #42 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
 #43 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
 #44 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #45 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #46 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #47 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #48 0x00007fff7985be96 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x6abe96)
 #49 0x00007fff7925998e clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa998e)
 #50 0x00007fff79258d57 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa8d57)
 #51 0x00007fff792549d9 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa49d9)
 #52 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
 #53 0x00007fff794001f4 clang::Sema::BuildMemberReferenceExpr(clang::Expr*, clang::QualType, clang::SourceLocation, bool, clang::CXXScopeSpec const&, clang::SourceLocation, clang::NamedDecl*, clang::LookupResult&, clang::TemplateArgumentListInfo const*, clang::Scope const*, bool, clang::Sema::ActOnMemberAccessExtraArgs*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x2501f4)
 #54 0x00007fff798b245a clang::Sema::CheckDerivedToBaseConversion(clang::QualType, clang::QualType, unsigned int, unsigned int, clang::SourceLocation, clang::SourceRange, clang::DeclarationName, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u>*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x70245a)
 #55 0x00007fff7970c000 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x55c000)
 #56 0x00007fff796fd9ff clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54d9ff)
 #57 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #58 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #59 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #60 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #61 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #62 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #63 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #64 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #65 0x00007fff797d846a clang::Sema::BuildMemberInitializer(clang::ValueDecl*, clang::Expr*, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x62846a)
 #66 0x00007fff796fda62 clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54da62)
 #67 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #68 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #69 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #70 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #71 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #72 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #73 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #74 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #75 0x00007fff796ff400 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54f400)
 #76 0x00007fff796fdb77 clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54db77)
 #77 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #78 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #79 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #80 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #81 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #82 0x00007fff79274269 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, clang::CXXConstructionKind, clang::SourceRange) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc4269)
 #83 0x00007fff79783db6 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d3db6)
 #84 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #85 0x00007fff796ff400 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54f400)
 #86 0x00007fff796fdb77 clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54db77)
 #87 0x00007fff796f6958 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x546958)
 #88 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
 #89 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
 #90 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
 #91 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
 #92 0x00007fff79784001 clang::QualType::isNonConstantStorage(clang::ASTContext const&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5d4001)
 #93 0x00007fff7977a074 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5ca074)
 #94 0x00007fff79326278 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x176278)
 #95 0x00007fff793eacfa clang::Sema::isQualifiedMemberAccess(clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x23acfa)
 #96 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
 #97 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
 #98 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
 #99 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#100 0x00007fff796f4887 clang::Sema::SpecialMemberOverloadResult::SpecialMemberOverloadResult(clang::CXXMethodDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x544887)
#101 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#102 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#103 0x00007fff792763d7 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xc63d7)
#104 0x00007fff793ef65e clang::Sema::MarkMemberReferenced(clang::MemberExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x23f65e)
#105 0x00007fff7925a3fd clang::Sema::BuildMemberExpr(clang::Expr*, bool, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation, clang::ValueDecl*, clang::DeclAccessPair, bool, clang::DeclarationNameInfo const&, clang::QualType, clang::ExprValueKind, clang::ExprObjectKind, clang::TemplateArgumentListInfo const*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xaa3fd)
#106 0x00007fff79259aed clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa9aed)
#107 0x00007fff79621c20 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x471c20)
#108 0x00007fff79254ae6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4ae6)
#109 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
#110 0x00007fff793fc73f clang::Sema::SubstAutoTypeDependent(clang::QualType) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x24c73f)
#111 0x00007fff793682bb clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b82bb)
#112 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
#113 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
#114 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
#115 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#116 0x00007fff79b2dea9 ImmediateCallVisitor::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97dea9)
#117 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#118 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#119 0x00007fff798ebfa0 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x73bfa0)
#120 0x00007fff7939b3a9 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1eb3a9)
#121 0x00007fff798bf625 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x70f625)
#122 0x00007fff798be352 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x70e352)
#123 0x00007fff79254f02 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4f02)
#124 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
#125 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
#126 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
#127 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
#128 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#129 0x00007fff79b2dea9 ImmediateCallVisitor::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97dea9)
#130 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#131 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#132 0x00007fff798ebfa0 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x73bfa0)
#133 0x00007fff7939b3a9 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1eb3a9)
#134 0x00007fff79621f01 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x471f01)
#135 0x00007fff79254ae6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4ae6)
#136 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
#137 0x00007fff793681c0 clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b81c0)
#138 0x00007fff7935af1b clang::TypeWithKeyword::getTagTypeKindForKeyword(clang::ElaboratedTypeKeyword) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1aaf1b)
#139 0x00007fff7941b2e2 clang::getReplacedTemplateParameterList(clang::Decl*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x26b2e2)
#140 0x00007fff7a794168 llvm::DenseMapBase<llvm::DenseMap<clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>, clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>::erase(clang::Module* const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x15e4168)
#141 0x00007fff7a796e0a llvm::DenseMapBase<llvm::DenseMap<clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>, clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>::erase(clang::Module* const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x15e6e0a)
#142 0x00007fff7a791093 llvm::DenseMapBase<llvm::DenseMap<clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>, clang::Module*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<clang::Module*, void>, llvm::detail::DenseSetPair<clang::Module*>>::erase(clang::Module* const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x15e1093)
#143 0x00007fff796fcd0f clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54cd0f)
#144 0x00007fff796f9b0e clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549b0e)
#145 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
#146 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#147 0x00007fff79b2dea9 ImmediateCallVisitor::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97dea9)
#148 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#149 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#150 0x00007fff798ebfa0 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x73bfa0)
#151 0x00007fff7939b3a9 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1eb3a9)
#152 0x00007fff79621f01 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x471f01)
#153 0x00007fff79254ae6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4ae6)
#154 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
#155 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
#156 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
#157 0x00007fff796fce43 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54ce43)
#158 0x00007fff796f9b0e clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549b0e)
#159 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
#160 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#161 0x00007fff79b2dea9 ImmediateCallVisitor::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97dea9)
#162 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#163 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#164 0x00007fff798ebfa0 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x73bfa0)
#165 0x00007fff7939b3a9 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1eb3a9)
#166 0x00007fff79621f01 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x471f01)
#167 0x00007fff79254ae6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4ae6)
#168 0x00007fff793684ca clang::computeDependence(clang::ParenListExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1b84ca)
#169 0x00007fff796fa439 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54a439)
#170 0x00007fff796f9a2d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x549a2d)
#171 0x00007fff796f8ff9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x548ff9)
#172 0x00007fff796f642e clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x54642e)
#173 0x00007fff79b2dea9 ImmediateCallVisitor::VisitCXXDefaultArgExpr(clang::CXXDefaultArgExpr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97dea9)
#174 0x00007fff79595c51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5c51)
#175 0x00007fff79595bf1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x3e5bf1)
#176 0x00007fff798ebfa0 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x73bfa0)
#177 0x00007fff7939b3a9 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1eb3a9)
#178 0x00007fff79621f01 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x471f01)
#179 0x00007fff79254ae6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa4ae6)
#180 0x00007fff792518ec clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0xa18ec)
#181 0x00007fff7924b32b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x9b32b)
#182 0x00007fff79247f8b clang::Parser::ParseExpression(clang::Parser::TypeCastState) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x97f8b)
#183 0x00007fff7924576a clang::Parser::ParseCompoundStatementBody(bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x9576a)
#184 0x00007fff7923c053 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x8c053)
#185 0x00007fff79770ee9 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5c0ee9)
#186 0x00007fff7976cf40 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5bcf40)
#187 0x00007fff79766915 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5b6915)
#188 0x00007fff79764905 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x5b4905)
#189 0x00007fff79997ae1 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x7e7ae1)
#190 0x00007fff799977a4 clang::ParseAST(clang::Sema&, bool, bool) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x7e77a4)
#191 0x00007fff79a40c0f clang::FrontendAction::Execute() (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x890c0f)
#192 0x00007fff79a40821 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x890821)
#193 0x00007fff79a40252 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x890252)
#194 0x00007ff7dc6543d2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0x43d2)
#195 0x00007ff7dc653ba6 llvm::raw_ostream::operator<<(llvm::StringRef) (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0x3ba6)
#196 0x00007fff7afdaf0d void std::__1::vector<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, std::__1::allocator<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>>::__assign_with_size[abi:nn190107]<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*>(std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, long long) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x1e2af0d)
#197 0x00007fff7d622994 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (C:\dev\MSYS2_temp\clang64\bin\libLLVM-20.dll+0xed2994)
#198 0x00007fff79aea1b7 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x93a1b7)
#199 0x00007fff79aa789b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x8f789b)
#200 0x00007fff79aa7530 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x8f7530)
#201 0x00007fff79aa725b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (C:\dev\MSYS2_temp\clang64\bin\libclang-cpp.dll+0x8f725b)
#202 0x00007ff7dc651bde clang_main(int, char**, llvm::ToolContext const&) (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0x1bde)
#203 0x00007ff7dc651075 main (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0x1075)
#204 0x00007ff7dc65abec WinMainCRTStartup (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0xabec)
#205 0x00007ff7dc65ac46 mainCRTStartup (C:\dev\MSYS2_temp\clang64\bin\clang++.exe+0xac46)
#206 0x00007ff84e1be8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#207 0x00007ff84ef9bf6c (C:\WINDOWS\SYSTEM32\ntdll.dll+0xbbf6c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.0
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/dev/MSYS2_temp/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:/Users/Gabor/AppData/Local/Temp/main-00221e.cpp
clang++: note: diagnostic msg: C:/Users/Gabor/AppData/Local/Temp/main-00221e.sh
clang++: note: diagnostic msg: 

********************
```

---

### Comment 6 - n0F4x

> Unfortunately, preprocessed source that Clang generated doesn't include `core.app` and `extensions` modules. Can you reproduce this using `#include` directive? You can also try to provide sources of those modules, but frankly, a self-contained reproducer is much easier for us to work with. Assuming that this is not a bug in modules implementation, of course.

This is a bug with modules if I had to guess. I had other problems with them and missing unnamed namespaces. You will find a similar definition of `First` and `Second` inside `utility.containers.StackedTuple` module.
The modules are pretty easy to navigate given [this](https://github.com/n0F4x/data-driven-game-engine/tree/84a5d213d0b8f8e17870311ded2bb7260be1cbab/lib/src ) source directory. Module names match the directory structure (and filename).

---

### Comment 7 - Endilll

Well, I guess this bug report will have to wait for someone else who is willing to go over repo etc., because I don't have time to do that, sorry.

---

### Comment 8 - n0F4x

Clang 20.1.1 still has it.
However, newer versions of my project seem to compile.

---

### Comment 9 - n0F4x

When compiled with Clang 20.1.3 I no longer get the crash, but it still seems to mess up the variadic concept template sequence
```cpp
template <typename T, typename Resource_T>
concept decays_to_factory_c =
    std::constructible_from<Resource_T, util::meta::result_of_t<T>>;

template <util::meta::decayed_c... Ts>
class StackedTuple {
public:
    template <decays_to_factory_c<Ts>... Factories_T>
    constexpr explicit StackedTuple(Factories_T&&... factories);
};
// ...
StackedTuple<First, Second>{[]{ return First{}; }, []{ return Second{}; }}
```
This complains that First cannot be constructed by Second, and vice versa.

With a different syntax, it works fine
```cpp
template <typename... Factories_T>
    requires (decays_to_factory_c<Factories_T, Ts> && ...)
constexpr explicit StackedTuple(Factories_T&&... factories);
```

---

