# 21.1.8  crash building KDE plasma

**Author:** blinkingled
**URL:** https://github.com/llvm/llvm-project/issues/182713
**Status:** Closed
**Labels:** clang:frontend, crash, needs-reduction
**Closed Date:** 2026-02-22T16:13:59Z

## Body

Using kde-builder to build KDE plasma from main branch fails with below error.

[61/134] Building CXX object src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o
FAILED: [code=1] src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o 
/usr/bin/clang++ -DKF6I18nLocaleData_EXPORTS -DKI18NLOCALEDATA_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18NLOCALEDATA_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DKI18N_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18N_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DQT_CORE_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x70000 -DQT_DISABLE_DEPRECATED_BEFORE=0x60b00 -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_DEBUG -DQT_NO_FOREACH -DQT_NO_KEYWORDS -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT -DQT_NO_URL_CAST_FROM_STRING -DQT_STRICT_ITERATORS -DQT_USE_QSTRINGBUILDER -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -I/volumes/fastdata/kde/build/ki18n/src/localedata -I/volumes/ssddata/kde/src/ki18n/src/localedata -I/volumes/fastdata/kde/build/ki18n -I/volumes/fastdata/kde/build/ki18n/src/i18n -I/volumes/ssddata/kde/src/ki18n/src/i18n -isystem /volumes/fastdata/kde/build/ki18n/src/localedata/KF6I18nLocaleData_autogen/include -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib/qt6/mkspecs/linux-g++ -fuse-ld=lld -march=native -mtune=native -flto -fno-operator-names -fno-exceptions -Wno-gnu-zero-variadic-macro-arguments -Wall -Wextra -Wcast-align -Wchar-subscripts -Wformat-security -Wno-long-long -Wpointer-arith -Wundef -Wnon-virtual-dtor -Woverloaded-virtual -Werror=return-type -Werror=init-self -Werror=undef -Wvla -Wdate-time -pedantic -Wzero-as-null-pointer-constant -fdiagnostics-color=always -O2 -g -DNDEBUG -std=c++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fno-direct-access-external-data -MD -MT src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -MF src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o.d -o src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -c /volumes/ssddata/kde/src/ki18n/src/localedata/spatial_index_property.cpp
clang++: warning: argument unused during compilation: '-fuse-ld=lld' [-Wunused-command-line-argument]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: /usr/bin/clang++ -DKF6I18nLocaleData_EXPORTS -DKI18NLOCALEDATA_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18NLOCALEDATA_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DKI18N_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18N_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DQT_CORE_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x70000 -DQT_DISABLE_DEPRECATED_BEFORE=0x60b00 -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_DEBUG -DQT_NO_FOREACH -DQT_NO_KEYWORDS -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT -DQT_NO_URL_CAST_FROM_STRING -DQT_STRICT_ITERATORS -DQT_USE_QSTRINGBUILDER -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -I/volumes/fastdata/kde/build/ki18n/src/localedata -I/volumes/ssddata/kde/src/ki18n/src/localedata -I/volumes/fastdata/kde/build/ki18n -I/volumes/fastdata/kde/build/ki18n/src/i18n -I/volumes/ssddata/kde/src/ki18n/src/i18n -isystem /volumes/fastdata/kde/build/ki18n/src/localedata/KF6I18nLocaleData_autogen/include -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib/qt6/mkspecs/linux-g++ -fuse-ld=lld -march=native -mtune=native -flto -fno-operator-names -fno-exceptions -Wno-gnu-zero-variadic-macro-arguments -Wall -Wextra -Wcast-align -Wchar-subscripts -Wformat-security -Wno-long-long -Wpointer-arith -Wundef -Wnon-virtual-dtor -Woverloaded-virtual -Werror=return-type -Werror=init-self -Werror=undef -Wvla -Wdate-time -pedantic -Wzero-as-null-pointer-constant -fdiagnostics-color=always -O2 -g -DNDEBUG -std=c++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fno-direct-access-external-data -MD -MT src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -MF src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o.d -o src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -c /volumes/ssddata/kde/src/ki18n/src/localedata/spatial_index_property.cpp
  	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:568:52: current parser token ')'
  	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:50:1: parsing namespace 'std'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:558:3: parsing struct/union/class body 'std::system_error'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:3961:5: instantiating function definition 'std::operator+<char, std::char_traits<char>, std::allocator<char>>'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:2216:7: instantiating function definition 'std::basic_string<char>::insert'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:2458:7: instantiating function definition 'std::basic_string<char>::replace'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.tcc:543:5: instantiating function definition 'std::basic_string<char>::_M_replace'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:458:7: instantiating function definition 'std::basic_string<char>::_S_move'
     0x00007f2858b081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
     0x00007f2858b05cc7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
     0x00007f2858b05cc7 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:373:31
     0x00007f28589da6b9 HandleCrash /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:73:5
     0x00007f28589da6b9 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:390:62
     0x00007f2857c4d2d0 (/usr/lib/libc.so.6+0x3e2d0)
     0x00007f28633cc4a6 clang::ImplicitConversionSequence::isInitialized() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/Overload.h:760:56
    
[stack-trace.txt](https://github.com/user-attachments/files/25462893/stack-trace.txt)

 0x00007f28633cc4a6 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Sema/SemaOverload.cpp:7818:53
[spatial_index_property-cfb94d.cpp](https://github.com/user-attachments/files/25462884/spatial_index_property-cfb94d.cpp)
[spatial_index_property-cfb94d.sh](https://github.com/user-attachments/files/25462883/spatial_index_property-cfb94d.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (blinkingled)

<details>
Using kde-builder to build KDE plasma from main branch fails with below error.

[61/134] Building CXX object src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o
FAILED: [code=1] src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o 
/usr/bin/clang++ -DKF6I18nLocaleData_EXPORTS -DKI18NLOCALEDATA_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18NLOCALEDATA_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DKI18N_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18N_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DQT_CORE_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x70000 -DQT_DISABLE_DEPRECATED_BEFORE=0x60b00 -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_DEBUG -DQT_NO_FOREACH -DQT_NO_KEYWORDS -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT -DQT_NO_URL_CAST_FROM_STRING -DQT_STRICT_ITERATORS -DQT_USE_QSTRINGBUILDER -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -I/volumes/fastdata/kde/build/ki18n/src/localedata -I/volumes/ssddata/kde/src/ki18n/src/localedata -I/volumes/fastdata/kde/build/ki18n -I/volumes/fastdata/kde/build/ki18n/src/i18n -I/volumes/ssddata/kde/src/ki18n/src/i18n -isystem /volumes/fastdata/kde/build/ki18n/src/localedata/KF6I18nLocaleData_autogen/include -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib/qt6/mkspecs/linux-g++ -fuse-ld=lld -march=native -mtune=native -flto -fno-operator-names -fno-exceptions -Wno-gnu-zero-variadic-macro-arguments -Wall -Wextra -Wcast-align -Wchar-subscripts -Wformat-security -Wno-long-long -Wpointer-arith -Wundef -Wnon-virtual-dtor -Woverloaded-virtual -Werror=return-type -Werror=init-self -Werror=undef -Wvla -Wdate-time -pedantic -Wzero-as-null-pointer-constant -fdiagnostics-color=always -O2 -g -DNDEBUG -std=c++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fno-direct-access-external-data -MD -MT src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -MF src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o.d -o src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -c /volumes/ssddata/kde/src/ki18n/src/localedata/spatial_index_property.cpp
clang++: warning: argument unused during compilation: '-fuse-ld=lld' [-Wunused-command-line-argument]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: /usr/bin/clang++ -DKF6I18nLocaleData_EXPORTS -DKI18NLOCALEDATA_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18NLOCALEDATA_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DKI18N_DEPRECATED_WARNINGS_SINCE=0x0 -DKI18N_DISABLE_DEPRECATED_BEFORE_AND_AT=0x0 -DQT_CORE_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x70000 -DQT_DISABLE_DEPRECATED_BEFORE=0x60b00 -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_DEBUG -DQT_NO_FOREACH -DQT_NO_KEYWORDS -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT -DQT_NO_URL_CAST_FROM_STRING -DQT_STRICT_ITERATORS -DQT_USE_QSTRINGBUILDER -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -I/volumes/fastdata/kde/build/ki18n/src/localedata -I/volumes/ssddata/kde/src/ki18n/src/localedata -I/volumes/fastdata/kde/build/ki18n -I/volumes/fastdata/kde/build/ki18n/src/i18n -I/volumes/ssddata/kde/src/ki18n/src/i18n -isystem /volumes/fastdata/kde/build/ki18n/src/localedata/KF6I18nLocaleData_autogen/include -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib/qt6/mkspecs/linux-g++ -fuse-ld=lld -march=native -mtune=native -flto -fno-operator-names -fno-exceptions -Wno-gnu-zero-variadic-macro-arguments -Wall -Wextra -Wcast-align -Wchar-subscripts -Wformat-security -Wno-long-long -Wpointer-arith -Wundef -Wnon-virtual-dtor -Woverloaded-virtual -Werror=return-type -Werror=init-self -Werror=undef -Wvla -Wdate-time -pedantic -Wzero-as-null-pointer-constant -fdiagnostics-color=always -O2 -g -DNDEBUG -std=c++20 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fno-direct-access-external-data -MD -MT src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -MF src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o.d -o src/localedata/CMakeFiles/KF6I18nLocaleData.dir/spatial_index_property.cpp.o -c /volumes/ssddata/kde/src/ki18n/src/localedata/spatial_index_property.cpp
  	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:568:52: current parser token ')'
  	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:50:1: parsing namespace 'std'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/system_error:558:3: parsing struct/union/class body 'std::system_error'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:3961:5: instantiating function definition 'std::operator+&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:2216:7: instantiating function definition 'std::basic_string&lt;char&gt;::insert'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:2458:7: instantiating function definition 'std::basic_string&lt;char&gt;::replace'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.tcc:543:5: instantiating function definition 'std::basic_string&lt;char&gt;::_M_replace'
 	/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/bits/basic_string.h:458:7: instantiating function definition 'std::basic_string&lt;char&gt;::_S_move'
     0x00007f2858b081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
     0x00007f2858b05cc7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
     0x00007f2858b05cc7 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:373:31
     0x00007f28589da6b9 HandleCrash /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:73:5
     0x00007f28589da6b9 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:390:62
     0x00007f2857c4d2d0 (/usr/lib/libc.so.6+0x3e2d0)
     0x00007f28633cc4a6 clang::ImplicitConversionSequence::isInitialized() const /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/Overload.h:760:56
    
[stack-trace.txt](https://github.com/user-attachments/files/25462893/stack-trace.txt)

 0x00007f28633cc4a6 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Sema/SemaOverload.cpp:7818:53
[spatial_index_property-cfb94d.cpp](https://github.com/user-attachments/files/25462884/spatial_index_property-cfb94d.cpp)
[spatial_index_property-cfb94d.sh](https://github.com/user-attachments/files/25462883/spatial_index_property-cfb94d.sh)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 3 - blinkingled

clang version 22.1.0-rc3 (https://github.com/llvm/llvm-project 8530a2cf135c6091438b92b3778d1f23bf77d905)
Target: x86_64-unknown-linux-gnu

That version dies in some other place.

[new-stack-trace.txt](https://github.com/user-attachments/files/25470298/new-stack-trace.txt)
[solidnamespace-84c04f.cpp](https://github.com/user-attachments/files/25470296/solidnamespace-84c04f.cpp)
[solidnamespace-84c04f.sh](https://github.com/user-attachments/files/25470297/solidnamespace-84c04f.sh)

Starting to look like I might have hardware issues - or GCC 15.2.1 segfaults building solid albeit at a different place. Closing until I can validate.

---

