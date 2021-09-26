/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <functional>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/value_serializer.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    constexpr char nul_byte()
    {
        return 0xFE_c;
    }

    const std::vector<char> serialized{
        // clang-format off
        nul_byte(), nul_byte(), nul_byte(), 0x02_c,
        nul_byte(), nul_byte(), 0x2A_c,     0xFF_c,
        nul_byte(), nul_byte(), 0xFD_c,     0xFE_c,     0x18_c,
        nul_byte(), nul_byte(), nul_byte(), 0x05_c,
        nul_byte(), nul_byte(), nul_byte(), nul_byte(),
        nul_byte(), nul_byte(), nul_byte(), nul_byte(),
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0x70_c,     0x69_c,     0x79_c,     0x6F_c,
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0x66_c,     0x75_c,     0x67_c,     0x61_c,
        nul_byte(), nul_byte(), nul_byte(), nul_byte(),
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0x68_c,     0x6F_c,     0x67_c,     0x65_c,
        // clang-format on
    };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ std::begin(serialized), std::end(serialized) });
    }

    const std::vector<char> serialized_fixed_value_size{
        // clang-format off
        nul_byte(), nul_byte(), nul_byte(), 0x02_c,
        nul_byte(), nul_byte(), 0x2A_c,     0xFF_c,
        nul_byte(), nul_byte(), 0xFD_c,     0xFE_c, 0x18_c,
        nul_byte(), nul_byte(), nul_byte(), 0x05_c,
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0xFF_c,     0xFF_c,     0xFF_c,     0xFF_c,
        nul_byte(), nul_byte(), nul_byte(), 0x9F_c,
        nul_byte(), nul_byte(), nul_byte(), 0x0E_c,
        0xFF_c,     0xFF_c,     0xFF_c,     0xFF_c,
        nul_byte(), nul_byte(), nul_byte(), 0x03_c,
        // clang-format on
    };

    std::unique_ptr<std::istream> create_input_stream_fixed_value_size()
    {
        return std::make_unique<std::stringstream>(
            std::string{ std::begin(serialized_fixed_value_size), std::end(serialized_fixed_value_size) });
    }

    const std::vector<uint32_t> base_check_array{ 0x00002AFF, 0x0000FE18 };

    const std::vector<char> serialized_broken{
        // clang-format off
        nul_byte(), nul_byte(), nul_byte(), 0x02_c,
        0x01_c,     0x23_c,     0x45_c,     0x67_c, 
        0x89_c,
        // clang-format on
    };

    std::unique_ptr<std::istream> create_broken_input_stream()
    {
        return std::make_unique<std::stringstream>(
            std::string{ std::begin(serialized_broken), std::end(serialized_broken) });
    }

    const std::vector<char> serialized_c_if{
        // clang-format off
        nul_byte(), nul_byte(), nul_byte(), 0x11_c,
        0xFF_c,     0xFF_c,     0xB6_c,     0xFF_c,
        0xFF_c,     0xFF_c,     0x8D_c,     0x4B_c,
        0xFF_c,     0xFF_c,     0x96_c,     0x75_c,
        0xFF_c,     0xFF_c,     0xA3_c,     0x6D_c,
        0xFF_c,     0xFF_c,     0x98_c,     0x61_c,
        0xFF_c,     0xFF_c,     0x97_c,     0x6D_c,
        0xFF_c,     0xFF_c,     0x93_c,     0x6F_c,
        0xFF_c,     0xFF_c,     0x99_c,     0x74_c,
        nul_byte(), nul_byte(), 0x09_c,     0x6F_c,
        nul_byte(), nul_byte(), nul_byte(), nul_byte(),
        0xFF_c,     0xFF_c,     0xAA_c,     0x54_c,
        0xFF_c,     0xFF_c,     0x9F_c,     0x61_c,
        0xFF_c,     0xFF_c,     0xAC_c,     0x6D_c,
        0xFF_c,     0xFF_c,     0xA0_c,     0x61_c,
        0xFF_c,     0xFF_c,     0xAE_c,     0x6E_c,
        nul_byte(), nul_byte(), 0x10_c,     0x61_c,
        nul_byte(), nul_byte(), 0x01_c,     nul_byte(),
        nul_byte(), nul_byte(), nul_byte(), 0x02_c,
        nul_byte(), nul_byte(), nul_byte(), nul_byte(),
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0x2A_c,     0x00_c,     0x00_c,     0x00_c,
        nul_byte(), nul_byte(), nul_byte(), 0x04_c,
        0x18_c,     0x00_c,     0x00_c,     0x00_c,
        // clang-format on
    };

    std::filesystem::path temporary_file_path(const std::vector<char>& initial_content = std::vector<char>{})
    {
        const auto path = std::filesystem::temp_directory_path() / "test_tetengo.trie.memory_storage";

        {
            std::ofstream stream{ path, std::ios_base::binary };
            stream.write(std::data(initial_content), std::size(initial_content));
        }

        return path;
    }

    std::vector<char> file_content(const std::filesystem::path& path)
    {
        const auto        file_size = std::filesystem::file_size(path);
        std::ifstream     stream{ path, std::ios_base::binary };
        std::vector<char> content(static_cast<std::size_t>(file_size), 0);
        stream.read(std::data(content), file_size);
        assert(stream.gcount() == static_cast<std::streamsize>(file_size));
        return content;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(memory_storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::memory_storage storage_{};
    }
    {
        const auto                              p_input_stream = create_input_stream();
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::string>string_deserializer{};
            return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
        } };
        const tetengo::trie::memory_storage storage_{ *p_input_stream, deserializer };

        BOOST_TEST(storage_.base_check_array() == base_check_array);
        BOOST_REQUIRE(storage_.value_at(4));
        BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(4)) == "hoge");
        BOOST_REQUIRE(storage_.value_at(2));
        BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(2)) == "fuga");
        BOOST_REQUIRE(storage_.value_at(1));
        BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(1)) == "piyo");
    }
    {
        const auto                              p_input_stream = create_input_stream_fixed_value_size();
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{};
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::memory_storage storage_{ *p_input_stream, deserializer };

        BOOST_TEST(storage_.base_check_array() == base_check_array);
        BOOST_REQUIRE(storage_.value_at(4));
        BOOST_TEST(std::any_cast<std::uint32_t>(*storage_.value_at(4)) == 3U);
        BOOST_REQUIRE(storage_.value_at(2));
        BOOST_TEST(std::any_cast<std::uint32_t>(*storage_.value_at(2)) == 14U);
        BOOST_REQUIRE(storage_.value_at(1));
        BOOST_TEST(std::any_cast<std::uint32_t>(*storage_.value_at(1)) == 159U);
    }
    {
        const auto p_input_stream = create_broken_input_stream();

        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::string>string_deserializer{};
            return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
        } };
        BOOST_CHECK_THROW(
            const tetengo::trie::memory_storage storage_(*p_input_stream, deserializer), std::ios_base::failure);
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createStorage(p_trie);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
    }
    {
        BOOST_TEST(!tetengo_trie_storage_createStorage(nullptr));
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMemoryStorage(file_path.c_str());
        BOOST_TEST(tetengo_trie_storage_size(p_storage) == 2U);

        const auto* const p_trie = tetengo_trie_trie_createWithStorage(p_storage);
        BOOST_SCOPE_EXIT(&p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        {
            const auto* const p_value = tetengo_trie_trie_find(p_trie, "Kumamoto");
            BOOST_TEST_REQUIRE(p_value);
            BOOST_TEST(*static_cast<const int*>(p_value) == 42);
        }
        {
            const auto* const p_value = tetengo_trie_trie_find(p_trie, "Tamana");
            BOOST_TEST_REQUIRE(p_value);
            BOOST_TEST(*static_cast<const int*>(p_value) == 24);
        }
        {
            const auto* const p_value = tetengo_trie_trie_find(p_trie, "Uto");
            BOOST_TEST(!p_value);
        }
    }
    {
        BOOST_TEST(!tetengo_trie_storage_createMemoryStorage(nullptr));
    }
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::memory_storage storage_{};

    BOOST_TEST(storage_.base_at(42) == 0);
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::memory_storage storage_{};

    storage_.set_base_at(42, 4242);

    BOOST_TEST(storage_.base_at(42) == 4242);
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::memory_storage storage_{};

    BOOST_TEST(storage_.check_at(24) == tetengo::trie::double_array::vacant_check_value());
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::memory_storage storage_{};

    storage_.set_check_at(24, 124);

    BOOST_TEST(storage_.check_at(24) == 124);
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::memory_storage storage_{};
        BOOST_TEST(std::size(storage_) == 0U);

        storage_.add_value_at(24, std::make_any<std::string>("hoge"));
        BOOST_TEST(std::size(storage_) == 25U);

        storage_.add_value_at(42, std::make_any<std::string>("fuga"));
        BOOST_TEST(std::size(storage_) == 43U);

        storage_.add_value_at(0, std::make_any<std::string>("piyo"));
        BOOST_TEST(std::size(storage_) == 43U);
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);

        BOOST_TEST(tetengo_trie_storage_size(p_storage) == 2U);
    }
    {
        BOOST_TEST(tetengo_trie_storage_size(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(filling_rate)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::memory_storage storage_{};

        for (auto i = static_cast<std::size_t>(0); i < 9; ++i)
        {
            if (i % 3 == 0)
            {
                storage_.set_base_at(i, static_cast<std::int32_t>(i * i));
                storage_.set_check_at(i, static_cast<std::uint8_t>(i));
            }
            else
            {
                storage_.set_base_at(i, storage_.base_at(i));
                storage_.set_check_at(i, storage_.check_at(i));
            }
        }

        BOOST_CHECK_CLOSE(storage_.filling_rate(), 3.0 / 9.0, 0.1);
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);

        BOOST_TEST(0.0 < tetengo_trie_storage_fillingRate(p_storage));
        BOOST_TEST(tetengo_trie_storage_fillingRate(p_storage) <= 1.0);
    }
    {
        BOOST_TEST(std::isnan(tetengo_trie_storage_fillingRate(nullptr)));
    }
}

BOOST_AUTO_TEST_CASE(base_check_array)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::memory_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_base_at(1, 0xFE);
    storage_.set_check_at(1, 24);

    const auto base_check_array = storage_.base_check_array();

    static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x0000FE18 };
    BOOST_TEST(base_check_array == expected);
}

BOOST_AUTO_TEST_CASE(value_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::memory_storage storage_{};

    BOOST_TEST(!storage_.value_at(42));
}

BOOST_AUTO_TEST_CASE(add_value_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::memory_storage storage_{};

    storage_.add_value_at(24, std::make_any<std::string>("hoge"));

    BOOST_TEST(!storage_.value_at(0));
    BOOST_REQUIRE(storage_.value_at(24));
    BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(24)) == "hoge");
    BOOST_TEST(!storage_.value_at(42));

    storage_.add_value_at(42, std::make_any<std::string>("fuga"));

    BOOST_REQUIRE(storage_.value_at(42));
    BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(42)) == "fuga");
    BOOST_TEST(!storage_.value_at(4242));

    storage_.add_value_at(0, std::make_any<std::string>("piyo"));

    BOOST_REQUIRE(storage_.value_at(0));
    BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(0)) == "piyo");
    BOOST_REQUIRE(storage_.value_at(42));
    BOOST_TEST(std::any_cast<std::string>(*storage_.value_at(42)) == "fuga");
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::memory_storage storage_{};

        storage_.set_base_at(0, 42);
        storage_.set_base_at(1, 0xFE);
        storage_.set_check_at(1, 24);

        storage_.add_value_at(4, std::make_any<std::string>("hoge"));
        storage_.add_value_at(2, std::make_any<std::string>("fuga"));
        storage_.add_value_at(1, std::make_any<std::string>("piyo"));

        std::ostringstream                    output_stream{};
        const tetengo::trie::value_serializer serializer{
            [](const std::any& object) {
                static const tetengo::trie::default_serializer<std::string> string_serializer{};
                const auto serialized = string_serializer(std::any_cast<std::string>(object));
                return std::vector<char>{ std::begin(serialized), std::end(serialized) };
            },
            0
        };
        storage_.serialize(output_stream, serializer);

        static const std::string expected{
            // clang-format off
            nul_byte(), nul_byte(), nul_byte(), 0x02_c,
            nul_byte(), nul_byte(), 0x2A_c,     0xFF_c,
            nul_byte(), nul_byte(), 0xFD_c,     0xFE_c,     0x18_c,
            nul_byte(), nul_byte(), nul_byte(), 0x05_c,
            nul_byte(), nul_byte(), nul_byte(), nul_byte(),
            nul_byte(), nul_byte(), nul_byte(), nul_byte(),
            nul_byte(), nul_byte(), nul_byte(), 0x04_c,
            0x70_c,     0x69_c,     0x79_c,     0x6F_c,
            nul_byte(), nul_byte(), nul_byte(), 0x04_c,
            0x66_c,     0x75_c,     0x67_c,     0x61_c,
            nul_byte(), nul_byte(), nul_byte(), nul_byte(),
            nul_byte(), nul_byte(), nul_byte(), 0x04_c,
            0x68_c,     0x6F_c,     0x67_c,     0x65_c,
            // clang-format on
        };
        const std::string serialized = output_stream.str();
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(serialized), std::end(serialized), std::begin(expected), std::end(expected));
    }
    {
        tetengo::trie::memory_storage storage_{};

        storage_.set_base_at(0, 42);
        storage_.set_base_at(1, 0xFE);
        storage_.set_check_at(1, 24);

        storage_.add_value_at(4, std::make_any<std::uint32_t>(3));
        storage_.add_value_at(2, std::make_any<std::uint32_t>(14));
        storage_.add_value_at(1, std::make_any<std::uint32_t>(159));

        std::ostringstream                    output_stream{};
        const tetengo::trie::value_serializer serializer{
            [](const std::any& object) {
                static const tetengo::trie::default_serializer<std::uint32_t> uint32_serializer{};
                const auto serialized = uint32_serializer(std::any_cast<std::uint32_t>(object));
                return std::vector<char>{ std::begin(serialized), std::end(serialized) };
            },
            sizeof(std::uint32_t)
        };
        storage_.serialize(output_stream, serializer);

        static const std::string expected{
            // clang-format off
            nul_byte(), nul_byte(), nul_byte(), 0x02_c,
            nul_byte(), nul_byte(), 0x2A_c,     0xFF_c,
            nul_byte(), nul_byte(), 0xFD_c,     0xFE_c, 0x18_c,
            nul_byte(), nul_byte(), nul_byte(), 0x05_c,
            nul_byte(), nul_byte(), nul_byte(), 0x04_c,
            0xFF_c,     0xFF_c,     0xFF_c,     0xFF_c,
            nul_byte(), nul_byte(), nul_byte(), 0x9F_c,
            nul_byte(), nul_byte(), nul_byte(), 0x0E_c,
            0xFF_c,     0xFF_c,     0xFF_c,     0xFF_c,
            nul_byte(), nul_byte(), nul_byte(), 0x03_c,
            // clang-format on
        };
        const std::string serialized = output_stream.str();
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(serialized), std::end(serialized), std::begin(expected), std::end(expected));
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);

        {
            const auto file_path = temporary_file_path();
            BOOST_SCOPE_EXIT(&file_path)
            {
                std::filesystem::remove(file_path);
            }
            BOOST_SCOPE_EXIT_END;

            tetengo_trie_storage_serialize(p_storage, file_path.c_str());

            static const std::string expected{
                // clang-format off
                nul_byte(), nul_byte(), nul_byte(), 0x11_c,
                0xFF_c,     0xFF_c,     0xB6_c,     0xFF_c,
                0xFF_c,     0xFF_c,     0x8D_c,     0x4B_c,
                0xFF_c,     0xFF_c,     0x96_c,     0x75_c,
                0xFF_c,     0xFF_c,     0xA3_c,     0x6D_c,
                0xFF_c,     0xFF_c,     0x98_c,     0x61_c,
                0xFF_c,     0xFF_c,     0x97_c,     0x6D_c,
                0xFF_c,     0xFF_c,     0x93_c,     0x6F_c,
                0xFF_c,     0xFF_c,     0x99_c,     0x74_c,
                nul_byte(), nul_byte(), 0x09_c,     0x6F_c,
                nul_byte(), nul_byte(), nul_byte(), nul_byte(),
                0xFF_c,     0xFF_c,     0xAA_c,     0x54_c,
                0xFF_c,     0xFF_c,     0x9F_c,     0x61_c,
                0xFF_c,     0xFF_c,     0xAC_c,     0x6D_c,
                0xFF_c,     0xFF_c,     0xA0_c,     0x61_c,
                0xFF_c,     0xFF_c,     0xAE_c,     0x6E_c,
                nul_byte(), nul_byte(), 0x10_c,     0x61_c,
                nul_byte(), nul_byte(), 0x01_c,     nul_byte(),
                nul_byte(), nul_byte(), nul_byte(), 0x02_c,
                nul_byte(), nul_byte(), nul_byte(), nul_byte(),
                nul_byte(), nul_byte(), nul_byte(), 0x04_c,
                0x2A_c,     0x00_c,     0x00_c,     0x00_c,
                nul_byte(), nul_byte(), nul_byte(), 0x04_c,
                0x18_c,     0x00_c,     0x00_c,     0x00_c,
                // clang-format on
            };
            const auto serialized = file_content(file_path);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(serialized), std::end(serialized), std::begin(expected), std::end(expected));
        }
    }
    {
        const auto file_path = temporary_file_path();
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_storage_serialize(nullptr, file_path.c_str());

        const auto serialized = file_content(file_path);
        BOOST_TEST(std::empty(serialized));
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);

        tetengo_trie_storage_serialize(p_storage, nullptr);
    }
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::memory_storage storage_{};

        storage_.set_base_at(0, 42);
        storage_.set_base_at(1, 0xFE);
        storage_.set_check_at(1, 24);

        const auto p_clone = storage_.clone();

        const auto base_check_array = p_clone->base_check_array();

        static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x0000FE18 };
        BOOST_TEST(base_check_array == expected);
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);
        auto* const       p_cloned = tetengo_trie_storage_clone(p_storage);
        BOOST_TEST(tetengo_trie_storage_size(p_storage) == tetengo_trie_storage_size(p_cloned));

        const auto* const p_cloned_trie = tetengo_trie_trie_createWithStorage(p_cloned);
        BOOST_SCOPE_EXIT(p_cloned_trie)
        {
            tetengo_trie_trie_destroy(p_cloned_trie);
        }
        BOOST_SCOPE_EXIT_END;

        {
            const auto p_found = tetengo_trie_trie_find(p_cloned_trie, "Kumamoto");
            BOOST_TEST_REQUIRE(p_found);
            BOOST_TEST(*static_cast<const int*>(p_found) == kumamoto_value);
        }
        {
            const auto p_found = tetengo_trie_trie_find(p_cloned_trie, "Tamana");
            BOOST_TEST_REQUIRE(p_found);
            BOOST_TEST(*static_cast<const int*>(p_found) == tamana_value);
        }
        {
            const auto p_found = tetengo_trie_trie_find(p_cloned_trie, "Uto");
            BOOST_TEST(!p_found);
        }
    }
    {
        BOOST_TEST(!tetengo_trie_storage_clone(nullptr));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
