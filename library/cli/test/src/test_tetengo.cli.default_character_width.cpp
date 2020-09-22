/*! \file
    \brief A default character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/default_character_width.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(cli)
BOOST_AUTO_TEST_SUITE(default_character_width)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& character_width = tetengo::cli::default_character_width::instance();
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    const auto& character_width = tetengo::cli::default_character_width::instance();

    BOOST_TEST(character_width.width_of(U'\u0042') == 1U);
    BOOST_TEST(character_width.width_of(U'\u2424') == 1U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
