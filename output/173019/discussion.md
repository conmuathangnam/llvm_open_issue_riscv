# clang++ ICE on Windows when calling templated function with lambda and function pointer argument (C++20)

**Author:** elecfrog
**URL:** https://github.com/llvm/llvm-project/issues/173019

## Body

**Description**
clang++ crashes with an access violation while parsing a C++ source file.
This is a clang frontend crash (ICE) that occurs during parsing of a function body.


The compiler crashes instead of emitting a diagnostic.

**Environment**
OS: Windows 10 x64

clang++ --version:
clang version 21.1.6
Target: x86_64-pc-windows-clang
Thread model: posix
InstalledDir: C:\Users\Administrator\scoop\apps\llvm\21.1.6\bin

**Source code (reproducer)**
```
template <typename T, typename Callback>
static inline void Combo(
    const char* label,
    T& current_index,
    std::vector<const char*> items,
    Callback on_changed,
    int popup_max_height_in_items = -1)
{
    Int current_index_int = static_cast<Int>(current_index);
    if (ImGui::Combo(label,
                     &current_index_int,
                     items.data(),
                     static_cast<Int>(items.size()),
                     popup_max_height_in_items))
    {
        if (current_index_int != static_cast<Int>(current_index))
        {
            current_index = static_cast<T>(current_index_int);
            on_changed(current_index);
        }
    }
}

template <typename T, typename Callback>
static inline void Combo(
    const char* label,
    T& current_index,
    int count,
    const char* (*getter)(T),
    Callback on_changed,
    int popup_max_height_in_items = -1)
{
    Int current_index_int = static_cast<Int>(current_index);

    auto item_getter = [](void* data, int idx, const char** out_text) -> bool {
        auto func = reinterpret_cast<const char* (*)(T)>(data);
        *out_text = func(static_cast<T>(idx));
        return true;
    };

    if (ImGui::Combo(label,
                     &current_index_int,
                     item_getter,
                     reinterpret_cast<void*>(getter),
                     count,
                     popup_max_height_in_items))
    {
        if (current_index_int != static_cast<Int>(current_index))
        {
            current_index = static_cast<T>(current_index_int);
            on_changed(current_index);
        }
    }
}

// Call site
ImGuiExt::Combo<GeometryType, void(GeometryType)>(
    "Geometry Type",
    m_CurrentGeometryType,
    GeometryFactory::GetCount(),
    GeometryFactory::GetName,
    [this](GeometryType type) {
        ReGenerateGeometry(type);
    });
```
**workaround**
```
       ImGuiExt::Combo("Geometry Type", m_CurrentGeometryType, GeometryFactory::GetCount(), GeometryFactory::GetName, [this](GeometryType type) {
            ReGenerateGeometry(type);
        });
```

```
**Crash backtrace**
`PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: C:\\Users\\Administrator\\scoop\\apps\\llvm\\current\\bin\\clang++.exe -DD3D11_SUPPORTED=1 -DD3D12_SUPPORTED=1 -DDILIGENT_DEBUG -DDILIGENT_DEVELOPMENT -DDILIGENT_RENDER_STATE_CACHE_SUPPORTED=1 -DDILIGENT_VK_SHARED=1 -DFMT_SHARED -DGLES_SUPPORTED=0 -DGLFW_DLL -DGL_SUPPORTED=1 -DIMGUI_DEFINE_MATH_OPERATORS -DMETAL_SUPPORTED=0 -DPLATFORM_WIN32=1 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_SHARED_LIB -DVK_USE_PLATFORM_WIN32_KHR -DVULKAN_SUPPORTED=1 -DWEBGPU_SUPPORTED=0 -ID:/Dev/sparrow/sparrow -ID:/Dev/sparrow/sparrow/.. -ID:/Dev/sparrow/sparrow/../third_party/DiligentCore -ID:/Dev/sparrow/sparrow/../third_party/DiligentTools -ID:/Dev/sparrow/sparrow/../third_party/DiligentTools/ThirdParty/args -ID:/Dev/sparrow/common -ID:/Dev/sparrow/third_party/DiligentCore/Common/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/Win32/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/Basic/interface -ID:/Dev/sparrow/third_party/DiligentCore/Primitives/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsTools/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsEngine/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsEngineVulkan/interface -ID:/Dev/sparrow/third_party/DiligentTools/ThirdParty/imgui -ID:/Dev/sparrow/third_party/DiligentTools/Imgui/interface -ID:/Dev/sparrow/third_party/DiligentTools/TextureLoader/interface -isystem D:/Dev/sparrow/build/windows-clang-debug/vcpkg_installed/x64-windows/include -O0 -g -Xclang -gcodeview -std=gnu++20 -D_DEBUG -D_DLL -D_MT -Xclang --dependent-lib=msvcrtd -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-nontrivial-memcall -MD -MT sparrow/CMakeFiles/sparrow_lib.dir/hello_cube.cpp.obj -MF sparrow\\CMakeFiles\\sparrow_lib.dir\\hello_cube.cpp.obj.d -o sparrow/CMakeFiles/sparrow_lib.dir/hello_cube.cpp.obj -c D:/Dev/sparrow/sparrow/hello_cube.cpp
1.	D:/Dev/sparrow/sparrow/hello_cube.cpp:439:10: current parser token ')'
2.	D:/Dev/sparrow/sparrow/hello_cube.cpp:24:1: parsing namespace 'spw'
3.	D:/Dev/sparrow/sparrow/hello_cube.cpp:434:1: parsing function body 'spw::HelloCube::UpdateUI'
4.	D:/Dev/sparrow/sparrow/hello_cube.cpp:434:1: in compound statement ('{}')
5.	D:/Dev/sparrow/sparrow/hello_cube.cpp:436:5: in compound statement ('{}')
Exception Code: 0xC0000005
 #0 0x00007ff7ca4330e9 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x8430e9)
 #1 0x00007ff7ca42c729 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83c729)
 #2 0x00007ff7ca42c486 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83c486)
 #3 0x00007ff7ca42b8b9 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83b8b9)
 #4 0x00007ff7ca471bdf (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x881bdf)
 #5 0x00007ff7cb08b29c (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x149b29c)
 #6 0x00007ff7cb02fd46 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x143fd46)
 #7 0x00007ff7cb025a17 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1435a17)
 #8 0x00007ff7cb0226b4 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14326b4)
 #9 0x00007ff7cb01fc37 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x142fc37)
#10 0x00007ff7cba3a323 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1e4a323)
#11 0x00007ff7cba39d1c (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1e49d1c)
#12 0x00007ff7cb396f0d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x17a6f0d)
#13 0x00007ff7cb0ea7a6 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14fa7a6)
#14 0x00007ff7cb0e5715 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14f5715)
#15 0x00007ff7cb01d382 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x142d382)
#16 0x00007ff7cb1bb993 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15cb993)
#17 0x00007ff7cb1b4927 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c4927)
#18 0x00007ff7cb1b377f (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c377f)
#19 0x00007ff7cb1b17ca (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c17ca)
#20 0x00007ff7cb1b13fc (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c13fc)
#21 0x00007ff7c9d40791 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x150791)
#22 0x00007ff7cb1b2739 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c2739)
#23 0x00007ff7cb1b1666 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c1666)
#24 0x00007ff7cbb16528 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1f26528)
#25 0x00007ff7cbb161e1 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1f261e1)
#26 0x00007ff7ca218af3 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x628af3)
#27 0x00007ff7ca21873d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x62873d)
#28 0x00007ff7ca2153f6 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x6253f6)
#29 0x00007ff7ca211f58 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x621f58)
#30 0x00007ff7ca2107ee (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x6207ee)
#31 0x00007ff7caafde2d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0xf0de2d)
#32 0x00007ff7ca3a1235 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x7b1235)
#33 0x00007ff7ca3a1028 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x7b1028)
#34 0x00007ff7ca0c4de5 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d4de5)
#35 0x00007ff7ca0c472e (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d472e)
#36 0x00007ff7ca0c430d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d430d)
#37 0x00007ff7ca0bb046 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4cb046)
#38 0x00007ff7ca0b85e3 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4c85e3)
#39 0x00007ff7cbbdbd70 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1febd70)
#40 0x00007ff848927344 (C:\Windows\System32\KERNEL32.DLL+0x17344)
#41 0x00007ff849aa26b1 (C:\Windows\SYSTEM32\ntdll.dll+0x526b1)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)`
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: elecfrog (elecfrog)

<details>
**Description**
clang++ crashes with an access violation while parsing a C++ source file.
This is a clang frontend crash (ICE) that occurs during parsing of a function body.


The compiler crashes instead of emitting a diagnostic.

**Environment**
OS: Windows 10 x64

clang++ --version:
clang version 21.1.6
Target: x86_64-pc-windows-clang
Thread model: posix
InstalledDir: C:\Users\Administrator\scoop\apps\llvm\21.1.6\bin

**Source code (reproducer)**
```
template &lt;typename T, typename Callback&gt;
static inline void Combo(
    const char* label,
    T&amp; current_index,
    std::vector&lt;const char*&gt; items,
    Callback on_changed,
    int popup_max_height_in_items = -1)
{
    Int current_index_int = static_cast&lt;Int&gt;(current_index);
    if (ImGui::Combo(label,
                     &amp;current_index_int,
                     items.data(),
                     static_cast&lt;Int&gt;(items.size()),
                     popup_max_height_in_items))
    {
        if (current_index_int != static_cast&lt;Int&gt;(current_index))
        {
            current_index = static_cast&lt;T&gt;(current_index_int);
            on_changed(current_index);
        }
    }
}

template &lt;typename T, typename Callback&gt;
static inline void Combo(
    const char* label,
    T&amp; current_index,
    int count,
    const char* (*getter)(T),
    Callback on_changed,
    int popup_max_height_in_items = -1)
{
    Int current_index_int = static_cast&lt;Int&gt;(current_index);

    auto item_getter = [](void* data, int idx, const char** out_text) -&gt; bool {
        auto func = reinterpret_cast&lt;const char* (*)(T)&gt;(data);
        *out_text = func(static_cast&lt;T&gt;(idx));
        return true;
    };

    if (ImGui::Combo(label,
                     &amp;current_index_int,
                     item_getter,
                     reinterpret_cast&lt;void*&gt;(getter),
                     count,
                     popup_max_height_in_items))
    {
        if (current_index_int != static_cast&lt;Int&gt;(current_index))
        {
            current_index = static_cast&lt;T&gt;(current_index_int);
            on_changed(current_index);
        }
    }
}

// Call site
ImGuiExt::Combo&lt;GeometryType, void(GeometryType)&gt;(
    "Geometry Type",
    m_CurrentGeometryType,
    GeometryFactory::GetCount(),
    GeometryFactory::GetName,
    [this](GeometryType type) {
        ReGenerateGeometry(type);
    });
```

```
**Crash backtrace**
`PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: C:\\Users\\Administrator\\scoop\\apps\\llvm\\current\\bin\\clang++.exe -DD3D11_SUPPORTED=1 -DD3D12_SUPPORTED=1 -DDILIGENT_DEBUG -DDILIGENT_DEVELOPMENT -DDILIGENT_RENDER_STATE_CACHE_SUPPORTED=1 -DDILIGENT_VK_SHARED=1 -DFMT_SHARED -DGLES_SUPPORTED=0 -DGLFW_DLL -DGL_SUPPORTED=1 -DIMGUI_DEFINE_MATH_OPERATORS -DMETAL_SUPPORTED=0 -DPLATFORM_WIN32=1 -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -DSPDLOG_SHARED_LIB -DVK_USE_PLATFORM_WIN32_KHR -DVULKAN_SUPPORTED=1 -DWEBGPU_SUPPORTED=0 -ID:/Dev/sparrow/sparrow -ID:/Dev/sparrow/sparrow/.. -ID:/Dev/sparrow/sparrow/../third_party/DiligentCore -ID:/Dev/sparrow/sparrow/../third_party/DiligentTools -ID:/Dev/sparrow/sparrow/../third_party/DiligentTools/ThirdParty/args -ID:/Dev/sparrow/common -ID:/Dev/sparrow/third_party/DiligentCore/Common/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/Win32/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/Basic/interface -ID:/Dev/sparrow/third_party/DiligentCore/Primitives/interface -ID:/Dev/sparrow/third_party/DiligentCore/Platforms/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsTools/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsEngine/interface -ID:/Dev/sparrow/third_party/DiligentCore/Graphics/GraphicsEngineVulkan/interface -ID:/Dev/sparrow/third_party/DiligentTools/ThirdParty/imgui -ID:/Dev/sparrow/third_party/DiligentTools/Imgui/interface -ID:/Dev/sparrow/third_party/DiligentTools/TextureLoader/interface -isystem D:/Dev/sparrow/build/windows-clang-debug/vcpkg_installed/x64-windows/include -O0 -g -Xclang -gcodeview -std=gnu++20 -D_DEBUG -D_DLL -D_MT -Xclang --dependent-lib=msvcrtd -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-nontrivial-memcall -MD -MT sparrow/CMakeFiles/sparrow_lib.dir/hello_cube.cpp.obj -MF sparrow\\CMakeFiles\\sparrow_lib.dir\\hello_cube.cpp.obj.d -o sparrow/CMakeFiles/sparrow_lib.dir/hello_cube.cpp.obj -c D:/Dev/sparrow/sparrow/hello_cube.cpp
1.	D:/Dev/sparrow/sparrow/hello_cube.cpp:439:10: current parser token ')'
2.	D:/Dev/sparrow/sparrow/hello_cube.cpp:24:1: parsing namespace 'spw'
3.	D:/Dev/sparrow/sparrow/hello_cube.cpp:434:1: parsing function body 'spw::HelloCube::UpdateUI'
4.	D:/Dev/sparrow/sparrow/hello_cube.cpp:434:1: in compound statement ('{}')
5.	D:/Dev/sparrow/sparrow/hello_cube.cpp:436:5: in compound statement ('{}')
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff7ca4330e9 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x8430e9)
 #<!-- -->1 0x00007ff7ca42c729 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83c729)
 #<!-- -->2 0x00007ff7ca42c486 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83c486)
 #<!-- -->3 0x00007ff7ca42b8b9 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x83b8b9)
 #<!-- -->4 0x00007ff7ca471bdf (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x881bdf)
 #<!-- -->5 0x00007ff7cb08b29c (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x149b29c)
 #<!-- -->6 0x00007ff7cb02fd46 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x143fd46)
 #<!-- -->7 0x00007ff7cb025a17 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1435a17)
 #<!-- -->8 0x00007ff7cb0226b4 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14326b4)
 #<!-- -->9 0x00007ff7cb01fc37 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x142fc37)
#<!-- -->10 0x00007ff7cba3a323 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1e4a323)
#<!-- -->11 0x00007ff7cba39d1c (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1e49d1c)
#<!-- -->12 0x00007ff7cb396f0d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x17a6f0d)
#<!-- -->13 0x00007ff7cb0ea7a6 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14fa7a6)
#<!-- -->14 0x00007ff7cb0e5715 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x14f5715)
#<!-- -->15 0x00007ff7cb01d382 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x142d382)
#<!-- -->16 0x00007ff7cb1bb993 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15cb993)
#<!-- -->17 0x00007ff7cb1b4927 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c4927)
#<!-- -->18 0x00007ff7cb1b377f (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c377f)
#<!-- -->19 0x00007ff7cb1b17ca (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c17ca)
#<!-- -->20 0x00007ff7cb1b13fc (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c13fc)
#<!-- -->21 0x00007ff7c9d40791 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x150791)
#<!-- -->22 0x00007ff7cb1b2739 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c2739)
#<!-- -->23 0x00007ff7cb1b1666 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x15c1666)
#<!-- -->24 0x00007ff7cbb16528 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1f26528)
#<!-- -->25 0x00007ff7cbb161e1 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1f261e1)
#<!-- -->26 0x00007ff7ca218af3 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x628af3)
#<!-- -->27 0x00007ff7ca21873d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x62873d)
#<!-- -->28 0x00007ff7ca2153f6 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x6253f6)
#<!-- -->29 0x00007ff7ca211f58 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x621f58)
#<!-- -->30 0x00007ff7ca2107ee (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x6207ee)
#<!-- -->31 0x00007ff7caafde2d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0xf0de2d)
#<!-- -->32 0x00007ff7ca3a1235 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x7b1235)
#<!-- -->33 0x00007ff7ca3a1028 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x7b1028)
#<!-- -->34 0x00007ff7ca0c4de5 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d4de5)
#<!-- -->35 0x00007ff7ca0c472e (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d472e)
#<!-- -->36 0x00007ff7ca0c430d (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4d430d)
#<!-- -->37 0x00007ff7ca0bb046 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4cb046)
#<!-- -->38 0x00007ff7ca0b85e3 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x4c85e3)
#<!-- -->39 0x00007ff7cbbdbd70 (C:\Users\Administrator\scoop\apps\llvm\current\bin\clang++.exe+0x1febd70)
#<!-- -->40 0x00007ff848927344 (C:\Windows\System32\KERNEL32.DLL+0x17344)
#<!-- -->41 0x00007ff849aa26b1 (C:\Windows\SYSTEM32\ntdll.dll+0x526b1)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)`
```
</details>


---

### Comment 2 - zyn0217

@elecfrog Hi, can you please share us with the reproducer that clang created when it crashed? Thanks!


---

### Comment 3 - elecfrog

My Function Body
```
    template <typename T, typename Callback>
    static inline void Combo(const char* label, T& current_index, std::vector<const char*> items, Callback on_changed, int popup_max_height_in_items = -1)
    {
        Int current_index_int = static_cast<Int>(current_index);
        if (ImGui::Combo(label, &current_index_int, items.data(), static_cast<Int>(items.size()), popup_max_height_in_items))
        {
            if (current_index_int != static_cast<Int>(current_index))
            {
                current_index = static_cast<T>(current_index_int);
                on_changed(current_index);
            }
        }
    }

    template <typename T, typename Callback>
    static inline void Combo(const char* label, T& current_index, int count, const char* (*getter)(T), Callback on_changed, int popup_max_height_in_items = -1)
    {
        Int current_index_int = static_cast<Int>(current_index);

        auto item_getter = [](void* data, int idx, const char** out_text) -> bool {
            auto func = reinterpret_cast<const char* (*)(T)>(data);
            *out_text = func(static_cast<T>(idx));
            return true;
        };

        if (ImGui::Combo(label,
                         &current_index_int,
                         item_getter,
                         reinterpret_cast<void*>(getter),
                         count,
                         popup_max_height_in_items))
        {
            if (current_index_int != static_cast<Int>(current_index))
            {
                current_index = static_cast<T>(current_index_int);
                on_changed(current_index);
            }
        }
    }
```

My Call

```
        ImGuiExt::Combo<GeometryType, void(GeometryType)>(
            "Geometry Type",
            m_CurrentGeometryType,
            GeometryFactory::GetCount(),
            GeometryFactory::GetName,
            [this](GeometryType type) {
                ReGenerateGeometry(type);
            });
```

Mini Sample to reproduce @zyn0217 

```
#include <vector>

struct ImGui
{
    static bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
    {
        return true;
    }
};

struct ImGuiExt
{
    template <typename T, typename Callback>
    static inline void Combo(const char* label, T& current_index, std::vector<const char*> items, Callback on_changed, int popup_max_height_in_items = -1)
    {
        int current_index_int = static_cast<int>(current_index);
        if (ImGui::Combo(label, &current_index_int, items.data(), static_cast<int>(items.size()), popup_max_height_in_items))
        {
            if (current_index_int != static_cast<int>(current_index))
            {
                current_index = static_cast<T>(current_index_int);
                on_changed(current_index);
            }
        }
    }

    template <typename T, typename Callback>
    static inline void Combo(const char* label, T& current_index, int count, const char* (*getter)(T), Callback on_changed, int popup_max_height_in_items = -1)
    {
        int current_index_int = static_cast<int>(current_index);

        auto item_getter = [](void* data, int idx, const char** out_text) -> bool {
            auto func = reinterpret_cast<const char* (*)(T)>(data);
            *out_text = func(static_cast<T>(idx));
            return true;
        };

        if (ImGui::Combo(label,
                         &current_index_int,
                         item_getter,
                         reinterpret_cast<void*>(getter),
                         count,
                         popup_max_height_in_items))
        {
            if (current_index_int != static_cast<int>(current_index))
            {
                current_index = static_cast<T>(current_index_int);
                on_changed(current_index);
            }
        }
    }
};





enum class GeometryType : int
{
    Box = 0,
    Cone = 1,
    Cylinder = 2,
    Sphere = 3,
    Plane = 4,
    Arrow = 5,
    Count
};

const char* GetName(GeometryType type)
{
    switch (type)
    {
    case GeometryType::Box:
        return "Box";
    case GeometryType::Cone:
        return "Cone";
    case GeometryType::Cylinder:
        return "Cylinder";
    case GeometryType::Sphere:
        return "Sphere";
    case GeometryType::Plane:
        return "Plane";
    case GeometryType::Arrow:
        return "Arrow";
    default:
        return "Unknown";
    }
}

int GetCount()
{
    return static_cast<int>(GeometryType::Count);
}

struct App
{
    void Run()
    {
        GeometryType m_CurrentGeometryType = GeometryType::Box;

        ImGuiExt::Combo<GeometryType, void(GeometryType)>(
            "Geometry Type",
            m_CurrentGeometryType,
            GetCount(),
             GetName,
            [this](GeometryType type) {
                // DoMyBussiness(type);
            });
    }
};

int main()
{
    App app;
    app.Run();

    return 0;
}

```

MSVC Output
```
39>D:\Dev\sparrow\main.cc(159,19): error C2664: “void ImGuiExt::Combo<GeometryType,void(GeometryType)>(const char *,T &,int,const char *(__cdecl *)(T),Callback (__cdecl *),int)”: 无法将参数 5 从“App::Run::<lambda_1>”转换为“Callback (__cdecl *)”
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         with
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         [
39>D:\Dev\sparrow\main.cc(159,19): error C2664:             T=GeometryType,
39>D:\Dev\sparrow\main.cc(159,19): error C2664:             Callback=void (GeometryType)
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         ]
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         and
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         [
39>D:\Dev\sparrow\main.cc(159,19): error C2664:             Callback=void (GeometryType)
39>D:\Dev\sparrow\main.cc(159,19): error C2664:         ]
39>    D:\Dev\sparrow\main.cc(166,13):
39>    没有可用于执行该转换的用户定义的转换运算符，或者无法调用该运算符
39>    D:\Dev\sparrow\main.cc(86,24):
39>    参见“ImGuiExt::Combo”的声明
39>    D:\Dev\sparrow\main.cc(72,24):
39>    可能是“void ImGuiExt::Combo(const char *,T &,std::vector<const char *,std::allocator<const char *>>,Callback,int)”
39>        D:\Dev\sparrow\main.cc(162,21):
39>        “初始化”: 无法从“int”转换为“std::vector<const char *,std::allocator<const char *>>”
39>            D:\Dev\sparrow\main.cc(162,21):
39>            class“std::vector<const char *,std::allocator<const char *>>”的构造函数声明为“explicit”
39>    D:\Dev\sparrow\main.cc(159,19):
39>    尝试匹配参数列表“(const char [14], GeometryType, int, overloaded-function, App::Run::<lambda_1>)”时
```

---

