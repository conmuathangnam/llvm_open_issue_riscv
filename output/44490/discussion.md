# Clang typo correction never completes, runs out of memory

**Author:** Kojoley
**URL:** https://github.com/llvm/llvm-project/issues/44490
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:43Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [45145](https://llvm.org/bz45145) |
| Version | trunk |
| OS | Windows NT |
| Attachments | [reproducer and console output](https://user-images.githubusercontent.com/92601389/143760463-4fc40b38-2731-44b1-83a5-8ae856504ec8.gz) |
| CC | @dwblaikie,@zygoloid,@vedantk |

## Extended Description 


LLVM ERROR: out of memory
Stack dump:
0.      Program arguments: G:\LLVM\bin\clang-cl.exe -cc1 -triple i386-pc-windows-msvc19.24.28316 -emit-obj -mrelax-all -mincremental-linker-compatible -disable-free -disable-llvm-verifier -discard-val
ue-names -main-file-name zip_view.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -relaxed-aliasing -fmath-errno -masm-verbose -mconstructor-aliases -target-cpu pentium4 -mllvm -x
86-asm-syntax=intel -D_DEBUG -D_MT -D_DLL --dependent-lib=msvcrtd --dependent-lib=oldnames -stack-protector 2 -fcxx-exceptions -fexceptions -fms-volatile -fdiagnostics-format msvc -gcodeview -debug-in
fo-kind=limited -coverage-notes-file C:\Working\Repositories\boost\zip_view.gcno -resource-dir G:\LLVM\lib\clang\9.0.1 -D BOOST_ALL_NO_LIB=1 -I . -internal-isystem G:\LLVM\lib\clang\9.0.1\include -int
ernal-isystem C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\include -internal-isystem C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt -intern
al-isystem C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\shared -internal-isystem C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\um -internal-isystem C:\Program Files (x86)\
Windows Kits\10\include\10.0.17763.0\winrt -O0 -Wall -fdeprecated-macro -fdebug-compilation-dir C:\Working\Repositories\boost -ferror-limit 19 -fmessage-length 0 -fno-use-cxa-atexit -fms-extensions -f
ms-compatibility -fms-compatibility-version=19.24.28316 -std=c++14 -fdelayed-template-parsing -fno-inline -fobjc-runtime=gcc -fdiagnostics-show-option -faddrsig -o bin.v2\libs\mpl\test\zip_view.test\c
lang-win-9.0.1\debug\threading-multi\zip_view.obj -x c++ libs\mpl\test\zip_view.cpp
1.      libs\mpl\test\zip_view.cpp:38:5 <Spelling=.\boost/mpl/assert.hpp:264:32>: current parser token ','
2.      libs\mpl\test\zip_view.cpp:32:1: parsing function body 'test31'
3.      libs\mpl\test\zip_view.cpp:32:1: in compound statement ('{}')
0x0000000140FC90C6 (0x0000000000000016 0x000007FEE0DF8741 0x0000001B00000000 0x000000014246FE62)
0x000007FEE0E4A9FF (0x0000000000000001 0x0000000000000000 0x0000000000000014 0x0000000000A39AD0), raise() + 0x1CF bytes(s)
0x000007FEE0E4B6F1 (0x0000000000000003 0x0000000000000003 0x0000000000A39AD0 0x0000000142D5DCE1), abort() + 0x31 bytes(s)
0x0000000140FBCB8D (0x0000000000000014 0x0000000000A39B60 0x0000000142914490 0x000000014291447C)
0x000000013F99158D (0x0000000000000001 0x0000000005C71858 0x0000000005C71850 0x0000000000A3A4C8)
0x00000001428CF668 (0x0000000000A39F28 0x00000001424DFF4C 0x0000000000000030 0x0000EBD203F62070)
0x000000014247B100 (0x0000000002820960 0x0000000000000000 0x0000000000000000 0x0000000000019000)
0x0000000142482F99 (0x0000000000000000 0x0000000000A3AB00 0x0000000000000015 0x0000000002820080)
0x0000000142482206 (0x0000000000A3A4F0 0x00000001429A8652 0x0000000005678800 0x0000000004E93718)
0x000000014247C9F3 (0x0000000005C73090 0x0000000000000000 0x0000000004FE27F8 0x00000001423DB077)
0x000000014247B0C1 (0x0000000280A88153 0x0000000000BC1DB3 0x0000000000000007 0x0000000180A88155)
0x0000000142482F99 (0x0000000000030000 0x0000000880A83EB7 0x0000000005E51210 0x0000000000020005)
0x0000000142482206 (0x0000000000000000 0x0000000000AD0000 0x0000000009250190 0x0000000000000000)
0x000000014247C9F3 (0x0000000000B68BA0 0x0000000100000000 0x0000000000000000 0x0000000142B9B0CE)
0x000000014247B0C1 (0x000000007FFFFFFF 0x0000000002E601B8 0x0000000000000000 0x0000000000000000)
0x0000000142482F99 (0x0000000000000000 0x0000000000000000 0x0000EBD203F614B0 0x0000000000A3B200)
0x0000000142482206 (0x0000000005C70700 0x0000000000A3B750 0x0000000002E601B8 0x0000000000BC6E90)
0x000000014247C9F3 (0x0000000080A88188 0x0000000000000000 0x0000000002E5FEA0 0x0000000000000000)
0x000000014247B0C1 (0x0000000000000000 0x0000000000A3BE00 0x0000000000A3C010 0x0000000000000001)
0x00000001424750AA (0x0000000000A3B9DC 0x000000000000000E 0x0000000005C72DB0 0x0000000142D5A2C3)
0x000000014246FF6A (0x0000000005BB08E0 0x0000000100000000 0x00000000036A5038 0x0000000000000000)
0x00000001424797ED (0x0000000000A3BCF0 0x000002EF00000000 0x0000000077AB3128 0x0000000000000000)
0x0000000142470426 (0x0000000000000000 0x0000000000000000 0x0000000000A3BEF0 0x0000000400000000)
0x000000014246FC2C (0x0000000005BB0878 0x0000000000B8E3F8 0x0000000000A3C040 0x0000000000B67CB0)
0x00000001424517F5 (0x0000000000000000 0x0000EBD203F67BE0 0x0000000000000000 0x0000000000000000)
0x00000001424EAFD5 (0x0000000000000000 0x00000001424DFC0F 0x0000000000000008 0x0000000002820080)
0x0000000142164755 (0x0000000000B85DB0 0x0000000000BC1CD5 0x0000000000000007 0x00000000036599F0)
0x00000001421008C7 (0x0000000000BCE870 0x0000000142B597D0 0x0000000000BC1CD5 0x0000000000A3C510)
0x000000014211BE30 (0x0000000005BB0610 0x0000000005BB0610 0x0000000000000000 0x0000000000000000)
0x0000000142151328 (0x0000000005BB0580 0x0000000005BB0580 0x0000000000000000 0x0000000000000000)
0x0000000142150E4C (0x0000000000B67CB0 0x0000000000A3C7A8 0x00000000000564AF 0x00000001411611CE)
0x0000000142159C69 (0x0000000005C706D8 0x000000014236EB76 0x000000000519E090 0x0000EBD280A88050)
0x000000014215A906 (0x0000000000B67CB0 0x0000000141161907 0x000000007FFFFFFF 0x0000000000A3CBC8)
0x00000001420DDD08 (0x00000000058C7170 0x0000000100A8468F 0x0000EBD203F67760 0x0000000000B4BC00)
0x0000000142121382 (0x0000000000010000 0x00000000007933C2 0x0000EBD203F66CD0 0x0000000000010000)
0x00000001420DD06B (0x00000000050943F0 0x0000000142B5EAA4 0x0000000000A3D860 0x00000000007913FA)
0x00000001420DC9FC (0x0000000000B89860 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00000001420DB88C (0x0000000000B87C70 0x0000000142D461DE 0x0000000002820080 0x0000000000000000)
0x00000001420D9DC2 (0x0000000000A3DE08 0x0000000000A3DE18 0x0000000000A3DE48 0x00000001415EB70A)
0x00000001420D5BCE (0x000000000000001F 0x0000EBD203F662A0 0x0000000000000000 0x000000000000000F)
0x000000014162AC52 (0x0000000000B50D80 0x0000000142D5A2C3 0x00000000000000C0 0x0000000000A3DE38)
0x00000001415EE64F (0x0000000000B62101 0x0000000000000000 0x0000000000A3ED01 0x0000000000000000)
0x0000000141681A37 (0x0000000000000282 0x000000000000017E 0x0000000000000000 0x0000000000000000)
0x000000013F976F57 (0x0000000000AD0000 0x0061000000000002 0x0000000077AB3128 0x0000000000000000)
0x000000013F974564 (0x0000000000000000 0x0000000000000000 0x01D5F57C4684595C 0x01D5F57C46847878)
0x0000000142D5A4F0 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00000000778A556D (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), BaseThreadInitThunk() + 0xD bytes(s)
0x0000000077B0372D (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 9.0.1
Target: i386-pc-windows-msvc
Thread model: posix
InstalledDir: G:\LLVM\bin
clang-cl: note: diagnostic msg: PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
clang-cl: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\User\AppData\Local\Temp\zip_view-b8f023.cpp
clang-cl: note: diagnostic msg: C:\Users\User\AppData\Local\Temp\zip_view-b8f023.sh
clang-cl: note: diagnostic msg:

********************

## Comments

### Comment 1 - vedantk

1.	libs\mpl\test\zip_view.cpp:38:5 <Spelling=.\boost/mpl/assert.hpp:264:32>: current parser token ','
2.	libs\mpl\test\zip_view.cpp:32:1: parsing function body 'test31'
3.	libs\mpl\test\zip_view.cpp:32:1: in compound statement ('{}')
 #&#8203;0 0x0000000004b7d114 PrintStackTraceSignalHandler(void*) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x4b7d114)
 #&#8203;1 0x0000000004b7abee llvm::sys::RunSignalHandlers() (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x4b7abee)
 #&#8203;2 0x0000000004b7d52c SignalHandler(int) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x4b7d52c)
 #&#8203;3 0x00007f4dca7c4890 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12890)
 #&#8203;4 0x00007f4dc948ae97 raise /build/glibc-OTsEL5/glibc-2.27/signal/../sysdeps/unix/sysv/linux/raise.c:51:0
 #&#8203;5 0x00007f4dc948c801 abort /build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c:81:0
 #&#8203;6 0x0000000004b05f23 llvm::report_bad_alloc_error(char const*, bool) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x4b05f23)
 #&#8203;7 0x0000000002d5b76f llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::StartNewSlab() (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x2d5b76f)
 #&#8203;8 0x0000000002d5b66c llvm::BumpPtrAllocatorImpl<llvm::MallocAllocator, 4096ul, 4096ul, 128ul>::Allocate(unsigned long, llvm::Align) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x2d5b66c)
 #&#8203;9 0x0000000006d945a7 clang::ASTContext::CreateTypeSourceInfo(clang::QualType, unsigned int) const (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x6d945a7)
#&#8203;10 0x00000000068d4d40 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformType(clang::TypeSourceInfo*) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68d4d40)
#&#8203;11 0x00000000068dcd44 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68dcd44)
#&#8203;12 0x00000000068de0e7 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68de0e7)
#&#8203;13 0x00000000068d6727 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68d6727)
#&#8203;14 0x00000000068d4ced clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformType(clang::TypeSourceInfo*) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68d4ced)
#&#8203;15 0x00000000068cd184 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformCStyleCastExpr(clang::CStyleCastExpr*) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68cd184)
#&#8203;16 0x00000000068d2b1b clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformParenExpr(clang::ParenExpr*) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68d2b1b)
#&#8203;17 0x00000000068c3dbd (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x68c3dbd)
#&#8203;18 0x000000000689dd56 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x689dd56)
#&#8203;19 0x00000000067ff6a7 clang::Sema::ActOnConstantExpression(clang::ActionResult<clang::Expr*, true>) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x67ff6a7)
#&#8203;20 0x00000000063df7f6 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x63df7f6)
#&#8203;21 0x000000000639413d clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/home/vsk/src/builds/llvm-project-master-RA/bin/clang-cl+0x639413d)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nikita Kniazev (Kojoley)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [45145](https://llvm.org/bz45145) |
| Version | trunk |
| OS | Windows NT |
| Attachments | [reproducer and console output](https://user-images.githubusercontent.com/92601389/143760463-4fc40b38-2731-44b1-83a5-8ae856504ec8.gz) |
| CC | @<!-- -->dwblaikie,@<!-- -->zygoloid,@<!-- -->vedantk |

## Extended Description 


LLVM ERROR: out of memory
Stack dump:
0.      Program arguments: G:\LLVM\bin\clang-cl.exe -cc1 -triple i386-pc-windows-msvc19.24.28316 -emit-obj -mrelax-all -mincremental-linker-compatible -disable-free -disable-llvm-verifier -discard-val
ue-names -main-file-name zip_view.cpp -mrelocation-model static -mthread-model posix -mdisable-fp-elim -relaxed-aliasing -fmath-errno -masm-verbose -mconstructor-aliases -target-cpu pentium4 -mllvm -x
86-asm-syntax=intel -D_DEBUG -D_MT -D_DLL --dependent-lib=msvcrtd --dependent-lib=oldnames -stack-protector 2 -fcxx-exceptions -fexceptions -fms-volatile -fdiagnostics-format msvc -gcodeview -debug-in
fo-kind=limited -coverage-notes-file C:\Working\Repositories\boost\zip_view.gcno -resource-dir G:\LLVM\lib\clang\9.0.1 -D BOOST_ALL_NO_LIB=1 -I . -internal-isystem G:\LLVM\lib\clang\9.0.1\include -int
ernal-isystem C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\include -internal-isystem C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt -intern
al-isystem C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\shared -internal-isystem C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\um -internal-isystem C:\Program Files (x86)\
Windows Kits\10\include\10.0.17763.0\winrt -O0 -Wall -fdeprecated-macro -fdebug-compilation-dir C:\Working\Repositories\boost -ferror-limit 19 -fmessage-length 0 -fno-use-cxa-atexit -fms-extensions -f
ms-compatibility -fms-compatibility-version=19.24.28316 -std=c++14 -fdelayed-template-parsing -fno-inline -fobjc-runtime=gcc -fdiagnostics-show-option -faddrsig -o bin.v2\libs\mpl\test\zip_view.test\c
lang-win-9.0.1\debug\threading-multi\zip_view.obj -x c++ libs\mpl\test\zip_view.cpp
1.      libs\mpl\test\zip_view.cpp:38:5 &lt;Spelling=.\boost/mpl/assert.hpp:264:32&gt;: current parser token ','
2.      libs\mpl\test\zip_view.cpp:32:1: parsing function body 'test31'
3.      libs\mpl\test\zip_view.cpp:32:1: in compound statement ('{}')
0x0000000140FC90C6 (0x0000000000000016 0x000007FEE0DF8741 0x0000001B00000000 0x000000014246FE62)
0x000007FEE0E4A9FF (0x0000000000000001 0x0000000000000000 0x0000000000000014 0x0000000000A39AD0), raise() + 0x1CF bytes(s)
0x000007FEE0E4B6F1 (0x0000000000000003 0x0000000000000003 0x0000000000A39AD0 0x0000000142D5DCE1), abort() + 0x31 bytes(s)
0x0000000140FBCB8D (0x0000000000000014 0x0000000000A39B60 0x0000000142914490 0x000000014291447C)
0x000000013F99158D (0x0000000000000001 0x0000000005C71858 0x0000000005C71850 0x0000000000A3A4C8)
0x00000001428CF668 (0x0000000000A39F28 0x00000001424DFF4C 0x0000000000000030 0x0000EBD203F62070)
0x000000014247B100 (0x0000000002820960 0x0000000000000000 0x0000000000000000 0x0000000000019000)
0x0000000142482F99 (0x0000000000000000 0x0000000000A3AB00 0x0000000000000015 0x0000000002820080)
0x0000000142482206 (0x0000000000A3A4F0 0x00000001429A8652 0x0000000005678800 0x0000000004E93718)
0x000000014247C9F3 (0x0000000005C73090 0x0000000000000000 0x0000000004FE27F8 0x00000001423DB077)
0x000000014247B0C1 (0x0000000280A88153 0x0000000000BC1DB3 0x0000000000000007 0x0000000180A88155)
0x0000000142482F99 (0x0000000000030000 0x0000000880A83EB7 0x0000000005E51210 0x0000000000020005)
0x0000000142482206 (0x0000000000000000 0x0000000000AD0000 0x0000000009250190 0x0000000000000000)
0x000000014247C9F3 (0x0000000000B68BA0 0x0000000100000000 0x0000000000000000 0x0000000142B9B0CE)
0x000000014247B0C1 (0x000000007FFFFFFF 0x0000000002E601B8 0x0000000000000000 0x0000000000000000)
0x0000000142482F99 (0x0000000000000000 0x0000000000000000 0x0000EBD203F614B0 0x0000000000A3B200)
0x0000000142482206 (0x0000000005C70700 0x0000000000A3B750 0x0000000002E601B8 0x0000000000BC6E90)
0x000000014247C9F3 (0x0000000080A88188 0x0000000000000000 0x0000000002E5FEA0 0x0000000000000000)
0x000000014247B0C1 (0x0000000000000000 0x0000000000A3BE00 0x0000000000A3C010 0x0000000000000001)
0x00000001424750AA (0x0000000000A3B9DC 0x000000000000000E 0x0000000005C72DB0 0x0000000142D5A2C3)
0x000000014246FF6A (0x0000000005BB08E0 0x0000000100000000 0x00000000036A5038 0x0000000000000000)
0x00000001424797ED (0x0000000000A3BCF0 0x000002EF00000000 0x0000000077AB3128 0x0000000000000000)
0x0000000142470426 (0x0000000000000000 0x0000000000000000 0x0000000000A3BEF0 0x0000000400000000)
0x000000014246FC2C (0x0000000005BB0878 0x0000000000B8E3F8 0x0000000000A3C040 0x0000000000B67CB0)
0x00000001424517F5 (0x0000000000000000 0x0000EBD203F67BE0 0x0000000000000000 0x0000000000000000)
0x00000001424EAFD5 (0x0000000000000000 0x00000001424DFC0F 0x0000000000000008 0x0000000002820080)
0x0000000142164755 (0x0000000000B85DB0 0x0000000000BC1CD5 0x0000000000000007 0x00000000036599F0)
0x00000001421008C7 (0x0000000000BCE870 0x0000000142B597D0 0x0000000000BC1CD5 0x0000000000A3C510)
0x000000014211BE30 (0x0000000005BB0610 0x0000000005BB0610 0x0000000000000000 0x0000000000000000)
0x0000000142151328 (0x0000000005BB0580 0x0000000005BB0580 0x0000000000000000 0x0000000000000000)
0x0000000142150E4C (0x0000000000B67CB0 0x0000000000A3C7A8 0x00000000000564AF 0x00000001411611CE)
0x0000000142159C69 (0x0000000005C706D8 0x000000014236EB76 0x000000000519E090 0x0000EBD280A88050)
0x000000014215A906 (0x0000000000B67CB0 0x0000000141161907 0x000000007FFFFFFF 0x0000000000A3CBC8)
0x00000001420DDD08 (0x00000000058C7170 0x0000000100A8468F 0x0000EBD203F67760 0x0000000000B4BC00)
0x0000000142121382 (0x0000000000010000 0x00000000007933C2 0x0000EBD203F66CD0 0x0000000000010000)
0x00000001420DD06B (0x00000000050943F0 0x0000000142B5EAA4 0x0000000000A3D860 0x00000000007913FA)
0x00000001420DC9FC (0x0000000000B89860 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00000001420DB88C (0x0000000000B87C70 0x0000000142D461DE 0x0000000002820080 0x0000000000000000)
0x00000001420D9DC2 (0x0000000000A3DE08 0x0000000000A3DE18 0x0000000000A3DE48 0x00000001415EB70A)
0x00000001420D5BCE (0x000000000000001F 0x0000EBD203F662A0 0x0000000000000000 0x000000000000000F)
0x000000014162AC52 (0x0000000000B50D80 0x0000000142D5A2C3 0x00000000000000C0 0x0000000000A3DE38)
0x00000001415EE64F (0x0000000000B62101 0x0000000000000000 0x0000000000A3ED01 0x0000000000000000)
0x0000000141681A37 (0x0000000000000282 0x000000000000017E 0x0000000000000000 0x0000000000000000)
0x000000013F976F57 (0x0000000000AD0000 0x0061000000000002 0x0000000077AB3128 0x0000000000000000)
0x000000013F974564 (0x0000000000000000 0x0000000000000000 0x01D5F57C4684595C 0x01D5F57C46847878)
0x0000000142D5A4F0 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00000000778A556D (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), BaseThreadInitThunk() + 0xD bytes(s)
0x0000000077B0372D (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 9.0.1
Target: i386-pc-windows-msvc
Thread model: posix
InstalledDir: G:\LLVM\bin
clang-cl: note: diagnostic msg: PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
clang-cl: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\User\AppData\Local\Temp\zip_view-b8f023.cpp
clang-cl: note: diagnostic msg: C:\Users\User\AppData\Local\Temp\zip_view-b8f023.sh
clang-cl: note: diagnostic msg:

********************
</details>


---

