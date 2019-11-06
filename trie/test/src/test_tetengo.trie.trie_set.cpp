/*!
    \brief A trie set.

    Copyright (C) 2019 kaoru
 */

#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie_set.hpp>


namespace
{
    const std::vector<std::string> expected_values_string{ "UTIGOSI", "UTO", "SETA", "SETA" };

    const std::vector<std::wstring> expected_values_string2{
        std::wstring{ 0x6253, 0x8D8A }, // Utigosi in Kanji
        std::wstring{ 0x5B87, 0x571F }, // Uto in Kanji
        std::wstring{ 0x702C, 0x7530 }, // Seta in Kanji
        std::wstring{ 0x702C, 0x7530 } // Seta in Kanji
    };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie_set)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie_set<std::string> set_{};

        // BOOST_TEST(set_.size() == 0);
    }
    {
        const tetengo::trie::trie_set<int> set_{};

        // BOOST_TEST(set_.size() == 0);
    }

    {
        const tetengo::trie::trie_set<std::string> set_{ expected_values_string.begin(), expected_values_string.end() };

        BOOST_TEST_REQUIRE(set_.size() == 3);
        // BOOST_TEST(*set_.find(expected_values_string[0]) == expected_values_string[0]);
        // BOOST_TEST(*set_.find(expected_values_string[1]) == expected_values_string[1]);
        // BOOST_TEST(*set_.find(expected_values_string[2]) == expected_values_string[2]);
    }

    {
        const tetengo::trie::trie_set<std::string> set_{ expected_values_string[0],
                                                         expected_values_string[1],
                                                         expected_values_string[2] };

        BOOST_TEST_REQUIRE(set_.size() == 3);
        // BOOST_TEST(*set_.find(expected_values_string[0]) == expected_values_string[0]);
        // BOOST_TEST(*set_.find(expected_values_string[1]) == expected_values_string[1]);
        // BOOST_TEST(*set_.find(expected_values_string[2]) == expected_values_string[2]);
    }

    {
        const tetengo::trie::trie_set<std::wstring> set_{ expected_values_string2.begin(), expected_values_string2.end() };

        BOOST_TEST_REQUIRE(set_.size() == 3);
        // BOOST_TEST(*set_.find(expected_values_string[0]) == expected_values_string[0]);
        // BOOST_TEST(*set_.find(expected_values_string[1]) == expected_values_string[1]);
        // BOOST_TEST(*set_.find(expected_values_string[2]) == expected_values_string[2]);
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();
    {
        const tetengo::trie::trie_set<std::string> set_{};

        set_.find("SETA");
    }
}

BOOST_AUTO_TEST_CASE(begin)
{
    BOOST_TEST_PASSPOINT();
    {
        const tetengo::trie::trie_set<std::string> set_{};

        set_.begin();
    }
}

BOOST_AUTO_TEST_CASE(end)
{
    BOOST_TEST_PASSPOINT();
    {
        const tetengo::trie::trie_set<std::string> set_{};

        set_.end();
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
