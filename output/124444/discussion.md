# [Clang][accepts-invalid] Clang accepts capturing const-qualified bit-fields by reference

**Author:** zwuis
**URL:** https://github.com/llvm/llvm-project/issues/124444

## Body

Clang accepts this code:

```cpp
void test1() {
  struct S { int a : 4; };
  const auto [x] = S{};
  [&x] {}(); // #1
  [&] { (void)x; }(); // #2
}

void test2() {
  struct S { const int a : 4; };
  auto [x] = S{};
  [&x] {}(); // #3
  [&] { (void)x; }(); // #4
}
```

According to [[expr.prim.lambda.capture]/12](https://eel.is/c++draft/expr.prim.lambda.capture#12.sentence-4), all of these 4 cases should be ill-formed:

> ... A bit-field or a member of an anonymous union shall not be captured by reference.

You can see examples in [P1381R1](https://wg21.link/P1381R1#what-can-we-do) showing that capturing structured bindings refering bit-fields "means" capturing the bit-fields.

Compiler Explorer: <https://godbolt.org/z/s8bY75Pqn>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Yanzuo Liu (zwuis)

<details>
Clang accepts this code:

```cpp
void test1() {
  struct S { int a : 4; };
  const auto [x] = S{};
  [&amp;x] {}(); // #<!-- -->1
  [&amp;] { (void)x; }(); // #<!-- -->2
}

void test2() {
  struct S { const int a : 4; };
  auto [x] = S{};
  [&amp;x] {}(); // #<!-- -->3
  [&amp;] { (void)x; }(); // #<!-- -->4
}
```

According to [[expr.prim.lambda.capture]/12](https://eel.is/c++draft/expr.prim.lambda.capture#<!-- -->12.sentence-4), all of these 4 cases should be ill-formed:

&gt; ... A bit-field or a member of an anonymous union shall not be captured by reference.

You can see examples in [P1381R1](https://wg21.link/P1381R1#what-can-we-do) showing that capturing structured bindings refering bit-fields "means" capturing the bit-fields.

Compiler Explorer: &lt;https://godbolt.org/z/s8bY75Pqn&gt;
</details>


---

### Comment 2 - frederick-vs-ja

If [CWG2737](https://cplusplus.github.io/CWG/issues/2737.html) gets accepted and implemented, IIUC these cases will also be naturally rejected.

---

### Comment 3 - pinskia

Note GCC has the same bug, filed as https://gcc.gnu.org/PR118667 .

---

