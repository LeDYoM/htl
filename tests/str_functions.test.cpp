#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("htps::strnlen_base", "[str][str_functions]")
{
    SECTION("Normal str")
    {
        const char* test_str{"This is a test"};
        CHECK(htps::strnlen(test_str) == 14U);
    }

    SECTION("Null ptr")
    {
        const char* test_str{nullptr};
        CHECK(htps::strnlen(test_str) == 0U);
    }

    SECTION("Empty string")
    {
        const char* test_str{""};
        CHECK(htps::strnlen(test_str) == 0U);
    }

    SECTION("Length one string")
    {
        const char* test_str{"a"};
        CHECK(htps::strnlen(test_str) == 1U);
    }

    SECTION("Length one string")
    {
        const char* test_str{"a"};
        CHECK(htps::strnlen(test_str) == 1U);
    }
}

TEST_CASE("htps::strn_len literal", "[str][str_functions]")
{
    SECTION("Normal str")
    {
        CHECK(htps::strnlen("This is a test") == 14U);
        static_assert(htps::strnlen("This is a test") == 14U);
        [[maybe_unused]] constexpr auto b{htps::strnlen("This is a test")};
    }

    SECTION("Null ptr")
    {
        CHECK(htps::strnlen(nullptr) == 0U);
        static_assert(htps::strnlen(nullptr) == 0U);
        [[maybe_unused]] constexpr auto b{htps::strnlen(nullptr)};
    }

    SECTION("Empty string")
    {
        CHECK(htps::strnlen("") == 0U);
        static_assert(htps::strnlen("") == 0U);
        [[maybe_unused]] constexpr auto b{htps::strnlen("")};
    }

    SECTION("Length one string")
    {
        CHECK(htps::strnlen("a") == 1U);
        static_assert(htps::strnlen("a") == 1U);
        [[maybe_unused]] constexpr auto b{htps::strnlen("a")};
    }
}

TEST_CASE("htps::strnlen_change_max_length", "[str][str_functions]")
{
    SECTION("Normal str")
    {
        const char* test_str{"This is a test"};
        CHECK(htps::strnlen(test_str, 10U) == 10U);
    }

    SECTION("Null ptr")
    {
        const char* test_str{nullptr};
        CHECK(htps::strnlen(test_str, 11U) == 0U);
    }

    SECTION("Empty string")
    {
        const char* test_str{""};
        CHECK(htps::strnlen(test_str, 50000U) == 0U);
    }

    SECTION("Length one string")
    {
        const char* test_str{"a"};
        CHECK(htps::strnlen(test_str, 0) == 0U);
    }

    SECTION("Length one string")
    {
        const char* test_str{"a"};
        CHECK(htps::strnlen(test_str, 5U) == 1U);
    }
}

TEST_CASE("htps::strncmp_base", "[str][str_functions]")
{
    SECTION("Normal equal strings")
    {
        const char* test_str{"Hello world"};
        const char* test_str2{"Hello world"};
        CHECK(htps::strncmp(test_str, test_str2) == 0);
    }

    SECTION("Normal short strings")
    {
        const char* test_str{""};
        const char* test_str2{""};
        CHECK(htps::strncmp(test_str, test_str2) == 0);
    }

    SECTION("Normal strings lhs is lower")
    {
        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello "};
            CHECK(htps::strncmp(test_str, test_str2) < 0);
        }

        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2) < 0);
        }

        {
            const char* test_str{"Ggggggggggggggggggggg"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2) < 0);
        }

        {
            const char* test_str{"Hallo_worldddddd"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2) < 0);
        }

        {
            const char* test_str{"Hello_worlc"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2) < 0);
        }
    }

    SECTION("Normal strings lhs is higher")
    {
        {
            const char* test_str{"Hello "};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Ggggggggggggggggggggg"};
            CHECK(htps::strncmp(test_str, test_str2) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hallo_worldddddd"};
            CHECK(htps::strncmp(test_str, test_str2) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello_worlc"};
            CHECK(htps::strncmp(test_str, test_str2) > 0);
        }
    }
}

TEST_CASE("htps::strncmp_constexpr", "[str][str_functions]")
{
    {
        CHECK(htps::strncmp("Hello world", "Hello world") == 0);
        static_assert(htps::strncmp("Hello world", "Hello world") == 0);
    }

    {
        CHECK(htps::strncmp("aaa", "aab") < 0);
        static_assert(htps::strncmp("aaa", "aab") < 0);
    }

    {
        CHECK(htps::strncmp("aab", "aaa") > 0);
        static_assert(htps::strncmp("aab", "aaa") > 0);
    }
}

TEST_CASE("htps::strncmp_starting_non_zero", "[str][str_functions]")
{
    SECTION("Normal equal strings")
    {
        const char* test_str{"Hello world"};
        const char* test_str2{"Hello world"};
        CHECK(htps::strncmp(test_str, test_str2, 5) == 0);
    }

    SECTION("Normal short strings")
    {
        const char* test_str{""};
        const char* test_str2{""};
        CHECK(htps::strncmp(test_str, test_str2, 1) == 0);
    }

    SECTION("Normal strings lhs is lower")
    {
        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello "};
            CHECK(htps::strncmp(test_str, test_str2, 4) < 0);
        }

        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2, 6) < 0);
        }

        {
            const char* test_str{"Hello_world_this_test"};
            const char* test_str2{"Hello_world_this_test    "};
            CHECK(htps::strncmp(test_str, test_str2, 12) < 0);
        }

        {
            const char* test_str{"Hallo_world"};
            const char* test_str2{"Hello_worldddddd"};
            CHECK(htps::strncmp(test_str, test_str2, 11) < 0);
        }

        {
            const char* test_str{"Hello_worlc"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2, 10) < 0);
        }
    }

    SECTION("Normal strings lhs is higher")
    {
        {
            const char* test_str{"Hello "};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2, 4) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2, 4) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Ggggggggggggggggggggg"};
            CHECK(htps::strncmp(test_str, test_str2, 4) > 0);
        }

        {
            const char* test_str{"Hello_worldddddddd"};
            const char* test_str2{"Hallo_world"};
            CHECK(htps::strncmp(test_str, test_str2, 10) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello_worlc"};
            CHECK(htps::strncmp(test_str, test_str2, 10) > 0);
        }
    }
}

TEST_CASE("htps::strncmp_index_non_zero", "[str][str_functions]")
{
    SECTION("Normal equal strings")
    {
        const char* test_str{"Hello world"};
        const char* test_str2{"Hello world"};
        CHECK(htps::strncmp(test_str, test_str2, 0, 5) == 0);
    }

    SECTION("Normal short strings")
    {
        const char* test_str{""};
        const char* test_str2{""};
        CHECK(htps::strncmp(test_str, test_str2, 2, 6) == 0);
    }

    SECTION("Normal strings lhs is lower")
    {
        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello "};
            CHECK(htps::strncmp(test_str, test_str2, 4, 10) < 0);
        }

        {
            const char* test_str{"Hello"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2, 6, 7) < 0);
        }

        {
            const char* test_str{"Hallo_world"};
            const char* test_str2{"Hello_worldddddd"};
            CHECK(htps::strncmp(test_str, test_str2, 11, 12) < 0);
        }

        {
            const char* test_str{"Hello_worlc"};
            const char* test_str2{"Hello_world"};
            CHECK(htps::strncmp(test_str, test_str2, 10, 20) < 0);
        }
    }

    SECTION("Normal strings lhs is higher")
    {
        {
            const char* test_str{"Hello "};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2, 4, 6) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello"};
            CHECK(htps::strncmp(test_str, test_str2, 4, 6) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Ggggggggggggggggggggg"};
            CHECK(htps::strncmp(test_str, test_str2, 4, 8) > 0);
        }

        {
            const char* test_str{"Hello_worldddddddd"};
            const char* test_str2{"Hallo_world"};
            CHECK(htps::strncmp(test_str, test_str2, 10, 12) > 0);
        }

        {
            const char* test_str{"Hello_world"};
            const char* test_str2{"Hello_worlc"};
            CHECK(htps::strncmp(test_str, test_str2, 10, 12) > 0);
        }
    }
}

TEST_CASE("htps::strncmp_index_and_length_compare_are_non_zero",
          "[str][str_functions]")
{
    {
        const char* test_str{"Hallo_world"};
        const char* test_str2{"Hello_worle"};
        CHECK(htps::strncmp(test_str, test_str2, 1, 5) < 0);
    }

    {
        const char* test_str{"Hallo_world"};
        const char* test_str2{"Hello_worle"};
        CHECK(htps::strncmp(test_str, test_str2, 2, 8) == 0);
    }

    {
        const char* test_str{"Hallo_world"};
        const char* test_str2{"Hello_worle"};
        CHECK(htps::strncmp(test_str, test_str2, 2, 10) == 0);
    }

    {
        const char* test_str{"Hello_world_this_test"};
        const char* test_str2{"Hello_world_this_test    "};
        CHECK(htps::strncmp(test_str, test_str2, 12, 16) == 0);
    }
}

TEST_CASE("htps::strncmp_constexpr_only", "[str][str_functions]")
{
    static_assert(htps::strncmp("Hallo_world", "Hello_worle", 1, 5) < 0);
    static_assert(htps::strncmp("Hallo_world", "Hello_worle", 2, 8) == 0);
    static_assert(htps::strncmp("Hallo_world", "Hello_worle", 2, 10) == 0);
    static_assert(htps::strncmp("Hello_world_this_test",
                                "Hello_world_this_test   ", 12, 16) == 0);
}

TEST_CASE("htps::starts_with", "[str][str_functions]")
{
    static_assert(htps::starts_with("Hello world", "Hello"));
    static_assert(!(htps::starts_with("world", "Hello")));
    static_assert(!htps::starts_with("Hello world", "Hello world!"));
    static_assert(htps::starts_with("", ""));
    static_assert(htps::starts_with("abc", ""));
    CHECK(htps::starts_with("Hello world", "Hello"));
    CHECK(!(htps::starts_with("world", "Hello")));
    CHECK(!htps::starts_with("Hello world", "Hello world!"));
    CHECK(htps::starts_with("", ""));
    CHECK(htps::starts_with("abc", ""));
}

TEST_CASE("htps::strnfind", "[str][str_functions]")
{
    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "_wo") == 5U);
        static_assert(htps::strnfind("Hello_world", "_wo") == 5U);
    }

    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "_wa") == htps::npos);
        static_assert(htps::strnfind("Hello_world", "_wa") == htps::npos);
    }

    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "_wO") == htps::npos);
        static_assert(htps::strnfind("Hello_world", "_wO") == htps::npos);
    }

    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "_world!") == htps::npos);
        static_assert(htps::strnfind("Hello_world", "_world!") == htps::npos);
    }

    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "H") == 0U);
        static_assert(htps::strnfind("Hello_world", "H") == 0U);
    }

    {
        const char* test_str{"Hello_world"};
        CHECK(htps::strnfind(test_str, "") == 0U);
        static_assert(htps::strnfind("Hello_world", "") == 0U);
    }

    {
        const char* test_str{""};
        CHECK(htps::strnfind(test_str, "") == 0U);
        static_assert(htps::strnfind("", "") == 0U);
    }

    {
        const char* test_str{""};
        CHECK(htps::strnfind(test_str, " ") == htps::npos);
        static_assert(htps::strnfind("", " ") == htps::npos);
    }
}
