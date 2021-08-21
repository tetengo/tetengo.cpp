/*! \file
    \brief A line counting reader.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/file_location.hpp>
#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.h>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value0{};

    const std::string stream_value1{ "hoge" };

    const std::string stream_value2{ "fuga\npi\n\nyo\n" };

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
            path /= "test_tetengo.json.line_counting_reader";
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

    std::unique_ptr<tetengo::json::reader> create_cpp_base_reader(const std::string& stream_value)
    {
        auto p_stream = std::make_unique<std::stringstream>(stream_value);
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::move(p_stream));
        return p_reader;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(line_counting_reader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };
    }
    {
        std::unique_ptr<tetengo::json::reader> p_base_reader{};
        BOOST_CHECK_THROW(
            const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) }, std::invalid_argument);
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_reader);
    }
    {
        const auto* const p_reader = tetengo_json_reader_createLineCountingReader(nullptr);
        BOOST_TEST(!p_reader);
    }
}

BOOST_AUTO_TEST_CASE(get_location)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        const auto& location = reader.get_location();
        BOOST_TEST(std::empty(location.line()));
        BOOST_TEST(location.line_index() == 0U);
        BOOST_TEST(location.column_index() == 0U);
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value1);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        const auto& location = reader.get_location();
        BOOST_TEST(location.line() == "hoge");
        BOOST_TEST(location.line_index() == 0U);
        BOOST_TEST(location.column_index() == 0U);
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value2);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        const auto& location = reader.get_location();
        BOOST_TEST(location.line() == "fuga\n");
        BOOST_TEST(location.line_index() == 0U);
        BOOST_TEST(location.column_index() == 0U);
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        tetengo_json_location_t location{};
        const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST((std::string_view{ location.line, location.line_length } == ""));
        BOOST_TEST(location.line_index == 0U);
        BOOST_TEST(location.column_index == 0U);
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        tetengo_json_location_t location{};
        const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
        BOOST_TEST(location.line_index == 0U);
        BOOST_TEST(location.column_index == 0U);
    }
    {
        const temporary_file file{ stream_value2 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        tetengo_json_location_t location{};
        const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
        BOOST_TEST(location.line_index == 0U);
        BOOST_TEST(location.column_index == 0U);
    }
    {
        tetengo_json_location_t location{};
        const auto              result = tetengo_json_reader_getLocation(nullptr, &location);
        BOOST_TEST(!result);
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        const auto result = tetengo_json_reader_getLocation(p_reader, nullptr);
        BOOST_TEST(!result);
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        tetengo_json_location_t location{};
        const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_TEST(!reader.has_next());
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value1);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_TEST(reader.has_next());
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value2);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_TEST(reader.has_next());
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(!tetengo_json_reader_hasNext(p_reader));
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_hasNext(p_reader));
    }
    {
        const temporary_file file{ stream_value2 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_hasNext(p_reader));
    }
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_CHECK_THROW([[maybe_unused]] const auto peeked = reader.peek(), std::logic_error);
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value1);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_TEST(reader.peek() == 'h');
    }
    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value2);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_TEST(reader.peek() == 'f');
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 0);
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'h');
    }
    {
        const temporary_file file{ stream_value2 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'f');
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                p_base_reader = create_cpp_base_reader(stream_value0);
        tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }
    {
        auto                                p_base_reader = create_cpp_base_reader(stream_value1);
        tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'h');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "hoge", 0, 0 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'o');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "hoge", 0, 1 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'g');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "hoge", 0, 2 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'e');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "hoge", 0, 3 }));
            reader.next();
        }
        {
            BOOST_TEST(!reader.has_next());
            BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = reader.peek(), std::logic_error);
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "hoge", 0, 4 }));
            BOOST_CHECK_THROW(reader.next(), std::logic_error);
        }
    }
    {
        auto                                p_base_reader = create_cpp_base_reader(stream_value2);
        tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'f');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "fuga\n", 0, 0 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'u');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "fuga\n", 0, 1 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'g');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "fuga\n", 0, 2 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'a');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "fuga\n", 0, 3 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == '\n');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "fuga\n", 0, 4 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'p');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "pi\n", 1, 0 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'i');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "pi\n", 1, 1 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == '\n');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "pi\n", 1, 2 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == '\n');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "\n", 2, 0 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'y');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "yo\n", 3, 0 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == 'o');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "yo\n", 3, 1 }));
            reader.next();
        }
        {
            BOOST_TEST_REQUIRE(reader.has_next());
            BOOST_TEST(reader.peek() == '\n');
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "yo\n", 3, 2 }));
            reader.next();
        }
        {
            BOOST_TEST(!reader.has_next());
            BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = reader.peek(), std::logic_error);
            BOOST_CHECK((reader.get_location() == tetengo::json::file_location{ "yo\n", 3, 3 }));
            BOOST_CHECK_THROW(reader.next(), std::logic_error);
        }
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        auto* const          p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        tetengo_json_reader_next(p_reader);
    }
    {
        const temporary_file file{ stream_value1 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        auto* const          p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'h');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 0U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'o');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 1U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'g');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 2U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'e');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 3U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST(!tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 0);
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "hoge"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 4U);
            tetengo_json_reader_next(p_reader);
        }
    }
    {
        const temporary_file file{ stream_value2 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        auto* const          p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'f');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 0U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'u');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 1U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'g');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 2U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'a');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 3U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == '\n');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "fuga\n"));
            BOOST_TEST(location.line_index == 0U);
            BOOST_TEST(location.column_index == 4U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'p');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "pi\n"));
            BOOST_TEST(location.line_index == 1U);
            BOOST_TEST(location.column_index == 0U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'i');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "pi\n"));
            BOOST_TEST(location.line_index == 1U);
            BOOST_TEST(location.column_index == 1U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == '\n');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "pi\n"));
            BOOST_TEST(location.line_index == 1U);
            BOOST_TEST(location.column_index == 2U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == '\n');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "\n"));
            BOOST_TEST(location.line_index == 2U);
            BOOST_TEST(location.column_index == 0U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'y');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "yo\n"));
            BOOST_TEST(location.line_index == 3U);
            BOOST_TEST(location.column_index == 0U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'o');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "yo\n"));
            BOOST_TEST(location.line_index == 3U);
            BOOST_TEST(location.column_index == 1U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == '\n');
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "yo\n"));
            BOOST_TEST(location.line_index == 3U);
            BOOST_TEST(location.column_index == 2U);
            tetengo_json_reader_next(p_reader);
        }
        {
            BOOST_TEST(!tetengo_json_reader_hasNext(p_reader));
            BOOST_TEST(tetengo_json_reader_peek(p_reader) == 0);
            tetengo_json_location_t location{};
            const auto              result = tetengo_json_reader_getLocation(p_reader, &location);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST((std::string_view{ location.line, location.line_length } == "yo\n"));
            BOOST_TEST(location.line_index == 3U);
            BOOST_TEST(location.column_index == 3U);
            tetengo_json_reader_next(p_reader);
        }
    }
}


BOOST_AUTO_TEST_CASE(base_reader)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                      p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::line_counting_reader reader{ std::move(p_base_reader) };

        [[maybe_unused]] const auto& base_reader = reader.base_reader();
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createLineCountingReader(p_base_reader);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_baseReader(p_reader));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
