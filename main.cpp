#include "MyString.hpp"
#include <iostream>
#include <cassert>
#include <string>

int main() {
    // Basic-0: Constructor
    {
        MyString s1;
        assert(s1.size() == 0);
        assert(s1.capacity() == 15);

        MyString s2("hello");
        assert(s2.size() == 5);
        assert(s2.capacity() == 15);
        assert(std::string(s2.c_str()) == "hello");

        MyString s3("this is a very long string that exceeds SSO");
        assert(s3.size() == 43);
        assert(s3.capacity() >= 43);
        assert(std::string(s3.c_str()) == "this is a very long string that exceeds SSO");
    }

    // Basic-1: Copy and Move
    {
        MyString s1("hello");
        MyString s2 = s1;
        assert(std::string(s2.c_str()) == "hello");
        assert(s2.size() == 5);

        MyString s3 = std::move(s1);
        assert(std::string(s3.c_str()) == "hello");
        assert(s1.size() == 0);

        MyString s4("long string long string long string");
        MyString s5 = s4;
        assert(std::string(s5.c_str()) == s4.c_str());
        
        MyString s6 = std::move(s4);
        assert(s4.size() == 0);
    }

    // Basic-2: append() and operator+
    {
        MyString s1("hello");
        s1.append(" world");
        assert(std::string(s1.c_str()) == "hello world");

        MyString s2("!!!");
        MyString s3 = s1 + s2;
        assert(std::string(s3.c_str()) == "hello world!!!");
    }

    // Basic-4: operator[] and at
    {
        MyString s1("hello");
        assert(s1[0] == 'h');
        assert(s1.at(1) == 'e');
        s1[0] = 'H';
        assert(s1[0] == 'H');
        
        try {
            s1.at(10);
            assert(false);
        } catch (const std::out_of_range&) {
        }
    }

    // Basic-5: resize
    {
        MyString s1("hello world");
        s1.resize(5);
        assert(std::string(s1.c_str()) == "hello");
        assert(s1.size() == 5);

        s1.resize(10);
        assert(s1.size() == 10);
        for (int i = 5; i < 10; ++i) {
            assert(s1[i] == '\0');
        }
    }

    // Iterators
    {
        MyString s1("abc");
        std::string res;
        for (auto it = s1.begin(); it != s1.end(); ++it) {
            res += *it;
        }
        assert(res == "abc");

        const MyString s2("def");
        res = "";
        for (auto it = s2.cbegin(); it != s2.cend(); ++it) {
            res += *it;
        }
        assert(res == "def");
    }

    std::cout << "All local tests passed!" << std::endl;
    return 0;
}
