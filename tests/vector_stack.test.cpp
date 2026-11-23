#include "catch_include.hpp"
#include <array>

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

TEST_CASE("vector_stack::vector_stack std::initializer_list constructor",
          "[vector_stack][std::initializer_list]")
{
    {
        vector_stack<s32, 5> v{-2, -1, 0, 1, 2};
        CHECK(v.size() == 5U);
        CHECK(v.capacity() == 5U);
        CHECK(v.full());
        CHECK(v[0] == -2);
        CHECK(v[1] == -1);
        CHECK(v[2] == 0);
        CHECK(v[3] == 1);
        CHECK(v[4] == 2);
    }

    {
        vector_stack<s32, 5> v{-2, -1};
        CHECK(v.size() == 2U);
        CHECK(v.capacity() == 5U);
        CHECK_FALSE(v.full());
        CHECK(v[0] == -2);
        CHECK(v[1] == -1);
    }

    {
        vector_stack<s32, 5> v{-2, -1, 0, 1, 2, 3, 4, 5};
        CHECK(v.size() == 5U);
        CHECK(v.capacity() == 5U);
        CHECK(v.full());
        CHECK(v[0] == -2);
        CHECK(v[1] == -1);
        CHECK(v[2] == 0);
        CHECK(v[3] == 1);
        CHECK(v[4] == 2);
    }
}

namespace
{
struct alignas(std::max_align_t) TestingDummy
{
    int a;
    int b;

    bool operator==(TestingDummy const&) const noexcept = default;
};
}  // namespace

TEST_CASE("vector_stack::back() front()", "[vector_stack]")
{
    vector_stack<TestingDummy, 4> v;

    v.emplace_back(5, 3);
    v.emplace_back(23, 45);

    CHECK(v.size() == 2U);
    CHECK(v.front() == TestingDummy{5, 3});
    CHECK(v.back() == TestingDummy{23, 45});

    v.emplace_back(329, 65536);
    CHECK(v.size() == 3U);
    CHECK(v.front() == TestingDummy{5, 3});
    CHECK(v.back() == TestingDummy{329, 65536});

    v.emplace_back(100000000, 993345664);

    CHECK(v.size() == 4U);
    CHECK(v.front() == TestingDummy{5, 3});
    CHECK(v[1] == TestingDummy{23, 45});
    CHECK(v[2] == TestingDummy{329, 65536});
    CHECK(v.back() == TestingDummy{100000000, 993345664});
}

TEST_CASE("vector_stack::push and pop", "[vector_stack]")
{
    vector_stack<TestingDummy, 4> v;
    CHECK(v.begin() == v.end());

    CHECK(v.empty());
    CHECK(v.size() == 0U);

    v.push_back(TestingDummy{1, 2});
    v.emplace_back(4, 5);
    v.push_back(TestingDummy{6, 7});
    v.emplace_back(8, 9);

    CHECK(v.full());
    CHECK(v.size() == 4U);

    CHECK(v.back() == TestingDummy{8, 9});

    v.pop_back();

    CHECK_FALSE(v.full());
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 3U);

    CHECK(v.back() == TestingDummy{6, 7});

    v.pop_back();

    CHECK_FALSE(v.full());
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 2U);

    CHECK(v.back() == TestingDummy{4, 5});

    v.pop_back();

    CHECK_FALSE(v.full());
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 1U);

    CHECK(v.back() == TestingDummy{1, 2});

    v.pop_back();

    CHECK_FALSE(v.full());
    CHECK(v.empty());
    CHECK(v.size() == 0U);

    CHECK(v.begin() == v.end());
}

TEST_CASE("vector_stack::comparison_operators", "[vector_stack]")
{
    vector_stack<TestingDummy, 4> v;
    vector_stack<TestingDummy, 4> v2;

    v.push_back(TestingDummy{55, 987});
    v2.emplace_back(56, 987);

    v.pop_back();
    v2.pop_back();

    CHECK(v == v2);
}

TEST_CASE("vector_stack::comparison operators", "[vector_stack]")
{
    vector_stack<TestingDummy, 4> v;
    vector_stack<TestingDummy, 4> v2;

    CHECK(v == v2);

    v.push_back(TestingDummy{56, 987});
    CHECK(v != v2);
    v2.emplace_back(56, 987);

    CHECK(v == v2);
    v.emplace_back(567567, 10000);
    CHECK(v != v2);

    v2.push_back(TestingDummy{567568, 10000});
    CHECK(v != v2);

    v2.pop_back();
    v2.push_back(TestingDummy{567567, 10000});

    CHECK(v2 == v);

    v.pop_back();
    CHECK_FALSE(v2 == v);
    v.push_back(TestingDummy{567568, 10000});
    CHECK_FALSE(v2 == v);

    v.clear();
    v2.clear();

    CHECK(v.size() == 0U);
    CHECK(v2.size() == 0U);
    CHECK(v == v2);
}

TEST_CASE("vector_stack::moving", "[vector_stack]")
{
    SECTION("With shared ptrs")
    {
        SECTION("Move constructor")
        {
            vector_stack<sptr<s32>, 5> v{msptr<s32>(-2), msptr<s32>(-1),
                                         msptr<s32>(0), msptr<s32>(1),
                                         msptr<s32>(2)};

            CHECK(v.size() == 5);
            CHECK(v.full());

            vector_stack<sptr<s32>, 5> v2{move(v)};

            CHECK(v2.size() == 5);
            CHECK(v2.full());
            CHECK(v.empty());
        }

        SECTION("Move assignment")
        {
            vector_stack<sptr<s32>, 5> v{msptr<s32>(-2), msptr<s32>(-1),
                                         msptr<s32>(0), msptr<s32>(1),
                                         msptr<s32>(2)};

            CHECK(v.size() == 5);
            CHECK(v.full());

            vector_stack<sptr<s32>, 5> v2{msptr<s32>(50)};
            CHECK(v2.size() == 1U);
            v2 = move(v);

            CHECK(v2.size() == 5);
            CHECK(v2.full());
            CHECK(v.empty());
        }
    }

    vector_stack<uptr<s32>, 5> g;

    SECTION("With unique ptrs")
    {
        SECTION("Move constructor")
        {
            vector_stack<uptr<s32>, 5> v;

            v.push_back(muptr<s32>(-2));
            v.push_back(muptr<s32>(-1));
            v.push_back(muptr<s32>(0));
            v.push_back(muptr<s32>(1));
            v.push_back(muptr<s32>(2));

            CHECK(v.size() == 5);
            CHECK(v.full());

            vector_stack<uptr<s32>, 5> v2{move(v)};

            CHECK(v2.size() == 5);
            CHECK(v2.full());
            CHECK(v.empty());
        }

        SECTION("Move assignment")
        {
            vector_stack<uptr<s32>, 5> v;

            v.push_back(muptr<s32>(-2));
            v.push_back(muptr<s32>(-1));
            v.push_back(muptr<s32>(0));
            v.push_back(muptr<s32>(1));
            v.push_back(muptr<s32>(2));

            CHECK(v.size() == 5);
            CHECK(v.full());

            vector_stack<uptr<s32>, 5> v2;
            v2.emplace_back(muptr<s32>(50));
            CHECK(v2.size() == 1U);
            v2 = move(v);

            CHECK(v2.size() == 5);
            CHECK(v2.full());
            CHECK(v.empty());
        }
    }
}

struct Char1
{
    char a;
    bool operator==(Char1 const&) const noexcept = default;
};

constexpr u32 kAlignment{1024};

struct alignas(kAlignment) Char1_kAlignment
{
    char a;
    bool operator==(Char1_kAlignment const&) const noexcept = default;
};

struct alignas(std::max_align_t) Char1_MaxAlign
{
    char a;
    bool operator==(Char1_MaxAlign const&) const noexcept = default;
};

TEST_CASE("vector_stack::overalignment", "[vector_stack]")
{
    if constexpr (sizeof(Char1_kAlignment) == kAlignment)
    {
        vector_stack<Char1_kAlignment, 2> data;

        CHECK(data.element_size() == sizeof(Char1_kAlignment));
        CHECK(data.element_offset(1U) == sizeof(Char1_kAlignment));

        static_assert(vector_stack<Char1_kAlignment, 2>::element_size() ==
                      sizeof(Char1_kAlignment));

        static_assert(vector_stack<Char1_kAlignment, 2>::element_offset(1U) ==
                      sizeof(Char1_kAlignment));

        CHECK_FALSE(data.full());

        data.push_back(Char1_kAlignment{5});
        data.emplace_back((char)12);

        CHECK(data[0].a == 5);
        CHECK(data[1] == Char1_kAlignment{(char)12});
        CHECK(data.full());
    }

    {
        vector_stack<Char1_MaxAlign, 2> data;

        CHECK(data.element_size() == sizeof(Char1_MaxAlign));
        CHECK(data.element_offset(1U) == sizeof(Char1_MaxAlign));

        static_assert(vector_stack<Char1_MaxAlign, 2>::element_size() ==
                      sizeof(Char1_MaxAlign));

        static_assert(vector_stack<Char1_MaxAlign, 2>::element_offset(1U) ==
                      sizeof(Char1_MaxAlign));

        CHECK_FALSE(data.full());

        data.push_back(Char1_MaxAlign{7});
        data.emplace_back((char)14);

        CHECK(data[0].a == 7);
        CHECK(data[1] == Char1_MaxAlign{(char)14});
        CHECK(data.full());
    }

    {
        vector_stack<Char1, 2> data;

        CHECK(data.element_size() == sizeof(Char1));
        CHECK(data.element_offset(1U) == sizeof(Char1));

        static_assert(vector_stack<Char1, 2>::element_size() ==
                      sizeof(Char1));

        static_assert(vector_stack<Char1, 2>::element_offset(1U) ==
                      sizeof(Char1));

        CHECK_FALSE(data.full());

        data.push_back(Char1{9});
        data.emplace_back((char)16);

        CHECK(data[0].a == 9);
        CHECK(data[1] == Char1{(char)16});
        CHECK(data.full());
    }
}
