#include "catch_include.hpp"

import htypes;

using namespace htps;

TEST_CASE("BasicDictionary::BasicDictionary", "[htypes][basic_dictionry]")
{
    BasicDictionary<int, int> testBasicDictionary;
    CHECK(testBasicDictionary.add(0, 5, false));
    CHECK(testBasicDictionary.find(0)->second == 5);
    CHECK((*(testBasicDictionary.find_checked(0).second)).second == 5);
    CHECK(testBasicDictionary.find_index(0).first == true);
    CHECK(testBasicDictionary.find_index(0).second == 0);
    CHECK(testBasicDictionary.find_index(1).first == false);
 }
