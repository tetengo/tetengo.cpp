/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/grapheme_splitter.hpp>


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

    using g = tetengo::cli::grapheme_splitter::grapheme_type;

    const auto& splitter = tetengo::cli::grapheme_splitter::instance();

    {
        const auto offsets = splitter.split({});
        BOOST_TEST(offsets.empty());
    }
    {
        const auto offsets = splitter.split({ g::cr });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = splitter.split({ g::cr, g::cr });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::cr, g::lf });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::cr, g::control });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::cr, g::other });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::lf });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = splitter.split({ g::lf, g::cr });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::lf, g::other });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::control });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = splitter.split({ g::control, g::cr });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::control, g::other });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::extend });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::zwj });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::regional });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::spacing_mark });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::l });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
    {
        const auto offsets = splitter.split({ g::extend, g::other });
        BOOST_TEST((offsets == std::vector<std::size_t>{ 0, 1, 2 }));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
