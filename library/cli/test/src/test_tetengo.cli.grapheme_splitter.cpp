/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/grapheme_splitter.hpp>


namespace
{
    using g_type = tetengo::cli::grapheme_splitter::grapheme_type;

    constexpr g_type graphemes[] = { g_type::cr,  g_type::lf,       g_type::control, g_type::extend,
                                     g_type::zwj, g_type::regional, g_type::prepend, g_type::spacing_mark,
                                     g_type::l,   g_type::v,        g_type::t,       g_type::lv,
                                     g_type::lvt, g_type::other };

    bool contains(const std::vector<std::pair<g_type, g_type>>& connectings, const g_type from, const g_type to)
    {
        return std::find(std::begin(connectings), std::end(connectings), std::make_pair(from, to)) !=
               std::end(connectings);
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(cli)
BOOST_AUTO_TEST_SUITE(grapheme_splitter)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& splitter = tetengo::cli::grapheme_splitter::instance();
}

BOOST_AUTO_TEST_CASE(split)
{
    BOOST_TEST_PASSPOINT();

    const auto& splitter = tetengo::cli::grapheme_splitter::instance();

    {
        const auto offsets = splitter.split({});
        BOOST_TEST(offsets.empty());
    }
    {
        std::vector<std::pair<g_type, g_type>> connectings;
        for (auto i = std::begin(graphemes); i != std::end(graphemes); ++i)
        {
            {
                const auto offsets = splitter.split({ *i });
                BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
            }

            for (auto j = std::begin(graphemes); j != std::end(graphemes); ++j)
            {
                const auto offsets = splitter.split({ *i, *j });

                BOOST_CHECK(offsets.size() == 2U || offsets.size() == 3U);
                if (offsets.size() == 2)
                {
                    BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
                    connectings.emplace_back(*i, *j);
                }
                else
                {
                    BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
                }
            }
        }
        BOOST_TEST(connectings.size() == 54U);
        BOOST_TEST(contains(connectings, g_type::cr, g_type::lf));
        BOOST_TEST(contains(connectings, g_type::extend, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::extend, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::extend, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::zwj, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::zwj, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::zwj, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::zwj, g_type::other));
        BOOST_TEST(contains(connectings, g_type::regional, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::regional, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::regional, g_type::regional));
        BOOST_TEST(contains(connectings, g_type::regional, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::regional));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::prepend));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::l));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::v));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::t));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::lv));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::lvt));
        BOOST_TEST(contains(connectings, g_type::prepend, g_type::other));
        BOOST_TEST(contains(connectings, g_type::spacing_mark, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::spacing_mark, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::spacing_mark, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::l, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::l, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::l, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::l, g_type::l));
        BOOST_TEST(contains(connectings, g_type::l, g_type::v));
        BOOST_TEST(contains(connectings, g_type::l, g_type::lv));
        BOOST_TEST(contains(connectings, g_type::l, g_type::lvt));
        BOOST_TEST(contains(connectings, g_type::v, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::v, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::v, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::v, g_type::v));
        BOOST_TEST(contains(connectings, g_type::v, g_type::t));
        BOOST_TEST(contains(connectings, g_type::t, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::t, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::t, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::t, g_type::t));
        BOOST_TEST(contains(connectings, g_type::lv, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::lv, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::lv, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::lv, g_type::v));
        BOOST_TEST(contains(connectings, g_type::lv, g_type::t));
        BOOST_TEST(contains(connectings, g_type::lvt, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::lvt, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::lvt, g_type::spacing_mark));
        BOOST_TEST(contains(connectings, g_type::lvt, g_type::t));
        BOOST_TEST(contains(connectings, g_type::other, g_type::extend));
        BOOST_TEST(contains(connectings, g_type::other, g_type::zwj));
        BOOST_TEST(contains(connectings, g_type::other, g_type::spacing_mark));
    }
    {
        const auto offsets = splitter.split({ g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = splitter.split({ g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = splitter.split({ g_type::regional, g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 3 }));
    }
    {
        const auto offsets = splitter.split({ g_type::regional, g_type::regional, g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 4 }));
    }
    {
        const auto offsets = splitter.split(
            { g_type::regional, g_type::regional, g_type::regional, g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 4, 5 }));
    }
    {
        const auto offsets =
            splitter.split({ g_type::regional, g_type::regional, g_type::extend, g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 3, 5 }));
    }
    {
        const auto offsets =
            splitter.split({ g_type::regional, g_type::regional, g_type::prepend, g_type::regional, g_type::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 5 }));
    }
    {
        const auto offsets = splitter.split({
            g_type::other, // U+1F469
            g_type::zwj, // U+200D
            g_type::other, //  U+2764
            g_type::extend, // U+FE0F
            g_type::zwj, // U+200D
            g_type::other, // U+1F48B
            g_type::zwj, // U+200D
            g_type::other // U+1F468
        });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 8 }));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
