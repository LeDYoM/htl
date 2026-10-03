#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("array::array", "[array]")
{
    SECTION("Default Constructor")
    {
        array<u32, 4U> m;

        CHECK(m.size() == 4U);
        CHECK_FALSE(m.empty());
    }

    SECTION("Copy constructor")
    {
        array<s32, 4U> m;

        m[0U] = 5;

        array<s32, 4U> m2(m);
        CHECK(m.size() == m2.size());
        CHECK(m[0U] == m2[0U]);
    }

    SECTION("Iterators constructor")
    {
        const u32 data[] = {0U, 1U, 2U, 3U, 4U};
        array<u32, 5U> v(data, data + 5);
        CHECK(v.size() == 5U);
        CHECK(v[0U] == data[0U]);
        CHECK(v[1U] == data[1U]);
        CHECK(v[2U] == data[2U]);
        CHECK(v[3U] == data[3U]);
        CHECK(v[4U] == data[4U]);
    }

    SECTION("Initializer list constructor")
    {
        array<u32, 11U> m;
        m = {1U, 9U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 0U, 1U};
        CHECK(m == array<u32, 11U>{1, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1});

        SECTION("Copy and move constructor")
        {
            auto m2(m);
            CHECK(m == m2);
            CHECK(m.size() == m2.size());

            auto m3(htps::move(m));
            CHECK(m2 == m3);
        }
    }

    SECTION("Construct from address and size")
    {
        SECTION("With sizeof")
        {
            const u32 arr[] = {4U, 3U, 2U, 1U};
            array<u32, 4U> v(arr, sizeof(arr) / sizeof(u32));
            CHECK(v.size() == 4U);
        }

        SECTION("With sizeof")
        {
            const u32 arr[] = {4U, 3U, 2U, 1U};
            array<u32, 4U> v(std::cbegin(arr), std::cend(arr));
            CHECK(v.size() == 4U);
        }
    }
}

TEST_CASE("array::insert", "[array]")
{
    array<s32, 2U> test_array{1, 2};

    SECTION("Insert at the begining")
    {
        CHECK(test_array[0U] == 1);
        CHECK(test_array[1U] == 2);

        test_array.insert(0U, 3);
        CHECK(test_array[0U] == 3);
        CHECK(test_array[1U] == 1);

        test_array.insert(0U, 4);
        CHECK(test_array[0U] == 4);
        CHECK(test_array[1U] == 3);
    }

    SECTION("Insert at the end")
    {
        CHECK(test_array[0U] == 1);
        CHECK(test_array[1U] == 2);

        test_array.insert(1U, 3);
        CHECK(test_array[0U] == 1);
        CHECK(test_array[1U] == 3);

        test_array.insert(1U, 4);
        CHECK(test_array[0U] == 1);
        CHECK(test_array[1U] == 4);
    }

    SECTION("Insert at the middle")
    {
        array<s32, 5U> test_array2{0, 1, 2, 3, 4};

        CHECK(test_array2[0U] == 0);
        CHECK(test_array2[1U] == 1);
        CHECK(test_array2[2U] == 2);
        CHECK(test_array2[3U] == 3);
        CHECK(test_array2[4U] == 4);

        test_array2.insert(2U, 5);
        CHECK(test_array2[0U] == 0);
        CHECK(test_array2[1U] == 1);
        CHECK(test_array2[2U] == 5);
        CHECK(test_array2[3U] == 2);
        CHECK(test_array2[4U] == 3);

        test_array2.insert(3U, 6);
        CHECK(test_array2[0U] == 0);
        CHECK(test_array2[1U] == 1);
        CHECK(test_array2[2U] == 5);
        CHECK(test_array2[3U] == 6);
        CHECK(test_array2[4U] == 2);
    }
}

class A
{
public:
    s32 b;
};

inline auto init_array_shared_pointers_A()
{
    array_shared_pointers<A, 10U> test_array1;
    test_array1[0U] = msptr<A>(A{1});
    test_array1[1U] = msptr<A>(A{-2});
    test_array1[2U] = msptr<A>(A{3});
    test_array1[3U] = msptr<A>(A{-2});
    test_array1[4U] = msptr<A>(A{5});
    test_array1[5U] = msptr<A>(A{100});
    test_array1[6U] = msptr<A>(A{-2});
    test_array1[7U] = msptr<A>(A{-2});
    test_array1[8U] = msptr<A>(A{-1});
    test_array1[9U] = msptr<A>(A{0});
    return test_array1;
}

TEST_CASE("array of shared pointers", "[array][sptr]")
{
    SECTION("Copy empty")
    {
        array_shared_pointers<A, 10U> empty_array;
        CHECK(empty_array.size() == 10U);

        array_shared_pointers<A, 10U> copy_empty(empty_array);
        CHECK(copy_empty.size() == empty_array.size());
    }

    SECTION("Move")
    {
        auto test_array1(init_array_shared_pointers_A());
        CHECK(test_array1.size() == 10U);

        array_shared_pointers<A, 10U> test_array2 = htps::move(test_array1);

        CHECK(test_array2.size() == 10U);

        CHECK_FALSE(test_array1[0U]);
        CHECK_FALSE(test_array1[2U]);
        CHECK_FALSE(test_array1[3U]);
        CHECK_FALSE(test_array1[4U]);
        CHECK_FALSE(test_array1[5U]);
        CHECK_FALSE(test_array1[6U]);
        CHECK_FALSE(test_array1[7U]);
        CHECK_FALSE(test_array1[8U]);
        CHECK_FALSE(test_array1[9U]);

        CHECK(test_array2[0U]);
        CHECK(test_array2[2U]);
        CHECK(test_array2[3U]);
        CHECK(test_array2[4U]);
        CHECK(test_array2[5U]);
        CHECK(test_array2[6U]);
        CHECK(test_array2[7U]);
        CHECK(test_array2[8U]);
        CHECK(test_array2[9U]);

        CHECK(test_array2[0U]->b == 1);
        CHECK(test_array2[1U]->b == -2);
        CHECK(test_array2[2U]->b == 3);
        CHECK(test_array2[3U]->b == -2);
        CHECK(test_array2[4U]->b == 5);
        CHECK(test_array2[5U]->b == 100);
        CHECK(test_array2[6U]->b == -2);
        CHECK(test_array2[7U]->b == -2);
        CHECK(test_array2[8U]->b == -1);
        CHECK(test_array2[9U]->b == 0);

        SECTION("Insert")
        {
            SECTION("At the begining")
            {
                test_array2.insert(0U, msptr<A>(A{42}));
                CHECK(test_array2[0U]->b == 42);
                CHECK(test_array2[1U]->b == 1);
                CHECK(test_array2[2U]->b == -2);
                CHECK(test_array2[3U]->b == 3);
                CHECK(test_array2[4U]->b == -2);
                CHECK(test_array2[5U]->b == 5);
                CHECK(test_array2[6U]->b == 100);
                CHECK(test_array2[7U]->b == -2);
                CHECK(test_array2[8U]->b == -2);
                CHECK(test_array2[9U]->b == -1);

                test_array2.insert(0U, msptr<A>(A{348}));
                CHECK(test_array2[0U]->b == 348);
                CHECK(test_array2[1U]->b == 42);
                CHECK(test_array2[2U]->b == 1);
                CHECK(test_array2[3U]->b == -2);
                CHECK(test_array2[4U]->b == 3);
                CHECK(test_array2[5U]->b == -2);
                CHECK(test_array2[6U]->b == 5);
                CHECK(test_array2[7U]->b == 100);
                CHECK(test_array2[8U]->b == -2);
                CHECK(test_array2[9U]->b == -2);

                SECTION("At the end")
                {
                    test_array2.insert(9U, msptr<A>(A{-8}));
                    CHECK(test_array2[0U]->b == 348);
                    CHECK(test_array2[1U]->b == 42);
                    CHECK(test_array2[2U]->b == 1);
                    CHECK(test_array2[3U]->b == -2);
                    CHECK(test_array2[4U]->b == 3);
                    CHECK(test_array2[5U]->b == -2);
                    CHECK(test_array2[6U]->b == 5);
                    CHECK(test_array2[7U]->b == 100);
                    CHECK(test_array2[8U]->b == -2);
                    CHECK(test_array2[9U]->b == -8);

                    test_array2.insert(9U, msptr<A>(A{-4}));
                    CHECK(test_array2[0U]->b == 348);
                    CHECK(test_array2[1U]->b == 42);
                    CHECK(test_array2[2U]->b == 1);
                    CHECK(test_array2[3U]->b == -2);
                    CHECK(test_array2[4U]->b == 3);
                    CHECK(test_array2[5U]->b == -2);
                    CHECK(test_array2[6U]->b == 5);
                    CHECK(test_array2[7U]->b == 100);
                    CHECK(test_array2[8U]->b == -2);
                    CHECK(test_array2[9U]->b == -4);
                }

                SECTION("On the middle")
                {
                    test_array2.insert(5U, msptr<A>(A{10000}));
                    CHECK(test_array2[0U]->b == 348);
                    CHECK(test_array2[1U]->b == 42);
                    CHECK(test_array2[2U]->b == 1);
                    CHECK(test_array2[3U]->b == -2);
                    CHECK(test_array2[4U]->b == 3);
                    CHECK(test_array2[5U]->b == 10000);
                    CHECK(test_array2[6U]->b == -2);
                    CHECK(test_array2[7U]->b == 5);
                    CHECK(test_array2[8U]->b == 100);
                    CHECK(test_array2[9U]->b == -2);

                    test_array2.insert(5U, msptr<A>(A{9999}));
                    CHECK(test_array2[0U]->b == 348);
                    CHECK(test_array2[1U]->b == 42);
                    CHECK(test_array2[2U]->b == 1);
                    CHECK(test_array2[3U]->b == -2);
                    CHECK(test_array2[4U]->b == 3);
                    CHECK(test_array2[5U]->b == 9999);
                    CHECK(test_array2[6U]->b == 10000);
                    CHECK(test_array2[7U]->b == -2);
                    CHECK(test_array2[8U]->b == 5);
                    CHECK(test_array2[9U]->b == 100);
                }
            }
        }
    }
}

TEST_CASE("array unique ptr", "[array][uptr]")
{
    array_unique_pointers<s32, 4U> test_array1;
    array_unique_pointers<s32, 4U> test_array2;

    CHECK(test_array1.size() == 4U);
    CHECK(test_array2.size() == 4U);

    test_array1[0U] = muptr<s32>(1);
    test_array2[0U] = muptr<s32>(1);

    CHECK((*test_array1[0U]) == (*test_array2[0U]));

    test_array1[1U] = muptr<s32>(2);
    test_array2[1U] = muptr<s32>(2);

    CHECK((*test_array1[1U]) == (*test_array2[1U]));

    CHECK_FALSE(test_array1[2U]);
    CHECK_FALSE(test_array2[2U]);

    *test_array1[0U] = 2;
    uptr<s32> t      = htps::move(test_array1[0U]);
    CHECK(*t == 2);
    CHECK_FALSE(test_array1[0U]);
    test_array1[0U] = htps::move(test_array2[0U]);
    CHECK_FALSE(test_array2[0U]);
    CHECK(test_array1[0U]);
}
