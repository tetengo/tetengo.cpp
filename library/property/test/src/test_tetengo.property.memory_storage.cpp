/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <optional>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(memory_storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::property::memory_storage::value_map_type master_value_map{};
        const tetengo::property::memory_storage           storage{ master_value_map };
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "foo");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::property::memory_storage::value_map_type master_value_map{};
        tetengo::property::memory_storage                 storage1{ master_value_map };

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        storage1.set_uint32(key, 42);

        const tetengo::property::memory_storage storage2{ master_value_map };
        BOOST_TEST(!storage2.get_uint32(key));

        storage1.save();

        const tetengo::property::memory_storage storage3{ master_value_map };
        BOOST_REQUIRE(storage3.get_uint32(key));
        BOOST_TEST(*storage3.get_uint32(key) == 42U);
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage1 = tetengo_property_storageLoader_load(p_loader, "foo");
        BOOST_SCOPE_EXIT(p_storage1)
        {
            tetengo_property_storage_destroy(p_storage1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage1);
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

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "foo");
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
