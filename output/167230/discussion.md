# [OpenMP][offload] Assertion `Arg < getNumArgs() && "Arg access out of range!"' failed

**Author:** ye-luo
**URL:** https://github.com/llvm/llvm-project/issues/167230
**Status:** Closed
**Labels:** clang:frontend, crash, constexpr
**Closed Date:** 2025-11-12T17:45:51Z

## Body

Failure caused by
cc9ad9afc6d22f9955a35aa8f62f2bbf11109673
[Clang] Add constexpr support for AVX512 permutex2 intrinsics (#165085)

reproducer main.cpp
```
#include <xmmintrin.h>
```
compile offload
```
$ clang++ -c -v -fopenmp --offload-arch=sm_86 main.cpp
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 64ad5d976dbd9bb140d81440c7d9cb093278dd31)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /scratch3/packages/llvm/main-20251109/bin
Build config: +assertions
Configuration file: /scratch3/packages/llvm/main-20251109/bin/x86_64-unknown-linux-gnu.cfg
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
Found CUDA installation: /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o, version 12.3
 "/scratch3/packages/llvm/main-20251109/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -fcoverage-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -disable-llvm-passes --offload-targets=nvptx64-nvidia-cuda -faddrsig -fdwarf2-cfi-asm -o /tmp/main-8fffff.bc -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward"
ignoring duplicate directory "/scratch3/packages/llvm/main-20251109/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include/x86_64-linux-gnu"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include
End of search list.
 "/scratch3/packages/llvm/main-20251109/bin/clang-22" -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.3 -mlink-builtin-bitcode /scratch3/packages/llvm/main-20251109/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_86 -target-feature +ptx83 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-8fffff.bc -fdwarf2-cfi-asm -o /tmp/main-sm_86-11e097.s -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward"
ignoring duplicate directory "/scratch3/packages/llvm/main-20251109/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include/x86_64-linux-gnu"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include
End of search list.
clang-22: /scratch3/opt/llvm-clang/llvm-project-nightly/clang/include/clang/AST/Expr.h:3086: const clang::Expr* clang::CallExpr::getArg(unsigned int) const: Assertion `Arg < getNumArgs() && "Arg access out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /scratch3/packages/llvm/main-20251109/bin/clang-22 -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.3 -mlink-builtin-bitcode /scratch3/packages/llvm/main-20251109/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_86 -target-feature +ptx83 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-8fffff.bc -fdwarf2-cfi-asm -o /tmp/main-sm_86-11e097.s -x c++ main.cpp
1.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2521:5: current parser token '__n128'
2.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2508:1: parsing function body '_mm_maskmove_si64'
3.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2508:1: in compound statement ('{}')
4.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2516:47: in compound statement ('{}')
 #0 0x000055bd86a779f0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x2e049f0)
 #1 0x000055bd86a746af llvm::sys::RunSignalHandlers() (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x2e016af)
 #2 0x000055bd86a74802 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f183d034520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f183d0889fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f183d0889fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f183d0889fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f183d034476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f183d01a7f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f183d01a71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f183d02be96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055bd8a202825 evalShuffleGeneric((anonymous namespace)::EvalInfo&, clang::CallExpr const*, clang::APValue&, llvm::function_ref<std::pair<unsigned int, int> (unsigned int, unsigned int)>) ExprConstant.cpp:0:0
#12 0x000055bd8a23324f (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#13 0x000055bd8a22fbe9 EvaluateVector(clang::Expr const*, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x000055bd8a1fd2cd Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x000055bd8a22c056 (anonymous namespace)::VectorExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#16 0x000055bd8a22fbe9 EvaluateVector(clang::Expr const*, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#17 0x000055bd8a1fd2cd Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#18 0x000055bd8a1ea7f8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#19 0x000055bd8a1edef8 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#20 0x000055bd8a1fd245 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#21 0x000055bd8a1fde46 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#22 0x000055bd8a2017fb clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x658e7fb)
#23 0x000055bd89123a3e clang::Sema::CheckForIntOverflow(clang::Expr const*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x54b0a3e)
#24 0x000055bd8915ae54 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x54e7e54)
#25 0x000055bd8951cf8e clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x58a9f8e)
#26 0x000055bd8984ff5e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5bdcf5e)
#27 0x000055bd88fee884 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537b884)
#28 0x000055bd88fe6ea0 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5373ea0)
#29 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#30 0x000055bd88fef04f clang::Parser::ParseCompoundStatementBody(bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c04f)
#31 0x000055bd88fef7f8 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#32 0x000055bd8a8a26d5 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x6c2f6d5)
#33 0x000055bd88fdffeb clang::Parser::ParseCompoundStatement(bool, unsigned int) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x536cfeb)
#34 0x000055bd88fe5c77 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5372c77)
#35 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#36 0x000055bd88fe8afb clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5375afb)
#37 0x000055bd88fe90f1 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53760f1)
#38 0x000055bd88fe691c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537391c)
#39 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#40 0x000055bd88fef04f clang::Parser::ParseCompoundStatementBody(bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c04f)
#41 0x000055bd88fef8cf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c8cf)
#42 0x000055bd88ee9a6f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5276a6f)
#43 0x000055bd88f2e1cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x52bb1cd)
#44 0x000055bd88ee3e8c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5270e8c)
#45 0x000055bd88ee45ef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x52715ef)
#46 0x000055bd88eee166 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x527b166)
#47 0x000055bd88eef1d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x527c1d5)
#48 0x000055bd88ecbfba clang::ParseAST(clang::Sema&, bool, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5258fba)
#49 0x000055bd876e0e09 clang::FrontendAction::Execute() (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x3a6de09)
#50 0x000055bd87660945 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x39ed945)
#51 0x000055bd877d6343 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x3b63343)
#52 0x000055bd84ac3c77 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe50c77)
#53 0x000055bd84aba36a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#54 0x000055bd84abe668 clang_main(int, char**, llvm::ToolContext const&) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe4b668)
#55 0x000055bd849c9adb main (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xd56adb)
#56 0x00007f183d01bd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#57 0x00007f183d01be40 call_init ./csu/../csu/libc-start.c:128:20
#58 0x00007f183d01be40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#59 0x000055bd84ab9a85 _start (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe46a85)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 64ad5d976dbd9bb140d81440c7d9cb093278dd31)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /scratch3/packages/llvm/main-20251109/bin
Build config: +assertions
Configuration file: /scratch3/packages/llvm/main-20251109/bin/x86_64-unknown-linux-gnu.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-sm_86-3749d5.cpp
clang++: note: diagnostic msg: /tmp/main-054f43.cpp
clang++: note: diagnostic msg: /tmp/main-sm_86-3749d5.sh
clang++: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ye Luo (ye-luo)

<details>
Failure caused by
cc9ad9afc6d22f9955a35aa8f62f2bbf11109673
[Clang] Add constexpr support for AVX512 permutex2 intrinsics (#<!-- -->165085)

reproducer main.cpp
```
#include &lt;xmmintrin.h&gt;
```
compile offload
```
$ clang++ -c -v -fopenmp --offload-arch=sm_86 main.cpp
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 64ad5d976dbd9bb140d81440c7d9cb093278dd31)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /scratch3/packages/llvm/main-20251109/bin
Build config: +assertions
Configuration file: /scratch3/packages/llvm/main-20251109/bin/x86_64-unknown-linux-gnu.cfg
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o, version 12.3
 "/scratch3/packages/llvm/main-20251109/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -fcoverage-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -disable-llvm-passes --offload-targets=nvptx64-nvidia-cuda -faddrsig -fdwarf2-cfi-asm -o /tmp/main-8fffff.bc -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward"
ignoring duplicate directory "/scratch3/packages/llvm/main-20251109/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include/x86_64-linux-gnu"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include
End of search list.
 "/scratch3/packages/llvm/main-20251109/bin/clang-22" -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.3 -mlink-builtin-bitcode /scratch3/packages/llvm/main-20251109/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_86 -target-feature +ptx83 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-8fffff.bc -fdwarf2-cfi-asm -o /tmp/main-sm_86-11e097.s -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11"
ignoring duplicate directory "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward"
ignoring duplicate directory "/scratch3/packages/llvm/main-20251109/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include/x86_64-linux-gnu"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11
 /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers
 /scratch3/packages/llvm/main-20251109/lib/clang/22/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
 /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include
End of search list.
clang-22: /scratch3/opt/llvm-clang/llvm-project-nightly/clang/include/clang/AST/Expr.h:3086: const clang::Expr* clang::CallExpr::getArg(unsigned int) const: Assertion `Arg &lt; getNumArgs() &amp;&amp; "Arg access out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /scratch3/packages/llvm/main-20251109/bin/clang-22 -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.3 -mlink-builtin-bitcode /scratch3/packages/llvm/main-20251109/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_86 -target-feature +ptx83 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_A1200_llvmNightly_cuda12.3_offload_cuda_real/src/Message -v -resource-dir /scratch3/packages/llvm/main-20251109/lib/clang/22 -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -I/nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/intel-mkl-2020.4.304-s7l3k4j5son3vwid2blpmktk7ckuulry/compilers_and_libraries_2020.4.304/linux/mkl/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /nfs/gce/projects/QMCPACK_dev/spack/opt/spack/linux-ubuntu22.04-skylake_avx512/gcc-9.4.0/cuda-12.3.2-cbovxskbgf5vjjxo4xrovedujpfhcc5o/include -internal-isystem /scratch3/packages/llvm/main-20251109/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-8fffff.bc -fdwarf2-cfi-asm -o /tmp/main-sm_86-11e097.s -x c++ main.cpp
1.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2521:5: current parser token '__n128'
2.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2508:1: parsing function body '_mm_maskmove_si64'
3.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2508:1: in compound statement ('{}')
4.	/scratch3/packages/llvm/main-20251109/lib/clang/22/include/xmmintrin.h:2516:47: in compound statement ('{}')
 #<!-- -->0 0x000055bd86a779f0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x2e049f0)
 #<!-- -->1 0x000055bd86a746af llvm::sys::RunSignalHandlers() (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x2e016af)
 #<!-- -->2 0x000055bd86a74802 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f183d034520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f183d0889fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f183d0889fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f183d0889fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f183d034476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f183d01a7f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f183d01a71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f183d02be96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x000055bd8a202825 evalShuffleGeneric((anonymous namespace)::EvalInfo&amp;, clang::CallExpr const*, clang::APValue&amp;, llvm::function_ref&lt;std::pair&lt;unsigned int, int&gt; (unsigned int, unsigned int)&gt;) ExprConstant.cpp:0:0
#<!-- -->12 0x000055bd8a23324f (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x000055bd8a22fbe9 EvaluateVector(clang::Expr const*, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x000055bd8a1fd2cd Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x000055bd8a22c056 (anonymous namespace)::VectorExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x000055bd8a22fbe9 EvaluateVector(clang::Expr const*, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->17 0x000055bd8a1fd2cd Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->18 0x000055bd8a1ea7f8 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->19 0x000055bd8a1edef8 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->20 0x000055bd8a1fd245 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->21 0x000055bd8a1fde46 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->22 0x000055bd8a2017fb clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x658e7fb)
#<!-- -->23 0x000055bd89123a3e clang::Sema::CheckForIntOverflow(clang::Expr const*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x54b0a3e)
#<!-- -->24 0x000055bd8915ae54 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x54e7e54)
#<!-- -->25 0x000055bd8951cf8e clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x58a9f8e)
#<!-- -->26 0x000055bd8984ff5e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5bdcf5e)
#<!-- -->27 0x000055bd88fee884 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537b884)
#<!-- -->28 0x000055bd88fe6ea0 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5373ea0)
#<!-- -->29 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#<!-- -->30 0x000055bd88fef04f clang::Parser::ParseCompoundStatementBody(bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c04f)
#<!-- -->31 0x000055bd88fef7f8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
#<!-- -->32 0x000055bd8a8a26d5 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x6c2f6d5)
#<!-- -->33 0x000055bd88fdffeb clang::Parser::ParseCompoundStatement(bool, unsigned int) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x536cfeb)
#<!-- -->34 0x000055bd88fe5c77 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5372c77)
#<!-- -->35 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#<!-- -->36 0x000055bd88fe8afb clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5375afb)
#<!-- -->37 0x000055bd88fe90f1 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53760f1)
#<!-- -->38 0x000055bd88fe691c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537391c)
#<!-- -->39 0x000055bd88fe77de clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x53747de)
#<!-- -->40 0x000055bd88fef04f clang::Parser::ParseCompoundStatementBody(bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c04f)
#<!-- -->41 0x000055bd88fef8cf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x537c8cf)
#<!-- -->42 0x000055bd88ee9a6f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5276a6f)
#<!-- -->43 0x000055bd88f2e1cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x52bb1cd)
#<!-- -->44 0x000055bd88ee3e8c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5270e8c)
#<!-- -->45 0x000055bd88ee45ef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x52715ef)
#<!-- -->46 0x000055bd88eee166 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x527b166)
#<!-- -->47 0x000055bd88eef1d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x527c1d5)
#<!-- -->48 0x000055bd88ecbfba clang::ParseAST(clang::Sema&amp;, bool, bool) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x5258fba)
#<!-- -->49 0x000055bd876e0e09 clang::FrontendAction::Execute() (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x3a6de09)
#<!-- -->50 0x000055bd87660945 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x39ed945)
#<!-- -->51 0x000055bd877d6343 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0x3b63343)
#<!-- -->52 0x000055bd84ac3c77 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe50c77)
#<!-- -->53 0x000055bd84aba36a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->54 0x000055bd84abe668 clang_main(int, char**, llvm::ToolContext const&amp;) (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe4b668)
#<!-- -->55 0x000055bd849c9adb main (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xd56adb)
#<!-- -->56 0x00007f183d01bd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->57 0x00007f183d01be40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x00007f183d01be40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->59 0x000055bd84ab9a85 _start (/scratch3/packages/llvm/main-20251109/bin/clang-22+0xe46a85)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 64ad5d976dbd9bb140d81440c7d9cb093278dd31)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /scratch3/packages/llvm/main-20251109/bin
Build config: +assertions
Configuration file: /scratch3/packages/llvm/main-20251109/bin/x86_64-unknown-linux-gnu.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-sm_86-3749d5.cpp
clang++: note: diagnostic msg: /tmp/main-054f43.cpp
clang++: note: diagnostic msg: /tmp/main-sm_86-3749d5.sh
clang++: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - EugeneZelenko

@RKSimon 

---

### Comment 3 - tbaederr

I can't reproduce that locally.

---

### Comment 4 - ye-luo

My llvm recipe
```
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_FOLDER \
    -DLLVM_ENABLE_BACKTRACES=ON \
    -DLLVM_ENABLE_WERROR=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_ENABLE_RTTI=ON \
    -DLLVM_TARGETS_TO_BUILD="X86;AMDGPU;NVPTX" \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_ENABLE_PROJECTS="clang;openmp" \
    -DLLVM_ENABLE_RUNTIMES="openmp;offload" \
    -DLLVM_RUNTIME_TARGETS='default;amdgcn-amd-amdhsa;nvptx64-nvidia-cuda' \
    -DRUNTIMES_amdgcn-amd-amdhsa_LLVM_ENABLE_RUNTIMES='openmp' \
    -DRUNTIMES_nvptx64-nvidia-cuda_LLVM_ENABLE_RUNTIMES='openmp' \
    -DLIBOMPTARGET_ENABLE_DEBUG=ON \
    ../llvm-project/llvm
```
@tbaederr did you have `LLVM_ENABLE_ASSERTIONS` on?

---

### Comment 5 - RKSimon

CC @chaitanyav


---

### Comment 6 - RKSimon

Still can't repro - that you hit this inside _mm_maskmove_si64 makes me suspicious as nothing in there should touch evalShuffleGeneric

I'm building a scratch compiler to match your cmake to see if I can repro then

---

### Comment 7 - RKSimon

My current guess is that your builtin enum ids are out of sync

---

### Comment 8 - ye-luo

> My current guess is that your builtin enum ids are out of sync

Where are `builtin enum ids` from? My nightly compiler builds always start from an empty build directory and installed to a dated folder.
I can reproduce this issue on two machines one with ubuntu 22.04 and one with SLES SP6.

---

### Comment 9 - RKSimon

The enums from BuiltinsX86.inc etc. - the build has crashed for me and I'm running out of time tonight - I'll try again tomorrow

---

### Comment 10 - chaitanyav

@RKSimon am building from scratch to reproduce this.

---

### Comment 11 - tbaederr

> [@tbaederr](https://github.com/tbaederr) did you have `LLVM_ENABLE_ASSERTIONS` on?

Yes, although I didn't specify the cuda parameter since my local build doesn't include that. Are the cuda and openmp command line parameters necessary for the crash to occur?

---

### Comment 12 - RKSimon

I think so, I also can't easily build with cuda and still haven't found a repro

---

### Comment 13 - chaitanyav

I am able to reproduce this 

```bash
╰─ ../llvm-install/bin/clang++ -c -v -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda main.cpp
clang version 22.0.0git (git@github.com:chaitanyav/llvm-project.git db8c38da44e20d1bce0b57bb548d248f470c623e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/naga/llvm-install/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/15.2.1
Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1
Selected GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Selected multilib: .;@m64
Found CUDA installation: /opt/cuda, version
 "/home/naga/llvm-install/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/naga/llvm-test -v -fcoverage-compilation-dir=/home/naga/llvm-test -resource-dir /home/naga/llvm-install/lib/clang/22 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/cuda/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=180 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -disable-llvm-passes --offload-targets=nvptx64-nvidia-cuda -faddrsig -fdwarf2-cfi-asm -o /tmp/main-70ede9.bc -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward"
ignoring duplicate directory "/home/naga/llvm-install/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward
 /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers
 /home/naga/llvm-install/lib/clang/22/include
 /usr/local/include
 /usr/include
 /opt/cuda/include
End of search list.
 "/home/naga/llvm-install/bin/clang-22" -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /opt/cuda/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.9 -mlink-builtin-bitcode /home/naga/llvm-install/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_120 -target-feature +ptx88 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/home/naga/llvm-test -v -resource-dir /home/naga/llvm-install/lib/clang/22 -internal-isystem /home/naga/llvm-install/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/cuda/include -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=180 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-70ede9.bc -fdwarf2-cfi-asm -o /tmp/main-sm_120-12052a.s -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward"
ignoring duplicate directory "/home/naga/llvm-install/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /home/naga/llvm-install/lib/clang/22/include/openmp_wrappers
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward
 /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers
 /home/naga/llvm-install/lib/clang/22/include
 /usr/local/include
 /usr/include
 /opt/cuda/include
End of search list.
clang-22: /home/naga/llvm-project/clang/include/clang/AST/Expr.h:3086: const Expr *clang::CallExpr::getArg(unsigned int) const: Assertion `Arg < getNumArgs() && "Arg access out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/naga/llvm-install/bin/clang-22 -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /opt/cuda/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.9 -mlink-builtin-bitcode /home/naga/llvm-install/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_120 -target-feature +ptx88 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/home/naga/llvm-test -v -resource-dir /home/naga/llvm-install/lib/clang/22 -internal-isystem /home/naga/llvm-install/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/cuda/include -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=180 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-70ede9.bc -fdwarf2-cfi-asm -o /tmp/main-sm_120-12052a.s -x c++ main.cpp
1.      /home/naga/llvm-install/lib/clang/22/include/xmmintrin.h:2521:5: current parser token '__n128'
2.      /home/naga/llvm-install/lib/clang/22/include/xmmintrin.h:2508:1: parsing function body '_mm_maskmove_si64'
3.      /home/naga/llvm-install/lib/clang/22/include/xmmintrin.h:2508:1: in compound statement ('{}')
4.      /home/naga/llvm-install/lib/clang/22/include/xmmintrin.h:2516:47: in compound statement ('{}')
 #0 0x000071f6d82592f6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/naga/llvm-install/bin/../lib/libLLVMSupport.so.22.0git+0x2592f6)
 #1 0x000071f6d8256705 llvm::sys::RunSignalHandlers() (/home/naga/llvm-install/bin/../lib/libLLVMSupport.so.22.0git+0x256705)
 #2 0x000071f6d825a104 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000071f6d783e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x000071f6d789894c (/usr/lib/libc.so.6+0x9894c)
 #5 0x000071f6d783e410 raise (/usr/lib/libc.so.6+0x3e410)
 #6 0x000071f6d782557a abort (/usr/lib/libc.so.6+0x2557a)
 #7 0x000071f6d78254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #8 0x000071f6d623b2a7 evalShuffleGeneric((anonymous namespace)::EvalInfo&, clang::CallExpr const*, clang::APValue&, llvm::function_ref<std::pair<unsigned int, int> (unsigned int, unsigned int)>) ExprConstant.cpp:0:0
 #9 0x000071f6d622f960 (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#10 0x000071f6d6204552 EvaluateVector(clang::Expr const*, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#11 0x000071f6d61b42c2 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#12 0x000071f6d62389ce (anonymous namespace)::VectorExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#13 0x000071f6d6204552 EvaluateVector(clang::Expr const*, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x000071f6d61b42c2 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x000071f6d61bb44f (anonymous namespace)::LValueExprEvaluator::VisitBinAssign(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#16 0x000071f6d61af472 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#17 0x000071f6d61b427b Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#18 0x000071f6d61b1c46 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#19 0x000071f6d61b1e82 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/home/naga/llvm-install/bin/../lib/../lib/libclangAST.so.22.0git+0x9b1e82)
#20 0x000071f6d4a6abff clang::Sema::CheckForIntOverflow(clang::Expr const*) (/home/naga/llvm-install/bin/../lib/../lib/libclangSema.so.22.0git+0x66abff)
#21 0x000071f6d4a6b5d0 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangSema.so.22.0git+0x66b5d0)
#22 0x000071f6d4dd3f3f clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangSema.so.22.0git+0x9d3f3f)
#23 0x000071f6d50488fa clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangSema.so.22.0git+0xc488fa)
#24 0x000071f6d6bc0a60 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x150a60)
#25 0x000071f6d6bc0006 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x150006)
#26 0x000071f6d6bca7a4 clang::Parser::ParseCompoundStatementBody(bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x15a7a4)
#27 0x000071f6d6bccda3 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::$_0>(long) ParseStmt.cpp:0:0
#28 0x000071f6d9fc093d clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/naga/llvm-install/bin/../lib/libclangBasic.so.22.0git+0x3c093d)
#29 0x000071f6d6bc1a65 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x151a65)
#30 0x000071f6d6bc0006 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x150006)
#31 0x000071f6d6bc4413 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x154413)
#32 0x000071f6d6bc1a84 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x151a84)
#33 0x000071f6d6bc0006 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x150006)
#34 0x000071f6d6bca7a4 clang::Parser::ParseCompoundStatementBody(bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x15a7a4)
#35 0x000071f6d6bcb800 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x15b800)
#36 0x000071f6d6be79db clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x1779db)
#37 0x000071f6d6b04c59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x94c59)
#38 0x000071f6d6be6ba3 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x176ba3)
#39 0x000071f6d6be637b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x17637b)
#40 0x000071f6d6be5288 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x175288)
#41 0x000071f6d6be3513 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x173513)
#42 0x000071f6d6ae8ede clang::ParseAST(clang::Sema&, bool, bool) (/home/naga/llvm-install/bin/../lib/../lib/libclangParse.so.22.0git+0x78ede)
#43 0x000071f6dabe8984 clang::FrontendAction::Execute() (/home/naga/llvm-install/bin/../lib/libclangFrontend.so.22.0git+0x1e8984)
#44 0x000071f6dab43c6d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/naga/llvm-install/bin/../lib/libclangFrontend.so.22.0git+0x143c6d)
#45 0x000071f6de2aacf2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/naga/llvm-install/bin/../lib/libclangFrontendTool.so.22.0git+0x5cf2)
#46 0x00005874597d1813 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/naga/llvm-install/bin/clang-22+0x15813)
#47 0x00005874597cd4c5 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#48 0x00005874597cc4f1 clang_main(int, char**, llvm::ToolContext const&) (/home/naga/llvm-install/bin/clang-22+0x104f1)
#49 0x00005874597dd7d6 main (/home/naga/llvm-install/bin/clang-22+0x217d6)
#50 0x000071f6d7827675 (/usr/lib/libc.so.6+0x27675)
#51 0x000071f6d7827729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#52 0x00005874597cae35 _start (/home/naga/llvm-install/bin/clang-22+0xee35)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (git@github.com:chaitanyav/llvm-project.git db8c38da44e20d1bce0b57bb548d248f470c623e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/naga/llvm-install/bin
Build config: +assertions
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-sm_120-4e1841.cpp
clang++: note: diagnostic msg: /tmp/main-7da997.cpp
clang++: note: diagnostic msg: /tmp/main-sm_120-4e1841.sh
clang++: note: diagnostic msg:
```


using this code snippet 

```cpp
╰─ cat main.cpp
#include <xmmintrin.h>
int main() { return 0; }
```

I am working on the fix now.

---

### Comment 14 - chaitanyav

i don't see the error anymore after syncing with TOT

```
 ../llvm-install/bin/clang++ -c -v -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda main.cpp
clang version 22.0.0git (git@github.com:chaitanyav/llvm-project.git 9fce00469d03d7bf9acbbcd6597206cbbbbbe238)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/naga/llvm-install/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/15.2.1
Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1
Selected GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Selected multilib: .;@m64
Found CUDA installation: /opt/cuda, version
 "/home/naga/llvm-install/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/naga/llvm-test -v -fcoverage-compilation-dir=/home/naga/llvm-test -resource-dir /home/naga/llvm-install/lib/clang/22 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/cuda/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=180 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -disable-llvm-passes --offload-targets=nvptx64-nvidia-cuda -faddrsig -fdwarf2-cfi-asm -o /tmp/main-a2ae13.bc -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward"
ignoring duplicate directory "/home/naga/llvm-install/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward
 /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers
 /home/naga/llvm-install/lib/clang/22/include
 /usr/local/include
 /usr/include
 /opt/cuda/include
End of search list.
 "/home/naga/llvm-install/bin/clang-22" -cc1 -triple nvptx64-nvidia-cuda -aux-triple x86_64-unknown-linux-gnu -S -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -no-integrated-as -fcuda-is-device -mllvm -enable-memcpyopt-without-libcalls -fno-threadsafe-statics -fcuda-allow-variadic-functions -mlink-builtin-bitcode /opt/cuda/nvvm/libdevice/libdevice.10.bc -target-sdk-version=12.9 -mlink-builtin-bitcode /home/naga/llvm-install/bin/../lib/nvptx64-nvidia-cuda/libomptarget-nvptx.bc -target-cpu sm_120 -target-feature +ptx88 -debugger-tuning=gdb -fno-dwarf-directory-asm -fdebug-compilation-dir=/home/naga/llvm-test -v -resource-dir /home/naga/llvm-install/lib/clang/22 -internal-isystem /home/naga/llvm-install/lib/clang/22/include/openmp_wrappers -include __clang_openmp_device_functions.h -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1 -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward -internal-isystem /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/cuda/include -internal-isystem /home/naga/llvm-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=180 -fvisibility=protected -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fopenmp-is-target-device -fopenmp-host-ir-file-path /tmp/main-a2ae13.bc -fdwarf2-cfi-asm -o /tmp/main-sm_120-fd3e37.s -x c++ main.cpp
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring nonexistent directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../x86_64-pc-linux-gnu/include"
ignoring nonexistent directory "/include"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu"
ignoring duplicate directory "/usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward"
ignoring duplicate directory "/home/naga/llvm-install/lib/clang/22/include"
ignoring duplicate directory "/usr/local/include"
ignoring duplicate directory "/usr/include"
#include "..." search starts here:
#include <...> search starts here:
 /home/naga/llvm-install/lib/clang/22/include/openmp_wrappers
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/x86_64-pc-linux-gnu
 /usr/lib64/gcc/x86_64-pc-linux-gnu/15.2.1/../../../../include/c++/15.2.1/backward
 /home/naga/llvm-install/lib/clang/22/include/llvm_libc_wrappers
 /home/naga/llvm-install/lib/clang/22/include
 /usr/local/include
 /usr/include
 /opt/cuda/include
End of search list.
 "/opt/cuda/bin/ptxas" -m64 -O0 -v --gpu-name sm_120 --output-file /tmp/main-sm_120-46cc1f.o /tmp/main-sm_120-fd3e37.s -c
ptxas info    : 20 bytes gmem
 "/home/naga/llvm-install/bin/llvm-offload-binary" -o /tmp/main-33c980.out --image=file=/tmp/main-sm_120-46cc1f.o,triple=nvptx64-nvidia-cuda,arch=sm_120,kind=openmp
 "/home/naga/llvm-install/bin/clang-22" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/naga/llvm-test -v -fcoverage-compilation-dir=/home/naga/llvm-test -resource-dir /home/naga/llvm-install/lib/clang/22 -ferror-limit 19 -fmessage-length=180 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -fembed-offload-object=/tmp/main-33c980.out --offload-targets=nvptx64-nvidia-cuda -faddrsig -fdwarf2-cfi-asm -o main.o -x ir /tmp/main-a2ae13.bc
clang -cc1 version 22.0.0git based upon LLVM 22.0.0git default target x86_64-unknown-linux-gnu
```

looks like [https://github.com/llvm/llvm-project/commit/ddaa2c307dc74cc4ec5d11bbf025d30f1c9ec22b.diff](https://github.com/llvm/llvm-project/commit/ddaa2c307dc74cc4ec5d11bbf025d30f1c9ec22b.diff) might have fixed this.

---

### Comment 15 - RKSimon

> looks like https://github.com/llvm/llvm-project/commit/ddaa2c307dc74cc4ec5d11bbf025d30f1c9ec22b.diff might have fixed this.

@ye-luo does that match what you are seeing with latest?


---

### Comment 16 - ye-luo

@RKSimon the crash I reported is gone but new issue occurred. Will open a new ticket.

---

### Comment 17 - ye-luo

@RKSimon I saw https://github.com/llvm/llvm-project/issues/167681 and the failures I saw can potentially related to what is currently being worked on. Is it possible for you take a look at the nightly build failures https://cdash.qmcpack.org/viewBuildError.php?buildid=135198 and let me know if you do need reproducers or I should just wait for a few days. The remaining issues can be reproduced in CPU only builds.

---

### Comment 18 - RKSimon

@ye-luo Thanks - any repros you can provide would be very useful.

Is #167681 a typo? I can't see how its relevant.

---

### Comment 19 - ye-luo

> [@ye-luo](https://github.com/ye-luo) Thanks - any repros you can provide would be very useful.
> 
> Is [#167681](https://github.com/llvm/llvm-project/issues/167681) a typo? I can't see how its relevant.

Probably they are not related. Added a new ticket https://github.com/llvm/llvm-project/issues/167746

---

