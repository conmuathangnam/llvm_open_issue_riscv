
#include <type_traits>
#include <utility>

struct xx
{
        template<typename Tfunc>
    void RunWithStage(const Tfunc&& functor);
};

template<typename Tfunc>
void xx::RunWithStage(const Tfunc &&functor)
{
    auto t = [this, functor=std::forward<const Tfunc>(functor)]() {
        functor();
    };
    t();
}

// verify RunWithStage is rvalue-only
namespace _asserts {
    template<typename T = void>
    concept RefRunnable = requires { [](decltype([](){}) f  ) -> decltype(T{}.RunWithStage(std::move(f))) { }; };
}
static_assert(!_asserts::RefRunnable<xx>, "lvalue ref functors forbidden");