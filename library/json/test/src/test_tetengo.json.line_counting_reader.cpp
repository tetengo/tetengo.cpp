/*! \file
    \brief A line counting reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value0{};

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
