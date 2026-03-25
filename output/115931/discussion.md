# [clang]  `Assertion ((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) && "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"` failed

**Author:** aperezdc
**URL:** https://github.com/llvm/llvm-project/issues/115931
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash-on-valid, lambda
**Closed Date:** 2024-12-03T02:10:19Z

## Body

This was found building WebKitGTK at commit https://github.com/WebKit/WebKit/commit/d9758a8fd190860ac6c4a468dfdea906786530e5

Clang was built using [this PKGBUILD for Arch Linux](https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=clang-prefixed-release).

Stack dump:

```
clang++: /usr/src/debug/clang-prefixed-release/llvm-project-llvmorg-19.1.2/clang/lib/Sema/SemaLambda.cpp:195: std::optional<unsigned int> clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(ArrayRef<const sema::FunctionScopeInfo *>, ValueDecl *, Sema &): Assertion `((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) && "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/clang/latest/bin/clang++ -Wextra -Wall -Werror=undefined-internal -Werror=undefined-inline -pipe -Wno-noexcept-type -Wno-psabi -Wno-misleading-indentation -Wno-parentheses-equality -Qunused-arguments -Wundef -Wpointer-arith -Wmissing-format-attribute -Wformat-security -Wcast-align -Wno-tautological-compare -fasynchronous-unwind-tables -fdebug-types-section -fno-strict-aliasing -fno-exceptions -fno-rtti -fcoroutines -ffunction-sections -fdata-sections -O3 -std=c++23 -fPIC -fvisibility=hidden -pthread -mfpmath=sse -msse -msse2 -fdiagnostics-color=always -fcolor-diagnostics -DBUILDING_GTK__=1 -DBUILDING_WEBKIT=1 -DBUILDING_WITH_CMAKE=1 -DBUILDING_WebCore -DBWRAP_EXECUTABLE=\"/usr/bin/bwrap\" -DDBUS_PROXY_EXECUTABLE=\"/usr/bin/xdg-dbus-proxy\" -DGETTEXT_PACKAGE=\"WebKitGTK-6.0\" -DHAVE_CONFIG_H=1 -DJSC_GLIB_API_ENABLED -DPAS_BMALLOC=1 -DSK_ASSUME_GL=0 -DSK_ASSUME_GL_ES=1 -DSK_ASSUME_WEBGL=0 -DSK_DISABLE_LEGACY_GL_MAKE_NATIVE_INTERFACE -DSK_DISABLE_LEGACY_IMAGE_READBUFFER -DSK_DISABLE_LEGACY_INIT_DECODERS -DSK_DISABLE_LEGACY_PNG_WRITEBUFFER -DSK_DISABLE_TRACING -DSK_DISABLE_WEBGL_INTERFACE -DSK_ENABLE_PRECOMPILE -DSK_R32_SHIFT=16 -DSK_RELEASE -DSK_TRIVIAL_ABI=[[clang::trivial_abi]] -DSTATICALLY_LINKED_WITH_PAL -DUSE_SYSTEM_EGL -I. -IWebCore/DerivedSources -I../Source/WebCore -I../Source/WebCore/Modules/ShapeDetection -I../Source/WebCore/Modules/ShapeDetection/Interfaces -I../Source/WebCore/Modules/WebGPU -I../Source/WebCore/Modules/WebGPU/InternalAPI -I../Source/WebCore/Modules/WebGPU/Implementation -I../Source/WebCore/Modules/airplay -I../Source/WebCore/Modules/applepay -I../Source/WebCore/Modules/applepay/paymentrequest -I../Source/WebCore/Modules/applicationmanifest -I../Source/WebCore/Modules/async-clipboard -I../Source/WebCore/Modules/audiosession -I../Source/WebCore/Modules/badge -I../Source/WebCore/Modules/beacon -I../Source/WebCore/Modules/cache -I../Source/WebCore/Modules/compression -I../Source/WebCore/Modules/contact-picker -I../Source/WebCore/Modules/cookie-consent -I../Source/WebCore/Modules/cookie-store -I../Source/WebCore/Modules/credentialmanagement -I../Source/WebCore/Modules/encryptedmedia -I../Source/WebCore/Modules/encryptedmedia/legacy -I../Source/WebCore/Modules/entriesapi -I../Source/WebCore/Modules/fetch -I../Source/WebCore/Modules/filesystemaccess -I../Source/WebCore/Modules/geolocation -I../Source/WebCore/Modules/highlight -I../Source/WebCore/Modules/identity -I../Source/WebCore/Modules/indexeddb -I../Source/WebCore/Modules/indexeddb/client -I../Source/WebCore/Modules/indexeddb/server -I../Source/WebCore/Modules/indexeddb/shared -I../Source/WebCore/Modules/mediacapabilities -I../Source/WebCore/Modules/mediacontrols -I../Source/WebCore/Modules/mediarecorder -I../Source/WebCore/Modules/mediasession -I../Source/WebCore/Modules/mediasource -I../Source/WebCore/Modules/mediastream -I../Source/WebCore/Modules/model-element -I../Source/WebCore/Modules/model-element/dummy -I../Source/WebCore/Modules/navigatorcontentutils -I../Source/WebCore/Modules/notifications -I../Source/WebCore/Modules/paymentrequest -I../Source/WebCore/Modules/permissions -I../Source/WebCore/Modules/pictureinpicture -I../Source/WebCore/Modules/plugins -I../Source/WebCore/Modules/push-api -I../Source/WebCore/Modules/remoteplayback -I../Source/WebCore/Modules/reporting -I../Source/WebCore/Modules/screen-wake-lock -I../Source/WebCore/Modules/speech -I../Source/WebCore/Modules/storage -I../Source/WebCore/Modules/streams -I../Source/WebCore/Modules/url-pattern -I../Source/WebCore/Modules/web-locks -I../Source/WebCore/Modules/webaudio -I../Source/WebCore/Modules/webauthn -I../Source/WebCore/Modules/webauthn/cbor -I../Source/WebCore/Modules/webauthn/fido -I../Source/WebCore/Modules/webcodecs -I../Source/WebCore/Modules/webdatabase -I../Source/WebCore/Modules/webdriver -I../Source/WebCore/Modules/websockets -I../Source/WebCore/Modules/webtransport -I../Source/WebCore/Modules/webxr -I../Source/WebCore/accessibility -I../Source/WebCore/accessibility/isolatedtree -I../Source/WebCore/animation -I../Source/WebCore/bin
1.      <eof> parser at end of file
2.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:112:33: instantiating function definition 'WebCore::Style::reflectSum<Range{-INF, INF}, Range{0.000000e+00, INF}>'
3.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:679:44: instantiating function definition 'WebCore::Style::LengthPercentageValue<Range{0.000000e+00, INF}>::switchOn<(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)>'
4.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:666:41: instantiating function definition 'WebCore::Style::LengthPercentageValue<Range{0.000000e+00, INF}>::visit<WTF::Visitor<(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)>>'
5.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:3143:12: instantiating class definition 'std::invoke_result<WTF::Visitor<(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)>, WebCore::Style::Length<Range{0.000000e+00, INF}>>'
6.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2656:12: instantiating class definition 'std::__invoke_result<WTF::Visitor<(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)>, WebCore::Style::Length<Range{0.000000e+00, INF}>>'
7.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2618:12: instantiating class definition 'std::__result_of_impl<false, false, WTF::Visitor<(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)>, WebCore::Style::Length<Range{0.000000e+00, INF}>>'
 #0 0x00007fa491b8a67d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x458a67d)
 #1 0x00007fa491b89469 llvm::sys::RunSignalHandlers() (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x4589469)
 #2 0x00007fa491aac134 (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x44ac134)
 #3 0x00007fa48d35d1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #4 0x00007fa48d3b63f4 (/usr/lib/libc.so.6+0x963f4)
 #5 0x00007fa48d35d120 raise (/usr/lib/libc.so.6+0x3d120)
 #6 0x00007fa48d3444c3 abort (/usr/lib/libc.so.6+0x244c3)
 #7 0x00007fa48d3443df (/usr/lib/libc.so.6+0x243df)
 #8 0x00007fa48d355177 (/usr/lib/libc.so.6+0x35177)
 #9 0x00007fa49954fd4f clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef<clang::sema::FunctionScopeInfo const*>, clang::ValueDecl*, clang::Sema&) (/opt/clang/19.1.2/bin/../lib/libclang-cpp.so.19.1+0x274fd4f)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 19.1.2
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/clang/19.1.2/bin
Build config: +assertions
```

I have zipped the source and run script because the source is huge and GitHub wouldn't take it otherwise: [source-and-runscript.zip](https://github.com/user-attachments/files/17721641/source-and-runscript.zip)


## Comments

### Comment 1 - aperezdc

For reference, the WebKit build was configured with:

```sh
CC=clang CXX=clang++ \
PATH="/opt/clang/latest/bin:$PATH" \
  cmake -DPORT=GTK -DCMAKE_INSTALL_PREFIX=$HOME/.prefix/webkitgtk4 \
   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release \
   -DENABLE_SPEECH_SYNTHESIS=OFF -GNinja -Bbuild-gtk4
```

---

### Comment 2 - asb

@aperezdc and I had a bit more of a look at that, this is the reduced test case we come up (using cvise and clang from the release/19.x and an interestingness script that is seeking the "The capture ready lambda for a potential capture can only be" error message. Because of this, other aspects of the input may of course have been broken along the way.

This produces the following minimised test case:

```c++
constexpr int value;
template <typename _Tp> _Up __declval;
                                  template < typename _Tp > auto declval  (__declval< _Tp >(0);
                                  template < typename > struct __result_of_success;
                                  template < bool, bool, typename... > struct __result_of_impl;
                                  template < typename _Fn> __result_of_success< decltype(declval< _Fn >) > _S_test;
                                  template < typename _Functor, typename... _ArgTypes > struct __result_of_impl< false, false, _Functor, _ArgTypes... > {
  using type = decltype(_S_test<_Functor>);                             }
                                  template < typename _Functor > struct __invoke_result     : __result_of_impl< value, value, _Functor                          >        ;
                                  template < typename _Tp > _Tp forward();
                                           template < typename _Callable>   invoke_result_t< _Callable  > invoke(       _Callable ;
                                           template < class A > struct Visitor   : A    struct Range {       }
                          All;
                            template < typename T > struct Point {                          T y   };
                            template < typename T > struct Size {                          T height0   };
                            template < Range = All > struct LengthPercentageValue {                          template < typename F > auto visit(F) {                   invoke(forward< F >());
                                  }
                                  template <typename... F> auto switchOn(F...) {
                                    Visitor<F...> __trans_tmp_1;
                                    visit(__trans_tmp_1)
                                  }
                                  }
                                  ;
                                  template <Range = All>
                                  struct LengthPercentage {
                                    LengthPercentageValue<> value;
                                  } template <auto aR, auto bR>
                                  auto reflectSum(LengthPercentage<aR> a,
                                                  LengthPercentage<bR>) {
                                    auto resultR(bR);
                                    a.value.switchOn(
                                        [&](auto) -> LengthPercentage<resultR> {
                                        });
                                  }
                                  struct Xywh {
                                    using Location = Point<LengthPercentage<>>;
                                    using Size = Size<LengthPercentage<>>;
                                    Location location;
                                    Size size;
                                    namespace;
                                    Xywh operator0_value auto operator0() {
                                      auto location(
                                          operator0_value.location) auto
                                          size(operator0_value.size);
                                      reflectSum(location.y, size.height)
```

I can confirm that the assert is hit in a current (or at least, as of this morning) HEAD clang.

I'll set off another reduction over night with an interestingness test that rejects inputs that produce additional errors.

---

### Comment 3 - shafik

Based on the last snippet, it looks like it started in clang-18: https://godbolt.org/z/Mozff8nGj

So if we think there is another clang-19 specific regression then we need more of a reduction.

---

### Comment 4 - shafik

@cor3ntin the first possible not minimal reproducer regression looks like is caused by 3ed9e9e3ace6f9ce320cf4e75cffa04a7c7241b5

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Adrian Perez (aperezdc)

<details>
This was found building WebKitGTK at commit https://github.com/WebKit/WebKit/commit/d9758a8fd190860ac6c4a468dfdea906786530e5

Clang was built using [this PKGBUILD for Arch Linux](https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=clang-prefixed-release).

Stack dump:

```
clang++: /usr/src/debug/clang-prefixed-release/llvm-project-llvmorg-19.1.2/clang/lib/Sema/SemaLambda.cpp:195: std::optional&lt;unsigned int&gt; clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(ArrayRef&lt;const sema::FunctionScopeInfo *&gt;, ValueDecl *, Sema &amp;): Assertion `((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) &amp;&amp; "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/clang/latest/bin/clang++ -Wextra -Wall -Werror=undefined-internal -Werror=undefined-inline -pipe -Wno-noexcept-type -Wno-psabi -Wno-misleading-indentation -Wno-parentheses-equality -Qunused-arguments -Wundef -Wpointer-arith -Wmissing-format-attribute -Wformat-security -Wcast-align -Wno-tautological-compare -fasynchronous-unwind-tables -fdebug-types-section -fno-strict-aliasing -fno-exceptions -fno-rtti -fcoroutines -ffunction-sections -fdata-sections -O3 -std=c++23 -fPIC -fvisibility=hidden -pthread -mfpmath=sse -msse -msse2 -fdiagnostics-color=always -fcolor-diagnostics -DBUILDING_GTK__=1 -DBUILDING_WEBKIT=1 -DBUILDING_WITH_CMAKE=1 -DBUILDING_WebCore -DBWRAP_EXECUTABLE=\"/usr/bin/bwrap\" -DDBUS_PROXY_EXECUTABLE=\"/usr/bin/xdg-dbus-proxy\" -DGETTEXT_PACKAGE=\"WebKitGTK-6.0\" -DHAVE_CONFIG_H=1 -DJSC_GLIB_API_ENABLED -DPAS_BMALLOC=1 -DSK_ASSUME_GL=0 -DSK_ASSUME_GL_ES=1 -DSK_ASSUME_WEBGL=0 -DSK_DISABLE_LEGACY_GL_MAKE_NATIVE_INTERFACE -DSK_DISABLE_LEGACY_IMAGE_READBUFFER -DSK_DISABLE_LEGACY_INIT_DECODERS -DSK_DISABLE_LEGACY_PNG_WRITEBUFFER -DSK_DISABLE_TRACING -DSK_DISABLE_WEBGL_INTERFACE -DSK_ENABLE_PRECOMPILE -DSK_R32_SHIFT=16 -DSK_RELEASE -DSK_TRIVIAL_ABI=[[clang::trivial_abi]] -DSTATICALLY_LINKED_WITH_PAL -DUSE_SYSTEM_EGL -I. -IWebCore/DerivedSources -I../Source/WebCore -I../Source/WebCore/Modules/ShapeDetection -I../Source/WebCore/Modules/ShapeDetection/Interfaces -I../Source/WebCore/Modules/WebGPU -I../Source/WebCore/Modules/WebGPU/InternalAPI -I../Source/WebCore/Modules/WebGPU/Implementation -I../Source/WebCore/Modules/airplay -I../Source/WebCore/Modules/applepay -I../Source/WebCore/Modules/applepay/paymentrequest -I../Source/WebCore/Modules/applicationmanifest -I../Source/WebCore/Modules/async-clipboard -I../Source/WebCore/Modules/audiosession -I../Source/WebCore/Modules/badge -I../Source/WebCore/Modules/beacon -I../Source/WebCore/Modules/cache -I../Source/WebCore/Modules/compression -I../Source/WebCore/Modules/contact-picker -I../Source/WebCore/Modules/cookie-consent -I../Source/WebCore/Modules/cookie-store -I../Source/WebCore/Modules/credentialmanagement -I../Source/WebCore/Modules/encryptedmedia -I../Source/WebCore/Modules/encryptedmedia/legacy -I../Source/WebCore/Modules/entriesapi -I../Source/WebCore/Modules/fetch -I../Source/WebCore/Modules/filesystemaccess -I../Source/WebCore/Modules/geolocation -I../Source/WebCore/Modules/highlight -I../Source/WebCore/Modules/identity -I../Source/WebCore/Modules/indexeddb -I../Source/WebCore/Modules/indexeddb/client -I../Source/WebCore/Modules/indexeddb/server -I../Source/WebCore/Modules/indexeddb/shared -I../Source/WebCore/Modules/mediacapabilities -I../Source/WebCore/Modules/mediacontrols -I../Source/WebCore/Modules/mediarecorder -I../Source/WebCore/Modules/mediasession -I../Source/WebCore/Modules/mediasource -I../Source/WebCore/Modules/mediastream -I../Source/WebCore/Modules/model-element -I../Source/WebCore/Modules/model-element/dummy -I../Source/WebCore/Modules/navigatorcontentutils -I../Source/WebCore/Modules/notifications -I../Source/WebCore/Modules/paymentrequest -I../Source/WebCore/Modules/permissions -I../Source/WebCore/Modules/pictureinpicture -I../Source/WebCore/Modules/plugins -I../Source/WebCore/Modules/push-api -I../Source/WebCore/Modules/remoteplayback -I../Source/WebCore/Modules/reporting -I../Source/WebCore/Modules/screen-wake-lock -I../Source/WebCore/Modules/speech -I../Source/WebCore/Modules/storage -I../Source/WebCore/Modules/streams -I../Source/WebCore/Modules/url-pattern -I../Source/WebCore/Modules/web-locks -I../Source/WebCore/Modules/webaudio -I../Source/WebCore/Modules/webauthn -I../Source/WebCore/Modules/webauthn/cbor -I../Source/WebCore/Modules/webauthn/fido -I../Source/WebCore/Modules/webcodecs -I../Source/WebCore/Modules/webdatabase -I../Source/WebCore/Modules/webdriver -I../Source/WebCore/Modules/websockets -I../Source/WebCore/Modules/webtransport -I../Source/WebCore/Modules/webxr -I../Source/WebCore/accessibility -I../Source/WebCore/accessibility/isolatedtree -I../Source/WebCore/animation -I../Source/WebCore/bin
1.      &lt;eof&gt; parser at end of file
2.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:112:33: instantiating function definition 'WebCore::Style::reflectSum&lt;Range{-INF, INF}, Range{0.000000e+00, INF}&gt;'
3.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:679:44: instantiating function definition 'WebCore::Style::LengthPercentageValue&lt;Range{0.000000e+00, INF}&gt;::switchOn&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;'
4.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:666:41: instantiating function definition 'WebCore::Style::LengthPercentageValue&lt;Range{0.000000e+00, INF}&gt;::visit&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;&gt;'
5.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:3143:12: instantiating class definition 'std::invoke_result&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
6.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2656:12: instantiating class definition 'std::__invoke_result&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
7.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2618:12: instantiating class definition 'std::__result_of_impl&lt;false, false, WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
 #<!-- -->0 0x00007fa491b8a67d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x458a67d)
 #<!-- -->1 0x00007fa491b89469 llvm::sys::RunSignalHandlers() (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x4589469)
 #<!-- -->2 0x00007fa491aac134 (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x44ac134)
 #<!-- -->3 0x00007fa48d35d1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #<!-- -->4 0x00007fa48d3b63f4 (/usr/lib/libc.so.6+0x963f4)
 #<!-- -->5 0x00007fa48d35d120 raise (/usr/lib/libc.so.6+0x3d120)
 #<!-- -->6 0x00007fa48d3444c3 abort (/usr/lib/libc.so.6+0x244c3)
 #<!-- -->7 0x00007fa48d3443df (/usr/lib/libc.so.6+0x243df)
 #<!-- -->8 0x00007fa48d355177 (/usr/lib/libc.so.6+0x35177)
 #<!-- -->9 0x00007fa49954fd4f clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef&lt;clang::sema::FunctionScopeInfo const*&gt;, clang::ValueDecl*, clang::Sema&amp;) (/opt/clang/19.1.2/bin/../lib/libclang-cpp.so.19.1+0x274fd4f)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 19.1.2
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/clang/19.1.2/bin
Build config: +assertions
```

I have zipped the source and run script because the source is huge and GitHub wouldn't take it otherwise: [source-and-runscript.zip](https://github.com/user-attachments/files/17721641/source-and-runscript.zip)

</details>


---

### Comment 6 - asb

Here's a better reproducer (using an interestingness test that discards transformations that introduce errors this time):

```c++
template <typename _Tp, typename _Up = _Tp> _Up __declval(int);
template <typename _Tp> auto declval() -> decltype(__declval<_Tp>(0));
template <typename _Tp> using decay_t = _Tp;
template <typename _Tp> _Tp forward();
template <class...> struct Visitor;
template <class A> struct Visitor<A> : A {};
template <class... F> Visitor<F...> makeVisitor(F...);
namespace CSS {
template <typename> struct Point {};
template <typename> struct Size {};
struct Range {
} All;
template <typename T>
concept RawNumeric = requires(T raw) { raw; };
template <Range> struct LengthPercentageRaw;
template <RawNumeric> struct PrimitiveNumeric;
template <Range R = All>
using LengthPercentage = PrimitiveNumeric<LengthPercentageRaw<R>>;
}
template <typename> struct ToStyle;
template <typename T> struct Point {
  T y();
};
template <typename T> struct Size {
  T height();
};
int toStyle_builderState, toStyle_symbolTable;
template <typename CSSType> auto toStyle(const CSSType &cssType) {
  return ToStyle<CSSType>{}(cssType, toStyle_builderState, toStyle_symbolTable);
}
template <typename CSSType>
using StyleType = decay_t<decltype(toStyle(declval<CSSType &>()))>;
template <typename CSSType> struct ToStyle<CSS::Point<CSSType>> {
  auto operator()(CSS::Point<CSSType>, int, int) {
    return Point<StyleType<CSSType>>{};
  }
};
template <typename CSSType> struct ToStyle<CSS::Size<CSSType>> {
  auto operator()(CSS::Size<CSSType>, int, int) {
    return Size<StyleType<CSSType>>{};
  }
};
template <CSS::Range = CSS::All> struct LengthPercentageValue {
  template <typename... F> auto switchOn(F...) {
    Visitor<F...> __trans_tmp_3 = makeVisitor(forward<F>()...)(__trans_tmp_3);
  }
};
template <CSS::Range = CSS::All> struct LengthPercentage {
  LengthPercentageValue<> value;
};
template <auto aR, auto bR>
auto reflectSum(LengthPercentage<aR>, LengthPercentage<bR> b) {
  auto resultR(bR);
  b.value.switchOn([&](auto) -> LengthPercentage<resultR> {});
}
namespace CSS {
struct Xywh {
  using Location = Point<LengthPercentage<>>;
  using Size = Size<LengthPercentage<>>;
  Location location;
  Size size;
};
}
template <> struct ToStyle<CSS::Xywh> {
  auto operator()(const CSS::Xywh &, const int &, const int &) -> int;
};
template <auto R> struct ToStyle<CSS::LengthPercentage<R>> {
  using From = CSS::LengthPercentage<>;
  using To = LengthPercentage<>;
  auto operator()(const From &, int, int) -> To;
};
auto ToStyle<CSS::Xywh>::operator()(const CSS::Xywh &value, const int &,
                                    const int &) -> int {
  auto location = toStyle(value.location);
  auto size = toStyle(value.size);
  reflectSum(location.y(), size.height())
```

---

### Comment 7 - Endilll

Thank you @asb for running cvise!
I took https://github.com/llvm/llvm-project/issues/115931#issuecomment-2478088369, and reduced it further (https://godbolt.org/z/eMExE46qj):
```cpp
struct Range {};

template <Range>
struct LengthPercentage {};

void reflectSum() {
  Range resultR;
  [&] (auto) -> LengthPercentage<resultR> { 
    return {};
  }(0);
}
```

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-c-20

Author: Adrian Perez (aperezdc)

<details>
This was found building WebKitGTK at commit https://github.com/WebKit/WebKit/commit/d9758a8fd190860ac6c4a468dfdea906786530e5

Clang was built using [this PKGBUILD for Arch Linux](https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=clang-prefixed-release).

Stack dump:

```
clang++: /usr/src/debug/clang-prefixed-release/llvm-project-llvmorg-19.1.2/clang/lib/Sema/SemaLambda.cpp:195: std::optional&lt;unsigned int&gt; clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(ArrayRef&lt;const sema::FunctionScopeInfo *&gt;, ValueDecl *, Sema &amp;): Assertion `((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) &amp;&amp; "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/clang/latest/bin/clang++ -Wextra -Wall -Werror=undefined-internal -Werror=undefined-inline -pipe -Wno-noexcept-type -Wno-psabi -Wno-misleading-indentation -Wno-parentheses-equality -Qunused-arguments -Wundef -Wpointer-arith -Wmissing-format-attribute -Wformat-security -Wcast-align -Wno-tautological-compare -fasynchronous-unwind-tables -fdebug-types-section -fno-strict-aliasing -fno-exceptions -fno-rtti -fcoroutines -ffunction-sections -fdata-sections -O3 -std=c++23 -fPIC -fvisibility=hidden -pthread -mfpmath=sse -msse -msse2 -fdiagnostics-color=always -fcolor-diagnostics -DBUILDING_GTK__=1 -DBUILDING_WEBKIT=1 -DBUILDING_WITH_CMAKE=1 -DBUILDING_WebCore -DBWRAP_EXECUTABLE=\"/usr/bin/bwrap\" -DDBUS_PROXY_EXECUTABLE=\"/usr/bin/xdg-dbus-proxy\" -DGETTEXT_PACKAGE=\"WebKitGTK-6.0\" -DHAVE_CONFIG_H=1 -DJSC_GLIB_API_ENABLED -DPAS_BMALLOC=1 -DSK_ASSUME_GL=0 -DSK_ASSUME_GL_ES=1 -DSK_ASSUME_WEBGL=0 -DSK_DISABLE_LEGACY_GL_MAKE_NATIVE_INTERFACE -DSK_DISABLE_LEGACY_IMAGE_READBUFFER -DSK_DISABLE_LEGACY_INIT_DECODERS -DSK_DISABLE_LEGACY_PNG_WRITEBUFFER -DSK_DISABLE_TRACING -DSK_DISABLE_WEBGL_INTERFACE -DSK_ENABLE_PRECOMPILE -DSK_R32_SHIFT=16 -DSK_RELEASE -DSK_TRIVIAL_ABI=[[clang::trivial_abi]] -DSTATICALLY_LINKED_WITH_PAL -DUSE_SYSTEM_EGL -I. -IWebCore/DerivedSources -I../Source/WebCore -I../Source/WebCore/Modules/ShapeDetection -I../Source/WebCore/Modules/ShapeDetection/Interfaces -I../Source/WebCore/Modules/WebGPU -I../Source/WebCore/Modules/WebGPU/InternalAPI -I../Source/WebCore/Modules/WebGPU/Implementation -I../Source/WebCore/Modules/airplay -I../Source/WebCore/Modules/applepay -I../Source/WebCore/Modules/applepay/paymentrequest -I../Source/WebCore/Modules/applicationmanifest -I../Source/WebCore/Modules/async-clipboard -I../Source/WebCore/Modules/audiosession -I../Source/WebCore/Modules/badge -I../Source/WebCore/Modules/beacon -I../Source/WebCore/Modules/cache -I../Source/WebCore/Modules/compression -I../Source/WebCore/Modules/contact-picker -I../Source/WebCore/Modules/cookie-consent -I../Source/WebCore/Modules/cookie-store -I../Source/WebCore/Modules/credentialmanagement -I../Source/WebCore/Modules/encryptedmedia -I../Source/WebCore/Modules/encryptedmedia/legacy -I../Source/WebCore/Modules/entriesapi -I../Source/WebCore/Modules/fetch -I../Source/WebCore/Modules/filesystemaccess -I../Source/WebCore/Modules/geolocation -I../Source/WebCore/Modules/highlight -I../Source/WebCore/Modules/identity -I../Source/WebCore/Modules/indexeddb -I../Source/WebCore/Modules/indexeddb/client -I../Source/WebCore/Modules/indexeddb/server -I../Source/WebCore/Modules/indexeddb/shared -I../Source/WebCore/Modules/mediacapabilities -I../Source/WebCore/Modules/mediacontrols -I../Source/WebCore/Modules/mediarecorder -I../Source/WebCore/Modules/mediasession -I../Source/WebCore/Modules/mediasource -I../Source/WebCore/Modules/mediastream -I../Source/WebCore/Modules/model-element -I../Source/WebCore/Modules/model-element/dummy -I../Source/WebCore/Modules/navigatorcontentutils -I../Source/WebCore/Modules/notifications -I../Source/WebCore/Modules/paymentrequest -I../Source/WebCore/Modules/permissions -I../Source/WebCore/Modules/pictureinpicture -I../Source/WebCore/Modules/plugins -I../Source/WebCore/Modules/push-api -I../Source/WebCore/Modules/remoteplayback -I../Source/WebCore/Modules/reporting -I../Source/WebCore/Modules/screen-wake-lock -I../Source/WebCore/Modules/speech -I../Source/WebCore/Modules/storage -I../Source/WebCore/Modules/streams -I../Source/WebCore/Modules/url-pattern -I../Source/WebCore/Modules/web-locks -I../Source/WebCore/Modules/webaudio -I../Source/WebCore/Modules/webauthn -I../Source/WebCore/Modules/webauthn/cbor -I../Source/WebCore/Modules/webauthn/fido -I../Source/WebCore/Modules/webcodecs -I../Source/WebCore/Modules/webdatabase -I../Source/WebCore/Modules/webdriver -I../Source/WebCore/Modules/websockets -I../Source/WebCore/Modules/webtransport -I../Source/WebCore/Modules/webxr -I../Source/WebCore/accessibility -I../Source/WebCore/accessibility/isolatedtree -I../Source/WebCore/animation -I../Source/WebCore/bin
1.      &lt;eof&gt; parser at end of file
2.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:112:33: instantiating function definition 'WebCore::Style::reflectSum&lt;Range{-INF, INF}, Range{0.000000e+00, INF}&gt;'
3.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:679:44: instantiating function definition 'WebCore::Style::LengthPercentageValue&lt;Range{0.000000e+00, INF}&gt;::switchOn&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;'
4.      ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes.h:666:41: instantiating function definition 'WebCore::Style::LengthPercentageValue&lt;Range{0.000000e+00, INF}&gt;::visit&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;&gt;'
5.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:3143:12: instantiating class definition 'std::invoke_result&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
6.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2656:12: instantiating class definition 'std::__invoke_result&lt;WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
7.      /usr/lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../include/c++/14.2.1/type_traits:2618:12: instantiating class definition 'std::__result_of_impl&lt;false, false, WTF::Visitor&lt;(lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:132:13), (lambda at ../Source/WebCore/style/values/primitives/StylePrimitiveNumericTypes+Evaluation.h:136:13)&gt;, WebCore::Style::Length&lt;Range{0.000000e+00, INF}&gt;&gt;'
 #<!-- -->0 0x00007fa491b8a67d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x458a67d)
 #<!-- -->1 0x00007fa491b89469 llvm::sys::RunSignalHandlers() (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x4589469)
 #<!-- -->2 0x00007fa491aac134 (/opt/clang/19.1.2/bin/../lib/libLLVM.so.19.1+0x44ac134)
 #<!-- -->3 0x00007fa48d35d1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #<!-- -->4 0x00007fa48d3b63f4 (/usr/lib/libc.so.6+0x963f4)
 #<!-- -->5 0x00007fa48d35d120 raise (/usr/lib/libc.so.6+0x3d120)
 #<!-- -->6 0x00007fa48d3444c3 abort (/usr/lib/libc.so.6+0x244c3)
 #<!-- -->7 0x00007fa48d3443df (/usr/lib/libc.so.6+0x243df)
 #<!-- -->8 0x00007fa48d355177 (/usr/lib/libc.so.6+0x35177)
 #<!-- -->9 0x00007fa49954fd4f clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef&lt;clang::sema::FunctionScopeInfo const*&gt;, clang::ValueDecl*, clang::Sema&amp;) (/opt/clang/19.1.2/bin/../lib/libclang-cpp.so.19.1+0x274fd4f)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 19.1.2
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/clang/19.1.2/bin
Build config: +assertions
```

I have zipped the source and run script because the source is huge and GitHub wouldn't take it otherwise: [source-and-runscript.zip](https://github.com/user-attachments/files/17721641/source-and-runscript.zip)

</details>


---

### Comment 9 - asb

> Thank you [@asb](https://github.com/asb) for running cvise! I took [#115931 (comment)](https://github.com/llvm/llvm-project/issues/115931#issuecomment-2478088369), and reduced it further (https://godbolt.org/z/eMExE46qj):
> 
> struct Range {};
> 
> template <Range>
> struct LengthPercentage {};
> 
> void reflectSum() {
>   Range resultR;
>   [&] (auto) -> LengthPercentage<resultR> { 
>     return {};
>   }(0);
> }

Out of interest - was that reduced manually or is there a tool assisted way to get a better minimised test case?

---

