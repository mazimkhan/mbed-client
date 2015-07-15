/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_m2mstring.h"


Test_M2MString::Test_M2MString()
{
    str = new String("test");
}

Test_M2MString::~Test_M2MString()
{
    delete str;
}

void Test_M2MString::test_copy_constructor()
{
    String s("name");
    String s1(s);
    CHECK(s1.p[1] == 'a');
}

void Test_M2MString::test_operator_assign()
{
    String s("name");
    String s1 = s;
    const String s2("yes");
    CHECK(s1.p[1] == 'a');

    s1 = "yeb";
    CHECK(s1.p[1] == 'e');
    s = s2;
    CHECK(s.p[1] == 'e');
}

void Test_M2MString::test_operator_add()
{
    String s("name");
    String s1("yeb");

    s += 'r';
    CHECK(s.p[4] == 'r');

    s += s1;
    CHECK(s.p[5] == 'y');

    s += "hoi";
    CHECK(s.p[9] == 'o');

    s1 += "somethingverylongggggggg";

}

void Test_M2MString::test_push_back()
{
    String s("name");

    s.push_back('r');
    CHECK(s.p[4] == 'r');
}

void Test_M2MString::test_operator_equals()
{
    String s("name");
    String s1("yeb");

    CHECK( (s == s1) == false);
    CHECK( (s == "name") == true);
    const char c = NULL;
    CHECK( (s == c) == false);
    free(s.p);
    s.p = NULL;
    CHECK( (s == c) == true);
}

void Test_M2MString::test_clear()
{
    String s("name");

    s.clear();
    CHECK(s.size_ == 0);
}

void Test_M2MString::test_size()
{
    String s("name");
    String s1("yeb");

    CHECK(s.size() == 4);
}

void Test_M2MString::test_length()
{
    String s("name");
    String s1("yeb");

    CHECK(s.length() == 4);
}

void Test_M2MString::test_capacity()
{
    String s("name");
    String s1("yeb");

    CHECK(s.capacity() == 4);
}

void Test_M2MString::test_empty()
{
    String s("name");
    String s1("yeb");

    CHECK(s.empty() == false);
}

void Test_M2MString::test_c_str()
{
    String s("name");
    String s1("yeb");

    CHECK( s.c_str() != NULL);
}

void Test_M2MString::test_reserve()
{
    String s("name");
    String s1("yeb");

    s.reserve(12);
    CHECK(s.allocated_ == 13);
}

void Test_M2MString::test_resize()
{
    String s("name");
    String s1("yeb");

    s.resize(2);
    CHECK( s.size() == 2);

    s.resize(12);
    CHECK( s.size() == 12);
}

void Test_M2MString::test_swap()
{
    String s("name");
    String s1("yeb");

    s.swap(s1);
    CHECK(s1.p[1] == 'a');

    CHECK(s.p[1] == 'e');
}

void Test_M2MString::test_substr()
{
    String s("name");
    String s1("yeb");

    CHECK(s.substr(1, 1) == "a");
    CHECK(s.substr(3, 4) == "e");
}

void Test_M2MString::test_operator_get()
{
    String s("name");
    const String s1("yeb");

    CHECK(s[1] == 'a');
    CHECK(s1[1] == 'e');
}

void Test_M2MString::test_at()
{
    String s("name");
    const String s1("yeb");

    CHECK(s.at(1) == 'a');
    CHECK(s.at(14) == '\0');
    CHECK(s1.at(1) == 'e');
    CHECK(s1.at(31) == '\0');
}

void Test_M2MString::test_erase()
{
    String s("name");
    String s1("yeb");

    s.erase(1,1);
    CHECK(s[1] == 'm');
}

void Test_M2MString::test_append()
{
    String s("name");
    String s1("yeb");

    s.append( s1.c_str(), 1 );
    CHECK(s.size() == 5);

    s.append( s1.c_str(), 15 );
    CHECK(s.size() == 8);
}

void Test_M2MString::test_compare()
{
    String s("name");
    String s1("yeb");
    String s2("name");
    String s3("nam");

    CHECK(s.compare(1,5, s1) < 0);
    CHECK(s1.compare(0,5, s2) > 0);
    CHECK(s.compare(0,4, s2) == 0);
    CHECK(s.compare(0,4, s3) > 0);

    CHECK(s.compare(1,5, "yeb") < 0);
    CHECK(s1.compare(0,5, "name") > 0);
    CHECK(s.compare(0,4, "name") == 0);
    CHECK(s.compare(0,4, "nam") > 0);
}

void Test_M2MString::test_find_last_of()
{
    String s("namenamename");
    String s1("yeb");

    CHECK(s.find_last_of('n') == 8);
}

void Test_M2MString::test_operator_lt()
{
    String s("name");
    String s1("yeb");
    String s2("yea");

    CHECK( (s < s1 ) == true);
    CHECK( (s1 < s2 ) == false);
}
