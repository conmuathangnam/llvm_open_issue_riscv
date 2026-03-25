# Clang 20.1.0 (assertions)

**Author:** Dong-hui-li
**URL:** https://github.com/llvm/llvm-project/issues/166721
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash
**Closed Date:** 2025-11-11T07:12:21Z

## Body

code:

```c++
#include <string>
#include <memory>

namespace DataSecurity {
    template<typename CryptoPolicy = int>
    class SecureContainer : public std::string {
    public:
        using std::string::string;
        
        size_t calculateIntegrity() const { 
            return this->size() * 0x9E3779B9; 
        }
        
        bool validateContent() const {
            return !this->empty();
        }
    };
    
    class DataHandler {
    public:
        template<typename InputType>
        static auto processInput(InputType&& source) {
            return SecureContainer(std::forward<InputType>(source));
        }
    };
    
    class ConfigurationManager {
    private:
        SecureContainer<> config_data;
        
    public:
        ConfigurationManager(const std::string& settings) 
            : config_data(DataHandler::processInput(settings)) {}
            
        size_t getSettingsHash() const {
            return config_data.calculateIntegrity();
        }
    };
}

class Application {
    DataSecurity::ConfigurationManager config;
    
public:
    Application() : config("default_settings") {}
    
    void initialize() {
        auto settings_value = config.getSettingsHash();
        if (settings_value > 0) {
            std::string data_sample = "runtime_data";
            auto processed = DataSecurity::DataHandler::processInput(data_sample);
        }
    }
};

int main() {
    Application app;
    app.initialize();
    
    std::string user_data = "user_provided_content";
    auto secured_data = DataSecurity::DataHandler::processInput(user_data);
    
    return 0;
}
```


-std=c++23
Clang 19: Works correctly (as you confirmed)
Clang 18 & 20.1.0: Contains the template deduction guide bug
This suggests the bug was introduced in version 18 and potentially fixed in a later version than 20.1.0

```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp:396: clang::NamedDecl* {anonymous}::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*): Assertion `getDepthAndIndex(NewParam).first == 0 && "Unexpected template parameter depth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:33:61: current parser token ')'
2.	<source>:4:1: parsing namespace 'DataSecurity'
3.	<source>:27:5: parsing struct/union/class body 'DataSecurity::ConfigurationManager'
4.	<source>:22:21: instantiating function definition 'DataSecurity::DataHandler::processInput<const std::basic_string<char> &>'
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000786821242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007868212969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000786821242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007868212287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078682122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000786821239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007363643 (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*) SemaTemplateDeductionGuide.cpp:0:0
#10 0x0000000007364794 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7364794)
#11 0x0000000006f67457 LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#12 0x0000000006f52b12 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6f52b12)
#13 0x0000000006f3170e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6f3170e)
#14 0x0000000006e24365 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6e24365)
#15 0x00000000073c8b5d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000073a846c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000073ab6f5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000073aba9c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000073d6cbe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
```

## Comments

### Comment 1 - zwuis

Could you please try 21 or `main` branch? Only most recent release is maintained.

---

### Comment 2 - frederick-vs-ja

This started to crash since [Clang 5](https://godbolt.org/z/vYsMfYo38), so this is probably not a regression. The bug was [fixed in Clang 19](https://godbolt.org/z/98rvhKzhn).

It seems that your code isn't valid because inheriting the constructor of `std::string` doesn't provide converting constructors from `const std::string&`/`std::string&&` to `SecureContainer<T>`.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Dong-hui-li)

<details>
code:

```c++
#include &lt;string&gt;
#include &lt;memory&gt;

namespace DataSecurity {
    template&lt;typename CryptoPolicy = int&gt;
    class SecureContainer : public std::string {
    public:
        using std::string::string;
        
        size_t calculateIntegrity() const { 
            return this-&gt;size() * 0x9E3779B9; 
        }
        
        bool validateContent() const {
            return !this-&gt;empty();
        }
    };
    
    class DataHandler {
    public:
        template&lt;typename InputType&gt;
        static auto processInput(InputType&amp;&amp; source) {
            return SecureContainer(std::forward&lt;InputType&gt;(source));
        }
    };
    
    class ConfigurationManager {
    private:
        SecureContainer&lt;&gt; config_data;
        
    public:
        ConfigurationManager(const std::string&amp; settings) 
            : config_data(DataHandler::processInput(settings)) {}
            
        size_t getSettingsHash() const {
            return config_data.calculateIntegrity();
        }
    };
}

class Application {
    DataSecurity::ConfigurationManager config;
    
public:
    Application() : config("default_settings") {}
    
    void initialize() {
        auto settings_value = config.getSettingsHash();
        if (settings_value &gt; 0) {
            std::string data_sample = "runtime_data";
            auto processed = DataSecurity::DataHandler::processInput(data_sample);
        }
    }
};

int main() {
    Application app;
    app.initialize();
    
    std::string user_data = "user_provided_content";
    auto secured_data = DataSecurity::DataHandler::processInput(user_data);
    
    return 0;
}
```


-std=c++23
Clang 19: Works correctly (as you confirmed)
Clang 18 &amp; 20.1.0: Contains the template deduction guide bug
This suggests the bug was introduced in version 18 and potentially fixed in a later version than 20.1.0

```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp:396: clang::NamedDecl* {anonymous}::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*): Assertion `getDepthAndIndex(NewParam).first == 0 &amp;&amp; "Unexpected template parameter depth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:33:61: current parser token ')'
2.	&lt;source&gt;:4:1: parsing namespace 'DataSecurity'
3.	&lt;source&gt;:27:5: parsing struct/union/class body 'DataSecurity::ConfigurationManager'
4.	&lt;source&gt;:22:21: instantiating function definition 'DataSecurity::DataHandler::processInput&lt;const std::basic_string&lt;char&gt; &amp;&gt;'
 #<!-- -->0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #<!-- -->1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #<!-- -->2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000786821242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007868212969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000786821242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007868212287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078682122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000786821239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007363643 (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->10 0x0000000007364794 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7364794)
#<!-- -->11 0x0000000006f67457 LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->12 0x0000000006f52b12 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6f52b12)
#<!-- -->13 0x0000000006f3170e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6f3170e)
#<!-- -->14 0x0000000006e24365 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6e24365)
#<!-- -->15 0x00000000073c8b5d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000073a846c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000073ab6f5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000073aba9c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000073d6cbe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
```
</details>


---

