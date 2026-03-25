// Minimal repro for LLDB assertion:
//   "Base subobject externally placed at overlapping offset"
//   (RecordLayoutBuilder.cpp, line 1283)
//
// The template alias produces two DW_TAG_typedef entries with identical
// DW_AT_name ("Alias<C, Trait>") but pointing at distinct Empty<> types.
// LLDB merges them, then asserts placing two "same" empty bases at offset 0.
//
// Build: clang++ -std=c++20 -g -O0 -o repro repro_final.cpp
// Repro: lldb --batch -o "b f" -o run -o "p obj.getX()" -- ./repro

template <typename T, template <typename> class TT>
struct Empty {};

template <typename T, template <typename> class TT>
using Alias = Empty<T, TT>;

struct A {
  template <typename U>
  struct Trait : Alias<U, Trait> {};
};

struct B {
  template <typename U>
  struct Trait : Alias<U, Trait> {};
};

class C;

class C : public A::Trait<C>, public B::Trait<C> {
public:
  int x = 0;
  int getX() const;
};

int C::getX() const { return x; }

__attribute__((noinline, optnone))
void f(C obj) { __builtin_debugtrap(); }

int main() {
  C obj;
  obj.x = 1;
  f(obj);
}
