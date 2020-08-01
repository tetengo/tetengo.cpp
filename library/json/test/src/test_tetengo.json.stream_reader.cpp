/*! \file
    \brief A stream reader.

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

            stream.write(content.data(), content.length());
        }

        const std::filesystem::path m_path;
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(stream_reader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
    }
    {
        auto p_stream = std::make_unique<std::istringstream>(stream_value);
        BOOST_CHECK_THROW(const tetengo::json::stream_reader reader(std::move(p_stream), 0), std::invalid_argument);
    }

    {
        const temporary_file file{ stream_value };

        const auto* const p_reader = tetengo_json_reader_createStreamReader(file.path().u8string().c_str(), 10);
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_reader);
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
}

BOOST_AUTO_TEST_CASE(get)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST(reader.get() == 'S');
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                         p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'S');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'h');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'i');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'o');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'u');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == ' ');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'M');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST(!reader.has_next());
        BOOST_CHECK_THROW(reader.get(), std::logic_error);
        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
