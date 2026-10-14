#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("weak_ptr::weak_ptr", "[types][weak_ptr][shared_ptr]")
{
    SECTION("null ptr")
    {
        SECTION("Default constructor")
        {
            sptr<int> a;
            CHECK(a.use_count() == 0U);
            wptr<int> b{a};
            CHECK(b.use_count() == 0U);
            CHECK(b.expired());
        }

        SECTION("nullptr constructor")
        {
            sptr<int> a{nullptr};
            CHECK(a.use_count() == 0U);
            wptr<int> b{a};
            CHECK(b.use_count() == 0U);
            CHECK(b.expired());
        }
    }

    SECTION("no null ptr")
    {
        SECTION("Construct from rvalue")
        {
            int* p{new int{42}};

            {
                sptr<int> a{htps::move(p)};
                CHECK(*a == 42);
                CHECK(a.use_count() == 1U);
                {
                    wptr<int> b{a};
                    CHECK(*a == 42);
                    CHECK(*(b.lock()) == 42);
                    CHECK(b.use_count() == 1U);
                }
                CHECK(a.use_count() == 1U);
            }
        }

        SECTION("Construct from prvalue")
        {
            sptr<int> a{new int{42}};
            CHECK(*a == 42);
            CHECK(a.use_count() == 1U);
            {
                wptr<int> b{a};
                CHECK(*a == 42);
                CHECK(*(b.lock()) == 42);
                CHECK(b.use_count() == 1U);
            }
            CHECK(a.use_count() == 1U);
        }
    }
}

TEST_CASE("weak_ptr move constructors", "[types][weak_ptr][shared_ptr]")
{
    SECTION("With nullptr")
    {
        sptr<int> a;
        CHECK(a.use_count() == 0U);
        wptr<int> b{htps::move(a)};
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 0U);
    }

    SECTION("With value")
    {
        sptr<int> a{new int{42}};
        CHECK(*a == 42);
        CHECK(a.use_count() == 1U);
        wptr<int> b{htps::move(a)};
        wptr<int> c{htps::move(b)};
        CHECK(a.use_count() == 1U);
        CHECK(b.use_count() == 0U);
        CHECK(c.use_count() == 1U);
    }
}

TEST_CASE("weak_ptr move assignment", "[types][weak_ptr][shared_ptr]")
{
    wptr<int> a;
    CHECK(a.use_count() == 0U);
    CHECK(a.lock() == nullptr);

    sptr<int> b{new int(12)};
    CHECK(b.use_count() == 1U);

    a = b;

    CHECK(a.use_count() == 1U);
    CHECK((*a.lock()) == 12);

    wptr<int> d{htps::move(a)};
    CHECK(d.use_count() == 1U);
    CHECK(a.use_count() == 0U);

    sptr<int> c{new int(12)};
    b = c;
    CHECK(d.use_count() == 0U);
}

TEST_CASE("weak_ptr copy assignment", "[types][weak_ptr][shared_ptr]")
{
    wptr<int> a;
    CHECK(a.use_count() == 0U);
    CHECK(a.lock() == nullptr);
    sptr<int> b{new int(12)};
    CHECK(b.use_count() == 1U);
    a = b;
    CHECK(a.use_count() == 1U);
    CHECK((*a.lock()) == 12);

    wptr<int> d;
    CHECK(d.use_count() == 0U);
    CHECK(a.use_count() == 1U);

    d = a;
    CHECK(d.use_count() == 1U);
    CHECK(a.use_count() == 1U);

    sptr<int> c{new int(12)};
    b = c;
    CHECK(d.use_count() == 0U);
    CHECK(a.use_count() == 0U);
}

TEST_CASE("weak_ptr leak testing", "[types][weak_ptr][shared_ptr]")
{
    sptr<int> a{new int{42}};
    CHECK(*a == 42);
    CHECK(a.use_count() == 1U);
    sptr<int> b{a};
    CHECK(a.use_count() == 2U);
    CHECK(b.use_count() == 2U);
    wptr<int> c{a};
    wptr<int> d{b};
    CHECK(c.use_count() == 2U);
    CHECK(d.use_count() == 2U);

    a = sptr<int>{new int{12}};
    CHECK(*a == 12);
    CHECK(*b == 42);
    CHECK(a.use_count() == 1U);
    CHECK(b.use_count() == 1U);
    CHECK(c.use_count() == 1U);
    CHECK(d.use_count() == 1U);
    CHECK(*(c.lock()) == 42);
    CHECK(*(d.lock()) == 42);

    a = htps::move(b);
    CHECK(*a == 42);
    CHECK(a.use_count() == 1U);
    b.reset();
    CHECK(b.use_count() == 0U);
    CHECK(c.use_count() == 1U);
    CHECK(d.use_count() == 1U);
    CHECK(*(c.lock()) == 42);
    CHECK(*(d.lock()) == 42);

    a = sptr<int>{nullptr};
    CHECK(a.use_count() == 0U);
    c = a;
    a = msptr<int>(55);
    CHECK(a.use_count() == 1U);
    CHECK(c.use_count() == 0U);
    CHECK(c.lock() == nullptr);

    a.reset();
    CHECK(a.use_count() == 0U);
    CHECK(c.use_count() == 0U);
}

namespace
{
struct IncompleteType;
}

TEST_CASE("weak_ptr incomplete type", "[types][shared_ptr][weak_ptr]")
{
    sptr<IncompleteType> a;
    wptr<IncompleteType> b;
    b = htps::move(a);
    b = a;
    CHECK(a.use_count() == 0U);
    CHECK(a == nullptr);
    CHECK(b.use_count() == 0U);
    CHECK(b == nullptr);
}

TEST_CASE("weak_ptr destroy_shared", "[types][shared_ptr][weak_ptr]")
{
    sptr<int> a;
    wptr<int> b{a};
    b = htps::move(a);
    b = a;
    CHECK(a.use_count() == 0U);
    CHECK(a == nullptr);
    CHECK(b.use_count() == 0U);
    CHECK(b == nullptr);
}
