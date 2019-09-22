/*!
    \brief A double array.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.h>
#include <tetengo/trie/double_array.hpp>

struct tetengo_trie_doublearray;


namespace
{
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

    const std::initializer_list<std::pair<std::string, std::int32_t>> expected_values{ { "UTIGOSI", 24 },
                                                                                       { "UTO", 2424 },
                                                                                       { "SETA", 42 } };

    const std::vector<std::uint32_t> expected_empty_base_check_array{
        //                  BASE  CHECK  BYTECHECK
        0x000000FF, // [ 0]    0,    -1,        -1
    };

    const std::vector<std::uint32_t> expected_base_check_array{
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

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
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
        const tetengo::trie::double_array double_array_{ expected_values };
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(base_check_array)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        const auto& base_check_array = double_array_.base_check_array();

        BOOST_TEST(base_check_array == expected_empty_base_check_array);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values };

        const auto& base_check_array = double_array_.base_check_array();

        BOOST_TEST(base_check_array == expected_base_check_array);
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
