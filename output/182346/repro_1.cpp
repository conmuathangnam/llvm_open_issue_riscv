// #include "repro.h"

#include <boost/pfr/core.hpp>

namespace {

struct Attributes
{
    int  attack;
    int defence;
};

}

void repro()
{
    Attributes attrs;

    int gold = 0;
    const auto  attrsToGold = [&](){
        boost::pfr::for_each_field(attrs, [&](const auto& attr) { gold += attr; });
    };
}
