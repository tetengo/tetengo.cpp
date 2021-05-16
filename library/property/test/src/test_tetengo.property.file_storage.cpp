/*! \file
    \brief A file storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>
#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace
{
    std::filesystem::path generic_path1()
    {
        std::string name = "test_tetengo.property.file_storage";
        return std::filesystem::path{ name };
    }

    const std::string_view json1{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"hoge\": 42,\n"
        "  \"fuga\": { # comment2\n"
        "    \"piyo\": \"foo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json11_empty{};

    const std::string_view json12_syntax_error{
        // clang-format off
        "{\n"
        "  \"hoge\": 42,\n"
        "  \"fuga\": {\n"
        "    \"piyo\": <>\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    class input_file : private boost::noncopyable
    {
    public:
        input_file(const std::filesystem::path& path, const std::string_view& content) : m_path{ path }
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
            ensure_directory_created(native_path);
            write_content(native_path, content);
        }

        ~input_file()
        {
            const auto native_top_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_top_path(m_path);
            std::filesystem::remove_all(native_top_path);
        }

        const std::filesystem::path& path() const
        {
            return m_path;
        }

    private:
        static void ensure_directory_created(const std::filesystem::path& native_path)
        {
            if (!native_path.has_parent_path() || std::filesystem::exists(native_path.parent_path()))
            {
                return;
            }
            std::filesystem::create_directories(native_path.parent_path());
        }

        static void write_content(const std::filesystem::path& native_path, const std::string_view& content)
        {
            std::ofstream stream{ native_path };
            stream.write(content.data(), content.size());
        }

        const std::filesystem::path m_path;
    };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(file_storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::property::file_storage::value_map_type value_map{};
        const tetengo::property::file_storage           storage{ std::move(value_map) };
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    //{
    //    tetengo::property::file_storage::value_map_type master_value_map{};
    //    tetengo::property::file_storage                 storage1{ master_value_map };

    //    const auto key = std::filesystem::path{ "hoge" } / "fuga";
    //    storage1.set_uint32(key, 42);

    //    const tetengo::property::file_storage storage2{ master_value_map };
    //    BOOST_TEST(!storage2.get_uint32(key));

    //    storage1.save();

    //    const tetengo::property::file_storage storage3{ master_value_map };
    //    BOOST_REQUIRE(storage3.get_uint32(key));
    //    BOOST_TEST(*storage3.get_uint32(key) == 42U);
    //}
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(file_storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::property::file_storage_loader loader{};
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        const input_file                             file{ generic_path1(), json1 };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_uint32("hoge"));
        BOOST_TEST(*p_storage->get_uint32("hoge") == 42U);
        BOOST_REQUIRE(p_storage->get_string(std::filesystem::path{ "fuga" } / "piyo"));
        BOOST_TEST(*p_storage->get_string(std::filesystem::path{ "fuga" } / "piyo") == "foo");
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "fuga" }));
    }
    {
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load("NONEXISTENT_FILE");

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_uint32("hoge"));
        BOOST_CHECK(!p_storage->get_uint32(std::filesystem::path{ "fuga" } / "piyo"));
    }
    {
        const input_file                             file{ generic_path1(), json11_empty };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_uint32("hoge"));
        BOOST_CHECK(!p_storage->get_uint32(std::filesystem::path{ "fuga" } / "piyo"));
    }
    {
        const input_file                             file{ generic_path1(), json12_syntax_error };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_uint32("hoge"));
        BOOST_CHECK(!p_storage->get_uint32(std::filesystem::path{ "fuga" } / "piyo"));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
