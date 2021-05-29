/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.hpp>


namespace
{
    const std::filesystem::path& property_set_path()
    {
        static const std::filesystem::path singleton{ "test_tetengo.property.property_set" };
        return singleton;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(property_set)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };
    }
}

BOOST_AUTO_TEST_CASE(get_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(set_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(get_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(set_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(update)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.update();
    }
}

BOOST_AUTO_TEST_CASE(commit)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.commit();
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
