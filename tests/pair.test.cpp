#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("pair::pair", "[types][pair]")
{
    pair<s32,s64> p{};
    CHECK(p.first == 0);
    CHECK(p.second == 0);

    pair<sptr<u32>,uptr<s32>> p2;
    CHECK(p2.first == nullptr);
    CHECK(p2.second == nullptr);

    pair p3{0, 0U};
    static_assert(std::is_same_v<decltype(p3)::first_type, s32>);
    static_assert(std::is_same_v<decltype(p3)::second_type, u32>);

    pair p4{0L, 0UL};
    static_assert(std::is_same_v<decltype(p4)::first_type, long>);
    static_assert(std::is_same_v<decltype(p4)::second_type, unsigned long>);
}

TEST_CASE("pair::equality_comparisons", "[types][pair]")
{
    int g=1024;
    int g2=1025;
    pair<s32, s32*> p{56, nullptr};
    pair<s32, s32*> p2{56, nullptr};
    pair<s32, s32*> p3{57, nullptr};
    pair<s32, s32*> p4{56, &g};
    pair<s32, s32*> p5{56, &g};
    pair<s32, s32*> p6{57, &g};
    pair<s32, s32*> p7{56, &g2};

    CHECK(p == p2);
    CHECK(p != p3);
    CHECK(p4 == p5);
    CHECK(p4 != p6);
    CHECK(p4 != p7);

    CHECK(p == pair<s32,s32*>{56, nullptr});
    CHECK(pair<s32,s32*>{56, nullptr} == p);

    CHECK(p2 != pair<s32,s32*>{57, nullptr});
    CHECK(pair<s32,s32*>{57, nullptr} != p);

    CHECK(p4 == pair<s32,s32*>{56, &g});
    CHECK(pair<s32,s32*>{56, &g} == p4);

    CHECK(p6 != pair<s32,s32*>{56, &g});
    CHECK(pair<s32,s32*>{56, &g} != p6);
}

TEST_CASE("pair::comparisons", "[types][pair]")
{
    pair<s32, s32*> p{56, nullptr};
    pair<s32, s32*> p2{55, nullptr};
    pair<s32, s32*> p3{57, nullptr};

    CHECK(p > p2);
    CHECK(p < p3);

    CHECK(pair<s32,s32*>{56, nullptr} > pair<s32,s32*>{55, nullptr});
    CHECK(pair<s32,s32*>{56, nullptr} < pair<s32,s32*>{57, nullptr});

    CHECK(p2 < p);
    CHECK(p3 > p);

    CHECK(p >= p2);
    CHECK(p <= p3);
}
