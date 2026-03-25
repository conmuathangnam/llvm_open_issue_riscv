# 1 "<built-in>"
# 1 "test.cc"
struct StageList;
using F = __attribute__((vector_size(16))) float;
using StageFn = void (*)(StageList list, int src, int dst, F r, F g, F b);
struct StageList {
  const StageFn* fn;
};

void m(StageList list, int src, int dst,  F r, F g, F b) {
  [[clang::musttail]] 
 return (*list.fn)(list, src, dst, r, g, b);
} 
