#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("vector_stack::vector_stack default constructor", "[vector_stack]")
{
    {
        vector_stack<u32, 1024U> v;

        CHECK(v.size() == 0U);
        CHECK(v.empty());
        CHECK(v.capacity() == 1024U);
        CHECK_FALSE(v.full());

        v.push_back(12U);
        CHECK(v[0] == 12U);
        CHECK(v.size() == 1U);
        CHECK_FALSE(v.empty());
        CHECK(v.capacity() == 1024U);
        CHECK_FALSE(v.full());
    }

    {
        vector_stack<u32, 1U> v2;

        CHECK(v2.size() == 0U);
        CHECK(v2.empty());
        CHECK(v2.capacity() == 1U);
        CHECK_FALSE(v2.full());

        v2.push_back(90U);
        CHECK(v2[0] == 90U);
        CHECK(v2.size() == 1U);
        CHECK_FALSE(v2.empty());
        CHECK(v2.capacity() == 1U);
        CHECK(v2.full());
    }
}
