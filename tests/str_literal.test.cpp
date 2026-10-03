/*
#include "catch_include.hpp"

#include <type_traits>

import htypes;

using namespace htps;

static constexpr size_type kBigStrLiteralSize{20U};
using big_str_literal = str_literal<kBigStrLiteralSize>;

static constexpr char kBigStringLiteralValueOneLess[kBigStrLiteralSize] =
    "123456789012345678";

static constexpr char kBigStringLiteralValue[kBigStrLiteralSize] =
    "1234567890123456789";

template <size_type expected_size, typename T>
constexpr void check_size(T const& a)
{
    CHECK(sizeof(a) >= ((sizeof(char) * expected_size) + sizeof(size_type)));
    CHECK(sizeof(a) <=
          ((sizeof(char) * expected_size) + (2 * sizeof(size_type))));
}

TEST_CASE("str_literal::str_literal", "[str_literal]")
{
    SECTION("Autodetected size")
    {
        str_literal a{""};
        CHECK(a.size() == 0U);
        CHECK(a.empty());
        check_size<1U>(a);
    }
}

TEST_CASE("str_literal::str_literal with char_pointer", "[str_literal]")
{
    str_literal a{"Hello"};
    CHECK(a.size() == 5U);
    CHECK_FALSE(a.empty());
    check_size<6U>(a);

    str_literal c{kBigStringLiteralValueOneLess};
    CHECK(c.size() == 19U);
    CHECK_FALSE(c.empty());
    check_size<kBigStrLiteralSize>(c);

    str_literal d{kBigStringLiteralValue};
    CHECK(d.size() == 19U);
    CHECK_FALSE(d.empty());
    check_size<kBigStrLiteralSize>(d);
}

TEST_CASE("str_literal::operator== and operator!=", "[str_literal]")
{
    str_literal a{"a"};
    str_literal b{"b"};
    str_literal c{"hello"};
    CHECK_FALSE(a.empty());
    CHECK_FALSE(b.empty());
    CHECK_FALSE(c.empty());
    CHECK(std::is_same_v<decltype(a), decltype(b)>);
    CHECK_FALSE(std::is_same_v<decltype(a), decltype(c)>);
    CHECK_FALSE(std::is_same_v<decltype(b), decltype(c)>);

    str_literal d{"hello"};
    str_literal e{"hello2"};
    CHECK(c == d);
    CHECK(c == "hello");
    CHECK_FALSE(c == e);
    CHECK(c != e);
    CHECK_FALSE(c == "");
    CHECK_FALSE(c == "hello2");
    CHECK(c != "");
    CHECK(c != "hello2");
}
*/
