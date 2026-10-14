#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("unique_ptr::unique_ptr", "[types][unique_ptr]")
{
    SECTION("null ptr")
    {
        SECTION("Default constructor")
        {
            uptr<int> a;
            CHECK(a == nullptr);
        }

        SECTION("nullptr constructor")
        {
            uptr<int> a{nullptr};
            uptr<int> b{htps::move(a)};
            CHECK(b == nullptr);
        }
    }

    SECTION("no null ptr")
    {
        SECTION("Construct from rvalue")
        {
            int* p{new int{42}};

            {
                uptr<int> a{htps::move(p)};
                CHECK(*a == 42);
                {
                    uptr<int> b{htps::move(a)};
                    CHECK(a == nullptr);
                    CHECK(*b == 42);
                }
            }
        }

        SECTION("Construct from prvalue")
        {
            uptr<int> a{new int{42}};
            CHECK(*a == 42);
            {
                uptr<int> b{htps::move(a)};
                CHECK(a == nullptr);
                CHECK(*b == 42);
            }
        }
    }
}

TEST_CASE("unique_ptr move constructors", "[types][unique_ptr]")
{
    SECTION("With nullptr")
    {
        uptr<int> a;
        CHECK(a == nullptr);
        uptr<int> b{htps::move(a)};
        CHECK(a == nullptr);
        CHECK(b == nullptr);
    }

    SECTION("With value")
    {
        uptr<int> a{new int{42}};
        CHECK(*a == 42);
        sptr<int> b{htps::move(a)};
        CHECK(a == nullptr);
        CHECK(*b == 42);
    }
}

TEST_CASE("unique_ptr move assignment", "[types][unique_ptr]")
{
    SECTION("With nullptr")
    {
        uptr<int> a;
        CHECK(a == nullptr);
        uptr<int> b{htps::move(a)};
        CHECK(a == nullptr);
        CHECK(b == nullptr);

        uptr<int> c{new int{42}};
        CHECK(*c == 42);
        a = htps::move(c);
        CHECK(*a == 42);
        CHECK(c == nullptr);
    }

    SECTION("With value")
    {
        uptr<int> a{new int{42}};
        CHECK(*a == 42);
        uptr<int> b{htps::move(a)};
        CHECK(a == nullptr);
        CHECK(*b == 42);

        uptr<int> c{new int{42}};
        CHECK(*c == 42);
        a = htps::move(c);
        CHECK(*a == 42);
        CHECK(c == nullptr);

        auto p{a.release()};
        CHECK(*p == 42);
        delete p;
    }
}

TEST_CASE("make_unique", "[types][unique_ptr]")
{
    uptr<int> a{muptr<int>(7)};
    CHECK(*a == 7);

    a.reset();
    CHECK(a == nullptr);
}

TEST_CASE("shared_ptr with unique_ptr", "[types][unique_ptr][shared_ptr]")
{
    SECTION("Construct sptr from uptr")
    {
        uptr<int> a{new int{42}};
        sptr<int> b{htps::move(a)};
        CHECK(a == nullptr);
        CHECK(b.use_count() == 1U);
    }

    SECTION("Assign to sptr from uptr")
    {
        uptr<int> a{new int{42}};
        sptr<int> b;
        CHECK(b.use_count() == 0U);
        b = htps::move(a);
        CHECK(a == nullptr);
        CHECK(b.use_count() == 1U);
    }
}

namespace
{
struct Base
{
    int a;
    Base(int _a) : a{_a} {}
    virtual ~Base() = default;
};
struct Derived : public Base
{
    int b;
    Derived(int _a, int _b) : Base{_a}, b{_b} {}

    ~Derived() override = default;
};

void set(uptr<Base> base) noexcept
{
    (void)(base);
}

}  // namespace

TEST_CASE("unique_ptr from base", "[types][unique_ptr]")
{
    uptr<Base> b{muptr<Derived>(1, 2)};
    set(htps::move(b));
    set(muptr<Derived>(1, 2));
    uptr<Derived> c{muptr<Derived>(1, 2)};
    set(htps::move(c));
    set(muptr<Base>(2));
}
