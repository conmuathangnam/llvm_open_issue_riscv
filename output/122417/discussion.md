# Clang++ 19.1.6 crash

**Author:** hlewin
**URL:** https://github.com/llvm/llvm-project/issues/122417
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-01-20T08:56:05Z

## Body

I have run into a compiler crash in a project. I do not have much to say as the project is large and I was moving things around quite a bit, so I cannot correlate the crash to something significant. 

Sorry for sharing the requested files per link, but I just cannot upload things here - the web form does not seem to accept cpp or sh files.

(Links removed - use reduced form)

```
/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang++ -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_STATIC_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_STATIC_LINK -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_STATIC_LINK=1 -DBOOST_LOG_NO_LIB -DBOOST_LOG_STATIC_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_STATIC_LINK -DBOOST_THREAD_USE_LIB -DHW_COMPONENTS_LINUX -DIGG_FLAME_DETECTION_LINUX -DIGG_GRILL_MODEL_LINUX -DIGG_HW_CONTROL_CRC_CHECKS -DIGG_HW_CTRL_LINUX -DIGG_PROTO_COMMON_LINUX -DIGG_PROTO_FACTORYMODE_LINUX -DIGG_PROTO_USERMODE_LINUX -DIGG_UTILS_LINUX -DLED_CTRL_LINUX -DLED_CTRL_STATIC -DLINUX_KBD_LINUX -DLINUX_KBD_STATIC -DOHN_LINUX -DRT_CRC_LINUX -DRT_CRC_STATIC -DSYSFS_HELPER_LINUX -DSYSFS_HELPER_STATIC -DWDYUTIL_LINUX -DWDYUTIL_STATIC -DWDY_LINUX -DYAML_CPP_STATIC_DEFINE -I/crypt/schickling/hw-control/hw-components -I/crypt/schickling/hw-control/igg-utils/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-utils/configured-sources -I/crypt/schickling/hw-control/igg-grill-model/include -I/crypt/schickling/hw-control/igg-flame-detection/include -I/crypt/schickling/hw-control/igg-proto/common/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/common -I/crypt/schickling/hw-control/nanopb -I/crypt/schickling/hw-control/igg-proto/factorymode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/factorymode/.. -I/crypt/schickling/hw-control/igg-proto/usermode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/usermode/.. -isystem /home/hlewin/.conan/data/led-ctrl/1.0.3/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/linux-kbd/1.1.2/_/_/package/1cb849f10f06ca8e22d28700ebe05e281957464d/include -isystem /home/hlewin/.conan/data/sysfs-helper/1.1.14/_/_/package/459bf51e564f4c424ed4f1b92ec47ff5bce2e284/include -isystem /home/hlewin/.conan/data/fmt/11.1.1/_/_/package/a4ef8d6ddb428f78b17f9ecc3d787b76c2b2e692/include -isystem /home/hlewin/.conan/data/boost/1.87.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/ohn/1.0.6/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/wdyUtil/1.1.3/_/_/package/353ff42eb0437016cc4680c41aa83243143f4e84/include -isystem /home/hlewin/.conan/data/reflectcpp/0.17.0/_/_/package/508ef01fd15253b1f7efc8d71c5d8951bb0984ed/include -isystem /home/hlewin/.conan/data/ctre/3.9.0/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/yyjson/0.10.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/yaml-cpp/0.8.0/_/_/package/bae2d1cac058c64c6c27de323aec65aae01fe06c/include -isystem /home/hlewin/.conan/data/rt-crc/1.0.9/_/_/package/f462dc3b8b1b5d80f43cd7848aec1edaed8c30d4/include -isystem /home/hlewin/.conan/data/rapidcsv/8.82/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/lib/pkgconfig/../../include -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -march=native -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu  -m64 -g -std=gnu++23 -fPIE -fcolor-diagnostics -UNDEBUG -Wno-unqualified-std-cast-call -Wall -Wextra -Wconversion -Wparentheses -Werror=format -Wnonnull -Werror=nonnull -Werror=inaccessible-base -Werror=init-self -Werror=sequence-point -Werror=return-type -Werror=shift-count-negative -Werror=shift-count-overflow -Werror=uninitialized -Wpacked -Wcast-align -Wpointer-arith -Wno-unknown-pragmas -Werror=implicit-int -Werror=implicit-function-declaration -Werror=return-stack-address -Wno-missing-field-initializers -Werror -Wno-psabi -frtti -funwind-tables -fexceptions -Og -O0 -D_WDY_DEBUG_ -gdwarf-5 -MD -MT CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -MF CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.d @CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.modmap -o CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -c /crypt/schickling/hw-control/src/main.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang++ -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_STATIC_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_STATIC_LINK -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_STATIC_LINK=1 -DBOOST_LOG_NO_LIB -DBOOST_LOG_STATIC_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_STATIC_LINK -DBOOST_THREAD_USE_LIB -DHW_COMPONENTS_LINUX -DIGG_FLAME_DETECTION_LINUX -DIGG_GRILL_MODEL_LINUX -DIGG_HW_CONTROL_CRC_CHECKS -DIGG_HW_CTRL_LINUX -DIGG_PROTO_COMMON_LINUX -DIGG_PROTO_FACTORYMODE_LINUX -DIGG_PROTO_USERMODE_LINUX -DIGG_UTILS_LINUX -DLED_CTRL_LINUX -DLED_CTRL_STATIC -DLINUX_KBD_LINUX -DLINUX_KBD_STATIC -DOHN_LINUX -DRT_CRC_LINUX -DRT_CRC_STATIC -DSYSFS_HELPER_LINUX -DSYSFS_HELPER_STATIC -DWDYUTIL_LINUX -DWDYUTIL_STATIC -DWDY_LINUX -DYAML_CPP_STATIC_DEFINE -I/crypt/schickling/hw-control/hw-components -I/crypt/schickling/hw-control/igg-utils/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-utils/configured-sources -I/crypt/schickling/hw-control/igg-grill-model/include -I/crypt/schickling/hw-control/igg-flame-detection/include -I/crypt/schickling/hw-control/igg-proto/common/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/common -I/crypt/schickling/hw-control/nanopb -I/crypt/schickling/hw-control/igg-proto/factorymode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/factorymode/.. -I/crypt/schickling/hw-control/igg-proto/usermode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/usermode/.. -isystem /home/hlewin/.conan/data/led-ctrl/1.0.3/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/linux-kbd/1.1.2/_/_/package/1cb849f10f06ca8e22d28700ebe05e281957464d/include -isystem /home/hlewin/.conan/data/sysfs-helper/1.1.14/_/_/package/459bf51e564f4c424ed4f1b92ec47ff5bce2e284/include -isystem /home/hlewin/.conan/data/fmt/11.1.1/_/_/package/a4ef8d6ddb428f78b17f9ecc3d787b76c2b2e692/include -isystem /home/hlewin/.conan/data/boost/1.87.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/ohn/1.0.6/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/wdyUtil/1.1.3/_/_/package/353ff42eb0437016cc4680c41aa83243143f4e84/include -isystem /home/hlewin/.conan/data/reflectcpp/0.17.0/_/_/package/508ef01fd15253b1f7efc8d71c5d8951bb0984ed/include -isystem /home/hlewin/.conan/data/ctre/3.9.0/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/yyjson/0.10.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/yaml-cpp/0.8.0/_/_/package/bae2d1cac058c64c6c27de323aec65aae01fe06c/include -isystem /home/hlewin/.conan/data/rt-crc/1.0.9/_/_/package/f462dc3b8b1b5d80f43cd7848aec1edaed8c30d4/include -isystem /home/hlewin/.conan/data/rapidcsv/8.82/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/lib/pkgconfig/../../include -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -march=native -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -m64 -g -std=gnu++23 -fPIE -fcolor-diagnostics -UNDEBUG -Wno-unqualified-std-cast-call -Wall -Wextra -Wconversion -Wparentheses -Werror=format -Wnonnull -Werror=nonnull -Werror=inaccessible-base -Werror=init-self -Werror=sequence-point -Werror=return-type -Werror=shift-count-negative -Werror=shift-count-overflow -Werror=uninitialized -Wpacked -Wcast-align -Wpointer-arith -Wno-unknown-pragmas -Werror=implicit-int -Werror=implicit-function-declaration -Werror=return-stack-address -Wno-missing-field-initializers -Werror -Wno-psabi -frtti -funwind-tables -fexceptions -Og -O0 -D_WDY_DEBUG_ -gdwarf-5 -MD -MT CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -MF CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.d @CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.modmap -o CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -c /crypt/schickling/hw-control/src/main.cpp
1.	<eof> parser at end of file
2.	/crypt/schickling/hw-control/igg-proto/factorymode/include/FactoryMode.h:569:8: instantiating function definition 'igg::proto::factorymode::runFactoryMode<igg::hw_bindings::DefaultConfiguration>'
3.	/crypt/schickling/hw-control/igg-proto/factorymode/include/FactoryMode.h:59:5: instantiating function definition 'igg::proto::factorymode::FactoryMode<igg::hw_bindings::DefaultConfiguration>::handleNextRequest'
4.	/crypt/schickling/hw-control/igg-proto/common/include/ServiceMode.h:131:5: instantiating function definition 'igg::proto::common::ServiceMode<igg::hw_bindings::DefaultConfiguration, igg::proto::factorymode::FactoryModeHwServiceRequests, igg::proto::factorymode::FactoryMode<igg::hw_bindings::DefaultConfiguration>>::handleNextRequest'
5.	/crypt/schickling/hw-control/igg-proto/common/include/RequestResponse.h:68:17: instantiating function definition 'igg::proto::RequestResponseBase<_FactoryModeHwServiceRequest, _FactoryModeHwServiceResponse, igg::proto::factorymode::FactoryModeHwServiceRequests, true>::parseRequestVariant'
6.	/crypt/schickling/hw-control/igg-proto/common/include/RequestResponse.h:73:14: instantiating function definition 'igg::proto::RequestResponseBase<_FactoryModeHwServiceRequest, _FactoryModeHwServiceResponse, igg::proto::factorymode::FactoryModeHwServiceRequests, true>::parseRequestVariant(const RequestType &)::(anonymous class)::operator()<0UL, 1UL, 2UL, 3UL, 4UL, 5UL, 6UL, 7UL, 8UL, 9UL>'
 #0 0x0000555b3a997c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x40b6c38)
 #1 0x0000555b3a99561e llvm::sys::RunSignalHandlers() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x40b461e)
 #2 0x0000555b3a90a1b9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f656bcd41d0 (/usr/lib/libc.so.6+0x3d1d0)
 #4 0x0000555b3e008d04 clang::Stmt::getSourceRange() const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x7727d04)
 #5 0x0000555b3d861791 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000555b3d8366b1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000555b3d865ed1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000555b3d836623 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f55623)
 #9 0x0000555b3d89f9cf clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbe9cf)
#10 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#11 0x0000555b3d89fad2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbead2)
#12 0x0000555b3cdd3b9f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f2b9f)
#13 0x0000555b3d781162 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6ea0162)
#14 0x0000555b3d22a02d clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x694902d)
#15 0x0000555b3d23efe5 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695dfe5)
#16 0x0000555b3d23da0a clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::LookupResult&, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695ca0a)
#17 0x0000555b3d41f716 clang::Sema::BuildPossibleImplicitMemberExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, clang::TemplateArgumentListInfo const*, clang::Scope const*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6b3e716)
#18 0x0000555b3d23dd02 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&, clang::DeclarationNameInfo const&, bool, clang::TypeSourceInfo**) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695cd02)
#19 0x0000555b3d86475e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#20 0x0000555b3d85f481 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000555b3d834e9e clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f53e9e)
#22 0x0000555b3d89bf35 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbaf35)
#23 0x0000555b3d88ff6c clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6faef6c)
#24 0x0000555b3d88f484 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fae484)
#25 0x0000555b3d8cfa34 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(long) SemaTemplateInstantiateDecl.cpp:0:0
#26 0x0000555b3cdd3b9f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f2b9f)
#27 0x0000555b3d89d9d8 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbc9d8)
#28 0x0000555b3d87ddfa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#29 0x0000555b3d865ed1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x0000555b3d836623 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f55623)
#31 0x0000555b3d89f9cf clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbe9cf)
#32 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#33 0x0000555b3d89faec clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbeaec)
#34 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#35 0x0000555b3d89faec clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbeaec)
#36 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#37 0x0000555b3cdd61e3 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f51e3)
#38 0x0000555b3cdd69b7 clang::Sema::ActOnEndOfTranslationUnit() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f59b7)
#39 0x0000555b3cbb1264 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x62d0264)
#40 0x0000555b3cbab1ae clang::ParseAST(clang::Sema&, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x62ca1ae)
#41 0x0000555b3b4fe22d clang::FrontendAction::Execute() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4c1d22d)
#42 0x0000555b3b46b604 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4b8a604)
#43 0x0000555b3b5c90ae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4ce80ae)
#44 0x0000555b3945aab5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b79ab5)
#45 0x0000555b3945797f ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x0000555b3b2c2a39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#47 0x0000555b3a909f68 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4028f68)
#48 0x0000555b3b2c2024 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x49e1024)
#49 0x0000555b3b27f9a6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x499e9a6)
#50 0x0000555b3b27fcde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x499ecde)
#51 0x0000555b3b29e4cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x49bd4cc)
#52 0x0000555b39456c69 clang_main(int, char**, llvm::ToolContext const&) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b75c69)
#53 0x0000555b39466fc3 main (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b85fc3)
#54 0x00007f656bcbce08 (/usr/lib/libc.so.6+0x25e08)
#55 0x00007f656bcbcecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#56 0x0000555b39454fe5 _start (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b73fe5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-906bec.cpp
clang++: note: diagnostic msg: /tmp/main-906bec.sh
clang++: note: diagnostic msg: 

********************
`


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Heiko Lewin (hlewin)

<details>
I have run into a compiler crash in a project. I do not have much to say as the project is large and I was moving things around quite a bit, so I cannot correlate the crash to something significant. 

Sorry for sharing the requested files per link, but I just cannot upload things here - the web form does not seem to accept cpp or sh files.

[main-906bec.cpp](https://worldiety-my.sharepoint.com/:u:/g/personal/heiko_lewin_worldiety_de/Ecu4Mlp1EUlCjb7-rOQpvvgBeQVJAXsecE5MJA6_KevS3g?e=DehvV8
)
[main-906bec.sh](https://worldiety-my.sharepoint.com/:u:/g/personal/heiko_lewin_worldiety_de/EZ1YbeYWRC5IlFk62wTolFAB8tYSVgTws5qv6JWrKE6Z3Q?e=VE2GrR)

```
/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang++ -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_STATIC_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_STATIC_LINK -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_STATIC_LINK=1 -DBOOST_LOG_NO_LIB -DBOOST_LOG_STATIC_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_STATIC_LINK -DBOOST_THREAD_USE_LIB -DHW_COMPONENTS_LINUX -DIGG_FLAME_DETECTION_LINUX -DIGG_GRILL_MODEL_LINUX -DIGG_HW_CONTROL_CRC_CHECKS -DIGG_HW_CTRL_LINUX -DIGG_PROTO_COMMON_LINUX -DIGG_PROTO_FACTORYMODE_LINUX -DIGG_PROTO_USERMODE_LINUX -DIGG_UTILS_LINUX -DLED_CTRL_LINUX -DLED_CTRL_STATIC -DLINUX_KBD_LINUX -DLINUX_KBD_STATIC -DOHN_LINUX -DRT_CRC_LINUX -DRT_CRC_STATIC -DSYSFS_HELPER_LINUX -DSYSFS_HELPER_STATIC -DWDYUTIL_LINUX -DWDYUTIL_STATIC -DWDY_LINUX -DYAML_CPP_STATIC_DEFINE -I/crypt/schickling/hw-control/hw-components -I/crypt/schickling/hw-control/igg-utils/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-utils/configured-sources -I/crypt/schickling/hw-control/igg-grill-model/include -I/crypt/schickling/hw-control/igg-flame-detection/include -I/crypt/schickling/hw-control/igg-proto/common/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/common -I/crypt/schickling/hw-control/nanopb -I/crypt/schickling/hw-control/igg-proto/factorymode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/factorymode/.. -I/crypt/schickling/hw-control/igg-proto/usermode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/usermode/.. -isystem /home/hlewin/.conan/data/led-ctrl/1.0.3/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/linux-kbd/1.1.2/_/_/package/1cb849f10f06ca8e22d28700ebe05e281957464d/include -isystem /home/hlewin/.conan/data/sysfs-helper/1.1.14/_/_/package/459bf51e564f4c424ed4f1b92ec47ff5bce2e284/include -isystem /home/hlewin/.conan/data/fmt/11.1.1/_/_/package/a4ef8d6ddb428f78b17f9ecc3d787b76c2b2e692/include -isystem /home/hlewin/.conan/data/boost/1.87.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/ohn/1.0.6/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/wdyUtil/1.1.3/_/_/package/353ff42eb0437016cc4680c41aa83243143f4e84/include -isystem /home/hlewin/.conan/data/reflectcpp/0.17.0/_/_/package/508ef01fd15253b1f7efc8d71c5d8951bb0984ed/include -isystem /home/hlewin/.conan/data/ctre/3.9.0/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/yyjson/0.10.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/yaml-cpp/0.8.0/_/_/package/bae2d1cac058c64c6c27de323aec65aae01fe06c/include -isystem /home/hlewin/.conan/data/rt-crc/1.0.9/_/_/package/f462dc3b8b1b5d80f43cd7848aec1edaed8c30d4/include -isystem /home/hlewin/.conan/data/rapidcsv/8.82/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/lib/pkgconfig/../../include -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -march=native -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu  -m64 -g -std=gnu++23 -fPIE -fcolor-diagnostics -UNDEBUG -Wno-unqualified-std-cast-call -Wall -Wextra -Wconversion -Wparentheses -Werror=format -Wnonnull -Werror=nonnull -Werror=inaccessible-base -Werror=init-self -Werror=sequence-point -Werror=return-type -Werror=shift-count-negative -Werror=shift-count-overflow -Werror=uninitialized -Wpacked -Wcast-align -Wpointer-arith -Wno-unknown-pragmas -Werror=implicit-int -Werror=implicit-function-declaration -Werror=return-stack-address -Wno-missing-field-initializers -Werror -Wno-psabi -frtti -funwind-tables -fexceptions -Og -O0 -D_WDY_DEBUG_ -gdwarf-5 -MD -MT CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -MF CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.d @<!-- -->CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.modmap -o CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -c /crypt/schickling/hw-control/src/main.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang++ -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_STATIC_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_STATIC_LINK -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_STATIC_LINK=1 -DBOOST_LOG_NO_LIB -DBOOST_LOG_STATIC_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_STATIC_LINK -DBOOST_THREAD_USE_LIB -DHW_COMPONENTS_LINUX -DIGG_FLAME_DETECTION_LINUX -DIGG_GRILL_MODEL_LINUX -DIGG_HW_CONTROL_CRC_CHECKS -DIGG_HW_CTRL_LINUX -DIGG_PROTO_COMMON_LINUX -DIGG_PROTO_FACTORYMODE_LINUX -DIGG_PROTO_USERMODE_LINUX -DIGG_UTILS_LINUX -DLED_CTRL_LINUX -DLED_CTRL_STATIC -DLINUX_KBD_LINUX -DLINUX_KBD_STATIC -DOHN_LINUX -DRT_CRC_LINUX -DRT_CRC_STATIC -DSYSFS_HELPER_LINUX -DSYSFS_HELPER_STATIC -DWDYUTIL_LINUX -DWDYUTIL_STATIC -DWDY_LINUX -DYAML_CPP_STATIC_DEFINE -I/crypt/schickling/hw-control/hw-components -I/crypt/schickling/hw-control/igg-utils/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-utils/configured-sources -I/crypt/schickling/hw-control/igg-grill-model/include -I/crypt/schickling/hw-control/igg-flame-detection/include -I/crypt/schickling/hw-control/igg-proto/common/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/common -I/crypt/schickling/hw-control/nanopb -I/crypt/schickling/hw-control/igg-proto/factorymode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/factorymode/.. -I/crypt/schickling/hw-control/igg-proto/usermode/include -I/crypt/schickling/hw-control/cmake-build-debug-clang/igg-proto/usermode/.. -isystem /home/hlewin/.conan/data/led-ctrl/1.0.3/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/linux-kbd/1.1.2/_/_/package/1cb849f10f06ca8e22d28700ebe05e281957464d/include -isystem /home/hlewin/.conan/data/sysfs-helper/1.1.14/_/_/package/459bf51e564f4c424ed4f1b92ec47ff5bce2e284/include -isystem /home/hlewin/.conan/data/fmt/11.1.1/_/_/package/a4ef8d6ddb428f78b17f9ecc3d787b76c2b2e692/include -isystem /home/hlewin/.conan/data/boost/1.87.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/ohn/1.0.6/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/wdyUtil/1.1.3/_/_/package/353ff42eb0437016cc4680c41aa83243143f4e84/include -isystem /home/hlewin/.conan/data/reflectcpp/0.17.0/_/_/package/508ef01fd15253b1f7efc8d71c5d8951bb0984ed/include -isystem /home/hlewin/.conan/data/ctre/3.9.0/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/include -isystem /home/hlewin/.conan/data/yyjson/0.10.0/_/_/package/e1c3081ae155e1b49f0f79edc3009b5af9e7cda4/include -isystem /home/hlewin/.conan/data/yaml-cpp/0.8.0/_/_/package/bae2d1cac058c64c6c27de323aec65aae01fe06c/include -isystem /home/hlewin/.conan/data/rt-crc/1.0.9/_/_/package/f462dc3b8b1b5d80f43cd7848aec1edaed8c30d4/include -isystem /home/hlewin/.conan/data/rapidcsv/8.82/_/_/package/04cb4c8c8550583b792d26dac817b59384df60e9/lib/pkgconfig/../../include -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -march=native -I/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include -stdlib++-isystem/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/include/c++/v1 --target=x86_64-pc-linux-gnu -m64 -g -std=gnu++23 -fPIE -fcolor-diagnostics -UNDEBUG -Wno-unqualified-std-cast-call -Wall -Wextra -Wconversion -Wparentheses -Werror=format -Wnonnull -Werror=nonnull -Werror=inaccessible-base -Werror=init-self -Werror=sequence-point -Werror=return-type -Werror=shift-count-negative -Werror=shift-count-overflow -Werror=uninitialized -Wpacked -Wcast-align -Wpointer-arith -Wno-unknown-pragmas -Werror=implicit-int -Werror=implicit-function-declaration -Werror=return-stack-address -Wno-missing-field-initializers -Werror -Wno-psabi -frtti -funwind-tables -fexceptions -Og -O0 -D_WDY_DEBUG_ -gdwarf-5 -MD -MT CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -MF CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.d @<!-- -->CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o.modmap -o CMakeFiles/igg-hw-ctrl.dir/src/main.cpp.o -c /crypt/schickling/hw-control/src/main.cpp
1.	&lt;eof&gt; parser at end of file
2.	/crypt/schickling/hw-control/igg-proto/factorymode/include/FactoryMode.h:569:8: instantiating function definition 'igg::proto::factorymode::runFactoryMode&lt;igg::hw_bindings::DefaultConfiguration&gt;'
3.	/crypt/schickling/hw-control/igg-proto/factorymode/include/FactoryMode.h:59:5: instantiating function definition 'igg::proto::factorymode::FactoryMode&lt;igg::hw_bindings::DefaultConfiguration&gt;::handleNextRequest'
4.	/crypt/schickling/hw-control/igg-proto/common/include/ServiceMode.h:131:5: instantiating function definition 'igg::proto::common::ServiceMode&lt;igg::hw_bindings::DefaultConfiguration, igg::proto::factorymode::FactoryModeHwServiceRequests, igg::proto::factorymode::FactoryMode&lt;igg::hw_bindings::DefaultConfiguration&gt;&gt;::handleNextRequest'
5.	/crypt/schickling/hw-control/igg-proto/common/include/RequestResponse.h:68:17: instantiating function definition 'igg::proto::RequestResponseBase&lt;_FactoryModeHwServiceRequest, _FactoryModeHwServiceResponse, igg::proto::factorymode::FactoryModeHwServiceRequests, true&gt;::parseRequestVariant'
6.	/crypt/schickling/hw-control/igg-proto/common/include/RequestResponse.h:73:14: instantiating function definition 'igg::proto::RequestResponseBase&lt;_FactoryModeHwServiceRequest, _FactoryModeHwServiceResponse, igg::proto::factorymode::FactoryModeHwServiceRequests, true&gt;::parseRequestVariant(const RequestType &amp;)::(anonymous class)::operator()&lt;0UL, 1UL, 2UL, 3UL, 4UL, 5UL, 6UL, 7UL, 8UL, 9UL&gt;'
 #<!-- -->0 0x0000555b3a997c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x40b6c38)
 #<!-- -->1 0x0000555b3a99561e llvm::sys::RunSignalHandlers() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x40b461e)
 #<!-- -->2 0x0000555b3a90a1b9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f656bcd41d0 (/usr/lib/libc.so.6+0x3d1d0)
 #<!-- -->4 0x0000555b3e008d04 clang::Stmt::getSourceRange() const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x7727d04)
 #<!-- -->5 0x0000555b3d861791 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000555b3d8366b1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000555b3d865ed1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000555b3d836623 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f55623)
 #<!-- -->9 0x0000555b3d89f9cf clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbe9cf)
#<!-- -->10 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#<!-- -->11 0x0000555b3d89fad2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbead2)
#<!-- -->12 0x0000555b3cdd3b9f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f2b9f)
#<!-- -->13 0x0000555b3d781162 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6ea0162)
#<!-- -->14 0x0000555b3d22a02d clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x694902d)
#<!-- -->15 0x0000555b3d23efe5 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695dfe5)
#<!-- -->16 0x0000555b3d23da0a clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::LookupResult&amp;, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695ca0a)
#<!-- -->17 0x0000555b3d41f716 clang::Sema::BuildPossibleImplicitMemberExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, clang::TemplateArgumentListInfo const*, clang::Scope const*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6b3e716)
#<!-- -->18 0x0000555b3d23dd02 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo const&amp;, bool, clang::TypeSourceInfo**) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x695cd02)
#<!-- -->19 0x0000555b3d86475e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000555b3d85f481 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000555b3d834e9e clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f53e9e)
#<!-- -->22 0x0000555b3d89bf35 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbaf35)
#<!-- -->23 0x0000555b3d88ff6c clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6faef6c)
#<!-- -->24 0x0000555b3d88f484 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fae484)
#<!-- -->25 0x0000555b3d8cfa34 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->26 0x0000555b3cdd3b9f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f2b9f)
#<!-- -->27 0x0000555b3d89d9d8 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbc9d8)
#<!-- -->28 0x0000555b3d87ddfa clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x0000555b3d865ed1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x0000555b3d836623 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6f55623)
#<!-- -->31 0x0000555b3d89f9cf clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbe9cf)
#<!-- -->32 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#<!-- -->33 0x0000555b3d89faec clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbeaec)
#<!-- -->34 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#<!-- -->35 0x0000555b3d89faec clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fbeaec)
#<!-- -->36 0x0000555b3d8a24cd clang::Sema::PerformPendingInstantiations(bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x6fc14cd)
#<!-- -->37 0x0000555b3cdd61e3 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f51e3)
#<!-- -->38 0x0000555b3cdd69b7 clang::Sema::ActOnEndOfTranslationUnit() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x64f59b7)
#<!-- -->39 0x0000555b3cbb1264 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x62d0264)
#<!-- -->40 0x0000555b3cbab1ae clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x62ca1ae)
#<!-- -->41 0x0000555b3b4fe22d clang::FrontendAction::Execute() (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4c1d22d)
#<!-- -->42 0x0000555b3b46b604 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4b8a604)
#<!-- -->43 0x0000555b3b5c90ae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4ce80ae)
#<!-- -->44 0x0000555b3945aab5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b79ab5)
#<!-- -->45 0x0000555b3945797f ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x0000555b3b2c2a39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000555b3a909f68 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x4028f68)
#<!-- -->48 0x0000555b3b2c2024 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x49e1024)
#<!-- -->49 0x0000555b3b27f9a6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x499e9a6)
#<!-- -->50 0x0000555b3b27fcde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x499ecde)
#<!-- -->51 0x0000555b3b29e4cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x49bd4cc)
#<!-- -->52 0x0000555b39456c69 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b75c69)
#<!-- -->53 0x0000555b39466fc3 main (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b85fc3)
#<!-- -->54 0x00007f656bcbce08 (/usr/lib/libc.so.6+0x25e08)
#<!-- -->55 0x00007f656bcbcecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#<!-- -->56 0x0000555b39454fe5 _start (/home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin/clang+++0x2b73fe5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /home/hlewin/.conan/data/llvm/19.1.6/_/_/package/688d490e62578a2f75304f915e89547651f25074/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-906bec.cpp
clang++: note: diagnostic msg: /tmp/main-906bec.sh
clang++: note: diagnostic msg: 

********************
`

</details>


---

### Comment 2 - MagentaTreehouse

Reduced:
```c++
template <int...>
struct int_sequence {};

template <int>
using element_t = int;

template <class>
void parseRequestVariant()
{
  []<int... index>(int_sequence<index...>) -> void {
    (
      [] {
        using T = element_t<index>;
      },
      ...);
  }(int_sequence<0>{});
}

int main()
{
  parseRequestVariant<void>();
}
```

Crash happens at different locations for Clang 19 and trunk.

---

Clang 19: https://compiler-explorer.com/z/Kvdo955sE

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/TreeTransform.h:15248:
clang::ExprResult clang::TreeTransform<Derived>::TransformCXXFoldExpr(clang::CXXFoldExpr*) [with Derived = {anonymous}::TemplateInstantiator; clang::ExprResult = clang::ActionResult<clang::Expr*>]:
Assertion `!Unexpanded.empty() && "Pack expansion without parameter packs?"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:8:6: instantiating function definition 'parseRequestVariant<void>'
3.	<source>:10:3: instantiating function definition 'parseRequestVariant()::(anonymous class)::operator()<0>'
 #0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
 #1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
 #2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007274e9242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007274e92969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007274e9242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007274e92287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007274e922871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007274e9239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000716996b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x000000000715bace clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000719a68f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000719ae44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071a64da clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71a64da)
#14 0x000000000721c318 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x721c318)
#15 0x000000000721a5bf clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x721a5bf)
#16 0x000000000721bd8c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x721bd8c)
#17 0x000000000721a5bf clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x721a5bf)
#18 0x000000000666152f clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#19 0x0000000006661d2a clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6661d2a)
#20 0x00000000064d904a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d904a)
#21 0x00000000064cc33a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc33a)
#22 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#23 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#24 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#25 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#26 0x0000000000c82b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#27 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x000000000443cba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#30 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#32 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#33 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#34 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#35 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#36 0x00007274e9229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x00007274e9229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
```

</details>

---

Clang trunk: https://compiler-explorer.com/z/4o43bnPnh

Assertion:
```console
clang++: /root/llvm-project/clang/lib/AST/ExprCXX.cpp:1960:
clang::CXXFoldExpr::CXXFoldExpr(clang::QualType, clang::UnresolvedLookupExpr*, clang::SourceLocation, clang::Expr*, clang::BinaryOperatorKind, clang::SourceLocation, clang::Expr*, clang::SourceLocation, std::optional<unsigned int>):
Assertion `((LHS && LHS->containsUnexpandedParameterPack()) != (RHS && RHS->containsUnexpandedParameterPack())) && "Exactly one of LHS or RHS should contain an unexpanded pack"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:8:6: instantiating function definition 'parseRequestVariant<void>'
 #0 0x0000000003c9d748 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c9d748)
 #1 0x0000000003c9b454 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c9b454)
 #2 0x0000000003be7fa8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072c07ae42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072c07ae969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072c07ae42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072c07ae287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072c07ae2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072c07ae39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000785fc93 clang::CXXFoldExpr::CXXFoldExpr(clang::QualType, clang::UnresolvedLookupExpr*, clang::SourceLocation, clang::Expr*, clang::BinaryOperatorKind, clang::SourceLocation, clang::Expr*, clang::SourceLocation, std::optional<unsigned int>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x785fc93)
#10 0x000000000722dde7 clang::Sema::BuildCXXFoldExpr(clang::UnresolvedLookupExpr*, clang::SourceLocation, clang::Expr*, clang::BinaryOperatorKind, clang::SourceLocation, clang::Expr*, clang::SourceLocation, std::optional<unsigned int>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722dde7)
#11 0x000000000717e266 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000716b729 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007199557 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000719a13e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000716aaa6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000716bffe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000717d236 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000716b814 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000007199557 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000719a13e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000071a2f14 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a2f14)
#22 0x000000000721ac07 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x721ac07)
#23 0x0000000007218d5f clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7218d5f)
#24 0x00000000067e814d clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#25 0x00000000067e8942 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e8942)
#26 0x000000000665488b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665488b)
#27 0x0000000006646c0a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6646c0a)
#28 0x000000000462c428 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462c428)
#29 0x00000000048ecd85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ecd85)
#30 0x000000000486f42e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486f42e)
#31 0x00000000049da31e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49da31e)
#32 0x0000000000cf07ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf07ff)
#33 0x0000000000ce82da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x0000000004672039 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000003be8454 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3be8454)
#36 0x000000000467262f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x000000000463615d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463615d)
#38 0x00000000046371de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46371de)
#39 0x000000000463e945 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463e945)
#40 0x0000000000ced633 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xced633)
#41 0x0000000000bb8be4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb8be4)
#42 0x000072c07ae29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x000072c07ae29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000ce7d85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce7d85)
```

</details>

---

