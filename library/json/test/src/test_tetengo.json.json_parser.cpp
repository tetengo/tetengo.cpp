/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/element.hpp>
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

    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json0), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_CHECK_THROW(parser.peek(), std::logic_error);
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json1), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
        BOOST_TEST(parsed.value() == "false");
        BOOST_TEST(parsed.attributes().empty());
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json2), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
        BOOST_TEST(parsed.value() == "false");
        BOOST_TEST(parsed.attributes().empty());
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json3), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
        BOOST_TEST(parsed.value().empty());
        BOOST_TEST(parsed.attributes().empty());
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json0), 10);
        tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_CHECK_THROW(parser.next(), std::logic_error);
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json1), 10);
        tetengo::json::json_parser parser{ std::move(p_reader) };

        parser.next();
        BOOST_CHECK_THROW(parser.next(), std::logic_error);
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json2), 10);
        tetengo::json::json_parser parser{ std::move(p_reader) };

        parser.next();
        BOOST_CHECK_THROW(parser.next(), std::logic_error);
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json3), 10);
        tetengo::json::json_parser parser{ std::move(p_reader) };

        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "42");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "true");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "3.1415926");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::string);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "hoge");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::object);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            BOOST_TEST(parsed.attributes().size() == 1U);
            BOOST_TEST(std::begin(parsed.attributes())->first == "name");
            BOOST_TEST(std::begin(parsed.attributes())->second == "Aso");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "1592");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            BOOST_TEST(parsed.attributes().size() == 1U);
            BOOST_TEST(std::begin(parsed.attributes())->first == "name");
            BOOST_TEST(std::begin(parsed.attributes())->second == "Tsurugi");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "1955");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::object);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::null);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "3.0e+5");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            parser.next();
        }

        BOOST_TEST(!parser.has_next());
        BOOST_CHECK_THROW(parser.next(), std::logic_error);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
