/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/json_grammar.hpp>
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
        // clang-format off
        "array open",
        "number 42",
        "boolean true",
        "number 3.1415926",
        "string hoge",
        "object open",
        "member open name:Aso",
        "number 1592",
        "member close",
        "member open name:Tsurugi",
        "number 1955",
        "member close",
        "object close",
        "array open",
        "null null",
        "number 3.0e+5",
        "array close",
        "array close",
        // clang-format on
    };

    struct primitive_handler_type
    {
        std::vector<std::string>& parsed;

        static std::string to_string(const tetengo::json::json_grammar::primitive_type_type type)
        {
            switch (type)
            {
            case tetengo::json::json_grammar::primitive_type_type::string:
                return "string";
            case tetengo::json::json_grammar::primitive_type_type::number:
                return "number";
            case tetengo::json::json_grammar::primitive_type_type::boolean:
                return "boolean";
            default:
                assert(type == tetengo::json::json_grammar::primitive_type_type::null);
                return "null";
            }
        }

        explicit primitive_handler_type(std::vector<std::string>& parsed) : parsed{ parsed } {}

        bool
        operator()(const tetengo::json::json_grammar::primitive_type_type type, const std::string_view& value) const
        {
            parsed.push_back(to_string(type) + " " + std::string{ value });
            return true;
        }
    };

    struct structure_handler_type
    {
        std::vector<std::string>& parsed;

        static std::string to_string(const tetengo::json::json_grammar::structure_type_type type)
        {
            switch (type)
            {
            case tetengo::json::json_grammar::structure_type_type::object:
                return "object";
            case tetengo::json::json_grammar::structure_type_type::member:
                return "member";
            default:
                assert(type == tetengo::json::json_grammar::structure_type_type::array);
                return "array";
            }
        }

        static std::string to_string(const tetengo::json::json_grammar::structure_open_close_type open_close)
        {
            switch (open_close)
            {
            case tetengo::json::json_grammar::structure_open_close_type::open:
                return "open";
            default:
                assert(open_close == tetengo::json::json_grammar::structure_open_close_type::close);
                return "close";
            }
        }

        static std::string to_string(const std::unordered_map<std::string_view, std::string_view>& attributes)
        {
            std::string stringized{};
            for (const auto& entry: attributes)
            {
                if (!std::empty(stringized))
                {
                    stringized += " ";
                }
                stringized += std::string{ entry.first } + ":" + std::string{ entry.second };
            }
            return stringized;
        }

        explicit structure_handler_type(std::vector<std::string>& parsed) : parsed{ parsed } {}

        bool operator()(
            const tetengo::json::json_grammar::structure_type_type        type,
            const tetengo::json::json_grammar::structure_open_close_type  open_close,
            const std::unordered_map<std::string_view, std::string_view>& attributes)
        {
            const std::string stringized_attributes = to_string(attributes);
            parsed.push_back(
                to_string(type) + " " + to_string(open_close) +
                (std::empty(stringized_attributes) ? "" : " " + stringized_attributes));
            return true;
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_grammar)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    std::vector<std::string>          parsed{};
    const tetengo::json::json_grammar grammar{ primitive_handler_type{ parsed }, structure_handler_type{ parsed } };
}

BOOST_AUTO_TEST_CASE(parse)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<std::string>          parsed{};
        const tetengo::json::json_grammar grammar{ primitive_handler_type{ parsed }, structure_handler_type{ parsed } };

        auto                         p_stream = std::make_unique<std::istringstream>(json0);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(!successful);
        BOOST_TEST(parsed == expected_parsed0);
    }
    {
        std::vector<std::string>          parsed{};
        const tetengo::json::json_grammar grammar{ primitive_handler_type{ parsed }, structure_handler_type{ parsed } };

        auto                         p_stream = std::make_unique<std::istringstream>(json1);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(successful);
        BOOST_TEST(parsed == expected_parsed1);
    }
    {
        std::vector<std::string>          parsed{};
        const tetengo::json::json_grammar grammar{ primitive_handler_type{ parsed }, structure_handler_type{ parsed } };

        auto                         p_stream = std::make_unique<std::istringstream>(json2);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(!successful);
        BOOST_TEST(parsed == expected_parsed2);
    }
    {
        std::vector<std::string>          parsed{};
        const tetengo::json::json_grammar grammar{ primitive_handler_type{ parsed }, structure_handler_type{ parsed } };

        auto                         p_stream = std::make_unique<std::istringstream>(json3);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
        const auto                   successful = grammar.parse(reader);
        BOOST_TEST(successful);
        BOOST_TEST(parsed == expected_parsed3);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
