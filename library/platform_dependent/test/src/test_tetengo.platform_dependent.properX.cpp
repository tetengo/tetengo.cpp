/*! \file
    \brief A property set file path.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <stdexcept>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(platform_dependent)
BOOST_AUTO_TEST_SUITE(property_set_file_path)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
}

BOOST_AUTO_TEST_CASE(to_native_path)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        const auto  native_path = resolver.to_native_path(std::filesystem::path{ "hoge" });
    }
    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        const auto  native_path = resolver.to_native_path(std::filesystem::path{ "hoge" } / "fuga");
    }
    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        BOOST_CHECK_THROW(
            const auto native_path = resolver.to_native_path(std::filesystem::path{}), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(to_native_top_path)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        const auto  native_path = resolver.to_native_top_path(std::filesystem::path{ "hoge" });
    }
    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        const auto  native_path = resolver.to_native_top_path(std::filesystem::path{ "hoge" } / "fuga");
    }
    {
        const auto& resolver = tetengo::platform_dependent::property_set_file_path::instance();
        BOOST_CHECK_THROW(
            const auto native_path = resolver.to_native_top_path(std::filesystem::path{}), std::invalid_argument);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
