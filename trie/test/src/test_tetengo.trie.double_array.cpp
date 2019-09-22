/*!
    \brief A double array.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.h>
#include <tetengo/trie/double_array.hpp>

struct tetengo_trie_doublearray;


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(make_base_check)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::int32_t base = 42;
        const std::int8_t  check = 24;

        const std::uint32_t base_check = tetengo::trie::double_array::make_base_check(base, check);

        BOOST_TEST(base_check == 0x00002A18U);
    }
    {
        // TODO: C style API
    }

    {
        const std::int32_t base = -42;
        const std::int8_t  check = -24;

        const std::uint32_t base_check = tetengo::trie::double_array::make_base_check(base, check);

        BOOST_TEST(base_check == 0xFFFFD6E8U);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(base_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::uint32_t base_check = 0x00002A18;

        const std::int32_t base = tetengo::trie::double_array::base_of(base_check);

        BOOST_TEST(base == 42);
    }
    {
        // TODO: C style API
    }

    {
        const std::uint32_t base_check = 0xFFFFD6E8;

        const std::int32_t base = tetengo::trie::double_array::base_of(base_check);

        BOOST_TEST(base == -42);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(check_of)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::uint32_t base_check = 0x00002A18;

        const std::int32_t check = tetengo::trie::double_array::check_of(base_check);

        BOOST_TEST(check == 24);
    }
    {
        // TODO: C style API
    }

    {
        const std::uint32_t base_check = 0xFFFFD6E8;

        const std::int32_t check = tetengo::trie::double_array::check_of(base_check);

        BOOST_TEST(check == -24);
    }
    {
        // TODO: C style API
    }
}

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
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
