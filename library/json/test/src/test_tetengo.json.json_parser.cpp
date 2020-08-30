/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string json0{};

    const std::vector<std::string> expected_parsed0{};

    const std::string json1{ "false" };

    const std::vector<std::string> expected_parsed1{ "boolean false" };

    const std::string json2{ "false true" };

    const std::vector<std::string> expected_parsed2{ "boolean false" };

    const std::string json3{
        "[ 42, true, 3.1415926, \"hoge\", { \"Aso\": 1592, \"Tsurugi\": 1955 }, [ null, 3.0e+5 ] ]"
    };

    const std::vector<std::string> expected_parsed3{
        "array open" /*                                                      */,
        "number 42" /*                                                       */,
        "boolean true" /*                                                    */,
        "number 3.1415926" /*                                                */,
        "string hoge" /*                                                     */,
        "object open" /*                                                     */,
        "member open name:Aso" /*                                            */,
        "number 1592" /*                                                     */,
        "member close" /*                                                    */,
        "member open name:Tsurugi" /*                                        */,
        "number 1955" /*                                                     */,
        "member close" /*                                                    */,
        "object close" /*                                                    */,
        "array open" /*                                                      */,
        "null null" /*                                                       */,
        "number 3.0e+5" /*                                                   */,
        "array close" /*                                                     */,
        "array close" /*                                                     */,
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_parser)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

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

    BOOST_WARN_MESSAGE(false, "Implement it.");
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
