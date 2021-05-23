/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/storage.hpp>
#include <tetengo/property/windows_registry_storage.hpp>


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

    BOOST_WARN_MESSAGE(false, "Implement it.");
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

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
