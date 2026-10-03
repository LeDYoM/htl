#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("span::span", "[span]")
{
    {
        int test_array[] = {0, 1, 2, 3, 4, 5};
        auto test_element{span<int>{test_array, test_array + 6U}};
        static_assert(!decltype(test_element)::is_const_v);
        CHECK(test_element.size() == 6U);
        CHECK(test_element[0U] == test_array[0U]);
        CHECK(test_array[0U] == 0U);
        CHECK(test_element[3U] == test_array[3U]);
        CHECK(test_array[3U] == test_array[3U]);
        CHECK(test_element[5U] == test_array[5U]);
        CHECK(test_array[5U] == 5U);
        CHECK_FALSE(test_element.empty());
    }

    {
        int const test_array_const[] = {0, 1, 2, 3, 4, 5};
        auto test_element_const{
            span<int const>{test_array_const, test_array_const + 6U}};
        static_assert(decltype(test_element_const)::is_const_v);
        CHECK(test_element_const.size() == 6U);
        CHECK(test_element_const[0U] == test_array_const[0U]);
        CHECK(test_array_const[0U] == 0U);
        CHECK(test_element_const[3U] == test_array_const[3U]);
        CHECK(test_array_const[3U] == 3U);
        CHECK(test_element_const[5U] == test_array_const[5U]);
        CHECK(test_array_const[5U] == 5U);
        CHECK_FALSE(test_element_const.empty());
    }
}

TEST_CASE("span::make_span_address", "[span]")
{
    {
        int test_array[] = {0, 1, 2, 3, 4, 5};
        auto test_element{span{&test_array[0U], test_array + 6U}};
        static_assert(!decltype(test_element)::is_const_v);
        CHECK(test_element[0U] == test_array[0U]);
        CHECK(test_array[0U] == 0U);
        CHECK(test_element[3U] == test_array[3U]);
        CHECK(test_array[3U] == test_array[3U]);
        CHECK(test_element[5U] == test_array[5U]);
        CHECK(test_array[5U] == 5U);
        CHECK(test_element.size() == 6U);
        CHECK_FALSE(test_element.empty());
    }

    {
        int const test_array_const[] = {0, 1, 2, 3, 4, 5};
        auto test_element_const{
            span{&test_array_const[0U], test_array_const + 6U}};
        static_assert(decltype(test_element_const)::is_const_v);
        CHECK(test_element_const.size() == 6U);
        CHECK(test_element_const[0U] == test_array_const[0U]);
        CHECK(test_array_const[0U] == 0U);
        CHECK(test_element_const[3U] == test_array_const[3U]);
        CHECK(test_array_const[3U] == 3U);
        CHECK(test_element_const[5U] == test_array_const[5U]);
        CHECK(test_array_const[5U] == 5U);
        CHECK_FALSE(test_element_const.empty());
    }
}

TEST_CASE("span::make_span_built_in_array", "[span]")
{
    {
        int test_array[] = {0, 1, 2, 3, 4, 5};
        auto test_element{span{test_array}};
        static_assert(!decltype(test_element)::is_const_v);
        CHECK(test_element.size() == 6U);
        CHECK(test_element[0U] == test_array[0U]);
        CHECK(test_array[0U] == 0U);
        CHECK(test_element[3U] == test_array[3U]);
        CHECK(test_array[3U] == test_array[3U]);
        CHECK(test_element[5U] == test_array[5U]);
        CHECK(test_array[5U] == 5U);
        CHECK_FALSE(test_element.empty());
    }

    {
        int const test_array_const[] = {0, 1, 2, 3, 4, 5};
        auto test_element_const{span{test_array_const}};
        static_assert(decltype(test_element_const)::is_const_v);
        CHECK(test_element_const.size() == 6U);
        CHECK(test_element_const[0U] == test_array_const[0U]);
        CHECK(test_array_const[0U] == 0U);
        CHECK(test_element_const[3U] == test_array_const[3U]);
        CHECK(test_array_const[3U] == 3U);
        CHECK(test_element_const[5U] == test_array_const[5U]);
        CHECK(test_array_const[5U] == 5U);
        CHECK_FALSE(test_element_const.empty());
    }
}

TEST_CASE("span::make_span_vector", "[span]")
{
    {
        vector<int> test_array{0, 1, 2, 3, 4, 5};
        span test_element{span{test_array}};
        static_assert(!decltype(test_element)::is_const_v);
        CHECK(test_element.size() == 6U);
        CHECK(test_element[0U] == test_array[0U]);
        CHECK(test_array[0U] == 0U);
        CHECK(test_element[3U] == test_array[3U]);
        CHECK(test_array[3U] == test_array[3U]);
        CHECK(test_element[5U] == test_array[5U]);
        CHECK(test_array[5U] == 5U);
        CHECK_FALSE(test_element.empty());
    }

    {
        vector<int> const test_array_const{0, 1, 2, 3, 4, 5};
        auto test_element_const{span{test_array_const}};
        static_assert(decltype(test_element_const)::is_const_v);
        CHECK(test_element_const.size() == 6U);
        CHECK(test_element_const[0U] == test_array_const[0U]);
        CHECK(test_array_const[0U] == 0U);
        CHECK(test_element_const[3U] == test_array_const[3U]);
        CHECK(test_array_const[3U] == 3U);
        CHECK(test_element_const[5U] == test_array_const[5U]);
        CHECK(test_array_const[5U] == 5U);
        CHECK_FALSE(test_element_const.empty());
    }

    {
        vector<int> test_array{0, 1, 2, 3, 4, 5};
        auto test_element{span{&test_array[0U], &test_array[3U]}};
        static_assert(!decltype(test_element)::is_const_v);
        CHECK(test_element.size() == 3U);
        CHECK(test_element[0U] == test_array[0U]);
        CHECK(test_array[0U] == 0U);
        CHECK(test_element[1U] == test_array[1U]);
        CHECK(test_array[1U] == test_array[1U]);
        CHECK(test_element[2U] == test_array[2U]);
        CHECK(test_array[2U] == 2U);
        CHECK_FALSE(test_element.empty());
    }

    {
        vector<int> const test_array_const{0, 1, 2, 3, 4, 5};
        auto test_element_const{
            span{&test_array_const[0U], &test_array_const[3U]}};
        static_assert(decltype(test_element_const)::is_const_v);
        CHECK(test_element_const.size() == 3U);
        CHECK(test_element_const[0U] == test_array_const[0U]);
        CHECK(test_array_const[0U] == 0U);
        CHECK(test_element_const[1U] == test_array_const[1U]);
        CHECK(test_array_const[1U] == 1U);
        CHECK(test_element_const[2U] == test_array_const[2U]);
        CHECK(test_array_const[2U] == 2U);
        CHECK_FALSE(test_element_const.empty());
    }
}

TEST_CASE("span::span_of_one_element", "[span]")
{
    SECTION("const")
    {
        int const data{5U};
        auto test_span{span{data}};
        CHECK(test_span.size() == 1U);
        static_assert(decltype(test_span)::is_const_v);
        CHECK(test_span[0U] == data);
    }

    SECTION("non-const")
    {
        int data{5U};
        auto test_span{span{data}};
        CHECK(test_span.size() == 1U);
        static_assert(!(decltype(test_span)::is_const_v));
        CHECK(test_span[0U] == data);
    }
}
