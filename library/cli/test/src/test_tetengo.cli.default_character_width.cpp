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

    BOOST_TEST(character_width.width_of(0, U'\u00A4') == 1U); // A
    BOOST_TEST(character_width.width_of(0, U'\uFF41') == 2U); // F
    BOOST_TEST(character_width.width_of(0, U'\uFF42') == 2U);
    BOOST_TEST(character_width.width_of(0, U'\uFF71') == 1U); // H
    BOOST_TEST(character_width.width_of(0, U'\uFF72') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u00A9') == 1U); // N
    BOOST_TEST(character_width.width_of(0, U'\u0061') == 1U); // Na
    BOOST_TEST(character_width.width_of(0, U'\u0062') == 1U);
    BOOST_TEST(character_width.width_of(0, U'\u3400') == 2U); // W
    BOOST_TEST(character_width.width_of(0, U'\u3401') == 2U);

    BOOST_TEST(character_width.width_of(0, U'\u0300') == 1U); // A
    BOOST_TEST(character_width.width_of(0, U'\u0301') == 1U);
    BOOST_TEST(character_width.width_of(U'\u00A4', U'\u0300') == 0U);
    BOOST_TEST(character_width.width_of(U'\u00A4', U'\u0301') == 0U);
    BOOST_TEST(character_width.width_of(0, U'\u20E1') == 1U); // N
    BOOST_TEST(character_width.width_of(U'\u00A9', U'\u20E1') == 0U);
    BOOST_TEST(character_width.width_of(0, U'\u3099') == 2U); // W
    BOOST_TEST(character_width.width_of(U'\u306F', U'\u3099') == 0U);
    BOOST_TEST(character_width.width_of(0, U'\u309A') == 2U);
    BOOST_TEST(character_width.width_of(U'\u306F', U'\u309A') == 0U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
