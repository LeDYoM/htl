#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("str::str Default constructor", "[str]")
{
    str test;
    CHECK(test == "");
    CHECK(test.size() == 0);
    CHECK(test.empty());
    CHECK_FALSE(test == "abc");

    test.push_back('S');
    CHECK(test == "S");
    CHECK(test == str{"S"});
    CHECK(test.size() == 1U);
}

TEST_CASE("str::str Construct with string_view", "[str][str_view]")
{
    str test{"hello"};
    CHECK(test == "hello");
    CHECK(test.size() == 5U);
    CHECK_FALSE(test.empty());

    str_view test2{test.c_str()};
    CHECK(test2 == "hello");
    CHECK(test2.size() == 5U);
    CHECK_FALSE(test2.empty());

    str test3(test2);
    CHECK(test3 == "hello");
    CHECK(test3.size() == 5U);
    CHECK_FALSE(test3.empty());
}

TEST_CASE("str::str Construct with str_literal", "[str][str_view][str_literal]")
{
    str_literal my_literal{"literal"};
    str test{my_literal};
    CHECK(test.size() == 7U);
    CHECK(test[0] == 'l');
    CHECK(test[1] == 'i');
    CHECK(test[2] == 't');
    CHECK(test[3] == 'e');
    CHECK(test[4] == 'r');
    CHECK(test[5] == 'a');
    CHECK(test[6] == 'l');
}

TEST_CASE("str::str Copy initialize", "[str]")
{
    str test("a");
    CHECK(test == "a");
    CHECK(test.size() == 1);

    str test2 = "a";
    CHECK(test2 == "a");
    CHECK(test2.size() == 1);

    char test_str[] = "test1_bigstring_to_test_here_and_now_in_this_test";
    str test3       = "test1_bigstring_to_test_here_and_now_in_this_test";
    CHECK(test3 == "test1_bigstring_to_test_here_and_now_in_this_test");
    CHECK(test3.size() == sizeof(test_str) - 1);
}

TEST_CASE("str::str Copy", "[str]")
{
    str test;
    test = "def";
    CHECK(test == "def");
    CHECK_FALSE(test != "def");
    CHECK_FALSE(test == "de");
    CHECK_FALSE(test == "def0");
    CHECK(test == "def");
    CHECK_FALSE(test == "");
    CHECK_FALSE("def" != test);
    CHECK_FALSE("de" == test);
    CHECK_FALSE("def0" == test);
    CHECK("def" == test);
    CHECK_FALSE("" == test);
}

TEST_CASE("str::operator= copy", "[str]")
{
    str test  = "Long enought string";
    str test2 = "abc";
    auto const capacity_now_test{test.capacity()};

    test = test2;
    CHECK(test.capacity() >= capacity_now_test);
}

TEST_CASE("str::Increment", "[str]")
{
    str test("This");
    test += " is a";
    test += str(" test");
    CHECK(test == "This is a test");
    CHECK(test.size() == sizeof("This is a test") - 1);
}

TEST_CASE("str::find", "[str]")
{
    SECTION("One char find")
    {
        str test("Lets play this");
        auto sep{test.find(' ')};
        CHECK(sep == 4U);
        str sub{test.substr(sep + 1)};
        CHECK(sub == "play this");
        str sub2{sub.substr(sub.find('z'))};
        CHECK(sub2 == "");
        str sub3{sub.substr(sub.find(' '))};
        CHECK(sub3 == " this");
        sub3 += make_str(3U);
        CHECK(sub3 == " this3");
        CHECK(test.find(0) == str::npos);
        CHECK(str{}.find(0) == str::npos);
        CHECK(str{""}.find(0) == str::npos);
        CHECK(str{""}.find(' ') == str::npos);
    }

    SECTION("str find")
    {
        size_type sep{0U};
        str test("Lets play this");
        sep = test.find("L");
        CHECK(sep == 0U);
        sep = test.find("Lets");
        CHECK(sep == 0U);
        sep = test.find("Lets play this");
        CHECK(sep == 0U);
        sep = test.find("Lets play this ");
        CHECK(sep == str::npos);
        sep = test.find("ets play thiq");
        CHECK(sep == str::npos);
        sep = test.find("hiqasd");
        CHECK(sep == str::npos);
        sep = test.find("hi");
        CHECK(sep == 11U);
        sep = test.find("");
        CHECK(sep == str::npos);
        CHECK(str{}.find("") == str::npos);
        CHECK(str{""}.find(" ") == str::npos);

        test = "o splot string. Lets see.";
        CHECK(test.cfind(" string") == 7U);
    }
}

TEST_CASE("str::substr", "[str]")
{
    str test("String to test substr=!other tests");
    CHECK(test.substr(str::npos) == "");
    CHECK(test.substr(str::npos).empty());
    CHECK(test.substr(str::npos).size() == 0U);
    CHECK(test.substr(0) == test);
    CHECK(test.substr(7, 21) == "to test substr=!other");
    CHECK(test.substr(7, 21).substr(8, 50) == "substr=!other");
    CHECK(test.substr(7, 21).substr(8, 2) == "su");
}

TEST_CASE("str::find_first_of", "[str]")
{
    str test("Try to=test! some parsing&&now");

    CHECK(test.find_first_of(' ') == 3U);
    CHECK(test.find_first_of('=') == 6U);
    CHECK(test.find_first_of('!') == 11U);
    CHECK(test.find_first_of('&') == 25U);
    test[25U] = '/';
    CHECK(test.find_first_of('/') == 25U);
    CHECK(test.find_first_of('&') == 26U);
    CHECK(test.find_first_of('w') == 29U);
    CHECK(test.find_first_of('0') == str::npos);

    CHECK(test.find_first_of("/") == 25U);
    CHECK(test.find_first_of("&") == 26U);
    CHECK(test.find_first_of("w") == 29U);
    CHECK(test.find_first_of("0") == str::npos);
    CHECK(test.find_first_of("//") == 25U);
    CHECK(test.find_first_of("rT") == 0U);
    CHECK(test.find_first_of("012345678900") == str::npos);
    CHECK(test.find_first_of("") == str::npos);
    CHECK(str{""}.find_first_of("") == str::npos);
}

TEST_CASE("str::find_last_of", "[str]")
{
    str test("Try to=test! some parsing&&now");

    CHECK(test.find_last_of(' ') == 17U);
    CHECK(test.find_last_of('=') == 6U);
    CHECK(test.find_last_of('!') == 11U);
    CHECK(test.find_last_of('&') == 26U);
    test[25U] = '/';
    CHECK(test.find_last_of('/') == 25U);
    CHECK(test.find_last_of('&') == 26U);
    CHECK(test.find_last_of('w') == 29U);
    CHECK(test.find_last_of('0') == str::npos);

    CHECK(test.find_last_of("/") == 25U);
    CHECK(test.find_last_of("&") == 26U);
    CHECK(test.find_last_of("w") == 29U);
    CHECK(test.find_last_of("0") == str::npos);
    CHECK(test.find_last_of("//") == 25U);
    CHECK(test.find_last_of("s w") == 29U);
    CHECK(test.find_last_of("012345678900") == str::npos);
    CHECK(test.find_last_of("") == str::npos);
    CHECK(str{""}.find_last_of("") == str::npos);
}

TEST_CASE("str conversions", "[str]")
{
    str test = str::to_str(static_cast<u64>(1234567890L));
    CHECK(test == "1234567890");
    test = str::to_str('A');
    CHECK(test == "65");
    test = "A";
    CHECK(test == "A");
    test.append(9876543210);
    CHECK(test == "A9876543210");
    test += "_";
    test += 1;
    CHECK(test == "A9876543210_1");
    test += 01;
    CHECK(test == "A9876543210_11");
    test += 0xf;
    CHECK(test == "A9876543210_1115");
    test += 010;
    CHECK(test == "A9876543210_11158");
}

TEST_CASE("str operations", "[str]")
{
    str f("the test");
    str f2("hello ");
    CHECK(f2 + f == "hello the test");
    CHECK((f2 += f) == "hello the test");
    CHECK(f2 == "hello the test");
}

TEST_CASE("str::split", "[str]")
{
    str const foo{"hello. This. To Split string. Lets see."};

    SECTION("One char separator")
    {
        auto str_splitted{foo.split('.')};
        CHECK(str_splitted.size() == 5U);
        CHECK(str_splitted[0U] == "hello");
        CHECK(str_splitted[1U] == " This");
        CHECK(str_splitted[2U] == " To Split string");
        CHECK(str_splitted[3U] == " Lets see");
        CHECK(str_splitted[4U] == "");

        str_splitted = str_splitted[2U].substr(4U).split('s');
        CHECK(str_splitted.size() == 2U);
        CHECK(str_splitted[0U] == "Split ");
        CHECK(str_splitted[1U] == "tring");

        str_splitted = foo.split(0);
        CHECK(str_splitted.size() == 1U);
        CHECK(str_splitted[0U] == foo);

        str_splitted = foo.split('z');
        CHECK(str_splitted.size() == 1U);
        CHECK(str_splitted[0U] == foo);

        CHECK(str{""}.split('/').size() == 1U);
        CHECK(str{""}.split('/')[0U] == "");

        CHECK(str{"A"}.split('/').size() == 1U);
        CHECK(str{"A"}.split('/')[0U] == "A");
    }

    SECTION("str separator of one")
    {
        auto str_splitted{foo.split(".")};
        CHECK(str_splitted.size() == 5U);
        CHECK(str_splitted[0U] == "hello");
        CHECK(str_splitted[1U] == " This");
        CHECK(str_splitted[2U] == " To Split string");
        CHECK(str_splitted[3U] == " Lets see");
        CHECK(str_splitted[4U] == "");

        str_splitted = str_splitted[2U].substr(4U).split("s");
        CHECK(str_splitted.size() == 2U);
        CHECK(str_splitted[0U] == "Split ");
        CHECK(str_splitted[1U] == "tring");

        str_splitted = foo.split("");
        CHECK(str_splitted.size() == 1U);
        CHECK(str_splitted[0U] == foo);

        str_splitted = foo.split("z");
        CHECK(str_splitted.size() == 1U);
        CHECK(str_splitted[0U] == foo);

        CHECK(str{""}.split("/").size() == 1U);
        CHECK(str{""}.split("/")[0U] == "");

        CHECK(str{"A"}.split("/").size() == 1U);
        CHECK(str{"A"}.split("/")[0U] == "A");

        CHECK(str{""}.split("").size() == 1U);
        CHECK(str{""}.split("")[0U] == "");

        CHECK(str{"A"}.split("").size() == 1U);
        CHECK(str{"A"}.split("")[0U] == "A");
    }

    //    str const foo{"hello. This. To Split string. Lets see."};

    SECTION("str separator")
    {
        auto str_splitted{foo.split(". T")};
        CHECK(str_splitted.size() == 3U);
        CHECK(str_splitted[0U] == "hello");
        CHECK(str_splitted[1U] == "his");
        CHECK(str_splitted[2U] == "o Split string. Lets see.");

        str_splitted = str_splitted[2U].split(" string");
        CHECK(str_splitted.size() == 2U);
        CHECK(str_splitted[0U] == "o Split");
        CHECK(str_splitted[1U] == ". Lets see.");

        str_splitted = str_splitted[1U].split(".");
        CHECK(str_splitted.size() == 3U);
        CHECK(str_splitted[0U] == "");
        CHECK(str_splitted[1U] == " Lets see");
        CHECK(str_splitted[2U] == "");
    }
}

TEST_CASE("str trims", "[str]")
{
    SECTION("ltrim")
    {
        str tst_str(" \f\n\r\t\va");
        tst_str.ltrim();
        CHECK(tst_str == "a");
    }

    SECTION("rtrim")
    {
        str tst_str("a \f\n\r\t\v");
        tst_str.rtrim();
        CHECK(tst_str == "a");
    }

    SECTION("trim")
    {
        str tst_str(" \f\n\r\t\va \f\n\r\t\v");
        tst_str.trim();
        CHECK(tst_str == "a");
    }

    SECTION("trim misc")
    {
        str tst_str(" \f\n\r\t\va b \f\n\r\t\v");
        tst_str.trim();
        CHECK(tst_str == "a b");

        tst_str = " \f\n\r\t\va b \f\n\r\t\v";
        tst_str.ltrim();
        CHECK(tst_str == "a b \f\n\r\t\v");

        tst_str = " \f\n\r\t\va b \f\n\r\t\v";
        tst_str.rtrim();
        CHECK(tst_str == " \f\n\r\t\va b");

        tst_str = " \f\n\r\t\va b \f\n\r\t\v";
        tst_str.ltrim();
        CHECK(tst_str == "a b \f\n\r\t\v");
        tst_str.rtrim();
        CHECK(tst_str == "a b");

        SECTION("trim misc 2")
        {
            tst_str += "qwe   ";
            tst_str = "    def" + tst_str;
            tst_str.trim();
            CHECK(tst_str == "defa bqwe");
        }
    }
}

TEST_CASE("str::convert", "[str]")
{
    str sInt("-42");
    str sIntCopy(sInt);
    str longStr(str::to_str(12345678900));
    str slongStr(str::to_str(-12345678900));
    s32 i{0};
    f32 j{0};
    u64 k{0};

    CHECK(sInt.convert(i));
    CHECK(i == -42);

    // Int string to f32 works
    CHECK(sInt.convert(j));

    CHECK(sInt == sIntCopy);

    CHECK_FALSE(longStr.convert(i));

    // 64bits signed int to f32 works
    CHECK(longStr.convert(j));
    CHECK(longStr.convert(k));
    CHECK(j == 12345678900.f);
    CHECK(k == 12345678900);

    // Warning: sign mismatch, but works
    CHECK(slongStr.convert(k));
}

TEST_CASE("str::has", "[str]")
{
    str tst_string{"String to test the has in function"};

    CHECK(tst_string.has("to test") == 7U);
    CHECK(tst_string.has("totest") == str::npos);
    CHECK(tst_string.has("String to test the has in function") == 0U);
    CHECK(tst_string.has("tring to test the has in function") == 1U);
    CHECK(tst_string.has("String to test the has in function ") == str::npos);
    CHECK(tst_string.has("function") == 26U);
    CHECK(tst_string.has("function ") == str::npos);
    CHECK(tst_string.has("") == 0U);
}

TEST_CASE("str::has_in", "[str]")
{
    str tst_string{"String to test the has in function"};

    CHECK(tst_string.has_in("to test", 7U));
    CHECK(tst_string.has_in("function", 26U));
    CHECK_FALSE(tst_string.has_in("function ", 26U));
    CHECK_FALSE(tst_string.has_in("function", 56U));
    CHECK_FALSE(tst_string.has_in("", 156U));
    CHECK(tst_string.has_in("", 15U));
}

TEST_CASE("str starts and ends with", "[str]")
{
    str tst_string("String to test");

    SECTION("Starts with")
    {
        CHECK(tst_string.starts_with(""));
        CHECK(tst_string.starts_with("String "));
        CHECK(tst_string.starts_with("String to test"));
        CHECK_FALSE(tst_string.starts_with(" "));
        CHECK_FALSE(tst_string.starts_with("Stringt"));
        CHECK_FALSE(tst_string.starts_with("String to test "));
        CHECK(tst_string.starts_with(""));
    }

    SECTION("Ends with")
    {
        CHECK(tst_string.ends_with(""));
        CHECK(tst_string.ends_with(" test"));
        CHECK(tst_string.ends_with("String to test"));
        CHECK_FALSE(tst_string.ends_with(" "));
        CHECK_FALSE(tst_string.ends_with("ttest"));
        CHECK_FALSE(tst_string.ends_with("String to test "));
        CHECK(tst_string.ends_with(""));
    }
}

TEST_CASE("str::c_str", "[str]")
{
    {
        str test{"str_test_data"};
        CHECK(0 == std::strcmp("str_test_data", test.c_str()));
        test += "_good";
        CHECK(0 == std::strcmp("str_test_data_good", test.c_str()));

        test = test.substr(1U);
        CHECK(0 != std::strcmp("str_test_data_good", test.c_str()));
        CHECK(0 == std::strcmp("tr_test_data_good", test.c_str()));
    }

    {
        str test;
        CHECK(0 == std::strcmp("", test.c_str()));
        test.push_back('b');
        CHECK_FALSE(test.empty());
        CHECK(0 == std::strcmp("b", test.c_str()));

        test = test.substr(1U);
        CHECK(test.empty());
        CHECK(0 == std::strcmp("", test.c_str()));

        test = test.substr(1U);
        CHECK(test.empty());
        CHECK(0 == std::strcmp("", test.c_str()));
    }
}

TEST_CASE("str_comparisons", "[str]")
{
    str tst_string{"AAAA"};
    str tst_string2{"AAAB"};
    str tst_string3{"AAA"};
    str tst_string4{"AAAA"};

    CHECK(tst_string < tst_string2);
    CHECK(tst_string2 > tst_string);

    CHECK(tst_string <= tst_string2);
    CHECK(tst_string2 >= tst_string);

    CHECK(tst_string != tst_string2);
    CHECK(tst_string2 != tst_string);

    CHECK_FALSE(tst_string != tst_string4);
    CHECK(tst_string <= tst_string3);
    CHECK(tst_string != tst_string3);
    CHECK_FALSE(tst_string > tst_string3);
    CHECK(tst_string >= tst_string3);
    CHECK(tst_string3 < tst_string);
    CHECK(tst_string3 < tst_string);
    CHECK(tst_string <= tst_string4);
    CHECK(tst_string >= tst_string4);
    CHECK(tst_string == tst_string4);
    CHECK_FALSE(tst_string != tst_string4);
}

TEST_CASE("str::fromCharAndSize", "[str]")
{
    str text0{str::fromCharAndSize('A', 5U)};
    str text1{str::fromCharAndSize('A', 5U)};
    CHECK(text0 == "AAAAA");
    CHECK("AAAAA" == text0);
    CHECK(text0 == text1);
    CHECK(text1 == text0);

    str text2{str::fromCharAndSize('A', 4U)};
    CHECK(text0 != text2);
    CHECK_FALSE(text0 == text2);

    str text3{str::fromCharAndSize(' ', 0U)};
    str text4{str::fromCharAndSize('A', 0U)};

    CHECK(text3.empty());
    CHECK(text4.empty());
    CHECK(text3 == text4);
}
