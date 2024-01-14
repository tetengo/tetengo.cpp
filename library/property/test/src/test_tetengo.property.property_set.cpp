/*! \file
    \brief A property set.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/propertySet.h>
#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.h>
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

    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_property_set);
    }
}

BOOST_AUTO_TEST_CASE(get_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        const auto o_value = property_set_.get_bool("alpha");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        auto       value = false;
        const auto result = tetengo_property_propertySet_getBool(p_property_set, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto       value = false;
        const auto result = tetengo_property_propertySet_getBool(nullptr, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        auto       value = false;
        const auto result = tetengo_property_propertySet_getBool(p_property_set, nullptr, &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        const auto result = tetengo_property_propertySet_getBool(p_property_set, "alpha", nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(set_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.set_bool("alpha", false);
        const auto o_value = property_set_.get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(!*o_value);
    }
    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.set_bool("alpha", true);
        const auto o_value = property_set_.get_bool("alpha");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value);
    }

    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setBool(p_property_set, "alpha", false);
        auto       value = false;
        const auto result = tetengo_property_propertySet_getBool(p_property_set, "alpha", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(!value);
    }
    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setBool(p_property_set, "alpha", true);
        auto       value = false;
        const auto result = tetengo_property_propertySet_getBool(p_property_set, "alpha", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value);
    }
    {
        tetengo_property_propertySet_setBool(nullptr, "alpha", false);
    }
    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setBool(p_property_set, nullptr, false);
    }
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        const auto o_value = property_set_.get_uint32("bravo");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_propertySet_getUint32(p_property_set, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_propertySet_getUint32(nullptr, "alpha", &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_propertySet_getUint32(p_property_set, nullptr, &value);
        BOOST_TEST(!result);
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        const auto result = tetengo_property_propertySet_getUint32(p_property_set, "alpha", nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.set_uint32("bravo", 42);
        const auto o_value = property_set_.get_uint32("bravo");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }

    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setUint32(p_property_set, "bravo", 42);
        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_propertySet_getUint32(p_property_set, "bravo", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value == 42U);
    }
    {
        tetengo_property_propertySet_setUint32(nullptr, "bravo", 0);
    }
    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setUint32(p_property_set, nullptr, 0);
    }
}

BOOST_AUTO_TEST_CASE(get_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        const auto o_value = property_set_.get_string("charlie");
        BOOST_CHECK(!o_value);
    }

    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        const auto length = tetengo_property_propertySet_getString(p_property_set, "charlie", nullptr, 0);
        BOOST_TEST(length == static_cast<size_t>(-1));
    }
    {
        const auto length = tetengo_property_propertySet_getString(nullptr, "charlie", nullptr, 0);
        BOOST_TEST(length == static_cast<size_t>(-1));
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        const auto length = tetengo_property_propertySet_getString(p_property_set, nullptr, nullptr, 0);
        BOOST_TEST(length == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(set_string)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.set_string("charlie", "hoge");
        const auto o_value = property_set_.get_string("charlie");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == "hoge");
    }

    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setString(p_property_set, "charlie", "hoge");
        const auto length = tetengo_property_propertySet_getString(p_property_set, "charlie", nullptr, 0);
        BOOST_TEST_REQUIRE(length == 4U);
        std::vector<char> value(length + 1, '\0');
        const auto        length_again =
            tetengo_property_propertySet_getString(p_property_set, "charlie", std::data(value), std::size(value));
        BOOST_TEST_REQUIRE(length_again == length);
        BOOST_TEST((std::string_view{ std::data(value), length } == "hoge"));
    }
    {
        tetengo_property_propertySet_setString(nullptr, "charlie", "hoge");
    }
    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setString(p_property_set, nullptr, "hoge");
    }
    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setString(p_property_set, "charlie", nullptr);
    }
}

BOOST_AUTO_TEST_CASE(update)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_storage_loader1 = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set1{ std::move(p_storage_loader1), property_set_path() };
        auto p_storage_loader2 = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set2{ std::move(p_storage_loader2), property_set_path() };

        property_set1.set_uint32("hoge.cpp", 42);
        property_set1.commit();
        {
            const auto o_value = property_set2.get_uint32("hoge.cpp");
            BOOST_CHECK(!o_value);
        }
        property_set2.update();
        {
            const auto o_value = property_set2.get_uint32("hoge.cpp");
            BOOST_REQUIRE(o_value);
            BOOST_TEST(*o_value == 42U);
        }
    }

    {
        auto* const p_storage_loader1 = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set1 =
            tetengo_property_propertySet_create(p_storage_loader1, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set1)
        {
            tetengo_property_propertySet_destroy(p_property_set1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set1);

        auto* const p_storage_loader2 = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set2 =
            tetengo_property_propertySet_create(p_storage_loader2, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set2)
        {
            tetengo_property_propertySet_destroy(p_property_set2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set2);

        tetengo_property_propertySet_setUint32(p_property_set1, "hoge", 42);
        tetengo_property_propertySet_commit(p_property_set1);
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_propertySet_getUint32(p_property_set2, "hoge", &value);
            BOOST_TEST(!result);
        }
        tetengo_property_propertySet_update(p_property_set2);
        {
            auto       value = static_cast<::uint32_t>(0);
            const auto result = tetengo_property_propertySet_getUint32(p_property_set2, "hoge", &value);
            BOOST_TEST_REQUIRE(result);
            BOOST_TEST(value == 42U);
        }
    }
}

BOOST_AUTO_TEST_CASE(commit)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                            p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        property_set_.set_uint32("hoge.cpp", 42);
        property_set_.commit();
    }
    {
        auto p_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();
        const tetengo::property::property_set property_set_{ std::move(p_storage_loader), property_set_path() };

        const auto o_value = property_set_.get_uint32("hoge.cpp");
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);
    }

    {
        auto* const p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        tetengo_property_propertySet_setUint32(p_property_set, "hoge", 42);
        tetengo_property_propertySet_commit(p_property_set);
    }
    {
        auto* const       p_storage_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        const auto* const p_property_set =
            tetengo_property_propertySet_create(p_storage_loader, property_set_path().string().c_str());
        BOOST_SCOPE_EXIT(p_property_set)
        {
            tetengo_property_propertySet_destroy(p_property_set);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_property_set);

        auto       value = static_cast<::uint32_t>(0);
        const auto result = tetengo_property_propertySet_getUint32(p_property_set, "hoge", &value);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(value == 42U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
