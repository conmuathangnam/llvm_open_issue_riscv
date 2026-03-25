# clang is crash

**Author:** akemihomura0105
**URL:** https://github.com/llvm/llvm-project/issues/165152
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

```log
/opt/LLVM/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBSONCXX_STATIC -DLUA_USE_DLOPEN -DMONGOCXX_STATIC -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FWRITE_UNLOCKED -DYAML_CPP_STATIC_DEFINE -I/workspaces/IdleGame/idle_server/src -I/workspaces/IdleGame/common/src -I/workspaces/IdleGame/proto/gen_src -I/workspaces/IdleGame/framework/net/src -I/workspaces/IdleGame/config/src -I/workspaces/IdleGame/framework/redis/src -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/concurrentqueue -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/vincentlaucsb-csv-parser -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/bsoncxx/v_noabi -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/mongocxx/v_noabi -Wall -Wextra -Werror -g -stdlib=libc++ -g -std=c++23 -MD -MT idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -MF idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o.d @idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o.modmap -o idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -c /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/LLVM/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name PlayerItemComp.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/workspaces/IdleGame/build/default -fcoverage-compilation-dir=/workspaces/IdleGame/build/default -resource-dir /opt/LLVM/lib/clang/21 -Wall -Wextra -Werror -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=idle_server:PlayerItemComp=idle_server/CMakeFiles/idle_server.dir/idle_server-PlayerItemComp.pcm -fmodule-file=idle_server:Player=idle_server/CMakeFiles/idle_server.dir/idle_server-Player.pcm -fmodule-file=idle_server:StackableItem=idle_server/CMakeFiles/idle_server.dir/idle_server-StackableItem.pcm -fmodule-file=config=config/CMakeFiles/idle_config.dir/config.pcm -fmodule-file=common=common/CMakeFiles/common.dir/common.pcm -fmodule-file=common:basic_parse=common/CMakeFiles/common.dir/common-basic_parse.pcm -fmodule-file=common:common_head=common/CMakeFiles/common.dir/common-common_head.pcm -fmodule-file=common:common_math=common/CMakeFiles/common.dir/common-common_math.pcm -fmodule-file=common:enable_singleton=common/CMakeFiles/common.dir/common-enable_singleton.pcm -fmodule-file=common:template_utils=common/CMakeFiles/common.dir/common-template_utils.pcm -fmodule-file=config:ConfigBase=config/CMakeFiles/idle_config.dir/config-ConfigBase.pcm -fmodule-file=config:ConfigMgr=config/CMakeFiles/idle_config.dir/config-ConfigMgr.pcm -fmodule-file=config:ConfigProperty=config/CMakeFiles/idle_config.dir/config-ConfigProperty.pcm -fmodule-file=config:ItemConfig=config/CMakeFiles/idle_config.dir/config-ItemConfig.pcm -fmodule-file=config:ServerConfig=config/CMakeFiles/idle_config.dir/config-ServerConfig.pcm -fmodule-file=idle_server:Item=idle_server/CMakeFiles/idle_server.dir/idle_server-Item.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -x pcm idle_server/CMakeFiles/idle_server.dir/idle_server-PlayerItemCompImpl.pcm
1.      <eof> parser at end of file
2.      /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp:45:39: LLVM IR generation of declaration 'PlayerItemComp::createItem'
3.      /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp:45:39: Generating code for declaration 'PlayerItemComp::createItem'
 #0 0x000059a280171197 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/LLVM/bin/clang-21+0x7f71197)
 #1 0x000059a28017198b SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000789a19b22330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x000059a282f42933 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/LLVM/bin/clang-21+0xad42933)
 #4 0x000059a282f41efe clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/opt/LLVM/bin/clang-21+0xad41efe)
 #5 0x000059a28325e3a4 clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/opt/LLVM/bin/clang-21+0xb05e3a4)
 #6 0x000059a28325f023 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0>(long) (.llvm.18326144431535631573) CGExpr.cpp:0:0
 #7 0x000059a28324ff4e clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04ff4e)
 #8 0x000059a282e93b7c clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xac93b7c)
 #9 0x000059a282ed7c8c clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xacd7c8c)
#10 0x000059a28325f039 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0>(long) (.llvm.18326144431535631573) CGExpr.cpp:0:0
#11 0x000059a28324ff4e clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04ff4e)
#12 0x000059a28324fb4d clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04fb4d)
#13 0x000059a28325972d clang::StmtVisitorBase<std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) (.llvm.3767776280653265475) CGExprScalar.cpp:0:0
#14 0x000059a28325933d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/LLVM/bin/clang-21+0xb05933d)
#15 0x000059a282ca7458 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/LLVM/bin/clang-21+0xaaa7458)
#16 0x000059a283280372 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/LLVM/bin/clang-21+0xb080372)
#17 0x000059a28327f632 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (/opt/LLVM/bin/clang-21+0xb07f632)
#18 0x000059a283249266 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/LLVM/bin/clang-21+0xb049266)
#19 0x000059a28324d4dd clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/LLVM/bin/clang-21+0xb04d4dd)
#20 0x000059a282e3bbfa clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/LLVM/bin/clang-21+0xac3bbfa)
#21 0x000059a282e4c416 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/LLVM/bin/clang-21+0xac4c416)
#22 0x000059a282e48a3e clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/LLVM/bin/clang-21+0xac48a3e)
#23 0x000059a282ec5c27 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/LLVM/bin/clang-21+0xacc5c27)
#24 0x000059a282ebd3e4 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (.llvm.12750047744568612189) ModuleBuilder.cpp:0:0
#25 0x000059a282ebd32c clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/LLVM/bin/clang-21+0xacbd32c)
#26 0x000059a280904971 clang::ASTReader::PassInterestingDeclsToConsumer() (/opt/LLVM/bin/clang-21+0x8704971)
#27 0x000059a2808b3ecf non-virtual thunk to clang::ASTReader::StartTranslationUnit(clang::ASTConsumer*) (/opt/LLVM/bin/clang-21+0x86b3ecf)
#28 0x000059a283fa69f0 clang::ParseAST(clang::Sema&, bool, bool) (.cold.0) bolt-pseudo.o:0:0
#29 0x000059a28374526d clang::FrontendAction::Execute() (/opt/LLVM/bin/clang-21+0xb54526d)
#30 0x000059a28374b96f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/LLVM/bin/clang-21+0xb54b96f)
#31 0x000059a28374b812 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/LLVM/bin/clang-21+0xb54b812)
#32 0x000059a28374af95 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/LLVM/bin/clang-21+0xb54af95)
#33 0x000059a28361c183 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x000059a283620928 clang_main(int, char**, llvm::ToolContext const&) (/opt/LLVM/bin/clang-21+0xb420928)
#35 0x000059a28361fb92 main (/opt/LLVM/bin/clang-21+0xb41fb92)
#36 0x0000789a19b071ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#37 0x0000789a19b0728b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#38 0x000059a2834e59e5 _start (/opt/LLVM/bin/clang-21+0xb2e59e5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.3 (https://github.com/llvm/llvm-project 450f52eec88f728c89a9efd667dbeaf2dad93826)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/LLVM/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/PlayerItemComp-c71e71.cppm
clang++: note: diagnostic msg: /tmp/PlayerItemComp-c71e71.sh
clang++: note: diagnostic msg: 

********************
```

[tmpPlayerItemComp-c71e71.sh](https://github.com/user-attachments/files/23148475/tmpPlayerItemComp-c71e71.sh)

[tmpPlayerItemComp-c71e71_cppm.txt](https://github.com/user-attachments/files/23148476/tmpPlayerItemComp-c71e71_cppm.txt)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: akemihomura (akemihomura0105)

<details>
/opt/LLVM/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DBSONCXX_STATIC -DLUA_USE_DLOPEN -DMONGOCXX_STATIC -DSPDLOG_FMT_EXTERNAL -DSPDLOG_FWRITE_UNLOCKED -DYAML_CPP_STATIC_DEFINE -I/workspaces/IdleGame/idle_server/src -I/workspaces/IdleGame/common/src -I/workspaces/IdleGame/proto/gen_src -I/workspaces/IdleGame/framework/net/src -I/workspaces/IdleGame/config/src -I/workspaces/IdleGame/framework/redis/src -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/concurrentqueue -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/vincentlaucsb-csv-parser -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/bsoncxx/v_noabi -isystem /workspaces/IdleGame/build/default/vcpkg_installed/x64-linux-clang-libstdcxx/include/mongocxx/v_noabi -Wall -Wextra -Werror -g -stdlib=libc++ -g -std=c++23 -MD -MT idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -MF idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o.d @<!-- -->idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o.modmap -o idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -c /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/LLVM/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name PlayerItemComp.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/workspaces/IdleGame/build/default -fcoverage-compilation-dir=/workspaces/IdleGame/build/default -resource-dir /opt/LLVM/lib/clang/21 -Wall -Wextra -Werror -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=idle_server:PlayerItemComp=idle_server/CMakeFiles/idle_server.dir/idle_server-PlayerItemComp.pcm -fmodule-file=idle_server:Player=idle_server/CMakeFiles/idle_server.dir/idle_server-Player.pcm -fmodule-file=idle_server:StackableItem=idle_server/CMakeFiles/idle_server.dir/idle_server-StackableItem.pcm -fmodule-file=config=config/CMakeFiles/idle_config.dir/config.pcm -fmodule-file=common=common/CMakeFiles/common.dir/common.pcm -fmodule-file=common:basic_parse=common/CMakeFiles/common.dir/common-basic_parse.pcm -fmodule-file=common:common_head=common/CMakeFiles/common.dir/common-common_head.pcm -fmodule-file=common:common_math=common/CMakeFiles/common.dir/common-common_math.pcm -fmodule-file=common:enable_singleton=common/CMakeFiles/common.dir/common-enable_singleton.pcm -fmodule-file=common:template_utils=common/CMakeFiles/common.dir/common-template_utils.pcm -fmodule-file=config:ConfigBase=config/CMakeFiles/idle_config.dir/config-ConfigBase.pcm -fmodule-file=config:ConfigMgr=config/CMakeFiles/idle_config.dir/config-ConfigMgr.pcm -fmodule-file=config:ConfigProperty=config/CMakeFiles/idle_config.dir/config-ConfigProperty.pcm -fmodule-file=config:ItemConfig=config/CMakeFiles/idle_config.dir/config-ItemConfig.pcm -fmodule-file=config:ServerConfig=config/CMakeFiles/idle_config.dir/config-ServerConfig.pcm -fmodule-file=idle_server:Item=idle_server/CMakeFiles/idle_server.dir/idle_server-Item.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o idle_server/CMakeFiles/idle_server.dir/src/PlayerItem/PlayerItemComp.cpp.o -x pcm idle_server/CMakeFiles/idle_server.dir/idle_server-PlayerItemCompImpl.pcm
1.      &lt;eof&gt; parser at end of file
2.      /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp:45:39: LLVM IR generation of declaration 'PlayerItemComp::createItem'
3.      /workspaces/IdleGame/idle_server/src/PlayerItem/PlayerItemComp.cpp:45:39: Generating code for declaration 'PlayerItemComp::createItem'
 #<!-- -->0 0x000059a280171197 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/LLVM/bin/clang-21+0x7f71197)
 #<!-- -->1 0x000059a28017198b SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000789a19b22330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x000059a282f42933 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/LLVM/bin/clang-21+0xad42933)
 #<!-- -->4 0x000059a282f41efe clang::CodeGen::CodeGenFunction::GetAddressOfBaseClass(clang::CodeGen::Address, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const* const*, clang::CXXBaseSpecifier const* const*, bool, clang::SourceLocation) (/opt/LLVM/bin/clang-21+0xad41efe)
 #<!-- -->5 0x000059a28325e3a4 clang::CodeGen::CodeGenFunction::EmitCastLValue(clang::CastExpr const*) (/opt/LLVM/bin/clang-21+0xb05e3a4)
 #<!-- -->6 0x000059a28325f023 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0&gt;(long) (.llvm.18326144431535631573) CGExpr.cpp:0:0
 #<!-- -->7 0x000059a28324ff4e clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04ff4e)
 #<!-- -->8 0x000059a282e93b7c clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xac93b7c)
 #<!-- -->9 0x000059a282ed7c8c clang::CodeGen::CodeGenFunction::EmitCallExprLValue(clang::CallExpr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xacd7c8c)
#<!-- -->10 0x000059a28325f039 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::$_0&gt;(long) (.llvm.18326144431535631573) CGExpr.cpp:0:0
#<!-- -->11 0x000059a28324ff4e clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04ff4e)
#<!-- -->12 0x000059a28324fb4d clang::CodeGen::CodeGenFunction::EmitCXXMemberCallExpr(clang::CXXMemberCallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/LLVM/bin/clang-21+0xb04fb4d)
#<!-- -->13 0x000059a28325972d clang::StmtVisitorBase&lt;std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) (.llvm.3767776280653265475) CGExprScalar.cpp:0:0
#<!-- -->14 0x000059a28325933d clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/LLVM/bin/clang-21+0xb05933d)
#<!-- -->15 0x000059a282ca7458 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/LLVM/bin/clang-21+0xaaa7458)
#<!-- -->16 0x000059a283280372 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/LLVM/bin/clang-21+0xb080372)
#<!-- -->17 0x000059a28327f632 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (/opt/LLVM/bin/clang-21+0xb07f632)
#<!-- -->18 0x000059a283249266 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/LLVM/bin/clang-21+0xb049266)
#<!-- -->19 0x000059a28324d4dd clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/LLVM/bin/clang-21+0xb04d4dd)
#<!-- -->20 0x000059a282e3bbfa clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/LLVM/bin/clang-21+0xac3bbfa)
#<!-- -->21 0x000059a282e4c416 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/LLVM/bin/clang-21+0xac4c416)
#<!-- -->22 0x000059a282e48a3e clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/LLVM/bin/clang-21+0xac48a3e)
#<!-- -->23 0x000059a282ec5c27 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/LLVM/bin/clang-21+0xacc5c27)
#<!-- -->24 0x000059a282ebd3e4 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (.llvm.12750047744568612189) ModuleBuilder.cpp:0:0
#<!-- -->25 0x000059a282ebd32c clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/LLVM/bin/clang-21+0xacbd32c)
#<!-- -->26 0x000059a280904971 clang::ASTReader::PassInterestingDeclsToConsumer() (/opt/LLVM/bin/clang-21+0x8704971)
#<!-- -->27 0x000059a2808b3ecf non-virtual thunk to clang::ASTReader::StartTranslationUnit(clang::ASTConsumer*) (/opt/LLVM/bin/clang-21+0x86b3ecf)
#<!-- -->28 0x000059a283fa69f0 clang::ParseAST(clang::Sema&amp;, bool, bool) (.cold.0) bolt-pseudo.o:0:0
#<!-- -->29 0x000059a28374526d clang::FrontendAction::Execute() (/opt/LLVM/bin/clang-21+0xb54526d)
#<!-- -->30 0x000059a28374b96f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/LLVM/bin/clang-21+0xb54b96f)
#<!-- -->31 0x000059a28374b812 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/LLVM/bin/clang-21+0xb54b812)
#<!-- -->32 0x000059a28374af95 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/LLVM/bin/clang-21+0xb54af95)
#<!-- -->33 0x000059a28361c183 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x000059a283620928 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/LLVM/bin/clang-21+0xb420928)
#<!-- -->35 0x000059a28361fb92 main (/opt/LLVM/bin/clang-21+0xb41fb92)
#<!-- -->36 0x0000789a19b071ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->37 0x0000789a19b0728b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->38 0x000059a2834e59e5 _start (/opt/LLVM/bin/clang-21+0xb2e59e5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.3 (https://github.com/llvm/llvm-project 450f52eec88f728c89a9efd667dbeaf2dad93826)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/LLVM/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/PlayerItemComp-c71e71.cppm
clang++: note: diagnostic msg: /tmp/PlayerItemComp-c71e71.sh
clang++: note: diagnostic msg: 

********************

[tmpPlayerItemComp-c71e71.sh](https://github.com/user-attachments/files/23148475/tmpPlayerItemComp-c71e71.sh)

[tmpPlayerItemComp-c71e71_cppm.txt](https://github.com/user-attachments/files/23148476/tmpPlayerItemComp-c71e71_cppm.txt)

</details>


---

### Comment 2 - akemihomura0105

@EugeneZelenko need I make a minimum reproduction?

---

### Comment 3 - EugeneZelenko

Minimal reproducer, preferably on https://godbolt.org, will be very helpful.

---

### Comment 4 - akemihomura0105

I have tried my best to minimalize this case.
[minimal_demo.zip](https://github.com/user-attachments/files/23285832/minimal_demo.zip)
cmake -B build -S . -G Ninja && cmake --build build 
This should reproduce the crash. @EugeneZelenko 

---

