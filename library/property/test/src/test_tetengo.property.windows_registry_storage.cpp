/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if defined(_WIN32)

#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/storage.hpp>
#include <tetengo/property/windows_registry_storage.hpp>


namespace
{
    tetengo::property::windows_registry_storage::value_map_type make_value_map()
    {
        tetengo::property::windows_registry_storage::value_map_type value_map{};
        value_map.insert(std::make_pair("alpha", true));
        value_map.insert(std::make_pair("bravo", static_cast<std::uint32_t>(42)));
        value_map.insert(
            std::make_pair((std::filesystem::path{ "charlie" } / "delta").string(), std::string{ "echo" }));
        return value_map;
    }

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

    bool has_registry_key(const std::filesystem::path& value_name)
    {
        auto       registry_key = key_full_path();
        const auto value_name_leaf = value_name.filename();
        if (value_name.has_parent_path())
        {
            registry_key /= value_name.parent_path();
        }
        const auto command = "reg query " + registry_key.string() + " /v " + value_name_leaf.string() + " > NUL 2> NUL";
        const auto result = std::system(command.c_str());
        return result == 0;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(windows_registry_storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::property::windows_registry_storage::value_map_type value_map{};
        const tetengo::property::windows_registry_storage           storage{ std::move(value_map) };
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        test_registry_entry                               test_registry_entry_{ true };
        const auto                                        value_map = make_value_map();
        const tetengo::property::windows_registry_storage storage{ value_map };
        storage.save();

        BOOST_TEST(has_registry_key("alpha"));
        BOOST_TEST(has_registry_key("bravo"));
        BOOST_TEST(has_registry_key("charlie"));
        BOOST_TEST(has_registry_key("delta\\echo"));
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
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#endif
