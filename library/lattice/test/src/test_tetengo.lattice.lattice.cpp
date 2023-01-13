/*! \file
    \brief A lattice.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits> // IWYU pragma: keep
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
    std::unique_ptr<tetengo::lattice::input> to_input(const char* const string)
    {
        return std::make_unique<tetengo::lattice::string_input>(string);
    }

    /*
                   +------------------mizuho/sakura/tsubame-------------------+
                   |                path cost: 4270/3220/2990                 |
                   |                                                          |
                   +------------ariake/rapid811------------+                  |
                   |          path cost: 2850/2010         |                  |
                   |                                       |                  |
        BOS--(Hakata)--kamome/local415--(Tosu)--local813--(Omuta)--local817--(Kumamoto)--EOS
                     path cost: 1640/1370   |   pc: 2830           pc: 3160   |     path cost:3390
                                            |                                 |
                                            +------------local815-------------+
                                                      path cost: 3550

        (0) 3390  BOS - tsubame - EOS
            [ sakura(3620),   local817(3760), local815(4050), mizuho(4670)   ]
        (1) 3620  BOS - sakura - EOS
            [ local817(3760), local815(4050), mizuho(4670)                   ]
        (2) 3760  BOS - rapid811 - local817 - EOS
            [ local815(4050), ariake(4600),   mizuho(4670),   local813(4680) ]
        (3) 4050  BOS - local415 - local815 - EOS
            [ kamome(4320),   ariake(4600),   mizuho(4670),   local813(4680) ]
        (4) 4320  BOS - kamome - local815 - EOS
            [ ariake(4600),   mizuho(4670),   local813(4680)                 ]
        (5) 4600  BOS - ariake - local817 - EOS
            [ mizuho(4670),   local813(4680)                                 ]
        (6) 4670  BOS - mizuho - EOS
            [ local813(4680)                                                 ]
        (7) 4680  BOS - local415 - local813 - local817 - EOS
            [ kamome(4950)                                                   ]
        (8) 4950  BOS - kamome - local813 - local817 - EOS
            [                                                                ]
        (9) ----  -
            [                                                                ]
    */
    const std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entries{
        { "[HakataTosu][TosuOmuta][OmutaKumamoto]",
          {
              { to_input("Hakata-Tosu-Omuta-Kumamoto"), std::string{ "mizuho" }, 3670 },
              { to_input("Hakata-Tosu-Omuta-Kumamoto"), std::string{ "sakura" }, 2620 },
              { to_input("Hakata-Tosu-Omuta-Kumamoto"), std::string{ "tsubame" }, 2390 },
          } },
        { "[HakataTosu][TosuOmuta]",
          {
              { to_input("Hakata-Tosu-Omuta"), std::string{ "ariake" }, 2150 },
              { to_input("Hakata-Tosu-Omuta"), std::string{ "rapid811" }, 1310 },
          } },
        { "[HakataTosu]",
          {
              { to_input("Hakata-Tosu"), std::string{ "kamome" }, 840 },
              { to_input("Hakata-Tosu"), std::string{ "local415" }, 570 },
          } },
        { "[TosuOmuta]",
          {
              { to_input("Tosu-Omuta"), std::string{ "local813" }, 860 },
          } },
        { "[TosuOmuta][OmutaKumamoto]",
          {
              { to_input("Tosu-Omuta-Kumamoto"), std::string{ "local815" }, 1680 },
          } },
        { "[OmutaKumamoto]",
          {
              { to_input("Omuta-Kumamoto"), std::string{ "local817" }, 950 },
          } },
    };

    const std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
        { { tetengo::lattice::entry::bos_eos(), { to_input("Hakata-Tosu-Omuta-Kumamoto"), {}, 0 } }, 600 },
        { { tetengo::lattice::entry::bos_eos(), { to_input("Hakata-Tosu-Omuta"), {}, 0 } }, 700 },
        { { tetengo::lattice::entry::bos_eos(), { to_input("Hakata-Tosu"), {}, 0 } }, 800 },
        { { tetengo::lattice::entry::bos_eos(), tetengo::lattice::entry::bos_eos() }, 8000 },
        { { { to_input("Hakata-Tosu"), {}, 0 }, { to_input("Tosu-Omuta-Kumamoto"), {}, 0 } }, 500 },
        { { { to_input("Hakata-Tosu"), {}, 0 }, { to_input("Tosu-Omuta"), {}, 0 } }, 600 },
        { { { to_input("Hakata-Tosu"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 6000 },
        { { { to_input("Hakata-Tosu-Omuta"), {}, 0 }, { to_input("Omuta-Kumamoto"), {}, 0 } }, 200 },
        { { { to_input("Hakata-Tosu-Omuta"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 2000 },
        { { { to_input("Tosu-Omuta"), {}, 0 }, { to_input("Omuta-Kumamoto"), {}, 0 } }, 300 },
        { { { to_input("Tosu-Omuta"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 3000 },
        { { { to_input("Hakata-Tosu-Omuta-Kumamoto"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 400 },
        { { { to_input("Tosu-Omuta-Kumamoto"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 500 },
        { { { to_input("Omuta-Kumamoto"), {}, 0 }, tetengo::lattice::entry::bos_eos() }, 600 },
    };

    std::size_t cpp_entry_hash(const tetengo::lattice::entry_view& entry)
    {
        return entry.p_key() ? entry.p_key()->hash_value() : 0;
    }

    bool cpp_entry_equal_to(const tetengo::lattice::entry_view& one, const tetengo::lattice::entry_view& another)
    {
        return (!one.p_key() && !another.p_key()) ||
               (one.p_key() && another.p_key() && *one.p_key() == *another.p_key());
    }

    std::unique_ptr<tetengo::lattice::vocabulary> create_cpp_vocabulary()
    {
        return std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            entries, connections, cpp_entry_hash, cpp_entry_equal_to);
    }

    std::unique_ptr<tetengo::lattice::vocabulary> create_cpp_empty_vocabulary()
    {
        return std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>>{},
            std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>>{},
            cpp_entry_hash,
            cpp_entry_equal_to);
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

    tetengo_lattice_vocabulary_t* create_c_vocabulary()
    {
        std::vector<tetengo_lattice_entry_t> entry_values{};
        std::vector<std::size_t>             entry_value_offsets{};
        entry_value_offsets.reserve(std::size(entries) + 1);
        for (const auto& e: entries)
        {
            entry_values.reserve(std::size(entry_values) + std::size(e.second));
            entry_value_offsets.push_back(std::size(entry_values));

            for (const auto& ev: e.second)
            {
                entry_values.push_back({ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(ev.p_key()),
                                         std::any_cast<std::string>(&ev.value()),
                                         ev.cost() });
            }
        }
        entry_value_offsets.push_back(std::size(entry_values));

        std::vector<tetengo_lattice_keyEntriesPair_t> key_entries_pairs{};
        key_entries_pairs.reserve(std::size(entries));
        for (auto i = static_cast<std::size_t>(0); i < std::size(entries); ++i)
        {
            const auto& entry = entries[i];
            key_entries_pairs.push_back({ { entry.first.c_str(), entry.first.length() },
                                          &entry_values[entry_value_offsets[i]],
                                          entry_value_offsets[i + 1] - entry_value_offsets[i] });
        }

        std::vector<tetengo_lattice_entry_t> connection_froms{};
        connection_froms.reserve(std::size(connections));
        std::vector<tetengo_lattice_entry_t> connection_tos{};
        connection_tos.reserve(std::size(connections));
        for (const auto& c: connections)
        {
            connection_froms.push_back({ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(c.first.first.p_key()),
                                         std::any_cast<std::string>(&c.first.first.value()),
                                         c.first.first.cost() });
            connection_tos.push_back({ reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(c.first.second.p_key()),
                                       std::any_cast<std::string>(&c.first.second.value()),
                                       c.first.second.cost() });
        }

        std::vector<tetengo_lattice_entriesConnectionCostPair_t> entries_connection_cost_pairs{};
        entries_connection_cost_pairs.reserve(std::size(connections));
        for (auto i = static_cast<std::size_t>(0); i < std::size(connections); ++i)
        {
            const auto& connection = connections[i];
            const auto& from = connection_froms[i];
            const auto& to = connection_tos[i];
            entries_connection_cost_pairs.push_back({ &from, &to, connection.second });
        }

        return tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(key_entries_pairs),
            std::size(key_entries_pairs),
            std::data(entries_connection_cost_pairs),
            std::size(entries_connection_cost_pairs),
            c_entry_hash,
            c_entry_equal_to);
    }

    tetengo_lattice_vocabulary_t* create_c_empty_vocabulary()
    {
        const std::vector<tetengo_lattice_keyEntriesPair_t>      key_entries_pairs{};
        std::vector<tetengo_lattice_entriesConnectionCostPair_t> entries_connection_cost_pairs{};
        return tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            std::data(key_entries_pairs),
            std::size(key_entries_pairs),
            std::data(entries_connection_cost_pairs),
            std::size(entries_connection_cost_pairs),
            c_entry_hash,
            c_entry_equal_to);
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(lattice)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                      p_vocabulary = create_cpp_vocabulary();
        const tetengo::lattice::lattice lattice_{ *p_vocabulary };
    }

    {
        const auto* const p_vocabulary = create_c_vocabulary();
        const auto* const p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_lattice);
    }
    {
        const auto* const p_lattice = tetengo_lattice_lattice_create(nullptr);

        BOOST_TEST(!p_lattice);
    }
}

BOOST_AUTO_TEST_CASE(step_count)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };

        BOOST_TEST(lattice_.step_count() == 1U);

        lattice_.push_back(to_input("[HakataTosu]"));

        BOOST_TEST(lattice_.step_count() == 2U);

        lattice_.push_back(to_input("[TosuOmuta]"));

        BOOST_TEST(lattice_.step_count() == 3U);

        lattice_.push_back(to_input("[OmutaKumamoto]"));
        BOOST_TEST(lattice_.step_count() == 4U);
    }

    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 1U);

        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_hakata_tosu);

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 2U);

        auto* const p_input_tosu_omuta = tetengo_lattice_input_createStringInput("[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_tosu_omuta);

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 3U);

        auto* const p_input_omuta_kumamoto = tetengo_lattice_input_createStringInput("[OmutaKumamoto]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_omuta_kumamoto);

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 4U);
    }
    {
        BOOST_TEST(tetengo_lattice_lattice_stepCount(nullptr) == 0U);
    }
}

BOOST_AUTO_TEST_CASE(nodes_at)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        {
            const auto& nodes = lattice_.nodes_at(0);

            BOOST_TEST_REQUIRE(std::size(nodes) == 1U);
            const std::vector<int> preceding_edge_costs{};
            BOOST_TEST(
                nodes[0].value().has_value() == tetengo::lattice::node::bos(&preceding_edge_costs).value().has_value());
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step() == i);
            }
        }
        {
            const auto& nodes = lattice_.nodes_at(1);

            BOOST_TEST_REQUIRE(std::size(nodes) == 2U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "kamome");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "local415");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step() == i);
            }
        }
        {
            const auto& nodes = lattice_.nodes_at(2);

            BOOST_TEST_REQUIRE(std::size(nodes) == 3U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "ariake");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "rapid811");
            BOOST_TEST(std::any_cast<std::string>(nodes[2].value()) == "local813");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step() == i);
            }
        }
        {
            const auto& nodes = lattice_.nodes_at(3);

            BOOST_TEST_REQUIRE(std::size(nodes) == 5U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "mizuho");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "sakura");
            BOOST_TEST(std::any_cast<std::string>(nodes[2].value()) == "tsubame");
            BOOST_TEST(std::any_cast<std::string>(nodes[3].value()) == "local815");
            BOOST_TEST(std::any_cast<std::string>(nodes[4].value()) == "local817");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step() == i);
            }
        }
        {
            BOOST_CHECK_THROW([[maybe_unused]] const auto& nodes = lattice_.nodes_at(4), std::out_of_range);
        }
    }

    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_hakata_tosu);
        auto* const p_input_tosu_omuta = tetengo_lattice_input_createStringInput("[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_tosu_omuta);
        auto* const p_input_omuta_kumamoto = tetengo_lattice_input_createStringInput("[OmutaKumamoto]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_omuta_kumamoto);

        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 0, nullptr);
            BOOST_TEST_REQUIRE(node_count == 1U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 0, std::data(nodes));
            BOOST_TEST(node_count_again == 1U);

            const std::vector<int> preceding_edge_costs{};
            tetengo_lattice_node_t bos{};
            tetengo_lattice_node_bos(std::data(preceding_edge_costs), std::size(preceding_edge_costs), &bos);
            BOOST_TEST(nodes[0].value_handle == bos.value_handle);
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step == i);
            }
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 1, nullptr);
            BOOST_TEST_REQUIRE(node_count == 2U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 1, std::data(nodes));
            BOOST_TEST(node_count_again == 2U);

            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[0].value_handle)) ==
                "kamome");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "local415");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step == i);
            }
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 2, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 2, std::data(nodes));
            BOOST_TEST(node_count_again == 3U);

            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[0].value_handle)) ==
                "ariake");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "rapid811");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local813");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step == i);
            }
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 3, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 3, std::data(nodes));
            BOOST_TEST(node_count_again == 5U);

            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[0].value_handle)) ==
                "mizuho");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "sakura");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "tsubame");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[3].value_handle)) ==
                "local815");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[4].value_handle)) ==
                "local817");
            for (std::size_t i = 0; i < std::size(nodes); ++i)
            {
                BOOST_TEST(nodes[i].index_in_step == i);
            }
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 4, nullptr);
            BOOST_TEST(node_count == 0U);
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(nullptr, 0, nullptr);
            BOOST_TEST(node_count == 0U);
        }
    }
}

BOOST_AUTO_TEST_CASE(push_back)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };

        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));
    }
    {
        const auto                p_vocabulary = create_cpp_empty_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };

        BOOST_CHECK_THROW(lattice_.push_back(to_input("[HakataTosu]")), std::invalid_argument);
    }

    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_hakata_tosu);
        auto* const p_input_tosu_omuta = tetengo_lattice_input_createStringInput("[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_tosu_omuta);
        auto* const p_input_omuta_kumamoto = tetengo_lattice_input_createStringInput("[OmutaKumamoto]");
        tetengo_lattice_lattice_pushBack(p_lattice, p_input_omuta_kumamoto);
    }
    {
        const auto* const p_vocabulary = create_c_empty_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        BOOST_TEST(!tetengo_lattice_lattice_pushBack(p_lattice, p_input_hakata_tosu));
    }
    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        BOOST_TEST(!tetengo_lattice_lattice_pushBack(nullptr, p_input_hakata_tosu));
        BOOST_TEST(!tetengo_lattice_lattice_pushBack(p_lattice, nullptr));
    }
}

BOOST_AUTO_TEST_CASE(settle)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };

        {
            const auto eos_node_and_preceding_edge_costs = lattice_.settle();

            BOOST_TEST(eos_node_and_preceding_edge_costs.first.preceding_step() == 0U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.best_preceding_node() == 0U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.path_cost() == 8000);

            const std::vector<int> expected_preceding_edge_costs{ 8000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(*eos_node_and_preceding_edge_costs.second),
                std::end(*eos_node_and_preceding_edge_costs.second),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        lattice_.push_back(to_input("[HakataTosu]"));
        {
            const auto eos_node_and_preceding_edge_costs = lattice_.settle();

            BOOST_TEST(eos_node_and_preceding_edge_costs.first.preceding_step() == 1U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.best_preceding_node() == 1U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.path_cost() == 7370);

            const std::vector<int> expected_preceding_edge_costs{ 6000, 6000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(*eos_node_and_preceding_edge_costs.second),
                std::end(*eos_node_and_preceding_edge_costs.second),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        lattice_.push_back(to_input("[TosuOmuta]"));
        {
            const auto eos_node_and_preceding_edge_costs = lattice_.settle();

            BOOST_TEST(eos_node_and_preceding_edge_costs.first.preceding_step() == 2U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.best_preceding_node() == 1U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.path_cost() == 4010);

            const std::vector<int> expected_preceding_edge_costs{ 2000, 2000, 3000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(*eos_node_and_preceding_edge_costs.second),
                std::end(*eos_node_and_preceding_edge_costs.second),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        lattice_.push_back(to_input("[OmutaKumamoto]"));
        {
            const auto eos_node_and_preceding_edge_costs = lattice_.settle();

            BOOST_TEST(eos_node_and_preceding_edge_costs.first.preceding_step() == 3U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.best_preceding_node() == 2U);
            BOOST_TEST(eos_node_and_preceding_edge_costs.first.path_cost() == 3390);

            const std::vector<int> expected_preceding_edge_costs{ 400, 400, 400, 500, 600 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(*eos_node_and_preceding_edge_costs.second),
                std::end(*eos_node_and_preceding_edge_costs.second),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }
    }
    {
        const auto                p_vocabulary = create_cpp_empty_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };

        const auto eos_node_and_preceding_edge_costs = lattice_.settle();
    }

    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        {
            const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
            BOOST_TEST(preceding_edge_cost_count == 1U);

            tetengo_lattice_node_t eos_node{};
            std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
            const auto             preceding_edge_cost_count_again =
                tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
            BOOST_TEST_REQUIRE(preceding_edge_cost_count_again == 1U);

            BOOST_TEST(eos_node.preceding_step == 0U);
            BOOST_TEST(eos_node.best_preceding_node == 0U);
            BOOST_TEST(eos_node.path_cost == 8000);

            const std::vector<int> expected_preceding_edge_costs{ 8000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(preceding_edge_costs),
                std::end(preceding_edge_costs),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        auto* const p_input_hakata_tosu = tetengo_lattice_input_createStringInput("[HakataTosu]");
        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, p_input_hakata_tosu));
        {
            const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
            BOOST_TEST(preceding_edge_cost_count == 2U);

            tetengo_lattice_node_t eos_node{};
            std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
            const auto             preceding_edge_cost_count_again =
                tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
            BOOST_TEST_REQUIRE(preceding_edge_cost_count_again == 2U);

            BOOST_TEST(eos_node.preceding_step == 1U);
            BOOST_TEST(eos_node.best_preceding_node == 1U);
            BOOST_TEST(eos_node.path_cost == 7370);

            const std::vector<int> expected_preceding_edge_costs{ 6000, 6000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(preceding_edge_costs),
                std::end(preceding_edge_costs),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        auto* const p_input_tosu_omuta = tetengo_lattice_input_createStringInput("[TosuOmuta]");
        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, p_input_tosu_omuta));
        {
            const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
            BOOST_TEST(preceding_edge_cost_count == 3U);

            tetengo_lattice_node_t eos_node{};
            std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
            const auto             preceding_edge_cost_count_again =
                tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
            BOOST_TEST_REQUIRE(preceding_edge_cost_count_again == 3U);

            BOOST_TEST(eos_node.preceding_step == 2U);
            BOOST_TEST(eos_node.best_preceding_node == 1U);
            BOOST_TEST(eos_node.path_cost == 4010);

            const std::vector<int> expected_preceding_edge_costs{ 2000, 2000, 3000 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(preceding_edge_costs),
                std::end(preceding_edge_costs),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }

        auto* const p_input_omuta_kumamoto = tetengo_lattice_input_createStringInput("[OmutaKumamoto]");
        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, p_input_omuta_kumamoto));
        {
            const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
            BOOST_TEST(preceding_edge_cost_count == 5U);

            tetengo_lattice_node_t eos_node{};
            std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
            const auto             preceding_edge_cost_count_again =
                tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
            BOOST_TEST_REQUIRE(preceding_edge_cost_count_again == 5U);

            BOOST_TEST(eos_node.preceding_step == 3U);
            BOOST_TEST(eos_node.best_preceding_node == 2U);
            BOOST_TEST(eos_node.path_cost == 3390);

            const std::vector<int> expected_preceding_edge_costs{ 400, 400, 400, 500, 600 };
            BOOST_CHECK_EQUAL_COLLECTIONS(
                std::begin(preceding_edge_costs),
                std::end(preceding_edge_costs),
                std::begin(expected_preceding_edge_costs),
                std::end(expected_preceding_edge_costs));
        }
    }
    {
        const auto* const p_vocabulary = create_c_empty_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        BOOST_TEST(preceding_edge_cost_count == 1U);

        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        const auto             preceding_edge_cost_count_again =
            tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        BOOST_TEST_REQUIRE(preceding_edge_cost_count_again == 1U);
    }
    {
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(42, 0);
        BOOST_TEST(tetengo_lattice_lattice_settle(nullptr, &eos_node, std::data(preceding_edge_costs)) == 0U);
    }
    {
        const auto* const p_vocabulary = create_c_vocabulary();
        auto* const       p_lattice = tetengo_lattice_lattice_create(p_vocabulary);
        BOOST_SCOPE_EXIT(p_lattice, p_vocabulary)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        tetengo_lattice_node_t eos_node{};
        const auto preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, &eos_node, nullptr);
        BOOST_TEST(preceding_edge_cost_count == 0U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
