/*! \file
    \brief A character width.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cstddef>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/grapheme_splitting/character_width.hpp>


namespace
{
    class concrete_character_width : public tetengo::text::grapheme_splitting::character_width
    {
    public:
        // constructors and destructor

        virtual ~concrete_character_width() = default;


    private:
        // virtual functions

        virtual std::size_t width_of_impl(const class_type class_, const emoji_type) const override
        {
            return class_ == class_type::fullwidth ? 2 : 1;
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(grapheme_splitting)
BOOST_AUTO_TEST_SUITE(character_width)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_character_width char_width{};
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    const concrete_character_width char_width{};

    BOOST_TEST(
        char_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::halfwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 1U);
    BOOST_TEST(
        char_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::halfwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 1U);
    BOOST_TEST(
        char_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::fullwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::emoji) == 2U);
    BOOST_TEST(
        char_width.width_of(
            tetengo::text::grapheme_splitting::character_width::class_type::fullwidth,
            tetengo::text::grapheme_splitting::character_width::emoji_type::normal) == 2U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
