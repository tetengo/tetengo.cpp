/*!
    \brief A trie set iterator.

    Copyright (C) 2019 kaoru
 */

#include <stdexcept>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie_set.hpp>
#include <tetengo/trie/trie_set_iterator.hpp>


namespace
{
    const std::vector<std::string> expected_values_string{ "UTIGOSI", "UTO", "SETA", "SETA" };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie_set_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie_set<std::string> set_{ expected_values_string.begin(), expected_values_string.end() };

        set_.begin();
    }
    {
        const tetengo::trie::trie_set<std::string> set_{ expected_values_string.begin(), expected_values_string.end() };

        set_.end();
    }
    {
        const tetengo::trie::trie_set<std::string> set_{ expected_values_string.begin(), expected_values_string.end() };

        set_.find("SETA");
    }
    {
        const auto value = 42;
        BOOST_CHECK_THROW(tetengo::trie::trie_set_iterator<int> iterator_(&value, nullptr), std::invalid_argument);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
