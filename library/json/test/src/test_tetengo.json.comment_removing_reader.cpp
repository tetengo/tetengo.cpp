/*! \file
    \brief A comment removing reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/comment_removing_reader.hpp>
#include <tetengo/json/reader.h>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value0{};

    const std::string stream_value1{ "REMREN" };

    const std::string stream_value2{ "RENREMREN" };

    const std::string expected_value2{ "REN" };

    const std::string stream_value3{ "RENRENREM\nRENREMREN\nREMRENREM\n" };

    const std::string expected_value3{ "RENREN\nREN\n\n" };

    const std::string stream_value4{ "REN'REN'REN\nREN'REN'REM\nREN'REM'REN\nREM'REN'REN\n" };

    const std::string expected_value4{ "REN'REN'REN\nREN'REN'\nREN'REM'REN\n\n" };

    const std::string stream_value5{ "REN\"REN\"REN\nREN\"REN\"REM\nREN\"REM\"REN\nREM\"REN\"REN\n" };

    const std::string expected_value5{ "REN\"REN\"REN\nREN\"REN\"\nREN\"REM\"REN\n\n" };

    const std::string stream_value6{ "RENREM\r\nREMREM\r\n" };

    const std::string expected_value6{ "REN\r\n\r\n" };

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
            path /= "test_tetengo.json.comment_removing_reader";
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

    std::unique_ptr<tetengo::json::reader> create_cpp_base_reader(const std::string& stream_value)
    {
        auto p_stream = std::make_unique<std::stringstream>(stream_value);
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::move(p_stream));
        return p_reader;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(comment_removing_reader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
    }
    {
        std::unique_ptr<tetengo::json::reader> p_base_reader{};
        BOOST_CHECK_THROW(
            const tetengo::json::comment_removing_reader reader(std::move(p_base_reader), "REM"),
            std::invalid_argument);
    }
    {
        auto p_base_reader = create_cpp_base_reader(stream_value0);
        BOOST_CHECK_THROW(
            const tetengo::json::comment_removing_reader reader(std::move(p_base_reader), ""), std::invalid_argument);
    }

    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createCommentRemovingReader(p_base_reader, "REM");
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_reader);
    }
    {
        const auto* const p_reader = tetengo_json_reader_createCommentRemovingReader(nullptr, "REM");
        BOOST_TEST(!p_reader);
    }
    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createCommentRemovingReader(p_base_reader, "");
        BOOST_TEST(!p_reader);
    }
    {
        const temporary_file file{ stream_value0 };
        auto* const          p_base_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        const auto* const    p_reader = tetengo_json_reader_createCommentRemovingReader(p_base_reader, nullptr);
        BOOST_TEST(!p_reader);
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_TEST(!reader.has_next());
    }
    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value1);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_TEST(!reader.has_next());
    }
    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value2);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_TEST(reader.has_next());
    }
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value0);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_CHECK_THROW(reader.peek(), std::logic_error);
    }
    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value1);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_CHECK_THROW(reader.peek(), std::logic_error);
    }
    {
        auto                                         p_base_reader = create_cpp_base_reader(stream_value2);
        const tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_TEST(reader.peek() == 'R');
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value0);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value1);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };

        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value2);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
        std::string                            read{};
        while (reader.has_next())
        {
            read.push_back(reader.peek());
            reader.next();
        }

        BOOST_TEST(read == expected_value2);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value3);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
        std::string                            read{};
        while (reader.has_next())
        {
            read.push_back(reader.peek());
            reader.next();
        }

        BOOST_TEST(read == expected_value3);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value4);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
        std::string                            read{};
        while (reader.has_next())
        {
            read.push_back(reader.peek());
            reader.next();
        }

        BOOST_TEST(read == expected_value4);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value5);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
        std::string                            read{};
        while (reader.has_next())
        {
            read.push_back(reader.peek());
            reader.next();
        }

        BOOST_TEST(read == expected_value5);
    }
    {
        auto                                   p_base_reader = create_cpp_base_reader(stream_value6);
        tetengo::json::comment_removing_reader reader{ std::move(p_base_reader), "REM" };
        std::string                            read{};
        while (reader.has_next())
        {
            read.push_back(reader.peek());
            reader.next();
        }

        BOOST_TEST(read == expected_value6);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
