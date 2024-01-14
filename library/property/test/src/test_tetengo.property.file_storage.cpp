/*! \file
    \brief A file storage.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <cstddef> // IWYU pragma: keep
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>
#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>


namespace
{
    const std::string_view json1{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": \"echo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json2{
        // clang-format off
        "{\n"
        "  \"\\\"\\b\\f\\n\\r\\t\": \"\xE3\x81\x82\\\\\\/\xF0\x9F\x98\x80\"\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json3_with_comment{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": { # comment2\n"
        "    \"delta\": \"echo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json4_escaped{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"\\\"\\\\\\/\\b\\f\\n\\r\\t\": \"\\u3042\\uD83d\\uDe00\"\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json11_empty{};

    const std::string_view json12_syntax_error{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": <>\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json13_unsupported_syntax{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": [24, \"echo\"],\n"
        "    \"foxtrot\": 4242\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    std::filesystem::path generic_path()
    {
        std::string name = "test_tetengo.property.file_storage";
        return std::filesystem::path{ name };
    }

    class test_file : private boost::noncopyable
    {
    public:
        explicit test_file(const std::filesystem::path& path) : m_path{ path }
        {
            {
                const auto native_top_path =
                    tetengo::platform_dependent::property_set_file_path::instance().to_native_top_path(m_path);
                std::error_code ec{};
                std::filesystem::remove_all(native_top_path, ec);
            }
            {
                const auto native_path =
                    tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
                std::error_code ec{};
                std::filesystem::remove_all(native_path, ec);
            }
        }

        test_file(const std::filesystem::path& path, const std::string_view& content) : m_path{ path }
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
            ensure_directory_created(native_path);
            write_content(native_path, content);
        }

        ~test_file()
        {
            {
                const auto native_top_path =
                    tetengo::platform_dependent::property_set_file_path::instance().to_native_top_path(m_path);
                std::error_code ec{};
                std::filesystem::remove_all(native_top_path, ec);
            }
            {
                const auto native_path =
                    tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
                std::error_code ec{};
                std::filesystem::remove_all(native_path, ec);
            }
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
            stream.write(std::data(content), std::size(content));
        }

        const std::filesystem::path m_path;
    };

    bool file_content_equal_to(const std::filesystem::path& path, const std::string_view& expected)
    {
        std::string content{};
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(path);
            std::ifstream stream{ native_path };
            while (stream)
            {
                std::vector<char> read(1024, '\0');
                stream.read(std::data(read), std::size(read));
                content.append(std::data(read), static_cast<std::size_t>(stream.gcount()));
            }
        }

        return content == expected;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(file_storage)


BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_file                              file{ generic_path() };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());
        BOOST_REQUIRE(p_storage);

        p_storage->set_bool("alpha", true);
        p_storage->set_uint32("bravo", static_cast<std::uint32_t>(42));
        p_storage->set_string((std::filesystem::path{ "charlie" } / "delta").string(), std::string{ "echo" });

        p_storage->save();

        BOOST_TEST(file_content_equal_to(generic_path(), json1));
    }
    {
        const test_file                              file{ generic_path(), "" };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());
        BOOST_REQUIRE(p_storage);

        p_storage->set_bool("alpha", true);
        p_storage->set_uint32("bravo", static_cast<std::uint32_t>(42));
        p_storage->set_string((std::filesystem::path{ "charlie" } / "delta").string(), std::string{ "echo" });

        p_storage->save();

        BOOST_TEST(file_content_equal_to(generic_path(), json1));
    }
    {
        const test_file                              file{ generic_path(), "" };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());
        BOOST_REQUIRE(p_storage);

        p_storage->set_string("\"\b\f\n\r\t", std::string{ "\xE3\x81\x82\\/\xF0\x9F\x98\x80" });

        p_storage->save();

        BOOST_TEST(file_content_equal_to(generic_path(), json2));
    }

    {
        const test_file   file{ generic_path() };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setBool(p_storage, "alpha", true);
        tetengo_property_storage_setUint32(p_storage, "bravo", static_cast<std::uint32_t>(42));
        tetengo_property_storage_setString(
            p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), "echo");

        tetengo_property_storage_save(p_storage);

        BOOST_TEST(file_content_equal_to(generic_path(), json1));
    }
    {
        const test_file   file{ generic_path(), "" };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setBool(p_storage, "alpha", true);
        tetengo_property_storage_setUint32(p_storage, "bravo", static_cast<std::uint32_t>(42));
        tetengo_property_storage_setString(
            p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), "echo");

        tetengo_property_storage_save(p_storage);

        BOOST_TEST(file_content_equal_to(generic_path(), json1));
    }
    {
        const test_file   file{ generic_path(), "" };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setString(p_storage, "\"\b\f\n\r\t", "\xE3\x81\x82\\/\xF0\x9F\x98\x80");

        tetengo_property_storage_save(p_storage);

        BOOST_TEST(file_content_equal_to(generic_path(), json2));
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

    {
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_loader);
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_file                              file{ generic_path() };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_CHECK(p_storage);
    }
    {
        const test_file                              file{ generic_path(), json3_with_comment };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("alpha"));
        BOOST_TEST(*p_storage->get_bool("alpha"));
        BOOST_REQUIRE(p_storage->get_uint32("bravo"));
        BOOST_TEST(*p_storage->get_uint32("bravo") == 42U);
        BOOST_REQUIRE(p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
        BOOST_TEST(*p_storage->get_string(std::filesystem::path{ "charlie" } / "delta") == "echo");
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" }));
    }
    {
        const test_file                              file{ generic_path(), json4_escaped };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_string("\"\\/\b\f\n\r\t"));
        constexpr std::string_view expected{ "\xE3\x81\x82\xF0\x9F\x98\x80" };
        BOOST_TEST(*p_storage->get_string("\"\\/\b\f\n\r\t") == expected);
    }
    {
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load("NONEXISTENT_FILE");

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json11_empty };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json12_syntax_error };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json13_unsupported_syntax };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("alpha"));
        BOOST_TEST(*p_storage->get_bool("alpha"));
        BOOST_REQUIRE(p_storage->get_uint32("bravo"));
        BOOST_TEST(*p_storage->get_uint32("bravo") == 42U);
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
        BOOST_REQUIRE(p_storage->get_uint32(std::filesystem::path{ "charlie" } / "foxtrot"));
        BOOST_TEST(*p_storage->get_uint32(std::filesystem::path{ "charlie" } / "foxtrot") == 4242U);
    }

    {
        const test_file   file{ generic_path(), json3_with_comment };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            auto       value = false;
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value == 42U);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), nullptr, 0);
            BOOST_TEST_REQUIRE(length > 0U);
            std::vector<char> value(length + 1, '\0');
            const auto        length_again = tetengo_property_storage_getString(
                p_storage,
                (std::filesystem::path{ "charlie" } / "delta").string().c_str(),
                std::data(value),
                std::size(value));
            BOOST_TEST_REQUIRE(length_again == length);
            BOOST_TEST((std::string_view{ std::data(value), length } == "echo"));
        }
        {
            const auto length = tetengo_property_storage_getString(p_storage, "charlie", nullptr, 0);
            BOOST_TEST(length == static_cast<size_t>(-1));
        }
    }
    {
        const test_file   file{ generic_path(), json4_escaped };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            const auto length = tetengo_property_storage_getString(p_storage, "\"\\/\b\f\n\r\t", nullptr, 0);
            BOOST_TEST_REQUIRE(length > 0U);
            std::vector<char> value(length + 1, '\0');
            const auto        length_again =
                tetengo_property_storage_getString(p_storage, "\"\\/\b\f\n\r\t", std::data(value), std::size(value));
            BOOST_TEST_REQUIRE(length_again == length);
            constexpr std::string_view expected{ "\xE3\x81\x82\xF0\x9F\x98\x80" };
            BOOST_TEST((std::string_view{ std::data(value), length } == expected));
        }
    }
    {
        const test_file   file{ generic_path(), json3_with_comment };
        const auto* const p_storage = tetengo_property_storageLoader_load(nullptr, file.path().string().c_str());
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, nullptr);
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "NONEXISTENT_FILE");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            auto       value = false;
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST(!result);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
            BOOST_TEST(!result);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), nullptr, 0);
            BOOST_TEST(length == static_cast<size_t>(-1));
        }
    }
    {
        const test_file   file{ generic_path(), json11_empty };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            auto       value = false;
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST(!result);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
            BOOST_TEST(!result);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), nullptr, 0);
            BOOST_TEST(length == static_cast<size_t>(-1));
        }
    }
    {
        const test_file   file{ generic_path(), json12_syntax_error };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            auto       value = false;
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST(!result);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
            BOOST_TEST(!result);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), nullptr, 0);
            BOOST_TEST(length == static_cast<size_t>(-1));
        }
    }
    {
        const test_file   file{ generic_path(), json13_unsupported_syntax };
        const auto* const p_loader = tetengo_property_storageLoader_createFileStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, file.path().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            auto       value = false;
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value == 42U);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "charlie" } / "delta").string().c_str(), nullptr, 0);
            BOOST_TEST(length == static_cast<size_t>(-1));
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(
                p_storage, (std::filesystem::path{ "charlie" } / "foxtrot").string().c_str(), &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value == 4242U);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
