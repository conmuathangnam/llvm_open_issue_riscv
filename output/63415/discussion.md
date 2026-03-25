# clang miscompilation with `std::optional`?

**Author:** shkoo
**URL:** https://github.com/llvm/llvm-project/issues/63415
**Status:** Closed
**Labels:** clang, c++17, miscompilation
**Closed Date:** 2025-04-26T07:16:16Z

## Body

I'm encountering a case where constructing a `std::optional` from `std::nullopt` behaves differently from using the default constructor, which I don't think should ever happen.  Best I can tell, using `std::nullopt` results in a poison value propagating that ends up causing a register to not be cleared, resulting in miscompilation?

Here's the version of Clang I'm using:
```console
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@ m64
Selected multilib: .;@ m64
Found CUDA installation: /usr/local/cuda, version 
```

Though the problem also looks like it occurs on compiler explorer for "clang trunk" with the "x86-64 gcc 10.3" toolchain.

I'm using the following options to generate the miscompile: `-std=c++17 -DMISCOMPILE -O2 --target=x86_64-pc-linux-gnu`.  Without the `-DMISCOMPILE` it compiles differently (and I believe correctly).

Compiler explorer shows that the `MISCOMPILE` version is missing a `xor %eax, %eax`:
Compiler Explorer link for Clang 15.0.0: https://godbolt.org/z/ded5q1f8W
Compiler Explorer link for Clang trunk: https://godbolt.org/z/rh87KMnje

I've used cvise to try to trim down the test case as much as I can:
```cpp
#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
namespace llvm {
template <typename To, typename, typename>
struct DefaultDoCastIfPossible {
    static To doCastIfPossible(int);
};
template <typename, typename, typename = void>
struct CastInfo;
template <typename To, typename From>
auto dyn_cast(From Val) {
    return CastInfo<To, From>::doCastIfPossible(Val);
}
template <typename>
class SmallVectorImpl;
}  // namespace llvm
using llvm::dyn_cast;
using llvm::SmallVectorImpl;
namespace llvm {
template <typename T, bool = std::is_class<T>::value>
struct function_traits : function_traits<decltype(&T::operator())> {};
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const, false> {
    template <size_t Index>
    using arg_t = std::tuple_element_t<Index, std::tuple<Args...>>;
};
}  // namespace llvm
namespace mlir {
struct LogicalResult {};
class Type {
   public:
    operator bool();
};
}  // namespace mlir
namespace llvm {
template <typename To, typename From>
struct CastInfo<
    To, From,
    std::enable_if_t<std::is_same_v<mlir::Type, std::remove_const_t<From>>>>
    : DefaultDoCastIfPossible<To, From, CastInfo<To, From>> {};
}  // namespace llvm
namespace mlir {
class ModuleOp;
class TypeConverter {
    using ConversionCallbackFn = std::function<std::optional<LogicalResult>(
        Type, SmallVectorImpl<Type> &, Type)>;

   public:
    void registerConversion(ConversionCallbackFn);
    template <typename FnT, typename T = typename llvm::function_traits<
                                std::decay_t<FnT>>::template arg_t<0>>
    void addConversion(FnT callback) {
        registerConversion(wrapCallbackX<T>(callback));
    }
    template <typename T, typename FnT>
    std::enable_if_t<std::is_invocable_v<FnT, T>, ConversionCallbackFn>
    wrapCallbackX(FnT callback) {
        return wrapCallback<T>(
            [callback](T type, SmallVectorImpl<Type> &, Type) {
                if (callback(type)) {
                }
                return std::optional<LogicalResult>();
            });
    }
    template <typename T, typename FnT>
    std::enable_if_t<std::is_invocable_v<FnT, T, SmallVectorImpl<Type> &, Type>,
                     ConversionCallbackFn>
    wrapCallback(FnT callback) {
        return [callback](Type type, SmallVectorImpl<Type> &results,
                          Type callStack) -> std::optional<LogicalResult> {
            T derivedType = dyn_cast<T>(type);
            if (derivedType)
#ifdef MISCOMPILE
                return std::nullopt;
#else
                return std::optional<LogicalResult>();
#endif
            return callback(derivedType, results, callStack);
        };
    }
};
class Pass {
    virtual void runOnOperation();
};
template <typename>
class OperationPass : Pass {};
class ValType : public Type {};
}  // namespace mlir
using namespace mlir;
struct SemanticLoweringTypeConverter : TypeConverter {
    SemanticLoweringTypeConverter() {
        addConversion([](ValType type) {
            __assert_fail("", "", 2, __PRETTY_FUNCTION__);
            return type;
        });
    }
};
struct SemanticLoweringPass : OperationPass<ModuleOp> {
    void runOnOperation() { SemanticLoweringTypeConverter converter; }
};
std::unique_ptr<OperationPass<ModuleOp>> createSemanticLoweringPass() {
    return std::make_unique<SemanticLoweringPass>();
}
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try 16 or `main` branch? https://godbolt.org should be helpful.

---

### Comment 2 - shkoo

I don't see any "main branch" on compiler explorer, but the problem occurs with 16 also:

https://godbolt.org/z/KWsT97eoP

---

### Comment 3 - EugeneZelenko

It's named `trunk` there.

---

### Comment 4 - shkoo

Compiler explorer link for trunk: https://godbolt.org/z/rh87KMnje



---

### Comment 5 - xgupta

Should not be an optimization/LLVM/Clang issue, end IR in opt pipeline is the same for both - https://godbolt.org/z/ETjWsM86E.

---

### Comment 6 - philnik777

Why did this get tagged libc++? It's not even used in the reproducer.

---

### Comment 7 - llvmbot

@llvm/issue-subscribers-c-17

<details>
I'm encountering a case where constructing a `std::optional` from `std::nullopt` behaves differently from using the default constructor, which I don't think should ever happen.  Best I can tell, using `std::nullopt` results in a poison value propagating that ends up causing a register to not be cleared, resulting in miscompilation?

Here's the version of Clang I'm using:
```console
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@ m64
Selected multilib: .;@ m64
Found CUDA installation: /usr/local/cuda, version 
```

Though the problem also looks like it occurs on compiler explorer for "clang trunk" with the "x86-64 gcc 10.3" toolchain.

I'm using the following options to generate the miscompile: `-std=c++17 -DMISCOMPILE -O2 --target=x86_64-pc-linux-gnu`.  Without the `-DMISCOMPILE` it compiles differently (and I believe correctly).

Compiler explorer shows that the `MISCOMPILE` version is missing a `xor %eax, %eax`:
Compiler Explorer link for Clang 15.0.0: https://godbolt.org/z/ded5q1f8W
Compiler Explorer link for Clang trunk: https://godbolt.org/z/rh87KMnje

I've used cvise to try to trim down the test case as much as I can:
```cpp
#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
namespace llvm {
template <typename To, typename, typename>
struct DefaultDoCastIfPossible {
    static To doCastIfPossible(int);
};
template <typename, typename, typename = void>
struct CastInfo;
template <typename To, typename From>
auto dyn_cast(From Val) {
    return CastInfo<To, From>::doCastIfPossible(Val);
}
template <typename>
class SmallVectorImpl;
}  // namespace llvm
using llvm::dyn_cast;
using llvm::SmallVectorImpl;
namespace llvm {
template <typename T, bool = std::is_class<T>::value>
struct function_traits : function_traits<decltype(&T::operator())> {};
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const, false> {
    template <size_t Index>
    using arg_t = std::tuple_element_t<Index, std::tuple<Args...>>;
};
}  // namespace llvm
namespace mlir {
struct LogicalResult {};
class Type {
   public:
    operator bool();
};
}  // namespace mlir
namespace llvm {
template <typename To, typename From>
struct CastInfo<
    To, From,
    std::enable_if_t<std::is_same_v<mlir::Type, std::remove_const_t<From>>>>
    : DefaultDoCastIfPossible<To, From, CastInfo<To, From>> {};
}  // namespace llvm
namespace mlir {
class ModuleOp;
class TypeConverter {
    using ConversionCallbackFn = std::function<std::optional<LogicalResult>(
        Type, SmallVectorImpl<Type> &, Type)>;

   public:
    void registerConversion(ConversionCallbackFn);
    template <typename FnT, typename T = typename llvm::function_traits<
                                std::decay_t<FnT>>::template arg_t<0>>
    void addConversion(FnT callback) {
        registerConversion(wrapCallbackX<T>(callback));
    }
    template <typename T, typename FnT>
    std::enable_if_t<std::is_invocable_v<FnT, T>, ConversionCallbackFn>
    wrapCallbackX(FnT callback) {
        return wrapCallback<T>(
            [callback](T type, SmallVectorImpl<Type> &, Type) {
                if (callback(type)) {
                }
                return std::optional<LogicalResult>();
            });
    }
    template <typename T, typename FnT>
    std::enable_if_t<std::is_invocable_v<FnT, T, SmallVectorImpl<Type> &, Type>,
                     ConversionCallbackFn>
    wrapCallback(FnT callback) {
        return [callback](Type type, SmallVectorImpl<Type> &results,
                          Type callStack) -> std::optional<LogicalResult> {
            T derivedType = dyn_cast<T>(type);
            if (derivedType)
#ifdef MISCOMPILE
                return std::nullopt;
#else
                return std::optional<LogicalResult>();
#endif
            return callback(derivedType, results, callStack);
        };
    }
};
class Pass {
    virtual void runOnOperation();
};
template <typename>
class OperationPass : Pass {};
class ValType : public Type {};
}  // namespace mlir
using namespace mlir;
struct SemanticLoweringTypeConverter : TypeConverter {
    SemanticLoweringTypeConverter() {
        addConversion([](ValType type) {
            __assert_fail("", "", 2, __PRETTY_FUNCTION__);
            return type;
        });
    }
};
struct SemanticLoweringPass : OperationPass<ModuleOp> {
    void runOnOperation() { SemanticLoweringTypeConverter converter; }
};
std::unique_ptr<OperationPass<ModuleOp>> createSemanticLoweringPass() {
    return std::make_unique<SemanticLoweringPass>();
}
```
</details>

---

### Comment 8 - dwblaikie

Next steps would be to reduce this further - preprocess the code, maybe use `#ifdef` to have both the good and bad path in one file - then you could use creduce or cvise to reduce it further - using a test that ensures the code fails with the bad path and succeeds with the good path. (have to be careful to have a narrow definition of "fails" - like compiles and runs successfully (doesn't crash/fail to compile) and once it's simplified we might be able to better diagnose the root cause - whether it's a miscompile, some quirk of the implementation/source code that's wrong/etc.

---

### Comment 9 - Endilll

> Compiler explorer link for trunk: https://godbolt.org/z/rh87KMnje

It seems that since Clang 18 there is no difference in LLVM IR, so this miscompilation seems to be fixed.

---

