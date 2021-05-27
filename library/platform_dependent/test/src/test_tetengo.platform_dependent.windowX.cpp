/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if defined(_WIN32)

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iterator>

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
                    const auto command =
                        "reg add " + key_full_path().string() + " /f /v hoge /t REG_DWORD /d 42 > NUL 2> NUL";
                    std::system(command.c_str());
                }
                {
                    const auto command =
                        "reg add " + key_full_path().string() + " /f /v fuga /t REG_SZ /d foo > NUL 2> NUL";
                    std::system(command.c_str());
                }
                {
                    const auto command = "reg add " + (key_full_path() / "piyo").string() +
                                         " /f /v piyoyo /t REG_SZ /d bar > NUL 2> NUL";
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

    {
        const test_registry_entry                                  test_registry_entry_{ true };
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
    }
    {
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
    }
}

BOOST_AUTO_TEST_CASE(child_subkeys)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry                                  test_registry_entry_{ true };
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 child_subkeys = reader.child_subkeys();
        BOOST_TEST_REQUIRE(child_subkeys.size() == 1U);
        BOOST_CHECK(child_subkeys[0] == "piyo");
    }
    {
        const test_registry_entry                                  test_registry_entry_{ true };
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() / "piyo" };
        const auto                                                 child_subkeys = reader.child_subkeys();
        BOOST_TEST(child_subkeys.empty());
    }
    {
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 child_subkeys = reader.child_subkeys();
        BOOST_TEST(child_subkeys.empty());
    }
}

BOOST_AUTO_TEST_CASE(value_names)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry                                  test_registry_entry_{ true };
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 value_names = reader.value_names();
        BOOST_TEST(value_names.size() == 2U);
        BOOST_CHECK(std::find(std::begin(value_names), std::end(value_names), "hoge") != std::end(value_names));
        BOOST_CHECK(std::find(std::begin(value_names), std::end(value_names), "fuga") != std::end(value_names));
    }
    {
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 value_names = reader.value_names();
        BOOST_TEST(value_names.empty());
    }
}

BOOST_AUTO_TEST_CASE(dword_value_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry                                  test_registry_entry_{ true };
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
    {
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 o_value = reader.dword_value_of("hoge");
        BOOST_CHECK(!o_value);
    }
}

BOOST_AUTO_TEST_CASE(string_value_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry                                  test_registry_entry_{ true };
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
    {
        const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
        const auto                                                 o_value = reader.string_value_of("fuga");
        BOOST_CHECK(!o_value);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(windows_registry_writer)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry                                  test_registry_entry_{ false };
        const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
    }
    {
        const test_registry_entry                                  test_registry_entry_{ true };
        const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
    }
}

BOOST_AUTO_TEST_CASE(set_dword_value_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry test_registry_entry_{ false };
        {
            const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
            writer.set_dword_value_of("hoge", 24);
        }
        {
            const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
            const auto                                                 o_value = reader.dword_value_of("hoge");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == 24U);
        }
    }
    {
        const test_registry_entry test_registry_entry_{ true };
        {
            const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
            writer.set_dword_value_of("hoge", 24);
        }
        {
            const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
            const auto                                                 o_value = reader.dword_value_of("hoge");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == 24U);
        }
    }
}

BOOST_AUTO_TEST_CASE(set_string_value_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_registry_entry test_registry_entry_{ false };
        {
            const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
            writer.set_string_value_of("fuga", "bar");
        }
        {
            const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
            const auto                                                 o_value = reader.string_value_of("fuga");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == "bar");
        }
    }
    {
        const test_registry_entry test_registry_entry_{ true };
        {
            const tetengo::platform_dependent::windows_registry_writer writer{ subkey() };
            writer.set_string_value_of("fuga", "bar");
        }
        {
            const tetengo::platform_dependent::windows_registry_reader reader{ subkey() };
            const auto                                                 o_value = reader.string_value_of("fuga");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == "bar");
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#endif
