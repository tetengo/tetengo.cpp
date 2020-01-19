/*!
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie_iterator.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie_iterator iterator{};
    }
    {
        // TODO: C style API
    }

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(increment)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
