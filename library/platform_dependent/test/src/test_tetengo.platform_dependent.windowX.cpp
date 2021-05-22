/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if _WIN32

#include <cstdlib>
#include <filesystem>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>


namespace
{
    static const std::filesystem::path& subkey()
    {
        static const std::filesystem::path singleton{ "test_tetengo.platform_dependent.windows_registry" };
        return singleton;
    }

    class test_registry_entry
    {
    public:
        explicit test_registry_entry(const bool preset)
        {
            if (preset)
            {
                {
                    const auto command = "reg add " + key_full_path().string() + " /f /v hoge /t REG_DWORD /d 42";
                    std::system(command.c_str());
                }
                {
                    const auto command = "reg add " + key_full_path().string() + " /f /v fuga /t REG_SZ /d foo";
                    std::system(command.c_str());
                }
            }
        }

        ~test_registry_entry()
        {
            {
                const auto command = "reg delete " + key_full_path().string() + " /f";
                std::system(command.c_str());
            }
        }

    private:
        static const std::filesystem::path& key_full_path()
        {
            static const std::filesystem::path singleton{ "HKCU\\SOFTWARE" / subkey() };
            return singleton;
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(platform_dependent)
BOOST_AUTO_TEST_SUITE(windows_registry_reader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const test_registry_entry test_registry_entry_{ true };

    const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
}

BOOST_AUTO_TEST_CASE(dword_value_of)
{
    BOOST_TEST_PASSPOINT();

    const test_registry_entry test_registry_entry_{ true };

    const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
    {
        const auto o_value = reader.dword_value_of("hoge");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }
    {
        const auto o_value = reader.dword_value_of("fuga");
        BOOST_CHECK(!o_value);
    }
    {
        const auto o_value = reader.dword_value_of("piyo");
        BOOST_CHECK(!o_value);
    }
}

BOOST_AUTO_TEST_CASE(string_value_of)
{
    BOOST_TEST_PASSPOINT();

    const test_registry_entry test_registry_entry_{ true };

    const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
    {
        const auto o_value = reader.string_value_of("fuga");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == "foo");
    }
    {
        const auto o_value = reader.string_value_of("hoge");
        BOOST_CHECK(!o_value);
    }
    {
        const auto o_value = reader.string_value_of("piyo");
        BOOST_CHECK(!o_value);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(windows_registry_writer)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry test_registry_entry_{ false };

        const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
    }
    {
        const test_registry_entry test_registry_entry_{ true };

        const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#endif
