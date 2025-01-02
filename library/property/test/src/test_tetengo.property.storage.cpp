/*! \file
    \brief A storage.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>


namespace
{
    class concrete_storage : public tetengo::property::storage
    {
    public:
        concrete_storage() : tetengo::property::storage{ value_map_type{} } {}

        virtual ~concrete_storage() = default;

        bool call_value_map() const
        {
            const auto& map = value_map();
            const auto  key = std::filesystem::path{ "hoge" } / "fuga";
            const auto  found = map.find(key.string());
            return found != std::end(map) && std::get<std::uint32_t>(found->second) == 42;
        }

    private:
        virtual void save_impl() const override {}
    };

    class concrete_storage_loader : public tetengo::property::storage_loader
    {
    public:
        virtual ~concrete_storage_loader() = default;

    private:
        virtual std::unique_ptr<tetengo::property::storage>
        load_impl(const std::filesystem::path& /*path*/) const override
        {
            return std::make_unique<concrete_storage>();
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(storage)


BOOST_AUTO_TEST_CASE(get_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage storage{};
        const auto             key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(!storage.get_bool(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        auto       value = false;
        BOOST_TEST(!tetengo_property_storage_getBool(p_storage, key.string().c_str(), &value));
    }
    {
        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        auto       value = false;
        BOOST_TEST(!tetengo_property_storage_getBool(nullptr, key.string().c_str(), &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto value = false;
        BOOST_TEST(!tetengo_property_storage_getBool(p_storage, nullptr, &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(!tetengo_property_storage_getBool(p_storage, key.string().c_str(), nullptr));
    }
}

BOOST_AUTO_TEST_CASE(set_bool)
{
    BOOST_TEST_PASSPOINT();

    {
        concrete_storage storage{};
        const auto       key = std::filesystem::path{ "hoge" } / "fuga";
        storage.set_bool(key, false);
        const auto o_value = storage.get_bool(key);
        BOOST_REQUIRE(o_value);
        BOOST_TEST(!*o_value);

        BOOST_CHECK(!storage.get_string(key));
    }
    {
        concrete_storage storage{};
        const auto       key = std::filesystem::path{ "hoge" } / "fuga";
        storage.set_bool(key, true);
        const auto o_value = storage.get_bool(key);
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value);

        BOOST_CHECK(!storage.get_string(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setBool(p_storage, key.string().c_str(), false);
        auto value = false;
        BOOST_TEST(tetengo_property_storage_getBool(p_storage, key.string().c_str(), &value));
        BOOST_TEST(!value);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setBool(p_storage, key.string().c_str(), true);
        auto value = false;
        BOOST_TEST(tetengo_property_storage_getBool(p_storage, key.string().c_str(), &value));
        BOOST_TEST(value);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setBool(nullptr, key.string().c_str(), true);
        auto value = false;
        BOOST_TEST(!tetengo_property_storage_getBool(p_storage, key.string().c_str(), &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setBool(p_storage, nullptr, true);
        auto value = false;
        BOOST_TEST(!tetengo_property_storage_getBool(p_storage, key.string().c_str(), &value));
    }
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage storage{};
        const auto             key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(!storage.get_uint32(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        auto       value = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage, key.string().c_str(), &value));
    }
    {
        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        auto       value = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(nullptr, key.string().c_str(), &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        auto value = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage, nullptr, &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage, key.string().c_str(), nullptr));
    }
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    {
        concrete_storage storage{};
        const auto       key = std::filesystem::path{ "hoge" } / "fuga";
        storage.set_uint32(key, 42);
        const auto o_value = storage.get_uint32(key);
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == 42U);

        BOOST_CHECK(!storage.get_string(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setUint32(p_storage, key.string().c_str(), 42);
        auto value = static_cast<::uint32_t>(0);
        BOOST_TEST(tetengo_property_storage_getUint32(p_storage, key.string().c_str(), &value));
        BOOST_TEST(value == 42U);
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setUint32(nullptr, key.string().c_str(), 1);
        auto value = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage, key.string().c_str(), &value));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setUint32(p_storage, nullptr, 1);
        auto value = static_cast<::uint32_t>(0);
        BOOST_TEST(!tetengo_property_storage_getUint32(p_storage, key.string().c_str(), &value));
    }
}

BOOST_AUTO_TEST_CASE(get_string)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage storage{};
        const auto             key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(!storage.get_string(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(
            tetengo_property_storage_getString(p_storage, key.string().c_str(), nullptr, 0) == static_cast<size_t>(-1));
    }
    {
        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        BOOST_TEST(
            tetengo_property_storage_getString(nullptr, key.string().c_str(), nullptr, 0) == static_cast<size_t>(-1));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_property_storage_getString(p_storage, nullptr, nullptr, 0) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(set_string)
{
    BOOST_TEST_PASSPOINT();

    {
        concrete_storage storage{};
        const auto       key = std::filesystem::path{ "hoge" } / "fuga";
        storage.set_string(key, "foo");
        const auto o_value = storage.get_string(key);
        BOOST_REQUIRE(o_value);
        BOOST_TEST(*o_value == "foo");

        BOOST_CHECK(!storage.get_uint32(key));
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setString(p_storage, key.string().c_str(), "foo");
        const auto length = tetengo_property_storage_getString(p_storage, key.string().c_str(), nullptr, 0);
        BOOST_TEST(length == 3U);
        std::vector<char> value(length + 1, '\0');
        const auto        length_again =
            tetengo_property_storage_getString(p_storage, key.string().c_str(), std::data(value), std::size(value));
        BOOST_TEST(length == length_again);
        BOOST_TEST(std::string_view{ std::data(value) } == "foo");
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setString(p_storage, key.string().c_str(), "foobar");
        const auto length = tetengo_property_storage_getString(p_storage, key.string().c_str(), nullptr, 0);
        BOOST_TEST(length == 6U);
        std::vector<char> value(5, '\0');
        const auto        length_again =
            tetengo_property_storage_getString(p_storage, key.string().c_str(), std::data(value), std::size(value));
        BOOST_TEST(length == length_again);
        BOOST_TEST(std::string_view{ std::data(value) } == "foob");
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setString(nullptr, key.string().c_str(), "foo");
        BOOST_TEST(
            tetengo_property_storage_getString(p_storage, key.string().c_str(), nullptr, 0) == static_cast<size_t>(-1));
    }
    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto key = std::filesystem::path{ "hoge" } / "fuga";
        tetengo_property_storage_setString(p_storage, nullptr, "foo");
        BOOST_TEST(
            tetengo_property_storage_getString(p_storage, key.string().c_str(), nullptr, 0) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage storage{};
        storage.save();
    }

    {
        const auto* const p_loader = tetengo_property_storageLoader_createMemoryStorageLoader();
        BOOST_SCOPE_EXIT(p_loader)
        {
            tetengo_property_storageLoader_destroy(p_loader);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_property_storageLoader_load(p_loader, "test_tetengo.property.storage");
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_property_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        tetengo_property_storage_save(p_storage);
    }
    {
        tetengo_property_storage_save(nullptr);
    }
}

BOOST_AUTO_TEST_CASE(value_map)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage{};
    const auto       key = std::filesystem::path{ "hoge" } / "fuga";
    storage.set_uint32(key, 42);
    BOOST_TEST(storage.call_value_map());
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage_loader factory{};
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage_loader factory{};

    const auto path = std::filesystem::path{ "foo" } / "bar";
    const auto p_storage = factory.load(path);
    BOOST_CHECK(p_storage);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
