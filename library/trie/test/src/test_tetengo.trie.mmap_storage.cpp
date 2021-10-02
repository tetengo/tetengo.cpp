/*! \file
    \brief A mmap storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/mmap_storage.hpp>


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

    const std::vector<char> serialized_fixed_value_size_with_header{
        // clang-format off
        
        // header
        0x01_c, 0x23_c, 0x45_c, 0x67_c, 0x89_c,

        // constent
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

    // const std::vector<uint32_t> base_check_array{ 0x00002AFF, 0x0000FE18 };

    // const std::vector<char> serialized_broken{
    //    // clang-format off
    //    nul_byte(), nul_byte(), nul_byte(), 0x02_c,
    //    0x01_c,     0x23_c,     0x45_c,     0x67_c,
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
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    const auto file_path = temporary_file_path(serialized_fixed_value_size);
    BOOST_SCOPE_EXIT(&file_path)
    {
        std::filesystem::remove(file_path);
    }
    BOOST_SCOPE_EXIT_END;

    tetengo::trie::mmap_storage storage{ file_path, 0 };

    BOOST_CHECK_THROW(storage.set_base_at(42, 4242), std::logic_error);
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
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    const auto file_path = temporary_file_path(serialized_fixed_value_size);
    BOOST_SCOPE_EXIT(&file_path)
    {
        std::filesystem::remove(file_path);
    }
    BOOST_SCOPE_EXIT_END;

    tetengo::trie::mmap_storage storage{ file_path, 0 };

    BOOST_CHECK_THROW(storage.set_base_at(24, 124), std::logic_error);
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(filling_rate)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(base_check_array)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
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
