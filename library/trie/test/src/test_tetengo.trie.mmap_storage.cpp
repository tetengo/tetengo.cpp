/*! \file
    \brief An mmap storage.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cmath>
#include <cstddef> // IWYU pragma: keep
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ios>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/cstdint.hpp> // IWYU pragma: keep
#include <boost/interprocess/file_mapping.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/value_serializer.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::vector<char> serialized{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x2A_c, 0xFF_c,
        0x00_c, 0x00_c, 0xFE_c, 0x18_c,
        0x00_c, 0x00_c, 0x00_c, 0x05_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x70_c, 0x69_c, 0x79_c, 0x6F_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x66_c, 0x75_c, 0x67_c, 0x61_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x68_c, 0x6F_c, 0x67_c, 0x65_c,
        // clang-format on
    };

    const std::vector<char> serialized_fixed_value_size{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x2A_c, 0xFF_c,
        0x00_c, 0x00_c, 0xFE_c, 0x18_c,
        0x00_c, 0x00_c, 0x00_c, 0x05_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0xFF_c, 0xFF_c, 0xFF_c, 0xFF_c,
        0x00_c, 0x00_c, 0x00_c, 0x9F_c,
        0x00_c, 0x00_c, 0x00_c, 0x0E_c,
        0xFF_c, 0xFF_c, 0xFF_c, 0xFF_c,
        0x00_c, 0x00_c, 0x00_c, 0x03_c,
        // clang-format on
    };

    const std::vector<char> serialized_fixed_value_size_with_header{
        // clang-format off
        
        // header
        0x01_c, 0x23_c, 0x45_c, 0x67_c, 0x89_c,

        // constent
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x2A_c, 0xFF_c,
        0x00_c, 0x00_c, 0xFE_c, 0x18_c,
        0x00_c, 0x00_c, 0x00_c, 0x05_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0xFF_c, 0xFF_c, 0xFF_c, 0xFF_c,
        0x00_c, 0x00_c, 0x00_c, 0x9F_c,
        0x00_c, 0x00_c, 0x00_c, 0x0E_c,
        0xFF_c, 0xFF_c, 0xFF_c, 0xFF_c,
        0x00_c, 0x00_c, 0x00_c, 0x03_c,
        // clang-format on
    };

    const std::vector<char> serialized_fixed_value_size_for_calculating_filling_rate{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x00_c, 0xFF_c,
        0x00_c, 0x00_c, 0xFE_c, 0x18_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        // clang-format on
    };

    const std::vector<char> serialized_c_if{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x11_c,
        0xFF_c, 0xFF_c, 0xB6_c, 0xFF_c,
        0xFF_c, 0xFF_c, 0x8D_c, 0x4B_c,
        0xFF_c, 0xFF_c, 0x96_c, 0x75_c,
        0xFF_c, 0xFF_c, 0xA3_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x98_c, 0x61_c,
        0xFF_c, 0xFF_c, 0x97_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x93_c, 0x6F_c,
        0xFF_c, 0xFF_c, 0x99_c, 0x74_c,
        0x00_c, 0x00_c, 0x09_c, 0x6F_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0xFF_c, 0xFF_c, 0xAA_c, 0x54_c,
        0xFF_c, 0xFF_c, 0x9F_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAC_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0xA0_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAE_c, 0x6E_c,
        0x00_c, 0x00_c, 0x10_c, 0x61_c,
        0x00_c, 0x00_c, 0x01_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x2A_c, 0x00_c, 0x00_c, 0x00_c,
        0x18_c, 0x00_c, 0x00_c, 0x00_c,
        // clang-format on
    };

    const std::vector<char> serialized_c_if_with_header{
        // clang-format off
        
        // header
        0x01_c, 0x23_c, 0x45_c, 0x67_c, 0x89_c, 0xAB_c, 0xCD_c,

        // constent
        0x00_c, 0x00_c, 0x00_c, 0x11_c,
        0xFF_c, 0xFF_c, 0xB6_c, 0xFF_c,
        0xFF_c, 0xFF_c, 0x8D_c, 0x4B_c,
        0xFF_c, 0xFF_c, 0x96_c, 0x75_c,
        0xFF_c, 0xFF_c, 0xA3_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x98_c, 0x61_c,
        0xFF_c, 0xFF_c, 0x97_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x93_c, 0x6F_c,
        0xFF_c, 0xFF_c, 0x99_c, 0x74_c,
        0x00_c, 0x00_c, 0x09_c, 0x6F_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0xFF_c, 0xFF_c, 0xAA_c, 0x54_c,
        0xFF_c, 0xFF_c, 0x9F_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAC_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0xA0_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAE_c, 0x6E_c,
        0x00_c, 0x00_c, 0x10_c, 0x61_c,
        0x00_c, 0x00_c, 0x01_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x2A_c, 0x00_c, 0x00_c, 0x00_c,
        0x18_c, 0x00_c, 0x00_c, 0x00_c,
        // clang-format on
    };

    const std::vector<char> serialized_broken{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x2A_c, 0xFF_c,
        0x00_c,
        // clang-format on
    };

    std::vector<uint32_t> base_check_array_of(const tetengo::trie::storage& storage_)
    {
        const auto            size = storage_.base_check_size();
        std::vector<uint32_t> array_{};
        array_.reserve(size);
        for (auto i = static_cast<std::size_t>(0); i < size; ++i)
        {
            array_.push_back((storage_.base_at(i) << 8) | storage_.check_at(i));
        }
        return array_;
    }

    std::filesystem::path temporary_file_path(const std::vector<char>& initial_content = std::vector<char>{})
    {
        const auto path = std::filesystem::temp_directory_path() / "test_tetengo.trie.mmap_storage";

        {
            std::ofstream stream{ path, std::ios_base::binary };
            stream.write(std::data(initial_content), std::size(initial_content));
        }

        return path;
    }

    std::filesystem::path output_temporary_file_path()
    {
        const auto path = std::filesystem::temp_directory_path() / "test_tetengo.trie.mmap_storage.output";

        {
            std::ofstream stream{ path, std::ios_base::binary };
        }

        return path;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(mmap_storage)


BOOST_AUTO_TEST_CASE(default_value_cache_capacity)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::mmap_storage::default_value_cache_capacity() > 0U);
}

BOOST_AUTO_TEST_CASE(vacantCheckValue)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo_trie_storage_vacantCheckValue() == tetengo::trie::double_array::vacant_check_value());
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };
    }
    {
        const auto file_path = temporary_file_path(serialized);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        BOOST_CHECK_THROW(
            const tetengo::trie::mmap_storage storage(file_mapping, 0, file_size, std::move(deserializer)),
            std::invalid_argument);
    }
    {
        const auto file_path = temporary_file_path(serialized_broken);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        BOOST_CHECK_THROW(
            const tetengo::trie::mmap_storage storage(file_mapping, 0, file_size, std::move(deserializer)),
            std::ios_base::failure);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        BOOST_CHECK_THROW(
            const tetengo::trie::mmap_storage storage(file_mapping, file_size + 1, file_size, std::move(deserializer)),
            std::invalid_argument);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_storage);
    }
    {
        const auto file_path = temporary_file_path(serialized);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_storage =
            tetengo_trie_storage_createMmapStorage(std::filesystem::path{ "NONEXISTENT_FILE" }.c_str(), 0);
        BOOST_TEST(!p_storage);
    }
    {
        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(nullptr, 0);
        BOOST_TEST(!p_storage);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), file_size + 1);
        BOOST_TEST(!p_storage);
    }
}

BOOST_AUTO_TEST_CASE(base_check_size)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_TEST(storage.base_check_size() == 2U);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        BOOST_TEST(storage.base_check_size() == 2U);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_storage) == 17U);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_storage) == 17U);
    }
    {
        BOOST_TEST(tetengo_trie_storage_baseCheckSize(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_TEST(storage.base_at(0) == 42);
        BOOST_TEST(storage.base_at(1) == 0xFE);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        BOOST_TEST(storage.base_at(0) == 42);
        BOOST_TEST(storage.base_at(1) == 0xFE);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 0) == -74);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 1) == -115);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 2) == -106);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 3) == -93);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 4) == -104);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 5) == -105);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 6) == -109);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 7) == -103);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 8) == 9);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 9) == 0);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 10) == -86);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 11) == -97);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 12) == -84);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 13) == -96);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 14) == -82);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 15) == 16);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 16) == 1);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 0) == -74);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 1) == -115);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 2) == -106);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 3) == -93);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 4) == -104);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 5) == -105);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 6) == -109);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 7) == -103);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 8) == 9);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 9) == 0);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 10) == -86);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 11) == -97);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 12) == -84);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 13) == -96);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 14) == -82);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 15) == 16);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 16) == 1);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 5);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 0) == 42);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 1) == 0xFE);
    }
    {
        BOOST_TEST(tetengo_trie_storage_baseAt(nullptr, 0) == INT32_MAX);
    }
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_CHECK_THROW(storage.set_base_at(42, 4242), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(!tetengo_trie_storage_setBaseAt(p_storage, 42, 4242));
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(!tetengo_trie_storage_setBaseAt(p_storage, 42, 4242));
    }
    {
        BOOST_TEST(!tetengo_trie_storage_setBaseAt(nullptr, 42, 4242));
    }
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_TEST(storage.check_at(0) == tetengo::trie::double_array::vacant_check_value());
        BOOST_TEST(storage.check_at(1) == 24);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        BOOST_TEST(storage.check_at(0) == tetengo::trie::double_array::vacant_check_value());
        BOOST_TEST(storage.check_at(1) == 24);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 0) == tetengo_trie_storage_vacantCheckValue());
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 1) == 0x4B);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 2) == 0x75);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 3) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 4) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 5) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 6) == 0x6F);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 7) == 0x74);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 8) == 0x6F);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 9) == 0x00);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 10) == 0x54);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 11) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 12) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 13) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 14) == 0x6E);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 15) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 16) == 0x00);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 0) == tetengo_trie_storage_vacantCheckValue());
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 1) == 0x4B);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 2) == 0x75);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 3) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 4) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 5) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 6) == 0x6F);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 7) == 0x74);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 8) == 0x6F);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 9) == 0x00);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 10) == 0x54);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 11) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 12) == 0x6D);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 13) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 14) == 0x6E);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 15) == 0x61);
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 16) == 0x00);
    }
    {
        BOOST_TEST(tetengo_trie_storage_checkAt(nullptr, 0) == UINT8_MAX);
    }
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_CHECK_THROW(storage.set_check_at(24, 124), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(!tetengo_trie_storage_setCheckAt(p_storage, 24, 124));
    }
    {
        BOOST_TEST(!tetengo_trie_storage_setCheckAt(nullptr, 24, 124));
    }
}

BOOST_AUTO_TEST_CASE(value_count)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_TEST(storage.value_count() == 5U);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        BOOST_TEST(storage.value_count() == 5U);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_valueCount(p_storage) == 2U);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(tetengo_trie_storage_valueCount(p_storage) == 2U);
    }
    {
        BOOST_TEST(tetengo_trie_storage_valueCount(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(value_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_TEST(!storage.value_at(0));
        BOOST_TEST_REQUIRE(storage.value_at(1));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(1)) == 159U);
        BOOST_TEST_REQUIRE(storage.value_at(2));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(2)) == 14U);
        BOOST_TEST(!storage.value_at(3));
        BOOST_TEST_REQUIRE(storage.value_at(4));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(4)) == 3U);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        BOOST_TEST(!storage.value_at(0));
        BOOST_TEST_REQUIRE(storage.value_at(1));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(1)) == 159U);
        BOOST_TEST_REQUIRE(storage.value_at(2));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(2)) == 14U);
        BOOST_TEST(!storage.value_at(3));
        BOOST_TEST_REQUIRE(storage.value_at(4));
        BOOST_TEST(*std::any_cast<std::uint32_t>(storage.value_at(4)) == 3U);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            const auto* const p_value =
                static_cast<const std::vector<char>*>(tetengo_trie_storage_valueAt(p_storage, 0));
            BOOST_TEST_REQUIRE(p_value);
            const auto int_value = *reinterpret_cast<const int*>(std::data(*p_value));
            BOOST_TEST(int_value == 42);
        }
        {
            const auto* const p_value =
                static_cast<const std::vector<char>*>(tetengo_trie_storage_valueAt(p_storage, 1));
            BOOST_TEST_REQUIRE(p_value);
            const auto int_value = *reinterpret_cast<const int*>(std::data(*p_value));
            BOOST_TEST(int_value == 24);
        }
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        {
            const auto* const p_value =
                static_cast<const std::vector<char>*>(tetengo_trie_storage_valueAt(p_storage, 0));
            BOOST_TEST_REQUIRE(p_value);
            const auto int_value = *reinterpret_cast<const int*>(std::data(*p_value));
            BOOST_TEST(int_value == 42);
        }
        {
            const auto* const p_value =
                static_cast<const std::vector<char>*>(tetengo_trie_storage_valueAt(p_storage, 1));
            BOOST_TEST_REQUIRE(p_value);
            const auto int_value = *reinterpret_cast<const int*>(std::data(*p_value));
            BOOST_TEST(int_value == 24);
        }
    }
    {
        BOOST_TEST(!tetengo_trie_storage_valueAt(nullptr, 0));
    }
}

BOOST_AUTO_TEST_CASE(add_value_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_CHECK_THROW(storage.add_value_at(24, 124), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto value = 124;
        BOOST_TEST(!tetengo_trie_storage_addValueAt(p_storage, 24, &value, sizeof(int)));
    }
    {
        const auto value = 124;
        BOOST_TEST(!tetengo_trie_storage_addValueAt(nullptr, 24, &value, sizeof(int)));
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_TEST(!tetengo_trie_storage_addValueAt(p_storage, 24, nullptr, 0));
    }
}

BOOST_AUTO_TEST_CASE(filling_rate)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_for_calculating_filling_rate);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        BOOST_CHECK_CLOSE(storage.filling_rate(), 1.0 / 2.0, 0.1);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_for_calculating_filling_rate);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        BOOST_CHECK_CLOSE(tetengo_trie_storage_fillingRate(p_storage), 1.0 / 2.0, 0.1);
    }
    {
        BOOST_TEST(std::isnan(tetengo_trie_storage_fillingRate(nullptr)));
    }
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        std::ostringstream                    output_stream{};
        const tetengo::trie::value_serializer serializer{
            [](const std::any& object) {
                static const tetengo::trie::default_serializer<std::uint32_t> uint32_serializer{ false };
                const auto serialized = uint32_serializer(std::any_cast<std::uint32_t>(object));
                return std::vector<char>{ std::begin(serialized), std::end(serialized) };
            },
            sizeof(std::uint32_t)
        };
        BOOST_CHECK_THROW(storage.serialize(output_stream, serializer), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto output_file_path = output_temporary_file_path();
        BOOST_SCOPE_EXIT(&output_file_path)
        {
            std::filesystem::remove(output_file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto result = tetengo_trie_storage_serialize(p_storage, output_file_path.c_str(), sizeof(int));
        BOOST_TEST(!result);
    }
    {
        const auto output_file_path = output_temporary_file_path();
        BOOST_SCOPE_EXIT(&output_file_path)
        {
            std::filesystem::remove(output_file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto result = tetengo_trie_storage_serialize(nullptr, output_file_path.c_str(), sizeof(int));
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 0, file_size, std::move(deserializer) };

        const auto p_clone = storage.clone();
        BOOST_REQUIRE(p_clone);
        BOOST_TEST(base_check_array_of(*p_clone) == base_check_array_of(storage));
        BOOST_TEST(p_clone->value_count() == storage.value_count());
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const boost::interprocess::file_mapping file_mapping{ file_path.c_str(), boost::interprocess::read_only };
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(file_path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::uint32_t>uint32_deserializer{ false };
            return uint32_deserializer(serialized);
        } };
        const tetengo::trie::mmap_storage storage{ file_mapping, 5, file_size, std::move(deserializer) };

        const auto p_clone = storage.clone();
        BOOST_REQUIRE(p_clone);
        BOOST_TEST(base_check_array_of(*p_clone) == base_check_array_of(storage));
        BOOST_TEST(p_clone->value_count() == storage.value_count());
    }

    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 0);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto* const p_clone = tetengo_trie_storage_clone(p_storage);
        BOOST_SCOPE_EXIT(p_clone)
        {
            tetengo_trie_storage_destroy(p_clone);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_clone);
        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_clone) == tetengo_trie_storage_baseCheckSize(p_storage));
        BOOST_TEST(tetengo_trie_storage_valueCount(p_clone) == tetengo_trie_storage_valueCount(p_storage));
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_storage_createMmapStorage(file_path.c_str(), 7);
        BOOST_SCOPE_EXIT(p_storage)
        {
            tetengo_trie_storage_destroy(p_storage);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_storage);

        const auto* const p_clone = tetengo_trie_storage_clone(p_storage);
        BOOST_SCOPE_EXIT(p_clone)
        {
            tetengo_trie_storage_destroy(p_clone);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_clone);
        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_clone) == tetengo_trie_storage_baseCheckSize(p_storage));
        BOOST_TEST(tetengo_trie_storage_valueCount(p_clone) == tetengo_trie_storage_valueCount(p_storage));
    }
    {
        BOOST_TEST(!tetengo_trie_storage_clone(nullptr));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
