/*!
    \brief A double array.

    Copyright (C) 2019 kaoru
 */

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.h>
#include <tetengo/trie/double_array.hpp>

struct tetengo_trie_doublearray;


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
    }
    {
        tetengo_trie_doublearray* const p_double_array = tetengo_trie_doublearray_create();
        BOOST_SCOPE_EXIT(p_double_array)
        {
            tetengo_trie_doublearray_destroy(p_double_array);
        }
        BOOST_SCOPE_EXIT_END;
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
