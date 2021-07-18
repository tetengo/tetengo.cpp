/*! \file
    \brief The usage test of tetengo::trie.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/


#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include "usage_tetengo.trie.search_c.h"
#include "usage_tetengo.trie.search_cpp.hpp"


BOOST_AUTO_TEST_SUITE(usagetest_tetengo)
BOOST_AUTO_TEST_SUITE(trie)


BOOST_AUTO_TEST_CASE(search)
{
    BOOST_TEST_PASSPOINT();

    usage_tetengo::trie::search();
    usage_tetengo_trie_search();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
