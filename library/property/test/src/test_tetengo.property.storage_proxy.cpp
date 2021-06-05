/*! \file
    \brief A storage proxy.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <optional>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.hpp>
#include <tetengo/property/storage_proxy.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(storage_proxy)


BOOST_AUTO_TEST_CASE(get_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_bool("alpha");
        BOOST_CHECK(!o_value);
    }
}

BOOST_AUTO_TEST_CASE(set_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_bool("alpha", false);
        const auto o_value = p_storage->get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(!*o_value);
    }
    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_bool("alpha", true);
        const auto o_value = p_storage->get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value);
    }
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_uint32("bravo");
        BOOST_CHECK(!o_value);
    }
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_uint32("bravo", 42);
        const auto o_value = p_storage->get_uint32("bravo");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }
}

BOOST_AUTO_TEST_CASE(get_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_string("charlie");
        BOOST_CHECK(!o_value);
    }
}

BOOST_AUTO_TEST_CASE(set_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_string("charlie", "foo");
        const auto o_value = p_storage->get_string("charlie");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == "foo");
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        {
            auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
            const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
            const auto                                    p_storage = loader.load("hoge");
            BOOST_CHECK(p_storage);

            p_storage->set_uint32("bravo", 42);
            p_storage->save();
        }
        {
            auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
            const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
            const auto                                    p_storage = loader.load("hoge");
            BOOST_CHECK(p_storage);

            const auto o_value = p_storage->get_uint32("bravo");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == 42U);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(storage_proxy_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_proxy_loader loader{ std::move(p_real_loader) };

        const auto p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
