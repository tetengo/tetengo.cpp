/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

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

    const auto& splitter = tetengo::cli::grapheme_splitter::instance();

    {
        static const std::vector<char32_t> code_points{};
        const auto                         offsets = splitter.split(code_points);
        BOOST_TEST(offsets.empty());
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
