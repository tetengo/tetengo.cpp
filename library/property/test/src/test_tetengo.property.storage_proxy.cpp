/*! \file
    \brief A storage proxy.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.h>
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
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_bool("alpha");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto       value = static_cast<int>(0);
        const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto       value = static_cast<int>(0);
        const auto result = tetengo_property_storage_getBool(nullptr, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto       value = static_cast<int>(0);
        const auto result = tetengo_property_storage_getBool(p_storage, nullptr, &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto result = tetengo_property_storage_getBool(p_storage, "alpha", nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(set_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_bool("alpha", false);
        const auto o_value = p_storage->get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(!*o_value);
    }
    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_bool("alpha", true);
        const auto o_value = p_storage->get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setBool(p_storage, "alpha", 0);
        auto       value = static_cast<int>(0);
        const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(!value);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setBool(p_storage, "alpha", 1);
        auto       value = static_cast<int>(0);
        const auto result = tetengo_property_storage_getBool(p_storage, "alpha", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value);
    }
    {
        tetengo_property_storage_setBool(nullptr, "alpha", 0);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setBool(p_storage, nullptr, 0);
    }
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_uint32("bravo");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
        BOOST_TEST(!result);
    }
    {
        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_storage_getUint32(nullptr, "bravo", &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_storage_getUint32(p_storage, nullptr, &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_uint32("bravo", 42);
        const auto o_value = p_storage->get_uint32("bravo");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setUint32(p_storage, "bravo", 42);
        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value == 42U);
    }
    {
        tetengo_property_storage_setUint32(nullptr, "bravo", 42);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setUint32(p_storage, nullptr, 42);
    }
}

BOOST_AUTO_TEST_CASE(get_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_string("charlie");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto length = tetengo_property_storage_getString(p_storage, "charlie", nullptr, 0);
        BOOST_TEST(length == 0U);
    }
    {
        const auto length = tetengo_property_storage_getString(nullptr, "charlie", nullptr, 0);
        BOOST_TEST(length == 0U);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto length = tetengo_property_storage_getString(p_storage, nullptr, nullptr, 0);
        BOOST_TEST(length == 0U);
    }
}

BOOST_AUTO_TEST_CASE(set_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_string("charlie", "foo");
        const auto o_value = p_storage->get_string("charlie");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == "foo");
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setString(p_storage, "charlie", "foo");
        const auto length = tetengo_property_storage_getString(p_storage, "charlie", nullptr, 0);
        BOOST_TEST_REQUIRE(length == 3U);
        std::vector<char> value(length + 1, '\0');
        const auto        length_again =
            tetengo_property_storage_getString(p_storage, "charlie", std::data(value), std::size(value));
        BOOST_TEST_REQUIRE(length_again == length);
        BOOST_TEST((std::string_view{ std::data(value), length } == "foo"));
    }
    {
        tetengo_property_storage_setString(nullptr, "charlie", "foo");
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setString(p_storage, nullptr, "foo");
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setString(p_storage, "charlie", nullptr);
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        p_storage->set_uint32("bravo", 42);
        p_storage->save();
    }
    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
        const auto                                    p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);

        const auto o_value = p_storage->get_uint32("bravo");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_setUint32(p_storage, "bravo", 42);
        tetengo_property_storage_save(p_storage);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_storage_getUint32(p_storage, "bravo", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value == 42U);
    }
    {
        tetengo_property_storage_save(nullptr);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(storage_loader_proxy)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };
    }
    {
        std::unique_ptr<tetengo::property::storage_loader> p_real_loader{};
        BOOST_CHECK_THROW(
            const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) }, std::invalid_argument);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_loader);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(nullptr);
        BOOST_TEST(!p_loader);
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_real_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::storage_loader_proxy loader{ std::move(p_real_loader) };

        const auto p_storage = loader.load("hoge");
        BOOST_CHECK(p_storage);
    }

    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "hoge");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);
    }
    {
        const auto* const p_storage = tetengo_property_storageLoader_load(nullptr, "hoge");
        BOOST_TEST(!p_storage);
    }
    {
        auto* const       p_real_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_loader = tetengo_property_storageLoader_createStorageLoaderProxy(p_real_loader);
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
