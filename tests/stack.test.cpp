#include "catch_include.hpp"

import htypes;

#include <stack>

using namespace htps;

TEST_CASE("stack::stack constructors")
{
    {
        stack<int> a;
        CHECK(a.empty());
        CHECK(a.size() == 0U);
    }

    {
        stack<int> a{2};
        CHECK_FALSE(a.empty());
        CHECK(a.size() == 1U);
        CHECK(a.top() == 2);
    }

    {
        stack<int> a(4);
        CHECK(a.empty());
        CHECK(a.size() == 0U);
        a.push_back(1);
        CHECK_FALSE(a.empty());
        CHECK(a.size() == 1U);
        CHECK(a.top() == 1);
    }

    {
        stack<int> a{4, 5, 6, 7, 8};
        CHECK_FALSE(a.empty());
        CHECK(a.size() == 5U);
        CHECK(a.top() == 8);
        a.pop_back();
        CHECK(a.size() == 4U);
        CHECK(a.top() == 7);
        a.pop_back();
        CHECK(a.size() == 3U);
        CHECK(a.top() == 6);
        a.pop_back();
        CHECK(a.size() == 2U);
        CHECK(a.top() == 5);
        a.pop_back();
        CHECK(a.size() == 1U);
        CHECK(a.top() == 4);
        a.pop_back();
        CHECK(a.size() == 0U);
        CHECK(a.empty());
    }
}

TEST_CASE("stack::clear constructors")
{
    stack<int> a{4, 5, 6, 7, 8};
    CHECK_FALSE(a.empty());
    CHECK(a.size() == 5U);
    CHECK(a.top() == 8);

    a.clear();

    CHECK(a.empty());
    CHECK(a.size() == 0U);

    a.clear();

    CHECK(a.empty());
    CHECK(a.size() == 0U);
}

TEST_CASE("stack::push and pop")
{
    stack<int> a;
    a.push_back(65);
    a.push_back(123);
    CHECK(a.back() == a.top());
    CHECK(a.top() == 123);
    CHECK(a.size() == 2U);
    a.pop_back();
    CHECK(a.back() == a.top());
    CHECK(a.top() == 65);
    CHECK(a.size() == 1U);
    a.pop_back();
    CHECK(a.size() == 0U);
    CHECK(a.empty());
}

TEST_CASE("stack::non copyable class")
{
    stack<uptr<int>> dummy;
    CHECK(dummy.empty());

    dummy.push_back(muptr<int>(4));
    CHECK(dummy.size() == 1U);
    CHECK_FALSE(dummy.empty());

    uptr<int> other{muptr<int>(3)};
    dummy.push_back(htps::move(other));

    CHECK_FALSE(other);
    CHECK(dummy.size() == 2U);
    CHECK(*(dummy.ctop()) == 3U);

    dummy.pop_back();
    CHECK(dummy.size() == 1U);

    dummy.push_back(nullptr);
    CHECK_FALSE(dummy.top());
    CHECK(dummy.size() == 2U);

    dummy.pop_back();
    CHECK(dummy.size() == 1U);
    CHECK(*(dummy.ctop()) == 4U);

    dummy.pop_back();
    CHECK(dummy.size() == 0U);
    CHECK(dummy.empty());
}

TEST_CASE("stack::operator==")
{
    stack<int> object_0;
    stack<int> object_1;

    CHECK(object_0.empty());
    CHECK(object_1.empty());
    CHECK(object_0 == object_1);
    CHECK_FALSE(object_0 != object_1);
    CHECK(object_0.size() == object_1.size());

    object_0.push_back(4);
    CHECK_FALSE(object_0 == object_1);
    CHECK(object_0 != object_1);
    CHECK(object_0.size() != object_1.size());

    object_1.push_back(7);
    CHECK_FALSE(object_0 == object_1);
    CHECK(object_0 != object_1);
    CHECK(object_0.size() == object_1.size());

    CHECK(object_0.cback() == 4);
    object_0.pop_back();
    CHECK_FALSE(object_0 == object_1);
    CHECK(object_0 != object_1);
    CHECK(object_0.size() != object_1.size());

    object_0.push_back(7);
    CHECK(object_0 == object_1);
    CHECK_FALSE(object_0 != object_1);
    CHECK(object_0.size() == object_1.size());
}

TEST_CASE("stack of shared pointers")
{
    stack<sptr<int>> dummy{msptr<int>(3), msptr<int>(4), msptr<int>(5)};
    CHECK(dummy.size() == 3);
    sptr result = move(dummy.back());
    CHECK(*result == 5);
    dummy.pop_back();
    dummy.push_back(msptr<int>(8));
    CHECK(*(dummy.cback()) == 8);
}
