# 1 "main.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 478 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "main.cpp" 2
template<typename T>
concept integral = true;

template<typename Value_>
struct Degrees { Value_ value; };

template<integral A, integral B, template<integral> class Class_>
constexpr bool operator==(Class_<A> const first, Class_<B> const second) {
 return first.value == second.value;
}

static_assert(Degrees{80} == Degrees{80});
