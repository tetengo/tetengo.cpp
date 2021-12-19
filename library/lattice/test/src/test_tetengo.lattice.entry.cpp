/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/string_input.hpp>


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

        BOOST_TEST(!bos_eos_.p_key());
        BOOST_TEST(!bos_eos_.value().has_value());
        BOOST_TEST(bos_eos_.cost() == 0);
    }

    {
        static auto* const p_bos_eos = tetengo_lattice_entryView_bosEos();

        const auto* const p_key = tetengo_lattice_entryView_createKeyOf(p_bos_eos->key_handle);
        BOOST_TEST(!p_key);
        BOOST_TEST(!tetengo_lattice_entryView_valueOf(p_bos_eos->value_handle));
        BOOST_TEST(p_bos_eos->cost == 0);
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry      entry1{ std::make_unique<tetengo::lattice::string_input>(key_mizuho),
                                              surface_mizuho,
                                              42 };
        const tetengo::lattice::entry_view view{ entry1 };
        const tetengo::lattice::entry      entry2{ view };
        tetengo::lattice::entry            entry3{ entry2 };
        const tetengo::lattice::entry      entry4{ std::move(entry3) };
    }

    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        [[maybe_unused]] const tetengo_lattice_entry_t entry{ tetengo_lattice_entry_createKeyHandle(p_key),
                                                              &surface_mizuho,
                                                              42 };
    }
    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const std::any                                     value{ reinterpret_cast<const void*>(&surface_mizuho) };
        [[maybe_unused]] const tetengo_lattice_entryView_t entry{
            tetengo_lattice_entryView_toKeyHandle(p_key),
            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
            42
        };
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ std::make_unique<tetengo::lattice::string_input>(key_mizuho),
                                              surface_mizuho,
                                              42 };

        BOOST_TEST_REQUIRE(entry_.p_key());
        BOOST_TEST_REQUIRE(entry_.p_key()->is<tetengo::lattice::string_input>());
        BOOST_TEST(entry_.p_key()->as<tetengo::lattice::string_input>().value() == key_mizuho);
    }

    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo_lattice_entry_t entry{ tetengo_lattice_entry_createKeyHandle(p_key), &surface_mizuho, 42 };

        const auto* const p_entry_key = tetengo_lattice_entry_createKeyOf(entry.key_handle);
        BOOST_SCOPE_EXIT(p_entry_key)
        {
            tetengo_lattice_input_destroy(p_entry_key);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_entry_key);
        BOOST_TEST(std::string_view{ tetengo_lattice_stringInput_value(p_entry_key) } == key_mizuho);
    }
    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ tetengo_lattice_entryView_toKeyHandle(p_key),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
                                                 42 };

        const auto* const p_entry_key = tetengo_lattice_entryView_createKeyOf(entry.key_handle);
        BOOST_SCOPE_EXIT(p_entry_key)
        {
            tetengo_lattice_input_destroy(p_entry_key);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_entry_key);
        BOOST_TEST(std::string_view{ tetengo_lattice_stringInput_value(p_entry_key) } == key_mizuho);
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ std::make_unique<tetengo::lattice::string_input>(key_mizuho),
                                              surface_mizuho,
                                              42 };

        BOOST_TEST(std::any_cast<std::string>(entry_.value()) == surface_mizuho);
    }

    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo_lattice_entry_t entry{ tetengo_lattice_entry_createKeyHandle(p_key), &surface_mizuho, 42 };

        BOOST_TEST(reinterpret_cast<const std::string*>(entry.p_value) == &surface_mizuho);
    }
    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ tetengo_lattice_entryView_toKeyHandle(p_key),
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
        const tetengo::lattice::entry entry_{ std::make_unique<tetengo::lattice::string_input>(key_mizuho),
                                              surface_mizuho,
                                              42 };

        BOOST_TEST(entry_.cost() == 42);
    }

    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo_lattice_entry_t entry{ tetengo_lattice_entry_createKeyHandle(p_key), &surface_mizuho, 42 };

        BOOST_TEST(entry.cost == 42);
    }
    {
        const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_key)
        {
            tetengo_lattice_input_destroy(p_key);
        }
        BOOST_SCOPE_EXIT_END;
        const std::any                    value{ reinterpret_cast<const void*>(&surface_mizuho) };
        const tetengo_lattice_entryView_t entry{ tetengo_lattice_entryView_toKeyHandle(p_key),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value),
                                                 42 };

        BOOST_TEST(entry.cost == 42);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
