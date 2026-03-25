#include "repro.h"

#include <type_safe/strong_typedef.hpp>
#include <boost/pfr/core.hpp>

namespace {

struct Attack :
    type_safe::strong_typedef<Attack, int>
{
    using strong_typedef::strong_typedef;
};

struct Defence :
    type_safe::strong_typedef<Attack, int>
{
    using strong_typedef::strong_typedef;
};

struct Attributes
{
    Attack  attack;
    Defence defence;
};

}

void repro()
{
    Attributes attrs;

    int gold = 0;
    const auto  attrsToGold = [&](){
        boost::pfr::for_each_field(attrs, [&](const auto& attr) { gold += type_safe::get(attr); });
    };
}
