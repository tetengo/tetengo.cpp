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
#include <system_error>
#include <type_traits>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>
#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace
{
    tetengo::property::file_storage::value_map_type make_value_map1()
    {
        tetengo::property::file_storage::value_map_type value_map{};
        value_map.insert(std::make_pair("hoge", true));
        value_map.insert(std::make_pair("fuga", static_cast<std::uint32_t>(42)));
        value_map.insert(std::make_pair((std::filesystem::path{ "piyo" } / "HOGE").string(), std::string{ "foo" }));
        return value_map;
    }

    [[maybe_unused]] const std::string_view json1{
        // clang-format off
        "{\n"
        "  \"hoge\": true,\n"
        "  \"fuga\": 42,\n"
        "  \"piyo\": {\n"
        "    \"HOGE\": \"foo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json1_with_comment{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"hoge\": true,\n"
        "  \"fuga\": 42,\n"
        "  \"piyo\": { # comment2\n"
        "    \"HOGE\": \"foo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json11_empty{};

    const std::string_view json12_syntax_error{
        // clang-format off
        "{\n"
        "  \"hoge\": true,\n"
        "  \"fuga\": 42,\n"
        "  \"piyo\": {\n"
        "    \"HOGE\": <>\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json13_unsupported_syntax{
        // clang-format off
        "{\n"
        "  \"hoge\": true,\n"
        "  \"fuga\": 42,\n"
        "  \"piyo\": {\n"
        "    \"HOGE\": [24, \"foo\"],\n"
        "    \"FUGA\": 4242\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    std::filesystem::path generic_path1()
    {
        std::string name = "test_tetengo.property.file_storage";
        return std::filesystem::path{ name };
    }

    class test_file : private boost::noncopyable
    {
    public:
        test_file(const std::filesystem::path& path, const std::string_view& content) : m_path{ path }
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
            ensure_directory_created(native_path);
            write_content(native_path, content);
        }

        ~test_file()
        {
            const auto native_top_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_top_path(m_path);
            std::error_code ec{};
            std::filesystem::remove_all(native_top_path, ec);
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
        const tetengo::property::file_storage           storage{ std::move(value_map), generic_path1() };
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_file                                 file{ generic_path1(), "" };
        tetengo::property::file_storage::value_map_type value_map = make_value_map1();
        const tetengo::property::file_storage           storage{ std::move(value_map), file.path() };
        storage.save();
    }
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
        const test_file                              file{ generic_path1(), json1_with_comment };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("hoge"));
        BOOST_TEST(*p_storage->get_bool("hoge"));
        BOOST_REQUIRE(p_storage->get_uint32("fuga"));
        BOOST_TEST(*p_storage->get_uint32("fuga") == 42U);
        BOOST_REQUIRE(p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE"));
        BOOST_TEST(*p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE") == "foo");
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "piyo" }));
    }
    {
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load("NONEXISTENT_FILE");

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("hoge"));
        BOOST_CHECK(!p_storage->get_uint32("fuga"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE"));
    }
    {
        const test_file                              file{ generic_path1(), json11_empty };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("hoge"));
        BOOST_CHECK(!p_storage->get_uint32("fuga"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE"));
    }
    {
        const test_file                              file{ generic_path1(), json12_syntax_error };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("hoge"));
        BOOST_CHECK(!p_storage->get_uint32("fuga"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE"));
    }
    {
        const test_file                              file{ generic_path1(), json13_unsupported_syntax };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("hoge"));
        BOOST_TEST(*p_storage->get_bool("hoge"));
        BOOST_REQUIRE(p_storage->get_uint32("fuga"));
        BOOST_TEST(*p_storage->get_uint32("fuga") == 42U);
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "piyo" } / "HOGE"));
        BOOST_REQUIRE(p_storage->get_uint32(std::filesystem::path{ "piyo" } / "FUGA"));
        BOOST_TEST(*p_storage->get_uint32(std::filesystem::path{ "piyo" } / "FUGA") == 4242U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
