/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.h>
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

    class temporary_file : private boost::noncopyable
    {
    public:
        explicit temporary_file(const std::string& content) : m_path{ make_temporary_path() }
        {
            write_content_to_file(m_path, content);
        }

        ~temporary_file()
        {
            std::filesystem::remove(m_path);
        }

        const std::filesystem::path path() const
        {
            return m_path;
        }

    private:
        static std::filesystem::path make_temporary_path()
        {
            auto path = std::filesystem::temp_directory_path();
            path /= "test_tetengo.json.stream_reader";
            return path;
        }

        static void write_content_to_file(const std::filesystem::path& path, const std::string& content)
        {
            if (std::filesystem::exists(path))
            {
                throw std::runtime_error{ "The temporary file already exists. Remove it if possible." };
            }
            std::ofstream stream{ path };
            if (!stream)
            {
                throw std::runtime_error{ "Can't create a temporary file." };
            }

            stream.write(content.data(), content.length());
        }

        const std::filesystem::path m_path;
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

    {
        const temporary_file file{ json0 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const auto* const p_parser = tetengo_json_jsonParser_create(nullptr);
        BOOST_TEST(!p_parser);
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

    {
        const temporary_file file{ json0 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(!tetengo_json_jsonParser_hasNext(p_parser));
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(tetengo_json_jsonParser_hasNext(p_parser));
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(tetengo_json_jsonParser_hasNext(p_parser));
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(tetengo_json_jsonParser_hasNext(p_parser));
    }
    {
        BOOST_TEST(!tetengo_json_jsonParser_hasNext(nullptr));
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

    {
        const temporary_file file{ json0 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(!tetengo_json_jsonParser_peek(p_parser));
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        const auto* const p_parsed = tetengo_json_jsonParser_peek(p_parser);
        BOOST_TEST_REQUIRE(p_parsed);
        const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
        BOOST_TEST_REQUIRE(p_parsed_type);
        BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_boolean());
        BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
        const auto* const value = tetengo_json_element_value(p_parsed);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view{ value } == "false");
        BOOST_TEST(tetengo_json_element_attributeKeys(p_parsed, nullptr) == 0U);
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
        BOOST_TEST_REQUIRE(p_parsed);
        const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
        BOOST_TEST_REQUIRE(p_parsed_type);
        BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_boolean());
        BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
        const auto* const value = tetengo_json_element_value(p_parsed);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view{ value } == "false");
        BOOST_TEST(tetengo_json_element_attributeKeys(p_parsed, nullptr) == 0U);
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_parser = tetengo_json_jsonParser_create(p_reader);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
        BOOST_TEST_REQUIRE(p_parsed);
        const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
        BOOST_TEST_REQUIRE(p_parsed_type);
        BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_array());
        BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
        const auto* const value = tetengo_json_element_value(p_parsed);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view{ value }.empty());
        BOOST_TEST(tetengo_json_element_attributeKeys(p_parsed, nullptr) == 0U);
    }
    {
        BOOST_TEST(!tetengo_json_jsonParser_peek(nullptr));
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
