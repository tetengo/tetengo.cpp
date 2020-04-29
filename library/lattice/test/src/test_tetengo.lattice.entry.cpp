/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cstddef>
#include <string>

#include <boost/core/ignore_unused.hpp>
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/string_view.h>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string key_mizuho{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                  to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

    const std::string surface_mizuho{ to_c(0xE7), to_c(0x91), to_c(0x9E), to_c(0xE7), to_c(0xA9), to_c(0x82) };

    const std::string key_tsubame{ to_c(0xE3), to_c(0x81), to_c(0xA4), to_c(0xE3), to_c(0x81),
                                   to_c(0xB0), to_c(0xE3), to_c(0x82), to_c(0x81) };

    const std::string surface_tsubame{ to_c(0xE7), to_c(0x87), to_c(0x95) };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(entry)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry      entry_{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry_view view_{ entry_ };
    }

    {
        const tetengo_lattice_entry entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };
        boost::ignore_unused(entry_);
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ key_mizuho, surface_mizuho, 42 };

        BOOST_TEST(entry_.key() == key_mizuho);
    }

    {
        const tetengo_lattice_entry entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

        BOOST_TEST(entry_.key.p_head == key_mizuho.c_str());
        BOOST_TEST(entry_.key.length == key_mizuho.length());
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ key_mizuho, surface_mizuho, 42 };

        BOOST_TEST(std::any_cast<std::string>(entry_.value()) == surface_mizuho);
    }

    {
        const tetengo_lattice_entry entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

        BOOST_TEST(reinterpret_cast<const std::string*>(entry_.p_value) == &surface_mizuho);
    }
}

BOOST_AUTO_TEST_CASE(cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ key_mizuho, surface_mizuho, 42 };

        BOOST_TEST(entry_.cost() == 42);
    }

    {
        const tetengo_lattice_entry entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

        BOOST_TEST(entry_.cost == 42);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry1{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry entry2{ key_mizuho, surface_mizuho, 42 };

        BOOST_CHECK(entry1 == entry2);
    }
    {
        const tetengo::lattice::entry entry1{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry entry2{ key_tsubame, surface_tsubame, 24 };

        BOOST_CHECK(entry1 != entry2);
    }
}

BOOST_AUTO_TEST_CASE(operator_hash)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry1{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry entry2{ key_mizuho, surface_mizuho, 42 };

        BOOST_TEST(std::hash<tetengo::lattice::entry>{}(entry1) == std::hash<tetengo::lattice::entry>{}(entry2));
    }
    {
        const tetengo::lattice::entry entry1{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry entry2{ key_tsubame, surface_tsubame, 24 };

        BOOST_TEST(std::hash<tetengo::lattice::entry>{}(entry1) != std::hash<tetengo::lattice::entry>{}(entry2));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
