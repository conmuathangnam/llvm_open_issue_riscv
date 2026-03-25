# [clang] Crash in ConstructAttributeList

**Author:** ReillyBrogan
**URL:** https://github.com/llvm/llvm-project/issues/187535
**Status:** Open
**Labels:** clang:codegen, crash

## Body

There is a regression with clang-22 building systemd 257.13 that I have bisected to 5e4fd5095add84a3f827695e92255fe95d59aab9:

```
FAILED: [code=1] src/shared/libsystemd-shared-257.a.p/creds-util.c.o
/home/reilly/aerynos/src-pkg/llvm-project/build/bin/clang -Isrc/shared/libsystemd-shared-257.a.p -Isrc/shared -I../src/shared -Isrc/basic -I../src/basic -Isrc/fundamental -I../src/fundamental -Isrc/systemd -I../src/systemd -I. -I.. -I../src/libsystemd/sd-bus -I../src/libsystemd/sd-device -I../src/libsystemd/sd-event -I../src/libsystemd/sd-hwdb -I../src/libsystemd/sd-id128 -I../src/libsystemd/sd-journal -I../src/libsystemd/sd-json -I../src/libsystemd/sd-netlink -I../src/libsystemd/sd-network -I../src/libsystemd/sd-path -I../src/libsystemd/sd-resolve -I../src/libsystemd/sd-varlink -I/usr/include/blkid -I/usr/include/libmount -I/usr/include/uuid -I/usr/include/json-c -I/usr/include/p11-kit-1 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wextra -std=gnu11 -Wno-missing-field-initializers -Wno-unknown-warning-option -Wno-unused-parameter -Warray-bounds -Wdate-time -Wendif-labels -Werror=format=2 -Werror=format-signedness -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Werror=missing-declarations -Werror=missing-prototypes -Werror=overflow -Werror=override-init -Werror=return-type -Werror=shift-count-overflow -Werror=undef -Wfloat-equal -Winit-self -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wold-style-definition -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-aliasing=2 -Wstrict-prototypes -Wunterminated-string-initialization -Wunused-function -Wwrite-strings '-Wno-error=#warnings' -Wno-string-plus-int -fdiagnostics-show-option -fno-common -fstack-protector -fstack-protector-strong -fstrict-flex-arrays=3 --param=ssp-buffer-size=4 -Wno-typedef-redefinition -Wno-gnu-variable-sized-type-not-at-end -ftrivial-auto-var-init=zero -Werror=shadow -fPIC -fno-strict-aliasing -fstrict-flex-arrays=1 -fvisibility=hidden -ffunction-sections -fdata-sections -include config.h -DWITH_GZFILEOP -pthread -fvisibility=default -MD -MQ src/shared/libsystemd-shared-257.a.p/creds-util.c.o -MF src/shared/libsystemd-shared-257.a.p/creds-util.c.o.d -o src/shared/libsystemd-shared-257.a.p/creds-util.c.o -c ../src/shared/creds-util.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/reilly/aerynos/src-pkg/llvm-project/build/bin/clang -Isrc/shared/libsystemd-shared-257.a.p -Isrc/shared -I../src/shared -Isrc/basic -I../src/basic -Isrc/fundamental -I../src/fundamental -Isrc/systemd -I../src/systemd -I. -I.. -I../src/libsystemd/sd-bus -I../src/libsystemd/sd-device -I../src/libsystemd/sd-event -I../src/libsystemd/sd-hwdb -I../src/libsystemd/sd-id128 -I../src/libsystemd/sd-journal -I../src/libsystemd/sd-json -I../src/libsystemd/sd-netlink -I../src/libsystemd/sd-network -I../src/libsystemd/sd-path -I../src/libsystemd/sd-resolve -I../src/libsystemd/sd-varlink -I/usr/include/blkid -I/usr/include/libmount -I/usr/include/uuid -I/usr/include/json-c -I/usr/include/p11-kit-1 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wextra -std=gnu11 -Wno-missing-field-initializers -Wno-unknown-warning-option -Wno-unused-parameter -Warray-bounds -Wdate-time -Wendif-labels -Werror=format=2 -Werror=format-signedness -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Werror=missing-declarations -Werror=missing-prototypes -Werror=overflow -Werror=override-init -Werror=return-type -Werror=shift-count-overflow -Werror=undef -Wfloat-equal -Winit-self -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wold-style-definition -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-aliasing=2 -Wstrict-prototypes -Wunterminated-string-initialization -Wunused-function -Wwrite-strings -Wno-error=#warnings -Wno-string-plus-int -fdiagnostics-show-option -fno-common -fstack-protector -fstack-protector-strong -fstrict-flex-arrays=3 --param=ssp-buffer-size=4 -Wno-typedef-redefinition -Wno-gnu-variable-sized-type-not-at-end -ftrivial-auto-var-init=zero -Werror=shadow -fPIC -fno-strict-aliasing -fstrict-flex-arrays=1 -fvisibility=hidden -ffunction-sections -fdata-sections -include config.h -DWITH_GZFILEOP -pthread -fvisibility=default -MD -MQ src/shared/libsystemd-shared-257.a.p/creds-util.c.o -MF src/shared/libsystemd-shared-257.a.p/creds-util.c.o.d -o src/shared/libsystemd-shared-257.a.p/creds-util.c.o -c ../src/shared/creds-util.c
1.      ../src/shared/creds-util.c:1182:1: current parser token 'int'
2.      ../src/shared/creds-util.c:789:5: LLVM IR generation of declaration 'encrypt_credential_and_warn'
3.      ../src/shared/creds-util.c:789:5: Generating code for declaration 'encrypt_credential_and_warn'
 #0 0x00007faa1a5f2f58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #1 0x00007faa1a5f0885 llvm::sys::RunSignalHandlers() /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x00007faa1a52c8d9 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00007faa1a52c8d9 CrashRecoverySignalHandler(int) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x00007faa18843ac0 (/usr/lib/libc.so.6+0x43ac0)
 #5 0x00007faa1f7c9c82 llvm::Type::isPointerTy() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/IR/Type.h:267:49
 #6 0x00007faa1f7c9c82 clang::CodeGen::CodeGenModule::ConstructAttributeList(llvm::StringRef, clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::CGCalleeInfo, llvm::AttributeList&, unsigned int&, bool, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGCall.cpp:3031:46
 #7 0x00007faa1f7d92ef clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::CGCallee const&, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&, llvm::CallBase**, bool, clang::SourceLocation, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGCall.cpp:5826:19
 #8 0x00007faa1f89d454 clang::CodeGen::CodeGenFunction::getDebugInfo() /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:2147:9
 #9 0x00007faa1f89d454 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:6592:25
#10 0x00007faa1f89bf2f clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:0:10
#11 0x00007faa1f8e926f clang::CodeGen::RValue::getScalarVal() const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGValue.h:73:17
#12 0x00007faa1f8e926f (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:631:36
#13 0x00007faa1f8d9422 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:452:52
#14 0x00007faa1f8d9422 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5878:8
#15 0x00007faa1f879ec6 clang::Expr::getType() const /home/reilly/aerynos/src-pkg/llvm-project/clang/include/clang/AST/Expr.h:144:37
#16 0x00007faa1f879ec6 clang::CodeGen::CodeGenFunction::EvaluateExprAsBool(clang::Expr const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:234:55
#17 0x00007faa1fa78dc1 clang::CodeGen::CodeGenFunction::EmitBranchOnBoolExpr(clang::Expr const*, llvm::BasicBlock*, llvm::BasicBlock*, unsigned long, clang::Stmt::Likelihood, clang::Expr const*, clang::VarDecl const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:2090:13
#18 0x00007faa1fa09579 llvm::IRBuilderBase::GetInsertBlock() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/IR/IRBuilder.h:201:47
#19 0x00007faa1fa09579 clang::CodeGen::CodeGenFunction::EmitBlock(llvm::BasicBlock*, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:653:37
#20 0x00007faa1fa09579 clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:971:3
#21 0x00007faa1fa14f40 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:588:22
#22 0x00007faa1fa77bc8 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1376:5
#23 0x00007faa1fa77bc8 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1628:5
#24 0x00007faa1fa9a032 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6401:3
#25 0x00007faa1fa937ef llvm::TimeTraceScope::~TimeTraceScope() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#26 0x00007faa1fa937ef clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4485:3
#27 0x00007faa1fa97545 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4196:5
#28 0x00007faa1fa92740 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7312:5
#29 0x00007faa1fb3926c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:20
#30 0x00007faa1fa67f88 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:173:22
#31 0x00007faa1e669c0a clang::ParseAST(clang::Sema&, bool, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#32 0x00007faa20228c1d clang::FrontendAction::Execute() /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1318:10
#33 0x00007faa201a0c74 llvm::Error::getPtr() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#34 0x00007faa201a0c74 llvm::Error::operator bool() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#35 0x00007faa201a0c74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#36 0x00007faa202b000f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#37 0x0000561e4de56303 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/cc1_main.cpp:300:15
#38 0x0000561e4de52a37 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:225:12
#39 0x0000561e4de5447d std::__atomic_base<int>::fetch_sub(int, std::memory_order) /usr/bin/../lib64/gcc/x86_64-aerynos-linux/15/../../../../include/c++/15/bits/atomic_base.h:641:16
#40 0x0000561e4de5447d llvm::ThreadSafeRefCountedBase<llvm::vfs::FileSystem>::Release() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:134:32
#41 0x0000561e4de5447d llvm::IntrusiveRefCntPtrInfo<llvm::vfs::FileSystem>::release(llvm::vfs::FileSystem*) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:164:38
#42 0x0000561e4de5447d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:233:7
#43 0x0000561e4de5447d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
#44 0x0000561e4de5447d clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:369:5
#45 0x0000561e4de5447d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#46 0x00007faa1fe341f9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Job.cpp:436:30
#47 0x00007faa1fe341f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#48 0x00007faa1a52c618 llvm::function_ref<void ()>::operator()() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#49 0x00007faa1a52c618 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#50 0x00007faa1fe33c55 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Job.cpp:436:7
#51 0x00007faa1fdf84af clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#52 0x00007faa1fdf864e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#53 0x00007faa1fe1278f llvm::SmallVectorBase<unsigned int>::empty() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/SmallVector.h:82:46
#54 0x00007faa1fe1278f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Driver.cpp:2244:23
#55 0x0000561e4de524d8 clang_main(int, char**, llvm::ToolContext const&) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:407:21
#56 0x0000561e4de606c7 main /home/reilly/aerynos/src-pkg/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#57 0x00007faa188273c1 __libc_start_call_main /mason/build/x86_64/glibc.git/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#58 0x00007faa188274d8 call_init /mason/build/x86_64/glibc.git/csu/../csu/libc-start.c:128:20
#59 0x00007faa188274d8 __libc_start_main /mason/build/x86_64/glibc.git/csu/../csu/libc-start.c:347:5
#60 0x0000561e4de507b5 _start /mason/build/x86_64/glibc.git/csu/../sysdeps/x86_64/start.S:117:0
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-aerynos-linux
Thread model: posix
InstalledDir: /home/reilly/aerynos/src-pkg/llvm-project/build/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/creds-util-cb455d.c
clang: note: diagnostic msg: /tmp/creds-util-cb455d.sh
clang: note: diagnostic msg:

********************
```

[creds-util-cb455d.c](https://github.com/user-attachments/files/26121995/creds-util-cb455d.c)
[creds-util-cb455d.sh](https://github.com/user-attachments/files/26121994/creds-util-cb455d.sh)

For the bug reproduction I built clang with `cmake -G Ninja -B build -DLLVM_APPEND_VC_REV=OFF -DCMAKE_BUILD_TYPE=Release -DCLANG_DEFAULT_PIE_ON_LINUX=ON -DCLANG_LINK_CLANG_DYLIB=ON -DENABLE_LINKER_BUILD_ID=ON -DLLVM_LINK_LLVM_DYLIB=ON -DLLVM_TARGET_ARCH=x86_64 -DLLVM_ENABLE_ZSTD=ON -DLLVM_LINK_LLVM_DYLIB=ON -DLLVM_TARGETS_TO_BUILD="BPF;X86" -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-aerynos-linux -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_TOOLS=OFF -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF llvm` (note that `x86_64-aerynos-linux` is just an alias of `x86_64-linux-gnu`). 

Systemd source was checked out at v257.13 and configured with `meson setup --buildtype=plain --prefix=/usr --libdir=lib --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=lib/systemd --includedir=/usr/include --datadir=/usr/share --mandir=/usr/share/man --infodir=/usr/share/info --localedir=/usr/share/locale --sysconfdir=/etc --localstatedir=/var --wrap-mode=nodownload aerynos-builddir -Dacl=enabled -Dadm-gid=16 -Dadm-group=true -Danalyze=true -Daudio-gid=11 -Dbacklight=true -Dbinfmt=true -Dblkid=enabled -Dbootloader=enabled -Dbpf-compiler=clang -Dbpf-framework=enabled -Dbzip2=enabled -Dcdrom-gid=15 -Dcoredump=true -Dcreate-log-dirs=false -Ddefault-compression=zstd -Ddefault-dnssec=no -Ddefault-timeout-sec=5 -Ddialout-gid=10 -Ddisk-gid=8 -Ddns-over-tls=true -Defi=true -Delfutils=enabled -Denvironment-d=true -Dfdisk=enabled -Dfirstboot=true -Dfuzz-tests=false -Dgshadow=true -Dhibernate=true -Dhomed=enabled -Dhostnamed=true -Didn=true -Dima=true -Dinitrd=true -Dinstall-sysconfdir=false -Dinstall-tests=false -Dipe=true -Dkernel-install=false -Dkmem-gid=3 -Dkmod=enabled -Dkvm-gid=61 -Dldconfig=true -Dlibarchive=enabled -Dlibcryptsetup=enabled -Dlibcurl=enabled -Dlibfido2=enabled -Dlibidn2=enabled -Dlocaled=true -Dlogind=true -Dlp-gid=9 -Dmachined=true -Dman=disabled -Dmicrohttpd=enabled -Dmode=release -Dmountfsd=true -Dnetworkd=true -Dnobody-group=nobody -Dnobody-user=nobody -Dnsresourced=true -Dnss-systemd=true -Dok-color=highlight-blue -Doomd=true -Dopenssl=enabled -Dp11kit=enabled -Dpam=enabled -Dpamconfdir=/usr/share/defaults/pam.d -Dpamlibdir=/usr/lib/security -Dpcre2=enabled -Dpolkit=enabled -Dportabled=true -Dpstore=true -Dpwquality=enabled -Dqrencode=enabled -Dquotacheck=true -Drandomseed=true -Drender-gid=30 -Drepart=enabled -Drfkill=true -Drpmmacrosdir=no -Dsbat-distro=aerynos -Dsbat-distro-summary=AerynOS -Dsbat-distro-pkgname=systemd -Dsbat-distro-version=257.13 -Dsbat-distro-url=https://aerynos.com/ -Dseccomp=enabled -Dsgx-gid=405 -Dshellprofiledir=/usr/share/defaults/profile.d -Dslow-tests=false -Dsmack=true -Dsplit-bin=true -Dsshconfdir=/usr/share/defaults/ssh/ssh_config.d -Dsshdconfdir=/usr/share/defaults/ssh/sshd_config.d -Dstatus-unit-format-default=combined -Dstoragetm=true -Dsysext=true -Dsysupdate=disabled -Dsysupdated=disabled -Dsysusers=true -Dsysvinit-path= -Dtape-gid=4 -Dtests=false -Dtimedated=true -Dtimesyncd=true -Dtmpfiles=true -Dtpm=true -Dtpm2=enabled -Dtty-gid=5 -Duser-path=/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin -Duserdb=true -Dusers-gid=100 -Dutmp=true -Dutmp-gid=13 -Dvconsole=true -Dvideo-gid=12 -Dwheel-gid=28 -Dwheel-group=true -Dxdg-autostart=true -Dxkbcommon=enabled -Dzlib=enabled`

Reverting 5e4fd5095add84a3f827695e92255fe95d59aab9 on top of llvmorg-21.1.1 is enough to get past the crash.

CC @qchateau as author of bisected commit.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Reilly Brogan (ReillyBrogan)

<details>
There is a regression with clang-22 building systemd 257.13 that I have bisected to 5e4fd5095add84a3f827695e92255fe95d59aab9:

```
FAILED: [code=1] src/shared/libsystemd-shared-257.a.p/creds-util.c.o
/home/reilly/aerynos/src-pkg/llvm-project/build/bin/clang -Isrc/shared/libsystemd-shared-257.a.p -Isrc/shared -I../src/shared -Isrc/basic -I../src/basic -Isrc/fundamental -I../src/fundamental -Isrc/systemd -I../src/systemd -I. -I.. -I../src/libsystemd/sd-bus -I../src/libsystemd/sd-device -I../src/libsystemd/sd-event -I../src/libsystemd/sd-hwdb -I../src/libsystemd/sd-id128 -I../src/libsystemd/sd-journal -I../src/libsystemd/sd-json -I../src/libsystemd/sd-netlink -I../src/libsystemd/sd-network -I../src/libsystemd/sd-path -I../src/libsystemd/sd-resolve -I../src/libsystemd/sd-varlink -I/usr/include/blkid -I/usr/include/libmount -I/usr/include/uuid -I/usr/include/json-c -I/usr/include/p11-kit-1 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wextra -std=gnu11 -Wno-missing-field-initializers -Wno-unknown-warning-option -Wno-unused-parameter -Warray-bounds -Wdate-time -Wendif-labels -Werror=format=2 -Werror=format-signedness -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Werror=missing-declarations -Werror=missing-prototypes -Werror=overflow -Werror=override-init -Werror=return-type -Werror=shift-count-overflow -Werror=undef -Wfloat-equal -Winit-self -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wold-style-definition -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-aliasing=2 -Wstrict-prototypes -Wunterminated-string-initialization -Wunused-function -Wwrite-strings '-Wno-error=#warnings' -Wno-string-plus-int -fdiagnostics-show-option -fno-common -fstack-protector -fstack-protector-strong -fstrict-flex-arrays=3 --param=ssp-buffer-size=4 -Wno-typedef-redefinition -Wno-gnu-variable-sized-type-not-at-end -ftrivial-auto-var-init=zero -Werror=shadow -fPIC -fno-strict-aliasing -fstrict-flex-arrays=1 -fvisibility=hidden -ffunction-sections -fdata-sections -include config.h -DWITH_GZFILEOP -pthread -fvisibility=default -MD -MQ src/shared/libsystemd-shared-257.a.p/creds-util.c.o -MF src/shared/libsystemd-shared-257.a.p/creds-util.c.o.d -o src/shared/libsystemd-shared-257.a.p/creds-util.c.o -c ../src/shared/creds-util.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/reilly/aerynos/src-pkg/llvm-project/build/bin/clang -Isrc/shared/libsystemd-shared-257.a.p -Isrc/shared -I../src/shared -Isrc/basic -I../src/basic -Isrc/fundamental -I../src/fundamental -Isrc/systemd -I../src/systemd -I. -I.. -I../src/libsystemd/sd-bus -I../src/libsystemd/sd-device -I../src/libsystemd/sd-event -I../src/libsystemd/sd-hwdb -I../src/libsystemd/sd-id128 -I../src/libsystemd/sd-journal -I../src/libsystemd/sd-json -I../src/libsystemd/sd-netlink -I../src/libsystemd/sd-network -I../src/libsystemd/sd-path -I../src/libsystemd/sd-resolve -I../src/libsystemd/sd-varlink -I/usr/include/blkid -I/usr/include/libmount -I/usr/include/uuid -I/usr/include/json-c -I/usr/include/p11-kit-1 -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wextra -std=gnu11 -Wno-missing-field-initializers -Wno-unknown-warning-option -Wno-unused-parameter -Warray-bounds -Wdate-time -Wendif-labels -Werror=format=2 -Werror=format-signedness -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Werror=missing-declarations -Werror=missing-prototypes -Werror=overflow -Werror=override-init -Werror=return-type -Werror=shift-count-overflow -Werror=undef -Wfloat-equal -Winit-self -Wmissing-include-dirs -Wmissing-noreturn -Wnested-externs -Wold-style-definition -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-aliasing=2 -Wstrict-prototypes -Wunterminated-string-initialization -Wunused-function -Wwrite-strings -Wno-error=#warnings -Wno-string-plus-int -fdiagnostics-show-option -fno-common -fstack-protector -fstack-protector-strong -fstrict-flex-arrays=3 --param=ssp-buffer-size=4 -Wno-typedef-redefinition -Wno-gnu-variable-sized-type-not-at-end -ftrivial-auto-var-init=zero -Werror=shadow -fPIC -fno-strict-aliasing -fstrict-flex-arrays=1 -fvisibility=hidden -ffunction-sections -fdata-sections -include config.h -DWITH_GZFILEOP -pthread -fvisibility=default -MD -MQ src/shared/libsystemd-shared-257.a.p/creds-util.c.o -MF src/shared/libsystemd-shared-257.a.p/creds-util.c.o.d -o src/shared/libsystemd-shared-257.a.p/creds-util.c.o -c ../src/shared/creds-util.c
1.      ../src/shared/creds-util.c:1182:1: current parser token 'int'
2.      ../src/shared/creds-util.c:789:5: LLVM IR generation of declaration 'encrypt_credential_and_warn'
3.      ../src/shared/creds-util.c:789:5: Generating code for declaration 'encrypt_credential_and_warn'
 #<!-- -->0 0x00007faa1a5f2f58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->1 0x00007faa1a5f0885 llvm::sys::RunSignalHandlers() /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->2 0x00007faa1a52c8d9 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00007faa1a52c8d9 CrashRecoverySignalHandler(int) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #<!-- -->4 0x00007faa18843ac0 (/usr/lib/libc.so.6+0x43ac0)
 #<!-- -->5 0x00007faa1f7c9c82 llvm::Type::isPointerTy() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/IR/Type.h:267:49
 #<!-- -->6 0x00007faa1f7c9c82 clang::CodeGen::CodeGenModule::ConstructAttributeList(llvm::StringRef, clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::CGCalleeInfo, llvm::AttributeList&amp;, unsigned int&amp;, bool, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGCall.cpp:3031:46
 #<!-- -->7 0x00007faa1f7d92ef clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&amp;, llvm::CallBase**, bool, clang::SourceLocation, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGCall.cpp:5826:19
 #<!-- -->8 0x00007faa1f89d454 clang::CodeGen::CodeGenFunction::getDebugInfo() /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.h:2147:9
 #<!-- -->9 0x00007faa1f89d454 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&amp;, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:6592:25
#<!-- -->10 0x00007faa1f89bf2f clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:0:10
#<!-- -->11 0x00007faa1f8e926f clang::CodeGen::RValue::getScalarVal() const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGValue.h:73:17
#<!-- -->12 0x00007faa1f8e926f (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:631:36
#<!-- -->13 0x00007faa1f8d9422 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:452:52
#<!-- -->14 0x00007faa1f8d9422 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExprScalar.cpp:5878:8
#<!-- -->15 0x00007faa1f879ec6 clang::Expr::getType() const /home/reilly/aerynos/src-pkg/llvm-project/clang/include/clang/AST/Expr.h:144:37
#<!-- -->16 0x00007faa1f879ec6 clang::CodeGen::CodeGenFunction::EvaluateExprAsBool(clang::Expr const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGExpr.cpp:234:55
#<!-- -->17 0x00007faa1fa78dc1 clang::CodeGen::CodeGenFunction::EmitBranchOnBoolExpr(clang::Expr const*, llvm::BasicBlock*, llvm::BasicBlock*, unsigned long, clang::Stmt::Likelihood, clang::Expr const*, clang::VarDecl const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:2090:13
#<!-- -->18 0x00007faa1fa09579 llvm::IRBuilderBase::GetInsertBlock() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/IR/IRBuilder.h:201:47
#<!-- -->19 0x00007faa1fa09579 clang::CodeGen::CodeGenFunction::EmitBlock(llvm::BasicBlock*, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:653:37
#<!-- -->20 0x00007faa1fa09579 clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&amp;) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:971:3
#<!-- -->21 0x00007faa1fa14f40 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CGStmt.cpp:588:22
#<!-- -->22 0x00007faa1fa77bc8 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1376:5
#<!-- -->23 0x00007faa1fa77bc8 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1628:5
#<!-- -->24 0x00007faa1fa9a032 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6401:3
#<!-- -->25 0x00007faa1fa937ef llvm::TimeTraceScope::~TimeTraceScope() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:9
#<!-- -->26 0x00007faa1fa937ef clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4485:3
#<!-- -->27 0x00007faa1fa97545 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4196:5
#<!-- -->28 0x00007faa1fa92740 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7312:5
#<!-- -->29 0x00007faa1fb3926c (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:20
#<!-- -->30 0x00007faa1fa67f88 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:173:22
#<!-- -->31 0x00007faa1e669c0a clang::ParseAST(clang::Sema&amp;, bool, bool) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#<!-- -->32 0x00007faa20228c1d clang::FrontendAction::Execute() /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1318:10
#<!-- -->33 0x00007faa201a0c74 llvm::Error::getPtr() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#<!-- -->34 0x00007faa201a0c74 llvm::Error::operator bool() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#<!-- -->35 0x00007faa201a0c74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#<!-- -->36 0x00007faa202b000f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#<!-- -->37 0x0000561e4de56303 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/cc1_main.cpp:300:15
#<!-- -->38 0x0000561e4de52a37 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:225:12
#<!-- -->39 0x0000561e4de5447d std::__atomic_base&lt;int&gt;::fetch_sub(int, std::memory_order) /usr/bin/../lib64/gcc/x86_64-aerynos-linux/15/../../../../include/c++/15/bits/atomic_base.h:641:16
#<!-- -->40 0x0000561e4de5447d llvm::ThreadSafeRefCountedBase&lt;llvm::vfs::FileSystem&gt;::Release() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:134:32
#<!-- -->41 0x0000561e4de5447d llvm::IntrusiveRefCntPtrInfo&lt;llvm::vfs::FileSystem&gt;::release(llvm::vfs::FileSystem*) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:164:38
#<!-- -->42 0x0000561e4de5447d llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:233:7
#<!-- -->43 0x0000561e4de5447d llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
#<!-- -->44 0x0000561e4de5447d clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:369:5
#<!-- -->45 0x0000561e4de5447d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x00007faa1fe341f9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Job.cpp:436:30
#<!-- -->47 0x00007faa1fe341f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->48 0x00007faa1a52c618 llvm::function_ref&lt;void ()&gt;::operator()() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->49 0x00007faa1a52c618 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /home/reilly/aerynos/src-pkg/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#<!-- -->50 0x00007faa1fe33c55 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Job.cpp:436:7
#<!-- -->51 0x00007faa1fdf84af clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#<!-- -->52 0x00007faa1fdf864e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#<!-- -->53 0x00007faa1fe1278f llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /home/reilly/aerynos/src-pkg/llvm-project/llvm/include/llvm/ADT/SmallVector.h:82:46
#<!-- -->54 0x00007faa1fe1278f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /home/reilly/aerynos/src-pkg/llvm-project/clang/lib/Driver/Driver.cpp:2244:23
#<!-- -->55 0x0000561e4de524d8 clang_main(int, char**, llvm::ToolContext const&amp;) /home/reilly/aerynos/src-pkg/llvm-project/clang/tools/driver/driver.cpp:407:21
#<!-- -->56 0x0000561e4de606c7 main /home/reilly/aerynos/src-pkg/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#<!-- -->57 0x00007faa188273c1 __libc_start_call_main /mason/build/x86_64/glibc.git/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->58 0x00007faa188274d8 call_init /mason/build/x86_64/glibc.git/csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007faa188274d8 __libc_start_main /mason/build/x86_64/glibc.git/csu/../csu/libc-start.c:347:5
#<!-- -->60 0x0000561e4de507b5 _start /mason/build/x86_64/glibc.git/csu/../sysdeps/x86_64/start.S:117:0
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-aerynos-linux
Thread model: posix
InstalledDir: /home/reilly/aerynos/src-pkg/llvm-project/build/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/creds-util-cb455d.c
clang: note: diagnostic msg: /tmp/creds-util-cb455d.sh
clang: note: diagnostic msg:

********************
```

[udev-builtin-blkid-5a9462.c](https://github.com/user-attachments/files/26121842/udev-builtin-blkid-5a9462.c)
[udev-builtin-blkid-5a9462.sh](https://github.com/user-attachments/files/26121843/udev-builtin-blkid-5a9462.sh)

For the bug reproduction I built clang with `cmake -G Ninja -B build -DLLVM_APPEND_VC_REV=OFF -DCMAKE_BUILD_TYPE=Release -DCLANG_DEFAULT_PIE_ON_LINUX=ON -DCLANG_LINK_CLANG_DYLIB=ON -DENABLE_LINKER_BUILD_ID=ON -DLLVM_LINK_LLVM_DYLIB=ON -DLLVM_TARGET_ARCH=x86_64 -DLLVM_ENABLE_ZSTD=ON -DLLVM_LINK_LLVM_DYLIB=ON -DLLVM_TARGETS_TO_BUILD="BPF;X86" -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-aerynos-linux -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_TOOLS=OFF -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF llvm` (note that `x86_64-aerynos-linux` is just an alias of `x86_64-linux-gnu`). 

Systemd source was checked out at v257.13 and configured with `meson setup --buildtype=plain --prefix=/usr --libdir=lib --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=lib/systemd --includedir=/usr/include --datadir=/usr/share --mandir=/usr/share/man --infodir=/usr/share/info --localedir=/usr/share/locale --sysconfdir=/etc --localstatedir=/var --wrap-mode=nodownload aerynos-builddir -Dacl=enabled -Dadm-gid=16 -Dadm-group=true -Danalyze=true -Daudio-gid=11 -Dbacklight=true -Dbinfmt=true -Dblkid=enabled -Dbootloader=enabled -Dbpf-compiler=clang -Dbpf-framework=enabled -Dbzip2=enabled -Dcdrom-gid=15 -Dcoredump=true -Dcreate-log-dirs=false -Ddefault-compression=zstd -Ddefault-dnssec=no -Ddefault-timeout-sec=5 -Ddialout-gid=10 -Ddisk-gid=8 -Ddns-over-tls=true -Defi=true -Delfutils=enabled -Denvironment-d=true -Dfdisk=enabled -Dfirstboot=true -Dfuzz-tests=false -Dgshadow=true -Dhibernate=true -Dhomed=enabled -Dhostnamed=true -Didn=true -Dima=true -Dinitrd=true -Dinstall-sysconfdir=false -Dinstall-tests=false -Dipe=true -Dkernel-install=false -Dkmem-gid=3 -Dkmod=enabled -Dkvm-gid=61 -Dldconfig=true -Dlibarchive=enabled -Dlibcryptsetup=enabled -Dlibcurl=enabled -Dlibfido2=enabled -Dlibidn2=enabled -Dlocaled=true -Dlogind=true -Dlp-gid=9 -Dmachined=true -Dman=disabled -Dmicrohttpd=enabled -Dmode=release -Dmountfsd=true -Dnetworkd=true -Dnobody-group=nobody -Dnobody-user=nobody -Dnsresourced=true -Dnss-systemd=true -Dok-color=highlight-blue -Doomd=true -Dopenssl=enabled -Dp11kit=enabled -Dpam=enabled -Dpamconfdir=/usr/share/defaults/pam.d -Dpamlibdir=/usr/lib/security -Dpcre2=enabled -Dpolkit=enabled -Dportabled=true -Dpstore=true -Dpwquality=enabled -Dqrencode=enabled -Dquotacheck=true -Drandomseed=true -Drender-gid=30 -Drepart=enabled -Drfkill=true -Drpmmacrosdir=no -Dsbat-distro=aerynos -Dsbat-distro-summary=AerynOS -Dsbat-distro-pkgname=systemd -Dsbat-distro-version=257.13 -Dsbat-distro-url=https://aerynos.com/ -Dseccomp=enabled -Dsgx-gid=405 -Dshellprofiledir=/usr/share/defaults/profile.d -Dslow-tests=false -Dsmack=true -Dsplit-bin=true -Dsshconfdir=/usr/share/defaults/ssh/ssh_config.d -Dsshdconfdir=/usr/share/defaults/ssh/sshd_config.d -Dstatus-unit-format-default=combined -Dstoragetm=true -Dsysext=true -Dsysupdate=disabled -Dsysupdated=disabled -Dsysusers=true -Dsysvinit-path= -Dtape-gid=4 -Dtests=false -Dtimedated=true -Dtimesyncd=true -Dtmpfiles=true -Dtpm=true -Dtpm2=enabled -Dtty-gid=5 -Duser-path=/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin -Duserdb=true -Dusers-gid=100 -Dutmp=true -Dutmp-gid=13 -Dvconsole=true -Dvideo-gid=12 -Dwheel-gid=28 -Dwheel-group=true -Dxdg-autostart=true -Dxkbcommon=enabled -Dzlib=enabled`

Reverting 5e4fd5095add84a3f827695e92255fe95d59aab9 on top of llvmorg-21.1.1 is enough to get past the crash.

CC @<!-- -->qchateau as author bisected commit.
</details>


---

