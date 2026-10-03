#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("Hash::string", "[Hash][str]")
{
    constexpr u32 str_chash{StringHash_v<"abc">};
    static_assert(str_chash != 0U);

    CHECK(str_chash == StringHash_v<"abc">);

    static_assert(StringHash_v<"abd"> != StringHash_v<"abc">);
}

TEST_CASE("Hash integers", "[Hash]")
{
    int const a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int const b[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    u32 const ha    = Hasher::hash(span{a, a + 10});
    u32 const hb    = Hasher::hash(span{b, b + 10});

    CHECK(ha != hb);
}

TEST_CASE("Hash floats", "[Hash]")
{
    f32 const a[10] = {0.1F, 1.1F, 2.1F, 3.1F, 4.1F,
                       5.1F, 6.1F, 7.1F, 8.1F, 9.1F};
    f32 const b[10] = {10.1F, 11.1F, 12.1F, 13.1F, 14.1F,
                       15.1F, 16.1F, 17.1F, 18.1F, 19.1F};
    u32 const ha    = Hasher::hash(span{a, a + 10});
    u32 const hb    = Hasher::hash(span{b, b + 10});

    CHECK(ha != hb);
}
