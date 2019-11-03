/*!
    \brief A trie set iterator.

    Copyright (C) 2019 kaoru
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie_set_iterator.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie_set_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::trie_set_iterator<int> iterator{};
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
