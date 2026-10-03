#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("vector::vector", "[vector]")
{
    vector<u32> m;

    CHECK(m.size() == 0U);
    CHECK(m.empty());
    CHECK(m.capacity() == 0U);

    SECTION("Check errorneous operations")
    {
        m.pop_back();
        CHECK(m.size() == 0U);
        CHECK(m.empty());
        CHECK(m.capacity() == 0U);

        m.clear();
        CHECK(m.size() == 0U);
        CHECK(m.empty());
        CHECK(m.capacity() == 0U);

        SECTION("Copy empty")
        {
            vector_shared_pointers<s32> empty_vector;
            CHECK(empty_vector.capacity() == 0U);
            CHECK(empty_vector.size() == 0U);

            vector_shared_pointers<s32> copy_empty(empty_vector);
            CHECK(copy_empty.capacity() == 0U);
            CHECK(copy_empty.size() == 0U);
        }
    }
}

TEST_CASE("vector::vector(size)", "[vector]")
{
    vector<s32> m(2U);
    CHECK(m.size() == 0);
    CHECK(m.empty());
    CHECK(m.capacity() == 2U);

    m.push_back(0);
    m.push_back(1);
    CHECK(m.size() == 2U);
    CHECK_FALSE(m.empty());
    CHECK(m.capacity() == 2U);

    CHECK(m[0] == 0);
    CHECK(m[1] == 1);
}

TEST_CASE("vector::vector(vector)", "[vector]")
{
    vector<s32> m;

    m.push_back(5);

    vector<s32> m2(m);
    CHECK(m.size() == m2.size());

    m.reserve(10U);
    CHECK(m.size() == 1U);

    vector<s32> m3(m);
    CHECK(m3.capacity() <= m.capacity());
    CHECK(m3.size() <= m.size());
    CHECK(m3[0U] == 5);
}

TEST_CASE("vector::vector(iterator)", "[vector]")
{
    const u32 data[] = {0U, 1U, 2U, 3U, 4U};
    vector<u32> v(data, data + 5);
    CHECK(v.size() == 5U);
    CHECK(v == vector<u32>{0U, 1U, 2U, 3U, 4U});
    CHECK(0U == v[0]);
    CHECK(1U == v[1]);
    CHECK(2U == v[2]);
    CHECK(3U == v[3]);
    CHECK(4U == v[4]);
}

TEST_CASE("vector::vector(std::initializer_list)", "[vector]")
{
    {
        vector<u32> m{1, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1};
        CHECK(m == vector<u32>{1, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1});

        SECTION("Copy and move constructor")
        {
            auto m2{m};
            CHECK(m == m2);
            CHECK(m.size() == m2.size());
            CHECK(m.capacity() == m2.capacity());

            auto m3{htps::move(m)};
            CHECK(m.empty());
            CHECK(m2 == m3);
        }
    }

    {
        sptr<u32> sp1{msptr<u32>(1)};
        sptr<u32> sp2{msptr<u32>(9)};
        vector<sptr<u32>> m{sp1, sp2};
        CHECK(m == vector<sptr<u32>>{htps::move(sp1), htps::move(sp2)});
    }
}

TEST_CASE("vector::vector::operator=(std::initializer_list)", "[vector]")
{
    SECTION("Simple case")
    {
        vector<u32> m;
        m = {1, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1};
        CHECK(m == vector<u32>{1, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1});

        SECTION("Copy and move constructor")
        {
            auto m2(m);
            CHECK(m == m2);
            CHECK(m.size() == m2.size());
            CHECK(m.capacity() == m2.capacity());

            auto m3(htps::move(m));
            CHECK(m.empty());
            CHECK(m2 == m3);
        }
    }

    SECTION("Vector of shared pointers")
    {
        sptr<u32> sp1{msptr<u32>(1)};
        sptr<u32> sp2{msptr<u32>(9)};
        vector<sptr<u32>> m;
        m = {sp1, sp2};
        vector<sptr<u32>> m2;
        m2 = {htps::move(sp1), htps::move(sp2)};
        CHECK(m == m2);
    }
}

TEST_CASE("vector::vector(iterator,size)", "[vector]")
{
    SECTION("With sizeof")
    {
        const u32 arr[] = {4, 3, 2, 1};
        vector<u32> v(arr, sizeof(arr) / sizeof(u32));
        CHECK(v.size() == 4U);
    }

    SECTION("With sizeof")
    {
        const u32 arr[] = {4, 3, 2, 1};
        vector<u32> v(std::cbegin(arr), std::cend(arr));
        CHECK(v.size() == 4U);
    }
}

TEST_CASE("vector::emplace", "[vector]")
{
    vector<u32> v;

    v.emplace_back(0);
    CHECK(v.size() == 1U);
    v.pop_back();
    CHECK(v.empty());
}

TEST_CASE("vector::find_first_of", "[vector]")
{
    vector<s32> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    CHECK(v.size() == 10U);
    CHECK(v.find_first_of(5) == &v[5U]);
    CHECK(v.find_first_of({5, 5}) == &v[5U]);
    CHECK(v.find_first_of({15, 5}) == &v[5U]);
    CHECK(v.find_first_of(15) == v.cend());
    CHECK(v.find_first_of({15, 16}) == v.cend());
    CHECK(v.find_first_of(0) == &v[0U]);
    CHECK(v.find_first_of(0) == v.begin());
    CHECK(v.find_last_of({18, 19, 20}) == v.cend());
    CHECK(v.find_last_of(vector<s32>{}) == v.cend());
}

TEST_CASE("vector::find_backwards", "[vector]")
{
    vector<s32> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    CHECK(v.size() == 11U);
    CHECK(v.find_backwards(0) == &v[10U]);
    CHECK(v.find_backwards(5) == &v[5U]);
    CHECK(v.find_backwards(15) == v.end());
    v[0U] = -1;
    CHECK(v.find_backwards(-1) == &v[0U]);
    CHECK(v.find_backwards(-1) == v.begin());
}

TEST_CASE("vector::cfind_backwards", "[vector]")
{
    vector<s32> const v{-1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    CHECK(v.size() == 11U);
    CHECK(v.cfind_backwards(0) == &v[10U]);
    CHECK(v.cfind_backwards(5) == &v[5U]);
    CHECK(v.cfind_backwards(15) == v.cend());
    CHECK(v.cfind_backwards(-1) == &v[0U]);
    CHECK(v.cfind_backwards(-1) == v.cbegin());
}

TEST_CASE("vector::find_last_of", "[vector]")
{
    vector<s32> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    CHECK(v.size() == 11U);
    CHECK(v.find_last_of(0) == &v[10U]);
    CHECK(v.find_last_of(5) == &v[5U]);
    CHECK(v.find_last_of(15) == v.end());
    v[0U] = -1;
    CHECK(v.find_last_of(-1) == &v[0U]);
    CHECK(v.find_last_of(-1) == v.begin());
    v[0U] = 0;

    CHECK(v.find_last_of({0, 15}) == &v[10U]);
    CHECK(v.find_last_of({15, 0}) == &v[10U]);
    CHECK(v.find_last_of({0, 0}) == &v[10U]);

    CHECK(v.find_last_of({5, 15, 0, 10, 18, 19, 20}) == &v[10U]);
    CHECK(v.find_last_of(15) == v.end());
    CHECK(v.find_last_of({18, 19, 20}) == v.end());
    CHECK(v.find_last_of(vector<s32>{}) == v.end());
}

class A
{
public:
    s32 b;
};

inline auto init_vector_shared_pointers_A()
{
    vector_shared_pointers<A> test_vector1;
    test_vector1.push_back(msptr<A>(A{1}));
    test_vector1.push_back(msptr<A>(A{-2}));
    test_vector1.push_back(msptr<A>(A{3}));
    test_vector1.push_back(msptr<A>(A{-2}));
    test_vector1.push_back(msptr<A>(A{5}));
    test_vector1.push_back(msptr<A>(A{100}));
    test_vector1.push_back(msptr<A>(A{-2}));
    test_vector1.push_back(msptr<A>(A{-2}));
    test_vector1.push_back(msptr<A>(A{-1}));
    test_vector1.push_back(msptr<A>(A{0}));
    return test_vector1;
}

TEST_CASE("vector of shared pointers::copy", "[vector]")
{
    vector_shared_pointers<A> empty_vector;
    CHECK(empty_vector.capacity() == 0U);
    CHECK(empty_vector.size() == 0U);
    CHECK(empty_vector.empty());

    vector_shared_pointers<A> copy_empty(empty_vector);
    CHECK(copy_empty.capacity() == 0U);
    CHECK(copy_empty.size() == 0U);
}

TEST_CASE("vector of shared pointers::move", "[vector]")
{
    vector_shared_pointers<A> test_vector1(init_vector_shared_pointers_A());
    CHECK(test_vector1.size() == 10U);

    vector_shared_pointers<A> test_vector2 = htps::move(test_vector1);
    CHECK(test_vector1.empty());
    CHECK(test_vector1.capacity() == 0U);

    CHECK(test_vector2.size() == 10U);

    vector_shared_pointers<A> test_vector3;
    test_vector3 = htps::move(test_vector2);
    CHECK(test_vector2.empty());
    CHECK(test_vector2.capacity() == 0U);

    CHECK(test_vector3.size() == 10U);
}

TEST_CASE("vector of shared pointers::add move add", "[vector]")
{
    vector_shared_pointers<A> test_vector1(init_vector_shared_pointers_A());
    CHECK(test_vector1.size() == 10U);

    vector_shared_pointers<A> test_vector2 = htps::move(test_vector1);
    CHECK(test_vector1.empty());
    CHECK(test_vector1.capacity() == 0U);

    CHECK(test_vector2.size() == 10U);

    vector_shared_pointers<A> test_vector3;
    test_vector3 = htps::move(test_vector2);
    CHECK(test_vector2.empty());
    CHECK(test_vector2.capacity() == 0U);

    CHECK(test_vector3.size() == 10U);

    test_vector1.push_back(msptr<A>(A{1}));
}

TEST_CASE("vector of shared pointers::remove", "[vector]")
{
    vector_shared_pointers<A> test_vector1(init_vector_shared_pointers_A());
    CHECK(test_vector1.size() == 10U);

    test_vector1[1] = nullptr;
    test_vector1[5] = nullptr;

    test_vector1.erase_values(nullptr);
    CHECK(test_vector1.size() == 8U);

    test_vector1.push_back(nullptr);
    CHECK(test_vector1.size() == 9U);
    test_vector1.erase_values(nullptr);
    CHECK(test_vector1.size() == 8U);

    SECTION("erase_one_index")
    {
        test_vector1.emplace_back(msptr<A>(A{50}));
        test_vector1.emplace_back(msptr<A>(A{51}));
        CHECK(test_vector1.size() == 10U);

        {
            auto const result = test_vector1.erase_one_index(8U);
            CHECK(test_vector1.size() == 9U);
            CHECK((*result)->b == 51);
        }

        {
            auto const result = test_vector1.erase_one_index(9U);
            CHECK(result == test_vector1.end());
        }
    }

    SECTION("erase_one keep order")
    {
        test_vector1.emplace_back(msptr<A>(A{50}));
        test_vector1.emplace_back(msptr<A>(A{51}));
        CHECK(test_vector1.size() == 10U);

        {
            auto const iterator   = test_vector1.begin();
            auto const value2     = (test_vector1[1U]->b);
            auto const value_last = (test_vector1[test_vector1.size() - 1U]->b);

            CHECK(std::next(iterator) == &(test_vector1[1U]));
            test_vector1.erase_one<false>(*test_vector1.begin());
            CHECK(test_vector1[0U]->b == value2);
            CHECK((test_vector1[test_vector1.size() - 1U]->b) == value_last);
        }
    }

    SECTION("Remove removed shared pointer")
    {
        sptr<A> temp = msptr<A>(A{42});
        wptr<A> weak = temp;
        test_vector1.push_back(htps::move(temp));
        CHECK(test_vector1.size() == 9U);
        CHECK_FALSE(weak.lock() == nullptr);
        test_vector1.pop_back();
        CHECK(test_vector1.size() == 8U);
        CHECK(weak.lock() == nullptr);
    }

    SECTION("Remove if")
    {
        sptr<A> temp = msptr<A>(A{42});
        wptr<A> weak = temp;
        test_vector1.push_back(htps::move(temp));
        CHECK(test_vector1.size() == 9U);
        CHECK_FALSE(weak.lock() == nullptr);
        test_vector1.erase_if(
            [](const sptr<A> element) { return element->b == 42; });
        CHECK(test_vector1.size() == 8U);
        CHECK(weak.lock() == nullptr);
    }
}

TEST_CASE("vector of shared pointers::emplace", "[vector]")
{
    vector_shared_pointers<A> test_vector;
    sptr<A> temp = msptr<A>(A{42});

    test_vector.emplace_back(new A{42});
    CHECK(test_vector.size() == 1U);
    test_vector.pop_back();
    CHECK(test_vector.empty());
}

TEST_CASE("vector of shared pointers::clear", "[vector]")
{
    vector_shared_pointers<A> test_vector1(init_vector_shared_pointers_A());
    CHECK(test_vector1.size() == 10U);

    sptr<A> temp = msptr<A>(A{42});
    wptr<A> weak = temp;
    test_vector1.push_back(htps::move(temp));
    CHECK_FALSE(weak.lock() == nullptr);
    test_vector1.clear();
    CHECK(weak.lock() == nullptr);
}

TEST_CASE("vector of shared pointers::shrink_to_fit", "[vector]")
{
    vector_shared_pointers<A> test_vector1(init_vector_shared_pointers_A());
    CHECK(test_vector1.size() == 10U);
    CHECK(test_vector1.capacity() >= 10U);

    test_vector1.pop_back();
    test_vector1.pop_back();
    CHECK(test_vector1.size() == 8U);
    CHECK(test_vector1.capacity() >= 10U);

    test_vector1.shrink_to_fit();
    CHECK(test_vector1.size() == 8U);
    CHECK(test_vector1.capacity() == 8U);

    test_vector1.clear();
    CHECK(test_vector1.size() == 0U);
    CHECK(test_vector1.capacity() == 8U);

    test_vector1.shrink_to_fit();
    CHECK(test_vector1.size() == 0U);
    CHECK(test_vector1.capacity() == 0U);

    vector<int> another;
    CHECK(another.empty());
    CHECK(another.capacity() == 0U);
    another.reserve(10U);
    CHECK(another.empty());
    CHECK(another.capacity() == 10U);
    another.shrink_to_fit();
    CHECK(another.empty());
    CHECK(another.capacity() == 0U);
}

TEST_CASE("vector::Unique ptr", "[vector]")
{
    vector_unique_pointers<s32> test_vector1;
    vector_unique_pointers<s32> test_vector2(4U);

    CHECK(test_vector1.capacity() == 0U);
    CHECK(test_vector2.capacity() == 4U);

    test_vector1.push_back(muptr<s32>(1));
    test_vector2.push_back(muptr<s32>(1));

    CHECK((*test_vector1[0U]) == (*test_vector2[0U]));

    test_vector1.emplace_back(muptr<s32>(2));
    test_vector2.emplace_back(muptr<s32>(2));

    CHECK((*test_vector1[1U]) == (*test_vector2[1U]));

    test_vector1.shrink_to_fit();
    test_vector2.shrink_to_fit();

    CHECK(test_vector1.capacity() == 2U);
    CHECK(test_vector1.capacity() == 2U);

    CHECK((*test_vector1[1U]) == (*test_vector2[1U]));

    test_vector1.clear();
    test_vector2.clear();

    test_vector1.shrink_to_fit();
    test_vector2.shrink_to_fit();

    CHECK(test_vector1.capacity() == 0U);
    CHECK(test_vector1.capacity() == 0U);

    test_vector1.shrink_to_fit();
    test_vector2.shrink_to_fit();

    CHECK(test_vector1.capacity() == 0U);
    CHECK(test_vector1.capacity() == 0U);
}

TEST_CASE("vector::Grow policy", "[vector]")
{
    vector<s32> default_test_vector;
    vector_storage<s32, AllocatorType<s32>, GrowPolicyDouble>
        double_grow_test_vector;

    CHECK(default_test_vector.empty());
    CHECK(default_test_vector.capacity() == 0U);

    CHECK(double_grow_test_vector.empty());
    CHECK(double_grow_test_vector.capacity() == 0U);

    default_test_vector.push_back(1);
    double_grow_test_vector.push_back(1);

    CHECK(default_test_vector.size() == 1U);
    CHECK(default_test_vector.capacity() == 1U);

    CHECK(double_grow_test_vector.size() == 1U);
    CHECK(double_grow_test_vector.capacity() == 1U);

    default_test_vector.push_back(2);
    double_grow_test_vector.push_back(2);

    CHECK(default_test_vector.size() == 2U);
    CHECK(default_test_vector.capacity() == 2U);

    CHECK(double_grow_test_vector.size() == 2U);
    CHECK(double_grow_test_vector.capacity() == 2U);

    default_test_vector.push_back(3);
    double_grow_test_vector.push_back(3);

    CHECK(default_test_vector.size() == 3U);
    CHECK(default_test_vector.capacity() == 3U);

    CHECK(double_grow_test_vector.size() == 3U);
    CHECK(double_grow_test_vector.capacity() == 4U);
}

struct MoveOnly
{
public:
    explicit MoveOnly(int b) : a{b} {}
    MoveOnly(const MoveOnly&)            = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) noexcept   = default;
    MoveOnly& operator=(MoveOnly&&) = default;
    int get() const noexcept { return a; }

private:
    int a;
};

TEST_CASE("vector::Movable only objects", "[vector]")
{
    static_assert(!std::is_copy_constructible_v<MoveOnly>);
    static_assert(!std::is_copy_assignable_v<MoveOnly>);

    static_assert(std::is_move_constructible_v<MoveOnly>);
    static_assert(std::is_move_assignable_v<MoveOnly>);

    vector<MoveOnly> v;
    v.push_back(MoveOnly{5});
    v.emplace_back(4);
    CHECK(5 == v[0U].get());
    CHECK(4 == v[1U].get());

    vector<MoveOnly> v2 = htps::move(v);
    CHECK(5 == v2[0U].get());
    CHECK(4 == v2[1U].get());

    SECTION("Swap")
    {
        vector<MoveOnly> v3;
        v3.swap(v2);

        CHECK(5 == v3[0U].get());
        CHECK(4 == v3[1U].get());
        CHECK(v.empty());
    }
}

TEST_CASE("vector::pop_back", "[vector]")
{
    vector<u32> v;

    CHECK(v.size() == 0U);
    CHECK(v.empty());
    CHECK(v.capacity() == 0U);

    CHECK(v.begin() == v.end());
    v.push_back(0);
    v.push_back(1);
    CHECK(v.size() == 2U);
    CHECK_FALSE(v.empty());
    CHECK(v.begin() != v.end());

    v.pop_back();
    CHECK(v.size() == 1U);
    CHECK_FALSE(v.empty());
    CHECK(v.begin() != v.end());

    v.pop_back();
    CHECK(v.size() == 0U);
    CHECK(v.empty());
    CHECK(v.begin() == v.end());
}

TEST_CASE("vector::insert_vector", "[vector]")
{
    SECTION("SimpleClass")
    {
        vector<MoveOnly> v;
        v.emplace_back(5);
        v.emplace_back(4);
        v.emplace_back(3);
        v.emplace_back(2);
        CHECK(v.size() == 4U);

        vector<MoveOnly> v2;
        CHECK(v2.empty());
        v2.insert(htps::move(v));
        CHECK(v2.size() == 4U);
        CHECK(v2[0].get() == 5);
        CHECK(v2[1].get() == 4);
        CHECK(v2[2].get() == 3);
        CHECK(v2[3].get() == 2);
    }

    SECTION("Shared pointers")
    {
        vector<sptr<u32>> v;
        v.push_back(msptr<u32>(0));
        v.push_back(msptr<u32>(1));
        CHECK_FALSE(v.empty());
        CHECK(v.size() == 2U);

        SECTION("Insert by copy")
        {
            vector<sptr<u32>> v2;
            v2.insert(v);
            CHECK_FALSE(v2.empty());
            CHECK(v2.size() == 2U);
            v2.push_back(msptr<u32>(2));
            CHECK(v2.size() == 3U);
            v2.insert(v);
            CHECK(v2.size() == 5U);
        }
    }
}

TEST_CASE("vector::insert_element", "[vector]")
{
    SECTION("Simple")
    {
        vector<u32> v;
        CHECK(v.empty());
        v.insert(v.begin(), 3);
        CHECK_FALSE(v.empty());
        CHECK(v.size() == 1U);
        CHECK(v[0] == 3);

        v.insert(v.begin(), 4);
        CHECK(v.size() == 2U);
        CHECK(v[0] == 4);
        CHECK(v[1] == 3);

        v.insert(v.begin() + 1, 5);
        CHECK(v.size() == 3U);
        CHECK(v[0] == 4);
        CHECK(v[1] == 5);
        CHECK(v[2] == 3);

        v.insert(v.end(), 6);
        CHECK(v.size() == 4U);
        CHECK(v[0] == 4);
        CHECK(v[1] == 5);
        CHECK(v[2] == 3);
        CHECK(v[3] == 6);

        v.pop_back();
        v.pop_back();
        CHECK(v.size() == 2U);
        CHECK(v[0] == 4);
        CHECK(v[1] == 5);

        v.insert(v.end(), 7);
        CHECK(v.size() == 3U);
        CHECK(v[0] == 4);
        CHECK(v[1] == 5);
        CHECK(v[2] == 7);
    }

    SECTION("Simple uninitialized")
    {
        vector<u32> v;
        v.reserve(5);
        CHECK(v.empty());
        v.insert(v.begin(), 2);
        CHECK_FALSE(v.empty());
        CHECK(v.size() == 1U);
        CHECK(v[0] == 2);

        v.insert(v.end(), 3);
        CHECK(v.size() == 2U);
        CHECK(v[0] == 2);
        CHECK(v[1] == 3);

        v.insert(v.begin() + 1, 4);
        CHECK(v.size() == 3U);
        CHECK(v[0] == 2);
        CHECK(v[1] == 4);
        CHECK(v[2] == 3);

        v.insert(v.begin() + 2, 5);
        CHECK(v.size() == 4U);
        CHECK(v[0] == 2);
        CHECK(v[1] == 4);
        CHECK(v[2] == 5);
        CHECK(v[3] == 3);

        v.insert(v.begin() + 3, 6);
        CHECK(v.size() == 5U);
        CHECK(v[0] == 2);
        CHECK(v[1] == 4);
        CHECK(v[2] == 5);
        CHECK(v[3] == 6);
        CHECK(v[4] == 3);

        v.insert(v.begin() + 4, 7);
        CHECK(v.size() == 6U);
        CHECK(v[0] == 2);
        CHECK(v[1] == 4);
        CHECK(v[2] == 5);
        CHECK(v[3] == 6);
        CHECK(v[4] == 7);
        CHECK(v[5] == 3);
    }

    SECTION("With shared pointers")
    {
        vector<sptr<u32>> v;

        CHECK(v.empty());
        v.insert(v.begin(), msptr<u32>(3));
        CHECK_FALSE(v.empty());
        CHECK(v.size() == 1U);
        CHECK((*v[0]) == 3);

        bool a = v[0] == 3;
        (void)(a);

        v.insert(v.begin(), msptr<u32>(4));
        CHECK(v.size() == 2U);
        CHECK((*v[0]) == 4);
        CHECK((*v[1]) == 3);

        v.insert(v.begin() + 1, msptr<u32>(5));
        CHECK(v.size() == 3U);
        CHECK((*v[0]) == 4);
        CHECK((*v[1]) == 5);
        CHECK((*v[2]) == 3);

        v.insert(v.end(), msptr<u32>(6));
        CHECK(v.size() == 4U);
        CHECK((*v[0]) == 4);
        CHECK((*v[1]) == 5);
        CHECK((*v[2]) == 3);
        CHECK((*v[3]) == 6);

        v.pop_back();
        v.pop_back();
        CHECK(v.size() == 2U);
        CHECK((*v[0]) == 4);
        CHECK((*v[1]) == 5);

        v.insert(v.end(), msptr<u32>(7));
        CHECK(v.size() == 3U);
        CHECK((*v[0]) == 4);
        CHECK((*v[1]) == 5);
        CHECK((*v[2]) == 7);
    }

    SECTION("Shared pointer uninitialized")
    {
        vector<sptr<u32>> v;

        v.reserve(8);
        CHECK(v.empty());
        v.insert(v.begin(), msptr<u32>(2));
        CHECK_FALSE(v.empty());
        CHECK(v.size() == 1U);
        CHECK(*(v[0]) == 2);

        v.insert(v.begin(), msptr<u32>(3));
        CHECK(v.size() == 2U);
        CHECK(*(v[0]) == 3);
        CHECK(*(v[1]) == 2);

        v.insert(v.end(), msptr<u32>(4));
        CHECK(v.size() == 3U);
        CHECK(*(v[0]) == 3);
        CHECK(*(v[1]) == 2);
        CHECK(*(v[2]) == 4);

        v.insert(v.begin(), msptr<u32>(5));
        CHECK(v.size() == 4U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 3);
        CHECK(*(v[2]) == 2);
        CHECK(*(v[3]) == 4);

        v.insert(v.begin() + 1, msptr<u32>(6));
        CHECK(v.size() == 5U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 6);
        CHECK(*(v[2]) == 3);
        CHECK(*(v[3]) == 2);
        CHECK(*(v[4]) == 4);

        v.insert(v.begin() + 2, msptr<u32>(7));
        CHECK(v.size() == 6U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 6);
        CHECK(*(v[2]) == 7);
        CHECK(*(v[3]) == 3);
        CHECK(*(v[4]) == 2);
        CHECK(*(v[5]) == 4);

        v.insert(v.begin() + 3, msptr<u32>(8));
        CHECK(v.size() == 7U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 6);
        CHECK(*(v[2]) == 7);
        CHECK(*(v[3]) == 8);
        CHECK(*(v[4]) == 3);
        CHECK(*(v[5]) == 2);
        CHECK(*(v[6]) == 4);

        v.insert(v.begin() + 4, msptr<u32>(9));
        CHECK(v.size() == 8U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 6);
        CHECK(*(v[2]) == 7);
        CHECK(*(v[3]) == 8);
        CHECK(*(v[4]) == 9);
        CHECK(*(v[5]) == 3);
        CHECK(*(v[6]) == 2);
        CHECK(*(v[7]) == 4);

        v.insert(v.begin() + 5, msptr<u32>(10));
        CHECK(v.size() == 9U);
        CHECK(*(v[0]) == 5);
        CHECK(*(v[1]) == 6);
        CHECK(*(v[2]) == 7);
        CHECK(*(v[3]) == 8);
        CHECK(*(v[4]) == 9);
        CHECK(*(v[5]) == 10);
        CHECK(*(v[6]) == 3);
        CHECK(*(v[7]) == 2);
        CHECK(*(v[8]) == 4);
    }
}

TEST_CASE("vector::set_at_index", "[vector]")
{
        vector<sptr<u32>> v;

        CHECK(v.empty());
        v.set_at_index(0, msptr<u32>(20U));
        CHECK(v.size() == 1U);
        CHECK(*(v[0]) == 20U);

        auto r{msptr<u32>(40U)};
        v.set_at_index(10, r);
        CHECK(v.size() == 11U);
        CHECK(*(v[10]) == 40U);
        CHECK(*r == 40U);


        CHECK(v[5] == nullptr);
        v.set_at_index(5, htps::move(r));
        CHECK(v.size() == 11U);
        CHECK(*(v[5]) == 40U);
        CHECK(r == nullptr);
}
