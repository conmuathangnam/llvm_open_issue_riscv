# -fwhole-program-vtables misoptimizes `select(, llvm.type.test, llvm.type.test)` pattern

**Author:** MatzeB
**URL:** https://github.com/llvm/llvm-project/issues/187049
**Status:** Open
**Labels:** miscompilation, LTO

## Body

I see code being misoptimized when `-fwhole-program-vtables` is used. Reproducer below.

My understanding is that `-fwhole-program-vtables` is supposed to work fine in combination with `-Wl,--lto-whole-program-visibility ` with the assumption that subclasses in external modules can be detected by visibility of type info at the linking step (https://reviews.llvm.org/D155659 ).

In the given example the type is only used in a shared object, but RTTI, destrcutor are in a different .so file. When looking at the `fine` function of the reproducer we just don't perf devirtualization as expected. But when looking at the `dont_remove_me_please` function the `WholeProgramDevirt` pass is not doing any work either (except changing `llvm.public.type.test` into `llvm.type.test` intrinsics), however we are left with a `assume(select(..., llvm.type.test(...), llvm.type.test(...)))` pattern which I suspect is causing trouble because all other `assume(llvm.type.test)` patterns are removed within the pass. With the `assume(select)` still around the following LowerTypeTests pass somehow replace both type-tests with `false` leaving us with `assume(false)` which in turn removes the whole function...

repro.cpp:
```c++
extern int puts(const char* str);

struct Base {
  Base();
  virtual ~Base() = default;
  virtual void action();
};
struct Derived : public Base {
  void action() override;
  Derived() = default;
  ~Derived() override;
};

// This function gets incorrectly removed!
void dont_remove_me_please(Base* b, bool isDerived) {
  if (isDerived) {
    Derived* d = static_cast<Derived*>(b);
    d->action();
  } else {
    b->action();
  }
  puts("hello");
}

void fine(Base *b, Derived *d) {
  // Just demonstrating that "normal" calls are fine.
  b->action();
  d->action();
}
```

compile.sh
```sh
#!/bin/bash
set -Eeux -o pipefail

CLANGBIN=$HOME/dev/llvm/build/bin
CLANGPP="${CLANGBIN}/clang++"

CXXFLAGS="-O3 -flto=thin"
CXXFLAGS+=" -fPIC"
CXXFLAGS+=" -fwhole-program-vtables"
LINKFLAGS="-B ${CLANGBIN} -fuse-ld=lld"
LINKFLAGS+=" -Wl,--lto-whole-program-visibility"
LINKFLAGS+=" -Wl,--lto-validate-all-vtables-have-type-infos"
LINKFLAGS+=" -fwhole-program-vtables"

mkdir -p build
${CLANGPP} ${CXXFLAGS} repro.cpp -c -o build/repro.o
${CLANGPP} ${LINKFLAGS} build/repro.o -flto=thin -fPIC -shared -o build/librepro.so
```

When inspecting the result with `nm build/libgroup.so` or `llvm-objdump -d build/libgroup.so` you will notice that the whole `dont_remove_me_please` function is empty. (so the address merged with the following `fine` function)

Tried with 17.x, 19.x and github/main all showing the same problem.

