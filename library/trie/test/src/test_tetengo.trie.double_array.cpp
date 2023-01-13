/*! \file
    \brief A double array.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/double_array_iterator.hpp>
#include <tetengo/trie/storage.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::vector<std::uint32_t> expected_empty_base_check_array_empty{
        //                  BASE  CHECK  BYTECHECK
        0x000000FF, // [ 0]    0,    -1,        -1
    };

    /*
              \0
        [ 0]+---[ 1]
            |
            |' '      \0
            +---[ 2]----[ 3]
    */

    const std::vector<std::pair<std::string, std::int32_t>> expected_values0{ { "", 42 }, { " ", 24 } };

    const std::vector<std::uint32_t> expected_base_check_array0{
        //                  BASE  CHECK  BYTECHECK
        0x000001FF, // [ 0]    1,    -1,        -1
        0x00002A00, // [ 1]   42,     0,         0
        0x000000FF, // [ 2]    0,    -1,        -1
        0x000000FF, // [ 3]    0,    -1,        -1
        0x000000FF, // [ 4]    0,    -1,        -1
        0x000000FF, // [ 5]    0,    -1,        -1
        0x000000FF, // [ 6]    0,    -1,        -1
        0x000000FF, // [ 7]    0,    -1,        -1
        0x000000FF, // [ 8]    0,    -1,        -1
        0x000000FF, // [ 9]    0,    -1,        -1
        0x000000FF, // [10]    0,    -1,        -1
        0x000000FF, // [11]    0,    -1,        -1
        0x000000FF, // [12]    0,    -1,        -1
        0x000000FF, // [13]    0,    -1,        -1
        0x000000FF, // [14]    0,    -1,        -1
        0x000000FF, // [15]    0,    -1,        -1
        0x000000FF, // [16]    0,    -1,        -1
        0x000000FF, // [17]    0,    -1,        -1
        0x000000FF, // [18]    0,    -1,        -1
        0x000000FF, // [19]    0,    -1,        -1
        0x000000FF, // [20]    0,    -1,        -1
        0x000000FF, // [21]    0,    -1,        -1
        0x000000FF, // [22]    0,    -1,        -1
        0x000000FF, // [23]    0,    -1,        -1
        0x000000FF, // [24]    0,    -1,        -1
        0x000000FF, // [25]    0,    -1,        -1
        0x000000FF, // [26]    0,    -1,        -1
        0x000000FF, // [27]    0,    -1,        -1
        0x000000FF, // [28]    0,    -1,        -1
        0x000000FF, // [29]    0,    -1,        -1
        0x000000FF, // [30]    0,    -1,        -1
        0x000000FF, // [31]    0,    -1,        -1
        0x000000FF, // [32]    0,    -1,        -1
        0x00002220, // [33]   34,     0,        32
        0x00001800, // [34]   24,    33,         0
    };

    /*
              S       E       T       A       \0
        [ 0]+---[ 1]----[ 2]----[ 4]----[ 5]----[ 6]
            |
            | U       T       I       G       O       S       I       \0
            +---[ 3]----[ 7]+---[ 8]----[ 9]----[10]----[11]----[12]----[13]
                            |
                            | O       \0
                            +---[14]----[15]
    */

    const std::vector<std::pair<std::string, std::int32_t>> expected_values3{ { "UTIGOSI", 24 },
                                                                              { "UTO", 2424 },
                                                                              { "SETA", 42 } };

    const std::vector<std::uint32_t> expected_base_check_array3{
        //                  BASE  CHECK  BYTECHECK
        0xFFFFAEFF, // [ 0]  -82,    -1,        -1
        0xFFFFBD53, // [ 1]  -67,     0,        83
        0xFFFFB045, // [ 2]  -80,     1,        69
        0xFFFFB355, // [ 3]  -77,     0,        85
        0xFFFFC454, // [ 4]  -60,     2,        84
        0x00000641, // [ 5]    6,     4,        65
        0x00002A00, // [ 6]   42,     5,         0
        0xFFFFBF54, // [ 7]  -65,     3,        84
        0xFFFFC249, // [ 8]  -62,     7,        73
        0xFFFFBB47, // [ 9]  -69,     8,        71
        0xFFFFB84F, // [10]  -72,     9,        79
        0xFFFFC353, // [11]  -61,    10,        83
        0x00000D49, // [12]   13,    11,        73
        0x00001800, // [13]   24,    12,         0
        0x00000F4F, // [14]   15,     7,        79
        0x00097800, // [15] 2424,    14,         0
    };

    /*
            0xE8    0xB5    0xA4    0xE7    0x80    0xAC      \0
        [ 0]----[ 1]----[ 2]----[ 3]+---[ 5]----[ 9]----[10]----[11]
                                    |
                                    |0xE6   0xB0    0xB4      \0
                                    +---[ 4]----[ 6]----[ 7]----[ 8]
    */

    const std::vector<std::pair<std::string, std::int32_t>> expected_values4{
        { { 0xE8_c, 0xB5_c, 0xA4_c, 0xE7_c, 0x80_c, 0xAC_c }, 24 }, // "Akase" in Kanji
        { { 0xE8_c, 0xB5_c, 0xA4_c, 0xE6_c, 0xB0_c, 0xB4_c }, 42 }, // "Akamizu" in Kanji
    };

    const std::vector<std::uint32_t> expected_base_check_array4{
        //                  BASE  CHECK  BYTECHECK
        0xffff19ff, // [ 0] -231,    -1,        -1
        0xffff4de8, // [ 1] -179,     0,       232
        0xffff5fb5, // [ 2] -161,     1,       181
        0xffff1ea4, // [ 3] -226,     2,       164
        0xffff56e6, // [ 4] -170,     3,       230
        0xffff89e7, // [ 5] -119,     3,       231
        0xffff53b0, // [ 6] -173,     4,       176
        0x000008b4, // [ 7]    8,     6,       180
        0x00002a00, // [ 8]   42,     7,         0
        0xffff5e80, // [ 9] -162,     5,       128
        0x00000bac, // [10]   11,     9,       172
        0x00001800, // [11]   24,    10,         0
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

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(null_building_observer_set)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& observer_set = tetengo::trie::double_array::null_building_observer_set();
}

BOOST_AUTO_TEST_CASE(default_density_factor)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::default_density_factor() > 0U);
}

BOOST_AUTO_TEST_CASE(key_terminator)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::key_terminator() == '\0');
}

BOOST_AUTO_TEST_CASE(vacant_check_value)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::vacant_check_value() == 0xFF);
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        BOOST_TEST(base_check_array_of(double_array_.get_storage()) == expected_empty_base_check_array_empty);
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values0 };

        BOOST_TEST(base_check_array_of(double_array_.get_storage()) == expected_base_check_array0);
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        BOOST_TEST(base_check_array_of(double_array_.get_storage()) == expected_base_check_array3);
    }
    {
        std::list<std::pair<std::string, std::int32_t>> expected_values3_as_list{ std::begin(expected_values3),
                                                                                  std::end(expected_values3) };

        const tetengo::trie::double_array double_array_{ std::begin(expected_values3_as_list),
                                                         std::end(expected_values3_as_list) };

        BOOST_TEST(base_check_array_of(double_array_.get_storage()) == expected_base_check_array3);
    }
    {
        const tetengo::trie::double_array double_array0{ expected_values3 };
        const auto&                       storage = double_array0.get_storage();

        const tetengo::trie::double_array double_array1{ storage.clone(), 0 };

        BOOST_TEST(base_check_array_of(double_array1.get_storage()) == expected_base_check_array3);
    }
    {
        const tetengo::trie::double_array double_array0{ expected_values3 };
        const auto&                       storage = double_array0.get_storage();

        const tetengo::trie::double_array double_array1{ storage.clone(), 8 };

        BOOST_TEST(base_check_array_of(double_array1.get_storage()) == expected_base_check_array3);

        const auto o_found = double_array1.find("GOSI");
        BOOST_REQUIRE(o_found);
        BOOST_TEST(*o_found == 24);
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        BOOST_TEST(base_check_array_of(double_array_.get_storage()) == expected_base_check_array4);
    }
    {
        BOOST_CHECK_THROW(
            const tetengo::trie::double_array double_array_(
                expected_values0, tetengo::trie::double_array::null_building_observer_set(), 0),
            std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        {
            const auto o_found = double_array_.find("SETA");
            BOOST_CHECK(!o_found);
        }
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        {
            const auto o_found = double_array_.find("SETA");
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 42);
        }
        {
            const auto o_found = double_array_.find("UTIGOSI");
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 24);
        }
        {
            const auto o_found = double_array_.find("UTO");
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 2424);
        }
        {
            const auto o_found = double_array_.find("SUIZENJI");
            BOOST_CHECK(!o_found);
        }
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        {
            const auto o_found = double_array_.find(std::string{ 0xE8_c, 0xB5_c, 0xA4_c, 0xE7_c, 0x80_c, 0xAC_c });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 24);
        }
        {
            const auto o_found = double_array_.find(std::string{ 0xE8_c, 0xB5_c, 0xA4_c, 0xE6_c, 0xB0_c, 0xB4_c });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 42);
        }
        {
            // "Suizenji" in Kanji
            const std::string key{ { 0xE6_c, 0xB0_c, 0xB4_c, 0xE5_c, 0x89_c, 0x8D_c, 0xE5_c, 0xAF_c, 0xBA_c } };
            const auto        o_found = double_array_.find(key);
            BOOST_CHECK(!o_found);
        }
    }
}

BOOST_AUTO_TEST_CASE(begin_end)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        const auto first = std::begin(double_array_);
        const auto last = std::end(double_array_);
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto first = std::begin(double_array_);
        const auto last = std::end(double_array_);
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        const auto first = std::begin(double_array_);
        const auto last = std::end(double_array_);
    }
}

BOOST_AUTO_TEST_CASE(subtrie)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        {
            const auto o_subtrie = double_array_.subtrie("U");
            BOOST_CHECK(o_subtrie);
            {
                const auto o_found = o_subtrie->find("TIGOSI");
                BOOST_REQUIRE(o_found);
                BOOST_TEST(*o_found == 24);
            }
            {
                const auto o_found = o_subtrie->find("TO");
                BOOST_REQUIRE(o_found);
                BOOST_TEST(*o_found == 2424);
            }
            {
                const auto o_found = o_subtrie->find("SETA");
                BOOST_CHECK(!o_found);
            }
            {
                const auto o_found = o_subtrie->find("UTIGOSI");
                BOOST_CHECK(!o_found);
            }
            {
                auto iterator = std::begin(*o_subtrie);

                BOOST_TEST(*iterator == 24);

                ++iterator;
                BOOST_TEST(*iterator == 2424);

                ++iterator;
                BOOST_CHECK(iterator == std::end(*o_subtrie));
            }

            const auto o_subtrie2 = o_subtrie->subtrie("TI");
            {
                const auto o_found = o_subtrie2->find("GOSI");
                BOOST_REQUIRE(o_found);
                BOOST_TEST(*o_found == 24);
            }
        }
        {
            const auto o_subtrie = double_array_.subtrie("T");
            BOOST_CHECK(!o_subtrie);
        }
    }
    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        const auto o_subtrie = double_array_.subtrie(std::string{ 0xE8_c, 0xB5_c, 0xA4_c, 0xE6_c });
        BOOST_CHECK(o_subtrie);
        {
            const auto o_found = o_subtrie->find(std::string{ 0xB0_c, 0xB4_c });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 42);
        }
    }
}

BOOST_AUTO_TEST_CASE(storage)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto base_check_array = base_check_array_of(double_array_.get_storage());

        BOOST_TEST(base_check_array == expected_base_check_array3);
    }
    {
        tetengo::trie::double_array double_array_{ expected_values3 };

        const auto base_check_array = base_check_array_of(double_array_.get_storage());

        BOOST_TEST(base_check_array == expected_base_check_array3);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
