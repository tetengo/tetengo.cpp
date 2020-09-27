/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/character_width.hpp>


namespace
{
    class concrete_character_width : public tetengo::cli::character_width
    {
    public:
        // constructors and destructor

        virtual ~concrete_character_width() = default;


    private:
        // virtual functions

        virtual std::size_t width_of_impl(const char32_t code_point) const override
        {
            return static_cast<std::size_t>(code_point) % 2 + 1;
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(cli)
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

    BOOST_TEST(char_width.width_of(U'\u1234') == 1);
    BOOST_TEST(char_width.width_of(U'\u4321') == 2);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
