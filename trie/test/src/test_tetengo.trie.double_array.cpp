/*!
    \brief A double array.

    Copyright (C) 2019 kaoru
 */

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.h>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/double_array_enumerator.hpp>
#include <tetengo/trie/storage.hpp>

struct tetengo_trie_doublearray;


namespace
{
    constexpr char to_c(const unsigned char uc)
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
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC) }, 24 }, // "Akase" in Kanji
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4) }, 42 }, // "Akamizu" in Kanji
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

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(null_building_observer_set)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::double_array::null_building_observer_set();

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(default_density_factor)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::default_density_factor() > 0U);

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(key_terminator)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::key_terminator() == '\0');

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(vacant_check_value)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::vacant_check_value() == 0xFF);

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        BOOST_TEST(double_array_.get_storage().base_check_array() == expected_empty_base_check_array_empty);
    }
    {
        tetengo_trie_doublearray* const p_double_array = tetengo_trie_doublearray_create();
        BOOST_SCOPE_EXIT(p_double_array)
        {
            tetengo_trie_doublearray_destroy(p_double_array);
        }
        BOOST_SCOPE_EXIT_END;
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values0 };

        BOOST_TEST(double_array_.get_storage().base_check_array() == expected_base_check_array0);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        BOOST_TEST(double_array_.get_storage().base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        std::list<std::pair<std::string, std::int32_t>> expected_values3_as_list{ std::begin(expected_values3),
                                                                                  std::end(expected_values3) };

        const tetengo::trie::double_array double_array_{ std::begin(expected_values3_as_list),
                                                         std::end(expected_values3_as_list) };

        BOOST_TEST(double_array_.get_storage().base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array0{ expected_values3 };
        const auto&                       storage = double_array0.get_storage();

        const tetengo::trie::double_array double_array1{ storage.clone(), 0 };

        BOOST_TEST(double_array1.get_storage().base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array0{ expected_values3 };
        const auto&                       storage = double_array0.get_storage();

        const tetengo::trie::double_array double_array1{ storage.clone(), 8 };

        BOOST_TEST(double_array1.get_storage().base_check_array() == expected_base_check_array3);

        const auto o_found = double_array1.find("GOSI");
        BOOST_REQUIRE(o_found);
        BOOST_TEST(*o_found == 24);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        BOOST_TEST(double_array_.get_storage().base_check_array() == expected_base_check_array4);
    }
    {
        // TODO: C style API
    }

    {
        BOOST_CHECK_THROW(
            const tetengo::trie::double_array double_array_(
                expected_values0, tetengo::trie::double_array::null_building_observer_set(), 0),
            std::invalid_argument);
    }
    {
        // TODO: C style API
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
        // TODO: C style API
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
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        {
            const auto o_found = double_array_.find(
                std::string{ to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC) });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 24);
        }
        {
            const auto o_found = double_array_.find(
                std::string{ to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4) });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 42);
        }
        {
            const std::string key{ { to_c(0xE6),
                                     to_c(0xB0),
                                     to_c(0xB4),
                                     to_c(0xE5),
                                     to_c(0x89),
                                     to_c(0x8D),
                                     to_c(0xE5),
                                     to_c(0xAF),
                                     to_c(0xBA) } }; // "Suizenji" in Kanji
            const auto        o_found = double_array_.find(key);
            BOOST_CHECK(!o_found);
        }
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(get_enumerator)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        const auto enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        const auto enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
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
                auto enumerator = o_subtrie->get_enumerator();

                BOOST_TEST(enumerator->first == "TIGOSI");
                BOOST_TEST(enumerator->second == 24);

                ++enumerator;
                BOOST_TEST(enumerator->first == "TO");
                BOOST_TEST(enumerator->second == 2424);

                ++enumerator;
                BOOST_CHECK(enumerator == tetengo::trie::double_array_enumerator{});
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
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values4 };

        const auto o_subtrie = double_array_.subtrie(std::string{ to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6) });
        BOOST_CHECK(o_subtrie);
        {
            const auto o_found = o_subtrie->find(std::string{ to_c(0xB0), to_c(0xB4) });
            BOOST_REQUIRE(o_found);
            BOOST_TEST(*o_found == 42);
        }
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(storage)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto& base_check_array = double_array_.get_storage().base_check_array();

        BOOST_TEST(base_check_array == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        tetengo::trie::double_array double_array_{ expected_values3 };

        const auto& base_check_array = double_array_.get_storage().base_check_array();

        BOOST_TEST(base_check_array == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
