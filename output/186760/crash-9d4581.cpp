# 1 "<built-in>"
# 1 "crash.cpp"
template <typename... Args>
struct outer {
    template <typename Type>
    struct inner {
        inner(Args...) { }
    };

    inner(Args...) -> inner<int>; // deduction guide
};

int main() {
    outer();
}
