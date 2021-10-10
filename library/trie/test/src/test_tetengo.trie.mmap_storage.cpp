/*! \file
    \brief An mmap storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <climits>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/storage.h>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

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
        // clang-format on
    };

    // const std::vector<uint32_t> base_check_array{ 0x00002AFF, 0x0000FE18 };

    // const std::vector<char> serialized_broken{
    //    // clang-format off
    //    0x00_c, 0x00_c, 0x00_c, 0x02_c,
    //    0x01_c, 0x23_c, 0x45_c, 0x67_c,
    //    0x89_c,
    //    // clang-format on
    //};

    std::filesystem::path temporary_file_path(const std::vector<char>& initial_content = std::vector<char>{})
    {
        const auto path = std::filesystem::temp_directory_path() / "test_tetengo.trie.mmap_storage";

        {
            std::ofstream stream{ path, std::ios_base::binary };
            stream.write(std::data(initial_content), std::size(initial_content));
        }

        return path;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(mmap_storage)


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

        const tetengo::trie::mmap_storage storage{ file_path, 0 };
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const tetengo::trie::mmap_storage storage{ file_path, 5 };
    }
    {
        BOOST_CHECK_THROW(const tetengo::trie::mmap_storage storage("NONEXISTENT_FILE", 0), std::ios_base::failure);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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
        BOOST_TEST(p_storage);
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

        const tetengo::trie::mmap_storage storage{ file_path, 0 };

        BOOST_TEST(storage.base_check_size() == 2U);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const tetengo::trie::mmap_storage storage{ file_path, 5 };

        BOOST_TEST(storage.base_check_size() == 2U);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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

        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_storage) == 2U);
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

        BOOST_TEST(tetengo_trie_storage_baseCheckSize(p_storage) == 2U);
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

        const tetengo::trie::mmap_storage storage{ file_path, 0 };

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

        const tetengo::trie::mmap_storage storage{ file_path, 5 };

        BOOST_TEST(storage.base_at(0) == 42);
        BOOST_TEST(storage.base_at(1) == 0xFE);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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

        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 0) == 42);
        BOOST_TEST(tetengo_trie_storage_baseAt(p_storage, 1) == 0xFE);
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
        BOOST_TEST(tetengo_trie_storage_baseAt(nullptr, 0) == INT_MAX);
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

        tetengo::trie::mmap_storage storage{ file_path, 0 };

        BOOST_CHECK_THROW(storage.set_base_at(42, 4242), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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

        const tetengo::trie::mmap_storage storage{ file_path, 0 };

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

        const tetengo::trie::mmap_storage storage{ file_path, 5 };

        BOOST_TEST(storage.check_at(0) == tetengo::trie::double_array::vacant_check_value());
        BOOST_TEST(storage.check_at(1) == 24);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 1) == 24);
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

        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 0) == tetengo_trie_storage_vacantCheckValue());
        BOOST_TEST(tetengo_trie_storage_checkAt(p_storage, 1) == 24);
    }
    {
        BOOST_TEST(tetengo_trie_storage_checkAt(nullptr, 0) == UCHAR_MAX);
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

        tetengo::trie::mmap_storage storage{ file_path, 0 };

        BOOST_CHECK_THROW(storage.set_check_at(24, 124), std::logic_error);
    }

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
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

BOOST_AUTO_TEST_CASE(value_size)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const tetengo::trie::mmap_storage storage{ file_path, 0 };

        BOOST_TEST(storage.value_size() == 5U);
    }
    {
        const auto file_path = temporary_file_path(serialized_fixed_value_size_with_header);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        const tetengo::trie::mmap_storage storage{ file_path, 5 };

        BOOST_TEST(storage.value_size() == 5U);
    }
}

BOOST_AUTO_TEST_CASE(value_at)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(add_value_at)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
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

        const tetengo::trie::mmap_storage storage{ file_path, 0 };

        BOOST_CHECK_CLOSE(storage.filling_rate(), 1.0 / 2.0, 0.1);
    }
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
