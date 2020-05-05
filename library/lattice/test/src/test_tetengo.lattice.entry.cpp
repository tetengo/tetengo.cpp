/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <string>

#include <boost/core/ignore_unused.hpp>
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


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(entry)


BOOST_AUTO_TEST_CASE(bos_eos)
{
    BOOST_TEST_PASSPOINT();

    {
        static auto& bos_eos_ = tetengo::lattice::entry::bos_eos();

        BOOST_TEST(bos_eos_.key().empty());
        BOOST_TEST(!bos_eos_.value().has_value());
        BOOST_TEST(bos_eos_.cost() == 0);
    }

    {
        static auto* const p_bos_eos = tetengo_lattice_entry_bosEos();

        BOOST_TEST(!p_bos_eos->key.p_head);
        BOOST_TEST(p_bos_eos->key.length == 0U);
        BOOST_TEST(!p_bos_eos->p_value);
        BOOST_TEST(p_bos_eos->cost == 0);
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry      entry1{ key_mizuho, surface_mizuho, 42 };
        const tetengo::lattice::entry_view view{ entry1 };
        const tetengo::lattice::entry      entry2{ view };
    }

    {
        const tetengo_lattice_entry_t entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };
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
        const tetengo_lattice_entry_t entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

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
        const tetengo_lattice_entry_t entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

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
        const tetengo_lattice_entry_t entry_{ { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 };

        BOOST_TEST(entry_.cost == 42);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
