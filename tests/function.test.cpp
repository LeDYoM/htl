#include "catch_include.hpp"

import htypes;

using namespace htps;

struct TestObject
{
    s32 add2(s32 const n) { return n + 2; }

    s32 add3(s32 const n) { return n + 3; }
};

struct ConstTestObject
{
    s32 add2(s32 const n) const { return n + 2; }

    s32 add3(s32 const n) const { return n + 3; }
};

struct ModifableTestObject
{
    s32 add2(s32 const n) const { return n + 2; }

    s32 add3(s32 const n) const { return n + 3; }

    s32 add2M(s32 const) const { return data + 2; }

    s32 add2M(s32 const n)
    {
        data = n + 2;
        return data;
    }

    s32 add3M(s32 const n)
    {
        data = n + 3;
        return data;
    }

    s32 data{0};
};

TEST_CASE("function::function", "[function]")
{
    SECTION("Empty")
    {
        function<s32(s32 const)> a;
        CHECK_FALSE(a);
    }

    SECTION("With lambda")
    {
        function<s32(s32 const)> b = [](s32 const n) { return n + 1; };
        CHECK(b);

        CHECK(4 == b(3));
    }

    SECTION("With function pointer")
    {
        TestObject a_obj;

        function<s32(s32 const)> a =
            function<s32(s32 const)>(&a_obj, &TestObject::add2);

        CHECK(6 == a(4));

        ConstTestObject const b_obj;

        function<s32(s32 const)> b =
            function<s32(s32 const)>(&b_obj, &ConstTestObject::add2);

        CHECK(10 == b(8));

        ModifableTestObject c_obj;

        function<s32(s32 const)> c =
            function<s32(s32 const)>(&c_obj, &ModifableTestObject::add2M);

        CHECK(c_obj.data == 0);
        CHECK(34 == c(32));
        CHECK(c_obj.data == 34);
    }
}

TEST_CASE("function::operator()", "[function]")
{
    function<s32(s32)> a = [](s32 d) { return d + 1; };
    s32 tmp{4};

    s32 tmp2 = a(tmp);
    CHECK(tmp2 == 5);
}

TEST_CASE("function::operator==() for lambdas", "[function]")
{
    function<s32(s32 const)> a = [](s32 const n) { return n + 1; };
    function<s32(s32 const)> b = [](s32 const n) { return n + 1; };

    // The lambda contains the same code, but the addresses are different
    CHECK_FALSE(a == b);
    CHECK_FALSE(a.equals(b));

    b = a;
    CHECK(a == b);
    CHECK(a.equals(b));
}

TEST_CASE("function::equals for pointer to members", "[function]")
{
    TestObject a_obj;
    TestObject b_obj;

    {
        auto a = function<s32(s32 const)>(&a_obj, &TestObject::add2);
        auto b = function<s32(s32 const)>(&b_obj, &TestObject::add2);

        // The lambda contains the same code, but the addresses are different
        CHECK_FALSE(a == b);
        CHECK_FALSE(a.equals(b));
        CHECK(a(5) == 7);
        CHECK(b(50) == 52);
    }

    {
        auto a = function<s32(s32 const)>(&a_obj, &TestObject::add2);
        auto b = function<s32(s32 const)>(&a_obj, &TestObject::add2);

        CHECK_FALSE(a == b);
        CHECK(a.equals(b));
        CHECK(a(5) == 7);
        CHECK(b(50) == 52);
    }

    {
        auto a = function<s32(s32 const)>(&a_obj, &TestObject::add2);
        auto b = function<s32(s32 const)>(&a_obj, &TestObject::add3);

        CHECK_FALSE(a == b);
        CHECK_FALSE(a.equals(b));
        CHECK(a(5) == 7);
        CHECK(b(50) == 53);
    }

    {
        ModifableTestObject c_obj;

        auto a = function<s32(s32 const)>(&c_obj, &ModifableTestObject::add2M);
        auto b = function<s32(s32 const)>(&c_obj, &ModifableTestObject::add2);

        CHECK_FALSE(a == b);
        CHECK_FALSE(a.equals(b));
        CHECK(c_obj.data == 0);
        CHECK(a(5) == 7);
        CHECK(b(50) == 52);
        CHECK(c_obj.data == 7);
    }
}

TEST_CASE("make_function", "[function]")
{
    TestObject a_obj;
    TestObject b_obj;
    const ConstTestObject c_obj;

    //    function<s32(s32 const)> const c =
    //        function<s32(s32 const)>(&c_obj, &ConstTestObject::add2);

    //    auto c =
    //        function<s32(s32 const)>(&c_obj, &ConstTestObject::add2);

    auto c = make_function(&c_obj, &ConstTestObject::add2);

    {
        auto a = make_function(&a_obj, &TestObject::add2);
        auto b = make_function(&b_obj, &TestObject::add2);

        CHECK_FALSE(a == b);
        CHECK_FALSE(a.equals(b));
        CHECK_FALSE(a == c);
        CHECK_FALSE(a.equals(c));
        CHECK(a(5) == 7);
        CHECK(b(50) == 52);
        CHECK(c(1) == 3);
    }

    {
        auto a = make_function(&a_obj, &TestObject::add2);
        auto b = make_function(&a_obj, &TestObject::add2);

        CHECK_FALSE(a == b);
        CHECK(a.equals(b));
        CHECK_FALSE(b == c);
        CHECK_FALSE(b.equals(c));
        CHECK(a(5) == 7);
        CHECK(b(50) == 52);
        CHECK(c(6) == 8);

        CHECK(a(b(c(85))) == 91);
    }
}
