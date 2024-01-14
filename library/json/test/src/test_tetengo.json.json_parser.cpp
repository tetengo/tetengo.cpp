/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <sstream> // IWYU pragma: keep
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
#include <tetengo/json/file_location.hpp>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.h>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>
#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf8.hpp>


namespace
{
    const std::string json0{};

    const std::string json1{ "false" };

    const std::string json2{ "false true" };

    // clang-format off
    const std::string json3{
        "[\n"
        "  42,\n"
        "  true,\n"
        "  3.1415926,\n"
        "  \"hoge\",\n"
        "  {\n"
        "    \"Aso\": 1592,\n"
        "    \"Tsurugi\": 1955\n"
        "  },\n"
        "  [\n"
        "    null,\n"
        "    3.0e+5\n"
        "  ]\n"
        "]\n"
    };
    // clang-format on

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
            path /= "test_tetengo.json.json_parser";
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

            stream.write(std::data(content), content.length());
        }

        const std::filesystem::path m_path;
    };

    std::string to_internal_encoding_path_string(const std::filesystem::path& path)
    {
        static const tetengo::text::encoder<tetengo::text::encoding::utf8> encoder{};
        return std::string{ encoder.decode(path.u8string()) };
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(json_parser)


BOOST_AUTO_TEST_CASE(default_buffer_capacity)
{
    BOOST_TEST_PASSPOINT();

    {
        BOOST_TEST(tetengo::json::json_parser::default_buffer_capacity() > 0U);
    }
    {
        BOOST_TEST(
            tetengo_json_jsonParser_defaultBufferCapacity() == tetengo::json::json_parser::default_buffer_capacity());
    }
}

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
        const tetengo::json::json_parser parser{ std::move(p_reader), 42 };
    }
    {
        BOOST_CHECK_THROW(const tetengo::json::json_parser parser{ nullptr }, std::invalid_argument);
    }

    {
        const temporary_file file{ json0 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser = tetengo_json_jsonParser_create(p_reader, 42);
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_parser);
    }
    {
        const auto* const p_parser =
            tetengo_json_jsonParser_create(nullptr, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_TEST(!p_parser);
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json0), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        BOOST_TEST(parser.has_next());
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
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        BOOST_TEST(tetengo_json_jsonParser_hasNext(p_parser));
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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

        BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = parser.peek(), std::runtime_error);
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json1), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
        BOOST_TEST(parsed.value() == "false");
        BOOST_TEST(std::empty(parsed.attributes()));
        const auto& file_location = parsed.get_file_location();
        BOOST_TEST((file_location == tetengo::json::file_location{ "false", 0, 5 }));
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json2), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
        BOOST_TEST(parsed.value() == "false");
        BOOST_TEST(std::empty(parsed.attributes()));
        const auto& file_location = parsed.get_file_location();
        BOOST_TEST((file_location == tetengo::json::file_location{ "false true", 0, 10 }));
    }
    {
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::make_unique<std::istringstream>(json3), 10);
        const tetengo::json::json_parser parser{ std::move(p_reader) };

        const auto& parsed = parser.peek();
        BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
        BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
        BOOST_TEST(std::empty(parsed.value()));
        BOOST_TEST(std::empty(parsed.attributes()));
        const auto& file_location = parsed.get_file_location();
        BOOST_TEST((file_location == tetengo::json::file_location{ "  true,\n", 2, 0 }));
    }

    {
        const temporary_file file{ json0 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
        BOOST_TEST(std::string_view{ p_file_location->line } == "false");
        BOOST_TEST(p_file_location->line_index == 0U);
        BOOST_TEST(p_file_location->column_index == 5U);
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
        BOOST_TEST(std::string_view{ p_file_location->line } == "false true");
        BOOST_TEST(p_file_location->line_index == 0U);
        BOOST_TEST(p_file_location->column_index == 10U);
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        const auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
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
        BOOST_TEST(std::empty(std::string_view{ value }));
        BOOST_TEST(tetengo_json_element_attributeKeys(p_parsed, nullptr) == 0U);
        const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
        BOOST_TEST(std::string_view{ p_file_location->line } == "  true,\n");
        BOOST_TEST(p_file_location->line_index == 2U);
        BOOST_TEST(p_file_location->column_index == 0U);
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

        parser.next();
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

        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "false");
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            BOOST_CHECK_THROW([[maybe_unused]] const auto& parsed = parser.peek(), std::runtime_error);
            parser.next();
        }
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
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  true,\n", 2, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "42");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  true,\n", 2, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::boolean);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "true");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  3.1415926,\n", 3, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "3.1415926");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  \"hoge\",\n", 4, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::string);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "hoge");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  {\n", 5, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::object);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    \"Tsurugi\": 1955\n", 7, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            BOOST_TEST(std::size(parsed.attributes()) == 1U);
            BOOST_TEST(std::begin(parsed.attributes())->first == "name");
            BOOST_TEST(std::begin(parsed.attributes())->second == "Aso");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    \"Tsurugi\": 1955\n", 7, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "1592");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    \"Tsurugi\": 1955\n", 7, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    \"Tsurugi\": 1955\n", 7, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            BOOST_TEST(std::size(parsed.attributes()) == 1U);
            BOOST_TEST(std::begin(parsed.attributes())->first == "name");
            BOOST_TEST(std::begin(parsed.attributes())->second == "Tsurugi");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  },\n", 8, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "1955");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  },\n", 8, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::member);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  },\n", 8, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::object);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  [\n", 9, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_open);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    3.0e+5\n", 11, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::null);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "    3.0e+5\n", 11, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::number);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::primitive);
            BOOST_TEST(parsed.value() == "3.0e+5");
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "  ]\n", 12, 0 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "]\n", 13, 2 }));
            parser.next();
        }
        {
            BOOST_TEST_REQUIRE(parser.has_next());
            const auto& parsed = parser.peek();
            BOOST_CHECK(parsed.type().name == tetengo::json::element::type_name_type::array);
            BOOST_CHECK(parsed.type().category == tetengo::json::element::type_category_type::structure_close);
            const auto& file_location = parsed.get_file_location();
            BOOST_TEST((file_location == tetengo::json::file_location{ "]\n", 13, 2 }));
            parser.next();
        }

        BOOST_TEST(!parser.has_next());
        BOOST_CHECK_THROW(parser.next(), std::logic_error);
    }

    {
        const temporary_file file{ json0 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        tetengo_json_jsonParser_next(p_parser);
        tetengo_json_jsonParser_next(p_parser);
    }
    {
        const temporary_file file{ json1 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        tetengo_json_jsonParser_next(p_parser);
        tetengo_json_jsonParser_next(p_parser);
    }
    {
        const temporary_file file{ json2 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_boolean());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "false");
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST(!p_parsed);
            tetengo_json_jsonParser_next(p_parser);
        }
        tetengo_json_jsonParser_next(p_parser);
    }
    {
        const temporary_file file{ json3 };
        auto* const          p_reader =
            tetengo_json_reader_createStreamReader(to_internal_encoding_path_string(file.path()).c_str(), 10);
        auto* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        BOOST_SCOPE_EXIT(p_parser)
        {
            tetengo_json_jsonParser_destroy(p_parser);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_parser);

        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_array());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  true,\n");
            BOOST_TEST(p_file_location->line_index == 2U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_number());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "42");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  true,\n");
            BOOST_TEST(p_file_location->line_index == 2U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_boolean());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "true");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  3.1415926,\n");
            BOOST_TEST(p_file_location->line_index == 3U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_number());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "3.1415926");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  \"hoge\",\n");
            BOOST_TEST(p_file_location->line_index == 4U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_string());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "hoge");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  {\n");
            BOOST_TEST(p_file_location->line_index == 5U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_object());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    \"Tsurugi\": 1955\n");
            BOOST_TEST(p_file_location->line_index == 7U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_member());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
            const auto parsed_attribute_count = tetengo_json_element_attributeKeys(p_parsed, nullptr);
            BOOST_TEST(parsed_attribute_count == 1U);
            const char* attribute_key = nullptr;
            const auto  parsed_attribute_count_again = tetengo_json_element_attributeKeys(p_parsed, &attribute_key);
            BOOST_TEST(parsed_attribute_count_again == parsed_attribute_count);
            BOOST_TEST(std::string_view{ attribute_key } == "name");
            BOOST_TEST(std::string_view{ tetengo_json_element_attributeValueOf(p_parsed, attribute_key) } == "Aso");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    \"Tsurugi\": 1955\n");
            BOOST_TEST(p_file_location->line_index == 7U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_number());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "1592");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    \"Tsurugi\": 1955\n");
            BOOST_TEST(p_file_location->line_index == 7U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_member());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureClose());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    \"Tsurugi\": 1955\n");
            BOOST_TEST(p_file_location->line_index == 7U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_member());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
            const auto parsed_attribute_count = tetengo_json_element_attributeKeys(p_parsed, nullptr);
            BOOST_TEST(parsed_attribute_count == 1U);
            const char* attribute_key = nullptr;
            const auto  parsed_attribute_count_again = tetengo_json_element_attributeKeys(p_parsed, &attribute_key);
            BOOST_TEST(parsed_attribute_count_again == parsed_attribute_count);
            BOOST_TEST(std::string_view{ attribute_key } == "name");
            BOOST_TEST(std::string_view{ tetengo_json_element_attributeValueOf(p_parsed, attribute_key) } == "Tsurugi");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  },\n");
            BOOST_TEST(p_file_location->line_index == 8U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_number());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "1955");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  },\n");
            BOOST_TEST(p_file_location->line_index == 8U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_member());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureClose());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  },\n");
            BOOST_TEST(p_file_location->line_index == 8U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_object());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureClose());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  [\n");
            BOOST_TEST(p_file_location->line_index == 9U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_array());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureOpen());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    3.0e+5\n");
            BOOST_TEST(p_file_location->line_index == 11U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_null());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "    3.0e+5\n");
            BOOST_TEST(p_file_location->line_index == 11U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_number());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_primitive());
            const auto* const value = tetengo_json_element_value(p_parsed);
            BOOST_TEST_REQUIRE(value);
            BOOST_TEST(std::string_view{ value } == "3.0e+5");
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "  ]\n");
            BOOST_TEST(p_file_location->line_index == 12U);
            BOOST_TEST(p_file_location->column_index == 0U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_array());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureClose());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "]\n");
            BOOST_TEST(p_file_location->line_index == 13U);
            BOOST_TEST(p_file_location->column_index == 2U);
            tetengo_json_jsonParser_next(p_parser);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_jsonParser_hasNext(p_parser));
            const auto* p_parsed = tetengo_json_jsonParser_peek(p_parser);
            BOOST_TEST_REQUIRE(p_parsed);
            const auto* const p_parsed_type = tetengo_json_element_type(p_parsed);
            BOOST_TEST_REQUIRE(p_parsed_type);
            BOOST_TEST(p_parsed_type->name == tetengo_json_element_typeName_array());
            BOOST_TEST(p_parsed_type->category == tetengo_json_element_typeCategory_structureClose());
            const auto* const p_file_location = tetengo_json_element_getFileLocation(p_parsed);
            BOOST_TEST(std::string_view{ p_file_location->line } == "]\n");
            BOOST_TEST(p_file_location->line_index == 13U);
            BOOST_TEST(p_file_location->column_index == 2U);
            tetengo_json_jsonParser_next(p_parser);
        }

        BOOST_TEST(!tetengo_json_jsonParser_hasNext(p_parser));
        tetengo_json_jsonParser_next(p_parser);
    }
    {
        tetengo_json_jsonParser_next(nullptr);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
