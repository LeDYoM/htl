#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("shared_ptr::shared_ptr", "[types][shared_ptr]")
{
    SECTION("null ptr")
    {
        SECTION("Default constructor")
        {
            sptr<int> a;
            CHECK(a.use_count() == 0U);
            sptr<int> b{a};
            CHECK(b.use_count() == 0U);
        }

        SECTION("nullptr constructor")
        {
            sptr<int> a{nullptr};
            CHECK(a.use_count() == 0U);
            sptr<int> b{a};
            CHECK(b.use_count() == 0U);
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
                    sptr<int> b{a};
                    CHECK(*a == 42);
                    CHECK(*b == 42);
                    CHECK(b.use_count() == 2U);
                }
                CHECK(a.use_count() == 1U);
            }
            CHECK(p != nullptr);
        }

        SECTION("Construct from prvalue")
        {
            sptr<int> a{new int{42}};
            CHECK(*a == 42);
            CHECK(a.use_count() == 1U);
            {
                sptr<int> b{a};
                CHECK(*a == 42);
                CHECK(*b == 42);
                CHECK(b.use_count() == 2U);
            }
            CHECK(a.use_count() == 1U);
        }
    }
}

TEST_CASE("shared_ptr move constructors", "[types][shared_ptr]")
{
    SECTION("With nullptr")
    {
        sptr<int> a;
        CHECK(a.use_count() == 0U);
        sptr<int> b{htps::move(a)};
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 0U);
    }

    SECTION("With value")
    {
        sptr<int> a{new int{42}};
        CHECK(*a == 42);
        CHECK(a.use_count() == 1U);
        sptr<int> b{htps::move(a)};
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 1U);
    }
}

TEST_CASE("shared_ptr move assignment", "[types][shared_ptr]")
{
    SECTION("With nullptr")
    {
        sptr<int> a;
        CHECK(a.use_count() == 0U);
        sptr<int> b;
        b = htps::move(a);
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 0U);

        sptr<int> c{new int{42}};
        CHECK(c.use_count() == 1U);
        a = htps::move(c);
        CHECK(a.use_count() == 1U);
        CHECK(c.use_count() == 0U);
    }

    SECTION("With value")
    {
        {
            sptr<int> a{new int{42}};
            CHECK(*a == 42);
            CHECK(a.use_count() == 1U);
            sptr<int> b;
            b = htps::move(a);
            CHECK(a.use_count() == 0U);
            CHECK(b.use_count() == 1U);
        }

        {
            sptr<int> c{new int{42}};
            CHECK(c.use_count() == 1U);
            sptr<int> a;
            a = htps::move(c);
            CHECK(a.use_count() == 1U);
            CHECK(c.use_count() == 0U);
        }
    }
}

TEST_CASE("shared_ptr copy assignment", "[types][shared_ptr]")
{
    SECTION("With nullptr")
    {
        sptr<int> a;
        CHECK(a.use_count() == 0U);
        sptr<int> b{htps::move(a)};
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 0U);

        sptr<int> c;
        c = b;
        CHECK(b.use_count() == 0U);
        CHECK(c.use_count() == 0U);
    }

    SECTION("With value")
    {
        sptr<int> a{new int{42}};
        CHECK(*a == 42);
        CHECK(a.use_count() == 1U);
        sptr<int> b{htps::move(a)};
        CHECK(a.use_count() == 0U);
        CHECK(b.use_count() == 1U);

        {
            sptr<int> c;
            c = b;
            CHECK(b.use_count() == 2U);
            CHECK(c.use_count() == 2U);

            {
                sptr<int> d{new int{126}};
                CHECK(d.use_count() == 1U);
                c = d;
                CHECK(c.use_count() == 2U);
                CHECK(d.use_count() == 2U);

                CHECK(*c == 126);
                CHECK(*d == 126);
                CHECK(*b == 42);
            }
            CHECK(*c == 126);
            CHECK(c.use_count() == 1U);
        }
        CHECK(*b == 42);
        CHECK(b.use_count() == 1);
    }
}

TEST_CASE("shared_ptr to const", "[types][shared_ptr]")
{
    std::shared_ptr<int const> a{new int{42}};
}

TEST_CASE("shared_ptr leak testing", "[types][shared_ptr]")
{
    sptr<int> a{new int{42}};
    CHECK(*a == 42);
    CHECK(a.use_count() == 1U);
    sptr<int> b{a};
    CHECK(a.use_count() == 2U);
    CHECK(b.use_count() == 2U);

    a = sptr<int>{new int{12}};
    CHECK(*a == 12);
    CHECK(*b == 42);
    CHECK(a.use_count() == 1U);
    CHECK(b.use_count() == 1U);

    a = htps::move(b);
    CHECK(*a == 42);
    CHECK(a.use_count() == 1U);
    b.reset();
    CHECK(b.use_count() == 0U);

    a = sptr<int>{nullptr};
    CHECK(a.use_count() == 0U);
    a = htps::move(b);
    CHECK(a.use_count() == 0U);
    CHECK(b.use_count() == 0U);

    a.reset();
    CHECK(a.use_count() == 0U);

    sptr<int> e{nullptr};
    CHECK(e.use_count() == 0U);

    a = e;
    CHECK(e.use_count() == 0U);
    CHECK(a.use_count() == 0U);

    a.reset(new int{4});
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

}  // namespace

TEST_CASE("shared_ptr dynamic_pointer_cast", "[types][shared_ptr]")
{
    SECTION("Const reference")
    {
        sptr<Base> base_ptr{msptr<Base>(56)};
        CHECK(base_ptr->a == 56);
        CHECK(base_ptr.use_count() == 1U);

        sptr<Derived> derived_ptr;
        derived_ptr = dynamic_pointer_cast<Derived>(base_ptr);
        CHECK(derived_ptr == nullptr);
        CHECK(base_ptr.use_count() == 1U);
        CHECK(derived_ptr.use_count() == 0U);

        base_ptr = msptr<Derived>(12, 48);
        CHECK(base_ptr->a == 12);
        CHECK(base_ptr.use_count() == 1U);
        derived_ptr = dynamic_pointer_cast<Derived>(base_ptr);
        CHECK(base_ptr.use_count() == 2U);
        CHECK(derived_ptr.use_count() == 2U);
        CHECK(derived_ptr != nullptr);
        CHECK(derived_ptr->a == 12);
        CHECK(derived_ptr->b == 48);
        CHECK(base_ptr->a == 12);
    }

    SECTION("RValue")
    {
        sptr<Base> base_ptr{msptr<Base>(56)};
        CHECK(base_ptr->a == 56);
        CHECK(base_ptr.use_count() == 1U);

        sptr<Derived> derived_ptr;
        derived_ptr = dynamic_pointer_cast<Derived>(htps::move(base_ptr));
        CHECK(derived_ptr == nullptr);
        CHECK(derived_ptr.use_count() == 0U);

        base_ptr = msptr<Derived>(12, 48);
        CHECK(base_ptr->a == 12);
        derived_ptr = dynamic_pointer_cast<Derived>(htps::move(base_ptr));
        CHECK(base_ptr == nullptr);
        CHECK(derived_ptr != nullptr);
        CHECK(derived_ptr->a == 12);
        CHECK(derived_ptr->b == 48);
    }
}

TEST_CASE("shared_ptr static_pointer_cast", "[types][shared_ptr]")
{
    SECTION("Const reference")
    {
        sptr<Base> base_ptr{msptr<Base>(56)};
        CHECK(base_ptr->a == 56);
        CHECK(base_ptr.use_count() == 1U);

        sptr<Derived> derived_ptr;
        derived_ptr = static_pointer_cast<Derived>(base_ptr);
        CHECK(derived_ptr != nullptr);
        CHECK(base_ptr.use_count() == 2U);
        CHECK(derived_ptr.use_count() == 2U);

        base_ptr = msptr<Derived>(12, 48);
        CHECK(base_ptr->a == 12);
        CHECK(base_ptr.use_count() == 1U);
        derived_ptr = static_pointer_cast<Derived>(base_ptr);
        CHECK(base_ptr.use_count() == 2U);
        CHECK(derived_ptr.use_count() == 2U);
        CHECK(derived_ptr != nullptr);
        CHECK(derived_ptr->a == 12);
        CHECK(derived_ptr->b == 48);
        CHECK(base_ptr->a == 12);
    }

    SECTION("RValue")
    {
        sptr<Base> base_ptr{msptr<Base>(56)};
        CHECK(base_ptr->a == 56);
        CHECK(base_ptr.use_count() == 1U);

        sptr<Derived> derived_ptr;
        derived_ptr = static_pointer_cast<Derived>(htps::move(base_ptr));
        CHECK(derived_ptr != nullptr);
        CHECK(derived_ptr.use_count() == 1U);

        base_ptr = msptr<Derived>(12, 48);
        CHECK(base_ptr->a == 12);
        derived_ptr = static_pointer_cast<Derived>(htps::move(base_ptr));
        CHECK(base_ptr == nullptr);
        CHECK(derived_ptr != nullptr);
        CHECK(derived_ptr->a == 12);
        CHECK(derived_ptr->b == 48);
    }
}

namespace
{
struct IncompleteType;
}

TEST_CASE("shared_ptr incomplete type", "[types][shared_ptr]")
{
    sptr<IncompleteType> a;
    CHECK(a.use_count() == 0U);
    CHECK(a == nullptr);
}

struct A
{
    virtual ~A() = default;
};

struct B
{
    virtual ~B() = default;
};

struct AB : public A, public B
{
    ~AB() override = default;
};

TEST_CASE("shared_ptr destroy pointer to base from another base",
          "[types][shared_ptr]")
{
    sptr<B> ab{msptr<AB>()};
    CHECK(ab.use_count() == 1U);
    ab.reset();
    CHECK(ab == nullptr);
}

TEST_CASE("shared_ptr assign another shared_ptr to base", "[types][shared_ptr]")
{
    sptr<B> ab1{msptr<AB>()};
    sptr<B> ab2{msptr<AB>()};
    ab2 = ab1;
    CHECK(ab1.use_count() == 2U);
    CHECK(ab2.use_count() == 2U);
    ab1.reset();
    CHECK(ab1 == nullptr);
    CHECK(ab2.use_count() == 1U);
    ab2.reset();
    CHECK(ab2 == nullptr);
}

TEST_CASE("shared_ptr move another shared_ptr to base", "[types][shared_ptr]")
{
    sptr<B> ab1{msptr<AB>()};
    sptr<B> ab2{msptr<AB>()};
    ab2 = htps::move(ab1);
    CHECK(ab1.use_count() == 1U);
    CHECK(ab1 != nullptr);
    CHECK(ab2.use_count() == 1U);
    CHECK(ab2 != nullptr);
    ab1.reset();
    CHECK(ab1 == nullptr);
    ab2.reset();
    CHECK(ab2 == nullptr);
}

TEST_CASE("shared_ptr assign another shared_ptr to base different types",
          "[types][shared_ptr]")
{
    sptr<AB> ab1{msptr<AB>()};
    sptr<B> ab2{msptr<AB>()};
    ab2 = ab1;
    CHECK(ab1.use_count() == 2U);
    CHECK(ab2.use_count() == 2U);
    ab1.reset();
    CHECK(ab1 == nullptr);
    CHECK(ab2.use_count() == 1U);
    ab2.reset();
    CHECK(ab2 == nullptr);
}

TEST_CASE("shared_ptr move another shared_ptr to base different types",
          "[types][shared_ptr]")
{
    sptr<B> ab1{msptr<AB>()};
    sptr<B> ab2{msptr<AB>()};
    ab2 = htps::move(ab1);
    CHECK(ab1.use_count() == 1U);
    CHECK(ab1 != nullptr);
    CHECK(ab2.use_count() == 1U);
    CHECK(ab2 != nullptr);
    ab1.reset();
    CHECK(ab1 == nullptr);
    ab2.reset();
    CHECK(ab2 == nullptr);
}
