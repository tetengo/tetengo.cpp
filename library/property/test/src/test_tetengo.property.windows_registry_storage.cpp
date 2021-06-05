/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if defined(_WIN32)

#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

#include <boost/algorithm/string.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>
#include <tetengo/property/windows_registry_storage.hpp>


namespace
{
    const std::filesystem::path& subkey()
    {
        static const std::filesystem::path singleton{ "test_tetengo.property.windows_registry_storage" };
        return singleton;
    }

    const std::filesystem::path& key_full_path()
    {
        static const std::filesystem::path singleton{ "HKCU\\SOFTWARE" / subkey() };
        return singleton;
    }

    struct temporary_file
    {
        static const std::filesystem::path& path()
        {
            static const auto singleton =
                std::filesystem::temp_directory_path() / ("test_tetengo.property.windows_registry_storage." +
                                                          boost::uuids::to_string(boost::uuids::random_generator{}()));
            return singleton;
        }

        ~temporary_file()
        {
            if (std::filesystem::exists(path()))
            {
                std::filesystem::remove(path());
            }
        }
    };

    struct test_registry_entry
    {
        explicit test_registry_entry(const bool preset)
        {
            if (preset)
            {
                {
                    const auto command =
                        "reg add " + key_full_path().string() + " /f /v alpha /t REG_DWORD /d 0 > NUL 2> NUL";
                    std::system(command.c_str());
                }
                {
                    const auto command =
                        "reg add " + key_full_path().string() + " /f /v bravo /t REG_DWORD /d 1 > NUL 2> NUL";
                    std::system(command.c_str());
                }
                {
                    const auto command =
                        "reg add " + key_full_path().string() + " /f /v charlie /t REG_DWORD /d 42 > NUL 2> NUL 2> NUL";
                    std::system(command.c_str());
                }
                {
                    const auto command =
                        "reg add " + (key_full_path() / "delta").string() + " /f /v echo /t REG_SZ /d foxtrot > NUL";
                    std::system(command.c_str());
                }
            }
        }

        ~test_registry_entry()
        {
            {
                const auto command = "reg delete " + key_full_path().string() + " /f > NUL 2> NUL";
                std::system(command.c_str());
            }
        }
    };

    bool has_registry_value_impl(
        const std::filesystem::path& file_path,
        const std::filesystem::path& value_name_leaf,
        const std::string_view&      value_type,
        const std::string_view&      value_data)
    {
        std::ifstream stream{ file_path };
        while (stream)
        {
            std::string line{};
            std::getline(stream, line);
            std::vector<std::string> split{};
            boost::algorithm::split(split, line, boost::is_any_of(" "), boost::algorithm::token_compress_on);
            if (std::size(split) >= 4 && split[1] == value_name_leaf && split[2] == value_type &&
                split[3] == value_data)
            {
                return true;
            }
        }
        return false;
    }

    bool has_registry_value(
        const std::filesystem::path& value_name,
        const std::string_view&      value_type,
        const std::string_view&      value_data)
    {
        auto       registry_key = key_full_path();
        const auto value_name_leaf = value_name.filename();
        if (value_name.has_parent_path())
        {
            registry_key /= value_name.parent_path();
        }

        const temporary_file temporary_file_;
        const auto command = "reg query " + registry_key.string() + " /v " + value_name_leaf.string() + " > " +
                             temporary_file::path().string() + " 2> NUL";
        const auto result = std::system(command.c_str());
        return result == 0 && has_registry_value_impl(temporary_file::path(), value_name_leaf, value_type, value_data);
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(windows_registry_storage)


BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        test_registry_entry                                      test_registry_entry_{ false };
        const tetengo::property::windows_registry_storage_loader loader{};
        const auto                                               p_storage = loader.load(subkey());
        BOOST_REQUIRE(p_storage);

        p_storage->set_bool("alpha", true);
        p_storage->set_uint32("bravo", static_cast<std::uint32_t>(42));
        p_storage->set_string((std::filesystem::path{ "charlie" } / "delta").string(), std::string{ "echo" });

        p_storage->save();

        BOOST_TEST(has_registry_value("alpha", "REG_DWORD", "0x1"));
        BOOST_TEST(has_registry_value("bravo", "REG_DWORD", "0x2a"));
        BOOST_TEST(has_registry_value("charlie\\delta", "REG_SZ", "echo"));
    }

    {
        test_registry_entry test_registry_entry_{ false };
        const auto* const   p_loader = tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, subkey().string().c_str());
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

        BOOST_TEST(has_registry_value("alpha", "REG_DWORD", "0x1"));
        BOOST_TEST(has_registry_value("bravo", "REG_DWORD", "0x2a"));
        BOOST_TEST(has_registry_value("charlie\\delta", "REG_SZ", "echo"));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(windows_registry_storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::property::windows_registry_storage_loader loader{};
    }

    {
        test_registry_entry test_registry_entry_{ false };
        const auto* const   p_loader = tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();
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
        test_registry_entry                                      test_registry_entry_{ false };
        const tetengo::property::windows_registry_storage_loader loader{};
        const auto                                               p_storage = loader.load(subkey());
        BOOST_REQUIRE(p_storage);
        {
            const auto o_value = p_storage->get_bool("alpha");
            BOOST_CHECK(!o_value);
        }
        {
            const auto o_value = p_storage->get_bool("bravo");
            BOOST_CHECK(!o_value);
        }
        {
            const auto o_value = p_storage->get_uint32("charlie");
            BOOST_CHECK(!o_value);
        }
        {
            const auto o_value = p_storage->get_string(std::filesystem::path{ "delta" } / "echo");
            BOOST_CHECK(!o_value);
        }
    }
    {
        test_registry_entry                                      test_registry_entry_{ true };
        const tetengo::property::windows_registry_storage_loader loader{};
        const auto                                               p_storage = loader.load(subkey());
        BOOST_REQUIRE(p_storage);
        {
            const auto o_value = p_storage->get_bool("alpha");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(!*o_value);
        }
        {
            const auto o_value = p_storage->get_bool("bravo");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value);
        }
        {
            const auto o_value = p_storage->get_uint32("charlie");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == 42U);
        }
        {
            const auto o_value = p_storage->get_string(std::filesystem::path{ "delta" } / "echo");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == "foxtrot");
        }
    }

    {
        test_registry_entry test_registry_entry_{ false };
        const auto* const   p_loader = tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, subkey().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);

        {
            auto       value = static_cast<int>(0);
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST(!result);
        }
        {
            auto       value = static_cast<int>(0);
            const auto result = tetengo_property_storage_getBool(p_storage, "bravo", &value);
            BOOST_TEST(!result);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "charlie", &value);
            BOOST_TEST(!result);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "delta" } / "echo").string().c_str(), nullptr, 0);
            BOOST_TEST(length == 0U);
        }
    }
    {
        test_registry_entry test_registry_entry_{ true };
        const auto* const   p_loader = tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, subkey().string().c_str());
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);

        {
            auto       value = static_cast<int>(0);
            const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(!value);
        }
        {
            auto       value = static_cast<int>(0);
            const auto result = tetengo_property_storage_getBool(p_storage, "bravo", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value);
        }
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_storage_getUint32(p_storage, "charlie", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value == 42U);
        }
        {
            const auto length = tetengo_property_storage_getString(
                p_storage, (std::filesystem::path{ "delta" } / "echo").string().c_str(), nullptr, 0);
            BOOST_TEST_REQUIRE(length == 7U);
            std::vector<char> value(length + 1, '\0');
            const auto        length_again = tetengo_property_storage_getString(
                p_storage,
                (std::filesystem::path{ "delta" } / "echo").string().c_str(),
                std::data(value),
                std::size(value));
            BOOST_TEST_REQUIRE(length_again == length);
            BOOST_TEST((std::string_view{ std::data(value), length } == "foxtrot"));
        }
    }
    {
        test_registry_entry test_registry_entry_{ false };
        const auto* const   p_storage = tetengo_property_storageLoader_load(nullptr, subkey().string().c_str());
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, nullptr);
        BOOST_TEST(!p_storage);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#endif
