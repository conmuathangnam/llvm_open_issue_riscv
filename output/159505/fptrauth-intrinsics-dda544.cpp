# 1 "<built-in>"
# 1 "/tmp/fptrauth-intrinsics.cpp"
class A {
  virtual void f();
};

template <int N> class B {
  class C : A {
    A a[N];
  } d;
};
