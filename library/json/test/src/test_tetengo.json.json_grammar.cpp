/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/json_grammar.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_grammar)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::json_grammar grammar{};
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
