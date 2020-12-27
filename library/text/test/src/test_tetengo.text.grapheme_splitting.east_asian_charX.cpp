/*! \file
    \brief An east Asian character width.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/grapheme_splitting/character_width.hpp>
#include <tetengo/text/grapheme_splitting/east_asian_characX.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(grapheme_splitting)
BOOST_AUTO_TEST_SUITE(east_asian_character_width)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& character_width =
        tetengo::text::grapheme_splitting::east_asian_character_width::instance();
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    const auto& character_width = tetengo::text::grapheme_splitting::east_asian_character_width::instance();

    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::ambiguous,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::ambiguous,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::fullwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::fullwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::halfwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 1U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::halfwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 1U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::neutral,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::neutral,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 1U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::narrow,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 1U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::narrow,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 1U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::wide,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 2U);
    BOOST_TEST(
        character_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::wide,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 2U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
