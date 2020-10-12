/*! \file
    \brief An east Asian character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/character_width.hpp>
#include <tetengo/text/east_asian_character_width.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(east_asian_character_width)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& character_width = tetengo::text::east_asian_character_width::instance();
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    const auto& character_width = tetengo::text::east_asian_character_width::instance();

    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::ambiguous) == 2U);
    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::fullwidth) == 2U);
    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::halfwidth) == 1U);
    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::neutral) == 1U);
    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::narrow) == 1U);
    BOOST_TEST(character_width.width_of(tetengo::text::character_width::class_type::wide) == 2U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
