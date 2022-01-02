/*! \file
    \brief A stream reader.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <sstream> // IWYU pragma: keep
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/reader.h>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value{ "Shikoku Mannaka" };

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

            stream.write(std::data(content), content.length());
        }

        const std::filesystem::path m_path;
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(stream_reader)


BOOST_AUTO_TEST_CASE(default_buffer_capacity)
{
    BOOST_TEST_PASSPOINT();

    {
        BOOST_TEST(tetengo::json::stream_reader::default_buffer_capacity() == 4096U);
    }

    {
        BOOST_TEST(
            tetengo_json_reader_streamReaderDefaultBufferCapacity() ==
            tetengo::json::stream_reader::default_buffer_capacity());
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
    }
    {
        std::unique_ptr<std::istream> p_stream{};
        BOOST_CHECK_THROW(const tetengo::json::stream_reader reader(std::move(p_stream), 10), std::invalid_argument);
    }
    {
        auto p_stream = std::make_unique<std::istringstream>(stream_value);
        BOOST_CHECK_THROW(const tetengo::json::stream_reader reader(std::move(p_stream), 0), std::invalid_argument);
    }

    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_reader);
    }
    {
        const auto* const p_reader = tetengo_json_reader_createStreamReader("NONEXISTENT_FILE", 10);
        BOOST_TEST(!p_reader);
    }
    {
        const auto* const p_reader = tetengo_json_reader_createStreamReader(nullptr, 10);
        BOOST_TEST(!p_reader);
    }
    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 0);
        BOOST_TEST(!p_reader);
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST(reader.has_next());
    }

    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_hasNext(p_reader));
    }
    {
        BOOST_TEST(!tetengo_json_reader_hasNext(nullptr));
    }
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST(reader.peek() == 'S');
    }

    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'S');
    }
    {
        BOOST_TEST(tetengo_json_reader_peek(nullptr) == 0);
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                         p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'S');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'h');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'i');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'o');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'u');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == ' ');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'M');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.peek() == 'a');
        reader.next();

        BOOST_TEST(!reader.has_next());
        BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = reader.peek(), std::logic_error);
        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }

    {
        const temporary_file file{ stream_value };

        auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'S');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'h');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'i');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'k');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'o');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'k');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'u');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == ' ');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'M');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'a');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'n');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'n');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'a');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'k');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST_REQUIRE(tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 'a');
        tetengo_json_reader_next(p_reader);

        BOOST_TEST(!tetengo_json_reader_hasNext(p_reader));
        BOOST_TEST(tetengo_json_reader_peek(p_reader) == 0);
        tetengo_json_reader_next(p_reader);
    }
    {
        tetengo_json_reader_next(nullptr);
    }
}


BOOST_AUTO_TEST_CASE(base_reader)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_CHECK_THROW([[maybe_unused]] const auto& base_reader = reader.base_reader(), std::logic_error);
    }

    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(
            /* convert needed */ reinterpret_cast<const char*>(file.path().u8string().c_str()), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_reader);

        BOOST_TEST(!tetengo_json_reader_baseReader(p_reader));
    }
    {
        BOOST_TEST(!tetengo_json_reader_baseReader(nullptr));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
