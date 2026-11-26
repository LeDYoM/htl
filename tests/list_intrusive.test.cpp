#include "catch_include.hpp"

import htypes;

using namespace htps;

struct IntrusiveListElement
{
    IntrusiveListElement* prev;
    IntrusiveListElement* next;
    int data;

    IntrusiveListElement(int _) : data{_} {}
};

TEST_CASE("list_intrusive::list_intrusive constructors",
          "[htypes][list_intrusive]")
{
    list_intrusive<IntrusiveListElement> v;
    CHECK(v.empty());
    CHECK(v.size() == 0U);

    v.push_back(1);
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 1U);

    int t = 2;
    v.push_back(move(t));
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 2U);

    t = 3;
    v.push_back(t);
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 3U);

    v.emplace_back(4);
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 4U);
}
