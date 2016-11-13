#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#include "vector.hpp"
#include <vector>

TEST_CASE( "Test fsx::vector" )
{
    fsx::vector<size_t> fsxvec;
    std::vector<size_t> stdvec;

    SECTION("Test size()")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.size() == stdvec.size());
    }

    SECTION("Test empty() and clear()")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.empty() == stdvec.empty());
        fsxvec.clear();
        stdvec.clear();
        REQUIRE(fsxvec.empty() == stdvec.empty());
    }

    SECTION("Test push_back()")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        size_t x = 0;
        for (auto& i : stdvec)
        {
            REQUIRE(i == fsxvec[x]);
            ++x;
        }
    }

    SECTION("Test at()")
    {
        for (int i = 0; i < 100; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.at(10) == stdvec.at(10));
        REQUIRE(fsxvec.at(5) == stdvec.at(5));
    }

    SECTION("Test resize()")
    {
        for (int i = 0; i < 10000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.size() == stdvec.size());

        fsxvec.resize(100);
        stdvec.resize(100);

        REQUIRE(fsxvec.size() == stdvec.size());

        size_t x = 0;
        for (auto& i : stdvec)
        {
            REQUIRE(i == fsxvec[x]);
            ++x;
        }

        fsxvec.push_back(101);
        stdvec.push_back(101);
        REQUIRE(stdvec[100] == fsxvec[100]);
    }

    SECTION("Test shrink_to_fit()")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.capacity() >= stdvec.capacity());
        fsxvec.shrink_to_fit();
        stdvec.shrink_to_fit();
        REQUIRE(fsxvec.capacity() == stdvec.capacity());
        fsxvec.clear();
        stdvec.clear();
        fsxvec.shrink_to_fit();
        stdvec.shrink_to_fit();
        REQUIRE(fsxvec.size() == stdvec.size());
        REQUIRE(fsxvec.capacity() == stdvec.capacity());
    }

    SECTION("Test reserve()")
    {
        REQUIRE(fsxvec.capacity() >= stdvec.capacity());
        fsxvec.reserve(1000);
        stdvec.reserve(1000);
        REQUIRE(fsxvec.capacity() == stdvec.capacity());
        REQUIRE(fsxvec.size() == stdvec.size());

        for (int i = 0; i < 10000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        REQUIRE(fsxvec.size() == stdvec.size());
        REQUIRE(fsxvec.capacity() >= stdvec.capacity());
        fsxvec.reserve(10);
        stdvec.reserve(10);
        REQUIRE(fsxvec.capacity() >= stdvec.capacity());
    }

    SECTION("Test erase(begin)")
    {
        for (int i = 0; i < 10000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        auto fsxiter = fsxvec.begin();
        auto stditer = stdvec.begin();

        int x = 0;
        while (fsxiter != fsxvec.end())
        {
            if (x % 2 == 0)
            {
                fsxiter = fsxvec.erase(fsxiter);
            }
            else
            {
                ++fsxiter;
            }
            ++x;
        }

        x = 0;
        while (stditer != stdvec.end())
        {
            if (x % 2 == 0)
            {
                stditer = stdvec.erase(stditer);
            }
            else
            {
                ++stditer;
            }
            ++x;
        }

        REQUIRE(fsxvec.size() == stdvec.size());

        for (size_t i = 0; i < stdvec.size(); ++i)
        {
            REQUIRE(fsxvec[i] == stdvec[i]);
        }
    }

    SECTION("Test erase(end)")
    {
        for (int i = 0; i < 10; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        fsxvec.erase(fsxvec.end() - 4);
        stdvec.erase(stdvec.end() - 4);

        for (size_t i = 0; i < stdvec.size(); ++i)
        {
            REQUIRE(fsxvec[i] == stdvec[i]);
        }
    }

    SECTION("Test values in erase(begin)")
    {
        for (int i = 0; i < 10; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }

        fsxvec.erase(fsxvec.begin() + 1);
        stdvec.erase(stdvec.begin() + 1);

        REQUIRE(fsxvec.size() == stdvec.size());
        for (size_t i = 0; i < stdvec.size(); ++i)
        {
            REQUIRE(stdvec[i] == fsxvec[i]);
        }
    }

    SECTION("Test erase(iter, iter)")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }
        fsxvec.erase(fsxvec.begin(), fsxvec.end());
        stdvec.erase(stdvec.begin(), stdvec.end());
        REQUIRE(fsxvec.size() == stdvec.size());
    }

    SECTION("Test values in erase(iter, iter)")
    {
        for (int i = 0; i < 1000; ++i)
        {
            fsxvec.push_back(i);
            stdvec.push_back(i);
        }
        fsxvec.erase(fsxvec.begin() + 1, fsxvec.end() - 12);
        stdvec.erase(stdvec.begin() + 1, stdvec.end() - 12);

        REQUIRE(fsxvec.size() == stdvec.size());

        for (size_t i = 0; i < stdvec.size(); ++i)
        {
            REQUIRE(stdvec[i] == fsxvec[i]);
        }
    }
}
