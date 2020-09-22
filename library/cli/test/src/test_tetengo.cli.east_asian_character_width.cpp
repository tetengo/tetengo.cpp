/*! \file
    \brief An east Asian character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/east_asian_character_width.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(cli)
BOOST_AUTO_TEST_SUITE(east_asian_character_width)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& character_width = tetengo::cli::east_asian_character_width::instance();
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    const auto& character_width = tetengo::cli::east_asian_character_width::instance();

    BOOST_TEST(character_width.width_of(0, U'\u0041') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u0042') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u0300') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u0303') == 1U);
    BOOST_TEST(character_width.width_of(U'\u0041', U'\u0300') == 0U);
    BOOST_TEST(character_width.width_of(U'\u0041', U'\u0303') == 0U);

    BOOST_TEST(character_width.width_of(0, U'\u309B') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u309C') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u3099') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u309A') == 1U);
    BOOST_TEST(character_width.width_of(U'\u304B', U'\u3099') == 0U);
    BOOST_TEST(character_width.width_of(U'\u304B', U'\u309A') == 0U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
