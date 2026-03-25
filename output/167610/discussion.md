# JIT is producing anomalous code when dividing complex numbers on macOS arm

**Author:** cianciosa
**URL:** https://github.com/llvm/llvm-project/issues/167610
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, orcjit, platform:macos
**Closed Date:** 2026-03-13T14:39:55Z

## Body

I'm trying to JIT C++ code in memory running into problems with complex types. The following example trys to JIT and run a function to divide two complex numbers.

```c++
#include "llvm/Support/VirtualFileSystem.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

int main(int argc, const char * argv[]) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    const std::string source(
        "#include <complex>\n"
        "extern \"C\" void test(const std::complex<float> &v0,\n"
        "                       const std::complex<float> &v1,\n"
        "                       std::complex<float> &o2) {\n"
        "    o2 = v0/v1;\n"
        "}"
    );

    clang::DiagnosticOptions diagnostic_options;
    auto diagnostic_printer = std::make_unique<clang::TextDiagnosticPrinter> (llvm::errs(),
                                                                              diagnostic_options);

    auto diagnostic_ids = llvm::makeIntrusiveRefCnt<clang::DiagnosticIDs> ();
    clang::DiagnosticsEngine diagnostic_engine(diagnostic_ids,
                                               diagnostic_options,
                                               diagnostic_printer.release());

    const char *filename = "hello.cpp";
    std::vector<const char *> args = {
        "-O3",
        "-ffast-math",
        filename,
        "-I/usr/local/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/SubFrameworks",
        "-fgnuc-version=4.2.1",
        "-std=gnu++2a"
    };

    auto invocation = std::make_shared<clang::CompilerInvocation> ();
    clang::CompilerInvocation::CreateFromArgs(*(invocation.get()), args,
                                              diagnostic_engine);

    llvm::StringRef source_code_data(source);
    auto buffer = llvm::MemoryBuffer::getMemBuffer(source_code_data);
    invocation->getPreprocessorOpts().addRemappedFile(filename,
                                                      buffer.release());

    clang::CompilerInstance clang(invocation);
    clang.createDiagnostics();

    clang::TargetOptions target_options;
    target_options.Triple = llvm::sys::getProcessTriple();
    auto *target_info = clang::TargetInfo::CreateTargetInfo(diagnostic_engine,
                                                            target_options);
    clang.setTarget(target_info);

    clang::EmitLLVMAction compilerAction;
    clang.ExecuteAction(compilerAction);

    auto ir_module = compilerAction.takeModule();
    auto ctx = std::unique_ptr<llvm::LLVMContext> (compilerAction.takeLLVMContext());

    auto jit_try = llvm::orc::LLJITBuilder().create();
    if (auto jiterror = jit_try.takeError()) {
        std::cerr << "Failed to build JIT : " << toString(std::move(jiterror)) << std::endl;
        exit(1);
    }
    auto jit = std::move(jit_try.get());

    jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(ir_module),
                                                 llvm::orc::ThreadSafeContext(std::move(ctx))));
    jit->linkStaticLibraryInto(jit->getMainJITDylib(), "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/lib/darwin/libclang_rt.osx.a");

    auto entry = std::move(jit->lookup("test")).get();
    auto kernel = entry.toPtr<void(*)(const std::complex<float> &, const std::complex<float> &, std::complex<float> &)> ();

    const std::complex<float> a(2,0);
    const std::complex<float> b(3,0);
    std::complex<float> c;
    std::cout << a << b << std::endl;
    kernel(a, b, c);
    std::cout << a << b << c << std::endl;
}
```
This should produce the expected output of 
```
(2,0)(3,0)
(2,0)(3,0)(0.666667,0)
```
Instead it Is giving me errant results.
```
(2,0)(3,0)
(2,0)(0,0)(0.666667,0.666667)
```

To make things worse if I change from `std::complex<float>` to `std::complex<double>`, `jit->lookup("test")` appears to go into an infinite loop. 

Here is the IR that clang produced. 

```llvm
; ModuleID = 'hello.cpp'
source_filename = "hello.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.6.0"

; Function Attrs: mustprogress nounwind
define void @test(ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v0, ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v1, ptr noundef nonnull writeonly align 4 captures(none) dereferenceable(8) initializes((0, 8)) %o2) loca$
entry:
  %0 = load float, ptr %v0, align 4, !tbaa !6
  %__im_.i.i = getelementptr inbounds nuw i8, ptr %v0, i64 4
  %1 = load float, ptr %__im_.i.i, align 4, !tbaa !9
  %2 = load float, ptr %v1, align 4, !tbaa !6
  %__im_.i6.i = getelementptr inbounds nuw i8, ptr %v1, i64 4
  %3 = load float, ptr %__im_.i6.i, align 4, !tbaa !9
  %call2.i = tail call reassoc nnan ninf nsz arcp afn noundef nofpclass(nan inf) { float, float } @__divsc3(float noundef nofpclass(nan inf) %0, float noundef nofpclass(nan inf) %1, float noundef nofpclass(nan inf) %2, float noundef nofpclass(nan inf) %3) #1
  %4 = extractvalue { float, float } %call2.i, 0
  %5 = extractvalue { float, float } %call2.i, 1
  store float %4, ptr %o2, align 4, !tbaa !10
  %ref.tmp.sroa.4.0..sroa_idx = getelementptr inbounds nuw i8, ptr %o2, i64 4
  store float %5, ptr %ref.tmp.sroa.4.0..sroa_idx, align 4, !tbaa !10
  ret void
}

declare { float, float } @__divsc3(float, float, float, float) local_unnamed_addr

attributes #0 = { mustprogress nounwind "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}
!llvm.errno.tbaa = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project.git ce175995533c7407a00d72633f867bf93d4d4499)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
!6 = !{!7, !8, i64 0}
!7 = !{!"_ZTSNSt3__17complexIfEE", !8, i64 0, !8, i64 4}
!8 = !{!"float", !4, i64 0}
!9 = !{!7, !8, i64 4}
!10 = !{!8, !8, i64 0}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Mark Cianciosa (cianciosa)

<details>
I'm trying to JIT C++ code in memory running into problems with complex types. The following example trys to JIT and run a function to divide two complex numbers.

```
#include "llvm/Support/VirtualFileSystem.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

int main(int argc, const char * argv[]) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    const std::string source(
        "#include &lt;complex&gt;\n"
        "extern \"C\" void test(const std::complex&lt;float&gt; &amp;v0,\n"
        "                       const std::complex&lt;float&gt; &amp;v1,\n"
        "                       std::complex&lt;float&gt; &amp;o2) {\n"
        "    o2 = v0/v1;\n"
        "}"
    );

    clang::DiagnosticOptions diagnostic_options;
    auto diagnostic_printer = std::make_unique&lt;clang::TextDiagnosticPrinter&gt; (llvm::errs(),
                                                                              diagnostic_options);

    auto diagnostic_ids = llvm::makeIntrusiveRefCnt&lt;clang::DiagnosticIDs&gt; ();
    clang::DiagnosticsEngine diagnostic_engine(diagnostic_ids,
                                               diagnostic_options,
                                               diagnostic_printer.release());

    const char *filename = "hello.cpp";
    std::vector&lt;const char *&gt; args = {
        "-O3",
        "-ffast-math",
        filename,
        "-I/usr/local/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/SubFrameworks",
        "-fgnuc-version=4.2.1",
        "-std=gnu++2a"
    };

    auto invocation = std::make_shared&lt;clang::CompilerInvocation&gt; ();
    clang::CompilerInvocation::CreateFromArgs(*(invocation.get()), args,
                                              diagnostic_engine);

    llvm::StringRef source_code_data(source);
    auto buffer = llvm::MemoryBuffer::getMemBuffer(source_code_data);
    invocation-&gt;getPreprocessorOpts().addRemappedFile(filename,
                                                      buffer.release());

    clang::CompilerInstance clang(invocation);
    clang.createDiagnostics();

    clang::TargetOptions target_options;
    target_options.Triple = llvm::sys::getProcessTriple();
    auto *target_info = clang::TargetInfo::CreateTargetInfo(diagnostic_engine,
                                                            target_options);
    clang.setTarget(target_info);

    clang::EmitLLVMAction compilerAction;
    clang.ExecuteAction(compilerAction);

    auto ir_module = compilerAction.takeModule();
    auto ctx = std::unique_ptr&lt;llvm::LLVMContext&gt; (compilerAction.takeLLVMContext());

    auto jit_try = llvm::orc::LLJITBuilder().create();
    if (auto jiterror = jit_try.takeError()) {
        std::cerr &lt;&lt; "Failed to build JIT : " &lt;&lt; toString(std::move(jiterror)) &lt;&lt; std::endl;
        exit(1);
    }
    auto jit = std::move(jit_try.get());

    jit-&gt;addIRModule(llvm::orc::ThreadSafeModule(std::move(ir_module),
                                                 llvm::orc::ThreadSafeContext(std::move(ctx))));
    jit-&gt;linkStaticLibraryInto(jit-&gt;getMainJITDylib(), "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/lib/darwin/libclang_rt.osx.a");

    auto entry = std::move(jit-&gt;lookup("test")).get();
    auto kernel = entry.toPtr&lt;void(*)(const std::complex&lt;float&gt; &amp;, const std::complex&lt;float&gt; &amp;, std::complex&lt;float&gt; &amp;)&gt; ();

    const std::complex&lt;float&gt; a(2,0);
    const std::complex&lt;float&gt; b(3,0);
    std::complex&lt;float&gt; c;
    std::cout &lt;&lt; a &lt;&lt; b &lt;&lt; std::endl;
    kernel(a, b, c);
    std::cout &lt;&lt; a &lt;&lt; b &lt;&lt; c &lt;&lt; std::endl;
}
```
This should produce the expected output of 
```
(2,0)(3,0)
(2,0)(3,0)(0.666667,0)
```
Instead it Is giving me errant results.
```
(2,0)(3,0)
(2,0)(0,0)(0.666667,0.666667)
```

To make things worse if I change from `std::complex&lt;float&gt;` to `std::complex&lt;double&gt;`, `jit-&gt;lookup("test")` appears to go into an infinite loop. 

Here is the IR that clang produced. 

```
; ModuleID = 'hello.cpp'
source_filename = "hello.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.6.0"

; Function Attrs: mustprogress nounwind
define void @<!-- -->test(ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v0, ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v1, ptr noundef nonnull writeonly align 4 captures(none) dereferenceable(8) initializes((0, 8)) %o2) loca$
entry:
  %0 = load float, ptr %v0, align 4, !tbaa !6
  %__im_.i.i = getelementptr inbounds nuw i8, ptr %v0, i64 4
  %1 = load float, ptr %__im_.i.i, align 4, !tbaa !9
  %2 = load float, ptr %v1, align 4, !tbaa !6
  %__im_.i6.i = getelementptr inbounds nuw i8, ptr %v1, i64 4
  %3 = load float, ptr %__im_.i6.i, align 4, !tbaa !9
  %call2.i = tail call reassoc nnan ninf nsz arcp afn noundef nofpclass(nan inf) { float, float } @<!-- -->__divsc3(float noundef nofpclass(nan inf) %0, float noundef nofpclass(nan inf) %1, float noundef nofpclass(nan inf) %2, float noundef nofpclass(nan inf) %3) #<!-- -->1
  %4 = extractvalue { float, float } %call2.i, 0
  %5 = extractvalue { float, float } %call2.i, 1
  store float %4, ptr %o2, align 4, !tbaa !10
  %ref.tmp.sroa.4.0..sroa_idx = getelementptr inbounds nuw i8, ptr %o2, i64 4
  store float %5, ptr %ref.tmp.sroa.4.0..sroa_idx, align 4, !tbaa !10
  ret void
}

declare { float, float } @<!-- -->__divsc3(float, float, float, float) local_unnamed_addr

attributes #<!-- -->0 = { mustprogress nounwind "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #<!-- -->1 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}
!llvm.errno.tbaa = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project.git ce175995533c7407a00d72633f867bf93d4d4499)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
!6 = !{!7, !8, i64 0}
!7 = !{!"_ZTSNSt3__17complexIfEE", !8, i64 0, !8, i64 4}
!8 = !{!"float", !4, i64 0}
!9 = !{!7, !8, i64 4}
!10 = !{!8, !8, i64 0}
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-orcjit

Author: Mark Cianciosa (cianciosa)

<details>
I'm trying to JIT C++ code in memory running into problems with complex types. The following example trys to JIT and run a function to divide two complex numbers.

```
#include "llvm/Support/VirtualFileSystem.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

int main(int argc, const char * argv[]) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    const std::string source(
        "#include &lt;complex&gt;\n"
        "extern \"C\" void test(const std::complex&lt;float&gt; &amp;v0,\n"
        "                       const std::complex&lt;float&gt; &amp;v1,\n"
        "                       std::complex&lt;float&gt; &amp;o2) {\n"
        "    o2 = v0/v1;\n"
        "}"
    );

    clang::DiagnosticOptions diagnostic_options;
    auto diagnostic_printer = std::make_unique&lt;clang::TextDiagnosticPrinter&gt; (llvm::errs(),
                                                                              diagnostic_options);

    auto diagnostic_ids = llvm::makeIntrusiveRefCnt&lt;clang::DiagnosticIDs&gt; ();
    clang::DiagnosticsEngine diagnostic_engine(diagnostic_ids,
                                               diagnostic_options,
                                               diagnostic_printer.release());

    const char *filename = "hello.cpp";
    std::vector&lt;const char *&gt; args = {
        "-O3",
        "-ffast-math",
        filename,
        "-I/usr/local/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks",
        "-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/SubFrameworks",
        "-fgnuc-version=4.2.1",
        "-std=gnu++2a"
    };

    auto invocation = std::make_shared&lt;clang::CompilerInvocation&gt; ();
    clang::CompilerInvocation::CreateFromArgs(*(invocation.get()), args,
                                              diagnostic_engine);

    llvm::StringRef source_code_data(source);
    auto buffer = llvm::MemoryBuffer::getMemBuffer(source_code_data);
    invocation-&gt;getPreprocessorOpts().addRemappedFile(filename,
                                                      buffer.release());

    clang::CompilerInstance clang(invocation);
    clang.createDiagnostics();

    clang::TargetOptions target_options;
    target_options.Triple = llvm::sys::getProcessTriple();
    auto *target_info = clang::TargetInfo::CreateTargetInfo(diagnostic_engine,
                                                            target_options);
    clang.setTarget(target_info);

    clang::EmitLLVMAction compilerAction;
    clang.ExecuteAction(compilerAction);

    auto ir_module = compilerAction.takeModule();
    auto ctx = std::unique_ptr&lt;llvm::LLVMContext&gt; (compilerAction.takeLLVMContext());

    auto jit_try = llvm::orc::LLJITBuilder().create();
    if (auto jiterror = jit_try.takeError()) {
        std::cerr &lt;&lt; "Failed to build JIT : " &lt;&lt; toString(std::move(jiterror)) &lt;&lt; std::endl;
        exit(1);
    }
    auto jit = std::move(jit_try.get());

    jit-&gt;addIRModule(llvm::orc::ThreadSafeModule(std::move(ir_module),
                                                 llvm::orc::ThreadSafeContext(std::move(ctx))));
    jit-&gt;linkStaticLibraryInto(jit-&gt;getMainJITDylib(), "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/17/lib/darwin/libclang_rt.osx.a");

    auto entry = std::move(jit-&gt;lookup("test")).get();
    auto kernel = entry.toPtr&lt;void(*)(const std::complex&lt;float&gt; &amp;, const std::complex&lt;float&gt; &amp;, std::complex&lt;float&gt; &amp;)&gt; ();

    const std::complex&lt;float&gt; a(2,0);
    const std::complex&lt;float&gt; b(3,0);
    std::complex&lt;float&gt; c;
    std::cout &lt;&lt; a &lt;&lt; b &lt;&lt; std::endl;
    kernel(a, b, c);
    std::cout &lt;&lt; a &lt;&lt; b &lt;&lt; c &lt;&lt; std::endl;
}
```
This should produce the expected output of 
```
(2,0)(3,0)
(2,0)(3,0)(0.666667,0)
```
Instead it Is giving me errant results.
```
(2,0)(3,0)
(2,0)(0,0)(0.666667,0.666667)
```

To make things worse if I change from `std::complex&lt;float&gt;` to `std::complex&lt;double&gt;`, `jit-&gt;lookup("test")` appears to go into an infinite loop. 

Here is the IR that clang produced. 

```
; ModuleID = 'hello.cpp'
source_filename = "hello.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.6.0"

; Function Attrs: mustprogress nounwind
define void @<!-- -->test(ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v0, ptr noundef nonnull readonly align 4 captures(none) dereferenceable(8) %v1, ptr noundef nonnull writeonly align 4 captures(none) dereferenceable(8) initializes((0, 8)) %o2) loca$
entry:
  %0 = load float, ptr %v0, align 4, !tbaa !6
  %__im_.i.i = getelementptr inbounds nuw i8, ptr %v0, i64 4
  %1 = load float, ptr %__im_.i.i, align 4, !tbaa !9
  %2 = load float, ptr %v1, align 4, !tbaa !6
  %__im_.i6.i = getelementptr inbounds nuw i8, ptr %v1, i64 4
  %3 = load float, ptr %__im_.i6.i, align 4, !tbaa !9
  %call2.i = tail call reassoc nnan ninf nsz arcp afn noundef nofpclass(nan inf) { float, float } @<!-- -->__divsc3(float noundef nofpclass(nan inf) %0, float noundef nofpclass(nan inf) %1, float noundef nofpclass(nan inf) %2, float noundef nofpclass(nan inf) %3) #<!-- -->1
  %4 = extractvalue { float, float } %call2.i, 0
  %5 = extractvalue { float, float } %call2.i, 1
  store float %4, ptr %o2, align 4, !tbaa !10
  %ref.tmp.sroa.4.0..sroa_idx = getelementptr inbounds nuw i8, ptr %o2, i64 4
  store float %5, ptr %ref.tmp.sroa.4.0..sroa_idx, align 4, !tbaa !10
  ret void
}

declare { float, float } @<!-- -->__divsc3(float, float, float, float) local_unnamed_addr

attributes #<!-- -->0 = { mustprogress nounwind "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #<!-- -->1 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}
!llvm.errno.tbaa = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project.git ce175995533c7407a00d72633f867bf93d4d4499)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
!6 = !{!7, !8, i64 0}
!7 = !{!"_ZTSNSt3__17complexIfEE", !8, i64 0, !8, i64 4}
!8 = !{!"float", !4, i64 0}
!9 = !{!7, !8, i64 4}
!10 = !{!8, !8, i64 0}
```
</details>


---

### Comment 3 - cianciosa

This issue was fixed back in November but note sure what commit fixed it.

---

