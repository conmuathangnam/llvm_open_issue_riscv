# 1 "clang-bug.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 499 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "clang-bug.cpp" 2
struct foo_tag {};

template<class T> struct foo {
 using tag = foo_tag;
 T run;
};

template<class T> concept isFoo = requires(T a) {a.run();};


template<int i, class T> auto complexify(T a) requires isFoo<T> {
 if constexpr (i > 0) {
  return complexify<i-1>(foo{ [a]{
   return 1+a.run();
  }});
 } else return a;
}


template<int i> auto complexify(int a) {
 return complexify<i-1>(foo{ [a]{
  return a;
 }});
}

int main() {
 auto a = complexify<389>(1);
}
