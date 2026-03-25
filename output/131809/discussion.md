# [clang] clang frontend command failed with exit code 139

**Author:** gcarq
**URL:** https://github.com/llvm/llvm-project/issues/131809

## Body

When compiling net-im/telegram-desktop-5.12.3 on Gentoo with a systemwide clang/libcxx profile, clang++ fails with exit code 139.

```
Stack dump:
0.      Program arguments: /usr/lib/llvm/19/bin/clang++ -DCRL_FORCE_COMMON_QUEUE -DDESKTOP_APP_DISABLE_X11_INTEGRATION -DGI_INLINE -DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_56 -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_56 -DG_LOG_DOMAIN=\"WebView\" -DKCOREADDONS_EXPORT= -DQT_CORE_LIB -DQT_DBUS_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x051500 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_CAST_FROM_BYTEARRAY -DQT_NO_DEBUG -DQT_NO_KEYWORDS -DQT_OPENGLWIDGETS_LIB -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKWIDGETS_LIB -DQT_QUICK_LIB -DQT_SVG_LIB -DQT_WAYLANDCOMPOSITOR_LIB -DQT_WIDGETS_LIB -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_webview -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_ui -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_ui/emoji_suggestions -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_base -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_rpl -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_crl -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_ui/gen -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib64/qt6/mkspecs/linux-clang -isystem /usr/include/qt6/QtCore/6.8.2 -isystem /usr/include/qt6/QtCore/6.8.2/QtCore -isystem /usr/include/qt6/QtGui -isystem /usr/include/qt6/QtGui/6.8.2 -isystem /usr/include/qt6/QtGui/6.8.2/QtGui -isystem /usr/include/qt6/QtOpenGL -isystem /usr/include/qt6/QtWidgets -isystem /usr/include/qt6/QtWidgets/6.8.2 -isystem /usr/include/qt6/QtWidgets/6.8.2/QtWidgets -isystem /usr/include/qt6/QtOpenGLWidgets -isystem /usr/include/qt6/QtNetwork -isystem /usr/include/qt6/QtSvg -isystem /usr/include/qt6/QtDBus -isystem /usr/include/qt6/QtQuick -isystem /usr/include/qt6/QtQml -isystem /usr/include/qt6/QtQmlIntegration -isystem /usr/include/qt6/QtQmlMeta -isystem /usr/include/qt6/QtQmlModels -isystem /usr/include/qt6/QtQmlWorkerScript -isystem /usr/include/qt6/QtQuickWidgets -isystem /usr/include/qt6/QtWaylandCompositor -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/expected/include -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/kcoreaddons/src/lib/io -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/kcoreaddons/src/lib/util -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/cmake/external/kcoreaddons/headers/public -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_base/gen -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/gen -isystem /usr/include/cppgir -isystem /usr/include/cppgir/gi -isystem /usr/include/cppgir/override -isystem /usr/include/gio-unix-2.0 -isystem /usr/lib64/libffi/include -isystem /usr/include/glib-2.0 -isystem /usr/lib64/glib-2.0/include -isystem /usr/include/libmount -isystem /usr/include/uuid -isystem /usr/include/json-c -isystem /usr/include/blkid -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_webview/gen -DNDEBUG -march=native -O3 -pipe -Werror=odr -Werror=strict-aliasing -flto=thin -std=gnu++20 -fPIC -fno-strict-aliasing -pipe -Wall -Wextra -Wno-unused-parameter -Wno-switch -Wno-missing-field-initializers -Wno-sign-compare -Wno-deprecated -pthread -MD -MT Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o -MF Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o.d -o Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o -c /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_webview/webview/platform/linux/webview_linux_webkitgtk.cpp
1.      <unknown> parser at unknown location
 #0 0x00007f72785f18da llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x37f18da)
 #1 0x00007f72785ee8d4 llvm::sys::RunSignalHandlers() (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x37ee8d4)
 #2 0x00007f72784fc734 (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x36fc734)
 #3 0x00007f7274c5c230 (/usr/lib64/libc.so.6+0x45230)
 #4 0x00007f727dfd3d10 clang::Lexer::SkipBlockComment(clang::Token&, char const*, bool&) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0xfd3d10)
 #5 0x00007f727dfd8275 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0xfd8275)
 #6 0x00007f727e05c9bd clang::Preprocessor::Lex(clang::Token&) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x105c9bd)
 #7 0x00007f727e06f4d2 (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x106f4d2)
 #8 0x00007f727e084d5a clang::Parser::SkipMalformedDecl() (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x1084d5a)
 #9 0x00007f727e083443 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x1083443)
#10 0x00007f727e17e58d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117e58d)
#11 0x00007f727e17def1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117def1)
#12 0x00007f727e17cd24 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117cd24)
#13 0x00007f727e17a8a6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117a8a6)
#14 0x00007f727e0669ae clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x10669ae)
#15 0x00007f7280817898 clang::FrontendAction::Execute() (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3817898)
#16 0x00007f7280765c34 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3765c34)
#17 0x00007f72808959bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x38959bb)
#18 0x000055d1fe8980c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm/19/bin/clang+++0x140c4)
#19 0x000055d1fe89462c (/usr/lib/llvm/19/bin/clang+++0x1062c)
#20 0x00007f728034beed (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x334beed)
#21 0x00007f72784fc430 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x36fc430)
#22 0x00007f728034b565 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x334b565)
#23 0x00007f72803033af clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x33033af)
#24 0x00007f728030373e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x330373e)
#25 0x00007f7280327b9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3327b9c)
#26 0x000055d1fe89360a clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm/19/bin/clang+++0xf60a)
#27 0x000055d1fe8a4f88 main (/usr/lib/llvm/19/bin/clang+++0x20f88)
#28 0x00007f7274c41df9 (/usr/lib64/libc.so.6+0x2adf9)
#29 0x00007f7274c41eb5 __libc_start_main (/usr/lib64/libc.so.6+0x2aeb5)
#30 0x000055d1fe8918c1 _start (/usr/lib/llvm/19/bin/clang+++0xd8c1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7+libcxx
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm/19/bin
Configuration file: /etc/clang/x86_64-pc-linux-gnu-clang++.cfg
clang++: note: diagnostic msg:
```

[webview_linux_webkitgtk-00af6b.tar.gz](https://github.com/user-attachments/files/19320977/webview_linux_webkitgtk-00af6b.tar.gz)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Michael Egger (gcarq)

<details>
When compiling net-im/telegram-desktop-5.12.3 on Gentoo with a systemwide clang/libcxx profile, clang++ fails with exit code 139.

```
Stack dump:
0.      Program arguments: /usr/lib/llvm/19/bin/clang++ -DCRL_FORCE_COMMON_QUEUE -DDESKTOP_APP_DISABLE_X11_INTEGRATION -DGI_INLINE -DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_56 -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_56 -DG_LOG_DOMAIN=\"WebView\" -DKCOREADDONS_EXPORT= -DQT_CORE_LIB -DQT_DBUS_LIB -DQT_DEPRECATED_WARNINGS_SINCE=0x051500 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_NO_CAST_FROM_BYTEARRAY -DQT_NO_DEBUG -DQT_NO_KEYWORDS -DQT_OPENGLWIDGETS_LIB -DQT_OPENGL_LIB -DQT_QMLINTEGRATION_LIB -DQT_QMLMETA_LIB -DQT_QMLMODELS_LIB -DQT_QMLWORKERSCRIPT_LIB -DQT_QML_LIB -DQT_QUICKWIDGETS_LIB -DQT_QUICK_LIB -DQT_SVG_LIB -DQT_WAYLANDCOMPOSITOR_LIB -DQT_WIDGETS_LIB -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_webview -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_ui -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_ui/emoji_suggestions -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_base -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_rpl -I/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_crl -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_ui/gen -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6 -isystem /usr/lib64/qt6/mkspecs/linux-clang -isystem /usr/include/qt6/QtCore/6.8.2 -isystem /usr/include/qt6/QtCore/6.8.2/QtCore -isystem /usr/include/qt6/QtGui -isystem /usr/include/qt6/QtGui/6.8.2 -isystem /usr/include/qt6/QtGui/6.8.2/QtGui -isystem /usr/include/qt6/QtOpenGL -isystem /usr/include/qt6/QtWidgets -isystem /usr/include/qt6/QtWidgets/6.8.2 -isystem /usr/include/qt6/QtWidgets/6.8.2/QtWidgets -isystem /usr/include/qt6/QtOpenGLWidgets -isystem /usr/include/qt6/QtNetwork -isystem /usr/include/qt6/QtSvg -isystem /usr/include/qt6/QtDBus -isystem /usr/include/qt6/QtQuick -isystem /usr/include/qt6/QtQml -isystem /usr/include/qt6/QtQmlIntegration -isystem /usr/include/qt6/QtQmlMeta -isystem /usr/include/qt6/QtQmlModels -isystem /usr/include/qt6/QtQmlWorkerScript -isystem /usr/include/qt6/QtQuickWidgets -isystem /usr/include/qt6/QtWaylandCompositor -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/expected/include -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/kcoreaddons/src/lib/io -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/kcoreaddons/src/lib/util -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/cmake/external/kcoreaddons/headers/public -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_base/gen -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/gen -isystem /usr/include/cppgir -isystem /usr/include/cppgir/gi -isystem /usr/include/cppgir/override -isystem /usr/include/gio-unix-2.0 -isystem /usr/lib64/libffi/include -isystem /usr/include/glib-2.0 -isystem /usr/lib64/glib-2.0/include -isystem /usr/include/libmount -isystem /usr/include/uuid -isystem /usr/include/json-c -isystem /usr/include/blkid -isystem /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full_build/Telegram/lib_webview/gen -DNDEBUG -march=native -O3 -pipe -Werror=odr -Werror=strict-aliasing -flto=thin -std=gnu++20 -fPIC -fno-strict-aliasing -pipe -Wall -Wextra -Wno-unused-parameter -Wno-switch -Wno-missing-field-initializers -Wno-sign-compare -Wno-deprecated -pthread -MD -MT Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o -MF Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o.d -o Telegram/lib_webview/CMakeFiles/lib_webview.dir/webview/platform/linux/webview_linux_webkitgtk.cpp.o -c /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/lib_webview/webview/platform/linux/webview_linux_webkitgtk.cpp
1.      &lt;unknown&gt; parser at unknown location
 #<!-- -->0 0x00007f72785f18da llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x37f18da)
 #<!-- -->1 0x00007f72785ee8d4 llvm::sys::RunSignalHandlers() (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x37ee8d4)
 #<!-- -->2 0x00007f72784fc734 (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x36fc734)
 #<!-- -->3 0x00007f7274c5c230 (/usr/lib64/libc.so.6+0x45230)
 #<!-- -->4 0x00007f727dfd3d10 clang::Lexer::SkipBlockComment(clang::Token&amp;, char const*, bool&amp;) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0xfd3d10)
 #<!-- -->5 0x00007f727dfd8275 clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0xfd8275)
 #<!-- -->6 0x00007f727e05c9bd clang::Preprocessor::Lex(clang::Token&amp;) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x105c9bd)
 #<!-- -->7 0x00007f727e06f4d2 (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x106f4d2)
 #<!-- -->8 0x00007f727e084d5a clang::Parser::SkipMalformedDecl() (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x1084d5a)
 #<!-- -->9 0x00007f727e083443 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x1083443)
#<!-- -->10 0x00007f727e17e58d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117e58d)
#<!-- -->11 0x00007f727e17def1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117def1)
#<!-- -->12 0x00007f727e17cd24 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117cd24)
#<!-- -->13 0x00007f727e17a8a6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x117a8a6)
#<!-- -->14 0x00007f727e0669ae clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x10669ae)
#<!-- -->15 0x00007f7280817898 clang::FrontendAction::Execute() (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3817898)
#<!-- -->16 0x00007f7280765c34 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3765c34)
#<!-- -->17 0x00007f72808959bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x38959bb)
#<!-- -->18 0x000055d1fe8980c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm/19/bin/clang+++0x140c4)
#<!-- -->19 0x000055d1fe89462c (/usr/lib/llvm/19/bin/clang+++0x1062c)
#<!-- -->20 0x00007f728034beed (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x334beed)
#<!-- -->21 0x00007f72784fc430 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm/19/bin/../lib64/libLLVM.so.19.1+libcxx+0x36fc430)
#<!-- -->22 0x00007f728034b565 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x334b565)
#<!-- -->23 0x00007f72803033af clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x33033af)
#<!-- -->24 0x00007f728030373e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x330373e)
#<!-- -->25 0x00007f7280327b9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm/19/bin/../lib64/libclang-cpp.so.19.1+libcxx+0x3327b9c)
#<!-- -->26 0x000055d1fe89360a clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm/19/bin/clang+++0xf60a)
#<!-- -->27 0x000055d1fe8a4f88 main (/usr/lib/llvm/19/bin/clang+++0x20f88)
#<!-- -->28 0x00007f7274c41df9 (/usr/lib64/libc.so.6+0x2adf9)
#<!-- -->29 0x00007f7274c41eb5 __libc_start_main (/usr/lib64/libc.so.6+0x2aeb5)
#<!-- -->30 0x000055d1fe8918c1 _start (/usr/lib/llvm/19/bin/clang+++0xd8c1)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7+libcxx
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm/19/bin
Configuration file: /etc/clang/x86_64-pc-linux-gnu-clang++.cfg
clang++: note: diagnostic msg:
```

[webview_linux_webkitgtk-00af6b.tar.gz](https://github.com/user-attachments/files/19320977/webview_linux_webkitgtk-00af6b.tar.gz)
</details>


---

### Comment 3 - gcarq

@EugeneZelenko with clang-20.1.0 the crash is gone, but it won't compile either due to https://github.com/llvm/llvm-project/issues/126231

```
In file included from /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/Manager.cpp:1:
In file included from /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/Manager.h:5:
In file included from /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/EncryptedConnection.h:5:
In file included from /tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/Message.h:4:
/usr/include/tg_owt/api/candidate.h:108:40: error: 'lifetimebound' attribute cannot be applied to a parameter of a function that returns void; did you mean 'lifetime_capture_by(X)'
  108 |   void set_type(absl::string_view type ABSL_ATTRIBUTE_LIFETIME_BOUND) {
      |                                        ^
/usr/include/absl/base/attributes.h:821:41: note: expanded from macro 'ABSL_ATTRIBUTE_LIFETIME_BOUND'
  821 | #define ABSL_ATTRIBUTE_LIFETIME_BOUND [[clang::lifetimebound]]
      |                                         ^
/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/Manager.cpp:270:24: warning: variable 'videoFormats' set but not used [-Wunused-but-set-variable]
  270 |         } else if (const auto videoFormats = absl::get_if<VideoFormatsMessage>(data)) {
      |                               ^
/tmp/portage/net-im/telegram-desktop-5.12.3/work/tdesktop-5.12.3-full/Telegram/ThirdParty/tgcalls/tgcalls/Manager.cpp:266:17: warning: variable 'candidatesList' set but not used [-Wunused-but-set-variable]
  266 |         if (const auto candidatesList = absl::get_if<CandidatesListMessage>(data)) {
      |                        ^
2 warnings and 1 error generated.
```

---

