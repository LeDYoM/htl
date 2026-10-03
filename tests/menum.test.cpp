#include "catch_include.hpp"

#include <limits>

import htypes;

using namespace htps;

enum class A : s32
{
    min        = 0,
    max        = 2,
    TestValue1 = min,
    TestValue2 = min + 1,
    TestValue3 = max
};

constexpr char const* to_str(const A value)
{
    switch (value)
    {
        case A::TestValue1:
            return "TestValue1";
            break;
        case A::TestValue2:
            return "TestValue2";
            break;
        case A::TestValue3:
            return "TestValue3";
            break;
    }
    return "";
}

TEST_CASE("menum", "[menum]")
{
    static_assert(MEnum<A>::min == A::min, "Error in min");
    static_assert(MEnum<A>::max == A::max, "Error in max");

    A a{A::TestValue1};
    CHECK(a == A::min);
    CHECK(a == A::TestValue1);

    A a1{A::TestValue3};
    CHECK(a1 == A::max);
    CHECK(a1 == A::TestValue3);

    // Check static data
    CHECK(MEnum<A>::min == A::min);
    CHECK(MEnum<A>::max == A::max);
    CHECK(MEnum<A>::min_numeric == 0);
    CHECK(MEnum<A>::max_numeric == 2);
    CHECK(MEnum<A>::min_str == str("TestValue1"));
    CHECK(MEnum<A>::max_str == str("TestValue3"));

    MEnum<A> v{A::TestValue1};
    while (v.nextIsValid())
    {
        ++v;
    }

    {
        const MEnum<A> v_temp{v};
        CHECK(v_temp.isValid());
        CHECK(v_temp.previousIsValid());
        CHECK_FALSE(v_temp.nextIsValid());
        CHECK(v_temp.getValue() == A::TestValue3);
    }

    while (v.previousIsValid())
    {
        --v;
    }

    {
        const MEnum<A> v_temp{v};
        CHECK(v_temp.isValid());
        CHECK_FALSE(v_temp.previousIsValid());
        CHECK(v_temp.nextIsValid());
        CHECK(v_temp.getValue() == A::TestValue1);
    }
}
