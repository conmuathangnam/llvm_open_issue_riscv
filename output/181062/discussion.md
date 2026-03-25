# Assertion `E->containsErrors() && "valid value-dependent expression should never " "reach invalid code path."

**Author:** venk-ks
**URL:** https://github.com/llvm/llvm-project/issues/181062
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2026-02-26T19:26:11Z

## Body

We observed the following crash when Clang attempted to compile code that involved a generic lambda, constexpr, and multiple levels of templatization. Bisection pointed to https://github.com/llvm/llvm-project/pull/161029, but we're not sure why exactly that's causing the crash.

[assertion.txt](https://github.com/user-attachments/files/25247627/assertion.txt)
[local_guest_context_optimizer-e07d6b.cpp](https://github.com/user-attachments/files/25247625/local_guest_context_optimizer-e07d6b.cpp)
[local_guest_context_optimizer-e07d6b.sh](https://github.com/user-attachments/files/25247626/local_guest_context_optimizer-e07d6b.sh)

## Comments

### Comment 1 - venk-ks

CC: @pirama-arumuga-nainar 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (venk-ks)

<details>
We observed the following crash when Clang attempted to compile code that involved a generic lambda, constexpr, and multiple levels of templatization. Bisection pointed to https://github.com/llvm/llvm-project/pull/161029, but we're not sure why exactly that's causing the crash.

[assertion.txt](https://github.com/user-attachments/files/25247627/assertion.txt)
[local_guest_context_optimizer-e07d6b.cpp](https://github.com/user-attachments/files/25247625/local_guest_context_optimizer-e07d6b.cpp)
[local_guest_context_optimizer-e07d6b.sh](https://github.com/user-attachments/files/25247626/local_guest_context_optimizer-e07d6b.sh)
</details>


---

### Comment 3 - shafik

CC @mizvekov 

---

### Comment 4 - mizvekov

c-vise reduction only gets so far: https://compiler-explorer.com/z/Pd36xh7no
```C++
namespace std {
inline namespace {
template <bool> using __enable_if_t = int;
template <bool> using enable_if_t = int;
template <int __v> struct integral_constant {
  static const int value = __v;
};
template <bool _Val> using _BoolConstant = integral_constant<_Val>;
template <class _Tp>
using __libcpp_remove_reference_t = __remove_reference_t(_Tp);
template <class _Tp>
using remove_reference_t = __libcpp_remove_reference_t<_Tp>;
template <class _Tp> _Tp __declval(int);
template <class _Tp> decltype(__declval<_Tp>(0)) declval();
} // namespace
inline namespace __1 {
template <class... _Args>
using __invoke_result_t = decltype(__builtin_invoke(_Args()...));
template <class... _Args>
constexpr __invoke_result_t<_Args...> __invoke(_Args... __args) {
  return __builtin_invoke(__args...);
}
template <class...> class tuple;
template <class...> struct __tuple_types;
template <class> struct tuple_size;
template <class _Tp, class> using __enable_if_tuple_size_imp = _Tp;
template <class _Tp>
struct tuple_size<__enable_if_tuple_size_imp<
    const _Tp, __enable_if_t<_BoolConstant<__is_volatile(int)>::value>>>
    : integral_constant<tuple_size<_Tp>::value> {};
template <class... _Tp>
struct tuple_size<tuple<_Tp...>> : integral_constant<sizeof...(_Tp)> {};
template <class _Tp>
constexpr decltype(sizeof(int)) tuple_size_v = tuple_size<_Tp>::value;
} // namespace __1
} // namespace std
typedef int int64_t;
namespace std {
namespace {
template <template <class> class _BaseType, class _Tp, _Tp _SequenceSize>
using __make_integer_sequence_impl =
    __make_integer_seq<_BaseType, _Tp, _SequenceSize>;
template <class _Tp, _Tp...> struct __integer_sequence;
template <decltype(sizeof(int))... _Indices>
using __index_sequence = __integer_sequence<decltype(sizeof(int)), _Indices...>;
template <decltype(sizeof(int)) _SequenceSize>
using __make_index_sequence =
    __make_integer_sequence_impl<__integer_sequence, decltype(sizeof(int)),
                                 _SequenceSize>;
template <class _Tp, _Tp...> struct integer_sequence {};
template <decltype(sizeof(int))... _Ip>
using index_sequence = integer_sequence<decltype(sizeof(int)), _Ip...>;
template <class _Tp, _Tp _Ep>
using make_integer_sequence =
    __make_integer_sequence_impl<integer_sequence, _Tp, _Ep>;
template <decltype(sizeof(int)) _Np>
using make_index_sequence = make_integer_sequence<decltype(sizeof(int)), _Np>;
template <class _T1, class _T2> struct pair {
  _T1 first;
  _T2 second;
};
template <decltype(sizeof(int)), class> struct tuple_element;
template <decltype(sizeof(int)) _Ip, class... _Types>
struct tuple_element<_Ip, __tuple_types<_Types...>> {
  using type = __type_pack_element<_Ip, _Types...>;
};
template <decltype(sizeof(int)) _Ip, class... _Tp>
using tuple_element_t = tuple_element<_Ip, _Tp...>::type;
template <decltype(sizeof(int)) _Ip, class... _Tp>
struct tuple_element<_Ip, tuple<_Tp...>> {
  using type = tuple_element<_Ip, __tuple_types<_Tp...>>::type;
};
} // namespace
template <class> struct initializer_list {
  const unsigned long *__begin_;
  decltype(sizeof(int)) __size_;
  constexpr const unsigned long *begin() { return __begin_; }
};
namespace {
template <class _Cp> constexpr auto begin(_Cp &__c) { return __c.begin(); }
constexpr unsigned min(initializer_list<unsigned long> __t) {
  long __trans_tmp_1 = *__t.begin();
  return __trans_tmp_1;
}
enum __element_count : decltype(sizeof(int));
template <class _Tp, class _Up>
constexpr void __constexpr_memmove(_Tp __dest, _Up __src, __element_count __n) {
  __builtin_memmove(__dest, __src, __n);
}
} // namespace
namespace __1 {
template <class _Hp> struct __tuple_leaf {
  _Hp __value_;
  constexpr _Hp get() { return __value_; }
};
template <class...> struct __tuple_impl;
template <decltype(sizeof(int))... _Indx, class... _Tp>
struct __tuple_impl<__index_sequence<_Indx...>, _Tp...> : __tuple_leaf<_Tp>... {
};
template <class... _Tp> struct tuple {
  __tuple_impl<__make_index_sequence<sizeof...(_Tp)>, _Tp...> __base_;
};
template <int _Ip, class... _Tp>
constexpr tuple_element<_Ip, tuple<_Tp...>>::type get(tuple<_Tp...> __t) {
  return __t.__base_.get();
}
template <class _Tuple0> tuple<> tuple_cat(_Tuple0);
template <class _Fn, class _Tuple>
constexpr auto __apply_tuple_impl(_Fn __f, _Tuple) {
  return __invoke(__f);
}
template <class _Fn, class _Tuple> constexpr auto apply(_Fn __f, _Tuple __t) {
  return __apply_tuple_impl(__f, __t);
}
template <class _Iter> struct __unwrap_range_impl {
  static constexpr auto __unwrap(_Iter __first, _Iter __last) {
    return pair{__first, __last};
  }
};
template <class _Iter, class _Sent>
constexpr auto __unwrap_range(_Iter __first, _Sent __last) {
  return __unwrap_range_impl<_Iter>::__unwrap(__first, __last);
}
template <class _In, class _Out>
constexpr void __copy_trivial_impl(_In *__first, _In *__last, _Out __result) {
  decltype(sizeof(int)) __n(__last - __first);
  __constexpr_memmove(__result, __first, __element_count(__n));
}
template <class _Algorithm, class _InIter, class _Sent, class _OutIter>
constexpr void __copy_move_unwrap_iters(_InIter __first, _Sent __last,
                                        _OutIter __out_first) {
  auto __range = __unwrap_range(__first, __last);
  _Algorithm()(__range.first, __range.second, __out_first);
}
struct __copy_impl {
  template <class _In, class _Out>
  constexpr void operator()(_In *__first, _In *__last, _Out __result) {
    __copy_trivial_impl(__first, __last, __result);
  }
};
template <class _InIter, class _Sent, class _OutIter>
void constexpr __copy(_InIter __first, _Sent __last, _OutIter __result) {
  __copy_move_unwrap_iters<__copy_impl>(__first, __last, __result);
}
template <class _InputIterator, class _OutputIterator>
constexpr void copy(_InputIterator __first, _InputIterator __last,
                    _OutputIterator __result) {
  __copy(__first, __last, __result);
}
template <class _Tp, int _Size> struct array {
  _Tp __elems_[_Size];
  constexpr _Tp *begin() { return __elems_; }
};
template <class _Tp, decltype(sizeof(int)) _Size>
struct tuple_size<array<_Tp, _Size>> : integral_constant<_Size> {};
template <class _InputIterator, class _Size, class _OutputIterator>
constexpr void copy_n(_InputIterator __first, _Size __orig_n,
                      _OutputIterator __result) {
  copy(__first, __first + __orig_n, __result);
}
} // namespace __1
} // namespace std
template <auto kValue_> struct MetaValue {
  static constexpr auto kValue = kValue_;
};
template <typename...> auto kMetaTypes = std::tuple{};
struct FilterHelper {
  template <typename> auto operator()() const { return kMetaTypes<>; }
};
template <decltype(sizeof(int))... Is>
std::tuple<MetaValue<Is>...> IndexesHelper(std::index_sequence<Is...>);
struct TypesToTypes {
  template <typename> class ConcatHelper;
  template <typename, auto> class FlatMapHelper;
  template <typename... TupleTypes>
  using Indexes = decltype(IndexesHelper(
      std::declval<std::make_index_sequence<std::min(
          {std::tuple_size_v<std::remove_reference_t<TupleTypes>>...})>>()));
  template <typename TupleType, auto>
  using Filter = FlatMapHelper<TupleType, FilterHelper{}>::Result;
  template <typename... TupleTypes>
  using Concat = decltype(tuple_cat(
      std::declval<typename ConcatHelper<TupleTypes>::Result>()...));
  template <typename TupleType, auto kLambda>
  using FlatMap = FlatMapHelper<TupleType, kLambda>::Result;
  template <typename... Types> struct ConcatHelper<std::tuple<Types...>> {
    using Result = std::tuple<>;
  };
  template <typename> class TupleOfTypesHelper;
  template <typename... Types> struct TupleOfTypesHelper<std::tuple<Types...>> {
    using Tuple = std::tuple<>;
  };
  template <typename... Types, auto kLambda>
  struct FlatMapHelper<std::tuple<Types...>, kLambda> {
    using Result = Concat<typename TupleOfTypesHelper<
        decltype(kLambda.template operator()<Types>())>::Tuple...>;
  };
};
template <typename TupleType, typename Lambda, decltype(sizeof(int))... Is>
constexpr auto MapHelper(Lambda lambda, std::index_sequence<Is...>) {
  return std::tuple<decltype(lambda.template operator()<
                             std::tuple_element_t<Is, TupleType>>())...> {};
}
struct TypesToValues {
  template <typename TupleType, typename Lambda>
  static constexpr auto Map(Lambda lambda) {
    return MapHelper<TupleType>(
        lambda, std::make_index_sequence<
                    std::tuple_size_v<std::remove_reference_t<TupleType>>>{});
  }
};
struct ValuesToTypes {
  template <auto> class MetaValuesHelper;
  template <auto kValues>
  using MetaValues = MetaValuesHelper<kValues>::MetaValues;
  template <typename TupleName, TupleName kValues>
  struct MetaValuesHelper<kValues> {
    template <decltype(sizeof(int))... Is>
    static std::tuple<MetaValue<get<Is>(*kValues)>...>
        MetaValuesFunc(std::index_sequence<Is...>);
    using MetaValues = decltype(MetaValuesFunc(
        std::declval<std::make_index_sequence<std::tuple_size_v<
            std::remove_reference_t<decltype(*kValues)>>>>()));
  };
};
struct ValuesToValues {
  template <typename ArrayElement, typename TupleType>
  static constexpr auto ToArray(TupleType tuple) {
    return apply([] { return std::array<ArrayElement, 1>{}; }, tuple);
  }
};
namespace x86_32_or_x86_64 {
struct Assembler {
  class Register;
};
} // namespace x86_32_or_x86_64
struct Assembler : x86_32_or_x86_64::Assembler {
  void Movq(Register, int64_t);
};
template <int N> struct StringLiteral {
  constexpr StringLiteral(char str[]) { std::copy_n(str, N, begin(value)); }
  std::array<char, N> value;
};
template <int N> StringLiteral(const char (&)[N]) -> StringLiteral<N>;
namespace berberis::device_arch_info {
template <typename OperandClassesTuple>
constexpr int kCountMemoryOperands =
    std::tuple_size_v<TypesToTypes::Filter<OperandClassesTuple, [] {}>>;
enum { kUse };
template <typename, int> struct OperandInfo;
class NoCPUIDRestriction;
template <auto, StringLiteral, bool, auto, typename...> class DeviceInsnInfo;
template <auto kEmitInsnFunc_, StringLiteral kMnemo_, bool kSideEffects_,
          auto GetOpcode, typename CPUIDRestriction_, typename... Operands_>
struct DeviceInsnInfo<kEmitInsnFunc_, kMnemo_, kSideEffects_, GetOpcode,
                      CPUIDRestriction_, std::tuple<Operands_...>> {
  using Operands = std::tuple<Operands_...>;
};
} // namespace berberis::device_arch_info
namespace x86_32_or_x86_64::device_arch_info {
using namespace berberis::device_arch_info;
struct Imm64;
} // namespace x86_32_or_x86_64::device_arch_info
namespace device_arch_info {
using namespace x86_32_or_x86_64::device_arch_info;
}
template <typename MacroAssemblers> struct MovqRegImm {
  using DeviceInsnInfo = device_arch_info::DeviceInsnInfo<
      static_cast<void (std::tuple_element_t<1, MacroAssemblers>::*)(
          typename std::tuple_element_t<1, MacroAssemblers>::Register,
          int64_t)>(&std::tuple_element_t<1, MacroAssemblers>::Movq),
      "", false, [] {}, device_arch_info::NoCPUIDRestriction,
      std::tuple<device_arch_info::OperandInfo<device_arch_info::Imm64,
                                               device_arch_info::kUse>>>;
};
template <typename DeviceInsnInfo_> struct MachineInsn {
  using MachineInsnInfo = DeviceInsnInfo_;
  template <auto> static void GenMachineInsnInfo();
  static constexpr std::array<
      MachineInsnInfo, 1 << (2 * device_arch_info::kCountMemoryOperands<
                                     typename DeviceInsnInfo_::Operands>)>
  GenMachineInsnInfos();
  using DeviceInsnInfo = DeviceInsnInfo_;
  using ConstructorArgsTuple =
      TypesToTypes::FlatMap<typename DeviceInsnInfo::Operands,
                            []<typename> { return kMetaTypes<>; }>;
  static constexpr std::array kInfos = GenMachineInsnInfos();
};
template <int>
constexpr auto kBaseIndexRegistersUsed(
    TypesToValues::Map<TypesToTypes::Indexes<std::array<int, 1>>>([]<typename> {
      std::array<bool, 2> result;
      return result;
    }));
template <typename DeviceInsnInfo>
constexpr auto MachineInsn<DeviceInsnInfo>::GenMachineInsnInfos()
    -> std::array<MachineInsnInfo,
                  1 << (2 * device_arch_info::kCountMemoryOperands<
                                typename DeviceInsnInfo::Operands>)> {
  return ValuesToValues::ToArray<MachineInsnInfo>(
      TypesToValues::Map<ValuesToTypes::MetaValues<
          &kBaseIndexRegistersUsed<device_arch_info::kCountMemoryOperands<
              typename DeviceInsnInfo::Operands>>>>(
          []<typename BaseIndexRegistersUsed> {
            return GenMachineInsnInfo<BaseIndexRegistersUsed::kValue>;
          }));
}
struct {
  template <template <typename> typename InsnType>
  auto NewInsn() -> std::enable_if_t<
      std::tuple_size_v<typename MachineInsn<typename InsnType<
          std::tuple<int, Assembler>>::DeviceInsnInfo>::ConstructorArgsTuple>>;
} *machine_ir_;
void ReplaceGetAndUpdateMap() { machine_ir_->NewInsn<MovqRegImm>(); }
```

---

### Comment 5 - VorfeedCanal

Note that c-vise made the code actually non-compileable on old versions of compilers (and probably invalid) while original version can be compiled and works.

The [issue with assertion disappears](https://compiler-explorer.com/z/hGMKrjYxe) when I try to fix minor issue in that reduced version by removing restriction from `NewInsn` that is no longer satisfied and adding `*` that over-eager c-vise removed like this:
```
  template <typename TupleName, TupleName* kValues>
  struct MetaValuesHelper<kValues> {
```



---

### Comment 6 - mizvekov

The c-vise reduction compiles successfully on the clang just before the patch, so it should be fine.

---

### Comment 7 - Endilll

> c-vise reduction only gets so far

I was wondering how creduce would handle it, and got 3x smaller reproducer:
```cpp
# 0 "" 3
using size_t = decltype(sizeof(0));
template <class...> struct tuple;
template <class> constexpr size_t tuple_size_v = 0;
template <class _Tp, _Tp> struct integer_sequence {};
template <size_t... _Ip>
using index_sequence = integer_sequence<size_t, _Ip...>;
template <class _T2> struct pair {
  _T2 second;
};
template <class _T1, class _T2> constexpr pair<_T2> make_pair(_T1, _T2) {
  return pair<_T2>();
}
template <size_t...> using tuple_element_t = int;
template <class _Cp> constexpr auto begin(_Cp &__c) { return __c.begin(); }
template <class _Algorithm, class _InIter, class _Sent, class _OutIter>
constexpr pair<_OutIter> __copy_move_unwrap_iters(_InIter, _Sent,
                                                  _OutIter __out_first) {
  _Algorithm()(0, 0, __out_first);
  return make_pair(0, __out_first);
}
struct __copy_impl {
  template <class _InIter, class _Sent, class _OutIter>
  constexpr pair<_OutIter> operator()(_InIter, _Sent, _OutIter __result) {
    *__result = 0;
    return make_pair(0, __result);
  }
};
template <class _InIter, class _Sent, class _OutIter>
pair<_OutIter> constexpr __copy(_InIter, _Sent, _OutIter __result) {
  return __copy_move_unwrap_iters<__copy_impl>(0, 0, __result);
}
template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator copy(_InputIterator, _OutputIterator __result) {
  return __copy(0, 0, __result).second;
}
template <int> struct array {
  using value_type = int;
  using pointer = value_type *;
  using iterator = pointer;
  int __elems_[1];
  constexpr iterator begin() { return __elems_; }
};
template <class _InputIterator, class _Size, class _OutputIterator>
constexpr _OutputIterator copy_n(_InputIterator, _Size,
                                 _OutputIterator __result) {
  return copy(0, __result);
}
template <auto> struct MetaValue;
struct TypesToValues {
  template <typename> static auto Map();
};
struct ValuesToTypes {
  template <auto> struct MetaValuesHelper;
  template <auto kValues>
  using MetaValues = MetaValuesHelper<kValues>::MetaValues;
  template <typename TupleName, TupleName kValues>
  struct MetaValuesHelper<kValues> {
    integer_sequence<unsigned long, 0> __trans_tmp_6;
    template <size_t... Is>
    static tuple<MetaValue<stdget<Is>(kValues)>...>
        MetaValuesFunc(index_sequence<Is...>);
    using MetaValues = decltype(MetaValuesFunc(__trans_tmp_6));
  };
};
using CodeEmitterBase = int;
template <int N> struct StringLiteral {
  constexpr StringLiteral(const char (&)[N]) {
    auto __trans_tmp_4 = begin(value);
    copy_n(0, 0, __trans_tmp_4);
  }
  array<0> value;
};
template <typename> constexpr size_t kCountMemoryOperands = tuple_size_v<int>;
struct NoCPUIDRestriction;
template <auto, StringLiteral, int, auto, typename...> struct DeviceInsnInfo;
template <auto kEmitInsnFunc_, StringLiteral kMnemo_, int kSideEffects_,
          auto GetOpcode, typename CPUIDRestriction_>
struct DeviceInsnInfo<kEmitInsnFunc_, kMnemo_, kSideEffects_, GetOpcode,
                      CPUIDRestriction_, tuple<>> {
  using Operands = tuple<>;
};
template <typename> struct MovqRegImm {
  using DeviceInsnInfo =
      DeviceInsnInfo<0, "", 0, [] {}, NoCPUIDRestriction, tuple<>>;
};
template <typename DeviceInsnInfo_> struct MachineInsn {
  static constexpr array<
      1 << (2 * kCountMemoryOperands<typename DeviceInsnInfo_::Operands>)>
  GenMachineInsnInfos();
  static constexpr array kInfos = GenMachineInsnInfos();
};
template <size_t> auto kBaseIndexRegistersUsed(0);
template <typename DeviceInsnInfo>
constexpr auto MachineInsn<DeviceInsnInfo>::GenMachineInsnInfos()
    -> array<1 << (2 *
                   kCountMemoryOperands<typename DeviceInsnInfo::Operands>)> {
  TypesToValues::Map<ValuesToTypes::MetaValues<
      &kBaseIndexRegistersUsed<kCountMemoryOperands<DeviceInsnInfo>>>>;
}
struct MachineIR {
  template <template <typename> typename InsnType>
  void NewInsn(tuple_element_t<MachineInsn<typename InsnType<
                   CodeEmitterBase>::DeviceInsnInfo>::ConstructorArgsTuple>,
               tuple_element_t<>);
};
struct LocalGuestContextOptimizer {
  int ReplaceGetAndUpdateMap();
  MachineIR *machine_ir_;
};
int LocalGuestContextOptimizer::ReplaceGetAndUpdateMap() {
  machine_ir_->NewInsn<MovqRegImm>(0, 0)
```

---

### Comment 8 - Endilll

I reduced it further (https://godbolt.org/z/G41ceb6aM):
```cpp
template <int>
struct integer_sequence {};

template <int>
struct array {};

template <int*>
struct MetaValuesHelper;

template <typename TupleName, TupleName kValues>
struct MetaValuesHelper<kValues> {
  template <int... Is>
  static array<undefined<Is>(kValues)...> MetaValuesFunc(integer_sequence<Is...>);
};

int kBaseIndexRegistersUsed;

constexpr array<0> GenMachineInsnInfos() {
  return decltype(MetaValuesHelper<&kBaseIndexRegistersUsed>::MetaValuesFunc(integer_sequence<0>{})){};
}

array<0> u = GenMachineInsnInfos();
```

---

### Comment 9 - VorfeedCanal

Note that #181561  PR is only tangentially related: it tries to fix an assertion on a detection of a problem that wasn't even present in original reproducer, it was added to code by c-vise, as I have already noted.

---

### Comment 10 - Z3rox-dev

To clarify: #181561 targets #181410 specifically (crash-on-invalid when a partial specialization is not more specialized than the primary). The cross-reference here was created from an earlier revision of the PR. The original crash in this issue on valid code appears to be a separate problem.


---

### Comment 11 - mizvekov

Yeah, normally I ignore warnings on interestingness tests in order to not overconstrain it, but in this case the `invalid-partial-specialization` warning is quite significant to the problem, so I'll redo it.

---

### Comment 12 - mizvekov

It's quite likely though that the reproducer with the warning suffices, after all the stack trace shows the crash happens within a SFINAE context, and warnings produced there are never emitted, so I think it's probably the same problem.

---

### Comment 13 - mizvekov

Another reduction from the original, this time preserving the property that it does not produce the `invalid-partial-specialization` warning/error: https://compiler-explorer.com/z/WWWvsWdv1
```C++
namespace std {
inline namespace {
struct enable_if {};
template <bool> using __enable_if_t = int;
template <bool> using enable_if_t = enable_if;
template <int __v> struct integral_constant {
  static const int value = __v;
};
template <bool _Val> using _BoolConstant = integral_constant<_Val>;
template <class _Tp>
using __libcpp_remove_reference_t = __remove_reference_t(_Tp);
template <class _Tp>
using remove_reference_t = __libcpp_remove_reference_t<_Tp>;
template <class _Tp> _Tp __declval(int);
template <class _Tp> decltype(__declval<_Tp>(0)) declval();
template <class _Tp, class _Up> constexpr bool is_same_v = __is_same(_Tp, _Up);
template <class _If> struct conditional {
  using type = _If;
};
template <bool, class _IfRes, class>
using conditional_t = conditional<_IfRes>::type;
} // namespace
inline namespace __1 {
template <class... _Args>
using __invoke_result_t = decltype(__builtin_invoke(_Args()...));
template <class... _Args>
constexpr __invoke_result_t<_Args...> __invoke(_Args... __args) {
  return __builtin_invoke(__args...);
}
template <class...> struct tuple {};
template <class...> struct __tuple_types;
template <class> struct tuple_size;
template <class _Tp, class> using __enable_if_tuple_size_imp = _Tp;
template <class _Tp>
struct tuple_size<__enable_if_tuple_size_imp<
    const _Tp, __enable_if_t<_BoolConstant<__is_volatile(int)>::value>>>
    : integral_constant<tuple_size<_Tp>::value> {};
template <class... _Tp>
struct tuple_size<tuple<_Tp...>> : integral_constant<sizeof...(_Tp)> {};
template <class _Tp>
constexpr decltype(sizeof(int)) tuple_size_v = tuple_size<_Tp>::value;
} // namespace __1
} // namespace std
typedef int int64_t;
namespace std {
namespace {
template <template <class> class _BaseType, class _Tp, _Tp _SequenceSize>
using __make_integer_sequence_impl =
    __make_integer_seq<_BaseType, _Tp, _SequenceSize>;
template <class _Tp, _Tp...> struct integer_sequence {};
template <decltype(sizeof(int))... _Ip>
using index_sequence = integer_sequence<decltype(sizeof(int)), _Ip...>;
template <class _Tp, _Tp _Ep>
using make_integer_sequence =
    __make_integer_sequence_impl<integer_sequence, _Tp, _Ep>;
template <decltype(sizeof(int)) _Np>
using make_index_sequence = make_integer_sequence<decltype(sizeof(int)), _Np>;
template <class _T1, class _T2> struct pair {
  _T1 first;
  _T2 second;
};
template <decltype(sizeof(int)), class> struct tuple_element;
template <decltype(sizeof(int)) _Ip, class... _Types>
struct tuple_element<_Ip, __tuple_types<_Types...>> {
  using type = __type_pack_element<_Ip, _Types...>;
};
template <decltype(sizeof(int)) _Ip, class... _Tp>
using tuple_element_t = tuple_element<_Ip, _Tp...>::type;
template <decltype(sizeof(int)) _Ip, class... _Tp>
struct tuple_element<_Ip, tuple<_Tp...>> {
  using type = tuple_element<_Ip, __tuple_types<_Tp...>>::type;
};
} // namespace
namespace __1 {
enum class __element_count;
template <class _Tp, class _Up>
constexpr void __constexpr_memmove(_Tp __dest, _Up __src, __element_count __n) {
  __builtin_memmove(__dest, __src, static_cast<decltype(sizeof(int))>(__n));
}
template <class _Tuple0> tuple<> tuple_cat(_Tuple0);
template <class _Fn, class _Tuple>
constexpr auto __apply_tuple_impl(_Fn __f, _Tuple) {
  return __invoke(__f);
}
template <class _Fn, class _Tuple> constexpr auto apply(_Fn __f, _Tuple __t) {
  return __apply_tuple_impl(__f, __t);
}
template <class _Iter> struct __unwrap_range_impl {
  static constexpr auto __unwrap(_Iter __first, _Iter __last) {
    return pair{__first, __last};
  }
};
template <class _Iter, class _Sent>
constexpr auto __unwrap_range(_Iter __first, _Sent __last) {
  return __unwrap_range_impl<_Iter>::__unwrap(__first, __last);
}
template <class _In, class _Out>
constexpr void __copy_trivial_impl(_In *__first, _In *__last, _Out __result) {
  decltype(sizeof(int)) __n(__last - __first);
  __constexpr_memmove(__result, __first, __element_count(__n));
}
template <class _Algorithm, class _InIter, class _Sent, class _OutIter>
constexpr void __copy_move_unwrap_iters(_InIter __first, _Sent __last,
                                        _OutIter __out_first) {
  auto __range = __unwrap_range(__first, __last);
  _Algorithm()(__range.first, __range.second, __out_first);
}
struct __copy_impl {
  template <class _In, class _Out>
  constexpr void operator()(_In *__first, _In *__last, _Out __result) {
    __copy_trivial_impl(__first, __last, __result);
  }
};
template <class _InIter, class _Sent, class _OutIter>
void constexpr __copy(_InIter __first, _Sent __last, _OutIter __result) {
  __copy_move_unwrap_iters<__copy_impl>(__first, __last, __result);
}
template <class _InputIterator, class _OutputIterator>
constexpr void copy(_InputIterator __first, _InputIterator __last,
                    _OutputIterator __result) {
  __copy(__first, __last, __result);
}
template <class _Tp, decltype(sizeof(int)) _Size> struct array {
  _Tp __elems_[_Size];
  constexpr _Tp *begin() { return __elems_; }
};
template <class _Tp, decltype(sizeof(int)) _Size>
struct tuple_size<array<_Tp, _Size>> : integral_constant<_Size> {};
template <int _Ip, class _Tp, decltype(sizeof(int)) _Size>
constexpr _Tp get(array<_Tp, _Size> __a) {
  return __a.__elems_[_Ip];
}
template <class _InputIterator, class _Size, class _OutputIterator>
constexpr void copy_n(_InputIterator __first, _Size __orig_n,
                      _OutputIterator __result) {
  copy(__first, __first + __orig_n, __result);
}
} // namespace __1
} // namespace std
template <auto> struct MetaValue;
template <typename...> auto kMetaTypes = std::tuple{};
std::tuple<> IndexesHelper(std::index_sequence<>);
struct FilterHelper {
  template <typename> auto operator()() const { return kMetaTypes<>; }
};
struct TypesToTypes {
  template <typename> class ConcatHelper;
  template <typename, auto> class FlatMapHelper;
  template <typename...>
  using Indexes =
      decltype(IndexesHelper(std::declval<std::make_index_sequence<0>>()));
  template <typename TupleType, auto>
  using Filter = FlatMapHelper<TupleType, FilterHelper{}>::Result;
  template <typename... TupleTypes>
  using Concat = decltype(tuple_cat(
      std::declval<typename ConcatHelper<TupleTypes>::Result>()...));
  template <typename TupleType, auto kLambda>
  using FlatMap = FlatMapHelper<TupleType, kLambda>::Result;
  template <typename... Types> struct ConcatHelper<std::tuple<Types...>> {
    using Result = std::tuple<>;
  };
  template <typename> class TupleOfTypesHelper;
  template <typename... Types> struct TupleOfTypesHelper<std::tuple<Types...>> {
    using Tuple = std::tuple<>;
  };
  template <typename... Types, auto kLambda>
  struct FlatMapHelper<std::tuple<Types...>, kLambda> {
    using Result = Concat<typename TupleOfTypesHelper<
        decltype(kLambda.template operator()<Types>())>::Tuple...>;
  };
};
struct TypesToValues {
  template <typename TupleType, typename Lambda>
  static constexpr auto Map(Lambda) {
    return std::make_index_sequence<
        std::tuple_size_v<std::remove_reference_t<TupleType>>>{};
  }
};
struct ValuesToTypes {
  template <auto> class MetaValuesHelper;
  template <auto kValues>
  using MetaValues = MetaValuesHelper<kValues>::MetaValues;
  template <typename TupleName, TupleName *kValues>
  struct MetaValuesHelper<kValues> {
    template <unsigned long... Is>
    static std::tuple<MetaValue<get<Is>(*kValues)>...>
        MetaValuesFunc(std::index_sequence<Is...>);
    using MetaValues = decltype(MetaValuesFunc(
        std::declval<std::make_index_sequence<std::tuple_size_v<
            std::remove_reference_t<decltype(*kValues)>>>>()));
  };
};
struct ValuesToValues {
  template <typename ArrayElement, typename TupleType>
  static constexpr auto ToArray(TupleType tuple) {
    return apply([] { return std::array<ArrayElement, 1>{}; }, tuple);
  }
};
struct Trans_NS_x86_32_or_x86_64_Assembler {
  class Register;
};
struct Assembler : Trans_NS_x86_32_or_x86_64_Assembler {
  void Movq(Register, int64_t);
};
struct MacroAssembler
    : std::conditional_t<std::is_same_v<int, void>, Assembler, int> {};
template <int N> struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) {
    char *__trans_tmp_2 = value.begin();
    std::copy_n(str, N, __trans_tmp_2);
  }
  std::array<char, 1> value;
};
namespace berberis::device_arch_info {
template <typename OperandClassesTuple>
constexpr unsigned kCountMemoryOperands =
    std::tuple_size_v<TypesToTypes::Filter<OperandClassesTuple, [] {}>>;
enum { kUse };
template <typename, int> struct OperandInfo;
class NoCPUIDRestriction;
template <auto, StringLiteral, bool, auto, typename...> class DeviceInsnInfo;
template <auto kEmitInsnFunc_, StringLiteral kMnemo_, bool kSideEffects_,
          auto GetOpcode, typename CPUIDRestriction_, typename... Operands_>
struct DeviceInsnInfo<kEmitInsnFunc_, kMnemo_, kSideEffects_, GetOpcode,
                      CPUIDRestriction_, std::tuple<Operands_...>> {
  using Operands = std::tuple<Operands_...>;
};
} // namespace berberis::device_arch_info
namespace Trans_NS_Trans_Anon_NS0_x86_32_or_x86_64::device_arch_info {
using namespace berberis::device_arch_info;
struct Imm64;
} // namespace Trans_NS_Trans_Anon_NS0_x86_32_or_x86_64::device_arch_info
namespace {
namespace device_arch_info {
using namespace Trans_NS_Trans_Anon_NS0_x86_32_or_x86_64::device_arch_info;
}
template <typename MacroAssemblers> struct MovqRegImm {
  using DeviceInsnInfo = device_arch_info::DeviceInsnInfo<
      static_cast<void (std::tuple_element_t<1, MacroAssemblers>::*)(
          typename std::tuple_element_t<1, MacroAssemblers>::Register,
          int64_t)>(&std::tuple_element_t<1, MacroAssemblers>::Movq),
      "", false, [] {}, device_arch_info::NoCPUIDRestriction,
      std::tuple<device_arch_info::OperandInfo<device_arch_info::Imm64,
                                               device_arch_info::kUse>>>;
};
template <typename DeviceInsnInfo_> struct MachineInsn {
  using MachineInsnInfo = DeviceInsnInfo_;
  static constexpr std::array<
      MachineInsnInfo, 1 << (2 * device_arch_info::kCountMemoryOperands<
                                     typename DeviceInsnInfo_::Operands>)>
  GenMachineInsnInfos();
  using DeviceInsnInfo = DeviceInsnInfo_;
  using ConstructorArgsTuple =
      TypesToTypes::FlatMap<typename DeviceInsnInfo::Operands,
                            []<typename> { return kMetaTypes<>; }>;
  static constexpr std::array kInfos = GenMachineInsnInfos();
};
template <long>
constexpr auto kBaseIndexRegistersUsed =
    ValuesToValues::ToArray<std::array<bool, 1>>(
        TypesToValues::Map<TypesToTypes::Indexes<>>([] {}));
template <typename DeviceInsnInfo>
constexpr auto MachineInsn<DeviceInsnInfo>::GenMachineInsnInfos()
    -> std::array<MachineInsnInfo,
                  1 << (2 * device_arch_info::kCountMemoryOperands<
                                typename DeviceInsnInfo::Operands>)> {
  return ValuesToValues::ToArray<MachineInsnInfo>(
      TypesToValues::Map<ValuesToTypes::MetaValues<
          &kBaseIndexRegistersUsed<device_arch_info::kCountMemoryOperands<
              typename DeviceInsnInfo::Operands>>>>([] {}));
}
struct MachineIR {
  template <template <typename> typename InsnType>
  auto NewInsn() -> std::enable_if_t<
      std::tuple_size_v<typename MachineInsn<typename InsnType<std::tuple<
          int, MacroAssembler>>::DeviceInsnInfo>::ConstructorArgsTuple>>;
};
} // namespace
MachineIR *machine_ir_;
void ReplaceGetAndUpdateMap() { machine_ir_->NewInsn<MovqRegImm>(); }
```

I confirm that https://github.com/llvm/llvm-project/pull/183348 fixes all reductions we have come up so far, so I am only including the small one produced by Vlad in the regression test suite.

---

