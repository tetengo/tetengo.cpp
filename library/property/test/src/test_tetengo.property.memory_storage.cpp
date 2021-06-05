/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(memory_storage)


BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::property::memory_storage_loader loader{};
        const auto p_storage1 = loader.load("test_tetengo.property.memory_storage.cpp");
        BOOST_REQUIRE(p_storage1);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        p_storage1->set_uint32(key, 42);

        const auto p_storage2 = loader.load("test_tetengo.property.memory_storage.cpp");
        BOOST_REQUIRE(p_storage2);
        BOOST_TEST(!p_storage2->get_uint32(key));

        p_storage1->save();

        const auto p_storage3 = loader.load("test_tetengo.property.memory_storage.cpp");
        BOOST_REQUIRE(p_storage3);
        BOOST_REQUIRE(p_storage3->get_uint32(key));
        BOOST_TEST(*p_storage3->get_uint32(key) == 42U);
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage1 = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.memory_storage");
        BOOST_SCOPE_EXIT(p_storage1)
        {
            tetengo_property_storage_destroy(p_storage1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage1);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setUint32(p_storage1, key.string().c_str(), 42);

        const auto* const p_storage2 =
            tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.memory_storage");
        BOOST_SCOPE_EXIT(p_storage2)
        {
            tetengo_property_storage_destroy(p_storage2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage2);
        auto value2 = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage2, key.string().c_str(), &value2));

        tetengo_property_storage_save(p_storage1);

        const auto* const p_storage3 =
            tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.memory_storage");
        BOOST_SCOPE_EXIT(p_storage3)
        {
            tetengo_property_storage_destroy(p_storage3);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage3);
        auto value3 = static_cast<::uint32_t>(0);
        BOOST_TEST(tetengo_property_storage_getUint32(p_storage3, key.string().c_str(), &value3));
        BOOST_TEST(value3 == 42U);
    }
    {
        tetengo_property_storage_save(nullptr);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(memory_storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::property::memory_storage_loader loader{};
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
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
        const tetengo::property::memory_storage_loader loader{};
        const auto                                     p_storage = loader.load("foo");
        BOOST_CHECK(p_storage);
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage =
            tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.memory_storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);
    }
    {
        const auto* const p_storage = tetengo_property_storageLoader_load(nullptr, "foo");
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
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
