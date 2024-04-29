// fxstring_test.cpp --- The test program for fxstring class
// License: MIT

#include "fxstring.h"
#include <cstring>

template <size_t t_buf_size>
using string_t = khmz::fxstring<char, t_buf_size>;

void khmz::fxstring_unittest(void)
{
    {
        string_t<3> str;
        assert(str.empty());
        assert(str.size() == 0);
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "") == 0);
        static_assert(sizeof(str) == 3 * sizeof(char), "The total size was wrong.");
    }
    {
        string_t<5> str(3, 'A');
        assert(!str.empty());
        assert(str.size() == 3);
        assert(str.max_size() == 4);
        assert(strcmp(str.c_str(), "AAA") == 0);
    }
    {
        string_t<3> str = { '1', '2' };
        assert(!str.empty());
        assert(str.size() == 2);
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str = { '1', '2', '3' };
        assert(!str.empty());
        assert(str.size() == 2);
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str;
        str = { '1', '2' };
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str;
        str = { '1', '2', '3' };
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str("12");
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str("1234");
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str = "12";
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str = "1234";
        assert(str.max_size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);
    }
    {
        string_t<3> str;
        assert(str.empty());
        assert(str.size() == 0);
        assert(str.max_size() == 2);

        str = "1";
        assert(!str.empty());
        assert(str.size() == 1);

        str = "1234";
        assert(!str.empty());
        assert(str.size() == 2);

        str.clear();
        assert(str.empty());
        assert(str.size() == 0);
    }
    {
        string_t<4> str;
        assert(str.empty());
        assert(str.size() == 0);
        assert(str.max_size() == 3);
        assert(strcmp(str.c_str(), "") == 0);

        str.assign("12");
        assert(!str.empty());
        assert(str.size() == 2);
        assert(strcmp(str.c_str(), "12") == 0);

        str.assign("1234");
        assert(!str.empty());
        assert(str.size() == 3);
        assert(strcmp(str.c_str(), "123") == 0);

        str.clear();
        assert(str.empty());
        assert(str.size() == 0);
    }
    {
        string_t<6> str("test1");

        assert(str.compare("test1") == 0);
        assert(str.compare("test2") <  0);
        assert(str.compare("test0") >  0);
        assert(str.compare("test2") <= 0);
        assert(str.compare("test0") >= 0);
        assert(str.compare("test2") != 0);
        assert(str.compare("test0") != 0);

        assert(str == "test1");
        assert(str <  "test2");
        assert(str >  "test0");
        assert(str <= "test2");
        assert(str >= "test0");
        assert(str != "test2");
        assert(str != "test0");
    }
    {
        string_t<5> str;
        str.assign(3, 'A');
        assert(str == "AAA");
        str.assign(4, 'A');
        assert(str == "AAAA");
        str.assign(10, 'A');
        assert(str == "AAAA");

        str.assign("1234", 2);
        assert(str == "12");
        str.assign("1234", 2, 1);
        assert(str == "3");
    }
    {
        string_t<3> str;
        str = { };
        assert(str == "");
        str = { 'A', 'B' };
        assert(str == "AB");
        str = { 'A', 'B', 'C' };
        assert(str == "AB");
        str.clear();
        assert(str == "");
        str += 'A';
        assert(str == "A");
        str += { 'B', 'C' };
        assert(str == "AB");
        str.clear();
        assert(str == "");
        str.append("A");
        assert(str == "A");
        str.append('A');
        assert(str == "AA");
        str.pop_back();
        assert(str == "A");
        str.append({ 'B' });
        assert(str == "AB");
    }
    {
        string_t<8> str = "ABABAC";
        size_t ich;
        ich = str.find('A');
        assert(ich == 0);
        ich = str.find("A");
        assert(ich == 0);
        ich = str.find('A', 1);
        assert(ich == 2);
        ich = str.find("A", 1);
        assert(ich == 2);
        ich = str.find("AB", 0);
        assert(ich == 0);
        ich = str.find("AB", 1);
        assert(ich == 2);
    }
    {
        string_t<8> str = "ABABAC";
        size_t ich;
        ich = str.rfind('A');
        assert(ich == 4);
        ich = str.rfind("A");
        assert(ich == 4);
        ich = str.rfind('A', 1);
        assert(ich == 0);
        ich = str.rfind("A", 1);
        assert(ich == 0);
        ich = str.rfind("AB", 4);
        assert(ich == 2);
        ich = str.rfind("AB", 3);
        assert(ich == 2);
        ich = str.rfind("C", 0);
        assert(ich == str.npos);
        ich = str.rfind("C", str.npos);
        assert(ich == 5);
    }
    {
        string_t<8> str = "ABCEAG";
        size_t ich;
        ich = str.find_first_of('E');
        assert(ich == 3);
        ich = str.find_first_of('E', 4);
        assert(ich == str.npos);
        ich = str.find_first_of("ECF");
        assert(ich == 2);
        ich = str.find_first_of("ECF", 3);
        assert(ich == 3);
        ich = str.find_first_of('A', 1);
        assert(ich == 4);
        ich = str.find_first_of("ECF", 1);
        assert(ich == 2);
    }
    {
        string_t<8> str = "ABCEAG";
        size_t ich;
        ich = str.find_first_not_of("ABC");
        assert(ich == 3);
        ich = str.find_first_not_of('E', 3);
        assert(ich == 4);
        ich = str.find_first_not_of("AEC", 2);
        assert(ich == 5);
        ich = str.find_first_not_of('B', 1);
        assert(ich == 2);
        ich = str.find_first_not_of("ABCEAG");
        assert(ich == str.npos);
    }
    {
        string_t<8> str = "ABCEAG";
        size_t ich;
        ich = str.find_last_not_of("ABC");
        assert(ich == 5);
        ich = str.find_last_not_of('E', 3);
        assert(ich == 2);
        ich = str.find_last_not_of("AEC", 3);
        assert(ich == 1);
        ich = str.find_last_not_of('B', 1);
        assert(ich == 0);
        ich = str.find_last_not_of("ABCEAG");
        assert(ich == str.npos);
    }
    {
        string_t<8> str = "ABCEAG";
        assert(str.erase(1, 3) == "AAG");
        assert(str == "AAG");
        str += "GGGG";
        assert(str == "AAGGGGG");
        str.erase(str.cbegin() + 2);
        assert(str == "AAGGGG");
        str.erase(str.cbegin() + 1, str.cbegin() + 2);
        assert(str == "AGGGG");
    }
    {
        string_t<4> str = "ABC";
        size_t i = 0;
        for (auto& ch : str)
        {
            switch (i)
            {
            case 0: assert(ch == 'A'); break;
            case 1: assert(ch == 'B'); break;
            case 2: assert(ch == 'C'); break;
            default:
                assert(0);
            }
            ++i;
        }
        assert(i == 3);
    }
    {
        string_t<4> str = "ABC";
        size_t i = 0;
        for (auto it = str.rbegin(); it != str.rend(); ++it)
        {
            switch (i)
            {
            case 0: assert(*it == 'C'); break;
            case 1: assert(*it == 'B'); break;
            case 2: assert(*it == 'A'); break;
            default:
                assert(0);
            }
            ++i;
        }
        assert(i == 3);
    }
    {
        string_t<8> str("BA");
        string_t<8>::iterator it = str.begin();
        string_t<8>::const_iterator cit = it;
    }
    {
        string_t<8> str = "ABC";
        str.insert(str.begin(), "D");
        assert(str == "DABC");
        str.insert(str.end(), "D");
        assert(str == "DABCD");
        str.insert(str.begin() + 1, "ABCDE");
        assert(str == "DABCDEA");
        str.clear();
        str.insert(str.end(), "AB");
        str.insert(str.begin() + 1, "AB");
        assert(str == "AABB");
        str.insert(str.end(), "ABCDEFG");
        assert(str == "AABBABC");
        str.clear();
        str.insert(0, "ABC");
        assert(str == "ABC");
        str.insert(3, "abc");
        assert(str == "ABCabc");
        str.insert(5, "123");
        assert(str == "ABCab12");
        str.insert(3, 3, 'X');
        assert(str == "ABCXXXa");
        str.insert(3, 5, 'Y');
        assert(str == "ABCYYYY");
    }
    {
        string_t<4> str = "ABC";
        str.replace(0, 1, "Z");
        assert(str == "ZBC");
        str.replace(0, 1, "xx");
        assert(str == "xxB");
        str = "ABC";
        str.replace(2, 2, "A");
        assert(str == "ABA");
        str.replace(1, 3, "");
        assert(str == "A");
    }
}

int main(void)
{
    khmz::fxstring_unittest();
    std::puts("success");
    return 0;
}
