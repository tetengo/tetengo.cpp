/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <iterator>
#include <string>
#include <string_view>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/stringView.h>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::string key_mizuho{ 0xE3_c, 0x81_c, 0xBF_c, 0xE3_c, 0x81_c, 0x9A_c, 0xE3_c, 0x81_c, 0xBB_c };

    const std::string surface_mizuho{ 0xE7_c, 0x91_c, 0x9E_c, 0xE7_c, 0xA9_c, 0x82_c };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(entry)


BOOST_AUTO_TEST_CASE(bos_eos)
{
    BOOST_TEST_PASSPOINT();

    {
        static auto& bos_eos_ = tetengo::lattice::entry::bos_eos();

        BOOST_TEST(std::empty(bos_eos_.key()));
        BOOST_TEST(!bos_eos_.value().has_value());
        BOOST_TEST(bos_eos_.cost() == 0);
    }

    {
        static auto* const p_bos_eos = tetengo_lattice_entryView_bosEos();

        BOOST_TEST_REQUIRE(p_bos_eos->key_handle);
        const auto* const p_key = tetengo_lattice_entryView_createKeyOf(p_bos_eos->key_handle);
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_temp_freeStringView(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_key);
        BOOST_TEST(p_key->length == 0U);
        BOOST_TEST(!tetengo_lattice_entryView_valueOf(p_bos_eos->value_handle));
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
        [[maybe_unused]] const tetengo_lattice_entry_t entry{
            reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(&key_mizuho), &surface_mizuho, 42
        };
    }
    {
        const std::string_view                             key_view{ key_mizuho };
        const std::any                                     value{ reinterpret_cast<const void*>(&surface_mizuho) };
        [[maybe_unused]] const tetengo_lattice_entryView_t entry{
            reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&key_view),
            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
            42
        };
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
        const tetengo_lattice_entry_t entry{ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(&key_mizuho),
                                             &surface_mizuho,
                                             42 };

        const auto* const p_entry_key = tetengo_lattice_entry_createKeyOf(entry.key_handle);
        BOOST_SCOPE_EXIT(p_entry_key)
        {
            tetengo_lattice_temp_freeStringView(p_entry_key);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_entry_key);
        BOOST_TEST(p_entry_key->p_head == key_mizuho.c_str());
        BOOST_TEST(p_entry_key->length == key_mizuho.length());
    }
    {
        const std::string_view            key_view{ key_mizuho };
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&key_view),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
                                                 42 };

        const auto* const p_entry_key = tetengo_lattice_entryView_createKeyOf(entry.key_handle);
        BOOST_SCOPE_EXIT(p_entry_key)
        {
            tetengo_lattice_temp_freeStringView(p_entry_key);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_entry_key);
        BOOST_TEST(p_entry_key->p_head == key_mizuho.c_str());
        BOOST_TEST(p_entry_key->length == key_mizuho.length());
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
        const tetengo_lattice_entry_t entry{ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(&key_mizuho),
                                             &surface_mizuho,
                                             42 };

        BOOST_TEST(reinterpret_cast<const std::string*>(entry.p_value) == &surface_mizuho);
    }
    {
        const std::string_view            key_view{ key_mizuho };
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&key_view),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
                                                 42 };

        BOOST_TEST(
            reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(entry.value_handle)) ==
            &surface_mizuho);
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
        const tetengo_lattice_entry_t entry{ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(&key_mizuho),
                                             &surface_mizuho,
                                             42 };

        BOOST_TEST(entry.cost == 42);
    }
    {
        const std::string_view            key_view{ key_mizuho };
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&key_view),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
                                                 42 };

        BOOST_TEST(entry.cost == 42);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
