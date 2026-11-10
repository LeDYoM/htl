#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("std::for_each", "[array]")
{
    vector<s32> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for_each(v.begin(), v.end(), [](auto& value) { value *= 2; })

        vector<s32>
            v1{0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    CHECK(v == v2);
}