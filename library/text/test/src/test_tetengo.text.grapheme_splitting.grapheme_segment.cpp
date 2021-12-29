/*! \file
    \brief A grapheme segment.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/grapheme_splitting/grapheme_segment.hpp>


namespace
{
    using bp = tetengo::text::grapheme_splitting::grapheme_segment::break_property_type;

    constexpr bp graphemes[] = { bp::cr,           bp::lf, bp::control, bp::extend, bp::zwj, bp::regional, bp::prepend,
                                 bp::spacing_mark, bp::l,  bp::v,       bp::t,      bp::lv,  bp::lvt,      bp::other };

    bool contains(const std::vector<std::pair<bp, bp>>& connectings, const bp from, const bp to)
    {
        return std::find(std::begin(connectings), std::end(connectings), std::make_pair(from, to)) !=
               std::end(connectings);
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(grapheme_splitting)
BOOST_AUTO_TEST_SUITE(grapheme_segment)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& segment_maker = tetengo::text::grapheme_splitting::grapheme_segment::instance();
}

BOOST_AUTO_TEST_CASE(segment_offsets)
{
    BOOST_TEST_PASSPOINT();

    const auto& segment_maker = tetengo::text::grapheme_splitting::grapheme_segment::instance();

    {
        const auto offsets = segment_maker.segment_offsets({});
        BOOST_TEST(std::empty(offsets));
    }
    {
        std::vector<std::pair<bp, bp>> connectings;
        for (auto i = std::begin(graphemes); i != std::end(graphemes); ++i)
        {
            {
                const auto offsets = segment_maker.segment_offsets({ *i });
                BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
            }

            for (auto j = std::begin(graphemes); j != std::end(graphemes); ++j)
            {
                const auto offsets = segment_maker.segment_offsets({ *i, *j });

                BOOST_CHECK(std::size(offsets) == 2U || std::size(offsets) == 3U);
                if (std::size(offsets) == 2)
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
        BOOST_TEST(std::size(connectings) == 54U);
        BOOST_TEST(contains(connectings, bp::cr, bp::lf));
        BOOST_TEST(contains(connectings, bp::extend, bp::extend));
        BOOST_TEST(contains(connectings, bp::extend, bp::zwj));
        BOOST_TEST(contains(connectings, bp::extend, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::zwj, bp::extend));
        BOOST_TEST(contains(connectings, bp::zwj, bp::zwj));
        BOOST_TEST(contains(connectings, bp::zwj, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::zwj, bp::other));
        BOOST_TEST(contains(connectings, bp::regional, bp::extend));
        BOOST_TEST(contains(connectings, bp::regional, bp::zwj));
        BOOST_TEST(contains(connectings, bp::regional, bp::regional));
        BOOST_TEST(contains(connectings, bp::regional, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::prepend, bp::extend));
        BOOST_TEST(contains(connectings, bp::prepend, bp::zwj));
        BOOST_TEST(contains(connectings, bp::prepend, bp::regional));
        BOOST_TEST(contains(connectings, bp::prepend, bp::prepend));
        BOOST_TEST(contains(connectings, bp::prepend, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::prepend, bp::l));
        BOOST_TEST(contains(connectings, bp::prepend, bp::v));
        BOOST_TEST(contains(connectings, bp::prepend, bp::t));
        BOOST_TEST(contains(connectings, bp::prepend, bp::lv));
        BOOST_TEST(contains(connectings, bp::prepend, bp::lvt));
        BOOST_TEST(contains(connectings, bp::prepend, bp::other));
        BOOST_TEST(contains(connectings, bp::spacing_mark, bp::extend));
        BOOST_TEST(contains(connectings, bp::spacing_mark, bp::zwj));
        BOOST_TEST(contains(connectings, bp::spacing_mark, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::l, bp::extend));
        BOOST_TEST(contains(connectings, bp::l, bp::zwj));
        BOOST_TEST(contains(connectings, bp::l, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::l, bp::l));
        BOOST_TEST(contains(connectings, bp::l, bp::v));
        BOOST_TEST(contains(connectings, bp::l, bp::lv));
        BOOST_TEST(contains(connectings, bp::l, bp::lvt));
        BOOST_TEST(contains(connectings, bp::v, bp::extend));
        BOOST_TEST(contains(connectings, bp::v, bp::zwj));
        BOOST_TEST(contains(connectings, bp::v, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::v, bp::v));
        BOOST_TEST(contains(connectings, bp::v, bp::t));
        BOOST_TEST(contains(connectings, bp::t, bp::extend));
        BOOST_TEST(contains(connectings, bp::t, bp::zwj));
        BOOST_TEST(contains(connectings, bp::t, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::t, bp::t));
        BOOST_TEST(contains(connectings, bp::lv, bp::extend));
        BOOST_TEST(contains(connectings, bp::lv, bp::zwj));
        BOOST_TEST(contains(connectings, bp::lv, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::lv, bp::v));
        BOOST_TEST(contains(connectings, bp::lv, bp::t));
        BOOST_TEST(contains(connectings, bp::lvt, bp::extend));
        BOOST_TEST(contains(connectings, bp::lvt, bp::zwj));
        BOOST_TEST(contains(connectings, bp::lvt, bp::spacing_mark));
        BOOST_TEST(contains(connectings, bp::lvt, bp::t));
        BOOST_TEST(contains(connectings, bp::other, bp::extend));
        BOOST_TEST(contains(connectings, bp::other, bp::zwj));
        BOOST_TEST(contains(connectings, bp::other, bp::spacing_mark));
    }
    {
        const auto offsets = segment_maker.segment_offsets({ bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = segment_maker.segment_offsets({ bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = segment_maker.segment_offsets({ bp::regional, bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 3 }));
    }
    {
        const auto offsets = segment_maker.segment_offsets({ bp::regional, bp::regional, bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 4 }));
    }
    {
        const auto offsets =
            segment_maker.segment_offsets({ bp::regional, bp::regional, bp::regional, bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 4, 5 }));
    }
    {
        const auto offsets =
            segment_maker.segment_offsets({ bp::regional, bp::regional, bp::extend, bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 3, 5 }));
    }
    {
        const auto offsets =
            segment_maker.segment_offsets({ bp::regional, bp::regional, bp::prepend, bp::regional, bp::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2, 5 }));
    }
    {
        const auto offsets = segment_maker.segment_offsets({
            bp::other, //  U+1F469
            bp::zwj, //    U+200D
            bp::other, //  U+2764
            bp::extend, // U+FE0F
            bp::zwj, //    U+200D
            bp::other, //  U+1F48B
            bp::zwj, //    U+200D
            bp::other //   U+1F468
        });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 8 }));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
