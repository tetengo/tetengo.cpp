/*! \file
    \brief An unordered map vocabulary.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>


namespace
{
    using key_type = tetengo::lattice::string_input;

    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::string key_mizuho{ 0xE3_c, 0x81_c, 0xBF_c, 0xE3_c, 0x81_c, 0x9A_c, 0xE3_c, 0x81_c, 0xBB_c };

    const std::string surface_mizuho{ 0xE7_c, 0x91_c, 0x9E_c, 0xE7_c, 0xA9_c, 0x82_c };

    const std::string key_sakura{ 0xE3_c, 0x81_c, 0x95_c, 0xE3_c, 0x81_c, 0x8F_c, 0xE3_c, 0x82_c, 0x89_c };

    const std::string surface_sakura1{ 0xE6_c, 0xA1_c, 0x9C_c };

    const std::string surface_sakura2{ 0xE3_c, 0x81_c, 0x95_c, 0xE3_c, 0x81_c, 0x8F_c, 0xE3_c, 0x82_c, 0x89_c };

    tetengo::lattice::node make_node(const tetengo::lattice::entry_view& entry)
    {
        static const std::vector<int> preceding_edge_costs{};
        return tetengo::lattice::node{ entry,
                                       0,
                                       std::numeric_limits<std::size_t>::max(),
                                       &preceding_edge_costs,
                                       std::numeric_limits<std::size_t>::max(),
                                       std::numeric_limits<int>::max() };
    }

    std::size_t cpp_entry_hash(const tetengo::lattice::entry_view& entry)
    {
        return entry.p_key() ? entry.p_key()->hash_value() : 0;
    }

    bool cpp_entry_equal_to(const tetengo::lattice::entry_view& one, const tetengo::lattice::entry_view& another)
    {
        return (!one.p_key() && !another.p_key()) ||
               (one.p_key() && another.p_key() && *one.p_key() == *another.p_key());
    }

    size_t c_entry_hash(const tetengo_lattice_entryView_t* const p_entry)
    {
        if (p_entry)
        {
            const auto* const p_entry_key = tetengo_lattice_entryView_createKeyOf(p_entry->key_handle);
            BOOST_SCOPE_EXIT(p_entry_key)
            {
                tetengo_lattice_input_destroy(p_entry_key);
            }
            BOOST_SCOPE_EXIT_END;
            return p_entry_key ? tetengo_lattice_input_hashValue(p_entry_key) : 0;
        }
        else
        {
            assert(false);
            return 0;
        }
    }

    int
    c_entry_equal_to(const tetengo_lattice_entryView_t* const p_one, const tetengo_lattice_entryView_t* const p_another)
    {
        if (p_one && p_another)
        {
            const auto* const p_one_key = tetengo_lattice_entryView_createKeyOf(p_one->key_handle);
            BOOST_SCOPE_EXIT(p_one_key)
            {
                tetengo_lattice_input_destroy(p_one_key);
            }
            BOOST_SCOPE_EXIT_END;
            const auto* const p_another_key = tetengo_lattice_entryView_createKeyOf(p_another->key_handle);
            BOOST_SCOPE_EXIT(p_another_key)
            {
                tetengo_lattice_input_destroy(p_another_key);
            }
            BOOST_SCOPE_EXIT_END;
            return (!p_one_key && !p_another_key) ||
                   (p_one_key && p_another_key && tetengo_lattice_input_equal(p_one_key, p_another_key));
        }
        else
        {
            assert(false);
            return 0;
        }
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(unordered_map_vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>>                entries{};
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{};
        const tetengo::lattice::unordered_map_vocabulary                                         vocabulary{
            std::move(entries), std::move(connections), cpp_entry_hash, cpp_entry_equal_to
        };
    }
    {
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entries{
            { key_mizuho, { { std::make_unique<tetengo::lattice::string_input>(key_mizuho), surface_mizuho, 42 } } },
            { key_sakura,
              { { std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura1, 24 },
                { std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura2, 2424 } } }
        };
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
            { std::make_pair(
                  tetengo::lattice::entry{
                      std::make_unique<tetengo::lattice::string_input>(key_mizuho), surface_mizuho, 42 },
                  tetengo::lattice::entry{
                      std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura1, 24 }),
              4242 }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{
            std::move(entries), std::move(connections), cpp_entry_hash, cpp_entry_equal_to
        };
    }

    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const std::vector<tetengo_lattice_entry_t> entries_mizuho{ { entry_key_handle_mizuho, &surface_mizuho, 42 } };
        const std::vector<tetengo_lattice_entry_t> entries_sakura{
            { entry_key_handle_sakura, &surface_sakura1, 24 }, { entry_key_handle_sakura, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_keyEntriesPair_t> entries{
            { { key_mizuho.c_str(), key_mizuho.length() }, std::data(entries_mizuho), std::size(entries_mizuho) },
            { { key_sakura.c_str(), key_sakura.length() }, std::data(entries_sakura), std::size(entries_sakura) }
        };

        const tetengo_lattice_entry_t connection_key_mizuho{ entry_key_handle_mizuho, &surface_mizuho, 42 };
        const tetengo_lattice_entry_t connection_key_sakura{ entry_key_handle_sakura, &surface_sakura1, 24 };
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> connections{
            { &connection_key_mizuho, &connection_key_sakura, 4242 }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(entries), 2, std::data(connections), 1, c_entry_hash, c_entry_equal_to);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_vocabulary);
    }
    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const tetengo_lattice_entry_t connection_key_mizuho{ entry_key_handle_mizuho, &surface_mizuho, 42 };
        const tetengo_lattice_entry_t connection_key_sakura{ entry_key_handle_sakura, &surface_sakura1, 24 };
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> connections{
            { &connection_key_mizuho, &connection_key_sakura, 4242 }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            nullptr, 0, std::data(connections), 1, c_entry_hash, c_entry_equal_to);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_vocabulary);
    }
    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const tetengo_lattice_entry_t connection_key_mizuho{ entry_key_handle_mizuho, &surface_mizuho, 42 };
        const tetengo_lattice_entry_t connection_key_sakura{ entry_key_handle_sakura, &surface_sakura1, 24 };
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> connections{
            { &connection_key_mizuho, &connection_key_sakura, 4242 }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            nullptr, 42, std::data(connections), 1, c_entry_hash, c_entry_equal_to);

        BOOST_TEST(!p_vocabulary);
    }
    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const std::vector<tetengo_lattice_entry_t> entries_mizuho{ { entry_key_handle_mizuho, &surface_mizuho, 42 } };
        const std::vector<tetengo_lattice_entry_t> entries_sakura{
            { entry_key_handle_sakura, &surface_sakura1, 24 }, { entry_key_handle_sakura, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_keyEntriesPair_t> entries{
            { { key_mizuho.c_str(), key_mizuho.length() }, std::data(entries_mizuho), std::size(entries_mizuho) },
            { { key_sakura.c_str(), key_sakura.length() }, std::data(entries_sakura), std::size(entries_sakura) }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(entries), 2, nullptr, 0, c_entry_hash, c_entry_equal_to);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_vocabulary);
    }
    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const std::vector<tetengo_lattice_entry_t> entries_mizuho{ { entry_key_handle_mizuho, &surface_mizuho, 42 } };
        const std::vector<tetengo_lattice_entry_t> entries_sakura{
            { entry_key_handle_sakura, &surface_sakura1, 24 }, { entry_key_handle_sakura, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_keyEntriesPair_t> entries{
            { { key_mizuho.c_str(), key_mizuho.length() }, std::data(entries_mizuho), std::size(entries_mizuho) },
            { { key_sakura.c_str(), key_sakura.length() }, std::data(entries_sakura), std::size(entries_sakura) }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(entries), 2, nullptr, 1, c_entry_hash, c_entry_equal_to);

        BOOST_TEST(!p_vocabulary);
    }
}

BOOST_AUTO_TEST_CASE(find_entries)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>>                entries{};
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{};
        const tetengo::lattice::unordered_map_vocabulary                                         vocabulary{
            std::move(entries), std::move(connections), cpp_entry_hash, cpp_entry_equal_to
        };

        {
            const auto found = vocabulary.find_entries(key_type{ key_mizuho });
            BOOST_TEST(std::empty(found));
        }
        {
            const auto found = vocabulary.find_entries(key_type{ key_sakura });
            BOOST_TEST(std::empty(found));
        }
    }
    {
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entries{
            { key_mizuho, { { std::make_unique<key_type>(key_mizuho), surface_mizuho, 42 } } },
            { key_sakura,
              { { std::make_unique<key_type>(key_sakura), surface_sakura1, 24 },
                { std::make_unique<key_type>(key_sakura), surface_sakura2, 2424 } } }
        };
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
            { std::make_pair(
                  tetengo::lattice::entry{ std::make_unique<key_type>(key_mizuho), surface_mizuho, 42 },
                  tetengo::lattice::entry{ std::make_unique<key_type>(key_sakura), surface_sakura1, 24 }),
              4242 }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{
            std::move(entries), std::move(connections), cpp_entry_hash, cpp_entry_equal_to
        };

        {
            const auto found = vocabulary.find_entries(key_type{ key_mizuho });
            BOOST_TEST_REQUIRE(std::size(found) == 1U);
            BOOST_TEST_REQUIRE(found[0].p_key());
            BOOST_TEST_REQUIRE(found[0].p_key()->is<key_type>());
            BOOST_TEST(found[0].p_key()->as<key_type>().value() == key_mizuho);
            BOOST_TEST(*std::any_cast<std::string>(found[0].value()) == surface_mizuho);
            BOOST_TEST(found[0].cost() == 42);
        }
        {
            const auto found = vocabulary.find_entries(key_type{ key_sakura });
            BOOST_TEST_REQUIRE(std::size(found) == 2U);
            BOOST_TEST_REQUIRE(found[0].p_key());
            BOOST_TEST_REQUIRE(found[0].p_key()->is<key_type>());
            BOOST_TEST(found[0].p_key()->as<key_type>().value() == key_sakura);
            BOOST_TEST(*std::any_cast<std::string>(found[0].value()) == surface_sakura1);
            BOOST_TEST(found[0].cost() == 24);
            BOOST_TEST_REQUIRE(found[1].p_key());
            BOOST_TEST_REQUIRE(found[1].p_key()->is<key_type>());
            BOOST_TEST(found[1].p_key()->as<key_type>().value() == key_sakura);
            BOOST_TEST(*std::any_cast<std::string>(found[1].value()) == surface_sakura2);
            BOOST_TEST(found[1].cost() == 2424);
        }
    }

    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;

        const std::vector<tetengo_lattice_entry_t> entries_mizuho{
            { tetengo_lattice_entry_toKeyHandle(p_input_mizuho), &surface_mizuho, 42 }
        };
        const std::vector<tetengo_lattice_entry_t> entries_sakura{
            { tetengo_lattice_entry_toKeyHandle(p_input_sakura), &surface_sakura1, 24 },
            { tetengo_lattice_entry_toKeyHandle(p_input_sakura), &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_keyEntriesPair_t> entries{
            { { key_mizuho.c_str(), key_mizuho.length() }, std::data(entries_mizuho), std::size(entries_mizuho) },
            { { key_sakura.c_str(), key_sakura.length() }, std::data(entries_sakura), std::size(entries_sakura) }
        };

        const tetengo_lattice_entry_t connection_key_mizuho{ tetengo_lattice_entry_toKeyHandle(p_input_mizuho),
                                                             &surface_mizuho,
                                                             42 };
        const tetengo_lattice_entry_t connection_key_sakura{ tetengo_lattice_entry_toKeyHandle(p_input_sakura),
                                                             &surface_sakura1,
                                                             24 };
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> connections{
            { &connection_key_mizuho, &connection_key_sakura, 4242 }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(entries),
            std::size(entries),
            std::data(connections),
            std::size(connections),
            c_entry_hash,
            c_entry_equal_to);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_vocabulary);

        {
            const auto* const p_key = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_key)
            {
                tetengo_lattice_input_destroy(p_key);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_key, nullptr);
            BOOST_TEST(entry_count == 1U);

            std::vector<tetengo_lattice_entryView_t> entry_values{ entry_count };
            const auto                               entry_count_again =
                tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_key, std::data(entry_values));
            BOOST_TEST_REQUIRE(entry_count_again == 1U);

            const auto* const p_entry_key0 = tetengo_lattice_entryView_createKeyOf(entry_values[0].key_handle);
            BOOST_SCOPE_EXIT(p_entry_key0)
            {
                tetengo_lattice_input_destroy(p_entry_key0);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_entry_key0);
            BOOST_TEST((std::string{ tetengo_lattice_stringInput_value(p_entry_key0) } == key_mizuho));
            BOOST_TEST(
                reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(entry_values[0].value_handle)) ==
                &surface_mizuho);
            BOOST_TEST(entry_values[0].cost == 42);
        }
        {
            const auto* const p_key = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_key)
            {
                tetengo_lattice_input_destroy(p_key);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_key, nullptr);
            BOOST_TEST(entry_count == 2U);

            std::vector<tetengo_lattice_entryView_t> entry_values{ entry_count };
            const auto                               entry_count_again =
                tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_key, std::data(entry_values));
            BOOST_TEST_REQUIRE(entry_count_again == 2U);

            const auto* const p_entry_key0 = tetengo_lattice_entryView_createKeyOf(entry_values[0].key_handle);
            BOOST_SCOPE_EXIT(p_entry_key0)
            {
                tetengo_lattice_input_destroy(p_entry_key0);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_entry_key0);
            BOOST_TEST((std::string{ tetengo_lattice_stringInput_value(p_entry_key0) } == key_sakura));
            BOOST_TEST(
                reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(entry_values[0].value_handle)) ==
                &surface_sakura1);
            BOOST_TEST(entry_values[0].cost == 24);
            const auto* const p_entry_key1 = tetengo_lattice_entryView_createKeyOf(entry_values[1].key_handle);
            BOOST_SCOPE_EXIT(p_entry_key1)
            {
                tetengo_lattice_input_destroy(p_entry_key1);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_entry_key1);
            BOOST_TEST((std::string{ tetengo_lattice_stringInput_value(p_entry_key1) } == key_sakura));
            BOOST_TEST(
                reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(entry_values[1].value_handle)) ==
                &surface_sakura2);
            BOOST_TEST(entry_values[1].cost == 2424);
        }
        {
            const auto* const p_key = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_key)
            {
                tetengo_lattice_input_destroy(p_key);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(nullptr, p_key, nullptr);
            BOOST_TEST(entry_count == 0U);
        }
        {
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, nullptr, nullptr);
            BOOST_TEST(entry_count == 0U);
        }
    }
}

BOOST_AUTO_TEST_CASE(find_connection)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entries{
            { key_mizuho, { { std::make_unique<tetengo::lattice::string_input>(key_mizuho), surface_mizuho, 42 } } },
            { key_sakura,
              { { std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura1, 24 },
                { std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura2, 2424 } } }
        };
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
            { std::make_pair(
                  tetengo::lattice::entry{
                      std::make_unique<tetengo::lattice::string_input>(key_mizuho), surface_mizuho, 42 },
                  tetengo::lattice::entry{
                      std::make_unique<tetengo::lattice::string_input>(key_sakura), surface_sakura1, 24 }),
              4242 }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{
            std::move(entries), std::move(connections), cpp_entry_hash, cpp_entry_equal_to
        };

        const auto entries_mizuho = vocabulary.find_entries(key_type{ key_mizuho });
        BOOST_TEST_REQUIRE(std::size(entries_mizuho) == 1U);
        const auto entries_sakura = vocabulary.find_entries(key_type{ key_sakura });
        BOOST_TEST_REQUIRE(std::size(entries_sakura) == 2U);

        {
            const auto connection = vocabulary.find_connection(make_node(entries_mizuho[0]), entries_sakura[0]);

            BOOST_TEST(connection.cost() == 4242);
        }
        {
            const auto connection = vocabulary.find_connection(make_node(entries_mizuho[0]), entries_mizuho[0]);

            BOOST_TEST(connection.cost() == std::numeric_limits<int>::max());
        }
    }

    {
        const auto* const p_input_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
        BOOST_SCOPE_EXIT(p_input_mizuho)
        {
            tetengo_lattice_input_destroy(p_input_mizuho);
        }
        BOOST_SCOPE_EXIT_END;
        const auto* const p_input_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
        BOOST_SCOPE_EXIT(p_input_sakura)
        {
            tetengo_lattice_input_destroy(p_input_sakura);
        }
        BOOST_SCOPE_EXIT_END;
        const auto entry_key_handle_mizuho = tetengo_lattice_entry_toKeyHandle(p_input_mizuho);
        const auto entry_key_handle_sakura = tetengo_lattice_entry_toKeyHandle(p_input_sakura);

        const std::vector<tetengo_lattice_entry_t> entries_mizuho{ { entry_key_handle_mizuho, &surface_mizuho, 42 } };
        const std::vector<tetengo_lattice_entry_t> entries_sakura{
            { entry_key_handle_sakura, &surface_sakura1, 24 }, { entry_key_handle_sakura, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_keyEntriesPair_t> entries{
            { { key_mizuho.c_str(), key_mizuho.length() }, std::data(entries_mizuho), std::size(entries_mizuho) },
            { { key_sakura.c_str(), key_sakura.length() }, std::data(entries_sakura), std::size(entries_sakura) }
        };

        const tetengo_lattice_entry_t connection_key_mizuho{ entry_key_handle_mizuho, &surface_mizuho, 42 };
        const tetengo_lattice_entry_t connection_key_sakura1{ entry_key_handle_sakura, &surface_sakura1, 24 };
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> connections{
            { &connection_key_mizuho, &connection_key_sakura1, 4242 }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(entries),
            std::size(entries),
            std::data(connections),
            std::size(connections),
            c_entry_hash,
            c_entry_equal_to);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_vocabulary);

        {
            const auto* const p_input_view_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_input_view_mizuho)
            {
                tetengo_lattice_input_destroy(p_input_view_mizuho);
            }
            BOOST_SCOPE_EXIT_END;
            const auto* const p_input_view_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_input_view_sakura)
            {
                tetengo_lattice_input_destroy(p_input_view_sakura);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_mizuho = tetengo_lattice_entryView_toKeyHandle(p_input_view_mizuho);
            const auto entry_view_key_handle_sakura = tetengo_lattice_entryView_toKeyHandle(p_input_view_sakura);

            const std::any                    value_mizuho{ reinterpret_cast<const void*>(&surface_mizuho) };
            const tetengo_lattice_entryView_t entry_mizuho{ entry_view_key_handle_mizuho,
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                &value_mizuho),
                                                            42 };
            const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            tetengo_lattice_node_t            node_mizuho{};
            BOOST_TEST_REQUIRE(tetengo_lattice_node_toNode(
                &entry_mizuho,
                0,
                std::numeric_limits<size_t>::max(),
                std::data(preceding_edge_costs),
                std::size(preceding_edge_costs),
                std::numeric_limits<size_t>::max(),
                std::numeric_limits<int>::max(),
                &node_mizuho));
            const std::any                    value_sakura1{ reinterpret_cast<const void*>(&surface_sakura1) };
            const tetengo_lattice_entryView_t entry_sakura1{ entry_view_key_handle_sakura,
                                                             reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                 &value_sakura1),
                                                             24 };
            tetengo_lattice_connection_t      connection{};
            const auto                        found =
                tetengo_lattice_vocabulary_findConnection(p_vocabulary, &node_mizuho, &entry_sakura1, &connection);
            BOOST_TEST(found);

            BOOST_TEST(connection.cost == 4242);
        }
        {
            const auto* const p_input_view_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_input_view_mizuho)
            {
                tetengo_lattice_input_destroy(p_input_view_mizuho);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_mizuho = tetengo_lattice_entryView_toKeyHandle(p_input_view_mizuho);

            const std::any                    value_mizuho{ reinterpret_cast<const void*>(&surface_mizuho) };
            const tetengo_lattice_entryView_t entry_mizuho{ entry_view_key_handle_mizuho,
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                &value_mizuho),
                                                            42 };
            const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            tetengo_lattice_node_t            node_mizuho{};
            BOOST_TEST_REQUIRE(tetengo_lattice_node_toNode(
                &entry_mizuho,
                0,
                std::numeric_limits<size_t>::max(),
                std::data(preceding_edge_costs),
                std::size(preceding_edge_costs),
                std::numeric_limits<size_t>::max(),
                std::numeric_limits<int>::max(),
                &node_mizuho));
            tetengo_lattice_connection_t connection{};
            const auto                   found =
                tetengo_lattice_vocabulary_findConnection(p_vocabulary, &node_mizuho, &entry_mizuho, &connection);
            BOOST_TEST(found);

            BOOST_TEST(connection.cost == std::numeric_limits<int>::max());
        }
        {
            const auto* const p_input_view_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_input_view_mizuho)
            {
                tetengo_lattice_input_destroy(p_input_view_mizuho);
            }
            BOOST_SCOPE_EXIT_END;
            const auto* const p_input_view_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_input_view_sakura)
            {
                tetengo_lattice_input_destroy(p_input_view_sakura);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_mizuho = tetengo_lattice_entryView_toKeyHandle(p_input_view_mizuho);
            const auto entry_view_key_handle_sakura = tetengo_lattice_entryView_toKeyHandle(p_input_view_sakura);

            const std::any                    value_mizuho{ reinterpret_cast<const void*>(&surface_mizuho) };
            const tetengo_lattice_entryView_t entry_mizuho{ entry_view_key_handle_mizuho,
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                &value_mizuho),
                                                            42 };
            const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            tetengo_lattice_node_t            node_mizuho{};
            BOOST_TEST_REQUIRE(tetengo_lattice_node_toNode(
                &entry_mizuho,
                0,
                std::numeric_limits<size_t>::max(),
                std::data(preceding_edge_costs),
                std::size(preceding_edge_costs),
                std::numeric_limits<size_t>::max(),
                std::numeric_limits<int>::max(),
                &node_mizuho));
            tetengo_lattice_connection_t      connection{};
            const std::any                    value_sakura2{ reinterpret_cast<const void*>(&surface_sakura2) };
            const tetengo_lattice_entryView_t entry_sakura2{ entry_view_key_handle_sakura,
                                                             reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                 &value_sakura2),
                                                             2424 };
            const auto                        found =
                tetengo_lattice_vocabulary_findConnection(nullptr, &node_mizuho, &entry_sakura2, &connection);
            BOOST_TEST(!found);
        }
        {
            const auto* const p_input_view_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_input_view_sakura)
            {
                tetengo_lattice_input_destroy(p_input_view_sakura);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_sakura = tetengo_lattice_entryView_toKeyHandle(p_input_view_sakura);

            const std::any                    value_sakura2{ reinterpret_cast<const void*>(&surface_sakura2) };
            const tetengo_lattice_entryView_t entry_sakura2{ entry_view_key_handle_sakura,
                                                             reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                 &value_sakura2),
                                                             2424 };
            tetengo_lattice_connection_t      connection{};
            const auto                        found =
                tetengo_lattice_vocabulary_findConnection(p_vocabulary, nullptr, &entry_sakura2, &connection);
            BOOST_TEST(!found);
        }
        {
            const auto* const p_input_view_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_input_view_mizuho)
            {
                tetengo_lattice_input_destroy(p_input_view_mizuho);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_mizuho = tetengo_lattice_entryView_toKeyHandle(p_input_view_mizuho);

            const std::any                    value_mizuho{ reinterpret_cast<const void*>(&surface_mizuho) };
            const tetengo_lattice_entryView_t entry_mizuho{ entry_view_key_handle_mizuho,
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                &value_mizuho),
                                                            42 };
            const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            tetengo_lattice_node_t            node_mizuho{};
            BOOST_TEST_REQUIRE(tetengo_lattice_node_toNode(
                &entry_mizuho,
                0,
                std::numeric_limits<size_t>::max(),
                std::data(preceding_edge_costs),
                std::size(preceding_edge_costs),
                std::numeric_limits<size_t>::max(),
                std::numeric_limits<int>::max(),
                &node_mizuho));
            tetengo_lattice_connection_t connection{};
            const auto                   found =
                tetengo_lattice_vocabulary_findConnection(p_vocabulary, &node_mizuho, nullptr, &connection);
            BOOST_TEST(!found);
        }
        {
            const auto* const p_input_view_mizuho = tetengo_lattice_input_createStringInput(key_mizuho.c_str());
            BOOST_SCOPE_EXIT(p_input_view_mizuho)
            {
                tetengo_lattice_input_destroy(p_input_view_mizuho);
            }
            BOOST_SCOPE_EXIT_END;
            const auto* const p_input_view_sakura = tetengo_lattice_input_createStringInput(key_sakura.c_str());
            BOOST_SCOPE_EXIT(p_input_view_sakura)
            {
                tetengo_lattice_input_destroy(p_input_view_sakura);
            }
            BOOST_SCOPE_EXIT_END;
            const auto entry_view_key_handle_mizuho = tetengo_lattice_entryView_toKeyHandle(p_input_view_mizuho);
            const auto entry_view_key_handle_sakura = tetengo_lattice_entryView_toKeyHandle(p_input_view_sakura);

            const std::any                    value_mizuho{ reinterpret_cast<const void*>(&surface_mizuho) };
            const tetengo_lattice_entryView_t entry_mizuho{ entry_view_key_handle_mizuho,
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                &value_mizuho),
                                                            42 };
            const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            tetengo_lattice_node_t            node_mizuho{};
            BOOST_TEST_REQUIRE(tetengo_lattice_node_toNode(
                &entry_mizuho,
                0,
                std::numeric_limits<size_t>::max(),
                std::data(preceding_edge_costs),
                std::size(preceding_edge_costs),
                std::numeric_limits<size_t>::max(),
                std::numeric_limits<int>::max(),
                &node_mizuho));
            const std::any                    value_sakura2{ reinterpret_cast<const void*>(&surface_sakura2) };
            const tetengo_lattice_entryView_t entry_sakura2{ entry_view_key_handle_sakura,
                                                             reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                 &value_sakura2),
                                                             2424 };
            const auto                        found =
                tetengo_lattice_vocabulary_findConnection(p_vocabulary, &node_mizuho, &entry_sakura2, nullptr);
            BOOST_TEST(!found);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
