/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string json0{};

    const std::string json1{ "false" };

    const std::string json2{ "false true" };

    const std::string json3{
        "[ 42, true, 3.1415926, \"hoge\", { \"Aso\": 1592, \"Tsurugi\": 1955 }, [ null, 3.0e+5 ] ]"
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_parser)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json0), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json1), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json2), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json3), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };
    }
    {
        BOOST_CHECK_THROW(
            const tetengo::json::json_parser parser{ std::unique_ptr<tetengo::json::reader>{} }, std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json0), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_TEST(!parser.has_next());
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json1), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_TEST(parser.has_next());
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json2), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_TEST(parser.has_next());
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json3), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_TEST(parser.has_next());
    }
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
