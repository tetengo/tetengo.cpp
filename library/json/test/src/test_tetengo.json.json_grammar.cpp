/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/json_grammar.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    std::string json0{};

    std::string json1{ "false" };

    std::string json2{ "false true" };

    std::string json3{ "[ 1, true, 3.1415926, \"hoge\" ]" };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_grammar)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::json_grammar grammar{};
}

BOOST_AUTO_TEST_CASE(parse)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::json_grammar grammar{};

    {
        auto                         p_stream = std::make_unique<std::istringstream>(json0);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(!successful);
    }
    {
        auto                         p_stream = std::make_unique<std::istringstream>(json1);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(successful);
    }
    {
        auto                         p_stream = std::make_unique<std::istringstream>(json2);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(!successful);
    }
    {
        auto                         p_stream = std::make_unique<std::istringstream>(json3);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(successful);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
