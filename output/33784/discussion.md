# `Assertion failed: begin[0] == '\'' && "Invalid token lexed"`, file `C:\src\llvm_package_500_rc4\llvm\tools\clang\lib\Lex\LiteralSupport.cpp`, line 1060

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/33784

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [34436](https://llvm.org/bz34436) |
| Version | 5.0 |
| OS | Windows NT |
| Attachments | [assoc_legendre-c08de7.sh](https://user-images.githubusercontent.com/60944935/143756247-24b819db-cd5b-46cf-94bc-d8d40e42850f.gz), [clang.exe-c00e3b.dmp](https://user-images.githubusercontent.com/60944935/143756248-a924d059-6029-465c-a11d-b34e567f70be.gz), [assoc_legendre-c08de7.cpp (zipped, part 1)](https://user-images.githubusercontent.com/60944935/143756249-b425c844-f24a-4aa4-aed6-d77c1ded583e.gz), [assoc_legendre-c08de7.cpp (zipped, part 2)](https://user-images.githubusercontent.com/60944935/143756250-6e22d4d9-c878-48dd-8c41-c1c4523b93c4.gz), [assoc_legendre-c08de7.cpp (zipped, part 3)](https://user-images.githubusercontent.com/60944935/143756251-06503d60-155c-46f5-8260-d94f2ee50635.gz), [boost_build_log.txt (zipped)](https://user-images.githubusercontent.com/60944935/143756252-753d1c27-21ee-46a6-b234-287275bdf90d.gz), [user-config.jam](https://user-images.githubusercontent.com/60944935/143756253-6c0a7b61-01ea-40ce-876f-5b5afe63dbf9.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@DimitryAndric |

## Extended Description 


compiling boost 1.65 on Windows via LLVM 5.0.0 RC4 causes a lot of crashes:
```
clang-linux.compile.c++.with-pch bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\assoc_legendre.obj

  "C:/Program Files/LLVM/bin/clang.exe" -c -x c++ -I"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\ATLMFC\include" -I"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include" -I"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\include\um" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\ucrt" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\shared" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\winrt" -O0 -g -Winvalid-pch -fno-inline -Wall -g -fmsc-version=1910 -DBOOST_USE_WINAPI_VERSION=0x0502 -DBOOST_USE_WINDOWS_H=1 -D_WIN32_WINNT=0x0502 -DNOMINMAX -pthread -m64 -Winvalid-pch -DBOOST_ALL_NO_LIB=1 -DBOOST_BUILD_PCH_ENABLED -I"." -I"libs\math\src\tr1" -Xclang -include-pth -Xclang "bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\..\src\tr1\pch.hpp.pth" -o "bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\assoc_legendre.obj" "libs\math\build\..\src\tr1\assoc_legendre.cpp"

<built-in>:1:10: warning: non-portable path to file '"c\\projects\boost\libs\math\build\..\src\tr1\pch.hpp"'; specified path differs in case from file name on disk [-Wnonportable-include-path]
#include "c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp"
         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         "c\\projects\boost\libs\math\build\..\src\tr1\pch.hpp"
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:10:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\limits:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\ymath.h:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:72:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\crtdefs.h:9:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:33: error: expected parameter declarator
        char (*__countof_helper(_UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
                                ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:223:24: note: expanded from macro '_UNALIGNED'
    #define _UNALIGNED __unaligned
                       ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:33: error: expected ')'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:223:24: note: expanded from macro '_UNALIGNED'
    #define _UNALIGNED __unaligned
                       ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:32: note: to match this '('
        char (*__countof_helper(_UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
                               ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:10:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\limits:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\ymath.h:6:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:654:2: error: expected member name or ';' after declaration specifiers
        __thiscall _Lockit();   // default construct
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:655:11: error: expected member name or ';' after declaration specifiers
        explicit __thiscall _Lockit(int);       // set the lock
        ~~~~~~~~ ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:656:2: error: expected member name or ';' after declaration specifiers
        __thiscall ~_Lockit() _NOEXCEPT;        // clear the lock
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:772:2: error: expected member name or ';' after declaration specifiers
        __thiscall _Init_locks();
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:773:2: error: expected member name or ';' after declaration specifiers
        __thiscall ~_Init_locks() _NOEXCEPT;
        ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
_NON_MEMBER_CALL_CV_REF(_IS_FUNCTION)
^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &&)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &&)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &&)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '>'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &&)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
Assertion failed: begin[0] == '\'' && "Invalid token lexed", file C:\src\llvm_package_500_rc4\llvm\tools\clang\lib\Lex\LiteralSupport.cpp, line 1060
Wrote crash dump file "C:\Users\SSE4\AppData\Local\Temp\clang.exe-c00e3b.dmp"
0x00007FF7D92680A6 (0x00007FF7DBF08701 0x0000015BFFB6A5F8 0x0000000000000000 0x00007FFB3DC9BF01)
0x00007FFB3DCFDB77 (0x0000000000000401 0x00007FF700000000 0x0000000000000000 0x00000057FDF89010), raise() + 0x1E7 bytes(s)
0x00007FFB3DCFEA01 (0x00007FFB00000003 0x00007FFB00000003 0x00007FF7DBF08B06 0x00007FF7DBF08732), abort() + 0x31 bytes(s)
0x00007FFB3DD006B1 (0x0000000000000424 0x00007FF7DBF08B06 0x0000015B82C1E038 0x00007FF7D945F1CE), _get_wpgmptr() + 0x1BE1 bytes(s)
0x00007FFB3DD009BF (0x0000015B8253FD33 0x0000015BFFFAD490 0x00000057FDF89318 0x00000057FDF891B0), _wassert() + 0x3F bytes(s)
0x00007FF7DAE635E5 (0x0000EC73DA3BCC02 0x0000015B82C1E038 0x0000000000000001 0x0000000000000000)
0x00007FF7DA4EF27A (0x00000057FDF89610 0x0000000000000000 0x0000015BFFB5F7E0 0x0000000000000000)
0x00007FF7DA0725AF (0x00000057FDF89E98 0x000000008026719E 0x00000057FDF8AE70 0x0000015B82C1E750)
0x00007FF7DA06E9A1 (0x0000000000000000 0x00000057FDF89F20 0x0000015B82C1E750 0x00007FF7DA5E969A)
0x00007FF7DA07C7BE (0x0000015B82BEA488 0x00007FF7DA57CAF0 0x00000057FDF8A6D0 0x00000057FDF8A108)
0x00007FF7DA08A7F1 (0x0000000000000040 0x0000000000000000 0x0000000000000000 0x00007FF7DAFE4106)
0x00007FF7DA073BA4 (0x00000057FDF8B3B0 0x0000015BFFF1C0B0 0x00000057FDF8B3B0 0x00007FF7DAE740BF)
0x00007FF7DA071F52 (0x0000015B82C1D750 0x0000005700000000 0xFFFFFFFFFFFFFFF8 0x00007FF7DA594107)
0x00007FF7D9FFF073 (0xAAAAAAAAAAAAAAAB 0x0000015BFFF1FF20 0x0000015BFFB54360 0x00007FF7DAE74595)
0x00007FF7D9FFE59D (0x0000000000000000 0x0000000000000000 0x0000015BFFFABEF8 0x80267252802671CD)
0x00007FF7D9FFE456 (0x00000057FDF8B4E0 0x00000057FDF8B4E0 0x0000000200000008 0x0000000000000000)
0x00007FF7D9FFE7EE (0x0000000A0025258E 0x0000015B82B3D6C0 0x00007FF700020005 0x0000000100252599)
0x00007FF7DA0810BC (0x0000000000000002 0x0000000000000000 0x00000057FDF8C390 0x0000015BFFB5F7E0)
0x00007FF7DA00BD05 (0x0000015B82BBEC20 0x0000000000252465 0x0000015BFFFABEB0 0x0000015BFFB54708)
0x00007FF7DA026DA8 (0x0000002AFFA70000 0x0000000000000104 0x0000000000000000 0x00007FFB412780B7)
0x00007FF7D9FFA2FE (0x0000015BFFAB3B01 0x00000057FDF8D278 0x0000015BFFAC0950 0x0000015BFFAC82D0)
0x00007FF7D9FF9BF7 (0x0000015BFFF24098 0x0000015BFFF240AC 0x00000057FDF8D558 0x00007FF7DA313037)
0x00007FF7D9FF9431 (0x0000015BFFF24098 0x00007FF7DA28F88B 0x0000000080263107 0x0000015B82BEA460)
0x00007FF7DA025FE2 (0x00000057FDF8D6A8 0x0000005700000000 0x0000015B82A09900 0x00000057FDF8D6D0)
0x00007FF7D9FDD081 (0x0000015BFFF1FF01 0x00007FF7DAE786A8 0x00000057FDF8D660 0x0000000000000000)
0x00007FF7DA005D1C (0x0000015B829DCD40 0x00007FF7DAE77102 0x00251D6A00000000 0x0000000000000000)
0x00007FF7DA0057AB (0x0000015BFFF1FF48 0x00007FF7DA00272A 0x0000000000000000 0x00000057FDF8DC48)
0x00007FF7DA02619B (0x00007FF7DAFE4036 0x00007FF700000000 0x0000015BFFF1FF10 0x0000000000000000)
0x00007FF7D9FDD081 (0x0000015BFFACA720 0x00007FF7DAFB7FD5 0x0000000000000000 0x00000057FDF8DCF0)
0x00007FF7D9FDC5C0 (0x0000000800000000 0x000000000000000B 0x0000015B00000000 0x0000000000000000)
0x00007FF7D9FD8FB6 (0x0000000000000000 0x0000000000000000 0x000000000000000F 0x0000015BFFA9D450)
0x00007FF7D98C354D (0x0000005700000014 0x00007FF7DBC66C8D 0x000000000000000E 0x0000015B00000015)
0x00007FF7D9886F2F (0x0000015BFFAA6950 0x0000015BFFAA6950 0x00007FF700000001 0x0000000000000038)
0x00007FF7D990E035 (0x0000000100000003 0x0000015BFFA70000 0x0000015B000003FF 0x0000000000000000)
0x00007FF7D7A864AC (0x000000000000006A 0x0000015BFFA800A6 0x0000015B00000000 0x0000015BFFA8B838)
0x00007FF7D7A84AB9 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FF7DAFE0EE1 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FFB3F648364 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), BaseThreadInitThunk() + 0x14 bytes(s)
0x00007FFB412A7091 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), RtlUserThreadStart() + 0x21 bytes(s)
clang.exe: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 5.0.0 (tags/RELEASE_500/rc4)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\Program Files\LLVM\bin
clang.exe: note: diagnostic msg: PLEASE submit a bug report to http://llvm.org/bugs/ and include the crash backtrace, preprocessed source, and associated run script.
clang.exe: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang.exe: note: diagnostic msg: C:\Users\SSE4\AppData\Local\Temp\assoc_legendre-c08de7.cpp
clang.exe: note: diagnostic msg: C:\Users\SSE4\AppData\Local\Temp\assoc_legendre-c08de7.sh
clang.exe: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot

still happens in 5.0.0 release

---

### Comment 2 - llvmbot

imitry Andric,

I've just uploaded `pch.hpp.pth` file.
with `pch=off` it compiles with no issues.
note that it's not vanilla boost 1.65.0 - it has numerous patches applied to address various clang build failures, that probably will appear in 1.66.0. as for `smart_ptr`, patch is here https://github.com/boostorg/smart_ptr/commit/868a870a596b3f1fb49f96a09d34ba3765c83eee.

---

### Comment 3 - llvmbot

[pch.hpp.pth (zipped, part6)](https://user-images.githubusercontent.com/60944935/143756254-d906e734-6362-415d-a62b-e8c79eb2f557.gz)

---

### Comment 4 - llvmbot

[pch.hpp.pth (zipped, part5)](https://user-images.githubusercontent.com/60944935/143756255-69b6f3fb-a2da-4021-99d1-994f9763fee8.gz)

---

### Comment 5 - llvmbot

[pch.hpp.pth (zipped, part4)](https://user-images.githubusercontent.com/60944935/143756256-70321b9b-2eb2-4c0b-bf66-f73747e1cfe0.gz)

---

### Comment 6 - llvmbot

[pch.hpp.pth (zipped, part3)](https://user-images.githubusercontent.com/60944935/143756257-8c2d78a1-68f8-4756-b769-3cc82257bd21.gz)

---

### Comment 7 - llvmbot

[pch.hpp.pth (zipped, part2)](https://user-images.githubusercontent.com/60944935/143756258-db1e1011-8728-46b3-842e-1c8bffd9e500.gz)

---

### Comment 8 - llvmbot

[pch.hpp.pth (zipped, part1)](https://user-images.githubusercontent.com/60944935/143756259-8f3f373c-775f-4871-b029-e093fb2b26b5.gz)

---

### Comment 9 - DimitryAndric

It does not assert here, with 5.0.0 (branches/release_50 311606), but then again, I do not have the PTH file you have apparently been using:
```
error: invalid or corrupt PTH file 'bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\..\src\tr1\pch.hpp.pth'
error: PTH file 'bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\..\src\tr1\pch.hpp.pth' does not designate an original source header file for -include-pth
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:12:
In file included from .\boost/math/special_functions/bessel.hpp:20:
In file included from .\boost/math/special_functions/detail/bessel_jy.hpp:14:
In file included from .\boost/math/special_functions/gamma.hpp:35:
In file included from .\boost/math/special_functions/bernoulli.hpp:16:
In file included from .\boost/math/special_functions/detail/bernoulli_details.hpp:11:
In file included from .\boost/detail/lightweight_mutex.hpp:20:
In file included from .\boost/smart_ptr/detail/lightweight_mutex.hpp:36:
.\boost/smart_ptr/detail/lwm_win32_cs.hpp:86:9: error: no member named 'InitializeCriticalSection' in namespace 'boost::detail'; did you mean simply 'InitializeCriticalSection'?
        boost::detail::InitializeCriticalSection(reinterpret_cast< ::_RTL_CRITICAL_SECTION* >(&cs_));
        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        InitializeCriticalSection
C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um\synchapi.h:152:1: note: 'InitializeCriticalSection' declared here
InitializeCriticalSection(
^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:12:
In file included from .\boost/math/special_functions/bessel.hpp:20:
In file included from .\boost/math/special_functions/detail/bessel_jy.hpp:14:
In file included from .\boost/math/special_functions/gamma.hpp:35:
In file included from .\boost/math/special_functions/bernoulli.hpp:16:
In file included from .\boost/math/special_functions/detail/bernoulli_details.hpp:11:
In file included from .\boost/detail/lightweight_mutex.hpp:20:
In file included from .\boost/smart_ptr/detail/lightweight_mutex.hpp:36:
.\boost/smart_ptr/detail/lwm_win32_cs.hpp:92:9: error: no member named 'DeleteCriticalSection' in namespace 'boost::detail'; did you mean simply 'DeleteCriticalSection'?
        boost::detail::DeleteCriticalSection(reinterpret_cast< ::_RTL_CRITICAL_SECTION* >(&cs_));
        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DeleteCriticalSection
C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um\synchapi.h:235:1: note: 'DeleteCriticalSection' declared here
DeleteCriticalSection(
^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:12:
In file included from .\boost/math/special_functions/bessel.hpp:20:
In file included from .\boost/math/special_functions/detail/bessel_jy.hpp:14:
In file included from .\boost/math/special_functions/gamma.hpp:35:
In file included from .\boost/math/special_functions/bernoulli.hpp:16:
In file included from .\boost/math/special_functions/detail/bernoulli_details.hpp:11:
In file included from .\boost/detail/lightweight_mutex.hpp:20:
In file included from .\boost/smart_ptr/detail/lightweight_mutex.hpp:36:
.\boost/smart_ptr/detail/lwm_win32_cs.hpp:111:13: error: no member named 'EnterCriticalSection' in namespace 'boost::detail'; did you mean simply 'EnterCriticalSection'?
            boost::detail::EnterCriticalSection(reinterpret_cast< ::_RTL_CRITICAL_SECTION* >(&m_.cs_));
            ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            EnterCriticalSection
C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um\synchapi.h:172:1: note: 'EnterCriticalSection' declared here
EnterCriticalSection(
^
In file included from <built-in>:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:12:
In file included from .\boost/math/special_functions/bessel.hpp:20:
In file included from .\boost/math/special_functions/detail/bessel_jy.hpp:14:
In file included from .\boost/math/special_functions/gamma.hpp:35:
In file included from .\boost/math/special_functions/bernoulli.hpp:16:
In file included from .\boost/math/special_functions/detail/bernoulli_details.hpp:11:
In file included from .\boost/detail/lightweight_mutex.hpp:20:
In file included from .\boost/smart_ptr/detail/lightweight_mutex.hpp:36:
.\boost/smart_ptr/detail/lwm_win32_cs.hpp:116:13: error: no member named 'LeaveCriticalSection' in namespace 'boost::detail'; did you mean simply 'LeaveCriticalSection'?
            boost::detail::LeaveCriticalSection(reinterpret_cast< ::_RTL_CRITICAL_SECTION* >(&m_.cs_));
            ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            LeaveCriticalSection
C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um\synchapi.h:180:1: note: 'LeaveCriticalSection' declared here
LeaveCriticalSection(
^
6 errors generated.
```
And it simply gives 6 compilation errors.  Maybe you can dig up the `bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\..\src\tr1\pch.hpp.pth` file, and post it too?

Alternatively, try to compile without precompiled headers, and see if the assertions still reproduce.

---

### Comment 10 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [34436](https://llvm.org/bz34436) |
| Version | 5.0 |
| OS | Windows NT |
| Attachments | [assoc_legendre-c08de7.sh](https://user-images.githubusercontent.com/60944935/143756247-24b819db-cd5b-46cf-94bc-d8d40e42850f.gz), [clang.exe-c00e3b.dmp](https://user-images.githubusercontent.com/60944935/143756248-a924d059-6029-465c-a11d-b34e567f70be.gz), [assoc_legendre-c08de7.cpp (zipped, part 1)](https://user-images.githubusercontent.com/60944935/143756249-b425c844-f24a-4aa4-aed6-d77c1ded583e.gz), [assoc_legendre-c08de7.cpp (zipped, part 2)](https://user-images.githubusercontent.com/60944935/143756250-6e22d4d9-c878-48dd-8c41-c1c4523b93c4.gz), [assoc_legendre-c08de7.cpp (zipped, part 3)](https://user-images.githubusercontent.com/60944935/143756251-06503d60-155c-46f5-8260-d94f2ee50635.gz), [boost_build_log.txt (zipped)](https://user-images.githubusercontent.com/60944935/143756252-753d1c27-21ee-46a6-b234-287275bdf90d.gz), [user-config.jam](https://user-images.githubusercontent.com/60944935/143756253-6c0a7b61-01ea-40ce-876f-5b5afe63dbf9.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor,@<!-- -->DimitryAndric |

## Extended Description 


compiling boost 1.65 on Windows via LLVM 5.0.0 RC4 causes a lot of crashes:
```
clang-linux.compile.c++.with-pch bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\assoc_legendre.obj

  "C:/Program Files/LLVM/bin/clang.exe" -c -x c++ -I"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\ATLMFC\include" -I"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include" -I"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\include\um" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\ucrt" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\shared" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\um" -I"C:\Program Files (x86)\Windows Kits\10\include\10.0.15063.0\winrt" -O0 -g -Winvalid-pch -fno-inline -Wall -g -fmsc-version=1910 -DBOOST_USE_WINAPI_VERSION=0x0502 -DBOOST_USE_WINDOWS_H=1 -D_WIN32_WINNT=0x0502 -DNOMINMAX -pthread -m64 -Winvalid-pch -DBOOST_ALL_NO_LIB=1 -DBOOST_BUILD_PCH_ENABLED -I"." -I"libs\math\src\tr1" -Xclang -include-pth -Xclang "bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\..\src\tr1\pch.hpp.pth" -o "bin.v2\libs\math\build\clang-gnu-linux-5.0\debug\link-static\threading-multi\assoc_legendre.obj" "libs\math\build\..\src\tr1\assoc_legendre.cpp"

&lt;built-in&gt;:1:10: warning: non-portable path to file '"c\\projects\boost\libs\math\build\..\src\tr1\pch.hpp"'; specified path differs in case from file name on disk [-Wnonportable-include-path]
#include "c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp"
         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         "c\\projects\boost\libs\math\build\..\src\tr1\pch.hpp"
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:10:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\limits:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\ymath.h:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:72:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\crtdefs.h:9:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:33: error: expected parameter declarator
        char (*__countof_helper(_UNALIGNED _CountofType (&amp;_Array)[_SizeOfArray]))[_SizeOfArray];
                                ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:223:24: note: expanded from macro '_UNALIGNED'
    #define _UNALIGNED __unaligned
                       ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:33: error: expected ')'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:223:24: note: expanded from macro '_UNALIGNED'
    #define _UNALIGNED __unaligned
                       ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vcruntime.h:232:32: note: to match this '('
        char (*__countof_helper(_UNALIGNED _CountofType (&amp;_Array)[_SizeOfArray]))[_SizeOfArray];
                               ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:10:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\limits:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\ymath.h:6:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:654:2: error: expected member name or ';' after declaration specifiers
        __thiscall _Lockit();   // default construct
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:655:11: error: expected member name or ';' after declaration specifiers
        explicit __thiscall _Lockit(int);       // set the lock
        ~~~~~~~~ ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:656:2: error: expected member name or ';' after declaration specifiers
        __thiscall ~_Lockit() _NOEXCEPT;        // clear the lock
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:772:2: error: expected member name or ';' after declaration specifiers
        __thiscall _Init_locks();
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\yvals.h:773:2: error: expected member name or ';' after declaration specifiers
        __thiscall ~_Init_locks() _NOEXCEPT;
        ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
_NON_MEMBER_CALL_CV_REF(_IS_FUNCTION)
^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:322:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, ) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:323:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;&amp;)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:316:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;&amp;)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:317:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;&amp;)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:318:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, volatile REF_OPT) \
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
In file included from &lt;built-in&gt;:1:
In file included from c:\projects\boost\libs\math\build\..\src\tr1\pch.hpp:9:
In file included from .\boost/math/special_functions.hpp:15:
In file included from .\boost/math/special_functions/airy.hpp:11:
In file included from .\boost/math/special_functions/math_fwd.hpp:26:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\vector:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xmemory0:9:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\new:6:
In file included from C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\exception:7:
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\type_traits:54:1: error: expected '&gt;'
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:324:2: note: expanded from macro '_NON_MEMBER_CALL_CV_REF'
        _NON_MEMBER_CALL_CV(FUNC, &amp;&amp;)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:319:2: note: expanded from macro '_NON_MEMBER_CALL_CV'
        _NON_MEMBER_CALL(FUNC, const volatile REF_OPT)
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:313:2: note: expanded from macro '_NON_MEMBER_CALL'
        _EMIT_VECTORCALL(FUNC, CV_REF_OPT, )
        ^
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include\xstddef:302:7: note: expanded from macro '_EMIT_VECTORCALL'
        FUNC(__vectorcall, OPT1, OPT2)
             ^
Assertion failed: begin[0] == '\'' &amp;&amp; "Invalid token lexed", file C:\src\llvm_package_500_rc4\llvm\tools\clang\lib\Lex\LiteralSupport.cpp, line 1060
Wrote crash dump file "C:\Users\SSE4\AppData\Local\Temp\clang.exe-c00e3b.dmp"
0x00007FF7D92680A6 (0x00007FF7DBF08701 0x0000015BFFB6A5F8 0x0000000000000000 0x00007FFB3DC9BF01)
0x00007FFB3DCFDB77 (0x0000000000000401 0x00007FF700000000 0x0000000000000000 0x00000057FDF89010), raise() + 0x1E7 bytes(s)
0x00007FFB3DCFEA01 (0x00007FFB00000003 0x00007FFB00000003 0x00007FF7DBF08B06 0x00007FF7DBF08732), abort() + 0x31 bytes(s)
0x00007FFB3DD006B1 (0x0000000000000424 0x00007FF7DBF08B06 0x0000015B82C1E038 0x00007FF7D945F1CE), _get_wpgmptr() + 0x1BE1 bytes(s)
0x00007FFB3DD009BF (0x0000015B8253FD33 0x0000015BFFFAD490 0x00000057FDF89318 0x00000057FDF891B0), _wassert() + 0x3F bytes(s)
0x00007FF7DAE635E5 (0x0000EC73DA3BCC02 0x0000015B82C1E038 0x0000000000000001 0x0000000000000000)
0x00007FF7DA4EF27A (0x00000057FDF89610 0x0000000000000000 0x0000015BFFB5F7E0 0x0000000000000000)
0x00007FF7DA0725AF (0x00000057FDF89E98 0x000000008026719E 0x00000057FDF8AE70 0x0000015B82C1E750)
0x00007FF7DA06E9A1 (0x0000000000000000 0x00000057FDF89F20 0x0000015B82C1E750 0x00007FF7DA5E969A)
0x00007FF7DA07C7BE (0x0000015B82BEA488 0x00007FF7DA57CAF0 0x00000057FDF8A6D0 0x00000057FDF8A108)
0x00007FF7DA08A7F1 (0x0000000000000040 0x0000000000000000 0x0000000000000000 0x00007FF7DAFE4106)
0x00007FF7DA073BA4 (0x00000057FDF8B3B0 0x0000015BFFF1C0B0 0x00000057FDF8B3B0 0x00007FF7DAE740BF)
0x00007FF7DA071F52 (0x0000015B82C1D750 0x0000005700000000 0xFFFFFFFFFFFFFFF8 0x00007FF7DA594107)
0x00007FF7D9FFF073 (0xAAAAAAAAAAAAAAAB 0x0000015BFFF1FF20 0x0000015BFFB54360 0x00007FF7DAE74595)
0x00007FF7D9FFE59D (0x0000000000000000 0x0000000000000000 0x0000015BFFFABEF8 0x80267252802671CD)
0x00007FF7D9FFE456 (0x00000057FDF8B4E0 0x00000057FDF8B4E0 0x0000000200000008 0x0000000000000000)
0x00007FF7D9FFE7EE (0x0000000A0025258E 0x0000015B82B3D6C0 0x00007FF700020005 0x0000000100252599)
0x00007FF7DA0810BC (0x0000000000000002 0x0000000000000000 0x00000057FDF8C390 0x0000015BFFB5F7E0)
0x00007FF7DA00BD05 (0x0000015B82BBEC20 0x0000000000252465 0x0000015BFFFABEB0 0x0000015BFFB54708)
0x00007FF7DA026DA8 (0x0000002AFFA70000 0x0000000000000104 0x0000000000000000 0x00007FFB412780B7)
0x00007FF7D9FFA2FE (0x0000015BFFAB3B01 0x00000057FDF8D278 0x0000015BFFAC0950 0x0000015BFFAC82D0)
0x00007FF7D9FF9BF7 (0x0000015BFFF24098 0x0000015BFFF240AC 0x00000057FDF8D558 0x00007FF7DA313037)
0x00007FF7D9FF9431 (0x0000015BFFF24098 0x00007FF7DA28F88B 0x0000000080263107 0x0000015B82BEA460)
0x00007FF7DA025FE2 (0x00000057FDF8D6A8 0x0000005700000000 0x0000015B82A09900 0x00000057FDF8D6D0)
0x00007FF7D9FDD081 (0x0000015BFFF1FF01 0x00007FF7DAE786A8 0x00000057FDF8D660 0x0000000000000000)
0x00007FF7DA005D1C (0x0000015B829DCD40 0x00007FF7DAE77102 0x00251D6A00000000 0x0000000000000000)
0x00007FF7DA0057AB (0x0000015BFFF1FF48 0x00007FF7DA00272A 0x0000000000000000 0x00000057FDF8DC48)
0x00007FF7DA02619B (0x00007FF7DAFE4036 0x00007FF700000000 0x0000015BFFF1FF10 0x0000000000000000)
0x00007FF7D9FDD081 (0x0000015BFFACA720 0x00007FF7DAFB7FD5 0x0000000000000000 0x00000057FDF8DCF0)
0x00007FF7D9FDC5C0 (0x0000000800000000 0x000000000000000B 0x0000015B00000000 0x0000000000000000)
0x00007FF7D9FD8FB6 (0x0000000000000000 0x0000000000000000 0x000000000000000F 0x0000015BFFA9D450)
0x00007FF7D98C354D (0x0000005700000014 0x00007FF7DBC66C8D 0x000000000000000E 0x0000015B00000015)
0x00007FF7D9886F2F (0x0000015BFFAA6950 0x0000015BFFAA6950 0x00007FF700000001 0x0000000000000038)
0x00007FF7D990E035 (0x0000000100000003 0x0000015BFFA70000 0x0000015B000003FF 0x0000000000000000)
0x00007FF7D7A864AC (0x000000000000006A 0x0000015BFFA800A6 0x0000015B00000000 0x0000015BFFA8B838)
0x00007FF7D7A84AB9 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FF7DAFE0EE1 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FFB3F648364 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), BaseThreadInitThunk() + 0x14 bytes(s)
0x00007FFB412A7091 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), RtlUserThreadStart() + 0x21 bytes(s)
clang.exe: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 5.0.0 (tags/RELEASE_500/rc4)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\Program Files\LLVM\bin
clang.exe: note: diagnostic msg: PLEASE submit a bug report to http://llvm.org/bugs/ and include the crash backtrace, preprocessed source, and associated run script.
clang.exe: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang.exe: note: diagnostic msg: C:\Users\SSE4\AppData\Local\Temp\assoc_legendre-c08de7.cpp
clang.exe: note: diagnostic msg: C:\Users\SSE4\AppData\Local\Temp\assoc_legendre-c08de7.sh
clang.exe: note: diagnostic msg: 

********************
```
</details>


---

