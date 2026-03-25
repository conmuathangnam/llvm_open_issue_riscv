# Crash backtrace

**Author:** EricRobertsSGI
**URL:** https://github.com/llvm/llvm-project/issues/160734
**Status:** Closed
**Labels:** clang-tidy, incomplete, crash
**Closed Date:** 2026-01-09T15:12:14Z

## Body

> PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.

Stack dump:
```
0.  Program arguments: C:\\TC\\Tools\\LLVM\\19.1.1\\x86_64-windows-msvc\\bin\\clang-tidy.exe ../Src/DocumentXml.cpp -quiet -warnings-as-errors=-*,bugprone-*,clang-analyzer-*,misc-*,modernize-avoid-bind,modernize-deprecated-headers,modernize-make-shared,modernize-make-unique,modernize-use-nullptr,performance-*,readability-avoid-const-params-in-decls,readability-const-return-type,readability-non-const-parameter,readability-redundant-*,readability-uniqueptr-delete-release,smokinggun-redundant-pure-call -- -x c++ -std=gnu++1z -DCODE_LAYER_ENGINE -DSGI_DEBUG -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -D_FORTIFY_SOURCE=2 -DANDROID=1 -D_LIBCPP_STD_VER=17 -D_REENTERANT -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_WANT_LIB_EXT1__=1 -D__BIONIC_NO_PAGE_SIZE_MACRO -DSGI_USE_STORE=1 -DSGI_USE_WWISE=1 -DSGI_ENABLE_OPENGL_ES3=1 -DGLES_SILENCE_DEPRECATION=1 -DSGI_ONLINE_ENABLE_LIVEMANAGER -DSGI_ONLINE_ENABLE_ACHIEVEMENTS -DSGI_USE_LIBWEBM=1 -DSGI_USE_LIBVPX=1 -DSGI_USE_APPCENTER=1 -DSGI_USE_PHOTOLIBRARY=1 -DSGI_USE_FILEDIALOG=1 -DSGI_USE_OPENSSL=1 -DSGI_THREADING_MULTICORE -D__ANDROID_MIN_SDK_VERSION__=24 -D__clang_analyzer__ -D__clang_tidy__ --sysroot "C:\\Program Files (x86)\\Android\\android-sdk\\ndk\\27.2.12479018\\toolchains\\llvm\\prebuilt\\windows-x86_64\\sysroot" -target aarch64-linux-android24 -IC:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc -IC:/TC/b668999203ed33d/Dev/Src/Engine/Util/Src -IC:/TC/b668999203ed33d/Dev/Src/Engine/FileIO/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/EditorCore/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Foundation/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Math/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Network/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Platform/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Primitives/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Profiler/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Remote/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/TextIO/Inc --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/cxx-stl/llvm-libc++/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/cxx-stl/llvm-libc++abi/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/aarch64-linux-android" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/android/support/include" -isystem C:/TC/b668999203ed33d/Dev/External -isystem C:/TC/b668999203ed33d/Dev/External/vcpkg/installed/arm64-android-sgi/include -isystem C:/TC/b668999203ed33d/Dev/External/libcxx-support/llvm18_0_0/include -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/aarch64-linux-android" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/lib/clang/18.0.3/include" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include"
1.  <eof> parser at end of file
2.  While analyzing stack: 
  #0 Calling Sgi::Util::Document::Group::AppendProperty(std::string_view, class std::variant<struct std::monostate, std::string, class std::filesystem::path, _Bool, int, unsigned int, long, unsigned long, float, double, struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<int, 10>, double>, struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<long, 32>, double>, struct Sgi::Math::BasicVector<float, 2>, struct Sgi::Math::BasicVector<float, 3>, struct Sgi::Math::BasicVector<float, 4>, struct Sgi::Math::BasicVector<double, 2>, struct Sgi::Math::BasicVector<double, 3>, struct Sgi::Math::BasicVector<double, 4>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<int, 10>, double>, 2>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<int, 10>, double>, 3>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<int, 10>, double>, 4>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<long, 32>, double>, 2>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<long, 32>, double>, 3>, struct Sgi::Util::Document::DocumentFxVector<struct Sgi::Util::Document::DocumentFx<struct Sgi::Math::FixedPoint<long, 32>, double>, 4>, struct Sgi::Math::BasicVector<int, 2>, struct Sgi::Math::BasicVector<int, 3>, struct Sgi::Math::BasicVector<int, 4>, struct Sgi::Math::BasicVector<unsigned int, 2>, struct Sgi::Math::BasicVector<unsigned int, 3>, struct Sgi::Math::BasicVector<unsigned int, 4>, class Sgi::Math::Colour, struct Sgi::PosixTime, class Sgi::TimeSpan> &&) at line 441
  #1 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadGroupWithoutDataInheritance(const pugi::xml_node &, const DocumentReadOptions &, Group &) at line 416
  #2 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadGroupWithoutDataInheritance(const pugi::xml_node &, const DocumentReadOptions &, Group &) at line 720
  #3 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadRootGroup(const pugi::xml_document &, const DocumentReadOptions &) at line 977
  #4 Calling Sgi::Util::Document::Detail::ReadXmlFromString(std::string_view, const DocumentReadOptions &)
3.  C:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc\Util/DocumentGroup.h:449:44: Error evaluating statement
4.  C:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc\Util/DocumentGroup.h:449:44: Error evaluating statement
Exception Code: 0xC0000005
 #0 0x00007ff7409a4814 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1464814)
 #1 0x00007ff7409a4992 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1464992)
 #2 0x00007ff7412aefc0 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d6efc0)
 #3 0x00007ff74133d759 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfd759)
 #4 0x00007ff74133c15c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc15c)
 #5 0x00007ff74133c47b (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc47b)
 #6 0x00007ff7412ac89c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d6c89c)
 #7 0x00007ff74133c5b9 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc5b9)
 #8 0x00007ff74133aea4 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfaea4)
 #9 0x00007ff7412e7fd7 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1da7fd7)
#10 0x00007ff7412e5cf1 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1da5cf1)
#11 0x00007ff7412f546f (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1db546f)
#12 0x00007ff7412deb32 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9eb32)
#13 0x00007ff7412dd3aa (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9d3aa)
#14 0x00007ff7412dd76c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9d76c)
#15 0x00007ff740fe170b (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa170b)
#16 0x00007ff740fe093d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa093d)
#17 0x00007ff740fe0c95 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa0c95)
#18 0x00007ff740ff8603 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1ab8603)
#19 0x00007ff740fe138d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa138d)
#20 0x00007ff74143d4dc (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1efd4dc)
#21 0x00007ff74160a3d7 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x20ca3d7)
#22 0x00007ff741442b87 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1f02b87)
#23 0x00007ff741442a11 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1f02a11)
#24 0x00007ff74142983c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1ee983c)
#25 0x00007ff740f69e30 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a29e30)
#26 0x00007ff740f9b9f3 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a5b9f3)
#27 0x00007ff740f6a02d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a2a02d)
#28 0x00007ff740f69bd0 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a29bd0)
#29 0x00007ff740f691ce (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a291ce)
#30 0x00007ff740f9b85e (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a5b85e)
#31 0x00007ff73f57cee4 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x3cee4)
#32 0x00007ff74053b14c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0xffb14c)
#33 0x00007fff870d259d (C:\Windows\System32\KERNEL32.DLL+0x1259d)
#34 0x00007fff886caf78 (C:\Windows\SYSTEM32\ntdll.dll+0x5af78)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Eric Roberts (EricRobertsSGI)

<details>
&gt; PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.

Stack dump:
```
0.  Program arguments: C:\\TC\\Tools\\LLVM\\19.1.1\\x86_64-windows-msvc\\bin\\clang-tidy.exe ../Src/DocumentXml.cpp -quiet -warnings-as-errors=-*,bugprone-*,clang-analyzer-*,misc-*,modernize-avoid-bind,modernize-deprecated-headers,modernize-make-shared,modernize-make-unique,modernize-use-nullptr,performance-*,readability-avoid-const-params-in-decls,readability-const-return-type,readability-non-const-parameter,readability-redundant-*,readability-uniqueptr-delete-release,smokinggun-redundant-pure-call -- -x c++ -std=gnu++1z -DCODE_LAYER_ENGINE -DSGI_DEBUG -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -D_FORTIFY_SOURCE=2 -DANDROID=1 -D_LIBCPP_STD_VER=17 -D_REENTERANT -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_WANT_LIB_EXT1__=1 -D__BIONIC_NO_PAGE_SIZE_MACRO -DSGI_USE_STORE=1 -DSGI_USE_WWISE=1 -DSGI_ENABLE_OPENGL_ES3=1 -DGLES_SILENCE_DEPRECATION=1 -DSGI_ONLINE_ENABLE_LIVEMANAGER -DSGI_ONLINE_ENABLE_ACHIEVEMENTS -DSGI_USE_LIBWEBM=1 -DSGI_USE_LIBVPX=1 -DSGI_USE_APPCENTER=1 -DSGI_USE_PHOTOLIBRARY=1 -DSGI_USE_FILEDIALOG=1 -DSGI_USE_OPENSSL=1 -DSGI_THREADING_MULTICORE -D__ANDROID_MIN_SDK_VERSION__=24 -D__clang_analyzer__ -D__clang_tidy__ --sysroot "C:\\Program Files (x86)\\Android\\android-sdk\\ndk\\27.2.12479018\\toolchains\\llvm\\prebuilt\\windows-x86_64\\sysroot" -target aarch64-linux-android24 -IC:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc -IC:/TC/b668999203ed33d/Dev/Src/Engine/Util/Src -IC:/TC/b668999203ed33d/Dev/Src/Engine/FileIO/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/EditorCore/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Foundation/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Math/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Network/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Platform/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Primitives/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Profiler/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/Remote/Inc -IC:/TC/b668999203ed33d/Dev/Src/Core/TextIO/Inc --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/cxx-stl/llvm-libc++/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/cxx-stl/llvm-libc++abi/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/aarch64-linux-android" --include-directory "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/sources/android/support/include" -isystem C:/TC/b668999203ed33d/Dev/External -isystem C:/TC/b668999203ed33d/Dev/External/vcpkg/installed/arm64-android-sgi/include -isystem C:/TC/b668999203ed33d/Dev/External/libcxx-support/llvm18_0_0/include -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/aarch64-linux-android" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/lib/clang/18.0.3/include" -isystem "C:/Program Files (x86)/Android/android-sdk/ndk/27.2.12479018/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include"
1.  &lt;eof&gt; parser at end of file
2.  While analyzing stack: 
  #<!-- -->0 Calling Sgi::Util::Document::Group::AppendProperty(std::string_view, class std::variant&lt;struct std::monostate, std::string, class std::filesystem::path, _Bool, int, unsigned int, long, unsigned long, float, double, struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;int, 10&gt;, double&gt;, struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;long, 32&gt;, double&gt;, struct Sgi::Math::BasicVector&lt;float, 2&gt;, struct Sgi::Math::BasicVector&lt;float, 3&gt;, struct Sgi::Math::BasicVector&lt;float, 4&gt;, struct Sgi::Math::BasicVector&lt;double, 2&gt;, struct Sgi::Math::BasicVector&lt;double, 3&gt;, struct Sgi::Math::BasicVector&lt;double, 4&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;int, 10&gt;, double&gt;, 2&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;int, 10&gt;, double&gt;, 3&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;int, 10&gt;, double&gt;, 4&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;long, 32&gt;, double&gt;, 2&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;long, 32&gt;, double&gt;, 3&gt;, struct Sgi::Util::Document::DocumentFxVector&lt;struct Sgi::Util::Document::DocumentFx&lt;struct Sgi::Math::FixedPoint&lt;long, 32&gt;, double&gt;, 4&gt;, struct Sgi::Math::BasicVector&lt;int, 2&gt;, struct Sgi::Math::BasicVector&lt;int, 3&gt;, struct Sgi::Math::BasicVector&lt;int, 4&gt;, struct Sgi::Math::BasicVector&lt;unsigned int, 2&gt;, struct Sgi::Math::BasicVector&lt;unsigned int, 3&gt;, struct Sgi::Math::BasicVector&lt;unsigned int, 4&gt;, class Sgi::Math::Colour, struct Sgi::PosixTime, class Sgi::TimeSpan&gt; &amp;&amp;) at line 441
  #<!-- -->1 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadGroupWithoutDataInheritance(const pugi::xml_node &amp;, const DocumentReadOptions &amp;, Group &amp;) at line 416
  #<!-- -->2 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadGroupWithoutDataInheritance(const pugi::xml_node &amp;, const DocumentReadOptions &amp;, Group &amp;) at line 720
  #<!-- -->3 Calling Sgi::Util::Document::Detail::(anonymous namespace)::ReadRootGroup(const pugi::xml_document &amp;, const DocumentReadOptions &amp;) at line 977
  #<!-- -->4 Calling Sgi::Util::Document::Detail::ReadXmlFromString(std::string_view, const DocumentReadOptions &amp;)
3.  C:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc\Util/DocumentGroup.h:449:44: Error evaluating statement
4.  C:/TC/b668999203ed33d/Dev/Src/Engine/Util/Inc\Util/DocumentGroup.h:449:44: Error evaluating statement
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff7409a4814 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1464814)
 #<!-- -->1 0x00007ff7409a4992 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1464992)
 #<!-- -->2 0x00007ff7412aefc0 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d6efc0)
 #<!-- -->3 0x00007ff74133d759 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfd759)
 #<!-- -->4 0x00007ff74133c15c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc15c)
 #<!-- -->5 0x00007ff74133c47b (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc47b)
 #<!-- -->6 0x00007ff7412ac89c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d6c89c)
 #<!-- -->7 0x00007ff74133c5b9 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfc5b9)
 #<!-- -->8 0x00007ff74133aea4 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1dfaea4)
 #<!-- -->9 0x00007ff7412e7fd7 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1da7fd7)
#<!-- -->10 0x00007ff7412e5cf1 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1da5cf1)
#<!-- -->11 0x00007ff7412f546f (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1db546f)
#<!-- -->12 0x00007ff7412deb32 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9eb32)
#<!-- -->13 0x00007ff7412dd3aa (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9d3aa)
#<!-- -->14 0x00007ff7412dd76c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1d9d76c)
#<!-- -->15 0x00007ff740fe170b (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa170b)
#<!-- -->16 0x00007ff740fe093d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa093d)
#<!-- -->17 0x00007ff740fe0c95 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa0c95)
#<!-- -->18 0x00007ff740ff8603 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1ab8603)
#<!-- -->19 0x00007ff740fe138d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1aa138d)
#<!-- -->20 0x00007ff74143d4dc (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1efd4dc)
#<!-- -->21 0x00007ff74160a3d7 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x20ca3d7)
#<!-- -->22 0x00007ff741442b87 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1f02b87)
#<!-- -->23 0x00007ff741442a11 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1f02a11)
#<!-- -->24 0x00007ff74142983c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1ee983c)
#<!-- -->25 0x00007ff740f69e30 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a29e30)
#<!-- -->26 0x00007ff740f9b9f3 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a5b9f3)
#<!-- -->27 0x00007ff740f6a02d (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a2a02d)
#<!-- -->28 0x00007ff740f69bd0 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a29bd0)
#<!-- -->29 0x00007ff740f691ce (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a291ce)
#<!-- -->30 0x00007ff740f9b85e (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x1a5b85e)
#<!-- -->31 0x00007ff73f57cee4 (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0x3cee4)
#<!-- -->32 0x00007ff74053b14c (C:\TC\Tools\LLVM\19.1.1\x86_64-windows-msvc\bin\clang-tidy.exe+0xffb14c)
#<!-- -->33 0x00007fff870d259d (C:\Windows\System32\KERNEL32.DLL+0x1259d)
#<!-- -->34 0x00007fff886caf78 (C:\Windows\SYSTEM32\ntdll.dll+0x5af78)
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch?

Please provide reproducer, preferably on https://godbolt.org.

---

